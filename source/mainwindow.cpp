/**
 * Represent the view of the sprite editor. Connects all of the slots and signals between the model and the view.
 * Sets the icons for each button and creates a new button for each new frome that is made. Handles the clicking of
 * buttons and sends signals to the model to handle the clicks.
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]
 * March 31, 2025
 *
 * Checked by [redacted], March 31, 2025
 **/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvaslabel.h"
#include <QPainter>
#include <QColorDialog>
#include <QPixmap>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QTimer>


MainWindow::MainWindow(Model* model, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), model(model) {
    ui->setupUi(this);

    // Creates a container for all of the frame buttons.
    contentLayout = new QVBoxLayout(ui->frames);
    contentLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    // Sets the initial value of the slider and displays the text.
    ui->fpsSlider->setValue(animationFPS);
    ui->fpsCountLabel->setText(QString::number(animationFPS) + " FPS");

    // Create the timer and the initial interval.
    animationTimer = new QTimer(this);
    animationTimer->setInterval(1000 / animationFPS);

    // Create and set button icons.
    QIcon brush(":/icons/brush.png");
    QIcon erase(":/icons/eraser.png");
    QIcon fill(":/icons/fill.png");
    QIcon newFrame(":/icons/addNewFrame.png");
    QIcon eyeDropper(":/icons/eyeDropper.png");
    QIcon removeFrame(":/icons/removeCurrentFrame.png");
    QIcon duplicateFrame(":/icons/duplicateFrame.png");
    ui->drawButton->setIcon(brush);
    ui->eraseButton->setIcon(erase);
    ui->fillButton->setIcon(fill);
    ui->addNewFrame->setIcon(newFrame);
    ui->eyeDropperButton->setIcon(eyeDropper);
    ui->removeFrame->setIcon(removeFrame);
    ui->duplicateFrame->setIcon(duplicateFrame);

    // Disable some buttons initially
    ui->addNewFrame->setEnabled(false);
    ui->removeFrame->setEnabled(false);
    ui->duplicateFrame->setEnabled(false);

    // Dialog/Setup connections
    connect(&newFile, &NewFile::sendSize, this, &MainWindow::setupNewView);
    connect(this, &MainWindow::setupModel, model, &Model::setupSprite);
    connect(model, &Model::loadedProject, this, &MainWindow::setupLoadView);

    // Canvas connections
    connect(ui->canvas, &CanvasLabel::draw, this, &MainWindow::canvasInput);    // Get canvas-relative input from the canvas
    connect(this, &MainWindow::sendPixelInput, model, &Model::editImage);       // Send sprite-relative input to model
    connect(model, &Model::canvasDraw, this, &MainWindow::canvasDraw);          // Recieve the sprite image to draw
    connect(model, &Model::animated, this, &MainWindow::animationDraw);

    // Button connections
    connect(ui->drawButton, &QPushButton::clicked, this, &MainWindow::brushButtonClicked);
    connect(ui->eraseButton, &QPushButton::clicked, this, &MainWindow::eraseButtonClicked);
    connect(ui->fillButton, &QPushButton::clicked, this, &MainWindow::fillButtonClicked);
    connect(ui->eyeDropperButton, &QPushButton::clicked, this, &MainWindow::eyeDropperButtonClicked);
    connect(ui->addNewFrame, &QPushButton::clicked, this, &MainWindow::newFrameClicked);
    connect(ui->fpsSlider, &QSlider::valueChanged, this, &MainWindow::sliderValueChanged);
    connect(ui->colorPicker, &QPushButton::clicked, this, &MainWindow::colorPickerClicked);
    connect(ui->removeFrame, &QPushButton::clicked, this, &MainWindow::removeFrame);
    connect(ui->duplicateFrame, &QPushButton::clicked, this, &MainWindow::duplicateFrameClicked);

    // Menu Action Connections
    connect(ui->newAction, &QAction::triggered, this, &MainWindow::newFileOpened);
    connect(ui->saveAction, &QAction::triggered, this, &MainWindow::saveButtonClicked);
    connect(ui->loadAction, &QAction::triggered, this, &MainWindow::loadButtonClicked);
    connect(this, &MainWindow::saveFile, model, &Model::Serialize);
    connect(this, &MainWindow::loadFile, model, &Model::Deserialize);

    // Button Action connections
    connect(this, &MainWindow::toolChanged, model, &Model::changeTool);
    connect(this, &MainWindow::colorChanged, model, &Model::changeColor);
    connect(this, &MainWindow::newFrameAdded, model, &Model::addSpriteFrame);
    connect(this, &MainWindow::frameRemoved, model, &Model::deleteSpriteFrame);
    connect(model, &Model::updateColor, this, &MainWindow::updatedColor);
    connect(this, &MainWindow::changeFrame, model, &Model::setSpriteFrame);
    connect(this, &MainWindow::duplicateFrame, model, &Model::duplicateSpriteFrame);

    // Animation connections
    connect(animationTimer, &QTimer::timeout, model, &Model::animateNextFrame);


    //Setting initial color to black
    currentColor = QColor(Qt::black);
    QString styleSheet = QString("background-color: rgba(%1, %2, %3, %4);")
        .arg(currentColor.red())
        .arg(currentColor.green())
        .arg(currentColor.blue())
        .arg(currentColor.alpha());
    ui->colorPicker->setStyleSheet(styleSheet);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete animationTimer;
}

void MainWindow::setupNewView(int spriteSize){
    setupView(spriteSize);

    // Send sprite size info to Model for sprite setup
    emit setupModel(spriteSize);

    //Setting up the scroll view and adding the first button.
    QPushButton *button = new QPushButton("Frame 1", nullptr);
    framesVector.append(button);
    button->setProperty("buttonID", framesVector.size()); //sets the id of the button
    contentLayout->addWidget(button); // adds the button to a group
    ui->scrollArea->setWidget(ui->frames); // put the buttons into the scroll view.

    // Connect the new button's click signal to a lambda that identifies which button is clicked
    connect(button, &QPushButton::clicked, [this, button]() {
        currentFrame = button->property("buttonID").toInt();
        this->ui->mainFrameNum->setText("Frame " + QString::number(currentFrame));

        // sends a signal to the model to tell it which frame to show.
        emit changeFrame(currentFrame);
    });

    animationTimer->start();
}

void MainWindow::setupLoadView(int spriteSize, int frameCount){
    setupView(spriteSize);

    //Create buttons for all of the loaded frames.
    for (int i = 0; i < frameCount; ++i) {
        QPushButton *button = new QPushButton("Frame " + QString::number(i + 1), nullptr);
        framesVector.append(button);
        button->setProperty("buttonID", framesVector.size()); //sets the id of the button
        contentLayout->addWidget(button); // adds the button to a group
        ui->scrollArea->setWidget(ui->frames); // put the buttons into the scroll view.

        // Connect the new button's click signal to a lambda that identifies which button is clicked
        connect(button, &QPushButton::clicked, [this, button]() {
            currentFrame = button->property("buttonID").toInt();
            this->ui->mainFrameNum->setText("Frame " + QString::number(currentFrame));

            // sends a signal to the model to tell it which frame to show.
            emit changeFrame(currentFrame);
        });
    }

    animationTimer->start();
}

void MainWindow::setupView(int spriteSize){

    animationTimer->stop();
    currentFrame = 1;

    this->spriteSize = spriteSize;

    // Delete any old data
    while (QLayoutItem* item = contentLayout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
    framesVector.clear();

    // Setup canvas checker background
    int checkerSize = 1;
    QColor checkerColor1(200,200,200);
    QColor checkerColor2(150,150,150);

    // Create 2x2 checker pixmap pattern
    QPixmap checkerMap(checkerSize*2, checkerSize*2);
    QPainter painter(&checkerMap);
    painter.fillRect(0, 0, checkerSize, checkerSize, checkerColor1);
    painter.fillRect(checkerSize, 0, checkerSize, checkerSize, checkerColor2);
    painter.fillRect(0, checkerSize, checkerSize, checkerSize, checkerColor2);
    painter.fillRect(checkerSize, checkerSize, checkerSize, checkerSize, checkerColor1);
    painter.end();

    // Generate full checkerboard pattern
    QPixmap canvasCheckerboard(spriteSize, spriteSize);
    QPainter tilePainter(&canvasCheckerboard);
    for (int x = 0; x < canvasCheckerboard.width(); x += checkerSize * 2) {
        for (int y = 0; y < canvasCheckerboard.height(); y += checkerSize * 2) {
            tilePainter.drawPixmap(x, y, checkerMap);
        }
    }
    tilePainter.end();

    // Apply checkerboard to canvas_background
    ui->canvas_background->setPixmap(canvasCheckerboard.scaled(ui->canvas_background->width(), ui->canvas_background->height(), Qt::IgnoreAspectRatio, Qt::FastTransformation));

    // Enable any buttons which need enabling
    ui->addNewFrame->setEnabled(true);
    ui->duplicateFrame->setEnabled(true);
    ui->removeFrame->setEnabled(false);
}

void MainWindow::canvasInput(QPoint mousePos){

    //Check to make sure a sprite is set up
    if(spriteSize <= 0)
        return;

    int pixelX = mousePos.x() * spriteSize / ui->canvas->width();
    int pixelY = mousePos.y() * spriteSize / ui->canvas->height();

    // Sending relative pixel position.
    emit sendPixelInput(QPoint(pixelX, pixelY));
}

void MainWindow::colorPickerClicked(){
    QColor color = QColorDialog::getColor(currentColor, this, "Select Color", QColorDialog::ShowAlphaChannel); // Color picker popup.

    if(color.isValid()) {
        QString styleSheet = QString("background-color: rgba(%1, %2, %3, %4);")
            .arg(color.red())
            .arg(color.green())
            .arg(color.blue())
            .arg(color.alpha());
        ui->colorPicker->setStyleSheet(styleSheet); //setting selected color to colorPicker button.

        currentColor = color;
        emit colorChanged(currentColor);
    }
}

void MainWindow::canvasDraw(QImage spriteImage){

    QPixmap canvasMap = QPixmap::fromImage(spriteImage).scaled(ui->canvas->width(), ui->canvas->height(), Qt::IgnoreAspectRatio, Qt::FastTransformation);

    // Set the Canvas Label's Pixmap
    ui->canvas->setPixmap(canvasMap);
}

void MainWindow::animationDraw(QImage spriteImage){

    //Create 2 seperate sized animations (scaled to fit and real size)
    QPixmap spriteMap = QPixmap::fromImage(spriteImage).scaled(ui->animationView->width(), ui->animationView->height(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    QPixmap spriteMapRealSize = QPixmap::fromImage(spriteImage);

    // Set the animation view's Pixmap
    ui->animationView->setPixmap(spriteMap);
    ui->trueSizeAnimation->setPixmap(spriteMapRealSize);
}

void MainWindow::eraseButtonClicked(){
    ui->eraseButton -> setFocus();
    emit toolChanged(Tool::ERASER);
}

void MainWindow::fillButtonClicked(){
    ui->fillButton -> setFocus();
    emit toolChanged(Tool::FILL);
}

void MainWindow::brushButtonClicked(){
    ui->drawButton -> setFocus();
    emit toolChanged(Tool::PEN);
}

void MainWindow::eyeDropperButtonClicked(){
    ui->eyeDropperButton -> setFocus();
    emit toolChanged(Tool::EYEDROPPER);
}

void MainWindow::newFrameClicked(){

    QPushButton *button = new QPushButton("Frame " + QString::number(framesVector.size() + 1), nullptr);
    framesVector.append(button);
    contentLayout->addWidget(button);
    button->setProperty("buttonID", framesVector.size()); //sets the id of the button

    // Connect the new button's click signal to a lambda that identifies which button is clicked
    connect(button, &QPushButton::clicked, [this, button]() {
        currentFrame = button->property("buttonID").toInt();
        this->ui->mainFrameNum->setText("Frame " + QString::number(currentFrame));

        // sends a signal to the model to tell it which frame to show.
        emit changeFrame(currentFrame);
    });

    // If more than one frames, allow user to remove frames.
    if(framesVector.size() > 1)
        ui->removeFrame->setEnabled(true);

    emit newFrameAdded();
}

void MainWindow::duplicateFrameClicked()
{
    int zeroBased=currentFrame-1;
    emit duplicateFrame(zeroBased);
    if (zeroBased < 0 || zeroBased >= framesVector.size()) {
        return;  // do nothing if user tries to duplicate a nonexistent frame
    }

    QPushButton *button = new QPushButton("Frame " + QString::number(framesVector.size()+1), nullptr);
    framesVector.append(button);
    contentLayout->addWidget(button);
    button->setProperty("buttonID", framesVector.size());

    connect(button, &QPushButton::clicked, [this, button]() {
        currentFrame = button->property("buttonID").toInt();
        ui->mainFrameNum->setText("Frame " + QString::number(currentFrame));
        emit changeFrame(currentFrame); // tell Model to switch to that frame
    });

    // Optionally re-enable removeFrame if needed
    if (framesVector.size() > 1)
        ui->removeFrame->setEnabled(true);
}


void MainWindow::newFileOpened(){
    newFile.setModal(true);
    newFile.show();
}

void MainWindow::updatedColor(QColor color){
    QString styleSheet = QString("background-color: rgba(%1, %2, %3, %4);")
                             .arg(color.red())
                             .arg(color.green())
                             .arg(color.blue())
                             .arg(color.alpha());
    ui->colorPicker->setStyleSheet(styleSheet);
}

void MainWindow::sliderValueChanged(){
    //send signal stating the current fps value.
    ui->fpsCountLabel->setText(QString::number(ui->fpsSlider->value()) + " fps");
    animationFPS = ui->fpsSlider->value();

    if(animationTimer->isActive()){
        //Change the animation speed to match the updated fps.
        animationTimer->stop();
        animationTimer->start(1000 / animationFPS);
    }
}

void MainWindow::removeFrame(){

    int zeroBased = currentFrame - 1;
    if (zeroBased < 0 || zeroBased >= framesVector.size()) {
        return; // nothing to remove
    }

    // Remove button from layout.
    QPushButton *buttonToRemove = framesVector.takeAt(zeroBased);
    contentLayout->removeWidget(buttonToRemove);

    animationTimer->stop();
    currentFrame = 1;

    emit frameRemoved(buttonToRemove->property("buttonID").toInt() - 1);
    delete buttonToRemove;

    // Renumber all the remaining buttons
    for (int i = 0; i < framesVector.size(); ++i) {
        QPushButton *button = framesVector[i];
        button->setText("Frame " + QString::number(i + 1)); // Renumbering starts from 1
        button->setProperty("buttonID", i + 1);
    }

    // Don't allow a user to remove a frame when there is only one frame.
    if(framesVector.size() <= 1)
        ui->removeFrame->setEnabled(false);

    animationTimer->start();
}

void MainWindow::saveButtonClicked()
{
    QUrl fileUrl = QFileDialog::getSaveFileUrl(this, "Save Project", QUrl(), "*.ssp");
    // If they canceled saving, exit
    if (fileUrl.isEmpty()) return;

    QString filePath = fileUrl.toLocalFile();

    emit saveFile(filePath);
}

void MainWindow::loadButtonClicked()
{
    QUrl fileUrl = QFileDialog::getOpenFileUrl(this, "Load Project", QUrl(), "*.ssp");
    // If they canceled saving, exit
    if (fileUrl.isEmpty()) return;

    QString filePath = fileUrl.toLocalFile();

    emit loadFile(filePath);
}

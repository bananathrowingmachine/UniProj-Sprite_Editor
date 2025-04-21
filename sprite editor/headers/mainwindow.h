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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QImage>
#include "model.h"
#include "newfile.h"
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QColor currentColor;
    Model *model = nullptr;
    NewFile newFile;  //The dialog window.
    QVBoxLayout *contentLayout; // The layout that holds all of the frame buttons.
    QVector<QPushButton*> framesVector; // List of all the frame buttons.
    int currentFrame = 1;
    int spriteSize = 0;

    // Animation variables
    QTimer* animationTimer;
    int animationFPS = 5;

    /**
     * Helper method that sets up the gui once a new file has been opened or loaded.
     * @param spriteSize - the size inputed from the user.
     */
    void setupView(int spriteSize);

public slots:

    /**
     * Sets up the inital properties of the view.
     * @param spriteSize
     */
    void setupNewView(int spriteSize); // TODO: RENAME TO setupNewView

    /**
     * Sets up the inital properties of the view.
     * @param spriteSize
     */
    void setupLoadView(int spriteSize, int frameCount);

    /**
     * Converts canvas-relative input to sprite-relative input
     * @param mousePos - the mouse position input
     */
    void canvasInput(QPoint mousePos);

    /**
     * Opens the color pallet and allow a user to change the color. Will also change the color of the
     * button to match the color selected.
     */
    void colorPickerClicked();

    /**
     * Updates the main canvas to the image passed in.
     * @param spriteImage - the image to be drawn.
     */
    void canvasDraw(QImage spriteImage);

    /**
     * Changes the image in the animation view and the real size animation view.
     * @param spriteImage - the image to be drawn.
     */
    void animationDraw(QImage spriteImage);

    /**
     * Tells the model that the tool has been changed to the brush tool and focuses the brush button.
     */
    void brushButtonClicked();

    /**
     * Tells the model that the tool has been changed to the eraser tool and focuses the eraser button.
     */
    void eraseButtonClicked();

    /**
     * Tells the model that the tool has been changed to the fill tool and focuses the fill button.
     */
    void fillButtonClicked();

    /**
     * Tells the model that the tool has been changed to the eyeDropper and focuses
     * the eyeDropper button.
     */
    void eyeDropperButtonClicked();

    /**
     * Once the new frame button is clicked, it will send a signal here to create a new frame. It
     * will also add a new frame button to the gui.
     */
    void newFrameClicked();

    /**
     * Once the duplicate frame button clicked, it will send a signal to duplicte current frame.
     * and also add a new frame button to the list.
     */
    void duplicateFrameClicked();

    /**
     * Once the menu item, createNewFile, has been clicked it will send a signal here to open the
     * newFile dialog box.
     */
    void newFileOpened();

    /**
     * Once the color picker has recognized a color, it will send a signal here to change the color
     * of the colorPicker box.
     * @param color
     */
    void updatedColor(QColor color);

    /**
     * Whenever the slider's value changes it will update the slider value label and then also send a signal
     * with the fps.
     */
    void sliderValueChanged();

    /**
     * Remove a frame from the project.
     */
    void removeFrame();

    /**
     * Once clicked, it will prompt the user to pick a location for the file to be saved. If approved it will
     * get the file path and save the project in that location. If rejected nothing will happen.
     */
    void saveButtonClicked();

    /**
     * Once clicked, it will prompt the user to pick a file to be opened. If approved it will send the file information
     * to the model. If rejected nothing will happen.
     */
    void loadButtonClicked();


signals:

    /**
     * Emmited once one of the editor tools are clicked on.
     */
    void toolChanged(Tool);

    /**
     * Emitted once the color picked has selected a color.
     * @param currentColor - color selected from the color picker.
     */
    void colorChanged(QColor currentColor);

    /**
     * Emitted once the new frame button is clicked.
     */
    void newFrameAdded();

    /**
     * Emitted once the remove frame button is clicked.
     * @param frameIndex - the frame to remove.
     */
    void frameRemoved(int frameIndex);

    /**
     * Emitted when mouse is pressed down in the canvas.
     * @param pixelPos - the relative pixel position to the canvas.
     */
    void sendPixelInput(QPoint pixelPos);

    /**
     * Emitted once a new project is made.
     * @param spriteSize - size inputed from the user.
     */
    void setupModel(int spriteSize);

    /**
     * Emitted once a frame button is clicked.
     * @param frameID - the frame that was clicked.
     */
    void changeFrame(int frameID);

    /**
     * Emitted once a file path is selected.
     * @param path - the path of the file.
     */
    void saveFile(QString path); // std::filesystem::path path

    /**
     * Emitted once a file is selected.
     * @param path
     */
    void loadFile(QString path); //std::filesystem::path path

    /**
     * Emitted when the slider changes position.
     * @param fps - frames per second determined by slider.
     */
    void sendFPS(int fps);

    /**
     * @brief Emitted once duplicate button clicked
     * @param frameIndex - Index of frame whne being clicked.
     */
    void duplicateFrame(int frameIndex);

public:
    MainWindow(Model* model, QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H

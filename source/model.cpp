/**
 * Represents the main logic of the Sprite Editor. Connects elements of the model
 * together in one place. Handles drawing mechanics, animation, as well as
 * serializing and deserializing projects.
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]
 * March 31, 2025
 *
 * Checked by [redacted], March 31, 2025
 **/

#include "model.h"
#include <QQueue>
#include <QSet>
#include <QFile>

Model::Model(QObject *parent) : QObject{parent} {
}

Model::~Model(){
    delete sprite;
}

void Model::editImage(QPoint pos){
    switch(currentTool){
    case Tool::PEN:
        sprite->setPixel(pos, currentColor);
        break;
    case Tool::ERASER:
        sprite->setPixel(pos, QColor(0,0,0,0));
        break;
    case Tool::FILL:
        Model::fillImage(pos);
        break;
    case Tool::EYEDROPPER:
        currentColor = sprite->getColor(pos);
        emit updateColor(currentColor);
    default:
        return;
    }
    emit canvasDraw(sprite->getFrame());
}

void Model::changeTool(Tool tool){
    currentTool = tool;
}

void Model::changeColor(QColor color){
    currentColor = color;
}

void Model::setupSprite(int size){
    sprite = new Sprite(size);
    currentAnimationFrameIndex = 0;
    emit canvasDraw(sprite->getFrame());
}

void Model::addSpriteFrame(){
    sprite->addFrame();
}

void Model::deleteSpriteFrame(int frameIndex){
    if(sprite == nullptr)
        return;

    currentAnimationFrameIndex = 0;
    sprite->deleteFrame(frameIndex);
    sprite->getFrame(0, true);
    emit canvasDraw(sprite->getFrame());
}

void Model::duplicateSpriteFrame(int frameIndex)
{
    sprite->duplicateFrame(frameIndex);
}

void Model::setSpriteFrame(int frameID){
    if(sprite == nullptr)
        return;

    sprite->getFrame(frameID - 1, true);
    emit canvasDraw(sprite->getFrame());
}

void Model::animateNextFrame(){
    emit animated(sprite->getFrame(currentAnimationFrameIndex, false));
    // Increment frameIndex (loop it)
    currentAnimationFrameIndex = (currentAnimationFrameIndex + 1) % sprite->getFrameCount();
}

void Model::fillImage(QPoint pos){
    QColor formerColor = sprite->getColor(pos);
    QQueue<QPoint> q;
    QSet<QPoint> visited;
    q.enqueue(pos);
    sprite->setPixel(pos, currentColor);
    while (!q.empty()) {
        QPoint currentPoint = q.dequeue();
        if (!visited.contains(currentPoint)) {
            visited.insert(currentPoint);
            QPoint newPoints[4] = {QPoint(currentPoint.x() + 1, currentPoint.y()), QPoint(currentPoint.x() - 1, currentPoint.y()), QPoint(currentPoint.x(), currentPoint.y() + 1), QPoint(currentPoint.x() + 1, currentPoint.y() - 1)};
            for (QPoint point : newPoints){
                if (!(point.x() < 0 || point.x() >= sprite->getWidth() || point.y() < 0 || point.y() >= sprite->getWidth()) && formerColor == sprite->getColor(point)) {
                    q.enqueue(point);
                    sprite->setPixel(point, currentColor);
                }
            }
        }
    }
}

void Model::Serialize(QString path){
    QFile json(path);
    if (!json.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Failed to open file for writing:" << json.errorString();
        return;
    }
    json.write(sprite->Serialize().toUtf8());
    json.close();
    //TODO: Fix 'device not open' error
}

void Model::Deserialize(QString path){
    QFile json(path);
    json.open(QIODevice::ReadOnly);
    sprite = sprite->Deserialize(json.readAll());
    json.close();
    //TODO: Fix 'device not open' error

    currentAnimationFrameIndex = 0;
    emit loadedProject(sprite->getWidth(), sprite->getFrameCount());
    emit canvasDraw(sprite->getFrame());
}



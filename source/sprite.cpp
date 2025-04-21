/**
 * File representing a Sprite object.
 * A Sprite can contain multiple frames as QImage objects.
 * You can add and remove frames, as well as save/load sprites to/from files
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]
 * March 31, 2025
 *
 * Checked by [redacted], March 31, 2025
 **/

#include "sprite.h"

Sprite::Sprite(int width) : width{width} {
    addFrame();
    currentFrameIndex = 0;
}

Sprite::~Sprite(){}

void Sprite::setPixel(QPoint pos, QColor color){
    QImage& currentFrame = frames.at(currentFrameIndex);
    currentFrame.setPixelColor(pos, color);
}

QColor Sprite::getColor(QPoint pos){
    return frames[currentFrameIndex].pixelColor(pos);
}

void Sprite::addFrame(){
    QImage image(width, width, QImage::Format_ARGB32);
    image.fill(QColor(0,0,0,0));
    frames.push_back(image);
}

QImage& Sprite::getFrame(){
    return frames[currentFrameIndex];
}

QImage& Sprite::getFrame(int frame, bool setCurrent = false){
    try {
        QImage& result = frames.at(frame);
        if (setCurrent)
            currentFrameIndex = frame;
        return result;
    } catch(const std::out_of_range& e) {
        throw;
    }
}

void Sprite::deleteFrame(){
    frames.erase(frames.begin() + currentFrameIndex);
    if (currentFrameIndex != 0)
        currentFrameIndex--;
}

void Sprite::deleteFrame(int frame){
    try {
        frames.at(frame);
        frames.erase(frames.begin() + frame);
    } catch(const std::out_of_range& e) {
        throw;
    }
}

void Sprite::duplicateFrame(int frameIndex)
{

    QImage copy = frames[frameIndex].copy();

    frames.insert(frames.begin() + frameIndex + 1, copy);
}

int Sprite::getWidth() {
    return width;
}

int Sprite::getFrameCount(){
    return frames.size();
}

QString Sprite::Serialize() {
    QJsonArray framesArray;
    for (QImage& image : frames) {
        QJsonArray jsonColors;
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < width; j++) {
                QColor colorToSerialize = image.pixelColor(QPoint(i,j));
                QJsonObject colorInfo;

                colorInfo["red"] = colorToSerialize.red();
                colorInfo["green"] = colorToSerialize.green();
                colorInfo["blue"] = colorToSerialize.blue();
                colorInfo["alpha"] = colorToSerialize.alpha();

                jsonColors.append(colorInfo);
            }
        }
        framesArray.append(jsonColors);
    }

    QJsonDocument doc(framesArray);
    return doc.toJson(QJsonDocument::Indented);
}

Sprite* Sprite::Deserialize(const QByteArray& jsonData){
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isArray()) {
        return nullptr;
    }
    QJsonArray framesArray = doc.array();
    int jsonWidth = sqrt(int(framesArray[0].toArray().count()));
    Sprite* newSprite = new Sprite(jsonWidth);
    newSprite->frames = {};

    for (int x = 0; x < framesArray.size(); x++) {
        QJsonValue frameVal = framesArray[x];
        QJsonArray frame = frameVal.toArray();
        newSprite->addFrame();
        newSprite->currentFrameIndex = x;

        for (int i = 0; i < frame.count(); i++) {
            QJsonObject colorObj = frame[i].toObject();
            QColor color(colorObj["red"].toInt(), colorObj["green"].toInt(), colorObj["blue"].toInt(), colorObj["alpha"].toInt());
            newSprite->setPixel(QPoint(i/jsonWidth, i%jsonWidth), color);
        }
    }

    newSprite->currentFrameIndex = 0;

    return newSprite;
}


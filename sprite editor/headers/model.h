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

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QColor>
#include <QPoint>
#include <filesystem>
#include "sprite.h"

enum class Tool {PEN, ERASER, FILL, EYEDROPPER};

class Model : public QObject
{
    Q_OBJECT

private:
    Sprite *sprite = nullptr;
    Tool currentTool = Tool::PEN;
    QColor currentColor = QColor(Qt::black);
    int currentAnimationFrameIndex = 0;

    /**
     * Replaces all recursively adjacent pixels of the clicked on pixels color to the currentColor
     * @param x - starting x pos
     * @param y - starting y pos
     */
    void fillImage(QPoint pos);

public:
    /**
     * Constructs a model object.
     * @param parent
     */
    Model(QObject *parent = nullptr);
    ~Model();

signals:
    /**
     * Emitted when the sprite's frame's have been edited.
     * @param frame - the sprite's frame to be drawn
     */
    void canvasDraw(QImage& frame);

    /**
     * Emitted when the current color changes.
     */
    void updateColor(QColor);

    /**
     * Emitted when the sprite animates to the next frame.
     * @param frame - the frame in the animation which the sprite is now on
     */
    void animated(QImage& frame);

    /**
     * Emitted after a project is deserialized.
     * @param spriteSize - the sprite's size (note that sprites are square)
     * @param frameCount - the number of frames this sprite has
     */
    void loadedProject(int spriteSize, int frameCount);

public slots:
    /**
     * Will edit the current frame selected by the user.
     * @param pos - the position to draw, relative to the image
     */
    void editImage(QPoint pos);

    /**
     * Will change the current tool selected by the user, which modifies what happens when a image pixel is clicked.
     * @param tool - the new tool
     */
    void changeTool(Tool tool);

    /**
     * Will change the current color, which is the color any clicked image pixel will be turned into.
     * @param color - the new color
     */
    void changeColor(QColor color);

    /**
     * Sets up this project's sprite with the given size.
     * @param size - the width/height of the sprite's frame
     */
    void setupSprite(int size);

    /**
     * Makes a new frame in the sprite.
     */
    void addSpriteFrame();

    /**
     * Deletes the currently selected frame from the sprite.
     * @param frameIndex - the index of the frame to delete
     */
    void deleteSpriteFrame(int frameIndex);

    /**
     * Sets the current frame in sprite.
     * @param frameID - the index of the frame to set as current
     */
    void setSpriteFrame(int frameID);

    /**
     * duplicate the frame of selected index.
     * @param frameIndex - the index of the frame to dupe.
     */
    void duplicateSpriteFrame(int frameIndex);
    /**
     * Switches the currentAnimationFrameIndex to the next frame
     * and emits animated.
     */
    void animateNextFrame();

    /**
     * Serializes the project into JSON format.
     * @param path - the path to serialize to
     */
    void Serialize(QString path); // std::filesystem::path path

    /**
     * Deserializes a .ssp file into the project.
     * @param path - the path of the .ssp file to deserialize
     */
    void Deserialize(QString path); // td::filesystem::path path
};

#endif // MODEL_H

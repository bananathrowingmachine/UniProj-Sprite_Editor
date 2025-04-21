#ifndef SPRITE_H
#define SPRITE_H

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

#include <vector>
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "frame.h"
using std::vector;

/**
 * Represents a Sprite object; a small pixelated image or animation.
 */
class Sprite{
private:
    int width;
    vector<QImage> frames;
    int currentFrameIndex = 0;

public:

    /**
     * Constructs a Sprite object
     * @param width - the width of this sprite in pixels
     */
    Sprite(int width);

    /**
     * Destructor for a sprite.
     */
    ~Sprite();

    /**
     * Sets the color of a pixel at the given x y position in current frame.
     * @param pos - The point
     * @param color - the color to set the pixel to
     */
    void setPixel(QPoint pos, QColor color);

    /**
     * Retrieves the color of the pixel at the given position on this Sprite's
     * current frame.
     * @param pos - The point
     * @return QColor pixel color
     */
    QColor getColor(QPoint pos);

    /**
     * Adds a new blank white frame to this sprite.
     */
    void addFrame();

    /**
     * Adds a new frame to this sprite given the image data holding a 2D
     * array of QColors. The imageData's dimensions must match the sprite's
     * width and height.
     * @param imageData
     */
    void addFrame(vector<vector<QColor>> imageData);

    /**
     * Gets the current frame according to the currentFrameIndex
     * @return QImage reference to the current frame
     */
    QImage& getFrame();

    /**
     * Gets the chosen frame according to the input
     * @param frame - the frame to return
     * @param setCurrent - if the frame selected should become the current frame (default = false)
     * @return QImage reference to the chosen frameva
     */
    QImage& getFrame(int frame, bool setCurrent);

    /**
     * Deletes the current frame according to the currentFrameIndex
     */
    void deleteFrame();

    /**
     * Deletes the chosen frame according to the input
     * @param frame - the frame to delete
     */
    void deleteFrame(int frame);

    /**
     * @brief Duplicate the frame of certain index.
     * @param frameIndex - the frame to duplicate
     */
    void duplicateFrame(int frameIndex);

    /**
     * Returns the int pixel width (and also height of the sprite.
     * @return int pixel length
     */
    int getWidth();

    /**
     * Returns the amount of frames contained in the sprite
     * @return int frame count
     */
    int getFrameCount();

    /**
     * Serializes the sprite into JSON
     * @return QString JSON representation of the sprite
     */
    QString Serialize();

    /**
     * Deserializes the given data into a new sprite object
     * @param jsonData - A QByteArray containing the JSON data of the sprite
     * @return Sprite* deserialized sprite
     */
    Sprite* Deserialize(const QByteArray& jsonData);
};

#endif // SPRITE_H

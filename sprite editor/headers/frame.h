#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include <QColor>
using std::vector;

class Frame{
private:
    int width, height;
    vector<vector<QColor>> imageData;
public:
    Frame(int width, int height);
    Frame(vector<vector<QColor>>& imageData);
    ~Frame();
    /**
     * Sets the color of a pixel at the given x y position in the frame.
     * @param x - the x position of the pixel
     * @param y - the y position of the pixel
     * @param color - the color to set the pixel to
     */
    void setPixel(int x, int y, QColor color);
};

#endif // FRAME_H

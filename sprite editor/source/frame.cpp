#include "frame.h"

Frame::Frame(int width, int height) : width{width}, height{height} {
    // Initialize the image data to the correct width and height with blank white pixels
    imageData = vector<vector<QColor>>(height, vector<QColor>(width, QColor(255,255,255,255)));
}

Frame::Frame(vector<vector<QColor>>& imageData) {

    this->height = imageData.size();
    this->width = (height > 0) ? imageData[0].size() : 0;
    this->imageData = imageData;
}

Frame::~Frame(){

}

void Frame::setPixel(int x, int y, QColor color){
    imageData[y][x] = color;
}

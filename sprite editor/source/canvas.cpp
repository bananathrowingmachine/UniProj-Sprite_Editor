#include "canvas.h"

Canvas::Canvas() {}

void Canvas::mousePressEvent(QMouseEvent *event){

}

void Canvas::mouseMoveEvent(QMouseEvent *event){

    if(isDrawing){
        // Get mouse position
        QPoint mouseLoc = event->pos();
        // Convert screen space to Canvas space
        // Call sprite's draw method
        sprite.draw(mouseLoc.x(), mouseLoc.y());
        // Update Canvas
    }
}

/**
 * Represent the Canvas area of sprite editor. It is an interactive drawing surface where mouse actions can
 * be recorded to trigger drawing actions.
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]
 * March 31, 2025
 *
 * Checked by [redacted], March 31, 2025
 **/

#include "canvaslabel.h"

CanvasLabel::CanvasLabel(QWidget *parent) : QLabel(parent) {
    setMouseTracking(true);
}

void CanvasLabel::mouseMoveEvent(QMouseEvent *event){
    QPoint mousePos = event->pos();
    if(isDrawing)
        emit draw(mousePos);
}

void CanvasLabel::mousePressEvent(QMouseEvent *event){
    QPoint mousePos = event->pos();
    emit draw(mousePos);
    isDrawing = true;
}

void CanvasLabel::mouseReleaseEvent(QMouseEvent *event){
    isDrawing = false;
}

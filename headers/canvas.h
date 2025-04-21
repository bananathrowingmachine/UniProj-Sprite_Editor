#ifndef CANVAS_H
#define CANVAS_H

#include <QPoint>
#include<QMouseEvent>
#include "sprite.h"

class Canvas : public QGraphicsView{
private:
    bool isDrawing;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event) override;

public:
    Canvas(QWidget* parent);
    ~Canvas();
    Sprite sprite;

signals:
    void updateSignal();
};

#endif // CANVAS_H

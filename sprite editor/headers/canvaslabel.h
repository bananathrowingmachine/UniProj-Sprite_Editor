/**
 * Represent the Canvas area of sprite editor. It is an interactive drawing surface where mouse movements can
 * be recorded to trigger drawing actions.
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]
 * March 31, 2025
 *
 * Checked by [redacted], March 31, 2025
 **/

#ifndef CANVASLABEL_H
#define CANVASLABEL_H

#include <QLabel>
#include <QPoint>
#include <QMouseEvent>

class CanvasLabel : public QLabel
{
    Q_OBJECT

signals:
    void draw(QPoint pos);

public:
    CanvasLabel(QWidget *parent = nullptr);

private:
    bool isDrawing = false;
    /**
     * Handle mouse move event, mousePos contains the coordinates relative to this CanvasLabel.
     * @param event - the mouse event
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * Handle mouse press event, mousePos contains the coordinates relative to this CanvasLabel.
     * @param event - the mouse event
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * Handle mouse release event, mousePos contains the coordinates relative to this CanvasLabel
     * @param event - the mouse event
     */
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // CANVASLABEL_H

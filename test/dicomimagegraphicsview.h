#ifndef DICOMIMAGEGRAPHICSVIEW_H
#define DICOMIMAGEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QMouseEvent>
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>


class DicomImageGraphicsItem;


class DicomImageGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit DicomImageGraphicsView(QWidget *parent = 0);

    void SetPixmap(QPixmap pixmap);

signals:
    void DicomWindowDelta(QPointF);

protected:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;
  
private:
    QGraphicsScene * _scene = nullptr;
    DicomImageGraphicsItem *_image_item = nullptr;

};

#endif // DICOMIMAGEGRAPHICSVIEW_H

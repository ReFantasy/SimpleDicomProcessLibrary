#ifndef DICOMIMAGEGRAPHICSITEM_H
#define DICOMIMAGEGRAPHICSITEM_H

#include <QGraphicsPixmapItem>

class DicomImageGraphicsItem : public QGraphicsPixmapItem
{
public:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
};

#endif // DICOMIMAGEGRAPHICSITEM_H

#ifndef DICOMIMAGEGRAPHICSITEM_H
#define DICOMIMAGEGRAPHICSITEM_H

#include <QGraphicsPixmapItem>

class DicomImageGraphicsItem : public QGraphicsPixmapItem
{
public:
    //DicomImageGraphicsItem(QGraphicsItem* parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)override;


};

#endif // DICOMIMAGEGRAPHICSITEM_H

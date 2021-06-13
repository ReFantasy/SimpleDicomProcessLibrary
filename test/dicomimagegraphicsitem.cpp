#include "dicomimagegraphicsitem.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>



void DicomImageGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"image pos: "<<event->pos();
    //qDebug()<<this->pixmap().size();
    
}

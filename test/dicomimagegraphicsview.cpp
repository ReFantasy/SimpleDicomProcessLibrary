#include "dicomimagegraphicsview.h"
#include "dicomimagegraphicsitem.h"
#include <QMouseEvent>
#include <QGraphicsView>
#include "easylogging++.h"
#include <QDebug>

QPointF _win_start;

DicomImageGraphicsView::DicomImageGraphicsView(QWidget *parent)
    :QGraphicsView(parent)
{
    this->setCursor(Qt::CrossCursor);
    //this->setBackgroundBrush(QColor(0x11,0x2f,0x41));

    //setAlignment(Qt::AlignLeft | Qt::AlignTop);
    _image_item = new DicomImageGraphicsItem;
    _scene = new QGraphicsScene;
    _scene->addItem(_image_item);


    this->setScene(_scene);
    this->setMouseTracking(true);
    _scene->setFocusItem(_image_item);

}

void DicomImageGraphicsView::SetPixmap(QPixmap pixmap)
{

    // input new picture
    _image_item->setPixmap(pixmap);

    _scene->setSceneRect({ 0,0,
                           static_cast<double>(pixmap.width()),
                           static_cast<double>(pixmap.height()) });
	
    
}

void DicomImageGraphicsView::mousePressEvent(QMouseEvent* event)
{
    _win_start = event->pos();

    QGraphicsView::mousePressEvent(event);
}



void DicomImageGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    if ((event->buttons() == Qt::LeftButton))
    {
        QPointF _win_offset = event->pos()- _win_start;
        _win_start = event->pos();
        DicomWindowDelta(_win_offset);
    }
		

    QGraphicsView::mouseMoveEvent(event);
}

void DicomImageGraphicsView::wheelEvent(QWheelEvent *event)
{
    auto _delta = event->delta()/8.0/360.0;
    auto sc = (1+ _delta);

    // view sale
    this->scale(sc, sc);
}




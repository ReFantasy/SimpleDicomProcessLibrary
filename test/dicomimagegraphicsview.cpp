#include "dicomimagegraphicsview.h"
#include "dicomimagegraphicsitem.h"
#include <QMouseEvent>
#include <QGraphicsView>
#include "easylogging++.h"

DicomImageGraphicsView::DicomImageGraphicsView(QWidget *parent)
    :QGraphicsView(parent)
{
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
    _image_item->setPixmap(pixmap);

	/*_scene->setSceneRect({ 0,0,
							static_cast<double>(pixmap.width()),
							static_cast<double>(pixmap.height()) });*/
}

void DicomImageGraphicsView::mousePressEvent(QMouseEvent* event)
{
    
}




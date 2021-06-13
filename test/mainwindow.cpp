﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "easylogging++.h"
#include "colormap.h"
#include <QFileDialog>
#include <QString>
#include <QPixmap>
#include <QToolBar>
#include <QSlider>
#include <QSpacerItem>
#include <QComboBox>
#include <QMimeData>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    LOG(INFO)<<"launch program...";

    ui->setupUi(this);
    ui->menubar->setNativeMenuBar(false);

    LOG(INFO)<<"create dicom series object...";
    _dicom_series = std::make_shared<DicomSeries>();

    ui->graphicsView->setMouseTracking(true);

    connect(ui->graphicsView, &DicomImageGraphicsView::DicomWindowDelta, this, &MainWindow::DicomWindowChanged);

    // status windows label
    auto win_label = new QLabel(this);
    win_label->setObjectName("window_label");
    this->statusBar()->addWidget(win_label);
    
    // add tool bar
    auto toolBar = new QToolBar(this);
    toolBar->setObjectName(QString::fromUtf8("tool_bar"));
    this->addToolBar(Qt::TopToolBarArea, toolBar);
    
    // add slider to control series frames
    hslider_series_frame = new QSlider(this);
    hslider_series_frame->setOrientation(Qt::Orientation::Horizontal);
    connect(hslider_series_frame, &QSlider::valueChanged, this, &MainWindow::ShowNewSeriesFrame);

    // add slider to control dicom frames
    hslider_dicom_frame = new QSlider(this);
    hslider_dicom_frame->setOrientation(Qt::Orientation::Horizontal);
    connect(hslider_dicom_frame, &QSlider::valueChanged, this, &MainWindow::ShowNewDicomFrame);

    // add pseudo colormap
    colormap = new QComboBox(this);
    colormap->addItem("ORIGIN",COLORMAP::COLORMAP_ORIGIN);
    colormap->addItem("RESERVE",COLORMAP::COLORMAP_REVERSE);
    colormap->addItem("RAINBOW",COLORMAP::COLORMAP_RAINBOW);
    colormap->addItem("HOT",COLORMAP::COLORMAP_HOT);
    colormap->addItem("TURBO",COLORMAP::COLORMAP_TURBO);
    colormap->addItem("AUTUMN",COLORMAP::COLORMAP_AUTUMN);
    colormap->addItem("BONE",COLORMAP::COLORMAP_BONE);
    colormap->addItem("COOL",COLORMAP::COLORMAP_COOL);
    colormap->addItem("HSV",COLORMAP::COLORMAP_HSV);
    colormap->addItem("JET",COLORMAP::COLORMAP_JET);
    colormap->addItem("OCEAN",COLORMAP::COLORMAP_OCEAN);
    colormap->addItem("PARULA",COLORMAP::COLORMAP_PARULA);
    colormap->addItem("PINK",COLORMAP::COLORMAP_PINK);
    colormap->addItem("SPRING",COLORMAP::COLORMAP_SPRING);
    colormap->addItem("SUMMER",COLORMAP::COLORMAP_SUMMER);
    colormap->addItem("WINTER",COLORMAP::COLORMAP_WINTER);
    connect(colormap, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeColorMap(int)));



    toolBar->addWidget(new QLabel("Series:"));
    toolBar->addWidget(hslider_series_frame);
    toolBar->addWidget(new QLabel("Frames:"));
    toolBar->addWidget(hslider_dicom_frame);
    toolBar->addWidget(colormap);
    
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
        //qDebug()<<event->mimeData()->urls();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimedata = event->mimeData();
    if(mimedata->hasUrls())
    {
        qDebug()<<"ok";
    }
}

void MainWindow::on_actionOpenFile_N_triggered()
{

    QString file_name  = QFileDialog::getOpenFileName(this);
    LOG(INFO)<<QString("try to open file %1").arg(file_name).toLocal8Bit().data();

    std::shared_ptr<DicomFile> df = std::make_shared<DicomFile>();
    auto res = df->LoadFile(file_name.toStdString());
    if(res)
    {
        // delete existed dicom
        _dicom_series = std::make_shared<DicomSeries>();

        LOG(INFO)<<"open file success!";
        ui->graphicsView->resetTransform();
        _dicom_series->Add(df);

        // set slider
        hslider_series_frame->setEnabled(true);
        hslider_dicom_frame->setEnabled(true);

        hslider_series_frame->setRange(0, 0);
        hslider_series_frame->setValue(0);
        hslider_series_frame->setEnabled(false);

        hslider_dicom_frame->setRange(0, df->GetNumberOfFrames()-1);
        hslider_dicom_frame->setValue(0);
        if (df->GetNumberOfFrames() <= 1)
            hslider_dicom_frame->setEnabled(false);

        DicomWindowChanged(QPointF{ 0,0 });

    }
    else
    {
        LOG(ERROR)<<"open file failed!";
    }

}


void MainWindow::on_actionOpen_DICOM_Folder_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this);
    LOG(INFO)<<QString("try to open folder %1").arg(dir).toLocal8Bit().data();

    auto tmp_series = std::make_shared<DicomSeries>();
    auto res  = tmp_series->ReadDir(dir.toStdString());
    if(res)
    {
        // delete existed dicom
        _dicom_series = tmp_series;

        LOG(INFO)<<"open folder success!";
        ui->graphicsView->resetTransform();


        // set slider
        hslider_series_frame->setEnabled(true);
        hslider_dicom_frame->setEnabled(true);

        hslider_series_frame->setRange(0, _dicom_series->GetTotalFrames()-1);
        hslider_series_frame->setValue(0);
        if (_dicom_series->GetTotalFrames() - 1 < 1)
            hslider_series_frame->setEnabled(false);

        int max_frammes = 0;
        for (int i = 0; i < _dicom_series->GetTotalFrames(); i++)
        {
            int curframes = _dicom_series->GetDicom(i)->GetNumberOfFrames()-1;
            if (max_frammes < curframes)
                max_frammes = curframes;
        }
        hslider_dicom_frame->setRange(0, max_frammes);
        hslider_dicom_frame->setValue(0);
        if ((max_frammes - 1) < 1)
            hslider_dicom_frame->setEnabled(false);

        DicomWindowChanged(QPointF{ 0,0 });

    }
    else
    {
        LOG(ERROR)<<"open folder failed!";
    }
}

void MainWindow::ShowNewSeriesFrame(int frame)
{
    ShowNewFrame(frame, hslider_dicom_frame->value());
}

void MainWindow::ShowNewDicomFrame(int frame)
{
    ShowNewFrame(hslider_series_frame->value(),frame);
}

void MainWindow::DicomWindowChanged(QPointF windelta)
{
    if (_dicom_series == nullptr || _dicom_series->GetTotalFrames() < 1)
        return;

    double c, w;
    _dicom_series->GetWindow(c, w);
    c += windelta.x();
    w += windelta.y();
    w = (w<2?2:w);
    
    _dicom_series->SetWindow(c, w);
    ShowNewFrame(hslider_series_frame->value(), hslider_dicom_frame->value());

    auto win_label = this->statusBar()->findChild<QLabel*>("window_label");
    win_label->setText(QString("\x20\x20WC:%1 WW:%2").arg(c).arg(w));
}


void MainWindow::on_actionSave_triggered()
{
    QString file_name = QFileDialog::getSaveFileName(this, "Save as picture", "", tr("*bmp;*jpg"));
    LOG(INFO) << QString("try to open file %1").arg(file_name).toLocal8Bit().data();


}

void MainWindow::ChangeColorMap(int a)
{
    if (_dicom_series->GetTotalFrames() < 1)
        return;

    auto di = _dicom_series->GetDicom(hslider_series_frame->value());
    int w = di->GetWidth();
    int h = di->GetHeight();
    auto pixelData = di->GetOutputData(hslider_dicom_frame->value());
    QImage image(pixelData, w, h, w, QImage::Format_Indexed8);
    image.setColorTable(ColorMapFactory((COLORMAP)colormap->currentData().toUInt()));

    ui->graphicsView->SetPixmap(QPixmap::fromImage(image));
}

void MainWindow::ShowNewFrame(int series_frame, int dicom_frame)
{
    if (_dicom_series->GetTotalFrames() < 1)
        return;

    auto di = _dicom_series->GetDicom(series_frame);
    int w = di->GetWidth();
    int h = di->GetHeight();
    auto pixelData = di->GetOutputData(dicom_frame);
    QImage image(pixelData, w, h, w, QImage::Format_Indexed8);
    image.setColorTable(ColorMapFactory((COLORMAP)colormap->currentData().toUInt()));

    ui->graphicsView->SetPixmap(QPixmap::fromImage(image));
}

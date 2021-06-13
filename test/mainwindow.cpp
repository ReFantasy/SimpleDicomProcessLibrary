#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "easylogging++.h"
#include <QFileDialog>
#include <QString>
#include <QPixmap>
#include <QToolBar>
#include <QSlider>
#include <QSpacerItem>


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


    toolBar->addWidget(new QLabel("Series:"));
    toolBar->addWidget(hslider_series_frame);
    toolBar->addWidget(new QLabel("Frames:"));
    toolBar->addWidget(hslider_dicom_frame);
    
}

MainWindow::~MainWindow()
{
    delete ui;
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
        ui->graphicsView->resetMatrix();
        _dicom_series->Add(df);

		// set slider
		hslider_series_frame->setEnabled(true);
		hslider_dicom_frame->setEnabled(true);

        hslider_series_frame->setRange(0, 0);
		hslider_series_frame->setValue(0);
		hslider_series_frame->valueChanged(0);
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
        ui->graphicsView->resetMatrix();


        // set slider
        hslider_series_frame->setEnabled(true);
        hslider_dicom_frame->setEnabled(true);

        hslider_series_frame->setRange(0, _dicom_series->GetTotalFrames()-1);
        hslider_series_frame->setValue(0);
        hslider_series_frame->valueChanged(0);
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
	if (_dicom_series->GetTotalFrames() < 1)
		return;

    auto di = _dicom_series->GetDicom(frame);
	int w = di->GetWidth();
	int h = di->GetHeight();
	auto pixelData = di->GetOutputData(hslider_dicom_frame->value());
	QImage image(pixelData, w, h, w, QImage::Format_Indexed8);
	ui->graphicsView->SetPixmap(QPixmap::fromImage(image));
}

void MainWindow::ShowNewDicomFrame(int frame)
{
    if (_dicom_series->GetTotalFrames() < 1)
        return;

	auto di = _dicom_series->GetDicom(hslider_series_frame->value());
	int w = di->GetWidth();
	int h = di->GetHeight();
	auto pixelData = di->GetOutputData(frame);
	QImage image(pixelData, w, h, w, QImage::Format_Indexed8);
	ui->graphicsView->SetPixmap(QPixmap::fromImage(image));
}

void MainWindow::DicomWindowChanged(QPointF windelta)
{
    if (_dicom_series == nullptr || _dicom_series->GetTotalFrames() < 1)
        return;

    double c, w;
    _dicom_series->GetWindow(c, w);
    c += windelta.x();
    w += windelta.y();
    if(w<2)w=2;
    
    _dicom_series->SetWindow(c, w);
    ShowNewSeriesFrame(hslider_series_frame->value());

	auto win_label = this->statusBar()->findChild<QLabel*>("window_label");
	win_label->setText(QString("\x20\x20WC:%1 WW:%2").arg(c).arg(w));
}


void MainWindow::on_actionSave_triggered()
{
	QString file_name = QFileDialog::getSaveFileName(this, "Save as picture", "", tr("*bmp;*jpg"));
	LOG(INFO) << QString("try to open file %1").arg(file_name).toLocal8Bit().data();

	//if (_dicom_series->GetTotalFrames() < 1)
	//	return;

 //   // TODO Save aicom as picture
	//auto di = _dicom_series->GetDicom(hslider_series_frame->value());
	//int w = di->GetWidth();
	//int h = di->GetHeight();
	//auto pixelData = di->GetOutputData(hslider_dicom_frame->value());
	//QImage image(pixelData, w, h, w, QImage::Format_Indexed8);
	//QPixmap picture = QPixmap::fromImage(image);
}

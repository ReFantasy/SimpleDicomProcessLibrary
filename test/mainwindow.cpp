#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "easylogging++.h"
#include <QFileDialog>
#include <QString>
#include <QPixmap>

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
    //auto res = _dicom_series->ReadDir("/Users/refantasy/Desktop/CT1001051/401_351");

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
        _dicom_series->Add(df);
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


		int w = _dicom_series->GetDicom(0)->GetWidth();
		int h = _dicom_series->GetDicom(0)->GetHeight();
		auto pixelData = _dicom_series->GetDicom(0)->GetOutputData(0);
		QImage image(pixelData, w, h, w, QImage::Format_Indexed8);
		auto pixmap = QPixmap::fromImage(image);
        ui->graphicsView->SetPixmap(pixmap);
    }
    else
    {
        LOG(ERROR)<<"open folder failed!";
    }
}


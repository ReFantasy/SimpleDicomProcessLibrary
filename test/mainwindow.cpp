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

    //auto res = _dicom_series->ReadDir("/Users/refantasy/Desktop/CT1001051/401_351");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpenFile_N_triggered()
{

    QString file_name  = QFileDialog::getOpenFileName(this, tr("文件对话框"));
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
    QString dir = QFileDialog::getExistingDirectory(this, tr("文件对话框"));
    LOG(INFO)<<QString("try to open folder %1").arg(dir).toLocal8Bit().data();

    auto tmp_series = std::make_shared<DicomSeries>();
    auto res  = tmp_series->ReadDir(dir.toStdString());
    if(res)
    {
        // delete existed dicom
        _dicom_series = tmp_series;

        LOG(INFO)<<"open folder success!";
    }
    else
    {
        LOG(ERROR)<<"open folder failed!";
    }
}


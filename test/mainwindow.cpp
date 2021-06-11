#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "easylogging++.h"
#include <QDebug>

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

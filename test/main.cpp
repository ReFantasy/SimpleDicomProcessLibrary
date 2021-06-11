#include <QApplication>
#include "mainwindow.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Simple Dicom ToolKit");

    //LOG(TRACE)   << "***** trace log  *****";
    //LOG(DEBUG)   << "***** debug log  *****";
    //LOG(ERROR)   << "***** error log  *****";
    //LOG(WARNING) << "***** warning log  *****";
    //LOG(INFO)    << "***** info log  *****";

    w.show();
    return a.exec();
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include "global.h"
#include "core.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 菜单栏 文件 打开文件
    void on_actionOpenFile_N_triggered();

    void on_actionOpen_DICOM_Folder_triggered();

    void ShowNewSeriesFrame(int frame);
    void ShowNewDicomFrame(int frame);

    void DicomWindowChanged(QPointF windelta);

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;

private:
    // dicom series
    std::shared_ptr<DicomSeries> _dicom_series;

private:
    QSlider* hslider_series_frame;
    QSlider* hslider_dicom_frame;
};

#endif // MAINWINDOW_H

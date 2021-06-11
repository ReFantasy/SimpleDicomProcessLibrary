#include <QApplication>
#include <QMainWindow>
#include <iostream>
#include <thread>
#include <vector>
#include "global.h"
#include "core.h"

using namespace std;

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	QMainWindow w;

	w.show();
	return a.exec();
}
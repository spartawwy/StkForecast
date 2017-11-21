#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include <qtextcodec.h>

int main(int argc, char *argv[])
{
#if 0
	QCoreApplication::addLibraryPath("D:\\Qt\\qt5.2.1_win32\\bin\\plugins");
#endif
    QCoreApplication::addLibraryPath(".");
	QCoreApplication::addLibraryPath("./plugins");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

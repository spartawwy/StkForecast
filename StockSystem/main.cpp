#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include "kline_wall.h"
#include <qtextcodec.h>

int main(int argc, char *argv[])
{
#if 1
	QCoreApplication::addLibraryPath("D:\\Qt\\qt5.2.1_win32\\bin\\plugins");
#endif
    QCoreApplication::addLibraryPath(".");
	QCoreApplication::addLibraryPath("./plugins");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    QApplication a(argc, argv);
#if 0
    MainWindow w;
#else
    KLineWall   w;
#endif
    w.show();
    return a.exec();
}

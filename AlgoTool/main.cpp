#include "algotool.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>

#define APP_CODE_TEXT "GBK"

int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("D:\\Qt\\qt5.2.1_win32\\bin\\plugins");
 
    QCoreApplication::addLibraryPath(".");
    QCoreApplication::addLibraryPath("./plugins");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName(APP_CODE_TEXT));

    QApplication a(argc, argv);
    AlgoTool w;
    w.show();
    return a.exec();
}

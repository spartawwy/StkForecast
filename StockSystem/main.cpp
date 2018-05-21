#include <QtWidgets/QApplication>
#include <qtextcodec.h>

#include <Tlib/core/tsystem_core_paths.h>
#include <TLib/tool/tsystem_exe_frame.h>

#include "stk_forecast_app.h"
 
#define APP_CODE_TEXT "GBK"

class TheFrame : public TSystem::ExecutableFrame
{

public:
    int main(int argc, char* argv[])
    {
        TSystem::utility::ProjectTag("WZF");
#ifdef PUBLISH
        if( !ValidSerial() )
            return -1;
#else
        QCoreApplication::addLibraryPath("D:\\Qt\\qt5.2.1_win32\\bin\\plugins");
#endif
        QCoreApplication::addLibraryPath(".");
        QCoreApplication::addLibraryPath("./plugins");
        QTextCodec::setCodecForLocale(QTextCodec::codecForName(APP_CODE_TEXT));

        StkForecastApp stk_forecast_app(argc, argv); 
        int ret = 0; 
        PrintAppInfo(stk_forecast_app);

        if( stk_forecast_app.Init() )
        { 
            PrintLaunchDone();
            ret = stk_forecast_app.exec(); 
        }else
            stk_forecast_app.Stop();

        stk_forecast_app.WaitShutdown();

        return ret;
    }
};

int main(int argc, char* argv[])
{
    TheFrame frame;
    return frame.LaunchServer(argc, argv);
}
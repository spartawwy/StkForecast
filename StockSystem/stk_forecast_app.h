#ifndef STK_FORECAST_APP_SDF8533_H_
#define STK_FORECAST_APP_SDF8533_H_

#include <memory>

#include <QtWidgets/QApplication>

#include <TLib/tool/tsystem_server_client_appbase.h>
#include <TLib/core/tsystem_communication_common.h>
#include <TLib/core/tsystem_serialization.h>

class ExchangeCalendar;
class DataBase;
class StockMan;
class MainWindow;
class KLineWall;
class StkForecastApp : public QApplication, public TSystem::ServerClientAppBase
{
    Q_OBJECT

public:

    StkForecastApp(int argc, char* argv[]); 
    ~StkForecastApp();

    bool Init();
    void Stop();

    std::shared_ptr<ExchangeCalendar>&  exchange_calendar() { return exchange_calendar_;}

protected:

    virtual void HandleNodeHandShake(TSystem::communication::Connection* , const TSystem::Message& ) override {};
    virtual void HandleNodeDisconnect(std::shared_ptr<TSystem::communication::Connection>& 
        , const TSystem::TError& ) override {};

private:

    std::shared_ptr<MainWindow>  main_window_;

    std::shared_ptr<DataBase>  data_base_;
    std::shared_ptr<ExchangeCalendar>  exchange_calendar_;
    std::shared_ptr<StockMan>  stock_man_;
};
#endif // STK_FORECAST_APP_SDF8533_H_
#include "stk_forecast_app.h"

#include <qmessageBox>
#include <qdebug.h>

#include <Tlib/core/tsystem_core_common.h>

#include "database.h"
#include "stock_man.h"

#include "mainwindow.h"
#include "kline_wall.h"

StkForecastApp::StkForecastApp(int argc, char* argv[])
    : QApplication(argc, argv)
    , ServerClientAppBase("client", "stk_forecast", "0.1")
    , data_base_(nullptr)
    , stock_man_(nullptr)
    , main_window_(nullptr)
{

}

StkForecastApp::~StkForecastApp()
{

}

bool StkForecastApp::Init()
{
    option_dir_type(AppBase::DirType::STAND_ALONE_APP);
    option_validate_app(false);

    std::string cur_dir(".//");
    work_dir(cur_dir);
    local_logger_.SetDir(cur_dir);
    //---------------
    data_base_ = std::make_shared<DataBase>(this);
    if( !data_base_->Initialize() )
    {
        return false;
    }
    stock_man_ = std::make_shared<StockMan>();

    data_base_->LoadAllStockBaseInfo(stock_man_);

    if( !stock_man_->Initialize() )
    {
        return false;
    }
    //---------------
    main_window_ = std::make_shared<MainWindow>(this);
    if( !main_window_->Initialize() )
        return false;
    main_window_->show();
    return true;
}

void StkForecastApp::Stop()
{
    Shutdown();
    this->quit();
}
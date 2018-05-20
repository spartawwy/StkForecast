#include "stock_man.h"

#include <qt_windows.h>
#include <cassert>

#include "py_data_man.h"
#include "database.h"

StockMan::StockMan(std::shared_ptr<Database>& db) : data_base_(db)
{
    
}

bool StockMan::Initialize()
{
    char chBuf[0x8000]={0};  
    DWORD dwSize = GetEnvironmentVariable("STK_DATA_DIR", chBuf, 0x10000);  
    if( dwSize <= 0 )
        return false;
    str_stk_data_path_ = chBuf;
    return true;
}

void StockMan::GetStockData(KLineType kline_type, const std::string &stk_code, int start_date, int end_date)
{ 
    assert(stk_code.length() == 6 );
    if( kline_type == KLineType::KLINE_DAY )
    {
        std::string target_file_path = str_stk_data_path_ + "/kline/" + stk_code;

    }
}

//bool StockMan::LoadAllStockBaseInfo()
//{
//
//}
#ifndef  STOCK_MAN_SDF3SDF_H_
#define  STOCK_MAN_SDF3SDF_H_

#include <memory>
#include <unordered_map>

#include "stkfo_common.h"

/*
  thread to get every stock's histry kline
*/
typedef std::unordered_map<std::string, std::shared_ptr<T_StockBaseInfoItem> > T_Str2StockBaseInfoItem_Map;

class Database;
class StockMan
{
public:

    StockMan(/*std::shared_ptr<Database>& db*/);
    ~StockMan(){}
    bool Initialize();

    void GetStockData(TypePeriod kline_type, const std::string &stk_code, int start_date, int end_date);

private:

    //bool LoadAllStockBaseInfo();

    //std::shared_ptr<Database> data_base_;

    std::string str_stk_data_path_;

    // (code , T_StockBaseInfoItem)
    T_Str2StockBaseInfoItem_Map code_stock_baseinfo_item_map_;
    T_Str2StockBaseInfoItem_Map pinyin_stock_baseinfo_item_map_;

    friend class DataBase;
};
#endif
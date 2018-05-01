#ifndef STOCKDAYINFO_H
#define STOCKDAYINFO_H

#include <string>
 
class StockDayInfo
{
public:
    StockDayInfo();

public:
        std::string stockNum;//股票代号
        //std::string transacteDate;//交易日期
        int   date;//交易日期
        float openPrice;//开盘价
        float closePrice;//收盘价
        float maxPrice;//最高价
        float minPrice;//最低价
        float marketMoney;//成交额
        float marketAmount;//成交量


public:
        void SetStockNum(std::string stockNum);
        std::string GetStockNum();
        float GetChangeRate();
        void ReadData(std::string strLine);
        void Display();
        float GetMinPrice();
        float GetMaxPrice();
        float GetOpenPrice();
        float GetClosePrice();

};

#endif // STOCKDAYINFO_H

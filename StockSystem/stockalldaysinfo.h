#ifndef STOCKALLDAYSINFO_H
#define STOCKALLDAYSINFO_H
#include <list>
#include <iostream>
#include <map>

#include "stockdayinfo.h"

#include "stk_quoter_api.h"

typedef std::list<T_StockHisDataItem>  T_HisDataItemList;

static bool compare(const T_StockHisDataItem &left_h, const T_StockHisDataItem &right_h)
{
    return left_h.date < right_h.date;
}
//static bool compare(const T_StockHisDataItem *left_h, const T_StockHisDataItem *right_h)
//{
//    return left_h->date < right_h->date;
//}
static bool compare_index(const T_StockHisDataItem *left_h, const T_StockHisDataItem *right_h)
{
    return left_h->date < right_h->date;
}
//using namespace std;

class StockAllDaysInfo
{
public:
    StockAllDaysInfo();
    bool Init();
public:
    //list容器，数据类型为一只股票一天的消息，是StockAllDaysInfo的数据成员
    std::list<StockDayInfo> stockAllDaysInfoList;

    //从fileName指定的磁盘路径中将数据一行一行读取出来，每一行初始化一个StockDayInfo对象
    void LoadDataFromFile(std::string fileName);
    T_HisDataItemList* LoadStockData(const std::string &stk_code, int start_date, int end_date);
	     
public:
    float GetLowestMinPrice();
    float GetHighestMaxPrice();

	float GetHisDataLowestMinPrice(const std::string& stock);
	float GetHisDataHighestMaxPrice(const std::string& stock);

public:
    std::list<StockDayInfo> GetStockAllDaysInfoList();
    std::map<std::string,  std::list<StockDayInfo> > stock_days_info_;

    std::map<std::string, T_HisDataItemList> stock_his_items_;

private:
    StkHisDataDelegate stk_his_data_;
    StkRelHisDataDelegate stk_hisdata_release_;

};

#endif // STOCKALLDAYSINFO_H

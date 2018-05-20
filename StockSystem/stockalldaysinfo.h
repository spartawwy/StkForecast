#ifndef STOCKALLDAYSINFO_H
#define STOCKALLDAYSINFO_H

#include <list>
#include <iostream>
#include <memory>
#include <map>
#include <vector>
 
#include "stkfo_common.h"

#include "stockdayinfo.h"
#include "stk_quoter_api.h"


//typedef std::list<T_StockHisDataItem>  T_HisDataItemList;
typedef std::vector<std::shared_ptr<T_KlineDataItem> >  T_HisDataItemContainer;

class StockAllDaysInfo
{
public:

    StockAllDaysInfo();
    bool Init();

public:
    //list容器，数据类型为一只股票一天的消息，是StockAllDaysInfo的数据成员
    //std::list<StockDayInfo> stockAllDaysInfoList;
    std::vector<std::shared_ptr<T_KlineDataItem> > day_kline_data_container_;

    //从fileName指定的磁盘路径中将数据一行一行读取出来，每一行初始化一个StockDayInfo对象
    void LoadDataFromFile(std::string &fileName);

    T_HisDataItemContainer* LoadStockData(const std::string &stk_code, int start_date, int end_date);
	     
public:

    float GetLowestMinPrice();
    float GetHighestMaxPrice();

	float GetHisDataLowestMinPrice(const std::string& stock);
	float GetHisDataHighestMaxPrice(const std::string& stock);

public:
    //std::list<StockDayInfo> GetStockAllDaysInfoList();
    //std::map<std::string,  std::list<StockDayInfo> > stock_days_info_;

    // (stock , data)
    std::map<std::string, T_HisDataItemContainer> stock_his_items_;

private:
    StkHisDataDelegate stk_his_data_;
    StkRelHisDataDelegate stk_hisdata_release_;

    std::shared_ptr<PyDataMan> py_data_man_;
};

#endif // STOCKALLDAYSINFO_H

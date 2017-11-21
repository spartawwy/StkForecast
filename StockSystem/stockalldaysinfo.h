#ifndef STOCKALLDAYSINFO_H
#define STOCKALLDAYSINFO_H
#include <list>
#include <iostream>
#include "stockdayinfo.h"

//using namespace std;

class StockAllDaysInfo
{
public:
    StockAllDaysInfo();

public:
    //list容器，数据类型为一只股票一天的消息，是StockAllDaysInfo的数据成员
    std::list<StockDayInfo> stockAllDaysInfoList;

    //从fileName指定的磁盘路径中将数据一行一行读取出来，每一行初始化一个StockDayInfo对象
    void LoadDataFromFile(std::string fileName);

public:
    float GetLowestMinPrice();
    float GetHighestMaxPrice();
    std::list<StockDayInfo> GetStockAllDaysInfoList();

};

#endif // STOCKALLDAYSINFO_H

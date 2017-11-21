#include "stockalldaysinfo.h"
#include <fstream>
#include <iostream>
#include <sstream>

StockAllDaysInfo::StockAllDaysInfo()
{
    LoadDataFromFile("./data/600030.dat");

}


void StockAllDaysInfo::LoadDataFromFile(std::string fileName)
{
    std::ifstream inputFile(fileName.c_str());
    std::string strLine;
    // read line at time until end-of-file
    while(getline(inputFile, strLine))
    {
        //cout<<strLine<<endl;
        StockDayInfo stockDayInfo;
        //将strLine中的所有字段拆解并赋值给stockDayInfo的各个数据成员
        stockDayInfo.ReadData(strLine);
        //stockDayInfo.Display();
        this->stockAllDaysInfoList.push_back(stockDayInfo);
    }
    inputFile.close();
}

//在链表中搜索出最低价中的最小值
float StockAllDaysInfo::GetLowestMinPrice()
{
    float lowestMinPrice = 100000000.0f;
    std::list<StockDayInfo>::iterator iter;
    for(iter = stockAllDaysInfoList.begin(); iter != stockAllDaysInfoList.end(); iter++)
    {
        //搜索当前元素是否比现有的最小值要更小，更小则替换
        if(lowestMinPrice > ((StockDayInfo)(*iter)).GetMinPrice())
        {
            lowestMinPrice = ((StockDayInfo)(*iter)).GetMinPrice();
        }
    }

    return lowestMinPrice;
}

float StockAllDaysInfo::GetHighestMaxPrice()
{
    float higestMaxPrice = 0.0f;
    std::list<StockDayInfo>::iterator iter;
    for(iter = stockAllDaysInfoList.begin(); iter != stockAllDaysInfoList.end(); iter++)
    {
        //搜索当前元素是否比现有的最小值要更小，更小则替换
        if(higestMaxPrice < ((StockDayInfo)(*iter)).GetMaxPrice())
        {
            higestMaxPrice = ((StockDayInfo)(*iter)).GetMaxPrice();
        }
    }
    return higestMaxPrice;
}

std::list<StockDayInfo> StockAllDaysInfo::GetStockAllDaysInfoList()
{
    return this->stockAllDaysInfoList;
}

#include "stockalldaysinfo.h"
#include <fstream>
#include <iostream>
#include <sstream>


#include <Windows.h>
#include <QtWidgets/QMessageBox>

#include <assert.h>

StockAllDaysInfo::StockAllDaysInfo()
{
    //LoadDataFromFile("./data/600030.dat");

}

bool StockAllDaysInfo::Init()
{
    //-----------------
    HMODULE moudle_handl = LoadLibrary("StkQuoter.dll");
    if( !moudle_handl )
    {
        QMessageBox::information(nullptr, "ALERT", "can't load stkQuoter.dll");
        return false;
    }

    stk_his_data_ = (StkHisDataDelegate)GetProcAddress(moudle_handl, "StkHisData");
    stk_hisdata_release_ = (StkRelHisDataDelegate)GetProcAddress(moudle_handl, "StkRelHisData");
    if( stk_his_data_ && stk_hisdata_release_ )
        return true;
    else
        return false;
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

// date is save from recent to remote
void StockAllDaysInfo::LoadStockData(const std::string &stk_code, int start_date, int end_date)
{
    assert( stk_his_data_ && stk_hisdata_release_ );

    //stock_days_info_.find(
    T_StockHisDataItem *p_data_items;
    int count = stk_his_data_(const_cast<char*>(stk_code.c_str()), start_date, end_date, &p_data_items);
    if( !p_data_items )
    {
        return;
    }
    auto iter = stock_his_items_.find(stk_code);

    if( iter == stock_his_items_.end() )
       iter = stock_his_items_.insert(std::make_pair(stk_code, std::list<T_StockHisDataItem>())).first;

    for( int i = 0; i < count; ++i )
    {
        if( iter->second.begin() == iter->second.end() )
         {
             iter->second.push_back(p_data_items[i]);
             continue;
         }
        if( iter->second.rbegin()->date > p_data_items[i].date )
            iter->second.push_back(p_data_items[i]);
    }
    iter->second.sort(compare);
    //std::sort(iter->second.begin(), iter->second.end(), compare_index);
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

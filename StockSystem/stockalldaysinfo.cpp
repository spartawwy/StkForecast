#include "stockalldaysinfo.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <memory>

#include <Windows.h>
#include <QtWidgets/QMessageBox>
#include <QDebug>


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
T_HisDataItemList* StockAllDaysInfo::LoadStockData(const std::string &stk_code, int start_date, int end_date)
{
    assert( stk_his_data_ && stk_hisdata_release_ );

    //stock_days_info_.find(
    T_StockHisDataItem *p_data_items = nullptr;
    int count = stk_his_data_(const_cast<char*>(stk_code.c_str()), start_date, end_date, &p_data_items);

    if( !p_data_items )
    {
        return nullptr;
    }

    std::list<std::shared_ptr<T_KlineDateItem> > T_KlineDateItems;

    for( int k = 0; k < count; ++k )
    {
        auto k_item = std::make_shared<T_KlineDateItem>(p_data_items[k]);
        T_KlineDateItems.push_back(std::move(k_item)); 
    }
    // sort T_KlineDateItems by day 

    // todo: 1) find lowest price k, in every 3 
           //2) judge if fit the ftracture 
    // 


    auto iter = stock_his_items_.find(stk_code);

    if( iter == stock_his_items_.end() )
       iter = stock_his_items_.insert(std::make_pair(stk_code, T_HisDataItemList())).first;

    for( int i = 0; i < count; ++i )
    {
        if( iter->second.begin() == iter->second.end() )
         {
             iter->second.push_back(p_data_items[i]);
             continue;
         }
		/*bool is_find = std::find_if( std::begin(iter->second), std::end(iter->second), [&](T_HisDataItemList::reference entry)
		{
			return entry.date == p_data_items[i].date;
		});*/
        if( iter->second.rbegin()->date < p_data_items[i].date )
		{
			qDebug() << " rbegin data " << iter->second.rbegin()->date << "\n";
            iter->second.push_back(p_data_items[i]);
		}else if( iter->second.begin()->date > p_data_items[i].date )
		{
			iter->second.push_front(p_data_items[i]);
		}
    }
	stk_hisdata_release_(p_data_items);

    iter->second.sort(compare);
    //std::sort(iter->second.begin(), iter->second.end(), compare_index);
	return std::addressof(iter->second);
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

float StockAllDaysInfo::GetHisDataLowestMinPrice(const std::string& stock)
{
	auto iter = stock_his_items_.find(stock);
	if( iter == stock_his_items_.end() )
		return 0.0;
	iter->second;

	float lowestMinPrice = 100000000.0f; 
	std::for_each( std::begin(iter->second), std::end(iter->second), [&](const T_StockHisDataItem& entry)
    { 
		if( lowestMinPrice > entry.low_price )
        {
            lowestMinPrice = entry.low_price;
        }
    }); 
    return lowestMinPrice;
}

float StockAllDaysInfo::GetHisDataHighestMaxPrice(const std::string& stock)
{
	auto iter = stock_his_items_.find(stock);
	if( iter == stock_his_items_.end() )
		return 0.0;
	float higestMaxPrice = 0.0f; 
	std::for_each( std::begin(iter->second), std::end(iter->second), [&](const T_StockHisDataItem& entry)
    { 
		if( higestMaxPrice < entry.high_price )
        {
            higestMaxPrice = entry.high_price;
        }
    }); 
	return higestMaxPrice;
}


std::list<StockDayInfo> StockAllDaysInfo::GetStockAllDaysInfoList()
{
    return this->stockAllDaysInfoList;
}

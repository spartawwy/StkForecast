#include "stockalldaysinfo.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <memory>

#include <Windows.h>
#include <QtWidgets/QMessageBox>
#include <QDebug>

#include "py_data_man.h"

#define RESERVE_CAPACITY_IN_T_VECTOR    1024*16
//#define RESERVE_SIZE_IN_T_VECTOR    1024*16
//#define FIRST_STARTPOS_IN_T_VECTOR  10000
//#define USE_STK_QUOTER

static bool compare(const T_StockHisDataItem &left_h, const T_StockHisDataItem &right_h)
{
    return left_h.date < right_h.date; // from small to big
}
 
// from small to big
static bool dompare( std::shared_ptr<T_KlineDataItem> &lh, std::shared_ptr<T_KlineDataItem> &rh)
{
    return lh->stk_item.date < rh->stk_item.date;
}

static bool bompare(const T_KlineDataItem &lh, const T_KlineDataItem &rh)
{
    return lh.stk_item.date < rh.stk_item.date;
}

// 下分形遍历
void TraverseSetUpwardFractal( std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items);
// 上分形遍历
void TraverseSetDownwardFractal( std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items);

StockAllDaysInfo::StockAllDaysInfo()
    : WinnerHisHq_GetKData_(nullptr)
{
    //LoadDataFromFile("./data/600030.dat");

}

bool StockAllDaysInfo::Init()
{
    py_data_man_ = std::make_shared<PyDataMan>();
    bool ret = py_data_man_->Initiate();

#ifdef USE_STK_QUOTER
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
#endif
    return ret;
}

//void StockAllDaysInfo::LoadDataFromFile(std::string &fileName)
//{
//    std::ifstream inputFile(fileName.c_str());
//    std::string strLine; 
//    while(getline(inputFile, strLine))
//    { 
//    }
//    inputFile.close();
//}

// date is save from older date to newer. ps: data in container is series trade date
T_HisDataItemContainer* StockAllDaysInfo::AppendStockData(const std::string &stk_code, int start_date, int end_date)
{
    T_StockHisDataItem *p_data_items = nullptr;
    const int count = 0;

#ifdef USE_STK_QUOTER
    assert( stk_his_data_ && stk_hisdata_release_ );
    // ret p_data_items is from big date to small date. [0].date is biggest
    count = stk_his_data_(const_cast<char*>(stk_code.c_str()), start_date, end_date, &p_data_items);
#else

    static auto call_back_fun = [](T_K_Data *k_data, bool is_end, void *para)
    {

    };

    T_KDataCallBack  call_back_obj;
    call_back_obj.call_back_func = call_back_fun;
    bool is_index = false;
    char error_info[1024] = {"\0"};
    WinnerHisHq_GetKData_(const_cast<char*>(stk_code.c_str()), PeriodType::PERIOD_DAY, start_date, end_date
                                                           , &call_back_obj, is_index, error_info);

#endif
    if( !p_data_items )
    {
        return nullptr;
    }
#if 1
    // save data to stock_his_items_ and sort it ---------------------------
    auto iter_already = stock_his_items_.find(stk_code);
    if( iter_already == stock_his_items_.end() )
        iter_already = stock_his_items_.insert(std::make_pair(stk_code, T_HisDataItemContainer())).first;
    if( count < 1 )
        return std::addressof(iter_already->second);
    // only can insert to back of front 
    std::deque<std::shared_ptr<T_KlineDataItem> > &items_in_container = iter_already->second; 
    if( !items_in_container.empty() )
    {
        if( p_data_items[0].date < items_in_container.back()->stk_item.date )
        { 
            for( int k = 0; k < count; ++k )
            {
                auto ck_val = p_data_items[k].date;
                if( p_data_items[k].date < items_in_container.front()->stk_item.date )
                {
                    auto k_item = std::make_shared<T_KlineDataItem>(p_data_items[k]); 
                    items_in_container.push_front(std::move(k_item));
                }
            }
        }else
        {
            for( int k = count; k > 0; --k )
            {
                if( p_data_items[k-1].date > items_in_container.back()->stk_item.date )
                {
                    auto k_item = std::make_shared<T_KlineDataItem>(p_data_items[k-1]); 
                    items_in_container.push_back(std::move(k_item));
                }
            }
        }
        items_in_container.front()->stk_item.date;
    }else
    {
        for( int k = count; k > 0; --k )
        {
            auto k_item = std::make_shared<T_KlineDataItem>(p_data_items[k-1]); 
            items_in_container.push_back(std::move(k_item));
        }
    }
     
    // sort T_KlineDateItems by day from small to bigger
    std::sort(items_in_container.begin(), items_in_container.end(), dompare);
     
#endif 
#ifdef USE_STK_QUOTER
	stk_hisdata_release_(p_data_items);
#endif

    TraverseSetUpwardFractal(items_in_container);

    TraverseSetDownwardFractal(items_in_container);
    
	return std::addressof(iter_already->second);

}

//在链表中搜索出最低价中的最小值
float StockAllDaysInfo::GetLowestMinPrice()
{
    float lowestMinPrice = 100000000.0f;
    //std::list<StockDayInfo>::iterator iter;
    for(auto iter = day_kline_data_container_.begin(); iter != day_kline_data_container_.end(); iter++)
    {
        //搜索当前元素是否比现有的最小值要更小，更小则替换
        if( lowestMinPrice > (*iter)->stk_item.low_price )
        {
            lowestMinPrice = (*iter)->stk_item.low_price;
        }
    }

    return lowestMinPrice;
}

float StockAllDaysInfo::GetHighestMaxPrice()
{
    float higestMaxPrice = 0.0f;
    //std::list<StockDayInfo>::iterator iter;
    for( auto iter = day_kline_data_container_.begin(); iter != day_kline_data_container_.end(); iter++)
    { 
        if(higestMaxPrice < (*iter)->stk_item.high_price )
        {
            higestMaxPrice = (*iter)->stk_item.high_price;
        }
    }
    return higestMaxPrice;
}

float StockAllDaysInfo::GetHisDataLowestMinPrice(const std::string& stock)
{
	auto iter = stock_his_items_.find(stock);
	if( iter == stock_his_items_.end() )
		return 0.0;
	 
	float lowestMinPrice = 100000000.0f; 
	std::for_each( std::begin(iter->second), std::end(iter->second), [&](const std::shared_ptr<T_KlineDataItem>& entry)
    { 
		if( lowestMinPrice > entry->stk_item.low_price )
        {
            lowestMinPrice = entry->stk_item.low_price;
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
	std::for_each( std::begin(iter->second), std::end(iter->second), [&](const std::shared_ptr<T_KlineDataItem>& entry)
    { 
		if( higestMaxPrice < entry->stk_item.high_price )
        {
            higestMaxPrice = entry->stk_item.high_price;
        }
    }); 
	return higestMaxPrice;
}

 
void TraverseSetUpwardFractal( std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items)
{
    if( kline_data_items.size() < 1 )
        return;
    unsigned int index = 1;
    while( index < kline_data_items.size() - 1 )
    {
        if( kline_data_items[index-1]->stk_item.low_price < kline_data_items[index]->stk_item.low_price
            || kline_data_items[index+1]->stk_item.low_price < kline_data_items[index]->stk_item.low_price )
        {
            ++index;
            continue;
        }
        int n_fractal_ahead = 0;
        int n_fractal_follow = 0;
        // search fractal ahead  -----------
        unsigned int index_to_check = index;
        for( int k = index_to_check - 1; k > 0; )
        {
            if( kline_data_items[k]->stk_item.low_price > kline_data_items[index_to_check]->stk_item.low_price )
            { 
                if( kline_data_items[k]->stk_item.high_price > kline_data_items[index_to_check]->stk_item.high_price )
                { 
                    ++n_fractal_ahead;
                    index_to_check = k;
                } 
                --k;
            }else
            { 
                break;
            }
            
        }
         
        if( n_fractal_ahead > 0 )
        {   // search fractal follow  --------  
            index_to_check = index;
            for( unsigned int k = index + 1; k < kline_data_items.size(); )
            {
                if( kline_data_items[k]->stk_item.low_price > kline_data_items[index_to_check]->stk_item.low_price )
                {
                    if( kline_data_items[k]->stk_item.low_price > kline_data_items[index_to_check]->stk_item.high_price )
                    {
                        // it's transfer k line 
                    }else if( kline_data_items[k]->stk_item.high_price > kline_data_items[index_to_check]->stk_item.high_price )
                    {
                        //kline_data_items[k]->type |= UPWARD_FRACTAL;
                        ++n_fractal_follow;
                        index_to_check = k; 
                    } /* {else
                       kline_data_items[k]->type |= INSUFFIC_FRACTAL; 
                       }*/
                    ++k;
                     
                }else
                    break;
            }
        }

        if( n_fractal_ahead > 0 && n_fractal_follow > 0 )
        {
            kline_data_items[index]->type |= BTM_AXIS_T_3;
            if( n_fractal_ahead > 1 && n_fractal_follow > 1 )
            {
                kline_data_items[index]->type |= BTM_AXIS_T_5;
                if( n_fractal_ahead > 2 && n_fractal_follow > 2 )
                {
                    kline_data_items[index]->type |= BTM_AXIS_T_7;
                    if( n_fractal_ahead > 3 && n_fractal_follow > 3 )
                    {
                        kline_data_items[index]->type |= BTM_AXIS_T_9;
                        if( n_fractal_ahead > 4 && n_fractal_follow > 4 )
                        {
                            kline_data_items[index]->type |= BTM_AXIS_T_11;
                        }
                    }
                }
            }
        }
        ++index;
    }//while
}

void TraverseSetDownwardFractal( std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items)
{
    if( kline_data_items.size() < 1 )
        return;
    unsigned int index = 1;
    while( index < kline_data_items.size() - 1 )
    {
        if( kline_data_items[index-1]->stk_item.high_price > kline_data_items[index]->stk_item.high_price
            || kline_data_items[index+1]->stk_item.high_price > kline_data_items[index]->stk_item.high_price )
        {
            ++index;
            continue;
        }
        int n_fractal_ahead = 0;
        int n_fractal_follow = 0;
        // search fractal ahead  -----------
        int index_to_check = index;
        for( int k = index_to_check - 1; k > 0; )
        {
            if( kline_data_items[k]->stk_item.high_price < kline_data_items[index_to_check]->stk_item.high_price )
            { 
                if( kline_data_items[k]->stk_item.low_price < kline_data_items[index_to_check]->stk_item.low_price )
                { 
                    ++n_fractal_ahead;
                    index_to_check = k;
                } 
                --k;
            }else
            { 
                break;
            }
            
        }
         
        if( n_fractal_ahead > 0 )
        {   // search fractal follow  --------  
            index_to_check = index;
            for( unsigned int k = index + 1; k < kline_data_items.size(); )
            {
                if( kline_data_items[k]->stk_item.high_price < kline_data_items[index_to_check]->stk_item.high_price )
                {
                    if( kline_data_items[k]->stk_item.high_price < kline_data_items[index_to_check]->stk_item.low_price )
                    {
                        // it's transfer k line 
                    }else if( kline_data_items[k]->stk_item.low_price < kline_data_items[index_to_check]->stk_item.low_price )
                    {
                        //kline_data_items[k]->type |= UPWARD_FRACTAL;
                        ++n_fractal_follow;
                        index_to_check = k; 
                    } /* {else
                       kline_data_items[k]->type |= INSUFFIC_FRACTAL; 
                       }*/
                    ++k;
                     
                }else
                    break;
            }
        }

        if( n_fractal_ahead > 0 && n_fractal_follow > 0 )
        {
            kline_data_items[index]->type |= TOP_AXIS_T_3;
            if( n_fractal_ahead > 1 && n_fractal_follow > 1 )
            {
                kline_data_items[index]->type |= TOP_AXIS_T_5;
                if( n_fractal_ahead > 2 && n_fractal_follow > 2 )
                {
                    kline_data_items[index]->type |= TOP_AXIS_T_7;
                    if( n_fractal_ahead > 3 && n_fractal_follow > 3 )
                    {
                        kline_data_items[index]->type |= TOP_AXIS_T_9;
                        if( n_fractal_ahead > 4 && n_fractal_follow > 4 )
                        {
                            kline_data_items[index]->type |= TOP_AXIS_T_11;
                        }
                    }
                }
            }
        }
        ++index;
    }//while
}
#include "stockalldaysinfo.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <memory>

#include <TLib/core/tsystem_time.h>

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
    , WinnerHisHq_Connect_(nullptr)
    , WinnerHisHq_DisConnect_(nullptr)
    , p_stk_hisdata_item_vector_(nullptr)
{
    //LoadDataFromFile("./data/600030.dat");

}

StockAllDaysInfo::~StockAllDaysInfo()
{
    if( WinnerHisHq_DisConnect_ )
        WinnerHisHq_DisConnect_();
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
#else
    HMODULE moudle_handle = LoadLibrary("winner_api.dll");
    if( !moudle_handle )
    {
        QMessageBox::information(nullptr, "ALERT", "can't load winner_api.dll");
        return false;
    }
    WinnerHisHq_Connect_ = (WinnerHisHq_ConnectDelegate)GetProcAddress(moudle_handle, "WinnerHisHq_Connect"); 
    if ( !WinnerHisHq_Connect_ )
    {
        std::cout << " GetProcAddress WinnerHisHq_Connect fail " << std::endl;
        return false;
    }

    WinnerHisHq_DisConnect_ =  (WinnerHisHq_DisconnectDelegate)GetProcAddress(moudle_handle, "WinnerHisHq_Disconnect"); 
    char result[1024] = {0};
    char error[1024] = {0};
#if 0
    ret = WinnerHisHq_Connect("192.168.1.5", 50010, result, error);
#else
    ret  = 0 == WinnerHisHq_Connect_("128.1.4.156", 50010, result, error);
#endif 
    if( ret )
    {
        WinnerHisHq_GetKData_ = (WinnerHisHq_GetKDataDelegate)GetProcAddress(moudle_handle, "WinnerHisHq_GetKData");
        if( WinnerHisHq_GetKData_ )
            return true;
        else
            return false;
    }else
    {
        QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("连接行情服务器失败!")); 
    }
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

void call_back_fun(T_K_Data *k_data, bool is_end, void *para/*, std::vector<T_StockHisDataItem> &data_item_vector*/)
{
    T_KDataCallBack *cb_obj = (T_KDataCallBack*)para;
    
    StockAllDaysInfo *p_stk_alldaysinfo_obj = (StockAllDaysInfo *)(cb_obj->para);
    std::vector<T_StockHisDataItem> *p_vector = p_stk_alldaysinfo_obj ? p_stk_alldaysinfo_obj->p_stk_hisdata_item_vector_ : nullptr;
    
    T_StockHisDataItem item;
    item.open_price = k_data->open;
    item.close_price = k_data->close;
    item.high_price = k_data->high;
    item.low_price = k_data->low;
    item.vol - k_data->vol;
    item.date = k_data->yyyymmdd;
    if( p_vector )
    {
        p_vector->push_back(std::move(item));
        if( is_end )
        {
            p_stk_alldaysinfo_obj->is_fetched_stk_hisdata_ = true;
        }
    }
}
// date is save from older date to newer. ps: data in container is series trade date
T_HisDataItemContainer* StockAllDaysInfo::AppendStockData(PeriodType period_type, const std::string &stk_code, int start_date, int end_date)
{
    int count = 0;
    T_HisDataItemContainer & items_in_container = GetHisDataContainer(period_type, stk_code);
   /* auto iter_already = data_item_container.find(stk_code);
    if( iter_already == day_stock_his_items_.end() )
        iter_already = day_stock_his_items_.insert(std::make_pair(stk_code, T_HisDataItemContainer())).first;
    */
#ifdef USE_STK_QUOTER
    T_StockHisDataItem *p_data_items = nullptr;
    assert( stk_his_data_ && stk_hisdata_release_ );
    // ret p_data_items is from big date to small date. [0].date is biggest
    count = stk_his_data_(const_cast<char*>(stk_code.c_str()), start_date, end_date, &p_data_items);
    if( count < 1 )
      return std::addressof(iter_already->second);
#else 
    //std::vector<T_StockHisDataItem> data_item_vector;
    if( !WinnerHisHq_GetKData_ )
        return std::addressof(items_in_container);

    call_back_obj_.call_back_func = call_back_fun;
    call_back_obj_.para = this;
    if( p_stk_hisdata_item_vector_ ) 
        delete p_stk_hisdata_item_vector_;
    p_stk_hisdata_item_vector_ = new std::vector<T_StockHisDataItem>();
   
    is_fetched_stk_hisdata_ = false;
    bool is_index = false;
    char error_info[1024] = {"\0"};
    WinnerHisHq_GetKData_(const_cast<char*>(stk_code.c_str()), PeriodType::PERIOD_DAY, start_date, end_date
                                                           , &call_back_obj_, is_index, error_info);

    std::vector<T_StockHisDataItem> &p_data_items = *p_stk_hisdata_item_vector_;
    bool ret = TSystem::WaitFor( [this]()->bool
    { 
        return this->is_fetched_stk_hisdata_;
    }, 30 * 1000 * 1000);
     
    if( !ret )
    {
        delete p_stk_hisdata_item_vector_;
        p_stk_hisdata_item_vector_ = nullptr;
        return std::addressof(items_in_container);
    }
    count = p_data_items.size();
#endif
    
#if 1
    // save data to day_stock_his_items_ and sort it ---------------------------
   
    // only can insert to back of front 
    //std::deque<std::shared_ptr<T_KlineDataItem> > &items_in_container = iter_already->second; 
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
#else
     delete p_stk_hisdata_item_vector_;
     p_stk_hisdata_item_vector_ = nullptr;
#endif

    TraverseSetUpwardFractal(items_in_container);

    TraverseSetDownwardFractal(items_in_container);
    
	return std::addressof(items_in_container);

}

T_HisDataItemContainer & StockAllDaysInfo::GetHisDataContainer(PeriodType period_type, const std::string& code)
{
    T_CodeMapHisDataItemContainer *p_code_map_container = nullptr;
    switch(period_type)
    {
    case PeriodType::PERIOD_5M:   p_code_map_container = &m5_stock_his_items_; break;
    case PeriodType::PERIOD_15M:  p_code_map_container = &m15_stock_his_items_; break;
    case PeriodType::PERIOD_30M:  p_code_map_container = &m30_stock_his_items_; break;
    case PeriodType::PERIOD_HOUR: p_code_map_container = &hour_stock_his_items_; break;
    case PeriodType::PERIOD_DAY:  p_code_map_container = &day_stock_his_items_; break; 
    case PeriodType::PERIOD_WEEK: p_code_map_container = &week_stock_his_items_; break;
    case PeriodType::PERIOD_MON:  p_code_map_container = &mon_stock_his_items_; break;
    default: assert(false);
    }
    auto container_iter = p_code_map_container->find(code);
    if( container_iter == p_code_map_container->end() )
        container_iter = p_code_map_container->insert(std::make_pair(code, T_HisDataItemContainer())).first;
    return container_iter->second;
}

//在链表中搜索出最低价中的最小值
float StockAllDaysInfo::GetLowestMinPrice(PeriodType period_type, std::string &code)
{
    float lowestMinPrice = 100000000.0f;
    T_HisDataItemContainer & item_container = GetHisDataContainer(period_type, code);
    for(auto iter = item_container.begin(); iter != item_container.end(); ++iter)
    {
        //搜索当前元素是否比现有的最小值要更小，更小则替换
        if( lowestMinPrice > (*iter)->stk_item.low_price )
        {
            lowestMinPrice = (*iter)->stk_item.low_price;
        }
    }
    return lowestMinPrice;
}

float StockAllDaysInfo::GetHighestMaxPrice(PeriodType period_type, std::string &code)
{
    float higestMaxPrice = 0.0f;
   T_HisDataItemContainer & item_container = GetHisDataContainer(period_type, code);
    for(auto iter = item_container.begin(); iter != item_container.end(); ++iter)
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
	auto iter = day_stock_his_items_.find(stock);
	if( iter == day_stock_his_items_.end() )
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
	auto iter = day_stock_his_items_.find(stock);
	if( iter == day_stock_his_items_.end() )
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
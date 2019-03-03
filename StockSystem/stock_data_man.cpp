#include "stock_data_man.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <memory>

#include <TLib/core/tsystem_utility_functions.h>
#include <TLib/core/tsystem_time.h>

#include <Windows.h>
#include <QtWidgets/QMessageBox>
#include <QDebug>

#ifdef USE_PYTHON_QUOTE
#include "py_data_man.h"
#endif

#include "tdx_hq_wrapper.h"
#include "zhibiao.h"
#include "exchange_calendar.h"

#define RESERVE_CAPACITY_IN_T_VECTOR    1024*16
//#define RESERVE_SIZE_IN_T_VECTOR    1024*16
//#define FIRST_STARTPOS_IN_T_VECTOR  10000

static bool compare(const T_StockHisDataItem &lh, const T_StockHisDataItem &rh)
{
    return (lh.date < rh.date) || (lh.date == rh.date && lh.hhmmss < rh.hhmmss);
}
 
// from small to big
static bool dompare( std::shared_ptr<T_KlineDataItem> &lh, std::shared_ptr<T_KlineDataItem> &rh)
{
    return (lh->stk_item.date < rh->stk_item.date) || (lh->stk_item.date == rh->stk_item.date && lh->stk_item.hhmmss < rh->stk_item.hhmmss);
}

static bool bompare(const T_KlineDataItem &lh, const T_KlineDataItem &rh)
{
    return (lh.stk_item.date < rh.stk_item.date) || (lh.stk_item.date == rh.stk_item.date && lh.stk_item.hhmmss < rh.stk_item.hhmmss);
}

// 下分形遍历
void TraverseSetUpwardFractal( std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items);
// 上分形遍历
void TraverseSetDownwardFractal( std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items);

StockDataMan::StockDataMan(ExchangeCalendar *p_exchange_calendar)
    : m5_stock_his_items_(1024)
    , m15_stock_his_items_(1024)
    , m30_stock_his_items_(1024)
    , hour_stock_his_items_(1024)
    , day_stock_his_items_(1024)
    , week_stock_his_items_(1024)
    , mon_stock_his_items_(1024)
#ifdef USE_WINNER_API
    , WinnerHisHq_GetKData_(nullptr)
    , WinnerHisHq_Connect_(nullptr)
    , WinnerHisHq_DisConnect_(nullptr)
#endif
    , p_stk_hisdata_item_vector_(nullptr)
    , tdx_hq_wrapper_(p_exchange_calendar)
    , p_exchange_calendar_(p_exchange_calendar)
{
    //LoadDataFromFile("./data/600030.dat");
    zhibiao_types_.push_back(ZhibiaoType::MOMENTUM); // momentum is in pos MOMENTUM_POS: 0
}

StockDataMan::~StockDataMan()
{
#ifdef USE_WINNER_API
    if( WinnerHisHq_DisConnect_ )
        WinnerHisHq_DisConnect_();
#endif
}

bool StockDataMan::Init()
{
    bool ret = true;
#ifdef USE_PYTHON_QUOTE
    py_data_man_ = std::make_shared<PyDataMan>();
    ret = py_data_man_->Initiate();
#endif

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
#elif defined(USE_WINNER_API)
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
    if( !_stricmp(TSystem::utility::host().c_str(), "hzdev103") )
        ret  = 0 == WinnerHisHq_Connect_("128.1.4.156", 50010, result, error);
    else
        ret  = 0 == WinnerHisHq_Connect_("192.168.1.5", 50010, result, error);
 
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
#elif defined(USE_TDXHQ)

    return tdx_hq_wrapper_.Init();

#endif

    return ret;
}

#ifdef USE_WINNER_API
void call_back_fun(T_K_Data *k_data, bool is_end, void *para/*, std::vector<T_StockHisDataItem> &data_item_vector*/)
{
    T_KDataCallBack *cb_obj = (T_KDataCallBack*)para;
    
    StockDataMan *p_stk_data_man_obj = (StockDataMan *)(cb_obj->para);
    std::vector<T_StockHisDataItem> *p_vector = p_stk_data_man_obj ? p_stk_data_man_obj->p_stk_hisdata_item_vector_ : nullptr;
    
    T_StockHisDataItem item;
    item.open_price = k_data->open;
    item.close_price = k_data->close;
    item.high_price = k_data->high;
    item.low_price = k_data->low;
    item.vol = k_data->vol;
    item.date = k_data->yyyymmdd;
    item.hhmmss = k_data->hhmmdd;
    if( p_vector )
    {
        p_vector->push_back(std::move(item));
        if( is_end )
        {
            p_stk_data_man_obj->is_fetched_stk_hisdata_ = true;
        }
    }
}
#endif

T_HisDataItemContainer* StockDataMan::FindStockData(PeriodType period_type, const std::string &stk_code, int start_date, int end_date, int cur_hhmm, bool /*is_index*/)
{
    assert( !stk_code.empty() ); 
    T_HisDataItemContainer & items_in_container = GetHisDataContainer(period_type, stk_code);
    int real_start_date = exchange_calendar()->CeilingTradeDate(start_date);
    int real_end_date = exchange_calendar()->FloorTradeDate(end_date);
    int start_hhmm = 0;
    switch(period_type)
    {
        case PeriodType::PERIOD_YEAR:
        case PeriodType::PERIOD_MON:
        case PeriodType::PERIOD_WEEK:
        case PeriodType::PERIOD_DAY:
            start_hhmm = 0; break;
        case PeriodType::PERIOD_HOUR:
            start_hhmm = 1030; break;
        case PeriodType::PERIOD_30M:
            start_hhmm = 1000; break;
        case PeriodType::PERIOD_15M:
            start_hhmm = 945; break;
        case PeriodType::PERIOD_5M:
            start_hhmm = 935; break;
    }
    if( FindDataIndex(items_in_container, real_start_date, start_hhmm) > -1 && FindDataIndex(items_in_container, real_end_date, cur_hhmm) > -1 )
        return &items_in_container;
    return nullptr;
}

// date is save from older(smaller) date to newer(bigger). ps: data in container is series trade date
T_HisDataItemContainer* StockDataMan::AppendStockData(PeriodType period_type, const std::string &stk_code, int start_date, int end_date, bool is_index)
{
    assert( !stk_code.empty() );
    int count = 0;
    T_HisDataItemContainer & items_in_container = GetHisDataContainer(period_type, stk_code);
   
#ifdef USE_STK_QUOTER
    T_StockHisDataItem *p_data_items = nullptr;
    assert( stk_his_data_ && stk_hisdata_release_ );
    // ret p_data_items is from big date to small date. [0].date is biggest
    count = stk_his_data_(const_cast<char*>(stk_code.c_str()), start_date, end_date, &p_data_items);
    if( count < 1 )
      return std::addressof(iter_already->second);

#elif defined(USE_WINNER_API)
    //std::vector<T_StockHisDataItem> data_item_vector;
    if( !WinnerHisHq_GetKData_ )
        return std::addressof(items_in_container);

    call_back_obj_.call_back_func = call_back_fun;
    call_back_obj_.para = this;
    if( p_stk_hisdata_item_vector_ ) 
        delete p_stk_hisdata_item_vector_;
    p_stk_hisdata_item_vector_ = new std::vector<T_StockHisDataItem>();
   
    is_fetched_stk_hisdata_ = false; 
    char error_info[1024] = {"\0"};
    std::string code = stk_code;
    if( is_index )
        code = TransIndex2TusharedCode(stk_code);
    WinnerHisHq_GetKData_(const_cast<char*>(code.c_str()), period_type, start_date, end_date
                                                           , &call_back_obj_, is_index, error_info);
    // vector's date is from small to big
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

#elif defined(USE_TDXHQ)
    std::string code = stk_code;
    if( is_index )
        code = TransIndex2TusharedCode(stk_code);
    auto p_stk_hisdata_item_vector = new std::vector<T_StockHisDataItem>();
    std::vector<T_StockHisDataItem> &p_data_items = *p_stk_hisdata_item_vector;  
    bool ret = tdx_hq_wrapper_.GetHisKBars(code, is_index, ToTypePeriod(period_type), start_date, end_date, *p_stk_hisdata_item_vector);
    if( !ret || p_data_items.empty() )
    {
        delete p_stk_hisdata_item_vector_;
        p_stk_hisdata_item_vector_ = nullptr;
        return std::addressof(items_in_container);
    }
    count = p_stk_hisdata_item_vector->size();
#endif
    
    // save data to day_stock_his_items_ and sort it ---------------------------
   
    // only can insert to back or front  
    if( !items_in_container.empty() )
    {
#if  defined(USE_STK_QUOTER)
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
#elif defined(USE_WINNER_API) || defined(USE_TDXHQ)
        if( p_data_items[p_data_items.size()-1].date <= items_in_container.back()->stk_item.date )
        { 
            for( int k = count; k > 0; --k )
            {
                //auto ck_val = p_data_items[k-1].date;
                if( p_data_items[k-1].date < items_in_container.front()->stk_item.date || 
                    (p_data_items[k-1].date == items_in_container.front()->stk_item.date &&
                    p_data_items[k-1].hhmmss < items_in_container.front()->stk_item.hhmmss) )
                {
                    auto k_item = std::make_shared<T_KlineDataItem>(p_data_items[k-1]); 
                    k_item->zhibiao_atoms.push_back(std::move(std::make_shared<MomentumZhibiao>()));
                    items_in_container.push_front(std::move(k_item));
                }
            }
        }else
        {
            for( int k = 0; k < count; ++k )
            {
                if( p_data_items[k].date > items_in_container.back()->stk_item.date ||
                    (p_data_items[k].date == items_in_container.back()->stk_item.date &&
                    p_data_items[k].hhmmss > items_in_container.back()->stk_item.hhmmss) )
                {
                    auto k_item = std::make_shared<T_KlineDataItem>(p_data_items[k]); 
                    k_item->zhibiao_atoms.push_back(std::move(std::make_shared<MomentumZhibiao>()));
                    items_in_container.push_back(std::move(k_item));
                }
            }
        }
        //items_in_container.front()->stk_item.date;
    }else
    {
        for( int k = 0; k < count; ++k )
        {
            auto k_item = std::make_shared<T_KlineDataItem>(p_data_items[k]); 
            k_item->zhibiao_atoms.push_back(std::move(std::make_shared<MomentumZhibiao>()));
            items_in_container.push_back(std::move(k_item));
        }
    }
#endif
    
    // sort T_KlineDateItems by day from small to bigger
    std::sort(items_in_container.begin(), items_in_container.end(), dompare);

    CaculateZhibiao(items_in_container);

#if defined(USE_WINNER_API) || defined(USE_TDXHQ) 
    delete p_stk_hisdata_item_vector;
    p_stk_hisdata_item_vector = nullptr;
#else
    stk_hisdata_release_(p_data_items);
#endif

    TraverseSetUpwardFractal(items_in_container);

    TraverseSetDownwardFractal(items_in_container);
    
	return std::addressof(items_in_container);

}

bool StockDataMan::UpdateLatestItemStockData(PeriodType period_type, const std::string &stk_code, bool is_index)
{
    assert( !stk_code.empty() );
   
    T_HisDataItemContainer & items_in_container = GetHisDataContainer(period_type, stk_code);
    
    T_StockHisDataItem  item;
    memset(&item, 0, sizeof(item));
    tdx_hq_wrapper_.GetLatestKBar(stk_code, is_index, ToTypePeriod(period_type), item);
    if( items_in_container.back()->stk_item.date == item.date && items_in_container.back()->stk_item.hhmmss == item.hhmmss )
    {
        memcpy( std::addressof(items_in_container.back()->stk_item), &item, sizeof(item) );
        CaculateZhibiao(items_in_container);
        return true;
    }else
        return false;
}

// ps: data has sorted
void StockDataMan::CaculateZhibiao(T_HisDataItemContainer &data_items_in_container)
{
    for (unsigned int i = 0; i < zhibiao_types_.size(); ++i )
    {
        switch(zhibiao_types_[i])
        {
        case ZhibiaoType::MOMENTUM:
            {
                MomentumZhibiao::Caculate(data_items_in_container);
                break;
            }
        default: break;
        }
    }
}

T_HisDataItemContainer & StockDataMan::GetHisDataContainer(PeriodType period_type, const std::string& code)
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
 
// ok: ret <  MAX_PRICE
float StockDataMan::GetHisDataLowestMinPrice(PeriodType period_type, const std::string& code, int start_date, int end_date)
{ 
	float lowestMinPrice = MAX_PRICE; 
    assert(start_date <= end_date);
    auto index_tuple = GetDateIndexFromContainer(period_type, code, start_date, end_date);
    if( index_tuple == std::make_tuple(-1, -1) )
        return lowestMinPrice;
    T_HisDataItemContainer & container = GetHisDataContainer(period_type, code);
     
    for( int j = std::get<0>(index_tuple); j <= std::get<1>(index_tuple); ++j )
    {
        if( lowestMinPrice > container.at(j)->stk_item.low_price )
        {
            lowestMinPrice = container.at(j)->stk_item.low_price;
        }
    }
    return lowestMinPrice;
}

// ok : > 0.0 
float StockDataMan::GetHisDataHighestMaxPrice(PeriodType period_type, const std::string& code, int start_date, int end_date)
{
    float higestMaxPrice = MIN_PRICE; 
    assert(start_date <= end_date);
    auto index_tuple = GetDateIndexFromContainer(period_type, code, start_date, end_date);
    if( index_tuple == std::make_tuple(-1, -1) )
        return higestMaxPrice;

    T_HisDataItemContainer & container = GetHisDataContainer(period_type, code);

    for( int j = std::get<0>(index_tuple); j <= std::get<1>(index_tuple); ++j )
    {
        if( higestMaxPrice < container.at(j)->stk_item.high_price )
        {
            higestMaxPrice = container.at(j)->stk_item.high_price;
        }
    }
	return higestMaxPrice;
}

// <-1, -1>: fail
std::tuple<int, int> StockDataMan::GetDateIndexFromContainer(PeriodType period_type, const std::string& stock, int start_date, int end_date)
{
    assert(start_date <= end_date);
    //std::tuple<int, int> result = std::make_tuple(-1, -1);

    T_HisDataItemContainer & container = GetHisDataContainer(period_type, stock);
    int temp_start_date = container.at(0)->stk_item.date;
    if( end_date < container.at(0)->stk_item.date )
        return std::make_tuple(-1, -1);
    if( start_date > container.at(0)->stk_item.date )
        temp_start_date = start_date;

    int start_index = -1;
    int end_index = -1;
    for( unsigned int i = 0; i < container.size(); ++i )
    {
        if( temp_start_date == container.at(i)->stk_item.date ) 
        {
            start_index = i;
            break;
        }
    }

    for( unsigned int i = container.size(); i > 0; --i )
    {
        if( end_date == container.at(i-1)->stk_item.date ) 
        {
            end_index = i - 1;
            break;
        }
    }
    if( start_index == -1 )
        start_index = 0;
    if( end_index == -1 )
        end_index = container.size() - 1;
    return std::make_tuple(start_index, end_index);
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
            kline_data_items[index]->type |= int(FractalType::BTM_AXIS_T_3);
            if( n_fractal_ahead > 1 && n_fractal_follow > 1 )
            {
                kline_data_items[index]->type |= int(FractalType::BTM_AXIS_T_5);
                if( n_fractal_ahead > 2 && n_fractal_follow > 2 )
                {
                    kline_data_items[index]->type |= int(FractalType::BTM_AXIS_T_7);
                    if( n_fractal_ahead > 3 && n_fractal_follow > 3 )
                    {
                        kline_data_items[index]->type |= int(FractalType::BTM_AXIS_T_9);
                        if( n_fractal_ahead > 4 && n_fractal_follow > 4 )
                        {
                            kline_data_items[index]->type |= int(FractalType::BTM_AXIS_T_11);
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
            kline_data_items[index]->type |= int(FractalType::TOP_AXIS_T_3);
            if( n_fractal_ahead > 1 && n_fractal_follow > 1 )
            {
                kline_data_items[index]->type |= int(FractalType::TOP_AXIS_T_5);
                if( n_fractal_ahead > 2 && n_fractal_follow > 2 )
                {
                    kline_data_items[index]->type |= int(FractalType::TOP_AXIS_T_7);
                    if( n_fractal_ahead > 3 && n_fractal_follow > 3 )
                    {
                        kline_data_items[index]->type |= int(FractalType::TOP_AXIS_T_9);
                        if( n_fractal_ahead > 4 && n_fractal_follow > 4 )
                        {
                            kline_data_items[index]->type |= int(FractalType::TOP_AXIS_T_11);
                        }
                    }
                }
            }
        }
        ++index;
    }//while
}


TypePeriod ToTypePeriod(PeriodType src)
{
    switch(src)
    {
    case PeriodType::PERIOD_5M: return TypePeriod::PERIOD_5M;
    case PeriodType::PERIOD_15M: return TypePeriod::PERIOD_15M;
    case PeriodType::PERIOD_30M: return TypePeriod::PERIOD_30M;
    case PeriodType::PERIOD_HOUR: return TypePeriod::PERIOD_HOUR;
    case PeriodType::PERIOD_DAY: return TypePeriod::PERIOD_DAY;
    case PeriodType::PERIOD_WEEK: return TypePeriod::PERIOD_WEEK;
    case PeriodType::PERIOD_MON: return TypePeriod::PERIOD_MON;
    assert(false); 
    }
    return TypePeriod::PERIOD_DAY;
}

// < 0 : meaning no related data
int FindDataIndex(T_HisDataItemContainer &data_items_in_container, int date, int cur_hhmm)
{
    for( unsigned int i = 0; i < data_items_in_container.size(); ++i )
    {
        if( data_items_in_container.at(i)->stk_item.date == date && data_items_in_container.at(i)->stk_item.hhmmss == cur_hhmm)
            return i;
    }
    return -1;
}
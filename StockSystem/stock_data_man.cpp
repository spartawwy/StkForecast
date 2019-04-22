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
void TraverseSetUpwardFractal(std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items);
// 上分形遍历
void TraverseSetDownwardFractal(std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items);

void TraverseClearFractalType(std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items);

void TraverseAjustFractal(std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items);
 

StockDataMan::StockDataMan(ExchangeCalendar *p_exchange_calendar)
    : m5_stock_his_items_(1024)
    , m15_stock_his_items_(1024)
    , m30_stock_his_items_(1024)
    , hour_stock_his_items_(1024)
    , day_stock_his_items_(1024)
    , week_stock_his_items_(1024)
    , mon_stock_his_items_(1024)
    , m5_stock_bi_items_(64)
    , m15_stock_bi_items_(64)
    , m30_stock_bi_items_(64)
    , hour_stock_bi_items_(64)
    , day_stock_bi_items_(64)
    , week_stock_bi_items_(64)
    , mon_stock_bi_items_(64)
    , m5_stock_struct_datas_(64)
    , m15_stock_struct_datas_(64)
    , m30_stock_struct_datas_(64)
    , hour_stock_struct_datas_(64)
    , day_stock_struct_datas_(64)
    , week_stock_struct_datas_(64)
    , mon_stock_struct_datas_(64)
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
        if( p_data_items[p_data_items.size()-1].date < items_in_container.back()->stk_item.date )
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
    TraverseClearFractalType(items_in_container);

    TraverseSetUpwardFractal(items_in_container);

    TraverseSetDownwardFractal(items_in_container);
    
    TraverseAjustFractal(items_in_container);

    TraverseGetBi(period_type, code, items_in_container);

    TraverseGetStuctLines(period_type, code, items_in_container);

    TraversGetSections(period_type, code, items_in_container);

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
 
T_BiContainer & StockDataMan::GetBiContainer(PeriodType period_type, const std::string& code)
{
    T_CodeMapBiContainer *p_code_map_container = nullptr;
    switch(period_type)
    {
    case PeriodType::PERIOD_5M:   p_code_map_container = &m5_stock_bi_items_; break;
    case PeriodType::PERIOD_15M:  p_code_map_container = &m15_stock_bi_items_; break;
    case PeriodType::PERIOD_30M:  p_code_map_container = &m30_stock_bi_items_; break;
    case PeriodType::PERIOD_HOUR: p_code_map_container = &hour_stock_bi_items_; break;
    case PeriodType::PERIOD_DAY:  p_code_map_container = &day_stock_bi_items_; break; 
    case PeriodType::PERIOD_WEEK: p_code_map_container = &week_stock_bi_items_; break;
    case PeriodType::PERIOD_MON:  p_code_map_container = &mon_stock_bi_items_; break;
    default: assert(false);
    }
    auto container_iter = p_code_map_container->find(code);
    if( container_iter == p_code_map_container->end() )
        container_iter = p_code_map_container->insert(std::make_pair(code, T_BiContainer())).first;
    return container_iter->second;
}

T_StructLineContainer &StockDataMan::GetStructLineContainer(PeriodType period_type, const std::string& code)
{
    T_CodeMapStructDataContainer *p_code_map_container = nullptr;
    switch(period_type)
    {
    case PeriodType::PERIOD_5M:   p_code_map_container = &m5_stock_struct_datas_; break;
    case PeriodType::PERIOD_15M:  p_code_map_container = &m15_stock_struct_datas_; break;
    case PeriodType::PERIOD_30M:  p_code_map_container = &m30_stock_struct_datas_; break;
    case PeriodType::PERIOD_HOUR: p_code_map_container = &hour_stock_struct_datas_; break;
    case PeriodType::PERIOD_DAY:  p_code_map_container = &day_stock_struct_datas_; break; 
    case PeriodType::PERIOD_WEEK: p_code_map_container = &week_stock_struct_datas_; break;
    case PeriodType::PERIOD_MON:  p_code_map_container = &mon_stock_struct_datas_; break;
    default: assert(false);
    }
    auto container_iter = p_code_map_container->find(code);
    if( container_iter == p_code_map_container->end() )
        container_iter = p_code_map_container->insert(std::make_pair(code, T_StructData())).first;
    return container_iter->second.struct_line_container;
}


T_SectionContainer & StockDataMan::GetStructSectionContainer(PeriodType period_type, const std::string& code)
{
    T_CodeMapStructDataContainer *p_code_map_container = nullptr;
    switch(period_type)
    {
    case PeriodType::PERIOD_5M:   p_code_map_container = &m5_stock_struct_datas_; break;
    case PeriodType::PERIOD_15M:  p_code_map_container = &m15_stock_struct_datas_; break;
    case PeriodType::PERIOD_30M:  p_code_map_container = &m30_stock_struct_datas_; break;
    case PeriodType::PERIOD_HOUR: p_code_map_container = &hour_stock_struct_datas_; break;
    case PeriodType::PERIOD_DAY:  p_code_map_container = &day_stock_struct_datas_; break; 
    case PeriodType::PERIOD_WEEK: p_code_map_container = &week_stock_struct_datas_; break;
    case PeriodType::PERIOD_MON:  p_code_map_container = &mon_stock_struct_datas_; break;
    default: assert(false);
    }
    auto container_iter = p_code_map_container->find(code);
    if( container_iter == p_code_map_container->end() )
        container_iter = p_code_map_container->insert(std::make_pair(code, T_StructData())).first;
    return container_iter->second.section_container;
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
 
/*   \/ 
    kline_data_items[0] is smallest time  */
void TraverseSetUpwardFractal( std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items)
{
    if( kline_data_items.size() < 1 )
        return;
    unsigned int index = 1;
    while( index < kline_data_items.size() - 1 )
    {
        //debug -------
        int ck_date = kline_data_items[index]->stk_item.date;
        ck_date = ck_date;
        //--------
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
        int k = index_to_check - 1;
        int ahead_k_num = k;
        for( ; k > 0; )
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
                if( kline_data_items[k]->stk_item.low_price < kline_data_items[index]->stk_item.low_price )
                    break;
                --k;
            }
        }
        ahead_k_num = ahead_k_num - k;
        if( n_fractal_ahead > 0 )
        {   // search fractal follow  --------  
            index_to_check = index;
            bool is_transfer_k = false;
            for( unsigned int k = index + 1; k < kline_data_items.size() && k < index + 1 + 3*ahead_k_num; )
            {
                if( kline_data_items[k]->stk_item.low_price > kline_data_items[index_to_check]->stk_item.low_price )
                {
                    if( !is_transfer_k && kline_data_items[k]->stk_item.low_price > kline_data_items[index_to_check]->stk_item.high_price )
                    {
                        // it's transfer k line 
                        is_transfer_k = true;
                    }else if( kline_data_items[k]->stk_item.high_price > kline_data_items[index_to_check]->stk_item.high_price )
                    {
                        //kline_data_items[k]->type |= UPWARD_FRACTAL;
                        ++n_fractal_follow;
                        index_to_check = k; 
                        is_transfer_k = false;
                    } /* {else
                       kline_data_items[k]->type |= INSUFFIC_FRACTAL; 
                       }*/
                    ++k;
                }else if( kline_data_items[k]->stk_item.low_price < kline_data_items[index_to_check]->stk_item.low_price )
                    break;
                else
                    ++k;
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
                            kline_data_items[index]->type |= int(FractalType::BTM_AXIS_T_11);
                    }
                }
            }
        }
        ++index;
    }//while
}

/*   /\ 
    kline_data_items[0] is smallest time  */
void TraverseSetDownwardFractal( std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items)
{
    if( kline_data_items.size() < 1 )
        return;
    unsigned int index = 1;
    while( index < kline_data_items.size() - 1 )
    { 
        //debug -------
        int ck_date = kline_data_items[index]->stk_item.date;
        ck_date = ck_date;
        //--------
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
        int k = index_to_check - 1;
        int ahead_k_num = k;
        for( ; k > 0; )
        {
            if( kline_data_items[k]->stk_item.low_price < kline_data_items[index_to_check]->stk_item.low_price )
            {
                if( kline_data_items[k]->stk_item.high_price < kline_data_items[index_to_check]->stk_item.high_price )
                { 
                    ++n_fractal_ahead;
                    index_to_check = k;
                }else
                   ;// ahead out containter 
                 --k;
            }else 
            {
                if( kline_data_items[k]->stk_item.high_price > kline_data_items[index]->stk_item.high_price )
                    break;
                else // unenough line or inner container line
                    --k;
            } 
        }//for
        ahead_k_num = ahead_k_num - k;
         
        if( n_fractal_ahead > 0 )
        {   // search fractal follow  --------  
            index_to_check = index;
            bool is_transfer_k = false;
            for( unsigned int k = index + 1; k < kline_data_items.size() && k < index + 1 + 3*ahead_k_num; )
            {
                if( kline_data_items[k]->stk_item.high_price < kline_data_items[index_to_check]->stk_item.high_price )
                {
                    if( !is_transfer_k && kline_data_items[k]->stk_item.high_price < kline_data_items[index_to_check]->stk_item.low_price )
                    {
                        // it's transfer k line 
                        is_transfer_k = true;
                    }else if( kline_data_items[k]->stk_item.low_price < kline_data_items[index_to_check]->stk_item.low_price )
                    {
                        //kline_data_items[k]->type |= UPWARD_FRACTAL;
                        ++n_fractal_follow;
                        index_to_check = k; 
                        is_transfer_k = false;
                    } /* {else
                       kline_data_items[k]->type |= INSUFFIC_FRACTAL; 
                       }*/
                    ++k;
                     
                }else 
                {   // out container k
                    if( kline_data_items[k]->stk_item.low_price < kline_data_items[index_to_check]->stk_item.low_price + 0.0001 )
                        ++k;
                    else
                        break;
                }
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

void TraverseClearFractalType(std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items)
{
    if( kline_data_items.size() < 1 )
        return;
    unsigned int index = kline_data_items.size();
    while( --index > 0 )
    {
        kline_data_items[index]->type = (int)FractalType::UNKNOW_FRACTAL;
    }
}

void TraverseAjustFractal( std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items)
{
    static auto find_left_btm_frac = [](std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items, int index)->int
    {
        int i = index;
        while( --i >= 0 )
        {
            if( BtmestFractalType(kline_data_items[i]->type) != FractalType::UNKNOW_FRACTAL )
                return i;
        }
        return -1;
    };
    static auto find_left_top_frac = [](std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items, int index)->int
    {
        int i = index;
        while( --i >= 0 )
        {
            if( MaxFractalType(kline_data_items[i]->type) >= FractalType::TOP_AXIS_T_3 )
                return i;
        }
        return -1;
    };
    // from index to j (not contain index)
    static auto find_left_btm = [](std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items, int index, int j)->int
    {
        double tmp_price = MAX_PRICE;
        int i = index;
        int target_i = -1;
        while( --i > j && i > -1 )
        {
            if( kline_data_items[i]->stk_item.low_price < tmp_price )
            {
                tmp_price = kline_data_items[i]->stk_item.low_price;
                target_i = i;
            }
        }
        return target_i;
    };
    // from index to j (contain index and j)
    static auto find_left_top = [](std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items, int index, int j)->int
    {
        double tmp_price = MIN_PRICE;
        int i = index;
        int target_i = -1;
        while( i-- >= j && i > -1 )
        {
            if( kline_data_items[i]->stk_item.high_price > tmp_price )
            {
                tmp_price = kline_data_items[i]->stk_item.high_price;
                target_i = i;
            }
        }
        return target_i;
    };

    static auto find_left_top_uncontain = [](std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items, int index, int j)->int
    {
        double tmp_price = MIN_PRICE;
        int i = index;
        int target_i = -1;
        while( --i > j && i > -1 )
        {
            if( kline_data_items[i]->stk_item.high_price > tmp_price )
            {
                tmp_price = kline_data_items[i]->stk_item.high_price;
                target_i = i;
            }
        }
        return target_i;
    };
    // ps : uncontain index and nearby_index
    static auto get_min_max = [](std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items, int index, int nearby_index)->std::tuple<double, int, double, int>
    { 
        double tmp_max_price = MIN_PRICE;
        double tmp_min_price = MAX_PRICE;
        int i = index;
        int target_min_index = i - 1;
        int target_max_index = i - 1;
        while( --i > nearby_index && i > -1 )
        {
            if( kline_data_items[i]->stk_item.low_price < tmp_min_price )
            {
                tmp_min_price = kline_data_items[i]->stk_item.low_price;
                target_min_index = i;
            }
            if( kline_data_items[i]->stk_item.high_price > tmp_max_price )
            {
                tmp_max_price = kline_data_items[i]->stk_item.high_price;
                target_max_index = i;
            }
        }
        return std::make_tuple(tmp_min_price, target_min_index, tmp_max_price, target_max_index);
    };
    if( kline_data_items.size() < 1 )
        return;
    unsigned int index = kline_data_items.size();
    while( --index > 0 )
    {
        int frac_date = kline_data_items[index]->stk_item.date; 
        if( kline_data_items[index]->type == (int)FractalType::UNKNOW_FRACTAL )
            continue;
        if( IsBtmFractal(kline_data_items[index]->type) ) // btm frac
        {
            int btm_index = find_left_btm_frac(kline_data_items, index);
            int top_index = find_left_top_frac(kline_data_items, index);
            if( btm_index > top_index ) // btm is nearby
            {
                if( index - btm_index == 1 && !IsTopFractal(kline_data_items[index]->type) )
                {
                    if( kline_data_items[index]->stk_item.low_price < kline_data_items[btm_index]->stk_item.low_price )
                        ClearBtmFractal(kline_data_items[btm_index]->type);
                    else
                        ClearBtmFractal(kline_data_items[index]->type);
                    continue;
                }
                // find left k which pirce is higest
                auto min_max_tuple = get_min_max(kline_data_items, index, btm_index);
                double left_min_price = std::get<0>(min_max_tuple);
                int left_min_index = std::get<1>(min_max_tuple);
                double left_top_price = std::get<2>(min_max_tuple);
                int left_top_index = std::get<3>(min_max_tuple);
                //int left_top_index = find_left_top_uncontain(kline_data_items, index, btm_index);
                if( left_top_index > 0 
                    && left_top_price > kline_data_items[btm_index]->stk_item.high_price 
                    && left_top_price > kline_data_items[index]->stk_item.high_price)
                { 
                    kline_data_items[left_top_index]->type |= (int)FractalType::TOP_FAKE;
                    qDebug() << __LINE__ << " set top fake flag date " <<  kline_data_items[left_top_index]->stk_item.date << " " <<  kline_data_items[left_top_index]->stk_item.hhmmss;
                    if( left_min_price < kline_data_items[index]->stk_item.low_price 
                        && left_min_index >= left_top_index )
                    {
                        qDebug() << __LINE__ << "index " << index << " left_min_index " << left_min_index << " left_top_index " << left_top_index;
                        kline_data_items[left_min_index]->type |= (int)FractalType::BTM_FAKE;
                        qDebug() << __LINE__ << " set btm fake flag date " <<  kline_data_items[left_min_index]->stk_item.date<< " " <<  kline_data_items[left_min_index]->stk_item.hhmmss;
                        ClearBtmFractal(kline_data_items[index]->type);
                        qDebug() << __LINE__ << " clear btm fake date " <<  kline_data_items[index]->stk_item.date << " " <<  kline_data_items[index]->stk_item.hhmmss;
                    }
                }else if( !IsTopFractal(kline_data_items[index]->type) )
                {
                    if( kline_data_items[btm_index]->stk_item.low_price > kline_data_items[index]->stk_item.low_price )
                    {
                        // clear serial btm flag 
                        ClearBtmFractal(kline_data_items[btm_index]->type);
                        int tmp_top_index = find_left_top_frac(kline_data_items, btm_index);
                        int tmp_btm_index = find_left_btm_frac(kline_data_items, btm_index);
                        while( tmp_btm_index > -1 && tmp_btm_index > tmp_top_index )
                        {
                            ClearBtmFractal(kline_data_items[tmp_btm_index]->type);
                            int tmp_index = tmp_btm_index;
                            tmp_top_index = find_left_top_frac(kline_data_items, tmp_index);
                            tmp_btm_index = find_left_btm_frac(kline_data_items, tmp_index);
                        }
                    }else
                        ClearBtmFractal(kline_data_items[index]->type);
                    
                }
                 
            }else if( top_index > -1 )// top_index is nearby
            {
                // find max min price between   
                auto obj_tuple = get_min_max(kline_data_items, index, top_index);
                const double min_price = std::get<0>(obj_tuple);
                const int min_p_index = std::get<1>(obj_tuple);
                const double max_price = std::get<2>(obj_tuple);
                const int max_p_index = std::get<3>(obj_tuple);
                if( max_price > kline_data_items[top_index]->stk_item.high_price )
                { 
                    ClearTopFractal(kline_data_items[top_index]->type);
                    qDebug() << __LINE__ << " clear top flag date " <<  kline_data_items[top_index]->stk_item.date;
                    kline_data_items[max_p_index]->type |= (int)FractalType::TOP_FAKE;
                    qDebug() << __LINE__ << " set top fake flag date " <<  kline_data_items[max_p_index]->stk_item.date;
                }
                if( btm_index > -1 && min_price < kline_data_items[index]->stk_item.low_price )
                {
                    ClearBtmFractal(kline_data_items[index]->type);
                    qDebug() << __LINE__ << " clear btm fake date " <<  kline_data_items[index]->stk_item.date;
                    kline_data_items[min_p_index]->type |= (int)FractalType::BTM_FAKE;
                    qDebug() << __LINE__ << " set btm fake flag date " <<  kline_data_items[min_p_index]->stk_item.date;
                } 
            }
        }

        if( IsTopFractal(kline_data_items[index]->type) ) // top frac
        {
            int frac_date = kline_data_items[index]->stk_item.date; 
            int top_index = find_left_top_frac(kline_data_items, index);
            int btm_index = find_left_btm_frac(kline_data_items, index);
            if( top_index > btm_index ) // left top fram is nearby
            {
                if( index - top_index == 1 && !IsBtmFractal(kline_data_items[index]->type) )
                {
                    if( kline_data_items[index]->stk_item.high_price > kline_data_items[top_index]->stk_item.high_price )
                        ClearTopFractal(kline_data_items[top_index]->type);
                    else
                        ClearTopFractal(kline_data_items[index]->type);
                    continue;
                }
                // find left k which pirce is lowest
                int left_btm_index = find_left_btm(kline_data_items, index, top_index);
                if( left_btm_index > 0 
                    && kline_data_items[left_btm_index]->stk_item.low_price < kline_data_items[index]->stk_item.low_price /*+ 0.0001*/
                    && kline_data_items[left_btm_index]->stk_item.low_price < kline_data_items[top_index]->stk_item.low_price /*+ 0.0001*/)
                { 
                    kline_data_items[left_btm_index]->type |= (int)FractalType::BTM_FAKE;
                    qDebug() << __LINE__ << " set btm fake flag date " <<  kline_data_items[left_btm_index]->stk_item.date;
                }else
                {
                    if( kline_data_items[top_index]->stk_item.high_price < kline_data_items[index]->stk_item.high_price )
                    {
                        // clear serial top flag 
                        ClearTopFractal(kline_data_items[top_index]->type);
                        int tmp_top_index = find_left_top_frac(kline_data_items, top_index);
                        int tmp_btm_index = find_left_btm_frac(kline_data_items, top_index);
                        while( tmp_top_index > -1 && tmp_top_index > tmp_btm_index )
                        {
                            ClearTopFractal(kline_data_items[tmp_top_index]->type);
                            int tmp_index = tmp_top_index;
                            tmp_top_index = find_left_top_frac(kline_data_items, tmp_index);
                            tmp_btm_index = find_left_btm_frac(kline_data_items, tmp_index);
                        }
                    }else
                        ClearTopFractal(kline_data_items[index]->type);
                } 
                
            }else if( btm_index > -1 ) // btm_index is nearby
            {
                // find max min price between   
                auto obj_tuple = get_min_max(kline_data_items, index, btm_index);
                const double min_price = std::get<0>(obj_tuple);
                const int min_p_index = std::get<1>(obj_tuple);
                const double max_price = std::get<2>(obj_tuple);
                const int max_p_index = std::get<3>(obj_tuple);
                if( top_index > -1 && max_price > kline_data_items[index]->stk_item.high_price )
                {
                    ClearTopFractal(kline_data_items[index]->type);
                    qDebug() << __LINE__ << " clear top flag date " <<  kline_data_items[index]->stk_item.date;
                    kline_data_items[max_p_index]->type |= (int)FractalType::TOP_FAKE;
                    qDebug() << __LINE__ << " set top fake flag date " <<  kline_data_items[max_p_index]->stk_item.date;
                }
                if( min_price < kline_data_items[btm_index]->stk_item.low_price )
                {
                    ClearBtmFractal(kline_data_items[btm_index]->type);
                    qDebug() << __LINE__ << " clear btm flag date " <<  kline_data_items[btm_index]->stk_item.date;
                    kline_data_items[min_p_index]->type |= (int)FractalType::BTM_FAKE;
                    qDebug() << __LINE__ << " set btm fake flag date " <<  kline_data_items[min_p_index]->stk_item.date;
                } 
            }
        }
    }
}


// ps: called after TraverseAjustFractal
void StockDataMan::TraverseGetBi(PeriodType period_type, const std::string &code, std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items)
{
    static auto create_start_end_point =[](std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items, int i, int index)->std::tuple<T_BiPoint, T_BiPoint>
    {
        T_BiPoint  start;
        start.date = kline_data_items[i]->stk_item.date;
        start.hhmm = kline_data_items[i]->stk_item.hhmmss;
        start.index = i;
        start.frac_type = FractalType(kline_data_items[i]->type);
        T_BiPoint  end;
        end.date = kline_data_items[index]->stk_item.date;
        end.hhmm = kline_data_items[index]->stk_item.hhmmss;
        end.index = index;
        end.frac_type = FractalType(kline_data_items[index]->type);
        end.date = kline_data_items[index]->stk_item.date;
        return std::make_tuple(start, end);
    };

    if( kline_data_items.size() < 1 )
        return;
    T_BiContainer &container = GetBiContainer(period_type, code); 
    container.clear();
    unsigned int index = kline_data_items.size();
    while( --index > 0 )
    {
        if( IsTopFractal(kline_data_items[index]->type) )
        {
            int ck_index_date = kline_data_items[index]->stk_item.date;
            int i = index;
            int max_count = 30;
            double low_price = MAX_PRICE;
            int btm_index = -1;
            int target_index = -1;
            while( --i >= 0 && max_count-- > 0 )
            {
                int ck_i_date = kline_data_items[i]->stk_item.date;
                if( kline_data_items[i]->stk_item.high_price > kline_data_items[index]->stk_item.high_price )
                    break;
                else if( IsBtmFractal(kline_data_items[i]->type) && kline_data_items[i]->stk_item.low_price < low_price)
                { 
                    btm_index = i;
                    low_price = kline_data_items[i]->stk_item.low_price;
                    if( index - i > 5 )
                        target_index = i;
                }else if( IsTopFractal(kline_data_items[i]->type) && btm_index > -1 && btm_index - i > 5 )
                    break;
            }
            if( target_index > -1 )
            {
                auto start_end = create_start_end_point(kline_data_items, target_index, index);
                container.push_front( std::make_shared<T_Bi>(BiType::UP, std::get<0>(start_end), std::get<1>(start_end)) );
                index = target_index + 1;
            }

        }else if( IsBtmFractal(kline_data_items[index]->type) )
        {
            int i = index;
            int max_count = 30;
            double high_price = MIN_PRICE;
            int high_index = -1;
            int target_index = -1;
            while( --i >= 0 && max_count-- > 0 )
            {
                if( kline_data_items[i]->stk_item.low_price < kline_data_items[index]->stk_item.low_price)
                    break;
                if( IsTopFractal(kline_data_items[i]->type) && kline_data_items[i]->stk_item.high_price > high_price)
                {
                    high_index = i;
                    high_price = kline_data_items[i]->stk_item.high_price;
                    if( index - i > 5 )
                        target_index = i;
                }else if( IsBtmFractal(kline_data_items[i]->type) && high_index > -1 && high_index - i > 5 )
                    break;
            }
            if( target_index > -1 )
            {
                auto start_end = create_start_end_point(kline_data_items, target_index, index);
                container.push_front( std::make_shared<T_Bi>(BiType::DOWN, std::get<0>(start_end), std::get<1>(start_end)) );
                index = target_index + 1;
            }
        }
    }
}

int find_next_btm_fractal(std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items, int from_index)
{
    int local_index = from_index;
    while( local_index >= 0 )
    {
        if( IsBtmFractal(kline_data_items[local_index]->type) )
            return local_index;
        --local_index;
    }
    return -1;
}

int find_next_top_fractal(std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items, int from_index)
{
    int local_index = from_index;
    while( local_index >= 0 )
    {
        if( IsTopFractal(kline_data_items[local_index]->type) )
            return local_index;
        --local_index;
    }
    return -1;
}

void find_down_towardleft_end(std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items, bool is_src_k_same, double lowest_price, int start, int &end) 
{
    if( start == 0 )
        return;
    // start is top fractal. A
    int a_date = kline_data_items[start]->stk_item.date;
    double price_a = kline_data_items[start]->stk_item.high_price;
    // 1.find next btm fractal B 
    int btm_index_b = -1;
    if( !is_src_k_same && IsBtmFractal(kline_data_items[start]->type) ) // start is both top and btm fractal line 
        btm_index_b = start;
    else
        btm_index_b = find_next_btm_fractal(kline_data_items, start - 1);
    if( btm_index_b == -1 )
        return; 

    const double price_b = kline_data_items[btm_index_b]->stk_item.low_price;
    //assert( price_b < price_a );
    // find next top fractal C
    int top_index_c = -1;
    if( btm_index_b != start && IsTopFractal(kline_data_items[btm_index_b]->type) ) // start is both top and btm fractal line 
        top_index_c = btm_index_b;
    else
        top_index_c = find_next_top_fractal(kline_data_items, btm_index_b - 1);

    if( top_index_c == -1 )
    {
        end = btm_index_b;
        return;
    }

    //assert( IsTopFractal(kline_data_items[top_index_c]->type) ); 
    double price_c = kline_data_items[top_index_c]->stk_item.high_price;
    assert( !(price_c < price_b ) );

    if( price_c < price_a )// judge if C is lower than A
    { 
        if( !(price_b > lowest_price) ) // price b <= lowest_price 
        {
            lowest_price = price_b;
            end = btm_index_b;
            find_down_towardleft_end(kline_data_items, btm_index_b == top_index_c, lowest_price, top_index_c, end);
        }
        else  // price_b > lowest_price
            return;
    }else // higher than A: set B as end point
    {
        if( !(price_b > lowest_price) ) // price_b <= lowest_price
        {
            end = btm_index_b;
        }
        return;
    }
} 


void find_up_towardleft_end(std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items, bool is_src_k_same, double highest_price, int start, int &end) 
{
    if( start == 0 )
        return;
    // start is top fractal. A
    int a_date = kline_data_items[start]->stk_item.date;
    double price_a = kline_data_items[start]->stk_item.low_price;
    // 1.find next top fractal B 
    int top_index_b = -1;
    if( !is_src_k_same && IsTopFractal(kline_data_items[start]->type) ) // start is both top and btm fractal line 
        top_index_b = start;
    else
        top_index_b = find_next_top_fractal(kline_data_items, start - 1);
    if( top_index_b == -1 )
        return; 

    const double price_b = kline_data_items[top_index_b]->stk_item.high_price;

    // find next btm fractal C
    int btm_index_c = -1;
    if( top_index_b != start && IsBtmFractal(kline_data_items[top_index_b]->type) ) // start is both top and btm fractal line 
        btm_index_c = top_index_b;
    else
        btm_index_c = find_next_btm_fractal(kline_data_items, top_index_b - 1);

    if( btm_index_c == -1 )
    {
        end = top_index_b;
        return;
    }

    double price_c = kline_data_items[btm_index_c]->stk_item.low_price;
    assert( !(price_c > price_b ) );

    if( price_c > price_a )// judge if C is higher than A
    { 
        if( !(price_b < highest_price) ) // price b >= highest_price 
        {
            highest_price = price_b;
            end = top_index_b;
            find_up_towardleft_end(kline_data_items, top_index_b == btm_index_c, highest_price, btm_index_c, end);
        }
        else  // price_b < highest_price
            return;
    }else // C is lower than A: set B as end point
    {
        if( !(price_b < highest_price) ) // price_b >= highest_price
        {
            end = top_index_b;
        }
        return;
    }
} 

// ps : towards left 
void StockDataMan::TraverseGetStuctLines(PeriodType period_type, const std::string &code, std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items)
{
    auto find_next_up_struct_line_end_hight_p = [](std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items, int index, double pre_btm_price)->double
    { 
        if( kline_data_items.size() < 1 )
            return -1 * MAX_PRICE;
        int temp_index = index;
        double temp_pre_btm_price = pre_btm_price;
        while( --temp_index > 0 )
        {
            if( IsTopFractal(kline_data_items[temp_index]->type) )
            {
                int temp_end = temp_index;
                double temp_b_price = MAX_PRICE;
                int temp_b_index = find_next_btm_fractal(kline_data_items, temp_index);
                if( temp_b_index != -1 )
                {
                    temp_b_price = kline_data_items[temp_b_index]->stk_item.low_price;
                    temp_end = temp_b_index;
                } 
                find_down_towardleft_end(kline_data_items, false, temp_b_price, temp_index, temp_end);
                if( temp_index != temp_end )
                {
                    if( kline_data_items[temp_end]->stk_item.low_price < temp_pre_btm_price )
                    {  
                        return kline_data_items[temp_index]->stk_item.high_price;
                    }
                } 
            }else if( IsBtmFractal(kline_data_items[temp_index]->type) )
                temp_pre_btm_price = MAX_PRICE;
        } // while( --temp_index > 0 )
        return -1 * MAX_PRICE;
    };

    auto find_next_down_struct_line_end_low_p = [](std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items, int index, double pre_top_price)->double
    { 
        if( kline_data_items.size() < 1 )
            return -1 * MAX_PRICE;
        int temp_index = index;
        double temp_pre_top_price = pre_top_price;
        while( --temp_index > 0 )
        {
            if( IsBtmFractal(kline_data_items[temp_index]->type) )
            {
                int temp_end = temp_index;
                double temp_b_price = MIN_PRICE;
                int temp_b_index = find_next_top_fractal(kline_data_items, temp_index);
                if( temp_b_index != -1 )
                {
                    temp_b_price = kline_data_items[temp_b_index]->stk_item.high_price;
                    temp_end = temp_b_index;
                } 
                find_up_towardleft_end(kline_data_items, false, temp_b_price, temp_index, temp_end);
                if( temp_index != temp_end )
                {
                    if( kline_data_items[temp_end]->stk_item.high_price > temp_pre_top_price )
                    {  
                        return kline_data_items[temp_index]->stk_item.low_price;
                    }
                } 
            }else if( IsTopFractal(kline_data_items[temp_index]->type) )
                temp_pre_top_price = MIN_PRICE;
        } // while( --temp_index > 0 )
        return -1 * MAX_PRICE;
    };
     
    if( kline_data_items.size() < 1 )
        return;
    T_StructLineContainer &container = GetStructLineContainer(period_type, code); 
    container.clear();
    unsigned int index = kline_data_items.size();
    double pre_btm_price = MAX_PRICE;
    double pre_top_price = MIN_PRICE;
    bool is_pre_add_up_line = false;
    bool is_pre_add_down_line = false;
    // towards left 
    while( --index > 0 )
    { 
        int ck_index_date = kline_data_items[index]->stk_item.date;
        if( IsTopFractal(kline_data_items[index]->type) )
        { 
            pre_top_price = MIN_PRICE;

             int end = index; 
             double b_price = MAX_PRICE;
             int b_index = find_next_btm_fractal(kline_data_items, index);
             if( b_index != -1 )
             {
                 b_price = kline_data_items[b_index]->stk_item.low_price;
                 end = b_index;
             }
             
             find_down_towardleft_end(kline_data_items, false, b_price, index, end);
             
             if( index != end )
             {
                 bool is_to_add_line = false;
                 if( kline_data_items[end]->stk_item.low_price < pre_btm_price )
                 {
                     is_to_add_line = true;
                 }else
                 { 
                     double next_up_end_h_p = find_next_up_struct_line_end_hight_p(kline_data_items, end, kline_data_items[end]->stk_item.low_price);
                     if( next_up_end_h_p > 0.0 && next_up_end_h_p < kline_data_items[index]->stk_item.high_price )
                        is_to_add_line = true;
                 }
                 if( is_to_add_line )
                 {
                     pre_btm_price = kline_data_items[end]->stk_item.low_price;
                    /* if( !container.empty() && container[container.size()-1]->type == LineType::UP )
                     {
                         auto line_down = std::make_shared<T_StructLine>(LineType::DOWN, index, container[container.size()-1]->beg_index);
                         container.push_back(std::move(line_down));
                         is_pre_add_down_line = true;
                     }*/
                     auto line = std::make_shared<T_StructLine>(LineType::UP, end, index);
                     container.push_back(std::move(line));
                     
                 }

                 index = end + 1;
             } 
            
        }else if( IsBtmFractal(kline_data_items[index]->type) )
        {
             pre_btm_price = MAX_PRICE; 

             int end = index; 
             double b_price = MIN_PRICE;
             int b_index = find_next_top_fractal(kline_data_items, index);
             if( b_index != -1 )
             {
                 b_price = kline_data_items[b_index]->stk_item.high_price;
                 end = b_index;
             } 
             find_up_towardleft_end(kline_data_items, false, b_price, index, end);
             
             if( index != end )
             {
                 bool is_to_add_down_line = false;
                 if( kline_data_items[end]->stk_item.high_price > pre_top_price )
                 {
                     is_to_add_down_line = true;
                 }else
                 { 
                     double next_down_end_l_p = find_next_down_struct_line_end_low_p(kline_data_items, end, kline_data_items[end]->stk_item.high_price);
                     if( next_down_end_l_p > 0.0 && next_down_end_l_p > kline_data_items[index]->stk_item.low_price )
                        is_to_add_down_line = true;
                 }
                 if( is_to_add_down_line )
                 {
                     pre_top_price = kline_data_items[end]->stk_item.high_price;
                     auto line = std::make_shared<T_StructLine>(LineType::DOWN, end, index);
                     container.push_back(std::move(line));
                 }
                 index = end + 1;
             } 
        }

    } // while 
}

// ps:  TraverseGetStuctLines have to be called before 
void StockDataMan::TraversGetSections(PeriodType period_type, const std::string &code, std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items)
{
    const int cst_least_line_num = 4; // have to >= 4
    if( kline_data_items.size() < cst_least_line_num )
        return;
    
    T_StructLineContainer &struct_line_container = GetStructLineContainer(period_type, code); 
    if( struct_line_container.empty() )
        return;

    T_SectionContainer &container = GetStructSectionContainer(period_type, code); 
    container.clear();
    unsigned int index = kline_data_items.size();
    // toward left 
    // todo:---------- 
    //  set line_count = 0;
    // 1: find up struct line a, and pre down struct line b
    // 2: judge if down struct line c begin point is lower than end point of a (endp_a)
    //    true: todo : 2.1: line_count++; find next down struct line d 
    //                 2.2: if begin point of d is higher than begin point of a (begp_a)
    //                      ture : goto 2.1
    //                      false: if line_count > 1 :
    //                                2.2.1: true : 2.2.1.1: set struct up line after line d as start line 0 of section 
    //                                              2.2.1.2. set end point of line 0 as top left of setion
    //                                              2.2.1.3  add section
    //                                       false: goto 1
    //   
    int i = 0;
    for( ; i < struct_line_container.size(); ++i ){ struct_line_container[i]->type == LineType::UP; break; }
    if( i > struct_line_container.size() - cst_least_line_num )
        return;
    int line_count = 1;
    int line_0_end_index = struct_line_container[i]->end_index;
    int line_0_beg_index = struct_line_container[i]->beg_index;
    const double line_0_end_price = kline_data_items[line_0_end_index]->stk_item.high_price;
    while( false )
    {
        assert( struct_line_container[i + 1]->type == LineType::DOWN );
        assert( struct_line_container[i + 2]->type == LineType::UP ); // line c
        if( kline_data_items[struct_line_container[i + 2]->beg_index]->stk_item.low_price > line_0_end_price ) 
            break;
        line_count += 2;
        if( i + 2 + 1 < struct_line_container.size() ) // line d is last line it's down line
        {
            // get min price in ( beg_d, end_c, end_a) as top left
            // get max price in ( end_d, beg_a) as btm ; set (beg_a + end_a)/2 as right

            /*if( kline_data_items[struct_line_container[i + 2 + 1]->beg_index]->stk_item.high_price < line_0_end_price )
            {
                
            }*/
        }

        i += 3;
        if( i + 1 > struct_line_container.size() )
            break;
        //assert( struct_line_container[i + 2 + 2]->type == LineType::UP ); // line c'

    }
   
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
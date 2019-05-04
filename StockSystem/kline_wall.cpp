#include "kline_wall.h"

#include <cassert>
#include <tuple>
//#include <array>

//#include <qevent.h> 
#include <qdebug.h>
#include <qdatetime.h>

#include <TLib/core/tsystem_utility_functions.h>

#include "mainwindow.h"
#include "stkfo_common.h"
#include "stk_forecast_app.h"
#include "exchange_calendar.h"


using namespace TSystem;


void KLineWall::AppendData()
{ 
    int oldest_day = QDateTime::currentDateTime().toString("yyyyMMdd").toInt();
    if( !p_hisdata_container_->empty() )
        oldest_day = p_hisdata_container_->front()->stk_item.date;

    QDate qdate_obj(oldest_day/10000, (oldest_day%10000)/100, oldest_day%100);
    int start_date = qdate_obj.addDays( -1 * (4 * 30) ).toString("yyyyMMdd").toInt(); 
    switch(k_type_)
    {
    case TypePeriod::PERIOD_YEAR: start_date = qdate_obj.addDays( -1 * (5 * 12 * 30) ).toString("yyyyMMdd").toInt(); break;
    case TypePeriod::PERIOD_MON:
    case TypePeriod::PERIOD_WEEK: start_date = qdate_obj.addDays( -1 * (2 * 12 * 30) ).toString("yyyyMMdd").toInt(); break;
    case TypePeriod::PERIOD_DAY: start_date = qdate_obj.addDays( -1 * (1 * 12 * 30) ).toString("yyyyMMdd").toInt(); break;
    case TypePeriod::PERIOD_HOUR:start_date = qdate_obj.addDays( -1 * (6 * 30) ).toString("yyyyMMdd").toInt(); break;
    case TypePeriod::PERIOD_30M: start_date = qdate_obj.addDays( -1 * (3 * 30) ).toString("yyyyMMdd").toInt(); break;
    case TypePeriod::PERIOD_15M: start_date = qdate_obj.addDays( -1 * (1 * 30) ).toString("yyyyMMdd").toInt(); break;
    case TypePeriod::PERIOD_5M: start_date = qdate_obj.addDays( -1 * (1 * 15) ).toString("yyyyMMdd").toInt(); break;
    default: break;
    }
     
    app_->stock_data_man().AppendStockData(ToPeriodType(k_type_), stock_code_, start_date, oldest_day, is_index_);
}

void KLineWall::AppendPreData(int date)
{
    assert( date > 19800000 && date < 20500000 );

    int oldest_day = QDateTime::currentDateTime().toString("yyyyMMdd").toInt();
    if( !p_hisdata_container_->empty() )
        oldest_day = p_hisdata_container_->front()->stk_item.date;

    if( date > oldest_day )
        return; 
    app_->stock_data_man().AppendStockData(ToPeriodType(k_type_), stock_code_, date, oldest_day, is_index_);
}

void KLineWall::RestTypePeriod(TypePeriod  type)
{ 
    if( k_type_ == type )
        return;
    switch( type )
    {
    case TypePeriod::PERIOD_5M:
    case TypePeriod::PERIOD_15M:
    case TypePeriod::PERIOD_30M:
    case TypePeriod::PERIOD_HOUR:
    case TypePeriod::PERIOD_DAY:
    case TypePeriod::PERIOD_WEEK:
    case TypePeriod::PERIOD_MON:
        ResetStock(stock_code_.c_str(), type, is_index_);
        break;
    }
}

void KLineWall::UpdateKwallMinMaxPrice()
{
    std::tuple<float, float> price_tuple;
    std::tuple<int, int, int, int> date_times_tuple;
    if( GetContainerMaxMinPrice(ToPeriodType(k_type_), stock_code_, k_num_, price_tuple, date_times_tuple) )
    {
        double try_new_high = std::get<0>(price_tuple) * cst_k_mm_enlarge_times;
        if( try_new_high < this->highestMaxPrice_ || try_new_high > this->highestMaxPrice_)
            SetHighestMaxPrice(try_new_high);
        double try_new_low = std::get<1>(price_tuple) * cst_k_mm_narrow_times;
        if( try_new_low < this->lowestMinPrice_ || try_new_low > this->lowestMinPrice_)
            SetLowestMinPrice(try_new_low);
        highest_price_date_ = std::get<0>(date_times_tuple);
        highest_price_hhmm_ = std::get<1>(date_times_tuple);
        lowest_price_date_ = std::get<2>(date_times_tuple);
        lowest_price_hhmm_ = std::get<3>(date_times_tuple);
    }
}

PeriodType KLineWall::ToPeriodType(TypePeriod src)
{
    switch(src)
    {
    case TypePeriod::PERIOD_5M: return PeriodType::PERIOD_5M;
    case TypePeriod::PERIOD_15M: return PeriodType::PERIOD_15M;
    case TypePeriod::PERIOD_30M: return PeriodType::PERIOD_30M;
    case TypePeriod::PERIOD_HOUR: return PeriodType::PERIOD_HOUR;
    case TypePeriod::PERIOD_DAY: return PeriodType::PERIOD_DAY;
    case TypePeriod::PERIOD_WEEK: return PeriodType::PERIOD_WEEK;
    case TypePeriod::PERIOD_MON: return PeriodType::PERIOD_MON;
        assert(false); 
    }
    return PeriodType::PERIOD_DAY;
}

double KLineWall::GetCurWinKLargetstVol()
{
    double largest_vol = 0.0;
    int k = k_num_;
    for( auto iter = p_hisdata_container_->rbegin() + k_rend_index_;
        iter != p_hisdata_container_->rend() && k > 0; 
        ++iter, --k)
        if( (*iter)->stk_item.vol > largest_vol ) 
            largest_vol = (*iter)->stk_item.vol;
    return largest_vol;
}

T_KlineDataItem * KLineWall::GetKLineDataItemByDate(int date, int hhmm)
{ 
    for( auto iter = p_hisdata_container_->rbegin();
        iter != p_hisdata_container_->rend(); 
        ++iter )
    {   
        if( iter->get()->stk_item.date == date && iter->get()->stk_item.hhmmss == hhmm )
            return iter->get(); 
    }
    return nullptr;
}

// dates : tuple<highest related date, highest related hhmm, lowest related date, lowest related hhmm>
bool KLineWall::GetContainerMaxMinPrice(PeriodType period_type, const std::string& code, int k_num, std::tuple<float, float>& ret, std::tuple<int, int, int, int> &date_times)
{
    T_HisDataItemContainer &container = app_->stock_data_man().GetHisDataContainer(period_type, code);
    if( container.empty() )
        return false;
    assert( container.size() > k_rend_index_ );

    unsigned int start_index = container.size() - k_rend_index_ > k_num ? (container.size() - k_rend_index_ - k_num) : 0; 
    if( start_index == container.size() )
        start_index = container.size() - 1;
    unsigned int end_index = container.size() - 1 > 0 ? container.size() - 1 - k_rend_index_ : 0;
    if( end_index < 0 )
        end_index = 0;
    float highest_price = MIN_PRICE;
    float lowest_price = MAX_PRICE;
    int highest_price_date = 0;
    int highest_price_hhmm = 0;
    int lowest_price_date = 0;
    int lowest_price_hhmm = 0;
    for( unsigned int i = start_index; i <= end_index; ++ i )
    {
        if( container.at(i)->stk_item.high_price > highest_price )
        {
            highest_price = container.at(i)->stk_item.high_price; 
            highest_price_date = container.at(i)->stk_item.date;
            highest_price_hhmm = container.at(i)->stk_item.hhmmss;
        }
        if( container.at(i)->stk_item.low_price < lowest_price )
        {
            lowest_price = container.at(i)->stk_item.low_price;
            lowest_price_date = container.at(i)->stk_item.date;
            lowest_price_hhmm = container.at(i)->stk_item.hhmmss;
        }
    } 

    double tmp_price = forcast_man_.FindMaxForcastPrice(code, ToTypePeriod(period_type), container.at(start_index)->stk_item.date, container.at(end_index)->stk_item.date);
    if( tmp_price > highest_price )
        highest_price = tmp_price;
    tmp_price = forcast_man_.FindMinForcastPrice(code, ToTypePeriod(period_type), container.at(start_index)->stk_item.date, container.at(end_index)->stk_item.date);
    if( tmp_price < lowest_price )
        lowest_price = tmp_price;

    ret = std::make_tuple(highest_price, lowest_price);
    date_times = std::make_tuple(highest_price_date, highest_price_hhmm, lowest_price_date, lowest_price_hhmm);
    return true;
}

// ps : contain iter
int KLineWall::FindTopItem_TowardLeft(T_HisDataItemContainer &his_data, T_HisDataItemContainer::reverse_iterator iter, int k_index, T_KlinePosData *&left_pos_data)
{
    auto left_tgt_iter = iter;
    int cp_j = k_index;
    for( ; left_tgt_iter != his_data.rend() && cp_j > 0; 
        ++left_tgt_iter, --cp_j)
    {
        int type = (*left_tgt_iter)->type;
        auto left_frac_type = MaxFractalType(type);
        if( left_frac_type >= FractalType::TOP_AXIS_T_3 )
            break;
    }
    if( left_tgt_iter != his_data.rend() && cp_j > 0 )
    {
        left_pos_data = std::addressof(left_tgt_iter->get()->kline_posdata(wall_index_));
        return cp_j;
    }else
        return 0;
}

// ps : contain iter
int KLineWall::FindTopFakeItem_TowardLeft(T_HisDataItemContainer &his_data, T_HisDataItemContainer::reverse_iterator iter, int k_index, T_KlinePosData *&left_pos_data)
{
    auto left_tgt_iter = iter;
    int cp_j = k_index - 1;
    for( ; left_tgt_iter != his_data.rend() && cp_j > 0; 
        ++left_tgt_iter, --cp_j)
    { 
        if( (*left_tgt_iter)->type == (int)FractalType::TOP_FAKE )
            break;
    }
    if( left_tgt_iter != his_data.rend() && cp_j > 0 )
    {
        left_pos_data = std::addressof(left_tgt_iter->get()->kline_posdata(wall_index_));
        return cp_j;
    }else
        return 0;
}

// ps : not contain iter
int KLineWall::FindBtmItem_TowardLeft(T_HisDataItemContainer &his_data, T_HisDataItemContainer::reverse_iterator iter, int k_index, T_KlinePosData *&left_pos_data)
{
    auto left_tgt_iter = iter + 1;
    int cp_j = k_index - 1;
    for( ; left_tgt_iter != his_data.rend() && cp_j > 0; 
        ++left_tgt_iter, --cp_j) // find left btm_axis_iter 
    {
        auto left_frac_type = BtmestFractalType((*left_tgt_iter)->type);
        if( left_frac_type != FractalType::UNKNOW_FRACTAL )
            break;
    }

    if( left_tgt_iter != his_data.rend() && cp_j > 0 )
    {
        left_pos_data = std::addressof(left_tgt_iter->get()->kline_posdata(wall_index_));
        return cp_j;
    }else
        return 0;
}

// ps : contain iter
int KLineWall::FindBtmFakeItem_TowardLeft(T_HisDataItemContainer &his_data, T_HisDataItemContainer::reverse_iterator iter, int k_index, T_KlinePosData *&left_pos_data)
{
    auto left_tgt_iter = iter;
    int cp_j = k_index - 1;
    for( ; left_tgt_iter != his_data.rend() && cp_j > 0; 
        ++left_tgt_iter, --cp_j) // find left btm_axis_iter 
    {
        if( (*left_tgt_iter)->type == (int)FractalType::BTM_FAKE )
            break;
    }

    if( left_tgt_iter != his_data.rend() && cp_j > 0 )
    {
        left_pos_data = std::addressof(left_tgt_iter->get()->kline_posdata(wall_index_));
        return cp_j;
    }else
        return 0;
}

int KLineWall::Calculate_k_mm_h()
{
    int mm_h = this->height() - HeadHeight() - BottomHeight();
    for( unsigned int i = 0 ; i < zb_windows_.size(); ++i )
    {
        if( zb_windows_[i] )
            mm_h -= zb_windows_[i]->Height();
    }
    return mm_h;
}

void KLineWall::ClearForcastData()
{ 
    auto iter = forcast_man_.Find2pDownForcastVector(stock_code_, k_type_);
    if( iter )
        iter->clear();
    auto iter0 = forcast_man_.Find2pUpForcastVector(stock_code_, k_type_);
    if( iter0 )
        iter0->clear();
    auto iter_3pdown_vector = forcast_man_.Find3pForcastVector(stock_code_, k_type_, true);
    if( iter_3pdown_vector )
        iter_3pdown_vector->clear();
    auto iter_3pup_vector = forcast_man_.Find3pForcastVector(stock_code_, k_type_, false);
    if( iter_3pup_vector )
        iter_3pup_vector->clear();
}


// ret: <date, hhmm>
std::tuple<int, int> GetKDataTargetDateTime(TypePeriod type_period, QDate date, QTime time)
{
    static auto get_hhmm = [](int hhmm_para, int *tp_array, int num)->int
    {
        assert(num > 0);
        for( int i = 0; i < num; ++i )
        {
            if( hhmm_para <= tp_array[i] ) 
                return tp_array[i];
        }
        return tp_array[num-1];
    };

    // 20 k line per 30 days
    int start_date = 0;
    int span_day = 0;
    int hhmm = 0; 
    int tmp_hhmm = time.hour() * 100 + time.minute();

    //std::array<int, 4> hour_array = {1030, 1300, 1400, 1500};
    switch( type_period )
    {
    case TypePeriod::PERIOD_YEAR: 
    case TypePeriod::PERIOD_MON:
        span_day = -1 * 365 * 10;
        break;
    case TypePeriod::PERIOD_DAY:
    case TypePeriod::PERIOD_WEEK:
        span_day = -1 * (WOKRPLACE_DEFUALT_K_NUM * 30 / 20);
        break;
    case TypePeriod::PERIOD_HOUR:  // ndchk 
        {
            span_day = -1 * (WOKRPLACE_DEFUALT_K_NUM * 30 / (20 * 4));
            //10:30 13:00 14:00 15:00
            int tp_array[] = { 1030, 1300, 1400, 1500 };
            hhmm = get_hhmm(tmp_hhmm, tp_array, sizeof(tp_array)/sizeof(tp_array[0]));
            break;
        }
    case TypePeriod::PERIOD_30M:
        {
            span_day = -1 * (WOKRPLACE_DEFUALT_K_NUM * 30 / (20 * 4 * 2)); 
            int tp_array[] = { 1000, 1030, 1100, 1130, 1330, 1400, 1430, 1500 };
            hhmm = get_hhmm(tmp_hhmm, tp_array, sizeof(tp_array)/sizeof(tp_array[0]));
            break;
        }
    case TypePeriod::PERIOD_15M:
        {
            span_day = -1 * (WOKRPLACE_DEFUALT_K_NUM * 30 / (20 * 4 * 2 * 2));  
            int tp_array[] = { 945, 1000, 1015, 1030, 1045, 1100, 1115, 1130, 1315, 1330, 1345, 1400, 1415, 1430, 1445, 1500};
            hhmm = get_hhmm(tmp_hhmm, tp_array, sizeof(tp_array)/sizeof(tp_array[0]));
            break;
        }
    case TypePeriod::PERIOD_5M:
        {
            span_day = -1 * (WOKRPLACE_DEFUALT_K_NUM * 30 / (20 * 4 * 2 * 2 * 3));
            int tp_array[] = {935,940,945,950,955,1000,1005,1010,1015,1020,1025,1030,1035,1040,1045,1050,1055,1100,1105
                ,1110,1115,1120,1125,1130,1305,1310,1315,1320,1325,1330,1335,1340,1345,1350,1355,1400,1405
                ,1410,1415,1420,1425,1430,1435,1440,1445,1450,1455,1500};
            hhmm = get_hhmm(tmp_hhmm, tp_array, sizeof(tp_array)/sizeof(tp_array[0]));
            break;
        }
    }
    start_date = date.addDays(span_day).toString("yyyyMMdd").toInt();
    return std::make_tuple(start_date, hhmm);
}

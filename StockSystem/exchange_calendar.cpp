#include "exchange_calendar.h"

#include <ctime>
#include <chrono>
#include <TLib/core/tsystem_time.h>
 
ExchangeCalendar::ExchangeCalendar() : min_trade_date_(0), max_trade_date_(0)
{ 
    trade_dates_ = std::make_shared<T_DateMapIsopen>(10*1024);
}

bool ExchangeCalendar::IsTradeDate(int date)
{
     assert(trade_dates_->size() > 0);

     T_DateMapIsopen &date_map_opend = *trade_dates_;
     auto iter = date_map_opend.find(date);
     return iter != date_map_opend.end() && iter->second;
}

bool ExchangeCalendar::IsTradeTime(int hhmm)
{
    return hhmm >= 915 &&  hhmm <= 1500;
}

// ps: ceiling trade date may be bigger then param date. if fail return 0
int ExchangeCalendar::CeilingTradeDate(int date)
{
    assert(trade_dates_->size() > 0); 
    T_DateMapIsopen &date_map_opend = *trade_dates_;
    int a = 0;
    for( int i = 0; i < 30; ++i )
    {
        a = DateAddDays(date, i);
        if( a > max_trade_date_ )
            return 0;
        auto iter = date_map_opend.find(a);
        if( iter != date_map_opend.end() && iter->second )
           return a;
    }
    return 0;
}

// ps: ceiling trade date may be smaller then param date. if fail return 0
int ExchangeCalendar::FloorTradeDate(int date)
{
    assert(trade_dates_->size() > 0); 
    T_DateMapIsopen &date_map_opend = *trade_dates_;
    int a = 0;
    for( int i = 0; i < 30; ++i )
    {
        a = DateAddDays(date, -1 * i);
        if( a < min_trade_date_ )
            return 0;
        auto iter = date_map_opend.find(a);
        if( iter != date_map_opend.end() && iter->second )
            return a;
    }
    return 0;
}

// pre n trade date 
int ExchangeCalendar::PreTradeDate(int date, unsigned int n)
{   
    assert(trade_dates_->size() > 0);
    // auto date_time_point = TSystem::MakeTimePoint(date/10000, (date % 10000) / 100, date % 100);
    unsigned int count = 0;
    int i = 1;
    T_DateMapIsopen &date_map_opend = *trade_dates_;
    int a = 0;
    while( count < n )
    {
        a = DateAddDays(date, -1 * i);  
        if( a < min_trade_date_ )
            return 0;
        auto iter = date_map_opend.find(a);
        if( iter != date_map_opend.end() && iter->second )
            ++count;
        ++i;
    }
    return a;
}

int ExchangeCalendar::NextTradeDate(int date, unsigned int n)
{   
    assert(trade_dates_->size() > 0);
    //auto date_time_point = TSystem::MakeTimePoint(date/10000, (date % 10000) / 100, date % 100);
    unsigned int count = 0;
    int i = 1;
    T_DateMapIsopen &date_map_opend = *trade_dates_;
    int a = 0;
    while( count < n )
    {
        a = DateAddDays(date, i);  
        if( a > max_trade_date_ )
            return 0;
        auto iter = date_map_opend.find(a);
        if( iter != date_map_opend.end() && iter->second )
            ++count;
        ++i;
    }
    return a;
}

// ps: end_date <= today . ndedt
T_TupleIndexLen ExchangeCalendar::GetStartIndexAndLen_backforward(TypePeriod type_period, int start_date, int end_date)
{
    assert(trade_dates_->size() > 0);
    assert(start_date <= end_date);

    time_t rawtime;
    struct tm * timeinfo;
    time( &rawtime );
    timeinfo = localtime( &rawtime ); 
    int today = TSystem::Today();
    if( timeinfo->tm_hour * 100 + timeinfo->tm_min < 930 )
    {
        today = DateAddDays(today, -1);
    }
    const int latest_trade_date = FloorTradeDate(today);

    int actual_start_date = CeilingTradeDate(start_date);

    int actual_end_date = end_date;
    if( actual_end_date >= today )
        actual_end_date = today;
    actual_end_date = FloorTradeDate(actual_end_date);

    int start_index = DateTradingSpan(actual_end_date, latest_trade_date);

    int span_len = DateTradingSpan(actual_start_date, actual_end_date) + 1;
     
    switch( type_period )
    {
    case TypePeriod::PERIOD_DAY: break;
    case TypePeriod::PERIOD_WEEK:
        if( start_index > 5 )
            start_index /= 5;
        if( span_len > 5 )
            span_len /= 5;
        break;
    case TypePeriod::PERIOD_HOUR:
        start_index *= 4;
        span_len *= 4;
        break;
    case TypePeriod::PERIOD_30M:
        start_index *= 8;
        span_len *= 8;
        break;
    case TypePeriod::PERIOD_15M:
        start_index *= 16;
        span_len *= 16;
        break;
    case TypePeriod::PERIOD_5M:
        start_index *= 16*3;
        span_len *= 16*3;
        break;
    }
    return std::make_tuple(start_index, span_len);
}

// return span of trading dates between
// ps: start_date <= end_date
int  ExchangeCalendar::DateTradingSpan(int start_date, int end_date)
{
    assert(start_date <= end_date);

    if( start_date == end_date ) 
        return 0;
    int target_end_date = end_date;
    if( !IsTradeDate(target_end_date) )
        target_end_date = PreTradeDate(target_end_date, 1);

    int target_start_date = start_date;
    if( !IsTradeDate(target_start_date) )
        target_start_date = NextTradeDate(target_start_date, 1);
    if( target_end_date <= target_start_date ) 
        return 0;

    T_DateMapIsopen &date_map_opend = *trade_dates_;
    int tmp_date = 0;
    unsigned int span = 0;
    int i = 1;
    do{
        tmp_date = DateAddDays(target_end_date, -1 * i++);  
        auto iter = date_map_opend.find(tmp_date);
        if( iter != date_map_opend.end() && iter->second )
            ++span;
    }while( target_start_date < tmp_date );

    return span;
}

int ExchangeCalendar::TodayAddDays(int days)
{
    std::time_t day_t = 0;
    using namespace std::chrono;
    system_clock::time_point now = system_clock::now();
    if( days >= 0 )
        day_t = system_clock::to_time_t(now + std::chrono::hours(24*days));
    else
        day_t = system_clock::to_time_t(now - std::chrono::hours(24*abs(days)));
    tm tm_day_t;
    _localtime64_s(&tm_day_t, &day_t);
    return (tm_day_t.tm_year + 1900) * 10000 + (tm_day_t.tm_mon + 1) * 100 + tm_day_t.tm_mday;
}

int ExchangeCalendar::DateAddDays(int date, int days)
{
    auto date_time_point = TSystem::MakeTimePoint(date/10000, (date % 10000) / 100, date % 100);
    std::time_t day_t = 0;

    using namespace std::chrono;
    if( days >= 0 )
        day_t = system_clock::to_time_t(date_time_point + std::chrono::hours(24*days));
    else
        day_t = system_clock::to_time_t(date_time_point - std::chrono::hours(24*abs(days)));
    tm tm_day_t;
    _localtime64_s(&tm_day_t, &day_t);
    return (tm_day_t.tm_year + 1900) * 10000 + (tm_day_t.tm_mon + 1) * 100 + tm_day_t.tm_mday;
}
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
     return date_map_opend.find(date) != date_map_opend.end();
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

// ps: end_date <= today
T_TupleIndexLen ExchangeCalendar::GetStartDateAndLen_backforward(int start_date, int end_date)
{
    assert(trade_dates_->size() > 0);
    assert(start_date <= end_date);

    const int today = TSystem::Today();
    int actual_end_date = end_date;
    if( actual_end_date >= today )
        actual_end_date = today;
    int start_index = DateTradingSpan(actual_end_date, today);
    int len = DateTradingSpan(start_date, actual_end_date);
    return std::make_tuple(start_index, len + 1);
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
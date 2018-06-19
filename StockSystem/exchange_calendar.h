#ifndef EXCHANGE_CALENDAR_SDF3SDFS_H_
#define EXCHANGE_CALENDAR_SDF3SDFS_H_

#include <vector>
#include <memory>
#include <unordered_map>

//#include "common_base.h"
 
typedef std::unordered_map<int, bool> T_DateMapIsopen ;

class ExchangeCalendar
{
public:

	 // return: yyyymmdd
    static int TodayAddDays(int days=0);
    static int DateAddDays(int date, int days);

    ExchangeCalendar();

    bool IsTradeDate(int date);
    int PreTradeDate(int date, unsigned int n);
    int NextTradeDate(int date, unsigned int n);
	 
private:

    std::shared_ptr<T_DateMapIsopen> trade_dates_;

    //std::vector<T_CalendarDate>  calendar_date_;
    int min_trade_date_;
    int max_trade_date_;
    friend class DBMoudle;
};

#endif
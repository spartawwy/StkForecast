#ifndef TDX_HQ_WRAPPER_H_SDFDS34FFSDDS
#define TDX_HQ_WRAPPER_H_SDFDS34FFSDDS

//#include <memory>
#include <vector>
#include "stkfo_common.h"

class ExchangeCalendar;
class TdxHqWrapper
{
public:
    TdxHqWrapper(ExchangeCalendar  *exchange_calendar);
    ~TdxHqWrapper();

    bool Init();
    bool ConnectServer();
    bool GetHisKBars(const std::string &code, bool is_index, TypePeriod kbar_type, int start_date, int end_date, std::vector<T_StockHisDataItem> &items);

private:

    int conn_handle_;
    ExchangeCalendar  *exchange_calendar_;
};

#endif
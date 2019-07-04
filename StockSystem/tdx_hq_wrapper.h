#ifndef TDX_HQ_WRAPPER_H_SDFDS34FFSDDS
#define TDX_HQ_WRAPPER_H_SDFDS34FFSDDS

//#include <memory>
#include <vector>
#include <mutex>

#include "stkfo_common.h"

class ExchangeCalendar;
class TdxHqWrapper
{
public:
    TdxHqWrapper(ExchangeCalendar  *exchange_calendar);
    ~TdxHqWrapper();

    bool Init();
    
    bool GetHisKBars(const std::string &code, bool is_index, TypePeriod kbar_type, int start_date, int end_date, std::vector<T_StockHisDataItem> &items);

    bool GetLatestKBar(const std::string &code, bool is_index, TypePeriod kbar_type, T_StockHisDataItem &item);

private:

    bool __GetHisKBars(const std::string &code, bool is_index, TypePeriod kbar_type, short start, short &count, std::vector<T_StockHisDataItem> &items);

    bool ConnectServer();
    bool ReconnectServer();
    bool IsConnhandleValid();

    int _ConnectServer();

    int conn_handle_;
    std::mutex  conn_handle_mutex_;

    ExchangeCalendar  *exchange_calendar_;
};

#define  MAX_K_COUNT 800 // 虽然(TdxHq_GetSecurityBars TdxHq_GetIndexBars)接口说明最大值 800, 但实际测试为 420
#endif
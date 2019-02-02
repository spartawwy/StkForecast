#ifndef TDX_HQ_WRAPPER_H_SDFDS34FFSDDS
#define TDX_HQ_WRAPPER_H_SDFDS34FFSDDS

#include "stkfo_common.h"

class TdxHqWrapper
{
public:
    TdxHqWrapper();
    ~TdxHqWrapper();

    bool Init();

    int GetHisKBars(const std::string &code, TypePeriod kbar_type, int start_date, int end_date);

private:

    int conn_handle_;
};

#endif
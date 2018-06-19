#include "stdafx.h"

#include <iostream>
#include <memory>
#include <unordered_map>

#ifdef TEST_TSL_HQAPI

#include "tls_hq_api.h"

#pragma comment(lib, "TlsHqApi.lib")

typedef std::unordered_map<__int64, std::shared_ptr<T_BarDataItem> >  T_Time_BarData_Map;

void GetBarDataCallback(void* p_unorder_map, T_BarDataItem* p_input)
{
   assert(p_input);
   T_Time_BarData_Map * holder = (T_Time_BarData_Map*)p_unorder_map;
   __int64 date_time = (__int64)p_input->date * 1000000 + (__int64)p_input->time;

   auto data_item = std::make_shared<T_BarDataItem>();
   *data_item = *p_input;
   holder->insert(std::make_pair(date_time, std::move(data_item)));

}

int main(void ) 
{
    // (time val, T_BarDataItem)
    std::unordered_map<__int64, std::shared_ptr<T_BarDataItem> >  bars_dataitem_15min_holder(1024*10);

    // get 140 
    int ret = TlsHq_Initialize(true, true);
	if( ret <= 0 )
	{
		std::cout << "TlsHq_Initialize fail!";
		getchar();
		return 0;
	}
    char error[1024] = {0};
    T_BarDataItem *p_bar_data_item = nullptr;

    short start = 0;
    short count = 100;
    int val  = 0;
    TlsHq_GetDarDataItemsUseCallBack( KLINE_CATEGORY_15MIN,
        MARKET_SH,
        "600196",
        start,
        &count,
        GetBarDataCallback,
        &bars_dataitem_15min_holder,
        error);

    TlsHq_GetDarDataItemsUseCallBack( KLINE_CATEGORY_15MIN,
        MARKET_SH,
        "600196",
        start,
        &count,
        GetBarDataCallback,
        &bars_dataitem_15min_holder,
        error);

#if 0
    val = TlsHq_GetDarDataItems(
                            KLINE_CATEGORY_15MIN,
                            MARKET_SH,
                            "600196",
                            start,
                            &count,
                            &p_bar_data_item,
                            error);
    delete[] p_bar_data_item;
#endif
    getchar();
    return 0;
}
#endif
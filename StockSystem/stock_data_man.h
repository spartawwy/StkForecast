#ifndef STOCK_DATA_MAN_H
#define STOCK_DATA_MAN_H

//#include <list>
//#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
 
#include "stkfo_common.h"

#include "stockdayinfo.h"

#include "stk_quoter_api.h"
 
#include "winner_hq_api.h"
  
#include "tdx_hq_wrapper.h"
 

TypePeriod ToTypePeriod(PeriodType src);

class ExchangeCalendar;
class StockDataMan
{
public:

    StockDataMan(ExchangeCalendar *p_exchange_calendar);
    ~StockDataMan();
    bool Init();

    //std::vector<std::shared_ptr<T_KlineDataItem> > &day_kline_data_container() { return day_kline_data_container_; }
    ExchangeCalendar * exchange_calendar() { return p_exchange_calendar_;}
public:

    //从fileName指定的磁盘路径中将数据一行一行读取出来，每一行初始化一个StockDayInfo对象
    //void LoadDataFromFile(std::string &fileName);

    T_HisDataItemContainer* FindStockData(PeriodType period_type, const std::string &stk_code, int start_date, int end_date, int cur_hhmm, bool is_index=false);
    T_HisDataItemContainer* AppendStockData(PeriodType period_type, const std::string &stk_code, int start_date, int end_date, bool is_index=false);
	     
    bool UpdateLatestItemStockData(PeriodType period_type, const std::string &stk_code, bool is_index=false);
    void TraverseGetBi(PeriodType period_type, const std::string &code, std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items);

    void TraverseGetStuctLines(PeriodType period_type, const std::string &code, std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items);
    void TraversGetSections(PeriodType period_type, const std::string &code, std::deque<std::shared_ptr<T_KlineDataItem> > &kline_data_items);

public:
      
	float GetHisDataLowestMinPrice(PeriodType period_type, const std::string& code, int start_date, int end_date);
	float GetHisDataHighestMaxPrice(PeriodType period_type, const std::string& code, int start_date, int end_date);

public: 

    T_HisDataItemContainer &GetHisDataContainer(PeriodType period_type, const std::string& code);
    T_BiContainer &GetBiContainer(PeriodType period_type, const std::string& code);
    T_StructLineContainer &GetStructLineContainer(PeriodType period_type, const std::string& code);
    T_SectionContainer &GetStructSectionContainer(PeriodType period_type, const std::string& code/*, int wall_index*/);

    // (stock , data)  date is from small to big
    T_CodeMapHisDataItemContainer m5_stock_his_items_;
    T_CodeMapHisDataItemContainer m15_stock_his_items_;
    T_CodeMapHisDataItemContainer m30_stock_his_items_;
    T_CodeMapHisDataItemContainer hour_stock_his_items_;
    T_CodeMapHisDataItemContainer day_stock_his_items_;
    T_CodeMapHisDataItemContainer week_stock_his_items_;
    T_CodeMapHisDataItemContainer mon_stock_his_items_;

    T_CodeMapBiContainer m5_stock_bi_items_;
    T_CodeMapBiContainer m15_stock_bi_items_;
    T_CodeMapBiContainer m30_stock_bi_items_;
    T_CodeMapBiContainer hour_stock_bi_items_;
    T_CodeMapBiContainer day_stock_bi_items_;
    T_CodeMapBiContainer week_stock_bi_items_;
    T_CodeMapBiContainer mon_stock_bi_items_;

    T_CodeMapStructDataContainer m5_stock_struct_datas_;
    T_CodeMapStructDataContainer m15_stock_struct_datas_;
    T_CodeMapStructDataContainer m30_stock_struct_datas_;
    T_CodeMapStructDataContainer hour_stock_struct_datas_;
    T_CodeMapStructDataContainer day_stock_struct_datas_;
    T_CodeMapStructDataContainer week_stock_struct_datas_;
    T_CodeMapStructDataContainer mon_stock_struct_datas_;

#ifndef USE_STK_QUOTER
    std::vector<T_StockHisDataItem> *p_stk_hisdata_item_vector_;
    bool is_fetched_stk_hisdata_;
#endif 
private:

    std::tuple<int, int> GetDateIndexFromContainer(PeriodType period_type, const std::string& stock, int start_date, int end_date);
    void CaculateZhibiao(T_HisDataItemContainer &data_items_in_container);

private:
#ifdef USE_STK_QUOTER
    StkHisDataDelegate stk_his_data_;
    StkRelHisDataDelegate stk_hisdata_release_;
#elif defined(USE_WINNER_API)
    WinnerHisHq_ConnectDelegate WinnerHisHq_Connect_;
    WinnerHisHq_DisconnectDelegate WinnerHisHq_DisConnect_;
    WinnerHisHq_GetKDataDelegate  WinnerHisHq_GetKData_;
    T_KDataCallBack  call_back_obj_;

#elif defined(USE_TDXHQ)
    TdxHqWrapper  tdx_hq_wrapper_;
#endif
     
#ifdef USE_PYTHON_QUOTE
    std::shared_ptr<PyDataMan> py_data_man_;
#endif
  
   std::vector<ZhibiaoType> zhibiao_types_;
   ExchangeCalendar *p_exchange_calendar_;
};

// < 0 : meaning no related data
int FindDataIndex(T_HisDataItemContainer &data_items_in_container, int date, int cur_hhmm);

#endif // STOCK_DATA_MAN_H

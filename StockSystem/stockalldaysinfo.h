#ifndef STOCKALLDAYSINFO_H
#define STOCKALLDAYSINFO_H

#include <list>
#include <iostream>
#include <memory>
#include <map>
#include <vector>

#include <QtCore/QPoint>

#include "stockdayinfo.h"
#include "stk_quoter_api.h"

#define UNKNOW_FRACTAL   0
#define BTM_AXIS_T_3     0x00000001
#define BTM_AXIS_T_5     0x00000002
#define BTM_AXIS_T_7     0x00000004
#define BTM_AXIS_T_9     0x00000008
#define BTM_AXIS_T_11    0x00000010

#define TOP_AXIS_T_3     0x00000020
#define TOP_AXIS_T_5     0x00000040
#define TOP_AXIS_T_7     0x00000080
#define TOP_AXIS_T_9     0x00000100
#define TOP_AXIS_T_11    0x00000200

#define UPWARD_FRACTAL   0x10000000
#define DOWNWARD_FRACTAL 0x20000000
#define INSUFFIC_FRACTAL 0x40000000  
 
#define CST_MAGIC_POINT QPoint(-1, -1)
  
typedef struct _t_kline_pos_data
{
    int  date; 
    int x_left;
    int x_right;
    int height;
    QPoint columnar_top_left;
    QPoint top;
    QPoint bottom;
    _t_kline_pos_data() : date(0), x_left(-1), x_right(-1), height(-1), columnar_top_left(CST_MAGIC_POINT), top(CST_MAGIC_POINT), bottom(CST_MAGIC_POINT) {}
}T_KlinePosData;

typedef struct _t_kline_dataitem
{
    T_StockHisDataItem  stk_item;
    T_KlinePosData  kline_posdata;
    int  type;
    _t_kline_dataitem() : type(UNKNOW_FRACTAL), kline_posdata()
    {
        memset(&stk_item, 0, sizeof(stk_item));
    }
    explicit _t_kline_dataitem(const _t_kline_dataitem & lh)
    {
        memcpy(this, &lh, sizeof(lh)); 
    }
    explicit _t_kline_dataitem(const T_StockHisDataItem & stock_his_data_item)
    {
        memcpy(&stk_item, &stock_his_data_item, sizeof(stock_his_data_item)); 
    }
}T_KlineDataItem;

//typedef std::list<T_StockHisDataItem>  T_HisDataItemList;
typedef std::vector<std::shared_ptr<T_KlineDataItem> >  T_HisDataItemContainer;

class StockAllDaysInfo
{
public:

    StockAllDaysInfo();
    bool Init();

public:
    //list容器，数据类型为一只股票一天的消息，是StockAllDaysInfo的数据成员
    //std::list<StockDayInfo> stockAllDaysInfoList;
    std::vector<std::shared_ptr<T_KlineDataItem> > day_kline_data_container_;

    //从fileName指定的磁盘路径中将数据一行一行读取出来，每一行初始化一个StockDayInfo对象
    void LoadDataFromFile(std::string &fileName);

    T_HisDataItemContainer* LoadStockData(const std::string &stk_code, int start_date, int end_date);
	     
public:

    float GetLowestMinPrice();
    float GetHighestMaxPrice();

	float GetHisDataLowestMinPrice(const std::string& stock);
	float GetHisDataHighestMaxPrice(const std::string& stock);

public:
    //std::list<StockDayInfo> GetStockAllDaysInfoList();
    //std::map<std::string,  std::list<StockDayInfo> > stock_days_info_;

    // (stock , data)
    std::map<std::string, T_HisDataItemContainer> stock_his_items_;

private:
    StkHisDataDelegate stk_his_data_;
    StkRelHisDataDelegate stk_hisdata_release_;

};

#endif // STOCKALLDAYSINFO_H

#ifndef  STKFO_COMMON_SDF3DSF_H_
#define  STKFO_COMMON_SDF3DSF_H_

#include <string>
#include <QString>

#include <QtCore/QPoint>

#include "stk_quoter_api.h"

//#define UNKNOW_FRACTAL   0
//#define BTM_AXIS_T_3     0x00000001
//#define BTM_AXIS_T_5     0x00000002
//#define BTM_AXIS_T_7     0x00000004
//#define BTM_AXIS_T_9     0x00000008
//#define BTM_AXIS_T_11    0x00000010
//
//#define TOP_AXIS_T_3     0x00000020
//#define TOP_AXIS_T_5     0x00000040
//#define TOP_AXIS_T_7     0x00000080
//#define TOP_AXIS_T_9     0x00000100
//#define TOP_AXIS_T_11    0x00000200
enum class FractalType : int
{
 UNKNOW_FRACTAL  = 0,
 BTM_AXIS_T_3    = 0x00000001,
 BTM_AXIS_T_5    = 0x00000002,
 BTM_AXIS_T_7    = 0x00000004,
 BTM_AXIS_T_9    = 0x00000008,
 BTM_AXIS_T_11   = 0x00000010,

 TOP_AXIS_T_3    = 0x00000020,
 TOP_AXIS_T_5    = 0x00000040,
 TOP_AXIS_T_7    = 0x00000080,
 TOP_AXIS_T_9    = 0x00000100,
 TOP_AXIS_T_11   = 0x00000200,
};

#define UPWARD_FRACTAL   0x10000000
#define DOWNWARD_FRACTAL 0x20000000
#define INSUFFIC_FRACTAL 0x40000000  

//#define CST_MAGIC_POINT QPoint(-1, -1)
#define CST_MAGIC_POINT QPointF(-1, -1)
#define MAX_PRICE 100000000.0f
#define MIN_PRICE 0.0f
//enum class KLineType : unsigned char
//{
//    KLINE_DAY = 0,
//    KLINE_60MIN,
//    KLINE_30MIN, 
//};

enum class TypePeriod : unsigned char
{
    PERIOD_5M = 1,
    PERIOD_15M,
    PERIOD_30M,
    PERIOD_HOUR,
    PERIOD_DAY,
    PERIOD_WEEK,
    PERIOD_MON,
};


class PyDataMan;
class T_KlinePosData
{
public:
    T_KlinePosData() : date(0), x_left(0.0), x_right(0.0), height(0.0), columnar_top_left(CST_MAGIC_POINT), top(CST_MAGIC_POINT), bottom(CST_MAGIC_POINT) {}
    void Clear(){date = 0; x_left = 0.0; x_right = 0.0; height = 0.0; columnar_top_left = CST_MAGIC_POINT;top = CST_MAGIC_POINT; bottom = CST_MAGIC_POINT;}
    int  date; 
    double x_left;
    double x_right;
    double height;
    QPointF columnar_top_left;
    QPointF top;
    QPointF bottom;

};

typedef struct _t_kline_dataitem
{
    T_StockHisDataItem  stk_item;
    T_KlinePosData  kline_posdata;
    int  type;
    _t_kline_dataitem() : type(int(FractalType::UNKNOW_FRACTAL)), kline_posdata()
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

class T_StockBaseInfoItem
{ 
public:
    T_StockBaseInfoItem() : time_to_market(0){}
    std::string code;
    std::string pinyin;
    std::string name;
    int  time_to_market;
    std::string industry;
    std::string area;
    std::string remark;
};

enum class DrawAction : unsigned char 
{ 
    DRAWING_FOR_2PDOWN_C = 1, 
    DRAWING_FOR_2PUP_C, 
    DRAWING_FOR_3PDOWN_D, 
    DRAWING_FOR_3PUP_D, 
    NO_ACTION = 255
};

bool IsNumber(const std::string& str);

std::string TransIndexPinYin2Code(const std::string &code);
std::string TransIndex2TusharedCode(const std::string &code);

FractalType  MaxFractalType(int val);

#endif // STKFO_COMMON_SDF3DSF_H_
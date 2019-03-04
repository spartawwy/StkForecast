#ifndef  STKFO_COMMON_SDF3DSF_H_
#define  STKFO_COMMON_SDF3DSF_H_

#include <string>
#include <memory>
#include <unordered_map>
#include <deque>
#include <QString>

#include <QtCore/QPoint>

#include "stk_quoter_api.h"

#define  OUT
#define  IN
#define  INOUT
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

//#define USE_STK_QUOTER  // get k line info from stk quoter
//#define USE_WINNER_API  // get k line info from winnersystem 
#define USE_TDXHQ

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

#define MARKET_TYPE_SH  1
#define MARKET_TYPE_SZ  0
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
    PERIOD_YEAR,
};


class PyDataMan;
class T_KlinePosData
{
public:
    T_KlinePosData() : date(0), hhmm(0), x_left(0.0), x_right(0.0), height(0.0), columnar_top_left(CST_MAGIC_POINT), top(CST_MAGIC_POINT), bottom(CST_MAGIC_POINT) {}
    T_KlinePosData(const T_KlinePosData &lh)
        : date(lh.date), hhmm(lh.hhmm), x_left(lh.x_left), x_right(lh.x_right), height(lh.height), columnar_top_left(lh.columnar_top_left), top(lh.top), bottom(lh.bottom) {}
    T_KlinePosData(T_KlinePosData &&lh)
        : date(lh.date), hhmm(lh.hhmm), x_left(lh.x_left), x_right(lh.x_right), height(lh.height), columnar_top_left(lh.columnar_top_left), top(lh.top), bottom(lh.bottom) {}

    T_KlinePosData & operator = (const T_KlinePosData &lh)
    {
        if( this == &lh ) return *this;
        date = lh.date;
        hhmm = lh.hhmm;
        x_left = lh.x_left;
        x_right = lh.x_right;
        height = lh.height;
        columnar_top_left = lh.columnar_top_left;
        top = lh.top;
        bottom = lh.bottom;
    }
    void Clear(){date = 0; hhmm = 0; x_left = 0.0; x_right = 0.0; height = 0.0; columnar_top_left = CST_MAGIC_POINT;top = CST_MAGIC_POINT; bottom = CST_MAGIC_POINT;}
    
    int  date; 
    int  hhmm;
    double x_left;
    double x_right;
    double height;
    QPointF columnar_top_left;
    QPointF top;
    QPointF bottom;

};

enum class ZhibiaoType: unsigned char
{
    VOL = 0,
    MOMENTUM 
};
class ZhiBiaoAtom
{
public:
    ZhiBiaoAtom(){}
    virtual ~ZhiBiaoAtom() {} 

    virtual void val0( double ){}
    virtual double val0(){ return 0.0;}
    virtual void val1( double ){}
    virtual double val1(){ return 0.0;}
    virtual void val2( double ){}
    virtual double val2(){ return 0.0;}
    virtual void val3( double ){}
    virtual double val3(){ return 0.0;}
};

class T_KlineDataItem //_t_kline_dataitem
{
public:
    T_StockHisDataItem  stk_item;
    
    int  type;
    std::vector<std::shared_ptr<ZhiBiaoAtom> > zhibiao_atoms;
    T_KlineDataItem() : type(int(FractalType::UNKNOW_FRACTAL)), kline_posdata_1(), kline_posdata_0()
    {
        memset(&stk_item, 0, sizeof(stk_item));
    }
    explicit T_KlineDataItem(const T_KlineDataItem & lh)
    {
        if( this == &lh ) 
            return;
        CreateHelper(lh);
    }
    explicit T_KlineDataItem(T_KlineDataItem && lh): stk_item(std::move(lh.stk_item)), kline_posdata_1(std::move(lh.kline_posdata_1)), kline_posdata_0(std::move(lh.kline_posdata_0))
        , zhibiao_atoms(std::move(lh.zhibiao_atoms))
    {  
    }
    T_KlineDataItem & operator = (const T_KlineDataItem & lh)
    {
        if( this == &lh )
            return *this;
        CreateHelper(lh);
        return *this;
    }
    explicit T_KlineDataItem(const T_StockHisDataItem & stock_his_data_item): type(int(FractalType::UNKNOW_FRACTAL)), kline_posdata_0(), kline_posdata_1()
    {
        memcpy(&stk_item, &stock_his_data_item, sizeof(stock_his_data_item)); 
    }
    T_KlinePosData & kline_posdata(int index = 0)
    {
        if( index == 0 ) return kline_posdata_0;
        else return kline_posdata_1;
    }

private:
    void CreateHelper(const T_KlineDataItem & lh)
    {
        memcpy(&stk_item, &lh.stk_item, sizeof(lh.stk_item));
        this->kline_posdata_0 = lh.kline_posdata_0;
        this->kline_posdata_1 = lh.kline_posdata_1;
        this->zhibiao_atoms = lh.zhibiao_atoms;
    }
    T_KlinePosData  kline_posdata_0;
    T_KlinePosData  kline_posdata_1;
};

class T_StockBaseInfoItem
{ 
public:
    T_StockBaseInfoItem() : time_to_market(0){}
    std::string code;
    int type;  // 0--normal stock 1--index code
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

bool TransIndexPinYin2CodeName(const std::string &pinyin, std::string &code, std::string &name);
bool TransIndexCode2Name(const std::string &code, std::string &name);
std::string TransIndex2TusharedCode(const std::string &code);

FractalType  MaxFractalType(int val);
FractalType  BtmestFractalType(int val);

#ifndef T_K_Data
typedef struct _t_k_data
{
    int yyyymmdd;
    int hhmmdd;
    double open;
    double close;
    double high;
    double low;
    int vol;
}T_K_Data;
#define T_K_Data  T_K_Data
#endif

typedef std::deque<std::shared_ptr<T_KlineDataItem> >  T_HisDataItemContainer;
typedef std::unordered_map<std::string, T_HisDataItemContainer>  T_CodeMapHisDataItemContainer;


bool IsStrAlpha(const std::string& str);
bool IsStrNum(const std::string& str);
void utf8ToGbk(std::string& strUtf8);
void gbkToUtf8(std::string& strGbk);

#define  MOMENTUM_POS 0

#endif // STKFO_COMMON_SDF3DSF_H_
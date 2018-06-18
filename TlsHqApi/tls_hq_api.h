#ifndef TLS_HQ_API_SDFS546345D_H_
#define TLS_HQ_API_SDFS546345D_H_
 

#ifdef TLSHQAPI_EXPORTS 
#define TLSHQ_IMEXPORT  __declspec(dllexport)
#else
#define TLSHQ_IMEXPORT  __declspec(dllimport)
#endif 

struct T_BarDataItem
{
public:
    T_BarDataItem() : date(0), time(0), open_price(0.0), close_price(0.0)
        , high_price(0.0), low_price(0.0), vol(0.0), amount(0.0)
    {  
    }
    T_BarDataItem(int date_p, int time_p, double open_price_p, double close_price_p
        , double high_price_p, double low_price_p, double vol_p, double amount_p)
        : date(date_p), time(time_p), open_price(open_price_p), close_price(close_price_p)
        , high_price(high_price_p), low_price(low_price_p), vol(vol_p), amount(amount_p)
    { 
    }
    T_BarDataItem(T_BarDataItem &&lh): date(lh.date), time(lh.time), open_price(lh.open_price), close_price(lh.close_price)
        , high_price(lh.high_price), low_price(lh.low_price), vol(lh.vol), amount(lh.amount) 
    {
    }
    T_BarDataItem & operator =(const T_BarDataItem &lh)
    {
        if( this == &lh ) return *this;
        date = lh.date;
        time = lh.time;
        open_price = lh.open_price;
        close_price = lh.close_price;
        high_price = lh.high_price;
        low_price = lh.low_price;
        vol = lh.vol;
        amount = lh.amount;
    }
    int  date; 
    int  time;
    double open_price;
    double close_price;
    double high_price;
    double low_price; 
    double vol;
    double amount; 
};

typedef void (*BarDataCallBack)(void* /*holder*/, T_BarDataItem* /*input*/);

#ifdef __cplusplus
extern "C" {
#endif
  
 //数据种类, 0->5分钟K线    1->15分钟K线    2->30分钟K线  3->1小时K线    4->日K线  5->周K线  6->月K线  7->1分钟K线  8->1分钟K线  9->日K线  10->季K线  11->年K线
 
// nCategory
#define  KLINE_CATEGORY_DAY    '\4'
#define  KLINE_CATEGORY_60MIN  '\3'
#define  KLINE_CATEGORY_30MIN  '\2'
#define  KLINE_CATEGORY_15MIN  '\1'
#define  KLINE_CATEGORY_WEEK   '\5'
#define  KLINE_CATEGORY_MONTH  '\6'
#define  KLINE_CATEGORY_MIN    '\7'
#define  KLINE_CATEGORY_YEAR   11
 
// nMarket
#define  MARKET_SH  '\1'
#define  MARKET_SZ  '\0'


/*
* return: > 0 --success otherwise is error no
* enable_tdx , enable_python: > 0 enable
*/
int TLSHQ_IMEXPORT TlsHq_Initialize(int enable_tdx, int enable_python);

/*
* return: > 0 --success otherwise is error no
* notice: by the end, client have to use delete[] to release ret_items
*/
int TLSHQ_IMEXPORT TlsHq_GetDarDataItems(
    char nCategory,
    char nMarket,
    const char *pszZqdm,
    short nStart,
    short *nCount,
    T_BarDataItem **ret_items,
    char *pszErrInfo);

int TLSHQ_IMEXPORT TlsHq_GetDarDataItemsUseCallBack(
    char nCategory,
    char nMarket,
    const char *pszZqdm,
    short nStart,
    short *nCount,
    BarDataCallBack call_back,
    void *call_back_para,
    char *pszErrInfo);

#ifdef __cplusplus
}
#endif

#endif
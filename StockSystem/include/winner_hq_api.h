#ifndef WINNER_API_SDF32DF_H_
#define WINNER_API_SDF32DF_H_
 
//API使用流程为: 应用程序先调用WinnerHisHq_Connect连接赢家历史服务器,然后才可以调用其他接口获取行情数据,应用程序应自行处理网络断线问题, 接口是线程安全的
//如果断线，调用任意api函数后，api会返回已经断线的错误信息，应用程序应根据此错误信息重新连接服务器。

//3.各个函数功能说明
/// <summary>
/// 连接赢者行情服务器
/// </summary>
/// <param name="IP">服务器IP,可在券商通达信软件登录界面“通讯设置”按钮内查得</param>
/// <param name="Port">服务器端口</param>
/// <param name="Result">此API执行返回后  </param>
/// <param name="ErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
/// <returns>成功返回0, 失败返回其他</returns>
typedef int (__cdecl* WinnerHisHq_ConnectDelegate)(char* IP, int Port, char* Result, char* ErrInfo);

/// <summary>
/// 断开同服务器的连接
/// </summary>
typedef void(__cdecl* WinnerHisHq_DisconnectDelegate)();

typedef struct _t_quote_atom_data
{
    // 'code','date','time','price','change','volume','amount','type'
    char code[16];
    //int  date; //yyyymmdd
    __int64  time; //HHMMSS
    double price; // .2f
    double price_change;
    int  vol;
    unsigned char bid_type; // 0: buy_pan  1 :sell_pan
}T_QuoteAtomData;

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

#define T_K_Data T_K_Data
#endif
typedef void (*FenbiCallBack)(T_QuoteAtomData *quote_data, bool is_end, void *para);
typedef struct _t_fenbi_call_back
{
    FenbiCallBack  call_back_func;
    void *para;
    int date;
    unsigned int serial;
    _t_fenbi_call_back() : call_back_func(nullptr), para(nullptr), date(0), serial(0){} 
    _t_fenbi_call_back(const _t_fenbi_call_back &lh) : call_back_func(lh.call_back_func), para(lh.para), date(lh.date), serial(lh.serial){} 
}T_FenbiCallBack;

 //----------------------------
typedef void (*KDataCallBack)(T_K_Data *k_data, bool is_end, void *para);
typedef struct _t_kdata_call_back
{
    KDataCallBack  call_back_func;
    void *para;
    int date;
    unsigned int serial;
    _t_kdata_call_back() : call_back_func(nullptr), para(nullptr), date(0), serial(0){} 
    _t_kdata_call_back(const _t_kdata_call_back &lh) : call_back_func(lh.call_back_func), para(lh.para), date(lh.date), serial(lh.serial){} 
}T_KDataCallBack;


 
/// 获取历史分时数据
/// </summary>
/// <param name="Zqdm">证券代码</param>
/// <param name="Date">日期, 比如2014年1月1日为整数20140101</param>
/// <param name="FenbiCallBack"> 回调函数</param>
/// <param name="ErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
/// <returns>成功返回0, 失败返回其他 -1--未连接</returns>
typedef int (__cdecl* WinnerHisHq_GetHisFenbiDataDelegate)(char* Zqdm, int Date, T_FenbiCallBack *call_back_para, char* ErrInfo);

/// <summary>
/// 批量获取历史分时数据
/// </summary>
/// <param name="Zqdm">证券代码</param>
/// <param name="Date">日期, 比如2014年1月1日为整数20140101</param>
/// <param name="FenbiCallBack"> 回调函数</param>
/// <param name="ErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
/// <returns>成功返回0, 失败返回其他 -1--未连接</returns>
typedef int (__cdecl* WinnerHisHq_GetHisFenbiDataBatchDelegate)(char* Zqdm, int date_begin, int date_end, T_FenbiCallBack *call_back_para, char* ErrInfo);

enum class PeriodType : unsigned char
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

enum class FqType : unsigned char
{
    FQ_NO, 
    FQ_BEFORE,  //前复权
    FQ_AFTER,   //后复权
};
/// <summary>
/// 获取证券指定范围的的K线数据
/// </summary>
/// <param name="Category">K线种类, 0->5分钟K线 1->15分钟K线 2->30分钟K线 3->1小时K线 4->日K线 5->周K线 6->月K线 7->1分钟 8->1分钟K线 9->日K线 10->季K线 11->年K线< / param>
/// <param name="Market">市场代码, 0->深圳 1->上海</param>
/// <param name="Zqdm">证券代码</param>
/// <param name="Start">范围的开始位置,最后一条K线位置是0, 前一条是1, 依此类推</param>
/// <param name="Count">范围的大小，API执行前,表示用户要请求的K线数目, API执行后,保存了实际返回的K线数目, 最大值800</param>
/// <param name="Result">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
/// <param name="ErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
/// <returns>成功返货0, 失败返回 < 0</returns>
//typedef bool(__cdecl* TdxHq_GetSecurityBarsDelegate)(byte Category, byte Market, char* Zqdm, short Start, short& Count, char* Result, char* ErrInfo);
/// <summary>

typedef int (__cdecl* WinnerHisHq_GetKDataDelegate)(char* Zqdm, PeriodType type, int date_begin, int date_end
                                                           , T_KDataCallBack *call_back_para, bool is_index, char *ErrInfo);




#endif // WINNER_API_SDF32DF_H_
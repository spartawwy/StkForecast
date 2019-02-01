#ifndef __HQ_API_H
#define __HQ_API_H

#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif

//
// 行情API
//

//1.行情API均是TdxHqApi.dll文件的导出函数，包括以下函数：(所有行情函数均为客户端主动请求查询，不是服务器推送)

//2.API使用流程为: 应用程序先调用TdxHq_Connect连接通达信行情服务器,然后才可以调用其他接口获取行情数据,应用程序应自行处理网络断线问题, 接口是线程安全的

//3.各个函数功能说明

//
//  连接通达信行情服务器,服务器地址可在券商软件登录界面中的通讯设置中查得
//
// <param name="pszIP">服务器IP</param>
// <param name="nPort">服务器端口</param>
// <param name="pszResult">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
// <param name="pszErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
//
// <returns>成功返回行情连接ID, 失败返回0</returns>
//
int WINAPI TdxHq_Connect(
    const char *pszIP,
    short nPort,
    char *pszResult,
    char *pszErrInfo);


//
// 断开同服务器的连接
//
void WINAPI TdxHq_Disconnect(int nConnID);


//
//
//
void WINAPI TdxHq_SetTimeout(
    int nConnID,
    int nReadTimeout,
    int nWriteTimeout);


//
// 获取指定市场内的证券数目
//
bool WINAPI TdxHq_GetSecurityCount(
    int nConnID,
    char nMarket,
    short *pnCount,
    char *pszErrInfo);


//
// 获取指定市场内的证券列表
//
bool WINAPI TdxHq_GetSecurityList(
    int nConnID,
    char nMarket,
    short nStart,
    short *pnCount,
    char *pszResult,
    char *pszErrInfo);


//
// 批量获取多个证券的五档报价数据
//
// <param name="nMarket">市场代码,   0->深圳     1->上海, 第i个元素表示第i个证券的市场代码</param>
// <param name="pszZqdm">证券代码, Count个证券代码组成的数组</param>
// <param name="nCount">API执行前,表示用户要请求的证券数目,最大290, API执行后,保存了实际返回的数目</param>
// <param name="pszResult">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
// <param name="pszErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
//
// <returns>成功返货true, 失败返回false</returns>
//
bool WINAPI TdxHq_GetSecurityQuotes(
    int nConnID,
    const char nMarket[],
    const char *pszZqdm[],
    short *pnCount,
    char *pszResult,
    char *pszErrInfo);


//
// 获取证券的K线数据
//
// <param name="nCategory">K线种类, 0->5分钟K线    1->15分钟K线    2->30分钟K线  3->1小时K线    4->日K线  5->周K线  6->月K线  7->1分钟  8->1分钟K线  9->日K线  10->季K线  11->年K线< / param>
// <param name="nMarket">市场代码,   0->深圳     1->上海</param>
// <param name="pszZqdm">证券代码</param>
// <param name="nStart">K线开始位置,最后一条K线位置是0, 前一条是1, 依此类推</param>
// <param name="nCount">API执行前,表示用户要请求的K线数目, API执行后,保存了实际返回的K线数目, 最大值800</param>
// <param name="pszResult">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
// <param name="pszErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
//
// <returns>成功返货true, 失败返回false</returns>
//
bool WINAPI TdxHq_GetSecurityBars(
    int nConnID,
    char nCategory,
    char nMarket,
    const char *pszZqdm,
    short nStart,
    short *pnCount,
    char *pszResult,
    char *pszErrInfo);


//
// 获取指数的K线数据
//
// <!-- <param name="nCategory"> -->K线种类, 0->5分钟K线    1->15分钟K线    2->30分钟K线  3->1小时K线    4->日K线  5->周K线  6->月K线  7->1分钟  8->1分钟K线  9->日K线  10->季K线  11->年K线< / param>
// <param name="nMarket">市场代码,   0->深圳     1->上海</param>
// <param name="pszZqdm">证券代码</param>
// <param name="nStart">K线开始位置,最后一条K线位置是0, 前一条是1, 依此类推</param>
// <param name="nCount">API执行前,表示用户要请求的K线数目, API执行后,保存了实际返回的K线数目,最大值800</param>
// <param name="pszResult">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
// <param name="pszErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
//
// <returns>成功返货true, 失败返回false</returns>
//
bool WINAPI TdxHq_GetIndexBars(
    int nConnID,
    char nCategory,
    char nMarket,
    const char *pszZqdm,
    short nStart,
    short *pnCount,
    char *pszResult,
    char *pszErrInfo);


//
// 获取分时数据
//
// <param name="nMarket">市场代码,   0->深圳     1->上海</param>
// <param name="pszZqdm">证券代码</param>
// <param name="pszResult">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
// <param name="pszErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
//
// <returns>成功返货true, 失败返回false</returns>
//
bool WINAPI TdxHq_GetMinuteTimeData(
    int nConnID,
    char nMarket,
    const char *pszZqdm,
    char *pszResult,
    char *pszErrInfo);


//
// 获取历史分时数据
//
// <param name="nMarket">市场代码,   0->深圳     1->上海</param>
// <param name="pszZqdm">证券代码</param>
// <param name="nDate">日期, 比如2014年1月1日为整数20140101</param>
// <param name="pszResult">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
// <param name="pszErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
//
// <returns>成功返货true, 失败返回false</returns>
//
bool WINAPI TdxHq_GetHistoryMinuteTimeData(
    int nConnID,
    char nMarket,
    const char *pszZqdm,
    int nDate,
    char *pszResult,
    char *pszErrInfo);


//
// 获取分时成交数据
//
// <param name="nMarket">市场代码,   0->深圳     1->上海</param>
// <param name="pszZqdm">证券代码</param>
// <param name="nStart">K线开始位置,最后一条K线位置是0, 前一条是1, 依此类推</param>
// <param name="nCount">API执行前,表示用户要请求的K线数目, API执行后,保存了实际返回的K线数目</param>
// <param name="pszResult">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
// <param name="pszErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
//
// <returns>成功返货true, 失败返回false</returns>
//
bool WINAPI TdxHq_GetTransactionData(
    int nConnID,
    char nMarket,
    const char *pszZqdm,
    short nStart,
    short *pnCount,
    char *pszResult,
    char *pszErrInfo);


//
// 获取历史分时成交数据
//
// <param name="nMarket">市场代码,   0->深圳     1->上海</param>
// <param name="pszZqdm">证券代码</param>
// <param name="nStart">K线开始位置,最后一条K线位置是0, 前一条是1, 依此类推</param>
// <param name="nCount">API执行前,表示用户要请求的K线数目, API执行后,保存了实际返回的K线数目</param>
// <param name="nDate">日期, 比如2014年1月1日为整数20140101</param>
// <param name="pszResult">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
// <param name="pszErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
//
// <returns>成功返货true, 失败返回false</returns>
//
bool WINAPI TdxHq_GetHistoryTransactionData(
    int nConnID,
    char nMarket,
    const char *pszZqdm,
    short nStart,
    short *pnCount,
    int date,
    char *pszResult,
    char *pszErrInfo);


//
// 获取F10资料的分类
//
// <param name="nMarket">市场代码,   0->深圳     1->上海</param>
// <param name="pszZqdm">证券代码</param>
// <param name="pszResult">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
// <param name="pszErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
//
// <returns>成功返货true, 失败返回false</returns>
//
bool WINAPI TdxHq_GetCompanyInfoCategory(
    int nConnID,
    char nMarket,
    const char *pszZqdm,
    char *pszResult,
    char *pszErrInfo);


//
// 获取F10资料的某一分类的内容
//
// <param name="nMarket">市场代码,   0->深圳     1->上海</param>
// <param name="pszZqdm">证券代码</param>
// <param name="pszFileName">类目的文件名, 由TdxHq_GetCompanyInfoCategory返回信息中获取</param>
// <param name="nStart">类目的开始位置, 由TdxHq_GetCompanyInfoCategory返回信息中获取</param>
// <param name="nLength">类目的长度, 由TdxHq_GetCompanyInfoCategory返回信息中获取</param>
// <param name="pszResult">此API执行返回后，Result内保存了返回的查询数据,出错时为空字符串。</param>
// <param name="pszErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
//
// <returns>成功返货true, 失败返回false</returns>
//
bool WINAPI TdxHq_GetCompanyInfoContent(
    int nConnID,
    char nMarket,
    const char *pszZqdm,
    const char *pszFileName,
    int nStart,
    int nLength,
    char *pszResult,
    char *pszErrInfo);


//
// 获取除权除息信息
//
// <param name="nMarket">市场代码,   0->深圳     1->上海</param>
// <param name="pszZqdm">证券代码</param>
// <param name="pszResult">此API执行返回后，Result内保存了返回的查询数据,出错时为空字符串。</param>
// <param name="pszErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
//
// <returns>成功返货true, 失败返回false</returns>
//
bool WINAPI TdxHq_GetXDXRInfo(
    int nConnID,
    char nMarket,
    const char *pszZqdm,
    char *pszResult,
    char *pszErrInfo);


//
// 获取财务信息
//
// <param name="nMarket">市场代码,   0->深圳     1->上海</param>
// <param name="pszZqdm">证券代码</param>
// <param name="pszResult">此API执行返回后，Result内保存了返回的查询数据,出错时为空字符串。</param>
// <param name="pszErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
//
// <returns>成功返货true, 失败返回false</returns>
//
bool WINAPI TdxHq_GetFinanceInfo(
    int nConnID,
    char nMarket,
    const char *pszZqdm,
    char *pszResult,
    char *pszErrInfo);

#ifdef __cplusplus
}
#endif

#endif

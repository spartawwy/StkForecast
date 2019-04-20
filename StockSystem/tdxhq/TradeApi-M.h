#ifndef __TRADE_API_M_H
#define __TRADE_API_M_H

#include <Windows.h>

#define MAX_RESULT_SIZE   1024*1024
#define MAX_ERRINFO_SIZE  1024


#ifdef __cplusplus
extern "C" {
#endif

//
// 打开通达信实例
//
int WINAPI OpenTdx(
    short nClientType,
    const char *pszClientVersion,
    char nCliType,
    char nVipTermFlag,
    char *pszErrInfo);

//
// 关闭通达信实例
//
void WINAPI CloseTdx();

//
// 交易账户登录
//
// 参数:
//     nQsid           - 券商标识
//     pszHost         - 券商交易服务器IP
//     nPort           - 券商交易服务器端口
//     pszVersion      - 设置通达信客户端的版本号
//     nYybId          - 营业部代码，请到网址 http://www.chaoguwaigua.com/downloads/qszl.htm 查询
//     nAccountType    - 登录账号类型
//     pszAccountNo    - 完整的登录账号，券商一般使用资金帐户或客户号
//     pszTradeAccount - 交易账号，一般与登录帐号相同. 请登录券商通达信软件，查询股东列表，股东列表内的资金帐号就是交易帐号, 具体查询方法请见网站“热点问答”栏目
//     pszJyPassword   - 交易密码
//     pszTxPassword   - 通讯密码
//     pszErrInfo      - 此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。
//
// 返回值:
//     客户端ID，失败时返回0
//
int WINAPI Logon(
    int nQsid,
    const char *pszHost,
    short nPort,
    const char *pszVersion,
    short nYybId,
    char nAccountType,
    const char *pszAccountNo,
    const char *pszTradeAccount,
    const char *pszJyPassword,
    const char *pszTxPassword,
    char *pszErrInfo);

//
// 交易账户登录
//
// 参数:
//     nQsid           - 券商标识
//     pszHost         - 券商交易服务器IP
//     nPort           - 券商交易服务器端口
//     pszVersion      - 设置通达信客户端的版本号
//     nYybId          - 营业部代码，请到网址 http://www.chaoguwaigua.com/downloads/qszl.htm 查询
//     nAccountType    - 登录账号类型
//     pszAccountNo    - 完整的登录账号，券商一般使用资金帐户或客户号
//     pszTradeAccount - 交易账号，一般与登录帐号相同. 请登录券商通达信软件，查询股东列表，股东列表内的资金帐号就是交易帐号, 具体查询方法请见网站“热点问答”栏目
//     pszJyPassword   - 交易密码
//     pszTxPassword   - 通讯密码
//     pszQsInfoEx     - 针对一些券商的附加信息, 缺省填写0, 大多数用户无需关心
//     pszErrInfo      - 此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。
//
// 返回值:
//     客户端ID，失败时返回0
//
int WINAPI LogonEx(
    int nQsid,
    const char *pszHost,
    short nPort,
    const char *pszVersion,
    short nYybId,
    char nAccountType,
    const char *pszAccountNo,
    const char *pszTradeAccount,
    const char *pszJyPassword,
    const char *pszTxPassword,
    const char* pszOtherInfo,
    char *pszErrInfo);

//
// 交易账户注销
//
// 参数:
//     nClientID - 客户端ID
//
void WINAPI Logoff(int nClientID);

//
//
//
bool WINAPI IsConnectOK(int nClientID);

//
//
//
const char * WINAPI GetMachineInfo(int nClientID, char *pszBuff, int size);

//
//  0 - 成功
// -1 - 参数错误 ERR_PARAM_CHECK
// -2 - 内存错误 ERR_MEMORY
// -3 - 逻辑错误 ERR_LOGIC
//

//
// 查询各种交易数据
//
// 参数:
//     nClientID   - 客户端ID
//     nCategory   - 表示查询信息的种类，
//                  0 资金
//                  1 股份
//                  2 当日委托
//                  3 当日成交
//                  4 可撤单
//                  5 股东代码
//                  6 融资余额
//                  7 融券余额
//                  8 可融证券
//                  9
//                 10
//                 11
//                 12 可申购新股查询
//                 13 新股申购额度查询
//                 14 配号查询
//                 15 中签查询
//
//     pszResult   - 此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，
//                   行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。
//                   出错时为空字符串。
//     pszErrInfo  - 同Logon函数的ErrInfo说明
//
int WINAPI QueryData(
    int nClientID,
    int nCategory,
    char *pszResult,
    char *pszErrInfo);

int WINAPI QueryDataEx(
    int nClientID,
    int nCategory,
    int nBatchNum,
    char *pszSeekStr,
    char *pszResult,
    char *pszErrInfo);

//
// 下委托交易证券
//
// 参数:
//     nClientID  - 客户端ID
//     nCategory  - 表示委托的种类，
//                  0 买入
//                  1 卖出
//                  2 融资买入
//                  3 融券卖出
//                  4 买券还券
//                  5 卖券还款
//                  6 现券还券
//                  7 担保品买入
//                  8 担保品卖出
//     nPriceType - 表示报价方式
//                  0 上海限价委托 深圳限价委托
//                  1 (市价委托)深圳对方最优价格
//                  2 (市价委托)深圳本方最优价格
//                  3 (市价委托)深圳即时成交剩余撤销
//                  4 (市价委托)上海五档即成剩撤 深圳五档即成剩撤
//                  5 (市价委托)深圳全额成交或撤销
//                  6 (市价委托)上海五档即成转限价
//     pszGddm    - 股东代码, 交易上海股票填上海的股东代码；交易深圳的股票填入深圳的股东代码
//     pszZqdm    - 证券代码
//     fPrice     - 委托价格
//     nQuantity  - 委托数量
//     pszResult  - 同上,其中含有委托编号数据
//     pszErrInfo - 同上
//
int WINAPI SendOrder(
    int nClientID,
    int nCategory,
    int PriceType,
    const char *pszGddm,
    const char *pszZqdm,
    float fPrice,
    int nQuantity,
    char *pszResult,
    char *pszErrInfo);

//
// 撤委托
//
// 参数:
//     nClientID     - 客户端ID
//     nExchangeID   - 交易所ID， 上海1，深圳0(招商证券普通账户深圳是2)
//     pszHth        - 表示要撤的目标委托的编号
//     pszResult     - 同上
//     pszErrInfo    - 同上
//
int WINAPI CancelOrder(
    int nClientID,
    char nMarket,
    const char *pszHth,
    char *pszResult,
    char *pszErrInfo);

//
// 参数:
//     nClientID     - 客户端ID
//     nExchangeID   - 交易所ID， 上海1，深圳0(招商证券普通账户深圳是2)
//     pszHth        - 表示要撤的目标委托的编号
//     pszGddm       - 股东代码
//     pszResult     - 同上
//     pszErrInfo    - 同上
//
int WINAPI CancelOrderEx(
    int nClientID,
    char nMarket,
    const char *pszHth,
    const char *pszGddm,
    char *pszResult,
    char *pszErrInfo);

//
// 获取证券的实时五档行情
//
// 参数:
//     nClientID  - 客户端ID
//     pszZqdm    - 证券代码
//     pszResult  - 同上
//     pszErrInfo - 同上
//
int WINAPI GetQuote(
    int nClientID,
    const char *pszZqdm,
    char *pszResult,
    char *pszErrInfo);

//
//
//
int WINAPI GetTradableQuantity(
    int nClientID,
    char nCategory,
    int nPriceType,
    const char *pszGddm,
    const char *pszZqdm,
    float fPrice,
    char *pszResult,
    char *pszErrInfo);

//
// 融资融券账户直接还款
//
int WINAPI Repay(
    int nClientID,
    const char *pszAmount,
    char *pszResult,
    char *pszErrInfo);

//
//
//
int WINAPI QueryHistoryData(
    int nClientID,
    int nCategory,
    const char *pszBeginDate,
    const char *pszEndDate,
    char *pszResult,
    char *pszErrInfo);

int WINAPI QueryHistoryDataEx(
    int nClientID,
    int nCategory,
    int nBatchNum,
    char *pszSeekStr,
    const char *pszBeginDate,
    const char *pszEndDate,
    char *pszResult,
    char *pszErrInfo);

//
// 单账户批量查询各类交易数据
//
int WINAPI QueryDatas(
    int nClientID,
    int nCategory[],
    int nCount,
    char* pszResultOK[],
    char* pszResultFail[],
    char* pszErrInfo);

//
// 单账户批量下单
//
int WINAPI SendOrders(
    int nClientID,
    int nCategory[],
    int nPriceType[],
    const char *pszGddm[],
    const char *pszZqdm[],
    float fPrice[],
    int nQuantity[],
    int nCount,
    char* pszResultOK[],
    char* pszResultFail[],
    char* pszErrInfo);

//
// 单账户批量撤单
//
int WINAPI CancelOrders(
    int nClientID,
    const char nMarket[],
    const char *pszOrderID[],
    int nCount,
    char* pszResultOK[],
    char* pszResultFail[],
    char* pszErrInfo);

//
// 单账户批量获取五档报价
//
int WINAPI GetQuotes(
    int nClientID,
    const char *pszZqdm[],
    int nCount,
    char* pszResultOK[],
    char* pszResultFail[],
    char* pszErrInfo);

//
// 一键打新
//
int WINAPI QuickIPO(int nClientID);

//
// 一键打新
//
int WINAPI QuickIPODetail(
    int nClientID,
    int nCount,
    char* pszResultOK[],
    char* pszResultFail[],
    char* pszErrInfo);

//
// 批量向不同账户查询各类交易数据
//
int WINAPI QueryMultiAccountsDatas(
    int nClientID[],
    int nCategory[],
    int nCount,
    char* pszResultOK[],
    char* pszResultFail[]);

//
// 批量向不同账户下单
//
int WINAPI SendMultiAccountsOrders(
    int nClientID[],
    int nCategory[],
    int nPriceType[],
    const char *pszGddm[],
    const char *pszZqdm[],
    float fPrice[],
    int nQuantity[],
    int nCount,
    char *pszResult[],
    char *pszErrInfo[]);

//
// 批量向不同账户撤单
//
int WINAPI CancelMultiAccountsOrders(
    int nClientID[],
    char nMarket[],
    const char *pszOrderID[],
    int nCount,
    char *pszResult[],
    char *pszErrInfo[]);

//
// 批量向不同账户获取五档报价
//
int WINAPI GetMultiAccountsQuotes(
    int nClientID[],
    const char *pszZqdm[],
    int nCount,
    char *pszResult[],
    char *pszErrInfo[]);


#ifdef __cplusplus
}
#endif

#endif

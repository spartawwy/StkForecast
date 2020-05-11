// TradeXDemo-M.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>

using namespace std;

#include "TradeX.h"

int test_trade_multi_funcs();

int test_hq_batch_funcs(const char *pszHqSvrIP, short nPort);
int test_exhq_batch_funcs(const char *pszHqSvrIP, short nPort);

int test_l2hq_funcs();

int _tmain(int argc, _TCHAR* argv[])
{
    //////////////////////////////////////////////////////////////////////////////////

    cout << "\n";
    cout << "\n";
    cout << "\t**********************************************************************" << endl;
    cout << "\t*                                TradeX v1.4.0                       *" << endl;
    cout << "\t*                                                                    *" << endl;
    cout << "\t*  TradeX 股票程序交易接口全新发布！                                 *" << endl;
    cout << "\t*                                                                    *" << endl;
    cout << "\t*  版本描述：                                                        *" << endl;
    cout << "\t*  1）支持普通账户/融资融券信用账户业务，包括全新的担保品买入、卖出  *" << endl;
    cout << "\t*   和免配置一键打新功能；解决了华福证券50条记录的限制；             *" << endl;
    cout << "\t*  2）支持批量多连接下单和多账户同时下单，每秒批量下单可达数百单；   *" << endl;
    cout << "\t*  3）支持股票的普通行情、Level 2十档、逐笔行情以及期货等扩展行情，  *" << endl;
    cout << "\t*   允许同时批量多连接取行情；                                       *" << endl;
    cout << "\t*  4）直连交易服务器和行情服务器，无中转，安全、稳定，实盘运行中；   *" << endl;
    cout << "\t*  5）支持C++，C#，Python，Delphi，Java，易语言，AutoIt等语言；      *" << endl;
    cout << "\t*  6）完美兼容trade.dll，解决了华泰、光大等券商的连接问题。          *" << endl;
    cout << "\t*                                                                    *" << endl;
    cout << "\t*  技术QQ群：318139137  QQ：3048747297                               *" << endl;
    cout << "\t*  技术首页：https://tradexdll.com/                                  *" << endl;
    cout << "\t*  http://pan.baidu.com/s/1jIjYq1K                                   *" << endl;
    cout << "\t*                                                                    *" << endl;
    cout << "\t**********************************************************************" << endl;
    cout << "\n";

    cout << "按回车键进行测试..." << endl;
    //cin.get();

    cout << endl;
    cout << "\n";

    //
    //
    //
    if (0)
    {
        cout << "测试交易API, 按回车键继续...\n" << std::endl;
        if (!test_trade_multi_funcs())
        {
            cout << "测试结束!!!" << endl;
            cin.get();
            //return 0;
        }
    }

    //
    //
    if (1)
    {
        cout << "测试行情API, 按回车键继续...\n" << std::endl;
        //test_hq_batch_funcs("14.17.75.71", 7709);  
        test_hq_batch_funcs("218.75.126.9", 7709); //218.75.126.9
    }

    //
    //
    if (1)
    {
        cout << "测试扩展行情API, 按回车键继续...\n" << std::endl;
        test_exhq_batch_funcs("59.175.238.38", 7727);
    }

    //
    //
    if (0)
    {
        cout << "测试L2行情API, 按回车键继续...\n" << std::endl;
        test_l2hq_funcs();
    }

    cout << "测试结束!!!" << endl;
    cin.get();

    return 0;
}



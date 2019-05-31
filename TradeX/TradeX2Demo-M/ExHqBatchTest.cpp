
/*
市场代码        市场名称
1       临时股
4       郑州商品期权
5       大连商品期权
6       上海商品期权
7       中金所期权
8       上海股票期权
27      香港指数
28      郑州商品
29      大连商品
30      上海期货
31      香港主板
33      开放式基金
34      货币型基金
38      宏观指标
40      中国概念股
41      美股知名公司
42      商品指数
43      B股转H股
44      股转系统
46      上海黄金
47      中金所期货
48      香港创业板
49      香港基金
50      渤海商品
54      国债预发行
56      阳光私募基金
57      券商集合理财
58      券商货币理财
60      主力期货合约
62      中证指数
68      风控指数
70      扩展板块指数
74      美国股票
*/
#include "stdafx.h"

#include "TradeX.h"

#include <iostream>

using namespace std;

#define E1  1 //1 // TdxExHq_GetMarkets
#define E2  1 //1 // TdxExHq_GetInstrumentCount
#define E3  1 //1 // TdxExHq_GetInstrumentInfo
#define E4  0 //1 // TdxExHq_GetInstrumentQuote
#define E5  1 //1 // TdxExHq_GetInstrumentBars
#define E6  0 //1 // TdxExHq_GetMinuteTimeData
#define E7  1 // TdxExHq_GetHistoryMinuteTimeData
#define E8  1 // TdxExHq_GetTransactionData
#define E9  1 // TdxExHq_GetHistoryTransactionData

int test_exhq_batch_funcs(const char *pszHqSvrIP, short nPort)
{
    //开始获取行情数据
    char* Result = new char[1024 * 1024];
    char* ErrInfo = new char[256];
    short Count = 10;

    //连接服务器
    int nConn = TdxExHq_Connect(pszHqSvrIP, nPort, Result, ErrInfo);
    if (nConn < 0)
    {
        cout << ErrInfo << endl;
        return 0;
    }

    std::cout << Result << std::endl;

    bool bool1;

#if E1
    {
        cout << "\n*** TdxExHq_GetMarkets\n";

        bool1 = TdxExHq_GetMarkets(nConn, Result, ErrInfo);
        if (!bool1)
        {
            cout << ErrInfo << endl;//连接失败
            getchar();
            return 0;
        }

        cout << Result << endl;
        getchar();
    }
#endif

#if E2
    {
        cout << "\n*** TdxExHq_GetInstrumentCount\n";

        int nCount = -1;
        bool1 = TdxExHq_GetInstrumentCount(nConn, &nCount, ErrInfo);
        if (!bool1)
        {
            cout << ErrInfo << endl;//连接失败
            getchar();
            return 0;
        }

        cout << "Count = " << nCount << endl;
        getchar();
    }
#endif

#if E3
    {
        cout << "\n*** TdxExHq_GetInstrumentInfo\n";

        Count = 50;
        bool1 = TdxExHq_GetInstrumentInfo(nConn, 0, &Count,  Result, ErrInfo); //Count最大511
        if (!bool1)
        {
            cout << ErrInfo << endl;
            return 0;
        }

        cout << "Count = " << Count << endl;
        cout << Result << endl;
        getchar();
    }
#endif

#if E4
    {
        cout << "\n*** TdxExHq_GetInstrumentQuote\n";

        bool1 = TdxExHq_GetInstrumentQuote(nConn, 47, "IF1702",  Result, ErrInfo);
        if (!bool1)
        {
            cout << ErrInfo << endl;
            return 0;
        }

        cout << Result << endl;
        getchar();
    }
#endif

#if E5
    {
        cout << "\n*** TdxExHq_GetInstrumentBars\n";

        // 0->5分钟K线    1->15分钟K线    2->30分钟K线  3->1小时K线    4->日K线  5->周K线  6->月K线  7->1分钟  8->1分钟K线  9->日K线  10->季K线  11->年K线
        Count = 50;
        bool1 = TdxExHq_GetInstrumentBars(nConn, 7, 539, "HSI", 0, &Count, Result, ErrInfo);
        if (!bool1)
        {
            cout << ErrInfo << endl;
            return 0;
        }

        cout << Result << endl;
        getchar();

        Count = 50;
        bool1 = TdxExHq_GetInstrumentBars(nConn, 7, 30, "SC1907", 0, &Count, Result, ErrInfo);
        if (!bool1)
        {
            cout << ErrInfo << endl;
            return 0;
        }

        cout << Result << endl;
        getchar();
    }
#endif

#if E6
    {
        cout << "\n*** TdxExHq_GetMinuteTimeData\n";

        bool1 = TdxExHq_GetMinuteTimeData(nConn, 47, "IF1702", Result, ErrInfo);
        if (!bool1)
        {
            cout << ErrInfo << endl;
            return 0;
        }

        cout << Result << endl;
        getchar();
    }
#endif

#if E7
    {
        cout << "\n*** TdxExHq_GetHistoryMinuteTimeData\n";

        bool1 = TdxExHq_GetHistoryMinuteTimeData(nConn, 47, "IF1702", 20170113, Result, ErrInfo);
       // bool1 = TdxExHq_GetHistoryMinuteTimeData(nConn, 1, "600196", 20180113, Result, ErrInfo);
        if (!bool1)
        {
            cout << ErrInfo << endl;
            return 0;
        }

        cout << Result << endl;
        getchar();
    }
#endif

#if E8
    {
        cout << "\n*** TdxExHq_GetTransactionData\n";

        Count = 200;
        bool1 = TdxExHq_GetTransactionData(nConn, 47, "IF1702", 0, &Count, Result, ErrInfo);
        if (!bool1)
        {
            cout << ErrInfo << endl;
            return 0;
        }

        cout << Result << endl;
        getchar();
    }
#endif

#if E9
    {
        cout << "\n*** TdxExHq_GetHistoryTransactionData\n";

        bool1 = TdxExHq_GetHistoryTransactionData(nConn, 47, "IF1702", 20170113, 0, &Count, Result, ErrInfo);
        if (!bool1)
        {
            cout << ErrInfo << endl;
            return 0;
        }

        cout << Result << endl;
        getchar();
    }
#endif

    TdxExHq_Disconnect(nConn);

    cout << "已经断开行情服务器" << endl;

    getchar();
    return 0;
}



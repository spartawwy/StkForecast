
/*
�г�����        �г�����
1       ��ʱ��
4       ֣����Ʒ��Ȩ
5       ������Ʒ��Ȩ
6       �Ϻ���Ʒ��Ȩ
7       �н�����Ȩ
8       �Ϻ���Ʊ��Ȩ
27      ���ָ��
28      ֣����Ʒ
29      ������Ʒ
30      �Ϻ��ڻ�
31      �������
33      ����ʽ����
34      �����ͻ���
38      ���ָ��
40      �й������
41      ����֪����˾
42      ��Ʒָ��
43      B��תH��
44      ��תϵͳ
46      �Ϻ��ƽ�
47      �н����ڻ�
48      ��۴�ҵ��
49      ��ۻ���
50      ������Ʒ
54      ��ծԤ����
56      ����˽ļ����
57      ȯ�̼������
58      ȯ�̻������
60      �����ڻ���Լ
62      ��ָ֤��
68      ���ָ��
70      ��չ���ָ��
74      ������Ʊ
*/
#include "stdafx.h"

#include "TradeX.h"

#include <iostream>

using namespace std;

//#define E1  1 //1 // TdxExHq_GetMarkets
#define E2  0 //1 // TdxExHq_GetInstrumentCount
#define E3  1 //1 // TdxExHq_GetInstrumentInfo
//#define E4  1 //1 // TdxExHq_GetInstrumentQuote
#define E5  1 //1 // TdxExHq_GetInstrumentBars
#define E6  0 //1 // TdxExHq_GetMinuteTimeData
#define E7  1 // TdxExHq_GetHistoryMinuteTimeData
//#define E8  1 // TdxExHq_GetTransactionData
//#define E9  1 // TdxExHq_GetHistoryTransactionData

int test_exhq_batch_funcs(const char *pszHqSvrIP, short nPort)
{
    //��ʼ��ȡ��������
    char* Result = new char[1024 * 1024];
    char* ErrInfo = new char[256];
    short Count = 10;

    //���ӷ�����
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
            cout << ErrInfo << endl;//����ʧ��
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
            cout << ErrInfo << endl;//����ʧ��
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
        bool1 = TdxExHq_GetInstrumentInfo(nConn, 0, &Count,  Result, ErrInfo); //Count���511
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

        //bool1 = TdxExHq_GetInstrumentQuote(nConn, 47, "IF1702",  Result, ErrInfo);
        bool1 = TdxExHq_GetInstrumentQuote(nConn, 30, "SC2003",  Result, ErrInfo);
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
#if 0
        // 0->5����K��    1->15����K��    2->30����K��  3->1СʱK��    4->��K��  5->��K��  6->��K��  7->1����  8->1����K��  9->��K��  10->��K��  11->��K��
        Count = 50;
        bool1 = TdxExHq_GetInstrumentBars(nConn, 7, 539, "HSI", 0, &Count, Result, ErrInfo);
        if (!bool1)
        {
            cout << ErrInfo << endl;
            return 0;
        }

        cout << Result << endl;
        getchar();
#endif
        // 0->5����K��    1->15����K��    2->30����K��  3->1СʱK��    4->��K��  5->��K��  6->��K��  7->1����  8->1����K��  9->��K��  10->��K��  11->��K��
        Count = 400; //500;
        bool1 = TdxExHq_GetInstrumentBars(nConn, 0, 30, "SCL9", 0, &Count, Result, ErrInfo);
        if (!bool1)
        {
            cout << ErrInfo << endl;
            return 0;
        }

        cout << Result << endl;
        getchar();
#if 0
        cout << " print day " << endl;
        Count = 400; //500;
        bool1 = TdxExHq_GetInstrumentBars(nConn, 4, 30, "SC2003", 0, &Count, Result, ErrInfo);
        if (!bool1)
        {
            cout << ErrInfo << endl;
            return 0;
        }

        cout << Result << endl;
        getchar();
#endif
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

        bool1 = TdxExHq_GetHistoryMinuteTimeData(nConn, 30, "SCL9", 20170113, Result, ErrInfo);
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

    cout << "�Ѿ��Ͽ����������" << endl;

    getchar();
    return 0;
}



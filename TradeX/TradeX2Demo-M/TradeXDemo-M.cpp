// TradeXDemo-M.cpp : �������̨Ӧ�ó������ڵ㡣
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
    cout << "\t*  TradeX ��Ʊ�����׽ӿ�ȫ�·�����                                 *" << endl;
    cout << "\t*                                                                    *" << endl;
    cout << "\t*  �汾������                                                        *" << endl;
    cout << "\t*  1��֧����ͨ�˻�/������ȯ�����˻�ҵ�񣬰���ȫ�µĵ���Ʒ���롢����  *" << endl;
    cout << "\t*   ��������һ�����¹��ܣ�����˻���֤ȯ50����¼�����ƣ�             *" << endl;
    cout << "\t*  2��֧�������������µ��Ͷ��˻�ͬʱ�µ���ÿ�������µ��ɴ����ٵ���   *" << endl;
    cout << "\t*  3��֧�ֹ�Ʊ����ͨ���顢Level 2ʮ������������Լ��ڻ�����չ���飬  *" << endl;
    cout << "\t*   ����ͬʱ����������ȡ���飻                                       *" << endl;
    cout << "\t*  4��ֱ�����׷����������������������ת����ȫ���ȶ���ʵ�������У�   *" << endl;
    cout << "\t*  5��֧��C++��C#��Python��Delphi��Java�������ԣ�AutoIt�����ԣ�      *" << endl;
    cout << "\t*  6����������trade.dll������˻�̩������ȯ�̵��������⡣          *" << endl;
    cout << "\t*                                                                    *" << endl;
    cout << "\t*  ����QQȺ��318139137  QQ��3048747297                               *" << endl;
    cout << "\t*  ������ҳ��https://tradexdll.com/                                  *" << endl;
    cout << "\t*  http://pan.baidu.com/s/1jIjYq1K                                   *" << endl;
    cout << "\t*                                                                    *" << endl;
    cout << "\t**********************************************************************" << endl;
    cout << "\n";

    cout << "���س������в���..." << endl;
    //cin.get();

    cout << endl;
    cout << "\n";

    //
    //
    //
    if (0)
    {
        cout << "���Խ���API, ���س�������...\n" << std::endl;
        if (!test_trade_multi_funcs())
        {
            cout << "���Խ���!!!" << endl;
            cin.get();
            //return 0;
        }
    }

    //
    //
    if (1)
    {
        cout << "��������API, ���س�������...\n" << std::endl;
        //test_hq_batch_funcs("14.17.75.71", 7709);  
        test_hq_batch_funcs("218.75.126.9", 7709); //218.75.126.9
    }

    //
    //
    if (1)
    {
        cout << "������չ����API, ���س�������...\n" << std::endl;
        test_exhq_batch_funcs("59.175.238.38", 7727);
    }

    //
    //
    if (0)
    {
        cout << "����L2����API, ���س�������...\n" << std::endl;
        test_l2hq_funcs();
    }

    cout << "���Խ���!!!" << endl;
    cin.get();

    return 0;
}



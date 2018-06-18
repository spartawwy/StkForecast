
#include "stdafx.h"

#include "TradeX.h"

#include <iostream>
#include <list>
#include <regex>

#include "hq_common.h"

using namespace std;

#define F1  0 // TdxHq_GetSecurityCount
#define F2  0 // TdxHq_GetSecurityList
#define F3  0 // TdxHq_GetMinuteTimeData
#define F4  1 // TdxHq_GetSecurityBars
#define F5  0 // TdxHq_GetHistoryMinuteTimeData
#define F6  0 // TdxHq_GetIndexBars
#define F7  0 // TdxHq_GetTransactionData
#define F8  1 // TdxHq_GetHistoryTransactionData
#define F9  0 // TdxHq_GetSecurityQuotes
#define F10 0 // TdxHq_GetCompanyInfoCategory
#define F11 1 // TdxHq_GetCompanyInfoContent
#define F12 1 // TdxHq_GetXDXRInfo
#define F13 1 // TdxHq_GetFinanceInfo

 template<typename FUNCTYPE, typename HOLDER_TYPE>
 void ParseData(char *src, const std::string &regex_str, FUNCTYPE func, HOLDER_TYPE &holder)
 {
	std::string  str_result;
	std::regex reg_obj(regex_str);
	std::smatch  match_res;

	char *p_target = src;
	char *q = src;
	bool is_first_line = true;
	while( *q != '\0' )
	{
		while( *q != '\n' && *q != '\0' ) ++q;

		if( is_first_line )
		{
			is_first_line = false;
			 
			continue;
		}

		if( *q != '\0' )
		{
			*q = '\0';
			std::string tmp_str = p_target;
			if( std::regex_match(tmp_str.cbegin(), tmp_str.cend(), match_res, reg_obj) )
			{
				func(match_res, holder);
			}
			 
			++q;
			p_target = q;
		}else 
		{
			std::string tmp_str = p_target;
			if( std::regex_match(tmp_str.cbegin(), tmp_str.cend(), match_res, reg_obj) )
			{
				func(match_res, holder);
			}
		}
		
	}

 }

int test_hq_funcs(const char *pszHqSvrIP, short nPort)
{
    //开始获取行情数据
    char* Result = new char[1024 * 1024];
    char* ErrInfo = new char[256];
    short Count = 10;

    //连接服务器
    bool bool1 = TdxHq_Connect(pszHqSvrIP, nPort, Result, ErrInfo);
    if (!bool1)
    {
        cout << ErrInfo << endl;
        return -1;
    }

    std::cout << Result << std::endl;

#if F1
    cout << "\n*** TdxHq_GetSecurityCount\n";

    Count = -1;
    bool1 = TdxHq_GetSecurityCount(0, &Count, ErrInfo);
    if (!bool1)
    {
        cout << ErrInfo << endl;//连接失败
        getchar();
        return 0;
    }

    cout << "Count = " << Count << endl;
    getchar();
#endif

#if F2
    cout << "\n*** TdxHq_GetSecurityList\n";

    Count = 100;
    bool1 = TdxHq_GetSecurityList(0, 0, &Count, Result, ErrInfo);
    if (!bool1)
    {
        cout << ErrInfo << endl;//连接失败
        getchar();
        return 0;
    }

    cout << Result << endl;
    cout << "Count = " << Count << endl;
    getchar();
#endif

#if F3
    cout << "\n*** TdxHq_GetMinuteTimeData\n";

    //获取分时图数据
    bool1 = TdxHq_GetMinuteTimeData(0, "000001",  Result, ErrInfo);
    if (!bool1)
    {
        cout << ErrInfo << endl;
        return 0;
    }

    cout << Result << endl;
    getchar();
#endif

#if F4
    cout << "\n*** TdxHq_GetSecurityBars\n";

    //获取股票K线数据  保存了实际返回的K线数目, 最大值800； 
    
	/* 时间    开盘价  收盘价  最高价  最低价  成交量  成交额
	20180111  43.350000  43.820000  44.240000  43.190000  12231884 536383680.000000
	"^([\\d]{8})\\s([\\d]+\\.[\\d]+)\\s([\\d]+\\.[\\d]+)\\s([\\d]+\\.[\\d]+)\\s([\\d]+\\.[\\d]+)\\s([\\d]+)\\s([\\d]+\\.[\\d]+)$"
	*/
	int start = 0;
	std::list<T_KlineDataItem>  stk_kline_date_list;
#if 0	 
	static auto fetch_dayk_data = [](std::smatch &result_match, std::list<T_KlineDataItem> &holder)
	{
		//auto val = std::stoi(result_match[1]);
		T_KlineDataItem k_data( std::stoi(result_match[1]), 0, std::stod(result_match[2]), std::stod(result_match[3])
			, std::stod(result_match[4]), std::stod(result_match[5]), std::stod(result_match[6]), std::stod(result_match[7]));
		holder.push_back(std::move(k_data));
	};
	 
	start = 0;
	Count = 800;
	int kline_type = 4;
	int total_k_num = 0;
	while( Count == 800 )
	{
		bool1 = TdxHq_GetSecurityBars(kline_type, 1/*market*/, "600196", start, &Count, Result, ErrInfo);//数据种类, 0->5分钟K线    1->15分钟K线    2->30分钟K线  3->1小时K线    4->日K线  5->周K线  6->月K线  7->1分钟K线  8->1分钟K线  9->日K线  10->季K线  11->年K线
		if (!bool1)
		{
			std::cout << ErrInfo << endl;
			return 0;
		}
		//std::cout << Result << endl;
		ParseData(Result, "^([\\d]{8})\\s([\\d]+\\.[\\d]+)\\s([\\d]+\\.[\\d]+)\\s([\\d]+\\.[\\d]+)\\s([\\d]+\\.[\\d]+)\\s([\\d]+)\\s([\\d]+\\.[\\d]+)$"
			, fetch_dayk_data, stk_kline_date_list);
		std::cout << "=============================\n" << std::endl;

		start += Count; 
		total_k_num += Count;
	}
	std::cout << "=============end================\n" << std::endl;
	stk_kline_date_list.sort([](T_KlineDataItem& lh, T_KlineDataItem &rh){ return lh.date < rh.date; });

	std::for_each( std::begin(stk_kline_date_list), std::end(stk_kline_date_list), [](T_KlineDataItem &entry)
	{
		std::cout << entry.date << std::endl;
	});
#endif
	  
#if 0 
	std::cout << "=============60min================\n" << std::endl;
	// 时间    开盘价  收盘价  最高价  最低价  成交量  成交额 
	//2018-02-26 10:30        40.200000       40.110000       40.720000       39.730000       5595700 225395808.000000
	std::list<T_KlineDataItem>  stk_kline_60m_list;
	static auto fetch_hourk_data = [](std::smatch &result_match, std::list<T_KlineDataItem> &holder)
	{
		int date = std::stod(result_match[1]) * 10000 + std::stod(result_match[2]) * 100 + std::stod(result_match[3]);
		int time = std::stod(result_match[4]) * 100 + std::stod(result_match[5]); //hhmm
		T_KlineDataItem k_data( date, time, std::stod(result_match[6]), std::stod(result_match[7])
			, std::stod(result_match[8]), std::stod(result_match[9]), std::stod(result_match[10]), std::stod(result_match[11]));
		holder.push_back(std::move(k_data));
	};

	start = 0;
	Count = 100;
	int kline_type = 2;
	int total_k_num = 0;
	 
	bool1 = TdxHq_GetSecurityBars(kline_type, 1/*market*/, "600196", start, &Count, Result, ErrInfo);//数据种类, 0->5分钟K线    1->15分钟K线    2->30分钟K线  3->1小时K线    4->日K线  5->周K线  6->月K线  7->1分钟K线  8->1分钟K线  9->日K线  10->季K线  11->年K线
	if (!bool1)
	{
		std::cout << ErrInfo << endl;
		return 0;
	}
	std::cout << Result << endl;
 
	ParseData(Result, "^([\\d]{4})-([\\d]{2})-([\\d]{2})\\s([\\d]{2}):([\\d]{2})\\s([\\d]+\\.[\\d]+)\\s([\\d]+\\.[\\d]+)\\s([\\d]+\\.[\\d]+)\\s([\\d]+\\.[\\d]+)\\s([\\d]+)\\s([\\d]+\\.[\\d]+)$"
		, fetch_hourk_data, stk_kline_60m_list);
	std::cout << "=============================\n" << std::endl;
	start += Count; 
	total_k_num += Count; 
#endif

	std::cout << "=============30min================\n" << std::endl;
	//时间    开盘价  收盘价  最高价  最低价  成交量  成交额 
	//2018-05-30 10:00        44.690000       44.260000       44.690000       43.300000  10160700 447412544.000000
	//2018-05-30 10:30        44.400000       44.380000       45.050000       44.250000  3747800	167260096.000000
	std::list<T_KlineDataItem>  stk_kline_min_list;
	static auto fetch_hourk_data = [](std::smatch &result_match, std::list<T_KlineDataItem> &holder)
	{
		int date = std::stod(result_match[1]) * 10000 + std::stod(result_match[2]) * 100 + std::stod(result_match[3]);
		int time = std::stod(result_match[4]) * 100 + std::stod(result_match[5]); //hhmm
		T_KlineDataItem k_data( date, time, std::stod(result_match[6]), std::stod(result_match[7])
			, std::stod(result_match[8]), std::stod(result_match[9]), std::stod(result_match[10]), std::stod(result_match[11]));
		holder.push_back(std::move(k_data));
	};

	start = 0;
	Count = 100;
	int kline_type = 7;  // 1 min 
	int total_k_num = 0;
	 
	bool1 = TdxHq_GetSecurityBars(kline_type, 1/*market*/, "600196", start, &Count, Result, ErrInfo);//数据种类, 0->5分钟K线    1->15分钟K线    2->30分钟K线  3->1小时K线    4->日K线  5->周K线  6->月K线  7->1分钟K线  8->1分钟K线  9->日K线  10->季K线  11->年K线
	if (!bool1)
	{
		std::cout << ErrInfo << endl;
		return 0;
	}
	std::cout << Result << endl;
#if 1
	ParseData(Result, "^([\\d]{4})-([\\d]{2})-([\\d]{2})\\s([\\d]{2}):([\\d]{2})\\s([\\d]+\\.[\\d]+)\\s([\\d]+\\.[\\d]+)\\s([\\d]+\\.[\\d]+)\\s([\\d]+\\.[\\d]+)\\s([\\d]+)\\s([\\d]+\\.[\\d]+)$"
		, fetch_hourk_data, stk_kline_min_list);
	std::cout << "=============================\n" << std::endl;
#endif
	start += Count; 
	total_k_num += Count; 
	 
	getchar();

#if 0
	start = 800;
	Count = 800;
	bool1 = TdxHq_GetSecurityBars(4, 1/*market*/, "600196", start, &Count, Result, ErrInfo);//数据种类, 0->5分钟K线    1->15分钟K线    2->30分钟K线  3->1小时K线    4->日K线  5->周K线  6->月K线  7->1分钟K线  8->1分钟K线  9->日K线  10->季K线  11->年K线
    if (!bool1)
    {
        cout << ErrInfo << endl;
        return 0;
    }
	cout << Result << endl;

	Count = 800;
	start = 0;
    //bool1 = TdxHq_GetSecurityBars(8, 0, "000001", 100, &Count, Result, ErrInfo);//数据种类, 0->5分钟K线    1->15分钟K线    2->30分钟K线  3->1小时K线    4->日K线  5->周K线  6->月K线  7->1分钟K线  8->1分钟K线  9->日K线  10->季K线  11->年K线
    //bool1 = TdxHq_GetSecurityBars(4, 1, "160135", 100, &Count, Result, ErrInfo);//数据种类, 0->5分钟K线    1->15分钟K线    2->30分钟K线  3->1小时K线    4->日K线  5->周K线  6->月K线  7->1分钟K线  8->1分钟K线  9->日K线  10->季K线  11->年K线
    bool1 = TdxHq_GetSecurityBars(4, 1/*market*/, "600196", start, &Count, Result, ErrInfo);//数据种类, 0->5分钟K线    1->15分钟K线    2->30分钟K线  3->1小时K线    4->日K线  5->周K线  6->月K线  7->1分钟K线  8->1分钟K线  9->日K线  10->季K线  11->年K线
    if (!bool1)
    {
        cout << ErrInfo << endl;
        return 0;
    } 
    cout << Result << endl;
    cout << "Count = " << Count << endl;
	cout << endl;
#endif
	 
    getchar();
#endif






#if F5
    cout << "\n*** TdxHq_GetHistoryMinuteTimeData\n";

    //获取历史分时图数据
    bool1 = TdxHq_GetHistoryMinuteTimeData(0, "000001", 20140904, Result, ErrInfo);
    if (!bool1)
    {
        cout << ErrInfo << endl;
        return 0;
    }

    cout << Result << endl;
    getchar();
#endif

#if F6
    cout << "\n*** TdxHq_GetIndexBars\n";

    //获取指数K线数据
    Count = 100;
    bool1 = TdxHq_GetIndexBars(4, 1, "000001", 0, &Count, Result, ErrInfo);//数据种类, 0->5分钟K线    1->15分钟K线    2->30分钟K线  3->1小时K线    4->日K线  5->周K线  6->月K线  7->1分钟K线     8->1分钟K线    9->日K线  10->季K线  11->年K线
    //bool1 = TdxHq_GetIndexBars(4, 0, "880557", 0, &Count, Result, ErrInfo);//数据种类, 0->5分钟K线    1->15分钟K线    2->30分钟K线  3->1小时K线    4->日K线  5->周K线  6->月K线  7->1分钟K线     8->1分钟K线    9->日K线  10->季K线  11->年K线
    //bool1 = TdxHq_GetIndexBars(4, 1, "880380", 0, &Count, Result, ErrInfo);//数据种类, 0->5分钟K线    1->15分钟K线    2->30分钟K线  3->1小时K线    4->日K线  5->周K线  6->月K线  7->1分钟K线     8->1分钟K线    9->日K线  10->季K线  11->年K线
    //bool1 = TdxHq_GetIndexBars(4, 0, "880380", 0, &Count, Result, ErrInfo);//数据种类, 0->5分钟K线    1->15分钟K线    2->30分钟K线  3->1小时K线    4->日K线  5->周K线  6->月K线  7->1分钟K线     8->1分钟K线    9->日K线  10->季K线  11->年K线
    if (!bool1)
    {
        cout << ErrInfo << endl;
        return 0;
    }

    cout << Result << endl;
    cout << "Count = " << Count << endl;
    getchar();
#endif

#if F7
    cout << "\n*** TdxHq_GetTransactionData\n";

    //获取分笔图数据 2000
    Count = 100;
    bool1 = TdxHq_GetTransactionData(1/*market*/, "600196", 0, &Count, Result, ErrInfo);
    if (!bool1)
    {
        cout << ErrInfo << endl;
        return 0;
    }

    cout << Result << endl;

    cout << "Count = " << Count << endl;
    getchar();
#endif

#if F8
    cout << "\n*** TdxHq_GetHistoryTransactionData\n";

    //获取历史分笔图数据 精确到分,一分数笔成交
    Count = 100;
    //bool1 = TdxHq_GetHistoryTransactionData(0, "000001", 0, &Count, 20140904,  Result, ErrInfo);
    bool1 = TdxHq_GetHistoryTransactionData(1, "600196", 0, &Count, 20140904,  Result, ErrInfo);
    if (!bool1)
    {
        cout << ErrInfo << endl;
        return 0;
    }

    cout << Result << endl;
	// 时间    价格    现量    笔数    买卖    保留
	// 14:54   44.389999       12      0       0       0
	std::string  str_result;
	std::regex reg_obj("^([\\d]+)\\s$");
	std::smatch  match_res;

	char *p_target = Result;
	char *q = Result;

	while( *q != '\0' )
	{
		while( *q != '\n' && *q != '\0' ) ++q;
		
		if( *q != '\0' )
		{
			std::string tmp_str = p_target;
			if( std::regex_match(tmp_str.cbegin(), tmp_str.cend(), match_res, reg_obj) )
			{

			}

			*q = '\0';
		}
		  
		++q;
	}

	 

	std::regex_match(str_result.cbegin(), str_result.cend(), match_res, reg_obj);

    cout << "Count = " << Count << endl;

    getchar();
#endif

#if F9
    cout << "\n*** TdxHq_GetSecurityQuotes\n";

    //获取五档报价数据
    char xMarket[] = {0,1, 0, 1 };
    const char* Zqdm[] = { "000001", "600030", "000002", "601288" };
    short ZqdmCount = 4;
    bool1 = TdxHq_GetSecurityQuotes(xMarket, Zqdm, &ZqdmCount, Result, ErrInfo);
    if (!bool1)
    {
        cout << ErrInfo << endl;
        return 0;
    }

    cout << Result << endl;
    getchar();
#endif

#if F10
    cout << "\n*** TdxHq_GetCompanyInfoCategory\n";

    //获取F10数据的类别
    bool1 = TdxHq_GetCompanyInfoCategory(0, "000001", Result, ErrInfo);
    if (!bool1)
    {
        cout << ErrInfo << endl;
        return 0;
    }

    cout << Result << endl;
    getchar();
#endif

#if F11
    cout << "\n*** TdxHq_GetCompanyInfoContent\n";

    //获取F10数据的某类别的内容
    bool1 = TdxHq_GetCompanyInfoContent(1, "600030", "600030.txt", 142577, 5211, Result, ErrInfo);
    if (!bool1)
    {
        cout << ErrInfo << endl;
        return 0;
    }

    cout << Result << endl;
    getchar();
#endif

#if F12
    cout << "\n*** TdxHq_GetXDXRInfo\n";

    //获取除权除息信息
    bool1 = TdxHq_GetXDXRInfo(0, "000001", Result, ErrInfo);
    if (!bool1)
    {
        cout << ErrInfo << endl;
        return 0;
    }

    cout << Result << endl;
    getchar();
#endif

#if F13
    cout << "\n*** TdxHq_GetFinanceInfo\n";

    //获取财务信息
    bool1 = TdxHq_GetFinanceInfo(0, "000001", Result, ErrInfo);
    if (!bool1)
    {
        cout << ErrInfo << endl;
        return 0;
    }

    cout << Result << endl;
    getchar();
#endif

    TdxHq_Disconnect();

    cout << "已经断开行情服务器" << endl;

    getchar();
    return 0;
}


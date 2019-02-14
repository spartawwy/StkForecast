#include "tdx_hq_wrapper.h"

#include <boost/lexical_cast.hpp>
#include <string>
#include <iostream>
#include <cassert>
#include <regex>

#include "tdxhq/HqApi.h"
#include "exchange_calendar.h"

#include <qdebug.h>

#pragma comment(lib, "TradeX2-M.lib")

TdxHqWrapper::TdxHqWrapper(ExchangeCalendar  *exchange_calendar)
    : conn_handle_(0)
    , exchange_calendar_(exchange_calendar)
{

}

TdxHqWrapper::~TdxHqWrapper() 
{ 
    if( conn_handle_ ) 
        TdxHq_Disconnect(conn_handle_); 
}

bool TdxHqWrapper::Init()
{
   return ConnectServer();
}

bool TdxHqWrapper::ConnectServer()
{
    //开始获取行情数据

    const int cst_result_len = 1024 * 1024;
    const int cst_err_len = 1024; 

    char* m_szResult = new char[cst_result_len];
    char* m_szErrInfo = new char[cst_err_len];

    memset(m_szResult, 0, cst_result_len);
    memset(m_szErrInfo, 0, cst_err_len);

    short Count = 10;

    //连接服务器
    conn_handle_ = TdxHq_Connect("218.75.126.9", 7709, m_szResult, m_szErrInfo);
    if( conn_handle_ < 0 )
    {
        std::cout << m_szErrInfo << std::endl;
        return false;
    }

    std::cout << m_szResult << std::endl;

    return true;
}

// items date is from small to big
bool TdxHqWrapper::GetHisKBars(const std::string &code, bool is_index, TypePeriod kbar_type, int start_date, int end_date, std::vector<T_StockHisDataItem> &items)
{
    bool (WINAPI* pFuncGetSecurityKData)(int nConnID,
                                char nCategory,
                                char nMarket,
                                const char *pszZqdm,
                                short nStart,
                                short *pnCount,
                                char *pszResult,
                                char *pszErrInfo);
    if( is_index )
        pFuncGetSecurityKData = &TdxHq_GetIndexBars;
    else 
        pFuncGetSecurityKData = &TdxHq_GetSecurityBars;

    assert(exchange_calendar_);
    std::vector<T_K_Data> resut;
    const int cst_result_len = 1024 * 1024;
    const int cst_err_len = 1024; 

    char* m_szResult = new char[cst_result_len];
    char* m_szErrInfo = new char[cst_err_len];

    memset(m_szResult, 0, cst_result_len);
    memset(m_szErrInfo, 0, cst_err_len);
    
    //获取股票K线数据
    
    //数据种类, 0->5分钟K线    1->15分钟K线    2->30分钟K线  3->1小时K线    4->日K线  5->周K线  6->月K线  7->1分钟K线  8->1分钟K线  9->日K线  10->季K线  11->年K线
    int ktype = 4;
    switch(kbar_type)
    {
    case TypePeriod::PERIOD_YEAR: ktype = 11; break;
    case TypePeriod::PERIOD_MON:  ktype = 6; break;
    case TypePeriod::PERIOD_WEEK: ktype = 5; break;
    case TypePeriod::PERIOD_DAY:  ktype = 4; break;
    case TypePeriod::PERIOD_HOUR: ktype = 3; break;
    case TypePeriod::PERIOD_30M:  ktype = 2; break;
    case TypePeriod::PERIOD_15M:  ktype = 1; break;
    case TypePeriod::PERIOD_5M:   ktype = 0; break;
    default:
        assert(false);
        ktype = 4; 
        break;
    }
    int market_type = MARKET_TYPE_SH;
    if( is_index )
    {
        if( code.at(0) == '3' )
            market_type = MARKET_TYPE_SZ;
        else
            market_type = MARKET_TYPE_SH;
    }else if( code.at(0) == '6' )
        market_type = MARKET_TYPE_SH;
    else 
        market_type = MARKET_TYPE_SZ;

    auto tuple_index_len = exchange_calendar_->GetStartDateAndLen_backforward(start_date, end_date);
    
    short start = std::get<0>(tuple_index_len);  // back forward
    short count = std::get<1>(tuple_index_len);
    bool bool1 = pFuncGetSecurityKData(conn_handle_, ktype, market_type, const_cast<char*>(code.c_str()), start, &count, m_szResult, m_szErrInfo);
    if( !bool1 )
    { 
        if( ConnectServer() )
        {
            bool1 = pFuncGetSecurityKData(conn_handle_, ktype, market_type, const_cast<char*>(code.c_str()), start, &count, m_szResult, m_szErrInfo);
            if( !bool1 )
                return false;
        } else
            return false;
    }
    if( strlen(m_szResult) < 1 )
    {
        std::cout << " result empty !" << std::endl;
        return false;
    }
    qDebug() << m_szResult << "\n";

    const bool has_time = ( ktype < 4 || ktype == 7 || ktype == 8 ) ? true : false;
    std::string expresstion_str;
    if( has_time )
        //expresstion_str = "^(\\d{4}-\\d{2}-\\d{2})\\s(\\d{2}:\\d{2})\\t(\\d+\\.\\d+)\\t(\\d+\\.\\d+)\\t(\\d+\\.\\d+)\\t(\\d+\\.\\d+)\\t(\\d+)\\t(\\d+\\.\\d+)$";
        expresstion_str = "^(\\d{4}-\\d{2}-\\d{2})\\s+(\\d{2}:\\d{2})\\s+(\\d+\\.\\d+)\\s+(\\d+\\.\\d+)\\s+(\\d+\\.\\d+)\\s+(\\d+\\.\\d+)\\s+(\\d+)\\s+(\\d+\\.\\d+).*";
    else
        expresstion_str = "^(\\d{8})\\s+(\\d+\\.\\d+)\\s+(\\d+\\.\\d+)\\s+(\\d+\\.\\d+)\\s+(\\d+\\.\\d+)\\s+(\\d+)\\s+(\\d+\\.\\d+).*";
        //expresstion_str = "^(\\d{8})\\t(\\d+\\.\\d+)\\t(\\d+\\.\\d+)\\t(\\d+\\.\\d+)\\t(\\d+\\.\\d+)\\t(\\d+)\\t(\\d+\\.\\d+)$";

    std::regex  regex_obj(expresstion_str);
    char *p = m_szResult;
    while( *p != '\0' && *p != '\n') ++p;

    ++p;

    char *p_line_beg = p;
    while( *p != '\0')
    {
        p_line_beg = p;
        while( *p != '\0' && *p != '\n')++p;
        *p = '\0';
        ++p;
        std::string src_str = p_line_beg;

        std::smatch  match_result;
        if( std::regex_match(src_str.cbegin(), src_str.cend(), match_result, regex_obj) )
        {
            int index = 1;
            T_StockHisDataItem  k_data;
            try
            {
                std::cout << match_result[index] << " "; // date
                k_data.date = boost::lexical_cast<int>(match_result[index]);
                ++index;
                if( has_time )
                {
                    k_data.hhmmss = boost::lexical_cast<int>(match_result[index]);
                    ++index;
                }
                k_data.open_price = boost::lexical_cast<double>(match_result[index]);
                ++index;
                k_data.close_price = boost::lexical_cast<double>(match_result[index]);
                ++index;
                k_data.high_price = boost::lexical_cast<double>(match_result[index]);
                ++index;
                k_data.low_price = boost::lexical_cast<double>(match_result[index]);
                ++index;
                k_data.vol = boost::lexical_cast<double>(match_result[index]);
                ++index;
                k_data.capital = boost::lexical_cast<double>(match_result[index]);
                items.push_back(std::move(k_data));

            }catch(boost::exception& e )
            {
                e = e; 
            }
        }else
            qDebug() << "match fail!\n";
    }
    //std::cout << m_szResult << std::endl;
    return true;
}

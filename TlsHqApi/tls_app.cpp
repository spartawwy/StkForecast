#include "tls_app.h"

#include <direct.h>
#include <regex>
#include <list>
#include <Windows.h>
  
#include "HqApi.h"
 
#include "rwini.h"
#include "tls_hq_common.h"

#pragma comment(lib, "TradeX.lib")


template<typename FUNCTYPE, typename HOLDER_TYPE>
void ParseData(char *src, const std::string &regex_str, FUNCTYPE func, HOLDER_TYPE &holder)
{
    std::string  str_result;
    std::regex reg_obj(regex_str);
    std::smatch  match_res;

    char *p_target = src;
    char *q = src;
    bool is_first_line = true;
    int index = 0;
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
                func(match_res, holder, index++);
            ++q;
            p_target = q;
        }else 
        {
            std::string tmp_str = p_target;
            if( std::regex_match(tmp_str.cbegin(), tmp_str.cend(), match_res, reg_obj) )
                func(match_res, holder, index++);
        }
    }
}

template<typename FUNCTYPE, typename PARATYPE>
void ParseDataUseCallBack(char *src, const std::string &regex_str, FUNCTYPE func, PARATYPE para)
{
    static auto fill_item = []( std::smatch &result_match, FUNCTYPE func, PARATYPE para)
    {
        T_BarDataItem item;
        int date = std::stoi(result_match[1]) * 10000 + std::stoi(result_match[2]) * 100 + std::stoi(result_match[3]);
        int time = std::stoi(result_match[4]) * 100 + std::stoi(result_match[5]); //hhmm
        item.date = date;
        item.time = time;
        item.open_price = std::stod(result_match[6]);
        item.close_price = std::stod(result_match[7]);
        item.high_price = std::stod(result_match[8]);
        item.low_price = std::stod(result_match[9]);
        item.vol = std::stod(result_match[10]);
        item.amount = std::stod(result_match[11]);
        func(para, &item);
    };
    std::string  str_result;
    std::regex reg_obj(regex_str);
    std::smatch  match_res;

    char *p_target = src;
    char *q = src;
    bool is_first_line = true;
    int index = 0;
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
                fill_item(match_res, func, para); 
            } 
            ++q;
            p_target = q;
        }else 
        {
            std::string tmp_str = p_target;
            if( std::regex_match(tmp_str.cbegin(), tmp_str.cend(), match_res, reg_obj) )
            {
                fill_item(match_res, func, para);
                /*T_BarDataItem data_item;
                fill_item(match_res, data_item);
                func(match_res, para, &data_item);*/
            }
        }
    }// while
}

TlsApp& TlsApp::GetInstance()
{
    static TlsApp tls_app;
    return tls_app;
}

TlsApp::TlsApp()
    : tdx_server_ip_()
    , tdx_server_port_(0)
{ 
    char buf[1024] = {0};
    GetModuleFileName(NULL, buf, sizeof(buf));
    std::string path = buf;
    path = path.substr(0, path.rfind("\\")); 
    work_dir_ = path;
    path += "\\TlshqApi.ini";

    Crwini  ini_obj(path.c_str());
    tdx_server_ip_ = ini_obj.ReadString("TDXCFG", "server_ip");
    tdx_server_port_ = ini_obj.ReadInt("TDXCFG", "server_port");

}

TlsApp::~TlsApp()
{
    TdxHq_Disconnect();
}

bool TlsApp::Init(int enable_tdx, int enable_python)
{ 
    enable_tdx_ = bool(enable_tdx);
    enable_python_ = bool(enable_python);

    char stk_data_dir[256] = {0};
    unsigned int ret_env_size = sizeof(stk_data_dir);
    getenv_s(&ret_env_size, stk_data_dir, ret_env_size, "STK_DATA_DIR");
    if( strlen(stk_data_dir) == 0 )
    {
        stk_data_dir_ = work_dir_ +"/StockHisData";
       _mkdir(stk_data_dir_.c_str()); 
    }else
    {
        stk_data_dir_ = stk_data_dir;
        if( stk_data_dir_.at(stk_data_dir_.size() - 1) == '\\' || stk_data_dir_.at(stk_data_dir_.size() - 1) == '/' )
            stk_data_dir_.resize(stk_data_dir_.size() - 1);
    } 

    bool ret = Reconnect();
    return ret;
}

bool TlsApp::Reconnect()
{
    //TdxHq_Disconnect();
    char* Result = new char[1024 * 1024];
    char* ErrInfo = new char[256];
    short Count = 10;
    if( enable_tdx_ )
    {
        //connect server 
        bool bool1 = TdxHq_Connect(tdx_server_ip_.c_str(), tdx_server_port_, Result, ErrInfo);
        if (!bool1)
        {
            //cout << ErrInfo << endl;
            WriteLog("TlsApp::Init TdxHq_Connect fail:%s %s", Result, ErrInfo);
            return false;
        }
    }
    return true;
}

bool TlsApp::GetBarDataItems(
    char nCategory,
    char nMarket,
    const char *pszZqdm,
    short nStart,
    short *nCount,
    T_BarDataItem **ret_items,
    char *pszErrInfo)
{

    //std::cout << "=============30min================\n" << std::endl;
	//时间    开盘价  收盘价  最高价  最低价  成交量  成交额 
	//2018-05-30 10:00        44.690000       44.260000       44.690000       43.300000  10160700 447412544.000000
	//2018-05-30 10:30        44.400000       44.380000       45.050000       44.250000  3747800	167260096.000000
	//std::list<T_BarDataItem>  stk_kline_min_list;
	static auto fetch_min_data = [](std::smatch &result_match, T_BarDataItem *p_items, int index)
	{
        //assert(index >= 0);
		int date = std::stoi(result_match[1]) * 10000 + std::stoi(result_match[2]) * 100 + std::stoi(result_match[3]);
		int time = std::stoi(result_match[4]) * 100 + std::stoi(result_match[5]); //hhmm
		/*T_BarDataItem k_data( date, time, std::stod(result_match[6]), std::stod(result_match[7])
			, std::stod(result_match[8]), std::stod(result_match[9]), std::stod(result_match[10]), std::stod(result_match[11]));
		holder.push_back(std::move(k_data));*/
        p_items[index].date = date;
        p_items[index].time = time;
        p_items[index].open_price = std::stod(result_match[6]);
        p_items[index].close_price = std::stod(result_match[7]);
        p_items[index].high_price = std::stod(result_match[8]);
        p_items[index].low_price = std::stod(result_match[9]);
        p_items[index].vol = std::stod(result_match[10]);
        p_items[index].amount = std::stod(result_match[11]);
	};
    if( enable_tdx_ )
    {
        char* result = new char[1024 * 1024];
        bool ret = 
        TdxHq_GetSecurityBars(nCategory, nMarket, pszZqdm, nStart, nCount, result, pszErrInfo);
        if( !ret )
            return false;
        if( *nCount > 0 )
            *ret_items = new T_BarDataItem[*nCount < 512 ? 512 : *nCount];

        ParseData(result, "^([\\d]{4})-([\\d]{2})-([\\d]{2})\\s([\\d]{2}):([\\d]{2})\\s([\\d]+\\.[\\d]+)\\s([\\d]+\\.[\\d]+)\\s([\\d]+\\.[\\d]+)\\s([\\d]+\\.[\\d]+)\\s([\\d]+)\\s([\\d]+\\.[\\d]+)$"
            , fetch_min_data, *ret_items);
        return true;
    }
    return false;
}

bool TlsApp::GetDarDataItemsUseCallBack(
    char nCategory,
    char nMarket,
    const char *pszZqdm,
    short nStart,
    short *nCount,
    BarDataCallBack call_back,
    void *call_back_para,
    char *pszErrInfo)
{  
    if( enable_tdx_ )
    {
        char* result = new char[1024 * 1024];
        bool ret = 
        TdxHq_GetSecurityBars(nCategory, nMarket, pszZqdm, nStart, nCount, result, pszErrInfo);
        if( !ret )
            return false;  
        ParseDataUseCallBack(result, "^([\\d]{4})-([\\d]{2})-([\\d]{2})\\s([\\d]{2}):([\\d]{2})\\s([\\d]+\\.[\\d]+)\\s([\\d]+\\.[\\d]+)\\s([\\d]+\\.[\\d]+)\\s([\\d]+\\.[\\d]+)\\s([\\d]+)\\s([\\d]+\\.[\\d]+)$"
            , call_back, call_back_para);
        return true;
    }
    return false;
}
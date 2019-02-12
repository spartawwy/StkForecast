#include "stkfo_common.h"
#include <algorithm>

bool IsNumber(const std::string& str)
{
	//bool ret = true;
	//std::for_each( std::begin(str), std::end(str), [&ret](char entry)
	for( unsigned int i = 0; i < str.length(); ++i )
	{
		if( str.at(i) < '0' || str.at(i) > '9' )
		  return false;
	}
	return true;
	/*{
		if( entry < '\0' || entry > '\9' )
		{
			ret = false;
		}
	}*/
}


std::string TransIndexPinYin2Code(const std::string &code)
{
    if( code == "SZZS" ) //上证指数
        return "000001";
    else if( code == "SZCZ" ) // 深圳成指
        return "399001"; 
    else if( code == "ZXBZ" ) //中小板指数
        return "399005"; 
    else if( code == "CYBZ" ) //创业板指数
        return "399006";
    else if( code == "SZ50" ) // 上证50
        return "000016"; 
    else if( code == "HS300" ) // 沪深300 
        return "000300"; 
    return code;
}

std::string TransIndex2TusharedCode(const std::string &code)
{
    if( code == "999999" || code == "SZZS" ) //上证指数
        return "000001";
    else if( code == "399001" || code == "SZCZ" ) // 深圳成指
        return "399001"; 
    else if( code == "399005" || code == "ZXBZ" ) //中小板指数
        return "399005"; 
    else if( code == "399006" || code == "CYBZ" ) //创业板指数
        return "399006";
    else if( code == "000016" || code == "SZ50" ) // 上证50
        return "000016"; 
    else if( code == "000300" || code == "HS300" ) // 沪深300 
        return "000300"; 
    return code;
}

FractalType  MaxFractalType(int val)
{
    if( (val & int(FractalType::TOP_AXIS_T_11)) == int(FractalType::TOP_AXIS_T_11) )
        return FractalType::TOP_AXIS_T_11;
    else if( (val & int(FractalType::TOP_AXIS_T_9)) == int(FractalType::TOP_AXIS_T_9) )
        return FractalType::TOP_AXIS_T_9;
    else if( (val & int(FractalType::TOP_AXIS_T_7)) == int(FractalType::TOP_AXIS_T_7) )
        return FractalType::TOP_AXIS_T_7;
    else if( (val & int(FractalType::TOP_AXIS_T_5)) == int(FractalType::TOP_AXIS_T_5) )
        return FractalType::TOP_AXIS_T_5;
    else if( (val & int(FractalType::TOP_AXIS_T_3)) == int(FractalType::TOP_AXIS_T_3) )
        return FractalType::TOP_AXIS_T_3;

    else if( (val & int(FractalType::BTM_AXIS_T_11)) == int(FractalType::BTM_AXIS_T_11) )
        return FractalType::BTM_AXIS_T_11;
    else if( (val & int(FractalType::BTM_AXIS_T_9)) == int(FractalType::BTM_AXIS_T_9) )
        return FractalType::BTM_AXIS_T_9;
    else if( (val & int(FractalType::BTM_AXIS_T_7)) == int(FractalType::BTM_AXIS_T_7) )
        return FractalType::BTM_AXIS_T_7;
    else if( (val & int(FractalType::BTM_AXIS_T_5)) == int(FractalType::BTM_AXIS_T_5) )
        return FractalType::BTM_AXIS_T_5;
    else if( (val & int(FractalType::BTM_AXIS_T_3)) == int(FractalType::BTM_AXIS_T_3) )
        return FractalType::BTM_AXIS_T_3;
    else
        return FractalType::UNKNOW_FRACTAL;
}


FractalType  BtmestFractalType(int val)
{
    if( (val & int(FractalType::BTM_AXIS_T_11)) == int(FractalType::BTM_AXIS_T_11) )
        return FractalType::BTM_AXIS_T_11;
    else if( (val & int(FractalType::BTM_AXIS_T_9)) == int(FractalType::BTM_AXIS_T_9) )
        return FractalType::BTM_AXIS_T_9;
    else if( (val & int(FractalType::BTM_AXIS_T_7)) == int(FractalType::BTM_AXIS_T_7) )
        return FractalType::BTM_AXIS_T_7;
    else if( (val & int(FractalType::BTM_AXIS_T_5)) == int(FractalType::BTM_AXIS_T_5) )
        return FractalType::BTM_AXIS_T_5;
    else if( (val & int(FractalType::BTM_AXIS_T_3)) == int(FractalType::BTM_AXIS_T_3) )
        return FractalType::BTM_AXIS_T_3;
    else
        return FractalType::UNKNOW_FRACTAL;
}
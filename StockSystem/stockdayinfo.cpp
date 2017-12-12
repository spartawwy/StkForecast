#include "stockdayinfo.h"

#include <fstream>
#include <iostream>
#include <sstream>

//using namespace std;

StockDayInfo::StockDayInfo()
{

}

void StockDayInfo::SetStockNum(std::string stockNum)
{
        this->stockNum = stockNum;
}

std::string StockDayInfo::GetStockNum()
{
        return this->stockNum;
}
float StockDayInfo::GetChangeRate()
{
        return 100*(closePrice-openPrice)/openPrice;
}

//将strLine中的所有字段拆解并赋值给stockDayInfo的各个数据成员
void StockDayInfo::ReadData(std::string strLine)
{
    std::istringstream isstr(strLine);

    std::string transacteDate;

    isstr>>stockNum
        >>transacteDate
        >>openPrice
        >>closePrice
        >>maxPrice
        >>minPrice
        >>marketMoney
        >>marketAmount;
    auto iter = transacteDate.find("-");
    if( iter != std::string::npos )
    {
       date = atoi(transacteDate.substr(0, 4).c_str()) * 10000 + atoi(transacteDate.substr(5, 2).c_str()) * 100 + atoi(transacteDate.substr(9, 2).c_str());
    }
}

void StockDayInfo::Display()
{
    std::cout<<stockNum << "\t"
            << date << "\t"
            << openPrice << "\t"
            << closePrice << "\t"
            << maxPrice << "\t"
            << minPrice << "\t"
            << marketMoney << "\t"
            << marketAmount << std::endl;
}

float StockDayInfo::GetMinPrice()
{
    return this->minPrice;
}

float StockDayInfo::GetMaxPrice()
{
    return this->maxPrice;
}


float StockDayInfo::GetOpenPrice()
{
    return this->openPrice;
}

float StockDayInfo::GetClosePrice()
{
    return this->closePrice;
}

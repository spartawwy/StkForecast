#include "forcast_man.h"

ForcastMan::ForcastMan()
    : stock_2pdown_forcast_30m_(1024)
    , stock_2pdown_forcast_h_(1024)
    , stock_2pdown_forcast_d_(1024)
    , stock_2pdown_forcast_w_(1024)
    , stock_2pdown_forcast_mon_(1024)
    , stock_2pup_forcast_30m_(1024)
    , stock_2pup_forcast_h_(1024)
    , stock_2pup_forcast_d_(1024)
    , stock_2pup_forcast_w_(1024)
    , stock_2pup_forcast_mon_(1024)
{

}

void ForcastMan::Append(TypePeriod type_period, const std::string &code, T_Data2pDownForcast &forcast_data)
{
    _Append2pForcast(type_period, code, forcast_data); 
}


void ForcastMan::Append(TypePeriod type_period, const std::string &code, T_Data2pUpForcast &forcast_data)
{
    _Append2pForcast(type_period, code, forcast_data);
}

std::vector<T_Data2pDownForcast> * ForcastMan::Find2pForcastDown(const std::string &code, TypePeriod type_period)
{
    switch (type_period)
    {
    case TypePeriod::PERIOD_DAY:
        {
            auto vector_iter = stock_2pdown_forcast_d_.find(code);
            if( vector_iter == stock_2pdown_forcast_d_.end() )
                return nullptr;
            return std::addressof(vector_iter->second);
        }
    default:
        return nullptr;
    }
    
}

bool ForcastMan::HasIn2pDownwardForcast(const std::string &code, TypePeriod type_period, T_KlineDataItem &item_a, T_KlineDataItem &item_b)
{
    static auto has_in2pforcasts = [this](std::vector<T_Data2pDownForcast>& data_vector, int date_a, int date_b)->bool
    { 
        if( data_vector.empty() ) return false;
        unsigned int i = 0;
        for( ; i < data_vector.size(); ++i )
        {
            if( data_vector.at(i).date_a == date_a && data_vector.at(i).date_b == date_b )
                break;
        }
        return i != data_vector.size();
    };

    if( item_a.stk_item.high_price > item_b.stk_item.high_price ) // forcast down
    {
        switch (type_period)
        {
        case TypePeriod::PERIOD_DAY:
            {
                auto vector_iter = stock_2pdown_forcast_d_.find(code);
                if( vector_iter == stock_2pdown_forcast_d_.end() )
                    return false;
                //std::vector<T_Data2pDownForcast> & vector_iter->second;
                return has_in2pforcasts(vector_iter->second, item_a.kline_posdata.date, item_b.kline_posdata.date);
            }
        default: 
            return false;
        }
    }
    
}

std::tuple<double, double, double>  ForcastC_ABDown(double a, double b)
{
    double c2 = sqrtf(a * b);
    double c1 = sqrtf(b * c2);
    double c3 = c2 * c2 / c1;
    return std::make_tuple(c1, c2, c3);
}

std::tuple<double, double, double>  ForcastC_ABUp(double a, double b)
{
    double c2 = sqrtf(a * b);
    double c1 = sqrtf(b * c2);
    double c3 = c2 * c2 / c1;
    return std::make_tuple(c1, c2, c3);
}
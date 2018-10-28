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
    , stock_3pdown_forcast_30m_(1024)
    , stock_3pdown_forcast_h_(1024)
    , stock_3pdown_forcast_d_(1024)
    , stock_3pdown_forcast_w_(1024)
    , stock_3pdown_forcast_mon_(1024)
    , stock_3pup_forcast_30m_(1024)
    , stock_3pup_forcast_h_(1024)
    , stock_3pup_forcast_d_(1024)
    , stock_3pup_forcast_w_(1024)
    , stock_3pup_forcast_mon_(1024)
    , df_no_use_(1)
    , uf_no_use_(1)
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

std::vector<T_Data2pDownForcast> * ForcastMan::Find2pDownForcastVector(const std::string &code, TypePeriod type_period)
{
    Code2pDownForcastType & holder_ref = Get2pDownDataHolder(type_period);
    auto vector_iter = holder_ref.find(code);
    if( vector_iter == stock_2pdown_forcast_d_.end() )
        return nullptr;
    return std::addressof(vector_iter->second);
}

std::vector<T_Data2pUpForcast> * ForcastMan::Find2pUpForcastVector(const std::string &code, TypePeriod type_period)
{
    Code2pUpForcastType & holder_ref = Get2pUpDataHolder(type_period);
    auto vector_iter = holder_ref.find(code);
    if( vector_iter == stock_2pup_forcast_d_.end() )
        return nullptr;
    return std::addressof(vector_iter->second);
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

    Code2pDownForcastType & code_2pdown_fcst = Get2pDownDataHolder(type_period);
    auto vector_iter = code_2pdown_fcst.find(code);
    if( vector_iter != code_2pdown_fcst.end() )
        return has_in2pforcasts(vector_iter->second, item_a.kline_posdata.date, item_b.kline_posdata.date);
    else 
        return false;
}

bool ForcastMan::HasIn2pUpForcast(const std::string &code, TypePeriod type_period, T_KlineDataItem &item_a, T_KlineDataItem &item_b)
{
    static auto has_in2pforcasts = [this](std::vector<T_Data2pUpForcast>& data_vector, int date_a, int date_b)->bool
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
    Code2pUpForcastType &code_2pup_fcst = Get2pUpDataHolder(type_period);
    auto vector_iter = code_2pup_fcst.find(code);
    if( vector_iter != code_2pup_fcst.end() )
        return has_in2pforcasts(vector_iter->second, item_a.kline_posdata.date, item_b.kline_posdata.date);
    else 
        return false;
}

Code2pUpForcastType & ForcastMan::Get2pUpDataHolder(TypePeriod type_period)
{
    switch (type_period)
    {
    case TypePeriod::PERIOD_30M: return stock_2pup_forcast_30m_;
    case TypePeriod::PERIOD_HOUR: return stock_2pup_forcast_h_;
    case TypePeriod::PERIOD_DAY: return stock_2pup_forcast_d_;
    case TypePeriod::PERIOD_WEEK: return stock_2pup_forcast_w_;
    case TypePeriod::PERIOD_MON: return stock_2pup_forcast_mon_;
    default: assert(false);
    }
    return uf_no_use_;
}

Code2pDownForcastType & ForcastMan::Get2pDownDataHolder(TypePeriod type_period)
{
    switch (type_period)
    {
    case TypePeriod::PERIOD_30M: return stock_2pdown_forcast_30m_;
    case TypePeriod::PERIOD_HOUR: return stock_2pdown_forcast_h_;
    case TypePeriod::PERIOD_DAY: return stock_2pdown_forcast_d_;
    case TypePeriod::PERIOD_WEEK: return stock_2pdown_forcast_w_;
    case TypePeriod::PERIOD_MON: return stock_2pdown_forcast_mon_;
    default: assert(false);
    }
    return df_no_use_;
}

Code3pForcastType & ForcastMan::Get3pDataHolder(TypePeriod type_period, bool is_down)
{
    switch (type_period)
    {
    case TypePeriod::PERIOD_30M:  return is_down ? stock_3pdown_forcast_30m_ : stock_3pup_forcast_30m_;
    case TypePeriod::PERIOD_HOUR: return is_down ? stock_3pdown_forcast_h_ : stock_3pup_forcast_h_;
    case TypePeriod::PERIOD_DAY:  return is_down ? stock_3pdown_forcast_d_ : stock_3pup_forcast_d_;
    case TypePeriod::PERIOD_WEEK: return is_down ? stock_3pdown_forcast_w_ : stock_3pup_forcast_w_;
    case TypePeriod::PERIOD_MON:  return is_down ? stock_3pdown_forcast_mon_ : stock_3pup_forcast_mon_;
    default: assert(false);
    }
    return no_use_3p_;
}

T_Data3pForcast * ForcastMan::Find3pForcast(const std::string &code, TypePeriod type_period, bool is_down_forward, T_KlineDataItem &item_a, T_KlineDataItem &item_b)
{
    /* static auto has_in3pforcasts = [this](std::vector<T_Data3pForcast>& data_vector, int date_a, int date_b)->bool
    { 
    if( data_vector.empty() ) return false;
    unsigned int i = 0;
    for( ; i < data_vector.size(); ++i )
    {
    if( data_vector.at(i).date_a == date_a && data_vector.at(i).date_b == date_b )
    break;
    }
    return i != data_vector.size();
    };*/
    Code3pForcastType &code_3p_fcst = Get3pDataHolder(type_period, is_down_forward);
    auto vector_iter = code_3p_fcst.find(code);
    if( vector_iter == code_3p_fcst.end() )
         return nullptr;
    if( vector_iter->second.empty() ) 
        return nullptr;
    unsigned int i = 0;
    for( ; i < vector_iter->second.size(); ++i )
    {
        if( vector_iter->second.at(i).date_a == item_a.stk_item.date 
            && vector_iter->second.at(i).date_b == item_b.stk_item.date )
            break;
    }
    if( i != vector_iter->second.size() )
        return std::addressof(vector_iter->second.at(i));
    else
        return nullptr;
}

std::vector<T_Data3pForcast> * ForcastMan::Find3pForcastVector(const std::string &code, TypePeriod type_period, bool is_down_forward)
{
    Code3pForcastType &code_3p_fcst = Get3pDataHolder(type_period, is_down_forward);
    auto vector_iter = code_3p_fcst.find(code);
    if( vector_iter == code_3p_fcst.end() )
        return nullptr;
    return std::addressof(vector_iter->second);
}

void ForcastMan::Append(TypePeriod type_period, const std::string &code, bool is_down_forward, T_Data3pForcast &data_3p )
{
    Code3pForcastType &code_3p_fcst = Get3pDataHolder(type_period, is_down_forward);

    auto vector_iter = code_3p_fcst.find(code);
    if( vector_iter == code_3p_fcst.end() )
        vector_iter = code_3p_fcst.insert(std::make_pair(code, std::vector<T_Data3pForcast>())).first;
    vector_iter->second.push_back(data_3p);
}

std::tuple<double, double, double>  ForcastC_ABDown(double a, double b)
{
    double c2 = sqrt(a * b);
    double c1 = sqrt(b * c2);
    double c3 = c1 + c2 - b;
    return std::make_tuple(c1, c2, c3);
}

std::tuple<double, double, double>  ForcastC_ABUp(double a, double b)
{
    double c2 = sqrt(a * b);
    double c1 = sqrt(b * c2);
    double c3 = 0.7279*a + 0.207*b + 0.06488*c1;
    return std::make_tuple(c1, c2, c3);
}

// ps: make sure a > b > 0
std::tuple<double, double, double>  ForcastD_ABC_Down(double A, double B)
{
    double D2 = B * sqrt( (B/A) + (1.309571 - B) / pow(pow(A,2), 2.10075) );
    double D1= sqrt(B*D2);
    double D3 = 0.254283*B + 0.745551*B*B/A;
    return std::make_tuple(D1, D2, D3);
}

// ps: make sure 0 < a < b
std::tuple<double, double, double>  ForcastD_ABC_Up(double A, double B)
{  
    double D2 = B + B - A;
    double D1 = 0.419913*D2 + 0.41571*B + (0.039433 + 0.164428*D2*D2)/B;
    double D3 = 1.089025*D2 + 0.214334*D1 - 0.30335*B;
    return std::make_tuple(D1, D2, D3);
}
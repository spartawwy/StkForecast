#include "forcast_man.h"

ForcastMan::ForcastMan(int wall_index)
    : wall_index_(wall_index)
    , stock_2pdown_forcast_15m_(1024)
    , stock_2pdown_forcast_30m_(1024)
    , stock_2pdown_forcast_h_(1024)
    , stock_2pdown_forcast_d_(1024)
    , stock_2pdown_forcast_w_(1024)
    , stock_2pdown_forcast_mon_(1024)
    , stock_2pup_forcast_15m_(1024)
    , stock_2pup_forcast_30m_(1024)
    , stock_2pup_forcast_h_(1024)
    , stock_2pup_forcast_d_(1024)
    , stock_2pup_forcast_w_(1024)
    , stock_2pup_forcast_mon_(1024)
    , stock_3pdown_forcast_15m_(1024)
    , stock_3pdown_forcast_30m_(1024)
    , stock_3pdown_forcast_h_(1024)
    , stock_3pdown_forcast_d_(1024)
    , stock_3pdown_forcast_w_(1024)
    , stock_3pdown_forcast_mon_(1024)
    , stock_3pup_forcast_15m_(1024)
    , stock_3pup_forcast_30m_(1024)
    , stock_3pup_forcast_h_(1024)
    , stock_3pup_forcast_d_(1024)
    , stock_3pup_forcast_w_(1024)
    , stock_3pup_forcast_mon_(1024)
    , df_no_use_(1)
    , uf_no_use_(1)
{

}

#if 0 
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
    if( holder_ref.empty() )
        return nullptr;
    auto vector_iter = holder_ref.find(code);
    if( vector_iter == holder_ref.end() )
        return nullptr;
    return std::addressof(vector_iter->second);
}

std::vector<T_Data2pUpForcast> * ForcastMan::Find2pUpForcastVector(const std::string &code, TypePeriod type_period)
{
    Code2pUpForcastType & holder_ref = Get2pUpDataHolder(type_period);
    if( holder_ref.empty() )
        return nullptr;
    auto vector_iter = holder_ref.find(code);
    if( vector_iter == holder_ref.end() )
        return nullptr;
    return std::addressof(vector_iter->second);
}

bool ForcastMan::HasIn2pDownwardForcast(const std::string &code, TypePeriod type_period, T_KlineDataItem &item_a, T_KlineDataItem &item_b)
{
    static auto has_in2pforcasts = [](std::vector<T_Data2pDownForcast>& data_vector, int date_a, int hhmm_a, int date_b, int hhmm_b)->bool
    { 
        if( data_vector.empty() ) return false;
        unsigned int i = 0;
        for( ; i < data_vector.size(); ++i )
        {
            if( data_vector.at(i).date_a == date_a && data_vector.at(i).date_b == date_b
                && data_vector.at(i).hhmm_a == hhmm_a && data_vector.at(i).hhmm_b == hhmm_b)
                break;
        }
        return i != data_vector.size();
    };

    Code2pDownForcastType & code_2pdown_fcst = Get2pDownDataHolder(type_period);
    auto vector_iter = code_2pdown_fcst.find(code);
    if( vector_iter != code_2pdown_fcst.end() )
        return has_in2pforcasts(vector_iter->second, item_a.kline_posdata(wall_index_).date, item_a.stk_item.hhmmss, item_b.kline_posdata(wall_index_).date, item_b.stk_item.hhmmss);
    else 
        return false;
}

bool ForcastMan::HasIn2pUpForcast(const std::string &code, TypePeriod type_period, T_KlineDataItem &item_a, T_KlineDataItem &item_b)
{
    static auto has_in2pforcasts = [](std::vector<T_Data2pUpForcast>& data_vector, int date_a, int hhmm_a, int date_b, int hhmm_b)->bool
    { 
        if( data_vector.empty() ) return false;
        unsigned int i = 0;
        for( ; i < data_vector.size(); ++i )
        {
             if( data_vector.at(i).date_a == date_a && data_vector.at(i).date_b == date_b
                && data_vector.at(i).hhmm_a == hhmm_a && data_vector.at(i).hhmm_b == hhmm_b)
                break;
        }
        return i != data_vector.size();
    };
    Code2pUpForcastType &code_2pup_fcst = Get2pUpDataHolder(type_period);
    auto vector_iter = code_2pup_fcst.find(code);
    if( vector_iter != code_2pup_fcst.end() )
        return has_in2pforcasts(vector_iter->second, item_a.kline_posdata(wall_index_).date, item_a.stk_item.hhmmss, item_b.kline_posdata(wall_index_).date, item_b.stk_item.hhmmss);
    else 
        return false;
}
#else 

void ForcastMan::Append(TypePeriod type_period, const std::string &code,  bool is_down_forward, T_Data2pForcast& forcast_data)
{ 
    Code2pForcastType & holder_ref = Get2pDataHolder(type_period, is_down_forward);
    auto vector_iter = holder_ref.find(code);
    if( vector_iter == holder_ref.end() )
        vector_iter = holder_ref.insert(std::make_pair(code, std::vector<T_Data2pForcast>())).first;
    vector_iter->second.push_back( *(T_Data2pForcast*)(&forcast_data) );
}

std::vector<T_Data2pForcast> * ForcastMan::Find2pForcastVector(const std::string &code, TypePeriod type_period, bool is_down_forward)
{
    Code2pForcastType & holder_ref = Get2pDataHolder(type_period, is_down_forward);
    if( holder_ref.empty() )
        return nullptr;
    auto vector_iter = holder_ref.find(code);
    if( vector_iter == holder_ref.end() )
        return nullptr;
    return std::addressof(vector_iter->second);
}
#endif 

Code2pForcastType & ForcastMan::Get2pDataHolder(TypePeriod type_period, bool is_down_forward)
{
    if( is_down_forward )
        return Get2pDownDataHolder(type_period);
    else
        return Get2pUpDataHolder(type_period);
}

Code2pForcastType & ForcastMan::Get2pUpDataHolder(TypePeriod type_period)
{
    switch (type_period)
    {
    case TypePeriod::PERIOD_5M: return stock_2pup_forcast_5m_;
    case TypePeriod::PERIOD_15M: return stock_2pup_forcast_15m_;
    case TypePeriod::PERIOD_30M: return stock_2pup_forcast_30m_;
    case TypePeriod::PERIOD_HOUR: return stock_2pup_forcast_h_;
    case TypePeriod::PERIOD_DAY: return stock_2pup_forcast_d_;
    case TypePeriod::PERIOD_WEEK: return stock_2pup_forcast_w_;
    case TypePeriod::PERIOD_MON: return stock_2pup_forcast_mon_;
    default: assert(false);
    }
    return uf_no_use_;
}

Code2pForcastType & ForcastMan::Get2pDownDataHolder(TypePeriod type_period)
{
    switch (type_period)
    {
    case TypePeriod::PERIOD_5M:  return stock_2pdown_forcast_5m_;
    case TypePeriod::PERIOD_15M: return stock_2pdown_forcast_15m_;
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
    case TypePeriod::PERIOD_5M:  return  is_down ? stock_3pdown_forcast_5m_ : stock_3pup_forcast_5m_;
    case TypePeriod::PERIOD_15M:  return is_down ? stock_3pdown_forcast_15m_ : stock_3pup_forcast_15m_;
    case TypePeriod::PERIOD_30M:  return is_down ? stock_3pdown_forcast_30m_ : stock_3pup_forcast_30m_;
    case TypePeriod::PERIOD_HOUR: return is_down ? stock_3pdown_forcast_h_ : stock_3pup_forcast_h_;
    case TypePeriod::PERIOD_DAY:  return is_down ? stock_3pdown_forcast_d_ : stock_3pup_forcast_d_;
    case TypePeriod::PERIOD_WEEK: return is_down ? stock_3pdown_forcast_w_ : stock_3pup_forcast_w_;
    case TypePeriod::PERIOD_MON:  return is_down ? stock_3pdown_forcast_mon_ : stock_3pup_forcast_mon_;
    default: assert(false);
    }
    return no_use_3p_;
}

T_Data2pForcast * ForcastMan::Find2pForcast(const std::string &code, TypePeriod type_period, bool is_down_forward, T_KlineDataItem &item_a, T_KlineDataItem &item_b)
{
    Code2pForcastType &code_2p_fcst = Get2pDataHolder(type_period, is_down_forward);
    auto vector_iter = code_2p_fcst.find(code);
    if( vector_iter == code_2p_fcst.end() )
         return nullptr;
    if( vector_iter->second.empty() ) 
        return nullptr;
    unsigned int i = 0;
    for( ; i < vector_iter->second.size(); ++i )
    {
        if( vector_iter->second.at(i).date_a == item_a.stk_item.date && vector_iter->second.at(i).hhmm_a == item_a.stk_item.hhmmss 
            && vector_iter->second.at(i).date_b == item_b.stk_item.date && vector_iter->second.at(i).hhmm_b == item_b.stk_item.hhmmss  )
            break;
    }
    if( i != vector_iter->second.size() )
        return std::addressof(vector_iter->second.at(i));
    else
        return nullptr;
}

T_Data3pForcast * ForcastMan::Find3pForcast(const std::string &code, TypePeriod type_period, bool is_down_forward, T_KlineDataItem &item_a, T_KlineDataItem &item_b)
{
    Code3pForcastType &code_3p_fcst = Get3pDataHolder(type_period, is_down_forward);
    auto vector_iter = code_3p_fcst.find(code);
    if( vector_iter == code_3p_fcst.end() )
         return nullptr;
    if( vector_iter->second.empty() ) 
        return nullptr;
    unsigned int i = 0;
    for( ; i < vector_iter->second.size(); ++i )
    {
        if( vector_iter->second.at(i).date_a == item_a.stk_item.date && vector_iter->second.at(i).hhmm_a == item_a.stk_item.hhmmss 
            && vector_iter->second.at(i).date_b == item_b.stk_item.date && vector_iter->second.at(i).hhmm_b == item_b.stk_item.hhmmss  )
            break; 
    }
    if( i != vector_iter->second.size() )
        return std::addressof(vector_iter->second.at(i));
    else
        return nullptr;
}

void ForcastMan::Remove3pForcastItem(const std::string &code, TypePeriod type_period, bool is_down_forward, T_KlineDataItem &item_a, T_KlineDataItem &item_b)
{
    T_Data3pForcast * data = Find3pForcast(code, type_period, is_down_forward, item_a, item_b);
    if( data )
        data->Clear();
}

std::vector<T_Data3pForcast> * ForcastMan::Find3pForcastVector(const std::string &code, TypePeriod type_period, bool is_down_forward)
{
    Code3pForcastType &code_3p_fcst = Get3pDataHolder(type_period, is_down_forward);
    auto vector_iter = code_3p_fcst.find(code);
    if( vector_iter == code_3p_fcst.end() )
        return nullptr;
    return std::addressof(vector_iter->second);
}

double ForcastMan::FindMaxForcastPrice(const std::string &code, TypePeriod type_period, int start_date, int end_date)
{
    assert(start_date <= end_date);
    Code3pForcastType &code_3p_down_fcst = Get3pDataHolder(type_period, false);
    auto vector_iter = code_3p_down_fcst.find(code);
    if( vector_iter == code_3p_down_fcst.end() )
        return 0.0;
    std::vector<T_Data3pForcast> & forcasts = vector_iter->second;
    double max_price = MIN_PRICE;
    for(int i = 0; i < forcasts.size(); ++i )
    {
        if( forcasts.at(i).date_c >= start_date && forcasts.at(i).date_c <= end_date )
        {
            if( forcasts.at(i).d3 > max_price )
                max_price = forcasts.at(i).d3;
        }
    }
    return max_price;
}

double ForcastMan::FindMinForcastPrice(const std::string &code, TypePeriod type_period, int start_date, int end_date)
{
    assert(start_date <= end_date);
    Code3pForcastType &code_3p_down_fcst = Get3pDataHolder(type_period, true);
    auto vector_iter = code_3p_down_fcst.find(code);
    if( vector_iter == code_3p_down_fcst.end() )
        return MAX_PRICE;
    std::vector<T_Data3pForcast> & forcasts = vector_iter->second;
    double min_price = MAX_PRICE;
    for(int i = 0; i < forcasts.size(); ++i )
    {
        if( forcasts.at(i).date_c >= start_date && forcasts.at(i).date_c <= end_date )
        {
            if( forcasts.at(i).d3 < min_price )
                min_price = forcasts.at(i).d3;
        }
    }
    return min_price;
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
#ifndef  FORCAST_MAN_SDF23SDFS_H
#define  FORCAST_MAN_SDF23SDFS_H

#include <cassert>
#include <string>
#include <unordered_map>
#include <tuple>

#include "stkfo_common.h"

class T_Data2pDownForcast
{
public:
    T_Data2pDownForcast() : date_a(0), date_b(0), c1(0.0), c2(0.0), c3(0.0) {}
    T_Data2pDownForcast(const T_Data2pDownForcast & lh) : 
        date_a(lh.date_a), date_b(lh.date_b), c1(lh.c1), c2(lh.c2), c3(lh.c3) {}
    T_Data2pDownForcast(T_Data2pDownForcast && lh) : 
        date_a(lh.date_a), date_b(lh.date_b), c1(lh.c1), c2(lh.c2), c3(lh.c3) {}
    std::string  stock_code;
    int date_a;
    int date_b;
    double  c1;
    double  c2;
    double  c3; 
};

class T_Data2pUpForcast
{
public:
    T_Data2pUpForcast() : date_a(0), date_b(0), c1(0.0), c2(0.0), c3(0.0) {}
    T_Data2pUpForcast(const T_Data2pUpForcast & lh) : 
        date_a(lh.date_a), date_b(lh.date_b), c1(lh.c1), c2(lh.c2), c3(lh.c3) {}
    T_Data2pUpForcast(T_Data2pUpForcast && lh) : 
        date_a(lh.date_a), date_b(lh.date_b), c1(lh.c1), c2(lh.c2), c3(lh.c3) {}
    std::string  stock_code;
    int date_a;
    int date_b;
    double  c1;
    double  c2;
    double  c3; 
};

// (code, T_Data2pDownForcasts)
typedef std::unordered_map<std::string, std::vector<T_Data2pDownForcast> > Code2pDownForcastType;

// (code, T_Data2pUpForcast)
typedef std::unordered_map<std::string, std::vector<T_Data2pUpForcast> > Code2pUpForcastType;

class ForcastMan
{
public:
    ForcastMan();
    ~ForcastMan(){}
     
    void Append(TypePeriod type_period, const std::string &code, T_Data2pDownForcast& ); 
    std::vector<T_Data2pDownForcast> * Find2pDownForcast(const std::string &code, TypePeriod type_period);
    bool HasIn2pDownwardForcast(const std::string &code, TypePeriod type_period, T_KlineDataItem &item_a, T_KlineDataItem &item_b);

    void Append(TypePeriod type_period, const std::string &code, T_Data2pUpForcast& );
    std::vector<T_Data2pUpForcast> * Find2pUpForcast(const std::string &code, TypePeriod type_period);
    bool HasIn2pUpForcast(const std::string &code, TypePeriod type_period, T_KlineDataItem &item_a, T_KlineDataItem &item_b);

private:
     
    template<typename T1,typename T2>
    struct is__same
    {
        operator bool(){return false;}
    };
    template<typename T1>
    struct is__same<T1,T1>
    {
        operator bool(){return true;}
    }; 


    template <typename DataForcastType>
    void _Append2pForcast(TypePeriod type_period, const std::string &code, DataForcastType &forcast_data)
    {
        Code2pDownForcastType *down_holder = nullptr;
        Code2pUpForcastType   *up_holder = nullptr;
        bool is_2pdown = false;
        if( is__same<DataForcastType, T_Data2pDownForcast>() )
            is_2pdown = true;

        if( is_2pdown )
            down_holder = &Get2pDownDataHolder(type_period);
        else
            up_holder = &Get2pUpDataHolder(type_period);
         
        if( is_2pdown )
        {
            auto vector_iter = down_holder->find(code);
            if( vector_iter == down_holder->end() )
                vector_iter = down_holder->insert(std::make_pair(code, std::vector<T_Data2pDownForcast>())).first;
            vector_iter->second.push_back( *(T_Data2pDownForcast*)(&forcast_data) );
        }else
        {
            auto vector_iter = up_holder->find(code);
            if( vector_iter == up_holder->end() )
                vector_iter = up_holder->insert(std::make_pair(code, std::vector<T_Data2pUpForcast>())).first;
            vector_iter->second.push_back( *(T_Data2pUpForcast*)(&forcast_data) );
        }
        
    }

    /*template <typename DataForcastType>
    std::vector<T_Data2pDownForcast> * _Find2pForcast(const std::string &code, TypePeriod type_period)
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
    }*/

    Code2pUpForcastType & Get2pUpDataHolder(TypePeriod type_period);
    Code2pDownForcastType & Get2pDownDataHolder(TypePeriod type_period);

    Code2pDownForcastType  stock_2pdown_forcast_30m_; // 30 minute
    Code2pDownForcastType  stock_2pdown_forcast_h_;
    Code2pDownForcastType  stock_2pdown_forcast_d_;
    Code2pDownForcastType  stock_2pdown_forcast_w_; // week
    Code2pDownForcastType  stock_2pdown_forcast_mon_; // month
    Code2pDownForcastType  df_no_use_;

    Code2pUpForcastType  stock_2pup_forcast_30m_; // 30 minute 
    Code2pUpForcastType  stock_2pup_forcast_h_;  
    Code2pUpForcastType  stock_2pup_forcast_d_;  
    Code2pUpForcastType  stock_2pup_forcast_w_;  
    Code2pUpForcastType  stock_2pup_forcast_mon_;  
    Code2pUpForcastType  uf_no_use_;  
     
};

// return: c1, c2, c3 ; ps: make sure a > b;
std::tuple<double, double, double>  ForcastC_ABDown(double a, double b);

// return: c1, c2, c3 ; ps: make sure a < b;
std::tuple<double, double, double>  ForcastC_ABUp(double a, double b);

 

#endif // FORCAST_MAN_SDF23SDFS_H
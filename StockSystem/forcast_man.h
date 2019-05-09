#ifndef  FORCAST_MAN_SDF23SDFS_H
#define  FORCAST_MAN_SDF23SDFS_H

#include <cassert>
#include <string>
#include <unordered_map>
#include <tuple>

#include "stkfo_common.h"

class T_DataForcast
{
public:
    T_DataForcast(bool down) : stock_code(), date_a(0), hhmm_a(0), date_b(0), hhmm_b(0), point_a(0.0,0.0), point_b(0.0,0.0), is_down(down) {}
    
    T_DataForcast(const T_DataForcast & lh) : stock_code(lh.stock_code)
        ,date_a(lh.date_a), hhmm_a(lh.hhmm_a), date_b(lh.date_b), hhmm_b(lh.hhmm_b), point_a(lh.point_a), point_b(lh.point_b), is_down(lh.is_down) {}
    
    T_DataForcast(T_DataForcast && lh) : stock_code(std::move(lh.stock_code)),
        date_a(lh.date_a), hhmm_a(lh.hhmm_a), date_b(lh.date_b), hhmm_b(lh.hhmm_b), point_a(lh.point_a), point_b(lh.point_b), is_down(lh.is_down) {}
     
    virtual void Clear(){ __Clear(); }
    virtual ~T_DataForcast(){}

    std::string  stock_code;
    int date_a;
    int hhmm_a;
    int date_b;
    int hhmm_b;

    QPointF point_a;
    QPointF point_b;

    bool is_down;

protected:
    T_DataForcast(){}
    void __Clear(){ stock_code.clear(); date_a = hhmm_a = date_b = hhmm_b = 0; is_down = false; point_a = QPointF(0.0,0.0); point_b = QPointF(0.0,0.0); }
};

class T_Data2pForcast : public T_DataForcast
{
public:

    T_Data2pForcast(bool down=false) : T_DataForcast(down), c1(0.0), c2(0.0), c3(0.0) {}

    T_Data2pForcast(const T_Data2pForcast & lh) : T_DataForcast(lh), c1(lh.c1), c2(lh.c2), c3(lh.c3){}
    
    T_Data2pForcast(T_Data2pForcast && lh) : T_DataForcast(std::move(lh)), c1(lh.c1), c2(lh.c2), c3(lh.c3) {}

    virtual void Clear() override { __Clear(); c1 = c2 = c3 = 0.0; } 
public:
    double  c1;
    double  c2;
    double  c3; 
};


class T_Data3pForcast : public T_DataForcast
{
public:
    explicit T_Data3pForcast(bool down=false) : T_DataForcast(down), date_c(0), hhmm_c(0), d1(0.0), d2(0.0), d3(0.0), point_c(0.0,0.0) {}

    T_Data3pForcast(const T_Data3pForcast & lh) : T_DataForcast(lh), date_c(lh.date_c), hhmm_c(lh.hhmm_c), d1(lh.d1), d2(lh.d2), d3(lh.d3), point_c(lh.point_c) {}

    T_Data3pForcast(T_Data3pForcast && lh) : T_DataForcast(std::move(lh)), date_c(lh.date_c), hhmm_c(lh.hhmm_c), d1(lh.d1), d2(lh.d2), d3(lh.d3), point_c(lh.point_c) {}

    virtual void Clear() override { __Clear(); date_c = hhmm_c = 0; d1 = d2 = d3 = 0.0; point_c = QPointF(0.0,0.0); } 

    int date_c;
    int hhmm_c;
    double  d1;
    double  d2;
    double  d3; 
     
    QPointF point_c;
};
 
// (code, T_Data2pUpForcast)
typedef std::unordered_map<std::string, std::vector<T_Data2pForcast> > Code2pForcastType;

typedef std::unordered_map<std::string, std::vector<T_Data3pForcast> > Code3pForcastType;

class ForcastMan
{
public:
    ForcastMan(int wall_index);
    ~ForcastMan(){}

    void Append(TypePeriod type_period, const std::string &code,  bool is_down_forward, T_Data2pForcast& ); 
    std::vector<T_Data2pForcast> * Find2pForcastVector(const std::string &code, TypePeriod type_period, bool is_down_forward);
    T_Data2pForcast * Find2pForcast(const std::string &code, TypePeriod type_period, bool is_down_forward, T_KlineDataItem &item_a, T_KlineDataItem &item_b);
     
    void Append(TypePeriod type_period, const std::string &code, bool is_down_forward, T_Data3pForcast& );
    std::vector<T_Data3pForcast> * Find3pForcastVector(const std::string &code, TypePeriod type_period, bool is_down_forward);
    T_Data3pForcast * Find3pForcast(const std::string &code, TypePeriod type_period, bool is_down_forward, T_KlineDataItem &item_a, T_KlineDataItem &item_b);
    void Remove3pForcastItem(const std::string &code, TypePeriod type_period, bool is_down_forward, T_KlineDataItem &item_a, T_KlineDataItem &item_b);

    double FindMaxForcastPrice(const std::string &code, TypePeriod type_period, int start_date, int end_date);
    double FindMinForcastPrice(const std::string &code, TypePeriod type_period, int start_date, int end_date);

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

    Code3pForcastType & Get3pDataHolder(TypePeriod type_period, bool is_down);
    Code2pForcastType & Get2pDataHolder(TypePeriod type_period, bool is_down_forward);
    Code2pForcastType & Get2pUpDataHolder(TypePeriod type_period);
    Code2pForcastType & Get2pDownDataHolder(TypePeriod type_period);

    Code2pForcastType  stock_2pdown_forcast_5m_; // 5 minute
    Code2pForcastType  stock_2pdown_forcast_15m_; // 15 minute
    Code2pForcastType  stock_2pdown_forcast_30m_; // 30 minute
    Code2pForcastType  stock_2pdown_forcast_h_;
    Code2pForcastType  stock_2pdown_forcast_d_;
    Code2pForcastType  stock_2pdown_forcast_w_; // week
    Code2pForcastType  stock_2pdown_forcast_mon_; // month
    Code2pForcastType  df_no_use_;

    Code2pForcastType  stock_2pup_forcast_5m_; // 5 minute 
    Code2pForcastType  stock_2pup_forcast_15m_; // 15 minute 
    Code2pForcastType  stock_2pup_forcast_30m_; // 30 minute 
    Code2pForcastType  stock_2pup_forcast_h_;  
    Code2pForcastType  stock_2pup_forcast_d_;  
    Code2pForcastType  stock_2pup_forcast_w_;  
    Code2pForcastType  stock_2pup_forcast_mon_;  
    Code2pForcastType  uf_no_use_;  

    Code3pForcastType    stock_3pdown_forcast_5m_;
    Code3pForcastType    stock_3pdown_forcast_15m_;
    Code3pForcastType    stock_3pdown_forcast_30m_;
    Code3pForcastType    stock_3pdown_forcast_h_;
    Code3pForcastType    stock_3pdown_forcast_d_;
    Code3pForcastType    stock_3pdown_forcast_w_;
    Code3pForcastType    stock_3pdown_forcast_mon_;

    Code3pForcastType    stock_3pup_forcast_5m_;
    Code3pForcastType    stock_3pup_forcast_15m_;
    Code3pForcastType    stock_3pup_forcast_30m_;
    Code3pForcastType    stock_3pup_forcast_h_;
    Code3pForcastType    stock_3pup_forcast_d_;
    Code3pForcastType    stock_3pup_forcast_w_;
    Code3pForcastType    stock_3pup_forcast_mon_;
    Code3pForcastType    no_use_3p_;

    int wall_index_;
};

// return: c1, c2, c3 ; ps: make sure a > b > 0;
std::tuple<double, double, double>  ForcastC_ABDown(double a, double b);

// return: c1, c2, c3 ; ps: make sure 0 < a < b;
std::tuple<double, double, double>  ForcastC_ABUp(double a, double b);

// return: d1, d2, d3 ;ps: make sure a > b > 0
std::tuple<double, double, double>  ForcastD_ABC_Down(double A, double B);

// return: d1, d2, d3 ; ps: make sure 0 < a < b
std::tuple<double, double, double>  ForcastD_ABC_Up(double A, double B);

#endif // FORCAST_MAN_SDF23SDFS_H
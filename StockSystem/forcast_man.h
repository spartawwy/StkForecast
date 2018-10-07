#ifndef  FORCAST_MAN_SDF23SDFS_H
#define  FORCAST_MAN_SDF23SDFS_H

#include <string>
#include <unordered_map>

#include "stkfo_common.h"

class T_PaintData2pForcast
{
public:
    T_PaintData2pForcast() : date_a(0), date_b(0), c1(0.0), c2(0.0), c3(0.0) {}
    T_PaintData2pForcast(const T_PaintData2pForcast & lh) : 
        date_a(lh.date_a), date_b(lh.date_b), c1(lh.c1), c2(lh.c2), c3(lh.c3) {}
    T_PaintData2pForcast(T_PaintData2pForcast && lh) : 
        date_a(lh.date_a), date_b(lh.date_b), c1(lh.c1), c2(lh.c2), c3(lh.c3) {}
    std::string  stock_code;
    int date_a;
    int date_b;
    double  c1;
    double  c2;
    double  c3; 
};

// (code, T_PaintData2pForcastDownwords)
typedef std::unordered_map<std::string, std::vector<T_PaintData2pForcast> > Code2pForcastDownType;

class ForcastMan
{
public:
    ForcastMan();
    ~ForcastMan(){}
     
    void Append(TypePeriod type_period, const std::string &code, T_PaintData2pForcast& );
    std::vector<T_PaintData2pForcast> * Find2pForcastDown(const std::string &code, TypePeriod type_period);
    bool HasIn2pDownwardForcast(const std::string &code, TypePeriod type_period, T_KlineDataItem &item_a, T_KlineDataItem &item_b);

private:
     
    Code2pForcastDownType  stock_2pforcastdown_period_30m_; // 30 minute
    Code2pForcastDownType  stock_2pforcastdown_period_h_;
    Code2pForcastDownType  stock_2pforcastdown_period_d_;
    Code2pForcastDownType  stock_2pforcastdown_period_w_; // week
    Code2pForcastDownType  stock_2pforcastdown_period_m_; // month
     
};
#endif // FORCAST_MAN_SDF23SDFS_H
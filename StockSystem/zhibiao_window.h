#ifndef ZHIBIAO_WINDOW_SDF2SDF_
#define ZHIBIAO_WINDOW_SDF2SDF_

#include "stkfo_common.h"
 
class ZhibiaoWindow
{
public:
    ZhibiaoWindow(ZhibiaoType zhibiao_type):zhibiao_type_(zhibiao_type), height_(60){}
    int height() { return height_; }
    ZhibiaoType zhibiao_type() {return zhibiao_type_;}

private:
    ZhibiaoType zhibiao_type_;
    int height_;
};


#endif
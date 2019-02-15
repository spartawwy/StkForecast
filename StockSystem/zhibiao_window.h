#ifndef ZHIBIAO_WINDOW_SDF2SDF_
#define ZHIBIAO_WINDOW_SDF2SDF_

#include "stkfo_common.h"
 
class QPainter;
class KLineWall;
class ZhibiaoWindow
{
public:

    ZhibiaoWindow(ZhibiaoType zhibiao_type, KLineWall *parent):zhibiao_type_(zhibiao_type), parent_(parent), height_(60){}
    void DrawWindow(QPainter &painter, int mm_w);
    int height() { return height_; }
    ZhibiaoType zhibiao_type() {return zhibiao_type_;}

private:
    ZhibiaoType zhibiao_type_;
    KLineWall *parent_;
    int height_;
};


#endif
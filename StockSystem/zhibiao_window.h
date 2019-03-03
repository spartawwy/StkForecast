#ifndef ZHIBIAO_WINDOW_SDF2SDF_
#define ZHIBIAO_WINDOW_SDF2SDF_

#include "stkfo_common.h"
 
class QPainter;
class KLineWall;
class ZhibiaoWindow
{
public:

    ZhibiaoWindow(/*ZhibiaoType zhibiao_type, */KLineWall *parent):/*zhibiao_type_(zhibiao_type),*/ parent_(parent), high_percent_(0.06){}
    ZhibiaoWindow(KLineWall *parent, double h_percent): parent_(parent), high_percent_(h_percent){}
    virtual ~ZhibiaoWindow(){}

    virtual void DrawWindow(QPainter &/*painter*/, int /*mm_w*/){}

    /* zhibiao window heiht = parent.height * high_percent_
    *  ps: high_percent < 1.0  */
    void high_percent(double ver ) { high_percent_ = ver; }
    double high_percent() { return high_percent_; }
    //ZhibiaoType zhibiao_type() {return zhibiao_type_;}
    int Height();

protected:

    //ZhibiaoType zhibiao_type_;
    KLineWall *parent_;
    double high_percent_;
};

class VolZhibiaoWin : public ZhibiaoWindow
{
public:
    VolZhibiaoWin(KLineWall *parent) : ZhibiaoWindow(parent, 0.06){}

    virtual void DrawWindow(QPainter &painter, int mm_w) override;

};

class MomentumZhibiaoWin : public ZhibiaoWindow
{
public:
    MomentumZhibiaoWin(KLineWall *parent) : ZhibiaoWindow(parent, 0.2/*0.08*/){}

    virtual void DrawWindow(QPainter &painter, int mm_w) override;
};
#endif
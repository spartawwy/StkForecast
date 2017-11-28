#ifndef K_LINE_WALL_SDF32DSF_
#define K_LINE_WALL_SDF32DSF_

#include <QtWidgets/QWidget>

#include "ui_klinewall.h"

#include "stockalldaysinfo.h"

class KLineWall : public QWidget
{
public:

    KLineWall();

    Ui_KLineWallForm  ui;

protected:

    void paintEvent(QPaintEvent*) override;
    void mouseDoubleClickEvent(QMouseEvent * e)  override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

private:

    std::list<StockDayInfo>  stk_days_infos_;

    std::string stockNum;
    StockAllDaysInfo stockAllDaysInfo;  //一支股票的所有天数的消息
    float lowestMinPrice;
    float highestMaxPrice;
    bool  show_cross_line_;
    bool  is_repaint_k_;

    int  k_num_;
};


#endif
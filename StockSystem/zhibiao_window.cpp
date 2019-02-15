#include "zhibiao_window.h"

#include <qpainter.h>

#include "kline_wall.h"



void ZhibiaoWindow::DrawWindow(QPainter &painter, int mm_w)
{ 
    const double item_w = double(mm_w - parent_->empty_right_w_ -  parent_->right_w_) / double( parent_->k_num_ + 1) ;
    const double k_bar_w = item_w * 3 / 4;
    const int right_end = double(mm_w -  parent_->empty_right_w_ -  parent_->right_w_) - k_bar_w;
    const double largest_vol = parent_->GetCurWinKLargetstVol();
     
    QPen red_pen; red_pen.setColor(Qt::red); red_pen.setStyle(Qt::SolidLine); red_pen.setWidth(1);
    QPen green_pen; green_pen.setColor(Qt::green); green_pen.setStyle(Qt::SolidLine); green_pen.setWidth(1);  
    QBrush red_brush(Qt::red);  
    QBrush green_brush(Qt::green);  
    //------------------  
    int k = parent_->k_num_;
    for( auto iter = parent_->p_hisdata_container_->rbegin();
        iter != parent_->p_hisdata_container_->rend() && k > 0; 
        ++iter, --k)
    {
        if( (*iter)->stk_item.close_price < (*iter)->stk_item.open_price )
        {
            painter.setPen(green_pen);
            painter.setBrush(green_brush);
        }else 
        {   
            painter.setPen(red_pen);
            painter.setBrush(red_brush);
        }
        double high = (*iter)->stk_item.vol * height() / largest_vol;
        double x_right = right_end - item_w * (parent_->k_num_ - k);  
        painter.drawRect(x_right, -1*high, -1*k_bar_w, high);
    }
    //------------------
}
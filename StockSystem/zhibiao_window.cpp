#include "zhibiao_window.h"

#include <qpainter.h>

#include "kline_wall.h"



void ZhibiaoWindow::DrawWindow(QPainter &painter, int mm_w)
{
    static auto GetLargestVol = [this]()->double
    {
        double largest_vol = 0.0;
        int k = parent_->k_num_;
        for( auto iter = parent_->p_hisdata_container_->rbegin();
            iter != parent_->p_hisdata_container_->rend() && k > 0; 
            ++iter, --k)
        if( (*iter)->stk_item.vol > largest_vol ) 
            largest_vol = (*iter)->stk_item.vol;
        return largest_vol;
    };

    const double item_w = double(mm_w - parent_->empty_right_w_ -  parent_->right_w_) / double( parent_->k_num_ + 1) ;
    const double k_bar_w = item_w * 3 / 4;
    const int right_end = double(mm_w -  parent_->empty_right_w_ -  parent_->right_w_) - k_bar_w;
    const double largest_vol = GetLargestVol();
     
    painter.translate(0, zb_h + lit_border_pen.width());
    trans_y_totoal += zb_h + lit_border_pen.width();
    painter.setPen(lit_border_pen);
    painter.drawLine(0, 0, mm_w, 0);
    //------------------  
    int k = k_num_;
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
        double height = (*iter)->stk_item.vol * height_ / largest_vol;
        double x_right = right_end - item_w * (k_num_ - k);  
        painter.drawRect(x_right, -1*height, -1*k_bar_w, height);
    }
    //------------------
}
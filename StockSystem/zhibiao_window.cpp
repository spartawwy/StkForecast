#include "zhibiao_window.h"

#include <qpainter.h>

#include "kline_wall.h"

int ZhibiaoWindow::Height()
{
   return int( parent_->height()* high_percent() );
}

void VolZhibiaoWin::DrawWindow(QPainter &painter, int mm_w)
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
        double high = (*iter)->stk_item.vol * Height() / largest_vol;
        double x_right = right_end - item_w * (parent_->k_num_ - k);  
        painter.drawRect(x_right, -1*high, -1*k_bar_w, high);
    }
    //------------------
}

////////////////////////////////////////

void MomentumZhibiaoWin::DrawWindow(QPainter &painter, int mm_w)
{
    const double half_h = Height() / 2;
    const double item_w = double(mm_w - parent_->empty_right_w_ -  parent_->right_w_) / double( parent_->k_num_ + 1) ;
    const double k_bar_w = item_w * 3 / 4;
    const int right_end = double(mm_w -  parent_->empty_right_w_ -  parent_->right_w_) - k_bar_w;
    const double largest_vol = parent_->GetCurWinKLargetstVol();

    QPen red_pen; red_pen.setColor(Qt::red); red_pen.setStyle(Qt::SolidLine); red_pen.setWidth(1);
     
    QPen magenta_pen; magenta_pen.setColor(Qt::magenta); magenta_pen.setStyle(Qt::SolidLine); magenta_pen.setWidth(0.5);
    QBrush transparent_brush(Qt::transparent);
    QPen yin_pen; yin_pen.setColor(Qt::cyan); yin_pen.setStyle(Qt::SolidLine); yin_pen.setWidth(0.2);
    QBrush yin_brush(Qt::cyan);  

    const double line_size = 0.5;
    const double h_translate = -1 * half_h;
    painter.translate(0, h_translate);

    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(line_size);
    pen.setStyle(Qt::DotLine); // ............
    painter.setPen(pen); 
    painter.drawLine(0, 0, right_end, 0);

    // find max value 
    double max_val = 0.0;
    int k = parent_->k_num_;
    for( auto iter = parent_->p_hisdata_container_->rbegin();
        iter != parent_->p_hisdata_container_->rend() && k > 0; 
        ++iter, --k)
    {
        auto val = fabs((*iter)->zhibiao_atoms[MOMENTUM_POS]->val3());
        if( val > max_val )
            max_val = val;
    }
    //-----------draw vertical lines-------  
    bool is_sign_change = false;
    double total_zb_val = 0.0;
    double tag_x_pos_end = right_end;
    //double tag_x_pos_beg = 0.0;
    double pre_val = 0.0;
    k = parent_->k_num_;
    for( auto iter = parent_->p_hisdata_container_->rbegin();
        iter != parent_->p_hisdata_container_->rend() && k > 0; 
        ++iter, --k)
    {
        auto val = (*iter)->zhibiao_atoms[MOMENTUM_POS]->val3();
         
        double high = val * half_h / max_val;
        double x_right = right_end - item_w * (parent_->k_num_ - k);  

        if( k != parent_->k_num_ )
        {
            if( (val > 0.0 && pre_val < 0.0) || (val < 0.0 && pre_val > 0.0) )
                is_sign_change = true;
        }else
            is_sign_change = false;
        if( is_sign_change )
        {
            is_sign_change = false;
            double font_y = 0.0;
            if( val < 0 ) 
                font_y = half_h;
            else
                font_y = half_h - painter.font().pointSize();
            char tmp_buf[16];
            sprintf_s(tmp_buf, sizeof(tmp_buf), "%.2f", total_zb_val);
            painter.drawText( (tag_x_pos_end + x_right)/2, (val < 0 ? 1 : -1) * font_y, QString("%1").arg(tmp_buf));
            total_zb_val = 0.0; 
            tag_x_pos_end = x_right; 
        }else
        {
            total_zb_val += val; 
        }
        pre_val = val;

        painter.setPen( (val > 0 ? magenta_pen : yin_pen) );
        painter.setBrush( (val > 0 ? transparent_brush : yin_brush) );
         
        painter.drawRect(x_right, 0, -1*k_bar_w, -1 * high);
    }
    painter.translate(0, -1 * h_translate);
    //------------------
}
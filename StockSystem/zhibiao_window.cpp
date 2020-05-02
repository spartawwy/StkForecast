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
    for( auto iter = parent_->p_hisdata_container_->rbegin() + parent_->k_rend_index_;
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

    QPen red_pen; red_pen.setColor(Qt::red); red_pen.setStyle(Qt::SolidLine); red_pen.setWidth(1);
     
    QPen magenta_pen; magenta_pen.setColor(Qt::magenta); magenta_pen.setStyle(Qt::SolidLine); magenta_pen.setWidth(0.5);
    QBrush transparent_brush(Qt::transparent);
    QPen yin_pen; yin_pen.setColor(Qt::cyan); yin_pen.setStyle(Qt::SolidLine); yin_pen.setWidth(0.2);
    QBrush yin_brush(Qt::cyan);  

    const double line_size = 0.5;

    //QPen pen;
    //pen.setColor(Qt::red);
    //pen.setWidth(line_size);
    //pen.setStyle(Qt::DotLine); // ............
    //painter.setPen(pen); 
    //painter.drawLine(0, 0, right_end, 0);

    QPen curve_pen;
    curve_pen.setColor(Qt::white);
    curve_pen.setWidth(line_size*2);
    curve_pen.setStyle(Qt::SolidLine);

    // find max min value 
    double min_negat_val = MAX_PRICE;
    double max_posit_val = MIN_PRICE;
    int k = parent_->k_num_;
    for( auto iter = parent_->p_hisdata_container_->rbegin() + parent_->k_rend_index_;
        iter != parent_->p_hisdata_container_->rend() && k > 0; 
        ++iter, --k)
    {
        double val_macd = (*iter)->zhibiao_atoms[MOMENTUM_POS]->val3();
        double val_dif = 3.12 * ((*iter)->zhibiao_atoms[MOMENTUM_POS]->val0() - (*iter)->zhibiao_atoms[MOMENTUM_POS]->val1());
        double dea_curve_val = 3.12 * (*iter)->zhibiao_atoms[MOMENTUM_POS]->val2();
        double maxv = std::max(val_macd, val_dif);
        maxv = std::max(maxv, dea_curve_val);
        double minv = std::min(val_macd, val_dif);
        minv = std::min(minv, dea_curve_val);
        if( maxv > max_posit_val )
            max_posit_val = maxv;
        if( minv < min_negat_val )
            min_negat_val = minv;
    }
    // calculate total val
    double total_val = 0.0;
    double zero_y = 0.0;
    if( max_posit_val * min_negat_val > 0.0 )
    {
        total_val = std::max(fabs(max_posit_val), fabs(min_negat_val));
    }else
    {
        total_val = fabs(max_posit_val - min_negat_val);
        zero_y = fabs(min_negat_val) * Height() / total_val;// will not be negative 
    }
    assert(total_val > 0.0);
    //-----------draw vertical bars-------  
    bool is_sign_change = false;
    double total_zb_val = 0.0;
    double tag_x_pos_end = right_end;
    double pre_val = 0.0;
    k = parent_->k_num_;
    for( auto iter = parent_->p_hisdata_container_->rbegin() + parent_->k_rend_index_;
        iter != parent_->p_hisdata_container_->rend() && k > 0; 
        ++iter, --k)
    {
        double curve_val = 3.12 * ((*iter)->zhibiao_atoms[MOMENTUM_POS]->val0() - (*iter)->zhibiao_atoms[MOMENTUM_POS]->val1());
        double curve_val_next = 0.0;
        double x_right_next = 0.0;
        if( (iter + 1) != parent_->p_hisdata_container_->rend() )
        {
            curve_val_next = 3.12 * ((*(iter+1))->zhibiao_atoms[MOMENTUM_POS]->val0() - (*(iter+1))->zhibiao_atoms[MOMENTUM_POS]->val1());
            x_right_next = right_end - item_w * (parent_->k_num_ - k + 1);  
        }
        double curve_high = curve_val * Height() / (total_val);
        double curve_high_next = curve_val_next * Height() / (total_val);

        auto val = (*iter)->zhibiao_atoms[MOMENTUM_POS]->val3();
        
        double bar_high = val * Height() / total_val;
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
                font_y = 0;
            else
                font_y = Height() - painter.font().pointSize();
            char tmp_buf[16];
            sprintf_s(tmp_buf, sizeof(tmp_buf), "%.2f", total_zb_val);
            painter.setPen( (val < 0 ? magenta_pen : yin_pen) );
            painter.drawText( (tag_x_pos_end + x_right)/2, -1 * font_y, QString("%1").arg(tmp_buf));
            total_zb_val = 0.0; 
            tag_x_pos_end = x_right; 
        }else
        {
            total_zb_val += val; 
        }
        pre_val = val;

        painter.setPen( (val > 0 ? magenta_pen : yin_pen) );
        painter.setBrush( (val > 0 ? transparent_brush : yin_brush) );
         
        painter.drawRect(x_right, -1 * zero_y, -1*k_bar_w, -1 * bar_high);

        painter.setPen(curve_pen);
        painter.drawLine(x_right, -1 * (curve_high + zero_y), x_right_next, -1 * (curve_high_next + zero_y));
    }
     
}
#include "minute_detail_wall.h"

#include <QPainter>

#include "stk_forecast_app.h"

MinuteDetailWall::MinuteDetailWall(StkForecastApp *app, QWidget *parent)
    : app_(app)
    , head_h_percent_(0.03)
    , bottom_h_percent_(0.04)
    , right_w_(30)
    , stock_code_()
    , stock_name_()
{
     // init ui -----------
    QPalette pal = this->palette();
    pal.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

bool MinuteDetailWall::Init()
{
    return true;
}

int MinuteDetailWall::Calculate_k_mm_h()
{
    int mm_h = this->height() - HeadHeight() - BottomHeight();
    /*for( unsigned int i = 0 ; i < zb_windows_.size(); ++i )
    {
        if( zb_windows_[i] )
            mm_h -= zb_windows_[i]->Height();
    }*/
    return mm_h;
}

void MinuteDetailWall::paintEvent(QPaintEvent* )
{
    static auto IsAreaShapeChange = [](MinuteDetailWall *mwall, int w, int h)->bool
    {
        return w != mwall->pre_mm_w_ || h!= mwall->pre_mm_h_;
    }; 
    /* window lay:********************
      |                        | header
      | minute price cure line | body 
      | vol windows 
      | bottom (show date)
    *********************************/
     
    QPainter painter(this); 
    auto old_font = painter.font();
    QPen red_pen; red_pen.setColor(Qt::red); red_pen.setStyle(Qt::SolidLine); red_pen.setWidth(1);
    QPen green_pen; green_pen.setColor(Qt::green); green_pen.setStyle(Qt::SolidLine); green_pen.setWidth(1);
    QBrush red_brush(Qt::red);  
    QBrush green_brush(Qt::green);  
    QPen border_pen(red_pen); border_pen.setWidth(2);
    QPen lit_border_pen(red_pen); lit_border_pen.setWidth(1);

    //qDebug() << "paintEvent QCursor::pos  x:" << QCursor::pos().x() << " y: "<< QCursor::pos().y() << "\n";
    auto pos_from_global = mapFromGlobal(QCursor::pos());
    //qDebug() << "paintEvent x:" << pos_from_global.x() << " y: "<< pos_from_global.y() << "\n";
      
    const int k_mm_h = Calculate_k_mm_h();
    const int mm_w = this->width();
     
    const bool is_area_shape_change = IsAreaShapeChange(this, this->width(), this->height());
    if( is_area_shape_change )
    {
        //UpdatePosDatas(); 
    }
    QString k_detail_str;
     

    const int price_scale_num = 8;
    const int scale_part_h = k_mm_h / price_scale_num; 
    int trans_y_totoal = 0;
     
    int h_axis_trans_in_paint_k_ = k_mm_h + HeadHeight(); 

    painter.translate(0, h_axis_trans_in_paint_k_); // translate frame of axes to k mm bottom
    trans_y_totoal += h_axis_trans_in_paint_k_;

    QPointF cur_mous_point_trans(cur_mouse_point_.x(), cur_mouse_point_.y() - h_axis_trans_in_paint_k_);

    auto ck_h = this->height();

    // draw stock code     -------------
    QPen pen; 
    pen.setColor(Qt::white);
    painter.setPen(pen);
    
    QFont font;  
    font.setPointSize(mm_w / 45); 
    painter.setFont(font);

    QString code_name = stock_code_ + stock_name_;
    painter.drawText(mm_w - right_w_ - code_name.size() * font.pointSize(), -1 * (h_axis_trans_in_paint_k_ - font.pointSize()*1.2), code_name);
     
    painter.setFont(old_font); 
    
    // right vertical line | -----------
    painter.setPen(border_pen);
    painter.drawLine(mm_w - right_w_, this->height() - h_axis_trans_in_paint_k_, mm_w - right_w_, -1 * this->height());
     
    // vertical' price scale ------------
    
    float highestMaxPrice_ = 50.0;
    float lowestMinPrice_ = 20.0;
    pen.setColor(Qt::red);
    pen.setStyle(Qt::DotLine); // ............
    painter.setPen(pen); 
    const float price_per_len = (highestMaxPrice_ - lowestMinPrice_) / float(k_mm_h);
    for(int i = 0; i < price_scale_num; i++)
    {
        int pos_y = (-1) * scale_part_h * i; 
        painter.drawText(mm_w - right_w_, pos_y, QString("%1").arg(lowestMinPrice_ + (price_per_len * scale_part_h * i) ));
        painter.drawLine(0, pos_y, mm_w - right_w_, pos_y);
    }
}


#include "kline_wall.h"

#include <cassert>
#include <tuple>
#include <QPainter>
#include <qevent.h> 
#include <qdebug.h>
//#include <QTime>
#include <qdatetime.h>
#include <QtWidgets/QMessageBox>

#include "mainwindow.h"
#include "stkfo_common.h"
#include "stk_forecast_app.h"
 

#define  WOKRPLACE_DEFUALT_K_NUM  (4*20)
#define  DEFAULT_CYCLE_TAG  "日线"

//static const QPoint CST_MAGIC_POINT(-1, -1);
static const int cst_default_year = 2017;

KLineWall::KLineWall(StkForecastApp *app, QWidget *parent) 
    : QWidget(parent) 
    , app_(app)
    , main_win_((MainWindow*)parent)
    , head_h_(30)
    , bottom1_h_(30)
    , bottom2_h_(30) 
    , stock_code_()
    , p_hisdata_container_(nullptr)
    , container_start_date_day_k_(0)
    , container_end_date_day_k_(0)
    , lowestMinPrice_(99.9)
    , highestMaxPrice_(0)
    , show_cross_line_(false)
    //, is_repaint_k_(true)
    , k_num_(WOKRPLACE_DEFUALT_K_NUM)
    , k_type_(TypePeriod::PERIOD_DAY)
    , k_cycle_tag_()
    , k_cycle_year_(0)
    , date_(0)
    , k_data_str_() 
	, stock_input_dlg_(this)
    , draw_action_(DrawAction::NO_ACTION)
    , pre_mm_w_(-1)
    , pre_mm_h_(-1)
{
    ui.setupUi(this);
    bottom_h_ = bottom1_h_ + bottom2_h_;
    ResetDrawState(DrawAction::NO_ACTION);
    //kline_pos_data_.reserve(WOKRPLACE_DEFUALT_K_NUM*2);
 
    // init ui -----------
    QPalette pal = this->palette();
    pal.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
 
}

bool KLineWall::Init()
{
    k_cycle_tag_ = DEFAULT_CYCLE_TAG;
    k_cycle_year_ = cst_default_year;
      
    auto ret = stockAllDaysInfo_.Init();
    if( ret )
    {
       return ResetStock("600196");
    }
    
    return false;
}

void KLineWall::Draw2pDownForcast(QPainter &painter, const int mm_h, double item_w)
{
    std::vector<T_Data2pDownForcast> *p_data_vector = forcast_man_.Find2pDownForcastVector(stock_code_, k_type_);

    if( p_data_vector && !p_data_vector->empty() )
    { 
        QPen pen;  
        //pen.setStyle(Qt::DotLine);
        pen.setColor(Qt::magenta);
        pen.setWidth(2);
        //painter.setPen(pen); 
        const auto font_size = painter.font().pointSizeF();
        std::vector<std::tuple<QPointF, double> >  fronts_to_draw;
        
        for( unsigned int i = 0; i < p_data_vector->size(); ++i )
        {
            T_Data2pDownForcast &data_2pforcastdown = p_data_vector->at(i);
            auto item_a = GetKLineDataItemByDate(data_2pforcastdown.date_a);
            auto item_b = GetKLineDataItemByDate(data_2pforcastdown.date_b);
            if( item_a && item_a->kline_posdata.date != 0 && item_b && item_b->kline_posdata.date != 0 )
            {
                if( abs(item_a->kline_posdata.top.y()) > abs(item_b->kline_posdata.top.y()) )  // y is negative
                {  
                    pen.setStyle(Qt::SolidLine); 
                    painter.setPen(pen);  
                    painter.drawLine(item_a->kline_posdata.top, item_b->kline_posdata.bottom);
fronts_to_draw.push_back(std::make_tuple(QPointF(item_a->kline_posdata.top.x()-item_w/2, item_a->kline_posdata.top.y()), item_a->stk_item.high_price));
fronts_to_draw.push_back(std::make_tuple(QPointF(item_b->kline_posdata.bottom.x()-item_w/2, item_b->kline_posdata.bottom.y() + painter.font().pointSizeF()), item_b->stk_item.low_price));
                    double y1 = get_pointc_y(data_2pforcastdown.c1, mm_h);
                    double y2 = get_pointc_y(data_2pforcastdown.c2, mm_h);
                    double y3 = get_pointc_y(data_2pforcastdown.c3, mm_h);
                    double x_b = item_b->kline_posdata.bottom.x();
                    pen.setStyle(Qt::DotLine); 
                    painter.setPen(pen);  
                    // vertical line ----
                    painter.drawLine(item_b->kline_posdata.bottom, QPointF(x_b, y1));
                    painter.drawLine(QPointF(x_b, y1), QPointF(x_b, y2));
                    painter.drawLine(QPointF(x_b, y2), QPointF(x_b, y3));
                    // horzon forcast line -----------
                    double h_line_left = item_b->kline_posdata.bottom.x() - item_w;
                    painter.drawLine(QPointF(h_line_left, y1), QPointF(x_b + 5*item_w, y1));
fronts_to_draw.push_back(std::make_tuple(QPointF(h_line_left - font_size*6, y1), data_2pforcastdown.c1));

                    painter.drawLine(QPointF(h_line_left, y2), QPointF(x_b + 10*item_w, y2));
fronts_to_draw.push_back(std::make_tuple(QPointF(h_line_left - font_size*6, y2), data_2pforcastdown.c2));

                    painter.drawLine(QPointF(h_line_left, y3), QPointF(x_b + 20*item_w, y3));
fronts_to_draw.push_back(std::make_tuple(QPointF(h_line_left - font_size*6, y3), data_2pforcastdown.c3));                        
                }else 
                {
                    painter.drawLine(item_a->kline_posdata.bottom, item_b->kline_posdata.top);

                }
            }
        } // for data_2pforcast_downs_

        pen.setColor(Qt::white);
        pen.setStyle(Qt::SolidLine); 
        painter.setPen(pen);
        char buf[32] = {0};
        std::for_each( std::begin(fronts_to_draw), std::end(fronts_to_draw), [&painter, &buf, this](std::tuple<QPointF, double>& in)
        {
            sprintf_s(buf, sizeof(buf), "%.2f\0", std::get<1>(in));
            painter.drawText(std::get<0>(in), buf);
        });
  }
  // end of paint 3pdates -------------------------

}

void KLineWall::Draw2pUpForcast(QPainter &painter, const int mm_h, double item_w)
{
    std::vector<T_Data2pUpForcast> *p_data_vector = forcast_man_.Find2pUpForcastVector(stock_code_, k_type_);
    if( !p_data_vector || p_data_vector->empty() )
        return;

    QPen pen;   
    pen.setColor(Qt::magenta);
    pen.setWidth(2);
    //painter.setPen(pen); 
    const auto font_size = painter.font().pointSizeF();
    std::vector<std::tuple<QPointF, double> >  fronts_to_draw;
        
    for( unsigned int i = 0; i < p_data_vector->size(); ++i )
    {
        T_Data2pUpForcast &data_2pforcast = p_data_vector->at(i);
        auto item_a = GetKLineDataItemByDate(data_2pforcast.date_a);
        auto item_b = GetKLineDataItemByDate(data_2pforcast.date_b);
        if( item_a && item_a->kline_posdata.date != 0 && item_b && item_b->kline_posdata.date != 0 )
        {
            if( abs(item_a->kline_posdata.top.y()) < abs(item_b->kline_posdata.top.y()) )  // y is negative
            {  
                pen.setStyle(Qt::SolidLine); 
                painter.setPen(pen);  
                painter.drawLine(item_a->kline_posdata.bottom, item_b->kline_posdata.top);
 fronts_to_draw.push_back(std::make_tuple(QPointF(item_a->kline_posdata.bottom.x()-item_w/2, item_a->kline_posdata.bottom.y()), item_a->stk_item.low_price));
 fronts_to_draw.push_back(std::make_tuple(QPointF(item_b->kline_posdata.top.x()-item_w/2, item_b->kline_posdata.top.y() + painter.font().pointSizeF()), item_b->stk_item.high_price));
                double y1 = get_pointc_y(data_2pforcast.c1, mm_h);
                double y2 = get_pointc_y(data_2pforcast.c2, mm_h);
                double y3 = get_pointc_y(data_2pforcast.c3, mm_h);
                double x_b = item_b->kline_posdata.bottom.x();
                pen.setStyle(Qt::DotLine); 
                painter.setPen(pen);  
                // vertical line ----
                painter.drawLine(item_b->kline_posdata.top, QPointF(x_b, y3));
                // horzon forcast line -----------
                double h_line_left = item_b->kline_posdata.bottom.x() - item_w;
                painter.drawLine(QPointF(h_line_left, y1), QPointF(x_b + 5*item_w, y1));
fronts_to_draw.push_back(std::make_tuple(QPointF(h_line_left - font_size*6, y1), data_2pforcast.c1));

                painter.drawLine(QPointF(h_line_left, y2), QPointF(x_b + 10*item_w, y2));
fronts_to_draw.push_back(std::make_tuple(QPointF(h_line_left - font_size*6, y2), data_2pforcast.c2));

                painter.drawLine(QPointF(h_line_left, y3), QPointF(x_b + 20*item_w, y3));
fronts_to_draw.push_back(std::make_tuple(QPointF(h_line_left - font_size*6, y3), data_2pforcast.c3));                        
            }else 
            {
                painter.drawLine(item_a->kline_posdata.top, item_b->kline_posdata.bottom);
            }
        }
    } // for data_2pforcast_downs_

    pen.setColor(Qt::white);
    pen.setStyle(Qt::SolidLine); 
    painter.setPen(pen);
    char buf[32] = {0};
    std::for_each( std::begin(fronts_to_draw), std::end(fronts_to_draw), [&painter, &buf, this](std::tuple<QPointF, double>& in)
    {
        sprintf_s(buf, sizeof(buf), "%.2f\0", std::get<1>(in));
        painter.drawText(std::get<0>(in), buf);
    });
}

void KLineWall::Draw3pDownForcast(QPainter &painter, const int mm_h, double item_w)
{
    std::vector<T_Data3pForcast> *p_data_vector = forcast_man_.Find3pForcastVector(stock_code_, k_type_, true);
    if( !p_data_vector || p_data_vector->empty() )
        return;

    QPen pen;  
    pen.setColor(Qt::magenta);
    pen.setWidth(2);
    const auto font_size = painter.font().pointSizeF();
    std::vector<std::tuple<QPointF, double> >  fronts_to_draw;
        
    for( unsigned int i = 0; i < p_data_vector->size(); ++i )
    {
        T_Data3pForcast &data_3pdown_forcast = p_data_vector->at(i);
        auto item_a = GetKLineDataItemByDate(data_3pdown_forcast.date_a);
        auto item_b = GetKLineDataItemByDate(data_3pdown_forcast.date_b);
        auto item_c = GetKLineDataItemByDate(data_3pdown_forcast.date_c);
        if( item_a && item_a->kline_posdata.date != 0 && item_b && item_b->kline_posdata.date != 0 )
        {
            if( abs(item_a->kline_posdata.top.y()) > abs(item_b->kline_posdata.top.y()) )  // y is negative
            {  
                pen.setStyle(Qt::SolidLine);
                painter.setPen(pen); 
                painter.drawLine(item_a->kline_posdata.top, item_b->kline_posdata.bottom);
                fronts_to_draw.push_back(std::make_tuple(QPointF(item_a->kline_posdata.top.x()-item_w/2, item_a->kline_posdata.top.y()), item_a->stk_item.high_price));
                if( item_c )
                {
                    painter.drawLine(item_b->kline_posdata.bottom, item_c->kline_posdata.top);
                    fronts_to_draw.push_back(std::make_tuple(QPointF(item_b->kline_posdata.bottom.x()-item_w/2, item_b->kline_posdata.bottom.y()), item_b->stk_item.low_price));
                    fronts_to_draw.push_back(std::make_tuple(QPointF(item_c->kline_posdata.top.x()-item_w/2, item_c->kline_posdata.top.y()), item_c->stk_item.high_price));
                    double y1 = get_pointc_y(data_3pdown_forcast.d1, mm_h);
                    double y2 = get_pointc_y(data_3pdown_forcast.d2, mm_h);
                    double y3 = get_pointc_y(data_3pdown_forcast.d3, mm_h);
                    double x_c = item_c->kline_posdata.top.x();
                    pen.setStyle(Qt::DotLine);
                    painter.setPen(pen); 
                    // vertical line ----
                    painter.drawLine(item_c->kline_posdata.top, QPointF(x_c, y3));
                    // horzon forcast line -----------
                    double h_line_left = item_c->kline_posdata.top.x() - item_w;
                    painter.drawLine(QPointF(h_line_left, y1), QPointF(x_c + 5*item_w, y1));
                    fronts_to_draw.push_back(std::make_tuple(QPointF(h_line_left - font_size*6, y1), data_3pdown_forcast.d1));

                    painter.drawLine(QPointF(h_line_left, y2), QPointF(x_c + 10*item_w, y2));
                    fronts_to_draw.push_back(std::make_tuple(QPointF(h_line_left - font_size*6, y2), data_3pdown_forcast.d2));

                    painter.drawLine(QPointF(h_line_left, y3), QPointF(x_c + 20*item_w, y3));
                    fronts_to_draw.push_back(std::make_tuple(QPointF(h_line_left - font_size*6, y3), data_3pdown_forcast.d3));     
                }
            }
        }
        
    }// for
    pen.setColor(Qt::white);
    pen.setStyle(Qt::SolidLine); 
    painter.setPen(pen);
    char buf[32] = {0};
    std::for_each( std::begin(fronts_to_draw), std::end(fronts_to_draw), [&painter, &buf, this](std::tuple<QPointF, double>& in)
    {
        sprintf_s(buf, sizeof(buf), "%.2f\0", std::get<1>(in));
        painter.drawText(std::get<0>(in), buf);
    });
}

void Draw3pUpForcast(QPainter &, const int mm_h, double item_w)
{

}

void KLineWall::UpdatePosDatas()
{
    assert(p_hisdata_container_);
    if( k_num_ <= 0 )
        return;
    // before update get pre item which drawing point in -------
    T_KlineDataItem *item_a = nullptr;
    T_KlineDataItem *item_b = nullptr;
    T_KlineDataItem *item_c = nullptr;
    if( drawing_line_A_ != CST_MAGIC_POINT )
        item_a = GetKLineDataItemByXpos(drawing_line_A_.x());
    if( drawing_line_B_ != CST_MAGIC_POINT )
        item_b = GetKLineDataItemByXpos(drawing_line_B_.x());
    if( drawing_line_C_ != CST_MAGIC_POINT )
        item_c = GetKLineDataItemByXpos(drawing_line_C_.x());

    // update ----------------------------------------------
    const int mm_h = this->height() - head_h_ - bottom_h_;
    const int mm_w = this->width();
    const int empty_right_w = 30;
    static const int right_w = 30;
    double item_w = double(mm_w - empty_right_w - right_w) / double(k_num_ + 1) ;
    //double space_between_k = item_w / 4;
    double k_bar_w = item_w * 3 / 4;
    // clear position data
    std::for_each( std::begin(*p_hisdata_container_), std::end(*p_hisdata_container_), [this](T_HisDataItemContainer::reference entry)
    {
        entry->kline_posdata.Clear();
    });

#ifdef DRAW_FROM_LEFT
    int j = 0;
    for( auto iter = p_hisdata_container_->begin();
        iter != p_hisdata_container_->end() && j < k_num_; 
        ++iter, ++j )
    { 
        T_KlinePosData &pos_data = iter->get()->kline_posdata;

        pos_data.date = iter->get()->stk_item.date;

        auto openPrice = (*iter)->stk_item.open_price;
        auto closePrice = (*iter)->stk_item.close_price;
        auto minPrice = (*iter)->stk_item.low_price;
        auto maxPrice = (*iter)->stk_item.high_price;

        pos_data.x_left = j * item_w + 1;
        pos_data.x_right = pos_data.x_left + k_bar_w;

        auto pos_y = -1 * mm_h * (openPrice - lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_);
        pos_data.height = -1 * mm_h *(closePrice - openPrice)/(highestMaxPrice_ - lowestMinPrice_);
        pos_data.columnar_top_left = QPoint(j * item_w + 1, pos_y);
         
        pos_data.top = QPoint(j * item_w + k_bar_w / 2, -1 * mm_h * (maxPrice-lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_));
        pos_data.bottom = QPoint(j * item_w + k_bar_w / 2, -1 * mm_h * (minPrice-lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_));
    }
#else

    // update position data --------------------------------
    int j = k_num_;
    auto right_end = double(mm_w - empty_right_w - right_w) - k_bar_w;

    for( auto iter = p_hisdata_container_->rbegin();
        iter != p_hisdata_container_->rend() && j > 0; 
        ++iter, --j)
    { 
        T_KlinePosData &pos_data = iter->get()->kline_posdata; 
        pos_data.date = iter->get()->stk_item.date;
        auto openPrice = (*iter)->stk_item.open_price;
        auto closePrice = (*iter)->stk_item.close_price;
        auto minPrice = (*iter)->stk_item.low_price;
        auto maxPrice = (*iter)->stk_item.high_price;

        //pos_data.x_left = j * item_w + 1;
        pos_data.x_right = right_end - item_w * (k_num_ - j);
        pos_data.x_left = pos_data.x_right - k_bar_w;
        auto pos_y = -1 * (openPrice - lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_) * mm_h;
        pos_data.height = -1 * (closePrice - openPrice)/(highestMaxPrice_ - lowestMinPrice_) * mm_h;
        pos_data.columnar_top_left = QPointF(pos_data.x_left, pos_y);

        pos_data.top = QPointF(pos_data.x_left + k_bar_w / 2, -1 * (maxPrice-lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_) * mm_h);
        pos_data.bottom = QPointF(pos_data.x_left + k_bar_w / 2, -1 * (minPrice-lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_) * mm_h);

        // update drawing line point ---------------------
        if( item_a && item_a->stk_item.date == iter->get()->stk_item.date )
        {
            switch(draw_action_)
            {
            case DrawAction::DRAWING_FOR_2PDOWN_C:
            case DrawAction::DRAWING_FOR_3PDOWN_D:
                drawing_line_A_ = pos_data.top;
                break;
            case DrawAction::DRAWING_FOR_2PUP_C:
            case DrawAction::DRAWING_FOR_3PUP_D:
                drawing_line_A_ = pos_data.bottom;
                break;
            }
        }else if( item_b && item_b->stk_item.date == iter->get()->stk_item.date )
        {
            switch(draw_action_)
            {
            case DrawAction::DRAWING_FOR_2PDOWN_C:
            case DrawAction::DRAWING_FOR_3PDOWN_D:
                drawing_line_B_ = pos_data.bottom;
                break;
            case DrawAction::DRAWING_FOR_2PUP_C:
            case DrawAction::DRAWING_FOR_3PUP_D:
                drawing_line_B_ = pos_data.top;
                break;
            }
        }else if( item_c && item_c->stk_item.date == iter->get()->stk_item.date )
        {
            switch(draw_action_)
            { 
            case DrawAction::DRAWING_FOR_3PDOWN_D:
                drawing_line_C_ = pos_data.top;
                break; 
            case DrawAction::DRAWING_FOR_3PUP_D:
                drawing_line_C_ = pos_data.bottom;
                break;
            }
        } 
    }
#endif
}

void KLineWall::mousePressEvent(QMouseEvent * event )
{ 
    static auto append_3pforcast_data = [this](bool is_down, T_KlineDataItem &item_a, T_KlineDataItem &item_b)
    {
        T_Data3pForcast  data_3p;
        data_3p.stock_code = stock_code_;
        data_3p.is_down = is_down;
        data_3p.date_a = item_a.stk_item.date; 
        data_3p.date_b = item_b.stk_item.date; 
        this->forcast_man_.Append(k_type_, stock_code_, true, data_3p);
    };
    //qDebug() << "paintEvent QCursor::pos  x:" << QCursor::pos().x() << " y: "<< QCursor::pos().y() << "\n"; 
     
	if( stock_input_dlg_.isVisible() )
		stock_input_dlg_.hide();

    if( draw_action_ == DrawAction::NO_ACTION )
        return;

    if( drawing_line_A_ == CST_MAGIC_POINT )
    {
        drawing_line_A_ = QPointF( event->pos().x(), event->pos().y() - height_axis_trans_in_paint_k());
        return;
    }

    auto item_a = GetKLineDataItemByXpos(drawing_line_A_.x());
    if( !item_a )
    { // todo: show warning msg
        return ResetDrawState(DrawAction::NO_ACTION); 
    }

    if( drawing_line_B_ == CST_MAGIC_POINT ) // if to generate point B
    {
        auto item_b = GetKLineDataItemByXpos(event->pos().x());
        if( !item_b )
            return;
        if( item_b->stk_item.date <= item_a->stk_item.date )
        {   // todo: show warning msg
            return;
        }
        if( draw_action_ == DrawAction::DRAWING_FOR_2PDOWN_C )
        { 
            if( forcast_man_.HasIn2pDownwardForcast(stock_code_, k_type_, *item_a, *item_b) )
                return ResetDrawState(draw_action_);  
            if( item_a->stk_item.high_price > item_b->stk_item.high_price )
            {
                T_Data2pDownForcast data_2pdown_fcst;
                data_2pdown_fcst.stock_code = stock_code_;
                data_2pdown_fcst.date_a = item_a->stk_item.date; 
                data_2pdown_fcst.date_b = item_b->stk_item.date; 

                auto c1_c2_c3 = ForcastC_ABDown(item_a->stk_item.high_price, item_b->stk_item.low_price);
                data_2pdown_fcst.c1 = std::get<0>(c1_c2_c3);
                data_2pdown_fcst.c2 = std::get<1>(c1_c2_c3);
                data_2pdown_fcst.c3 = std::get<2>(c1_c2_c3);
                forcast_man_.Append(k_type_, stock_code_, data_2pdown_fcst);
            } 
            return ResetDrawState(draw_action_); 

        }else if( draw_action_ == DrawAction::DRAWING_FOR_2PUP_C ) 
        {
            if( forcast_man_.HasIn2pUpForcast(stock_code_, k_type_, *item_a, *item_b) )
                return ResetDrawState(draw_action_);  
            if( item_a->stk_item.high_price < item_b->stk_item.high_price )
            {
                T_Data2pUpForcast data_2pup_fcst;
                data_2pup_fcst.stock_code = stock_code_; 
                data_2pup_fcst.date_a = item_a->stk_item.date; 
                data_2pup_fcst.date_b = item_b->stk_item.date; 

                auto c1_c2_c3 = ForcastC_ABUp(item_a->stk_item.low_price, item_b->stk_item.high_price);
                data_2pup_fcst.c1 = std::get<0>(c1_c2_c3);
                data_2pup_fcst.c2 = std::get<1>(c1_c2_c3);
                data_2pup_fcst.c3 = std::get<2>(c1_c2_c3);
                forcast_man_.Append(k_type_, stock_code_, data_2pup_fcst);
            }
            return ResetDrawState(draw_action_);  

        }else if( draw_action_ == DrawAction::DRAWING_FOR_3PDOWN_D )
        { 
            if( forcast_man_.Find3pForcast(stock_code_, k_type_, true, *item_a, *item_b) ) // already exist
                return ResetDrawState(draw_action_);  
            if( item_a->stk_item.high_price > item_b->stk_item.high_price )
            {
                drawing_line_B_ = item_b->kline_posdata.bottom;
                append_3pforcast_data(true, *item_a, *item_b);
                return;
            }else
            {
                // todo: show warning
            }

        }else if( draw_action_ == DrawAction::DRAWING_FOR_3PUP_D )
        {
            if( forcast_man_.Find3pForcast(stock_code_, k_type_, false, *item_a, *item_b) ) // already exist
                return ResetDrawState(draw_action_);  

            if( item_a->stk_item.high_price < item_b->stk_item.high_price )
            { 
                drawing_line_B_ = item_b->kline_posdata.top; 
                append_3pforcast_data(false, *item_a, *item_b);
                return;
            }else
            {
                // todo: show warning
            }
        }
    } // if( drawing_line_B_ == CST_MAGIC_POINT )
    else if( drawing_line_C_ == CST_MAGIC_POINT ) // if to generate point C
    {
        // judge date
        auto item_b = GetKLineDataItemByXpos(drawing_line_B_.x());
        if( !item_b )
        {   // todo: show warning msg
            return ResetDrawState(DrawAction::NO_ACTION); 
        }
        auto item_c = GetKLineDataItemByXpos(event->pos().x());
        if( !item_c || item_c->stk_item.date <= item_b->stk_item.date)
        {   // todo: show warning msg
            return;
        }

        if( draw_action_ == DrawAction::DRAWING_FOR_3PDOWN_D )
        {
            auto p_data_3pfcst = forcast_man_.Find3pForcast(stock_code_, k_type_, true, *item_a, *item_b);
            assert(p_data_3pfcst);
            // set to point C-----
            p_data_3pfcst->date_c = item_c->stk_item.date;
            //drawing_line_C_ = item_c->kline_posdata.top;
            auto d1_d2_d3 = ForcastD_ABC_Down(item_a->stk_item.high_price, item_b->stk_item.low_price);
            p_data_3pfcst->d1 = std::get<0>(d1_d2_d3);
            p_data_3pfcst->d2 = std::get<1>(d1_d2_d3);
            p_data_3pfcst->d3 = std::get<2>(d1_d2_d3);
            return ResetDrawState(draw_action_);  
        }else if( draw_action_ == DrawAction::DRAWING_FOR_3PUP_D )
        {
            auto p_data_3pfcst = forcast_man_.Find3pForcast(stock_code_, k_type_, false, *item_a, *item_b);
            assert(p_data_3pfcst);
            // set to point C-----
            p_data_3pfcst->date_c = item_c->stk_item.date;
            //drawing_line_C_ = item_b->kline_posdata.bottom;
            auto d1_d2_d3 = ForcastD_ABC_Up(item_a->stk_item.low_price, item_b->stk_item.high_price);
            p_data_3pfcst->d1 = std::get<0>(d1_d2_d3);
            p_data_3pfcst->d2 = std::get<1>(d1_d2_d3);
            p_data_3pfcst->d3 = std::get<2>(d1_d2_d3);
            return ResetDrawState(draw_action_);  
        }
    }        
}

void KLineWall::paintEvent(QPaintEvent*)
{
    static auto IsAreaShapeChange = [this](int w, int h)->bool
    {
        return w != this->pre_mm_w_ || h!= this->pre_mm_h_;
    }; 
 
    QPainter painter(this); 
    //qDebug() << "paintEvent QCursor::pos  x:" << QCursor::pos().x() << " y: "<< QCursor::pos().y() << "\n";
    auto pos_from_global = mapFromGlobal(QCursor::pos());
    //qDebug() << "paintEvent x:" << pos_from_global.x() << " y: "<< pos_from_global.y() << "\n";
     
    const int mm_h = this->height() - head_h_ - bottom_h_;
    const int mm_w = this->width();
    const int empty_right_w = 30;
    const int right_w = 30;
    float price_per_len = (highestMaxPrice_ - lowestMinPrice_) / float(mm_h);
      
    const bool is_area_shape_change = IsAreaShapeChange(this->width(), this->height());
    if( is_area_shape_change )
    {
        UpdatePosDatas(); 
    }
     /*
     ------------>
     |
     |
    \|/
    纵坐标(各行价格值)：((HighestMaxPrice - lowestMinPrice_) * i)/7  + lowestMinPrice_
    横坐标(日期)：分成60等份，每天15像素，每天的横坐标
    j*15+1  j*15*8
    每天的纵坐标:开盘价:350*(开盘价-lowestMinPrice_)/(HighestMaxPrice - lowestMinPrice_)
    收盘价:350*(收盘价-lowestMinPrice_)/(HighestMaxPrice - lowestMinPrice_)
    最高价:350*(最高价-lowestMinPrice_)/(HighestMaxPrice - lowestMinPrice_)
    最低价:350*(最盘价-lowestMinPrice_)/(HighestMaxPrice - lowestMinPrice_)

    矩形宽度：13像素
    矩形高度：350*(开盘价-收盘价)/(HighestMaxPrice - lowestMinPrice_)像素
    QFont font;  
    font.setPointSize(Y_SCALE_FONT_SIZE);      font.setFamily("Microsoft YaHei");  
    font.setLetterSpacing(QFont::AbsoluteSpacing,0);      painter.setFont(font);  
    //获取这种字体情况下，文本的长度和高度  
    QFontMetricsF fontMetrics(font);  
    qreal wid = fontMetrics.width(scale_val_str);      qreal height = fontMetrics.height();  
    */ 
    //painter.translate(30, 400);  //坐标平移
    //painter.drawText(0, 0, "(0,0)");

    painter.translate(0, height_axis_trans_in_paint_k()); // translate frame of axes to bottom
    QPointF cur_mous_point_trans(cur_mouse_point_.x(), cur_mouse_point_.y() - height_axis_trans_in_paint_k());

    do
    {   
        if( draw_action_ == DrawAction::NO_ACTION ) 
            break;
        if( drawing_line_A_ == CST_MAGIC_POINT ) 
            break;
        QPen pen;
        pen.setColor(Qt::magenta);
        pen.setWidth(2);
        painter.setPen(pen); 
        if( drawing_line_B_ == CST_MAGIC_POINT ) 
        {
            painter.drawLine(drawing_line_A_.x(), drawing_line_A_.y(), cur_mous_point_trans.x(), cur_mous_point_trans.y() );
            break;
        }else if( draw_action_ == DrawAction::DRAWING_FOR_3PDOWN_D || draw_action_ == DrawAction::DRAWING_FOR_3PUP_D )
        {
            painter.drawLine(drawing_line_B_.x(), drawing_line_B_.y(), cur_mous_point_trans.x(), cur_mous_point_trans.y() );
            break;
        }
        //qDebug() << " mouseMoveEvent DRAWING_FOR_2PDOWN_C " << drawing_line_A_.x() << " " << drawing_line_A_.y() << " " << cur_mouse_point_.x() << " " << cur_mouse_point_.y() << "\n";
    }while(0);

    QPen pen; 
    pen.setColor(Qt::white);
    painter.setPen(pen);
    auto old_font = painter.font();
     
    QFont font;  
    font.setPointSize(old_font.pointSize() * 2); 
   
    painter.setFont(font);
    painter.drawText(mm_w - right_w - 70, -1 *(this->height() - 100), stock_code_.c_str());
    painter.setFont(old_font); 
    pen.setColor(Qt::red);
    painter.setPen(pen);

    // right line
    painter.drawLine(mm_w - right_w, bottom_h_, mm_w - right_w, -1 * this->height());

    // bottom calender  ------------------------------------
    painter.drawLine(0, bottom1_h_, mm_w, bottom1_h_);
    painter.drawLine(0, bottom_h_-1, mm_w, bottom_h_-1);

    painter.drawText(mm_w - right_w, bottom_h_-1, QString::fromLocal8Bit(k_cycle_tag_.c_str()));
    painter.drawText(0, bottom_h_-1, QString("%1").arg(k_cycle_year_));
     
    pen.setStyle(Qt::DotLine); // ............
    painter.setPen(pen); 
    // right vertical'  price scale 
    const int num = 8;
    const int part_h = mm_h / num;
    for(int i = 0; i < num; i++)
    {
        int pos_y = (-1) * part_h*i; 
        painter.drawText(mm_w - right_w, pos_y, QString("%1").arg(lowestMinPrice_ + (price_per_len * part_h * i) ));
        painter.drawLine(0, pos_y, mm_w-right_w, pos_y);
    }
      
    //draw all k line -------------------------------------
      
    float openPrice; 
    float closePrice; 
    float maxPrice; 
    float minPrice; 
    //float marketMoney; //成交额
     
	if( p_hisdata_container_ )
	{   
        double item_w = double(mm_w - empty_right_w - right_w) / double(k_num_ + 1) ;
       
    // draw k_num_ k line ----------------------------------
#ifdef DRAW_FROM_LEFT
    int j = 0;
	for( auto iter = p_hisdata_container_->begin();
		iter != p_hisdata_container_->end() && j < k_num_; 
		++iter, ++j)
    { 
#else
        int j = k_num_;
        for( auto iter = p_hisdata_container_->rbegin();
            iter != p_hisdata_container_->rend() && j > 0; 
            ++iter, --j)
        { 
#endif
        T_KlinePosData &pos_data = iter->get()->kline_posdata;

        bool is_lowest_k = false;
        //draw every k line---------------
		minPrice = (*iter)->stk_item.low_price;
		maxPrice = (*iter)->stk_item.high_price;
		openPrice = (*iter)->stk_item.open_price;
		closePrice = (*iter)->stk_item.close_price;

        //qDebug() << openPrice<< "\t"  << closePrice << minPrice << "\t" << maxPrice<< "\t" << "\n";
          
		QBrush brush(QColor(255,0,0));  
		pen.setStyle(Qt::SolidLine);
         
        if(openPrice <= closePrice)
        {
            pen.setColor(QColor(255,0,0));
			brush.setColor(QColor(255,0,0));
        }else
        { 
            pen.setColor(QColor(0,255,0)); 
			brush.setColor(QColor(0,255,0));
        }
        // fengxin relate -------------------
        if( ((*iter)->type & BTM_AXIS_T_3) == BTM_AXIS_T_3 
            || ((*iter)->type & BTM_AXIS_T_5) == BTM_AXIS_T_5 
            || ((*iter)->type & BTM_AXIS_T_7) == BTM_AXIS_T_7
            || ((*iter)->type & BTM_AXIS_T_9) == BTM_AXIS_T_9
            || ((*iter)->type & BTM_AXIS_T_11) == BTM_AXIS_T_11)
        {
            //pen.setColor(QColor(0,0,255)); 
            //brush.setColor(QColor(0,0,255));
            if( iter->get()->stk_item.low_price < this->lowestMinPrice_ + 0.001 )
            {
                is_lowest_k = true; 
            }
        }else if( ((*iter)->type & TOP_AXIS_T_3) == TOP_AXIS_T_3 
            || ((*iter)->type & TOP_AXIS_T_5) == TOP_AXIS_T_5 
            || ((*iter)->type & TOP_AXIS_T_7) == TOP_AXIS_T_7
            || ((*iter)->type & TOP_AXIS_T_9) == TOP_AXIS_T_9
            || ((*iter)->type & TOP_AXIS_T_11) == TOP_AXIS_T_11)
        {
            //pen.setColor(QColor(0,255,255)); 
            //brush.setColor(QColor(0,255,255));
        }
		
		painter.setPen(pen);  
        painter.setBrush(brush);   
        // draw k columnar  ---------------------------------------    
        painter.drawRect(pos_data.columnar_top_left.x(), pos_data.columnar_top_left.y(), pos_data.x_right - pos_data.x_left, pos_data.height);
        painter.drawLine(pos_data.top.x(), pos_data.top.y(), pos_data.bottom.x(), pos_data.bottom.y());
 
        if( pos_from_global.x() >= pos_data.x_left && pos_from_global.x() <= pos_data.x_right )
            k_data_str_ = std::to_string((*iter)->stk_item.date);
      }  // for all k line 

        // paint 3pdatas ----------------------
        Draw2pDownForcast(painter, mm_h, item_w);
        Draw2pUpForcast(painter, mm_h, item_w);  

        Draw3pDownForcast(painter, mm_h, item_w);
    }

	// translate axis back
    painter.translate(0, -1 * height_axis_trans_in_paint_k());
    
    pen.setColor(Qt::white);
    pen.setStyle(Qt::SolidLine); 
    painter.setPen(pen);
    if( show_cross_line_ )
    {
        qDebug() << " show_cross_line_ pos y " << (float)pos_from_global.y() << "\n";
        // horizontal line 
        painter.drawLine(0, pos_from_global.y(), mm_w-right_w, pos_from_global.y());
        // vertical line 
        painter.drawLine(pos_from_global.x(), head_h_, pos_from_global.x(), this->height()); 
        painter.drawText( mm_w-right_w, pos_from_global.y(), QString("%1").arg(lowestMinPrice_ + price_per_len * (this->height() - bottom_h_ - pos_from_global.y()) ) );
    }
    painter.drawText(pos_from_global.x(), this->height()-1, k_data_str_.c_str());
   
    this->pre_mm_w_ = this->width();
    this->pre_mm_h_ = this->height();
}


void KLineWall::mouseDoubleClickEvent(QMouseEvent*)
{
    //qDebug() << " x:" << e->pos().x() <<" y:" << e->pos().y() << "\n";
    show_cross_line_ = !show_cross_line_;
    //qDebug() << "show_cross_line:" << show_cross_line_ << "\n";
    setMouseTracking(show_cross_line_);
      
    update();
}

// this->mapFromGlobal(this->cursor().pos()) == event.pos()
void KLineWall::mouseMoveEvent(QMouseEvent *e)
{ 
    auto pos = e->pos();
    //qDebug() << " mouseMoveEvent " << "\n";
    //auto pos_mapped = mapToGlobal(pos);
          
    //auto pos_mapped = mapFromGlobal(e->pos());
    //auto pos_mapped = mapToParent(e->pos());
    //is_repaint_k_ = false;

    if( draw_action_ != DrawAction::NO_ACTION )
    {
        //qDebug() << " mouseMoveEvent DRAWING_FOR_2PDOWN_C " << "\n";
        cur_mouse_point_ = e->pos();
    }
    update();
}

void KLineWall::keyPressEvent(QKeyEvent *e)
{
    //qDebug() << "key " << e->key() << "\n";
    assert(p_hisdata_container_);
    auto key_val = e->key();
    switch( key_val )
    {
    case Qt::Key_Up:
        {
            if( p_hisdata_container_->empty() )
                return;
            if( k_num_ > 1 )
            {
                k_num_ --;
                UpdatePosDatas();
                update();
            }
            break;
        }
    case Qt::Key_Down:
        {  
            k_num_ ++;
            if( k_num_ > p_hisdata_container_->size() )
            { 
                int oldest_day = QDateTime::currentDateTime().toString("yyyyMMdd").toInt();
                if( !p_hisdata_container_->empty() )
                    oldest_day = p_hisdata_container_->front()->stk_item.date;

                QDate qdate_obj(oldest_day/10000, (oldest_day%10000)/100, oldest_day%100);
                //int mons = p_hisdata_container_->size() / 20;
                auto start_date = qdate_obj.addDays( -1 * (4 * 30) ).toString("yyyyMMdd").toInt(); 
                auto p_container = stockAllDaysInfo_.AppendStockData(ToPeriodType(k_type_), stock_code_, start_date, oldest_day);
            }
            T_HisDataItemContainer &container = stockAllDaysInfo_.GetHisDataContainer(ToPeriodType(k_type_), stock_code_);
            int start_index = container.size() - k_num_; 
            start_index = start_index > 0 ? start_index : 0;
            int end_index = container.size() - 1 > 0 ? container.size() - 1 : 0;
            if( container.at(start_index)->stk_item.high_price > this->highestMaxPrice_ )
                this->highestMaxPrice_ = container.at(start_index)->stk_item.high_price;

            if( container.at(end_index)->stk_item.high_price > this->highestMaxPrice_ )
                this->highestMaxPrice_ = container.at(end_index)->stk_item.high_price;

            if( container.at(start_index)->stk_item.low_price < this->lowestMinPrice_ )
                this->lowestMinPrice_ = container.at(start_index)->stk_item.low_price;

            if( container.at(end_index)->stk_item.low_price < this->lowestMinPrice_ )
                this->lowestMinPrice_ = container.at(end_index)->stk_item.low_price; 
            
            UpdatePosDatas();
            update();
            break;
        } 
    case Qt::Key_0: case Qt::Key_1: case Qt::Key_2: case Qt::Key_3: case Qt::Key_4:  
    case Qt::Key_5: case Qt::Key_6: case Qt::Key_7: case Qt::Key_8: case Qt::Key_9: 
    case Qt::Key_A: case Qt::Key_B: case Qt::Key_C: case Qt::Key_D: case Qt::Key_E:
    case Qt::Key_F: case Qt::Key_G: case Qt::Key_H: case Qt::Key_I: case Qt::Key_J:
    case Qt::Key_K: case Qt::Key_L: case Qt::Key_M: case Qt::Key_N: case Qt::Key_O:
    case Qt::Key_P: case Qt::Key_Q: case Qt::Key_R: case Qt::Key_S: case Qt::Key_T:
    case Qt::Key_U: case Qt::Key_V: case Qt::Key_W: case Qt::Key_X: case Qt::Key_Y:
    case Qt::Key_Z:
		{
			qDebug() << __FUNCDNAME__ << "\n"; 
            stock_input_dlg_.ui.stock_input->clear();
            char tmpbuf[8] = {0};
            if( key_val >= Qt::Key_0 && key_val <= Qt::Key_9 )
                sprintf_s(tmpbuf, sizeof(tmpbuf), "%c", (char)key_val);
            else
                sprintf_s(tmpbuf, sizeof(tmpbuf), "%c", char(key_val+32));
            stock_input_dlg_.ui.stock_input->setText(tmpbuf);
			stock_input_dlg_.show();
		}
		break;
    default:
        break;
    }
     
    e->ignore();
}

void KLineWall::enterEvent(QEvent *)
{
    qDebug() << __FUNCTION__ << "\n";
}

void KLineWall::leaveEvent(QEvent *)
{
    qDebug() << __FUNCTION__ << "\n";
}

bool KLineWall::ResetStock(const QString& stock)
{  
    stock_code_ = stock.toLocal8Bit().data(); 

    auto cur_date = QDate::currentDate().year() * 10000 + QDate::currentDate().month() * 100 + QDate::currentDate().day();
    // 20 k line per 30 days
    auto start_date = QDate::currentDate().addDays(-1 * (WOKRPLACE_DEFUALT_K_NUM / 20 * 30) ).toString("yyyyMMdd").toInt();
	
    //p_hisdata_container_ = stockAllDaysInfo_.AppendStockData(stock_code_, 20171216, 20180108); 
	p_hisdata_container_ = stockAllDaysInfo_.AppendStockData(ToPeriodType(k_type_), stock_code_, start_date, cur_date);
	if( !p_hisdata_container_ )
		return false;
    container_start_date_day_k_ = start_date;
    container_end_date_day_k_ = cur_date;
    if( !p_hisdata_container_->empty() )
    {
        k_num_ = p_hisdata_container_->size() <= 60 ? p_hisdata_container_->size() : p_hisdata_container_->size() / 2;

#if 1
        int start_index = p_hisdata_container_->size() - k_num_; 
        start_index = start_index > 0 ? start_index : 0;
        int begin_date = p_hisdata_container_->at(start_index)->stk_item.date;
        this->highestMaxPrice_ = stockAllDaysInfo_.GetHisDataHighestMaxPrice(ToPeriodType(k_type_), stock_code_, begin_date, cur_date);
        this->lowestMinPrice_ = stockAllDaysInfo_.GetHisDataLowestMinPrice(ToPeriodType(k_type_), stock_code_, begin_date, cur_date);
#endif
    }else
    {
        this->highestMaxPrice_ = 20.0;
        this->lowestMinPrice_ = 0.0;
    }
    
    UpdatePosDatas();
 	return true;
}

T_KlineDataItem * KLineWall::GetKLineDataItemByXpos(int x)
{
#ifdef DRAW_FROM_LEFT
    int j = 0;
    for( auto iter = p_hisdata_container_->begin();
        iter != p_hisdata_container_->end() && j < k_num_; 
        ++iter, ++j)
    { 
#else
    int j = k_num_;
    for( auto iter = p_hisdata_container_->rbegin();
        iter != p_hisdata_container_->rend() && j > 0; 
        ++iter, --j)
    { 
#endif
        T_KlinePosData &pos_data = iter->get()->kline_posdata;
        if( pos_data.x_left == CST_MAGIC_POINT.x() )
            continue;
        if( (double)x > pos_data.x_left - 0.0001 && (double)x <= pos_data.x_right + 0.0001 )
            return iter->get();
    }
    return nullptr;
}

QPointF KLineWall::GetPointFromKLineDataItems(int x, bool is_get_top)
{
    //ps: dont't need untranslate cause x hadn't been translated : painter.translate(0, ...); //  
    /*int j = 0;
    for( auto iter = p_hisdata_container_->begin();
        iter != p_hisdata_container_->end() && j < k_num_; 
        ++iter, ++j)
    { 
        T_KlinePosData &pos_data = iter->get()->kline_posdata;
        if( pos_data.x_left == CST_MAGIC_POINT.x() )
            continue;
        if( x >= pos_data.x_left && x <= pos_data.x_right )
            return is_get_top ? pos_data.top : pos_data.bottom;
    }*/
    auto p_item = GetKLineDataItemByXpos(x);
    if( p_item ) return is_get_top ? p_item->kline_posdata.top : p_item->kline_posdata.bottom;
    return CST_MAGIC_POINT;
}

T_KlineDataItem * KLineWall::GetKLineDataItemByDate(int date)
{ 
    for( auto iter = p_hisdata_container_->rbegin();
        iter != p_hisdata_container_->rend(); 
        ++iter )
    {   
        if( iter->get()->stk_item.date == date )
            return iter->get(); 
    }
    return nullptr;
}

T_KlinePosData * KLineWall::GetKLinePosDataByDate(int date)
{
    for( auto iter = p_hisdata_container_->rbegin();
        iter != p_hisdata_container_->rend(); 
        ++iter )
    {   
        if( iter->get()->kline_posdata.date == date )
            return std::addressof(iter->get()->kline_posdata);
    }
    return nullptr;
}

void KLineWall::StockInputDlgRet()
{
	QString stock_code = stock_input_dlg_.ui.stock_input->text().trimmed();
	stock_input_dlg_.ui.stock_input->text().clear();

	if( stock_code.toUpper() == "SZZS" )
	{
		stock_code_ = "000001";
	}else
	{
		auto first_code = stock_code.toLocal8Bit().data();
		if( *first_code != '\0' && *first_code != '\2' && *first_code == '\3' && *first_code == '\6' )
			return;
		if( stock_code.length() != 6 || !IsNumber(stock_code.toLocal8Bit().data()) )
			return;
	}
	stock_code_ = stock_code.toLocal8Bit().data();

	ResetStock(stock_code_.c_str());
}
 
void KLineWall::ResetDrawState(DrawAction action)
{ 
    draw_action_ = DrawAction::NO_ACTION;  
    drawing_line_A_ = CST_MAGIC_POINT;
    drawing_line_B_ = CST_MAGIC_POINT;
    drawing_line_C_ = CST_MAGIC_POINT;

    switch(action)
    {
    case DrawAction::NO_ACTION:
        main_win_->UncheckBtnABDownPen();
        main_win_->UncheckBtnABUpPen();
        main_win_->UncheckBtnABCDownPen();
        main_win_->UncheckBtnABUpPen();
        break;
    case DrawAction::DRAWING_FOR_2PDOWN_C:
        main_win_->UncheckBtnABDownPen();
        break;
    case DrawAction::DRAWING_FOR_2PUP_C:
        main_win_->UncheckBtnABUpPen();
        break;
    case DrawAction::DRAWING_FOR_3PDOWN_D:
        main_win_->UncheckBtnABCDownPen();
        break;
    case DrawAction::DRAWING_FOR_3PUP_D:
        main_win_->UncheckBtnABCUpPen();
        break;
    default: assert(false);
    } 
    
}

void KLineWall::ClearForcastData()
{
    //data_2pforcast_downs_.clear();
   auto iter = forcast_man_.Find2pDownForcastVector(stock_code_, k_type_);
   if( iter )
       iter->clear();
   auto iter0 = forcast_man_.Find2pUpForcastVector(stock_code_, k_type_);
   if( iter0 )
       iter0->clear();
   auto iter_3pdown_vector = forcast_man_.Find3pForcastVector(stock_code_, k_type_, true);
   if( iter_3pdown_vector )
       iter_3pdown_vector->clear();
   auto iter_3pup_vector = forcast_man_.Find3pForcastVector(stock_code_, k_type_, false);
   if( iter_3pup_vector )
       iter_3pup_vector->clear();
}

PeriodType KLineWall::ToPeriodType(TypePeriod src)
{
    switch(src)
    {
    case TypePeriod::PERIOD_5M: return PeriodType::PERIOD_5M;
    case TypePeriod::PERIOD_15M: return PeriodType::PERIOD_15M;
    case TypePeriod::PERIOD_30M: return PeriodType::PERIOD_30M;
    case TypePeriod::PERIOD_HOUR: return PeriodType::PERIOD_HOUR;
    case TypePeriod::PERIOD_DAY: return PeriodType::PERIOD_DAY;
    case TypePeriod::PERIOD_WEEK: return PeriodType::PERIOD_WEEK;
    case TypePeriod::PERIOD_MON: return PeriodType::PERIOD_MON;
    assert(false); 
    }
    return PeriodType::PERIOD_DAY;
}

//void KLineWall::SetCursorShape(Qt::CursorShape& cursor_shapre)
//{
//    setCursor(cursor_shapre);
//}

//
//float KLineWall::HisDateItem_GetMinPrice()
//{
//	if( !p_hisdata_container_ ) 
//		return 0.0;
//
//}
//
//float KLineWall::HisDateItem_GetMaxPrice()
//{
//	if( !p_hisdata_container_ ) 
//		return 0.0;
//}
//
//float KLineWall::HisDateItem_GetOpenPrice()
//{
//	if( !p_hisdata_container_ ) 
//		return 0.0;
//}
//
//float KLineWall::HisDateItem_GetClosePrice()
//{
//	if( !p_hisdata_container_ ) 
//		return 0.0;
//}

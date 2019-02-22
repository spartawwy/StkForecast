
#include "kline_wall.h"

#include <cassert>
#include <tuple>
#include <QPainter>
#include <qevent.h> 
#include <qdebug.h>
#include <qdatetime.h>
#include <QtWidgets/QMessageBox>

#include <TLib/core/tsystem_utility_functions.h>

#include "mainwindow.h"
#include "stkfo_common.h"
#include "stk_forecast_app.h"
#include "exchange_calendar.h"

#define  WOKRPLACE_DEFUALT_K_NUM  (4*20 + 100)
#define  DEFAULT_CYCLE_TAG  "日线"

typedef std::tuple<QPointF, std::string> T_TuplePointStr;

using namespace TSystem;

//static const QPoint CST_MAGIC_POINT(-1, -1);
static const int cst_default_year = 2017;
static const double cst_k_mm_enlarge_times = 1.02; 
static const double cst_k_mm_narrow_times = 0.98; 

KLineWall::KLineWall(StkForecastApp *app, QWidget *parent) 
    : QWidget(parent) 
    , app_(app)
    , main_win_((MainWindow*)parent)
    , head_h_percent_(0.03)
   /* , bottom1_h_(30)
    , bottom2_h_(30) */
    , bottom_h_percent_(0.04)
    , empty_right_w_(30)
    , right_w_(30)
    , h_axis_trans_in_paint_k_(0)
    , stock_code_()
    , is_index_(false)
    , stock_data_man_(app->exchange_calendar().get())
    , p_hisdata_container_(nullptr)
    , lowestMinPrice_(99.9)
    , highestMaxPrice_(0.0)
    , show_cross_line_(false)
    , k_num_(WOKRPLACE_DEFUALT_K_NUM)
    , k_type_(TypePeriod::PERIOD_DAY)
    , k_cycle_tag_()
    , k_cycle_year_(0)
    , date_(0)
    , k_date_time_str_() 
	, stock_input_dlg_(this)
    , draw_action_(DrawAction::NO_ACTION)
    , pre_mm_w_(-1)
    , pre_mm_h_(-1)
{
    ui.setupUi(this);
    //bottom_h_ = bottom1_h_ + bottom2_h_;
    ResetDrawState(DrawAction::NO_ACTION); 
 
    // init ui -----------
    QPalette pal = this->palette();
    pal.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    setMouseTracking(true);
    
    zb_windows_.push_back( std::move(std::make_shared<MomentumZhibiaoWin>(this)) );
    zb_windows_.push_back( std::move(std::make_shared<VolZhibiaoWin>(this)) );
}

bool KLineWall::Init()
{
    k_cycle_tag_ = DEFAULT_CYCLE_TAG;
    k_cycle_year_ = cst_default_year;
      
    auto ret = stock_data_man_.Init();
    if( ret )
    {
       return ResetStock("600196", k_type_, false);
    }
    return false;
}

//int KLineWall::height_axis_trans_in_paint_k()
//{ 
//    int target_height = this->height() - BottomHeight(); 
//    for( unsigned int i = 0 ; i < zb_windows_.size(); ++i )
//    {
//        if( zb_windows_[i] )
//            target_height -= zb_windows_[i]->Height();
//    }
//    return target_height;
//}

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
        std::vector<T_TuplePointStr>  fronts_to_draw;
        
        for( unsigned int i = 0; i < p_data_vector->size(); ++i )
        {
            T_Data2pDownForcast &data_2pforcastdown = p_data_vector->at(i);
            auto item_a = GetKLineDataItemByDate(data_2pforcastdown.date_a, data_2pforcastdown.hhmm_a);
            auto item_b = GetKLineDataItemByDate(data_2pforcastdown.date_b, data_2pforcastdown.hhmm_b);
            if( item_a && item_a->kline_posdata.date != 0 && item_b && item_b->kline_posdata.date != 0 )
            {
                if( abs(item_a->kline_posdata.top.y()) > abs(item_b->kline_posdata.top.y()) )  // y is negative
                {  
                    pen.setStyle(Qt::SolidLine); 
                    painter.setPen(pen);  
                    painter.drawLine(item_a->kline_posdata.top, item_b->kline_posdata.bottom);
fronts_to_draw.push_back(std::make_tuple(QPointF(item_a->kline_posdata.top.x()-item_w/2, item_a->kline_posdata.top.y()), "A"));
fronts_to_draw.push_back(std::make_tuple(QPointF(item_b->kline_posdata.bottom.x()-item_w/2, item_b->kline_posdata.bottom.y() + painter.font().pointSizeF()), "B"));
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
fronts_to_draw.push_back(std::make_tuple(QPointF(h_line_left - font_size*6, y1), utility::FormatStr("C1: %.2f", data_2pforcastdown.c1))); 

                    painter.drawLine(QPointF(h_line_left, y2), QPointF(x_b + 10*item_w, y2));
fronts_to_draw.push_back(std::make_tuple(QPointF(h_line_left - font_size*6, y2), utility::FormatStr("C2: %.2f", data_2pforcastdown.c2))); 

                    painter.drawLine(QPointF(h_line_left, y3), QPointF(x_b + 20*item_w, y3));
fronts_to_draw.push_back(std::make_tuple(QPointF(h_line_left - font_size*6, y3), utility::FormatStr("C3: %.2f", data_2pforcastdown.c3)));            
                }else 
                {
                    painter.drawLine(item_a->kline_posdata.bottom, item_b->kline_posdata.top);

                }
            }
        } // for data_2pforcast_downs_

        pen.setColor(Qt::white);
        pen.setStyle(Qt::SolidLine); 
        painter.setPen(pen);
        //char buf[32] = {0};
        std::for_each( std::begin(fronts_to_draw), std::end(fronts_to_draw), [&painter, /*&buf, */this](T_TuplePointStr& in)
        {
            //sprintf_s(buf, sizeof(buf), "%.2f\0", std::get<1>(in));
            painter.drawText(std::get<0>(in), std::get<1>(in).c_str());
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
    pen.setColor(Qt::cyan);
    pen.setWidth(2);
    //painter.setPen(pen); 
    const auto font_size = painter.font().pointSizeF();
    std::vector<T_TuplePointStr> fronts_to_draw;
        
    for( unsigned int i = 0; i < p_data_vector->size(); ++i )
    {
        T_Data2pUpForcast &data_2pforcast = p_data_vector->at(i);
        auto item_a = GetKLineDataItemByDate(data_2pforcast.date_a, data_2pforcast.hhmm_a);
        auto item_b = GetKLineDataItemByDate(data_2pforcast.date_b, data_2pforcast.hhmm_b);
        if( item_a && item_a->kline_posdata.date != 0 && item_b && item_b->kline_posdata.date != 0 )
        {
            if( abs(item_a->kline_posdata.top.y()) < abs(item_b->kline_posdata.top.y()) )  // y is negative
            {  
                pen.setStyle(Qt::SolidLine); 
                painter.setPen(pen);  
                painter.drawLine(item_a->kline_posdata.bottom, item_b->kline_posdata.top);
 fronts_to_draw.push_back(std::make_tuple(QPointF(item_a->kline_posdata.bottom.x()-item_w/2, item_a->kline_posdata.bottom.y()), "A"));
 fronts_to_draw.push_back(std::make_tuple(QPointF(item_b->kline_posdata.top.x()-item_w/2, item_b->kline_posdata.top.y() + painter.font().pointSizeF()), "B"));
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
fronts_to_draw.push_back(std::make_tuple(QPointF(h_line_left - font_size*6, y1), utility::FormatStr("C1: %.2f", data_2pforcast.c1)));

                painter.drawLine(QPointF(h_line_left, y2), QPointF(x_b + 10*item_w, y2));
fronts_to_draw.push_back(std::make_tuple(QPointF(h_line_left - font_size*6, y2), utility::FormatStr("C2: %.2f", data_2pforcast.c2)));

                painter.drawLine(QPointF(h_line_left, y3), QPointF(x_b + 20*item_w, y3));
fronts_to_draw.push_back(std::make_tuple(QPointF(h_line_left - font_size*6, y3),  utility::FormatStr("C3: %.2f", data_2pforcast.c3)));           
            }else 
            {
                painter.drawLine(item_a->kline_posdata.top, item_b->kline_posdata.bottom);
            }
        }
    } // for data_2pforcast_downs_

    pen.setColor(Qt::white);
    pen.setStyle(Qt::SolidLine); 
    painter.setPen(pen); 
    std::for_each( std::begin(fronts_to_draw), std::end(fronts_to_draw), [&painter, this](T_TuplePointStr& in)
    {
        painter.drawText(std::get<0>(in), std::get<1>(in).c_str());
    });
}

void KLineWall::Draw3pDownForcast(QPainter &painter, const int mm_h, double item_w)
{
    _Draw3pForcast(painter, mm_h, item_w, true);

}

void KLineWall::Draw3pUpForcast(QPainter &painter, const int mm_h, double item_w)
{
    _Draw3pForcast(painter, mm_h, item_w, false);
}

void KLineWall::_Draw3pForcast(QPainter &painter, const int mm_h, double item_w, bool is_down_forward)
{
    std::vector<T_Data3pForcast> *p_data_vector = forcast_man_.Find3pForcastVector(stock_code_, k_type_, is_down_forward);
    if( !p_data_vector || p_data_vector->empty() )
        return;

    QPen pen;  
    if( is_down_forward )
        pen.setColor(Qt::cyan);
    else
        pen.setColor(Qt::magenta);
    pen.setWidth(2);
    const auto font_size = painter.font().pointSizeF();
    std::vector<T_TuplePointStr>  fronts_to_draw;
    double lowest_price = MAX_PRICE;
    double highest_price = MIN_PRICE;
    bool has_set_price = false;
    for( unsigned int i = 0; i < p_data_vector->size(); ++i )
    {
        T_Data3pForcast &data_3p_forcast = p_data_vector->at(i);
        auto item_a = GetKLineDataItemByDate(data_3p_forcast.date_a, data_3p_forcast.hhmm_a);
        auto item_b = GetKLineDataItemByDate(data_3p_forcast.date_b, data_3p_forcast.hhmm_b);
        auto item_c = GetKLineDataItemByDate(data_3p_forcast.date_c, data_3p_forcast.hhmm_c);
        QPointF point_a, point_b, point_c;
        double price_a = 0.0, price_b = 0.0, price_c = 0.0;
        if( item_a && item_a->kline_posdata.date != 0 && item_b && item_b->kline_posdata.date != 0 )
        {
            if( is_down_forward )
            {
                if( !(abs(item_a->kline_posdata.top.y()) > abs(item_b->kline_posdata.top.y())) )  // y is negative
                    continue;
                point_a = item_a->kline_posdata.top;
                point_b = item_b->kline_posdata.bottom; 
                point_c = item_c ? item_c->kline_posdata.top : QPointF(0.0, 0.0);
                price_a = item_a->stk_item.high_price;
                price_b = item_b->stk_item.low_price;
                price_c = item_c ? item_c->stk_item.high_price : 0.0;
            }else
            {
                if( !(abs(item_a->kline_posdata.bottom.y()) < abs(item_b->kline_posdata.bottom.y())) )  // y is negative
                    continue;
                point_a = item_a->kline_posdata.bottom;
                point_b = item_b->kline_posdata.top; 
                point_c = item_c ? item_c->kline_posdata.bottom : QPointF(0.0, 0.0);
                price_a = item_a->stk_item.low_price;
                price_b = item_b->stk_item.high_price;
                price_c = item_c ? item_c->stk_item.low_price : 0.0;
            }
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen); 
            painter.drawLine(point_a, point_b);
            fronts_to_draw.push_back(std::make_tuple(QPointF(point_a.x()-item_w/2, point_a.y()), "A"));
            if( item_c )
            {
                painter.drawLine(point_b, point_c);
                fronts_to_draw.push_back(std::make_tuple(QPointF(point_b.x()-item_w/2, point_b.y()), "B"));
                fronts_to_draw.push_back(std::make_tuple(QPointF(point_c.x()-item_w/2, point_c.y()), "C"));
                double y1 = get_pointc_y(data_3p_forcast.d1, mm_h);
                double y2 = get_pointc_y(data_3p_forcast.d2, mm_h);
                double y3 = get_pointc_y(data_3p_forcast.d3, mm_h);
                double x_c = point_c.x();
                pen.setStyle(Qt::DotLine);
                painter.setPen(pen); 
                // vertical line ----
                painter.drawLine(point_c, QPointF(x_c, y3));
                // horzon forcast line -----------
                double h_line_left = point_c.x() - item_w;
                painter.drawLine(QPointF(h_line_left, y1), QPointF(x_c + 5*item_w, y1));
                fronts_to_draw.push_back(std::make_tuple(QPointF(h_line_left - font_size*6, y1), utility::FormatStr("D1: %.2f", data_3p_forcast.d1)));

                painter.drawLine(QPointF(h_line_left, y2), QPointF(x_c + 10*item_w, y2));
                fronts_to_draw.push_back(std::make_tuple(QPointF(h_line_left - font_size*6, y2), utility::FormatStr("D2: %.2f", data_3p_forcast.d2)));

                painter.drawLine(QPointF(h_line_left, y3), QPointF(x_c + 20*item_w, y3));
                fronts_to_draw.push_back(std::make_tuple(QPointF(h_line_left - font_size*6, y3), utility::FormatStr("D3: %.2f", data_3p_forcast.d3)));

                lowest_price = std::min(lowest_price, data_3p_forcast.d1);
                lowest_price = std::min(lowest_price, data_3p_forcast.d3);
                highest_price = std::max(highest_price, data_3p_forcast.d1);
                highest_price = std::max(highest_price, data_3p_forcast.d3);
                has_set_price = true;
            }
        }

    }// for
    pen.setColor(Qt::white);
    pen.setStyle(Qt::SolidLine); 
    painter.setPen(pen); 
    std::for_each( std::begin(fronts_to_draw), std::end(fronts_to_draw), [&painter, this](T_TuplePointStr& in)
    { 
        painter.drawText(std::get<0>(in), std::get<1>(in).c_str());
    });
    /*if( has_set_price )
    {
    lowestMinPrice_ = std::min(lowest_price, lowestMinPrice_);
    highestMaxPrice_ = std::max(highest_price, highestMaxPrice_);
    }*/
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
    //int mm_h = this->height() - head_h_ - bottom_h_;
    const int k_mm_h = Calculate_k_mm_h();
     
    const int mm_w = this->width();
    double item_w = double(mm_w - empty_right_w_ - right_w_) / double(k_num_ + 1) ;
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

        auto pos_y = -1 * k_mm_h * (openPrice - lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_);
        pos_data.height = -1 * k_mm_h *(closePrice - openPrice)/(highestMaxPrice_ - lowestMinPrice_);
        pos_data.columnar_top_left = QPoint(j * item_w + 1, pos_y);
         
        pos_data.top = QPoint(j * item_w + k_bar_w / 2, -1 * k_mm_h * (maxPrice-lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_));
        pos_data.bottom = QPoint(j * item_w + k_bar_w / 2, -1 * k_mm_h * (minPrice-lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_));
    }
#else

    // update position data --------------------------------
    int j = k_num_;
    auto right_end = double(mm_w - empty_right_w_ - right_w_) - k_bar_w;

    for( auto iter = p_hisdata_container_->rbegin();
        iter != p_hisdata_container_->rend() && j > 0; 
        ++iter, --j)
    { 
        T_KlinePosData &pos_data = iter->get()->kline_posdata; 
        pos_data.date = iter->get()->stk_item.date;
        pos_data.hhmm = iter->get()->stk_item.hhmmss;
        auto openPrice = (*iter)->stk_item.open_price;
        auto closePrice = (*iter)->stk_item.close_price;
        auto minPrice = (*iter)->stk_item.low_price;
        auto maxPrice = (*iter)->stk_item.high_price;

        //pos_data.x_left = j * item_w + 1;
        pos_data.x_right = right_end - item_w * (k_num_ - j);
        pos_data.x_left = pos_data.x_right - k_bar_w;
        auto pos_y = -1 * (openPrice - lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_) * k_mm_h;
        pos_data.height = -1 * (closePrice - openPrice)/(highestMaxPrice_ - lowestMinPrice_) * k_mm_h;
        pos_data.columnar_top_left = QPointF(pos_data.x_left, pos_y);

        pos_data.top = QPointF(pos_data.x_left + k_bar_w / 2, -1 * (maxPrice-lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_) * k_mm_h);
        pos_data.bottom = QPointF(pos_data.x_left + k_bar_w / 2, -1 * (minPrice-lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_) * k_mm_h);

        // update drawing line point ---------------------
        if( item_a && item_a->stk_item.date == iter->get()->stk_item.date && item_a->stk_item.hhmmss == iter->get()->stk_item.hhmmss)
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
        }else if( item_b && item_b->stk_item.date == iter->get()->stk_item.date && item_b->stk_item.hhmmss == iter->get()->stk_item.hhmmss )
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
        }else if( item_c && item_c->stk_item.date == iter->get()->stk_item.date && item_c->stk_item.hhmmss == iter->get()->stk_item.hhmmss )
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
        data_3p.hhmm_a = item_a.stk_item.hhmmss; 
        data_3p.date_b = item_b.stk_item.date; 
        data_3p.hhmm_b = item_b.stk_item.hhmmss; 
        this->forcast_man_.Append(k_type_, stock_code_, is_down, data_3p);
    };
    //qDebug() << "paintEvent QCursor::pos  x:" << QCursor::pos().x() << " y: "<< QCursor::pos().y() << "\n"; 
     
	if( stock_input_dlg_.isVisible() )
		stock_input_dlg_.hide();

    if( draw_action_ == DrawAction::NO_ACTION )
        return;

    if( drawing_line_A_ == CST_MAGIC_POINT )
    {
        drawing_line_A_ = QPointF( event->pos().x(), event->pos().y() - h_axis_trans_in_paint_k_);
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
        if( item_b->stk_item.date < item_a->stk_item.date
            || item_b->stk_item.date == item_a->stk_item.date && item_b->stk_item.hhmmss <= item_a->stk_item.hhmmss )
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
                data_2pdown_fcst.hhmm_a = item_a->stk_item.hhmmss;
                data_2pdown_fcst.date_b = item_b->stk_item.date; 
                data_2pdown_fcst.hhmm_b = item_b->stk_item.hhmmss;

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
                data_2pup_fcst.hhmm_a = item_a->stk_item.hhmmss; 
                data_2pup_fcst.date_b = item_b->stk_item.date; 
                data_2pup_fcst.hhmm_b = item_b->stk_item.hhmmss; 

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
        if( !item_c || item_c->stk_item.date < item_b->stk_item.date 
            || (item_c->stk_item.date == item_b->stk_item.date && item_c->stk_item.hhmmss <= item_b->stk_item.hhmmss) )
        {   // todo: show warning msg
            return;
        }

        if( draw_action_ == DrawAction::DRAWING_FOR_3PDOWN_D )
        {
            auto p_data_3pfcst = forcast_man_.Find3pForcast(stock_code_, k_type_, true, *item_a, *item_b);
            assert(p_data_3pfcst);
            // set to point C-----
            p_data_3pfcst->date_c = item_c->stk_item.date;
            p_data_3pfcst->hhmm_c = item_c->stk_item.hhmmss;
            //drawing_line_C_ = item_c->kline_posdata.top;
            auto d1_d2_d3 = ForcastD_ABC_Down(item_a->stk_item.high_price, item_b->stk_item.low_price);
            p_data_3pfcst->d1 = std::get<0>(d1_d2_d3);
            p_data_3pfcst->d2 = std::get<1>(d1_d2_d3);
            p_data_3pfcst->d3 = std::get<2>(d1_d2_d3);

            double ori_price = lowestMinPrice_;
            lowestMinPrice_ = std::min(lowestMinPrice_, p_data_3pfcst->d1);
            lowestMinPrice_ = std::min(lowestMinPrice_, p_data_3pfcst->d3);
            if( lowestMinPrice_ < ori_price )
            {
                lowestMinPrice_ *= 0.98;
                UpdatePosDatas();
            }
            //update();
            return ResetDrawState(draw_action_);  

        }else if( draw_action_ == DrawAction::DRAWING_FOR_3PUP_D )
        {
            auto p_data_3pfcst = forcast_man_.Find3pForcast(stock_code_, k_type_, false, *item_a, *item_b);
            assert(p_data_3pfcst);
            // set to point C-----
            p_data_3pfcst->date_c = item_c->stk_item.date;
            p_data_3pfcst->hhmm_c = item_c->stk_item.hhmmss;
            //drawing_line_C_ = item_b->kline_posdata.bottom;
            auto d1_d2_d3 = ForcastD_ABC_Up(item_a->stk_item.low_price, item_b->stk_item.high_price);
            p_data_3pfcst->d1 = std::get<0>(d1_d2_d3);
            p_data_3pfcst->d2 = std::get<1>(d1_d2_d3);
            p_data_3pfcst->d3 = std::get<2>(d1_d2_d3);
            double ori_price = highestMaxPrice_;
            highestMaxPrice_ = std::max(highestMaxPrice_, p_data_3pfcst->d1);
            highestMaxPrice_ = std::max(highestMaxPrice_, p_data_3pfcst->d3);
            if( highestMaxPrice_ > ori_price )
            {
                highestMaxPrice_ *= 1.02;
                UpdatePosDatas();
            }
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
    /* window lay:********************
      |      | header
      | k mm | body 
      | zhi_biao windows 
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
     
    const bool is_area_shape_change = IsAreaShapeChange(this->width(), this->height());
    if( is_area_shape_change )
    {
        UpdatePosDatas(); 
    }
    QString k_detail_str;
     /*
     ------------>
     |
     |
    \|/
    纵坐标(各行价格值)：((HighestMaxPrice - lowestMinPrice_) * i)/7  + lowestMinPrice_
    横坐标(日期)：分成X等份， 每天的横坐标 
    QFont font;  
    font.setPointSize(Y_SCALE_FONT_SIZE);      font.setFamily("Microsoft YaHei");  
    font.setLetterSpacing(QFont::AbsoluteSpacing,0);      painter.setFont(font);  
    //获取这种字体情况下，文本的长度和高度  
    QFontMetricsF fontMetrics(font);  
    qreal wid = fontMetrics.width(scale_val_str);      qreal height = fontMetrics.height();  
    */ 
    //painter.translate(30, 400);  //坐标 向下 向右 平移 
    
    const int price_scale_num = 8;
    const int scale_part_h = k_mm_h / price_scale_num; 
    int trans_y_totoal = 0;
    //painter.translate(0, HeadHeight());
    //trans_y_totoal += HeadHeight();
    //h_axis_trans_in_paint_k_ = height_axis_trans_in_paint_k();
    h_axis_trans_in_paint_k_ = k_mm_h + HeadHeight(); 
    painter.translate(0, h_axis_trans_in_paint_k_); // translate frame of axes to k mm bottom
    trans_y_totoal += h_axis_trans_in_paint_k_;

    QPointF cur_mous_point_trans(cur_mouse_point_.x(), cur_mouse_point_.y() - h_axis_trans_in_paint_k_);
    do
    {   
        if( draw_action_ == DrawAction::NO_ACTION ) 
            break;
        if( drawing_line_A_ == CST_MAGIC_POINT ) 
            break; 
        QPen pen;
        if( draw_action_ == DrawAction::DRAWING_FOR_2PDOWN_C || draw_action_ == DrawAction::DRAWING_FOR_3PUP_D )
            pen.setColor(Qt::magenta);
        else
            pen.setColor(Qt::cyan);
        pen.setWidth(2);
        painter.setPen(pen); 
        if( drawing_line_B_ == CST_MAGIC_POINT ) // draw line AB
        {   
            painter.drawLine(drawing_line_A_.x(), drawing_line_A_.y(), cur_mous_point_trans.x(), cur_mous_point_trans.y() );
            break;
        }else if( draw_action_ == DrawAction::DRAWING_FOR_3PDOWN_D || draw_action_ == DrawAction::DRAWING_FOR_3PUP_D )
        {   // draw line BC
            painter.drawLine(drawing_line_B_.x(), drawing_line_B_.y(), cur_mous_point_trans.x(), cur_mous_point_trans.y() );
            break;
        }
        //qDebug() << " mouseMoveEvent DRAWING_FOR_2PDOWN_C " << drawing_line_A_.x() << " " << drawing_line_A_.y() << " " << cur_mouse_point_.x() << " " << cur_mouse_point_.y() << "\n";
    }while(0);

    auto ck_h = this->height();

    // draw stock code     -------------
    QPen pen; 
    pen.setColor(Qt::white);
    painter.setPen(pen);
    
    QFont font;  
    font.setPointSize(HeadHeight() * 0.9); 
    painter.setFont(font);
    painter.drawText(mm_w - right_w_ - 70, -1 * (h_axis_trans_in_paint_k_ - font.pointSize()), stock_code_.c_str());
     
    painter.setFont(old_font); 
    
    // right vertical line | -----------
    painter.setPen(border_pen);
    painter.drawLine(mm_w - right_w_, this->height() - h_axis_trans_in_paint_k_, mm_w - right_w_, -1 * this->height());
     
    // vertical' price scale ------------
    
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
        
    //draw k_num_ k line -------------------------------------
	if( p_hisdata_container_ && !p_hisdata_container_->empty() )
	{   
        char temp_str[1024];
        auto iter = p_hisdata_container_->rbegin();
        sprintf_s(temp_str, sizeof(temp_str), "%s 开:%.2f 高:%.2f 低:%.2f 收:%.2f  ", stock_code_.c_str(), (*iter)->stk_item.open_price
            , (*iter)->stk_item.high_price, (*iter)->stk_item.low_price, (*iter)->stk_item.close_price);
        k_detail_str = QString::fromLocal8Bit(temp_str);

        double item_w = double(mm_w - empty_right_w_ - right_w_) / double(k_num_ + 1) ;
        
#ifdef DRAW_FROM_LEFT
    int j = 0;
	for( auto iter = p_hisdata_container_->begin();
		iter != p_hisdata_container_->end() && j < k_num_; 
		++iter, ++j)
    { 
#else
        // for fengxin line and every k line--------------
        int j = k_num_;
        for( auto iter = p_hisdata_container_->rbegin();
            iter != p_hisdata_container_->rend() && j > 0; 
            ++iter, --j)
        { 
#endif
        T_KlinePosData &pos_data = iter->get()->kline_posdata;
         
        // fengxin relate -------------------
        auto rac_type = MaxFractalType((*iter)->type);
        if( rac_type > FractalType::UNKNOW_FRACTAL )
        {
            QPen fractal_pen;
            fractal_pen.setStyle(Qt::SolidLine);
            fractal_pen.setColor(QColor(0,0,255));
            painter.setPen(fractal_pen);

            T_KlinePosData *left_pos_data = nullptr;
            if( rac_type <= FractalType::BTM_AXIS_T_11 ) // bottom fractal 
            {   //painter.drawText(pos_data.bottom.x(), pos_data.bottom.y(), "BTM");
                if( FindTopFractalItem_TowardLeft(*p_hisdata_container_, iter, j, left_pos_data) )
                    painter.drawLine(pos_data.bottom.x(), pos_data.bottom.y(), left_pos_data->top.x(), left_pos_data->top.y());
            }else // top fractal 
            {   //painter.drawText(pos_data.top.x(), pos_data.top.y(), "TOP");
                if( FindBtmFractalItem_TowardLeft(*p_hisdata_container_, iter, j, left_pos_data) )
                    painter.drawLine(pos_data.top.x(), pos_data.top.y(), left_pos_data->bottom.x(), left_pos_data->bottom.y());
            }
        }

        //draw every k line---------------
        QBrush brush(QColor(255,0,0));  
        pen.setStyle(Qt::SolidLine);
        if( (*iter)->stk_item.open_price <= (*iter)->stk_item.close_price )
        { 
            pen.setColor(QColor(255,0,0));
            brush.setColor(QColor(255,0,0));
        }else
        { 
            pen.setColor(QColor(0,255,0)); 
            brush.setColor(QColor(0,255,0));
        }
        painter.setPen(pen);  
        painter.setBrush(brush);   
        painter.drawRect(pos_data.columnar_top_left.x(), pos_data.columnar_top_left.y(), pos_data.x_right - pos_data.x_left, pos_data.height);
        painter.drawLine(pos_data.top.x(), pos_data.top.y(), pos_data.bottom.x(), pos_data.bottom.y());
        
        if( pos_from_global.x() >= pos_data.x_left && pos_from_global.x() <= pos_data.x_right )
        {
            char temp_str[1024];
            if( k_type_ < TypePeriod::PERIOD_DAY )
                sprintf_s(temp_str, "%08d:%04d\0", (*iter)->stk_item.date, (*iter)->stk_item.hhmmss);
            else
                sprintf_s(temp_str, "%08d\0", (*iter)->stk_item.date);
            k_date_time_str_ = temp_str;
            
            sprintf_s(temp_str, sizeof(temp_str), "%s 开:%.2f 高:%.2f 低:%.2f 收:%.2f  \0", stock_code_.c_str(), (*iter)->stk_item.open_price
                                                  , (*iter)->stk_item.high_price, (*iter)->stk_item.low_price, (*iter)->stk_item.close_price);
            k_detail_str = QString::fromLocal8Bit(temp_str);
        }
      }  // for all k line 

        // paint 3pdatas ----------------------
        Draw2pDownForcast(painter, k_mm_h, item_w);
        Draw2pUpForcast(painter, k_mm_h, item_w);  

        Draw3pDownForcast(painter, k_mm_h, item_w);
        Draw3pUpForcast(painter, k_mm_h, item_w);
    } //if( p_hisdata_container_ )
   
    // a scale h place for show extreme low price 
    //painter.translate(0, scale_part_h);
    //trans_y_totoal += scale_part_h;

    //k line view bottom border horizontal line (----------)
    painter.setPen(lit_border_pen); 
    painter.drawLine(0, 0, mm_w, 0); 
 
    // draw zibiao----------------------- 

    const double item_w = double(mm_w - empty_right_w_ - right_w_) / double(k_num_ + 1) ;
    const double k_bar_w = item_w * 3 / 4;
    //const int right_end = double(mm_w - empty_right_w_ - right_w_) - k_bar_w;
    //const double largest_vol = GetCurWinKLargetstVol();
    for( unsigned int i = 0 ; i < zb_windows_.size(); ++i )
    {
        if( zb_windows_[i] )
        {
            const int zb_h = zb_windows_[i]->Height();
            painter.translate(0, zb_h + lit_border_pen.width());
            trans_y_totoal += zb_h + lit_border_pen.width();

            painter.setPen(lit_border_pen);
            painter.drawLine(0, 0, mm_w, 0);// horizental line -----
 
            zb_windows_[i]->DrawWindow(painter, mm_w);
            //------------------
        }
    }
    // end of draw zibiao-----------------------
 
    // bottom calender right tag ------------------------------------
    const int bttome_height = BottomHeight();
    painter.translate(0, bttome_height);
    trans_y_totoal += bttome_height;
    painter.setPen(border_pen);
    painter.drawLine(0, 0, mm_w, 0); // draw bottom line
     
    // draw left top k line detail ------
    painter.translate(0, -1 * trans_y_totoal); // translate axis back
    pen.setColor(Qt::white);
    pen.setStyle(Qt::SolidLine); 
    painter.setPen(pen); 
    font.setPointSize(HeadHeight() * 0.9);
    painter.setFont(font);
    painter.drawText(0, font.pointSize() * 1.2, k_detail_str);
    painter.setFont(old_font);
    // draw cross line --------------------
    if( show_cross_line_ )
    {
        qDebug() << " show_cross_line_ pos y " << (float)pos_from_global.y() << "\n";
        // horizontal line 
        painter.drawLine(0, pos_from_global.y(), mm_w-right_w_, pos_from_global.y());
        // vertical line 
        painter.drawLine(pos_from_global.x(), HeadHeight(), pos_from_global.x(), this->height()); 
        painter.drawText( mm_w-right_w_, pos_from_global.y(), QString("%1").arg(lowestMinPrice_ + price_per_len * (h_axis_trans_in_paint_k_ - pos_from_global.y()) ) );
    }

    // draw date
    painter.drawText(pos_from_global.x(), this->height()-1, k_date_time_str_.c_str());
   
    this->pre_mm_w_ = this->width();
    this->pre_mm_h_ = this->height();
}


void KLineWall::mouseDoubleClickEvent(QMouseEvent*)
{
    //qDebug() << " x:" << e->pos().x() <<" y:" << e->pos().y() << "\n";
    show_cross_line_ = !show_cross_line_;
    //qDebug() << "show_cross_line:" << show_cross_line_ << "\n";
    //setMouseTracking(show_cross_line_);
      
    update();
}

// this->mapFromGlobal(this->cursor().pos()) == event.pos()
void KLineWall::mouseMoveEvent(QMouseEvent *e)
{ 
    auto pos = e->pos();
    qDebug() << " mouseMoveEvent " << "\n";
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
    /*static auto get_container_max_min_price = [this](PeriodType period_type, const std::string& code, int k_num, std::tuple<float, float>& ret)->bool
    {
        T_HisDataItemContainer &container = stock_data_man_.GetHisDataContainer(period_type, code);
        if( container.empty() )
            return false;
        unsigned int start_index = container.size() > k_num ?  container.size() - k_num : 0; 
        unsigned int end_index = container.size() - 1 > 0 ? container.size() - 1 : 0;
        float highest_price = MIN_PRICE;
        float lowest_price = MAX_PRICE;
        for( unsigned int i = start_index; i < end_index; ++ i )
        {
            if( container.at(i)->stk_item.high_price > highest_price )
                highest_price = container.at(i)->stk_item.high_price; 
            if( container.at(i)->stk_item.low_price < lowest_price )
                lowest_price = container.at(i)->stk_item.low_price;
        } 
        double tmp_price = forcast_man_.FindMaxForcastPrice(code, ToTypePeriod(period_type), container.at(start_index)->stk_item.date, container.at(end_index)->stk_item.date);
        if( tmp_price > highest_price )
            highest_price = tmp_price;
        tmp_price = forcast_man_.FindMinForcastPrice(code, ToTypePeriod(period_type), container.at(start_index)->stk_item.date, container.at(end_index)->stk_item.date);
        if( tmp_price < lowest_price )
            lowest_price = tmp_price;

        ret = std::make_tuple(highest_price, lowest_price);
        return true;
    };*/

    static auto update_kwall_min_max_price = [this]()
    {
        std::tuple<float, float> price_tuple;
        if( GetContainerMaxMinPrice(ToPeriodType(k_type_), stock_code_, k_num_, price_tuple) )
        {
            double try_new_high = std::get<0>(price_tuple) * cst_k_mm_enlarge_times;
            if( try_new_high < this->highestMaxPrice_ || try_new_high > this->highestMaxPrice_)
                SetHighestMaxPrice(try_new_high);
            double try_new_low = std::get<1>(price_tuple) * cst_k_mm_narrow_times;
            if( try_new_low < this->lowestMinPrice_ || try_new_low > this->lowestMinPrice_)
                SetLowestMinPrice(try_new_low);
        }
    };
    assert(p_hisdata_container_);
    auto key_val = e->key();
    switch( key_val )
    {
    case Qt::Key_Up:  //zoom out 
        {
            if( p_hisdata_container_->empty() )
                return;
            if( k_num_ > 1 )
            {
                k_num_ --;
                update_kwall_min_max_price();
                UpdatePosDatas();
                update();
            }
            break;
        }
    case Qt::Key_Down: //zoom in 
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
                /*auto p_container = */
                stock_data_man_.AppendStockData(ToPeriodType(k_type_), stock_code_, start_date, oldest_day, is_index_);
            }

            update_kwall_min_max_price();
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

bool KLineWall::ResetStock(const QString& stock, TypePeriod type_period, bool is_index)
{  
    T_HisDataItemContainer & items_in_container = stock_data_man_.GetHisDataContainer(ToPeriodType(k_type_), stock.toLocal8Bit().data());

    if( k_type_ == type_period && stock_code_ ==  stock.toLocal8Bit().data() && !items_in_container.empty() )
        return true;

    stock_code_ = stock.toLocal8Bit().data(); 
    k_type_ = type_period;

    auto cur_date = QDate::currentDate().year() * 10000 + QDate::currentDate().month() * 100 + QDate::currentDate().day();
    // 20 k line per 30 days
    int start_date = 0;
    int span_day = 0;
    switch( type_period )
    {
    case TypePeriod::PERIOD_DAY:
    case TypePeriod::PERIOD_WEEK:
        span_day = -1 * (WOKRPLACE_DEFUALT_K_NUM * 30 / 20);
        break;
    case TypePeriod::PERIOD_HOUR:
        span_day = -1 * (WOKRPLACE_DEFUALT_K_NUM * 30 / (20 * 4));
        break;
    case TypePeriod::PERIOD_30M:
        span_day = -1 * (WOKRPLACE_DEFUALT_K_NUM * 30 / (20 * 4 * 2));
        break;
    case TypePeriod::PERIOD_15M:
        span_day = -1 * (WOKRPLACE_DEFUALT_K_NUM * 30 / (20 * 4 * 2 * 2));
        break;
    case TypePeriod::PERIOD_5M:
        span_day = -1 * (WOKRPLACE_DEFUALT_K_NUM * 30 / (20 * 4 * 2 * 2 * 3));
        break;
    }
    start_date = QDate::currentDate().addDays(span_day).toString("yyyyMMdd").toInt();
     
    p_hisdata_container_ = stock_data_man_.FindStockData(ToPeriodType(k_type_), stock_code_, start_date, cur_date);
    if( !p_hisdata_container_ )
    {
        //p_hisdata_container_ = stock_data_man_.AppendStockData(stock_code_, 20171216, 20180108); 
        p_hisdata_container_ = stock_data_man_.AppendStockData(ToPeriodType(k_type_), stock_code_, start_date, cur_date, is_index);
    }
	
    if( !p_hisdata_container_ )
		return false;

    this->is_index_ = is_index;
    if( !p_hisdata_container_->empty() )
    {
        if( p_hisdata_container_->size() > 180 )
            k_num_ = p_hisdata_container_->size() / 3;
        else if( p_hisdata_container_->size() > 60 )
            k_num_ = p_hisdata_container_->size() / 2;
        else 
            k_num_ = p_hisdata_container_->size();

        std::tuple<float, float> price_tuple;
        if( GetContainerMaxMinPrice(ToPeriodType(k_type_), stock_code_, k_num_, price_tuple) )
        {
            double try_new_high = std::get<0>(price_tuple) * cst_k_mm_enlarge_times;
            if( try_new_high < this->highestMaxPrice_ || try_new_high > this->highestMaxPrice_)
                SetHighestMaxPrice(try_new_high);
            double try_new_low = std::get<1>(price_tuple) * cst_k_mm_narrow_times;
            if( try_new_low < this->lowestMinPrice_ || try_new_low > this->lowestMinPrice_)
                SetLowestMinPrice(try_new_low);
        }
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
    auto p_item = GetKLineDataItemByXpos(x);
    if( p_item ) return is_get_top ? p_item->kline_posdata.top : p_item->kline_posdata.bottom;
    return CST_MAGIC_POINT;
}

T_KlineDataItem * KLineWall::GetKLineDataItemByDate(int date, int hhmm)
{ 
    for( auto iter = p_hisdata_container_->rbegin();
        iter != p_hisdata_container_->rend(); 
        ++iter )
    {   
        if( iter->get()->stk_item.date == date && iter->get()->stk_item.hhmmss == hhmm )
            return iter->get(); 
    }
    return nullptr;
}

T_KlinePosData * KLineWall::GetKLinePosDataByDate(int date, int hhmm)
{
    for( auto iter = p_hisdata_container_->rbegin();
        iter != p_hisdata_container_->rend(); 
        ++iter )
    {   
        if( iter->get()->kline_posdata.date == date && iter->get()->kline_posdata.hhmm == hhmm )
            return std::addressof(iter->get()->kline_posdata);
    }
    return nullptr;
}

bool KLineWall::GetContainerMaxMinPrice(PeriodType period_type, const std::string& code, int k_num, std::tuple<float, float>& ret)
{
    T_HisDataItemContainer &container = stock_data_man_.GetHisDataContainer(period_type, code);
    if( container.empty() )
        return false;
    unsigned int start_index = container.size() > k_num ?  container.size() - k_num : 0; 
    unsigned int end_index = container.size() - 1 > 0 ? container.size() - 1 : 0;
    float highest_price = MIN_PRICE;
    float lowest_price = MAX_PRICE;
    for( unsigned int i = start_index; i < end_index; ++ i )
    {
        if( container.at(i)->stk_item.high_price > highest_price )
            highest_price = container.at(i)->stk_item.high_price; 
        if( container.at(i)->stk_item.low_price < lowest_price )
            lowest_price = container.at(i)->stk_item.low_price;
    } 
    double tmp_price = forcast_man_.FindMaxForcastPrice(code, ToTypePeriod(period_type), container.at(start_index)->stk_item.date, container.at(end_index)->stk_item.date);
    if( tmp_price > highest_price )
        highest_price = tmp_price;
    tmp_price = forcast_man_.FindMinForcastPrice(code, ToTypePeriod(period_type), container.at(start_index)->stk_item.date, container.at(end_index)->stk_item.date);
    if( tmp_price < lowest_price )
        lowest_price = tmp_price;

    ret = std::make_tuple(highest_price, lowest_price);
    return true;
}

void KLineWall::StockInputDlgRet()
{
	QString stock_code = stock_input_dlg_.ui.stock_input->text().trimmed();
    QString stock_code_changed;
	stock_input_dlg_.ui.stock_input->text().clear();
    bool is_index = false;
	if( stock_code.toUpper() == "SZZS" || stock_code.toUpper() == "999999" )
	{
		stock_code_changed = "999999";
        is_index = true;
    }else if( stock_code.toUpper() == "SZCZ" 
        || stock_code.toUpper() == "SZCZ"
        || stock_code.toUpper() == "CYBZ"
        || stock_code.toUpper() == "SZ50"
        || stock_code.toUpper() == "HS300" )
    {
        stock_code_changed = TransIndexPinYin2Code(stock_code.toUpper().toLocal8Bit().data()).c_str();
        is_index = true;
    }else if( stock_code == "999999" /*|| stock_code == "000001"*/ || stock_code == "399001" || stock_code == "399005" 
        || stock_code == "399006" || stock_code == "000016" || stock_code == "000300" )
    {
        stock_code_changed = stock_code.toLocal8Bit().data();
        is_index = true;
	}else
	{
		auto first_code = stock_code.toLocal8Bit().data();
		if( *first_code != '\0' && *first_code != '\2' && *first_code == '\3' && *first_code == '\6' )
			return;
		if( stock_code.length() != 6 || !IsNumber(stock_code.toLocal8Bit().data()) )
			return;
        stock_code_changed = stock_code.toLocal8Bit().data();
	}
	
	ResetStock(stock_code_changed, k_type_, is_index);
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

void KLineWall::RestTypePeriod(TypePeriod  type)
{ 
    if( k_type_ == type )
        return;
    switch( type )
    {
    case TypePeriod::PERIOD_5M:
    case TypePeriod::PERIOD_15M:
    case TypePeriod::PERIOD_30M:
        ResetStock(stock_code_.c_str(), type, is_index_);
        break;
    case TypePeriod::PERIOD_HOUR:
        ResetStock(stock_code_.c_str(), type, is_index_);
        break;
    case TypePeriod::PERIOD_DAY:
        ResetStock(stock_code_.c_str(), type, is_index_);
        break;
    case TypePeriod::PERIOD_WEEK:
        ResetStock(stock_code_.c_str(), type, is_index_);
        break;
    }
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

double KLineWall::GetCurWinKLargetstVol()
{
    double largest_vol = 0.0;
    int k = k_num_;
    for( auto iter = p_hisdata_container_->rbegin();
        iter != p_hisdata_container_->rend() && k > 0; 
        ++iter, --k)
    if( (*iter)->stk_item.vol > largest_vol ) 
        largest_vol = (*iter)->stk_item.vol;
    return largest_vol;
}

bool KLineWall::FindTopFractalItem_TowardLeft(T_HisDataItemContainer &his_data, T_HisDataItemContainer::reverse_iterator iter, int k_index, T_KlinePosData *&left_pos_data)
{
    auto left_tgt_iter = iter + 1;
    int cp_j = k_index - 1;
    for( ; left_tgt_iter != his_data.rend() && cp_j > 0; 
        ++left_tgt_iter, --cp_j)
    {
        auto left_frac_type = MaxFractalType((*left_tgt_iter)->type);
        if( left_frac_type > FractalType::BTM_AXIS_T_11 )
            break;
    }
    if( left_tgt_iter != his_data.rend() && cp_j > 0 )
    {
        left_pos_data = std::addressof(left_tgt_iter->get()->kline_posdata);
        return true;
    }else
        return false;
}


bool KLineWall::FindBtmFractalItem_TowardLeft(T_HisDataItemContainer &his_data, T_HisDataItemContainer::reverse_iterator iter, int k_index, T_KlinePosData *&left_pos_data)
{
    auto left_tgt_iter = iter + 1;
    int cp_j = k_index - 1;
    for( ; left_tgt_iter != his_data.rend() && cp_j > 0; 
        ++left_tgt_iter, --cp_j) // find left btm_axis_iter 
    {
        auto left_frac_type = BtmestFractalType((*left_tgt_iter)->type);
        if( left_frac_type > FractalType::UNKNOW_FRACTAL )
            break;
    }

    if( left_tgt_iter != his_data.rend() && cp_j >= 0 )
    {
        left_pos_data = std::addressof(left_tgt_iter->get()->kline_posdata);
        return true;
    }else
        return false;
}

int KLineWall::Calculate_k_mm_h()
{
    int mm_h = this->height() - HeadHeight() - BottomHeight();
    for( unsigned int i = 0 ; i < zb_windows_.size(); ++i )
    {
        if( zb_windows_[i] )
            mm_h -= zb_windows_[i]->Height();
    }
    return mm_h;
}
 

//void KLineWall::SetCursorShape(Qt::CursorShape& cursor_shapre)
//{
//    setCursor(cursor_shapre);
//}
 
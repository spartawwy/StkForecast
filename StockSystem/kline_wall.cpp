
#include "kline_wall.h"
#include "mainwindow.h"
  
#include <cassert>
#include <QPainter>
#include <qevent.h> 
#include <qdebug.h>
//#include <QTime>
#include <qdatetime.h>
#include <QtWidgets/QMessageBox>

#include "stkfo_common.h"

#define  WOKRPLACE_DEFUALT_K_NUM  (4*20)
#define  DEFAULT_CYCLE_TAG  "日线"

//static const QPoint CST_MAGIC_POINT(-1, -1);
static const int cst_default_year = 2017;

KLineWall::KLineWall(QWidget *parent) 
    : QWidget(parent) 
    , main_win_((MainWindow*)parent)
    , head_h_(30)
    , bottom1_h_(30)
    , bottom2_h_(30) 
    , stock_code_()
    , p_hisdata_container_(nullptr)
    //, kline_pos_data_()
    , lowestMinPrice_(99.9)
    , highestMaxPrice_(0)
    , show_cross_line_(false)
    //, is_repaint_k_(true)
    , k_num_(WOKRPLACE_DEFUALT_K_NUM)
    , k_cycle_tag_()
    , k_cycle_year_(0)
    , date_(0)
    , k_data_str_()
    , cur_stock_code_()
	, stock_input_dlg_(this)
    , draw_action_(DrawAction::NO_ACTION)
    , pre_mm_w_(-1)
    , pre_mm_h_(-1)
{
    ui.setupUi(this);
    bottom_h_ = bottom1_h_ + bottom2_h_;
    ResetDrawState();
    //kline_pos_data_.reserve(WOKRPLACE_DEFUALT_K_NUM*2);

#if 0  // use file
    StockDayInfo std_data;
    std_data.ReadData("600816 20171116 14.30 14.25 14.42 14.09 339000000 235862");
    stk_days_infos_.push_back(std_data);
	this->highestMaxPrice_ = stockAllDaysInfo_.GetHighestMaxPrice();
    this->lowestMinPrice_ = stockAllDaysInfo_.GetLowestMinPrice();
    
#endif
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

void KLineWall::UpdateKLinePosDatas()
{
    assert(p_hisdata_container_);
    assert(k_num_ > 0);

    //QPainter painter(this);
    //painter.translate(0, this->height() - bottom_h_);
    const int mm_h = this->height() - head_h_ - bottom_h_;
    const int mm_w = this->width();
    const int empty_right_w = 30;
    static const int right_w = 30;
    double item_w = double(mm_w - empty_right_w - right_w) / double(k_num_ + 1) ;
    double space_between_k = item_w / 4;
    double k_bar_w = item_w * 3 / 4;
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
    }
#endif
}

void KLineWall::mousePressEvent(QMouseEvent * event )
{
    static auto has_in3pforcasts = [this](int date_a, int date_b)->bool
    {
        if( this->paint_3pdatas_.empty() ) return false;
        unsigned int i = 0;
        for( ; i < this->paint_3pdatas_.size(); ++i )
        {
            if( this->paint_3pdatas_.at(i).date_a == date_a && this->paint_3pdatas_.at(i).date_b == date_b )
                break;
        }
        return i != this->paint_3pdatas_.size();
    };

	if( stock_input_dlg_.isVisible() )
		stock_input_dlg_.hide();
     
    if( draw_action_ == DrawAction::DRAWING_FOR_C )
    {
        if( drawing_line_A_ == CST_MAGIC_POINT )
            drawing_line_A_ = event->pos();
        else
        {
            //drawing_line_A_ = GetPointFromKLineDataItems(drawing_line_A_.x(), true);
            auto item_a = GetKLineDataItemByXpos(drawing_line_A_.x());
            if( !item_a )
            { 
                return ResetDrawState(); 
                 
            }
            auto item_b = GetKLineDataItemByXpos(event->pos().x());
            if( !item_b )
                return;
            if( item_b->stk_item.date <= item_a->stk_item.date )
            {   // todo: show warning msg
                return;
            }
            if( has_in3pforcasts(item_a->stk_item.date, item_b->stk_item.date) )
            { 
                return ResetDrawState();  
            }

            T_PaintData3pForcastDownword data_3pforcast;
            data_3pforcast.date_a = item_a->stk_item.date; 
            data_3pforcast.date_b = item_b->stk_item.date; 
            if( item_a->stk_item.high_price > item_b->stk_item.high_price )
            {
                data_3pforcast.c2 = sqrtf(item_a->stk_item.high_price * item_b->stk_item.low_price);
                data_3pforcast.c1 = sqrtf(item_b->stk_item.low_price * data_3pforcast.c2);
                data_3pforcast.c3 = data_3pforcast.c2 * data_3pforcast.c2 / data_3pforcast.c1;
            }else if( item_a->stk_item.high_price < item_b->stk_item.high_price )
            {
                data_3pforcast.c2 = sqrtf(item_a->stk_item.low_price * item_b->stk_item.high_price);
                data_3pforcast.c1 = sqrtf(item_b->stk_item.high_price * data_3pforcast.c2);
                data_3pforcast.c3 = data_3pforcast.c2 * data_3pforcast.c2 / data_3pforcast.c1;
            }
 
            paint_3pdatas_.push_back(std::move(data_3pforcast));
            return ResetDrawState();  
        }
    }
}

void KLineWall::paintEvent(QPaintEvent*)
{
    static auto IsAreaShapeChange = [this](int w, int h)->bool
    {
        return w != this->pre_mm_w_ || h!= this->pre_mm_h_;
    };
    static auto SetAreaShapeChange = [this](int w, int h)
    {
        this->pre_mm_w_ = w;
        this->pre_mm_h_ = h;
    };
#if 1 
    QPainter painter(this); 
    qDebug() << "paintEvent QCursor::pos  x:" << QCursor::pos().x() << " y: "<< QCursor::pos().y() << "\n";
    auto pos_from_global = mapFromGlobal(QCursor::pos());
    //qDebug() << "paintEvent x:" << pos_from_global.x() << " y: "<< pos_from_global.y() << "\n";
     
    /*const int head_h = 30;
    const int bottom1_h = 30;
    const int bottom2_h = 30;
    const int bottom_h = bottom1_h + bottom2_h;*/

    const int mm_h = this->height() - head_h_ - bottom_h_;
    const int mm_w = this->width();
    const int empty_right_w = 30;
    static const int right_w = 30;
    float price_per_len = (highestMaxPrice_ - lowestMinPrice_) / float(mm_h);
      
    const bool is_area_shape_change = IsAreaShapeChange(this->width(), this->height());
    if( is_area_shape_change )
        UpdateKLinePosDatas();
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
    font.setPointSize(Y_SCALE_FONT_SIZE);  
    font.setFamily("Microsoft YaHei");  
    font.setLetterSpacing(QFont::AbsoluteSpacing,0);  
    painter.setFont(font);  
    //获取这种字体情况下，文本的长度和高度  
    QFontMetricsF fontMetrics(font);  
    qreal wid = fontMetrics.width(scale_val_str);  
    qreal height = fontMetrics.height();  
    */ 
    //painter.translate(30, 400);  //坐标平移
     
    if( draw_action_ == DrawAction::DRAWING_FOR_C && drawing_line_A_ != CST_MAGIC_POINT && drawing_line_B_ == CST_MAGIC_POINT )
    {   
        QPen pen;
        pen.setColor(Qt::magenta);
        pen.setWidth(2);
        painter.setPen(pen); 
        painter.drawLine(drawing_line_A_.x(), drawing_line_A_.y(), cur_mouse_point_.x(), cur_mouse_point_.y() );
        //painter.drawLine(0, 0, pos.x(), pos.y() );
        qDebug() << " mouseMoveEvent DRAWING_FOR_C " << drawing_line_A_.x() << " " << drawing_line_A_.y() << " " << cur_mouse_point_.x() << " " << cur_mouse_point_.y() << "\n";

    }
    painter.translate(0, this->height() - bottom_h_); // translate frame of axes to bottom
    //painter.drawText(0, 0, "(0,0)");
 
    QPen pen; 
    pen.setColor(Qt::white);
    painter.setPen(pen);
    auto old_font = painter.font();
     
    QFont font;  
    font.setPointSize(old_font.pointSize() * 2); 
   
    painter.setFont(font);
    painter.drawText(mm_w - right_w - 70, -1 *(this->height() - 100), cur_stock_code_.c_str());
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
       /* auto item_w = ((mm_w - empty_right_w - right_w)/ k_num_) ;
        int k_bar_w = item_w-2;
*/
        double item_w = double(mm_w - empty_right_w - right_w) / double(k_num_ + 1) ;
        double space_between_k = item_w / 4;
        double k_bar_w = item_w * 3 / 4;

        int t_cycle = 0;
        int index_tcycle_start = 0;
        int index_tcycle_second = 0;
        int index_last_tcycle_in_k_num = 0;
        bool has_first_tcycle_line_drawed = false;

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
        }
        else
        { 
            pen.setColor(QColor(0,255,0)); 
			brush.setColor(QColor(0,255,0));
        }
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
#if 0
		auto pos_y = -1 * mm_h * (openPrice - lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_);
		auto h_1 = -1 * mm_h *(closePrice - openPrice)/(highestMaxPrice_ - lowestMinPrice_);
        painter.drawRect(j * item_w + 1, pos_y, k_bar_w, h_1);  
        
		// draw k line from heigh price to low price----------
        const int point_x = j * item_w + k_bar_w / 2;
        const int point_low_y = -1 * mm_h * (minPrice-lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_);
        painter.drawLine(point_x
                        , -1 * mm_h * (maxPrice-lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_)
                        , point_x
                        , point_low_y);  
#else
        painter.drawRect(pos_data.columnar_top_left.x(), pos_data.columnar_top_left.y(), pos_data.x_right - pos_data.x_left, pos_data.height);
        painter.drawLine(pos_data.top.x(), pos_data.top.y(), pos_data.bottom.x(), pos_data.bottom.y());
#endif
        if( pos_from_global.x() >= pos_data.x_left && pos_from_global.x() <= pos_data.x_right )
            k_data_str_ = std::to_string((*iter)->stk_item.date);

        if( !paint_3pdatas_.empty() )
        {
            auto get_pointc_y = [this, mm_h](double c_val, T_KlineDataItem& item, bool is_bottom)->double
            { 
                /*if( is_bottom ) return c_val * item.kline_posdata.bottom.y() / item.stk_item.low_price;
                else return c_val * item.kline_posdata.top.y() / item.stk_item.high_price;*/ 
                return -1 * (c_val - lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_) * mm_h;
            };

            QPen pen;  
            pen.setStyle(Qt::DotLine);
            pen.setColor(Qt::magenta);
            pen.setWidth(2);
            painter.setPen(pen); 
            for( unsigned int i = 0; i < paint_3pdatas_.size(); ++i )
            {
                auto item_a = GetKLineDataItemByDate(paint_3pdatas_[i].date_a);
                auto item_b = GetKLineDataItemByDate(paint_3pdatas_[i].date_b);
                if( item_a && item_a->kline_posdata.date != 0 && item_b && item_b->kline_posdata.date != 0 )
                {
                    if( abs(item_a->kline_posdata.top.y()) > abs(item_b->kline_posdata.top.y()) )  // y is negative
                    { 
                        const auto font_size = painter.font().pointSizeF();
                        painter.drawLine(item_a->kline_posdata.top, item_b->kline_posdata.bottom);
                        painter.drawText(QPointF(item_a->kline_posdata.top.x()-item_w/2, item_a->kline_posdata.top.y()), QString("%1").arg(item_a->stk_item.high_price));
                        painter.drawText(QPointF(item_b->kline_posdata.bottom.x()-item_w/2, item_b->kline_posdata.bottom.y() + painter.font().pointSizeF()), QString("%1").arg(item_b->stk_item.low_price));

                        //double y1 = (paint_3pdatas_[i].c1 / item_b->stk_item.low_price) * item_b->kline_posdata.bottom.y();
                        double y1 = get_pointc_y(paint_3pdatas_[i].c1, *item_b, true);
                        //double y2 = (paint_3pdatas_[i].c2 / item_b->stk_item.low_price) * item_b->kline_posdata.bottom.y();
                        double y2 = get_pointc_y(paint_3pdatas_[i].c2, *item_b, true);
                        double y3 = get_pointc_y(paint_3pdatas_[i].c3, *item_b, true);
                        double x_b = item_b->kline_posdata.bottom.x();
                        // vertical line ----
                        painter.drawLine(item_b->kline_posdata.bottom, QPointF(x_b, y1));
                        painter.drawLine(QPointF(x_b, y1), QPointF(x_b, y2));
                        painter.drawLine(QPointF(x_b, y2), QPointF(x_b, y3));
                        // horzon forcast line 
                        painter.drawLine(QPointF(item_b->kline_posdata.bottom.x() - item_w, y1), QPointF(item_b->kline_posdata.bottom.x() + 5*item_w, y1));
                        painter.drawText(QPointF(item_b->kline_posdata.bottom.x() - item_w - font_size*6, y1), QString("%1").arg(paint_3pdatas_[i].c1));
                        painter.drawLine(QPointF(item_b->kline_posdata.bottom.x() - item_w, y2), QPointF(item_b->kline_posdata.bottom.x() + 10*item_w, y2));
                        painter.drawText(QPointF(item_b->kline_posdata.bottom.x() - item_w - font_size*6, y2), QString("%1").arg(paint_3pdatas_[i].c2));
                        painter.drawLine(QPointF(item_b->kline_posdata.bottom.x() - item_w, y3), QPointF(item_b->kline_posdata.bottom.x() + 20*item_w, y3));
                        painter.drawText(QPointF(item_b->kline_posdata.bottom.x() - item_w - font_size*6, y3), QString("%1").arg(paint_3pdatas_[i].c3));
                        
                    }else 
                    {
                        painter.drawLine(item_a->kline_posdata.bottom, item_b->kline_posdata.top);
                    }
                }
            }
        }
          
      }  // for all k line 
         
    }
	  
    painter.translate(0, -1 * (this->height() - bottom_h_));
    
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
    //is_repaint_k_ = true;
#endif

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
    qDebug() << " mouseMoveEvent " << "\n";
    //auto pos_mapped = mapToGlobal(pos);
          
    //auto pos_mapped = mapFromGlobal(e->pos());
    //auto pos_mapped = mapToParent(e->pos());
    //is_repaint_k_ = false;

    if( draw_action_ == DrawAction::DRAWING_FOR_C )
    {
        qDebug() << " mouseMoveEvent DRAWING_FOR_C " << "\n";
        cur_mouse_point_ = e->pos();
    }
    update();
}

void KLineWall::keyPressEvent(QKeyEvent *e)
{
    //qDebug() << "key " << e->key() << "\n";
    auto key_val = e->key();
    switch( key_val )
    {
    case Qt::Key_Up:
        {
            if( k_num_ > 1 )
            {
                k_num_ --;
                UpdateKLinePosDatas();
                update();
            }
            break;
        }
    case Qt::Key_Down:
        { 
            k_num_ ++;
            UpdateKLinePosDatas();
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
    cur_stock_code_ = stock.toLocal8Bit().data();
    //auto cur_time = QTime::currentTime();
    auto cur_date = QDate::currentDate().year() * 10000 + QDate::currentDate().month() * 100 + QDate::currentDate().day();
    auto start_date = QDate::currentDate().addDays(-1 * (WOKRPLACE_DEFUALT_K_NUM / 20 * 30) ).toString("yyyyMMdd").toInt();
	//p_hisdata_container_ = stockAllDaysInfo_.LoadStockData(cur_stock_code_, 20171216, 20180108);
	//p_hisdata_container_ = stockAllDaysInfo_.LoadStockData(cur_stock_code_, 20160806, 20160902);
	//p_hisdata_container_ = stockAllDaysInfo_.LoadStockData(cur_stock_code_, 20020420, 20020520);
	p_hisdata_container_ = stockAllDaysInfo_.LoadStockData(cur_stock_code_, start_date, cur_date);
	if( !p_hisdata_container_ )
		return false;

	auto iter = stockAllDaysInfo_.stock_his_items_.find(cur_stock_code_);

	this->highestMaxPrice_ = stockAllDaysInfo_.GetHisDataHighestMaxPrice(cur_stock_code_);
	this->lowestMinPrice_ = stockAllDaysInfo_.GetHisDataLowestMinPrice(cur_stock_code_);

    UpdateKLinePosDatas();
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
 
void KLineWall::ResetDrawState()
{ 
    draw_action_ = DrawAction::NO_ACTION;  
    drawing_line_A_ = CST_MAGIC_POINT;
    drawing_line_B_ = CST_MAGIC_POINT;
    main_win_->UncheckBtnABPen();
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


#include "kline_wall.h"

#include "dayklinedialog.h"

#include <QPainter>
#include <qevent.h> 
#include <qdebug.h>
//#include <QTime>
#include <qdatetime.h>
#include <QtWidgets/QMessageBox>

#include "stkfo_common.h"

static const QPoint CST_MAGIC_POINT(-1, -1);

KLineWall::KLineWall(QWidget *parent) 
    : QWidget(parent) 
	, p_hisdata_container_(nullptr)
    , stock_code_()
    , lowestMinPrice(99.9)
    , highestMaxPrice(0)
    , show_cross_line_(false)
    , is_repaint_k_(true)
    , k_num_(4*20)
    , k_cycle_tag_()
    , k_cycle_year_(0)
    , date_(0)
    , k_data_str_()
    , cur_stock_code_()
	, stock_input_dlg_(this)
    , draw_action_(DrawAction::NO_ACTION)
{
    ui.setupUi(this);

#if 0  // use file
    StockDayInfo std_data;
    std_data.ReadData("600816 20171116 14.30 14.25 14.42 14.09 339000000 235862");
    stk_days_infos_.push_back(std_data);
	this->highestMaxPrice = stockAllDaysInfo_.GetHighestMaxPrice();
    this->lowestMinPrice = stockAllDaysInfo_.GetLowestMinPrice();
    
#endif

    QPalette pal = this->palette();
    pal.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    k_cycle_tag_ = "日线";
    k_cycle_year_ = 2017;

	
    stockAllDaysInfo_.Init();
	ResetStock("600196");
    
}

void KLineWall::mousePressEvent ( QMouseEvent * event )
{
	if( stock_input_dlg_.isVisible() )
		stock_input_dlg_.hide();
     
    if( draw_action_ == DrawAction::DRAWING_FOR_C )
        drawing_line_A_ = event->pos();
    
}

void KLineWall::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    qDebug() << "paintEvent QCursor::pos  x:" << QCursor::pos().x() << " y: "<< QCursor::pos().y() << "\n";
    auto pos_from_global = mapFromGlobal(QCursor::pos());
    //qDebug() << "paintEvent x:" << pos_from_global.x() << " y: "<< pos_from_global.y() << "\n";
    if( draw_action_ == DrawAction::DRAWING_FOR_C && drawing_line_A_ != CST_MAGIC_POINT)
    {   
        QPen pen; 
        QBrush brush(Qt::red);
        pen.setColor(Qt::red);
        painter.setPen(pen);
        painter.setBrush(brush);

        painter.drawLine(drawing_line_A_.x(), drawing_line_A_.y(), cur_mouse_point_.x(), cur_mouse_point_.y() );
        //painter.drawLine(0, 0, pos.x(), pos.y() );
        qDebug() << " mouseMoveEvent DRAWING_FOR_C " << drawing_line_A_.x() << " " << drawing_line_A_.y() << " " << cur_mouse_point_.x() << " " << cur_mouse_point_.y() << "\n";
    }
    const int head_h = 30;
    const int bottom1_h = 30;
    const int bottom2_h = 30;
    const int bottom_h = bottom1_h + bottom2_h;
    const int mm_h = this->height() - head_h - bottom_h;
    const int mm_w = this->width();
    const int empty_right_w = 30;
    static const int right_w = 30;
    float price_per_len = (highestMaxPrice - lowestMinPrice) / float(mm_h);
      
     /*
     ------------>
     |
     |
    \|/
    纵坐标(各行价格值)：((HighestMaxPrice - lowestMinPrice) * i)/7  + lowestMinPrice
    横坐标(日期)：分成60等份，每天15像素，每天的横坐标
    j*15+1  j*15*8
    每天的纵坐标:开盘价:350*(开盘价-lowestMinPrice)/(HighestMaxPrice - lowestMinPrice)
    收盘价:350*(收盘价-lowestMinPrice)/(HighestMaxPrice - lowestMinPrice)
    最高价:350*(最高价-lowestMinPrice)/(HighestMaxPrice - lowestMinPrice)
    最低价:350*(最盘价-lowestMinPrice)/(HighestMaxPrice - lowestMinPrice)

    矩形宽度：13像素
    矩形高度：350*(开盘价-收盘价)/(HighestMaxPrice - lowestMinPrice)像素
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
     
    QPen pen; 
    pen.setColor(Qt::white);
    painter.setPen(pen);
    auto old_font = painter.font();

    painter.translate(0, this->height() - bottom_h);
    //painter.drawText(0, 0, "(0,0)");
    QFont font;  
    font.setPointSize(old_font.pointSize() * 2); 
    painter.setFont(font);
    painter.drawText(mm_w - right_w - 70, -1 *(this->height() - 100), cur_stock_code_.c_str());
    painter.setFont(old_font);
    //绘制背景各个虚线
    pen.setColor(Qt::red);
    painter.setPen(pen);

    // right line
    painter.drawLine(mm_w - right_w, bottom_h, mm_w - right_w, -1 * this->height());

    // bottom calender  
    painter.drawLine(0, bottom1_h, mm_w, bottom1_h);
    painter.drawLine(0, bottom_h-1, mm_w, bottom_h-1);

    painter.drawText(mm_w - right_w, bottom_h-1, QString::fromLocal8Bit(k_cycle_tag_.c_str()));
    painter.drawText(0, bottom_h-1, QString("%1").arg(k_cycle_year_));

    pen.setStyle(Qt::DotLine);   //设置画笔风格 点线
    painter.setPen(pen);

    //横向虚线
    const int num = 8;
    const int part_h = mm_h / num;
    for(int i = 0; i < num; i++)
    {
        int pos_y = (-1) * part_h*i;
        
        painter.drawText(mm_w - right_w, pos_y, QString("%1").arg(lowestMinPrice + (price_per_len * part_h * i) ));
        painter.drawLine(0, pos_y, mm_w-right_w, pos_y);
    }
      
    //draw all k line -------------------------------------
     
    std::list<StockDayInfo>::iterator iter0;
    float openPrice; 
    float closePrice; 
    float maxPrice; 
    float minPrice; 
    float marketMoney; //成交额
    int j = 0;

    //cout<<stockAllDaysInfo_.GetStockAllDaysInfoList().size()<<endl;
    //？？？只需要最后60个数据，若少于等于60个，则正常绘图
	if( p_hisdata_container_ )
	{  
        auto item_w = ((mm_w - empty_right_w - right_w)/ k_num_) ;
        int k_bar_w = item_w-2;

        int t_cycle = 0;
        int index_tcycle_start = 0;
        int index_tcycle_second = 0;
        int index_last_tcycle_in_k_num = 0;
        bool has_first_tcycle_line_drawed = false;

	for( auto iter = p_hisdata_container_->begin();
		iter != p_hisdata_container_->end() && j < k_num_; 
		++iter, ++j)
    { 
        bool is_lowest_k = false;
        //draw every k line---------------
		minPrice = (*iter)->stk_item.low_price;
		maxPrice = (*iter)->stk_item.high_price;
		openPrice = (*iter)->stk_item.open_price;
		closePrice = (*iter)->stk_item.close_price;

        //qDebug() << openPrice<< "\t"  << closePrice << minPrice << "\t" << maxPrice<< "\t" << "\n";
         

		 QBrush brush(QColor(255,0,0));  
		 pen.setStyle(Qt::SolidLine);
        //绘图每天的股票到K线图上
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
            if( iter->get()->stk_item.low_price < this->lowestMinPrice + 0.001 )
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
		//const auto base_val = mm_h / 2; //350 
		painter.setPen(pen);  
        painter.setBrush(brush);  
        // draw k rect ----------------    
		auto pos_y = -1 * mm_h * (openPrice - lowestMinPrice)/(highestMaxPrice - lowestMinPrice);
		auto h_1 = -1 * mm_h *(closePrice - openPrice)/(highestMaxPrice - lowestMinPrice);
        painter.drawRect(j * item_w + 1,  pos_y, k_bar_w, h_1);  

		// draw k line from heigh price to low price----------
        const int point_x = j * item_w + k_bar_w / 2;
        const int point_low_y = -1 * mm_h * (minPrice-lowestMinPrice)/(highestMaxPrice - lowestMinPrice);
        painter.drawLine(point_x
                        , -1 * mm_h * (maxPrice-lowestMinPrice)/(highestMaxPrice - lowestMinPrice)
                        , point_x
                        , point_low_y);  

        if( pos_from_global.x() >= j * item_w + 1 && pos_from_global.x() <= j * item_w + 1 + k_bar_w )
            k_data_str_ = std::to_string((*iter)->stk_item.date);

        if( !has_first_tcycle_line_drawed )
        { 
            if( is_lowest_k && j > 0 )
            {
                int k = j - 1; 
                for(  auto it = iter-1; k >= 0; --it, --k )
                {
                    if( ( it->get()->type & TOP_AXIS_T_3 ) == TOP_AXIS_T_3 )
                    {
                        auto pre_iter = iter - 1;
                        const int pre_iter_point_x = (j - 1) * item_w + k_bar_w / 2;
                        const int pre_iter_point_y = -1 * mm_h * ((*pre_iter)->stk_item.low_price-lowestMinPrice)/(highestMaxPrice - lowestMinPrice);
                        const int pre_top_ax_point_x = k * item_w + k_bar_w / 2;
                        const int pre_top_ax_point_low_y = -1 * mm_h * ((*it)->stk_item.high_price-lowestMinPrice)/(highestMaxPrice - lowestMinPrice);
                        painter.drawLine(pre_top_ax_point_x, pre_top_ax_point_low_y
                                        , pre_iter_point_x, pre_iter_point_y);
                        // tcycle vertical line ---------
                        pen.setColor(Qt::yellow); 
                        brush.setColor(Qt::yellow); 
		                painter.setPen(pen);  
                        painter.setBrush(brush);  
                        painter.drawLine(pre_iter_point_x, 0
                                         ,pre_iter_point_x, -1 * mm_h * (highestMaxPrice - lowestMinPrice));
                        index_tcycle_start = j - 1; 
                        t_cycle = j - k; 
                        index_tcycle_second = index_tcycle_start + t_cycle - 1;
                        has_first_tcycle_line_drawed = true;
                        break;
                    }
                }
            }
        }else
        {
            if( j == index_tcycle_second || (j > index_tcycle_second && (j - index_tcycle_second) % t_cycle == 0) ) 
            {  
                // tcycle vertical line ---------
                pen.setColor(Qt::yellow); 
                brush.setColor(Qt::yellow); 
		        painter.setPen(pen);  
                painter.setBrush(brush);   
                painter.drawLine(point_x, 0, point_x, -1 * mm_h * (highestMaxPrice - lowestMinPrice));
                index_last_tcycle_in_k_num = j;

                auto t_val = ((j - index_tcycle_second) / t_cycle + 1) * t_cycle;
                painter.drawText(point_x, -1 * mm_h, QString("%1").arg(t_val));
            }
        }
        
      }  // for all k line 

      if( t_cycle > 0 )
      { 
            pen.setColor(Qt::yellow); 
            QBrush brush(Qt::yellow);
		    painter.setPen(pen);  
            painter.setBrush(brush);   
            const int point_x_lastcycle_l = (index_last_tcycle_in_k_num + t_cycle) * item_w + k_bar_w / 2;
            painter.drawLine(point_x_lastcycle_l, 0, point_x_lastcycle_l, -1 * mm_h * (highestMaxPrice - lowestMinPrice));

            auto t_val = ((index_last_tcycle_in_k_num - index_tcycle_second) / t_cycle  + 2) * t_cycle;
            painter.drawText(point_x_lastcycle_l, -1 * mm_h, QString("%1").arg(t_val));
      }
    }
	  
    painter.translate(0, -1 * (this->height() - bottom_h));
    
    pen.setColor(Qt::white);
    pen.setStyle(Qt::SolidLine); 
    painter.setPen(pen);
    if( show_cross_line_ )
    {
        //qDebug() << "pos y " << (float)pos_from_global.y() << "\n";
        // horizontal line 
        painter.drawLine(0, pos_from_global.y(), mm_w-right_w, pos_from_global.y());
        // vertical line 
        painter.drawLine(pos_from_global.x(), head_h, pos_from_global.x(), this->height()); 
        painter.drawText( mm_w-right_w, pos_from_global.y(), QString("%1").arg(lowestMinPrice + price_per_len * (this->height() - bottom_h - pos_from_global.y()) ) );
    }
    painter.drawText(pos_from_global.x(), this->height()-1, k_data_str_.c_str());
    //is_repaint_k_ = true;
}

void KLineWall::mouseDoubleClickEvent(QMouseEvent *e)
{
    //qDebug() << " x:" << e->pos().x() <<" y:" << e->pos().y() << "\n";
    show_cross_line_ = !show_cross_line_;
    setMouseTracking(show_cross_line_);

    /*if( show_cross_line_ )
        setCursor(Qt::CrossCursor);*/
     
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
        update();
    }
    
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
                update();
            }
            break;
        }
    case Qt::Key_Down:
        { 
            k_num_ ++;
            update();
            break;
        }

    case Qt::Key_0: 
    case Qt::Key_1: 
    case Qt::Key_2: 
    case Qt::Key_3: 
    case Qt::Key_4: 
    case Qt::Key_5: 
    case Qt::Key_6: 
    case Qt::Key_7: 
    case Qt::Key_8: 
    case Qt::Key_9: 
    case Qt::Key_A:
    case Qt::Key_B:
    case Qt::Key_C:
    case Qt::Key_D:
    case Qt::Key_E:
    case Qt::Key_F:
    case Qt::Key_G:
    case Qt::Key_H:
    case Qt::Key_I:
    case Qt::Key_J:
    case Qt::Key_K:
    case Qt::Key_L:
    case Qt::Key_M:
    case Qt::Key_N:
    case Qt::Key_O:
    case Qt::Key_P:
    case Qt::Key_Q:
    case Qt::Key_R:
    case Qt::Key_S:
    case Qt::Key_T:
    case Qt::Key_U:
    case Qt::Key_V:
    case Qt::Key_W:
    case Qt::Key_X:
    case Qt::Key_Y:
    case Qt::Key_Z:
		{
			qDebug() << __FUNCDNAME__ << "\n"; 
            stock_input_dlg_.ui.stock_input->clear();
            char tmpbuf[8] = {0};
            if( key_val >= Qt::Key_0 && key_val <= Qt::Key_9 )
                sprintf(tmpbuf, "%c", (char)key_val);
            else
                sprintf(tmpbuf, "%c", char(key_val+32));
            stock_input_dlg_.ui.stock_input->setText(tmpbuf);
			stock_input_dlg_.show();
		}
		break;
    default:
        break;
    }

    /*if( e->key() == Qt::Key_Down && k_num_ > 0 )
    {
        k_num_ --;
        update();
    }*/
    e->ignore();
}

void KLineWall::enterEvent(QEvent *e)
{
    qDebug() << __FUNCTION__ << "\n";
}

void KLineWall::leaveEvent(QEvent *e)
{
    qDebug() << __FUNCTION__ << "\n";
}

void KLineWall::ResetStock(const QString& stock)
{
    cur_stock_code_ = stock.toLocal8Bit().data();
    //auto cur_time = QTime::currentTime();
    auto cur_date = QDate::currentDate().year() * 10000 + QDate::currentDate().month() * 100 + QDate::currentDate().day();
    auto start_date = QDate::currentDate().addDays(-4*30).toString("yyyyMMdd").toInt();
	//p_hisdata_container_ = stockAllDaysInfo_.LoadStockData(cur_stock_code_, 20171216, 20180108);
	//p_hisdata_container_ = stockAllDaysInfo_.LoadStockData(cur_stock_code_, 20160806, 20160902);
	//p_hisdata_container_ = stockAllDaysInfo_.LoadStockData(cur_stock_code_, 20020420, 20020520);
	p_hisdata_container_ = stockAllDaysInfo_.LoadStockData(cur_stock_code_, start_date, cur_date);
	if( !p_hisdata_container_ )
		return;

	auto iter = stockAllDaysInfo_.stock_his_items_.find(cur_stock_code_);

	this->highestMaxPrice = stockAllDaysInfo_.GetHisDataHighestMaxPrice(cur_stock_code_);
	this->lowestMinPrice = stockAllDaysInfo_.GetHisDataLowestMinPrice(cur_stock_code_);
 	 
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
 
void KLineWall::ResetDrawingPoint()
{ 
    drawing_line_A_ = CST_MAGIC_POINT;
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

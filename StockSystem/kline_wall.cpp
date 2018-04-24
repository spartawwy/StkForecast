
#include "kline_wall.h"

#include "dayklinedialog.h"

#include <QPainter>
#include <qevent.h>
#include <QtWidgets/QMessageBox>
#include <qdebug.h>
 
#include "stkfo_common.h"

KLineWall::KLineWall() 
    : QWidget(nullptr)
    , show_cross_line_(false)
    , is_repaint_k_(true)
    , k_num_(8)
	, p_hisdata_list_(nullptr)
	, stock_input_dlg_(this)
{
    ui.setupUi(this);

#if 0  // use file
    StockDayInfo std_data;
    std_data.ReadData("600816 20171116 14.30 14.25 14.42 14.09 339000000 235862");
    stk_days_infos_.push_back(std_data);
	this->highestMaxPrice = stockAllDaysInfo_.GetHighestMaxPrice();
    this->lowestMinPrice = stockAllDaysInfo_.GetLowestMinPrice();
#else
    
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
}

void KLineWall::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    qDebug() << "paintEvent QCursor::pos  x:" << QCursor::pos().x() << " y: "<< QCursor::pos().y() << "\n";
    auto pos_from_global = mapFromGlobal(QCursor::pos());
    qDebug() << "paintEvent x:" << pos_from_global.x() << " y: "<< pos_from_global.y() << "\n";
    
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
     ------------------------>
     |
     |
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
    */

    //坐标平移
    //painter.translate(30, 400);
     
    QPen pen; 
    pen.setColor(Qt::white);
  
    painter.translate(0, this->height() - bottom_h);
    painter.drawText(0, 0, "(0,0)");
 
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
    
    //    //纵向虚线 其实需要根据当前日期来确定具体定在什么位置 每个格子代表一个月
    //    for(int i = 0; i < 4; i++)
    //    {
    //        painter.drawLine(150*i,0,150*i,-350);
    //    }

    //画日K线图 -------------------------------------

    
    std::list<StockDayInfo>::iterator iter0;
    float openPrice;//开盘价
    float closePrice;//收盘价
    float maxPrice;//最高价
    float minPrice;//最低价
    float marketMoney;//成交额
    int j = 0;

    //cout<<stockAllDaysInfo_.GetStockAllDaysInfoList().size()<<endl;
    //？？？只需要最后60个数据，若少于等于60个，则正常绘图
	if( p_hisdata_list_ )
	{ 
		for( auto iter = p_hisdata_list_->begin();
			iter != p_hisdata_list_->end() && j < k_num_; 
			iter++, j++)
    {
        //绘图每天的股票消息
        //((StockDayInfo)(*iter)).Display();
        //读取每天的股票数据，获得一支股票的最高价 最低价 开盘价 收盘价
#if 0 
		minPrice = ((StockDayInfo)(*iter)).GetMinPrice();
        maxPrice = ((StockDayInfo)(*iter)).GetMaxPrice();
        openPrice = ((StockDayInfo)(*iter)).GetOpenPrice();
        closePrice = ((StockDayInfo)(*iter)).GetClosePrice();
#endif
		minPrice = (*iter)->stk_item.low_price;
		maxPrice = (*iter)->stk_item.high_price;
		openPrice = (*iter)->stk_item.open_price;
		closePrice = (*iter)->stk_item.close_price;
        qDebug()<<openPrice<<"\t" << closePrice <<minPrice<<"\t"<<maxPrice<<"\t"<< "\n";

         auto item_w = ((mm_w - empty_right_w - right_w)/ k_num_) ;
         int k_bar_w = item_w-2;

		 QBrush brush(QColor(255,0,0)); //画刷
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
        if( (*iter)->type & BTM_AXIS_T_3 == BTM_AXIS_T_3 
            || (*iter)->type & BTM_AXIS_T_5 == BTM_AXIS_T_5 
            || (*iter)->type & BTM_AXIS_T_7 == BTM_AXIS_T_7
            || (*iter)->type & BTM_AXIS_T_9 == BTM_AXIS_T_9
            || (*iter)->type & BTM_AXIS_T_11 == BTM_AXIS_T_11)
        {
            pen.setColor(QColor(0,0,255)); 
            brush.setColor(QColor(0,0,255));
        }
		//const auto base_val = mm_h / 2; //350 
		painter.setPen(pen); //添加画笔
        painter.setBrush(brush); //添加画刷
            
		auto pos_y = -1 * mm_h * (openPrice - lowestMinPrice)/(highestMaxPrice - lowestMinPrice);
		auto h_1 = -1 * mm_h *(closePrice - openPrice)/(highestMaxPrice - lowestMinPrice);
        painter.drawRect(j * item_w + 1,  pos_y, k_bar_w, h_1); //绘制矩形
		
        painter.drawLine(j * item_w + k_bar_w/2
            , -1 * mm_h * (maxPrice-lowestMinPrice)/(highestMaxPrice - lowestMinPrice)
            , j * item_w + k_bar_w/2
            , -1 * mm_h * (minPrice-lowestMinPrice)/(highestMaxPrice - lowestMinPrice));   //绘制直线

        if( pos_from_global.x() >= j * item_w + 1 && pos_from_global.x() <= j * item_w + 1 + k_bar_w )
            k_data_str_ = std::to_string((*iter)->stk_item.date);
      }  // for
    }
	  
    painter.translate(0, -1 * (this->height() - bottom_h));
    
    pen.setColor(Qt::white);
    pen.setStyle(Qt::SolidLine); 
    painter.setPen(pen);
    if( show_cross_line_ )
    {
        qDebug() << "pos y " << (float)pos_from_global.y() << "\n";
        painter.drawLine(0, pos_from_global.y(), mm_w-right_w, pos_from_global.y());
        painter.drawLine(pos_from_global.x(), head_h, pos_from_global.x(), this->height()/* - head_h*/); 
        painter.drawText( mm_w-right_w, pos_from_global.y(), QString("%1").arg(lowestMinPrice + price_per_len * (this->height() - bottom_h - pos_from_global.y()) ) );
      
      //painter.drawText( pos_from_global.x() - 30, -1 * (mm_h - pos_from_global.y()), QString("%1").arg(lowestMinPrice + (float)(mm_h - pos_from_global.y()) * price_per_len ));
    }
    painter.drawText(pos_from_global.x(), this->height()-1, k_data_str_.c_str());
    is_repaint_k_ = true;
}

void KLineWall::mouseDoubleClickEvent(QMouseEvent *e)
{
    qDebug() << " x:" << e->pos().x() <<" y:" << e->pos().y() << "\n";
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
    //auto pos_mapped = mapToGlobal(pos);
          
    //auto pos_mapped = mapFromGlobal(e->pos());
    //auto pos_mapped = mapToParent(e->pos());
    //is_repaint_k_ = false;
    update();
    
}

void KLineWall::keyPressEvent(QKeyEvent *e)
{
    qDebug() << "key " << e->key() << "\n";
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
}

void KLineWall::ResetStock(const QString& stock)
{
	cur_stock_code_ = stock.toLocal8Bit().data();

	//p_hisdata_list_ = stockAllDaysInfo_.LoadStockData(cur_stock_code_, 20171216, 20180108);
	//p_hisdata_list_ = stockAllDaysInfo_.LoadStockData(cur_stock_code_, 20160806, 20160902);
	//p_hisdata_list_ = stockAllDaysInfo_.LoadStockData(cur_stock_code_, 20020420, 20020520);
	p_hisdata_list_ = stockAllDaysInfo_.LoadStockData(cur_stock_code_, 20180108, 20180328);
	if( !p_hisdata_list_ )
		return;

	auto iter = stockAllDaysInfo_.stock_his_items_.find(cur_stock_code_);
	/*if( iter == stockAllDaysInfo_.stock_his_items_.end() )
		return;*/

	//p_hisdata_list_ = std::addressof(iter->second);

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

//
//float KLineWall::HisDateItem_GetMinPrice()
//{
//	if( !p_hisdata_list_ ) 
//		return 0.0;
//
//}
//
//float KLineWall::HisDateItem_GetMaxPrice()
//{
//	if( !p_hisdata_list_ ) 
//		return 0.0;
//}
//
//float KLineWall::HisDateItem_GetOpenPrice()
//{
//	if( !p_hisdata_list_ ) 
//		return 0.0;
//}
//
//float KLineWall::HisDateItem_GetClosePrice()
//{
//	if( !p_hisdata_list_ ) 
//		return 0.0;
//}

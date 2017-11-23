
#include "kline_wall.h"

#include "dayklinedialog.h"

#include <QPainter>
#include <qevent.h>

#include <qdebug.h>

KLineWall::KLineWall() 
    : QWidget(nullptr)
    , show_cross_line_(false)
{
    ui.setupUi(this);

#if 0
    StockDayInfo std_data;
    std_data.ReadData("600816 20171116 14.30 14.25 14.42 14.09 339000000 235862");
    stk_days_infos_.push_back(std_data);
#else
    this->highestMaxPrice = stockAllDaysInfo.GetHighestMaxPrice();
    this->lowestMinPrice = stockAllDaysInfo.GetLowestMinPrice();
#endif

    QPalette pal = this->palette();
    pal.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

void KLineWall::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    qDebug() << "paintEvent x:" << QCursor::pos().x() << " y: "<< QCursor::pos().y() << "\n";

    const int mm_h = this->height();
    const int mm_w = this->width();
    /*
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
    

    QPen pen; //画笔
    pen.setColor(Qt::white);
    pen.setStyle(Qt::SolidLine); 
    painter.setPen(pen);
#if 1 
    if( show_cross_line_ )
    {
        painter.drawLine(0, QCursor::pos().y(), mm_w, QCursor::pos().y());
        painter.drawLine(QCursor::pos().x(), 0, QCursor::pos().x(), mm_h);
    }
#else
    painter.drawLine(0, 50, mm_w, 50);
        //painter.drawLine(QCursor::pos().x(), 0, QCursor::pos().x(), mm_h);
#endif

    painter.translate(30, mm_h);

    painter.drawText(-30, 0, "(0,0)");

    //绘制背景各个虚线
    pen.setStyle(Qt::DotLine);   //设置画笔风格 点线
    
    painter.setPen(pen);
    //横向虚线
    const int num = 8;
    const int part_h = mm_h / num;
    for(int i = 0; i < num; i++)
    //for(int i = 0; i < 1; i++)
    {
        int pos_y = (-1) * part_h*i;
        painter.drawText(mm_w - 30*2, pos_y, QString("%1").arg(i+1));
        painter.drawLine(0, pos_y, mm_w-30, pos_y);
    }

    //    //纵向虚线 其实需要根据当前日期来确定具体定在什么位置 每个格子代表一个月
    //    for(int i = 0; i < 4; i++)
    //    {
    //        painter.drawLine(150*i,0,150*i,-350);
    //    }
    //画日K线图

    std::list<StockDayInfo>::iterator iter;
    float openPrice;//开盘价
    float closePrice;//收盘价
    float maxPrice;//最高价
    float minPrice;//最低价
    float marketMoney;//成交额
    int j = 0;
    //cout<<stockAllDaysInfo.GetStockAllDaysInfoList().size()<<endl;
    //？？？只需要最后60个数据，若少于等于60个，则正常绘图
    for(iter = stockAllDaysInfo.stockAllDaysInfoList.begin();
        iter != stockAllDaysInfo.stockAllDaysInfoList.end(); 
        iter++, j++)
    {
        //绘图每天的股票消息
        //((StockDayInfo)(*iter)).Display();
        //读取每天的股票数据，获得一支股票的最高价 最低价 开盘价 收盘价
        minPrice = ((StockDayInfo)(*iter)).GetMinPrice();
        maxPrice = ((StockDayInfo)(*iter)).GetMaxPrice();
        openPrice = ((StockDayInfo)(*iter)).GetOpenPrice();
        closePrice = ((StockDayInfo)(*iter)).GetClosePrice();
        std::cout<<minPrice<<"\t"<<maxPrice<<"\t"<<openPrice<<"\t"<<closePrice << std::endl;

         auto item_w = (mm_w / 8) ;
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
		const auto base_val = mm_h / 2; //350 
		painter.setPen(pen); //添加画笔
        painter.setBrush(brush); //添加画刷
            
		auto pos_y = -1 * base_val * (openPrice-lowestMinPrice)/(highestMaxPrice - lowestMinPrice);
		auto h_1 = -1 * base_val *(closePrice - openPrice)/(highestMaxPrice - lowestMinPrice);
        painter.drawRect(j * item_w + 1,  pos_y, k_bar_w, h_1); //绘制矩形
		
        painter.drawLine(j * item_w + k_bar_w/2, -1 * base_val * (maxPrice-lowestMinPrice)/(highestMaxPrice - lowestMinPrice),
            j * item_w + k_bar_w/2, -1 * base_val * (minPrice-lowestMinPrice)/(highestMaxPrice - lowestMinPrice));   //绘制直线

    }
}

void KLineWall::mouseDoubleClickEvent(QMouseEvent *e)
{
    qDebug() << " x:" << e->pos().x() <<" y:" << e->pos().y() << "\n";
    show_cross_line_ = !show_cross_line_;
    setMouseTracking(show_cross_line_);
    /*if( show_cross_line_ )
        setCursor(Qt::CrossCursor);*/
     
}

// this->mapFromGlobal(this->cursor().pos()) == event.pos()
void KLineWall::mouseMoveEvent(QMouseEvent *e)
{ 
    qDebug() << "===================================\n";
    //// 视图坐标原点(0,0)对应场景坐标（场景坐标）
    //qDebug() << "view->mapToScene(0, 0):" << this->mapToScene(0, 0);

    //// 场景坐标原点(0,0)对应视图坐标（视图坐标）
    //qDebug() << "view->mapFromScene(0, 0):" << view->mapFromScene(0, 0);

    //// 场景左上角坐标（场景坐标）
    //QPointF p1 = QPointF(scene->sceneRect().topLeft());
    //qDebug() << "p1:" << p1;

    //// 场景左上角对应视图坐标（视图坐标）
    //qDebug() << "view->mapFromScene(p1.x(), p1.y())" << view->mapFromScene(p1.x(), p1.y());
    //qDebug() << " x:" << e->pos().x() <<" y:" << e->pos().y() << "\n";
    // 
    qDebug() << "===================================\n";
    QPen pen; //画笔
    pen.setColor(Qt::white);
    pen.setStyle(Qt::SolidLine); 

    QPainter painter(this);
    painter.translate(30, 50);
    painter.setPen(pen);
    auto mm_w = this->width();
    auto mm_h = this->height();

#if 1 
    //if( show_cross_line_ )
    {
        auto pos = e->pos();
        auto pos_mapped = mapToGlobal(pos);

        //auto pos_mapped = mapFromGlobal(e->pos());
        //auto pos_mapped = mapToParent(e->pos());
        /*painter.drawLine(0, e->pos().y(), mm_w, e->pos().y());
        painter.drawLine(e->pos().x(), 0, e->pos().x(), mm_h);*/
        painter.drawLine(0, pos_mapped.y(), mm_w, pos_mapped.y());
        painter.drawLine(pos_mapped.x(), 0, pos_mapped.x(), mm_h);
    }
    update();
#endif
}
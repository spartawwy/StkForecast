#include "dayklinedialog.h"
#include "ui_dayklinedialog.h"

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QPainter>
#include <qdebug.h>


DayKLineDialog::DayKLineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DayKLineDialog)
{
    ui->setupUi(this);
    stockNum = "";

    this->highestMaxPrice = stockAllDaysInfo_.GetHighestMaxPrice();
    this->lowestMinPrice = stockAllDaysInfo_.GetLowestMinPrice();

    auto h = this->height();

    //--------------------
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setSceneRect(0, 0, 400, 400);
     
    //butterfly->setPos(scene->sceneRect().left(), scene->sceneRect().top());

    //scene->addItem(butterfly);
    QGraphicsView *view = new QGraphicsView;
    view->setScene(scene);

    view->show();

    // 视图坐标原点(0,0)对应场景坐标（场景坐标）
    qDebug() << "view->mapToScene(0, 0):" << view->mapToScene(0, 0);

    // 场景坐标原点(0,0)对应视图坐标（视图坐标）
    qDebug() << "view->mapFromScene(0, 0):" << view->mapFromScene(0, 0);

    // 场景左上角坐标（场景坐标）
    QPointF p1 = QPointF(scene->sceneRect().topLeft());
    qDebug() << "p1:" << p1;

    // 场景左上角对应视图坐标（视图坐标）
    qDebug() << "view->mapFromScene(p1.x(), p1.y())" << view->mapFromScene(p1.x(), p1.y());

    //--------------------------
}


DayKLineDialog::~DayKLineDialog()
{
    delete ui;
}

void DayKLineDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DayKLineDialog::setStockNum(std::string stockNum)
{
    this->stockNum = stockNum;

    this->setWindowTitle(QString("%1").arg(stockNum.c_str()) + QString::fromLocal8Bit("-日K线"));  // tmp no use
}

void DayKLineDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    auto mm_h = this->height();
    auto mm_w = this->width();
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
    painter.translate(30, 400);
    //painter.translate(70,600);

    painter.drawText(-30, 0, "(0,0)");
    QPen pen; //画笔
    //绘制背景各个虚线
    pen.setStyle(Qt::DotLine);   //设置画笔风格 点线
    painter.setPen(pen);
    //横向虚线
    for(int i = 0; i < 8; i++)
    {
        painter.drawLine(0, -50*i, 900, -50*i);
    }

    //    //纵向虚线 其实需要根据当前日期来确定具体定在什么位置 每个格子代表一个月
    //    for(int i = 0; i < 4; i++)
    //    {
    //        painter.drawLine(150*i,0,150*i,-350);
    //    }
    //画日K线图

    std::vector<std::shared_ptr<T_KlineDateItem> >::iterator iter;
    float openPrice;//开盘价
    float closePrice;//收盘价
    float maxPrice;//最高价
    float minPrice;//最低价
    float marketMoney;//成交额
    int j = 0;
    //cout<<stockAllDaysInfo_.GetStockAllDaysInfoList().size()<<endl;
    //？？？只需要最后60个数据，若少于等于60个，则正常绘图
    for(iter = stockAllDaysInfo_.KlineDataContainer_.begin();
        iter != stockAllDaysInfo_.KlineDataContainer_.end(); 
        iter++, j++)
    {
        //绘图每天的股票消息
        //((StockDayInfo)(*iter)).Display();
        //读取每天的股票数据，获得一支股票的最高价 最低价 开盘价 收盘价
        
      /*  minPrice = ((StockDayInfo)(*iter)).GetMinPrice();
        maxPrice = ((StockDayInfo)(*iter)).GetMaxPrice();
        openPrice = ((StockDayInfo)(*iter)).GetOpenPrice();
        closePrice = ((StockDayInfo)(*iter)).GetClosePrice();*/
        minPrice = (*iter)->stk_item.low_price;
        maxPrice = (*iter)->stk_item.high_price;
        openPrice = (*iter)->stk_item.open_price;
        closePrice = (*iter)->stk_item.close_price;

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

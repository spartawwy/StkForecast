#include "dayklinedialog.h"
#include "ui_dayklinedialog.h"
#include <QPainter>


DayKLineDialog::DayKLineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DayKLineDialog)
{
    ui->setupUi(this);
    stockNum = "";

    this->highestMaxPrice = stockAllDaysInfo.GetHighestMaxPrice();
    this->lowestMinPrice = stockAllDaysInfo.GetLowestMinPrice();

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
        painter.drawLine(0,-50*i,900,-50*i);
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
        iter != stockAllDaysInfo.stockAllDaysInfoList.end(); iter++,j++)
    {
        //绘图每天的股票消息
        //((StockDayInfo)(*iter)).Display();
        //读取每天的股票数据，获得一支股票的最高价 最低价 开盘价 收盘价
        minPrice = ((StockDayInfo)(*iter)).GetMinPrice();
        maxPrice = ((StockDayInfo)(*iter)).GetMaxPrice();
        openPrice = ((StockDayInfo)(*iter)).GetOpenPrice();
        closePrice = ((StockDayInfo)(*iter)).GetClosePrice();
        std::cout<<minPrice<<"\t"<<maxPrice<<"\t"<<openPrice<<"\t"<<closePrice << std::endl;
        //绘图每天的股票到K线图上
        if(openPrice <= closePrice)
        {
            pen.setStyle(Qt::SolidLine);
            pen.setColor(QColor(255,0,0));
            QBrush brush(QColor(255,0,0)); //画刷
            painter.setPen(pen); //添加画笔
            painter.setBrush(brush); //添加画刷
            painter.drawRect(j*15+1, -350*(openPrice-lowestMinPrice)/(highestMaxPrice - lowestMinPrice),
                13, -350*(closePrice - openPrice)/(highestMaxPrice - lowestMinPrice)); //绘制矩形
            painter.drawLine(j*15+8,-350*(maxPrice-lowestMinPrice)/(highestMaxPrice - lowestMinPrice),
                j*15+8, -350*(minPrice-lowestMinPrice)/(highestMaxPrice - lowestMinPrice));   //绘制直线

        }
        else
        {
            pen.setStyle(Qt::SolidLine);
            pen.setColor(QColor(0,255,0));
            QBrush brush(QColor(0,255,0)); //画刷
            painter.setPen(pen); //添加画笔
            painter.setBrush(brush); //添加画刷
            painter.drawRect(j*15+1, -350*(openPrice-lowestMinPrice)/(highestMaxPrice - lowestMinPrice),
                13, -350*(closePrice - openPrice)/(highestMaxPrice - lowestMinPrice)); //绘制矩形
            painter.drawLine(j*15+8, -350*(maxPrice-lowestMinPrice)/(highestMaxPrice - lowestMinPrice),
                j*15+8, -350*(minPrice-lowestMinPrice)/(highestMaxPrice - lowestMinPrice));   //绘制直线

        }
    }
}

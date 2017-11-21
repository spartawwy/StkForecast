#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QtWidgets/QCheckBox>
#include <iostream>
#include <string.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initUi();
    initTable();

    dayKLineDialog = NULL;

}

MainWindow::~MainWindow()
{
    delete timer;
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::updateDateTime()
{
    ui->labelCurrentTime->setText(
            QDateTime::currentDateTime().toString("yyyy-MM-dd HH:MM:ss"));

}

void MainWindow::initUi()
{
    this->setWindowTitle(QString(trUtf8("StockSystem")));

    timer = new QTimer(this);
    timer->setInterval(1000);
    timer->start();
    ui->labelCurrentTime->setText(
            QDateTime::currentDateTime().toString("yyyy-MM-dd HH:MM:ss"));
    connect(timer,SIGNAL(timeout()),this,SLOT(updateDateTime()));

    //init maintoolbar
    ui->mainToolBar->addAction(ui->actionExit);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(ui->labelFind);
    ui->mainToolBar->addWidget(ui->EdtFind);


    //init statusbar
    ui->statusBar->showMessage("hello",2000);
//    ui->statusBar->addWidget(ui->labelCurrentTime);
    ui->statusBar->addPermanentWidget(ui->labelCurrentTime);
}

void MainWindow::initTable()
{
    this->setCentralWidget(ui->tableWidget);  //让表格充满整个客户区域
    //代码 名称 新价 跌额 幅度 手率 交量 交额 开盘价 昨收价 最高价 最低价
    //表格初始化
    //设置列宽
    ui->tableWidget->setColumnWidth(0,60);
    ui->tableWidget->setColumnWidth(1,80);
    ui->tableWidget->setColumnWidth(2,40);
    ui->tableWidget->setColumnWidth(3,40);
    ui->tableWidget->setColumnWidth(4,40);
    ui->tableWidget->setColumnWidth(5,40);
    ui->tableWidget->setColumnWidth(6,80);
    ui->tableWidget->setColumnWidth(7,60);
    ui->tableWidget->setColumnWidth(8,50);
    ui->tableWidget->setColumnWidth(9,50);
    ui->tableWidget->setColumnWidth(10,50);
    ui->tableWidget->setColumnWidth(11,50);

    //设置整行选中
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);


    for(int i = 0; i < 10; i++)
    {
        //加载数据
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        //代码 名称 新价 跌额 幅度 手率 交量 交额 开盘价 昨收价 最高价 最低价
        //600031 三一重工 7.19 -0.07 -0.96 0.37 26.68 1.93 7.30 7.26 7.32 7.16
        //添加复选框
        //ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,0,new QCheckBox());

        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,new QTableWidgetItem("600031"));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(QString::fromLocal8Bit("三一重工")));
        //ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(trUtf8("三一重工")));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,new QTableWidgetItem("7.19"));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,new QTableWidgetItem("-0.07"));

        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,4,new QTableWidgetItem("-0.96"));
        
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,5,new QTableWidgetItem("0.37"));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,6,new QTableWidgetItem(QString::fromLocal8Bit("26.68万")));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,7,new QTableWidgetItem(QString::fromLocal8Bit("1.93亿")));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,8,new QTableWidgetItem("7.30"));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,9,new QTableWidgetItem("7.26"));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,10,new QTableWidgetItem("7.32"));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,11,new QTableWidgetItem("7.16"));
    }
}

//双击某一行，显示出该支股票的日K线
void MainWindow::on_tableWidget_itemDoubleClicked(QTableWidgetItem* item)
{
    //获取当前行的股票代号
//    cout<<item->row()<<"\t"<<"0"<<endl;
    string strStockNum = ui->tableWidget->item(item->row(),0)->text().toStdString();

    if(dayKLineDialog != NULL)
    {
        delete dayKLineDialog;
        dayKLineDialog = NULL;
    }
    dayKLineDialog = new DayKLineDialog(this);
    dayKLineDialog->setStockNum(strStockNum);
    dayKLineDialog->show();

}






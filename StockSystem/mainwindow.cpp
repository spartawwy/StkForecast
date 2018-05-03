
#include <windowsx.h>
#include <qt_windows.h> 


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <string.h>

#include <QDateTime>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QVBoxLayout>
#include <QString>

#include "kline_wall.h"
#include "title_bar.h"
#include "tool_bar.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , title_(nullptr)
    , kline_wall_(nullptr)
{
    ui->setupUi(this);
    /*delete ui->menuBar();
    ui->setMenuBar(nullptr);*/
      
    initUi();
       
}

MainWindow::~MainWindow()
{
#ifdef USE_STATUS_BAR
    delete timer;
#endif
    delete ui;
}


void MainWindow::initUi()
{  
#if 1
    // https://blog.csdn.net/qq_28093585/article/details/78517358
    this->setWindowFlags(Qt::FramelessWindowHint);  

    QWidget *wd = new QWidget(this);  
    QVBoxLayout *layout_all = new QVBoxLayout;  
    layout_all->setContentsMargins(0,0,0,0);  
    layout_all->setSpacing(0);  
 
    title_ = new TitleBar(this);
    layout_all->addWidget(title_);  

    kline_wall_ = new KLineWall(this);
    kline_wall_->setMouseTracking(true);
    auto tool_bar = new ToolBar(this);
    layout_all->addWidget(tool_bar);  
 
    layout_all->addWidget(kline_wall_);  
      
    wd->setLayout(layout_all);  
    this->setCentralWidget(wd);  
    kline_wall_->setFocusPolicy(Qt::StrongFocus);
#endif 
       
#ifdef USE_STATUS_BAR
    timer = new QTimer(this);
    timer->setInterval(1000);
    timer->start();
    ui->labelCurrentTime->setText(
            QDateTime::currentDateTime().toString("yyyy-MM-dd HH:MM:ss"));
    connect(timer,SIGNAL(timeout()),this,SLOT(updateDateTime()));
 
    ui->statusBar->showMessage("hello",2000); 
    ui->statusBar->addPermanentWidget(ui->labelCurrentTime);
#endif

}
 
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) 
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
 
bool MainWindow::eventFilter(QObject *o, QEvent *e)
{ 
    if( o == kline_wall_ )
    {
        switch ( e->type() )
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseMove:
            return true;
    }else
        return false;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    e->ignore();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::updateDateTime()
{
#ifdef USE_STATUS_BAR
    ui->labelCurrentTime->setText(
            QDateTime::currentDateTime().toString("yyyy-MM-dd HH:MM:ss"));
#endif
}

//
////双击某一行，显示出该支股票的日K线
//void MainWindow::on_tableWidget_itemDoubleClicked(QTableWidgetItem* item)
//{
////    //获取当前行的股票代号
//////    cout<<item->row()<<"\t"<<"0"<<endl;
////    string strStockNum = ui->tableWidget->item(item->row(),0)->text().toStdString();
////
////    if(dayKLineDialog != NULL)
////    {
////        delete dayKLineDialog;
////        dayKLineDialog = NULL;
////    }
////    dayKLineDialog = new DayKLineDialog(this);
////    dayKLineDialog->setStockNum(strStockNum);
////    dayKLineDialog->show();
//
//}
//





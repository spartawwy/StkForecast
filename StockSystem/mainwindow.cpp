﻿
#include <windowsx.h>
#include <qt_windows.h> 


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <string.h>

#include <QDateTime>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QComboBox>
#include <QString>
#include <QMessageBox>

#include "stk_forecast_app.h"
#include "kline_wall.h"
#include "title_bar.h"
#include "tool_bar.h"
#include "code_list_wall.h"

using namespace std;

MainWindow::MainWindow(StkForecastApp *app, QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , app_(app)
    , tool_bar_(nullptr)
    , title_(nullptr)
    , kline_wall_(nullptr)
    , code_list_wall_(nullptr)
{
    ui->setupUi(this);
     
}

MainWindow::~MainWindow()
{
#ifdef USE_STATUS_BAR
    delete timer;
#endif
    delete ui;
}


bool MainWindow::Initialize()
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

    kline_wall_ = new KLineWall(app_, this, 0);
    if( !kline_wall_->Init() )
        return false;
    kline_wall_->setMouseTracking(true);
    kline_wall_->RestTypePeriod(DEFAULT_TYPE_PERIOD);

    tool_bar_ = new ToolBar(this);
    tool_bar_->SetCurCycleType(DEFAULT_TYPE_PERIOD);
    bool ret = connect(tool_bar_->cycle_comb(), SIGNAL(currentIndexChanged(int)), this, SLOT(onCycleChange(int)));
    ret = ret;
     
    code_list_wall_ = new CodeListWall(app_, this);
    code_list_wall_->Init();

    layout_all->addWidget(tool_bar_);  
    layout_all->addWidget(kline_wall_);  
    layout_all->addWidget(code_list_wall_);  

    code_list_wall_->hide();

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
    return true;
}

void MainWindow::SetMainView(WallType wall_type)
{
    kline_wall_->hide();
    switch(wall_type)
    {    
    case WallType::KLINE: 
        code_list_wall_->hide();
        kline_wall_->show(); 
        break;
    case WallType::CODE_LIST: 
        kline_wall_->hide();
        code_list_wall_->show(); 
        break;
    default:break;
    }
}

void MainWindow::ResetKLineWallCode(const QString &code, const QString &cn_name, bool is_index)
{
    kline_wall_->ResetStock(code, cn_name, is_index);
}
 
void MainWindow::closeEvent(QCloseEvent * event)
{
    auto ret_button = QMessageBox::question(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("确定退出系统?"),
        QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
    if( ret_button == QMessageBox::Cancel )
        event->ignore();
    else
        app_->Stop();
}

void MainWindow::UncheckBtnABDownPen()
{
    if( tool_bar_ ) 
        tool_bar_->UncheckBtnABDownPen(); 
}

void MainWindow::UncheckBtnABUpPen()
{
    if( tool_bar_ ) 
        tool_bar_->UncheckBtnABUpPen(); 
}

void MainWindow::UncheckBtnABCDownPen()
{
    if( tool_bar_ ) 
        tool_bar_->UncheckBtnABCDownPen(); 
}

void MainWindow::UncheckBtnABCUpPen()
{
    if( tool_bar_ ) 
        tool_bar_->UncheckBtnABCUpPen(); 
}

void MainWindow::AddCode2CodeList(const QString &code, const QString &cn_name, bool is_index)
{
    code_list_wall_->AddCode(code , cn_name, is_index);
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
        {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseMove:
            return true;
        default: return false; // ndchk
        }
        
    }else
        return false;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    
    switch(e->key())
    {
        case Qt::Key_F5:
        {
            kline_wall_->show();
            code_list_wall_->hide();
        } break;
        case Qt::Key_F6:
        {
            kline_wall_->hide();
            code_list_wall_->show();
        } break;
        default:
            break;
    }
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
  
void MainWindow::onCycleChange(int /*index*/)
{
    assert(kline_wall_);
    tool_bar_->cycle_comb()->clearFocus();
    kline_wall_->RestTypePeriod( TypePeriod(tool_bar_->cycle_comb()->currentData().toInt()) );
     
}




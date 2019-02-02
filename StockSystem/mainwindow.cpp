
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

using namespace std;

MainWindow::MainWindow(StkForecastApp *app, QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , app_(app)
    , tool_bar_(nullptr)
    , title_(nullptr)
    , kline_wall_(nullptr)
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

    kline_wall_ = new KLineWall(app_, this);
    if( !kline_wall_->Init() )
        return false;
    kline_wall_->setMouseTracking(true);
    kline_wall_->RestTypePeriod(TypePeriod::PERIOD_DAY);

    tool_bar_ = new ToolBar(this);
    tool_bar_->SetCurCycleType(TypePeriod::PERIOD_DAY);
    bool ret = connect(tool_bar_->cycle_comb(), SIGNAL(currentIndexChanged(int)), this, SLOT(onCycleChange(int)));
    ret = ret;
     
    layout_all->addWidget(tool_bar_);  
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
    return true;
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
    kline_wall_->RestTypePeriod( TypePeriod(tool_bar_->cycle_comb()->currentData().toInt()) );
     
}




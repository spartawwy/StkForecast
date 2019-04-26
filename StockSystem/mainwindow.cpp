
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
    , kline_wall_main(nullptr)
    , kline_wall_sub(nullptr)
    , code_list_wall_(nullptr)
    , cur_kline_index_(WallIndex::MAIN)
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
    // view area ----------
    QHBoxLayout * view_layout = new QHBoxLayout;
    view_layout->setContentsMargins(0,0,0,0);  
    view_layout->setSpacing(1);  

    kline_wall_main = new KLineWall(app_, this, (int)WallIndex::MAIN);
    if( !kline_wall_main->Init() )
        return false;
    kline_wall_main->setMouseTracking(true);
    kline_wall_main->RestTypePeriod(DEFAULT_TYPE_PERIOD);
    kline_wall_main->setFocusPolicy(Qt::StrongFocus);
    view_layout->addWidget(kline_wall_main);

    kline_wall_sub = new KLineWall(app_, this, (int)WallIndex::SUB);
    if( !kline_wall_sub->Init() )
        return false;
    kline_wall_sub->setMouseTracking(true);
    kline_wall_sub->RestTypePeriod(DEFAULT_TYPE_PERIOD);
    kline_wall_sub->setFocusPolicy(Qt::StrongFocus);
    view_layout->addWidget(kline_wall_sub);

    kline_wall_sub->setVisible(false);
    // end of view area-------
    tool_bar_ = new ToolBar(this);
    tool_bar_->SetCurCycleType(DEFAULT_TYPE_PERIOD);
    bool ret = connect(tool_bar_->cycle_comb(), SIGNAL(currentIndexChanged(int)), this, SLOT(onCycleChange(int)));
    ret = ret;
     
    code_list_wall_ = new CodeListWall(app_, this);
    code_list_wall_->Init();

    layout_all->addWidget(tool_bar_);  
    layout_all->addLayout(view_layout);  
    layout_all->addWidget(code_list_wall_);  

    code_list_wall_->hide();

    wd->setLayout(layout_all);  
    this->setCentralWidget(wd);  
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

void MainWindow::SetCurKlineWallIndex(WallIndex index)
{
    if( cur_kline_index_ != index )
    { 
        CurKlineWall()->ResetDrawState(DrawAction::NO_ACTION);
    } 
    
    cur_kline_index_ = index; 
}

void MainWindow::SetMainView(WallType wall_type)
{
    kline_wall_main->hide();
    switch(wall_type)
    {    
    case WallType::KLINE: 
        code_list_wall_->hide();
        kline_wall_main->show(); 
        break;
    case WallType::CODE_LIST: 
        kline_wall_main->hide();
        code_list_wall_->show(); 
        break;
    default:break;
    }
}

void MainWindow::ResetKLineWallCode(const QString &code, const QString &cn_name, bool is_index)
{
    kline_wall_main->ResetStock(code, cn_name, is_index);
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
    if( o == kline_wall_main )
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
            kline_wall_main->show();
            code_list_wall_->hide();
        } break;
        case Qt::Key_F6:
        {
            kline_wall_main->hide();
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
    assert(kline_wall_main);
    tool_bar_->cycle_comb()->clearFocus();
    kline_wall_main->RestTypePeriod( TypePeriod(tool_bar_->cycle_comb()->currentData().toInt()) );
     
}




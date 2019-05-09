#include "train_dlg.h"

#include <QtWidgets/QComboBox>
#include <QMessageBox>

#include "mainwindow.h"
#include "tool_bar.h"
#include "kline_wall.h"


//static const int cst_period_day_index = 4;
 
TrainDlg::TrainDlg(KLineWall *parent,  MainWindow *main_win)
    : QWidget(nullptr)
    , parent_(parent)
    , main_win_(main_win)
    , trade_dlg_(this, true)
    , is_started_(false)
{
    ui.setupUi(this);

    trade_dlg_.setWindowFlags(trade_dlg_.windowFlags() | Qt::WindowStaysOnTopHint/*Qt::Dialog*/ );

    //ui.dateEditTrainBeg->mousePressEvent
    bool ret = connect(ui.calendar, SIGNAL(clicked(const QDate &)), this, SLOT(OnCalendarClicked(const QDate &)));
    ret = connect(ui.pbtnStart, SIGNAL(clicked()), this, SLOT(OnStartTrain()));
    ret = connect(ui.pbtnStop, SIGNAL(clicked()), this, SLOT(OnStopTrain()));
    ret = connect(ui.pbtnNextK, SIGNAL(clicked()), this, SLOT(OnMoveToNextK()));
    ret = connect(ui.pbtnPreK, SIGNAL(clicked()), this, SLOT(OnMoveToPreK()));
    ret = connect(ui.pbtnBuy, SIGNAL(clicked()), this, SLOT(OnOpenBuyWin()));
    ret = connect(ui.pbtnSell, SIGNAL(clicked()), this, SLOT(OnOpenSellWin()));

    ret = connect(trade_dlg_.ui.pbt_trade, SIGNAL(clicked()), this, SLOT(OnTrade()));

    ret = ret;
    ui.le_date->text().clear();
    ui.le_date->setReadOnly(true);
    OnStopTrain();
}

void TrainDlg::OnCalendarClicked(const QDate & date)
{ 
    ui.le_date->setText(date.toString("yyyyMMdd"));
}

void TrainDlg::closeEvent(QCloseEvent * event)
{
    main_win_->is_train_mode(false);
    parent_->k_rend_index_for_train_ = 0;
    main_win_->tool_bar()->main_cycle_comb()->setEnabled(true);

    is_started_ = false;

    OnStopTrain();
}

//void TrainDlg::hideEvent(QHideEvent * event)
//{
//    main_win_->is_train_mode(false);
//    parent_->k_rend_index_for_train_ = 0;
//    main_win_->tool_bar()->main_cycle_comb()->setEnabled(true);
//}

void TrainDlg::OnStartTrain()
{
    if( ui.le_date->text().isEmpty() )
    {
        ui.calendar->setFocus();
        return;
    }
    ui.calendar->setEnabled(false);
    ui.pbtnStart->setEnabled(false);
    ui.pbtnStop->setEnabled(true);

    ui.pbtnNextK->setEnabled(true);
    ui.pbtnPreK->setEnabled(true);

    ui.pbtnBuy->setEnabled(true);
    ui.pbtnSell->setEnabled(true);

    /*if( is_started_ )
    {   
         this->hide();
         auto ret = QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("是否重新开始训练?"), QMessageBox::Yes, QMessageBox::No); 
         if( ret == QMessageBox::No )
         {
             this->showNormal();
             return;
         }
         this->showNormal();
    }*/

    int date = ui.le_date->text().toInt();
    parent_->SetTrainStartDate(date);

    
    is_started_ = true;
}

void TrainDlg::OnStopTrain()
{
    //ui.le_date->text().clear();

    ui.calendar->setEnabled(true);
    ui.pbtnStart->setEnabled(true);
    ui.pbtnStop->setEnabled(false);

    ui.pbtnNextK->setEnabled(false);
    ui.pbtnPreK->setEnabled(false);

    ui.pbtnBuy->setEnabled(false);
    ui.pbtnSell->setEnabled(false);
}

void TrainDlg::OnMoveToNextK()
{
    parent_->MoveRightEndToNextKline();
}

void TrainDlg::OnMoveToPreK()
{
    parent_->MoveRightEndToPreKline();
}

void TrainDlg::OnOpenBuyWin()
{
    this->hide();
    trade_dlg_.is_sell(false);
    trade_dlg_.showNormal();
}

void TrainDlg::OnOpenSellWin()
{
    this->hide();
    trade_dlg_.is_sell(true);
    trade_dlg_.showNormal();
}

void TrainDlg::OnTrade()
{
    if( trade_dlg_.is_sell_ )
    {

    }else
    {

    }
}
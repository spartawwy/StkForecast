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
    account_info_.capital.avaliable = ui.dbspbBegCapital->value();
    account_info_.capital.frozen = 0.0;

    fee_rate_ = ui.dbspbFeeRate->value();

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
    if( account_info_.stock.frozen > 0 )
    {
        account_info_.stock.avaliable += account_info_.stock.frozen;
        account_info_.stock.frozen = 0;
    }
}

void TrainDlg::OnMoveToPreK()
{
    parent_->MoveRightEndToPreKline();
    if( !trade_records_stack_.empty() )
    {
        while( trade_records_stack_.top().date == parent_->CurTrainDate() )
        {
            TradeRecordAtom & trade_item = trade_records_stack_.top();
            if( trade_item.is_sell )
            {
                account_info_.capital.avaliable -= trade_item.price * trade_item.quantity - trade_item.fee;
/*
                trade_records_stack_
                account_info_.stock.frozen*/
            }
        }
    }
}

void TrainDlg::OnOpenBuyWin()
{
    this->hide();

    trade_dlg_.is_sell(false);
    trade_dlg_.SetDate(parent_->CurTrainDate());
    trade_dlg_.showNormal();
}

void TrainDlg::OnOpenSellWin()
{
    this->hide();
    trade_dlg_.is_sell(true);
    trade_dlg_.SetDate(parent_->CurTrainDate());
    trade_dlg_.showNormal();
}

void TrainDlg::OnTrade()
{
    this->setStatusTip("");

    if( trade_dlg_.is_sell_ )
    {

    }else
    {
        auto price = trade_dlg_.ui.le_price->text().toDouble();
        int quantity = trade_dlg_.ui.le_qty->text().toInt();
        quantity = quantity / 100 * 100;
        trade_dlg_.ui.le_qty->setText(QString("%1").arg(quantity));
        if( quantity < 100 )
        {
            this->setStatusTip(QString::fromLocal8Bit("购买数量不对!"));
            trade_dlg_.ui.le_qty->setFocus();
            return;
        }

        double capital_buy = price * quantity;
        double fee = CalculateFee(quantity, price, false);
        if( capital_buy + fee < account_info_.capital.avaliable )
        {
            this->setStatusTip(QString::fromLocal8Bit("可用资金不足!"));
            return;
        }
         
        trade_dlg_.ui.le_capital->setText(QString("%1").arg(capital_buy + fee));
        account_info_.capital.avaliable -= capital_buy + fee;
        account_info_.stock.frozen += quantity;

        TradeRecordAtom  trade_item;
        trade_item.date = trade_dlg_.date_;
        trade_item.price = price;
        trade_item.quantity = quantity;
        trade_item.fee = fee;
        trade_records_stack_.push(trade_item);

    }
}

double TrainDlg::CalculateFee(int quantity, double price, bool is_sell)
{
    assert(quantity > 0.0);
    assert(price > 0.0);
    double amount = quantity * price;
    double commission = amount < 5.1 ? 5.0 : (fee_rate_ * amount);
    return commission +  (is_sell ? amount / 1000 : 0);
}
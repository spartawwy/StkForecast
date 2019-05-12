
#include "train_trade_dlg.h"

#include <cassert>

#include <QString>
#include <QShowEvent>

#include "stkfo_common.h"

#include "train_dlg.h"

TrainTradeDlg::TrainTradeDlg(TrainDlg *train_dlg, bool is_sell) 
    : train_dlg_(train_dlg)
    , is_sell_(is_sell)
    , date_(0)
{
    ui.setupUi(this);
    bool ret = QObject::connect(ui.pbtn_close, SIGNAL(clicked()), SLOT(close()));
    ret = QObject::connect(ui.pbt_all, SIGNAL(clicked()), this, SLOT(OnBtnAllQuantity()));
    ret = QObject::connect(ui.pbtn_qty_half, SIGNAL(clicked()), this, SLOT(OnBtnHalfQuantity()));
    ret = QObject::connect(ui.pbtn_qty_one_third, SIGNAL(clicked()), this, SLOT(OnBtnOneThirdQuantity()));
    ret = QObject::connect(ui.pbtn_qty_one_fifth, SIGNAL(clicked()), this, SLOT(OnBtnOneFifthQuantity()));
    ret = ret;
}

void TrainTradeDlg::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
    train_dlg_->showNormal();
}

void TrainTradeDlg::showEvent(QShowEvent * event)
{
    QWidget::showEvent(event);
    if( event->type() != QEvent::Show )
        return;
    if( is_sell_ )
    {
        this->setWindowTitle(QString::fromLocal8Bit("卖出"));

        ui.lab_price->setText(QString::fromLocal8Bit("卖出价格"));
        ui.lab_qty_ava->setText(QString::fromLocal8Bit("可卖数量"));
        ui.lab_qty->setText(QString::fromLocal8Bit("卖出数量"));
        ui.lab_capital_ava->setText(QString::fromLocal8Bit("可用资金"));
        ui.lab_capital->setText(QString::fromLocal8Bit("资金"));
        ui.pbt_trade->setText(QString::fromLocal8Bit("卖出"));
    }else
    {
        this->setWindowTitle(QString::fromLocal8Bit("买入"));

        ui.lab_price->setText(QString::fromLocal8Bit("买入价格"));
        ui.lab_qty_ava->setText(QString::fromLocal8Bit("可买数量"));
        ui.lab_qty->setText(QString::fromLocal8Bit("买入数量"));
        ui.lab_capital_ava->setText(QString::fromLocal8Bit("可用资金"));
        ui.lab_capital->setText(QString::fromLocal8Bit("买入资金"));
        ui.pbt_trade->setText(QString::fromLocal8Bit("买入"));
    }
}

void TrainTradeDlg::SetStatusBar(const QString & val)
{
    ui.lab_status->setText(val);
}

void TrainTradeDlg::OnBtnAllQuantity()
{
    _onBtnQuantity(1.0);
}
void TrainTradeDlg::OnBtnHalfQuantity()
{
    _onBtnQuantity(0.5);
}

void TrainTradeDlg::OnBtnOneThirdQuantity()
{
    double val = 1.0 / 3.0;
    _onBtnQuantity(val);
}

void TrainTradeDlg::OnBtnOneFifthQuantity()
{
    _onBtnQuantity(1.0 / 5.0);
}

void TrainTradeDlg::_onBtnQuantity(double val)
{
    assert(val > 0.0);
    assert(val < 1.0001);
    if( is_sell_ )
    {
        ui.le_qty->setText( ToQString(int(train_dlg_->account_info().stock.avaliable * val)) );
    }else
    {
        const T_StockHisDataItem & stock_item = train_dlg_->CurHisStockDataItem();
        if( stock_item.date == 0 )
        {
            ui.le_qty->setText("0");
        }else
        {
            int qty = train_dlg_->account_info().capital.avaliable / stock_item.close_price;
            qty *= val;
            qty = qty / 100 * 100;
            ui.le_qty->setText(ToQString(qty));
        }
    }
}
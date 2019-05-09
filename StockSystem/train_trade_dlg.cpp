
#include "train_trade_dlg.h"

#include <QString>

TrainTradeDlg::TrainTradeDlg(bool is_sell) : is_sell_(is_sell)
{
    ui.setupUi(this);
    bool ret = QObject::connect(this, SIGNAL(showEvent(QShowEvent * )), SLOT(OnShowEvent(QShowEvent *)));

}

void TrainTradeDlg::OnShowEvent(QShowEvent * event)
{
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
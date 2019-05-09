#ifndef TRAIN_TRADE_DSFK3423_DLG_H_
#define TRAIN_TRADE_DSFK3423_DLG_H_

#include <QtWidgets/QWidget>

#include "ui_traintrade.h"

//class KLineWall;
//class MainWindow;
class TrainDlg;
class TrainTradeDlg : public QWidget
{
    Q_OBJECT

public:

    TrainTradeDlg(TrainDlg *train_dlg, bool is_sell);

    Ui::TrainTradeForm  ui;

    void is_sell(bool val) { is_sell_ = val; }

//private slots:
    virtual void closeEvent(QCloseEvent *) override;
    virtual void showEvent(QShowEvent *) override;

private:

    TrainDlg *train_dlg_;
    bool is_sell_;

    friend class TrainDlg;
};

#endif // TRAIN_TRADE_DSFK3423_DLG_H_

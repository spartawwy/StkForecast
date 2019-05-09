#ifndef TRAIN_TRADE_DSFK3423_DLG_H_
#define TRAIN_TRADE_DSFK3423_DLG_H_

#include <QtWidgets/QWidget>

#include "ui_traintrade.h"

class KLineWall;
class MainWindow;
class TrainTradeDlg : public QWidget
{
    Q_OBJECT

public:

    TrainTradeDlg(bool is_sell);

    Ui::TrainTradeForm  ui;

    void is_sell(bool val) { is_sell_ = val; }

private slots:
 
    void OnShowEvent(QShowEvent * event);

private:
    bool is_sell_;

};

#endif // TRAIN_TRADE_DSFK3423_DLG_H_

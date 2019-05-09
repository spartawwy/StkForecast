#ifndef TRAIN_DLG_SDFS23343543_H_
#define TRAIN_DLG_SDFS23343543_H_

#include <QtWidgets/QWidget>

#include "ui_traindlg.h"

#include "train_trade_dlg.h"

class KLineWall;
class MainWindow;
class TrainDlg : public QWidget
{
    Q_OBJECT

public:

    TrainDlg(KLineWall *parent, MainWindow *main_win);

public slots:

    void OnCalendarClicked(const QDate &);
    void OnStartTrain();
    void OnStopTrain();

    void OnMoveToNextK();
    void OnMoveToPreK();

    void OnOpenBuyWin();
    void OnOpenSellWin();

    void OnTrade();

protected:

    virtual void closeEvent(QCloseEvent *) override;
    //virtual void hideEvent(QHideEvent * event) override;


private:

    Ui::TrainDlgForm ui;

    TrainTradeDlg  trade_dlg_;

    KLineWall *parent_;
    MainWindow *main_win_;

    bool is_started_;
};

#endif // TRAIN_DLG_SDFS23343543_H_
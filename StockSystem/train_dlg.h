#ifndef TRAIN_DLG_SDFS23343543_H_
#define TRAIN_DLG_SDFS23343543_H_

//#include <unordered_map>
#include <stack>
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

    double CalculateFee(int quantity, double price, bool is_sell);

protected:

    virtual void closeEvent(QCloseEvent *) override;
    //virtual void hideEvent(QHideEvent * event) override;


private:

    Ui::TrainDlgForm ui;
     
    TrainTradeDlg  trade_dlg_;

    KLineWall *parent_;
    MainWindow *main_win_;
     
    bool is_started_;

    struct AccountAtom
    {
        //std::string  code;
        double  avaliable;
        double  frozen;
    };

    struct AccountInfo
    {
        AccountAtom  capital;
        //std::unordered_map<std::string, AccountAtom>  stocks;
        AccountAtom  stock;
    };

    struct TradeRecordAtom
    { 
        int date;
        bool is_sell;
        int quantity;
        double price;
        double fee;
        bool is_pre_date_freeze;
        TradeRecordAtom() : date(0), is_sell(false), quantity(0), price(0.0), fee(0.0), is_pre_date_freeze(false) { }
    };
    AccountInfo  account_info_;

    double fee_rate_;
     
    std::stack<TradeRecordAtom>  trade_records_stack_;
};


#endif // TRAIN_DLG_SDFS23343543_H_
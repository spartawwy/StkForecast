#ifndef TRAIN_DLG_SDFS23343543_H_
#define TRAIN_DLG_SDFS23343543_H_

//#include <unordered_map>
#include <vector>
#include <QtWidgets/QWidget>

#include "ui_traindlg.h"

#include "stkfo_common.h"
#include "train_trade_dlg.h"

struct AccountAtom
{ 
    double  avaliable;
    double  frozen;
    AccountAtom() : avaliable(0.0), frozen(0.0){}
};

struct AccountInfo
{
    AccountAtom  capital; 
    AccountAtom  stock;
};
enum RecordAction : unsigned char
{
    BUY = 0,
    SELL,
    UNFREEZE, // only related to stock, this action is in the front of current day's other action
};
class KLineWall;
class MainWindow;
class TrainDlg : public QWidget
{
    Q_OBJECT

public:

    TrainDlg(KLineWall *parent, MainWindow *main_win);
     
    const AccountInfo & account_info() { return account_info_; }

    const T_StockHisDataItem & CurHisStockDataItem();

    double CalculateFee(int quantity, double price, bool is_sell);

    int CalculateMaxQtyAllowBuy(double capital, double price);

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

    void PrintTradeRecords();

private:

    Ui::TrainDlgForm ui;
     
    TrainTradeDlg  trade_dlg_;

    KLineWall *parent_;
    MainWindow *main_win_;
     
    bool is_started_;
     
    class TradeRecordAtom
    { 
    public:
        int date;
        RecordAction action;
        int quantity;
        double price;
        double fee;

        explicit TradeRecordAtom() : date(0), action(RecordAction::BUY), quantity(0), price(0.0), fee(0.0){ }
        TradeRecordAtom(const TradeRecordAtom &lh) : date(lh.date), action(lh.action), quantity(lh.quantity), price(lh.price), fee(lh.fee){ }
        TradeRecordAtom & operator = (const TradeRecordAtom &lh)
        {
            if( &lh == this ) 
                return *this;
            date = lh.date; action = lh.action; quantity = lh.quantity; price = lh.price; fee = lh.fee;
            return *this;
        }

        QString ToQStr();
    };
    AccountInfo  account_info_;

    double fee_rate_;
     
    std::vector<TradeRecordAtom>  trade_records_;
};

std::string ToStr(RecordAction action);

#endif // TRAIN_DLG_SDFS23343543_H_
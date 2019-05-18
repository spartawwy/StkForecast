#include "train_dlg.h"

#include <QtWidgets/QComboBox>
#include <QMessageBox>

#include "mainwindow.h"
#include "tool_bar.h"
#include "kline_wall.h"


//static const int cst_period_day_index = 4;
 
static const double cst_default_ori_capital = 1000000.00;
static const double cst_default_fee_rate_percent = 0.025;
static const int cst_default_ori_stock_num = 0;

TrainDlg::TrainDlg(KLineWall *parent,  MainWindow *main_win)
    : QWidget(nullptr)
    , parent_(parent)
    , main_win_(main_win)
    , trade_dlg_(this, true)
    , is_started_(false)
    , account_info_()
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
     
    ui.dbspbBegCapital->setValue(cst_default_ori_capital);
    ui.dbspbFeeRate->setValue(cst_default_fee_rate_percent);
}

const T_StockHisDataItem & TrainDlg::CurHisStockDataItem()
{
    return parent_->CurTrainStockDataItem();
}

void TrainDlg::OnCalendarClicked(const QDate & date)
{ 
    ui.le_date->setText(date.toString("yyyyMMdd"));
}

void TrainDlg::closeEvent(QCloseEvent * /*event*/)
{
    main_win_->is_train_mode(false);
    parent_->k_rend_index_for_train_ = 0;
    main_win_->SubKlineWall()->k_rend_index_for_train_ = 0;
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
    trade_records_.clear();

    account_info_.capital.avaliable = ui.dbspbBegCapital->value();
    account_info_.capital.frozen = 0.0;
    account_info_.stock.avaliable = cst_default_ori_stock_num;
    account_info_.stock.frozen = 0.0;
    ui.le_cur_capital->setText(ToQString(account_info_.capital.avaliable + account_info_.capital.frozen));
    ui.le_cur_stock_num->setText(ToQString(cst_default_ori_stock_num));
    ui.lab_assets->setText(ToQString(cst_default_ori_capital));

    ui.plain_te_record->clear();

    ui.dbspbBegCapital->setEnabled(false);
    ui.dbspbFeeRate->setEnabled(false);
    fee_rate_ = ui.dbspbFeeRate->value() / 100;

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

    main_win_->SubKlineWall()->SetTrainStartDate(date);
    main_win_->SubKlineWall()->right_clicked_k_date(date);
    main_win_->SubKlineWall()->slotOpenRelatedSubKwall(false);
    

    is_started_ = true;
}

void TrainDlg::OnStopTrain()
{
    ui.calendar->setEnabled(true);
    ui.pbtnStart->setEnabled(true);
    ui.pbtnStop->setEnabled(false);

    ui.pbtnNextK->setEnabled(false);
    ui.pbtnPreK->setEnabled(false);

    ui.pbtnBuy->setEnabled(false);
    ui.pbtnSell->setEnabled(false);

    ui.dbspbFeeRate->setEnabled(true);
    ui.dbspbBegCapital->setEnabled(true);
}

void TrainDlg::OnMoveToNextK()
{
    parent_->MoveRightEndToNextDayK();
    main_win_->SubKlineWall()->MoveRightEndToNextDayK();

    const T_StockHisDataItem & stock_item = CurHisStockDataItem();
    //assert(trade_dlg_.date_ == stock_item.date);

    if( account_info_.stock.frozen > 0 )
    {
        TradeRecordAtom record_item;
        record_item.date = stock_item.date;
        record_item.action = RecordAction::UNFREEZE;
        record_item.quantity = account_info_.stock.frozen;
        trade_records_.push_back(record_item);

        account_info_.stock.avaliable += account_info_.stock.frozen;
        account_info_.stock.frozen = 0;
    }
    ui.le_cur_stock_num->setText(ToQString(int(account_info_.stock.avaliable + account_info_.stock.frozen)));
    double assets_today_end = stock_item.close_price * (account_info_.stock.avaliable + account_info_.stock.frozen) 
        + account_info_.capital.avaliable + account_info_.capital.frozen;
    ui.lab_assets->setText(ToQString(assets_today_end));
    PrintTradeRecords();
}

void TrainDlg::OnMoveToPreK()
{
    const T_StockHisDataItem & stock_item = CurHisStockDataItem();
    // roll back trade records of cur k line 's 
    while( !trade_records_.empty() && trade_records_.back().date == stock_item.date )
    {
        // do roll back ---------------
        TradeRecordAtom & trade_item = trade_records_.back();
        switch(trade_item.action) 
        {
        case RecordAction::SELL:
            {
                account_info_.capital.avaliable -= trade_item.price * trade_item.quantity - trade_item.fee;
                account_info_.stock.avaliable += trade_item.quantity;
            }break;
        case RecordAction::BUY:
            {
                account_info_.capital.avaliable += trade_item.price * trade_item.quantity + trade_item.fee;
                assert(account_info_.stock.frozen > trade_item.quantity - 0.0001);
                account_info_.stock.frozen -= trade_item.quantity;
            }break;
        case RecordAction::UNFREEZE:
            {
                assert(account_info_.stock.avaliable > trade_item.quantity - 0.0001);
                account_info_.stock.avaliable -= trade_item.quantity;
                account_info_.stock.frozen += trade_item.quantity;
            }break;
        default:assert(false); break;
        }
         
        trade_records_.pop_back();
    } // while 

    parent_->MoveRightEndToPreDayK();
    main_win_->SubKlineWall()->MoveRightEndToPreDayK();

    const T_StockHisDataItem & pre_stock_item = CurHisStockDataItem();
    ui.le_cur_stock_num->setText(ToQString(int(account_info_.stock.avaliable + account_info_.stock.frozen)));
    ui.le_cur_capital->setText(ToQString(account_info_.capital.avaliable + account_info_.capital.frozen));
    
    double assets_today_end = pre_stock_item.close_price * (account_info_.stock.avaliable + account_info_.stock.frozen) 
        + account_info_.capital.avaliable + account_info_.capital.frozen;
    ui.lab_assets->setText(ToQString(assets_today_end));
    PrintTradeRecords();
}

void TrainDlg::OnOpenBuyWin()
{
    trade_dlg_.SetStatusBar("");
    const T_StockHisDataItem & stock_item = CurHisStockDataItem();
    if( stock_item.date == 0 )
    {
        trade_dlg_.SetStatusBar(QString::fromLocal8Bit("当前日期无对应股票数据!"));
        return;
    }
    this->hide();
    trade_dlg_.is_sell(false);
    trade_dlg_.SetDate(stock_item.date);
    //const double allow_highest_price = (stock_item.open_price < stock_item.close_price ? stock_item.close_price : stock_item.open_price);

    trade_dlg_.ui.le_price->setText(ToQString(stock_item.close_price));
     
    int qty = CalculateMaxQtyAllowBuy(account_info_.capital.avaliable, stock_item.close_price);
    trade_dlg_.ui.le_qty_ava->setText(ToQString(qty));
    trade_dlg_.ui.le_qty->setText(ToQString(qty));
    trade_dlg_.ui.le_capital_ava->setText(ToQString(account_info_.capital.avaliable));
    trade_dlg_.ui.le_capital->clear();

    trade_dlg_.showNormal();
}

void TrainDlg::OnOpenSellWin()
{
    trade_dlg_.SetStatusBar("");
    const T_StockHisDataItem & stock_item = CurHisStockDataItem();
    if( stock_item.date == 0 )
    {
        trade_dlg_.SetStatusBar(QString::fromLocal8Bit("当前日期无对应股票数据!"));
        return;
    }
    this->hide();
    trade_dlg_.is_sell(true);
    trade_dlg_.SetDate(stock_item.date);
    //const double allow_highest_price = (stock_item.open_price < stock_item.close_price ? stock_item.close_price : stock_item.open_price);

    trade_dlg_.ui.le_price->setText(ToQString(stock_item.close_price));
    trade_dlg_.ui.le_qty_ava->setText(ToQString(account_info_.stock.avaliable));
    trade_dlg_.ui.le_qty->setText(ToQString(account_info_.stock.avaliable));
    trade_dlg_.ui.le_capital_ava->setText(ToQString(account_info_.capital.avaliable));
    trade_dlg_.ui.le_capital->clear();

    trade_dlg_.showNormal();
}

void TrainDlg::OnTrade()
{
    trade_dlg_.SetStatusBar("");
    if( trade_dlg_.date_ == 0 )
    {
        trade_dlg_.SetStatusBar(QString::fromLocal8Bit("当前日期无对应股票数据!"));
        return;
    }
    const T_StockHisDataItem & stock_item = CurHisStockDataItem();
    assert(trade_dlg_.date_ == stock_item.date);

    double price = trade_dlg_.ui.le_price->text().toDouble();
    int quantity = (int)trade_dlg_.ui.le_qty->text().toDouble();
    trade_dlg_.ui.le_qty->setText(ToQString(quantity));

    const double allow_lowest_price = (stock_item.open_price < stock_item.close_price ? stock_item.open_price : stock_item.close_price);
    const double allow_highest_price = (stock_item.open_price < stock_item.close_price ? stock_item.close_price : stock_item.open_price);
    if( price < allow_lowest_price || price > allow_highest_price ) 
    {
        trade_dlg_.SetStatusBar(QString::fromLocal8Bit("报价非法!"));
        trade_dlg_.ui.le_price->setFocus();
        return;
    }
    if( trade_dlg_.is_sell_ )
    {
        if( quantity <= 0 )
        {
            trade_dlg_.SetStatusBar(QString::fromLocal8Bit("卖出数量非法!"));
            trade_dlg_.ui.le_qty->setFocus();
            return;
        } 
        if( quantity > account_info_.stock.avaliable )
        {
            trade_dlg_.SetStatusBar(QString::fromLocal8Bit("可用数量不足!"));
            trade_dlg_.ui.le_qty->setFocus();
            return;
        }
        
        double fee = CalculateFee(quantity, price, true);
        double capital_income = price * quantity - fee;
        trade_dlg_.ui.le_capital->setText(ToQString(capital_income));

        account_info_.capital.avaliable += capital_income;
        account_info_.stock.avaliable -= quantity;

        TradeRecordAtom  trade_item;
        trade_item.date = trade_dlg_.date_;
        trade_item.action = RecordAction::SELL;
        trade_item.price = price;
        trade_item.quantity = quantity;
        trade_item.fee = fee;
        trade_records_.push_back(trade_item);

    }else
    {
        quantity = quantity / 100 * 100;
        
        if( quantity < 100 )
        {
            trade_dlg_.SetStatusBar(QString::fromLocal8Bit("购买数量非法!"));
            trade_dlg_.ui.le_qty->setFocus();
            return;
        }

        double capital_buy = price * quantity;
        double fee = CalculateFee(quantity, price, false);
        if( capital_buy + fee > account_info_.capital.avaliable )
        {
            trade_dlg_.SetStatusBar(QString::fromLocal8Bit("可用资金不足!"));
            return;
        }
         
        trade_dlg_.ui.le_capital->setText(ToQString(capital_buy + fee));

        account_info_.capital.avaliable -= capital_buy + fee;
        account_info_.stock.frozen += quantity;

        TradeRecordAtom  trade_item;
        trade_item.date = trade_dlg_.date_;
        trade_item.action = RecordAction::BUY;
        trade_item.price = price;
        trade_item.quantity = quantity;
        trade_item.fee = fee;
        trade_records_.push_back(trade_item);
    }
    
    // update capital and stock num on ui--------
    ui.le_cur_capital->setText(ToQString(account_info_.capital.avaliable));
    ui.le_cur_stock_num->setText(ToQString(int(account_info_.stock.avaliable + account_info_.stock.frozen)));

    double assets_today_end = stock_item.close_price * (account_info_.stock.avaliable + account_info_.stock.frozen) 
        + account_info_.capital.avaliable + account_info_.capital.frozen;
    ui.lab_assets->setText(ToQString(assets_today_end));

    trade_dlg_.hide();
    PrintTradeRecords();
    this->showNormal();
}

double TrainDlg::CalculateFee(int quantity, double price, bool is_sell)
{
    assert(quantity > 0.0);
    assert(price > 0.0);
    double amount = quantity * price;
    double commission = amount < 5.1 ? 5.0 : (fee_rate_ * amount);
    return commission +  (is_sell ? amount / 1000 : 0);
}

int TrainDlg::CalculateMaxQtyAllowBuy(double capital, double price)
{
    assert(capital > 0.0);
    assert(price > 0.0);
    int qty = capital / price;
    qty = qty / 100 * 100;
    if( qty < 100 )
        return 0;
    double fee = CalculateFee(qty, price, false);

    for( int i = 0; i < 100; ++i )
    {
        if( qty * price + fee > capital && qty >= 100 )
        {
            qty -= 100;
            fee = CalculateFee(qty, price, false);
        }else
            break;
    }
    return qty < 100 ? 0 : qty;
}

void TrainDlg::PrintTradeRecords()
{
    ui.plain_te_record->clear();

    QString records_str;
    for(unsigned int i = 0; i < trade_records_.size(); ++i )
    {
        records_str.append(trade_records_.at(i).ToQStr());
        records_str.append("\n");
    }
    ui.plain_te_record->setPlainText(records_str);
    //ui.plain_te_record->setText()
}

QString TrainDlg::TradeRecordAtom::ToQStr()
{
    char buf[1024] = {'\0'};
    sprintf_s(buf, "日期:%d %s 数量:%d 价格:%.2f 佣金税费:%.2f "
        , this->date
        , ToStr(this->action).c_str()
        , this->quantity
        , this->price
        , this->fee);
    return QString::fromLocal8Bit(buf);
}

std::string ToStr(RecordAction action)
{
    switch(action)
    {
    case RecordAction::BUY:  return "买"; break;
    case RecordAction::SELL: return "卖";break;
    case RecordAction::UNFREEZE: return "解冻";break;
    default:assert(false);
    }
    return "";
}
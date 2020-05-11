#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <QtWidgets/QTableWidgetItem>
 
#include "stockinput_dlg.h"

//#define  USE_TITLE_VCM
//#define  USE_STATUS_BAR 

enum class WallIndex : unsigned char
{
    MAIN,
    SUB,
};

namespace Ui 
{
    class MainWindow;
}
enum class WallType : unsigned char
{
    KLINE,
    MINUTE_DETAIL,
    CODE_LIST,
};

class TrainDlg;
class StkForecastApp;
class TitleBar;
class ToolBar;
class KLineWall;
class CodeListWall;
class MinuteDetailWall;
class MainWindow : public QMainWindow 
{
    Q_OBJECT

public:

    MainWindow(StkForecastApp *app, QWidget *parent = 0);
    ~MainWindow();

    bool Initialize();
    ToolBar* tool_bar() { return tool_bar_; }

    void SetCurKlineWallIndex(WallIndex index);
    KLineWall * MainKlineWall() { return kline_wall_main; }
    KLineWall * SubKlineWall() { return kline_wall_sub; }
    KLineWall * CurKlineWall() { if( cur_kline_index_ == WallIndex::MAIN ) return kline_wall_main; else return kline_wall_sub; }
    //void DoCurKlineWallIndexChange();
    //KLineWall * kline_wall(WallIndex index) { if( index == WallIndex::MAIN ) return kline_wall_main; else return kline_wall_sub; }

    void SetMainView(WallType wall_type);
    void ResetKLineWallCode(const QString &code, const QString &cn_name, bool is_index);

    void UncheckBtnABDownPen();
    void UncheckBtnABUpPen();
    void UncheckBtnABCDownPen();
    void UncheckBtnABCUpPen();

    void AddCode2CodeList(const QString &code, const QString &cn_name, bool is_index);

    void StockInputDlgRet();

    void PopTrainDlg();

    bool is_train_mode(){ return is_train_mode_; }
    void is_train_mode(bool val) { is_train_mode_ = val; }

    void UpdateStockData();

protected:

    virtual void closeEvent(QCloseEvent * event) override;
#ifdef USE_TITLE_VCM
    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
#endif
    virtual bool eventFilter(QObject *o, QEvent *e) override;

    virtual void keyPressEvent(QKeyEvent *e) override;
    virtual void changeEvent(QEvent *e) override;
    
    void updateDateTime();

private:
      
    Ui::MainWindow *ui;
    StkForecastApp *app_;
   
    QTimer *timer;
    int timer_update_kwall_inter_;

    // cutomer qt controler --------
    ToolBar        *tool_bar_;
    TitleBar       *title_;
    KLineWall      *kline_wall_main;
    KLineWall      *kline_wall_sub;
    CodeListWall   *code_list_wall_;
    MinuteDetailWall   *minute_detail_wall_;
    //DayKLineDialog *dayKLineDialog;
    WallIndex cur_kline_index_;

    StockInputDlg  stock_input_dlg_;

    TrainDlg *train_dlg_;
    volatile bool  is_train_mode_;

    WallType wall_type_;

private slots:

    void onTimer();

    //void on_tableWidget_itemDoubleClicked(QTableWidgetItem* item);
    void on_actionExit_triggered(); 
    void onMainKwallCycleChange(int /*index*/);
    void onSubKwallCycleChange(int /*index*/);
    
    friend class TrainDlg;
};

#endif // MAINWINDOW_H

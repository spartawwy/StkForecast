#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <QtWidgets/QTableWidgetItem>
 

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
    CODE_LIST,
};
class StkForecastApp;
class TitleBar;
class ToolBar;
class KLineWall;
class CodeListWall;
class MainWindow : public QMainWindow 
{
    Q_OBJECT

public:

    MainWindow(StkForecastApp *app, QWidget *parent = 0);
    ~MainWindow();

    bool Initialize();
    void SetCurKlineWallIndex(WallIndex index);
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

protected:

    virtual void closeEvent(QCloseEvent * event) override;
#ifdef USE_TITLE_VCM
    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
#endif
    virtual bool eventFilter(QObject *o, QEvent *e) override;

    virtual void keyPressEvent(QKeyEvent *e) override;
    virtual void changeEvent(QEvent *e) override;
    

private:
      
    Ui::MainWindow *ui;
    StkForecastApp *app_;
   
    QTimer *timer;

    // cutomer qt controler --------
    ToolBar        *tool_bar_;
    TitleBar       *title_;
    KLineWall      *kline_wall_main;
    KLineWall      *kline_wall_sub;
    CodeListWall   *code_list_wall_;
    //DayKLineDialog *dayKLineDialog;
    WallIndex cur_kline_index_;

private slots:

    //void on_tableWidget_itemDoubleClicked(QTableWidgetItem* item);
    void on_actionExit_triggered();
    void updateDateTime();

    void onCycleChange(int /*index*/);
};

#endif // MAINWINDOW_H

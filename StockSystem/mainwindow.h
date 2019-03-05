#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <QtWidgets/QTableWidgetItem>
 

//#define  USE_TITLE_VCM
//#define  USE_STATUS_BAR

namespace Ui 
{
    class MainWindow;
}
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
    KLineWall * kline_wall() { return kline_wall_; }

    void UncheckBtnABDownPen();
    void UncheckBtnABUpPen();
    void UncheckBtnABCDownPen();
    void UncheckBtnABCUpPen();

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
    KLineWall      *kline_wall_;
    CodeListWall   *code_list_wall_;
    //DayKLineDialog *dayKLineDialog;

private slots:

    //void on_tableWidget_itemDoubleClicked(QTableWidgetItem* item);
    void on_actionExit_triggered();
    void updateDateTime();

    void onCycleChange(int /*index*/);
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <QtWidgets/QTableWidgetItem>
#include "dayklinedialog.h"

//#define  USE_TITLE_VCM
//#define  USE_STATUS_BAR

namespace Ui 
{
    class MainWindow;
}
class KLineWall;
class TitleBar;
class MainWindow : public QMainWindow 
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
#ifdef USE_TITLE_VCM
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
#endif
    void keyPressEvent(QKeyEvent *e) override;
    void changeEvent(QEvent *e);

private:

    void initUi();

    Ui::MainWindow *ui;
    QTimer *timer;

    // cutomer qt controler --------
    TitleBar       *title_;
    KLineWall      *kline_wall_;
    //DayKLineDialog *dayKLineDialog;

private slots:

    //void on_tableWidget_itemDoubleClicked(QTableWidgetItem* item);
    void on_actionExit_triggered();
    void updateDateTime();
};

#endif // MAINWINDOW_H

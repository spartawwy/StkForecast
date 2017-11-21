#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <QtWidgets/QTableWidgetItem>
#include "dayklinedialog.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    void initUi();
    void initTable();
    DayKLineDialog* dayKLineDialog;

private slots:
    void on_tableWidget_itemDoubleClicked(QTableWidgetItem* item);
    void on_actionExit_triggered();
    void updateDateTime();
};

#endif // MAINWINDOW_H

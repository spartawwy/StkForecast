#ifndef DAYKLINEDIALOG_H
#define DAYKLINEDIALOG_H

#include <QtWidgets/QDialog>
#include <iostream>
#include <string>
#include "stockdayinfo.h"
#include "stockalldaysinfo.h"


namespace Ui {
    class DayKLineDialog;
}

class DayKLineDialog : public QDialog {
    Q_OBJECT
public:
    DayKLineDialog(QWidget *parent = 0);
    ~DayKLineDialog();
    void setStockNum(std::string stockNum);

protected:
    void changeEvent(QEvent *e);
    void paintEvent(QPaintEvent *);

private:
    Ui::DayKLineDialog *ui;
    std::string stockNum;
    StockAllDaysInfo stockAllDaysInfo_;  //一支股票的所有天数的消息
    float lowestMinPrice;
    float highestMaxPrice;

};

#endif // DAYKLINEDIALOG_H

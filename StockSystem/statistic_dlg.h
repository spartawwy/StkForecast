#ifndef STATISTIC_DLG_SDFD23SDF_H_
#define STATISTIC_DLG_SDFD23SDF_H_


#include "ui_statisticdlg.h"
 
#include <QtWidgets/QWidget>

class KLineWall;
//class HintList;
//class DataBase;
class StatisticDlg : public QWidget
{
    Q_OBJECT

public:
    StatisticDlg();

private slots:

    void slotDoBtnCloseClicked();

private:
    virtual void keyPressEvent(QKeyEvent *e) override ;

    Ui_Form ui;

    friend class KLineWall;
};
#endif
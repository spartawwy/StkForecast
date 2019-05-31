#ifndef ALGOTOOL_H
#define ALGOTOOL_H

#include <QtWidgets/QWidget>
#include "ui_algotool.h"

class AlgoTool : public QWidget
{
    Q_OBJECT

public:
    AlgoTool(QWidget *parent = 0);
    ~AlgoTool();

private slots:
    void on2pForcast();
    void on3pForcast();

private:
    Ui::AlgoToolClass ui;
};

#endif // ALGOTOOL_H

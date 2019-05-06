#ifndef TRAIN_DLG_SDFS23343543_H_
#define TRAIN_DLG_SDFS23343543_H_

#include <QtWidgets/QWidget>

#include "ui_traindlg.h"

class KLineWall;
class MainWindow;
class TrainDlg : public QWidget
{
    Q_OBJECT

public:

    TrainDlg(KLineWall *parent, MainWindow *main_win);

public slots:

    void OnCalendarClicked(const QDate &);
    void OnStartTrain();
    void OnMoveToNextK();
    void OnMoveToPreK();

protected:

    virtual void hideEvent(QHideEvent * event) override;


private:

    Ui::TrainDlgForm ui;

    KLineWall *parent_;
    MainWindow *main_win_;
};

#endif // TRAIN_DLG_SDFS23343543_H_
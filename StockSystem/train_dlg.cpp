#include "train_dlg.h"

#include "mainwindow.h"
#include "kline_wall.h"

TrainDlg::TrainDlg(KLineWall *parent,  MainWindow *main_win)
    : QWidget(nullptr)
    , parent_(parent)
    , main_win_(main_win)
{
    ui.setupUi(this);

    //ui.dateEditTrainBeg->mousePressEvent
}

void TrainDlg::hideEvent(QHideEvent * event)
{
    main_win_->is_train_mode(false);
}

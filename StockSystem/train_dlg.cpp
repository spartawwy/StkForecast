#include "train_dlg.h"

#include <QtWidgets/QComboBox>

#include "mainwindow.h"
#include "tool_bar.h"
#include "kline_wall.h"

//static const int cst_period_day_index = 4;
 
TrainDlg::TrainDlg(KLineWall *parent,  MainWindow *main_win)
    : QWidget(nullptr)
    , parent_(parent)
    , main_win_(main_win)
{
    ui.setupUi(this);

    //ui.dateEditTrainBeg->mousePressEvent
    bool ret = connect(ui.calendar, SIGNAL(clicked(const QDate &)), this, SLOT(OnCalendarClicked(const QDate &)));
    ret = connect(ui.pbtnStart, SIGNAL(clicked()), this, SLOT(OnStartTrain()));
    ret = ret;
}

void TrainDlg::OnCalendarClicked(const QDate & date)
{
    ui.le_date->setReadOnly(true);
    ui.le_date->setText(date.toString("yyyyMMdd"));
}

void TrainDlg::hideEvent(QHideEvent * event)
{
    main_win_->is_train_mode(false);
    parent_->k_rend_index_for_train_ = 0;
    main_win_->tool_bar()->main_cycle_comb()->setEnabled(true);
}

void TrainDlg::OnStartTrain()
{
    
    if( ui.le_date->text().isEmpty() )
        return;
    if( main_win_->tool_bar()->main_cycle_comb()->currentIndex() != COMBO_PERIOD_DAY_INDEX )
    {
        // ps: it will trigger onMainKwallCycleChange
        main_win_->tool_bar()->main_cycle_comb()->setCurrentIndex(COMBO_PERIOD_DAY_INDEX); // set current to period day
        //main_win_->onMainKwallCycleChange(0);
    }
    if( main_win_->tool_bar()->sub_cycle_comb()->currentIndex() == COMBO_PERIOD_DAY_INDEX )
    {
        // ps: it will trigger onSubKwallCycleChange
        main_win_->tool_bar()->sub_cycle_comb()->setCurrentIndex(COMBO_PERIOD_DAY_INDEX - 1);
        //main_win_->onSubKwallCycleChange(0);
    }
    main_win_->tool_bar()->main_cycle_comb()->setEnabled(false);

    int date = ui.le_date->text().toInt();
    parent_->SetTrainStartDate(date);

}
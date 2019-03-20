
#include "statistic_dlg.h"

StatisticDlg::StatisticDlg()
{
    ui.setupUi(this);

    bool ret = QObject::connect(ui.pbt_close, SIGNAL(clicked()), this, SLOT(slotDoBtnCloseClicked()));
    ret = ret ;
}

void StatisticDlg::slotDoBtnCloseClicked()
{
    hide();
}

// ps : keys which parent has procedured will not enter this function
void StatisticDlg::keyPressEvent(QKeyEvent *e)
{
	hide();
}
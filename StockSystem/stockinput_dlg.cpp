#include "stockinput_dlg.h"

#include "kline_wall.h"

StockInputDlg::StockInputDlg(KLineWall *parent)
	: parent_(parent)
{
	ui.setupUi(this);
}

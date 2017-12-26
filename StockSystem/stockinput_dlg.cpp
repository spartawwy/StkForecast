#include "stockinput_dlg.h"

#include "kline_wall.h"
#include <QDebug>
StockInputDlg::StockInputDlg(KLineWall *parent)
	: parent_(parent)
{
	ui.setupUi(this);
}

void StockInputDlg::keyPressEvent(QKeyEvent *e)
{
	/*char buf[1024] = {"\0"};
	sprintf_s(buf, "%0x\n", e->key() );
	qDebug() << "buf: " << buf << "\n";*/
	if( e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter )
	{
		qDebug() << __FUNCDNAME__ << e->key() << "\n";
		parent_->StockInputDlgRet();
		this->hide();
	}else if( e->key() == Qt::Key_Escape )
	{
		this->hide();
	}else 
		qDebug() << __FUNCDNAME__ << e->key() << "\n";
}
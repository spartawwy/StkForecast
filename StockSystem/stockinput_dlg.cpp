#include "stockinput_dlg.h"

#include <QDebug>
//#include "kline_wall.h"
#include "mainwindow.h"
#include "HintList.h"

#include "database.h"
//
//StockInputDlg::StockInputDlg(KLineWall *parent, std::shared_ptr<DataBase> &db)
//	: parent_(parent)
//    , db_(db)
StockInputDlg::StockInputDlg(MainWindow *parent, std::shared_ptr<DataBase> &db)
	: parent_(parent)
    , db_(db)
{
	ui.setupUi(this);
    m_list_hint_ = new HintList(this, ui.stock_input);
    m_list_hint_->hide();
    bool ret = QObject::connect(ui.stock_input, SIGNAL(textChanged(QString)), this, SLOT(FlushFromStationListWidget(QString)));
    ret = QObject::connect(m_list_hint_, SIGNAL(clicked(QModelIndex)), this, SLOT(OnClickedListWidget(QModelIndex)));
    ret = QObject::connect(m_list_hint_, SIGNAL(choiceStr(QString)), this, SLOT(ChangeFromStationText(QString)));
}

// ps : keys which parent has procedured will not enter this function
void StockInputDlg::keyPressEvent(QKeyEvent *e)
{ 
	if( e->key() == Qt::Key_Escape )
	{
        if( m_list_hint_->hasFocus() )
        {
            m_list_hint_->clear();
            m_list_hint_->hide();
            ui.stock_input->setFocus();
        }else
		    this->hide();
	}else 
		m_list_hint_->setFocus();
}


void StockInputDlg::FlushFromStationListWidget(QString str)
{
    //qDebug() << "FlushFromStationListWidget " << str << "\n";
    assert(db_); 
    m_list_hint_->clear();

    std::vector<T_StockCodeName> stockCodeNames;
    db_->GetStockCode(str.toLocal8Bit().data(), stockCodeNames);
     
    std::for_each( std::begin(stockCodeNames), std::end(stockCodeNames), [this](T_StockCodeName& entry)
    {  
        this->m_list_hint_->addItem( QString("%1/%2/%3").arg(entry.code.c_str()).arg(QString::fromLocal8Bit(entry.name.c_str())).arg(entry.type) );
    });
    if( m_list_hint_->count() > 0 )
    {
        int x,y, w,h;
        ui.stock_input->geometry().getRect(&x, &y, &w, &h);
        m_list_hint_->setGeometry( x + 4, y + 20, w, h + 80);
         
        m_list_hint_->setCurrentRow(0);
        m_list_hint_->ShowUI();
    }
} 

void StockInputDlg::OnClickedListWidget(QModelIndex index)
{
    QString text = m_list_hint_->item(index.row())->text();
    ChangeFromStationText(text);
}

void StockInputDlg::ChangeFromStationText(QString text)
{ 
    ui.stock_input->setText(text);
    m_list_hint_->hide();
    parent_->StockInputDlgRet();
    this->hide();
}
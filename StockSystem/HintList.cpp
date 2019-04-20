#include "HintList.h"

#include <qtimer.h>
#include <QKeyEvent>
#include <QLineEdit>
//#define AUTO_DISPEAR_HINT 

HintList::HintList(QWidget *parent, QLineEdit *le_stock) 
    : QListWidget(parent)
    , le_stock_(le_stock)
{
#ifdef AUTO_DISPEAR_HINT
    myTimer = new QTimer();
    myTimer->start(2000);
    bool ret = connect(myTimer,SIGNAL(timeout()),this,SLOT(checkSlots()));
#endif

}
 
HintList::~HintList()
{
}

void HintList::ShowUI()
{
    this->show();
#ifdef AUTO_DISPEAR_HINT
        myTimer->stop();
        myTimer->start();
#endif
}

void HintList::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Down)
    {
        this->setCurrentRow(this->currentRow()+1);
#ifdef AUTO_DISPEAR_HINT
        myTimer->stop();
        myTimer->start();
#endif
    }
    else if(event->key() == Qt::Key_Up)
    {
        this->setCurrentRow(this->currentRow()-1);
#ifdef AUTO_DISPEAR_HINT
        myTimer->stop();
        myTimer->start();
#endif
    }
    else if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        if( this->count() > 0 )
            emit choiceStr(this->currentItem()->text()); // trigger ChangeFromStationText
        else
            emit choiceStr(le_stock_->text().trimmed());
        this->close();
    }else if( event->key() == Qt::Key_Backspace )
    {
        le_stock_->setFocus();
    }else if( event->key() == Qt::Key_Escape )
    {
        this->close();
        le_stock_->setFocus();
    }
}

void HintList::checkSlots()
{
#ifdef AUTO_DISPEAR_HINT
        myTimer->stop(); 
#endif
    this->close();
}
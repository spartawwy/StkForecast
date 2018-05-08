#include "tool_bar.h"

#include <cassert>

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMessageBox>
#include <QEvent>
#include <QMouseEvent>
#include <QtWidgets/QApplication>

#include "mainwindow.h"
#include "kline_wall.h"

#define STR_ABPEN  "ABpen"
#define STR_CLRPEN  "CLRpen"

ToolBar::ToolBar(QWidget *parent)
    : QWidget(parent)
    , m_main_window(qobject_cast<MainWindow*>(parent))
    , kline_wall_( *(qobject_cast<MainWindow*>(parent)->kline_wall()) )
    , m_pABPen(nullptr)
    , m_pClearPen(nullptr)
{
    assert( qobject_cast<MainWindow*>(parent)->kline_wall() );
    setFixedHeight(30);

    m_pABPen = new QPushButton("/\\", this);
    m_pABPen->setFixedSize(27, 22);
    m_pABPen->setCheckable(true);
    m_pABPen->setObjectName(STR_ABPEN);  
    connect(m_pABPen, SIGNAL(clicked(bool)), this, SLOT(onClicked()));

    m_pClearPen = new QPushButton("C", this);
    m_pClearPen->setFixedSize(27, 22);
    m_pClearPen->setObjectName(STR_CLRPEN);
    connect(m_pClearPen, SIGNAL(clicked(bool)), this, SLOT(onClicked()));

    QHBoxLayout *pLayout = new QHBoxLayout(this);
    //pLayout->setSpacing(50);
    pLayout->addWidget(m_pABPen);
    pLayout->addSpacing(5);
    pLayout->addWidget(m_pClearPen);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(5, 0, 5, 0);
     
    setLayout(pLayout);
}

void ToolBar::UncheckBtnABPen()
{
    if( m_pABPen )
        m_pABPen->setChecked(false);
}

void ToolBar::mouseDoubleClickEvent(QMouseEvent *)
{

}

 // 进行鼠界面的拖动
void ToolBar::mousePressEvent(QMouseEvent *)
{

}

 // 设置界面标题与图标
bool ToolBar::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::WindowTitleChange:
        event = event;
        break;
    default: break;
    }
    return QWidget::eventFilter(obj, event);
}

void ToolBar::onClicked()
{
    auto p_btn = qobject_cast<QPushButton*>(sender());
    auto p_window = this->window();
    if( p_window->isTopLevel() )
    {
        if( p_btn == m_pABPen )
        {
            if( p_btn->isChecked() )
            {
                kline_wall_.setCursor(Qt::CrossCursor);
                kline_wall_.draw_action(KLineWall::DrawAction::DRAWING_FOR_C);
            }else
            { 
                kline_wall_.setCursor(Qt::ArrowCursor);
                /*if( kline_wall_.draw_action() == KLineWall::DrawAction::DRAWING_FOR_C )
                    kline_wall_.draw_action(KLineWall::DrawAction::NO_ACTION);*/
                kline_wall_.ResetDrawState();// ndchk
            }
        }else if( p_btn == m_pClearPen )
        {
            auto ret = QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("是否删除所有手动预测画线?"), QMessageBox::Yes, QMessageBox::No); 
            if( QMessageBox::Yes == ret )
            {
                kline_wall_.ResetDrawState(); 
                kline_wall_.ClearForcastDrawData();
            }
        }
    }
}
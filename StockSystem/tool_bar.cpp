#include "tool_bar.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QtWidgets/QApplication>

ToolBar::ToolBar(QWidget *parent)
    : QWidget(parent)
    , m_pABPen(nullptr)
    , m_pClearPen(nullptr)
{
    setFixedHeight(30);

    m_pABPen = new QPushButton("/\\", this);
    m_pABPen->setFixedSize(27, 22);
    m_pABPen->setCheckable(true);

    m_pClearPen = new QToolButton(this);
    m_pClearPen->setFixedSize(27, 22);

    QHBoxLayout *pLayout = new QHBoxLayout(this);
    //pLayout->setSpacing(50);
    pLayout->addWidget(m_pABPen);
    pLayout->addSpacing(5);
    pLayout->addWidget(m_pClearPen);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(5, 0, 5, 0);


    setLayout(pLayout);
}

void ToolBar::mouseDoubleClickEvent(QMouseEvent *event)
{

}

 // 进行鼠界面的拖动
void ToolBar::mousePressEvent(QMouseEvent *event)
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

}
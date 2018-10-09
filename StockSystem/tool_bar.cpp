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
#include <QPixmap>

#include "mainwindow.h"
#include "kline_wall.h"

#define STR_AB_DOWN_C_PEN  "AB_DOWN_C_pen"
#define STR_CLRPEN         "CLRpen"

ToolBar::ToolBar(QWidget *parent)
    : QWidget(parent)
    , m_main_window(qobject_cast<MainWindow*>(parent))
    , kline_wall_( *(qobject_cast<MainWindow*>(parent)->kline_wall()) )
    , ab_down_for_c_pen_(nullptr)
    , ab_up_for_c_pen_(nullptr)
    , clear_pen_(nullptr)
{
    assert( qobject_cast<MainWindow*>(parent)->kline_wall() );
    setFixedHeight(30);

    ab_down_for_c_pen_ = new QPushButton("");
    QPixmap icon1(tr("img/ab_down_c.png"));
    ab_down_for_c_pen_->setIcon(icon1);
    ab_down_for_c_pen_->setFixedSize(22, 22);
    ab_down_for_c_pen_->setCheckable(true);
    ab_down_for_c_pen_->setObjectName(STR_AB_DOWN_C_PEN);  
    connect(ab_down_for_c_pen_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
#if 1
    ab_up_for_c_pen_ = new QPushButton("");
    QPixmap icon2(tr("img/ab_up_c.png"));
    ab_up_for_c_pen_->setIcon(icon2);
    ab_up_for_c_pen_->setFixedSize(22, 22);
    ab_up_for_c_pen_->setCheckable(true);
    //ab_up_for_c_pen_->setObjectName(STR_AB_DOWN_C_PEN);  
    connect(ab_up_for_c_pen_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
#endif
    clear_pen_ = new QPushButton("C");
    clear_pen_->setFixedSize(27, 22);
    clear_pen_->setObjectName(STR_CLRPEN);
    connect(clear_pen_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));

    QHBoxLayout *pLayout = new QHBoxLayout(this);
    //pLayout->setSpacing(50);
    pLayout->addWidget(ab_down_for_c_pen_);
    pLayout->addSpacing(1);
    pLayout->addWidget(ab_up_for_c_pen_);
    pLayout->addSpacing(5);
    pLayout->addWidget(clear_pen_);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(5, 0, 5, 0);
     
    setLayout(pLayout);
}

void ToolBar::UncheckBtnABPen()
{
    if( ab_down_for_c_pen_ )
        ab_down_for_c_pen_->setChecked(false);
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
        if( p_btn == ab_down_for_c_pen_ || p_btn == ab_up_for_c_pen_ )
        {
            auto action = p_btn == ab_down_for_c_pen_ ? KLineWall::DrawAction::DRAWING_FOR_2PDOWN_C : KLineWall::DrawAction::DRAWING_FOR_2PUP_C;
            if( p_btn->isChecked() )
            {
                kline_wall_.setCursor(Qt::CrossCursor);
                kline_wall_.draw_action(action);
            }else
            { 
                kline_wall_.setCursor(Qt::ArrowCursor); 
                kline_wall_.ResetDrawState(); 
            }
        }else if( p_btn == clear_pen_ )
        {
            auto ret = QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("是否删除所有手动预测画线?"), QMessageBox::Yes, QMessageBox::No); 
            if( QMessageBox::Yes == ret )
            {
                kline_wall_.ResetDrawState(); 
                kline_wall_.ClearForcastData();
            }
        }
    }
}
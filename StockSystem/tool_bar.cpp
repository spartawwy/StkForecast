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
    , kline_wall_cursor_(Qt::ArrowCursor)
{
    assert( qobject_cast<MainWindow*>(parent)->kline_wall() );
    setFixedHeight(30);

    ab_down_for_c_pen_ = new QPushButton("");
    QPixmap icon1(tr("img/ab_down_c.png"));
    ab_down_for_c_pen_->setIcon(icon1);
    ab_down_for_c_pen_->setFixedSize(22, 22);
    ab_down_for_c_pen_->setCheckable(true);
    ab_down_for_c_pen_->setObjectName(STR_AB_DOWN_C_PEN);  
    bool ret = connect(ab_down_for_c_pen_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
 
    ab_up_for_c_pen_ = new QPushButton("");
    QPixmap icon2(tr("img/ab_up_c.png"));
    ab_up_for_c_pen_->setIcon(icon2);
    ab_up_for_c_pen_->setFixedSize(22, 22);
    ab_up_for_c_pen_->setCheckable(true);
    //ab_up_for_c_pen_->setObjectName(STR_AB_DOWN_C_PEN);  
    ret = connect(ab_up_for_c_pen_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
 
    abc_down_for_d_pen_ = new QPushButton("");
    QPixmap icon3(tr("img/abc_down_d.png"));
    abc_down_for_d_pen_->setIcon(icon3);
    abc_down_for_d_pen_->setFixedSize(21, 21);
    abc_down_for_d_pen_->setCheckable(true);
    ret = connect(abc_down_for_d_pen_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));

    abc_up_for_d_pen_ = new QPushButton("");
    QPixmap icon4(tr("img/abc_up_d.png"));
    abc_up_for_d_pen_->setIcon(icon4);
    abc_up_for_d_pen_->setFixedSize(21, 21);
    abc_up_for_d_pen_->setCheckable(true);
    ret = connect(abc_up_for_d_pen_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));

    clear_pen_ = new QPushButton("C");
    clear_pen_->setFixedSize(27, 22);
    clear_pen_->setObjectName(STR_CLRPEN);
    ret = connect(clear_pen_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));

    QHBoxLayout *pLayout = new QHBoxLayout(this);
    //pLayout->setSpacing(50);
    pLayout->addWidget(ab_down_for_c_pen_);
    pLayout->addSpacing(1);
    pLayout->addWidget(ab_up_for_c_pen_);
    pLayout->addSpacing(1);
    pLayout->addWidget(abc_down_for_d_pen_);
    pLayout->addSpacing(1);
    pLayout->addWidget(abc_up_for_d_pen_);
    pLayout->addSpacing(5);
    pLayout->addWidget(clear_pen_);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(5, 0, 5, 0);
     
    setLayout(pLayout);
}

void ToolBar::UncheckBtnABDownPen()
{
    if( ab_down_for_c_pen_ )
        ab_down_for_c_pen_->setChecked(false);
}

void ToolBar::UncheckBtnABUpPen()
{
    if( ab_up_for_c_pen_ )
        ab_up_for_c_pen_->setChecked(false);
}

void ToolBar::UncheckBtnABCDownPen()
{
    if( abc_down_for_d_pen_ )
        abc_down_for_d_pen_->setChecked(false);
}

void ToolBar::UncheckBtnABCUpPen()
{
    if( abc_up_for_d_pen_ )
        abc_up_for_d_pen_->setChecked(false);
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
        KLineWall::DrawAction action = KLineWall::DrawAction::NO_ACTION;
        if( p_btn == ab_down_for_c_pen_ )
            action = KLineWall::DrawAction::DRAWING_FOR_2PDOWN_C;
        else if( p_btn == ab_up_for_c_pen_ )
            action = KLineWall::DrawAction::DRAWING_FOR_2PUP_C;
        else if( p_btn == abc_down_for_d_pen_ )
            action = KLineWall::DrawAction::DRAWING_FOR_3PDOWN_D;
        else if( p_btn == abc_up_for_d_pen_ )
            action = KLineWall::DrawAction::DRAWING_FOR_3PUP_D;

        if( action != KLineWall::DrawAction::NO_ACTION )
        { 
            if( p_btn->isChecked() )
            {
                kline_wall_cursor_ = kline_wall_.cursor();

                kline_wall_.setCursor(Qt::CrossCursor);
                kline_wall_.draw_action(action);
            }else
            { 
                kline_wall_.setCursor(kline_wall_cursor_); 
                kline_wall_.ResetDrawState(action); 
            }

        }else if( p_btn == clear_pen_ )
        {
            auto ret = QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("是否删除所有手动预测画线?"), QMessageBox::Yes, QMessageBox::No); 
            if( QMessageBox::Yes == ret )
            {
                kline_wall_.ResetDrawState(KLineWall::DrawAction::NO_ACTION); 
                kline_wall_.ClearForcastData();
            }
        }
    }
}
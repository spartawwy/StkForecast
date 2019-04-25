#include "tool_bar.h"

#include <cassert>

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QMessageBox>
#include <QVariant>
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
    , show_structline_btn_(nullptr)
    , show_section_btn_(nullptr)
    , pre_btn_(nullptr)
    , cycle_comb_(nullptr)
    , pre_action_(DrawAction::NO_ACTION)
    , kline_wall_cursor_(Qt::ArrowCursor)
{
    assert( qobject_cast<MainWindow*>(parent)->kline_wall() );
    setFixedHeight(30);

    show_structline_btn_ = new QPushButton(QString::fromLocal8Bit("结构"));
    show_structline_btn_->setFixedSize(54, 22);
    show_structline_btn_->setCheckable(true);

    show_section_btn_ = new QPushButton(QString::fromLocal8Bit("中枢"));
    show_section_btn_->setFixedSize(54, 22);
    show_section_btn_->setCheckable(true);

    ab_down_for_c_pen_ = new QPushButton();
    QPixmap icon1(tr("img/ab_down_c.png"));
    ab_down_for_c_pen_->setIcon(icon1);
    ab_down_for_c_pen_->setFixedSize(22, 22);
    ab_down_for_c_pen_->setCheckable(true);
    ab_down_for_c_pen_->setObjectName(STR_AB_DOWN_C_PEN);  
    
    ab_up_for_c_pen_ = new QPushButton("");
    QPixmap icon2(tr("img/ab_up_c.png"));
    ab_up_for_c_pen_->setIcon(icon2);
    ab_up_for_c_pen_->setFixedSize(22, 22);
    ab_up_for_c_pen_->setCheckable(true);
    //ab_up_for_c_pen_->setObjectName(STR_AB_DOWN_C_PEN);  
     
    abc_down_for_d_pen_ = new QPushButton("");
    QPixmap icon3(tr("img/abc_down_d.png"));
    abc_down_for_d_pen_->setIcon(icon3);
    abc_down_for_d_pen_->setFixedSize(22, 22);
    abc_down_for_d_pen_->setCheckable(true);
   
    abc_up_for_d_pen_ = new QPushButton("");
    QPixmap icon4(tr("img/abc_up_d.png"));
    abc_up_for_d_pen_->setIcon(icon4);
    abc_up_for_d_pen_->setFixedSize(22, 22);
    abc_up_for_d_pen_->setCheckable(true);
   
    clear_pen_ = new QPushButton("C");
    clear_pen_->setFixedSize(27, 22);
    clear_pen_->setObjectName(STR_CLRPEN);

    cycle_comb_ = new QComboBox();
    cycle_comb_->addItem(QString::fromLocal8Bit("5分"), QVariant(int(TypePeriod::PERIOD_5M))); // index 0
    cycle_comb_->addItem(QString::fromLocal8Bit("15分"), QVariant(int(TypePeriod::PERIOD_15M)));
    cycle_comb_->addItem(QString::fromLocal8Bit("30分"), QVariant(int(TypePeriod::PERIOD_30M)));
    cycle_comb_->addItem(QString::fromLocal8Bit("时"), QVariant(int(TypePeriod::PERIOD_HOUR)));
    cycle_comb_->addItem(QString::fromLocal8Bit("日"), QVariant(int(TypePeriod::PERIOD_DAY)));
    cycle_comb_->addItem(QString::fromLocal8Bit("周"), QVariant(int(TypePeriod::PERIOD_WEEK)));
    cycle_comb_->addItem(QString::fromLocal8Bit("月"), QVariant(int(TypePeriod::PERIOD_MON)));
    cycle_comb_->setFixedSize(50, 22);

    ConnectAllDrawNormalBtn();
    bool ret = connect(show_structline_btn_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    ret = connect(show_section_btn_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    ret = connect(clear_pen_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));

    ret = ret;
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    //pLayout->setSpacing(50);
    pLayout->addWidget(show_structline_btn_);
    pLayout->addSpacing(1);
    pLayout->addWidget(show_section_btn_);
    pLayout->addSpacing(5);
    pLayout->addWidget(ab_down_for_c_pen_);
    pLayout->addSpacing(1);
    pLayout->addWidget(ab_up_for_c_pen_);
    pLayout->addSpacing(1);
    pLayout->addWidget(abc_down_for_d_pen_);
    pLayout->addSpacing(1);
    pLayout->addWidget(abc_up_for_d_pen_);
    pLayout->addSpacing(5);
    pLayout->addWidget(clear_pen_);
    pLayout->addSpacing(20);
    pLayout->addWidget(cycle_comb_);
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

void ToolBar::SetCurCycleType(TypePeriod type_period)
{
    cycle_comb_->setCurrentIndex(int(type_period) - 1);
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

    if( p_btn == show_structline_btn_ )
    {
        kline_wall_.SetShowStructLine(p_btn->isChecked());
        return;
    }
    if( p_btn == show_section_btn_ )
    {
        kline_wall_.SetShowSection(p_btn->isChecked());
        return;
    }

    if( pre_btn_ && p_btn != pre_btn_ )
    {
        kline_wall_.setCursor(kline_wall_cursor_); 
        DisConnectAllDrawNormalBtn();
        kline_wall_.ResetDrawState(pre_action_); 
        pre_btn_->setChecked(false);
        ConnectAllDrawNormalBtn();
    }
    pre_btn_ = p_btn;

    auto p_window = this->window();
    if( p_window->isTopLevel() )
    {
        if( p_btn == clear_pen_ )
        {
            auto ret = QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("是否删除所有手动预测画线?"), QMessageBox::Yes, QMessageBox::No); 
            if( QMessageBox::Yes == ret )
            {
                kline_wall_.ResetDrawState(DrawAction::NO_ACTION); 
                kline_wall_.ClearForcastData();
            }
            return;
        }

        DrawAction action = DrawAction::NO_ACTION;
        if( p_btn->isChecked() )
        {
            if( p_btn == ab_down_for_c_pen_ )
                action = DrawAction::DRAWING_FOR_2PDOWN_C;
            else if( p_btn == ab_up_for_c_pen_ )
                action = DrawAction::DRAWING_FOR_2PUP_C;
            else if( p_btn == abc_down_for_d_pen_ )
                action = DrawAction::DRAWING_FOR_3PDOWN_D;
            else if( p_btn == abc_up_for_d_pen_ )
                action = DrawAction::DRAWING_FOR_3PUP_D;
        }
        pre_action_ = action;
        if( action == DrawAction::NO_ACTION )
        { 
            kline_wall_.setCursor(kline_wall_cursor_); 
            kline_wall_.ResetDrawState(DrawAction::NO_ACTION); 
        }else{ 
            kline_wall_cursor_ = kline_wall_.cursor();

            kline_wall_.setCursor(Qt::CrossCursor);
            kline_wall_.draw_action(action);
        }  
    }
}

void ToolBar::ConnectAllDrawNormalBtn()
{
    bool ret = connect(ab_down_for_c_pen_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    ret = connect(ab_up_for_c_pen_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    ret = connect(abc_down_for_d_pen_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    ret = connect(abc_up_for_d_pen_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
}
void ToolBar::DisConnectAllDrawNormalBtn()
{
    bool ret = disconnect(ab_down_for_c_pen_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    ret = disconnect(ab_up_for_c_pen_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    ret = disconnect(abc_down_for_d_pen_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    ret = disconnect(abc_up_for_d_pen_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
}
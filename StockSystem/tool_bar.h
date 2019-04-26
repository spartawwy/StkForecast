#ifndef  TOOL_BAR_SFDDS32SDFDS_H_
#define  TOOL_BAR_SFDDS32SDFDS_H_


#include <QtWidgets/QWidget>
#include "stkfo_common.h"

class QLabel;
class QPushButton;
class QComboBox;
class QToolButton;
class MainWindow;
class KLineWall;

class ToolBar : public QWidget
{
    Q_OBJECT

public:

    explicit ToolBar(QWidget *parent = 0);
    ~ToolBar(){}

    void UncheckBtnABDownPen();
    void UncheckBtnABUpPen();
    void UncheckBtnABCDownPen();
    void UncheckBtnABCUpPen();

    QComboBox * cycle_comb(){ return cycle_comb_;}
    void SetCurCycleType(TypePeriod type_period);

protected:

    // 双击标题栏进行界面的最大化/还原
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override; 

    // 进行鼠界面的拖动
    virtual void mousePressEvent(QMouseEvent *event) override;

    // 设置界面标题与图标
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
     
private slots:
     
    void onClicked();

private:
     
    void ConnectAllDrawNormalBtn();
    void DisConnectAllDrawNormalBtn();

private:

    MainWindow *m_main_window;
    //KLineWall *kline_wall_;

    //QLabel *m_pIconLabel;
    //QLabel *m_pTitleLabel;

    QPushButton *ab_down_for_c_pen_;
    QPushButton *ab_up_for_c_pen_;
    QPushButton *abc_down_for_d_pen_;
    QPushButton *abc_up_for_d_pen_;
    QPushButton *clear_pen_;

    QPushButton *show_structline_btn_;
    QPushButton *show_section_btn_;

    QPushButton *pre_btn_; // for temp save pointer
    QComboBox   *cycle_comb_;
    DrawAction   pre_action_;
    //QPushButton *m_pMaximizeButton;
    //QPushButton *m_pCloseButton;

    QCursor kline_wall_cursor_;
};

#endif // TOOL_BAR_SFDDS32SDFDS_H_
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

    QComboBox * main_cycle_comb(){ return main_cycle_comb_;}
    QComboBox * sub_cycle_comb(){ return sub_cycle_comb_;}
    void SetMainKwallCurCycleType(TypePeriod type_period);
    void SetSubKwallCurCycleType(TypePeriod type_period);

    void SetShowSubKwallBtn(bool checked);

protected:

    // 双击标题栏进行界面的最大化/还原
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override; 

    // 进行鼠界面的拖动
    virtual void mousePressEvent(QMouseEvent *event) override;

    // 设置界面标题与图标
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
     
private slots:
     
    void onClicked();
    void onClickedClearBtn();
    void onClickedStructBtn();
    void onClickedShowSubKwallBtn();
    void onShowTrainModelWin();

private:
     
    void ConnectAllDrawNormalBtn();
    void DisConnectAllDrawNormalBtn();

private:

    MainWindow *m_main_window;

    QPushButton *ab_down_for_c_pen_;
    QPushButton *ab_up_for_c_pen_;
    QPushButton *abc_down_for_d_pen_;
    QPushButton *abc_up_for_d_pen_;
    QPushButton *clear_pen_;
    QPushButton *pre_btn_; // for temp save pointer

    QPushButton *main_show_structline_btn_;
    QPushButton *sub_show_structline_btn_;
    QPushButton *main_show_section_btn_;
    QPushButton *sub_show_section_btn_;

    QPushButton *show_sub_kwall_btn_;

    QComboBox   *main_cycle_comb_;
    QComboBox   *sub_cycle_comb_;

    DrawAction   pre_action_;
     
    QCursor kline_wall_cursor_;

    friend class TrainDlg;
};

#endif // TOOL_BAR_SFDDS32SDFDS_H_
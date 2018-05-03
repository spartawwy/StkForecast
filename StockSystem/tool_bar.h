#ifndef  TOOL_BAR_SFDDS32SDFDS_H_
#define  TOOL_BAR_SFDDS32SDFDS_H_


#include <QtWidgets/QWidget>

class QLabel;
class QPushButton;
class QToolButton;
class MainWindow;
class KLineWall;

class ToolBar : public QWidget
{
    Q_OBJECT

public:

    explicit ToolBar(QWidget *parent = 0);
    ~ToolBar(){}

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
     
    //void updateMaximize();

private:

    MainWindow *m_main_window;
    KLineWall &kline_wall_;

    //QLabel *m_pIconLabel;
    //QLabel *m_pTitleLabel;

    QPushButton *m_pABPen;
    QToolButton *m_pClearPen;

    //QPushButton *m_pMaximizeButton;
    //QPushButton *m_pCloseButton;
};

#endif // TOOL_BAR_SFDDS32SDFDS_H_
#ifndef K_LINE_WALL_SDF32DSF_
#define K_LINE_WALL_SDF32DSF_

#include <QtWidgets/QWidget>

#include "ui_klinewall.h"

#include "stockalldaysinfo.h"
#include "stockinput_dlg.h"

class KLineWall : public QWidget
{
public:
    enum class DrawAction : unsigned char { DRAWING_FOR_C = 1, DRAWING_FOR_D, NO_ACTION = 255};

    KLineWall(QWidget *parent);
	~KLineWall() { }
	 
	void StockInputDlgRet();

    //void SetCursorShape(Qt::CursorShape& cursor_shapre);
    void draw_action(DrawAction action) {  draw_action_ = action; }
    DrawAction draw_action(){ return draw_action_; }

    void ResetDrawingPoint();

protected:

    void paintEvent(QPaintEvent*) override;
	void mousePressEvent( QMouseEvent * event ) override;
    void mouseDoubleClickEvent(QMouseEvent * e)  override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

    void enterEvent(QEvent *e) override;
    void leaveEvent(QEvent *e) override;

private slots:

	void ResetStock(const QString& stock);

	/*float HisDateItem_GetMinPrice();
    float HisDateItem_GetMaxPrice();
    float HisDateItem_GetOpenPrice();
    float HisDateItem_GetClosePrice();*/

private:

	Ui_KLineWallForm  ui;

	StockInputDlg  stock_input_dlg_;

    std::list<StockDayInfo>  stk_days_infos_;
     
    StockAllDaysInfo stockAllDaysInfo_;  //一支股票的所有天数的消息
	T_HisDataItemContainer *p_hisdata_container_;

    std::string stock_code_;
    

    float lowestMinPrice;
    float highestMaxPrice;
    bool  show_cross_line_;
    bool  is_repaint_k_;

    int  k_num_;
    std::string k_cycle_tag_;
    int  k_cycle_year_;
    int  date_;
    std::string k_data_str_;

	std::string cur_stock_code_;

    DrawAction draw_action_;
    QPoint drawing_line_A_;
    QPoint drawing_line_B_;
    QPoint drawing_line_C_;

    QPoint cur_mouse_point_;
};


#endif
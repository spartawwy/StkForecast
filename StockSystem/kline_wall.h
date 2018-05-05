#ifndef K_LINE_WALL_SDF32DSF_
#define K_LINE_WALL_SDF32DSF_

#include <QtWidgets/QWidget>

#include "ui_klinewall.h"

#include "stockalldaysinfo.h"
#include "stockinput_dlg.h"
 
class T_PaintData3pForcastDownword
{
public:
    T_PaintData3pForcastDownword() : date_a(0), date_b(0), c1(0.0), c2(0.0), c3(0.0) {}
    T_PaintData3pForcastDownword(const T_PaintData3pForcastDownword & lh) : 
        date_a(lh.date_a), date_b(lh.date_b), c1(lh.c1), c2(lh.c2), c3(lh.c3) {}
    T_PaintData3pForcastDownword(T_PaintData3pForcastDownword && lh) : 
        date_a(lh.date_a), date_b(lh.date_b), c1(lh.c1), c2(lh.c2), c3(lh.c3) {}
    int date_a;
    int date_b;
    double c1;
    double c2;
    double c3;
    

};

class MainWindow;
class KLineWall : public QWidget
{
public:
    enum class DrawAction : unsigned char { DRAWING_FOR_C = 1, DRAWING_FOR_D, NO_ACTION = 255};

    KLineWall(QWidget *parent);
	~KLineWall() { }
	 
    bool Init();
	void StockInputDlgRet();

    //void SetCursorShape(Qt::CursorShape& cursor_shapre);
    void draw_action(DrawAction action) {  draw_action_ = action; }
    DrawAction draw_action(){ return draw_action_; }

    void ResetDrawState();

protected:

    void paintEvent(QPaintEvent*) override;
	void mousePressEvent( QMouseEvent * event ) override;
    void mouseDoubleClickEvent(QMouseEvent * e)  override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

    void enterEvent(QEvent *e) override;
    void leaveEvent(QEvent *e) override;

private slots:

	bool ResetStock(const QString& stock);

	/*float HisDateItem_GetMinPrice();
    float HisDateItem_GetMaxPrice();
    float HisDateItem_GetOpenPrice();
    float HisDateItem_GetClosePrice();*/

private:

    void UpdateKLinePosDatas();
    T_KlineDataItem * GetKLineDataItemByXpos(int x);
    QPoint GetPointFromKLineDataItems(int x, bool is_get_top);
    T_KlineDataItem * GetKLineDataItemByDate(int date);

    MainWindow  *main_win_;
	Ui_KLineWallForm  ui;
    const int head_h_;
    const int bottom1_h_;
    const int bottom2_h_;
    int bottom_h_;

	StockInputDlg  stock_input_dlg_;

    //std::list<StockDayInfo>  stk_days_infos_;
     
    std::string stock_code_;
    StockAllDaysInfo stockAllDaysInfo_;           //many stocks many daysinfo 
	T_HisDataItemContainer *p_hisdata_container_; //point to stockAllDaysInfo_'s one stock's data
    //std::vector<T_KlinePosData> kline_pos_data_;

    float lowestMinPrice_;
    float highestMaxPrice_;
    bool  show_cross_line_;
    //bool  is_repaint_k_;

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

    int pre_mm_w_;
    int pre_mm_h_;

    std::vector<T_PaintData3pForcastDownword> paint_3pdatas_;
};


#endif
#ifndef K_LINE_WALL_SDF32DSF_
#define K_LINE_WALL_SDF32DSF_

#include <QtWidgets/QWidget>

#include "ui_klinewall.h"

#include "stockalldaysinfo.h"
#include "stockinput_dlg.h"
 
#include "forcast_man.h"

//#define DRAW_FROM_LEFT
#ifndef DRAW_FROM_LEFT 
#define DRAW_FROM_RIGHT
#endif


class StkForecastApp;
class MainWindow;
class DataBase;
class StockMan;
class KLineWall : public QWidget
{
public:
     
    KLineWall(StkForecastApp *app, QWidget *parent);
	~KLineWall() { }
	 
    bool Init();
	void StockInputDlgRet();

    //void SetCursorShape(Qt::CursorShape& cursor_shapre);
    void draw_action(DrawAction action) {  draw_action_ = action; }
    DrawAction draw_action(){ return draw_action_; }

    void ResetDrawState(DrawAction draw_action);
    void ClearForcastData();

    PeriodType ToPeriodType(TypePeriod src);

protected:

    void paintEvent(QPaintEvent*) override;
	void mousePressEvent( QMouseEvent * event ) override;
    void mouseDoubleClickEvent(QMouseEvent * e)  override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

    void enterEvent(QEvent *e) override;
    void leaveEvent(QEvent *e) override;

private slots:

	bool ResetStock(const QString& stock, bool is_index=false);

	/*float HisDateItem_GetMinPrice();
    float HisDateItem_GetMaxPrice();
    float HisDateItem_GetOpenPrice();
    float HisDateItem_GetClosePrice();*/

private: 

    int height_axis_trans_in_paint_k(){ return this->height() - bottom_h_; }

    void Draw2pDownForcast(QPainter &, const int mm_h, double item_w);
    void Draw2pUpForcast(QPainter &, const int mm_h, double item_w);

    void Draw3pDownForcast(QPainter &, const int mm_h, double item_w);
    void Draw3pUpForcast(QPainter &, const int mm_h, double item_w);

    void UpdatePosDatas();
    //void UpdateDrawingLinePos();

    T_KlineDataItem * GetKLineDataItemByXpos(int x);
    QPointF GetPointFromKLineDataItems(int x, bool is_get_top);
    T_KlineDataItem * GetKLineDataItemByDate(int date);
    T_KlinePosData * GetKLinePosDataByDate(int date);

    double get_pointc_y(double c_val, int mm_h)
    {  
        return -1 * (c_val - lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_) * mm_h;
    }

    StkForecastApp *app_;
    MainWindow  *main_win_;
	Ui_KLineWallForm  ui;
    const int head_h_;
    const int bottom1_h_;
    const int bottom2_h_;
    int bottom_h_;
     
	StockInputDlg  stock_input_dlg_;
     
    std::string stock_code_;
    StockAllDaysInfo stockAllDaysInfo_;           //many stocks many daysinfo 
	T_HisDataItemContainer *p_hisdata_container_; //point to stockAllDaysInfo_'s one stock's data

    int container_start_date_day_k_; // nddel
    int container_end_date_day_k_;

    float lowestMinPrice_;
    float highestMaxPrice_;
    bool  show_cross_line_;
    //bool  is_repaint_k_;

    int  k_num_;
    TypePeriod  k_type_;
    std::string  k_cycle_tag_;
    
    int  k_cycle_year_;
    int  date_;
    std::string k_data_str_;

	//std::string cur_stock_code_;

    DrawAction draw_action_;
#if 0
    QPoint drawing_line_A_;
    QPoint drawing_line_B_;
    QPoint drawing_line_C_;

    QPoint cur_mouse_point_;
#else
    QPointF drawing_line_A_;
    QPointF drawing_line_B_;
    QPointF drawing_line_C_;

    QPointF cur_mouse_point_;
#endif
    int pre_mm_w_;
    int pre_mm_h_;
     
    ForcastMan  forcast_man_;
    std::vector<T_Data2pDownForcast> data_2pforcast_downs_;
};


#endif
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
    enum class DrawAction : unsigned char { DRAWING_FOR_2PDOWN_C = 1, DRAWING_FOR_2PUP_C, DRAWING_FOR_3PUP_D, NO_ACTION = 255};

    KLineWall(StkForecastApp *app, QWidget *parent);
	~KLineWall() { }
	 
    bool Init();
	void StockInputDlgRet();

    //void SetCursorShape(Qt::CursorShape& cursor_shapre);
    void draw_action(DrawAction action) {  draw_action_ = action; }
    DrawAction draw_action(){ return draw_action_; }

    void ResetDrawState();
    void ClearForcastDrawData();

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

    void Draw2pforcast(QPainter &, const int mm_h, double item_w);
    void UpdateKLinePosDatas();
    T_KlineDataItem * GetKLineDataItemByXpos(int x);
    QPointF GetPointFromKLineDataItems(int x, bool is_get_top);
    T_KlineDataItem * GetKLineDataItemByDate(int date);
    T_KlinePosData * GetKLinePosDataByDate(int date);

    StkForecastApp *app_;
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
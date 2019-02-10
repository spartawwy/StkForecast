#ifndef K_LINE_WALL_SDF32DSF_
#define K_LINE_WALL_SDF32DSF_

#include <QtWidgets/QWidget>

#include "ui_klinewall.h"

#include "stock_data_man.h"
#include "stockinput_dlg.h"
 
#include "forcast_man.h"

#include "zhibiao_window.h"

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

    void RestTypePeriod(TypePeriod  type);

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

	bool ResetStock(const QString& stock, TypePeriod type_period, bool is_index=false);

private: 
    
    int height_axis_trans_in_paint_k();

    void Draw2pDownForcast(QPainter &, const int mm_h, double item_w);
    void Draw2pUpForcast(QPainter &, const int mm_h, double item_w);

    void Draw3pDownForcast(QPainter &, const int mm_h, double item_w);
    void Draw3pUpForcast(QPainter &, const int mm_h, double item_w);
    void _Draw3pForcast(QPainter &, const int mm_h, double item_w, bool is_down_forward);

    void UpdatePosDatas();

    T_KlineDataItem * GetKLineDataItemByXpos(int x);
    QPointF GetPointFromKLineDataItems(int x, bool is_get_top);
    T_KlineDataItem * GetKLineDataItemByDate(int date);
    T_KlinePosData * GetKLinePosDataByDate(int date);

    double get_pointc_y(double c_val, int mm_h)
    {  
        return -1 * (c_val - lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_) * mm_h;
    }

    void SetLowestMinPrice(float val) { lowestMinPrice_ = val;}
    double GetLowestMinPrice() { return lowestMinPrice_; }
    void SetHighestMaxPrice(float val) { highestMaxPrice_ = val;}
    double GetHighestMaxPrice() { return highestMaxPrice_; }

    bool FindTopFractalItem_TowardLeft(T_HisDataItemContainer &his_data, T_HisDataItemContainer::reverse_iterator iter, int k_index, T_KlinePosData *&left_pos_data);

    int Calculate_mm_h();

    StkForecastApp *app_;
    MainWindow  *main_win_;
	Ui_KLineWallForm  ui;
    const int head_h_;
    const int bottom1_h_;
    const int bottom2_h_; 
    int bottom_h_;
     
	StockInputDlg  stock_input_dlg_;
     
    std::string    stock_code_;
    bool           is_index_;
    StockDataMan   stock_data_man_;              //many stocks many daysinfo 
	T_HisDataItemContainer *p_hisdata_container_; //point to stock_data_man_'s one stock's data
       
    
    double lowestMinPrice_;
    double highestMaxPrice_;
    bool  show_cross_line_;

    int  k_num_;
    TypePeriod  k_type_;
    std::string  k_cycle_tag_;
    
    int  k_cycle_year_;
    int  date_;
    std::string k_data_str_;

    DrawAction draw_action_;
 
    QPointF drawing_line_A_;
    QPointF drawing_line_B_;
    QPointF drawing_line_C_;

    QPointF cur_mouse_point_;

    int pre_mm_w_;
    int pre_mm_h_;
     
    ForcastMan  forcast_man_;
    std::vector<T_Data2pDownForcast> data_2pforcast_downs_;

    std::vector<std::shared_ptr<ZhibiaoWindow> > zb_windows_;
};


#endif
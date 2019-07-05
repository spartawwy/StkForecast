#ifndef K_LINE_WALL_SDF32DSF_
#define K_LINE_WALL_SDF32DSF_

#include <QtWidgets/QWidget>

#include "ui_klinewall.h"

#include "stock_data_man.h"
#ifdef STK_INPUT_KWALL
#include "stockinput_dlg.h"
#endif
#include "statistic_dlg.h"

#include "forcast_man.h"

#include "zhibiao_window.h"

//#define DRAW_FROM_LEFT
#ifndef DRAW_FROM_LEFT 
#define DRAW_FROM_RIGHT
#endif

#define  WOKRPLACE_DEFUALT_K_NUM  (4*20 + 100)
#define  DEFAULT_CYCLE_TAG  "»’œﬂ"

typedef std::tuple<QPointF, std::string> T_TuplePointStr;

class StkForecastApp;
class MainWindow;
class DataBase;
class StockMan;
class KLineWall : public QWidget
{
    Q_OBJECT

public:
     
    static const double cst_k_mm_enlarge_times; 
    static const double cst_k_mm_narrow_times; 

    KLineWall(StkForecastApp *app, QWidget *parent, int index, TypePeriod k_type);
	~KLineWall() { }
	 
    bool Init(); 

    //void SetCursorShape(Qt::CursorShape& cursor_shapre);
    void draw_action(DrawAction action) {  draw_action_ = action; }
    DrawAction draw_action(){ return draw_action_; }

    void ResetDrawState(DrawAction draw_action);
    void ClearForcastData();
    void SetShowStructLine(bool val);
    void SetShowSection(bool val);

    void RestTypePeriod(TypePeriod  type);

    PeriodType ToPeriodType(TypePeriod src);
    
    bool ResetStock(const QString& code, TypePeriod type_period, bool is_index);
    bool ResetStock(const QString& code, const QString& code_name, TypePeriod type_period, bool is_index);
    bool ResetStock(const QString& code, const QString& code_name, bool is_index)
    {
        return ResetStock(code, code_name, k_type_, is_index);
    }

    double GetCurWinKLargetstVol();

    int HeadHeight() { return int(height() * head_h_percent_); }
    int BottomHeight() { return int(height() * bottom_h_percent_); }
     
    void DoIfForcastLineNearbyCursor(QMouseEvent &e);

    void ShowDurationKlines(int date);

    void UpdateIfNecessary();
    // train mode --------
    void SetTrainStartDate(int date);
    void MoveRightEndToNextDayK();
    void MoveRightEndToPreDayK();
    const T_StockHisDataItem & CurTrainStockDataItem();
    void right_clicked_k_date(int val) { right_clicked_k_date_ = val; }
    int k_cur_train_date() { return k_cur_train_date_; }

    void Set_Cursor(Qt::CursorShape sp);

    void Emit_UpdateKwall() { emit sigUpdateKwall(); }

signals:
    void sigUpdateKwall();

public slots:
    void slotOpenRelatedSubKwall(bool);
    void slotUpdateKwall();

protected:

    void paintEvent(QPaintEvent*) override;
	void mousePressEvent( QMouseEvent * event ) override;
    void mouseReleaseEvent(QMouseEvent * e) override;

    void mouseDoubleClickEvent(QMouseEvent * e)  override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

    void enterEvent(QEvent *e) override;
    void leaveEvent(QEvent *e) override;

private slots:

    void slotOpenStatisticDlg(bool);
    void slotZoominSelect(bool);

    //void slotTbvTasksContextMenu(QPoint);
     
private: 
      
    bool Reset_Stock(const QString& stock, TypePeriod type_period, bool is_index, int oldest_date);
    void AppendData();
    void AppendPreData(int date);
    

    void Draw2pDownForcast(QPainter &, const int mm_h, double item_w);
    void Draw2pUpForcast(QPainter &, const int mm_h, double item_w);

    void Draw3pDownForcast(QPainter &, const int mm_h, double item_w);
    void Draw3pUpForcast(QPainter &, const int mm_h, double item_w);
    void _Draw3pForcast(QPainter &, const int mm_h, double item_w, bool is_down_forward);

    void DrawBi(QPainter &, const int mm_h);
    void DrawStructLine(QPainter &painter, const int mm_h);
    void DrawSection(QPainter &painter, const int mm_h);

    void UpdatePosDatas();
    void UpdateKwallMinMaxPrice();

    T_KlineDataItem * GetKLineDataItemByXpos(int x);
    QPointF GetPointFromKLineDataItems(int x, bool is_get_top);
    T_KlineDataItem * GetKLineDataItemByDate(int date, int hhmm);
    T_KlinePosData * GetKLinePosDataByDate(int date, int hhmm);

    double get_price_y(double price, int mm_h)
    {  
        return -1 * (price - lowestMinPrice_)/(highestMaxPrice_ - lowestMinPrice_) * mm_h;
    }

    void SetLowestMinPrice(float val) { lowestMinPrice_ = val;}
    double GetLowestMinPrice() { return lowestMinPrice_; }
    void SetHighestMaxPrice(float val) { highestMaxPrice_ = val;}
    double GetHighestMaxPrice() { return highestMaxPrice_; }

    bool GetContainerMaxMinPrice(PeriodType period_type, const std::string& code, int k_num, std::tuple<float, float>& ret, std::tuple<int, int, int, int> &date_times);

    int FindTopItem_TowardLeft(T_HisDataItemContainer &his_data, T_HisDataItemContainer::reverse_iterator iter, int k_index, T_KlinePosData *&left_pos_data);
    int FindTopFakeItem_TowardLeft(T_HisDataItemContainer &his_data, T_HisDataItemContainer::reverse_iterator iter, int k_index, T_KlinePosData *&left_pos_data);
    int FindBtmItem_TowardLeft(T_HisDataItemContainer &his_data, T_HisDataItemContainer::reverse_iterator iter, int k_index, T_KlinePosData *&left_pos_data);
    int FindBtmFakeItem_TowardLeft(T_HisDataItemContainer &his_data, T_HisDataItemContainer::reverse_iterator iter, int k_index, T_KlinePosData *&left_pos_data);

    int Calculate_k_mm_h();


    StkForecastApp *app_;
    MainWindow  *main_win_;
	Ui_KLineWallForm  ui;
    const int wall_index_;

    const double head_h_percent_;
    const double bottom_h_percent_;
     
    int empty_right_w_;
    int right_w_;
    
    int pre_mm_w_;
    int pre_mm_h_;
    int h_axis_trans_in_paint_k_;
#ifdef STK_INPUT_KWALL
	StockInputDlg  stock_input_dlg_;
#endif
    std::string    stock_code_;
    std::string    stock_name_;
    bool           is_index_;
   
	T_HisDataItemContainer *p_hisdata_container_; //point to stock_data_man_'s a stock's data
    
    double lowestMinPrice_;
    double highestMaxPrice_;
    int lowest_price_date_;
    int lowest_price_hhmm_;
    int highest_price_date_;
    int highest_price_hhmm_;

    bool  show_cross_line_;

    int  k_num_;
    int  k_rend_index_;
    int  pre_k_rend_index_;
    int  k_move_temp_index_;

    int  k_rend_index_for_train_;
    int  k_cur_train_date_;

    TypePeriod  k_type_;
    std::string  k_cycle_tag_;
    
    int  k_cycle_year_;
    int  date_;
    std::string k_date_time_str_;

    volatile DrawAction draw_action_;
 
    QPointF drawing_line_A_;
    QPointF drawing_line_B_;
    QPointF drawing_line_C_;

    QPointF cur_mouse_point_;

    bool mm_move_flag_;
    QPoint move_start_point_;
     
    bool area_select_flag_;
    QPointF area_sel_mouse_release_point_;

    QMenu * k_wall_menu_;
    QMenu * k_wall_menu_sub_;
    ForcastMan  forcast_man_;
    T_DataForcast *cur_select_forcast_;

    std::vector<std::shared_ptr<ZhibiaoWindow> > zb_windows_;

    bool is_draw_bi_;
    bool is_draw_struct_line_;
    bool is_draw_section_;

    int  right_clicked_k_date_; // right mouse click
     
    StatisticDlg  statistic_dlg_;
     

    friend class ZhibiaoWindow;
    friend class VolZhibiaoWin;
    friend class MomentumZhibiaoWin;
    friend class TrainDlg;
};

int CalculateSpanDays(TypePeriod type_period, int k_count);
// ret: <date, hhmm>
std::tuple<int, int> GetKDataTargetDateTime(ExchangeCalendar &exch_calender, TypePeriod type_period, int end_date, int tmp_hhmm, int max_k_count);
// ret: hhmm
int GetTargetTime(TypePeriod type_period, int tmp_hhmm);
// ret: hhmm
int GetKDataTargetTime(TypePeriod type_period);

int FindKRendIndex(T_HisDataItemContainer *p_hisdata_container, int date_val);

#endif // K_LINE_WALL_SDF32DSF_
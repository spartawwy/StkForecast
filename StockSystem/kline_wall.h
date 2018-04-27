#ifndef K_LINE_WALL_SDF32DSF_
#define K_LINE_WALL_SDF32DSF_

#include <QtWidgets/QWidget>

#include "ui_klinewall.h"

#include "stockalldaysinfo.h"
#include "stockinput_dlg.h"

class KLineWall : public QWidget
{
public:

    KLineWall();
	~KLineWall() { }
	 
	void StockInputDlgRet();

protected:

    void paintEvent(QPaintEvent*) override;
	void mousePressEvent( QMouseEvent * event ) override;
    void mouseDoubleClickEvent(QMouseEvent * e)  override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

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
};


#endif
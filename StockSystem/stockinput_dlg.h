#ifndef STOCK_INPUT_DLG_SDF3SDFS_
#define STOCK_INPUT_DLG_SDF3SDFS_

#include <QtWidgets/QWidget>

#include "ui_stockinputdlg.h"
 
class KLineWall;

class StockInputDlg : public QWidget
{
public:

    StockInputDlg(KLineWall *parent);

    Ui_StockInputDlgForm  ui;

protected:

   /* void paintEvent(QPaintEvent*) override;
    void mouseDoubleClickEvent(QMouseEvent * e)  override;
    void mouseMoveEvent(QMouseEvent *e) override;*/
    void keyPressEvent(QKeyEvent *e) override;

private slots:

	//void ResetStock(const QString& stock);

	/*float HisDateItem_GetMinPrice();
    float HisDateItem_GetMaxPrice();
    float HisDateItem_GetOpenPrice();
    float HisDateItem_GetClosePrice();*/

private:
	  
	KLineWall *parent_;
	//std::string cur_stock_code_;
};


#endif
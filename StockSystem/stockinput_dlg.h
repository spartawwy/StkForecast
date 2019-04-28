#ifndef STOCK_INPUT_DLG_SDF3SDFS_
#define STOCK_INPUT_DLG_SDF3SDFS_

#include <memory>
#include <QtWidgets/QWidget>

#include "ui_stockinputdlg.h"
 
class KLineWall;
class MainWindow;
class HintList;
class DataBase;
class StockInputDlg : public QWidget
{
    Q_OBJECT

public:

    //StockInputDlg(KLineWall *parent, std::shared_ptr<DataBase> &db);
    StockInputDlg(MainWindow *parent, std::shared_ptr<DataBase> &db);

    Ui_StockInputDlgForm  ui;

protected:

   /* void paintEvent(QPaintEvent*) override;
    void mouseDoubleClickEvent(QMouseEvent * e)  override;
    void mouseMoveEvent(QMouseEvent *e) override;*/
    void keyPressEvent(QKeyEvent *e) override;

private slots:

    void FlushFromStationListWidget(QString str);
    void OnClickedListWidget(QModelIndex index);
	void ChangeFromStationText(QString text);

private:
	  
	//KLineWall *parent_;
	MainWindow *parent_;
    
    std::shared_ptr<DataBase> &db_;
	//std::string cur_stock_code_;
    HintList *m_list_hint_;
};


#endif
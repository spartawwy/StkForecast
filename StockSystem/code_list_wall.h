#ifndef CODE_LIST_WALL_SDF23DFSD_H_
#define CODE_LIST_WALL_SDF23DFSD_H_

#include <QtWidgets/QWidget>

#include "ui_codelistwall.h"

class StkForecastApp;
//class MainWindow;
//class DataBase;
//class StockMan;
class CodeListWall : public QWidget
{
public:
     
    CodeListWall(StkForecastApp *app, QWidget *parent);
	~CodeListWall() { }

    bool Init();

private:

    void keyPressEvent(QKeyEvent *e) override;

private:

    StkForecastApp *app_;

    Ui_CodeListWallForm  ui;
};

#endif // CODE_LIST_WALL_SDF23DFSD_H_
#ifndef CODE_LIST_WALL_SDF23DFSD_H_
#define CODE_LIST_WALL_SDF23DFSD_H_

#include <memory>

#include <QtWidgets/QWidget>

#include "ui_codelistwall.h"

class StkForecastApp;
//class MainWindow;
//class DataBase;
//class StockMan;
class FavCodesSerial;
class CodeListWall : public QWidget
{
    Q_OBJECT

public:
     
    CodeListWall(StkForecastApp *app, QWidget *parent);
	~CodeListWall() { }

    bool Init();
    void AddCode(const QString &code, const QString &name, bool is_index);

private:

    void keyPressEvent(QKeyEvent *e) override;

private slots:

    void slotRowDoubleClicked(const QModelIndex &);

private:

    StkForecastApp *app_;

    Ui_CodeListWallForm  ui;
    std::shared_ptr<FavCodesSerial> serial_;
};

#endif // CODE_LIST_WALL_SDF23DFSD_H_
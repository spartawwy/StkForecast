#ifndef MINUTE_TETAIL_WALL_H_SDFASDDFDS2343D
#define MINUTE_TETAIL_WALL_H_SDFASDDFDS2343D

#include <QtWidgets/QWidget>

class StkForecastApp;
class MinuteDetailWall : public QWidget
{
    Q_OBJECT

public:
     
     MinuteDetailWall(StkForecastApp *app, QWidget *parent);
	~MinuteDetailWall() { }
	 
    bool Init(); 

    int Calculate_k_mm_h();

protected:

    void paintEvent(QPaintEvent*) override;

    int HeadHeight() { return int(height() * head_h_percent_); }
    int BottomHeight() { return int(height() * bottom_h_percent_); }

private:

    StkForecastApp *app_;

    int pre_mm_w_;
    int pre_mm_h_;

    const double head_h_percent_;
    const double bottom_h_percent_;

    QPointF cur_mouse_point_;

    QString stock_code_;
    QString stock_name_;

    int right_w_;
};
#endif
#include "algotool.h"

#include <boost/lexical_cast.hpp>

#include <string>
#include <tuple>

#include <QMessageBox>

std::tuple<double, double, double>  ForcastC_ABDown(double a, double b);
std::tuple<double, double, double>  ForcastC_ABUp(double a, double b);
// ps: make sure a > b > 0
std::tuple<double, double, double>  ForcastD_ABC_Down(double A, double B);

// ps: make sure 0 < a < b
std::tuple<double, double, double>  ForcastD_ABC_Up(double A, double B);


AlgoTool::AlgoTool(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    bool ret = QObject::connect(ui.pb_2p_forcast, SIGNAL(clicked(bool)), this, SLOT(on2pForcast()));
    ret = QObject::connect(ui.pb_3p_forcast, SIGNAL(clicked(bool)), this, SLOT(on3pForcast()));

    ret = ret;
}

AlgoTool::~AlgoTool()
{

}

void AlgoTool::on2pForcast()
{
    ui.lb_c1->clear();
    ui.lb_c2->clear();
    ui.lb_c3->clear();
    std::string str_a = ui.le_2p_a->text().trimmed().toLocal8Bit().data();
    std::string str_b = ui.le_2p_b->text().trimmed().toLocal8Bit().data();

    double value_a = 0.0;
    double value_b = 0.0;
    try
    {
        value_a = boost::lexical_cast<double>(str_a.c_str());
    }
    catch (boost::exception &e)
    {
        ui.le_2p_a->setFocus();
        return;
    } 

    try
    {
        value_b = boost::lexical_cast<double>(str_b.c_str());
    }
    catch (boost::exception &e)
    {
        ui.le_2p_b->setFocus();
        return;
    } 
    
    if( value_a > value_b )
    {
        auto result = ForcastC_ABDown(value_a, value_b);
        ui.lb_c1->setText( QString("%1").arg(std::get<0>(result)) );
        ui.lb_c2->setText( QString("%1").arg(std::get<1>(result)) );
        ui.lb_c3->setText( QString("%1").arg(std::get<2>(result)) );
    }else if( value_a < value_b )
    {
        auto result = ForcastC_ABUp(value_a, value_b);
        ui.lb_c1->setText( QString("%1").arg(std::get<0>(result)) );
        ui.lb_c2->setText( QString("%1").arg(std::get<1>(result)) );
        ui.lb_c3->setText( QString("%1").arg(std::get<2>(result)) );
    }else
    {
        QMessageBox::information(nullptr, "alert", QString::fromLocal8Bit("A和B值不应该相等!"));
    }
}

void AlgoTool::on3pForcast()
{
    ui.lb_3p_c1->clear();
    ui.lb_3p_c2->clear();
    ui.lb_3p_c3->clear();
    std::string str_a = ui.le_3p_a->text().trimmed().toLocal8Bit().data();
    std::string str_b = ui.le_3p_b->text().trimmed().toLocal8Bit().data();

    double value_a = 0.0;
    double value_b = 0.0;

    try
    {
        value_a = boost::lexical_cast<double>(str_a.c_str());
    }
    catch (boost::exception &e)
    {
        ui.le_3p_a->setFocus();
        return;
    }  

    try
    {
        value_b = boost::lexical_cast<double>(str_b.c_str());
    }
    catch (boost::exception &e)
    {
        ui.le_3p_b->setFocus();
        return;
    } 

    if( value_a > value_b )
    {
        auto result = ForcastD_ABC_Down(value_a, value_b);
        ui.lb_3p_c1->setText( QString("%1").arg(std::get<0>(result)) );
        ui.lb_3p_c2->setText( QString("%1").arg(std::get<1>(result)) );
        ui.lb_3p_c3->setText( QString("%1").arg(std::get<2>(result)) );
    }else if( value_a < value_b )
    {
        auto result = ForcastD_ABC_Up(value_a, value_b);
        ui.lb_3p_c1->setText( QString("%1").arg(std::get<0>(result)) );
        ui.lb_3p_c2->setText( QString("%1").arg(std::get<1>(result)) );
        ui.lb_3p_c3->setText( QString("%1").arg(std::get<2>(result)) );
    }else
    {
        ui.le_3p_a->setFocus();
        QMessageBox::information(nullptr, "alert", QString::fromLocal8Bit("A和B值不应该相等!"));
    }
}

/////////////////////////////

std::tuple<double, double, double>  ForcastC_ABDown(double a, double b)
{
    double c2 = sqrt(a * b);
    double c1 = sqrt(b * c2);
    double c3 = c1 + c2 - b;
    return std::make_tuple(c1, c2, c3);
}

std::tuple<double, double, double>  ForcastC_ABUp(double a, double b)
{
    double c2 = sqrt(a * b);
    double c1 = sqrt(b * c2);
    double c3 = 0.7279*a + 0.207*b + 0.06488*c1;
    return std::make_tuple(c1, c2, c3);
}

// ps: make sure a > b > 0
std::tuple<double, double, double>  ForcastD_ABC_Down(double A, double B)
{
    double D2 = B * sqrt( (B/A) + (1.309571 - B) / pow(pow(A,2), 2.10075) );
    double D1= sqrt(B*D2);
    double D3 = 0.254283*B + 0.745551*B*B/A;
    return std::make_tuple(D1, D2, D3);
}

// ps: make sure 0 < a < b
std::tuple<double, double, double>  ForcastD_ABC_Up(double A, double B)
{  
    double D2 = B + B - A;
    double D1 = 0.419913*D2 + 0.41571*B + (0.039433 + 0.164428*D2*D2)/B;
    double D3 = 1.089025*D2 + 0.214334*D1 - 0.30335*B;
    return std::make_tuple(D1, D2, D3);
}
/********************************************************************************
** Form generated from reading UI file 'traindlg.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRAINDLG_H
#define UI_TRAINDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TrainDlgForm
{
public:
    QDateEdit *dateEditTrainBeg;
    QLabel *label;
    QLabel *label_2;
    QDoubleSpinBox *dbspbBegCapital;
    QPushButton *pbtnBuy;
    QPushButton *pbtnSell;
    QPushButton *pbtnNextK;
    QPushButton *pbtnPreK;
    QLabel *label_3;
    QDoubleSpinBox *dbspbFeeRate;
    QLabel *label_4;
    QTableView *tableView;
    QLabel *label_5;

    void setupUi(QWidget *TrainDlgForm)
    {
        if (TrainDlgForm->objectName().isEmpty())
            TrainDlgForm->setObjectName(QStringLiteral("TrainDlgForm"));
        TrainDlgForm->resize(611, 399);
        dateEditTrainBeg = new QDateEdit(TrainDlgForm);
        dateEditTrainBeg->setObjectName(QStringLiteral("dateEditTrainBeg"));
        dateEditTrainBeg->setGeometry(QRect(110, 20, 131, 31));
        label = new QLabel(TrainDlgForm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 60, 61, 21));
        QFont font;
        font.setPointSize(10);
        label->setFont(font);
        label_2 = new QLabel(TrainDlgForm);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 20, 71, 21));
        label_2->setFont(font);
        dbspbBegCapital = new QDoubleSpinBox(TrainDlgForm);
        dbspbBegCapital->setObjectName(QStringLiteral("dbspbBegCapital"));
        dbspbBegCapital->setGeometry(QRect(110, 60, 111, 31));
        dbspbBegCapital->setMaximum(1e+07);
        dbspbBegCapital->setValue(1e+06);
        pbtnBuy = new QPushButton(TrainDlgForm);
        pbtnBuy->setObjectName(QStringLiteral("pbtnBuy"));
        pbtnBuy->setGeometry(QRect(40, 140, 75, 23));
        pbtnBuy->setFont(font);
        pbtnSell = new QPushButton(TrainDlgForm);
        pbtnSell->setObjectName(QStringLiteral("pbtnSell"));
        pbtnSell->setGeometry(QRect(150, 140, 75, 23));
        pbtnSell->setFont(font);
        pbtnNextK = new QPushButton(TrainDlgForm);
        pbtnNextK->setObjectName(QStringLiteral("pbtnNextK"));
        pbtnNextK->setGeometry(QRect(40, 100, 75, 23));
        pbtnNextK->setFont(font);
        pbtnPreK = new QPushButton(TrainDlgForm);
        pbtnPreK->setObjectName(QStringLiteral("pbtnPreK"));
        pbtnPreK->setGeometry(QRect(150, 100, 75, 23));
        pbtnPreK->setFont(font);
        label_3 = new QLabel(TrainDlgForm);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(280, 60, 61, 21));
        label_3->setFont(font);
        dbspbFeeRate = new QDoubleSpinBox(TrainDlgForm);
        dbspbFeeRate->setObjectName(QStringLiteral("dbspbFeeRate"));
        dbspbFeeRate->setGeometry(QRect(340, 60, 111, 31));
        dbspbFeeRate->setDecimals(4);
        dbspbFeeRate->setMaximum(99);
        dbspbFeeRate->setSingleStep(0.001);
        dbspbFeeRate->setValue(0.025);
        label_4 = new QLabel(TrainDlgForm);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(460, 60, 31, 21));
        label_4->setFont(font);
        tableView = new QTableView(TrainDlgForm);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(10, 200, 591, 171));
        label_5 = new QLabel(TrainDlgForm);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 180, 61, 21));
        label_5->setFont(font);

        retranslateUi(TrainDlgForm);

        QMetaObject::connectSlotsByName(TrainDlgForm);
    } // setupUi

    void retranslateUi(QWidget *TrainDlgForm)
    {
        TrainDlgForm->setWindowTitle(QApplication::translate("TrainDlgForm", "\350\256\255\347\273\203\346\250\241\345\274\217", 0));
        label->setText(QApplication::translate("TrainDlgForm", "\345\210\235\345\247\213\350\265\204\351\207\221:", 0));
        label_2->setText(QApplication::translate("TrainDlgForm", "\350\256\255\347\273\203\345\274\200\345\247\213\346\227\245:", 0));
        pbtnBuy->setText(QApplication::translate("TrainDlgForm", "\344\271\260", 0));
        pbtnSell->setText(QApplication::translate("TrainDlgForm", "\345\215\226", 0));
        pbtnNextK->setText(QApplication::translate("TrainDlgForm", "\344\270\213\344\270\200\346\240\271=>", 0));
        pbtnPreK->setText(QApplication::translate("TrainDlgForm", "<=\344\270\212\344\270\200\346\240\271", 0));
        label_3->setText(QApplication::translate("TrainDlgForm", "\346\211\213\347\273\255\350\264\271:", 0));
        label_4->setText(QApplication::translate("TrainDlgForm", "%", 0));
        label_5->setText(QApplication::translate("TrainDlgForm", "\346\210\220\344\272\244\350\256\260\345\275\225", 0));
    } // retranslateUi

};

namespace Ui {
    class TrainDlgForm: public Ui_TrainDlgForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAINDLG_H

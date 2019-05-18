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
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TrainDlgForm
{
public:
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
    QTableView *table_view_record;
    QLabel *label_5;
    QCalendarWidget *calendar;
    QLineEdit *le_date;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbtnStart;
    QPushButton *pbtnStop;
    QLineEdit *le_cur_capital;
    QLabel *label_6;
    QLabel *label_7;
    QLineEdit *le_cur_stock_num;
    QLabel *label_8;
    QLabel *lab_assets;
    QLabel *label_9;
    QPlainTextEdit *plain_te_record;

    void setupUi(QWidget *TrainDlgForm)
    {
        if (TrainDlgForm->objectName().isEmpty())
            TrainDlgForm->setObjectName(QStringLiteral("TrainDlgForm"));
        TrainDlgForm->resize(626, 566);
        label = new QLabel(TrainDlgForm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 170, 61, 21));
        QFont font;
        font.setPointSize(10);
        label->setFont(font);
        label_2 = new QLabel(TrainDlgForm);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 20, 71, 21));
        label_2->setFont(font);
        dbspbBegCapital = new QDoubleSpinBox(TrainDlgForm);
        dbspbBegCapital->setObjectName(QStringLiteral("dbspbBegCapital"));
        dbspbBegCapital->setGeometry(QRect(110, 170, 111, 31));
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
        pbtnNextK->setGeometry(QRect(150, 100, 75, 23));
        pbtnNextK->setFont(font);
        pbtnPreK = new QPushButton(TrainDlgForm);
        pbtnPreK->setObjectName(QStringLiteral("pbtnPreK"));
        pbtnPreK->setGeometry(QRect(40, 100, 75, 23));
        pbtnPreK->setFont(font);
        label_3 = new QLabel(TrainDlgForm);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(270, 170, 61, 21));
        label_3->setFont(font);
        dbspbFeeRate = new QDoubleSpinBox(TrainDlgForm);
        dbspbFeeRate->setObjectName(QStringLiteral("dbspbFeeRate"));
        dbspbFeeRate->setGeometry(QRect(330, 170, 111, 31));
        dbspbFeeRate->setDecimals(4);
        dbspbFeeRate->setMaximum(99);
        dbspbFeeRate->setSingleStep(0.001);
        dbspbFeeRate->setValue(0.025);
        label_4 = new QLabel(TrainDlgForm);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(460, 60, 31, 21));
        label_4->setFont(font);
        table_view_record = new QTableView(TrainDlgForm);
        table_view_record->setObjectName(QStringLiteral("table_view_record"));
        table_view_record->setGeometry(QRect(0, 530, 611, 16));
        label_5 = new QLabel(TrainDlgForm);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 250, 61, 21));
        label_5->setFont(font);
        calendar = new QCalendarWidget(TrainDlgForm);
        calendar->setObjectName(QStringLiteral("calendar"));
        calendar->setGeometry(QRect(250, 0, 261, 151));
        calendar->setGridVisible(false);
        le_date = new QLineEdit(TrainDlgForm);
        le_date->setObjectName(QStringLiteral("le_date"));
        le_date->setGeometry(QRect(110, 19, 111, 31));
        layoutWidget = new QWidget(TrainDlgForm);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(50, 60, 158, 25));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pbtnStart = new QPushButton(layoutWidget);
        pbtnStart->setObjectName(QStringLiteral("pbtnStart"));
        pbtnStart->setFont(font);

        horizontalLayout->addWidget(pbtnStart);

        pbtnStop = new QPushButton(layoutWidget);
        pbtnStop->setObjectName(QStringLiteral("pbtnStop"));
        pbtnStop->setFont(font);

        horizontalLayout->addWidget(pbtnStop);

        le_cur_capital = new QLineEdit(TrainDlgForm);
        le_cur_capital->setObjectName(QStringLiteral("le_cur_capital"));
        le_cur_capital->setGeometry(QRect(110, 210, 111, 31));
        label_6 = new QLabel(TrainDlgForm);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(40, 210, 61, 21));
        label_6->setFont(font);
        label_7 = new QLabel(TrainDlgForm);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(256, 210, 61, 21));
        label_7->setFont(font);
        le_cur_stock_num = new QLineEdit(TrainDlgForm);
        le_cur_stock_num->setObjectName(QStringLiteral("le_cur_stock_num"));
        le_cur_stock_num->setGeometry(QRect(330, 210, 111, 31));
        label_8 = new QLabel(TrainDlgForm);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(470, 210, 41, 21));
        label_8->setFont(font);
        lab_assets = new QLabel(TrainDlgForm);
        lab_assets->setObjectName(QStringLiteral("lab_assets"));
        lab_assets->setGeometry(QRect(510, 210, 101, 21));
        lab_assets->setFont(font);
        label_9 = new QLabel(TrainDlgForm);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(450, 180, 31, 16));
        label_9->setFont(font);
        plain_te_record = new QPlainTextEdit(TrainDlgForm);
        plain_te_record->setObjectName(QStringLiteral("plain_te_record"));
        plain_te_record->setGeometry(QRect(10, 270, 601, 251));

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
        pbtnStart->setText(QApplication::translate("TrainDlgForm", "\345\274\200\345\247\213", 0));
        pbtnStop->setText(QApplication::translate("TrainDlgForm", "\347\273\223\346\235\237", 0));
        label_6->setText(QApplication::translate("TrainDlgForm", "\345\217\257\347\224\250\350\265\204\351\207\221:", 0));
        label_7->setText(QApplication::translate("TrainDlgForm", "\345\275\223\345\211\215\350\202\241\346\225\260:", 0));
        label_8->setText(QApplication::translate("TrainDlgForm", "\346\235\203\347\233\212:", 0));
        lab_assets->setText(QApplication::translate("TrainDlgForm", "10000000:", 0));
        label_9->setText(QApplication::translate("TrainDlgForm", "%", 0));
    } // retranslateUi

};

namespace Ui {
    class TrainDlgForm: public Ui_TrainDlgForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAINDLG_H

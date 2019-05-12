/********************************************************************************
** Form generated from reading UI file 'traintrade.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRAINTRADE_H
#define UI_TRAINTRADE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TrainTradeForm
{
public:
    QLabel *lab_price;
    QLabel *lab_qty_ava;
    QLineEdit *le_price;
    QLineEdit *le_qty_ava;
    QLineEdit *le_qty;
    QLabel *lab_qty;
    QLabel *lab_capital_ava;
    QLineEdit *le_capital_ava;
    QLineEdit *le_capital;
    QLabel *lab_capital;
    QLabel *label_reason;
    QTextEdit *textEdit_reason;
    QPushButton *pbtn_close;
    QPushButton *pbt_trade;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbt_all;
    QPushButton *pbtn_qty_half;
    QPushButton *pbtn_qty_one_third;
    QPushButton *pbtn_qty_one_fifth;
    QLabel *lab_status;

    void setupUi(QWidget *TrainTradeForm)
    {
        if (TrainTradeForm->objectName().isEmpty())
            TrainTradeForm->setObjectName(QStringLiteral("TrainTradeForm"));
        TrainTradeForm->resize(484, 435);
        lab_price = new QLabel(TrainTradeForm);
        lab_price->setObjectName(QStringLiteral("lab_price"));
        lab_price->setGeometry(QRect(60, 20, 51, 31));
        lab_qty_ava = new QLabel(TrainTradeForm);
        lab_qty_ava->setObjectName(QStringLiteral("lab_qty_ava"));
        lab_qty_ava->setGeometry(QRect(60, 70, 51, 31));
        le_price = new QLineEdit(TrainTradeForm);
        le_price->setObjectName(QStringLiteral("le_price"));
        le_price->setGeometry(QRect(140, 30, 113, 20));
        le_qty_ava = new QLineEdit(TrainTradeForm);
        le_qty_ava->setObjectName(QStringLiteral("le_qty_ava"));
        le_qty_ava->setGeometry(QRect(140, 70, 113, 20));
        le_qty = new QLineEdit(TrainTradeForm);
        le_qty->setObjectName(QStringLiteral("le_qty"));
        le_qty->setGeometry(QRect(140, 110, 113, 20));
        lab_qty = new QLabel(TrainTradeForm);
        lab_qty->setObjectName(QStringLiteral("lab_qty"));
        lab_qty->setGeometry(QRect(60, 110, 51, 31));
        lab_capital_ava = new QLabel(TrainTradeForm);
        lab_capital_ava->setObjectName(QStringLiteral("lab_capital_ava"));
        lab_capital_ava->setGeometry(QRect(60, 150, 51, 31));
        le_capital_ava = new QLineEdit(TrainTradeForm);
        le_capital_ava->setObjectName(QStringLiteral("le_capital_ava"));
        le_capital_ava->setGeometry(QRect(140, 150, 113, 20));
        le_capital = new QLineEdit(TrainTradeForm);
        le_capital->setObjectName(QStringLiteral("le_capital"));
        le_capital->setGeometry(QRect(140, 190, 113, 20));
        lab_capital = new QLabel(TrainTradeForm);
        lab_capital->setObjectName(QStringLiteral("lab_capital"));
        lab_capital->setGeometry(QRect(60, 190, 51, 31));
        label_reason = new QLabel(TrainTradeForm);
        label_reason->setObjectName(QStringLiteral("label_reason"));
        label_reason->setGeometry(QRect(50, 270, 51, 31));
        textEdit_reason = new QTextEdit(TrainTradeForm);
        textEdit_reason->setObjectName(QStringLiteral("textEdit_reason"));
        textEdit_reason->setGeometry(QRect(110, 270, 321, 101));
        pbtn_close = new QPushButton(TrainTradeForm);
        pbtn_close->setObjectName(QStringLiteral("pbtn_close"));
        pbtn_close->setGeometry(QRect(270, 380, 75, 23));
        pbt_trade = new QPushButton(TrainTradeForm);
        pbt_trade->setObjectName(QStringLiteral("pbt_trade"));
        pbt_trade->setGeometry(QRect(189, 380, 75, 23));
        layoutWidget = new QWidget(TrainTradeForm);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(30, 230, 320, 25));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pbt_all = new QPushButton(layoutWidget);
        pbt_all->setObjectName(QStringLiteral("pbt_all"));

        horizontalLayout->addWidget(pbt_all);

        pbtn_qty_half = new QPushButton(layoutWidget);
        pbtn_qty_half->setObjectName(QStringLiteral("pbtn_qty_half"));

        horizontalLayout->addWidget(pbtn_qty_half);

        pbtn_qty_one_third = new QPushButton(layoutWidget);
        pbtn_qty_one_third->setObjectName(QStringLiteral("pbtn_qty_one_third"));

        horizontalLayout->addWidget(pbtn_qty_one_third);

        pbtn_qty_one_fifth = new QPushButton(layoutWidget);
        pbtn_qty_one_fifth->setObjectName(QStringLiteral("pbtn_qty_one_fifth"));

        horizontalLayout->addWidget(pbtn_qty_one_fifth);

        lab_status = new QLabel(TrainTradeForm);
        lab_status->setObjectName(QStringLiteral("lab_status"));
        lab_status->setGeometry(QRect(0, 410, 481, 20));
        QFont font;
        font.setPointSize(10);
        lab_status->setFont(font);

        retranslateUi(TrainTradeForm);

        QMetaObject::connectSlotsByName(TrainTradeForm);
    } // setupUi

    void retranslateUi(QWidget *TrainTradeForm)
    {
        TrainTradeForm->setWindowTitle(QApplication::translate("TrainTradeForm", "Form", 0));
        lab_price->setText(QApplication::translate("TrainTradeForm", "\344\271\260\345\205\245\344\273\267\346\240\274", 0));
        lab_qty_ava->setText(QApplication::translate("TrainTradeForm", "\345\217\257\344\271\260\346\225\260\351\207\217", 0));
        lab_qty->setText(QApplication::translate("TrainTradeForm", "\344\271\260\345\205\245\346\225\260\351\207\217", 0));
        lab_capital_ava->setText(QApplication::translate("TrainTradeForm", "\345\217\257\347\224\250\350\265\204\351\207\221", 0));
        lab_capital->setText(QApplication::translate("TrainTradeForm", "\344\271\260\345\205\245\350\265\204\351\207\221", 0));
        label_reason->setText(QApplication::translate("TrainTradeForm", "\344\272\244\346\230\223\347\220\206\347\224\261:", 0));
        pbtn_close->setText(QApplication::translate("TrainTradeForm", "\345\205\263\351\227\255", 0));
        pbt_trade->setText(QApplication::translate("TrainTradeForm", "\344\271\260\345\205\245", 0));
        pbt_all->setText(QApplication::translate("TrainTradeForm", "\345\205\250\351\207\217", 0));
        pbtn_qty_half->setText(QApplication::translate("TrainTradeForm", "1/2", 0));
        pbtn_qty_one_third->setText(QApplication::translate("TrainTradeForm", "1/3", 0));
        pbtn_qty_one_fifth->setText(QApplication::translate("TrainTradeForm", "1/5", 0));
        lab_status->setText(QApplication::translate("TrainTradeForm", "status", 0));
    } // retranslateUi

};

namespace Ui {
    class TrainTradeForm: public Ui_TrainTradeForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAINTRADE_H

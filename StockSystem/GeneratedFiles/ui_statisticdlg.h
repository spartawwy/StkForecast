/********************************************************************************
** Form generated from reading UI file 'statisticdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATISTICDLG_H
#define UI_STATISTICDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QLabel *label;
    QLabel *label_2;
    QLineEdit *le_scale_increase;
    QLineEdit *le_LowestPrice;
    QLineEdit *le_HighesttPrice;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *lab_knum;
    QPushButton *pbt_close;
    QLineEdit *le_scale_shake;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLineEdit *le_end_date;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *le_start_date;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(400, 300);
        label = new QLabel(Form);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 102, 51, 31));
        label_2 = new QLabel(Form);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 170, 51, 21));
        le_scale_increase = new QLineEdit(Form);
        le_scale_increase->setObjectName(QStringLiteral("le_scale_increase"));
        le_scale_increase->setGeometry(QRect(100, 110, 113, 20));
        le_LowestPrice = new QLineEdit(Form);
        le_LowestPrice->setObjectName(QStringLiteral("le_LowestPrice"));
        le_LowestPrice->setGeometry(QRect(100, 170, 113, 20));
        le_HighesttPrice = new QLineEdit(Form);
        le_HighesttPrice->setObjectName(QStringLiteral("le_HighesttPrice"));
        le_HighesttPrice->setGeometry(QRect(100, 200, 113, 20));
        label_3 = new QLabel(Form);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(40, 200, 51, 21));
        label_4 = new QLabel(Form);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(40, 70, 51, 31));
        lab_knum = new QLabel(Form);
        lab_knum->setObjectName(QStringLiteral("lab_knum"));
        lab_knum->setGeometry(QRect(100, 70, 91, 31));
        pbt_close = new QPushButton(Form);
        pbt_close->setObjectName(QStringLiteral("pbt_close"));
        pbt_close->setGeometry(QRect(280, 240, 75, 23));
        le_scale_shake = new QLineEdit(Form);
        le_scale_shake->setObjectName(QStringLiteral("le_scale_shake"));
        le_scale_shake->setGeometry(QRect(100, 138, 113, 20));
        label_5 = new QLabel(Form);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(40, 130, 51, 31));
        label_6 = new QLabel(Form);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(220, 110, 21, 16));
        label_7 = new QLabel(Form);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(220, 140, 21, 16));
        le_end_date = new QLineEdit(Form);
        le_end_date->setObjectName(QStringLiteral("le_end_date"));
        le_end_date->setGeometry(QRect(100, 48, 113, 20));
        label_8 = new QLabel(Form);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(40, 40, 51, 31));
        label_9 = new QLabel(Form);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(40, 10, 51, 31));
        le_start_date = new QLineEdit(Form);
        le_start_date->setObjectName(QStringLiteral("le_start_date"));
        le_start_date->setGeometry(QRect(100, 18, 113, 20));

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "\347\273\237\350\256\241", 0));
        label->setText(QApplication::translate("Form", "\345\214\272\351\227\264\346\266\250\345\271\205", 0));
        label_2->setText(QApplication::translate("Form", "\346\234\200\344\275\216\344\273\267", 0));
        label_3->setText(QApplication::translate("Form", "\346\234\200\351\253\230\344\273\267", 0));
        label_4->setText(QApplication::translate("Form", "\345\221\250\346\234\237\346\225\260", 0));
        lab_knum->setText(QApplication::translate("Form", "TextLabel", 0));
        pbt_close->setText(QApplication::translate("Form", "\345\205\263\351\227\255", 0));
        label_5->setText(QApplication::translate("Form", "\345\214\272\351\227\264\346\214\257\345\271\205", 0));
        label_6->setText(QApplication::translate("Form", "%", 0));
        label_7->setText(QApplication::translate("Form", "%", 0));
        label_8->setText(QApplication::translate("Form", "\347\273\223\346\235\237\346\227\266\351\227\264", 0));
        label_9->setText(QApplication::translate("Form", "\345\274\200\345\247\213\346\227\266\351\227\264", 0));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATISTICDLG_H

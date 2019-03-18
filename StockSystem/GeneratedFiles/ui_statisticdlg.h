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
    QLineEdit *leLowestPrice;
    QLineEdit *leHighesttPrice;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *lab_knum;
    QPushButton *pbt_close;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(400, 300);
        label = new QLabel(Form);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 42, 51, 31));
        label_2 = new QLabel(Form);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 80, 51, 21));
        le_scale_increase = new QLineEdit(Form);
        le_scale_increase->setObjectName(QStringLiteral("le_scale_increase"));
        le_scale_increase->setGeometry(QRect(100, 50, 113, 20));
        leLowestPrice = new QLineEdit(Form);
        leLowestPrice->setObjectName(QStringLiteral("leLowestPrice"));
        leLowestPrice->setGeometry(QRect(100, 80, 113, 20));
        leHighesttPrice = new QLineEdit(Form);
        leHighesttPrice->setObjectName(QStringLiteral("leHighesttPrice"));
        leHighesttPrice->setGeometry(QRect(100, 110, 113, 20));
        label_3 = new QLabel(Form);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(40, 110, 51, 21));
        label_4 = new QLabel(Form);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(240, 40, 51, 31));
        lab_knum = new QLabel(Form);
        lab_knum->setObjectName(QStringLiteral("lab_knum"));
        lab_knum->setGeometry(QRect(280, 40, 91, 31));
        pbt_close = new QPushButton(Form);
        pbt_close->setObjectName(QStringLiteral("pbt_close"));
        pbt_close->setGeometry(QRect(240, 200, 75, 23));

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
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATISTICDLG_H

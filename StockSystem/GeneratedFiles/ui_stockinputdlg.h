/********************************************************************************
** Form generated from reading UI file 'stockinputdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STOCKINPUTDLG_H
#define UI_STOCKINPUTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StockInputDlgForm
{
public:
    QLineEdit *stock_input;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QWidget *StockInputDlgForm)
    {
        if (StockInputDlgForm->objectName().isEmpty())
            StockInputDlgForm->setObjectName(QStringLiteral("StockInputDlgForm"));
        StockInputDlgForm->resize(269, 353);
        stock_input = new QLineEdit(StockInputDlgForm);
        stock_input->setObjectName(QStringLiteral("stock_input"));
        stock_input->setGeometry(QRect(0, 0, 261, 21));
        plainTextEdit = new QPlainTextEdit(StockInputDlgForm);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(0, 30, 271, 321));

        retranslateUi(StockInputDlgForm);

        QMetaObject::connectSlotsByName(StockInputDlgForm);
    } // setupUi

    void retranslateUi(QWidget *StockInputDlgForm)
    {
        StockInputDlgForm->setWindowTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class StockInputDlgForm: public Ui_StockInputDlgForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STOCKINPUTDLG_H

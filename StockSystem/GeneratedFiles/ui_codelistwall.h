/********************************************************************************
** Form generated from reading UI file 'codelistwall.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CODELISTWALL_H
#define UI_CODELISTWALL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CodeListWallForm
{
public:
    QTableView *tbview_codes;

    void setupUi(QWidget *CodeListWallForm)
    {
        if (CodeListWallForm->objectName().isEmpty())
            CodeListWallForm->setObjectName(QStringLiteral("CodeListWallForm"));
        CodeListWallForm->resize(549, 353);
        tbview_codes = new QTableView(CodeListWallForm);
        tbview_codes->setObjectName(QStringLiteral("tbview_codes"));
        tbview_codes->setGeometry(QRect(10, 20, 521, 321));
        tbview_codes->setContextMenuPolicy(Qt::CustomContextMenu);

        retranslateUi(CodeListWallForm);

        QMetaObject::connectSlotsByName(CodeListWallForm);
    } // setupUi

    void retranslateUi(QWidget *CodeListWallForm)
    {
        CodeListWallForm->setWindowTitle(QApplication::translate("CodeListWallForm", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class CodeListWallForm: public Ui_CodeListWallForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CODELISTWALL_H

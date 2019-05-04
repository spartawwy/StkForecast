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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TrainDlgForm
{
public:

    void setupUi(QWidget *TrainDlgForm)
    {
        if (TrainDlgForm->objectName().isEmpty())
            TrainDlgForm->setObjectName(QStringLiteral("TrainDlgForm"));
        TrainDlgForm->resize(611, 399);

        retranslateUi(TrainDlgForm);

        QMetaObject::connectSlotsByName(TrainDlgForm);
    } // setupUi

    void retranslateUi(QWidget *TrainDlgForm)
    {
        TrainDlgForm->setWindowTitle(QApplication::translate("TrainDlgForm", "\350\256\255\347\273\203\346\250\241\345\274\217", 0));
    } // retranslateUi

};

namespace Ui {
    class TrainDlgForm: public Ui_TrainDlgForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAINDLG_H

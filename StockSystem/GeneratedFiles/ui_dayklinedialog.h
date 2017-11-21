/********************************************************************************
** Form generated from reading UI file 'dayklinedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DAYKLINEDIALOG_H
#define UI_DAYKLINEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_DayKLineDialog
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DayKLineDialog)
    {
        if (DayKLineDialog->objectName().isEmpty())
            DayKLineDialog->setObjectName(QStringLiteral("DayKLineDialog"));
        DayKLineDialog->resize(950, 650);
        buttonBox = new QDialogButtonBox(DayKLineDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(60, 490, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(DayKLineDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), DayKLineDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DayKLineDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(DayKLineDialog);
    } // setupUi

    void retranslateUi(QDialog *DayKLineDialog)
    {
        DayKLineDialog->setWindowTitle(QApplication::translate("DayKLineDialog", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class DayKLineDialog: public Ui_DayKLineDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DAYKLINEDIALOG_H

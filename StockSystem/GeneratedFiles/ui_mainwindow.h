/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action;
    QAction *actionExit;
    QWidget *centralWidget;
    QLabel *labelCurrentTime;
    QLabel *labelFind;
    QLineEdit *EdtFind;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(712, 502);
        action = new QAction(MainWindow);
        action->setObjectName(QStringLiteral("action"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/gif/1.gif"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        labelCurrentTime = new QLabel(centralWidget);
        labelCurrentTime->setObjectName(QStringLiteral("labelCurrentTime"));
        labelCurrentTime->setGeometry(QRect(9, 89, 16, 16));
        labelFind = new QLabel(centralWidget);
        labelFind->setObjectName(QStringLiteral("labelFind"));
        labelFind->setGeometry(QRect(9, 45, 102, 16));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(10);
        sizePolicy.setHeightForWidth(labelFind->sizePolicy().hasHeightForWidth());
        labelFind->setSizePolicy(sizePolicy);
        EdtFind = new QLineEdit(centralWidget);
        EdtFind->setObjectName(QStringLiteral("EdtFind"));
        EdtFind->setGeometry(QRect(9, 63, 133, 20));
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "StockForcast", 0));
        action->setText(QApplication::translate("MainWindow", "?", 0));
        actionExit->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272(&x)", 0));
        actionExit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0));
        labelCurrentTime->setText(QString());
        labelFind->setText(QApplication::translate("MainWindow", "\346\237\245\346\211\276(\344\273\243\345\217\267/\345\220\215\347\247\260)\357\274\232", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

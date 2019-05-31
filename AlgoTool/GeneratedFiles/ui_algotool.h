/********************************************************************************
** Form generated from reading UI file 'algotool.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ALGOTOOL_H
#define UI_ALGOTOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AlgoToolClass
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QLineEdit *le_2p_a;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *le_2p_b;
    QPushButton *pb_2p_forcast;
    QLabel *label_3;
    QLabel *lb_c1;
    QLabel *lb_c2;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *lb_c3;
    QWidget *tab_2;
    QLabel *label_6;
    QLineEdit *le_3p_a;
    QLineEdit *le_3p_b;
    QLabel *label_7;
    QLineEdit *le_3p_c;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *lb_3p_c3;
    QLabel *lb_3p_c1;
    QLabel *label_11;
    QLabel *lb_3p_c2;
    QPushButton *pb_3p_forcast;

    void setupUi(QWidget *AlgoToolClass)
    {
        if (AlgoToolClass->objectName().isEmpty())
            AlgoToolClass->setObjectName(QStringLiteral("AlgoToolClass"));
        AlgoToolClass->resize(600, 400);
        tabWidget = new QTabWidget(AlgoToolClass);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(20, 20, 551, 351));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        le_2p_a = new QLineEdit(tab);
        le_2p_a->setObjectName(QStringLiteral("le_2p_a"));
        le_2p_a->setGeometry(QRect(60, 30, 111, 31));
        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 30, 21, 21));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 80, 21, 16));
        le_2p_b = new QLineEdit(tab);
        le_2p_b->setObjectName(QStringLiteral("le_2p_b"));
        le_2p_b->setGeometry(QRect(60, 70, 111, 31));
        pb_2p_forcast = new QPushButton(tab);
        pb_2p_forcast->setObjectName(QStringLiteral("pb_2p_forcast"));
        pb_2p_forcast->setGeometry(QRect(380, 270, 75, 23));
        label_3 = new QLabel(tab);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(310, 30, 21, 21));
        lb_c1 = new QLabel(tab);
        lb_c1->setObjectName(QStringLiteral("lb_c1"));
        lb_c1->setGeometry(QRect(350, 30, 101, 21));
        lb_c2 = new QLabel(tab);
        lb_c2->setObjectName(QStringLiteral("lb_c2"));
        lb_c2->setGeometry(QRect(350, 70, 101, 21));
        label_4 = new QLabel(tab);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(310, 70, 21, 21));
        label_5 = new QLabel(tab);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(310, 110, 21, 21));
        lb_c3 = new QLabel(tab);
        lb_c3->setObjectName(QStringLiteral("lb_c3"));
        lb_c3->setGeometry(QRect(350, 110, 101, 21));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        label_6 = new QLabel(tab_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(40, 80, 21, 16));
        le_3p_a = new QLineEdit(tab_2);
        le_3p_a->setObjectName(QStringLiteral("le_3p_a"));
        le_3p_a->setGeometry(QRect(70, 30, 111, 31));
        le_3p_b = new QLineEdit(tab_2);
        le_3p_b->setObjectName(QStringLiteral("le_3p_b"));
        le_3p_b->setEnabled(true);
        le_3p_b->setGeometry(QRect(70, 70, 111, 31));
        label_7 = new QLabel(tab_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(40, 30, 21, 21));
        le_3p_c = new QLineEdit(tab_2);
        le_3p_c->setObjectName(QStringLiteral("le_3p_c"));
        le_3p_c->setEnabled(false);
        le_3p_c->setGeometry(QRect(70, 110, 111, 31));
        label_8 = new QLabel(tab_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(40, 120, 21, 16));
        label_9 = new QLabel(tab_2);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(340, 80, 21, 21));
        label_10 = new QLabel(tab_2);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(340, 120, 21, 21));
        lb_3p_c3 = new QLabel(tab_2);
        lb_3p_c3->setObjectName(QStringLiteral("lb_3p_c3"));
        lb_3p_c3->setGeometry(QRect(380, 120, 101, 21));
        lb_3p_c1 = new QLabel(tab_2);
        lb_3p_c1->setObjectName(QStringLiteral("lb_3p_c1"));
        lb_3p_c1->setGeometry(QRect(380, 40, 101, 21));
        label_11 = new QLabel(tab_2);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(340, 40, 21, 21));
        lb_3p_c2 = new QLabel(tab_2);
        lb_3p_c2->setObjectName(QStringLiteral("lb_3p_c2"));
        lb_3p_c2->setGeometry(QRect(380, 80, 101, 21));
        pb_3p_forcast = new QPushButton(tab_2);
        pb_3p_forcast->setObjectName(QStringLiteral("pb_3p_forcast"));
        pb_3p_forcast->setGeometry(QRect(380, 270, 75, 23));
        tabWidget->addTab(tab_2, QString());

        retranslateUi(AlgoToolClass);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(AlgoToolClass);
    } // setupUi

    void retranslateUi(QWidget *AlgoToolClass)
    {
        AlgoToolClass->setWindowTitle(QApplication::translate("AlgoToolClass", "AlgoTool", 0));
        label->setText(QApplication::translate("AlgoToolClass", "A:", 0));
        label_2->setText(QApplication::translate("AlgoToolClass", "B:", 0));
        pb_2p_forcast->setText(QApplication::translate("AlgoToolClass", "\351\242\204\346\265\213", 0));
        label_3->setText(QApplication::translate("AlgoToolClass", "C1:", 0));
        lb_c1->setText(QApplication::translate("AlgoToolClass", "C1:", 0));
        lb_c2->setText(QApplication::translate("AlgoToolClass", "C2:", 0));
        label_4->setText(QApplication::translate("AlgoToolClass", "C2:", 0));
        label_5->setText(QApplication::translate("AlgoToolClass", "C3:", 0));
        lb_c3->setText(QApplication::translate("AlgoToolClass", "C2:", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("AlgoToolClass", "\344\270\244\347\202\271\351\242\204\346\265\213", 0));
        label_6->setText(QApplication::translate("AlgoToolClass", "B:", 0));
        label_7->setText(QApplication::translate("AlgoToolClass", "A:", 0));
        label_8->setText(QApplication::translate("AlgoToolClass", "C:", 0));
        label_9->setText(QApplication::translate("AlgoToolClass", "C2:", 0));
        label_10->setText(QApplication::translate("AlgoToolClass", "C3:", 0));
        lb_3p_c3->setText(QApplication::translate("AlgoToolClass", "C2:", 0));
        lb_3p_c1->setText(QApplication::translate("AlgoToolClass", "C1:", 0));
        label_11->setText(QApplication::translate("AlgoToolClass", "C1:", 0));
        lb_3p_c2->setText(QApplication::translate("AlgoToolClass", "C2:", 0));
        pb_3p_forcast->setText(QApplication::translate("AlgoToolClass", "\351\242\204\346\265\213", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("AlgoToolClass", "\344\270\211\347\202\271\351\242\204\346\265\213", 0));
    } // retranslateUi

};

namespace Ui {
    class AlgoToolClass: public Ui_AlgoToolClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ALGOTOOL_H

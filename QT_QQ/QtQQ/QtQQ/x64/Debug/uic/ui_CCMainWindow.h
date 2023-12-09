/********************************************************************************
** Form generated from reading UI file 'CCMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCMAINWINDOW_H
#define UI_CCMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CCMainWindowClass
{
public:

    void setupUi(QWidget *CCMainWindowClass)
    {
        if (CCMainWindowClass->objectName().isEmpty())
            CCMainWindowClass->setObjectName(QStringLiteral("CCMainWindowClass"));
        CCMainWindowClass->resize(660, 400);

        retranslateUi(CCMainWindowClass);

        QMetaObject::connectSlotsByName(CCMainWindowClass);
    } // setupUi

    void retranslateUi(QWidget *CCMainWindowClass)
    {
        CCMainWindowClass->setWindowTitle(QApplication::translate("CCMainWindowClass", "CCMainWindow", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CCMainWindowClass: public Ui_CCMainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCMAINWINDOW_H

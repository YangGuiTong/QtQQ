/********************************************************************************
** Form generated from reading UI file 'TalkWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TALKWINDOW_H
#define UI_TALKWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TalkWindow
{
public:

    void setupUi(QWidget *TalkWindow)
    {
        if (TalkWindow->objectName().isEmpty())
            TalkWindow->setObjectName(QStringLiteral("TalkWindow"));
        TalkWindow->resize(600, 400);

        retranslateUi(TalkWindow);

        QMetaObject::connectSlotsByName(TalkWindow);
    } // setupUi

    void retranslateUi(QWidget *TalkWindow)
    {
        TalkWindow->setWindowTitle(QApplication::translate("TalkWindow", "TalkWindow", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TalkWindow: public Ui_TalkWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TALKWINDOW_H

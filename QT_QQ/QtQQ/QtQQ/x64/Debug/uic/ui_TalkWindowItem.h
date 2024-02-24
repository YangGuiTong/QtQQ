/********************************************************************************
** Form generated from reading UI file 'TalkWindowItem.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TALKWINDOWITEM_H
#define UI_TALKWINDOWITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TalkWindowItemClass
{
public:

    void setupUi(QWidget *TalkWindowItemClass)
    {
        if (TalkWindowItemClass->objectName().isEmpty())
            TalkWindowItemClass->setObjectName(QStringLiteral("TalkWindowItemClass"));
        TalkWindowItemClass->resize(600, 400);

        retranslateUi(TalkWindowItemClass);

        QMetaObject::connectSlotsByName(TalkWindowItemClass);
    } // setupUi

    void retranslateUi(QWidget *TalkWindowItemClass)
    {
        TalkWindowItemClass->setWindowTitle(QApplication::translate("TalkWindowItemClass", "TalkWindowItem", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TalkWindowItemClass: public Ui_TalkWindowItemClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TALKWINDOWITEM_H

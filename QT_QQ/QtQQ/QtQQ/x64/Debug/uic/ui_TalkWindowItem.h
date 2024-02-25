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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TalkWindowItemClass
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *headlabel;
    QLabel *msgLabel;
    QPushButton *tclosebtn;

    void setupUi(QWidget *TalkWindowItemClass)
    {
        if (TalkWindowItemClass->objectName().isEmpty())
            TalkWindowItemClass->setObjectName(QStringLiteral("TalkWindowItemClass"));
        TalkWindowItemClass->resize(404, 42);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TalkWindowItemClass->sizePolicy().hasHeightForWidth());
        TalkWindowItemClass->setSizePolicy(sizePolicy);
        TalkWindowItemClass->setMinimumSize(QSize(0, 42));
        TalkWindowItemClass->setMaximumSize(QSize(16777215, 42));
        horizontalLayout = new QHBoxLayout(TalkWindowItemClass);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(6, 6, 6, 6);
        headlabel = new QLabel(TalkWindowItemClass);
        headlabel->setObjectName(QStringLiteral("headlabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(headlabel->sizePolicy().hasHeightForWidth());
        headlabel->setSizePolicy(sizePolicy1);
        headlabel->setMinimumSize(QSize(30, 30));
        headlabel->setMaximumSize(QSize(30, 30));

        horizontalLayout->addWidget(headlabel);

        msgLabel = new QLabel(TalkWindowItemClass);
        msgLabel->setObjectName(QStringLiteral("msgLabel"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(msgLabel->sizePolicy().hasHeightForWidth());
        msgLabel->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(msgLabel);

        tclosebtn = new QPushButton(TalkWindowItemClass);
        tclosebtn->setObjectName(QStringLiteral("tclosebtn"));
        sizePolicy1.setHeightForWidth(tclosebtn->sizePolicy().hasHeightForWidth());
        tclosebtn->setSizePolicy(sizePolicy1);
        tclosebtn->setMinimumSize(QSize(16, 16));
        tclosebtn->setMaximumSize(QSize(16, 16));

        horizontalLayout->addWidget(tclosebtn);


        retranslateUi(TalkWindowItemClass);

        QMetaObject::connectSlotsByName(TalkWindowItemClass);
    } // setupUi

    void retranslateUi(QWidget *TalkWindowItemClass)
    {
        TalkWindowItemClass->setWindowTitle(QApplication::translate("TalkWindowItemClass", "TalkWindowItem", Q_NULLPTR));
        headlabel->setText(QString());
        msgLabel->setText(QString());
        tclosebtn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class TalkWindowItemClass: public Ui_TalkWindowItemClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TALKWINDOWITEM_H

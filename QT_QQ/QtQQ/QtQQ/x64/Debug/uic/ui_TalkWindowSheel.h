/********************************************************************************
** Form generated from reading UI file 'TalkWindowSheel.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TALKWINDOWSHEEL_H
#define UI_TALKWINDOWSHEEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TalkWindowClass
{
public:
    QVBoxLayout *verticalLayout_2;
    QSplitter *splitter;
    QWidget *leftWidget;
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QStackedWidget *rightStackedWidget;

    void setupUi(QWidget *TalkWindowClass)
    {
        if (TalkWindowClass->objectName().isEmpty())
            TalkWindowClass->setObjectName(QStringLiteral("TalkWindowClass"));
        TalkWindowClass->resize(1043, 732);
        verticalLayout_2 = new QVBoxLayout(TalkWindowClass);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(1, 1, 1, 1);
        splitter = new QSplitter(TalkWindowClass);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setStyleSheet(QLatin1String("QSplitter::handle {\n"
"	background-color:white;\n"
"}\n"
"\n"
"QSplitter::handle:horizontal {\n"
"	width:1px;\n"
"}"));
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(1);
        splitter->setChildrenCollapsible(false);
        leftWidget = new QWidget(splitter);
        leftWidget->setObjectName(QStringLiteral("leftWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(leftWidget->sizePolicy().hasHeightForWidth());
        leftWidget->setSizePolicy(sizePolicy);
        leftWidget->setMinimumSize(QSize(110, 0));
        leftWidget->setMaximumSize(QSize(248, 16777215));
        verticalLayout = new QVBoxLayout(leftWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        listWidget = new QListWidget(leftWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setFrameShape(QFrame::NoFrame);
        listWidget->setProperty("titleskin", QVariant(true));

        verticalLayout->addWidget(listWidget);

        splitter->addWidget(leftWidget);
        rightStackedWidget = new QStackedWidget(splitter);
        rightStackedWidget->setObjectName(QStringLiteral("rightStackedWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(rightStackedWidget->sizePolicy().hasHeightForWidth());
        rightStackedWidget->setSizePolicy(sizePolicy1);
        splitter->addWidget(rightStackedWidget);

        verticalLayout_2->addWidget(splitter);


        retranslateUi(TalkWindowClass);

        QMetaObject::connectSlotsByName(TalkWindowClass);
    } // setupUi

    void retranslateUi(QWidget *TalkWindowClass)
    {
        TalkWindowClass->setWindowTitle(QApplication::translate("TalkWindowClass", "TalkWindowSheel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TalkWindowClass: public Ui_TalkWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TALKWINDOWSHEEL_H

/********************************************************************************
** Form generated from reading UI file 'RecviveFile.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECVIVEFILE_H
#define UI_RECVIVEFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RecviveFileClass
{
public:
    QWidget *titleWidget;
    QWidget *bodyWidget;
    QLabel *label;
    QPushButton *okBtn;
    QPushButton *cancelBtn;

    void setupUi(QWidget *RecviveFileClass)
    {
        if (RecviveFileClass->objectName().isEmpty())
            RecviveFileClass->setObjectName(QStringLiteral("RecviveFileClass"));
        RecviveFileClass->resize(515, 160);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RecviveFileClass->sizePolicy().hasHeightForWidth());
        RecviveFileClass->setSizePolicy(sizePolicy);
        RecviveFileClass->setMinimumSize(QSize(515, 160));
        RecviveFileClass->setMaximumSize(QSize(515, 160));
        titleWidget = new QWidget(RecviveFileClass);
        titleWidget->setObjectName(QStringLiteral("titleWidget"));
        titleWidget->setGeometry(QRect(0, 0, 515, 50));
        titleWidget->setProperty("titleskin", QVariant(true));
        bodyWidget = new QWidget(RecviveFileClass);
        bodyWidget->setObjectName(QStringLiteral("bodyWidget"));
        bodyWidget->setGeometry(QRect(0, 50, 515, 110));
        bodyWidget->setProperty("bottomskin", QVariant(true));
        label = new QLabel(bodyWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 20, 461, 16));
        okBtn = new QPushButton(bodyWidget);
        okBtn->setObjectName(QStringLiteral("okBtn"));
        okBtn->setGeometry(QRect(270, 70, 93, 28));
        cancelBtn = new QPushButton(bodyWidget);
        cancelBtn->setObjectName(QStringLiteral("cancelBtn"));
        cancelBtn->setGeometry(QRect(400, 70, 93, 28));

        retranslateUi(RecviveFileClass);

        QMetaObject::connectSlotsByName(RecviveFileClass);
    } // setupUi

    void retranslateUi(QWidget *RecviveFileClass)
    {
        RecviveFileClass->setWindowTitle(QApplication::translate("RecviveFileClass", "RecviveFile", Q_NULLPTR));
        label->setText(QApplication::translate("RecviveFileClass", "\346\235\245\350\207\252xxx\345\217\221\346\235\245\347\232\204\346\226\207\344\273\266\357\274\232\346\230\257\345\220\246\346\216\245\346\224\266\357\274\237", Q_NULLPTR));
        okBtn->setText(QApplication::translate("RecviveFileClass", "\347\241\256\345\256\232", Q_NULLPTR));
        cancelBtn->setText(QApplication::translate("RecviveFileClass", "\345\217\226\346\266\210", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class RecviveFileClass: public Ui_RecviveFileClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECVIVEFILE_H

/********************************************************************************
** Form generated from reading UI file 'SendFile.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SENDFILE_H
#define UI_SENDFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SendFileClass
{
public:
    QWidget *titleWidget;
    QWidget *bodyWidget;
    QPushButton *openBtn;
    QPushButton *sendBtn;
    QLineEdit *lineEdit;

    void setupUi(QWidget *SendFileClass)
    {
        if (SendFileClass->objectName().isEmpty())
            SendFileClass->setObjectName(QStringLiteral("SendFileClass"));
        SendFileClass->resize(519, 169);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SendFileClass->sizePolicy().hasHeightForWidth());
        SendFileClass->setSizePolicy(sizePolicy);
        SendFileClass->setMinimumSize(QSize(519, 169));
        SendFileClass->setMaximumSize(QSize(519, 169));
        titleWidget = new QWidget(SendFileClass);
        titleWidget->setObjectName(QStringLiteral("titleWidget"));
        titleWidget->setGeometry(QRect(0, 0, 519, 50));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(titleWidget->sizePolicy().hasHeightForWidth());
        titleWidget->setSizePolicy(sizePolicy1);
        titleWidget->setProperty("titleskin", QVariant(true));
        bodyWidget = new QWidget(SendFileClass);
        bodyWidget->setObjectName(QStringLiteral("bodyWidget"));
        bodyWidget->setGeometry(QRect(0, 50, 519, 119));
        bodyWidget->setProperty("bottomskin", QVariant(true));
        openBtn = new QPushButton(bodyWidget);
        openBtn->setObjectName(QStringLiteral("openBtn"));
        openBtn->setGeometry(QRect(20, 30, 93, 28));
        sendBtn = new QPushButton(bodyWidget);
        sendBtn->setObjectName(QStringLiteral("sendBtn"));
        sendBtn->setGeometry(QRect(410, 80, 93, 28));
        lineEdit = new QLineEdit(bodyWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(130, 30, 371, 28));

        retranslateUi(SendFileClass);

        QMetaObject::connectSlotsByName(SendFileClass);
    } // setupUi

    void retranslateUi(QWidget *SendFileClass)
    {
        SendFileClass->setWindowTitle(QApplication::translate("SendFileClass", "SendFile", Q_NULLPTR));
        openBtn->setText(QApplication::translate("SendFileClass", "\346\211\223\345\274\200...", Q_NULLPTR));
        sendBtn->setText(QApplication::translate("SendFileClass", "\345\217\221\351\200\201", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SendFileClass: public Ui_SendFileClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENDFILE_H

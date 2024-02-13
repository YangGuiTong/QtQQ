/********************************************************************************
** Form generated from reading UI file 'Contactltem.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTACTLTEM_H
#define UI_CONTACTLTEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ContactltemClass
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QLabel *userName;
    QLabel *signName;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *ContactltemClass)
    {
        if (ContactltemClass->objectName().isEmpty())
            ContactltemClass->setObjectName(QStringLiteral("ContactltemClass"));
        ContactltemClass->resize(483, 54);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ContactltemClass->sizePolicy().hasHeightForWidth());
        ContactltemClass->setSizePolicy(sizePolicy);
        ContactltemClass->setMinimumSize(QSize(0, 54));
        ContactltemClass->setMaximumSize(QSize(16777215, 54));
        horizontalLayout = new QHBoxLayout(ContactltemClass);
        horizontalLayout->setSpacing(12);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(6, 0, 6, 0);
        label = new QLabel(ContactltemClass);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMinimumSize(QSize(40, 40));
        label->setMaximumSize(QSize(40, 40));

        horizontalLayout->addWidget(label);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalSpacer = new QSpacerItem(98, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        userName = new QLabel(ContactltemClass);
        userName->setObjectName(QStringLiteral("userName"));

        verticalLayout->addWidget(userName);

        signName = new QLabel(ContactltemClass);
        signName->setObjectName(QStringLiteral("signName"));

        verticalLayout->addWidget(signName);

        verticalSpacer_2 = new QSpacerItem(98, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(ContactltemClass);

        QMetaObject::connectSlotsByName(ContactltemClass);
    } // setupUi

    void retranslateUi(QWidget *ContactltemClass)
    {
        ContactltemClass->setWindowTitle(QApplication::translate("ContactltemClass", "Contactltem", Q_NULLPTR));
        label->setText(QString());
        userName->setText(QString());
        signName->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ContactltemClass: public Ui_ContactltemClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTACTLTEM_H

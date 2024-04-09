/********************************************************************************
** Form generated from reading UI file 'QtQQ_Server.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTQQ_SERVER_H
#define UI_QTQQ_SERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtQQ_ServerClass
{
public:
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QTableWidget *tableWidget;
    QWidget *widget;
    QLabel *label;
    QGroupBox *groupBox;
    QFrame *line;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QComboBox *employeeDepBox;
    QLineEdit *nameLineEdit;
    QPushButton *addBtn;
    QLabel *headLabel;
    QComboBox *departmentBox;
    QPushButton *queryDepartmentBtn;
    QPushButton *queryIDBtn;
    QLineEdit *queryIDLineEdit;
    QPushButton *logoutBtn;
    QLineEdit *logoutIDLineEdit;
    QPushButton *selectPictureBtn;
    QPushButton *deleteBtn;
    QLineEdit *deleteLineEdit;
    QLineEdit *lineEditPwd;
    QWidget *alterWidget;
    QPushButton *alterBtn;
    QLabel *alterHeadLabel;
    QLineEdit *alterNameLineEdit;
    QLineEdit *alterIDLineEdit;
    QLabel *label_5;
    QComboBox *alterEmployeeStatusBox;
    QLabel *label_8;
    QLabel *label_11;
    QLineEdit *alterSignLineEdit;
    QLineEdit *alterPwdLineEdit;
    QPushButton *alterSelectPictureBtn;
    QLabel *label_6;
    QLabel *label_10;
    QComboBox *alterEmployeeDepBox;
    QLabel *label_7;
    QLabel *label_9;

    void setupUi(QDialog *QtQQ_ServerClass)
    {
        if (QtQQ_ServerClass->objectName().isEmpty())
            QtQQ_ServerClass->setObjectName(QStringLiteral("QtQQ_ServerClass"));
        QtQQ_ServerClass->resize(999, 692);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QtQQ_ServerClass->sizePolicy().hasHeightForWidth());
        QtQQ_ServerClass->setSizePolicy(sizePolicy);
        QtQQ_ServerClass->setMinimumSize(QSize(999, 692));
        QtQQ_ServerClass->setMaximumSize(QSize(999, 692));
        scrollArea = new QScrollArea(QtQQ_ServerClass);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(1, 1, 700, 500));
        sizePolicy.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy);
        scrollArea->setMinimumSize(QSize(700, 500));
        scrollArea->setMaximumSize(QSize(700, 500));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 698, 498));
        tableWidget = new QTableWidget(scrollAreaWidgetContents);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(0, 0, 700, 500));
        sizePolicy.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy);
        tableWidget->setMinimumSize(QSize(700, 500));
        tableWidget->setMaximumSize(QSize(700, 500));
        scrollArea->setWidget(scrollAreaWidgetContents);
        widget = new QWidget(QtQQ_ServerClass);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(702, 1, 296, 800));
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 0, 250, 200));
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(0, 210, 299, 481));
        line = new QFrame(groupBox);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(0, 210, 299, 3));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 30, 72, 21));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 60, 72, 21));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 90, 72, 21));
        employeeDepBox = new QComboBox(groupBox);
        employeeDepBox->setObjectName(QStringLiteral("employeeDepBox"));
        employeeDepBox->setGeometry(QRect(86, 30, 111, 22));
        nameLineEdit = new QLineEdit(groupBox);
        nameLineEdit->setObjectName(QStringLiteral("nameLineEdit"));
        nameLineEdit->setGeometry(QRect(86, 60, 111, 21));
        addBtn = new QPushButton(groupBox);
        addBtn->setObjectName(QStringLiteral("addBtn"));
        addBtn->setGeometry(QRect(200, 170, 81, 28));
        headLabel = new QLabel(groupBox);
        headLabel->setObjectName(QStringLiteral("headLabel"));
        headLabel->setGeometry(QRect(90, 90, 81, 71));
        departmentBox = new QComboBox(groupBox);
        departmentBox->setObjectName(QStringLiteral("departmentBox"));
        departmentBox->setGeometry(QRect(10, 230, 161, 27));
        queryDepartmentBtn = new QPushButton(groupBox);
        queryDepartmentBtn->setObjectName(QStringLiteral("queryDepartmentBtn"));
        queryDepartmentBtn->setGeometry(QRect(200, 230, 81, 31));
        queryIDBtn = new QPushButton(groupBox);
        queryIDBtn->setObjectName(QStringLiteral("queryIDBtn"));
        queryIDBtn->setGeometry(QRect(200, 270, 81, 31));
        queryIDLineEdit = new QLineEdit(groupBox);
        queryIDLineEdit->setObjectName(QStringLiteral("queryIDLineEdit"));
        queryIDLineEdit->setGeometry(QRect(10, 270, 161, 31));
        logoutBtn = new QPushButton(groupBox);
        logoutBtn->setObjectName(QStringLiteral("logoutBtn"));
        logoutBtn->setGeometry(QRect(200, 320, 81, 31));
        logoutIDLineEdit = new QLineEdit(groupBox);
        logoutIDLineEdit->setObjectName(QStringLiteral("logoutIDLineEdit"));
        logoutIDLineEdit->setGeometry(QRect(10, 320, 161, 31));
        selectPictureBtn = new QPushButton(groupBox);
        selectPictureBtn->setObjectName(QStringLiteral("selectPictureBtn"));
        selectPictureBtn->setGeometry(QRect(200, 90, 81, 31));
        deleteBtn = new QPushButton(groupBox);
        deleteBtn->setObjectName(QStringLiteral("deleteBtn"));
        deleteBtn->setGeometry(QRect(200, 370, 81, 31));
        deleteLineEdit = new QLineEdit(groupBox);
        deleteLineEdit->setObjectName(QStringLiteral("deleteLineEdit"));
        deleteLineEdit->setGeometry(QRect(10, 370, 161, 31));
        lineEditPwd = new QLineEdit(groupBox);
        lineEditPwd->setObjectName(QStringLiteral("lineEditPwd"));
        lineEditPwd->setGeometry(QRect(10, 170, 161, 21));
        lineEditPwd->setMaxLength(16);
        alterWidget = new QWidget(QtQQ_ServerClass);
        alterWidget->setObjectName(QStringLiteral("alterWidget"));
        alterWidget->setEnabled(true);
        alterWidget->setGeometry(QRect(0, 500, 701, 191));
        alterBtn = new QPushButton(alterWidget);
        alterBtn->setObjectName(QStringLiteral("alterBtn"));
        alterBtn->setGeometry(QRect(340, 100, 111, 41));
        alterHeadLabel = new QLabel(alterWidget);
        alterHeadLabel->setObjectName(QStringLiteral("alterHeadLabel"));
        alterHeadLabel->setGeometry(QRect(90, 100, 85, 85));
        alterNameLineEdit = new QLineEdit(alterWidget);
        alterNameLineEdit->setObjectName(QStringLiteral("alterNameLineEdit"));
        alterNameLineEdit->setGeometry(QRect(540, 13, 161, 21));
        alterNameLineEdit->setMaxLength(10);
        alterNameLineEdit->setReadOnly(false);
        alterIDLineEdit = new QLineEdit(alterWidget);
        alterIDLineEdit->setObjectName(QStringLiteral("alterIDLineEdit"));
        alterIDLineEdit->setGeometry(QRect(290, 13, 161, 21));
        alterIDLineEdit->setReadOnly(true);
        label_5 = new QLabel(alterWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(14, 13, 72, 21));
        alterEmployeeStatusBox = new QComboBox(alterWidget);
        alterEmployeeStatusBox->setObjectName(QStringLiteral("alterEmployeeStatusBox"));
        alterEmployeeStatusBox->setGeometry(QRect(90, 53, 111, 22));
        label_8 = new QLabel(alterWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(210, 53, 72, 21));
        label_11 = new QLabel(alterWidget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(460, 53, 72, 21));
        alterSignLineEdit = new QLineEdit(alterWidget);
        alterSignLineEdit->setObjectName(QStringLiteral("alterSignLineEdit"));
        alterSignLineEdit->setGeometry(QRect(290, 53, 161, 21));
        alterSignLineEdit->setMaxLength(15);
        alterSignLineEdit->setReadOnly(false);
        alterPwdLineEdit = new QLineEdit(alterWidget);
        alterPwdLineEdit->setObjectName(QStringLiteral("alterPwdLineEdit"));
        alterPwdLineEdit->setGeometry(QRect(540, 53, 161, 21));
        alterPwdLineEdit->setMaxLength(10);
        alterPwdLineEdit->setReadOnly(false);
        alterSelectPictureBtn = new QPushButton(alterWidget);
        alterSelectPictureBtn->setObjectName(QStringLiteral("alterSelectPictureBtn"));
        alterSelectPictureBtn->setGeometry(QRect(200, 103, 71, 31));
        label_6 = new QLabel(alterWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(210, 13, 72, 21));
        label_10 = new QLabel(alterWidget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(10, 103, 72, 21));
        alterEmployeeDepBox = new QComboBox(alterWidget);
        alterEmployeeDepBox->setObjectName(QStringLiteral("alterEmployeeDepBox"));
        alterEmployeeDepBox->setGeometry(QRect(90, 13, 111, 22));
        label_7 = new QLabel(alterWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(460, 13, 72, 21));
        label_9 = new QLabel(alterWidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(14, 53, 72, 21));

        retranslateUi(QtQQ_ServerClass);

        QMetaObject::connectSlotsByName(QtQQ_ServerClass);
    } // setupUi

    void retranslateUi(QDialog *QtQQ_ServerClass)
    {
        QtQQ_ServerClass->setWindowTitle(QApplication::translate("QtQQ_ServerClass", "QtQQ_Server", Q_NULLPTR));
        label->setText(QString());
        groupBox->setTitle(QString());
        label_2->setText(QApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\351\203\250\351\227\250\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\345\247\223\345\220\215\357\274\232", Q_NULLPTR));
        label_4->setText(QApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\345\257\270\347\205\247\357\274\232", Q_NULLPTR));
        nameLineEdit->setPlaceholderText(QApplication::translate("QtQQ_ServerClass", "\350\257\267\350\276\223\345\205\245\345\221\230\345\267\245\345\247\223\345\220\215", Q_NULLPTR));
        addBtn->setText(QApplication::translate("QtQQ_ServerClass", "\346\226\260\345\242\236\345\221\230\345\267\245", Q_NULLPTR));
        headLabel->setText(QApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\345\257\270\347\205\247", Q_NULLPTR));
        queryDepartmentBtn->setText(QApplication::translate("QtQQ_ServerClass", "\346\237\245\350\257\242\345\221\230\345\267\245", Q_NULLPTR));
        queryIDBtn->setText(QApplication::translate("QtQQ_ServerClass", "\346\237\245\350\257\242\345\221\230\345\267\245", Q_NULLPTR));
        queryIDLineEdit->setPlaceholderText(QApplication::translate("QtQQ_ServerClass", "\350\257\267\350\276\223\345\205\245\345\221\230\345\267\245ID\345\217\267", Q_NULLPTR));
        logoutBtn->setText(QApplication::translate("QtQQ_ServerClass", "\346\263\250\351\224\200\345\221\230\345\267\245", Q_NULLPTR));
        logoutIDLineEdit->setPlaceholderText(QApplication::translate("QtQQ_ServerClass", "\350\257\267\350\276\223\345\205\245\345\221\230\345\267\245ID\345\217\267", Q_NULLPTR));
        selectPictureBtn->setText(QApplication::translate("QtQQ_ServerClass", "...", Q_NULLPTR));
        deleteBtn->setText(QApplication::translate("QtQQ_ServerClass", "\345\210\240\351\231\244\345\221\230\345\267\245", Q_NULLPTR));
        deleteLineEdit->setPlaceholderText(QApplication::translate("QtQQ_ServerClass", "\350\257\267\350\276\223\345\205\245\345\221\230\345\267\245ID\345\217\267", Q_NULLPTR));
        lineEditPwd->setPlaceholderText(QApplication::translate("QtQQ_ServerClass", "\350\257\267\350\276\223\345\205\245\345\221\230\345\267\245\345\257\206\347\240\201", Q_NULLPTR));
        alterBtn->setText(QApplication::translate("QtQQ_ServerClass", "\344\277\256\346\224\271", Q_NULLPTR));
        alterHeadLabel->setText(QApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\345\257\270\347\205\247", Q_NULLPTR));
        alterNameLineEdit->setPlaceholderText(QString());
        alterIDLineEdit->setPlaceholderText(QString());
        label_5->setText(QApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\351\203\250\351\227\250\357\274\232", Q_NULLPTR));
        label_8->setText(QApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\347\255\276\345\220\215\357\274\232", Q_NULLPTR));
        label_11->setText(QApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\345\257\206\347\240\201\357\274\232", Q_NULLPTR));
        alterSignLineEdit->setPlaceholderText(QString());
        alterPwdLineEdit->setPlaceholderText(QString());
        alterSelectPictureBtn->setText(QApplication::translate("QtQQ_ServerClass", "...", Q_NULLPTR));
        label_6->setText(QApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\345\267\245\345\217\267\357\274\232", Q_NULLPTR));
        label_10->setText(QApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\345\257\270\347\205\247\357\274\232", Q_NULLPTR));
        label_7->setText(QApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\345\247\223\345\220\215\357\274\232", Q_NULLPTR));
        label_9->setText(QApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\347\212\266\346\200\201\357\274\232", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QtQQ_ServerClass: public Ui_QtQQ_ServerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTQQ_SERVER_H

#pragma once

#include "BasicWindow.h"
#include "ui_RecviveFile.h"

class RecviveFile : public BasicWindow
{
	Q_OBJECT

public:
	RecviveFile(QWidget *parent = nullptr);
	~RecviveFile();
	void setMsg(QString &msgLabel);


signals:
	void refuseFile();


private slots:
	void on_cancelBtn_clicked();
	void on_okBtn_clicked();



private:
	Ui::RecviveFileClass ui;
};

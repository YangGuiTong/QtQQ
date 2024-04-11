#pragma once

#include "BasicWindow.h"
#include "ui_UserLogin.h"

class UserLogin : public BasicWindow
{
	Q_OBJECT

public:
	UserLogin(QWidget *parent = nullptr);
	~UserLogin();

private slots:
	void onLoginBtnClicked();

private:
	void initControl();
	bool connectMySql();
	bool verfyAccountCode(bool &isAccountLogin, QString &strAccount);
	void initTemporaryAP();	// ��ʼ�����������

private:
	Ui::UserLogin ui;
};

#pragma once

#include "BasicWindow.h"
#include "ui_UserLogin.h"

class UserLogin : public BasicWindow
{
	Q_OBJECT

public:
	UserLogin(BasicWindow *parent = nullptr);
	~UserLogin();

private slots:
	void onLoginBtnClicked();

private:
	void initControl();

private:
	Ui::UserLoginClass ui;
};

#pragma once

#include <QtWidgets/QWidget>
#include "ui_CCMainWindow.h"

#include "MyLogger.h"
#include "BasicWindow.h"

class CCMainWindow : public BasicWindow
{
    Q_OBJECT

public:
    CCMainWindow(QWidget *parent = nullptr);
    ~CCMainWindow();

	void initControl();
	void setUserName(const QString &username);
	void setLevePixmap(int level);
	void setHeadPixmap(const QString &headPath);

private:
    Ui::CCMainWindowClass ui;
};

#pragma once

#include <QtWidgets/QWidget>
#include "ui_CCMainWindow.h"

#include "BasicWindow.h"

class CCMainWindow : public BasicWindow
{
    Q_OBJECT

public:
    CCMainWindow(QWidget *parent = nullptr);
    ~CCMainWindow();

	void initTimer();										// 初始化等级计时器
	void initControl();
	void setUserName(const QString &username);
	void setLevePixmap(int level);
	void setHeadPixmap(const QString &headPath);			// 设置头像
	void setStatusMenuIcon(const QString &statusPath);		// 设置在线状态

	// 添加应用部件(app图片路径，app部件对象名)
	QWidget *addOtherAppExtension(const QString &appPath, const QString appName);


private slots:
	void onAppIconClicked();

private:
    Ui::CCMainWindowClass ui;
};

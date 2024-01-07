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

	void initTimer();										// ��ʼ���ȼ���ʱ��
	void initControl();
	void setUserName(const QString &username);
	void setLevePixmap(int level);
	void setHeadPixmap(const QString &headPath);			// ����ͷ��
	void setStatusMenuIcon(const QString &statusPath);		// ��������״̬

	// ���Ӧ�ò���(appͼƬ·����app����������)
	QWidget *addOtherAppExtension(const QString &appPath, const QString appName);


private slots:
	void onAppIconClicked();

private:
    Ui::CCMainWindowClass ui;
};

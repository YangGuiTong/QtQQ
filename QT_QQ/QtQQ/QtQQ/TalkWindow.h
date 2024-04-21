#pragma once

#include <QWidget>
#include "ui_TalkWindow.h"
#include "public_type.h"
#include "ScreenShotWidget.h"
#include "SendFile.h"


class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent, const QString &uid);
	~TalkWindow();

public:
	void addEmotionImage(int emotionNum);
	void setWindowName(const QString &name);

	QString GetTalkId();

private:
	void initControl();
	void initGroupTalkStatus();

	int getCompDepID();			// 获取公司群ID

	void initPtoPTalk();		// 初始化单聊
	void initTalkWindow();		// 初始化群聊
	//void initCompanyTalk();		// 初始化公司群聊天
	//void initPersonelTalk();	// 初始化人事部聊天
	//void initMarketTalk();		// 初始化市场部聊天
	//void initDevelopTalk();		// 初始化研发部聊天

	void addPeopInfo(QTreeWidgetItem *pRootGroupItem, int employeeID);

private slots:
	void onItemDoubleClicked(QTreeWidgetItem * item, int column);

	void onSendBtnClicked(bool);

	void onFileOpenBtnClicked(bool);

	void onScreenshotBtnClicked(bool);
	

private:
	Ui::TalkWindow ui;

	QString m_talkId;
	QMap<QTreeWidgetItem *, QString> m_groupPeopleMap;	// 所有分组联系人姓名
	bool m_isGroupTalk;		// 判断是否为群聊


	ScreenShotWidget *m_widget;	// 截图类
	SendFile *m_sendFile;			// 发送文件类，选择文件

	friend class TalkWindowSheel;
};
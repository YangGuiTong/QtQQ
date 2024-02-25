#pragma once

#include <QWidget>
#include "ui_TalkWindowSheel.h"
#include "BasicWindow.h"
#include "EmotionWindow.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"


#include <QListWidget>
#include <QMap>

class QListWidgetItem;


enum GroupType {
	COMPANY				= 0,		// 公司群
	PERSONELGROUP		= 1,		// 人事部
	DEVELOPMENTGROUP	= 2,		// 研发部
	MARKETGROUP			= 3,		// 市场部
	PTOP				= 4,		// 同事之间单独聊天

	GROUP_TYPE_COUNT
};

class TalkWindowSheel : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowSheel(QWidget *parent = nullptr);
	~TalkWindowSheel();

public:
	// 添加新的聊天窗口
	void addTalkWindow(TalkWindow *talkWindow, TalkWindowItem *talkWindowItem, GroupType grouptype);
	// 设置当前聊天窗口
	void setCurrentWidget(QWidget *widget);

private:
	void initControl();	// 初始化控件

public slots:
	void onEmotionBtnClicked(bool);		// 表情按钮点击后执行的槽函数

private slots:
	void onTalkWindowItemClicked(QListWidgetItem *item);	// 左侧列表点击后执行的槽函数
	void onEmotionItemClicked(int emotionNum);				// 表情被选中

private:
	Ui::TalkWindowClass ui;
	QMap<QListWidgetItem *, QWidget *> m_talkwindowItemMap;	// 打开的聊天窗口
	EmotionWindow *m_emotionWindow;		// 表情窗口
};

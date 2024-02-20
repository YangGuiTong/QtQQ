#pragma once

#include <QWidget>
#include "ui_TalkWindowSheel.h"
#include "BasicWindow.h"
#include "EmotionWindow.h"


#include <QMap>

class TalkWindow;
class TalkWindowItem;
class QListWidgetItem;


class TalkWindowSheel : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowSheel(QWidget *parent = nullptr);
	~TalkWindowSheel();

public:
	// 添加新的聊天窗口
	void addTalkWindow(TalkWindow *talkWindow, TalkWindowItem *talkWindowItem);
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

#pragma once

#include <QObject>

#include "TalkWindowSheel.h"

class WindowManager  : public QObject
{
	Q_OBJECT

public:
	WindowManager();
	~WindowManager();

public:
	QString findWindowName(QWidget *qsWindow);	// 根据窗体寻找名字

	QWidget *findWindowName(const QString &qsWindowName);
	void deleteWindowName(const QString &qsWindowName);
	void addWindowName(const QString &qsWindowName, QWidget *qWidget);

	static WindowManager *getInstance();
	void addNewTalkWindow(const QString &uid);

	TalkWindowSheel *getTalkWindowSheel();
	QString getCreatingTalkId();

private:
	TalkWindowSheel *m_talkwindowshell;
	QMap<QString, QWidget *> m_windowMap;
	QString m_strCreatingTalkId = "";		// 正在创建的聊天窗口（QQ号）
};

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
	QWidget *findWindowName(const QString &qsWindowName);
	void deleteWindowName(const QString &qsWindowName);
	void addWindowName(const QString &qsWindowName, QWidget *qWidget);

	static WindowManager *getInstance();
	void addNewTalkWindow(const QString &uid);

private:
	TalkWindowSheel *m_talkwindowshell;
	QMap<QString, QWidget *> m_windowMap;
};

#pragma once

#include <QWidget>
#include <QSystemTrayIcon>

#include "public_type.h"

class SysTray  : public QSystemTrayIcon
{
	Q_OBJECT

public:
	SysTray(QWidget *parent);
	~SysTray();

public slots:
	void onIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
	void initSystemTray();
	void addSystrayMenu();

private:
	QWidget *m_parent;
};

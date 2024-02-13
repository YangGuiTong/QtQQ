#pragma once

#include <QWidget>
#include "ui_Contactltem.h"

class Contactltem : public QWidget
{
	Q_OBJECT

public:
	Contactltem(QWidget *parent = nullptr);
	~Contactltem();

	void setUserName(const QString &userName);
	void setSignName(const QString &signName);
	void setHeadPixmap(const QPixmap &headPath);
	QString getUserName() const;
	QSize getHeadLabelSize() const;

private:
	void initControl();

private:
	Ui::ContactltemClass ui;
};

#pragma once

#include <QWidget>
#include "ui_TalkWindowItem.h"

#include <QPixmap>

class TalkWindowItem : public QWidget
{
	Q_OBJECT

public:
	TalkWindowItem(QWidget *parent = nullptr);
	~TalkWindowItem();


	void setHeadPixmap(const QString &headPath);
	void setMsgLabelContent(const QString &msg);
	QString getMsgLabelText();

private:
	void initControl();

signals:
	void signalCloseClicked();

private:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	Ui::TalkWindowItemClass ui;
};

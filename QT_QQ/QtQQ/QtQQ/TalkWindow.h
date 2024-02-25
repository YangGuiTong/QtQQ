#pragma once

#include <QWidget>
#include "ui_TalkWindow.h"

class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent = nullptr);
	~TalkWindow();

public:
	void addEmotionImage(int emotionNum);

private:
	Ui::TalkWindow ui;
};

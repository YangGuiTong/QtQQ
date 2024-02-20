#pragma once

#include <QWidget>
#include "ui_TalkWindowSheel.h"

class TalkWindowSheel : public QWidget
{
	Q_OBJECT

public:
	TalkWindowSheel(QWidget *parent = nullptr);
	~TalkWindowSheel();

private:
	Ui::TalkWindowClass ui;
};

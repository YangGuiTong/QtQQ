#pragma once

#include <QWidget>
#include "ui_SkinWindow.h"

#include "BasicWindow.h"

class SkinWindow : public BasicWindow
{
	Q_OBJECT

public:
	SkinWindow(QWidget *parent = nullptr);
	~SkinWindow();

public slots:
	void onShowClose();


public:
	void initControl();

private:
	Ui::SkinWindow ui;
};

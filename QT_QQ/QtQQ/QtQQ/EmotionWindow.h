#pragma once

#include <QWidget>
#include "ui_EmotionWindow.h"

class EmotionWindow : public QWidget
{
	Q_OBJECT

public:
	EmotionWindow(QWidget *parent = nullptr);
	~EmotionWindow();

private:
	void initControl();


private slots:
	void addEmotion(int emotionNum);


signals:
	void signalEmotionWindowHide();
	void signalEmotionItemClicked(int emotionNum);

private:
	void paintEvent(QPaintEvent *event);

private:
	Ui::EmotionWindow ui;
};

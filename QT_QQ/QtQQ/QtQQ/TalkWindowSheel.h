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
	// ����µ����촰��
	void addTalkWindow(TalkWindow *talkWindow, TalkWindowItem *talkWindowItem);
	// ���õ�ǰ���촰��
	void setCurrentWidget(QWidget *widget);

private:
	void initControl();	// ��ʼ���ؼ�

public slots:
	void onEmotionBtnClicked(bool);		// ���鰴ť�����ִ�еĲۺ���

private slots:
	void onTalkWindowItemClicked(QListWidgetItem *item);	// ����б�����ִ�еĲۺ���
	void onEmotionItemClicked(int emotionNum);				// ���鱻ѡ��

private:
	Ui::TalkWindowClass ui;
	QMap<QListWidgetItem *, QWidget *> m_talkwindowItemMap;	// �򿪵����촰��
	EmotionWindow *m_emotionWindow;		// ���鴰��
};

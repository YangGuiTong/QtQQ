#pragma once

#include <QWidget>
#include "ui_TalkWindow.h"
#include "public_type.h"
#include "ScreenShotWidget.h"
#include "SendFile.h"


class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent, const QString &uid);
	~TalkWindow();

public:
	void addEmotionImage(int emotionNum);
	void setWindowName(const QString &name);

	QString GetTalkId();

private:
	void initControl();
	void initGroupTalkStatus();

	int getCompDepID();			// ��ȡ��˾ȺID

	void initPtoPTalk();		// ��ʼ������
	void initTalkWindow();		// ��ʼ��Ⱥ��
	//void initCompanyTalk();		// ��ʼ����˾Ⱥ����
	//void initPersonelTalk();	// ��ʼ�����²�����
	//void initMarketTalk();		// ��ʼ���г�������
	//void initDevelopTalk();		// ��ʼ���з�������

	void addPeopInfo(QTreeWidgetItem *pRootGroupItem, int employeeID);

private slots:
	void onItemDoubleClicked(QTreeWidgetItem * item, int column);

	void onSendBtnClicked(bool);

	void onFileOpenBtnClicked(bool);

	void onScreenshotBtnClicked(bool);
	

private:
	Ui::TalkWindow ui;

	QString m_talkId;
	QMap<QTreeWidgetItem *, QString> m_groupPeopleMap;	// ���з�����ϵ������
	bool m_isGroupTalk;		// �ж��Ƿ�ΪȺ��


	ScreenShotWidget *m_widget;	// ��ͼ��
	SendFile *m_sendFile;			// �����ļ��࣬ѡ���ļ�

	friend class TalkWindowSheel;
};
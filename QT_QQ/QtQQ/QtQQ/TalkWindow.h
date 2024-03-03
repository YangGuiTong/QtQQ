#pragma once

#include <QWidget>
#include "ui_TalkWindow.h"
#include "public_type.h"


class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent, const QString &uid);
	~TalkWindow();

public:
	void addEmotionImage(int emotionNum);
	void setWindowName(const QString &name);


private:
	void initControl();
	void initPtoPTalk();		// ��ʼ������
	void initCompanyTalk();		// ��ʼ����˾Ⱥ����
	void initPersonelTalk();	// ��ʼ�����²�����
	void initMarketTalk();		// ��ʼ���г�������
	void initDevelopTalk();		// ��ʼ���з�������

	void addPeopInfo(QTreeWidgetItem *pRootGroupItem);

private slots:
	void onItemDoubleClicked(QTreeWidgetItem * item, int column);

	void onSendBtnClicked(bool);

	

private:
	Ui::TalkWindow ui;

	QString m_talkId;
	QMap<QTreeWidgetItem *, QString> m_groupPeopleMap;	// ���з�����ϵ������
};

#pragma once

#include <QWidget>
#include "ui_TalkWindow.h"
#include "public_type.h"


class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent, const QString &uid, GroupType groupType);
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
	void onSendBtnClicked();
	void onItemDoubleClicked(QTreeWidgetItem * item, int column);

private:
	Ui::TalkWindow ui;

	QString m_talkId;
	GroupType m_groupType;
	QMap<QTreeWidgetItem *, QString> m_groupPeopleMap;	// ���з�����ϵ������
};

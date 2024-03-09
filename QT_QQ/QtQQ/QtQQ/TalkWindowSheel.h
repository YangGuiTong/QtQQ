#pragma once

#include <QWidget>
#include "ui_TalkWindowSheel.h"
#include "BasicWindow.h"
#include "EmotionWindow.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"


#include <QListWidget>
#include <QMap>
#include <QTcpSocket>
#include <QUdpSocket>


class TalkWindowSheel : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowSheel(QWidget *parent = nullptr);
	~TalkWindowSheel();

public:
	// ����µ����촰��
	void addTalkWindow(TalkWindow *talkWindow, TalkWindowItem *talkWindowItem, QString uid);
	// ���õ�ǰ���촰��
	void setCurrentWidget(QWidget *widget);

	const QMap<QListWidgetItem *, QWidget *>& getTalkWindowItemMap() const;

private:
	void initControl();			// ��ʼ���ؼ�
	void initTcpSocket();		// ��ʼ��TCP
	void initUdpSocket();		// ��ʼ��UDP

	QStringList getEmployeeID();	// ��ȡ����Ա��QQ��
	bool createJSFile(QStringList &employeesList);

public slots:
	void onEmotionBtnClicked(bool);		// ���鰴ť�����ִ�еĲۺ���

	// �ͻ��˷���TCP���ݣ����������ݣ��������ͣ��ļ���
	void updateSendTcpMsg(QString &strData, int &msgType, QString fileName = "");

	

private slots:
	void onTalkWindowItemClicked(QListWidgetItem *item);	// ����б�����ִ�еĲۺ���
	void onEmotionItemClicked(int emotionNum);				// ���鱻ѡ��

	// ����UDP�㲥�յ�������
	void processPendingData();

private:
	Ui::TalkWindowClass ui;
	QMap<QListWidgetItem *, QWidget *> m_talkwindowItemMap;	// �򿪵����촰��
	EmotionWindow *m_emotionWindow;		// ���鴰��

private:
	QTcpSocket *m_tcpClientSocket;		// tcp�ͻ���
	QUdpSocket *m_udpReceiver;			// udp���ն�
};

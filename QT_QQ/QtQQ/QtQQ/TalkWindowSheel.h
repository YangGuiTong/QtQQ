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
	// 添加新的聊天窗口
	void addTalkWindow(TalkWindow *talkWindow, TalkWindowItem *talkWindowItem, QString uid);
	// 设置当前聊天窗口
	void setCurrentWidget(QWidget *widget);

	const QMap<QListWidgetItem *, QWidget *>& getTalkWindowItemMap() const;

private:
	void initControl();			// 初始化控件
	void initTcpSocket();		// 初始化TCP
	void initUdpSocket();		// 初始化UDP

	QStringList getEmployeeID();	// 获取所有员工QQ号
	bool createJSFile(QStringList &employeesList);

	// 根据员工QQ号获取员工名字
	QString getEmployeeName(int employeesID);

	void handleReceivedMsg(QString senderEmployeeID, int msgType, QString strMsg);

	void LoadMessage(QString senderID, int msgType, QString strMsg);

signals:
	void signalReload();

public slots:
	void onEmotionBtnClicked(bool);		// 表情按钮点击后执行的槽函数

	// 客户端发送TCP数据（参数：数据，数据类型，文件）
	void updateSendTcpMsg(QString &strData, int &msgType, QString fileName = "");

	void onLoadMessage();

private slots:
	void onTalkWindowItemClicked(QListWidgetItem *item);	// 左侧列表点击后执行的槽函数
	void onEmotionItemClicked(int emotionNum);				// 表情被选中

	// 处理UDP广播收到的数据
	void processPendingData();

private:
	Ui::TalkWindowClass ui;
	QMap<QListWidgetItem *, QWidget *> m_talkwindowItemMap;	// 打开的聊天窗口
	EmotionWindow *m_emotionWindow;		// 表情窗口

private:
	QTcpSocket *m_tcpClientSocket;		// tcp客户端
	QUdpSocket *m_udpReceiver;			// udp接收端

	// 第一个int代表接收方的QQ号，QJsonArray代表聊天记录
	//QMap<int, QJsonArray> m_message_info;	// 聊天记录
	/*
		QJsonArray格式如下:

		[
			{ "sender": "12345",    "message": "内容" },
			{ "sender": "54321",    "message": "内容" },
			{ "sender": "54321",    "message": "内容" },
			{ "sender": "12345",    "message": "内容" } 
		]
	*/
};

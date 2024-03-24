#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>


class TcpServer : public QTcpServer {
	Q_OBJECT

public:
	TcpServer(int prot);
	~TcpServer();


public:
	bool run();		// 监听


protected:
	// 当客户端有新的连接时
	void incomingConnection(qintptr socketDescriptor) override;


signals:
	void signalTcpMsgComes(QByteArray &btData);


private slots:
	// 处理数据
	void SocketDataProcessiong(QByteArray &SendData, int descriptor);
	// 断开连接处理
	void SocketDisconnecgted(int descriptor);


private:
	// 数据解析，聊天记保存数据库
	void processPendingData(QByteArray &SendData);

	// 將聊天記錄插入Map中和數據庫中，参数三是信息类型，0表示表情，1表示文本；参数四个群聊标志，1表示群聊，0表示单聊
	void MessageInsert(const QString sender, const QString receiver, const int msgType, const int groupType, const QString strMsg, const int second = 0);
	
	// 将聊天记录重新插入数据库，参数三是更新标志，1更新，0插入；参数四是聊天记录
	void MessageSaveDataBase(const QString sender, const QString receiver, const int updateFlag, const QJsonArray messageArr);

private:
	qint16 m_port;		// 端口号
	QList<QTcpSocket *> m_tcpSocketConnectList;


};

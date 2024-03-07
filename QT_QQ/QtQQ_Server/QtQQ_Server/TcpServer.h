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
	void incommingConnection(qintptr socketDescriptor);


signals:
	void signalTcpMsgComes(QByteArray &btData);


private slots:
	// 处理数据
	void SocketDataProcessiong(QByteArray &SendData, int descriptor);
	// 断开连接处理
	void SocketDisconnecgted(int descriptor);



private:
	int m_port;		// 端口号
	QList<QTcpSocket *> m_tcpSocketConnectList;
};

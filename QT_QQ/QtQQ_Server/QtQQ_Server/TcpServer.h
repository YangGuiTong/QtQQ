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
	bool run();		// ����


protected:
	// ���ͻ������µ�����ʱ
	void incommingConnection(qintptr socketDescriptor);


signals:
	void signalTcpMsgComes(QByteArray &btData);


private slots:
	// ��������
	void SocketDataProcessiong(QByteArray &SendData, int descriptor);
	// �Ͽ����Ӵ���
	void SocketDisconnecgted(int descriptor);



private:
	int m_port;		// �˿ں�
	QList<QTcpSocket *> m_tcpSocketConnectList;
};

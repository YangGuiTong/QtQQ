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
	void incomingConnection(qintptr socketDescriptor) override;


signals:
	void signalTcpMsgComes(QByteArray &btData);


private slots:
	// ��������
	void SocketDataProcessiong(QByteArray &SendData, int descriptor);
	// �Ͽ����Ӵ���
	void SocketDisconnecgted(int descriptor);



private:
	qint16 m_port;		// �˿ں�
	QList<QTcpSocket *> m_tcpSocketConnectList;
};

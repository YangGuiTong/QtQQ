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
	// ���ݽ���������Ǳ������ݿ�
	void processPendingData(QByteArray &SendData);

	// ������ӛ䛲���Map�к͔������У�����������Ϣ���ͣ�0��ʾ���飬1��ʾ�ı��������ĸ�Ⱥ�ı�־��1��ʾȺ�ģ�0��ʾ����
	void MessageInsert(const QString sender, const QString receiver, const int msgType, const int groupType, const QString strMsg, const int second = 0);
	
	// �������¼���²������ݿ⣬�������Ǹ��±�־��1���£�0���룻�������������¼
	void MessageSaveDataBase(const QString sender, const QString receiver, const int updateFlag, const QJsonArray messageArr);

private:
	qint16 m_port;		// �˿ں�
	QList<QTcpSocket *> m_tcpSocketConnectList;


};

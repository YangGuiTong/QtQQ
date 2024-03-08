#pragma once

#include <QTcpSocket>

class TcpSocket : public QTcpSocket {
	Q_OBJECT

public:
	TcpSocket();
	~TcpSocket();

	void run();

signals:
	void signalGetDataFromClient(QByteArray &, int);	// �ӿͻ����յ����ݺ����ź�server��������Ҫ����
	void signalClientDisconnect(int);					// ����server�пͻ��˶Ͽ�����


private slots:
	void onReceiveData();			// ����readyRead�źŶ�ȡ������
	void onClientDisconnect();		// �ͻ��˶Ͽ�����

private:
	int m_descriptor;		// ������������Ψһ��ʶ
};

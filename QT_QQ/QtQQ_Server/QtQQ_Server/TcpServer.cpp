#include "TcpServer.h"
#include "TcpSocket.h"
#include "public_type.h"

TcpServer::TcpServer(int prot) : m_port(prot) { }

TcpServer::~TcpServer() { }

bool TcpServer::run() {

	if (this->listen(QHostAddress::AnyIPv4, m_port)) {
		QString text = QString::fromLocal8Bit(QString("�����������˿� %1 �ɹ���").arg(m_port).toUtf8());
		MyLogDEBUG(text.toUtf8());
		qDebug() << QString::fromLocal8Bit(text.toUtf8());

		return true;
	}

	MyLogDEBUG(QString("�����������˿� %1 ʧ�ܣ�").arg(m_port).toUtf8());
	qDebug() << QString::fromLocal8Bit(QString("�����������˿� %1 ʧ�ܣ�").arg(m_port).toLocal8Bit());
	
	return false;
}

void TcpServer::incomingConnection(qintptr socketDescriptor) {
	QString text = QString("�µ����ӣ�%1").arg(socketDescriptor);
	MyLogDEBUG(text .toUtf8());
	qDebug() << QString::fromLocal8Bit(text.toLocal8Bit());
	
	TcpSocket *tcpsocket = new TcpSocket();
	tcpsocket->setSocketDescriptor(socketDescriptor);
	tcpsocket->run();

	// �յ��ͻ������ݺ�server���д���
	//connect(tcpsocket, SIGNAL(signalGetDataFromClient(QByteArray &, int)), 
	//		this, SLOT(SocketDataProcessiong(QByteArray &SendData, int descriptor)));
	connect(tcpsocket, &TcpSocket::signalGetDataFromClient, this, &TcpServer::SocketDataProcessiong);

	// �յ��ͻ��˶Ͽ����Ӻ�server���д���
	connect(tcpsocket, &TcpSocket::signalClientDisconnect, this, &TcpServer::SocketDisconnecgted);

	// ��socket��ӵ�������
	m_tcpSocketConnectList.append(tcpsocket);
}

void TcpServer::SocketDataProcessiong(QByteArray & SendData, int descriptor) {


	for (int i = 0; i < m_tcpSocketConnectList.size(); i++) {
		QTcpSocket *item = m_tcpSocketConnectList.at(i);
		int des = item->socketDescriptor();	// ��ȡtcpsocket������
		if (des == descriptor) {
			QString text = QString("����IP�� %1  ���������ݣ�%2")
				.arg(item->peerAddress().toString()).arg(QString(SendData));
			MyLogDEBUG(text .toUtf8());
			qDebug() << QString::fromLocal8Bit(text.toLocal8Bit());

			// �㲥
			emit signalTcpMsgComes(SendData);
		}

	}
}

void TcpServer::SocketDisconnecgted(int descriptor) { 
	for (int i = 0; i < m_tcpSocketConnectList.size(); i++) {
		QTcpSocket *item = m_tcpSocketConnectList.at(i);
		int des = item->socketDescriptor();	// ��ȡtcpsocket������
		if (des == descriptor || -1 == des) {
			m_tcpSocketConnectList.removeAt(i);
			item->deleteLater();

			QString text = QString("tcpSocket��������%1  IP�� %2  �Ͽ�����!").arg(descriptor).arg(item->peerAddress().toString());
			MyLogDEBUG(text .toUtf8());
			qDebug() << QString::fromLocal8Bit(text.toLocal8Bit());

			break;
		}

	}
}

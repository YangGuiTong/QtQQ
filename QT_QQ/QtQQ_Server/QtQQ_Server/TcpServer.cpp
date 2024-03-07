#include "TcpServer.h"
#include "public_type.h"


TcpServer::TcpServer(int prot) { }

TcpServer::~TcpServer() { }

bool TcpServer::run() {

	if (this->listen(QHostAddress::Any, m_port)) {
		MyLogDEBUG(QString("����ļ����˿� %1 �ɹ���").arg(m_port).toUtf8());
		qDebug() << QString("����ļ����˿� %1 �ɹ���").arg(m_port);

		return true;
	}

	MyLogDEBUG(QString("����ļ����˿� %1 ʧ�ܣ�").arg(m_port).toUtf8());
	qDebug() << QString("����ļ����˿� %1 ʧ�ܣ�").arg(m_port);

	return false;
}

void TcpServer::incommingConnection(qintptr socketDescriptor) {

}

void TcpServer::SocketDataProcessiong(QByteArray & SendData, int descriptor) {
	
	

	for (int i = 0; i < m_tcpSocketConnectList.size(); i++) {
		QTcpSocket *item = m_tcpSocketConnectList.at(i);
		int des = item->socketDescriptor();	// ��ȡtcpsocket������
		if (des == descriptor) {
			QString text = QString("����IP�� %1  ���������ݣ�%2")
				.arg(item->peerAddress().toString()).arg(QString(SendData));
			MyLogDEBUG(text .toUtf8());
			qDebug() << text;

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

			QString text = QString("tcpSocket��������%1  IP�� %2  �Ͽ�����!")
				.arg(descriptor).arg(item->peerAddress().toString());
			MyLogDEBUG(text .toUtf8());
			qDebug() << text;

			break;
		}

	}
}

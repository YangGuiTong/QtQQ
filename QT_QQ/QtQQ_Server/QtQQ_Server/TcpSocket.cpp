#include "TcpSocket.h"
#include "public_type.h"

TcpSocket::TcpSocket() { }

TcpSocket::~TcpSocket() { }

void TcpSocket::run() {
	// ����������
	m_descriptor = this->socketDescriptor();

	connect(this, SIGNAL(readyRead()), this, SLOT(onReceiveData()));
	connect(this, SIGNAL(disconnected()), this, SLOT(onClientDisconnect()));
}

void TcpSocket::onReceiveData() {
	QByteArray buffer = this->readAll();

	if (!buffer.isEmpty()) {
		QString strData = QString::fromLocal8Bit(buffer);

		//QString text = QString("�յ������� %1 �����ݣ�%2")
		//	.arg(m_descriptor).arg(strData);
		//MyLogDEBUG(text .toUtf8());
		//qDebug() << text;

		// ������յ��˿ͻ������ݵ��ź�
		emit signalGetDataFromClient(buffer, m_descriptor);
	}
}

void TcpSocket::onClientDisconnect() {
	emit signalClientDisconnect(m_descriptor);
}

#include "TcpSocket.h"
#include "public_type.h"

TcpSocket::TcpSocket() { }

TcpSocket::~TcpSocket() { }

void TcpSocket::run() {
	// 设置描述符
	m_descriptor = this->socketDescriptor();

	connect(this, SIGNAL(readyRead()), this, SLOT(onReceiveData()));
	connect(this, SIGNAL(disconnected()), this, SLOT(onClientDisconnect()));
}

void TcpSocket::onReceiveData() {
	QByteArray buffer = this->readAll();

	if (!buffer.isEmpty()) {
		QString strData = QString::fromLocal8Bit(buffer);

		//QString text = QString("收到描述符 %1 的数据：%2")
		//	.arg(m_descriptor).arg(strData);
		//MyLogDEBUG(text .toUtf8());
		//qDebug() << text;

		// 发射接收到了客户端数据的信号
		emit signalGetDataFromClient(buffer, m_descriptor);
	}
}

void TcpSocket::onClientDisconnect() {
	emit signalClientDisconnect(m_descriptor);
}

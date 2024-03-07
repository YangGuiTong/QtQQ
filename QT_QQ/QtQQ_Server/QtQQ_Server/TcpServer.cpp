#include "TcpServer.h"
#include "public_type.h"


TcpServer::TcpServer(int prot) { }

TcpServer::~TcpServer() { }

bool TcpServer::run() {

	if (this->listen(QHostAddress::Any, m_port)) {
		MyLogDEBUG(QString("服务的监听端口 %1 成功！").arg(m_port).toUtf8());
		qDebug() << QString("服务的监听端口 %1 成功！").arg(m_port);

		return true;
	}

	MyLogDEBUG(QString("服务的监听端口 %1 失败！").arg(m_port).toUtf8());
	qDebug() << QString("服务的监听端口 %1 失败！").arg(m_port);

	return false;
}

void TcpServer::incommingConnection(qintptr socketDescriptor) {

}

void TcpServer::SocketDataProcessiong(QByteArray & SendData, int descriptor) {
	
	

	for (int i = 0; i < m_tcpSocketConnectList.size(); i++) {
		QTcpSocket *item = m_tcpSocketConnectList.at(i);
		int des = item->socketDescriptor();	// 获取tcpsocket描述符
		if (des == descriptor) {
			QString text = QString("来自IP： %1  发来的数据：%2")
				.arg(item->peerAddress().toString()).arg(QString(SendData));
			MyLogDEBUG(text .toUtf8());
			qDebug() << text;

			// 广播
			emit signalTcpMsgComes(SendData);
		}

	}
}

void TcpServer::SocketDisconnecgted(int descriptor) { 
	for (int i = 0; i < m_tcpSocketConnectList.size(); i++) {
		QTcpSocket *item = m_tcpSocketConnectList.at(i);
		int des = item->socketDescriptor();	// 获取tcpsocket描述符
		if (des == descriptor || -1 == des) {
			m_tcpSocketConnectList.removeAt(i);
			item->deleteLater();

			QString text = QString("tcpSocket描述符：%1  IP： %2  断开连接!")
				.arg(descriptor).arg(item->peerAddress().toString());
			MyLogDEBUG(text .toUtf8());
			qDebug() << text;

			break;
		}

	}
}

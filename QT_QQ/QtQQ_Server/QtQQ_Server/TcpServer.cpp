#include "TcpServer.h"
#include "TcpSocket.h"
#include "public_type.h"

TcpServer::TcpServer(int prot) : m_port(prot) { }

TcpServer::~TcpServer() { }

bool TcpServer::run() {

	if (this->listen(QHostAddress::AnyIPv4, m_port)) {
		QString text = QString::fromLocal8Bit(QString("服务器监听端口 %1 成功！").arg(m_port).toUtf8());
		MyLogDEBUG(text.toUtf8());
		qDebug() << QString::fromLocal8Bit(text.toUtf8());

		return true;
	}

	MyLogDEBUG(QString("服务器监听端口 %1 失败！").arg(m_port).toUtf8());
	qDebug() << QString::fromLocal8Bit(QString("服务器监听端口 %1 失败！").arg(m_port).toLocal8Bit());
	
	return false;
}

void TcpServer::incomingConnection(qintptr socketDescriptor) {
	QString text = QString("新的连接：%1").arg(socketDescriptor);
	MyLogDEBUG(text .toUtf8());
	qDebug() << QString::fromLocal8Bit(text.toLocal8Bit());
	
	TcpSocket *tcpsocket = new TcpSocket();
	tcpsocket->setSocketDescriptor(socketDescriptor);
	tcpsocket->run();

	// 收到客户端数据后，server进行处理
	//connect(tcpsocket, SIGNAL(signalGetDataFromClient(QByteArray &, int)), 
	//		this, SLOT(SocketDataProcessiong(QByteArray &SendData, int descriptor)));
	connect(tcpsocket, &TcpSocket::signalGetDataFromClient, this, &TcpServer::SocketDataProcessiong);

	// 收到客户端断开连接后，server进行处理
	connect(tcpsocket, &TcpSocket::signalClientDisconnect, this, &TcpServer::SocketDisconnecgted);

	// 将socket添加到链表中
	m_tcpSocketConnectList.append(tcpsocket);
}

void TcpServer::SocketDataProcessiong(QByteArray & SendData, int descriptor) {


	for (int i = 0; i < m_tcpSocketConnectList.size(); i++) {
		QTcpSocket *item = m_tcpSocketConnectList.at(i);
		int des = item->socketDescriptor();	// 获取tcpsocket描述符
		if (des == descriptor) {
			QString text = QString("来自IP： %1  发来的数据：%2")
				.arg(item->peerAddress().toString()).arg(QString(SendData));
			MyLogDEBUG(text .toUtf8());
			qDebug() << QString::fromLocal8Bit(text.toLocal8Bit());

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

			QString text = QString("tcpSocket描述符：%1  IP： %2  断开连接!").arg(descriptor).arg(item->peerAddress().toString());
			MyLogDEBUG(text .toUtf8());
			qDebug() << QString::fromLocal8Bit(text.toLocal8Bit());

			break;
		}

	}
}

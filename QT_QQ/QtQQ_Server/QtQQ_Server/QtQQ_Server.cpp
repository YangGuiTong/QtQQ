#include "QtQQ_Server.h"

#include "public_type.h"

QtQQ_Server::QtQQ_Server(QWidget *parent)
	: QDialog(parent) {
	ui.setupUi(this);

	initTcpSocket();
}

QtQQ_Server::~QtQQ_Server() { }

void QtQQ_Server::initTcpSocket() { 
	m_tcpServer = new TcpServer(gtcpProt);
	m_tcpServer->run();

	// 收到tcp客户端发来的信息后进行UDP广播
	connect(m_tcpServer, &TcpServer::signalTcpMsgComes, this, &QtQQ_Server::onUDPbroadMsg);
}



void QtQQ_Server::onUDPbroadMsg(QByteArray &btData) {

}
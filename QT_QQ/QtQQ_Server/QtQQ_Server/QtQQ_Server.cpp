#include "QtQQ_Server.h"

#include "public_type.h"

#include <QPixmap>

QtQQ_Server::QtQQ_Server(QWidget *parent)
	: QDialog(parent) {
	ui.setupUi(this);

	QPixmap p;
	p.load(":QT.png");
	p.scaled(ui.label->size(), Qt::KeepAspectRatio);
	ui.label->setScaledContents(true);
	ui.label->setPixmap(p);
	ui.label->setAlignment(Qt::AlignCenter);


	initTcpSocket();


}

QtQQ_Server::~QtQQ_Server() {
	if (m_tcpServer) {
		delete m_tcpServer;
		m_tcpServer = nullptr;
	}

}

void QtQQ_Server::initTcpSocket() { 
	m_tcpServer = new TcpServer(gtcpProt);
	m_tcpServer->run();

	// �յ�tcp�ͻ��˷�������Ϣ�����UDP�㲥
	connect(m_tcpServer, &TcpServer::signalTcpMsgComes, this, &QtQQ_Server::onUDPbroadMsg);
}



void QtQQ_Server::onUDPbroadMsg(QByteArray &btData) {
	QString text = QString("���ݣ�%2 ��ʼ�㲥").arg(QString(btData));
	MyLogDEBUG(text .toUtf8());
	qDebug() << QString::fromLocal8Bit(text.toLocal8Bit());
}
#pragma once

#include <QtWidgets/QDialog>
#include "ui_QtQQ_Server.h"

#include "TcpServer.h"

class QtQQ_Server : public QDialog
{
    Q_OBJECT

public:
    QtQQ_Server(QWidget *parent = nullptr);
    ~QtQQ_Server();

private:
	void initTcpSocket();


private slots:
	void onUDPbroadMsg(QByteArray &btData);

private:
    Ui::QtQQ_ServerClass ui;

	TcpServer *m_tcpServer;
};

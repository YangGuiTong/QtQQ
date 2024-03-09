#pragma once

#include <QtWidgets/QDialog>
#include "ui_QtQQ_Server.h"

#include "TcpServer.h"

#include <QSqlQueryModel>

class QtQQ_Server : public QDialog
{
    Q_OBJECT

public:
    QtQQ_Server(QWidget *parent = nullptr);
    ~QtQQ_Server();

private:
	void initTcpSocket();
	bool connectMySql();
	int getCompDepID();	// ��ȡ��˾ȺQQ��
	void updateTaleData(int depID = 0, int employeeID = 0);

	void setDepNameMap();
	void setStatusMap();
	void setOnLineMap();

private slots:
	void onUDPbroadMsg(QByteArray &btData);

private:
    Ui::QtQQ_ServerClass ui;

	TcpServer *m_tcpServer;

	int m_compDepID;		// ��˾ȺQQ��
	QSqlQueryModel *m_queryInfoModel;	// ��ѯ����Ա����Ϣģ��

	QMap<QString, QString> m_statusMap;		// ״̬
	QMap<QString, QString> m_depNameMap;	// ��������
	QMap<QString, QString> m_onlineMap;		// ����״̬
};

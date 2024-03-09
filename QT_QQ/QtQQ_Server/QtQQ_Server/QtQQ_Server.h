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
	int getCompDepID();	// 获取公司群QQ号
	void updateTaleData(int depID = 0, int employeeID = 0);

	void setDepNameMap();
	void setStatusMap();
	void setOnLineMap();

private slots:
	void onUDPbroadMsg(QByteArray &btData);

private:
    Ui::QtQQ_ServerClass ui;

	TcpServer *m_tcpServer;

	int m_compDepID;		// 公司群QQ号
	QSqlQueryModel *m_queryInfoModel;	// 查询所有员工信息模型

	QMap<QString, QString> m_statusMap;		// 状态
	QMap<QString, QString> m_depNameMap;	// 部门名称
	QMap<QString, QString> m_onlineMap;		// 在线状态
};

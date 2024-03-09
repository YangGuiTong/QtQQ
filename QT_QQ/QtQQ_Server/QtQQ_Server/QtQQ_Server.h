#pragma once

#include <QtWidgets/QDialog>
#include "ui_QtQQ_Server.h"

#include "TcpServer.h"

#include <QSqlQueryModel>
#include <QTimer>

class QtQQ_Server : public QDialog
{
    Q_OBJECT

public:
    QtQQ_Server(QWidget *parent = nullptr);
    ~QtQQ_Server();

private:
	void initComboBoxData();		// 初始化组合框数据
	void initTcpSocket();
	bool connectMySql();
	int getCompDepID();	// 获取公司群QQ号
	void updateTaleData(int depID = 0, int employeeID = 0);

	void setDepNameMap();
	void setStatusMap();
	void setOnLineMap();

	// 根据员工QQ号获取部门号
	QString getDepartment(int employeesID);
	// 根据员工QQ号获取员工名字
	QString getEmployeeName(int employeesID);

private slots:
	void onUDPbroadMsg(QByteArray &btData);
	void onRefresh();

	void on_queryDepartmentBtn_clicked();	// 查询员工 - 条件：部门
	void on_queryIDBtn_clicked();			// 查询员工 - 条件：QQ号
	void on_logoutBtn_clicked();			// 注销员工 - 条件：QQ号

	void on_selectPictureBtn_clicked();		// 选中图片按钮
	void on_addBtn_clicked();				// 新增员工

private:
    Ui::QtQQ_ServerClass ui;

	TcpServer *m_tcpServer;

	int m_depID;			// 部门号
	int m_employeeID;		// 员工QQ号

	int m_compDepID;		// 公司群QQ号
	QSqlQueryModel *m_queryInfoModel;	// 查询所有员工信息模型

	QMap<QString, QString> m_statusMap;		// 状态
	QMap<QString, QString> m_depNameMap;	// 部门名称
	QMap<QString, QString> m_onlineMap;		// 在线状态

	QTimer *m_timer;		// 定时刷新数据

	QString m_pixPath;		// 头像路径
};

#pragma once

#include <QtWidgets/QDialog>
#include "ui_QtQQ_Server.h"

#include "TcpServer.h"

#include <QSqlQueryModel>
#include <QTimer>
#include <QUdpSocket>

class QtQQ_Server : public QDialog
{
    Q_OBJECT

public:
    QtQQ_Server(QWidget *parent = nullptr);
    ~QtQQ_Server();

private:
	void initComboBoxData();		// ��ʼ����Ͽ�����
	void initTcpSocket();
	void initUdpSocket();
	bool connectMySql();
	int getCompDepID();	// ��ȡ��˾ȺQQ��
	void updateTaleData(int depID = 0, int employeeID = 0);

	void setDepNameMap();
	void setStatusMap();
	void setOnLineMap();

	// ����Ա��QQ�Ż�ȡ���ź�
	QString getDepartment(const int employeesID);
	// ����Ա��QQ�Ż�ȡԱ������
	QString getEmployeeName(int employeesID);
	// ����Ա��QQ�Ż�ȡԱ������
	QString getEmployeePwd(const int employeeID);

	// ��ȡ���ݿ��е������¼
	void ReadDatabaseMessage();

	// ����������ʱ�������г�Ա��Ϊ����״̬
	void Offline();

	// ��ѯ����ʾԱ����Ϣ
	void ShowAlterEmployeeInfo(const int employeesID);

private slots:
	void onUDPbroadMsg(QByteArray &btData);
	void onRefresh();

	void on_queryDepartmentBtn_clicked();	// ��ѯԱ�� - ����������
	void on_queryIDBtn_clicked();			// ��ѯԱ�� - ������QQ��
	void on_logoutBtn_clicked();			// ע��Ա�� - ������QQ��

	void on_selectPictureBtn_clicked();		// ѡ��ͼƬ��ť
	void on_addBtn_clicked();				// ����Ա��
	void on_deleteBtn_clicked();			// ɾ��Ա��

	void on_alterSelectPictureBtn_clicked();	// �޸��е�ѡ��ͼƬ��ַ
	void on_alterBtn_clicked();					// �޸İ�ť

private:
    Ui::QtQQ_ServerClass ui;

	TcpServer *m_tcpServer;		// tcp�����
	QUdpSocket *m_udpSender;	// udp�㲥

	int m_depID;			// ���ź�
	int m_employeeID;		// Ա��QQ��

	int m_compDepID;		// ��˾ȺQQ��
	QSqlQueryModel *m_queryInfoModel;	// ��ѯ����Ա����Ϣģ��

	QMap<QString, QString> m_statusMap;		// ״̬
	QMap<QString, QString> m_depNameMap;	// ��������
	QMap<QString, QString> m_onlineMap;		// ����״̬

	QTimer *m_timer;		// ��ʱˢ������

	QString m_pixPath;		// ͷ��·��
	QString m_alterPoxPath;	// �޸ĵ�ͼƬ·��
};

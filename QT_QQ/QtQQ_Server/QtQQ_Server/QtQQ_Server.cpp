#include "QtQQ_Server.h"

#include "public_type.h"

#include <QPixmap>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QHeaderView>
#include <QTableWidget>
#include <QSqlRecord>

QtQQ_Server::QtQQ_Server(QWidget *parent)
	: QDialog(parent) {
	ui.setupUi(this);

	QPixmap p;
	p.load(":QT.png");
	p.scaled(ui.label->size(), Qt::KeepAspectRatio);
	ui.label->setScaledContents(true);
	ui.label->setPixmap(p);
	ui.label->setAlignment(Qt::AlignCenter);

	if (!connectMySql()) {
		QString text = QString("�������ݿ�ʧ��!");
		MyLogDEBUG(text .toUtf8());
		qDebug() << QString::fromLocal8Bit(text.toLatin1().constData());
		QMessageBox::warning(NULL, tr("��ʾ"), tr("�������ݿ�ʧ�ܣ�"));
		close();
		return;
	}

	setDepNameMap();
	setStatusMap();
	setOnLineMap();

	m_queryInfoModel = new QSqlQueryModel(this);
	QString sql = QString("SELECT * FROM tab_employees;");
	m_queryInfoModel->setQuery(sql);

	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	updateTaleData();


	QStringList strList;
	strList << "���²�" << "�з���" << "�г���";
	ui.employeeDepBox->addItems(strList);

	strList.clear();
	strList << "��˾Ⱥ" << "���²�" << "�з���" << "�г���";
	ui.departmentBox->addItems(strList);


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

bool QtQQ_Server::connectMySql() {
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setDatabaseName("qtqq");
	db.setHostName("localhost");
	db.setUserName("root");
	db.setPassword("yang");
	db.setPort(3306);

	if (db.open()) {
		return true;
	}


	return false;
}

int QtQQ_Server::getCompDepID() {
	return 0;
}

void QtQQ_Server::updateTaleData(int depID, int employeeID) { 
	MyLogDEBUG(QString("���±�����ݣ�depID = %1, employeeID = %2").arg(depID).arg(employeeID).toUtf8());

	ui.tableWidget->clear();

	if (depID && depID != m_compDepID) {	// ��ѯ����
		QString sql = QString("SELECT * FROM tab_employees WHERE departmentID=%1;").arg(depID);
		MyLogDEBUG(sql.toUtf8());
		m_queryInfoModel->setQuery(sql);

	} else if (employeeID) {	// ��ȷ����
		QString sql = QString("SELECT * FROM tab_employees WHERE employeeID=%1;").arg(employeeID);
		MyLogDEBUG(sql.toUtf8());
		m_queryInfoModel->setQuery(sql);
	
	} else {	// ��ѯ����
		QString sql = QString("SELECT * FROM tab_employees;");
		MyLogDEBUG(sql.toUtf8());
		m_queryInfoModel->setQuery(sql);
	}

	int rows = m_queryInfoModel->rowCount();		// ���������ܼ�¼����
	int columns = m_queryInfoModel->columnCount();	// �����������ֶ�����
	QModelIndex index;	// ģ������

	// ���ñ�������������
	ui.tableWidget->setRowCount(rows);
	ui.tableWidget->setColumnCount(columns);

	// ���ñ�ͷ
	QStringList headers;
	headers << tr("����") << tr("����") << tr("Ա������") << tr("Ա��ǩ��") << tr("Ա��״̬") << tr("Ա��ͷ��") << tr("����״̬");
	ui.tableWidget->setHorizontalHeaderLabels(headers);

	// �����еȿ�
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			index = m_queryInfoModel->index(i, j);	// �У���
			QString strData = m_queryInfoModel->data(index).toString();	// ��ȡi��j�е�����

			// ��ȡ�ֶ�����
			QSqlRecord record = m_queryInfoModel->record(i);	// ��ǰ�еļ�¼
			QString strRecordName = record.fieldName(j);		// ��ǰ�еļ�¼

			if (strRecordName == QLatin1String("departmentID")) {
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_depNameMap.value(strData)));
				continue;
			}

			if (strRecordName == QLatin1String("status")) {
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_statusMap.value(strData)));
				continue;
			}

			if (strRecordName == QLatin1String("online")) {
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_onlineMap.value(strData)));
				continue;
			}

			ui.tableWidget->setItem(i, j, new QTableWidgetItem(strData));
		}
	}
}

void QtQQ_Server::setDepNameMap() {
	m_depNameMap.insert("2001", tr("���²�"));
	m_depNameMap.insert("2002", tr("�з���"));
	m_depNameMap.insert("2003", tr("�г���"));
}

void QtQQ_Server::setStatusMap() {
	m_statusMap.insert("1", tr("��Ч"));
	m_statusMap.insert("2", tr("��ע��"));
}

void QtQQ_Server::setOnLineMap() {
	m_onlineMap.insert("1", tr("����"));
	m_onlineMap.insert("2", tr("����"));
	m_onlineMap.insert("3", tr("����"));
	m_onlineMap.insert("4", tr("æµ"));
}



void QtQQ_Server::onUDPbroadMsg(QByteArray &btData) {
	QString text = QString("���ݣ�%2 ��ʼ�㲥").arg(QString(btData));
	MyLogDEBUG(text .toUtf8());
	qDebug() << QString::fromLocal8Bit(text.toLocal8Bit());
}
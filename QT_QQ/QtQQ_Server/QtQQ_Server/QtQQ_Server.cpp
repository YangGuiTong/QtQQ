#include "QtQQ_Server.h"

#include "public_type.h"

#include <QPixmap>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QHeaderView>
#include <QTableWidget>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QFileDialog>

// ��һ��int�����ͷ���QQ�ţ��ڶ���int������շ���QQ�ţ�QJsonArray���������¼
QMultiMap<int, QMap<int, QJsonArray>> g_message_info;	// �����¼

QtQQ_Server::QtQQ_Server(QWidget *parent)
	: QDialog(parent), m_pixPath("") {
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

	QStringList strList;
	strList << "���²�" << "�з���" << "�г���";
	ui.employeeDepBox->addItems(strList);

	strList.clear();
	strList << "��˾Ⱥ" << "���²�" << "�з���" << "�г���";
	ui.departmentBox->addItems(strList);

	initComboBoxData();

	m_queryInfoModel = new QSqlQueryModel(this);
	QString sql = QString("SELECT * FROM tab_employees;");
	m_queryInfoModel->setQuery(sql);

	// ���ñ������༭
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	// ��ʼ����ѯ��˾Ⱥ����Ա����Ϣ
	m_depID = getCompDepID();
	m_compDepID = m_depID;
	m_employeeID = 0;

	updateTaleData();


	// ��ʱˢ������
	m_timer = new QTimer(this);
	m_timer->setInterval(1000);
	m_timer->start();
	connect(m_timer, &QTimer::timeout, this, &QtQQ_Server::onRefresh);


	initTcpSocket();
	initUdpSocket();

	ui.queryIDLineEdit->setValidator(new QRegExpValidator(QRegExp("^[0-9]+$"), ui.queryIDLineEdit));
	ui.logoutIDLineEdit->setValidator(new QRegExpValidator(QRegExp("^[0-9]+$"), ui.logoutIDLineEdit));


	ReadDatabaseMessage();
}

QtQQ_Server::~QtQQ_Server() {
	if (m_tcpServer) {
		delete m_tcpServer;
		m_tcpServer = nullptr;
	}

}

void QtQQ_Server::initComboBoxData() { 

	QString itemText;	// ��Ͽ����ı�

	// ��ȡ��˾�ܵĲ�����
	QSqlQueryModel queryDepModel;
	QString sql = QString("SELECT * FROM tab_department;");
	MyLogDEBUG(sql.toUtf8());
	
	queryDepModel.setQuery(sql);
	int depCounts = queryDepModel.rowCount() - 1;	// ��������Ӧ��ȥ��˾Ⱥһ����¼

	for (int i = 0; i < depCounts; i++) {
		itemText = ui.employeeDepBox->itemText(i);
		QSqlQuery queryDepID;
		sql = QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'").arg(itemText);
		queryDepID.exec(sql);
		queryDepID.next();
		
		// ����Ա������������Ͽ������Ϊ��Ӧ�Ĳ��ź�
		ui.employeeDepBox->setItemData(i, queryDepID.value(0).toInt());
	}

	// ��һ������˾Ⱥ������
	for (int i = 0; i < depCounts + 1; i++) {
		itemText = ui.departmentBox->itemText(i);
		QSqlQuery queryDepID;
		sql = QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'").arg(itemText);
		queryDepID.exec(sql);
		queryDepID.next();

		// ���ò�����Ͽ������Ϊ��Ӧ�Ĳ��ź�
		ui.departmentBox->setItemData(i, queryDepID.value(0).toInt());
	}
}

void QtQQ_Server::initTcpSocket() {
	m_tcpServer = new TcpServer(gTcpPort);
	m_tcpServer->run();

	// �յ�tcp�ͻ��˷�������Ϣ�����UDP�㲥
	connect(m_tcpServer, &TcpServer::signalTcpMsgComes, this, &QtQQ_Server::onUDPbroadMsg);
}

void QtQQ_Server::initUdpSocket() { 
	m_udpSender = new QUdpSocket(this);
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

	QString sql = QString("SELECT departmentID FROM tab_department WHERE department_name='��˾Ⱥ'");
	QSqlQuery query(sql);
	query.exec();

	query.next();
	int compDepID = query.value("departmentID").toInt();

	return compDepID;
}

void QtQQ_Server::updateTaleData(int depID, int employeeID) { 
	//MyLogDEBUG(QString("���±�����ݣ�depID = %1, employeeID = %2").arg(depID).arg(employeeID).toUtf8());

	ui.tableWidget->clear();

	if (depID && depID != m_compDepID) {	// ��ѯ����
		QString sql = QString("SELECT * FROM tab_employees WHERE departmentID=%1;").arg(depID);
		//MyLogDEBUG(sql.toUtf8());
		m_queryInfoModel->setQuery(sql);

	} else if (employeeID) {	// ��ȷ����
		QString sql = QString("SELECT * FROM tab_employees WHERE employeeID=%1;").arg(employeeID);
		//MyLogDEBUG(sql.toUtf8());
		m_queryInfoModel->setQuery(sql);
	
	} else {	// ��ѯ����
		QString sql = QString("SELECT * FROM tab_employees;");
		//MyLogDEBUG(sql.toUtf8());
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

QString QtQQ_Server::getDepartment(int employeesID) {

	QString sql = QString("SELECT department_name FROM tab_department WHERE departmentID=\
						  (SELECT departmentID FROM tab_employees WHERE employeeID=%1);").arg(employeesID);
	MyLogDEBUG(sql.toUtf8());
	QSqlQuery query(sql);
	query.exec();

	if (!query.next()) {
		MyLogDEBUG(QString("û�в�ѯ���κ���Ϣ").toUtf8());
		return QString("");
	}

	QString department_name = query.value(0).toString();

	return department_name;
}

QString QtQQ_Server::getEmployeeName(int employeesID) {
	
	QString sql = QString("SELECT employee_name FROM tab_employees WHERE employeeID=%1;").arg(employeesID);
	//MyLogDEBUG(sql.toUtf8());
	QSqlQuery query(sql);
	query.exec();

	if (!query.next()) {
		MyLogDEBUG(QString("Ա��QQ�ţ�%1  û�в�ѯ���κ���Ϣ").arg(employeesID).toUtf8());
		return QString("");
	}

	QString employee_name = query.value(0).toString();

	return employee_name;


}

void QtQQ_Server::ReadDatabaseMessage() { 

	QString sql = QString("SELECT * FROM tab_chat;");
	QSqlQuery query(sql);
	query.exec();

	while (query.next()) {
		int sender = query.value(1).toInt();
		int receiver = query.value(2).toInt();
		QString message = query.value(3).toString();
		message = message.simplified();		// ȥ����ͷ��β�м�������ַ���\r\n\t
		QJsonArray messageArr = QJsonDocument::fromJson(message.toLocal8Bit()).array();

		// ����
		QMap<int, QJsonArray> messageMap;
		messageMap.insert(receiver, messageArr);
		g_message_info.insert(sender, messageMap);
	}
}

void QtQQ_Server::onRefresh() {
	updateTaleData(m_depID, m_employeeID);
}

void QtQQ_Server::on_queryDepartmentBtn_clicked() {

	ui.queryIDLineEdit->clear();
	ui.logoutIDLineEdit->clear();

	m_employeeID = 0;
	m_depID = ui.departmentBox->currentData().toInt();
	QString curText = ui.departmentBox->currentText();
	MyLogDEBUG(QString("��ѯ %1 �µ�����Ա����Ϣ����ţ�%2").arg(curText).arg(m_depID).toUtf8());

	updateTaleData(m_depID, m_employeeID);
}

void QtQQ_Server::on_queryIDBtn_clicked() {

	ui.logoutIDLineEdit->clear();

	int employeeID = ui.queryIDLineEdit->text().toInt();
	if (0 == employeeID) {
		QMessageBox::information(this, tr("��ʾ"), tr("������Ա��QQ�ţ�"));
		ui.queryIDLineEdit->setFocus();
		return;
	}

	QString sql = QString("SELECT * FROM tab_employees WHERE employeeID=%1").arg(employeeID);
	QSqlQuery query(sql);
	query.exec();
	if (!query.next()) {
		QMessageBox::information(this, tr("��ʾ"), tr("��������ȷ��Ա��QQ�ţ�"));
		ui.queryIDLineEdit->setFocus();
		return;
	}

	// ����commobox�ı���ʾ��ǰ��ѯ��Ա������
	QString department_name = getDepartment(employeeID);
	ui.departmentBox->currentIndexChanged(department_name);
	ui.departmentBox->currentTextChanged(department_name);
	ui.departmentBox->setCurrentText(department_name);

	m_depID = 0;
	m_employeeID = employeeID;

	// ��ȡԱ��������
	QString employee_name = getEmployeeName(employeeID);

	MyLogDEBUG(QString("��ѯQQ�ţ�%1  Ա��������%2 ��Ϣ").arg(m_employeeID).arg(employee_name).toUtf8());
	updateTaleData(m_depID, m_employeeID);
}

void QtQQ_Server::on_logoutBtn_clicked() {

	ui.queryIDLineEdit->clear();

	int employeeID = ui.logoutIDLineEdit->text().toInt();
	if (0 == employeeID) {
		QMessageBox::information(this, tr("��ʾ"), tr("������Ա��QQ�ţ�"));
		ui.logoutIDLineEdit->setFocus();
		return;
	}

	QString sql = QString("SELECT * FROM tab_employees WHERE employeeID=%1").arg(employeeID);
	QSqlQuery query(sql);
	query.exec();
	if (!query.next()) {
		QMessageBox::information(this, tr("��ʾ"), tr("��������ȷ��Ա��QQ�ţ�"));
		ui.logoutIDLineEdit->setFocus();
		return;
	}


	sql = QString("UPDATE tab_employees SET status=2, online=1 WHERE employeeID=%1").arg(employeeID);
	//MyLogDEBUG(sql.toUtf8());
	query.exec(sql);

	// ��ȡע��Ա��������
	QString employee_name = getEmployeeName(employeeID);

	QString text = QString("Ա�� '%1' ����ҵQQ��'%2' �ѱ�ע����").arg(employee_name).arg(employeeID);
	MyLogDEBUG(text.toUtf8());
	QMessageBox::information(this, tr("��ʾ"), tr(text.toUtf8()));

	updateTaleData(m_depID, m_employeeID);
}

void QtQQ_Server::on_selectPictureBtn_clicked() {

	// ��ȡѡ���ͷ��·��
	m_pixPath = QFileDialog::getOpenFileName(this, tr("ѡ��ͷ��"), ".", "*.png;;*.jpg");
	if (0 == m_pixPath.size()) {
		return;
	}

	// ��ͷ����ʾ����ǩ
	QPixmap pixmap;
	pixmap.load(m_pixPath);

	qreal widthRatio = (qreal)ui.headLabel->width() / (qreal)pixmap.width();
	qreal heightRatio = (qreal)ui.headLabel->height() / (qreal)pixmap.height();

	QSize size(pixmap.width() * widthRatio, pixmap.height() * heightRatio);
	ui.headLabel->setPixmap(pixmap.scaled(size));
}

void QtQQ_Server::on_addBtn_clicked() {
	// ���Ա������������
	QString strName = ui.nameLineEdit->text();
	if (!strName.size()) {
		QMessageBox::information(this, tr("��ʾ"), tr("������Ա��������"));
		ui.nameLineEdit->setFocus();
		return;
	}

	// ���Ա��ѡ��ͷ��
	if (!m_pixPath.size()) {
		QMessageBox::information(this, tr("��ʾ"), tr("��ѡ��Ա��ͷ��·����"));
		return;
	}

	/* ���ݿ�����µ�Ա������ */
	// ��ȡԱ��QQ��
	QString sql = QString("SELECT MAX(employeeID) FROM tab_employees;");
	QSqlQuery maxEmployeeID(sql);
	maxEmployeeID.exec();
	maxEmployeeID.next();

	int employeeID = maxEmployeeID.value(0).toInt() + 1;

	// Ա������QQ��
	int depID = ui.employeeDepBox->currentData().toInt();

	// ͼƬ·����ʽ����Ϊ  xxx\xxx\xxx
	m_pixPath.replace("/", "\\\\");

	sql = QString("INSERT INTO tab_employees(departmentID, employeeID, employee_name, picture) VALUES(%1, %2, '%3', '%4')")
		.arg(depID).arg(employeeID).arg(strName).arg(m_pixPath);
	MyLogDEBUG(sql.toUtf8());

	QSqlQuery insertSql(sql);
	insertSql.exec();

	QMessageBox::information(this, tr("��ʾ"), tr("�����ɹ�"));
	m_pixPath = "";
	ui.headLabel->setText(("  Ա������"));
	ui.nameLineEdit->clear();
}



void QtQQ_Server::onUDPbroadMsg(QByteArray &btData) {
	QString text = QString("���ݣ�%1 ��ʼ�㲥").arg(QString(btData));
	MyLogDEBUG(text.toUtf8());
	qDebug() << QString::fromLocal8Bit(text.toLocal8Bit());


	for (quint16 port = gUdpPort; port < gUdpPort + 200; port++) {
		m_udpSender->writeDatagram(btData, btData.size(), QHostAddress::Broadcast, port);
	}
}
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
		QString text = QString("连接数据库失败!");
		MyLogDEBUG(text .toUtf8());
		qDebug() << QString::fromLocal8Bit(text.toLatin1().constData());
		QMessageBox::warning(NULL, tr("提示"), tr("连接数据库失败！"));
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
	strList << "人事部" << "研发部" << "市场部";
	ui.employeeDepBox->addItems(strList);

	strList.clear();
	strList << "公司群" << "人事部" << "研发部" << "市场部";
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

	// 收到tcp客户端发来的信息后进行UDP广播
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
	MyLogDEBUG(QString("更新表格数据：depID = %1, employeeID = %2").arg(depID).arg(employeeID).toUtf8());

	ui.tableWidget->clear();

	if (depID && depID != m_compDepID) {	// 查询部门
		QString sql = QString("SELECT * FROM tab_employees WHERE departmentID=%1;").arg(depID);
		MyLogDEBUG(sql.toUtf8());
		m_queryInfoModel->setQuery(sql);

	} else if (employeeID) {	// 精确查找
		QString sql = QString("SELECT * FROM tab_employees WHERE employeeID=%1;").arg(employeeID);
		MyLogDEBUG(sql.toUtf8());
		m_queryInfoModel->setQuery(sql);
	
	} else {	// 查询所有
		QString sql = QString("SELECT * FROM tab_employees;");
		MyLogDEBUG(sql.toUtf8());
		m_queryInfoModel->setQuery(sql);
	}

	int rows = m_queryInfoModel->rowCount();		// 总行数（总记录数）
	int columns = m_queryInfoModel->columnCount();	// 总列数（总字段数）
	QModelIndex index;	// 模型索引

	// 设置表格的行数、列数
	ui.tableWidget->setRowCount(rows);
	ui.tableWidget->setColumnCount(columns);

	// 设置表头
	QStringList headers;
	headers << tr("部门") << tr("工号") << tr("员工姓名") << tr("员工签名") << tr("员工状态") << tr("员工头像") << tr("在线状态");
	ui.tableWidget->setHorizontalHeaderLabels(headers);

	// 设置列等宽
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			index = m_queryInfoModel->index(i, j);	// 行，列
			QString strData = m_queryInfoModel->data(index).toString();	// 获取i行j列的数据

			// 获取字段名称
			QSqlRecord record = m_queryInfoModel->record(i);	// 当前行的记录
			QString strRecordName = record.fieldName(j);		// 当前列的记录

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
	m_depNameMap.insert("2001", tr("人事部"));
	m_depNameMap.insert("2002", tr("研发部"));
	m_depNameMap.insert("2003", tr("市场部"));
}

void QtQQ_Server::setStatusMap() {
	m_statusMap.insert("1", tr("有效"));
	m_statusMap.insert("2", tr("已注销"));
}

void QtQQ_Server::setOnLineMap() {
	m_onlineMap.insert("1", tr("离线"));
	m_onlineMap.insert("2", tr("在线"));
	m_onlineMap.insert("3", tr("隐身"));
	m_onlineMap.insert("4", tr("忙碌"));
}



void QtQQ_Server::onUDPbroadMsg(QByteArray &btData) {
	QString text = QString("数据：%2 开始广播").arg(QString(btData));
	MyLogDEBUG(text .toUtf8());
	qDebug() << QString::fromLocal8Bit(text.toLocal8Bit());
}
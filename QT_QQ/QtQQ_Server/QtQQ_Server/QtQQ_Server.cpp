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

// 第一个int代表发送方的QQ号，第二个int代表接收方的QQ号，QJsonArray代表聊天记录
QMultiMap<int, QMap<int, QJsonArray>> g_message_info;	// 聊天记录

QtQQ_Server::QtQQ_Server(QWidget *parent)
	: QDialog(parent), m_pixPath("") {
	ui.setupUi(this);

	setWindowTitle("Qt_即时通讯服务器");

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

	QStringList strList;
	strList << "人事部" << "研发部" << "市场部";
	ui.employeeDepBox->addItems(strList);
	ui.alterEmployeeDepBox->addItems(strList);

	strList.clear();
	strList << "公司群" << "人事部" << "研发部" << "市场部";
	ui.departmentBox->addItems(strList);

	strList.clear();
	strList << "有效" << "已注销";
	ui.alterEmployeeStatusBox->addItems(strList);

	initComboBoxData();

	m_queryInfoModel = new QSqlQueryModel(this);
	QString sql = QString("SELECT * FROM tab_employees;");
	m_queryInfoModel->setQuery(sql);

	// 设置表格不允许编辑
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	// 初始化查询公司群所有员工信息
	m_depID = getCompDepID();
	m_compDepID = m_depID;
	m_employeeID = 0;

	updateTaleData();
	Offline();


	// 定时刷新数据
	m_timer = new QTimer(this);
	m_timer->setInterval(1000);
	m_timer->start();
	connect(m_timer, &QTimer::timeout, this, &QtQQ_Server::onRefresh);


	initTcpSocket();
	initUdpSocket();

	ui.queryIDLineEdit->setValidator(new QRegExpValidator(QRegExp("^[0-9]+$"), ui.queryIDLineEdit));
	ui.logoutIDLineEdit->setValidator(new QRegExpValidator(QRegExp("^[0-9]+$"), ui.logoutIDLineEdit));


	ReadDatabaseMessage();

	ui.lineEditPwd->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9]+$")));

	ui.alterWidget->setVisible(false);
}

QtQQ_Server::~QtQQ_Server() {
	if (m_tcpServer) {
		delete m_tcpServer;
		m_tcpServer = nullptr;
	}

}

void QtQQ_Server::initComboBoxData() { 

	QString itemText;	// 组合框项文本

	// 获取公司总的部门数
	QSqlQueryModel queryDepModel;
	QString sql = QString("SELECT * FROM tab_department;");
	MyLogDEBUG(sql.toUtf8());
	
	queryDepModel.setQuery(sql);
	int depCounts = queryDepModel.rowCount() - 1;	// 部门总数应减去公司群一条记录

	for (int i = 0; i < depCounts; i++) {
		itemText = ui.employeeDepBox->itemText(i);
		QSqlQuery queryDepID;
		sql = QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'").arg(itemText);
		queryDepID.exec(sql);
		queryDepID.next();
		
		// 设置员工所属部门组合框的数据为相应的部门号
		ui.employeeDepBox->setItemData(i, queryDepID.value(0).toInt());
	}

	// 多一个“公司群”部门
	for (int i = 0; i < depCounts + 1; i++) {
		itemText = ui.departmentBox->itemText(i);
		QSqlQuery queryDepID;
		sql = QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'").arg(itemText);
		queryDepID.exec(sql);
		queryDepID.next();

		// 设置部门组合框的数据为相应的部门号
		ui.departmentBox->setItemData(i, queryDepID.value(0).toInt());

		static int company = 1;
		if (1 == company) {
			company = 0;
			continue;
		}
		// 设置部门组合框的数据为相应的部门号
		ui.alterEmployeeDepBox->setItemData(i-1, queryDepID.value(0).toInt());
	}
}

void QtQQ_Server::initTcpSocket() {
	m_tcpServer = new TcpServer(gTcpPort);
	m_tcpServer->run();

	// 收到tcp客户端发来的信息后进行UDP广播
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

	QString sql = QString("SELECT departmentID FROM tab_department WHERE department_name='公司群'");
	QSqlQuery query(sql);
	query.exec();

	query.next();
	int compDepID = query.value("departmentID").toInt();

	return compDepID;
}

void QtQQ_Server::updateTaleData(int depID, int employeeID) { 
	//MyLogDEBUG(QString("更新表格数据：depID = %1, employeeID = %2").arg(depID).arg(employeeID).toUtf8());

	ui.tableWidget->clear();

	if (depID && depID != m_compDepID) {	// 查询部门
		QString sql = QString("SELECT * FROM tab_employees WHERE departmentID=%1;").arg(depID);
		//MyLogDEBUG(sql.toUtf8());
		m_queryInfoModel->setQuery(sql);

	} else if (employeeID) {	// 精确查找
		QString sql = QString("SELECT * FROM tab_employees WHERE employeeID=%1;").arg(employeeID);
		//MyLogDEBUG(sql.toUtf8());
		m_queryInfoModel->setQuery(sql);
	
	} else {	// 查询所有
		QString sql = QString("SELECT * FROM tab_employees;");
		//MyLogDEBUG(sql.toUtf8());
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

QString QtQQ_Server::getDepartment(const int employeesID) {

	QString sql = QString("SELECT department_name FROM tab_department WHERE departmentID=\
						  (SELECT departmentID FROM tab_employees WHERE employeeID=%1);").arg(employeesID);
	MyLogDEBUG(sql.toUtf8());
	QSqlQuery query(sql);
	query.exec();

	if (!query.next()) {
		MyLogDEBUG(QString("没有查询到任何信息").toUtf8());
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
		MyLogDEBUG(QString("员工QQ号：%1  没有查询到任何信息").arg(employeesID).toUtf8());
		return QString("");
	}

	QString employee_name = query.value(0).toString();

	return employee_name;
}

QString QtQQ_Server::getEmployeePwd(const int employeeID) {
	QString sql = QString("SELECT code FROM tab_accounts WHERE employeeID=%1;").arg(employeeID);
	QSqlQuery query(sql);
	query.exec();

	if (!query.next()) {
		MyLogDEBUG(QString("员工QQ号：%1  没有查询到密码信息").arg(employeeID).toUtf8());
		return QString("");
	}

	QString employee_pwd = query.value("code").toString();

	return employee_pwd;
}

void QtQQ_Server::ReadDatabaseMessage() { 

	QString sql = QString("SELECT * FROM tab_chat;");
	QSqlQuery query(sql);
	query.exec();

	while (query.next()) {
		int sender = query.value(1).toInt();
		int receiver = query.value(2).toInt();
		QString message = query.value(3).toString();
		message = message.simplified();		// 去除开头结尾中间的特殊字符，\r\n\t
		QJsonArray messageArr = QJsonDocument::fromJson(message.toLocal8Bit()).array();

		// 保存
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
	MyLogDEBUG(QString("查询 %1 下的所有员工信息，编号：%2").arg(curText).arg(m_depID).toUtf8());

	updateTaleData(m_depID, m_employeeID);

	ui.alterWidget->setVisible(false);
}

void QtQQ_Server::on_queryIDBtn_clicked() {
	ui.logoutIDLineEdit->clear();
	ui.deleteLineEdit->clear();

	int employeeID = ui.queryIDLineEdit->text().toInt();
	if (0 == employeeID) {
		QMessageBox::information(this, tr("提示"), tr("请输入员工QQ号！"));
		ui.queryIDLineEdit->setFocus();
		return;
	}

	QString sql = QString("SELECT * FROM tab_employees WHERE employeeID=%1").arg(employeeID);
	QSqlQuery query(sql);
	query.exec();
	if (!query.next()) {
		QMessageBox::information(this, tr("提示"), tr("请输入正确的员工QQ号！"));
		ui.queryIDLineEdit->setFocus();
		return;
	}

	// 设置commobox文本显示当前查询的员工部门
	QString department_name = getDepartment(employeeID);
	ui.departmentBox->currentIndexChanged(department_name);
	ui.departmentBox->currentTextChanged(department_name);
	ui.departmentBox->setCurrentText(department_name);

	m_depID = 0;
	m_employeeID = employeeID;

	// 获取员工的姓名
	QString employee_name = getEmployeeName(employeeID);

	MyLogDEBUG(QString("查询QQ号：%1  员工姓名：%2 信息").arg(m_employeeID).arg(employee_name).toUtf8());
	updateTaleData(m_depID, m_employeeID);

	ShowAlterEmployeeInfo(employeeID);

	ui.alterWidget->setVisible(true);
}

void QtQQ_Server::on_logoutBtn_clicked() {
	ui.alterWidget->setVisible(false);

	ui.queryIDLineEdit->clear();
	ui.deleteLineEdit->clear();

	int employeeID = ui.logoutIDLineEdit->text().toInt();
	if (0 == employeeID) {
		QMessageBox::information(this, tr("提示"), tr("请输入员工QQ号！"));
		ui.logoutIDLineEdit->setFocus();
		return;
	}

	QString sql = QString("SELECT * FROM tab_employees WHERE employeeID=%1").arg(employeeID);
	QSqlQuery query(sql);
	query.exec();
	if (!query.next()) {
		QMessageBox::information(this, tr("提示"), tr("请输入正确的员工QQ号！"));
		ui.logoutIDLineEdit->setFocus();
		return;
	}


	sql = QString("UPDATE tab_employees SET status=2, online=1 WHERE employeeID=%1").arg(employeeID);
	//MyLogDEBUG(sql.toUtf8());
	query.exec(sql);

	// 获取注销员工的姓名
	QString employee_name = getEmployeeName(employeeID);

	QString text = QString("员工 '%1' 的企业账号：'%2' 已被注销！").arg(employee_name).arg(employeeID);
	MyLogDEBUG(text.toUtf8());
	QMessageBox::information(this, tr("提示"), tr(text.toUtf8()));

	updateTaleData(m_depID, m_employeeID);
}

void QtQQ_Server::on_selectPictureBtn_clicked() {
	ui.alterWidget->setVisible(false);

	// 获取选择的头像路径
	m_pixPath = QFileDialog::getOpenFileName(this, tr("选择头像"), ".", "*.png;;*.jpg");
	if (0 == m_pixPath.size()) {
		return;
	}

	// 将头像显示到标签
	QPixmap pixmap;
	pixmap.load(m_pixPath);

	qreal widthRatio = (qreal)ui.headLabel->width() / (qreal)pixmap.width();
	qreal heightRatio = (qreal)ui.headLabel->height() / (qreal)pixmap.height();

	QSize size(pixmap.width() * widthRatio, pixmap.height() * heightRatio);
	ui.headLabel->setPixmap(pixmap.scaled(size));
}

void QtQQ_Server::on_addBtn_clicked() {
	ui.alterWidget->setVisible(false);

	// 检测员工姓名的输入
	QString strName = ui.nameLineEdit->text();
	if (!strName.size()) {
		QMessageBox::information(this, tr("提示"), tr("请输入员工姓名！"));
		ui.nameLineEdit->setFocus();
		return;
	}

	// 检测员工选择头像
	if (!m_pixPath.size()) {
		QMessageBox::information(this, tr("提示"), tr("请选择员工头像路径！"));
		return;
	}

	// 密码判断
	QString pwd = ui.lineEditPwd->text();
	if (!pwd.size()) {
		QMessageBox::information(this, tr("提示"), tr("请输入密码！"));
		ui.lineEditPwd->setFocus();
		return;
	}
	if (5 > pwd.length()) {
		QMessageBox::information(this, tr("提示"), tr("密码长度小于5位"));
		ui.lineEditPwd->setFocus();
		return;
	}



	/* 数据库插入新的员工数据 */
	// 获取员工QQ号
	QString sql = QString("SELECT MAX(employeeID) FROM tab_employees;");
	QSqlQuery maxEmployeeID(sql);
	maxEmployeeID.exec();
	maxEmployeeID.next();

	int employeeID = maxEmployeeID.value(0).toInt() + 1;

	// 员工部门QQ号
	int depID = ui.employeeDepBox->currentData().toInt();

	// 图片路径格式设置为  xxx\xxx\xxx
	m_pixPath.replace("/", "\\\\");

	sql = QString("INSERT INTO tab_employees(departmentID, employeeID, employee_name, picture) VALUES(%1, %2, '%3', '%4')")
		.arg(depID).arg(employeeID).arg(strName).arg(m_pixPath);
	MyLogDEBUG(sql.toUtf8());

	QSqlQuery insertSql(sql);
	insertSql.exec();


	sql = QString("INSERT INTO tab_accounts VALUES(%1, '%2', '%3');").arg(employeeID).arg(strName).arg(pwd);
	QSqlQuery insertAccountSql;
	insertAccountSql.exec(sql);


	QMessageBox::information(this, tr("提示"), tr("新增成功"));
	m_pixPath = "";
	ui.headLabel->setText(("  员工寸照"));
	ui.nameLineEdit->clear();
	ui.lineEditPwd->clear();
}

void QtQQ_Server::on_deleteBtn_clicked() {
	ui.alterWidget->setVisible(false);

	ui.queryIDLineEdit->clear();
	ui.logoutIDLineEdit->clear();

	int employeeID = ui.deleteLineEdit->text().toInt();
	if (0 == employeeID) {
		QMessageBox::information(this, tr("提示"), tr("请输入员工QQ号！"));
		ui.logoutIDLineEdit->setFocus();
		return;
	}

	QString sql = QString("SELECT * FROM tab_employees WHERE employeeID=%1").arg(employeeID);
	QSqlQuery query(sql);
	query.exec();
	if (!query.next()) {
		QMessageBox::information(this, tr("提示"), tr("请输入正确的员工QQ号！"));
		ui.logoutIDLineEdit->setFocus();
		return;
	}


	sql = QString("DELETE FROM tab_employees WHERE employeeID=%1").arg(employeeID);
	query.exec(sql);


	sql = QString("DELETE FROM tab_accounts WHERE employeeID=%1").arg(employeeID);
	query.exec(sql);



	// 获取删除员工的姓名
	QString employee_name = getEmployeeName(employeeID);

	QString text = QString("员工 '%1' 的企业账号：'%2' 已被删除！").arg(employee_name).arg(employeeID);
	MyLogDEBUG(text.toUtf8());
	QMessageBox::information(this, tr("提示"), tr(text.toUtf8()));

	updateTaleData(m_depID, m_employeeID);
}

void QtQQ_Server::on_alterSelectPictureBtn_clicked() {
	// 获取选择的头像路径
	m_alterPoxPath = QFileDialog::getOpenFileName(this, tr("选择头像"), ".", "*.png;;*.jpg");
	if (0 == m_alterPoxPath.size()) {
		return;
	}

	// 将头像显示到标签
	QPixmap pixmap;
	pixmap.load(m_alterPoxPath);

	qreal widthRatio = (qreal)ui.alterHeadLabel->width() / (qreal)pixmap.width();
	qreal heightRatio = (qreal)ui.alterHeadLabel->height() / (qreal)pixmap.height();

	QSize size(pixmap.width() * widthRatio, pixmap.height() * heightRatio);
	ui.alterHeadLabel->setPixmap(pixmap.scaled(size));
}

void QtQQ_Server::on_alterBtn_clicked() { 

	// 员工部门
	QString alterEmployeeDep = ui.alterEmployeeDepBox->currentText();


	// 员工工号
	QString alterID = ui.alterIDLineEdit->text();

	// 员工姓名
	QString alterName = ui.alterNameLineEdit->text();
	if (alterName.isEmpty()) {
		QMessageBox::information(this, tr("提示"), tr("请输入员工姓名！"));
		return;
	}

	// 员工状态
	QString alterEmployeeStatus = ui.alterEmployeeStatusBox->currentText();
	int status = alterEmployeeStatus == "有效" ? 1 : 2;
	

	// 员工个性签名
	QString alterSign = ui.alterSignLineEdit->text();

	// 员工密码
	QString alterPwd = ui.alterPwdLineEdit->text();
	if (alterPwd.isEmpty()) {
		QMessageBox::information(this, tr("提示"), tr("请输入员工密码！"));
		return;
	}

	// 员工头像路径
	if (!m_alterPoxPath.size()) {
		QMessageBox::information(this, tr("提示"), tr("请选择员工头像路径！"));
		return;
	}
	//m_alterPoxPath.replace("/", "\\\\");

	int departmentID = ui.alterEmployeeDepBox->currentData().toInt();


	// 更新数据库数据
	QString sql = QString("UPDATE tab_employees SET departmentID=%1, employee_name='%2', employee_sign='%3', status=%4, picture='%5' WHERE employeeID=%6;")
					.arg(departmentID).arg(alterName).arg(alterSign).arg(status).arg(m_alterPoxPath).arg(alterID);
	QSqlQuery alterQuery;
	if (!alterQuery.exec(sql)) {
		MyLogDEBUG(sql.toUtf8());
		MyLogDEBUG("更新失败");
		QMessageBox::information(this, tr("提示"), tr("更新失败！"));
		return;
	}

	sql = QString("UPDATE tab_accounts SET account='%1', code='%2' WHERE employeeID=%3;").arg(alterName).arg(alterPwd).arg(alterID);
	QSqlQuery updateQuery;
	if (!updateQuery.exec(sql)) {
		MyLogDEBUG(sql.toUtf8());
		MyLogDEBUG("更新失败");
		QMessageBox::information(this, tr("提示"), tr("更新失败！"));
		return;
	}

	m_alterPoxPath = "";

	QMessageBox::information(this, tr("提示"), tr("修改成功！"));
	ui.alterWidget->setVisible(false);
}



void QtQQ_Server::onUDPbroadMsg(QByteArray &btData) {
	QString text = QString("数据：%1 开始广播").arg(QString(btData));
	MyLogDEBUG(text.toUtf8());
	qDebug() << QString::fromLocal8Bit(text.toLocal8Bit());


	for (quint16 port = gUdpPort; port < gUdpPort + 200; port++) {
		m_udpSender->writeDatagram(btData, btData.size(), QHostAddress::Broadcast, port);
	}
}


void QtQQ_Server::Offline() {

	// 更新登录状态为离线
	QString strSqlStatus = QString("UPDATE tab_employees SET online = 1");
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();

	QApplication::quit();
}

void QtQQ_Server::ShowAlterEmployeeInfo(const int employeesID) {

	QString sql = QString("SELECT * FROM tab_employees WHERE employeeID=%1").arg(employeesID);
	QSqlQuery query(sql);
	query.exec();
	if (!query.next()) {
		QMessageBox::information(this, tr("提示"), tr("请输入正确的员工QQ号！"));
		ui.logoutIDLineEdit->setFocus();
		return;
	}

	// 部门名称
	QString department_name = getDepartment(employeesID);
	ui.alterEmployeeDepBox->currentIndexChanged(department_name);
	ui.alterEmployeeDepBox->currentTextChanged(department_name);
	ui.alterEmployeeDepBox->setCurrentText(department_name);

	// 员工id
	QString employeeID = query.value("employeeID").toString();
	ui.alterIDLineEdit->setText(employeeID);

	// 员工名字
	QString employee_name = query.value("employee_name").toString();
	ui.alterNameLineEdit->setText(employee_name);

	// 个性签名
	QString employee_sign = query.value("employee_sign").toString();
	ui.alterSignLineEdit->setText(employee_sign);

	// 员工状态,1有效，2已注销
	int status = query.value("status").toInt();
	QString strStatus = status == 1 ? "有效" : "已注销";
	ui.alterEmployeeStatusBox->currentIndexChanged(strStatus);
	ui.alterEmployeeStatusBox->currentTextChanged(strStatus);
	ui.alterEmployeeStatusBox->setCurrentText(strStatus);

	// 图片
	m_alterPoxPath = query.value("picture").toString();
	// 将头像显示到标签
	QPixmap pixmap;
	pixmap.load(m_alterPoxPath);
	qreal widthRatio = (qreal)ui.alterHeadLabel->width() / (qreal)pixmap.width();
	qreal heightRatio = (qreal)ui.alterHeadLabel->height() / (qreal)pixmap.height();
	QSize size(pixmap.width() * widthRatio, pixmap.height() * heightRatio);
	ui.alterHeadLabel->setPixmap(pixmap.scaled(size));


	// 密码
	QString employee_pwd = getEmployeePwd(employeesID);
	ui.alterPwdLineEdit->setText(employee_pwd);
}

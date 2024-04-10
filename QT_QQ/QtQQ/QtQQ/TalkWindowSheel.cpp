#include "TalkWindowSheel.h"
#include "CommonUtils.h"
#include "EmotionWindow.h"
#include "public_type.h"
#include "TalkWindowItem.h"
#include "WindowManager.h"
#include "QMsgTextEdit.h"
#include "RecviveFile.h"

#include <QSqlQueryModel>
#include <QMessageBox>
#include <QFile>
#include <QSqlQuery>
#include <QThread>

QString gfileName;		// 文件名称
QString gfileData;		// 文件内容

QString currentAccount = "";	// 当前窗口账号

extern QString gLoginEmployeeID;
extern QMultiMap<int, QMap<int, QJsonArray>> g_message_info;	// 聊天记录

TalkWindowSheel::TalkWindowSheel(QWidget *parent)
	: BasicWindow(parent) {
	ui.setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
	initTcpSocket();
	initUdpSocket();

	
	QFile file("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (!file.size()) {
		QStringList employeesIDList = getEmployeeID();
		if (!createJSFile(employeesIDList)) {
			MyLogDEBUG(QString("更新js文件数据失败").toUtf8());
			QMessageBox::information(this, "提示", "更新js文件数据失败");
		}
	}
	
	messTimer = new QTimer(this);
	connect(messTimer, &QTimer::timeout, this, &TalkWindowSheel::onLoadMessage);
}

TalkWindowSheel::~TalkWindowSheel() {

	delete m_emotionWindow;
	m_emotionWindow = NULL;

}

void TalkWindowSheel::addTalkWindow(TalkWindow * talkWindow, TalkWindowItem * talkWindowItem, QString uid) {
	MyLogDEBUG(QString("uid：%1").arg(uid).toUtf8());

	ui.rightStackedWidget->addWidget(talkWindow);
	connect(m_emotionWindow, SIGNAL(signalEmotionWindowHide()), talkWindow, SLOT(onSetEmotionBtnStatus()));

	QListWidgetItem *aItem = new QListWidgetItem(ui.listWidget);
	m_talkwindowItemMap.insert(aItem, talkWindow);

	aItem->setSelected(true);

	
	// 判断是群聊还是单聊
	QSqlQueryModel sqlDepModel;
	QString sql = QString("SELECT picture FROM tab_department WHERE departmentID = %1").arg(uid);
	MyLogDEBUG(QString("sql语句：%1").arg(sql).toUtf8());
	sqlDepModel.setQuery(sql);

	int rows = sqlDepModel.rowCount();
	if (rows == 0) {	// 单聊
		sql = QString("SELECT picture FROM tab_employees WHERE employeeID = %1").arg(uid);
		sqlDepModel.setQuery(sql);
	}

	QModelIndex index;
	index = sqlDepModel.index(0, 0);	// 0行0列
	
	QImage img;
	img.load(sqlDepModel.data(index).toString());

	talkWindowItem->setHeadPixmap(QPixmap::fromImage(img));	// 设置头像
	ui.listWidget->addItem(aItem);
	ui.listWidget->setItemWidget(aItem, talkWindowItem);

	onTalkWindowItemClicked(aItem);

	connect(talkWindowItem, &TalkWindowItem::signalCloseClicked, [talkWindowItem, talkWindow, aItem, this]() {
		m_talkwindowItemMap.remove(aItem);
		talkWindow->close();
		ui.listWidget->takeItem(ui.listWidget->row(aItem));
		delete talkWindowItem;

		ui.rightStackedWidget->removeWidget(talkWindow);
		if (ui.rightStackedWidget->count() < 1) {
			close();
		}
	});
}

void TalkWindowSheel::setCurrentWidget(QWidget * widget) {
	MyLogDEBUG(QString("设置当前窗口").toUtf8());
	ui.rightStackedWidget->setCurrentWidget(widget);
}
const QMap<QListWidgetItem*, QWidget*>& TalkWindowSheel::getTalkWindowItemMap() const {
	return m_talkwindowItemMap;
}

void TalkWindowSheel::initControl() {
	MyLogDEBUG(QString("Qt聊天窗口初始化").toUtf8());

	loadStyleSheet("TalkWindow");
	setWindowTitle(QString::fromLocal8Bit("Qt聊天窗口"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();		// 隐藏表情窗口

	QList<int> leftWidgetSize;
	leftWidgetSize << 154 << width() - 154;
	ui.splitter->setSizes(leftWidgetSize);		// 分类器设置尺寸

	ui.listWidget->setStyle(new CustomProxyStyle(this));

	connect(ui.listWidget, &QListWidget::itemClicked, this, &TalkWindowSheel::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionItemClicked(int)));
}

void TalkWindowSheel::initTcpSocket() {
	MyLogDEBUG(QString("初始化TCP客户端").toUtf8());

	m_tcpClientSocket = new QTcpSocket(this);
	m_tcpClientSocket->connectToHost("127.0.0.1", gTcpPort);

}

void TalkWindowSheel::initUdpSocket() {
	MyLogDEBUG(QString("初始化UDP客户端").toUtf8());

	m_udpReceiver = new QUdpSocket(this);

	for (quint16 port = gUdpPort; port < gUdpPort + 200; port++) {
		if (m_udpReceiver->bind(port, QUdpSocket::ShareAddress)) {
			// 只要有一个端口绑定成功就可以退出啦
			break;
		}
	}

	connect(m_udpReceiver, &QUdpSocket::readyRead, this, &TalkWindowSheel::processPendingData);
}

QStringList TalkWindowSheel::getEmployeeID() {
	QStringList employeesIDList;
	QSqlQueryModel queryModel;
	QString sql = QString("SELECT employeeID FROM tab_employees WHERE status = 1");
	MyLogDEBUG(QString("sql语句：%1").arg(sql).toUtf8());

	queryModel.setQuery(sql);
	// 获得员工总数
	int employeesNum = queryModel.rowCount();
	MyLogDEBUG(QString("当前一共有：%1个员工").arg(employeesNum).toUtf8());
	QModelIndex index;
	for (int i = 0; i < employeesNum; i++) {
		index = queryModel.index(i, 0);		// 行 列
		employeesIDList << queryModel.data(index).toString();
	}

	return employeesIDList;
}

bool TalkWindowSheel::createJSFile(QStringList & employeesList) {
	MyLogDEBUG(QString("创建js文件").toUtf8());

	// 读取txt文件数据
	QString strFileTxt = ":/Resources/MainWindow/MsgHtml/msgtmpl.txt";
	QFile fileRead(strFileTxt);
	QString strFile = "";

	if (fileRead.open(QIODevice::ReadOnly)) {
		strFile = fileRead.readAll();
		fileRead.close();
	} else {
		MyLogDEBUG(QString("读取msgtmpl.txt文件失败").toUtf8());
		QMessageBox::information(this, tr("提示"), tr("读取msgtmpl.txt文件失败"));
		return false;
	}

	// 替换(external0, appendHtml0用作自己发信息使用
	QFile fileWrite("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (fileWrite.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		// 更新空值
		QString strSourceInitNull = "var external = null;";

		// 更新初始化
		QString strSourceInit = "external = channel.objects.external;";

		// 更新newWebChannel
		QString strSourceNew = "new QWebChannel(qt.webChannelTransport,			\
									function(channel) {							\
										external0 = channel.objects.external0;	\
										external = channel.objects.external;	\
									}											\
								);												\
								";

		// 更新追加recvHtml，脚本中有双引号无法直接进行赋值，采用读文件方式
		QString strSourceRecvHtml = "";
		QFile fileRecvHtml(":/Resources/MainWindow/MsgHtml/recvHtml.txt");
		if (fileRecvHtml.open(QIODevice::ReadOnly)) {
			strSourceRecvHtml = fileRecvHtml.readAll();
			fileRecvHtml.close();
		} else {
			MyLogDEBUG(QString("读取recvHtml.txt文件失败").toUtf8());
			QMessageBox::information(this, tr("提示"), tr("读取recvHtml.txt文件失败"));
			return false;
		}


		// 保存替换后的脚本
		QString strReplaceInitNull = "";
		QString strReplaceInit = "";
		QString strReplaceNew = "";
		QString strReplaceRecvHtml = "";

		for (int i = 0; i < employeesList.length(); i++) {
			// 编辑替换后的空值
			QString strInitNull = strSourceInitNull;
			strInitNull.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInitNull += strInitNull;
			strReplaceInitNull += "\n";

			// 编辑替换后的初始值
			QString strInit = strSourceInit;
			strInit.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInit += strInit;
			strReplaceInit += "\n";

			// 编辑替换后的 newWebChannel
			QString strNew = strSourceNew;
			strNew.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceNew += strNew;
			strReplaceNew += "\n";

			// 编辑替换后的 recvHtml
			QString strRecvHtml = strSourceRecvHtml;
			strRecvHtml.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strRecvHtml.replace("recvHtml", QString("recvHtml_%1").arg(employeesList.at(i)));
			strReplaceRecvHtml += strRecvHtml;
			strReplaceRecvHtml += "\n\n";
		}

		strFile.replace(strSourceInitNull, strReplaceInitNull);
		strFile.replace(strSourceInit, strReplaceInit);
		strFile.replace(strSourceNew, strReplaceNew);
		strFile.replace(strSourceRecvHtml, strReplaceRecvHtml);

		MyLogDEBUG(QString("最终生成的脚本数据：\n%1\n").arg(strFile).toUtf8());

		QTextStream stream(&fileWrite);
		stream << strFile;
		fileWrite.close();

		return true;
	
	} else {
		MyLogDEBUG(QString("写入msgtmpl.js文件失败").toUtf8());
		QMessageBox::information(this, tr("提示"), tr("写入msgtmpl.js文件失败"));
		return false;
	}
}

QString TalkWindowSheel::getEmployeeName(int employeesID) {
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

void TalkWindowSheel::handleReceivedMsg(QString senderEmployeeID, int msgType, QString strMsg) {
	QMsgTextEdit msgTextEdit;
	msgTextEdit.setFont(QFont("Microsoft YaHei"));
	msgTextEdit.setText(strMsg);

	if (msgType == 1) {			// 文本信息
		msgTextEdit.document()->toHtml();
	} else if (msgType == 0) {	// 表情信息
		const int emotionWidth = 3;
		int emotionNum = strMsg.length() / emotionWidth;
		for (int i = 0; i < emotionNum; i++) {
			msgTextEdit.addEmotionUrl(strMsg.mid(i * emotionWidth, emotionWidth).toInt());
		}
	}

	QString htmlText = msgTextEdit.document()->toHtml();

	// 文本html如果没有字体则添加字体    msgFont.txt
	if (!htmlText.contains(".png") && !htmlText.contains("</span>")) {
		QString fontHtml;
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly)) {
			fontHtml = file.readAll();
			fontHtml.replace("%1", strMsg);
			file.close();

		} else {
			MyLogDEBUG(QString("msgFont.txt文件打开失败！").toUtf8());
			QMessageBox::information(this, "提示", "msgFont.txt文件打开失败！");
			return;
		}

		if (!htmlText.contains(fontHtml)) {
			htmlText.replace(strMsg, fontHtml);
		}
	}

	TalkWindow *talkWindow = dynamic_cast<TalkWindow *>(ui.rightStackedWidget->currentWidget());
	//talkWindow->ui.msgWidget->appendMsg(htmlText);
	//talkWindow->ui.msgWidget->appendMsg(htmlText, senderEmployeeID);


	emit signalReload();

	QString curAccount = "";
	QString currentWindowAccount = WindowManager::getInstance()->findWindowName(talkWindow);
	currentAccount = currentWindowAccount;

	messTimer->stop();
	// 启动定时器加载聊天记录
	messTimer->start(320);

	ReadDatabaseMessage();
}

void TalkWindowSheel::LoadMessage(QString senderID, int msgType, QString strMsg) {
	QMsgTextEdit msgTextEdit;
	msgTextEdit.setFont(QFont("Microsoft YaHei"));
	msgTextEdit.setText(strMsg);

	if (msgType == 1) {			// 文本信息
		msgTextEdit.document()->toHtml();
	} else if (msgType == 0) {	// 表情信息
		const int emotionWidth = 3;
		int emotionNum = strMsg.length() / emotionWidth;
		for (int i = 0; i < emotionNum; i++) {
			msgTextEdit.addEmotionUrl(strMsg.mid(i * emotionWidth, emotionWidth).toInt());
		}
	}

	QString htmlText = msgTextEdit.document()->toHtml();

	// 文本html如果没有字体则添加字体    msgFont.txt
	if (!htmlText.contains(".png") && !htmlText.contains("</span>")) {
		QString fontHtml;
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly)) {
			fontHtml = file.readAll();
			fontHtml.replace("%1", strMsg);
			file.close();

		} else {
			MyLogDEBUG(QString("msgFont.txt文件打开失败！").toUtf8());
			QMessageBox::information(this, "提示", "msgFont.txt文件打开失败！");
			return;
		}

		if (!htmlText.contains(fontHtml)) {
			htmlText.replace(strMsg, fontHtml);
		}
	}

	TalkWindow *talkWindow = dynamic_cast<TalkWindow *>(ui.rightStackedWidget->currentWidget());

	if (senderID == gLoginEmployeeID) {
		talkWindow->ui.msgWidget->LoadMsg(htmlText);
	} else {
		talkWindow->ui.msgWidget->LoadMsg(htmlText, senderID);
	}
	
}


// 文本数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型(1) + 数据长度 + 数据
// 表情数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型(0) + 表情个数 + images + 数据
// 文件数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型(2) + 文件长度 + "bytes" + 文件名称 + "data_begin" + 文件内容
// msgType 0表情信息	1文本信息	2文件信息
void TalkWindowSheel::updateSendTcpMsg(QString & strData, int & msgType, QString fileName) {
	MyLogDEBUG(QString("数据通过网络发送，strData = %1   msgType = %2   fileName = %3")
			   .arg(strData).arg(msgType).arg(fileName).toUtf8());

	// 获取当前获得窗口
	TalkWindow *curTalkWindow = dynamic_cast<TalkWindow *>(ui.rightStackedWidget->currentWidget());
	QString talkId = curTalkWindow->GetTalkId();

	QString strGroupFlag = "";
	QString strSend = "";
	if (talkId.length() == 4) {		// QQ群长度为4
		strGroupFlag = "1";
	} else if (talkId.length() == 5) {	// 个人QQ号为5
		strGroupFlag = "0";
	} else {
		MyLogDEBUG(QString("标识有误：%1").arg(talkId.length()).toUtf8());
		return;
	}


	const int sourceDataLength = strData.length();
	int dataLength = QString::number(sourceDataLength).length();
	QString strdataLength = "";

	if (1 == msgType) {		// 发送文本信息
		// 文本信息的长度约定为5位
		if (1 == dataLength) {
			strdataLength = "0000" + QString::number(sourceDataLength);
		} else if (2 == dataLength) {
			strdataLength = "000" + QString::number(sourceDataLength);
		} else if (3 == dataLength) {
			strdataLength = "00" + QString::number(sourceDataLength);
		} else if (4 == dataLength) {
			strdataLength = "0" + QString::number(sourceDataLength);
		} else if (5 == dataLength) {
			strdataLength = QString::number(sourceDataLength);
		} else {
			MyLogDEBUG(QString("数据长度有误：%1").arg(dataLength).toUtf8());
			QMessageBox::information(this, "提示", "不合理的数据长度");
			return;
		}

		// 发送的信息汇总
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "1" + strdataLength + strData;
	
	} else if (0 == msgType) {	// 发送的表情信息
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "0" + strData;
	
	} else if (2 == msgType) {	// 发送的文件信息
		// 获取文件的字节长度
		QByteArray bt = strData.toUtf8();
		QString strFileLength = QString::number(bt.length());

		strSend = strGroupFlag + gLoginEmployeeID + talkId + "2" + strFileLength + "bytes" + fileName + "data_begin" + strData;
	
	} else {
		MyLogDEBUG(QString("要发送的数据类型有误：%1").arg(msgType).toUtf8());
		QMessageBox::information(this, "提示", "要发送的数据类型有误");
		return;
	}

	MyLogDEBUG(QString("发送出去的数据：%1").arg(strSend).toUtf8());

	QByteArray dataBt;
	dataBt.resize(strSend.length());
	dataBt = strSend.toUtf8();

	m_tcpClientSocket->write(dataBt);
}


void TalkWindowSheel::onEmotionBtnClicked(bool) {
	MyLogDEBUG(QString("表情按钮被点击").toUtf8());

	m_emotionWindow->setVisible(m_emotionWindow->isVisible());
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));	// 将当前控件的相对位置转换为屏幕的绝对位置

	emotionPoint.setX(emotionPoint.x() + 170);
	emotionPoint.setY(emotionPoint.y() + 220);
	m_emotionWindow->move(emotionPoint);
	m_emotionWindow->show();
}


void TalkWindowSheel::onTalkWindowItemClicked(QListWidgetItem *item) {

	QWidget *talkWindowWidget = m_talkwindowItemMap.find(item).value();
	ui.rightStackedWidget->setCurrentWidget(talkWindowWidget);
	emit signalReload();

	QString curAccount = "";
	QString currentWindowAccount = WindowManager::getInstance()->findWindowName(talkWindowWidget);
	currentAccount = currentWindowAccount;

	messTimer->stop();
	// 启动定时器加载聊天记录
	messTimer->start(320);

	ReadDatabaseMessage();

	return;

	/*
	QString curAccount = "";
	QString currentWindowAccount = WindowManager::getInstance()->findWindowName(talkWindowWidget);
	currentAccount = currentWindowAccount;
	if (4 == currentWindowAccount.length()) {
		curAccount = currentWindowAccount;
	} else {
		curAccount = gLoginEmployeeID;
	}


	QJsonArray messageArr;
	// 找到匹配的收信者的qq对应的聊天记录
	QList<QMap<int, QJsonArray>> messageList = g_message_info.values(curAccount.toInt());
	for (int i = 0; i < messageList.size(); i++) {
		QMap<int, QJsonArray> temMap = messageList.at(i);
		if (temMap.firstKey() == currentWindowAccount.toInt()) {
			messageArr = temMap.first();
			break;
		}
	}

	if (0 >= messageArr.size()) {
		return;
	}

	for (int i = 0; i < messageArr.size(); i++) {
		QJsonObject messageObj = messageArr.at(i).toObject();
		QString sender = messageObj.value("sender").toString();
		QString message = messageObj.value("message").toString();
		int msgType = message.mid(0, 1).toInt();	// 获得信息类型

		message = message.mid(1);		// 移除第一位数据类型

		// 聊天记录插入
		LoadMessage(sender, msgType, message);
	}
	*/
}

void TalkWindowSheel::onEmotionItemClicked(int emotionNum) {
	MyLogDEBUG(QString("%1 表情被点击").arg(emotionNum).toUtf8());

	TalkWindow *curTalkWindow = dynamic_cast<TalkWindow *>(ui.rightStackedWidget->currentWidget());
	if (curTalkWindow) {
		curTalkWindow->addEmotionImage(emotionNum);
	}
}

/********************************************************************************************************************************************************
	
	数据包格式：
		文本数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型（1） + 数据长度 + 数据
		表情数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型（0） + 表情个数 + images + 表情名称
		文件数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型（2） + 文件字节数 + bytes + 文件名 + data_begin + 文件数据

		群聊标志占1位，0表示单聊，1表示群聊
		信息类型占1位，0表示表情信息，1表示文本信息，2白哦是文件信息

		QQ号占5位，群QQ号占4位，数据长度占5位，表情名称占3位
		（注意：当群聊标志为1时，则数据包中没有收信息员工QQ号，而时收信息群QQ号；
				当群聊标志为0时，则数据包中没有收信息群QQ号，而是收信息员工QQ号；）

		群聊文本信息如：1100012001100005Hello		表示QQ10001向群2001发送文本信息，长度是5，数据是Hello
		单聊图片信息如：0100011000202images060125		表示QQ10001向QQ10002发送表情信息，表情个数是2，表情名称是60.png和125.png
		群聊文件信息如：1100052000210bytestest.txtdata_beginHelloworld	表示QQ10005向群2000发送文件信息，文件长度是10，文件名是test.txt，文件内容是Helloworld

		群聊文件信息解析：1 10001 2001  1 00005 Hello
		单聊图片信息解析：0 10001 10002 0 2 060和125
		群聊文件信息解析：1 10005 2000  2 10 test.txt Helloworld

*********************************************************************************************************************************************************/
void TalkWindowSheel::processPendingData() {
	// 端口中有未处理的数据
	while (m_udpReceiver->hasPendingDatagrams()) {
		const static int groupFlagWidth = 1;	// 群聊标志占位
		const static int groupWidth = 4;		// 群QQ号宽度
		const static int employeeWidth = 5;		// 员工QQ号宽度
		const static int msgTypeWidth = 1;		// 信息类型宽度
		const static int msgLengthWidth = 5;	// 文本信息长度的宽度
		const static int pictureWidth = 3;		// 表情图片的宽度


		// 读取udp数据		
		int btDataSize = m_udpReceiver->pendingDatagramSize();
		QByteArray btData(btDataSize, Qt::Initialization::Uninitialized);
		m_udpReceiver->readDatagram(btData.data(), btDataSize);

		QString strData = btData.data();
		QString strWindowID;	// 聊天窗口id，群聊则是群号，单聊则是员工qq号
		QString strSendEmployeeID, strRecvieEmployeeID;		// 发送端QQ号和接收端QQ号
		QString strMsg;			// 数据
		
		int msgLen;				// 数据长度
		int msgType;			// 数据类型

		MyLogDEBUG(QString("UDP收到数据是：%1").arg(strData).toUtf8());
		strSendEmployeeID = strData.mid(groupFlagWidth, employeeWidth);	// 获取发送端QQ号

		// 自己发的信息不做处理
		if (strSendEmployeeID == gLoginEmployeeID) {
			return;
		}

		
		if (btData[0] == '1') {		// 群聊
			strWindowID = strData.mid(groupFlagWidth + employeeWidth, groupWidth);		// 获取接收端群号

			// 获取当前获得窗口
			//TalkWindow *curTalkWindow = dynamic_cast<TalkWindow *>(ui.rightStackedWidget->currentWidget());
			//QString talkId = curTalkWindow->GetTalkId();
			
			// 不是发给自己的不接受处理
			bool flag = false;
			for (int i = 0; i < ui.rightStackedWidget->count(); i++) {
				// 获取当前获得窗口
				TalkWindow *curTalkWindow = dynamic_cast<TalkWindow *>(ui.rightStackedWidget->widget(i));
				QString talkId = curTalkWindow->GetTalkId();

				if (talkId == strWindowID) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				return;
			}


			QChar cMsgType = btData[groupFlagWidth + employeeWidth + groupWidth];
			if (cMsgType == '1') {			// 文本信息
				msgType = 1;
				msgLen = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth, msgLengthWidth).toInt();	// 获取信息长度
				strMsg = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth + msgLengthWidth, msgLen);	// 获取文本信息

			} else if (cMsgType == '0') {	// 表情信息
				msgType = 0;
				int posImages = strData.indexOf("images");
				strMsg = strData.right(strData.length() - posImages - QString("images").length());	// 获取所有表情名称，信息数据


			} else if (cMsgType == '2') {	// 文件信息
				msgType = 2;
				int bytesWidth = QString("bytes").length();
				int posBytes = strData.indexOf("bytes");
				int data_begin_width = QString("data_begin").length();
				int pos_data_begin = strData.indexOf("data_begin");

				// 获取文件名称
				QString fileName = strData.mid(posBytes + bytesWidth, pos_data_begin - bytesWidth - posBytes);
				gfileName = fileName;

				// 文件内容
				//int dataLenthWidth = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth, posBytes).toInt();
				int posData = pos_data_begin + data_begin_width;
				strMsg = strData.mid(posData);		// 获取文件数据
				gfileData = strMsg;

				// 根据employeeID获取发送者姓名				
				int employeeID = strSendEmployeeID.toInt();
				QString sender = getEmployeeName(employeeID);

				RecviveFile *recvFile = new RecviveFile(this);
				connect(recvFile, &RecviveFile::refuseFile, [this]() { return; });
				QString msgLabel = "收到来自" + sender + "发来的文件，是否接收？";
				recvFile->setMsg(msgLabel);
				recvFile->show();
			}

		} else {	// 单聊
			strRecvieEmployeeID = strData.mid(groupFlagWidth + employeeWidth, employeeWidth);	// 接收者QQ号
			strWindowID = strSendEmployeeID;													// 发送者QQ号

			// 不是发给我的信息不做处理
			if (strRecvieEmployeeID != gLoginEmployeeID) {
				return;
			}

			// 获取信息的类型
			QChar cMsgType = btData[groupFlagWidth + employeeWidth + employeeWidth];
			if (cMsgType == '1') {			// 文本信息
				msgType = 1;

				// 文本信息长度
				msgLen = strData.mid(groupFlagWidth + employeeWidth + employeeWidth + msgTypeWidth, msgLengthWidth).toInt();
				// 文本信息
				strMsg = strData.mid(groupFlagWidth + employeeWidth + employeeWidth + msgTypeWidth + msgLengthWidth, msgLen);
			
			} else if (cMsgType == '0') {	// 表情信息
				msgType = 0;
				int posImages = strData.indexOf("images");
				int imagesWidth = QString("images").length();
				//strMsg = strData.right(strData.length() - posImages - imagesWidth);	// 获取所有表情名称，信息数据
				strMsg = strData.mid(posImages + imagesWidth);
			
			} else if (cMsgType == '2') {	// 文件信息
				msgType = 2;

				int bytesWidth = QString("bytes").length();
				int posBytes = strData.indexOf("bytes");
				int data_begin_width = QString("data_begin").length();
				int pos_data_begin = strData.indexOf("data_begin");

				// 根据employeeID获取发送者姓名				
				int employeeID = strSendEmployeeID.toInt();
				QString sender = getEmployeeName(employeeID);

				// 文件名称
				QString fileName = strData.mid(posBytes + bytesWidth, pos_data_begin - posBytes - bytesWidth);
				gfileName = fileName;

				// 文件内容长度
				//int dataLenthWidth = strData.mid(groupFlagWidth + employeeWidth + employeeWidth + msgTypeWidth, posBytes).toInt();

				// 文件内容
				strMsg = strData.mid(pos_data_begin + data_begin_width);
				gfileData = strMsg;

				RecviveFile *recvFile = new RecviveFile(this);
				connect(recvFile, &RecviveFile::refuseFile, [this]() { return; });
				QString msgLabel = "收到来自  " + sender + "  发来的文件，是否接收？";
				recvFile->setMsg(msgLabel);
				recvFile->show();
			}
		}

		// 将聊天窗口设为活动的窗口
		QWidget *widget = WindowManager::getInstance()->findWindowName(strWindowID);
		if (widget) {	// 聊天窗口存在
			this->setCurrentWidget(widget);

			// 同步激活左侧聊天窗口
			QListWidgetItem *item = m_talkwindowItemMap.key(widget);
			item->setSelected(true);

		} else {		// 聊天窗口未打开
			return;
		}


		if (msgType != 2) {
			handleReceivedMsg(strSendEmployeeID, msgType, strMsg);
		
		} else if (msgType == 2) {	// 文件信息另作处理

		}

		
	}
}

void TalkWindowSheel::onLoadNewMessage() {


}



// 加载聊天记记录
void TalkWindowSheel::onLoadMessage() { 
	messTimer->stop();
	QString curAccount;
	if (4 == currentAccount.length()) {
		curAccount = currentAccount;
	} else {
		curAccount = gLoginEmployeeID;
	}


	QJsonArray messageArr;
	// 找到匹配的收信者的qq对应的聊天记录
	QList<QMap<int, QJsonArray>> messageList = g_message_info.values(curAccount.toInt());
	for (int i = 0; i < messageList.size(); i++) {
		QMap<int, QJsonArray> temMap = messageList.at(i);
		if (temMap.firstKey() == currentAccount.toInt()) {
			messageArr = temMap.first();
			break;
		}
	}

	if (0 >= messageArr.size()) {
		return;
	}

	for (int i = 0; i < messageArr.size(); i++) {
		QJsonObject messageObj = messageArr.at(i).toObject();
		QString sender = messageObj.value("sender").toString();
		QString message = messageObj.value("message").toString();
		int msgType = message.mid(0, 1).toInt();	// 获得信息类型

		message = message.mid(1);		// 移除第一位数据类型

		// 聊天记录插入
		LoadMessage(sender, msgType, message);
	}
}


void TalkWindowSheel::ReadDatabaseMessage() {
	QString sql = QString("SELECT * FROM tab_chat");
	QSqlQuery query;
	query.exec(sql);

	g_message_info.clear();

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
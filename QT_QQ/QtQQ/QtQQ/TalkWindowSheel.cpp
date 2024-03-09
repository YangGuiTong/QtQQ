#include "TalkWindowSheel.h"
#include "CommonUtils.h"
#include "EmotionWindow.h"
#include "public_type.h"
#include "TalkWindowItem.h"

#include <QSqlQueryModel>
#include <QMessageBox>
#include <QFile>

extern QString gLoginEmployeeID;

TalkWindowSheel::TalkWindowSheel(QWidget *parent)
	: BasicWindow(parent) {
	ui.setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
	initTcpSocket();

	
	QFile file("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (!file.size()) {
		QStringList employeesIDList = getEmployeeID();
		if (!createJSFile(employeesIDList)) {
			MyLogDEBUG(QString("更新js文件数据失败").toUtf8());
			QMessageBox::information(this, "提示", "更新js文件数据失败");
		}
	}
	
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
	m_tcpClientSocket->connectToHost("127.0.0.1", gtcpProt);

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

// 文本数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型(1) + 数据长度 + 数据
// 表情数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型(0) + 表情个数 + images + 数据
// 文件数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号（群QQ号） + 信息类型(2) + 文件长度 + "bytes" + 文件名称 + "data_begin" + 文件内容
// msgType 0表情信息	1文本信息	2文件信息
void TalkWindowSheel::updateSendTcpMsg(QString & strData, int & msgType, QString fileName) {
	MyLogDEBUG(QString("数据通过网络发送，strData = %1   msgType = %2   sFile = %3").toUtf8());

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
}

void TalkWindowSheel::onEmotionItemClicked(int emotionNum) {
	MyLogDEBUG(QString("%1 表情被点击").arg(emotionNum).toUtf8());

	TalkWindow *curTalkWindow = dynamic_cast<TalkWindow *>(ui.rightStackedWidget->currentWidget());
	if (curTalkWindow) {
		curTalkWindow->addEmotionImage(emotionNum);
	}
}

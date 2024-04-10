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

QString gfileName;		// �ļ�����
QString gfileData;		// �ļ�����

QString currentAccount = "";	// ��ǰ�����˺�

extern QString gLoginEmployeeID;
extern QMultiMap<int, QMap<int, QJsonArray>> g_message_info;	// �����¼

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
			MyLogDEBUG(QString("����js�ļ�����ʧ��").toUtf8());
			QMessageBox::information(this, "��ʾ", "����js�ļ�����ʧ��");
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
	MyLogDEBUG(QString("uid��%1").arg(uid).toUtf8());

	ui.rightStackedWidget->addWidget(talkWindow);
	connect(m_emotionWindow, SIGNAL(signalEmotionWindowHide()), talkWindow, SLOT(onSetEmotionBtnStatus()));

	QListWidgetItem *aItem = new QListWidgetItem(ui.listWidget);
	m_talkwindowItemMap.insert(aItem, talkWindow);

	aItem->setSelected(true);

	
	// �ж���Ⱥ�Ļ��ǵ���
	QSqlQueryModel sqlDepModel;
	QString sql = QString("SELECT picture FROM tab_department WHERE departmentID = %1").arg(uid);
	MyLogDEBUG(QString("sql��䣺%1").arg(sql).toUtf8());
	sqlDepModel.setQuery(sql);

	int rows = sqlDepModel.rowCount();
	if (rows == 0) {	// ����
		sql = QString("SELECT picture FROM tab_employees WHERE employeeID = %1").arg(uid);
		sqlDepModel.setQuery(sql);
	}

	QModelIndex index;
	index = sqlDepModel.index(0, 0);	// 0��0��
	
	QImage img;
	img.load(sqlDepModel.data(index).toString());

	talkWindowItem->setHeadPixmap(QPixmap::fromImage(img));	// ����ͷ��
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
	MyLogDEBUG(QString("���õ�ǰ����").toUtf8());
	ui.rightStackedWidget->setCurrentWidget(widget);
}
const QMap<QListWidgetItem*, QWidget*>& TalkWindowSheel::getTalkWindowItemMap() const {
	return m_talkwindowItemMap;
}

void TalkWindowSheel::initControl() {
	MyLogDEBUG(QString("Qt���촰�ڳ�ʼ��").toUtf8());

	loadStyleSheet("TalkWindow");
	setWindowTitle(QString::fromLocal8Bit("Qt���촰��"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();		// ���ر��鴰��

	QList<int> leftWidgetSize;
	leftWidgetSize << 154 << width() - 154;
	ui.splitter->setSizes(leftWidgetSize);		// ���������óߴ�

	ui.listWidget->setStyle(new CustomProxyStyle(this));

	connect(ui.listWidget, &QListWidget::itemClicked, this, &TalkWindowSheel::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionItemClicked(int)));
}

void TalkWindowSheel::initTcpSocket() {
	MyLogDEBUG(QString("��ʼ��TCP�ͻ���").toUtf8());

	m_tcpClientSocket = new QTcpSocket(this);
	m_tcpClientSocket->connectToHost("127.0.0.1", gTcpPort);

}

void TalkWindowSheel::initUdpSocket() {
	MyLogDEBUG(QString("��ʼ��UDP�ͻ���").toUtf8());

	m_udpReceiver = new QUdpSocket(this);

	for (quint16 port = gUdpPort; port < gUdpPort + 200; port++) {
		if (m_udpReceiver->bind(port, QUdpSocket::ShareAddress)) {
			// ֻҪ��һ���˿ڰ󶨳ɹ��Ϳ����˳���
			break;
		}
	}

	connect(m_udpReceiver, &QUdpSocket::readyRead, this, &TalkWindowSheel::processPendingData);
}

QStringList TalkWindowSheel::getEmployeeID() {
	QStringList employeesIDList;
	QSqlQueryModel queryModel;
	QString sql = QString("SELECT employeeID FROM tab_employees WHERE status = 1");
	MyLogDEBUG(QString("sql��䣺%1").arg(sql).toUtf8());

	queryModel.setQuery(sql);
	// ���Ա������
	int employeesNum = queryModel.rowCount();
	MyLogDEBUG(QString("��ǰһ���У�%1��Ա��").arg(employeesNum).toUtf8());
	QModelIndex index;
	for (int i = 0; i < employeesNum; i++) {
		index = queryModel.index(i, 0);		// �� ��
		employeesIDList << queryModel.data(index).toString();
	}

	return employeesIDList;
}

bool TalkWindowSheel::createJSFile(QStringList & employeesList) {
	MyLogDEBUG(QString("����js�ļ�").toUtf8());

	// ��ȡtxt�ļ�����
	QString strFileTxt = ":/Resources/MainWindow/MsgHtml/msgtmpl.txt";
	QFile fileRead(strFileTxt);
	QString strFile = "";

	if (fileRead.open(QIODevice::ReadOnly)) {
		strFile = fileRead.readAll();
		fileRead.close();
	} else {
		MyLogDEBUG(QString("��ȡmsgtmpl.txt�ļ�ʧ��").toUtf8());
		QMessageBox::information(this, tr("��ʾ"), tr("��ȡmsgtmpl.txt�ļ�ʧ��"));
		return false;
	}

	// �滻(external0, appendHtml0�����Լ�����Ϣʹ��
	QFile fileWrite("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (fileWrite.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		// ���¿�ֵ
		QString strSourceInitNull = "var external = null;";

		// ���³�ʼ��
		QString strSourceInit = "external = channel.objects.external;";

		// ����newWebChannel
		QString strSourceNew = "new QWebChannel(qt.webChannelTransport,			\
									function(channel) {							\
										external0 = channel.objects.external0;	\
										external = channel.objects.external;	\
									}											\
								);												\
								";

		// ����׷��recvHtml���ű�����˫�����޷�ֱ�ӽ��и�ֵ�����ö��ļ���ʽ
		QString strSourceRecvHtml = "";
		QFile fileRecvHtml(":/Resources/MainWindow/MsgHtml/recvHtml.txt");
		if (fileRecvHtml.open(QIODevice::ReadOnly)) {
			strSourceRecvHtml = fileRecvHtml.readAll();
			fileRecvHtml.close();
		} else {
			MyLogDEBUG(QString("��ȡrecvHtml.txt�ļ�ʧ��").toUtf8());
			QMessageBox::information(this, tr("��ʾ"), tr("��ȡrecvHtml.txt�ļ�ʧ��"));
			return false;
		}


		// �����滻��Ľű�
		QString strReplaceInitNull = "";
		QString strReplaceInit = "";
		QString strReplaceNew = "";
		QString strReplaceRecvHtml = "";

		for (int i = 0; i < employeesList.length(); i++) {
			// �༭�滻��Ŀ�ֵ
			QString strInitNull = strSourceInitNull;
			strInitNull.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInitNull += strInitNull;
			strReplaceInitNull += "\n";

			// �༭�滻��ĳ�ʼֵ
			QString strInit = strSourceInit;
			strInit.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInit += strInit;
			strReplaceInit += "\n";

			// �༭�滻��� newWebChannel
			QString strNew = strSourceNew;
			strNew.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceNew += strNew;
			strReplaceNew += "\n";

			// �༭�滻��� recvHtml
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

		MyLogDEBUG(QString("�������ɵĽű����ݣ�\n%1\n").arg(strFile).toUtf8());

		QTextStream stream(&fileWrite);
		stream << strFile;
		fileWrite.close();

		return true;
	
	} else {
		MyLogDEBUG(QString("д��msgtmpl.js�ļ�ʧ��").toUtf8());
		QMessageBox::information(this, tr("��ʾ"), tr("д��msgtmpl.js�ļ�ʧ��"));
		return false;
	}
}

QString TalkWindowSheel::getEmployeeName(int employeesID) {
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

void TalkWindowSheel::handleReceivedMsg(QString senderEmployeeID, int msgType, QString strMsg) {
	QMsgTextEdit msgTextEdit;
	msgTextEdit.setFont(QFont("Microsoft YaHei"));
	msgTextEdit.setText(strMsg);

	if (msgType == 1) {			// �ı���Ϣ
		msgTextEdit.document()->toHtml();
	} else if (msgType == 0) {	// ������Ϣ
		const int emotionWidth = 3;
		int emotionNum = strMsg.length() / emotionWidth;
		for (int i = 0; i < emotionNum; i++) {
			msgTextEdit.addEmotionUrl(strMsg.mid(i * emotionWidth, emotionWidth).toInt());
		}
	}

	QString htmlText = msgTextEdit.document()->toHtml();

	// �ı�html���û���������������    msgFont.txt
	if (!htmlText.contains(".png") && !htmlText.contains("</span>")) {
		QString fontHtml;
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly)) {
			fontHtml = file.readAll();
			fontHtml.replace("%1", strMsg);
			file.close();

		} else {
			MyLogDEBUG(QString("msgFont.txt�ļ���ʧ�ܣ�").toUtf8());
			QMessageBox::information(this, "��ʾ", "msgFont.txt�ļ���ʧ�ܣ�");
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
	// ������ʱ�����������¼
	messTimer->start(320);

	ReadDatabaseMessage();
}

void TalkWindowSheel::LoadMessage(QString senderID, int msgType, QString strMsg) {
	QMsgTextEdit msgTextEdit;
	msgTextEdit.setFont(QFont("Microsoft YaHei"));
	msgTextEdit.setText(strMsg);

	if (msgType == 1) {			// �ı���Ϣ
		msgTextEdit.document()->toHtml();
	} else if (msgType == 0) {	// ������Ϣ
		const int emotionWidth = 3;
		int emotionNum = strMsg.length() / emotionWidth;
		for (int i = 0; i < emotionNum; i++) {
			msgTextEdit.addEmotionUrl(strMsg.mid(i * emotionWidth, emotionWidth).toInt());
		}
	}

	QString htmlText = msgTextEdit.document()->toHtml();

	// �ı�html���û���������������    msgFont.txt
	if (!htmlText.contains(".png") && !htmlText.contains("</span>")) {
		QString fontHtml;
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly)) {
			fontHtml = file.readAll();
			fontHtml.replace("%1", strMsg);
			file.close();

		} else {
			MyLogDEBUG(QString("msgFont.txt�ļ���ʧ�ܣ�").toUtf8());
			QMessageBox::information(this, "��ʾ", "msgFont.txt�ļ���ʧ�ܣ�");
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


// �ı����ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�ȺQQ�ţ� + ��Ϣ����(1) + ���ݳ��� + ����
// �������ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�ȺQQ�ţ� + ��Ϣ����(0) + ������� + images + ����
// �ļ����ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�ȺQQ�ţ� + ��Ϣ����(2) + �ļ����� + "bytes" + �ļ����� + "data_begin" + �ļ�����
// msgType 0������Ϣ	1�ı���Ϣ	2�ļ���Ϣ
void TalkWindowSheel::updateSendTcpMsg(QString & strData, int & msgType, QString fileName) {
	MyLogDEBUG(QString("����ͨ�����緢�ͣ�strData = %1   msgType = %2   fileName = %3")
			   .arg(strData).arg(msgType).arg(fileName).toUtf8());

	// ��ȡ��ǰ��ô���
	TalkWindow *curTalkWindow = dynamic_cast<TalkWindow *>(ui.rightStackedWidget->currentWidget());
	QString talkId = curTalkWindow->GetTalkId();

	QString strGroupFlag = "";
	QString strSend = "";
	if (talkId.length() == 4) {		// QQȺ����Ϊ4
		strGroupFlag = "1";
	} else if (talkId.length() == 5) {	// ����QQ��Ϊ5
		strGroupFlag = "0";
	} else {
		MyLogDEBUG(QString("��ʶ����%1").arg(talkId.length()).toUtf8());
		return;
	}


	const int sourceDataLength = strData.length();
	int dataLength = QString::number(sourceDataLength).length();
	QString strdataLength = "";

	if (1 == msgType) {		// �����ı���Ϣ
		// �ı���Ϣ�ĳ���Լ��Ϊ5λ
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
			MyLogDEBUG(QString("���ݳ�������%1").arg(dataLength).toUtf8());
			QMessageBox::information(this, "��ʾ", "����������ݳ���");
			return;
		}

		// ���͵���Ϣ����
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "1" + strdataLength + strData;
	
	} else if (0 == msgType) {	// ���͵ı�����Ϣ
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "0" + strData;
	
	} else if (2 == msgType) {	// ���͵��ļ���Ϣ
		// ��ȡ�ļ����ֽڳ���
		QByteArray bt = strData.toUtf8();
		QString strFileLength = QString::number(bt.length());

		strSend = strGroupFlag + gLoginEmployeeID + talkId + "2" + strFileLength + "bytes" + fileName + "data_begin" + strData;
	
	} else {
		MyLogDEBUG(QString("Ҫ���͵�������������%1").arg(msgType).toUtf8());
		QMessageBox::information(this, "��ʾ", "Ҫ���͵�������������");
		return;
	}

	MyLogDEBUG(QString("���ͳ�ȥ�����ݣ�%1").arg(strSend).toUtf8());

	QByteArray dataBt;
	dataBt.resize(strSend.length());
	dataBt = strSend.toUtf8();

	m_tcpClientSocket->write(dataBt);
}


void TalkWindowSheel::onEmotionBtnClicked(bool) {
	MyLogDEBUG(QString("���鰴ť�����").toUtf8());

	m_emotionWindow->setVisible(m_emotionWindow->isVisible());
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));	// ����ǰ�ؼ������λ��ת��Ϊ��Ļ�ľ���λ��

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
	// ������ʱ�����������¼
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
	// �ҵ�ƥ��������ߵ�qq��Ӧ�������¼
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
		int msgType = message.mid(0, 1).toInt();	// �����Ϣ����

		message = message.mid(1);		// �Ƴ���һλ��������

		// �����¼����
		LoadMessage(sender, msgType, message);
	}
	*/
}

void TalkWindowSheel::onEmotionItemClicked(int emotionNum) {
	MyLogDEBUG(QString("%1 ���鱻���").arg(emotionNum).toUtf8());

	TalkWindow *curTalkWindow = dynamic_cast<TalkWindow *>(ui.rightStackedWidget->currentWidget());
	if (curTalkWindow) {
		curTalkWindow->addEmotionImage(emotionNum);
	}
}

/********************************************************************************************************************************************************
	
	���ݰ���ʽ��
		�ı����ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�ȺQQ�ţ� + ��Ϣ���ͣ�1�� + ���ݳ��� + ����
		�������ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�ȺQQ�ţ� + ��Ϣ���ͣ�0�� + ������� + images + ��������
		�ļ����ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�ȺQQ�ţ� + ��Ϣ���ͣ�2�� + �ļ��ֽ��� + bytes + �ļ��� + data_begin + �ļ�����

		Ⱥ�ı�־ռ1λ��0��ʾ���ģ�1��ʾȺ��
		��Ϣ����ռ1λ��0��ʾ������Ϣ��1��ʾ�ı���Ϣ��2��Ŷ���ļ���Ϣ

		QQ��ռ5λ��ȺQQ��ռ4λ�����ݳ���ռ5λ����������ռ3λ
		��ע�⣺��Ⱥ�ı�־Ϊ1ʱ�������ݰ���û������ϢԱ��QQ�ţ���ʱ����ϢȺQQ�ţ�
				��Ⱥ�ı�־Ϊ0ʱ�������ݰ���û������ϢȺQQ�ţ���������ϢԱ��QQ�ţ���

		Ⱥ���ı���Ϣ�磺1100012001100005Hello		��ʾQQ10001��Ⱥ2001�����ı���Ϣ��������5��������Hello
		����ͼƬ��Ϣ�磺0100011000202images060125		��ʾQQ10001��QQ10002���ͱ�����Ϣ�����������2������������60.png��125.png
		Ⱥ���ļ���Ϣ�磺1100052000210bytestest.txtdata_beginHelloworld	��ʾQQ10005��Ⱥ2000�����ļ���Ϣ���ļ�������10���ļ�����test.txt���ļ�������Helloworld

		Ⱥ���ļ���Ϣ������1 10001 2001  1 00005 Hello
		����ͼƬ��Ϣ������0 10001 10002 0 2 060��125
		Ⱥ���ļ���Ϣ������1 10005 2000  2 10 test.txt Helloworld

*********************************************************************************************************************************************************/
void TalkWindowSheel::processPendingData() {
	// �˿�����δ���������
	while (m_udpReceiver->hasPendingDatagrams()) {
		const static int groupFlagWidth = 1;	// Ⱥ�ı�־ռλ
		const static int groupWidth = 4;		// ȺQQ�ſ��
		const static int employeeWidth = 5;		// Ա��QQ�ſ��
		const static int msgTypeWidth = 1;		// ��Ϣ���Ϳ��
		const static int msgLengthWidth = 5;	// �ı���Ϣ���ȵĿ��
		const static int pictureWidth = 3;		// ����ͼƬ�Ŀ��


		// ��ȡudp����		
		int btDataSize = m_udpReceiver->pendingDatagramSize();
		QByteArray btData(btDataSize, Qt::Initialization::Uninitialized);
		m_udpReceiver->readDatagram(btData.data(), btDataSize);

		QString strData = btData.data();
		QString strWindowID;	// ���촰��id��Ⱥ������Ⱥ�ţ���������Ա��qq��
		QString strSendEmployeeID, strRecvieEmployeeID;		// ���Ͷ�QQ�źͽ��ն�QQ��
		QString strMsg;			// ����
		
		int msgLen;				// ���ݳ���
		int msgType;			// ��������

		MyLogDEBUG(QString("UDP�յ������ǣ�%1").arg(strData).toUtf8());
		strSendEmployeeID = strData.mid(groupFlagWidth, employeeWidth);	// ��ȡ���Ͷ�QQ��

		// �Լ�������Ϣ��������
		if (strSendEmployeeID == gLoginEmployeeID) {
			return;
		}

		
		if (btData[0] == '1') {		// Ⱥ��
			strWindowID = strData.mid(groupFlagWidth + employeeWidth, groupWidth);		// ��ȡ���ն�Ⱥ��

			// ��ȡ��ǰ��ô���
			//TalkWindow *curTalkWindow = dynamic_cast<TalkWindow *>(ui.rightStackedWidget->currentWidget());
			//QString talkId = curTalkWindow->GetTalkId();
			
			// ���Ƿ����Լ��Ĳ����ܴ���
			bool flag = false;
			for (int i = 0; i < ui.rightStackedWidget->count(); i++) {
				// ��ȡ��ǰ��ô���
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
			if (cMsgType == '1') {			// �ı���Ϣ
				msgType = 1;
				msgLen = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth, msgLengthWidth).toInt();	// ��ȡ��Ϣ����
				strMsg = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth + msgLengthWidth, msgLen);	// ��ȡ�ı���Ϣ

			} else if (cMsgType == '0') {	// ������Ϣ
				msgType = 0;
				int posImages = strData.indexOf("images");
				strMsg = strData.right(strData.length() - posImages - QString("images").length());	// ��ȡ���б������ƣ���Ϣ����


			} else if (cMsgType == '2') {	// �ļ���Ϣ
				msgType = 2;
				int bytesWidth = QString("bytes").length();
				int posBytes = strData.indexOf("bytes");
				int data_begin_width = QString("data_begin").length();
				int pos_data_begin = strData.indexOf("data_begin");

				// ��ȡ�ļ�����
				QString fileName = strData.mid(posBytes + bytesWidth, pos_data_begin - bytesWidth - posBytes);
				gfileName = fileName;

				// �ļ�����
				//int dataLenthWidth = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth, posBytes).toInt();
				int posData = pos_data_begin + data_begin_width;
				strMsg = strData.mid(posData);		// ��ȡ�ļ�����
				gfileData = strMsg;

				// ����employeeID��ȡ����������				
				int employeeID = strSendEmployeeID.toInt();
				QString sender = getEmployeeName(employeeID);

				RecviveFile *recvFile = new RecviveFile(this);
				connect(recvFile, &RecviveFile::refuseFile, [this]() { return; });
				QString msgLabel = "�յ�����" + sender + "�������ļ����Ƿ���գ�";
				recvFile->setMsg(msgLabel);
				recvFile->show();
			}

		} else {	// ����
			strRecvieEmployeeID = strData.mid(groupFlagWidth + employeeWidth, employeeWidth);	// ������QQ��
			strWindowID = strSendEmployeeID;													// ������QQ��

			// ���Ƿ����ҵ���Ϣ��������
			if (strRecvieEmployeeID != gLoginEmployeeID) {
				return;
			}

			// ��ȡ��Ϣ������
			QChar cMsgType = btData[groupFlagWidth + employeeWidth + employeeWidth];
			if (cMsgType == '1') {			// �ı���Ϣ
				msgType = 1;

				// �ı���Ϣ����
				msgLen = strData.mid(groupFlagWidth + employeeWidth + employeeWidth + msgTypeWidth, msgLengthWidth).toInt();
				// �ı���Ϣ
				strMsg = strData.mid(groupFlagWidth + employeeWidth + employeeWidth + msgTypeWidth + msgLengthWidth, msgLen);
			
			} else if (cMsgType == '0') {	// ������Ϣ
				msgType = 0;
				int posImages = strData.indexOf("images");
				int imagesWidth = QString("images").length();
				//strMsg = strData.right(strData.length() - posImages - imagesWidth);	// ��ȡ���б������ƣ���Ϣ����
				strMsg = strData.mid(posImages + imagesWidth);
			
			} else if (cMsgType == '2') {	// �ļ���Ϣ
				msgType = 2;

				int bytesWidth = QString("bytes").length();
				int posBytes = strData.indexOf("bytes");
				int data_begin_width = QString("data_begin").length();
				int pos_data_begin = strData.indexOf("data_begin");

				// ����employeeID��ȡ����������				
				int employeeID = strSendEmployeeID.toInt();
				QString sender = getEmployeeName(employeeID);

				// �ļ�����
				QString fileName = strData.mid(posBytes + bytesWidth, pos_data_begin - posBytes - bytesWidth);
				gfileName = fileName;

				// �ļ����ݳ���
				//int dataLenthWidth = strData.mid(groupFlagWidth + employeeWidth + employeeWidth + msgTypeWidth, posBytes).toInt();

				// �ļ�����
				strMsg = strData.mid(pos_data_begin + data_begin_width);
				gfileData = strMsg;

				RecviveFile *recvFile = new RecviveFile(this);
				connect(recvFile, &RecviveFile::refuseFile, [this]() { return; });
				QString msgLabel = "�յ�����  " + sender + "  �������ļ����Ƿ���գ�";
				recvFile->setMsg(msgLabel);
				recvFile->show();
			}
		}

		// �����촰����Ϊ��Ĵ���
		QWidget *widget = WindowManager::getInstance()->findWindowName(strWindowID);
		if (widget) {	// ���촰�ڴ���
			this->setCurrentWidget(widget);

			// ͬ������������촰��
			QListWidgetItem *item = m_talkwindowItemMap.key(widget);
			item->setSelected(true);

		} else {		// ���촰��δ��
			return;
		}


		if (msgType != 2) {
			handleReceivedMsg(strSendEmployeeID, msgType, strMsg);
		
		} else if (msgType == 2) {	// �ļ���Ϣ��������

		}

		
	}
}

void TalkWindowSheel::onLoadNewMessage() {


}



// ��������Ǽ�¼
void TalkWindowSheel::onLoadMessage() { 
	messTimer->stop();
	QString curAccount;
	if (4 == currentAccount.length()) {
		curAccount = currentAccount;
	} else {
		curAccount = gLoginEmployeeID;
	}


	QJsonArray messageArr;
	// �ҵ�ƥ��������ߵ�qq��Ӧ�������¼
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
		int msgType = message.mid(0, 1).toInt();	// �����Ϣ����

		message = message.mid(1);		// �Ƴ���һλ��������

		// �����¼����
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
		message = message.simplified();		// ȥ����ͷ��β�м�������ַ���\r\n\t
		QJsonArray messageArr = QJsonDocument::fromJson(message.toLocal8Bit()).array();

		// ����
		QMap<int, QJsonArray> messageMap;
		messageMap.insert(receiver, messageArr);
		g_message_info.insert(sender, messageMap);
	}
}
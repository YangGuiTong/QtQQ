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
			MyLogDEBUG(QString("����js�ļ�����ʧ��").toUtf8());
			QMessageBox::information(this, "��ʾ", "����js�ļ�����ʧ��");
		}
	}
	
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
	m_tcpClientSocket->connectToHost("127.0.0.1", gtcpProt);

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

// �ı����ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�ȺQQ�ţ� + ��Ϣ����(1) + ���ݳ��� + ����
// �������ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�ȺQQ�ţ� + ��Ϣ����(0) + ������� + images + ����
// �ļ����ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�ȺQQ�ţ� + ��Ϣ����(2) + �ļ����� + "bytes" + �ļ����� + "data_begin" + �ļ�����
// msgType 0������Ϣ	1�ı���Ϣ	2�ļ���Ϣ
void TalkWindowSheel::updateSendTcpMsg(QString & strData, int & msgType, QString fileName) {
	MyLogDEBUG(QString("����ͨ�����緢�ͣ�strData = %1   msgType = %2   sFile = %3").toUtf8());

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
}

void TalkWindowSheel::onEmotionItemClicked(int emotionNum) {
	MyLogDEBUG(QString("%1 ���鱻���").arg(emotionNum).toUtf8());

	TalkWindow *curTalkWindow = dynamic_cast<TalkWindow *>(ui.rightStackedWidget->currentWidget());
	if (curTalkWindow) {
		curTalkWindow->addEmotionImage(emotionNum);
	}
}

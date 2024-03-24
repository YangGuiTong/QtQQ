#include "MsgWebView.h"
#include "TalkWindowSheel.h"
#include "WindowManager.h"

#include <QFile>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QWebChannel>
#include <QSqlQueryModel>

#include <public_type.h>

extern QString gstrLoginHeadPath;	// ��¼�ߵ�ͷ��·��
extern QMultiMap<int, QMap<int, QJsonArray>> g_message_info;	// �����¼

MsgHtmlObj::MsgHtmlObj(QObject *parent, QString msgLPicPath) : QObject(parent), m_msgLPicPath(msgLPicPath){
	initHtmlTmpl();
}

void MsgHtmlObj::initHtmlTmpl() {
	m_msgLHtmlTmpl = getMsgTmplHtml("msgleftTmpl");
	m_msgLHtmlTmpl.replace("%1", m_msgLPicPath);

	m_msgRHtmlTmpl = getMsgTmplHtml("msgrightTmpl");
	m_msgRHtmlTmpl.replace("%1", gstrLoginHeadPath);
}

QString MsgHtmlObj::getMsgTmplHtml(const QString & code) {
	//MyLogDEBUG(QString("׼�����ļ���%1.html").arg(code).toUtf8());
	QFile file(":Resources/MainWindow/MsgHtml/" + code + ".html");
	file.open(QFile::ReadOnly);
	QString strData;

	if (file.isOpen()) {
		strData = QLatin1String(file.readAll());
		//MyLogDEBUG(QString("�ļ��򿪳ɹ�����ȡ�����ļ������ǣ�%1").arg(strData).toUtf8());
	} else {
		MyLogDEBUG(QString("�ļ���ʧ��").toUtf8());
		QMessageBox::information(nullptr, "tips", "Failed to init html!");
		return "";
	}

	file.close();
	return strData;
}


bool MsgWebPage::acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) {
	// ������qrc:/*.html
	if (url.scheme() == QString("qrc")) {		// �ж�url����
		MyLogDEBUG(QString("%1  ����qrc���ͣ�����true").toUtf8());
		return true;
	}
	
	MyLogDEBUG(QString("%1  ������qrc���ͣ�����false").toUtf8());
	return false;
}




MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent), m_channel(new QWebChannel(this)) {

	MsgWebPage *page = new MsgWebPage(this);
	setPage(page);

	m_msgHtmlObj = new MsgHtmlObj(this);
	m_channel->registerObject("external0", m_msgHtmlObj);

	TalkWindowSheel *talkWindowShell = WindowManager::getInstance()->getTalkWindowSheel();
	connect(this, &MsgWebView::signalSendMsg, talkWindowShell, &TalkWindowSheel::updateSendTcpMsg);
	connect(this, &MsgWebView::signalLoadMsg, talkWindowShell, &TalkWindowSheel::onLoadMessage);
	connect(talkWindowShell, &TalkWindowSheel::signalReload, this, &MsgWebView::onReloadMsgTmpl);


	// ��ǰ�����������촰�ڵ�ID��QQ�ţ�
	QString strTalkId = WindowManager::getInstance()->getCreatingTalkId();

	QSqlQueryModel queryEmployeeModel;
	QString strEmployeeID, strPicturePath;
	QString strExternal;
	bool isGroupTalk = false;	// �Ƿ���Ⱥ��

	// ��ȡ��˾ȺID
	queryEmployeeModel.setQuery(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'").arg("��˾Ⱥ"));
	QModelIndex companyIndex = queryEmployeeModel.index(0, 0);
	QString strCompanyId = queryEmployeeModel.data(companyIndex).toString();

	if (strTalkId == strCompanyId) {	// ��˾Ⱥ��
		isGroupTalk = true;
		queryEmployeeModel.setQuery("SELECT employeeID, picture FROM tab_employees WHERE status = 1;");
	} else {
		if (strTalkId.length() == 4) {	// ����Ⱥ��
			isGroupTalk = true;
			queryEmployeeModel.setQuery(QString("SELECT employeeID, picture FROM tab_employees WHERE status = 1 AND departmentID = %1;").arg(strTalkId));
		} else {	// ��������
			queryEmployeeModel.setQuery(QString("SELECT picture FROM tab_employees WHERE status = 1 AND employeeID = %1;").arg(strTalkId));

			QModelIndex index = queryEmployeeModel.index(0, 0);
			strPicturePath = queryEmployeeModel.data(index).toString();

			strExternal = "external_" + strTalkId;
			MsgHtmlObj *msgHtmlObj = new MsgHtmlObj(this, strPicturePath);
			m_channel->registerObject(strExternal, msgHtmlObj);
		}
	}

	if (isGroupTalk) {
		QModelIndex employeeModelIndex, pictureModelIndex;
		int rows = queryEmployeeModel.rowCount();
		for (int i = 0; i < rows; i++) {
			employeeModelIndex = queryEmployeeModel.index(i, 0);
			pictureModelIndex = queryEmployeeModel.index(i, 1);

			strEmployeeID = queryEmployeeModel.data(employeeModelIndex).toString();
			strPicturePath = queryEmployeeModel.data(pictureModelIndex).toString();

			strExternal = "external_" + strEmployeeID;

			MsgHtmlObj *msgHtmlObj = new MsgHtmlObj(this, strPicturePath);
			m_channel->registerObject(strExternal, msgHtmlObj);
		}
	}


	this->page()->setWebChannel(m_channel);
	// ��ʼ������Ϣҳ��
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));



	//emit signalLoadMsg();
}

MsgWebView::~MsgWebView() { }

void MsgWebView::appendMsg(const QString & html, QString strObj) {
	//MyLogDEBUG(QString("׷����Ϣ��htmlΪ��%1").arg(html).toUtf8());

	QJsonObject msgObj;
	QString qsMsg;
	const QList<QStringList> msgLst = parseHtml(html);		// ����html

	int imageNum = 0;	// ��¼���͵�һ����Ϣ�еı������
	int msgType = 1;	// ��Ϣ���ͣ�0�Ǳ�����Ϣ��1���ı���Ϣ��2���ļ�
	bool isImageMsg = false;
	QString strData;	// ���͵����ݣ�������Ϊ3λ������055��008��155��

	for (int i = 0; i < msgLst.size(); i++) {
		if (msgLst.at(i).at(0) == "img") {
			QString imagePath = msgLst.at(i).at(1);
			QPixmap pixmap;

			// ��ȡ��������
			QString strEmotionPath = "qrc:/Resources/MainWindow/emotion/";
			int pos = strEmotionPath.size();
			QString strEmotionName = imagePath.mid(pos);
			strEmotionName.replace(".png", "");

			isImageMsg = true;

			// ���ݱ������Ƶĳ��Ƚ������ñ������ݣ�����3λ����3λ������23����Ϊ023
			int emotionNameL = strEmotionName.length();
			if (1 == emotionNameL) {
				strData = strData + "00" + strEmotionName;
			} else if (2 == emotionNameL) {
				strData = strData + "0" + strEmotionName;
			} else if (3 == emotionNameL) {
				strData = strData + strEmotionName;
			}

			// ������Ϣ����
			msgType = 0;
			imageNum++;

			if (imagePath.left(3) == "qrc") {
				pixmap.load(imagePath.mid(3));		// ȥ������·���е�qrc
			
			} else {
				pixmap.load(imagePath);
			}

			QString imgPath = QString("<img src=\"%1\" width=\"%2\" height=\"%3\" />")
								.arg(imagePath).arg(pixmap.width()).arg(pixmap.height());
			qsMsg += imgPath;

		} else if (msgLst.at(i).at(0) == "text") {
			qsMsg += msgLst.at(i).at(1);

			msgType = 1;
			strData = qsMsg;
		}
	}

	msgObj.insert("MSG", qsMsg);
	const QString &Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);
	MyLogDEBUG(QString("����ɷ�װ����Ϣ��%1").arg(Msg).toUtf8());

	if (strObj == "0") {	// ����Ϣ
		this->page()->runJavaScript(QString("appendHtml0(%1)").arg(Msg));

		if (isImageMsg) {
			strData = QString::number(imageNum) + "images" + strData;
		}

		// ����Ϣͨ�����緢�ͳ�ȥ
		emit signalSendMsg(strData, msgType);
	
	} else {
		// ����Ϣ
		this->page()->runJavaScript(QString("recvHtml_%1(%2)").arg(strObj).arg(Msg));
	}
}

void MsgWebView::LoadMsg(const QString & html, QString strObj) {

	QJsonObject msgObj;
	QString qsMsg;
	const QList<QStringList> msgLst = parseHtml(html);		// ����html

	int imageNum = 0;	// ��¼���͵�һ����Ϣ�еı������
	int msgType = 1;	// ��Ϣ���ͣ�0�Ǳ�����Ϣ��1���ı���Ϣ��2���ļ�
	bool isImageMsg = false;
	QString strData;	// ���͵����ݣ�������Ϊ3λ������055��008��155��

	for (int i = 0; i < msgLst.size(); i++) {
		if (msgLst.at(i).at(0) == "img") {
			QString imagePath = msgLst.at(i).at(1);
			QPixmap pixmap;

			// ��ȡ��������
			QString strEmotionPath = "qrc:/Resources/MainWindow/emotion/";
			int pos = strEmotionPath.size();
			QString strEmotionName = imagePath.mid(pos);
			strEmotionName.replace(".png", "");

			isImageMsg = true;

			// ���ݱ������Ƶĳ��Ƚ������ñ������ݣ�����3λ����3λ������23����Ϊ023
			int emotionNameL = strEmotionName.length();
			if (1 == emotionNameL) {
				strData = strData + "00" + strEmotionName;
			} else if (2 == emotionNameL) {
				strData = strData + "0" + strEmotionName;
			} else if (3 == emotionNameL) {
				strData = strData + strEmotionName;
			}

			// ������Ϣ����
			msgType = 0;
			imageNum++;

			if (imagePath.left(3) == "qrc") {
				pixmap.load(imagePath.mid(3));		// ȥ������·���е�qrc

			} else {
				pixmap.load(imagePath);
			}

			QString imgPath = QString("<img src=\"%1\" width=\"%2\" height=\"%3\" />")
				.arg(imagePath).arg(pixmap.width()).arg(pixmap.height());
			qsMsg += imgPath;

		} else if (msgLst.at(i).at(0) == "text") {
			qsMsg += msgLst.at(i).at(1);

			msgType = 1;
			strData = qsMsg;
		}
	}

	msgObj.insert("MSG", qsMsg);
	const QString &Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);


	if (strObj == "0") {	
		// ����Ϣ
		this->page()->runJavaScript(QString("appendHtml0(%1)").arg(Msg));

	} else {
		// ����Ϣ
		this->page()->runJavaScript(QString("recvHtml_%1(%2)").arg(strObj).arg(Msg));
	}
}

QList<QStringList> MsgWebView::parseHtml(const QString & html) {
	//MyLogDEBUG(QString("������htmlΪ��%1").arg(html).toUtf8());

	QDomDocument doc;
	doc.setContent(html);

	const QDomElement &root = doc.documentElement();	// �ڵ�Ԫ��
	const QDomNode &node = root.firstChildElement("body");

	return parseDocNode(node);
}

QList<QStringList> MsgWebView::parseDocNode(const QDomNode & node) {
	//MyLogDEBUG(QString("QDomNode��%1").arg(node.toElement().text()).toUtf8());

	QList<QStringList> attribute;
	const QDomNodeList &list = node.childNodes();		// ���������ӽڵ�

	for (int i = 0; i < list.count(); i++) {
		const QDomNode &node = list.at(i);
		if (node.isElement()) {
			// ת��Ԫ��
			const QDomElement &element = node.toElement();
			if (element.tagName() == "img") {
				QStringList attributeList;
				attributeList << "img" << element.attribute("src");
				attribute << attributeList;
			
			} 
			
			if (element.tagName() == "span") {
				QStringList attributeList;
				attributeList << "text" << element.text();
				attribute << attributeList;
			
			}

			// ��������ӽڵ�
			if (node.hasChildNodes()) {
				attribute << parseDocNode(node);
			}
		}
	}

	return attribute;
}


void MsgWebView::onReloadMsgTmpl() {
	// ��ʼ������Ϣҳ��
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));
}
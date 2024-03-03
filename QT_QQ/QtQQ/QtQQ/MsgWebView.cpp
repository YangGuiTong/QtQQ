#include "MsgWebView.h"
#include "TalkWindowSheel.h"
#include "WindowManager.h"

#include <QFile>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QWebChannel>

#include <public_type.h>


MsgHtmlObj::MsgHtmlObj(QObject *parent) : QObject(parent) {
	initHtmlTmpl();
}

void MsgHtmlObj::initHtmlTmpl() {
	m_msgLHtmlTmpl = getMsgTmplHtml("msgleftTmpl");
	m_msgRHtmlTmpl = getMsgTmplHtml("msgrightTmpl");
}

QString MsgHtmlObj::getMsgTmplHtml(const QString & code) {
	MyLogDEBUG(QString("׼�����ļ���%1.html").arg(code).toUtf8());
	QFile file(":Resources/MainWindow/MsgHtml/" + code + ".html");
	file.open(QFile::ReadOnly);
	QString strData;

	if (file.isOpen()) {
		strData = QLatin1String(file.readAll());
		MyLogDEBUG(QString("�ļ��򿪳ɹ�����ȡ�����ļ������ǣ�%1").arg(strData).toUtf8());
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
	: QWebEngineView(parent) {

	MsgWebPage *page = new MsgWebPage(this);
	setPage(page);

	QWebChannel *channel = new QWebChannel(this);
	m_msgHtmlObj = new MsgHtmlObj(this);
	channel->registerObject("external0", m_msgHtmlObj);
	this->page()->setWebChannel(channel);

	TalkWindowSheel *talkWindowShell = WindowManager::getInstance()->getTalkWindowSheel();
	connect(this, &MsgWebView::signalSendMsg, talkWindowShell, &TalkWindowSheel::updateSendTcpMsg);


	// ��ʼ������Ϣҳ��
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));
}

MsgWebView::~MsgWebView() { }

void MsgWebView::appendMsg(const QString & html) {
	MyLogDEBUG(QString("׷����Ϣ��htmlΪ��%1").arg(html).toUtf8());

	QJsonObject msgObj;
	QString qsMsg;
	const QList<QStringList> msgLst = parseHtml(html);		// ����html

	int imageNum = 0;	// ��¼���͵�һ����Ϣ�еı������
	int msgType = 1;	// ��Ϣ���ͣ�0�Ǳ�����Ϣ��1���ı���Ϣ��2���ļ�
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

	this->page()->runJavaScript(QString("appendHtml0(%1)").arg(Msg));

	// ����Ϣͨ�����緢�ͳ�ȥ
	emit signalSendMsg(strData, msgType);
}

QList<QStringList> MsgWebView::parseHtml(const QString & html) {
	MyLogDEBUG(QString("������htmlΪ��%1").arg(html).toUtf8());

	QDomDocument doc;
	doc.setContent(html);

	const QDomElement &root = doc.documentElement();	// �ڵ�Ԫ��
	const QDomNode &node = root.firstChildElement("body");

	return parseDocNode(node);
}

QList<QStringList> MsgWebView::parseDocNode(const QDomNode & node) {
	MyLogDEBUG(QString("QDomNode��%1").arg(node.toElement().text()).toUtf8());

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

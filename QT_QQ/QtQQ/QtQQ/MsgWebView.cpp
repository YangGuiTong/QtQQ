#include "MsgWebView.h"

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
	MyLogDEBUG(QString("准备打开文件：%1.html").arg(code).toUtf8());
	QFile file(":Resources/MainWindow/MsgHtml/" + code + ".html");
	file.open(QFile::ReadOnly);
	QString strData;

	if (file.isOpen()) {
		strData = QLatin1String(file.readAll());
		MyLogDEBUG(QString("文件打开成功，读取到的文件内容是：%1").arg(strData).toUtf8());
	} else {
		MyLogDEBUG(QString("文件打开失败").toUtf8());
		QMessageBox::information(nullptr, "tips", "Failed to init html!");
		return "";
	}

	file.close();
	return strData;
}


bool MsgWebPage::acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) {
	// 仅接受qrc:/*.html
	if (url.scheme() == QString("qrc")) {		// 判断url类型
		MyLogDEBUG(QString("%1  属于qrc类型，返回true").toUtf8());
		return true;
	}
	
	MyLogDEBUG(QString("%1  不属于qrc类型，返回false").toUtf8());
	return false;
}




MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent) {

	MsgWebPage *page = new MsgWebPage(this);
	setPage(page);

	QWebChannel *channel = new QWebChannel(this);
	m_msgHtmlObj = new MsgHtmlObj(this);
	channel->registerObject("external", m_msgHtmlObj);
	this->page()->setWebChannel(channel);
}

MsgWebView::~MsgWebView() { }

void MsgWebView::appendMsg(const QString & html) {
	MyLogDEBUG(QString("追加信息的html为：%1").arg(html).toUtf8());

	QJsonObject msgObj;
	QString qsMsg;
	const QList<QStringList> msgLst = parseHtml(html);		// 解析html

	for (int i = 0; i < msgLst.size(); i++) {
		if (msgLst.at(i).at(0) == "img") {
			QString imagePath = msgLst.at(i).at(1);
			QPixmap pixmap;
			if (imagePath.left(3) == "qrc") {
				pixmap.load(imagePath.mid(3));		// 去掉表情路径中的qrc
			
			} else {
				pixmap.load(imagePath);
			}

			QString imgPath = QString("<img src=\"%1\" width=\"%2\" height=\"%3\" />")
								.arg(imagePath).arg(pixmap.width()).arg(pixmap.height());
			qsMsg += imgPath;

		} else if (msgLst.at(i).at(0) == "text") {
			qsMsg += msgLst.at(i).at(1);
		}
	}

	msgObj.insert("MSG", qsMsg);
	const QString &Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);
	MyLogDEBUG(QString("已完成封装的信息：%1").arg(Msg).toUtf8());

	this->page()->runJavaScript(QString("appendHtml(%1)").arg(Msg));
}

QList<QStringList> MsgWebView::parseHtml(const QString & html) {
	MyLogDEBUG(QString("解析的html为：%1").arg(html).toUtf8());

	QDomDocument doc;
	doc.setContent(html);

	const QDomElement &root = doc.documentElement();	// 节点元素
	const QDomNode &node = root.firstChildElement("body");

	return parseDocNode(node);
}

QList<QStringList> MsgWebView::parseDocNode(const QDomNode & node) {
	MyLogDEBUG(QString("QDomNode：%1").arg(node.nodeValue()).toUtf8());

	QList<QStringList> attribute;
	const QDomNodeList &list = node.childNodes();		// 返回左右子节点

	for (int i = 0; i < list.count(); i++) {
		const QDomNode &node = list.at(i);
		if (node.isElement()) {
			// 转换元素
			const QDomElement &element = node.toElement();
			if (element.tagName() == "img") {
				QStringList attributeList;
				attributeList << "img" << element.attribute("src");
				attribute << attributeList;
			
			} else if (element.tagName() == "span") {
				QStringList attributeList;
				attributeList << "text" << element.text();
			
			}

			// 如果还有子节点
			if (node.hasChildNodes()) {
				attribute << parseDocNode(node);
			}
		}
	}

	return attribute;
}

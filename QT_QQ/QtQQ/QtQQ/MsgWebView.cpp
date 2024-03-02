#include "MsgWebView.h"

#include <QFile>
#include <QMessageBox>

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
	if (url.scheme == QString("qrc")) {		// 判断url类型
		MyLogDEBUG(QString("%1  属于qrc类型，返回true").toUtf8());
		return true;
	}

	MyLogDEBUG(QString("%1  不属于qrc类型，返回false").toUtf8());
	return false;
}




MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent) { }

MsgWebView::~MsgWebView() { }
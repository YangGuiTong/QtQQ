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
	if (url.scheme == QString("qrc")) {		// �ж�url����
		MyLogDEBUG(QString("%1  ����qrc���ͣ�����true").toUtf8());
		return true;
	}

	MyLogDEBUG(QString("%1  ������qrc���ͣ�����false").toUtf8());
	return false;
}




MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent) { }

MsgWebView::~MsgWebView() { }
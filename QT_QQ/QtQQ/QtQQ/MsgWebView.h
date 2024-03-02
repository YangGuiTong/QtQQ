#pragma once

#include <QWebEngineView>
#include <QDomNode>




class MsgHtmlObj : public QObject {
	Q_OBJECT

	// �յ�����Ϣ
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)
	// ����ȥ����Ϣ
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTITY signalMsgHtml)


public:
	MsgHtmlObj(QObject *parent);

signals:
	void signalMsgHtml(const QString &html);

private:
	void initHtmlTmpl();		// ��ʼ��������ҳ
	QString getMsgTmplHtml(const QString &code);


private:
	QString m_msgLHtmlTmpl;		// �յ�����Ϣ
	QString m_msgRHtmlTmpl;		// ����ȥ����Ϣ
};


class MsgWebPage : public QWebEnginePage {
	Q_OBJECT

public:
	MsgWebPage(QObject *parent = nullptr) : QWebEnginePage(parent) {

	}

protected:
	bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame);

};




// QWebEngineView����ͼ��Ӣ����ʾ��ҳ
class MsgWebView  : public QWebEngineView
{
	Q_OBJECT

public:
	MsgWebView(QWidget *parent);
	~MsgWebView();

	void appendMsg(const QString &html);


private:
	QList<QStringList> parseHtml(const QString &html);			// ����html
	// QT������DOM�ڵ㣨���ԡ�˵�����ı��ȣ�������ʹ��QDomNode��ʾ
	QList<QStringList> parseDocNode(const QDomNode &node);		// �����ڵ�


private:
	MsgHtmlObj *m_msgHtmlObj;
};

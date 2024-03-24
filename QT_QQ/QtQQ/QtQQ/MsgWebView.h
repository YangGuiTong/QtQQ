#pragma once

#include <QWebEngineView>
#include <QDomNode>




class MsgHtmlObj : public QObject {
	Q_OBJECT

	// 收到的信息
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)
	// 发出去的信息
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)


public:
	MsgHtmlObj(QObject *parent, QString msgLPicPath = "");	// msgLPicPath: 发信息人来的头像路径

signals:
	void signalMsgHtml(const QString &html);

private:
	void initHtmlTmpl();		// 初始化聊天网页
	QString getMsgTmplHtml(const QString &code);


private:
	QString m_msgLPicPath;		// 发信息人的头像路径
	QString m_msgLHtmlTmpl;		// 收到的信息
	QString m_msgRHtmlTmpl;		// 发出去的信息
};


class MsgWebPage : public QWebEnginePage {
	Q_OBJECT

public:
	MsgWebPage(QObject *parent = nullptr) : QWebEnginePage(parent) {

	}

protected:
	bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame);

};




// QWebEngineView：视图显示网页
class MsgWebView  : public QWebEngineView
{
	Q_OBJECT

public:
	MsgWebView(QWidget *parent);
	~MsgWebView();

	void appendMsg(const QString &html, QString strObj = "0");

	void LoadMsg(const QString &html, QString strObj = "0");


private:
	QList<QStringList> parseHtml(const QString &html);			// 解析html
	// QT中所有DOM节点（属性、说明、文本等）都可以使用QDomNode表示
	QList<QStringList> parseDocNode(const QDomNode &node);		// 解析节点


signals:
	void signalSendMsg(QString &strData, int &msgType, QString sFile = "");
	void signalLoadMsg();

public slots:
	void onReloadMsgTmpl();


private:
	MsgHtmlObj *m_msgHtmlObj;
	QWebChannel *m_channel;
};

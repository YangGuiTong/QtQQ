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

extern QString gstrLoginHeadPath;	// 登录者的头像路径
extern QMultiMap<int, QMap<int, QJsonArray>> g_message_info;	// 聊天记录

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
	//MyLogDEBUG(QString("准备打开文件：%1.html").arg(code).toUtf8());
	QFile file(":Resources/MainWindow/MsgHtml/" + code + ".html");
	file.open(QFile::ReadOnly);
	QString strData;

	if (file.isOpen()) {
		strData = QLatin1String(file.readAll());
		//MyLogDEBUG(QString("文件打开成功，读取到的文件内容是：%1").arg(strData).toUtf8());
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
	: QWebEngineView(parent), m_channel(new QWebChannel(this)) {

	MsgWebPage *page = new MsgWebPage(this);
	setPage(page);

	m_msgHtmlObj = new MsgHtmlObj(this);
	m_channel->registerObject("external0", m_msgHtmlObj);

	TalkWindowSheel *talkWindowShell = WindowManager::getInstance()->getTalkWindowSheel();
	connect(this, &MsgWebView::signalSendMsg, talkWindowShell, &TalkWindowSheel::updateSendTcpMsg);
	connect(this, &MsgWebView::signalLoadMsg, talkWindowShell, &TalkWindowSheel::onLoadMessage);
	connect(talkWindowShell, &TalkWindowSheel::signalReload, this, &MsgWebView::onReloadMsgTmpl);


	// 当前正构建的聊天窗口的ID（QQ号）
	QString strTalkId = WindowManager::getInstance()->getCreatingTalkId();

	QSqlQueryModel queryEmployeeModel;
	QString strEmployeeID, strPicturePath;
	QString strExternal;
	bool isGroupTalk = false;	// 是否是群聊

	// 获取公司群ID
	queryEmployeeModel.setQuery(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'").arg("公司群"));
	QModelIndex companyIndex = queryEmployeeModel.index(0, 0);
	QString strCompanyId = queryEmployeeModel.data(companyIndex).toString();

	if (strTalkId == strCompanyId) {	// 公司群聊
		isGroupTalk = true;
		queryEmployeeModel.setQuery("SELECT employeeID, picture FROM tab_employees WHERE status = 1;");
	} else {
		if (strTalkId.length() == 4) {	// 其他群聊
			isGroupTalk = true;
			queryEmployeeModel.setQuery(QString("SELECT employeeID, picture FROM tab_employees WHERE status = 1 AND departmentID = %1;").arg(strTalkId));
		} else {	// 单独聊天
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
	// 初始化收信息页面
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));



	//emit signalLoadMsg();
}

MsgWebView::~MsgWebView() { }

void MsgWebView::appendMsg(const QString & html, QString strObj) {
	//MyLogDEBUG(QString("追加信息的html为：%1").arg(html).toUtf8());

	QJsonObject msgObj;
	QString qsMsg;
	const QList<QStringList> msgLst = parseHtml(html);		// 解析html

	int imageNum = 0;	// 记录发送的一段信息中的表情个数
	int msgType = 1;	// 信息类型：0是表情信息，1是文本信息，2是文件
	bool isImageMsg = false;
	QString strData;	// 发送的数据（表情宽度为3位，例：055，008，155）

	for (int i = 0; i < msgLst.size(); i++) {
		if (msgLst.at(i).at(0) == "img") {
			QString imagePath = msgLst.at(i).at(1);
			QPixmap pixmap;

			// 获取表情名称
			QString strEmotionPath = "qrc:/Resources/MainWindow/emotion/";
			int pos = strEmotionPath.size();
			QString strEmotionName = imagePath.mid(pos);
			strEmotionName.replace(".png", "");

			isImageMsg = true;

			// 根据表情名称的长度进行设置表情数据，不足3位则补足3位，例如23，则补为023
			int emotionNameL = strEmotionName.length();
			if (1 == emotionNameL) {
				strData = strData + "00" + strEmotionName;
			} else if (2 == emotionNameL) {
				strData = strData + "0" + strEmotionName;
			} else if (3 == emotionNameL) {
				strData = strData + strEmotionName;
			}

			// 设置信息类型
			msgType = 0;
			imageNum++;

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

			msgType = 1;
			strData = qsMsg;
		}
	}

	msgObj.insert("MSG", qsMsg);
	const QString &Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);
	MyLogDEBUG(QString("已完成封装的信息：%1").arg(Msg).toUtf8());

	if (strObj == "0") {	// 发信息
		this->page()->runJavaScript(QString("appendHtml0(%1)").arg(Msg));

		if (isImageMsg) {
			strData = QString::number(imageNum) + "images" + strData;
		}

		// 将信息通过网络发送出去
		emit signalSendMsg(strData, msgType);
	
	} else {
		// 收信息
		this->page()->runJavaScript(QString("recvHtml_%1(%2)").arg(strObj).arg(Msg));
	}
}

void MsgWebView::LoadMsg(const QString & html, QString strObj) {

	QJsonObject msgObj;
	QString qsMsg;
	const QList<QStringList> msgLst = parseHtml(html);		// 解析html

	int imageNum = 0;	// 记录发送的一段信息中的表情个数
	int msgType = 1;	// 信息类型：0是表情信息，1是文本信息，2是文件
	bool isImageMsg = false;
	QString strData;	// 发送的数据（表情宽度为3位，例：055，008，155）

	for (int i = 0; i < msgLst.size(); i++) {
		if (msgLst.at(i).at(0) == "img") {
			QString imagePath = msgLst.at(i).at(1);
			QPixmap pixmap;

			// 获取表情名称
			QString strEmotionPath = "qrc:/Resources/MainWindow/emotion/";
			int pos = strEmotionPath.size();
			QString strEmotionName = imagePath.mid(pos);
			strEmotionName.replace(".png", "");

			isImageMsg = true;

			// 根据表情名称的长度进行设置表情数据，不足3位则补足3位，例如23，则补为023
			int emotionNameL = strEmotionName.length();
			if (1 == emotionNameL) {
				strData = strData + "00" + strEmotionName;
			} else if (2 == emotionNameL) {
				strData = strData + "0" + strEmotionName;
			} else if (3 == emotionNameL) {
				strData = strData + strEmotionName;
			}

			// 设置信息类型
			msgType = 0;
			imageNum++;

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

			msgType = 1;
			strData = qsMsg;
		}
	}

	msgObj.insert("MSG", qsMsg);
	const QString &Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);


	if (strObj == "0") {	
		// 发信息
		this->page()->runJavaScript(QString("appendHtml0(%1)").arg(Msg));

	} else {
		// 收信息
		this->page()->runJavaScript(QString("recvHtml_%1(%2)").arg(strObj).arg(Msg));
	}
}

QList<QStringList> MsgWebView::parseHtml(const QString & html) {
	//MyLogDEBUG(QString("解析的html为：%1").arg(html).toUtf8());

	QDomDocument doc;
	doc.setContent(html);

	const QDomElement &root = doc.documentElement();	// 节点元素
	const QDomNode &node = root.firstChildElement("body");

	return parseDocNode(node);
}

QList<QStringList> MsgWebView::parseDocNode(const QDomNode & node) {
	//MyLogDEBUG(QString("QDomNode：%1").arg(node.toElement().text()).toUtf8());

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
			
			} 
			
			if (element.tagName() == "span") {
				QStringList attributeList;
				attributeList << "text" << element.text();
				attribute << attributeList;
			
			}

			// 如果还有子节点
			if (node.hasChildNodes()) {
				attribute << parseDocNode(node);
			}
		}
	}

	return attribute;
}


void MsgWebView::onReloadMsgTmpl() {
	// 初始化收信息页面
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));
}
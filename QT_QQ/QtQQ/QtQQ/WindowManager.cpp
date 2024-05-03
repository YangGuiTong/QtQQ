#include "WindowManager.h"
#include "public_type.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"

#include <QSqlQueryModel>
#include <QThread>

// 单例模式，创建全局静态对象
Q_GLOBAL_STATIC(WindowManager, theInstalce)

WindowManager::WindowManager() : QObject(nullptr), m_talkwindowshell(nullptr) {
	messageTimer = new QTimer(this);
	connect(messageTimer, &QTimer::timeout, this, &WindowManager::onMessageTimer);
}

WindowManager::~WindowManager() {

}

QString WindowManager::findWindowName(QWidget * qsWindow) {
	
	return m_windowMap.key(qsWindow);
}

QWidget * WindowManager::findWindowName(const QString & qsWindowName) {
	MyLogDEBUG(QString("根据名字：%1 寻找窗口").arg(qsWindowName).toUtf8());

	if (m_windowMap.contains(qsWindowName)) {
		return m_windowMap.value(qsWindowName);
	}

	return nullptr;
}

void WindowManager::deleteWindowName(const QString & qsWindowName) {
	MyLogDEBUG(QString("根据名字：%1 移除窗口").arg(qsWindowName).toUtf8());
	m_windowMap.remove(qsWindowName);
}

void WindowManager::addWindowName(const QString & qsWindowName, QWidget * qWidget) {
	MyLogDEBUG(QString("根据名字：%1 添加窗口").arg(qsWindowName).toUtf8());
	if (!m_windowMap.contains(qsWindowName)) {
		m_windowMap.insert(qsWindowName, qWidget);
	}
}

WindowManager * WindowManager::getInstance() {
	return theInstalce();
}

void WindowManager::addNewTalkWindow(const QString & uid) {
	MyLogDEBUG(QString("添加新的窗口，uid = %1").arg(uid).toUtf8());
	if (m_talkwindowshell == nullptr) {
		m_talkwindowshell = new TalkWindowSheel;
		connect(m_talkwindowshell, &TalkWindowSheel::destroyed, [this](QObject *obj) {
			m_talkwindowshell = nullptr;
		});
	}

	QWidget *widget = findWindowName(uid);
	if (!widget) {
		MyLogDEBUG(QString("根据 %1 没有找到窗口，new一个新的窗口").arg(uid).toUtf8());
		
		m_strCreatingTalkId = uid;

		TalkWindow *talkwindow = new TalkWindow(m_talkwindowshell, uid);
		TalkWindowItem *talkwindowItem = new TalkWindowItem(talkwindow);

		m_strCreatingTalkId = "";

		// 判断是群聊还是单聊
		QSqlQueryModel sqlDepModel;
		QString sql = QString("SELECT department_name, sign FROM tab_department WHERE departmentID = %1").arg(uid);
		MyLogDEBUG(QString("sql语句：%1").arg(sql).toUtf8());
		sqlDepModel.setQuery(sql);

		int rows = sqlDepModel.rowCount();
		if (rows == 0) {	// 单聊
			QString sql = QString("SELECT employee_name, employee_sign FROM tab_employees WHERE employeeID = %1").arg(uid);
			MyLogDEBUG(QString("sql语句：%1").arg(sql).toUtf8());
			sqlDepModel.setQuery(sql);

		}

		QModelIndex indexDepIndex, signIndex;
		indexDepIndex = sqlDepModel.index(0, 0);	// 0行0列
		signIndex = sqlDepModel.index(0, 1);		// 0行1列
		QString strWindowName = sqlDepModel.data(signIndex).toString();
		QString strMsgLabel = sqlDepModel.data(indexDepIndex).toString();

		talkwindow->setWindowName(strWindowName);			// 窗口名称
		talkwindowItem->setMsgLabelContent(strMsgLabel);	// 左侧联系人文本显示

		m_talkwindowshell->addTalkWindow(talkwindow, talkwindowItem, uid);

	} else {
		MyLogDEBUG(QString("根据 %1 找到窗口，直接添加").arg(uid).toUtf8());
		
		// 左侧聊天列表设为选中
		QListWidgetItem *item = m_talkwindowshell->getTalkWindowItemMap().key(widget);
		item->setSelected(true);

		// 设置右侧当前聊天窗口
		m_talkwindowshell->setCurrentWidget(widget);
	}

	m_talkwindowshell->show();
	m_talkwindowshell->activateWindow();

	//QThread::sleep(1);

	//m_talkwindowshell->onLoadMessage();

	emit m_talkwindowshell->signalReload();

	messageTimer->start(350);
}

TalkWindowSheel * WindowManager::getTalkWindowSheel() {
	return m_talkwindowshell;
}

QString WindowManager::getCreatingTalkId() {
	return m_strCreatingTalkId;
}

void WindowManager::onMessageTimer() { 
	
	
	m_talkwindowshell->onLoadMessage();
	messageTimer->stop();
}

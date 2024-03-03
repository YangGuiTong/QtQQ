#include "WindowManager.h"
#include "public_type.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"

#include <QSqlQueryModel>

// 单例模式，创建全局静态对象
Q_GLOBAL_STATIC(WindowManager, theInstalce)

WindowManager::WindowManager() : QObject(nullptr), m_talkwindowshell(nullptr) {

}

WindowManager::~WindowManager() {

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
		
		TalkWindow *talkwindow = new TalkWindow(m_talkwindowshell, uid);
		TalkWindowItem *talkwindowItem = new TalkWindowItem(talkwindow);

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

/*
		switch (groupType) {
			case COMPANY:
			{
				talkwindow->setWindowName(QString("这个群一天也不想呆了"));
				talkwindowItem->setMsgLabelContent(QString("Qt公司群"));
				break;
			}
			case PERSONELGROUP:
			{
				talkwindow->setWindowName(QString("今天上班不努力，明天努力找工作"));
				talkwindowItem->setMsgLabelContent(QString("Qt人事群"));
				break;
			}
			case MARKETGROUP:
			{
				talkwindow->setWindowName(QString("到处造谣拐骗"));
				talkwindowItem->setMsgLabelContent(QString("Qt市场群"));
				break;
			}
			case DEVELOPMENTGROUP:
			{
				talkwindow->setWindowName(QString("据说只有两种编程语言，一种是天天挨骂的，另一种是没人用的"));
				talkwindowItem->setMsgLabelContent(QString("Qt研发群"));
				break;
			}
			case PTOP:
			{
				talkwindow->setWindowName(QString("这个家伙"));
				talkwindowItem->setMsgLabelContent(strPeopel);
				break;
			}
			default:
			{
				break;
			}
		}

		m_talkwindowshell->addTalkWindow(talkwindow, talkwindowItem, groupType);
*/
		

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

}

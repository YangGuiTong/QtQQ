#include "WindowManager.h"
#include "public_type.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"

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

void WindowManager::addNewTalkWindow(const QString & uid, GroupType groupType, const QString & strPeopel) {
	MyLogDEBUG(QString("添加新的窗口，uid = %1, groupType = %2, strPeopel = %3")
				.arg(uid).arg(groupType).arg(strPeopel).toUtf8());
	if (m_talkwindowshell == nullptr) {
		m_talkwindowshell = new TalkWindowSheel;
		connect(m_talkwindowshell, &TalkWindowSheel::destroyed, [this](QObject *obj) {
			m_talkwindowshell = nullptr;
		});
	}

	QWidget *widget = findWindowName(uid);
	if (!widget) {
		MyLogDEBUG(QString("根据 %1 没有找到窗口，new一个新的窗口").arg(uid).toUtf8());
		
		TalkWindow *talkwindow = new TalkWindow(m_talkwindowshell, uid, groupType);
		TalkWindowItem *talkwindowItem = new TalkWindowItem(talkwindow);

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

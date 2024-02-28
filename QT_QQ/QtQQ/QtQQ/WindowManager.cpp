#include "WindowManager.h"
#include "public_type.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"

// ����ģʽ������ȫ�־�̬����
Q_GLOBAL_STATIC(WindowManager, theInstalce)

WindowManager::WindowManager() : QObject(nullptr), m_talkwindowshell(nullptr) {

}

WindowManager::~WindowManager() {

}

QWidget * WindowManager::findWindowName(const QString & qsWindowName) {
	MyLogDEBUG(QString("�������֣�%1 Ѱ�Ҵ���").arg(qsWindowName).toUtf8());

	if (m_windowMap.contains(qsWindowName)) {
		return m_windowMap.value(qsWindowName);
	}

	return nullptr;
}

void WindowManager::deleteWindowName(const QString & qsWindowName) {
	MyLogDEBUG(QString("�������֣�%1 �Ƴ�����").arg(qsWindowName).toUtf8());
	m_windowMap.remove(qsWindowName);
}

void WindowManager::addWindowName(const QString & qsWindowName, QWidget * qWidget) {
	MyLogDEBUG(QString("�������֣�%1 ��Ӵ���").arg(qsWindowName).toUtf8());
	if (!m_windowMap.contains(qsWindowName)) {
		m_windowMap.insert(qsWindowName, qWidget);
	}
}

WindowManager * WindowManager::getInstance() {
	return theInstalce();
}

void WindowManager::addNewTalkWindow(const QString & uid, GroupType groupType, const QString & strPeopel) {
	MyLogDEBUG(QString("����µĴ��ڣ�uid = %1, groupType = %2, strPeopel = %3")
				.arg(uid).arg(groupType).arg(strPeopel).toUtf8());
	if (m_talkwindowshell == nullptr) {
		m_talkwindowshell = new TalkWindowSheel;
		connect(m_talkwindowshell, &TalkWindowSheel::destroyed, [this](QObject *obj) {
			m_talkwindowshell = nullptr;
		});
	}

	QWidget *widget = findWindowName(uid);
	if (!widget) {
		MyLogDEBUG(QString("���� %1 û���ҵ����ڣ�newһ���µĴ���").arg(uid).toUtf8());
		
		TalkWindow *talkwindow = new TalkWindow(m_talkwindowshell, uid, groupType);
		TalkWindowItem *talkwindowItem = new TalkWindowItem(talkwindow);

		switch (groupType) {
			case COMPANY:
			{
				talkwindow->setWindowName(QString("���Ⱥһ��Ҳ�������"));
				talkwindowItem->setMsgLabelContent(QString("Qt��˾Ⱥ"));
				break;
			}
			case PERSONELGROUP:
			{
				talkwindow->setWindowName(QString("�����ϰ಻Ŭ��������Ŭ���ҹ���"));
				talkwindowItem->setMsgLabelContent(QString("Qt����Ⱥ"));
				break;
			}
			case MARKETGROUP:
			{
				talkwindow->setWindowName(QString("������ҥ��ƭ"));
				talkwindowItem->setMsgLabelContent(QString("Qt�г�Ⱥ"));
				break;
			}
			case DEVELOPMENTGROUP:
			{
				talkwindow->setWindowName(QString("��˵ֻ�����ֱ�����ԣ�һ�������찤��ģ���һ����û���õ�"));
				talkwindowItem->setMsgLabelContent(QString("Qt�з�Ⱥ"));
				break;
			}
			case PTOP:
			{
				talkwindow->setWindowName(QString("����һ�"));
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
		MyLogDEBUG(QString("���� %1 �ҵ����ڣ�ֱ�����").arg(uid).toUtf8());
		
		// ��������б���Ϊѡ��
		QListWidgetItem *item = m_talkwindowshell->getTalkWindowItemMap().key(widget);
		item->setSelected(true);

		// �����Ҳ൱ǰ���촰��
		m_talkwindowshell->setCurrentWidget(widget);
	}

	m_talkwindowshell->show();
	m_talkwindowshell->activateWindow();

}

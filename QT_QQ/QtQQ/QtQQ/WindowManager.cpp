#include "WindowManager.h"
#include "public_type.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"

#include <QSqlQueryModel>
#include <QThread>

// ����ģʽ������ȫ�־�̬����
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

void WindowManager::addNewTalkWindow(const QString & uid) {
	MyLogDEBUG(QString("����µĴ��ڣ�uid = %1").arg(uid).toUtf8());
	if (m_talkwindowshell == nullptr) {
		m_talkwindowshell = new TalkWindowSheel;
		connect(m_talkwindowshell, &TalkWindowSheel::destroyed, [this](QObject *obj) {
			m_talkwindowshell = nullptr;
		});
	}

	QWidget *widget = findWindowName(uid);
	if (!widget) {
		MyLogDEBUG(QString("���� %1 û���ҵ����ڣ�newһ���µĴ���").arg(uid).toUtf8());
		
		m_strCreatingTalkId = uid;

		TalkWindow *talkwindow = new TalkWindow(m_talkwindowshell, uid);
		TalkWindowItem *talkwindowItem = new TalkWindowItem(talkwindow);

		m_strCreatingTalkId = "";

		// �ж���Ⱥ�Ļ��ǵ���
		QSqlQueryModel sqlDepModel;
		QString sql = QString("SELECT department_name, sign FROM tab_department WHERE departmentID = %1").arg(uid);
		MyLogDEBUG(QString("sql��䣺%1").arg(sql).toUtf8());
		sqlDepModel.setQuery(sql);

		int rows = sqlDepModel.rowCount();
		if (rows == 0) {	// ����
			QString sql = QString("SELECT employee_name, employee_sign FROM tab_employees WHERE employeeID = %1").arg(uid);
			MyLogDEBUG(QString("sql��䣺%1").arg(sql).toUtf8());
			sqlDepModel.setQuery(sql);

		}

		QModelIndex indexDepIndex, signIndex;
		indexDepIndex = sqlDepModel.index(0, 0);	// 0��0��
		signIndex = sqlDepModel.index(0, 1);		// 0��1��
		QString strWindowName = sqlDepModel.data(signIndex).toString();
		QString strMsgLabel = sqlDepModel.data(indexDepIndex).toString();

		talkwindow->setWindowName(strWindowName);			// ��������
		talkwindowItem->setMsgLabelContent(strMsgLabel);	// �����ϵ���ı���ʾ

		m_talkwindowshell->addTalkWindow(talkwindow, talkwindowItem, uid);

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

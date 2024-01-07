#include "SysTray.h"
#include "CustomMenu.h"


SysTray::SysTray(QWidget *parent) : m_parent(parent), QSystemTrayIcon(parent)
{
	initSystemTray();
	show();
}

SysTray::~SysTray()
{}

void SysTray::initSystemTray() { 
	QString icon = tr(":Resources/MainWindow/app/logo.ico");
	setIcon(QIcon(icon));

	setToolTip(tr("Qt_QQ_��ʱͨѶ"));

	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onIconActivated(QSystemTrayIcon::ActivationReason)));


	icon = "��ʼ��ϵͳ����ͼ�꣺" + icon;
	MyLogDEBUG(icon.toUtf8());
}

void SysTray::addSystrayMenu() {

	CustomMenu *customMenu = new CustomMenu(m_parent);
	customMenu->addCustomMenu("onShow", ":/Resources/MainWindow/app/logo.ico", tr("��ʾ"));
	customMenu->addCustomMenu("onQuit", ":/Resources/MainWindow/app/page_close_btn_hover.png", tr("�˳�"));

	connect(customMenu->getAction("onShow"), SIGNAL(triggered(bool)), m_parent, SLOT(onShowNormal(bool)));
	connect(customMenu->getAction("onQuit"), SIGNAL(triggered(bool)), m_parent, SLOT(onShowQuit(bool)));

	customMenu->exec(QCursor::pos());
	delete customMenu;
	customMenu = nullptr;
}


void SysTray::onIconActivated(QSystemTrayIcon::ActivationReason reason) {
	if (reason == QSystemTrayIcon::Trigger) {
		MyLogDEBUG("���ڻָ��������ʾ");

		m_parent->show();
		m_parent->activateWindow();	// �����
	
	} else if (reason == QSystemTrayIcon::Context) {
		MyLogDEBUG("ϵͳ���̲˵���ʾ");
		addSystrayMenu();
	}
}
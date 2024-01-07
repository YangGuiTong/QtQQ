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

	setToolTip(tr("Qt_QQ_即时通讯"));

	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onIconActivated(QSystemTrayIcon::ActivationReason)));


	icon = "初始化系统托盘图标：" + icon;
	MyLogDEBUG(icon.toUtf8());
}

void SysTray::addSystrayMenu() {

	CustomMenu *customMenu = new CustomMenu(m_parent);
	customMenu->addCustomMenu("onShow", ":/Resources/MainWindow/app/logo.ico", tr("显示"));
	customMenu->addCustomMenu("onQuit", ":/Resources/MainWindow/app/page_close_btn_hover.png", tr("退出"));

	connect(customMenu->getAction("onShow"), SIGNAL(triggered(bool)), m_parent, SLOT(onShowNormal(bool)));
	connect(customMenu->getAction("onQuit"), SIGNAL(triggered(bool)), m_parent, SLOT(onShowQuit(bool)));

	customMenu->exec(QCursor::pos());
	delete customMenu;
	customMenu = nullptr;
}


void SysTray::onIconActivated(QSystemTrayIcon::ActivationReason reason) {
	if (reason == QSystemTrayIcon::Trigger) {
		MyLogDEBUG("窗口恢复活动窗口显示");

		m_parent->show();
		m_parent->activateWindow();	// 活动窗口
	
	} else if (reason == QSystemTrayIcon::Context) {
		MyLogDEBUG("系统托盘菜单显示");
		addSystrayMenu();
	}
}
#include "UserLogin.h"
#include "CCMainWindow.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

QString gLoginEmployeeID;	// 登录者的QQ号

UserLogin::UserLogin(QWidget *parent) : BasicWindow(parent) {
	ui.setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	initTitleBar();
	setTitleBarTitle(QStringLiteral(""), QStringLiteral(":/Resources/MainWindow/qqlogoclassic.png"));
	loadStyleSheet(QStringLiteral("UserLogin"));
	initControl();
}

UserLogin::~UserLogin() { }


void UserLogin::initControl() {
	MyLogDEBUG(QString("登录窗口初始化").toUtf8());

	QLabel *headlabel = new QLabel(this);
	headlabel->setFixedSize(68, 68);
	QPixmap pix(QStringLiteral(":/Resources/MainWindow/head_mask.png"));
	headlabel->setPixmap(getRoundImage(QPixmap(QStringLiteral(":/Resources/MainWindow/app/logo.ico")), pix, headlabel->size()));	// 将获取到的圆头像设置Label
	headlabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);		// 将label图像移动到中心点

	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnClicked);
}



void UserLogin::onLoginBtnClicked() {
	MyLogDEBUG(QString("登录按钮被按下！").toUtf8());

	bool isAccountLogin;
	QString strAccount;	// 账号或QQ号

	close();

	CCMainWindow *mainwindow = new CCMainWindow();
	mainwindow->show();
}
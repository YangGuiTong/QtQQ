#include "UserLogin.h"
#include "CCMainWindow.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

QString gLoginEmployeeID;	// ��¼�ߵ�QQ��

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
	MyLogDEBUG(QString("��¼���ڳ�ʼ��").toUtf8());

	QLabel *headlabel = new QLabel(this);
	headlabel->setFixedSize(68, 68);
	QPixmap pix(QStringLiteral(":/Resources/MainWindow/head_mask.png"));
	headlabel->setPixmap(getRoundImage(QPixmap(QStringLiteral(":/Resources/MainWindow/app/logo.ico")), pix, headlabel->size()));	// ����ȡ����Բͷ������Label
	headlabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);		// ��labelͼ���ƶ������ĵ�

	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnClicked);
}



void UserLogin::onLoginBtnClicked() {
	MyLogDEBUG(QString("��¼��ť�����£�").toUtf8());

	bool isAccountLogin;
	QString strAccount;	// �˺Ż�QQ��

	close();

	CCMainWindow *mainwindow = new CCMainWindow();
	mainwindow->show();
}
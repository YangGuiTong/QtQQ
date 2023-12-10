#include "UserLogin.h"
#include "CCMainWindow.h"

UserLogin::UserLogin(BasicWindow *parent)
	: BasicWindow(parent) {
	ui.setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	initTiitleBar();
	setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");
	loadStyleSheet("UserLogin");
	initControl();
}

UserLogin::~UserLogin() { }



void UserLogin::initControl() {
	QLabel *headlabel = new QLabel(this);
	headlabel->setFixedSize(68, 68);
	QPixmap pix(":/Resources/MainWindow/head_mask.png");
	headlabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/girl.png"), pix, headlabel->size()));
	headlabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);
	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnClicked);
}

void UserLogin::onLoginBtnClicked() {
	close();
	CCMainWindow *mainwindow = new CCMainWindow();
	mainwindow->show();
}
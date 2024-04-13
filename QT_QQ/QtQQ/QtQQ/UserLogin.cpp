#include "UserLogin.h"
#include "CCMainWindow.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSettings>

QString gLoginEmployeeID;	// 登录者的QQ号

UserLogin::UserLogin(QWidget *parent) : BasicWindow(parent) {
	ui.setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	initTitleBar();
	setTitleBarTitle(QStringLiteral(""), QStringLiteral(":/Resources/MainWindow/qqlogoclassic.png"));
	loadStyleSheet(QStringLiteral("UserLogin"));
	initControl();

	initTemporaryAP();

	ui.forgetWordbtn->setVisible(false);
	//ui.forgetWordbtn->setEnabled(false);
	ui.registBtn->setVisible(false);
	//ui.registBtn->setEnabled(false);
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


	if (!connectMySql()) {
		QMessageBox::information(nullptr, "提示", "数据库连接失败");
		close();
	}
}

bool UserLogin::connectMySql() {
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setDatabaseName("qtqq");
	db.setHostName("localhost");
	db.setUserName("root");
	db.setPassword("yang");
	db.setPort(3306);

	if (db.open()) {
		MyLogDEBUG(QString("客户端连接数据库成功！").toUtf8());
		return true;
	}


	return false;
}

bool UserLogin::verfyAccountCode(bool &isAccountLogin, QString &strAccount) {
	MyLogDEBUG(QString("账号密码验证").toUtf8());
	QString strAccountInput = ui.editUserAccount->text();
	QString strCodeInput = ui.editPassword->text();

	// 输入员工号（QQ号登录）
	QString strSqlCode = QString("SELECT code FROM tab_accounts WHERE employeeID=%1").arg(strAccountInput);
	MyLogDEBUG(QString("sql语句：%1").arg(strSqlCode).toUtf8());
	QSqlQuery queryEmployeeID(strSqlCode);
	queryEmployeeID.exec();

	if (queryEmployeeID.first()) {		// 指向结构集的第一条
		// 数据库中qq号对应的密码
		QString strCode = queryEmployeeID.value(0).toString();

		if (strCode == strCodeInput) {
			gLoginEmployeeID = strAccountInput;

			isAccountLogin = false;
			strAccount = strAccountInput;
			return true;
		}

		return false;	
	} 

	// 账号登录
	strSqlCode = QString("SELECT code, employeeID FROM tab_accounts WHERE account = '%1'").arg(strAccountInput);
	MyLogDEBUG(QString("sql语句：%1").arg(strSqlCode).toUtf8());
	QSqlQuery queryAccount(strSqlCode);
	queryAccount.exec();

	if (queryAccount.first()) {
		QString strCode = queryAccount.value(0).toString();

		if (strCode == strCodeInput) {
			gLoginEmployeeID = queryAccount.value(1).toString();

			isAccountLogin = true;
			strAccount = strAccountInput;
			return true;
		
		} 

		return false;
	}

	return false;
}

bool UserLogin::verfyAccountLogout() {
	MyLogDEBUG(QString("账号验证是否已被注销").toUtf8());
	QString strAccountInput = ui.editUserAccount->text();

	// 输入员工号（QQ号登录）
	QString strSqlCode = QString("SELECT status FROM tab_employees WHERE employeeID=%1").arg(strAccountInput);
	MyLogDEBUG(QString("sql语句：%1").arg(strSqlCode).toUtf8());
	QSqlQuery queryEmployeeID(strSqlCode);
	queryEmployeeID.exec();

	if (queryEmployeeID.first()) {		// 指向结构集的第一条
		// 数据库中qq号对应的状态
		int strStatus = queryEmployeeID.value(0).toInt();

		if (1 == strStatus) {
			return true;
		}

		return false;
	}

	// 账号登录
	strSqlCode = QString("SELECT employeeID FROM tab_accounts WHERE account = '%1'").arg(strAccountInput);
	MyLogDEBUG(QString("sql语句：%1").arg(strSqlCode).toUtf8());
	QSqlQuery queryAccount(strSqlCode);
	queryAccount.exec();

	if (queryAccount.first()) {
		QString strEmployeeID = queryAccount.value(0).toString();

		strSqlCode = QString("SELECT status FROM tab_employees WHERE employeeID=%1").arg(strEmployeeID);
		MyLogDEBUG(QString("sql语句：%1").arg(strSqlCode).toUtf8());
		QSqlQuery queryStatus(strSqlCode);
		queryStatus.exec();

		if (queryStatus.first()) {		// 指向结构集的第一条
			// 数据库中qq号对应的状态
			int strStatus = queryStatus.value(0).toInt();

			if (1 == strStatus) {
				return true;
			}

			return false;
		}
	}

	return false;
}

void UserLogin::initTemporaryAP() {

	const QString &&path = QApplication::applicationDirPath() + "/" + QString("tradeprintinfo.ini");
	QSettings settings(path, QSettings::IniFormat);

	// 读取ini文件中保存的账号和密码
	auto account = settings.value(QStringLiteral("TemporaryAP/account"));
	auto pwd = settings.value(QStringLiteral("TemporaryAP/pwd"));
	if (account.isNull() || pwd.isNull()) {
		return;
	}
	
	QString account_ = account.toString();
	QString pwd_ = pwd.toString();
	if (account_.isEmpty() || 0 == account_.length() || pwd_.isEmpty() || 0 == pwd_.length()) {
		return;
	}

	ui.editUserAccount->setText(account_);
	ui.editPassword->setText(pwd_);

	ui.checkBox->setChecked(true);
}



void UserLogin::onLoginBtnClicked() {
	MyLogDEBUG(QString("登录按钮被按下！").toUtf8());

	bool isAccountLogin = false;;
	QString strAccount = "";		// 账号或者QQ号

	if (!verfyAccountLogout()) {
		QMessageBox::information(NULL, "提示", "您输入的账号已被注销，请输入有效账号！");
		ui.editPassword->setText("");
		ui.editUserAccount->setText("");
		ui.checkBox->setChecked(false);
		return;
	}

	if (!verfyAccountCode(isAccountLogin, strAccount)) {
		QMessageBox::information(NULL, "提示", "您输入的账号或密码有误，请重新输入！");
		ui.editPassword->setText("");
		ui.editUserAccount->setText("");
		ui.checkBox->setChecked(false);
		return;
	}

	// 更新登录状态为登录
	QString strSqlStatus = QString("UPDATE tab_employees SET online = 2 WHERE employeeID = %1").arg(gLoginEmployeeID);
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();

	// 处理密码保存
	const QString &&path = QApplication::applicationDirPath() + "/" + QString("tradeprintinfo.ini");
	QSettings settings(path, QSettings::IniFormat);
	if (!ui.checkBox->isChecked()) {
		settings.setValue(QStringLiteral("TemporaryAP/account"), "");
		settings.setValue(QStringLiteral("TemporaryAP/pwd"), "");
	} else {
		QString strAccountInput = ui.editUserAccount->text();
		QString strCodeInput = ui.editPassword->text();
		settings.setValue(QStringLiteral("TemporaryAP/account"), strAccountInput);
		settings.setValue(QStringLiteral("TemporaryAP/pwd"), strCodeInput);
	}

	close();
	CCMainWindow *mainwindow = new CCMainWindow(strAccount, isAccountLogin);
	mainwindow->show();
}
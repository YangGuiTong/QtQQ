#include "CCMainWindow.h"

#include <QProxyStyle>
#include <QPainter>
#include <QTimer>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QSqlQuery>

#include "SkinWindow.h"
#include "SysTray.h"
#include "NotifyManager.h"
#include "WindowManager.h"
#include "TalkWindowSheel.h"

// 第一个int代表发送方的QQ号，第二个int代表接收方的QQ号，QJsonArray代表聊天记录
QMultiMap<int, QMap<int, QJsonArray>> g_message_info;	// 聊天记录

QString gstrLoginHeadPath;	// 登录者的头像路径
extern QString gLoginEmployeeID;	// 登录者的QQ号
extern QString currentAccount;		// 当前窗口账号

class CustomProxyStayle : public QProxyStyle {
public:
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
							   const QWidget *widget = nullptr) const { 
		if (element == PE_FrameFocusRect) {
			return;
		
		} else {
			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}
	}
};

CCMainWindow::CCMainWindow(QString account, bool isAccountLogin, QWidget *parent)
	: BasicWindow(parent), m_account(account), m_isAccountLogin(isAccountLogin) {
	ui.setupUi(this);

	MyLogDEBUG("主窗体初始化");

	Qt::WindowFlags flag = windowFlags();
	setWindowFlag(Qt::Tool);
	loadStyleSheet(QStringLiteral("CCMainWindow"));

	setHeadPixmap(getHeadPixturePath());
	initControl();
	initTimer();
}

CCMainWindow::~CCMainWindow() {


}

void CCMainWindow::initTimer() {

	QTimer *timer = new QTimer(this);
	timer->setInterval(1 * 60 * 1000);

	connect(timer, &QTimer::timeout, [this]() {
		static int level = 1;
		if (99 == level) {
			level = 0;
		}
		level++;

		setLevePixmap(level);
	});

	timer->start();
}

void CCMainWindow::initControl() {
	// 树获取焦点时不绘制边框
	ui.treeWidget->setStyle(new CustomProxyStayle());

	setLevePixmap(1);
	//setHeadPixmap(":/Resources/MainWindow/girl.png");
	setStatusMenuIcon(":/Resources/MainWindow/StatusSucceeded.png");

	QHBoxLayout *appUpLayout = new QHBoxLayout;
	appUpLayout->setContentsMargins(0, 0, 0, 0);
	appUpLayout->addWidget(addOtherAppExtension(":Resources/MainWindow/app/app_7.png", "app_7"));
	appUpLayout->addWidget(addOtherAppExtension(":Resources/MainWindow/app/app_2.png", "app_2"));
	appUpLayout->addWidget(addOtherAppExtension(":Resources/MainWindow/app/app_3.png", "app_3"));
	appUpLayout->addWidget(addOtherAppExtension(":Resources/MainWindow/app/app_4.png", "app_4"));
	appUpLayout->addWidget(addOtherAppExtension(":Resources/MainWindow/app/app_5.png", "app_5"));
	appUpLayout->addWidget(addOtherAppExtension(":Resources/MainWindow/app/app_6.png", "app_6"));
	appUpLayout->addWidget(addOtherAppExtension(":Resources/MainWindow/app/skin.png", "app_skin"));	
	appUpLayout->addStretch();
	//appUpLayout->setSpacing(2);
	ui.appWidget->setLayout(appUpLayout);

	ui.bottomLayout_up->addWidget(addOtherAppExtension(":Resources/MainWindow/app/app_10.png", "app_10"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":Resources/MainWindow/app/app_8.png", "app_8"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":Resources/MainWindow/app/app_11.png", "app_11"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":Resources/MainWindow/app/app_9.png", "app_9"));
	ui.bottomLayout_up->addStretch();

	initContactTree();

	// 个性签名
	ui.lineEdit->installEventFilter(this);
	// 好友搜索
	ui.searchLineEdit->installEventFilter(this);


	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowHide(bool)));
	//connect(ui.sysclose, SIGNAL(clicked(bool)), this, SLOT(onShowClose(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), this, SLOT(onShowQuit(bool)));
	connect(NotifyManager::getInstance(), &NotifyManager::signalSkinChanged, [this]() {
		updateSeachStyle();
	});

	// 系统托盘
	SysTray *systray = new SysTray(this);

	// 读取聊天记录
	ReadDatabaseMessage();

	InitSign();
}

void CCMainWindow::updateSeachStyle() { 
	ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{background-color:rgba(%1,%2,%3,50);border-bottom:1px solid rgba(%1,%2,%3, 30)}\
											QPushButton#searchBtn{border-image:url(:Resources/MainWindow/search/search_icon.png)}")
									.arg(m_colorBackGround.red()).arg(m_colorBackGround.green()).arg(m_colorBackGround.blue()));
}

void CCMainWindow::addCompanyDeps(QTreeWidgetItem * pRootGroupItem, int DepID) {
	MyLogDEBUG(QString("群id：%1").arg(DepID).toUtf8());

	QTreeWidgetItem *pChild = new QTreeWidgetItem;
	QPixmap pix;
	pix.load(":Resources/MainWindow/head_mask.png");

	// 添加子节点
	pChild->setData(0, Qt::UserRole, 1);	// 子项设置为1
	pChild->setData(0, Qt::UserRole + 1, DepID);


	// 获取公司、部门头像
	QPixmap groupPix;
	QString sql = QString("SELECT picture FROM tab_department WHERE departmentID = %1").arg(DepID);
	QSqlQuery queryPixmap(sql);
	MyLogDEBUG(QString("sql语句：%1").arg(sql).toUtf8());
	queryPixmap.exec();
	queryPixmap.next();
	// 获得头像
	groupPix.load(queryPixmap.value(0).toString());

	// 获得部门名称
	QString strDepName;
	sql = QString("SELECT department_name FROM tab_department WHERE departmentID = %1").arg(DepID);
	QSqlQuery queryDepName(sql);
	MyLogDEBUG(QString("sql语句：%1").arg(sql).toUtf8());
	queryDepName.exec();
	queryDepName.next();
	strDepName = queryDepName.value(0).toString();

	Contactltem *pContactItem = new Contactltem(ui.treeWidget);
	pContactItem->setHeadPixmap(getRoundImage(groupPix, pix, pContactItem->getHeadLabelSize()));
	pContactItem->setUserName(strDepName);

	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);

	//m_groupMap.insert(pChild, sDeps);
}

QString CCMainWindow::getHeadPixturePath() {

	QString strPicturePath = "";
	QString sql = "";


	if (m_isAccountLogin) {		// 账号登录
		sql = QString("SELECT employeeID FROM tab_accounts WHERE account = '%1'").arg(gLoginEmployeeID);
		MyLogDEBUG(QString("sql语句：%1").arg(sql).toUtf8());
		QSqlQuery quereEmployeeId(sql);
		quereEmployeeId.exec();
		quereEmployeeId.next();
		int employeeId = quereEmployeeId.value(0).toInt();
	}
	
	sql = QString("SELECT picture FROM tab_employees WHERE employeeID = %1").arg(gLoginEmployeeID);
	MyLogDEBUG(QString("sql语句：%1").arg(sql).toUtf8());
	QSqlQuery queryPicture(sql);
	queryPicture.exec();
	queryPicture.next();

	strPicturePath = queryPicture.value(0).toString();

	gstrLoginHeadPath = strPicturePath;
	return strPicturePath;
}

void CCMainWindow::ReadDatabaseMessage() { 
	QString sql = QString("SELECT * FROM tab_chat");
	QSqlQuery query;
	query.exec(sql);

	while (query.next()) {
		int sender = query.value(1).toInt();
		int receiver = query.value(2).toInt();
		QString message = query.value(3).toString();
		message = message.simplified();		// 去除开头结尾中间的特殊字符，\r\n\t
		QJsonArray messageArr = QJsonDocument::fromJson(message.toLocal8Bit()).array();

		// 保存
		QMap<int, QJsonArray> messageMap;
		messageMap.insert(receiver, messageArr);
		g_message_info.insert(sender, messageMap);
	}
}

void CCMainWindow::InitSign() {
	QString sql = QString("SELECT employee_sign FROM tab_employees WHERE employeeID = %1").arg(gLoginEmployeeID.toInt());
	qDebug() << sql;
	QSqlQuery querySing;
	querySing.exec(sql);
	querySing.next();

	QString employee_sign = querySing.value("employee_sign").toString();
	ui.lineEdit->setText(employee_sign);

	connect(ui.lineEdit, &QLineEdit::editingFinished, this, [=]() {
		QString employee_sign = ui.lineEdit->text();

		QString sql = QString("UPDATE tab_employees SET employee_sign = '%1' WHERE employeeID = %2").arg(employee_sign).arg(gLoginEmployeeID.toInt());
		qDebug() << sql;
		QSqlQuery querySing;
		querySing.exec(sql);
	});
}

void CCMainWindow::setUserName(const QString & username) {
	MyLogDEBUG(QString("设置用户名为：%1").arg(username).toUtf8());

	ui.nameLabel->adjustSize();	// 根据内容进行设置大小

	// 文本过长则进行省略...
	QString userName = ui.nameLabel->fontMetrics().elidedText(username, Qt::TextElideMode::ElideRight, ui.nameLabel->width());
	ui.nameLabel->setText(userName);
	ui.nameLabel->setToolTip(username);
}

void CCMainWindow::setLevePixmap(int level) {
	MyLogDEBUG(QString("设置等级为：%1").arg(level).toUtf8());

	QPixmap levelPixmap(ui.levelBtn->size());
	levelPixmap.fill(Qt::transparent);

	QPainter painter(&levelPixmap);
	painter.drawPixmap(0, 4, QPixmap(QStringLiteral(":/Resources/MainWindow/lv.png")));

	int unitNum = level % 10;	// 个位数
	int tenNum = level / 10;	// 十位数

	// 十位,截取图片中的部分进行绘制,图片 左上角和右下角区域
	painter.drawPixmap(10, 4, QPixmap(QStringLiteral(":/Resources/MainWindow/levelvalue.png")), tenNum * 6, 0, 6, 7);

	// 个位
	painter.drawPixmap(16, 4, QPixmap(QStringLiteral(":/Resources/MainWindow/levelvalue.png")), unitNum * 6, 0, 6, 7);

	ui.levelBtn->setIcon(levelPixmap);
	ui.levelBtn->setIconSize(ui.levelBtn->size());
}

void CCMainWindow::setHeadPixmap(const QString & headPath) {
	MyLogDEBUG(QString("设置头像路径：%1").arg(headPath).toUtf8());

	QPixmap pix;
	pix.load(":Resources/MainWindow/head_mask.png");
	ui.headLabel->setPixmap(getRoundImage(QPixmap(headPath), pix, ui.headLabel->size()));


}

void CCMainWindow::setStatusMenuIcon(const QString & statusPath) { 
	MyLogDEBUG(QString("设置状态路径：%1").arg(statusPath).toUtf8());

	QPixmap statusBtnPixmap(ui.statusBtn->size());		// 定义图片，并设置大小
	statusBtnPixmap.fill(Qt::transparent);				// 图片填充为透明

	QPainter painter(&statusBtnPixmap);					// 定义画家类，使用图片作为父类，即后面操作都是在图片上进行的
	painter.drawPixmap(4, 4, QPixmap(statusPath));		// 画图，根据路径

	ui.statusBtn->setIcon(statusBtnPixmap);				// 按钮设置图片
	ui.statusBtn->setIconSize(ui.statusBtn->size());	// 按钮设置图片大小
}

QWidget * CCMainWindow::addOtherAppExtension(const QString & appPath, const QString appName) {
	MyLogDEBUG(QString("生成按钮返回 (%1, %2)").arg(appPath).arg(appName).toUtf8());

	QPushButton *btn = new QPushButton(this);
	btn->setFixedSize(20, 20);
	
	QPixmap pixmap(btn->size());
	pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);
	QPixmap appPixmap(appPath);
	painter.drawPixmap((btn->width() - appPixmap.width()) / 2, (btn->height() - appPixmap.height()) / 2, appPixmap);
	btn->setIcon(pixmap);
	btn->setIconSize(btn->size());

	btn->setObjectName(appName);
	btn->setProperty("hasborder", true);

	connect(btn, &QPushButton::clicked, this, &CCMainWindow::onAppIconClicked);

	return btn;
}

void CCMainWindow::initContactTree() { 
	MyLogDEBUG(QString("树初始化").toUtf8());
	// 展开与收缩的信号
	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(onItemClicked(QTreeWidgetItem *, int)));
	connect(ui.treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(onItemExpanded(QTreeWidgetItem *)));
	connect(ui.treeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem *)), this, SLOT(onItemCollapsed(QTreeWidgetItem *)));
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem *, int)));

	// 根节点
	QTreeWidgetItem *pRootGroupItem = new QTreeWidgetItem;
	pRootGroupItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	pRootGroupItem->setData(0, Qt::UserRole, 0);	// 根项数据设为0

	RootContatItem *pItemName = new RootContatItem(true, ui.treeWidget);



	// 获取公司部门ID（公司群号）
	QString sql = QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'").arg("公司群");
	QSqlQuery queryCompDepID(sql);
	MyLogDEBUG(QString("sql语句：%1").arg(sql).toUtf8());

	queryCompDepID.exec();
	queryCompDepID.first();
	int CompDepID = queryCompDepID.value(0).toInt();

	// 获取QQ登录者的所在的部门ID
	sql = QString("SELECT departmentID FROM tab_employees WHERE employeeID = %1").arg(gLoginEmployeeID);
	QSqlQuery querySelfDepID(sql);
	MyLogDEBUG(QString("sql语句：%1").arg(sql).toUtf8());

	querySelfDepID.exec();
	querySelfDepID.first();
	int SelfDepID = querySelfDepID.value(0).toInt();

	// 初始化公司群和自己所对应的部门群
	addCompanyDeps(pRootGroupItem, CompDepID);
	addCompanyDeps(pRootGroupItem, SelfDepID);


	QString strGroupName = QString::fromLocal8Bit("Qt即时通讯");
	pItemName->setText(strGroupName);

	// 插入分组节点
	ui.treeWidget->addTopLevelItem(pRootGroupItem);
	ui.treeWidget->setItemWidget(pRootGroupItem, 0, pItemName);


	//QStringList sCompDeps;	// 公司部门
	//sCompDeps << QString::fromLocal8Bit("公司群");
	//sCompDeps << QString::fromLocal8Bit("人事部");
	//sCompDeps << QString::fromLocal8Bit("研发部");
	//sCompDeps << QString::fromLocal8Bit("市场部");

	//for (int index = 0; index < sCompDeps.size(); index++) {
	//	addCompanyDeps(pRootGroupItem, sCompDeps.at(index));
	//}




}

void CCMainWindow::resizeEvent(QResizeEvent * event) {
	setUserName(QString::fromLocal8Bit("Qt即时通讯软件"));
	BasicWindow::resizeEvent(event);
}

bool CCMainWindow::eventFilter(QObject * obj, QEvent * event) {
	if (ui.searchLineEdit == obj) {
		// 键盘焦点事件
		if (event->type() == QEvent::FocusIn) {
			ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{background-color:rgb(255,255,255);border-bottom:1px solid rgba(%1,%2,%3, 100)}\
													QPushButton#searchBtn{border-image:url(:Resources/MainWindow/search/main_search_deldown.png)}\
													QPushButton#searchBtn:hover{border-image:url(:Resources/MainWindow/search/main_search_delhighlight.png)}\
													QPushButton#searchBtn:pressed{border-image:url(:Resources/MainWindow/search/main_search_delhighdown.png)}")
											.arg(m_colorBackGround.red()).arg(m_colorBackGround.green()).arg(m_colorBackGround.blue()));
		
		} else if (event->type() == QEvent::FocusOut) {
			updateSeachStyle();
		}
	}


	return false;
}

void CCMainWindow::mousePressEvent(QMouseEvent * event) {
	if (qApp->widgetAt(event->pos()) != ui.searchLineEdit && ui.searchLineEdit->hasFocus()) {
		ui.searchLineEdit->clearFocus();
	} else if (qApp->widgetAt(event->pos()) != ui.lineEdit && ui.lineEdit->hasFocus()) {
		ui.lineEdit->clearFocus();
	}

	BasicWindow::mousePressEvent(event);
}

void CCMainWindow::onItemClicked(QTreeWidgetItem * item, int column) {
	bool bIsChild = item->data(0, Qt::UserRole).toBool();

	MyLogDEBUG(QString("树项被单击").toUtf8());
	if (!bIsChild) {
		item->setExpanded(!item->isExpanded());	// 未展开则展开子项
	}
}

void CCMainWindow::onItemExpanded(QTreeWidgetItem * item) {
	MyLogDEBUG(QString("树项被展开").toUtf8());

	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (!bIsChild) {
		// 将基类对象指针或引用转换成派生类指针
		RootContatItem *pRootItem = dynamic_cast<RootContatItem *>(ui.treeWidget->itemWidget(item, 0));
		if (pRootItem) {
			pRootItem->setExpanded(true);
		}
	}
}

void CCMainWindow::onItemCollapsed(QTreeWidgetItem * item) {
	MyLogDEBUG(QString("树项被收缩").toUtf8());

	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (!bIsChild) {
		// 将基类对象指针或引用转换成派生类指针
		RootContatItem *pRootItem = dynamic_cast<RootContatItem *>(ui.treeWidget->itemWidget(item, 0));
		if (pRootItem) {
			pRootItem->setExpanded(false);
		}
	}
}

void CCMainWindow::onItemDoubleClicked(QTreeWidgetItem * item, int column) { 
	MyLogDEBUG(QString("树项被双击").toUtf8());

	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (bIsChild) {
		currentAccount = item->data(0, Qt::UserRole + 1).toString();
		WindowManager::getInstance()->addNewTalkWindow(currentAccount);
	}
}



void CCMainWindow::onAppIconClicked() {

	// 判断信号发送者的对象名是否是app_skin
	if (sender()->objectName() == "app_skin") {
		MyLogDEBUG(QString("皮肤按钮被点击").toUtf8());

		SkinWindow *skinWindow = new SkinWindow;
		skinWindow->show();
	
	} else if (sender()->objectName() == "app_2") {
		MyLogDEBUG(QString("QQ空间按钮被点击").toUtf8());

	} else if (sender()->objectName() == "app_3") {
		MyLogDEBUG(QString("腾讯新闻按钮被点击").toUtf8());

	} else if (sender()->objectName() == "app_4") {
		MyLogDEBUG(QString("购物按钮被点击").toUtf8());

	} else if (sender()->objectName() == "app_5") {
		MyLogDEBUG(QString("花环按钮被点击").toUtf8());

	} else if (sender()->objectName() == "app_6") {
		MyLogDEBUG(QString("体育按钮被点击").toUtf8());

	} else if (sender()->objectName() == "app_7") {
		MyLogDEBUG(QString("网络按钮被点击").toUtf8());

	} else if (sender()->objectName() == "app_8") {
		MyLogDEBUG(QString("游戏按钮被点击").toUtf8());

	} else if (sender()->objectName() == "app_9") {
		MyLogDEBUG(QString("雨伞按钮被点击").toUtf8());

	} else if (sender()->objectName() == "app_10") {
		MyLogDEBUG(QString("云按钮被点击").toUtf8());

	} else if (sender()->objectName() == "app_11") {
		MyLogDEBUG(QString("QQ农场按钮被点击").toUtf8());

	} else {
		MyLogERROR(QString("未识别按钮类型。。。").toUtf8());
	}

}
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

// ��һ��int�����ͷ���QQ�ţ��ڶ���int������շ���QQ�ţ�QJsonArray���������¼
QMultiMap<int, QMap<int, QJsonArray>> g_message_info;	// �����¼

QString gstrLoginHeadPath;	// ��¼�ߵ�ͷ��·��
extern QString gLoginEmployeeID;	// ��¼�ߵ�QQ��
extern QString currentAccount;		// ��ǰ�����˺�

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

	MyLogDEBUG("�������ʼ��");

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
	// ����ȡ����ʱ�����Ʊ߿�
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

	// ����ǩ��
	ui.lineEdit->installEventFilter(this);
	// ��������
	ui.searchLineEdit->installEventFilter(this);


	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowHide(bool)));
	//connect(ui.sysclose, SIGNAL(clicked(bool)), this, SLOT(onShowClose(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), this, SLOT(onShowQuit(bool)));
	connect(NotifyManager::getInstance(), &NotifyManager::signalSkinChanged, [this]() {
		updateSeachStyle();
	});

	// ϵͳ����
	SysTray *systray = new SysTray(this);

	// ��ȡ�����¼
	ReadDatabaseMessage();

	InitSign();
}

void CCMainWindow::updateSeachStyle() { 
	ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{background-color:rgba(%1,%2,%3,50);border-bottom:1px solid rgba(%1,%2,%3, 30)}\
											QPushButton#searchBtn{border-image:url(:Resources/MainWindow/search/search_icon.png)}")
									.arg(m_colorBackGround.red()).arg(m_colorBackGround.green()).arg(m_colorBackGround.blue()));
}

void CCMainWindow::addCompanyDeps(QTreeWidgetItem * pRootGroupItem, int DepID) {
	MyLogDEBUG(QString("Ⱥid��%1").arg(DepID).toUtf8());

	QTreeWidgetItem *pChild = new QTreeWidgetItem;
	QPixmap pix;
	pix.load(":Resources/MainWindow/head_mask.png");

	// ����ӽڵ�
	pChild->setData(0, Qt::UserRole, 1);	// ��������Ϊ1
	pChild->setData(0, Qt::UserRole + 1, DepID);


	// ��ȡ��˾������ͷ��
	QPixmap groupPix;
	QString sql = QString("SELECT picture FROM tab_department WHERE departmentID = %1").arg(DepID);
	QSqlQuery queryPixmap(sql);
	MyLogDEBUG(QString("sql��䣺%1").arg(sql).toUtf8());
	queryPixmap.exec();
	queryPixmap.next();
	// ���ͷ��
	groupPix.load(queryPixmap.value(0).toString());

	// ��ò�������
	QString strDepName;
	sql = QString("SELECT department_name FROM tab_department WHERE departmentID = %1").arg(DepID);
	QSqlQuery queryDepName(sql);
	MyLogDEBUG(QString("sql��䣺%1").arg(sql).toUtf8());
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


	if (m_isAccountLogin) {		// �˺ŵ�¼
		sql = QString("SELECT employeeID FROM tab_accounts WHERE account = '%1'").arg(gLoginEmployeeID);
		MyLogDEBUG(QString("sql��䣺%1").arg(sql).toUtf8());
		QSqlQuery quereEmployeeId(sql);
		quereEmployeeId.exec();
		quereEmployeeId.next();
		int employeeId = quereEmployeeId.value(0).toInt();
	}
	
	sql = QString("SELECT picture FROM tab_employees WHERE employeeID = %1").arg(gLoginEmployeeID);
	MyLogDEBUG(QString("sql��䣺%1").arg(sql).toUtf8());
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
		message = message.simplified();		// ȥ����ͷ��β�м�������ַ���\r\n\t
		QJsonArray messageArr = QJsonDocument::fromJson(message.toLocal8Bit()).array();

		// ����
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
	MyLogDEBUG(QString("�����û���Ϊ��%1").arg(username).toUtf8());

	ui.nameLabel->adjustSize();	// �������ݽ������ô�С

	// �ı����������ʡ��...
	QString userName = ui.nameLabel->fontMetrics().elidedText(username, Qt::TextElideMode::ElideRight, ui.nameLabel->width());
	ui.nameLabel->setText(userName);
	ui.nameLabel->setToolTip(username);
}

void CCMainWindow::setLevePixmap(int level) {
	MyLogDEBUG(QString("���õȼ�Ϊ��%1").arg(level).toUtf8());

	QPixmap levelPixmap(ui.levelBtn->size());
	levelPixmap.fill(Qt::transparent);

	QPainter painter(&levelPixmap);
	painter.drawPixmap(0, 4, QPixmap(QStringLiteral(":/Resources/MainWindow/lv.png")));

	int unitNum = level % 10;	// ��λ��
	int tenNum = level / 10;	// ʮλ��

	// ʮλ,��ȡͼƬ�еĲ��ֽ��л���,ͼƬ ���ϽǺ����½�����
	painter.drawPixmap(10, 4, QPixmap(QStringLiteral(":/Resources/MainWindow/levelvalue.png")), tenNum * 6, 0, 6, 7);

	// ��λ
	painter.drawPixmap(16, 4, QPixmap(QStringLiteral(":/Resources/MainWindow/levelvalue.png")), unitNum * 6, 0, 6, 7);

	ui.levelBtn->setIcon(levelPixmap);
	ui.levelBtn->setIconSize(ui.levelBtn->size());
}

void CCMainWindow::setHeadPixmap(const QString & headPath) {
	MyLogDEBUG(QString("����ͷ��·����%1").arg(headPath).toUtf8());

	QPixmap pix;
	pix.load(":Resources/MainWindow/head_mask.png");
	ui.headLabel->setPixmap(getRoundImage(QPixmap(headPath), pix, ui.headLabel->size()));


}

void CCMainWindow::setStatusMenuIcon(const QString & statusPath) { 
	MyLogDEBUG(QString("����״̬·����%1").arg(statusPath).toUtf8());

	QPixmap statusBtnPixmap(ui.statusBtn->size());		// ����ͼƬ�������ô�С
	statusBtnPixmap.fill(Qt::transparent);				// ͼƬ���Ϊ͸��

	QPainter painter(&statusBtnPixmap);					// ���廭���࣬ʹ��ͼƬ��Ϊ���࣬���������������ͼƬ�Ͻ��е�
	painter.drawPixmap(4, 4, QPixmap(statusPath));		// ��ͼ������·��

	ui.statusBtn->setIcon(statusBtnPixmap);				// ��ť����ͼƬ
	ui.statusBtn->setIconSize(ui.statusBtn->size());	// ��ť����ͼƬ��С
}

QWidget * CCMainWindow::addOtherAppExtension(const QString & appPath, const QString appName) {
	MyLogDEBUG(QString("���ɰ�ť���� (%1, %2)").arg(appPath).arg(appName).toUtf8());

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
	MyLogDEBUG(QString("����ʼ��").toUtf8());
	// չ�����������ź�
	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(onItemClicked(QTreeWidgetItem *, int)));
	connect(ui.treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(onItemExpanded(QTreeWidgetItem *)));
	connect(ui.treeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem *)), this, SLOT(onItemCollapsed(QTreeWidgetItem *)));
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem *, int)));

	// ���ڵ�
	QTreeWidgetItem *pRootGroupItem = new QTreeWidgetItem;
	pRootGroupItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	pRootGroupItem->setData(0, Qt::UserRole, 0);	// ����������Ϊ0

	RootContatItem *pItemName = new RootContatItem(true, ui.treeWidget);



	// ��ȡ��˾����ID����˾Ⱥ�ţ�
	QString sql = QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'").arg("��˾Ⱥ");
	QSqlQuery queryCompDepID(sql);
	MyLogDEBUG(QString("sql��䣺%1").arg(sql).toUtf8());

	queryCompDepID.exec();
	queryCompDepID.first();
	int CompDepID = queryCompDepID.value(0).toInt();

	// ��ȡQQ��¼�ߵ����ڵĲ���ID
	sql = QString("SELECT departmentID FROM tab_employees WHERE employeeID = %1").arg(gLoginEmployeeID);
	QSqlQuery querySelfDepID(sql);
	MyLogDEBUG(QString("sql��䣺%1").arg(sql).toUtf8());

	querySelfDepID.exec();
	querySelfDepID.first();
	int SelfDepID = querySelfDepID.value(0).toInt();

	// ��ʼ����˾Ⱥ���Լ�����Ӧ�Ĳ���Ⱥ
	addCompanyDeps(pRootGroupItem, CompDepID);
	addCompanyDeps(pRootGroupItem, SelfDepID);


	QString strGroupName = QString::fromLocal8Bit("Qt��ʱͨѶ");
	pItemName->setText(strGroupName);

	// �������ڵ�
	ui.treeWidget->addTopLevelItem(pRootGroupItem);
	ui.treeWidget->setItemWidget(pRootGroupItem, 0, pItemName);


	//QStringList sCompDeps;	// ��˾����
	//sCompDeps << QString::fromLocal8Bit("��˾Ⱥ");
	//sCompDeps << QString::fromLocal8Bit("���²�");
	//sCompDeps << QString::fromLocal8Bit("�з���");
	//sCompDeps << QString::fromLocal8Bit("�г���");

	//for (int index = 0; index < sCompDeps.size(); index++) {
	//	addCompanyDeps(pRootGroupItem, sCompDeps.at(index));
	//}




}

void CCMainWindow::resizeEvent(QResizeEvent * event) {
	setUserName(QString::fromLocal8Bit("Qt��ʱͨѶ���"));
	BasicWindow::resizeEvent(event);
}

bool CCMainWindow::eventFilter(QObject * obj, QEvent * event) {
	if (ui.searchLineEdit == obj) {
		// ���̽����¼�
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

	MyLogDEBUG(QString("�������").toUtf8());
	if (!bIsChild) {
		item->setExpanded(!item->isExpanded());	// δչ����չ������
	}
}

void CCMainWindow::onItemExpanded(QTreeWidgetItem * item) {
	MyLogDEBUG(QString("���չ��").toUtf8());

	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (!bIsChild) {
		// ���������ָ�������ת����������ָ��
		RootContatItem *pRootItem = dynamic_cast<RootContatItem *>(ui.treeWidget->itemWidget(item, 0));
		if (pRootItem) {
			pRootItem->setExpanded(true);
		}
	}
}

void CCMainWindow::onItemCollapsed(QTreeWidgetItem * item) {
	MyLogDEBUG(QString("�������").toUtf8());

	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (!bIsChild) {
		// ���������ָ�������ת����������ָ��
		RootContatItem *pRootItem = dynamic_cast<RootContatItem *>(ui.treeWidget->itemWidget(item, 0));
		if (pRootItem) {
			pRootItem->setExpanded(false);
		}
	}
}

void CCMainWindow::onItemDoubleClicked(QTreeWidgetItem * item, int column) { 
	MyLogDEBUG(QString("���˫��").toUtf8());

	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (bIsChild) {
		currentAccount = item->data(0, Qt::UserRole + 1).toString();
		WindowManager::getInstance()->addNewTalkWindow(currentAccount);
	}
}



void CCMainWindow::onAppIconClicked() {

	// �ж��źŷ����ߵĶ������Ƿ���app_skin
	if (sender()->objectName() == "app_skin") {
		MyLogDEBUG(QString("Ƥ����ť�����").toUtf8());

		SkinWindow *skinWindow = new SkinWindow;
		skinWindow->show();
	
	} else if (sender()->objectName() == "app_2") {
		MyLogDEBUG(QString("QQ�ռ䰴ť�����").toUtf8());

	} else if (sender()->objectName() == "app_3") {
		MyLogDEBUG(QString("��Ѷ���Ű�ť�����").toUtf8());

	} else if (sender()->objectName() == "app_4") {
		MyLogDEBUG(QString("���ﰴť�����").toUtf8());

	} else if (sender()->objectName() == "app_5") {
		MyLogDEBUG(QString("������ť�����").toUtf8());

	} else if (sender()->objectName() == "app_6") {
		MyLogDEBUG(QString("������ť�����").toUtf8());

	} else if (sender()->objectName() == "app_7") {
		MyLogDEBUG(QString("���簴ť�����").toUtf8());

	} else if (sender()->objectName() == "app_8") {
		MyLogDEBUG(QString("��Ϸ��ť�����").toUtf8());

	} else if (sender()->objectName() == "app_9") {
		MyLogDEBUG(QString("��ɡ��ť�����").toUtf8());

	} else if (sender()->objectName() == "app_10") {
		MyLogDEBUG(QString("�ư�ť�����").toUtf8());

	} else if (sender()->objectName() == "app_11") {
		MyLogDEBUG(QString("QQũ����ť�����").toUtf8());

	} else {
		MyLogERROR(QString("δʶ��ť���͡�����").toUtf8());
	}

}
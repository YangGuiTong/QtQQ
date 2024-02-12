#include "CCMainWindow.h"

#include <QProxyStyle>
#include <QPainter>
#include <QTimer>
#include <QEvent>

#include "SkinWindow.h"
#include "SysTray.h"
#include "NotifyManager.h"

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

CCMainWindow::CCMainWindow(QWidget *parent)
	: BasicWindow(parent) {
	ui.setupUi(this);

	MyLogDEBUG("�������ʼ��");

	Qt::WindowFlags flag = windowFlags();
	setWindowFlag(Qt::Tool);
	loadStyleSheet(QStringLiteral("CCMainWindow"));

	initControl();
	initTimer();
}

CCMainWindow::~CCMainWindow() {


}

void CCMainWindow::initTimer() {

	QTimer *timer = new QTimer(this);
	timer->setInterval(60000);

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

	setLevePixmap(17);
	setHeadPixmap(":/Resources/MainWindow/girl.png");
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


	// ����ǩ��
	ui.lineEdit->installEventFilter(this);
	// ��������
	ui.searchLineEdit->installEventFilter(this);


	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowHide(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), this, SLOT(onShowClose(bool)));
	connect(NotifyManager::getInstance(), &NotifyManager::signalSkinChanged, [this]() {
		updateSeachStyle();
	});

	// ϵͳ����
	SysTray *systray = new SysTray(this);

}

void CCMainWindow::updateSeachStyle() { 
	ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{background-color:rgba(%1,%2,%3,50);border-bottom:1px solid rgba(%1,%2,%3, 30)}\
											QPushButton#searchBtn{border-image:url(:Resources/MainWindow/search/search_icon.png)}")
									.arg(m_colorBackGround.red()).arg(m_colorBackGround.green()).arg(m_colorBackGround.blue()));
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
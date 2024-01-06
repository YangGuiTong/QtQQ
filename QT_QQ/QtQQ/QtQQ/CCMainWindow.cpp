#include "CCMainWindow.h"

#include <QProxyStyle>
#include <QPainter>

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

	LOG_DEBUG("主窗体初始化");

	Qt::WindowFlags flag = windowFlags();
	setWindowFlag(Qt::Tool);
	loadStyleSheet(QStringLiteral("CCMainWindow"));

	initControl();
}

CCMainWindow::~CCMainWindow() {


}

void CCMainWindow::initControl() { 
	// 树获取焦点时不绘制边框
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

}

void CCMainWindow::setUserName(const QString & username) {
	LOG_DEBUG(QString("设置用户名为：%1").arg(username).toStdString());
}

void CCMainWindow::setLevePixmap(int level) {
	LOG_DEBUG(QString("设置等级为：%1").arg(level).toStdString());

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
	LOG_DEBUG(QString("设置头像路径：%1").arg(headPath).toStdString());

	QPixmap pix;
	pix.load(":Resources/MainWindow/head_mask.png");
	ui.headLabel->setPixmap(getRoundImage(QPixmap(headPath), pix, ui.headLabel->size()));


}

void CCMainWindow::setStatusMenuIcon(const QString & statusPath) { 
	LOG_DEBUG(QString("设置状态路径：%1").arg(statusPath).toStdString());

	QPixmap statusBtnPixmap(ui.statusBtn->size());		// 定义图片，并设置大小
	statusBtnPixmap.fill(Qt::transparent);				// 图片填充为透明

	QPainter painter(&statusBtnPixmap);					// 定义画家类，使用图片作为父类，即后面操作都是在图片上进行的
	painter.drawPixmap(4, 4, QPixmap(statusPath));		// 画图，根据路径

	ui.statusBtn->setIcon(statusBtnPixmap);				// 按钮设置图片
	ui.statusBtn->setIconSize(ui.statusBtn->size());	// 按钮设置图片大小
}

QWidget * CCMainWindow::addOtherAppExtension(const QString & appPath, const QString appName) {
	LOG_DEBUG(QString("CCMainWindow::addOtherAppExtension(%1, %2)").arg(appPath).arg(appName).toStdString());

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



void CCMainWindow::onAppIconClicked() {

}
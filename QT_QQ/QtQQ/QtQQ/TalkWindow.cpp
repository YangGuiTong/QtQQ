#include "TalkWindow.h"
#include "RootContatItem.h"
#include "Contactltem.h"
#include "CommonUtils.h"
#include "WindowManager.h"

TalkWindow::TalkWindow(QWidget *parent, const QString &uid, GroupType groupType)
	: QWidget(parent), m_talkId(uid), m_groupType(groupType) {
	ui.setupUi(this);

	WindowManager::getInstance()->addWindowName(m_talkId, this);

	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
}

TalkWindow::~TalkWindow() {

}

void TalkWindow::addEmotionImage(int emotionNum) {

}

void TalkWindow::setWindowName(const QString & name) {
	ui.nameLabel->setText(name);
}

void TalkWindow::initControl() {
	MyLogDEBUG(QString("初始化").toUtf8());

	QList<int> rightWidgetSize;
	rightWidgetSize << 600 << 138;
	ui.bodySplitter->setSizes(rightWidgetSize);

	ui.textEdit->setFontPointSize(10);
	ui.textEdit->setFocus();

	connect(ui.sysmin, SIGNAL(clicked(bool)), parent(), SLOT(onShowMin(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));
	connect(ui.closeBtn, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));
	
	connect(ui.faceBtn, SIGNAL(clicked(bool)), parent(), SLOT(onEmotionBtnClicked(bool)));
	connect(ui.sendBtn, SIGNAL(clicked(bool)), parent(), SLOT(onSendBtnClicked(bool)));

	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem *, int)));

	switch (m_groupType) {
		case COMPANY:
		{
			initCompanyTalk();
			break;
		}
		case PERSONELGROUP:
		{
			initPersonelTalk();
			break;
		}
		case MARKETGROUP:
		{
			initMarketTalk();
			break;
		}
		case DEVELOPMENTGROUP:
		{
			initDevelopTalk();
			break;
		}
		default:	// 单聊
		{
			initPtoPTalk();
			break;
		}		

	}
}

void TalkWindow::initPtoPTalk() {
	MyLogDEBUG(QString("个人聊天窗口初始化").toUtf8());

	QPixmap pixSkin;
	pixSkin.load(":Resources/MainWindow/skin.png");

	ui.widget->setFixedSize(pixSkin.size());

	QLabel *skinLabel = new QLabel(ui.widget);
	skinLabel->setPixmap(pixSkin);
	skinLabel->setFixedSize(ui.widget->size());
}

void TalkWindow::initCompanyTalk() {
	MyLogDEBUG(QString("公司群聊天窗口初始化").toUtf8());

	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	// 设置data，用于区分根项子项
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContatItem *pItemName = new RootContatItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);		// TalkWindowSheel高度 - TalkWindowSheel头高（talkwindow titleWidget)

	int nEmployeeNum = 50;
	QString qsGroupName = QString::fromLocal8Bit("公司群 %1/%2").arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);

	// 插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	// 展开
	pRootItem->setExpanded(true);

	for (int i = 0; i < nEmployeeNum; i++) {
		addPeopInfo(pRootItem);
	}
}

void TalkWindow::initPersonelTalk() {
	MyLogDEBUG(QString("人事群聊天窗口初始化").toUtf8());

	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	// 设置data，用于区分根项子项
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContatItem *pItemName = new RootContatItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);		// TalkWindowSheel高度 - TalkWindowSheel头高（talkwindow titleWidget)

	int nEmployeeNum = 7;
	QString qsGroupName = QString::fromLocal8Bit("人事群 %1/%2").arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);

	// 插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	// 展开
	pRootItem->setExpanded(true);

	for (int i = 0; i < nEmployeeNum; i++) {
		addPeopInfo(pRootItem);
	}
}

void TalkWindow::initMarketTalk() {
	MyLogDEBUG(QString("市场群聊天窗口初始化").toUtf8());

	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	// 设置data，用于区分根项子项
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContatItem *pItemName = new RootContatItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);		// TalkWindowSheel高度 - TalkWindowSheel头高（talkwindow titleWidget)

	int nEmployeeNum = 15;
	QString qsGroupName = QString::fromLocal8Bit("市场群 %1/%2").arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);

	// 插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	// 展开
	pRootItem->setExpanded(true);

	for (int i = 0; i < nEmployeeNum; i++) {
		addPeopInfo(pRootItem);
	}
}

void TalkWindow::initDevelopTalk() { 
	MyLogDEBUG(QString("研发群聊天窗口初始化").toUtf8());

	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	// 设置data，用于区分根项子项
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContatItem *pItemName = new RootContatItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);		// TalkWindowSheel高度 - TalkWindowSheel头高（talkwindow titleWidget)

	int nEmployeeNum = 28;
	QString qsGroupName = QString::fromLocal8Bit("研发群 %1/%2").arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);

	// 插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	// 展开
	pRootItem->setExpanded(true);

	for (int i = 0; i < nEmployeeNum; i++) {
		addPeopInfo(pRootItem);
	}
}

void TalkWindow::addPeopInfo(QTreeWidgetItem * pRootGroupItem) {
	QTreeWidgetItem *pChild = new QTreeWidgetItem();

	QPixmap pixl;
	pixl.load(":Resources/MainWindow/head_mask.png");
	//QImage image(":Resources/MainWindow/girl.png");
	const QPixmap image(":Resources/MainWindow/girl.png");

	// 添加子节点
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1, QString::number((int)pChild));

	Contactltem *pContactItem = new Contactltem(ui.treeWidget);

	static int i = 0;
	pContactItem->setHeadPixmap(CommonUtils::getRoundImage(image, pixl, pContactItem->getHeadLabelSize()));
	pContactItem->setUserName(QString::fromLocal8Bit("白云%1").arg(i));
	i++;
	pContactItem->setSignName(QString::fromLocal8Bit("这是一个个性签名"));

	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);

	QString str = pContactItem->getUserName();
	m_groupPeopleMap.insert(pChild, str);
}

void TalkWindow::onItemDoubleClicked(QTreeWidgetItem * item, int column) {

}


void TalkWindow::onSendBtnClicked() {

}
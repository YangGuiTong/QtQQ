#include "TalkWindow.h"
#include "RootContatItem.h"
#include "Contactltem.h"
#include "CommonUtils.h"
#include "WindowManager.h"

#include <QToolTip>
#include <QFile>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>

TalkWindow::TalkWindow(QWidget *parent, const QString &uid)
	: QWidget(parent), m_talkId(uid) {
	ui.setupUi(this);

	WindowManager::getInstance()->addWindowName(m_talkId, this);

	setAttribute(Qt::WA_DeleteOnClose);

	initGroupTalkStatus();
	initControl();
}

TalkWindow::~TalkWindow() {
	WindowManager::getInstance()->deleteWindowName(m_talkId);
}

void TalkWindow::addEmotionImage(int emotionNum) {
	ui.textEdit->setFocus();
	ui.textEdit->addEmotionUrl(emotionNum);
}

void TalkWindow::setWindowName(const QString & name) {
	ui.nameLabel->setText(name);
}

QString TalkWindow::GetTalkId() {
	return m_talkId;
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
	connect(ui.sendBtn, SIGNAL(clicked(bool)), this, SLOT(onSendBtnClicked(bool)));

	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem *, int)));


	if (m_isGroupTalk) {
		initTalkWindow();
	} else {
		initPtoPTalk();
	}


/*
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
*/
}

void TalkWindow::initGroupTalkStatus() {
	QSqlQueryModel sqlDepModel;

	QString sql = QString("SELECT * FROM tab_department WHERE departmentID = %1").arg(m_talkId);
	MyLogDEBUG(QString("sql语句：%1").arg(sql).toUtf8());
	sqlDepModel.setQuery(sql);

	int rows = sqlDepModel.rowCount();
	if (rows == 0) {		// 单独聊天
		m_isGroupTalk = false;
	} else {
		m_isGroupTalk = true;
	}
}

int TalkWindow::getCompDepID() {
	QString sql = QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'").arg("公司群");
	MyLogDEBUG(QString("sql语句：%1").arg(sql).toUtf8());
	QSqlQuery queryDepID(sql);
	queryDepID.next();

	int companyId = queryDepID.value(0).toInt();

	return companyId;
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

void TalkWindow::initTalkWindow() { 
	MyLogDEBUG(QString("群聊天窗口初始化").toUtf8());
	
	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	
	// 设置data，用于区分根项子项
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContatItem *pItemName = new RootContatItem(false, ui.treeWidget);
	
	ui.treeWidget->setFixedHeight(646);		// TalkWindowSheel高度 - TalkWindowSheel头高（talkwindow titleWidget)
	
	// 当前聊天的群组名
	QString strGroupName;
	QString sql = QString("SELECT department_name FROM tab_department WHERE departmentID = %1").arg(m_talkId);
	MyLogDEBUG(QString("sql语句：%1").arg(sql).toUtf8());
	QSqlQuery queryGroupName(sql);
	queryGroupName.exec();
	if (queryGroupName.next()) {
		strGroupName = queryGroupName.value(0).toString();
	}

	

	QSqlQueryModel queryEmployeeModel;
	if (getCompDepID() == m_talkId.toInt()) {		// 公司群
		QString sql = QString("SELECT employeeID FROM tab_employees WHERE status = 1");
		MyLogDEBUG(QString("sql语句：%1").arg(sql).toUtf8());
		queryEmployeeModel.setQuery(sql);
	
	} else {
		QString sql = QString("SELECT employeeID FROM tab_employees WHERE status = 1 AND departmentID = %1").arg(m_talkId);
		MyLogDEBUG(QString("sql语句：%1").arg(sql).toUtf8());
		queryEmployeeModel.setQuery(sql);
	}
	int nEmployeeNum = queryEmployeeModel.rowCount();

	QString qsGroupName = QString::fromLocal8Bit("%1 %2/%3").arg(strGroupName).arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);
	
	// 插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);
	
	// 展开
	pRootItem->setExpanded(true);
	
	for (int i = 0; i < nEmployeeNum; i++) {
		QModelIndex modelIndex = queryEmployeeModel.index(i, 0);
		int employeeID = queryEmployeeModel.data(modelIndex).toInt();

		// 添加子节点
		addPeopInfo(pRootItem, employeeID);
	}
}

//
//void TalkWindow::initCompanyTalk() {
//	MyLogDEBUG(QString("公司群聊天窗口初始化").toUtf8());
//
//	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
//	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
//
//	// 设置data，用于区分根项子项
//	pRootItem->setData(0, Qt::UserRole, 0);
//	RootContatItem *pItemName = new RootContatItem(false, ui.treeWidget);
//
//	ui.treeWidget->setFixedHeight(646);		// TalkWindowSheel高度 - TalkWindowSheel头高（talkwindow titleWidget)
//
//	int nEmployeeNum = 50;
//	QString qsGroupName = QString::fromLocal8Bit("公司群 %1/%2").arg(0).arg(nEmployeeNum);
//	pItemName->setText(qsGroupName);
//
//	// 插入分组节点
//	ui.treeWidget->addTopLevelItem(pRootItem);
//	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);
//
//	// 展开
//	pRootItem->setExpanded(true);
//
//	for (int i = 0; i < nEmployeeNum; i++) {
//		addPeopInfo(pRootItem);
//	}
//}
//
//void TalkWindow::initPersonelTalk() {
//	MyLogDEBUG(QString("人事群聊天窗口初始化").toUtf8());
//
//	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
//	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
//
//	// 设置data，用于区分根项子项
//	pRootItem->setData(0, Qt::UserRole, 0);
//	RootContatItem *pItemName = new RootContatItem(false, ui.treeWidget);
//
//	ui.treeWidget->setFixedHeight(646);		// TalkWindowSheel高度 - TalkWindowSheel头高（talkwindow titleWidget)
//
//	int nEmployeeNum = 7;
//	QString qsGroupName = QString::fromLocal8Bit("人事群 %1/%2").arg(0).arg(nEmployeeNum);
//	pItemName->setText(qsGroupName);
//
//	// 插入分组节点
//	ui.treeWidget->addTopLevelItem(pRootItem);
//	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);
//
//	// 展开
//	pRootItem->setExpanded(true);
//
//	for (int i = 0; i < nEmployeeNum; i++) {
//		addPeopInfo(pRootItem);
//	}
//}
//
//void TalkWindow::initMarketTalk() {
//	MyLogDEBUG(QString("市场群聊天窗口初始化").toUtf8());
//
//	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
//	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
//
//	// 设置data，用于区分根项子项
//	pRootItem->setData(0, Qt::UserRole, 0);
//	RootContatItem *pItemName = new RootContatItem(false, ui.treeWidget);
//
//	ui.treeWidget->setFixedHeight(646);		// TalkWindowSheel高度 - TalkWindowSheel头高（talkwindow titleWidget)
//
//	int nEmployeeNum = 15;
//	QString qsGroupName = QString::fromLocal8Bit("市场群 %1/%2").arg(0).arg(nEmployeeNum);
//	pItemName->setText(qsGroupName);
//
//	// 插入分组节点
//	ui.treeWidget->addTopLevelItem(pRootItem);
//	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);
//
//	// 展开
//	pRootItem->setExpanded(true);
//
//	for (int i = 0; i < nEmployeeNum; i++) {
//		addPeopInfo(pRootItem);
//	}
//}
//
//void TalkWindow::initDevelopTalk() { 
//	MyLogDEBUG(QString("研发群聊天窗口初始化").toUtf8());
//
//	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
//	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
//
//	// 设置data，用于区分根项子项
//	pRootItem->setData(0, Qt::UserRole, 0);
//	RootContatItem *pItemName = new RootContatItem(false, ui.treeWidget);
//
//	ui.treeWidget->setFixedHeight(646);		// TalkWindowSheel高度 - TalkWindowSheel头高（talkwindow titleWidget)
//
//	int nEmployeeNum = 28;
//	QString qsGroupName = QString::fromLocal8Bit("研发群 %1/%2").arg(0).arg(nEmployeeNum);
//	pItemName->setText(qsGroupName);
//
//	// 插入分组节点
//	ui.treeWidget->addTopLevelItem(pRootItem);
//	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);
//
//	// 展开
//	pRootItem->setExpanded(true);
//
//	for (int i = 0; i < nEmployeeNum; i++) {
//		addPeopInfo(pRootItem);
//	}
//}

void TalkWindow::addPeopInfo(QTreeWidgetItem * pRootGroupItem, int employeeID) {
	QTreeWidgetItem *pChild = new QTreeWidgetItem();


	// 添加子节点
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1, employeeID);

	Contactltem *pContactItem = new Contactltem(ui.treeWidget);

	QPixmap pixl;
	pixl.load(":Resources/MainWindow/head_mask.png");


	// 获取名称、签名、头像
	QString strName, strSign, strPicturePath, sql;
	QSqlQueryModel queryInfoModel;
	sql = QString("SELECT employee_name, employee_sign, picture FROM tab_employees WHERE employeeID = %1").arg(employeeID);
	queryInfoModel.setQuery(sql);

	QModelIndex nameIndex, signIndex, pictureInex;
	nameIndex = queryInfoModel.index(0, 0);	// 行，列
	signIndex = queryInfoModel.index(0, 1);	// 行，列
	pictureInex = queryInfoModel.index(0, 2);	// 行，列

	strName = queryInfoModel.data(nameIndex).toString();
	strSign = queryInfoModel.data(signIndex).toString();
	strPicturePath = queryInfoModel.data(pictureInex).toString();


	QImage imageHead;
	imageHead.load(strPicturePath);

	pContactItem->setHeadPixmap(CommonUtils::getRoundImage(QPixmap::fromImage(imageHead), pixl, pContactItem->getHeadLabelSize()));
	pContactItem->setUserName(strName);
	pContactItem->setSignName(strSign);

	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);

	QString str = pContactItem->getUserName();
	m_groupPeopleMap.insert(pChild, str);
}

void TalkWindow::onItemDoubleClicked(QTreeWidgetItem * item, int column) {

	int bIsChild = item->data(0, Qt::UserRole).toBool();
	if (bIsChild) {
		MyLogDEBUG(QString("单聊双击").toUtf8());

		QString sgtrPeopelName = m_groupPeopleMap.value(item);
		WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString());
	}
}


void TalkWindow::onSendBtnClicked(bool) {
	if (ui.textEdit->toPlainText().isEmpty()) {
		QToolTip::showText(this->mapToGlobal(QPoint(630, 660)), 
						   "发送的信息不能为空！", this, QRect(0, 0, 120, 100), 2000);
		return;
	}

	QString html = ui.textEdit->document()->toHtml();
	MyLogDEBUG(QString("发送的信息为：%1").arg(html).toUtf8());

	// 文本html如果没有字体则添加字体    msgFont.txt
	if (!html.contains(".png") && !html.contains("</span>")) {
		QString fontHtml;
		QString text = ui.textEdit->toPlainText();
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly)) {
			fontHtml = file.readAll();
			fontHtml.replace("%1", text);
			file.close();
		
		} else {
			MyLogDEBUG(QString("msgFont.txt文件打开失败！").toUtf8());
			QMessageBox::information(this, "提示", "msgFont.txt文件打开失败！"); 
			return;
		}

		if (!html.contains(fontHtml)) {
			html.replace(text, fontHtml);
		}
	}

	ui.textEdit->clear();
	ui.textEdit->deleteAllEmotionImage();
	
	ui.msgWidget->appendMsg(html);
}
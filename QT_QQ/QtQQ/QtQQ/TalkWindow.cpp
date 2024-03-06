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
	MyLogDEBUG(QString("��ʼ��").toUtf8());

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
		default:	// ����
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
	MyLogDEBUG(QString("sql��䣺%1").arg(sql).toUtf8());
	sqlDepModel.setQuery(sql);

	int rows = sqlDepModel.rowCount();
	if (rows == 0) {		// ��������
		m_isGroupTalk = false;
	} else {
		m_isGroupTalk = true;
	}
}

int TalkWindow::getCompDepID() {
	QString sql = QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'").arg("��˾Ⱥ");
	MyLogDEBUG(QString("sql��䣺%1").arg(sql).toUtf8());
	QSqlQuery queryDepID(sql);
	queryDepID.next();

	int companyId = queryDepID.value(0).toInt();

	return companyId;
}

void TalkWindow::initPtoPTalk() {
	MyLogDEBUG(QString("�������촰�ڳ�ʼ��").toUtf8());

	QPixmap pixSkin;
	pixSkin.load(":Resources/MainWindow/skin.png");

	ui.widget->setFixedSize(pixSkin.size());

	QLabel *skinLabel = new QLabel(ui.widget);
	skinLabel->setPixmap(pixSkin);
	skinLabel->setFixedSize(ui.widget->size());
}

void TalkWindow::initTalkWindow() { 
	MyLogDEBUG(QString("Ⱥ���촰�ڳ�ʼ��").toUtf8());
	
	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	
	// ����data���������ָ�������
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContatItem *pItemName = new RootContatItem(false, ui.treeWidget);
	
	ui.treeWidget->setFixedHeight(646);		// TalkWindowSheel�߶� - TalkWindowSheelͷ�ߣ�talkwindow titleWidget)
	
	// ��ǰ�����Ⱥ����
	QString strGroupName;
	QString sql = QString("SELECT department_name FROM tab_department WHERE departmentID = %1").arg(m_talkId);
	MyLogDEBUG(QString("sql��䣺%1").arg(sql).toUtf8());
	QSqlQuery queryGroupName(sql);
	queryGroupName.exec();
	if (queryGroupName.next()) {
		strGroupName = queryGroupName.value(0).toString();
	}

	

	QSqlQueryModel queryEmployeeModel;
	if (getCompDepID() == m_talkId.toInt()) {		// ��˾Ⱥ
		QString sql = QString("SELECT employeeID FROM tab_employees WHERE status = 1");
		MyLogDEBUG(QString("sql��䣺%1").arg(sql).toUtf8());
		queryEmployeeModel.setQuery(sql);
	
	} else {
		QString sql = QString("SELECT employeeID FROM tab_employees WHERE status = 1 AND departmentID = %1").arg(m_talkId);
		MyLogDEBUG(QString("sql��䣺%1").arg(sql).toUtf8());
		queryEmployeeModel.setQuery(sql);
	}
	int nEmployeeNum = queryEmployeeModel.rowCount();

	QString qsGroupName = QString::fromLocal8Bit("%1 %2/%3").arg(strGroupName).arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);
	
	// �������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);
	
	// չ��
	pRootItem->setExpanded(true);
	
	for (int i = 0; i < nEmployeeNum; i++) {
		QModelIndex modelIndex = queryEmployeeModel.index(i, 0);
		int employeeID = queryEmployeeModel.data(modelIndex).toInt();

		// ����ӽڵ�
		addPeopInfo(pRootItem, employeeID);
	}
}

//
//void TalkWindow::initCompanyTalk() {
//	MyLogDEBUG(QString("��˾Ⱥ���촰�ڳ�ʼ��").toUtf8());
//
//	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
//	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
//
//	// ����data���������ָ�������
//	pRootItem->setData(0, Qt::UserRole, 0);
//	RootContatItem *pItemName = new RootContatItem(false, ui.treeWidget);
//
//	ui.treeWidget->setFixedHeight(646);		// TalkWindowSheel�߶� - TalkWindowSheelͷ�ߣ�talkwindow titleWidget)
//
//	int nEmployeeNum = 50;
//	QString qsGroupName = QString::fromLocal8Bit("��˾Ⱥ %1/%2").arg(0).arg(nEmployeeNum);
//	pItemName->setText(qsGroupName);
//
//	// �������ڵ�
//	ui.treeWidget->addTopLevelItem(pRootItem);
//	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);
//
//	// չ��
//	pRootItem->setExpanded(true);
//
//	for (int i = 0; i < nEmployeeNum; i++) {
//		addPeopInfo(pRootItem);
//	}
//}
//
//void TalkWindow::initPersonelTalk() {
//	MyLogDEBUG(QString("����Ⱥ���촰�ڳ�ʼ��").toUtf8());
//
//	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
//	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
//
//	// ����data���������ָ�������
//	pRootItem->setData(0, Qt::UserRole, 0);
//	RootContatItem *pItemName = new RootContatItem(false, ui.treeWidget);
//
//	ui.treeWidget->setFixedHeight(646);		// TalkWindowSheel�߶� - TalkWindowSheelͷ�ߣ�talkwindow titleWidget)
//
//	int nEmployeeNum = 7;
//	QString qsGroupName = QString::fromLocal8Bit("����Ⱥ %1/%2").arg(0).arg(nEmployeeNum);
//	pItemName->setText(qsGroupName);
//
//	// �������ڵ�
//	ui.treeWidget->addTopLevelItem(pRootItem);
//	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);
//
//	// չ��
//	pRootItem->setExpanded(true);
//
//	for (int i = 0; i < nEmployeeNum; i++) {
//		addPeopInfo(pRootItem);
//	}
//}
//
//void TalkWindow::initMarketTalk() {
//	MyLogDEBUG(QString("�г�Ⱥ���촰�ڳ�ʼ��").toUtf8());
//
//	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
//	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
//
//	// ����data���������ָ�������
//	pRootItem->setData(0, Qt::UserRole, 0);
//	RootContatItem *pItemName = new RootContatItem(false, ui.treeWidget);
//
//	ui.treeWidget->setFixedHeight(646);		// TalkWindowSheel�߶� - TalkWindowSheelͷ�ߣ�talkwindow titleWidget)
//
//	int nEmployeeNum = 15;
//	QString qsGroupName = QString::fromLocal8Bit("�г�Ⱥ %1/%2").arg(0).arg(nEmployeeNum);
//	pItemName->setText(qsGroupName);
//
//	// �������ڵ�
//	ui.treeWidget->addTopLevelItem(pRootItem);
//	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);
//
//	// չ��
//	pRootItem->setExpanded(true);
//
//	for (int i = 0; i < nEmployeeNum; i++) {
//		addPeopInfo(pRootItem);
//	}
//}
//
//void TalkWindow::initDevelopTalk() { 
//	MyLogDEBUG(QString("�з�Ⱥ���촰�ڳ�ʼ��").toUtf8());
//
//	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
//	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
//
//	// ����data���������ָ�������
//	pRootItem->setData(0, Qt::UserRole, 0);
//	RootContatItem *pItemName = new RootContatItem(false, ui.treeWidget);
//
//	ui.treeWidget->setFixedHeight(646);		// TalkWindowSheel�߶� - TalkWindowSheelͷ�ߣ�talkwindow titleWidget)
//
//	int nEmployeeNum = 28;
//	QString qsGroupName = QString::fromLocal8Bit("�з�Ⱥ %1/%2").arg(0).arg(nEmployeeNum);
//	pItemName->setText(qsGroupName);
//
//	// �������ڵ�
//	ui.treeWidget->addTopLevelItem(pRootItem);
//	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);
//
//	// չ��
//	pRootItem->setExpanded(true);
//
//	for (int i = 0; i < nEmployeeNum; i++) {
//		addPeopInfo(pRootItem);
//	}
//}

void TalkWindow::addPeopInfo(QTreeWidgetItem * pRootGroupItem, int employeeID) {
	QTreeWidgetItem *pChild = new QTreeWidgetItem();


	// ����ӽڵ�
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1, employeeID);

	Contactltem *pContactItem = new Contactltem(ui.treeWidget);

	QPixmap pixl;
	pixl.load(":Resources/MainWindow/head_mask.png");


	// ��ȡ���ơ�ǩ����ͷ��
	QString strName, strSign, strPicturePath, sql;
	QSqlQueryModel queryInfoModel;
	sql = QString("SELECT employee_name, employee_sign, picture FROM tab_employees WHERE employeeID = %1").arg(employeeID);
	queryInfoModel.setQuery(sql);

	QModelIndex nameIndex, signIndex, pictureInex;
	nameIndex = queryInfoModel.index(0, 0);	// �У���
	signIndex = queryInfoModel.index(0, 1);	// �У���
	pictureInex = queryInfoModel.index(0, 2);	// �У���

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
		MyLogDEBUG(QString("����˫��").toUtf8());

		QString sgtrPeopelName = m_groupPeopleMap.value(item);
		WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString());
	}
}


void TalkWindow::onSendBtnClicked(bool) {
	if (ui.textEdit->toPlainText().isEmpty()) {
		QToolTip::showText(this->mapToGlobal(QPoint(630, 660)), 
						   "���͵���Ϣ����Ϊ�գ�", this, QRect(0, 0, 120, 100), 2000);
		return;
	}

	QString html = ui.textEdit->document()->toHtml();
	MyLogDEBUG(QString("���͵���ϢΪ��%1").arg(html).toUtf8());

	// �ı�html���û���������������    msgFont.txt
	if (!html.contains(".png") && !html.contains("</span>")) {
		QString fontHtml;
		QString text = ui.textEdit->toPlainText();
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly)) {
			fontHtml = file.readAll();
			fontHtml.replace("%1", text);
			file.close();
		
		} else {
			MyLogDEBUG(QString("msgFont.txt�ļ���ʧ�ܣ�").toUtf8());
			QMessageBox::information(this, "��ʾ", "msgFont.txt�ļ���ʧ�ܣ�"); 
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
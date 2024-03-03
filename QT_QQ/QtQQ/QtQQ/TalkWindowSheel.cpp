#include "TalkWindowSheel.h"
#include "CommonUtils.h"
#include "EmotionWindow.h"
#include "public_type.h"
#include "TalkWindowItem.h"

#include <QSqlQueryModel>

TalkWindowSheel::TalkWindowSheel(QWidget *parent)
	: BasicWindow(parent) {
	ui.setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
	initTcpSocket();
}

TalkWindowSheel::~TalkWindowSheel() {

	delete m_emotionWindow;
	m_emotionWindow = NULL;

}

void TalkWindowSheel::addTalkWindow(TalkWindow * talkWindow, TalkWindowItem * talkWindowItem, QString uid) {
	MyLogDEBUG(QString("uid��%1").arg(uid).toUtf8());

	ui.rightStackedWidget->addWidget(talkWindow);
	connect(m_emotionWindow, SIGNAL(signalEmotionWindowHide()), talkWindow, SLOT(onSetEmotionBtnStatus()));

	QListWidgetItem *aItem = new QListWidgetItem(ui.listWidget);
	m_talkwindowItemMap.insert(aItem, talkWindow);

	aItem->setSelected(true);

	
	// �ж���Ⱥ�Ļ��ǵ���
	QSqlQueryModel sqlDepModel;
	QString sql = QString("SELECT picture FROM tab_department WHERE departmentID = %1").arg(uid);
	MyLogDEBUG(QString("sql��䣺%1").arg(sql).toUtf8());
	sqlDepModel.setQuery(sql);

	int rows = sqlDepModel.rowCount();
	if (rows == 0) {	// ����
		sql = QString("SELECT picture FROM tab_employees WHERE employeeID = %1").arg(uid);
		sqlDepModel.setQuery(sql);
	}

	QModelIndex index;
	index = sqlDepModel.index(0, 0);	// 0��0��
	
	QImage img;
	img.load(sqlDepModel.data(index).toString());

	talkWindowItem->setHeadPixmap(QPixmap::fromImage(img));	// ����ͷ��
	ui.listWidget->addItem(aItem);
	ui.listWidget->setItemWidget(aItem, talkWindowItem);

	onTalkWindowItemClicked(aItem);

	connect(talkWindowItem, &TalkWindowItem::signalCloseClicked, [talkWindowItem, talkWindow, aItem, this]() {
		m_talkwindowItemMap.remove(aItem);
		talkWindow->close();
		ui.listWidget->takeItem(ui.listWidget->row(aItem));
		delete talkWindowItem;

		ui.rightStackedWidget->removeWidget(talkWindow);
		if (ui.rightStackedWidget->count() < 1) {
			close();
		}
	});
}

void TalkWindowSheel::setCurrentWidget(QWidget * widget) {
	MyLogDEBUG(QString("���õ�ǰ����").toUtf8());
	ui.rightStackedWidget->setCurrentWidget(widget);
}
const QMap<QListWidgetItem*, QWidget*>& TalkWindowSheel::getTalkWindowItemMap() const {
	return m_talkwindowItemMap;
}

void TalkWindowSheel::initControl() {
	MyLogDEBUG(QString("Qt���촰�ڳ�ʼ��").toUtf8());

	loadStyleSheet("TalkWindow");
	setWindowTitle(QString::fromLocal8Bit("Qt���촰��"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();		// ���ر��鴰��

	QList<int> leftWidgetSize;
	leftWidgetSize << 154 << width() - 154;
	ui.splitter->setSizes(leftWidgetSize);		// ���������óߴ�

	ui.listWidget->setStyle(new CustomProxyStyle(this));

	connect(ui.listWidget, &QListWidget::itemClicked, this, &TalkWindowSheel::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionItemClicked(int)));
}

void TalkWindowSheel::initTcpSocket() {
	MyLogDEBUG(QString("��ʼ��TCP�ͻ���").toUtf8());

	m_tcpClientSocket = new QTcpSocket(this);
	m_tcpClientSocket->connectToHost("127.0.0.1", gtcpProt);

}

void TalkWindowSheel::updateSendTcpMsg(QString & strData, int & msgType, QString sFile) {


}


void TalkWindowSheel::onEmotionBtnClicked(bool) {
	MyLogDEBUG(QString("���鰴ť�����").toUtf8());

	m_emotionWindow->setVisible(m_emotionWindow->isVisible());
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));	// ����ǰ�ؼ������λ��ת��Ϊ��Ļ�ľ���λ��

	emotionPoint.setX(emotionPoint.x() + 170);
	emotionPoint.setY(emotionPoint.y() + 220);
	m_emotionWindow->move(emotionPoint);
	m_emotionWindow->show();
}


void TalkWindowSheel::onTalkWindowItemClicked(QListWidgetItem *item) {

	QWidget *talkWindowWidget = m_talkwindowItemMap.find(item).value();
	ui.rightStackedWidget->setCurrentWidget(talkWindowWidget);
}

void TalkWindowSheel::onEmotionItemClicked(int emotionNum) {
	MyLogDEBUG(QString("%1 ���鱻���").arg(emotionNum).toUtf8());

	TalkWindow *curTalkWindow = dynamic_cast<TalkWindow *>(ui.rightStackedWidget->currentWidget());
	if (curTalkWindow) {
		curTalkWindow->addEmotionImage(emotionNum);
	}
}

#include "TalkWindowItem.h"
#include "public_type.h"
#include "CommonUtils.h"

TalkWindowItem::TalkWindowItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	initControl();
}

TalkWindowItem::~TalkWindowItem()
{}

void TalkWindowItem::setHeadPixmap(const QPixmap& pixmap) {
	MyLogDEBUG(QString("设置头像").toUtf8());

	QPixmap mask = QPixmap(":/Resources/MainWindow/head_mask.png");
	//QPixmap pixmap = QPixmap(":/Resources/MainWindow/girl.png");

	const QPixmap &headpixmap = CommonUtils::getRoundImage(pixmap, mask, ui.headlabel->size());
	ui.headlabel->setPixmap(headpixmap);
}

void TalkWindowItem::setMsgLabelContent(const QString & msg) {
	MyLogDEBUG(QString("设置名字：%1").arg(msg).toUtf8());

	ui.msgLabel->setText(msg);
}

QString TalkWindowItem::getMsgLabelText() {
	
	return ui.msgLabel->text();
}

void TalkWindowItem::initControl() {
	MyLogDEBUG(QString("Qt聊天列表窗口初始化").toUtf8());

	ui.tclosebtn->setVisible(false);
	connect(ui.tclosebtn, SIGNAL(clicked(bool)), this, SIGNAL(signalCloseClicked()));
}

void TalkWindowItem::enterEvent(QEvent * event) {
	MyLogDEBUG(QString("鼠标进入聊天列表窗口").toUtf8());
	ui.tclosebtn->setVisible(true);
	__super::enterEvent(event);
}

void TalkWindowItem::leaveEvent(QEvent * event) {
	MyLogDEBUG(QString("鼠标离开聊天列表窗口").toUtf8());
	ui.tclosebtn->setVisible(false);
	__super::leaveEvent(event);
}

void TalkWindowItem::resizeEvent(QResizeEvent * event) {
	__super::resizeEvent(event);
}

#include "TalkWindowSheel.h"
#include "CommonUtils.h"
#include "EmotionWindow.h"
#include "public_type.h"
#include "TalkWindowItem.h"

TalkWindowSheel::TalkWindowSheel(QWidget *parent)
	: BasicWindow(parent) {
	ui.setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
}

TalkWindowSheel::~TalkWindowSheel() {

	delete m_emotionWindow;
	m_emotionWindow = NULL;

}

void TalkWindowSheel::addTalkWindow(TalkWindow * talkWindow, TalkWindowItem * talkWindowItem, GroupType grouptype) {
	MyLogDEBUG(QString("添加聊天窗口, 类型为：%1").arg(grouptype).toUtf8());

	ui.rightStackedWidget->addWidget(talkWindow);
	connect(m_emotionWindow, SIGNAL(signalEmotionWindowHide()), talkWindow, SLOT(onSetEmotionBtnStatus()));

	QListWidgetItem *aItem = new QListWidgetItem(ui.listWidget);
	m_talkwindowItemMap.insert(aItem, talkWindow);

	aItem->setSelected(true);

	const QPixmap pix(":Resources/MainWindow/girl.png");
	talkWindowItem->setHeadPixmap("");	// 设置头像
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
	MyLogDEBUG(QString("设置当前窗口").toUtf8());
	ui.rightStackedWidget->setCurrentWidget(widget);
}
const QMap<QListWidgetItem*, QWidget*>& TalkWindowSheel::getTalkWindowItemMap() const {
	return m_talkwindowItemMap;
}

void TalkWindowSheel::initControl() {
	MyLogDEBUG(QString("Qt聊天窗口初始化").toUtf8());

	loadStyleSheet("TalkWindow");
	setWindowTitle(QString::fromLocal8Bit("Qt聊天窗口"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();		// 隐藏表情窗口

	QList<int> leftWidgetSize;
	leftWidgetSize << 154 << width() - 154;
	ui.splitter->setSizes(leftWidgetSize);		// 分类器设置尺寸

	ui.listWidget->setStyle(new CustomProxyStyle(this));

	connect(ui.listWidget, &QListWidget::itemClicked, this, &TalkWindowSheel::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionItemClicked(int)));
}


void TalkWindowSheel::onEmotionBtnClicked(bool) {
	MyLogDEBUG(QString("表情按钮被点击").toUtf8());

	m_emotionWindow->setVisible(m_emotionWindow->isVisible());
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));	// 将当前控件的相对位置转换为屏幕的绝对位置

	emotionPoint.setX(emotionPoint.x() + 170);
	emotionPoint.setY(emotionPoint.y() + 220);
	m_emotionWindow->move(emotionPoint);
}


void TalkWindowSheel::onTalkWindowItemClicked(QListWidgetItem *item) {

	QWidget *talkWindowWidget = m_talkwindowItemMap.find(item).value();
	ui.rightStackedWidget->setCurrentWidget(talkWindowWidget);
}

void TalkWindowSheel::onEmotionItemClicked(int emotionNum) {
	MyLogDEBUG(QString("%1 表情被点击").arg(emotionNum).toUtf8());

	TalkWindow *curTalkWindow = dynamic_cast<TalkWindow *>(ui.rightStackedWidget->currentWidget());
	if (curTalkWindow) {
		curTalkWindow->addEmotionImage(emotionNum);
	}
}

#include "TalkWindowSheel.h"
#include "CommonUtils.h"


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

void TalkWindowSheel::initControl() { 
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

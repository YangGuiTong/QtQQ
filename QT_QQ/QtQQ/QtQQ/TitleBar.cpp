#include "TitleBar.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QFile>

#define BUTTON_HEIGHT		27		// 按鈕高度
#define BUTTON_WIDTH		27		// 按鈕宽度
#define	TITLE_HEIGHT		27		// 标题栏高度



TitleBar::TitleBar(QWidget *parent) : QWidget(parent) {

	m_isPressed = false;
	m_buttonType = MIX_MAX_BUTTON;

	initControl();
	initConnections();
	loadStyleSheet("Title");
}

TitleBar::~TitleBar() {

}

void TitleBar::initControl() {
	m_pIcon = new QLabel(this);
	m_pTitleContent = new QLabel(this);

	m_pButtonMin = new QPushButton(this);
	m_pButtonRestore = new QPushButton(this);
	m_pButtonMax = new QPushButton(this);
	m_ButtonClose = new QPushButton(this);

	// 设置按钮固定大小
	m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_ButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

	// 设置对象名
	m_pTitleContent->setObjectName("TitleContent");
	m_pButtonMin->setObjectName("ButtonMin");
	m_pButtonRestore->setObjectName("ButtonRestore");
	m_pButtonMax->setObjectName("ButtonMax");
	m_ButtonClose->setObjectName("ButtonClose");

	// 设置布局
	QHBoxLayout *mylayout = new QHBoxLayout(this);
	mylayout->addWidget(m_pIcon);
	mylayout->addWidget(m_pTitleContent);
	mylayout->addWidget(m_pButtonRestore);
	mylayout->addWidget(m_pButtonMax);
	mylayout->addWidget(m_ButtonClose);

	mylayout->setContentsMargins(5, 0, 0, 0);	// 设置布局的间隙
	mylayout->setSpacing(0);					// 设置布局里部件间的间隙

	m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);		// 水平可拉伸，垂直固定
	setFixedHeight(TITLE_HEIGHT);				// 固定高度
	setWindowFlag(Qt::FramelessWindowHint);		// 无边款
}


// 信号与槽连接
void TitleBar::initConnections() {
	connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
	connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_ButtonClose, SIGNAL(cliecked()), this, SLOT(onButtonCloseClicked()));
}


// 设置标题栏图标
void TitleBar::setrTitleIcon(QString &filePath) {
	QPixmap titleIcon(filePath);
	m_pIcon->setFixedSize(titleIcon.size());
	m_pIcon->setPixmap(titleIcon);
}

// 设置标题栏内容
void TitleBar::setTitleContent(QString &titleContent) {
	m_pTitleContent->setText(titleContent);
	m_titleContent = titleContent;
}

// 设置标题栏的长度
void TitleBar::setTitleWidth(int width) {
	setFixedWidth(width);
}

// 设置标题栏按钮类型
void TitleBar::setButtonType(ButtonType buttontype) {
	m_buttonType = buttontype;

	switch (m_buttonType) {
		case MIN_BUTTON:
		{
			m_pButtonRestore->setVisible(false);
			m_pButtonMax->setVisible(false);
		}
		break;
		case MIX_MAX_BUTTON:
		{
			m_pButtonRestore->setVisible(false);
			//m_pButtonMax->setVisible(true);
		}
		break;
		case MIX_RESTORE_BUTTON:
		{
			/*m_pButtonMax->setVisible(false);
			m_pButtonRestore->setVisible(true);*/
		}
		case QNLY_CLOSE_BUTTON:
		{
			m_pButtonRestore->setVisible(false);
			m_pButtonMin->setVisible(false);
			m_pButtonMax->setVisible(false);
		}
		break;
		case COUNT:
		{

		}
		break;
		default:
			break;
	}
}


// 保存窗口最大化前窗口的位置及大小
void TitleBar::saveRestoreInfo(const QPoint &point, const QSize &size) {
	m_restorePos = point;
	m_restoreSize = size;
}


// 获取窗口最大化前窗口的位置及大小
void TitleBar::getRestoreInfo(QPoint &point, QSize &size) {
	point = m_restorePos;
	size = m_restoreSize;
}

// 绘制标题栏
void TitleBar::paintEvent(QPaintEvent *event) {
	// 设置背景色
	QPainter painter(this);
	QPainterPath pathBack;
	pathBack.setFillRule(Qt::WindingFill);	// 设置填充规则
	pathBack.addRoundedRect(QRect(0, 0, width(), height()), 3, 3);	// 添加圆角矩阵到绘图路径
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	// 当窗口最大化或还原后，窗口长度改变，标题栏相应做出改变；
	if (width() != parentWidget()->width()) {
		setFixedWidth(parentWidget()->width());
	}
}


// 双击响应事件，主要实现双击标题栏进行最大化最小化的操作
void TitleBar::mouseDoubleClickEvent(QMouseEvent *event) {
	// 只有存在最大化、最小化按钮才有效
	if (m_pButtonMax->isVisible()) {
		onButtonMaxClicked();

	} else {
		onButtonRestoreClicked();
	}

	return QWidget::mouseDoubleClickEvent(event);
}

// 通过鼠标按钮下、鼠标移动、鼠标释放事件实现拖动标题栏达到移动窗口效果
void TitleBar::mousePressEvent(QMouseEvent *event) {
	if (m_buttonType == MIX_MAX_BUTTON) {
		// 在窗口最大化时禁止拖动窗口
		if (m_pButtonMax->isVisible()) {
			m_isPressed = true;
			m_startMovePos = event->globalPos();	// 事件发生时鼠标坐标在全局位置
		}

	} else {
		m_isPressed = true;
		m_startMovePos = event->globalPos();
	}

	return QWidget::mousePressEvent(event);
}

void TitleBar::mouseMoveEvent(QMouseEvent *event) {
	if (m_isPressed) {
		QPoint movePoint = event->globalPos() - m_startMovePos;
		QPoint widgetPos = parentWidget()->pos();
		m_startMovePos = event->globalPos();

		parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
	}

	return QWidget::mouseMoveEvent(event);
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event) {
	m_isPressed = false;
	return QWidget::mouseReleaseEvent(event);
}

// 加载样式表
void TitleBar::loadStyleSheet(const QString &sheetName) {
	QFile file(":/Resources/Qss" + sheetName + ".css");
	file.open(QFile::ReadOnly);
	if (file.isOpen()) {
		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());

		setStyleSheet(styleSheet);

		file.close();
	}
}


void TitleBar::onButtonMinClicked() {
	emit signalButtonMinClicked();
}

void TitleBar::onButtonRestoreClicked() {
	m_pButtonRestore->setVisible(false);
	m_pButtonMax->setVisible(true);
	emit signalButtonRestoreClicked();
}

void TitleBar::onButtonMaxClicked() {
	m_pButtonRestore->setVisible(true);
	m_pButtonMax->setVisible(false);
	emit signalButtonMaxClicked();
}

void TitleBar::onButtonCloseClicked() {
	emit signalButtonMinClicked();
}
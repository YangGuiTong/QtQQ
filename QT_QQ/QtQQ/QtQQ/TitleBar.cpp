#include "TitleBar.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QFile>

#define BUTTON_HEIGHT		27		// ���o�߶�
#define BUTTON_WIDTH		27		// ���o���
#define	TITLE_HEIGHT		27		// �������߶�



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

	// ���ð�ť�̶���С
	m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_ButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

	// ���ö�����
	m_pTitleContent->setObjectName("TitleContent");
	m_pButtonMin->setObjectName("ButtonMin");
	m_pButtonRestore->setObjectName("ButtonRestore");
	m_pButtonMax->setObjectName("ButtonMax");
	m_ButtonClose->setObjectName("ButtonClose");

	// ���ò���
	QHBoxLayout *mylayout = new QHBoxLayout(this);
	mylayout->addWidget(m_pIcon);
	mylayout->addWidget(m_pTitleContent);
	mylayout->addWidget(m_pButtonRestore);
	mylayout->addWidget(m_pButtonMax);
	mylayout->addWidget(m_ButtonClose);

	mylayout->setContentsMargins(5, 0, 0, 0);	// ���ò��ֵļ�϶
	mylayout->setSpacing(0);					// ���ò����ﲿ����ļ�϶

	m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);		// ˮƽ�����죬��ֱ�̶�
	setFixedHeight(TITLE_HEIGHT);				// �̶��߶�
	setWindowFlag(Qt::FramelessWindowHint);		// �ޱ߿�
}


// �ź��������
void TitleBar::initConnections() {
	connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
	connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_ButtonClose, SIGNAL(cliecked()), this, SLOT(onButtonCloseClicked()));
}


// ���ñ�����ͼ��
void TitleBar::setrTitleIcon(QString &filePath) {
	QPixmap titleIcon(filePath);
	m_pIcon->setFixedSize(titleIcon.size());
	m_pIcon->setPixmap(titleIcon);
}

// ���ñ���������
void TitleBar::setTitleContent(QString &titleContent) {
	m_pTitleContent->setText(titleContent);
	m_titleContent = titleContent;
}

// ���ñ������ĳ���
void TitleBar::setTitleWidth(int width) {
	setFixedWidth(width);
}

// ���ñ�������ť����
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


// ���洰�����ǰ���ڵ�λ�ü���С
void TitleBar::saveRestoreInfo(const QPoint &point, const QSize &size) {
	m_restorePos = point;
	m_restoreSize = size;
}


// ��ȡ�������ǰ���ڵ�λ�ü���С
void TitleBar::getRestoreInfo(QPoint &point, QSize &size) {
	point = m_restorePos;
	size = m_restoreSize;
}

// ���Ʊ�����
void TitleBar::paintEvent(QPaintEvent *event) {
	// ���ñ���ɫ
	QPainter painter(this);
	QPainterPath pathBack;
	pathBack.setFillRule(Qt::WindingFill);	// ����������
	pathBack.addRoundedRect(QRect(0, 0, width(), height()), 3, 3);	// ���Բ�Ǿ��󵽻�ͼ·��
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	// ��������󻯻�ԭ�󣬴��ڳ��ȸı䣬��������Ӧ�����ı䣻
	if (width() != parentWidget()->width()) {
		setFixedWidth(parentWidget()->width());
	}
}


// ˫����Ӧ�¼�����Ҫʵ��˫�����������������С���Ĳ���
void TitleBar::mouseDoubleClickEvent(QMouseEvent *event) {
	// ֻ�д�����󻯡���С����ť����Ч
	if (m_pButtonMax->isVisible()) {
		onButtonMaxClicked();

	} else {
		onButtonRestoreClicked();
	}

	return QWidget::mouseDoubleClickEvent(event);
}

// ͨ����갴ť�¡�����ƶ�������ͷ��¼�ʵ���϶��������ﵽ�ƶ�����Ч��
void TitleBar::mousePressEvent(QMouseEvent *event) {
	if (m_buttonType == MIX_MAX_BUTTON) {
		// �ڴ������ʱ��ֹ�϶�����
		if (m_pButtonMax->isVisible()) {
			m_isPressed = true;
			m_startMovePos = event->globalPos();	// �¼�����ʱ���������ȫ��λ��
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

// ������ʽ��
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
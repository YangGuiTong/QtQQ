#include "BasicWindow.h"
#include "CommonUtils.h"
#include "NotifyManager.h"

#include <QFile>
#include <QStyleOption>
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QDesktopWidget>

BasicWindow::BasicWindow(QWidget *parent) : QDialog(parent) {

	m_colorBackGround = CommonUtils::getDefaultSkinColor();		// ��ȡ�����ļ��е���ɫ

	setWindowFlags(Qt::FramelessWindowHint);		// �ޱ߿�
	setAttribute(Qt::WA_TranslucentBackground);		// ͸��

	connect(NotifyManager::getInstance(), SIGNAL(signalSkinChanged(const QColor&)), this, SLOT(onSingalSkinChanged(const QColor&)));
}

BasicWindow::~BasicWindow() {

}

void BasicWindow::onSingalSkinChanged(const QColor &color) {
	m_colorBackGround = color;
	loadStyleSheet(m_styleName);

	MyLogDEBUG("��ɫ�л�");
}

void BasicWindow::setTitleBarTitle(const QString &title, const QString &icon) {
	m_pTitleBar->setrTitleIcon(icon);
	m_pTitleBar->setTitleContent(title);
}

void BasicWindow::initTitleBar(ButtonType buttontype) {
	m_pTitleBar = new TitleBar(this);

	m_pTitleBar->setButtonType(buttontype);
	m_pTitleBar->move(0, 0);

	connect(m_pTitleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(m_pTitleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_pTitleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_pTitleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
}

void BasicWindow::loadStyleSheet(const QString &sheetName) {
	m_styleName = sheetName;

	QString filePath = QString(":/Resources/QSS/%1.css").arg(sheetName);
	QFile file(filePath);
	file.open(QFile::ReadOnly);

	if (file.isOpen()) {
		setStyleSheet("");
		QString qsstyleSheet = QLatin1String(file.readAll());

		// ��ȡ�û���ǰRGBֵ
		QString r = QString::number(m_colorBackGround.red());
		QString g = QString::number(m_colorBackGround.green());
		QString b = QString::number(m_colorBackGround.blue());

		qsstyleSheet += QString("QWidget[titleskin=true]\
								 {background-color:rgb(%1,%2,%3);\
								 border-top-left-radius:4px;\
								 border-top-right-radius:4px;}\
								 QWidget[bottomskin=true]\
								 {border-top:1px solid rgba(%1,%2,%3,100);\
								 background-color:rgba(%1,%2,%3,50);\
								 border-bottom-left-radius:4px;\
								 border-bottom-right-radius:4px;}")
								.arg(r).arg(g).arg(b);

		setStyleSheet(qsstyleSheet);

		MyLogDEBUG(QString("BasicWindow::loadStyleSheet ������ʽ�ļ� %1 �ɹ�").arg(filePath).toUtf8());

		file.close();
	}
	else {
		MyLogDEBUG(QString("BasicWindow::loadStyleSheet ����ʽ�ļ� %1 ʧ��").arg(filePath).toUtf8());
	}
}


// ��ʼ��������ɫ
void BasicWindow::initBackGroundColor() {
	QStyleOption opt;
	opt.init(this);

	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

// ���໯����ʱ����Ҫ��д��ͼʱ�����ñ���ͼ
void BasicWindow::paintEvent(QPaintEvent *event) {
	initBackGroundColor();
	QDialog::paintEvent(event);
}

// ͷ��תԲͷ��
QPixmap BasicWindow::getRoundImage(const QPixmap &src, QPixmap &mask, QSize maskSize) {
	if (maskSize == QSize(0, 0)) {
		maskSize = mask.size();
	} else {
		mask = mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	// ����ת�����ͼ��
	QImage resultImage(maskSize, QImage::Format_ARGB32_Premultiplied);

	QPainter painter(&resultImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawPixmap(0, 0, mask);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.drawPixmap(0, 0, src.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	painter.end();

	return QPixmap::fromImage(resultImage);
}

void BasicWindow::onShowClose(bool) {
	close();
}

void BasicWindow::onShowMin(bool) {
	showMinimized();
}

void BasicWindow::onShowHide(bool) {
	hide();
}

void BasicWindow::onShowNormal(bool) {
	show();
	activateWindow();
}

void BasicWindow::onShowQuit(bool) {
	QApplication::quit();
}

// ����ƶ�
void BasicWindow::mouseMoveEvent(QMouseEvent *e) {
	if (m_mousePressed && (e->buttons() == Qt::LeftButton)) {
		// e->globalPos() �¼�����ʱȫ�����꣬�������Ļ���Ͻ�(0, 0)
		move(e->globalPos() - m_mousePoint);
		e->accept();
	}
}

// ��갴��
void BasicWindow::mousePressEvent(QMouseEvent *e) {
	if (e->button() == Qt::LeftButton) {
		// pos() �¼�����ʱ����ڴ������Ͻǵ�ƫ��(0, 0)
		m_mousePressed = true;
		m_mousePoint = e->globalPos() - pos();
		e->accept();
	}
}

// ����ͷ��¼�
void BasicWindow::mouseReleaseEvent(QMouseEvent *e) {
	m_mousePressed = false;
}

void BasicWindow::onButtonMinClicked() {
	if (Qt::Tool == (windowFlags() & Qt::Tool)) {
		hide();
	}
	else {
		showMinimized();
	}
}

void BasicWindow::onButtonRestoreClicked() {
	QPoint windowPos;
	QSize windowSize;
	m_pTitleBar->getRestoreInfo(windowPos, windowSize);
	setGeometry(QRect(windowPos, windowSize));
}

void BasicWindow::onButtonCloseClicked() {
	MyLogDEBUG("�û������رյ�¼����");
	close();
}

void BasicWindow::onButtonMaxClicked() {
	m_pTitleBar->saveRestoreInfo(pos(), QSize(width(), height()));
	QRect desktopRect = QApplication::desktop()->availableGeometry();
	QRect factRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
	setGeometry(factRect);
}
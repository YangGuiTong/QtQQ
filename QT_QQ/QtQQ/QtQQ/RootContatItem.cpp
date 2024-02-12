#include "RootContatItem.h"

#include <QPainter>
#include <QPixmap>

RootContatItem::RootContatItem(bool hasArrow, QWidget *parent)
	: QLabel(parent), m_rotation(0), m_hasArrow(hasArrow) { 

	setFixedHeight(32);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	
	// ��ʼ�����Զ���
	m_animation = new QPropertyAnimation(this, "rotation");
	m_animation->setDuration(50);	// ���õ��ζ���ʱ��50ms
	m_animation->setEasingCurve(QEasingCurve::InQuad);	// ���������������� t^2��0��ʼ����


}

RootContatItem::~RootContatItem() { 


}

void RootContatItem::setText(const QString & title) {
	m_titleText = title;
	update();
}

void RootContatItem::setExpanded(bool expand) {
	if (expand) {
		m_animation->setEndValue(90);	// �������Զ����Ľ���ֵ
	} else {
		m_animation->setEndValue(0);
	}

	m_animation->start();
}

int RootContatItem::rotation() {
	return m_rotation;
}

void RootContatItem::setRotation(int rotation) {
	this->m_rotation = rotation;
	update();
}

void RootContatItem::paintEvent(QPaintEvent * event) {
	QPainter painter(this);
	painter.setRenderHint(QPainter::TextAntialiasing, true);	// �����

	QFont font;
	font.setPointSize(10);
	painter.setFont(font);
	painter.drawText(24, 0, width() - 24, height(), Qt::AlignLeft | Qt::AlignVCenter, m_titleText);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	painter.save();		// ���滭������

	if (m_hasArrow) {
		QPixmap pixmap;
		pixmap.load(":/Resources/MainWindow/arrow.png");

		QPixmap tmpPixmap(pixmap.size());
		tmpPixmap.fill(Qt::transparent);

		QPainter p(&tmpPixmap);
		p.setRenderHint(QPainter::SmoothPixmapTransform, true);

		// ����ϵƫ��(X����ƫ�ƣ�y����ƫ�ƣ�
		p.translate(pixmap.width() / 2, pixmap.height() / 2);
		p.rotate(m_rotation);	// ��ת����ϵ��˳ʱ�룩
		p.drawPixmap(0 - pixmap.width() / 2, 0 - pixmap.height() / 2, pixmap);

		painter.drawPixmap(6, (height() - pixmap.height()) / 2, tmpPixmap);
		painter.restore();	// �ָ���������
	} else {

	}

	QLabel::paintEvent(event);
}

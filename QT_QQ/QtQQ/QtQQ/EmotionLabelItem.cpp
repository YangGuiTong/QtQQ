#include "EmotionLabelItem.h"
#include "public_type.h"

#include <QMovie>

EmotionLabelItem::EmotionLabelItem(QWidget *parent)
	: QClickLabel(parent) {
	
	initControl();

	connect(this, &QClickLabel::clicked, [this] {
		MyLogDEBUG(QString("表情 %1 被点击").arg(m_emotionName).toUtf8());
		emit emotionClicked(m_emotionName);
	});
}

EmotionLabelItem::~EmotionLabelItem() { }

void EmotionLabelItem::setEmotionName(int emotionName) { 
	m_emotionName = emotionName;
	QString imageName = QString(":Resources/MainWindow/emotion/%1.png").arg(m_emotionName);

	m_apngMovie = new QMovie(imageName, "apng", this);
	m_apngMovie->start();
	m_apngMovie->stop();
	setMovie(m_apngMovie);

	//MyLogDEBUG(QString("设置表情 %1").arg(imageName).toUtf8());
}

void EmotionLabelItem::initControl() { 
	//MyLogDEBUG(QString("表情显示初始化").toUtf8());

	setAlignment(Qt::AlignCenter);	// 居中对齐
	setObjectName("emotionLabelItem");
	setFixedSize(32, 32);
}

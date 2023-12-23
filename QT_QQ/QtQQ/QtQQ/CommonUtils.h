#pragma once

#include <QPixmap>
#include <QSize>
#include <QProxyStyle>

#include "MyLogger.h"


#pragma execution_character_set("utf-8") // qt֧����ʾ����

// �ı�ϵͳĬ�ϵĲ������
class CustomProxyStyle : public QProxyStyle {

public:
	CustomProxyStyle(QObject *parent) {
		setParent(parent);
	}

	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = 0) const {
		if (PE_FrameFocusRect == element) {
			// ȥ��windows�в���Ĭ�ϵı߿�����߿򣬲�����ȡ����ʱֱ�ӷ��أ������л���
			return;
		} else {
			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}
	}
};




class CommonUtils {

public:
	CommonUtils();

public:
	static QPixmap getRoundImage(const QPixmap &src, QPixmap &mask, QSize maskSize = QSize(0, 0));
	static void loadStyleSheet(QWidget *widget, const QString &sheetName);
	static void setDefaultSkinColor(const QColor &color);
	static QColor getDefaultSkinColor();
};


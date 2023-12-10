#include "CommonUtils.h"

#include <QPainter>
#include <QFile>
#include <QWidget>
#include <QApplication>
#include <QSettings>

CommonUtils::CommonUtils() { 

}

QPixmap CommonUtils::getRoundImage(const QPixmap &src, QPixmap &mask, QSize masksize) {
	if (masksize == QSize(0, 0)) {
		masksize = mask.size();
	} else {
		mask = mask.scaled(masksize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	QImage resultImage(masksize, QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&resultImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawPixmap(0, 0, mask);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.drawPixmap(0, 0, src.scaled(masksize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	painter.end();

	return QPixmap::fromImage(resultImage);
}

void CommonUtils::loadStyleSheet(QWidget *widget, const QString &sheetName) { 
	QFile file(":/Resources/QSS/" + sheetName + ".css");

	file.open(QFile::ReadOnly);
	if (file.isOpen()) {
		widget->setStyleSheet("");
		QString qsstyleSheet = QLatin1Literal(file.readAll());
		widget->setStyleSheet(qsstyleSheet);
		file.close();
	}
}

void CommonUtils::setDefaultSkinColor(const QColor &color) {
	const QString &&path = QApplication::applicationDirPath() + "/" + QString("tradeprintinfo.ini");

	QSettings settings(path, QSettings::IniFormat);
	settings.setValue("DefaultSkin/red", color.red());
	settings.setValue("DefaultSkin/green", color.green());
	settings.setValue("DefaultSkin/blue", color.blue());
}

QColor CommonUtils::getDefaultSkinColor() {
	QColor color;

	const QString &&path = QApplication::applicationDirPath() + "/" + QString("tradeprintinfo.ini");
	if (!QFile::exists(path)) {
		setDefaultSkinColor(QColor(22, 154, 218));
	} else {
		QSettings settings(path, QSettings::IniFormat);
		color.setRed(settings.value("DefaultSkin/red").toInt());
		color.setGreen(settings.value("Default/green").toInt());
		color.setBlue(settings.value("DefaultSkin/blue").toInt());
	}

	return color;
}

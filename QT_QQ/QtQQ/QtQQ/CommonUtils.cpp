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
		LOG_DEBUG(QString("CommonUtils::loadStyleSheet 加载样式文件 %1.css 成功").arg(sheetName).toStdString());
	
	} else {
		LOG_DEBUG(QString("CommonUtils::loadStyleSheet 加载样式文件 %1.css 失败").arg(sheetName).toStdString());
	}
}

void CommonUtils::setDefaultSkinColor(const QColor &color) {
	const QString &&path = QApplication::applicationDirPath() + "/" + QString("tradeprintinfo.ini");

	QSettings settings(path, QSettings::IniFormat);
	settings.setValue(QStringLiteral("DefaultSkin/red"), color.red());
	settings.setValue(QStringLiteral("DefaultSkin/green"), color.green());
	settings.setValue(QStringLiteral("DefaultSkin/blue"), color.blue());
}

QColor CommonUtils::getDefaultSkinColor() {
	QColor color;

	const QString &&path = QApplication::applicationDirPath() + "/" + QString("tradeprintinfo.ini");
	if (!QFile::exists(path)) {
		setDefaultSkinColor(QColor(22, 154, 218));
		color.setRed(22);
		color.setGreen(154);
		color.setBlue(218);

		LOG_DEBUG("读取tradeprintinfo.ini文件失败，使用默认指定颜色：rgb(22,154,218);");

	} else {
		QSettings settings(path, QSettings::IniFormat);
		int r = settings.value(QStringLiteral("DefaultSkin/red")).toInt();
		int g = settings.value(QStringLiteral("Default/green")).toInt();
		int b = settings.value(QStringLiteral("DefaultSkin/blue")).toInt();
		color.setRed(r);
		color.setGreen(g);
		color.setBlue(b);

		LOG_DEBUG(QString("读取tradeprintinfo.ini文件存储背景颜色为：rgb(%1,%2,%3);").arg(r).arg(g).arg(b).toStdString());
	}

	return color;
}

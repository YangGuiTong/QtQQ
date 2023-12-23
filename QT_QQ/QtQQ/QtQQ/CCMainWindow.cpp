#include "CCMainWindow.h"

#include <QProxyStyle>
#include <QPainter>

class CustomProxyStayle : public QProxyStyle {
public:
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
							   const QWidget *widget = nullptr) const { 
		if (element == PE_FrameFocusRect) {
			return;
		
		} else {
			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}
	}
};

CCMainWindow::CCMainWindow(QWidget *parent)
	: BasicWindow(parent) {
	ui.setupUi(this);

	LOG_DEBUG("�������ʼ��");

	Qt::WindowFlags flag = windowFlags();
	setWindowFlag(Qt::Tool);
	loadStyleSheet(QStringLiteral("CCMainWindow"));

	initControl();
}

CCMainWindow::~CCMainWindow() {


}

void CCMainWindow::initControl() { 
	// ����ȡ����ʱ�����Ʊ߿�
	ui.treeWidget->setStyle(new CustomProxyStayle());

	setLevePixmap(17);

}

void CCMainWindow::setUserName(const QString & username) {
	LOG_DEBUG(QString("�����û���Ϊ��%1").arg(username).toStdString());
}

void CCMainWindow::setLevePixmap(int level) {
	LOG_DEBUG(QString("���õȼ�Ϊ��%1").arg(level).toStdString());

	QPixmap levelPixmap(ui.levelBtn->size());
	levelPixmap.fill(Qt::transparent);

	QPainter painter(&levelPixmap);
	painter.drawPixmap(0, 4, QPixmap(":/Resources/MainWindow/lv.png"));

	int unitNum = level % 10;	// ��λ��
	int tenNum = level / 10;	// ʮλ��

	// ʮλ,��ȡͼƬ�еĲ��ֽ��л���,ͼƬ ���ϽǺ����½�����
	painter.drawPixmap(10, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), tenNum * 6, 0, 6, 7);

	// ��λ
	painter.drawPixmap(16, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), unitNum * 6, 0, 6, 7);

	ui.levelBtn->setIcon(levelPixmap);
	ui.levelBtn->setIconSize(ui.levelBtn->size());
}

void CCMainWindow::setHeadPixmap(const QString & headPath) {
	LOG_DEBUG(QString("����ͷ��·����%1").arg(headPath).toStdString());
}

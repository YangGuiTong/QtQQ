#include "Contactltem.h"
#include "public_type.h"

Contactltem::Contactltem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initControl();
}

Contactltem::~Contactltem()
{}

void Contactltem::setUserName(const QString & userName) {
	MyLogDEBUG(QString("�����û�����%1").arg(userName).toUtf8());
	ui.userName->setText(userName);
}

void Contactltem::setSignName(const QString & signName) {
	MyLogDEBUG(QString("���ø���ǩ����%1").arg(signName).toUtf8());
	ui.signName->setText(signName);
}

void Contactltem::setHeadPixmap(const QPixmap & headPath) { 
	ui.label->setPixmap(headPath);
}

QString Contactltem::getUserName() const {
	return ui.userName->text();
}

QSize Contactltem::getHeadLabelSize() const {
	return ui.label->size();
}

void Contactltem::initControl() { 


}

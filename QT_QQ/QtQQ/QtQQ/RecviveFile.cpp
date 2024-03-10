#include "RecviveFile.h"

#include <QFileDialog>
#include <QMessageBox>

extern QString gfileName;
extern QString gfileData;

RecviveFile::RecviveFile(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	initTitleBar();
	setTitleBarTitle("选择文件", ":/Resources/MainWindow/qqlogoclassic.png");
	loadStyleSheet("ReceiveFile");

	this->move(100, 400);

	

	connect(m_pTitleBar, &TitleBar::signalButtonCloseClicked, this, &RecviveFile::refuseFile);
}

RecviveFile::~RecviveFile()
{}

void RecviveFile::setMsg(QString & msgLabel) {
	ui.label->setText(msgLabel);
}

void RecviveFile::on_cancelBtn_clicked() {
	emit refuseFile();
	this->close();
}

void RecviveFile::on_okBtn_clicked() {
	this->close();

	// 获取想要保存的文件路径
	QString fileDirPath = QFileDialog::getExistingDirectory(nullptr, "文件保存路径", "/");

	QString filePath = fileDirPath + "/" + gfileName;

	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly)) {
		QMessageBox::information(nullptr, "提示", "文件接收失败！");
	} else {
		file.write(gfileData.toUtf8());
		file.close();
		QMessageBox::information(nullptr, "提示", "文件接收成功！");
	}

	
}

#include "SendFile.h"
#include "TalkWindowSheel.h"
#include "WindowManager.h"

#include <QFileDialog>
#include <QMessageBox>

SendFile::SendFile(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);

	m_filePath = "";

	setAttribute(Qt::WA_DeleteOnClose);
	initTitleBar();
	setTitleBarTitle("ѡ���ļ�", ":/Resources/MainWindow/qqlogoclassic.png");
	loadStyleSheet("SendFile");

	this->move(100, 400);

	TalkWindowSheel *talkWindowShell = WindowManager::getInstance()->getTalkWindowSheel();

	connect(this, &SendFile::sendFileClicked, talkWindowShell, &TalkWindowSheel::updateSendTcpMsg);
}

SendFile::~SendFile()
{}

void SendFile::on_openBtn_clicked() {
	m_filePath = QFileDialog::getOpenFileName(this, "ѡ���ļ�", "/", "���͵��ļ�(*.txt *.doc);;�����ļ�(*.*);;");

	ui.lineEdit->setText(m_filePath);
}

void SendFile::on_sendBtn_clicked() {
	if (!m_filePath.isEmpty()) {
		QFile file(m_filePath);
		if (file.open(QIODevice::ReadOnly)) {
			int msgType = 2;
			QString str = file.readAll();

			// �ļ�����
			QFileInfo fileInfo(m_filePath);
			QString fileName = fileInfo.fileName();

			emit sendFileClicked(str, msgType, fileName);
			file.close();
		
		} else {
			QMessageBox::information(this, tr("��ʾ"), QString("�����ļ���%1 ʧ�ܣ�").arg(m_filePath));
		}

		m_filePath = "";
		this->close();
	}
}

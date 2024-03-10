#pragma once

#include "BasicWindow.h"
#include "ui_SendFile.h"

class SendFile : public BasicWindow
{
	Q_OBJECT

public:
	SendFile(QWidget *parent = nullptr);
	~SendFile();


signals:
	// �����ļ�������ź�
	void sendFileClicked(QString &strData, int &msgType, QString fileName);

private slots:
	void on_openBtn_clicked();		// ���ļ�
	void on_sendBtn_clicked();		// ����

private:
	Ui::SendFileClass ui;

	QString m_filePath;			// �û�ѡ��������·��
};

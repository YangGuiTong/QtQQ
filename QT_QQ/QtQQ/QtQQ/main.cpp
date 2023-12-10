#include "CCMainWindow.h"
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QTextCodec>

#include "UserLogin.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //CCMainWindow w;

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");//����"GBK",���ִ�Сд
	QTextCodec::setCodecForLocale(codec);

	char path[1024] = { 0 };
	GetModuleFileNameA(NULL, path, MAX_PATH);		// ��ȡ������·�����磺E:\Tools\qq.exe
	*strrchr(path, '\\') = '\0';					// ��ȡ·�����磺E:\Tools
	std::string logFilePath = path;
	logFilePath += "\\config\\log.conf";

	qDebug() << QString::fromStdString(logFilePath);
	if (!MyLogger::instance()->init(logFilePath)) {
		fprintf(stderr, "init log module failed.\n");
		return -1;
	}

	LOG_DEBUG("\n");
	LOG_DEBUG("------------------------------------------------------------------------------------------------------");

	UserLogin *userLogin = new UserLogin;
	userLogin->show();



    //w.show();
    return a.exec();
}

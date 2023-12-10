#include "CCMainWindow.h"
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QTextCodec>

#include "UserLogin.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //CCMainWindow w;

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");//或者"GBK",不分大小写
	QTextCodec::setCodecForLocale(codec);

	char path[1024] = { 0 };
	GetModuleFileNameA(NULL, path, MAX_PATH);		// 获取到完整路径，如：E:\Tools\qq.exe
	*strrchr(path, '\\') = '\0';					// 截取路径，如：E:\Tools
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

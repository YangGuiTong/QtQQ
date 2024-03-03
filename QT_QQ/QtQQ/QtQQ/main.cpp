#include "CCMainWindow.h"
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QTextCodec>

#include <QQuickWindow>
#include <QSGRendererInterface>

#include "UserLogin.h"


int main(int argc, char *argv[])
{
	QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software);

    QApplication a(argc, argv);
    //CCMainWindow w;

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");//����"GBK",���ִ�Сд
	QTextCodec::setCodecForLocale(codec);

	char path[1024] = { 0 };
	GetModuleFileNameA(NULL, path, MAX_PATH);		// ��ȡ������·�����磺E:\Tools\qq.exe
	*strrchr(path, '\\') = '\0';					// ��ȡ·�����磺E:\Tools
	//std::string logFilePath = path;
	//logFilePath += "\\config\\log.conf";
	std::string logPath = path;
	logPath += "\\log\\Qt_QQ";
	

	//qDebug() << QString::fromStdString(logFilePath);
	//if (!MyLogger::instance()->init(logFilePath)) {
	//	fprintf(stderr, "init log module failed.\n");
	//	return -1;
	//}


	if (!MyLog::init(logPath, true)) {
		fprintf(stderr, "init log module failed.\n");
		return -1;
	}

	MyLogDEBUG("\n\n------------------------------------------------------------------------------------------------------");


	a.setQuitOnLastWindowClosed(false);


	UserLogin *userLogin = new UserLogin;
	userLogin->show();



    //w.show();
    return a.exec();
}

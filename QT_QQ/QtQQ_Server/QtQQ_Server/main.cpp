#include "QtQQ_Server.h"
#include <QtWidgets/QApplication>

#include <QTextCodec>

#include "public_type.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtQQ_Server w;






	QTextCodec *codec = QTextCodec::codecForName("UTF-8");//����"GBK",���ִ�Сд
	QTextCodec::setCodecForLocale(codec);

	char path[1024] = { 0 };
	GetModuleFileNameA(NULL, path, MAX_PATH);		// ��ȡ������·�����磺E:\Tools\qq.exe
	*strrchr(path, '\\') = '\0';					// ��ȡ·�����磺E:\Tools
	std::string logPath = path;
	logPath += "\\log\\QtServer";

	if (!MyLog::init(logPath, true)) {
		fprintf(stderr, "init log module failed.\n");
		return -1;
	}

	MyLogDEBUG("\n\n------------------------------------------------------------------------------------------------------");








    w.show();
    return a.exec();
}

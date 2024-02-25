#include "MyLog.h"

#include <time.h>

MyLog* MyLog::log = NULL;
std::string MyLog::_screenInfo = "screenInfo";
std::string MyLog::_logName = "log";

log4cpp::Category& root = log4cpp::Category::getRoot();
log4cpp::Category& MyLog::logCat = root.getInstance(MyLog::_logName);
log4cpp::Category& MyLog::coutCat = root.getInstance(MyLog::_screenInfo);

// ���ȼ�
log4cpp::Priority::PriorityLevel MyLog::coutPri = COUNT_PRITY;		// ����̨
log4cpp::Priority::PriorityLevel MyLog::logPri = LOG_PRITY;		// �����ļ�

log4cpp::PatternLayout* MyLog::logLayout = NULL;
log4cpp::PatternLayout* MyLog::screenLayout = NULL;
log4cpp::PatternLayout* MyLog::logLayout2 = NULL;	/* �ؾ������ */

log4cpp::FileAppender* MyLog::logFile = NULL;//�ļ���־����
log4cpp::OstreamAppender* MyLog::logScreen = NULL;//��Ļ��־����
log4cpp::RollingFileAppender *MyLog::rollLogFile;	/* �ؾ������ */

bool MyLog::init(std::string logName, bool logBakcName, bool toScreen) {

	std::string year, month, day, hour, min, sec;
	getCurrentTime(year, month, day, hour, min, sec);

	// �ж���������ļ�������Ϊ�գ���ΪĬ�ϲ�������ʹ�õ�ǰ������.log��Ϊ��־�ļ���
	if ("" == logName || "time" == logName) {
		logName = year + month + day + ".log";

	} else if (true == logBakcName) {
		logName = logName + "_" + year + "-" + month + "-" + day + ".log";
	}

	if (MyLog::log == NULL) {
		MyLog::log = new MyLog(toScreen);

		MyLog::_logName = logName;

		log4cpp::Category& logCat = root.getInstance(MyLog::_logName);
		log4cpp::Category& coutCat = root.getInstance(MyLog::_screenInfo);

		logScreen = new log4cpp::OstreamAppender("logScreen", &std::cout);
		logFile = new log4cpp::FileAppender("logFile", MyLog::_logName);								/* Ȼ��ע����� */
		rollLogFile = new log4cpp::RollingFileAppender("rollLogFile", MyLog::_logName, 1024*1024, 5);	/* �ؾ������ */	// ������־�ļ���С1M��5���ؾ�


		//���ò���
		MyLog::logLayout = new log4cpp::PatternLayout();	/* Ȼ��ע����� */
		MyLog::screenLayout = new log4cpp::PatternLayout();
		MyLog::logLayout2 = new log4cpp::PatternLayout();	/* �ؾ������ */
		logLayout->setConversionPattern("%d{%Y-%m-%d %H:%M:%S.%l} [%p]%m%n");
		screenLayout->setConversionPattern("%d{%Y-%m-%d %H:%M:%S.%l} [%p] %m%n");
		logLayout2->setConversionPattern("%d{%Y-%m-%d %H:%M:%S} [%p] %m%n");
		MyLog::logScreen->setLayout(screenLayout);
		MyLog::logFile->setLayout(logLayout);			/* Ȼ��ע����� */
		MyLog::rollLogFile->setLayout(logLayout2);	/* �ؾ������ */


		//׷�ӵ�Ŀ¼	
		MyLog::logCat.addAppender(MyLog::logFile);			/* Ȼ��ע����� */
		//MyLog::logCat.addAppender(MyLog::rollLogFile);	/* �ؾ������ */
		MyLog::coutCat.addAppender(MyLog::logScreen);

		//�������ȼ�
		MyLog::logCat.setPriority(MyLog::logPri);
		MyLog::coutCat.setPriority(MyLog::coutPri);
	}
	MyLog::log->outToScreen = toScreen;


	return true;
}

//��ȡ��־����,Ĭ�ϲ���ѡ���Ƿ��������Ļ
MyLog* MyLog::getLog(bool toScreen) {
	MyLog::log->outToScreen = toScreen;

	if (NULL == MyLog::log) {
		printf("MyLog::log is NULL, please use MyLog::init!\n");
		return NULL;
	}

	return MyLog::log;
}
//������־����
void MyLog::destoryLog() {
	log4cpp::Category::shutdown();
	delete MyLog::log;
}
//������־��¼���ȼ�
void MyLog::setPri(log4cpp::Priority::PriorityLevel coutLevel, log4cpp::Priority::PriorityLevel logLevel) {
	MyLog::logPri = logLevel;
	MyLog::coutPri = coutLevel;
	MyLog::logCat.setPriority(MyLog::logPri);
	MyLog::coutCat.setPriority(MyLog::coutPri);
}
//��¼��־,���ò���__FILE__, __LINE__ ,__FUNCTION__
void MyLog::warn(const char * msg, int line, const char *function) {
	char info[4096] = { 0 };
	sprintf(info, "[%s][%d]: %s", function, line, msg);
	if (this->outToScreen) {
		logCat.warn(info);
		coutCat.warn(info);
	} else {
		logCat.warn(info);
	}
}
void MyLog::error(const char * msg, int line, const char *function) {
	char info[4096] = { 0 };
	sprintf(info, "[%s][%d]: %s", function, line, msg);
	if (this->outToScreen) {
		logCat.error(info);
		coutCat.error(info);
	} else {
		logCat.error(info);
	}
}
void MyLog::debug(const char * msg, int line, const char *function) {
	char info[4096] = { 0 };
	sprintf(info, "[%s][%d]: %s", function, line, msg);
	if (this->outToScreen) {
		logCat.debug(info);
		coutCat.debug(info);
	} else {
		logCat.debug(info);
	}
}
void MyLog::info(const char * msg, int line, const char *function) {
	char info[4096] = { 0 };
	sprintf(info, "[%s][%d]: %s", function, line, msg);

	if (this->outToScreen) {
		logCat.info(info);
		coutCat.info(info);
	} else {
		logCat.info(info);
	}
}

std::string MyLog::getCurrentTime(std::string& year, std::string& month, std::string& day, std::string& hour, std::string& min, std::string& sec) {
	// ��ȡϵͳʱ�� - ������ʱ����
	time_t _time;
	struct tm* target_time;
	time(&_time);
	target_time = localtime(&_time);


	year = std::to_string(target_time->tm_year + 1900);
	month = target_time->tm_mon + 1 > 9 ? std::to_string(target_time->tm_mon + 1) : "0" + std::to_string(target_time->tm_mon + 1);
	day = target_time->tm_mday > 9 ? std::to_string(target_time->tm_mday) : "0" + std::to_string(target_time->tm_mday);
	hour = target_time->tm_hour > 9 ? std::to_string(target_time->tm_hour) : "0" + std::to_string(target_time->tm_hour);
	min = target_time->tm_min > 9 ? std::to_string(target_time->tm_min) : "0" + std::to_string(target_time->tm_min);
	sec = target_time->tm_sec > 9 ? std::to_string(target_time->tm_sec) : "0" + std::to_string(target_time->tm_sec);

	return year + month + day + hour + min + sec;
}
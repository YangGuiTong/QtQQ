#ifndef _MY_LOG_H_
#define _MY_LOG_H_

#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/RollingFileAppender.hh>

#include <string>


// ���ȼ�
#define COUNT_PRITY			log4cpp::Priority::INFO;		// ����̨
#define LOG_PRITY			log4cpp::Priority::DEBUG;		// �����ļ�


/*���õ���ģʽ��ƣ���������category����һ�������������Ļ����Ϣ��һ�������¼����־����Ϣ��
ͨ���������ȼ���𣬿���ʵ��������Ϣ����¼����־�У�����error�����ϵ���Ϣʱ��ӡ����Ļ��*/
class MyLog {
private:
	MyLog(bool b) {
		outToScreen = b;
	}
	~MyLog() { }
	static MyLog * log;
	bool outToScreen;//�Ƿ������־��Ϣ����Ļ
	static std::string _screenInfo;//��Ļ��־��Ϣ
	static std::string _logName;//�ļ���־����

	static log4cpp::Category& logCat;
	static log4cpp::Category& coutCat;

	static log4cpp::FileAppender* logFile;//�ļ���־����
	static log4cpp::OstreamAppender* logScreen;//��Ļ��־����
	static log4cpp::RollingFileAppender *rollLogFile;	/* �ؾ������ */

	static log4cpp::Priority::PriorityLevel logPri;//�ļ���־���ȼ�
	static log4cpp::Priority::PriorityLevel coutPri;//��Ļ��־���ȼ�

	static log4cpp::PatternLayout* logLayout;//��־���� 
	static log4cpp::PatternLayout* screenLayout;//��Ļ���� 
	static log4cpp::PatternLayout* logLayout2;	/* �ؾ������ */

private:
	// ���ص�ǰ������ʱ����
	static std::string getCurrentTime(std::string& year, std::string& month, std::string& day, std::string& hour, std::string& min, std::string& sec);

public:
	// ��ʼ����־������Ϣ
	static bool init(std::string logName = "time", bool logBakcName = false, bool toScreen = false);
	//��ȡ��־����,Ĭ�ϲ���ѡ���Ƿ��������Ļ
	static MyLog* getLog(bool toScreen = false);
	//������־����
	static void destoryLog();
	//������־��¼���ȼ�
	static void setPri(log4cpp::Priority::PriorityLevel coutLevel, log4cpp::Priority::PriorityLevel logLevel);
	//��¼��־,���ò��� __LINE__ ,__FUNCTION__
	void warn(const char * msg, int line = __LINE__, const char *function = "warn");
	void error(const char * msg, int line = __LINE__, const char *function = "error");
	void debug(const char * msg, int line = __LINE__, const char *function = "debug");
	void info(const char * msg, int line = __LINE__, const char *function = "info");
};

//Ϊ����ÿ�ε��ö�Ҫ��д����__LINE__��__FUNCTION__,����ʹ�ô������ĺ궨��
#define  MyLogWARN(msg) MyLog::getLog()->warn(msg,__LINE__,__FUNCTION__);
#define  MyLogINFO(msg) MyLog::getLog()->info(msg,__LINE__,__FUNCTION__);
#define  MyLogERROR(msg) MyLog::getLog()->error(msg,__LINE__,__FUNCTION__);
#define  MyLogDEBUG(msg) MyLog::getLog()->debug(msg,__LINE__,__FUNCTION__);

#endif
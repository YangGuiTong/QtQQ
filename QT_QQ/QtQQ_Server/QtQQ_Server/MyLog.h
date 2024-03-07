#ifndef _MY_LOG_H_
#define _MY_LOG_H_

#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/RollingFileAppender.hh>

#include <string>


// 优先级
#define COUNT_PRITY			log4cpp::Priority::INFO;		// 控制台
#define LOG_PRITY			log4cpp::Priority::DEBUG;		// 本地文件


/*采用单例模式设计，包含两个category对象，一个负责输出到屏幕的信息，一个负责记录到日志的信息，
通过设置优先级差别，可以实现所有信息都记录在日志中，遇到error及以上的信息时打印到屏幕上*/
class MyLog {
private:
	MyLog(bool b) {
		outToScreen = b;
	}
	~MyLog() { }
	static MyLog * log;
	bool outToScreen;//是否输出日志信息到屏幕
	static std::string _screenInfo;//屏幕日志信息
	static std::string _logName;//文件日志名称

	static log4cpp::Category& logCat;
	static log4cpp::Category& coutCat;

	static log4cpp::FileAppender* logFile;//文件日志输入
	static log4cpp::OstreamAppender* logScreen;//屏幕日志输入
	static log4cpp::RollingFileAppender *rollLogFile;	/* 回卷用这个 */

	static log4cpp::Priority::PriorityLevel logPri;//文件日志优先级
	static log4cpp::Priority::PriorityLevel coutPri;//屏幕日志优先级

	static log4cpp::PatternLayout* logLayout;//日志布局 
	static log4cpp::PatternLayout* screenLayout;//屏幕布局 
	static log4cpp::PatternLayout* logLayout2;	/* 回卷用这个 */

private:
	// 返回当前年月日时分秒
	static std::string getCurrentTime(std::string& year, std::string& month, std::string& day, std::string& hour, std::string& min, std::string& sec);

public:
	// 初始化日志配置信息
	static bool init(std::string logName = "time", bool logBakcName = false, bool toScreen = false);
	//获取日志函数,默认参数选择是否输出到屏幕
	static MyLog* getLog(bool toScreen = false);
	//销毁日志对象
	static void destoryLog();
	//设置日志记录优先级
	static void setPri(log4cpp::Priority::PriorityLevel coutLevel, log4cpp::Priority::PriorityLevel logLevel);
	//记录日志,调用参数 __LINE__ ,__FUNCTION__
	void warn(const char * msg, int line = __LINE__, const char *function = "warn");
	void error(const char * msg, int line = __LINE__, const char *function = "error");
	void debug(const char * msg, int line = __LINE__, const char *function = "debug");
	void info(const char * msg, int line = __LINE__, const char *function = "info");
};

//为避免每次调用都要填写参数__LINE__和__FUNCTION__,可以使用带参数的宏定义
#define  MyLogWARN(msg) MyLog::getLog()->warn(msg,__LINE__,__FUNCTION__);
#define  MyLogINFO(msg) MyLog::getLog()->info(msg,__LINE__,__FUNCTION__);
#define  MyLogERROR(msg) MyLog::getLog()->error(msg,__LINE__,__FUNCTION__);
#define  MyLogDEBUG(msg) MyLog::getLog()->debug(msg,__LINE__,__FUNCTION__);

#endif
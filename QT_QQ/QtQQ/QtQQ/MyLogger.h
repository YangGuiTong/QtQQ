#ifndef _MY_LOGGER_H_
#define _MY_LOGGER_H_

#include <string>
#include <log4cpp/Category.hh>

class MyLogger {
public:
	bool init(const std::string &log_conf_file);			// 指定加载log配置文件
	static MyLogger *instance() { return &_instance; };		// 单例模式，返回自己

	log4cpp::Category *GetHandle() { return _category; };	

private:
	static MyLogger _instance;
	log4cpp::Category *_category;	// 通过此对象可以实现日志写入
};

/* 宏定义，方便调用 */
#define LOG_DEBUG MyLogger::instance()->GetHandle()->debug		// 调试
#define LOG_INFO MyLogger::instance()->GetHandle()->info		// 信息，消息
#define Log_NOTICE MyLogger::instance()->GetHandle()->notice	// 通知
#define LOG_WARN MyLogger::instance()->GetHandle()->warn		// 警告
#define LOG_ERROR MyLogger::instance()->GetHandle()->error		// 错误
#define LOG_FATAL MyLogger::instance()->GetHandle()->fatal		// 致命错误

/*
 * __LINE__ : 文件中的当前行号；
 * __FILE__ : 文件的完整路径和文件名；如果用在包含文件中，则返回包含文件名；
 * __FUNCTION__ : 函数名字。
 */
#define LOG(__level) log4cpp::Category::getRoot() << log4cpp::Priority::__level << "[" << __FUNCTION__ << "][" << __LINE__ << "]: "
//#define LOG(__level) log4cpp::Category::getRoot() << log4cpp::Priority::__level << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]: "

#endif
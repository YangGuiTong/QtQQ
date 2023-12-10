#include "MyLogger.h"

#include <iostream>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/PropertyConfigurator.hh>


// 静态变量，需要在类外部初始化一下
MyLogger MyLogger::_instance;

bool MyLogger::init(const std::string &log_conf_file) {
	try {
		log4cpp::PropertyConfigurator::configure(log_conf_file);	// 初始化log配置文件
	} catch (log4cpp::ConfigureFailure &f) {
		std::cerr << "load log config file " << log_conf_file.c_str() << " failed with result: " << f.what() << std::endl;
		return false;
	}

	// 初始化成功后，使用getRoot()获取操作日志的对象
	_category = &log4cpp::Category::getRoot();
	
	return true;
}
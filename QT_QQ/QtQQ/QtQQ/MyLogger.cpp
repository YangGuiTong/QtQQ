#include "MyLogger.h"

#include <iostream>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/PropertyConfigurator.hh>


// ��̬��������Ҫ�����ⲿ��ʼ��һ��
MyLogger MyLogger::_instance;

bool MyLogger::init(const std::string &log_conf_file) {
	try {
		log4cpp::PropertyConfigurator::configure(log_conf_file);	// ��ʼ��log�����ļ�
	} catch (log4cpp::ConfigureFailure &f) {
		std::cerr << "load log config file " << log_conf_file.c_str() << " failed with result: " << f.what() << std::endl;
		return false;
	}

	// ��ʼ���ɹ���ʹ��getRoot()��ȡ������־�Ķ���
	_category = &log4cpp::Category::getRoot();
	
	return true;
}
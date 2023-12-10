#ifndef _MY_LOGGER_H_
#define _MY_LOGGER_H_

#include <string>
#include <log4cpp/Category.hh>

class MyLogger {
public:
	bool init(const std::string &log_conf_file);			// ָ������log�����ļ�
	static MyLogger *instance() { return &_instance; };		// ����ģʽ�������Լ�

	log4cpp::Category *GetHandle() { return _category; };	

private:
	static MyLogger _instance;
	log4cpp::Category *_category;	// ͨ���˶������ʵ����־д��
};

/* �궨�壬������� */
#define LOG_DEBUG MyLogger::instance()->GetHandle()->debug		// ����
#define LOG_INFO MyLogger::instance()->GetHandle()->info		// ��Ϣ����Ϣ
#define Log_NOTICE MyLogger::instance()->GetHandle()->notice	// ֪ͨ
#define LOG_WARN MyLogger::instance()->GetHandle()->warn		// ����
#define LOG_ERROR MyLogger::instance()->GetHandle()->error		// ����
#define LOG_FATAL MyLogger::instance()->GetHandle()->fatal		// ��������

/*
 * __LINE__ : �ļ��еĵ�ǰ�кţ�
 * __FILE__ : �ļ�������·�����ļ�����������ڰ����ļ��У��򷵻ذ����ļ�����
 * __FUNCTION__ : �������֡�
 */
#define LOG(__level) log4cpp::Category::getRoot() << log4cpp::Priority::__level << "[" << __FUNCTION__ << "][" << __LINE__ << "]: "
//#define LOG(__level) log4cpp::Category::getRoot() << log4cpp::Priority::__level << "[" << __FILE__ << "][" << __FUNCTION__ << "][" << __LINE__ << "]: "

#endif
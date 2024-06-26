#ifndef PUBLIC_TYPE_H
#define PUBLIC_TYPE_H

#include "MyLog.h"
#include "MyLogger.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QMap>
#include <QJsonDocument>


#pragma execution_character_set("utf-8") // qt支持显示中文


enum GroupType {
	COMPANY = 0,		// 公司群
	PERSONELGROUP = 1,		// 人事部
	DEVELOPMENTGROUP = 2,		// 研发部
	MARKETGROUP = 3,		// 市场部
	PTOP = 4,		// 同事之间单独聊天

	GROUP_TYPE_COUNT
};



const int gTcpPort = 10086;
const int gUdpPort = 10010;

#endif	// PUBLIC_TYPE_H
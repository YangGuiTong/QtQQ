#pragma once

#include "MyLog.h"
#include "MyLogger.h"


#pragma execution_character_set("utf-8") // qt֧����ʾ����


enum GroupType {
	COMPANY = 0,		// ��˾Ⱥ
	PERSONELGROUP = 1,		// ���²�
	DEVELOPMENTGROUP = 2,		// �з���
	MARKETGROUP = 3,		// �г���
	PTOP = 4,		// ͬ��֮�䵥������

	GROUP_TYPE_COUNT
};
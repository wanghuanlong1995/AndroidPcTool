// 落羽杉配置和设置相关的
#pragma once

#include <afxwin.h>   

// 是否可以上外网
#define CONFIG_CAN_LOGIN_NET _T("CONFIG_CAN_LOGIN_NET")

// 扩展工具目录字段
#define CONFIG_TOOL_EX_PATH _T("CONFIG_TOOL_EX_PATH")

// 截屏导出目录字段
#define CONFIG_SHOT_PIC_PATH  _T("CONFIG_SHOT_PIC_PATH")

// 日志导出目录字段
#define CONFIG_LOG_PATH  _T("CONFIG_LOG_PULL_PATH")

// 常用目录使用字段
#define USE_PATH_COUNT 12
#define TOTAL_PATH_COUNT (3 + USE_PATH_COUNT)

#define CONFIG_USE_PATH1  _T("CONFIG_USE_PATH1")
#define CONFIG_USE_PATH2  _T("CONFIG_USE_PATH2")
#define CONFIG_USE_PATH3  _T("CONFIG_USE_PATH3")
#define CONFIG_USE_PATH4  _T("CONFIG_USE_PATH4")
#define CONFIG_USE_PATH5  _T("CONFIG_USE_PATH5")
#define CONFIG_USE_PATH6  _T("CONFIG_USE_PATH6")
#define CONFIG_USE_PATH7  _T("CONFIG_USE_PATH7")
#define CONFIG_USE_PATH8  _T("CONFIG_USE_PATH8")
#define CONFIG_USE_PATH9  _T("CONFIG_USE_PATH9")
#define CONFIG_USE_PATH10  _T("CONFIG_USE_PATH10")
#define CONFIG_USE_PATH11  _T("CONFIG_USE_PATH11")
#define CONFIG_USE_PATH12  _T("CONFIG_USE_PATH12")

class MenuConfig {

private:
	int id; 
	CString name;
	int mainParentIndex;
	int ParentIndex;
};
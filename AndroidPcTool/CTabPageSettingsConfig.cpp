// CTabPageSettingsConfig.cpp: 实现文件
//

#include "pch.h"
#include "AndroidPcTool.h"
#include "afxdialogex.h"
#include "CTabPageSettingsConfig.h"


// CTabPageSettingsConfig 对话框

IMPLEMENT_DYNAMIC(CTabPageSettingsConfig, CDialogEx)

CTabPageSettingsConfig::CTabPageSettingsConfig(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SETTINGS_CONFIG, pParent)
{

}

CTabPageSettingsConfig::~CTabPageSettingsConfig()
{
}

void CTabPageSettingsConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabPageSettingsConfig, CDialogEx)
END_MESSAGE_MAP()


// CTabPageSettingsConfig 消息处理程序

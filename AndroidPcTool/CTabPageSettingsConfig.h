#pragma once
#include "afxdialogex.h"


// CTabPageSettingsConfig 对话框

class CTabPageSettingsConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CTabPageSettingsConfig)

public:
	CTabPageSettingsConfig(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTabPageSettingsConfig();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETTINGS_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

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
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ConfigKeyList;
	CComboBox m_ConfigTypeList;
	CStatic m_TypeFrom;
	afx_msg void OnBnClickedButtonReadBuildProp();
	CStringA doread(CString type);
	CStringA doWrite(CString type, CString value);
	afx_msg void OnCbnSelchangeComboConfigKetList();
	afx_msg void OnBnClickedButtonWriterProp();
	CEdit m_Value;
};

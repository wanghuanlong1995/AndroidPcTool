#pragma once
#include "afxdialogex.h"


// CTabPageBatterySet 对话框

class CTabPageBatterySet : public CDialogEx
{
	DECLARE_DYNAMIC(CTabPageBatterySet)

public:
	CTabPageBatterySet(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTabPageBatterySet();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BATTERY_SETTINGS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	// 电量值
	CString batteryValue;
	afx_msg void OnBnClickedButtonBatterySet();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonBatteryReset();
	afx_msg void OnBnClickedButtonBatteryGet();
};

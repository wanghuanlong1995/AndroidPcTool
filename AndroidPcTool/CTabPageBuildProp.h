#pragma once
#include "afxdialogex.h"

// CTabPageBuildProp 对话框

class CTabPageBuildProp : public CDialogEx
{
	DECLARE_DYNAMIC(CTabPageBuildProp)

public:
	CTabPageBuildProp(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTabPageBuildProp();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BUILD_PROP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_buildPropList;
	CString propValue;
	afx_msg void OnBnClickedButtonReadBuildProp();
	afx_msg void OnCbnSelchangeComboBuildPropList();
};

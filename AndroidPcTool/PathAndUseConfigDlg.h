#pragma once
#include "afxdialogex.h"


// PathAndUseConfigDlg 对话框

class PathAndUseConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PathAndUseConfigDlg)

public:
	PathAndUseConfigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PathAndUseConfigDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN_PATH_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

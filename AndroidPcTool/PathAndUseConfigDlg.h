#pragma once
#include "afxdialogex.h"
#include"ConfigSet.h"

// PathAndUseConfigDlg 对话框

class PathAndUseConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PathAndUseConfigDlg)

public:
	PathAndUseConfigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PathAndUseConfigDlg();
	CWinApp* pApp;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN_PATH_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpenWeb(UINT nID);
	afx_msg void buttonSelectDir(UINT nID);
	afx_msg void openDir(UINT nID);
	// 扩展工具路径
	CString pathEx;
	// 截图保存路径
	CString pathShot;
	// 日志保存路径
	CString pathLog;
	// 是否可以登录外网
	BOOL canLoginNet;
	afx_msg void OnBnClickedCheckLogin();
	CString path1;
	CString path2;
	CString path3;
	CString path4;
	CString path5;
	CString path6;
	CString path7;
	CString path8;
	CString path9;
	CString path10;
	CString path11;
	CString path12;
};

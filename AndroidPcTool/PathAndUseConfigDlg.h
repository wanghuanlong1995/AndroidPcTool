#pragma once
#include "afxdialogex.h"
#include "ConfigSet.h"

// PathAndUseConfigDlg 对话框

class PathAndUseConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PathAndUseConfigDlg)

public:
	PathAndUseConfigDlg(CWnd* pParent = nullptr);
	virtual ~PathAndUseConfigDlg();
	CWinApp* pApp;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN_PATH_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpenWeb(UINT nID);
	afx_msg void OnBnClickedCheckLogin();

	// paths[0]=扩展工具路径, paths[1]=截图路径, paths[2]=日志路径, paths[3..14]=常用目录1~12
	CString paths[TOTAL_PATH_COUNT];
	BOOL canLoginNet;


    afx_msg void buttonSelectDir(int index);
    afx_msg void openDir(int index);
};

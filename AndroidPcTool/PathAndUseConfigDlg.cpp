// PathAndUseConfigDlg.cpp: 实现文件
//

#include "pch.h"
#include "AndroidPcTool.h"
#include "afxdialogex.h"
#include "PathAndUseConfigDlg.h"


// PathAndUseConfigDlg 对话框

IMPLEMENT_DYNAMIC(PathAndUseConfigDlg, CDialogEx)

PathAndUseConfigDlg::PathAndUseConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MAIN_PATH_CONFIG, pParent)
{

}

PathAndUseConfigDlg::~PathAndUseConfigDlg()
{
}

void PathAndUseConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PathAndUseConfigDlg, CDialogEx)
END_MESSAGE_MAP()


// PathAndUseConfigDlg 消息处理程序

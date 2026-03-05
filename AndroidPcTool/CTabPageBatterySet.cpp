// CTabPageBatterySet.cpp: 实现文件
//

#include "pch.h"
#include "AndroidPcTool.h"
#include "afxdialogex.h"
#include "CTabPageBatterySet.h"
#include "AndroidPcToolDlg.h"

// CTabPageBatterySet 对话框

IMPLEMENT_DYNAMIC(CTabPageBatterySet, CDialogEx)

CTabPageBatterySet::CTabPageBatterySet(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BATTERY_SETTINGS, pParent)
	, batteryValue(_T(""))
{

}

CTabPageBatterySet::~CTabPageBatterySet()
{
}

void CTabPageBatterySet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BATTERY_VALUE, batteryValue);
}


BEGIN_MESSAGE_MAP(CTabPageBatterySet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BATTERY_SET, &CTabPageBatterySet::OnBnClickedButtonBatterySet)
	ON_BN_CLICKED(IDC_BUTTON3, &CTabPageBatterySet::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_BATTERY_RESET, &CTabPageBatterySet::OnBnClickedButtonBatteryReset)
	ON_BN_CLICKED(IDC_BUTTON_BATTERY_GET, &CTabPageBatterySet::OnBnClickedButtonBatteryGet)
END_MESSAGE_MAP()


// CTabPageBatterySet 消息处理程序

void CTabPageBatterySet::OnBnClickedButtonBatterySet()
{
	UpdateData(TRUE);
	AndroidPcToolDlg* dlg = (AndroidPcToolDlg*)(GetParent()->GetParent());
	CString cmd("adb shell dumpsys battery set level ");
	cmd.Append(batteryValue);
	dlg->cmdAndShowEdit(CStringA(cmd));
}

void CTabPageBatterySet::OnBnClickedButton3()
{
	AndroidPcToolDlg* dlg = (AndroidPcToolDlg*)(GetParent()->GetParent());
	CString cmd("adb shell dumpsys battery unplug");
	dlg->cmdAndShowEdit(CStringA(cmd));
}

void CTabPageBatterySet::OnBnClickedButtonBatteryReset()
{
	AndroidPcToolDlg* dlg = (AndroidPcToolDlg*)(GetParent()->GetParent());
	CString cmd("adb shell dumpsys battery reset");
	dlg->cmdAndShowEdit(CStringA(cmd));
}

void CTabPageBatterySet::OnBnClickedButtonBatteryGet()
{
	AndroidPcToolDlg* dlg = (AndroidPcToolDlg*)(GetParent()->GetParent());
	CString cmd("adb shell dumpsys battery get level");
	batteryValue = dlg->cmdAndShowEdit(CStringA(cmd));
	UpdateData(FALSE);
}

// CTabPageBatterySet.cpp: 实现文件
//

#include "pch.h"
#include "AndroidPcTool.h"
#include "afxdialogex.h"
#include "CTabPageBatterySet.h"
#include "AndroidPcToolDlg.h"

namespace {
	AndroidPcToolDlg* dlg;
}

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

BOOL  CTabPageBatterySet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	dlg = (AndroidPcToolDlg*)(GetParent()->GetParent());
	return TRUE;
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
	CString cmd("adb shell dumpsys battery set level ");
	cmd.Append(batteryValue);
	dlg->cmdAndShowEdit(CStringA(cmd));
}

void CTabPageBatterySet::OnBnClickedButton3()
{
	dlg->cmdAndShowEdit("adb shell dumpsys battery unplug");
}

void CTabPageBatterySet::OnBnClickedButtonBatteryReset()
{
	dlg->cmdAndShowEdit("adb shell dumpsys battery reset");
}

void CTabPageBatterySet::OnBnClickedButtonBatteryGet()
{
	batteryValue = dlg->cmdAndShowEdit("adb shell dumpsys battery get level");
	UpdateData(FALSE);
}

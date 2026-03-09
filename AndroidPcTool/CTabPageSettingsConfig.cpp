// CTabPageSettingsConfig.cpp: 实现文件
//

#include "pch.h"
#include "AndroidPcTool.h"
#include "afxdialogex.h"
#include "CTabPageSettingsConfig.h"
#include "AndroidPcToolDlg.h"

namespace {
	AndroidPcToolDlg* dlg;
}

// CTabPageSettingsConfig 对话框

IMPLEMENT_DYNAMIC(CTabPageSettingsConfig, CDialogEx)

CTabPageSettingsConfig::CTabPageSettingsConfig(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SETTINGS_CONFIG, pParent)
{

}

CTabPageSettingsConfig::~CTabPageSettingsConfig()
{
}

void CTabPageSettingsConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CONFIG_KET_LIST, m_ConfigKeyList);
	DDX_Control(pDX, IDC_COMBO_CONFIG_TYPE_LIST, m_ConfigTypeList);
	DDX_Control(pDX, IDC_STATIC_TYPE_FROM, m_TypeFrom);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_Value);
}


BEGIN_MESSAGE_MAP(CTabPageSettingsConfig, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_READ_BUILD_PROP, &CTabPageSettingsConfig::OnBnClickedButtonReadBuildProp)
	ON_CBN_SELCHANGE(IDC_COMBO_CONFIG_KET_LIST, &CTabPageSettingsConfig::OnCbnSelchangeComboConfigKetList)
	ON_BN_CLICKED(IDC_BUTTON_WRITER_PROP, &CTabPageSettingsConfig::OnBnClickedButtonWriterProp)
END_MESSAGE_MAP()


BOOL  CTabPageSettingsConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	CString iniPath;
	::GetModuleFileName(nullptr, iniPath.GetBuffer(MAX_PATH), MAX_PATH);
	iniPath.ReleaseBuffer();
	iniPath = iniPath.Left(iniPath.ReverseFind('\\') + 1) + _T("AndroidPcToolSettings.ini");

	// 2. 初始化配置读取器
	IniConfigReader configReader(iniPath);

	// 3. 填充BuildProp ComboBox
	std::vector<CString> buildPropItems;

	if (configReader.GetSectionItems(_T("SettingsConfig"), buildPropItems))
	{
		m_ConfigKeyList.ResetContent();
		for (const auto& item : buildPropItems)
		{
			// 跳过以#开头的注释行
			if (!item.IsEmpty() && item[0] != _T('#'))
			{
				m_ConfigKeyList.AddString(item);
			}
		}
	}

	m_ConfigKeyList.AddString(L"device_name");
	m_ConfigKeyList.AddString(L"keyboard_three_finger_quick_swipe_up");


	m_ConfigKeyList.SetCurSel(0);

	m_ConfigTypeList.AddString(L"all");
	m_ConfigTypeList.AddString(L"system");
	m_ConfigTypeList.AddString(L"global");
	m_ConfigTypeList.AddString(L"secure");
	m_ConfigTypeList.SetCurSel(0);

	m_TypeFrom.SetWindowTextW(L"");

	dlg = (AndroidPcToolDlg*)(GetParent()->GetParent());
	return TRUE;
}

// CTabPageSettingsConfig 消息处理程序

void CTabPageSettingsConfig::OnBnClickedButtonReadBuildProp()
{

	if (m_ConfigTypeList.GetCurSel() != 0) {
		CString str;
		m_ConfigTypeList.GetWindowTextW(str);
		doread(str);
		return;
	}

	CStringA res = doread(L"system");
	if (!res.IsEmpty() && res.Find("Invalid") == -1 && res.Find("null") == -1 && (res.GetBuffer() != nullptr)) {
		return;
	}

	res = doread(L"global");
	if (!res.IsEmpty() && res.Find("Invalid") == -1 && res.Find("null") == -1 && (res.GetBuffer() != nullptr)) {
		return;
	}

	res = doread(L"secure");
}

CStringA CTabPageSettingsConfig::doread(CString type)
{
	CString cmd("adb shell settings get ");
	cmd.Append(type);
	m_TypeFrom.SetWindowTextW(type);
	cmd.Append(L" ");

	CString str;
	m_ConfigKeyList.GetLBText(m_ConfigKeyList.GetCurSel(), str);
	cmd.Append(str);
	CStringA res = dlg->cmdAndShowEdit(CStringA(cmd));
	m_Value.SetWindowTextW(CString(res));
	return res;
}


CStringA CTabPageSettingsConfig::doWrite(CString type, CString value)
{
	CString cmd("adb shell settings put ");
	cmd.Append(type);
	m_TypeFrom.SetWindowTextW(type);
	cmd.Append(L" ");

	CString str;
	m_ConfigKeyList.GetWindowTextW(str);
	cmd.Append(str);
	cmd.Append(L" ");
	cmd.Append(value);
	return dlg->cmdAndShowEdit(CStringA(cmd));
}

void CTabPageSettingsConfig::OnCbnSelchangeComboConfigKetList()
{
	OnBnClickedButtonReadBuildProp();
}

void CTabPageSettingsConfig::OnBnClickedButtonWriterProp()
{
	if (m_ConfigTypeList.GetCurSel() == 0) {
		MessageBoxA(GetSafeHwnd(), "写入需要选择其他类型不能选择all", "类型错误", MB_ICONWARNING | MB_OK);
		return;
	}
	CString type;
	m_ConfigTypeList.GetWindowTextW(type);
	CString value;
	m_Value.GetWindowTextW(value);
	doWrite(type, value);
}

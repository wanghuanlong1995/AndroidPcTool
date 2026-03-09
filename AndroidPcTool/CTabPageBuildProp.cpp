// CTabPageBuildProp.cpp: 实现文件
//

#include "pch.h"
#include "AndroidPcTool.h"
#include "afxdialogex.h"
#include "CTabPageBuildProp.h"
#include "AndroidPcToolDlg.h"


// CTabPageBuildProp 对话框

IMPLEMENT_DYNAMIC(CTabPageBuildProp, CDialogEx)

namespace {
	AndroidPcToolDlg* dlg;
}

CTabPageBuildProp::CTabPageBuildProp(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BUILD_PROP, pParent)
	, propValue(_T(""))
{
}

CTabPageBuildProp::~CTabPageBuildProp()
{
}

void CTabPageBuildProp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BUILD_PROP_LIST, m_buildPropList);
	DDX_Text(pDX, IDC_EDIT_BUILD_PROP_VALUE, propValue);

}

 BOOL  CTabPageBuildProp::OnInitDialog()
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

	 if (configReader.GetSectionItems(_T("AndroidConfig"), buildPropItems))
	 {
		 m_buildPropList.ResetContent();
		 for (const auto& item : buildPropItems)
		 {
			 // 跳过以#开头的注释行
			 if (!item.IsEmpty() && item[0] != _T('#'))
			 {
				 m_buildPropList.AddString(item);
			 }
		 }
	 }



	 m_buildPropList.AddString(L"ro.build.version.incremental");
	 m_buildPropList.AddString(L"Build.BRAND");
	 m_buildPropList.AddString(L"persist.sys.usb.config");
	 m_buildPropList.AddString(L"ro.boot.hwboardid");
	 m_buildPropList.AddString(L"persist.debug.dalvik.vm.jdwp.enabled");
	 m_buildPropList.SetCurSel(3);

	 dlg = (AndroidPcToolDlg*)(GetParent()->GetParent());
	 return TRUE;
}


BEGIN_MESSAGE_MAP(CTabPageBuildProp, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_READ_BUILD_PROP, &CTabPageBuildProp::OnBnClickedButtonReadBuildProp)
	ON_CBN_SELCHANGE(IDC_COMBO_BUILD_PROP_LIST, &CTabPageBuildProp::OnCbnSelchangeComboBuildPropList)
END_MESSAGE_MAP()


// CTabPageBuildProp 消息处理程序

void CTabPageBuildProp::OnBnClickedButtonReadBuildProp()
{
	UpdateData(TRUE);
	CString strText;
	m_buildPropList.GetLBText(m_buildPropList.GetCurSel(), strText);
	CString cmd ("adb shell getprop ");
	cmd.Append(strText);
	propValue = dlg->cmdAndShowEdit(CStringA(cmd));
	UpdateData(FALSE);
}

void CTabPageBuildProp::OnCbnSelchangeComboBuildPropList()
{
	OnBnClickedButtonReadBuildProp();
}

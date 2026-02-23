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
    , pathEx(_T(""))
    , pathShot(_T(""))
    , pathLog(_T(""))
    , canLoginNet(FALSE)
    , path1(_T(""))
    , path2(_T(""))
    , path3(_T(""))
    , path4(_T(""))
    , path5(_T(""))
    , path6(_T(""))
    , path7(_T(""))
    , path8(_T(""))
    , path9(_T(""))
    , path10(_T(""))
    , path11(_T(""))
    , path12(_T(""))
{
	pApp = AfxGetApp();

    if (!pApp) {
        MessageBoxA(NULL, "配置", "配置初始化失败", MB_OK);
    }

    canLoginNet = pApp->GetProfileInt(_T("Settings"), CONFIG_CAN_LOGIN_NET, TRUE);

    pathEx = pApp->GetProfileString(_T("Settings"), CONFIG_TOOL_EX_PATH, _T(""));
    pathShot = pApp->GetProfileString(_T("Settings"), CONFIG_SHOT_PIC_PATH, _T(""));
    pathLog = pApp->GetProfileString(_T("Settings"), CONFIG_LOG_PATH, _T(""));
    pathLog = pApp->GetProfileString(_T("Settings"), CONFIG_USE_PATH1, _T(""));
    pathLog = pApp->GetProfileString(_T("Settings"), CONFIG_USE_PATH2, _T(""));
    pathLog = pApp->GetProfileString(_T("Settings"), CONFIG_USE_PATH3, _T(""));
    pathLog = pApp->GetProfileString(_T("Settings"), CONFIG_USE_PATH4, _T(""));
    pathLog = pApp->GetProfileString(_T("Settings"), CONFIG_USE_PATH5, _T(""));
    pathLog = pApp->GetProfileString(_T("Settings"), CONFIG_USE_PATH6, _T(""));
    pathLog = pApp->GetProfileString(_T("Settings"), CONFIG_USE_PATH7, _T(""));
    pathLog = pApp->GetProfileString(_T("Settings"), CONFIG_USE_PATH8, _T(""));
    pathLog = pApp->GetProfileString(_T("Settings"), CONFIG_USE_PATH9, _T(""));
    pathLog = pApp->GetProfileString(_T("Settings"), CONFIG_USE_PATH10, _T(""));
    pathLog = pApp->GetProfileString(_T("Settings"), CONFIG_USE_PATH11, _T(""));
    pathLog = pApp->GetProfileString(_T("Settings"), CONFIG_USE_PATH12, _T(""));
}

PathAndUseConfigDlg::~PathAndUseConfigDlg()
{
}

void PathAndUseConfigDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_EX, pathEx);
    DDX_Text(pDX, IDC_EDIT_SHOT_PATH, pathShot);
    DDX_Text(pDX, IDC_EDIT_LOG, pathLog);
    DDX_Check(pDX, IDC_CHECK_LOGIN, canLoginNet);
    DDX_Text(pDX, IDC_EDIT1, path1);
    DDX_Text(pDX, IDC_EDIT2, path2);
    DDX_Text(pDX, IDC_EDIT3, path3);
    DDX_Text(pDX, IDC_EDIT4, path4);
    DDX_Text(pDX, IDC_EDIT5, path5);
    DDX_Text(pDX, IDC_EDIT6, path6);
    DDX_Text(pDX, IDC_EDIT7, path7);
    DDX_Text(pDX, IDC_EDIT8, path8);
    DDX_Text(pDX, IDC_EDIT9, path9);
    DDX_Text(pDX, IDC_EDIT10, path10);
    DDX_Text(pDX, IDC_EDIT11, path11);
    DDX_Text(pDX, IDC_EDIT12, path12);
    DDX_Text(pDX, IDC_EDIT1, path1);
  
}


BEGIN_MESSAGE_MAP(PathAndUseConfigDlg, CDialogEx)
    ON_BN_CLICKED(IDC_CHECK_LOGIN, &PathAndUseConfigDlg::OnBnClickedCheckLogin)
    ON_COMMAND_RANGE(10, 52815, &PathAndUseConfigDlg::OnOpenWeb)
END_MESSAGE_MAP()


void PathAndUseConfigDlg::OnOpenWeb(UINT nID)
{
    switch (nID)
    {
    case IDC_BUTTON_SELECT_DIR_EX:
    case IDC_BUTTON_SELECT_DIR_SHOT:
    case IDC_BUTTON_SELECT_DIR_LOG:
    case IDC_BUTTON_SELECT_DIR_1:
    case IDC_BUTTON_SELECT_DIR_2:
    case IDC_BUTTON_SELECT_DIR_3:
    case IDC_BUTTON_SELECT_DIR_4:
    case IDC_BUTTON_SELECT_DIR_5:
    case IDC_BUTTON_SELECT_DIR_6:
    case IDC_BUTTON_SELECT_DIR_7:
    case IDC_BUTTON_SELECT_DIR_8:
    case IDC_BUTTON_SELECT_DIR_9:
    case IDC_BUTTON_SELECT_DIR_10:
    case IDC_BUTTON_SELECT_DIR_11:
    case IDC_BUTTON_SELECT_DIR_12:
        buttonSelectDir(nID);
        break;

    case IDC_BUTTON_OPEN_EX:
    case IDC_BUTTON_OPEN_SHOT:
    case IDC_BUTTON_OPEN_LOG:
    case IDC_BUTTON_OPEN_1:
    case IDC_BUTTON_OPEN_2:
    case IDC_BUTTON_OPEN_3:
    case IDC_BUTTON_OPEN_4:
    case IDC_BUTTON_OPEN_5:
    case IDC_BUTTON_OPEN_6:
    case IDC_BUTTON_OPEN_7:
    case IDC_BUTTON_OPEN_8:
    case IDC_BUTTON_OPEN_9:
    case IDC_BUTTON_OPEN_10:
    case IDC_BUTTON_OPEN_11:
    case IDC_BUTTON_OPEN_12:
        openDir(nID);
        break;
    default:
        break;
    }
}

// 弹出目录选择对话框，返回选中的目录路径
CString selectDirectoryByFileDialog()
{
    
    CString strSelectedDir;
    // 1. 配置对话框参数
    CFileDialog dlg(
        TRUE,                      // TRUE=打开对话框，FALSE=保存对话框
        NULL,                      // 默认文件扩展名
        NULL,                      // 默认文件名
        OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
        _T("所有文件夹 (*.*)|*.*||"),  // 过滤规则，仅显示文件夹
        AfxGetMainWnd()            // 父窗口指针
    );

    // 2. 隐藏文件列表，强制仅显示目录
    dlg.m_ofn.lpstrTitle = _T("选择目标目录"); // 设置对话框标题
    dlg.m_ofn.lpstrFilter = _T("文件夹|*\0\0"); // 仅显示文件夹

    // 3. 弹出对话框并处理结果
    if (dlg.DoModal() == IDOK)
    {
        // 获取选中的目录路径
        strSelectedDir = dlg.GetPathName();
        // 移除可能的文件名后缀（如果误选了文件）
        PathRemoveFileSpec(strSelectedDir.GetBuffer(MAX_PATH));
        strSelectedDir.ReleaseBuffer();
    }

    return strSelectedDir;
}

#include <shobjidl.h> // 包含现代对话框头文件
#pragma comment(lib, "ole32.lib") // 链接OLE32库

// 弹出现代风格目录选择对话框
CString SelectDirectoryByModernDialog()
{
    CString strSelectedDir;
    IFileOpenDialog* pDialog = NULL;

    // 1. 初始化COM环境
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) return strSelectedDir;

    // 2. 创建对话框实例
    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, (void**)&pDialog);
    if (SUCCEEDED(hr))
    {
        // 3. 配置为目录选择模式
        DWORD dwOptions;
        pDialog->GetOptions(&dwOptions);
        pDialog->SetOptions(dwOptions | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);

        // 4. 弹出对话框
        hr = pDialog->Show(AfxGetMainWnd()->GetSafeHwnd());
        if (SUCCEEDED(hr))
        {
            // 5. 获取选中的目录
            IShellItem* pItem = NULL;
            hr = pDialog->GetResult(&pItem);
            if (SUCCEEDED(hr))
            {
                PWSTR pszPath = NULL;
                pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
                strSelectedDir = CString(pszPath);
                CoTaskMemFree(pszPath);
                pItem->Release();
            }
        }
        pDialog->Release();
    }

    // 6. 释放COM环境
    CoUninitialize();
    return strSelectedDir;
}


void PathAndUseConfigDlg::buttonSelectDir(UINT nID)
{
    CString path = SelectDirectoryByModernDialog();
    if (path.IsEmpty()) {
        return;
    }

    CString pathkey;
    switch (nID)
    {
    case IDC_BUTTON_SELECT_DIR_EX:
        pathEx = path;
        pathkey = CONFIG_TOOL_EX_PATH;
        break;
    case IDC_BUTTON_SELECT_DIR_SHOT:
        pathShot = path;
        pathkey = CONFIG_SHOT_PIC_PATH;
        break;
    case IDC_BUTTON_SELECT_DIR_LOG:
        pathLog = path;
        pathkey = CONFIG_LOG_PATH;
        break;
    case IDC_BUTTON_SELECT_DIR_1:
        pathLog = path;
        pathkey = CONFIG_USE_PATH1;
        break;
    case IDC_BUTTON_SELECT_DIR_2:
        pathLog = path;
        pathkey = CONFIG_USE_PATH2;
        break;
    case IDC_BUTTON_SELECT_DIR_3:
        pathLog = path;
        pathkey = CONFIG_USE_PATH3;
        break;
    case IDC_BUTTON_SELECT_DIR_4:
        pathLog = path;
        pathkey = CONFIG_USE_PATH4;
        break;
    case IDC_BUTTON_SELECT_DIR_5:
        pathLog = path;
        pathkey = CONFIG_USE_PATH5;
        break;
    case IDC_BUTTON_SELECT_DIR_6:
        pathLog = path;
        pathkey = CONFIG_USE_PATH6;
        break;
    case IDC_BUTTON_SELECT_DIR_7:
        pathLog = path;
        pathkey = CONFIG_LOG_PATH;
        break;
    case IDC_BUTTON_SELECT_DIR_8:
        pathLog = path;
        pathkey = CONFIG_LOG_PATH;
        break;
    case IDC_BUTTON_SELECT_DIR_9:
        pathLog = path;
        pathkey = CONFIG_LOG_PATH;
        break;
    case IDC_BUTTON_SELECT_DIR_10:
        pathLog = path;
        pathkey = CONFIG_LOG_PATH;
        break;
    case IDC_BUTTON_SELECT_DIR_11:
        pathLog = path;
        pathkey = CONFIG_LOG_PATH;
        break;
    case IDC_BUTTON_SELECT_DIR_12:
        pathLog = path;
        pathkey = CONFIG_LOG_PATH;
        break;
    default:
        break;
    }

    pApp->WriteProfileString(_T("Settings"), pathkey, path);
    UpdateData(false);
}

void PathAndUseConfigDlg::openDir(UINT nID)
{
    UpdateData(false);
    CString path;
    switch (nID)
    {
    case IDC_BUTTON_OPEN_EX:
        path = CONFIG_TOOL_EX_PATH;
        break;
    case IDC_BUTTON_OPEN_SHOT:
        path = CONFIG_SHOT_PIC_PATH;
        break;
    case IDC_BUTTON_OPEN_LOG:
        path = CONFIG_LOG_PATH;
        break;
    default:
        return;
        break;
    }

    ShellExecute(NULL, L"open", pApp->GetProfileString(_T("Settings"), path, _T("")), L"", L"", SW_SHOWNORMAL);
}

void PathAndUseConfigDlg::OnBnClickedCheckLogin()
{
    UpdateData(true);
    pApp->WriteProfileInt(_T("Settings"), CONFIG_CAN_LOGIN_NET, canLoginNet);  
}

// PathAndUseConfigDlg.cpp: 实现文件
//

#include "pch.h"
#include "AndroidPcTool.h"
#include "afxdialogex.h"
#include "PathAndUseConfigDlg.h"

// 是否可以上外网
#define CONFIG_CAN_LOGIN_NET _T("CONFIG_CAN_LOGIN_NET")
#include <shobjidl.h>

#pragma comment(lib, "ole32.lib")

// PathAndUseConfigDlg 对话框

IMPLEMENT_DYNAMIC(PathAndUseConfigDlg, CDialogEx)

// 路径配置映射表：selectBtnId, openBtnId, editId, configKey
struct PathConfig {
    UINT selectBtnId;
    UINT openBtnId;
    UINT editId;
    LPCTSTR configKey;
};

static const PathConfig PATH_CONFIGS[TOTAL_PATH_COUNT] = {
    { IDC_BUTTON_SELECT_DIR_EX,   IDC_BUTTON_OPEN_EX,   IDC_EDIT_EX,        CONFIG_TOOL_EX_PATH  },
    { IDC_BUTTON_SELECT_DIR_SHOT, IDC_BUTTON_OPEN_SHOT, IDC_EDIT_SHOT_PATH, CONFIG_SHOT_PIC_PATH },
    { IDC_BUTTON_SELECT_DIR_LOG,  IDC_BUTTON_OPEN_LOG,  IDC_EDIT_LOG,       CONFIG_LOG_PATH      },
    { IDC_BUTTON_SELECT_DIR_1,    IDC_BUTTON_OPEN_1,    IDC_EDIT1,          CONFIG_USE_PATH1     },
    { IDC_BUTTON_SELECT_DIR_2,    IDC_BUTTON_OPEN_2,    IDC_EDIT2,          CONFIG_USE_PATH2     },
    { IDC_BUTTON_SELECT_DIR_3,    IDC_BUTTON_OPEN_3,    IDC_EDIT3,          CONFIG_USE_PATH3     },
    { IDC_BUTTON_SELECT_DIR_4,    IDC_BUTTON_OPEN_4,    IDC_EDIT4,          CONFIG_USE_PATH4     },
    { IDC_BUTTON_SELECT_DIR_5,    IDC_BUTTON_OPEN_5,    IDC_EDIT5,          CONFIG_USE_PATH5     },
    { IDC_BUTTON_SELECT_DIR_6,    IDC_BUTTON_OPEN_6,    IDC_EDIT6,          CONFIG_USE_PATH6     },
    { IDC_BUTTON_SELECT_DIR_7,    IDC_BUTTON_OPEN_7,    IDC_EDIT7,          CONFIG_USE_PATH7     },
    { IDC_BUTTON_SELECT_DIR_8,    IDC_BUTTON_OPEN_8,    IDC_EDIT8,          CONFIG_USE_PATH8     },
    { IDC_BUTTON_SELECT_DIR_9,    IDC_BUTTON_OPEN_9,    IDC_EDIT9,          CONFIG_USE_PATH9     },
    { IDC_BUTTON_SELECT_DIR_10,   IDC_BUTTON_OPEN_10,   IDC_EDIT10,         CONFIG_USE_PATH10    },
    { IDC_BUTTON_SELECT_DIR_11,   IDC_BUTTON_OPEN_11,   IDC_EDIT11,         CONFIG_USE_PATH11    },
    { IDC_BUTTON_SELECT_DIR_12,   IDC_BUTTON_OPEN_12,   IDC_EDIT12,         CONFIG_USE_PATH12    },
};

PathAndUseConfigDlg::PathAndUseConfigDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_MAIN_PATH_CONFIG, pParent)
    , canLoginNet(FALSE)
{
    pApp = AfxGetApp();
    //if (!pApp) {
    //    MessageBoxA(NULL, "配置", "配置初始化失败", MB_OK);
    //    return;
    //}

    canLoginNet = pApp->GetProfileInt(_T("Settings"), CONFIG_CAN_LOGIN_NET, TRUE);
    for (int i = 0; i < TOTAL_PATH_COUNT; i++) {
        paths[i] = pApp->GetProfileString(_T("Settings"), PATH_CONFIGS[i].configKey, _T(""));
    }
}

PathAndUseConfigDlg::~PathAndUseConfigDlg()
{
}

void PathAndUseConfigDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_CHECK_LOGIN, canLoginNet);
    for (int i = 0; i < TOTAL_PATH_COUNT; i++) {
        DDX_Text(pDX, PATH_CONFIGS[i].editId, paths[i]);
    }
}


BEGIN_MESSAGE_MAP(PathAndUseConfigDlg, CDialogEx)
    ON_BN_CLICKED(IDC_CHECK_LOGIN, &PathAndUseConfigDlg::OnBnClickedCheckLogin)
    ON_COMMAND_RANGE(10, 52815, &PathAndUseConfigDlg::OnOpenWeb)
END_MESSAGE_MAP()


void PathAndUseConfigDlg::OnOpenWeb(UINT nID)
{
    for (int i = 0; i < TOTAL_PATH_COUNT; i++) {
        if (nID == PATH_CONFIGS[i].selectBtnId) {
            buttonSelectDir(i);
            return;
        }
        if (nID == PATH_CONFIGS[i].openBtnId) {
            openDir(i);
            return;
        }
    }
}

// 弹出现代风格目录选择对话框
static CString SelectDirectoryByModernDialog()
{
    CString strSelectedDir;
    IFileOpenDialog* pDialog = NULL;

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) return strSelectedDir;

    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, (void**)&pDialog);
    if (SUCCEEDED(hr))
    {
        DWORD dwOptions;
        pDialog->GetOptions(&dwOptions);
        pDialog->SetOptions(dwOptions | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);

        hr = pDialog->Show(AfxGetMainWnd()->GetSafeHwnd());
        if (SUCCEEDED(hr))
        {
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

    CoUninitialize();
    return strSelectedDir;
}


void PathAndUseConfigDlg::buttonSelectDir(int index)
{
    CString path = SelectDirectoryByModernDialog();
    if (path.IsEmpty()) return;

    paths[index] = path;
    pApp->WriteProfileString(_T("Settings"), PATH_CONFIGS[index].configKey, path);
    UpdateData(FALSE);
}

void PathAndUseConfigDlg::openDir(int index)
{
    UpdateData(TRUE);
    CString path = paths[index];
    if (path.IsEmpty()) return;

    ShellExecute(NULL, _T("open"), path, _T(""), _T(""), SW_SHOWNORMAL);
}

void PathAndUseConfigDlg::OnBnClickedCheckLogin()
{
    UpdateData(TRUE);
    pApp->WriteProfileInt(_T("Settings"), CONFIG_CAN_LOGIN_NET, canLoginNet);
}

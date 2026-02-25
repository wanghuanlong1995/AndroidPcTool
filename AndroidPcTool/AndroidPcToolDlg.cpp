
// AndroidPcToolDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "AndroidPcTool.h"
#include "AndroidPcToolDlg.h"
#include "PathAndUseConfigDlg.h"
#include "afxdialogex.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cctype>
#include <sstream>
#include <Shlwapi.h>
#include <urlmon.h>
#include <iphlpapi.h>
#include <wincrypt.h>

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "advapi32.lib")

static const LPCTSTR USE_PATH_KEYS[USE_PATH_COUNT] = {
        CONFIG_USE_PATH1,
        CONFIG_USE_PATH2,
        CONFIG_USE_PATH3,
        CONFIG_USE_PATH4,
        CONFIG_USE_PATH5,
        CONFIG_USE_PATH6,
        CONFIG_USE_PATH7,
        CONFIG_USE_PATH8,
        CONFIG_USE_PATH9,
        CONFIG_USE_PATH10,
        CONFIG_USE_PATH11,
        CONFIG_USE_PATH12,
};

struct ConfigPickItem {
    LPCTSTR displayName;
    LPCTSTR key;
    bool isInt;
};

static const ConfigPickItem CONFIG_PICK_ITEMS[] = {
        { _T("是否可登录外网账号"), CONFIG_CAN_LOGIN_NET, true },
        { _T("扩展工具路径"), CONFIG_TOOL_EX_PATH, false },
        { _T("导出截屏路径"), CONFIG_SHOT_PIC_PATH, false },
        { _T("导出日志路径"), CONFIG_LOG_PATH, false },
        { _T("常用目录1路径"), CONFIG_USE_PATH1, false },
        { _T("常用目录2路径"), CONFIG_USE_PATH2, false },
        { _T("常用目录3路径"), CONFIG_USE_PATH3, false },
        { _T("常用目录4路径"), CONFIG_USE_PATH4, false },
        { _T("常用目录5路径"), CONFIG_USE_PATH5, false },
        { _T("常用目录6路径"), CONFIG_USE_PATH6, false },
        { _T("常用目录7路径"), CONFIG_USE_PATH7, false },
        { _T("常用目录8路径"), CONFIG_USE_PATH8, false },
        { _T("常用目录9路径"), CONFIG_USE_PATH9, false },
        { _T("常用目录10路径"), CONFIG_USE_PATH10, false },
        { _T("常用目录11路径"), CONFIG_USE_PATH11, false },
        { _T("常用目录12路径"), CONFIG_USE_PATH12, false },
};

static constexpr int CONFIG_PICK_COUNT = (int)(sizeof(CONFIG_PICK_ITEMS) / sizeof(CONFIG_PICK_ITEMS[0]));

static int FindMenuItemPosById(CMenu* menu, UINT id)
{
    if (!menu) return -1;
    const int count = menu->GetMenuItemCount();
    for (int i = 0; i < count; i++) {
        if (menu->GetMenuItemID(i) == id) return i;
    }
    return -1;
}

static CMenu* FindSubMenuContainingId(CMenu* mainMenu, UINT id)
{
    if (!mainMenu) return nullptr;
    const int count = mainMenu->GetMenuItemCount();
    for (int i = 0; i < count; i++) {
        CMenu* sub = mainMenu->GetSubMenu(i);
        if (!sub) continue;
        if (FindMenuItemPosById(sub, id) >= 0) return sub;
    }
    return nullptr;
}

static CString ExtractVersionFromCaption(const CString& caption)
{
    int vPos = caption.ReverseFind(_T('v'));
    if (vPos < 0) vPos = caption.ReverseFind(_T('V'));
    if (vPos < 0) return CString();

    CString ver = caption.Mid(vPos + 1);
    ver.Trim();
    // 截断到第一个空格（如果有）
    int spacePos = ver.Find(_T(' '));
    if (spacePos >= 0) ver = ver.Left(spacePos);
    return ver;
}

static bool TryParseSemver(const CString& ver, std::vector<int>& parts)
{
    parts.clear();
    CString v = ver;
    v.Trim();
    if (v.IsEmpty()) return false;
    if (v[0] == _T('v') || v[0] == _T('V')) v = v.Mid(1);

    int pos = 0;
    CString token = v.Tokenize(_T("."), pos);
    while (!token.IsEmpty()) {
        token.Trim();
        for (int i = 0; i < token.GetLength(); i++) {
            if (!_istdigit(token[i])) return false;
        }
        parts.push_back(_ttoi(token));
        token = v.Tokenize(_T("."), pos);
    }
    return !parts.empty();
}

static int CompareSemver(const CString& a, const CString& b)
{
    std::vector<int> pa, pb;
    if (!TryParseSemver(a, pa) || !TryParseSemver(b, pb)) return 0;
    const size_t n = max(pa.size(), pb.size());
    pa.resize(n, 0);
    pb.resize(n, 0);
    for (size_t i = 0; i < n; i++) {
        if (pa[i] < pb[i]) return -1;
        if (pa[i] > pb[i]) return 1;
    }
    return 0;
}

static std::string ExtractJsonStringValue(const std::string& json, const char* key)
{
    // 极简 JSON 字符串字段提取：匹配 "key":"value"
    // 只用于 release 信息的少量字段，避免引入 JSON 依赖
    std::string needle = "\"";
    needle += key;
    needle += "\"";
    size_t p = json.find(needle);
    if (p == std::string::npos) return {};
    p = json.find(':', p);
    if (p == std::string::npos) return {};
    p = json.find('"', p);
    if (p == std::string::npos) return {};
    size_t start = p + 1;
    size_t end = start;
    while (end < json.size()) {
        if (json[end] == '\\\\') { end += 2; continue; }
        if (json[end] == '"') break;
        end++;
    }
    if (end <= start || end >= json.size()) return {};
    return json.substr(start, end - start);
}

static CString GetEnvVarW(const wchar_t* name)
{
    DWORD needed = GetEnvironmentVariableW(name, nullptr, 0);
    if (needed == 0) return CString();
    std::wstring buf;
    buf.resize(needed);
    //DWORD got = GetEnvironmentVariableW(name, buf.data(), needed);
    //if (got == 0) return CString();
    // got 不含终止符
    return CString(buf.c_str());
}

static CString TodayYmd()
{
    SYSTEMTIME st{};
    GetLocalTime(&st);
    CString s;
    s.Format(_T("%04d%02d%02d"), st.wYear, st.wMonth, st.wDay);
    return s;
}

static bool TryParseYmd(const CString& ymd, SYSTEMTIME& out)
{
    if (ymd.GetLength() != 8) return false;
    for (int i = 0; i < 8; i++) if (!_istdigit(ymd[i])) return false;
    out = {};
    out.wYear = (WORD)_ttoi(ymd.Mid(0, 4));
    out.wMonth = (WORD)_ttoi(ymd.Mid(4, 2));
    out.wDay = (WORD)_ttoi(ymd.Mid(6, 2));
    return out.wYear > 1970 && out.wMonth >= 1 && out.wMonth <= 12 && out.wDay >= 1 && out.wDay <= 31;
}

static int DaysInMonth(int year, int month)
{
    static const int days[]{ 31,28,31,30,31,30,31,31,30,31,30,31 };
    int d = days[month - 1];
    bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (month == 2 && leap) d++;
    return d;
}

static SYSTEMTIME AddMonthsClamped(const SYSTEMTIME& in, int months)
{
    int y = in.wYear;
    int m = in.wMonth + months;
    while (m > 12) { m -= 12; y++; }
    while (m < 1) { m += 12; y--; }
    int d = min(in.wDay, DaysInMonth(y, m));
    SYSTEMTIME out = in;
    out.wYear = (WORD)y;
    out.wMonth = (WORD)m;
    out.wDay = (WORD)d;
    return out;
}

static COleDateTime ToOleDate(const SYSTEMTIME& st)
{
    return COleDateTime(st.wYear, st.wMonth, st.wDay, 0, 0, 0);
}

static std::string Md5HexLower(const std::string& data)
{
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    BYTE hash[16]{};
    DWORD hashLen = sizeof(hash);

    if (!CryptAcquireContextW(&hProv, nullptr, nullptr, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) return {};
    if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) { CryptReleaseContext(hProv, 0); return {}; }

    CryptHashData(hHash, (const BYTE*)data.data(), (DWORD)data.size(), 0);
    if (!CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashLen, 0)) {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return {};
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    static const char* hex = "0123456789abcdef";
    std::string out;
    out.resize(32);
    for (int i = 0; i < 16; i++) {
        out[i * 2] = hex[(hash[i] >> 4) & 0xF];
        out[i * 2 + 1] = hex[hash[i] & 0xF];
    }
    return out;
}

static std::string GetPrimaryMacHex()
{
    ULONG size = 0;
    GetAdaptersInfo(nullptr, &size);
    if (size == 0) return {};

    std::vector<BYTE> buf(size);
    PIP_ADAPTER_INFO pInfo = reinterpret_cast<PIP_ADAPTER_INFO>(buf.data());
    if (GetAdaptersInfo(pInfo, &size) != NO_ERROR) return {};

    for (PIP_ADAPTER_INFO p = pInfo; p; p = p->Next) {
        if (p->AddressLength != 6) continue;
        // 跳过全 0 的地址
        bool allZero = true;
        for (UINT i = 0; i < p->AddressLength; i++) if (p->Address[i] != 0) { allZero = false; break; }
        if (allZero) continue;

        char mac[13]{};
        sprintf_s(mac, "%02X%02X%02X%02X%02X%02X",
                  p->Address[0], p->Address[1], p->Address[2], p->Address[3], p->Address[4], p->Address[5]);
        return mac;
    }
    return {};
}

static CString GetMachineCode()
{
    const std::string macHex = GetPrimaryMacHex();
    if (macHex.empty()) return CString();
    const std::string md5 = Md5HexLower(macHex);
    return CString(md5.c_str());
}

static bool CopyToClipboardText(HWND hwnd, const CString& text)
{
    if (!OpenClipboard(hwnd)) return false;
    EmptyClipboard();

    const size_t bytes = (text.GetLength() + 1) * sizeof(wchar_t);
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, bytes);
    if (!hMem) { CloseClipboard(); return false; }

    void* p = GlobalLock(hMem);
    memcpy(p, (LPCWSTR)text, bytes);
    GlobalUnlock(hMem);

    SetClipboardData(CF_UNICODETEXT, hMem);
    CloseClipboard();
    return true;
}

static CString ReadClipboardText(HWND hwnd)
{
    CString text;
    if (!OpenClipboard(hwnd)) return text;
    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (!hData) { CloseClipboard(); return text; }
    LPCWSTR p = (LPCWSTR)GlobalLock(hData);
    if (p) {
        text = p;
        GlobalUnlock(hData);
    }
    CloseClipboard();
    return text;
}


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// AndroidPcToolDlg 对话框



AndroidPcToolDlg::AndroidPcToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ANDROIDPCTOOL_DIALOG, pParent)
	, m_isTopSelft(FALSE)
	, m_isAutoOpenPullDir(TRUE)
	, m_editShowResut(_T(""))
	, m_isScrcpyTop(FALSE)
	, m_editInputPath(_T(""))
	, m_isAutoInstallApk(FALSE)
	, m_deviceDIr(_T(""))
	, m_StringMd5(_T(""))
	, m_MinNoTaskShow(TRUE)
	, m_nid()
	, m_install_g(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	pApp = AfxGetApp();
}

void AndroidPcToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_TOP_SELF, m_isTopSelft);
	DDX_Check(pDX, IDC_CHECK_AUTO_OPEN_DIR, m_isAutoOpenPullDir);
	DDX_Text(pDX, IDC_EDIT_SHOW_RESULT, m_editShowResut);
	DDX_Check(pDX, IDC_CHECK_SCE_CPY_TOP, m_isScrcpyTop);
	DDX_Control(pDX, IDC_RADIO_COMMON_LOG, m_radionCommonLogs);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_dragInputEdit);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_editInputPath);
	DDX_Check(pDX, IDC_CHECK_IS_AUTO_INSTALL, m_isAutoInstallApk);
	DDX_Control(pDX, IDC_COMBO_DEVICE_DIR, m_comboBoxDeviceDir);
	DDX_CBString(pDX, IDC_COMBO_DEVICE_DIR, m_deviceDIr);
	DDX_Text(pDX, IDC_STATIC_FILE_MD5, m_StringMd5);
	DDX_Check(pDX, IDC_CHECK_MIN_NO_TASK_SHOW, m_MinNoTaskShow);
	DDX_Check(pDX, IDC_CHECK_INSTALL_G, m_install_g);
}

BEGIN_MESSAGE_MAP(AndroidPcToolDlg, CDialogEx)
    ON_MESSAGE(WM_USER + 1, OnTrayIcon)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, &AndroidPcToolDlg::OnToolTipNotify)
	ON_BN_CLICKED(IDC_CHECK_TOP_SELF, &AndroidPcToolDlg::OnBnClickedCheckTopSelft)
	ON_BN_CLICKED(IDC_BUTTON_TOP_PATH, &AndroidPcToolDlg::OnBnClickedButtonTopPath)
	ON_BN_CLICKED(IDC_CHECK_SCE_CPY_TOP, &AndroidPcToolDlg::OnBnClickedCheckScecpyTop)
	ON_BN_CLICKED(IDC_BUTTON_TOP_APK_VERSION, &AndroidPcToolDlg::OnBnClickedButtonTopApkVersion)
	ON_BN_CLICKED(IDC_MFC_MENUBUTTON_PULL_LOG, &AndroidPcToolDlg::OnBnClickedMfcmenubuttonPullLog)
	ON_COMMAND(ID_CONIF_PATH_AND_USE, &AndroidPcToolDlg::OnConifPathAndUse)
	ON_BN_CLICKED(IDC_BUTTON_PULL_TOP_APK, &AndroidPcToolDlg::OnBnClickedButtonPullTopApk)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_APP, &AndroidPcToolDlg::OnBnClickedButtonClearApp)
	ON_BN_CLICKED(IDC_BUTTON_INSTALL_APK, &AndroidPcToolDlg::OnBnClickedButtonInstallApk)
	ON_BN_CLICKED(IDC_BUTTON_ENTER_SETTINGS, &AndroidPcToolDlg::OnBnClickedButtonApkInSettings)
	ON_BN_CLICKED(IDC_BUTTON_PUSH, &AndroidPcToolDlg::OnBnClickedButtonPush)
	ON_BN_CLICKED(IDC_BUTTON_LS, &AndroidPcToolDlg::OnBnClickedButtonLs)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE_DIR, &AndroidPcToolDlg::OnCbnSelchangeComboDeviceDir)
	ON_COMMAND_RANGE(10, 52815,&AndroidPcToolDlg::OnOpenWeb)
	ON_BN_CLICKED(IDC_BUTTON_PULL, &AndroidPcToolDlg::OnBnClickedButtonPull)
END_MESSAGE_MAP()


// AndroidPcToolDlg 消息处理程序

BOOL AndroidPcToolDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }


    // 获取菜单指针
    CMenu * pMenu = GetMenu();
    if (pMenu)
    {
        // 暂时移除联想的相关显示
        pMenu->RemoveMenu(6, MF_BYPOSITION);

        // 运行时补齐/修正菜单（避免直接改 .rc 造成资源编码问题）
        // 0) “配置(&C)”里补齐“获取配置项/导出配置”
        {
            CMenu* configMenu = FindSubMenuContainingId(pMenu, ID_CONIF_PATH_AND_USE);
            if (configMenu) {
                if (FindMenuItemPosById(configMenu, ID_CONFIG_GET_ITEM) < 0) {
                    const int posCfg = FindMenuItemPosById(configMenu, ID_CONIF_PATH_AND_USE);
                    const int insertPos = (posCfg >= 0) ? (posCfg + 1) : configMenu->GetMenuItemCount();
                    configMenu->InsertMenu(insertPos, MF_BYPOSITION | MF_STRING, ID_CONFIG_GET_ITEM, _T("获取配置项"));
                    configMenu->InsertMenu(insertPos + 1, MF_BYPOSITION | MF_STRING, ID_CONFIG_EXPORT_ALL, _T("导出配置"));
                }
            }
        }

        // 1) “代码仓(&R)”里新增 AOSP
        {
            CMenu* repoMenu = FindSubMenuContainingId(pMenu, ID_GITHUP);
            if (repoMenu && FindMenuItemPosById(repoMenu, ID_AOSP) < 0) {
                const int posHF = FindMenuItemPosById(repoMenu, ID_HF);
                const int insertPos = (posHF >= 0) ? (posHF + 1) : repoMenu->GetMenuItemCount();
                repoMenu->InsertMenu(insertPos, MF_BYPOSITION | MF_STRING, ID_AOSP, _T("AOSP"));
            }
        }

        // 2) “帮助(&H)”里把“版本更新”命令修正为 ID_VERSION_UPDATE（避免与打赏共用 ID）
        {
            CMenu* helpMenu = FindSubMenuContainingId(pMenu, ID_ABOUT);
            if (helpMenu) {
                for (int i = 0; i < helpMenu->GetMenuItemCount(); i++) {
                    CString text;
                    helpMenu->GetMenuString(i, text, MF_BYPOSITION);
                    text.Trim();

                    if (text == _T("版本更新")) {
                        helpMenu->ModifyMenu(i, MF_BYPOSITION | MF_STRING, ID_VERSION_UPDATE, _T("版本更新"));
                    }
                    else if (text == _T("打赏")) {
                        helpMenu->ModifyMenu(i, MF_BYPOSITION | MF_STRING, ID_REWARD, _T("打赏"));
                    }
                }

                // 新增“激活”入口（试用到期后使用）
                if (FindMenuItemPosById(helpMenu, ID_ACTIVATE) < 0) {
                    helpMenu->InsertMenu(1, MF_BYPOSITION | MF_STRING, ID_ACTIVATE, _T("激活"));
                }
            }
        }

        DrawMenuBar();
    }

    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    // 托盘相关
    ZeroMemory(&m_nid, sizeof(NOTIFYICONDATA));
    m_nid.cbSize = sizeof(NOTIFYICONDATA);
    m_nid.hWnd = GetSafeHwnd();
    m_nid.uID = 1;
    m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    m_nid.uCallbackMessage = WM_USER + 1;  // 自定义消息
    m_nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    wcscpy_s(m_nid.szTip, L"AndroidPcTool");// 提示文本
    Shell_NotifyIcon(NIM_ADD, &m_nid);

    // 托盘右键菜单
    m_trayMenu.LoadMenu(IDR_TRAY_MENU);
    m_trayCommonPathsMenu.CreatePopupMenu();

    // 控件初始化
    m_radionCommonLogs.SetCheck(TRUE);

    m_comboBoxDeviceDir.InsertString(0, L"system/app/HwLauncher6");
    m_comboBoxDeviceDir.InsertString(0, L"system/app/Bluetooth");
    m_comboBoxDeviceDir.InsertString(0, L"system/framework");
    m_comboBoxDeviceDir.InsertString(0, L"system/priv-app/SystemUI");
    m_comboBoxDeviceDir.InsertString(0, L"system/priv-app/TeleService");
    m_comboBoxDeviceDir.InsertString(0, L"system/app/HwLauncher3");
    m_comboBoxDeviceDir.InsertString(0, L"system/priv-app/Settings");
    m_comboBoxDeviceDir.InsertString(0, L"system/priv-app/ZuiSettings");
    m_comboBoxDeviceDir.InsertString(0, L"system/priv-app/ZuiWallpaperSetting");
    m_comboBoxDeviceDir.InsertString(0, L"sdcard/Pictures/screenrecorder");
    m_comboBoxDeviceDir.InsertString(0, L"sdcard/debuglogger");
    m_comboBoxDeviceDir.InsertString(0, L"sdcard/log");

    m_comboBoxDeviceDir.SetCurSel(0);


    // 初始化工具提示控件
    m_tooltip.Create(this);
    // 设置提示样式：气泡样式 + 总是显示
    m_tooltip.SetMaxTipWidth(SHRT_MAX);
    m_tooltip.Activate(TRUE);

    // 为具体控件添加提示（控件ID和提示文本对应）
    m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_TOP_ACTIVITY), L"adb shell dumpsys \"activity top | grep ACTIVITY | tail -n 1\"");
    m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_TOP_APK_VERSION), L"adb shell dumpsys package [packageName] | findstr version");
    m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_ENTER_SETTINGS), L"adb shell am start -a android.settings.APPLICATION_DETAILS_SETTINGS -d package: [packageName]");
    //m_tooltip.AddTool(GetDlgItem(IDC_EDIT1), L"请输入用户名");

     // 试用期 & 激活状态
    {
    	bool activated = false;
    	const CString machineCode = GetMachineCode();
    	const CString actTime = pApp->GetProfileString(_T("Settings"), CONFIG_ACTIVATION_TIME, _T(""));
    	const CString actCode = pApp->GetProfileString(_T("Settings"), CONFIG_ACTIVATION_CODE, _T(""));

    	if (!machineCode.IsEmpty() && !actTime.IsEmpty() && !actCode.IsEmpty()) {
    		const std::string expected = Md5HexLower(std::string(CStringA(machineCode + actTime).GetString()));
    		if (!expected.empty() && actCode.CompareNoCase(CString(expected.c_str())) == 0) {
    			activated = true;
    		}
    	}

    	CString caption;
    	GetWindowText(caption);
    	const int suffixPos = caption.Find(_T(" ("));
    	if (suffixPos >= 0) caption = caption.Left(suffixPos);

    	if (activated) {
    		SetWindowText(caption + _T(" (已激活)"));
    	}
    	else {
    		CString firstRun = pApp->GetProfileString(_T("Settings"), CONFIG_FIRST_RUN_DATE, _T(""));
    		if (firstRun.IsEmpty()) {
    			firstRun = TodayYmd();
    			pApp->WriteProfileString(_T("Settings"), CONFIG_FIRST_RUN_DATE, firstRun);
    		}

    		SYSTEMTIME stFirst{};
    		if (!TryParseYmd(firstRun, stFirst)) {
    			stFirst = {};
    			GetLocalTime(&stFirst);
    			firstRun = TodayYmd();
    			pApp->WriteProfileString(_T("Settings"), CONFIG_FIRST_RUN_DATE, firstRun);
    		}

    		const SYSTEMTIME stEnd = AddMonthsClamped(stFirst, 6);
    		COleDateTime dtNow = COleDateTime::GetCurrentTime();
    		COleDateTime dtEnd = ToOleDate(stEnd);
    		COleDateTimeSpan span = dtEnd - dtNow;
    		const int daysLeft = (int)span.GetTotalDays();

    		if (daysLeft >= 0) {
    			CString s;
    			s.Format(_T(" (试用剩余%d天)"), daysLeft);
    			SetWindowText(caption + s);
    		}
    		else {
    			SetWindowText(caption + _T(" (试用已到期，请在“帮助->激活”中激活)"));

    			// 到期后禁用主要功能（保留菜单“激活”入口）
    			static const UINT kDisableIds[] = {
    				IDC_BUTTON_SHOT_AND_PULL,
    				IDC_BUTTON_OPEN_SCR_CPY,
    				IDC_BUTTON_TOP_ACTIVITY,
    				IDC_BUTTON_TOP_PATH,
    				IDC_BUTTON_TOP_APK_VERSION,
    				IDC_BUTTON_PULL_TOP_APK,
    				IDC_BUTTON_CLEAR_APP,
    				IDC_BUTTON_ENTER_SETTINGS,
    				IDC_MFCMENUBUTTON_KILL_ABD,
    				IDC_MFCMENUBUTTON_KILL_JAVA,
    				IDC_BUTTON_UI_VIEWER,
    				IDC_BUTTON_PX_COOK,
    				IDC_BUTTON_GIF,
    				IDC_BUTTON_FAN_HUN_XIAO,
    				IDC_BUTTON_OPEN_JADX,
    				IDC_BUTTON_INSTALL_APK,
    				IDC_BUTTON_PULL,
    				IDC_BUTTON_PUSH,
    				IDC_BUTTON_LS,
    				IDC_MFC_MENUBUTTON_PULL_LOG,
    				IDC_BUTTON_DEL_LOG,
    			};
    			for (UINT id : kDisableIds) {
    				if (CWnd* w = GetDlgItem(id)) w->EnableWindow(FALSE);
    			}

    			MessageBox(_T("试用期已到期，请在菜单“帮助->激活”完成激活。"), _T("提示"), MB_OK | MB_ICONWARNING);
    		}
    	}
    }

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void AndroidPcToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    UpdateData(TRUE);
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else if (nID == SC_MINIMIZE && lParam != 0)
    {
        ShowWindow(m_MinNoTaskShow ? SW_HIDE : SW_MINIMIZE);  // 隐藏窗口
        return;

    }
    else if (nID == SC_RESTORE)
    {
        ShowWindow(SW_SHOWNORMAL);  // 显示窗口
        ShowWindow(m_MinNoTaskShow ? SW_SHOW : SW_SHOWNORMAL);  // 显示窗口
        SetForegroundWindow();  // 激活窗口
        return;
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void AndroidPcToolDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR AndroidPcToolDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

LRESULT AndroidPcToolDlg::OnTrayIcon(WPARAM wParam, LPARAM lParam)
{
    if (lParam == WM_LBUTTONDOWN)
    {
        UpdateData(TRUE);
        ShowWindow(m_MinNoTaskShow ? SW_SHOW : SW_SHOWNORMAL);  // 显示窗口
        SetForegroundWindow();  // 激活窗口
    } else if (lParam == WM_RBUTTONDOWN)
    {
        CPoint pos;
        GetCursorPos(&pos);

        CMenu* pSubMenu = m_trayMenu.GetSubMenu(0);  // 获取子菜单
        if (pSubMenu)
        {
            // 设置菜单弹出位置，使其在前台显示
            SetForegroundWindow();

            // 运行时动态插入“常用目录打开”（在 APK 目录下面；空配置不显示）
            {
                // 先移除旧的“常用目录打开”入口（避免重复插入）
                for (int i = 0; i < pSubMenu->GetMenuItemCount(); i++) {
                    CString text;
                    pSubMenu->GetMenuString(i, text, MF_BYPOSITION);
                    if (text == _T("常用目录打开")) {
                        pSubMenu->RemoveMenu(i, MF_BYPOSITION);
                        break;
                    }
                }

                // 清空子菜单并按配置重新生成
                while (m_trayCommonPathsMenu.GetMenuItemCount() > 0) {
                    m_trayCommonPathsMenu.DeleteMenu(0, MF_BYPOSITION);
                }

                int added = 0;
                for (int i = 0; i < USE_PATH_COUNT; i++) {
                    const CString path = pApp->GetProfileString(_T("Settings"), USE_PATH_KEYS[i], _T(""));
                    if (path.IsEmpty()) continue;

                    LPCTSTR fileName = PathFindFileName(path);
                    CString itemText;
                    itemText.Format(_T("常用目录%d：%s"), i + 1, (fileName && *fileName) ? fileName : path.GetString());
                    m_trayCommonPathsMenu.AppendMenu(MF_STRING, ID_TRAY_COMMON_PATH_BASE + i, itemText);
                    added++;
                }

                if (added > 0) {
                    int insertPos = pSubMenu->GetMenuItemCount();
                    for (int i = 0; i < pSubMenu->GetMenuItemCount(); i++) {
                        if (pSubMenu->GetMenuItemID(i) == ID_DIR_APK) {
                            insertPos = i + 1;
                            break;
                        }
                    }
                    pSubMenu->InsertMenu(insertPos, MF_BYPOSITION | MF_POPUP, (UINT_PTR)m_trayCommonPathsMenu.GetSafeHmenu(), _T("常用目录打开"));
                }
            }

            pSubMenu->TrackPopupMenu(TPM_RIGHTALIGN | TPM_BOTTOMALIGN, pos.x + 70, pos.y - 20, this);
            PostMessage(WM_NULL);  // 释放菜单消息
        }
    }
    return 0;
}


void AndroidPcToolDlg::OnBnClickedCheckTopSelft()
{
    UpdateData(TRUE);
    // 设置窗口的是否置顶状态
    ::SetWindowPos(GetSafeHwnd(), m_isTopSelft ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}


void AndroidPcToolDlg::OnBnClickedCheckScecpyTop()
{
    HWND hwnd = FindWindowA("SDL_app", NULL);
    if (hwnd == NULL) {
        AfxMessageBox(_T("没找到投屏工具窗口，请检查是否已经启动投屏"));
        return;
    }

    UpdateData(TRUE);
    // 设置窗口的是否置顶状态
    ::SetWindowPos(hwnd, m_isScrcpyTop ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}


void AndroidPcToolDlg::openWeb(const char* url)
{
    ShellExecuteA(NULL, "open", url, "", "", SW_SHOWNORMAL);
}

void AndroidPcToolDlg::setViewHide(int viewId)
{
    GetDlgItem(viewId)->ShowWindow(SW_HIDE);
}

// 无控制台窗口执行命令，返回标准输出+标准错误
static std::string RunHiddenCommand(const char* cmd)
{
    std::string command = std::string("cmd.exe /c ") + cmd;

    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
    HANDLE hRead = NULL, hWrite = NULL;
    if (!CreatePipe(&hRead, &hWrite, &sa, 0))
        return {};
    SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si = {};
    si.cb          = sizeof(si);
    si.dwFlags     = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    si.hStdOutput  = hWrite;
    si.hStdError   = hWrite;

    PROCESS_INFORMATION pi = {};
    std::string result;
    if (CreateProcessA(NULL, const_cast<char*>(command.c_str()),
                       NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
    {
        CloseHandle(hWrite);
        char buf[1024];
        DWORD dwRead = 0;
        while (ReadFile(hRead, buf, sizeof(buf) - 1, &dwRead, NULL) && dwRead)
        {
            buf[dwRead] = '\0';
            result += buf;
        }
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else
    {
        CloseHandle(hWrite);
    }
    CloseHandle(hRead);
    return result;
}

CStringA AndroidPcToolDlg::cmdAndShowEdit(CStringA cmd, bool isNeedShowDefalutMsg)
{
    std::string output = RunHiddenCommand(cmd.GetString());
    m_editShowResut = output.empty() && isNeedShowDefalutMsg
                      ? L"请检查设备连接或者是否解锁"
                      : CString(output.c_str());
    UpdateData(FALSE);
    return output.c_str();
}

CStringA AndroidPcToolDlg::cmdAndShowTopApkEdit(CStringA cmd)
{
    return CStringA();
}


// 处理工具提示文本请求
BOOL AndroidPcToolDlg::OnToolTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
    TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMHDR;
    UINT_PTR nID = pNMHDR->idFrom;

    // 如果是控件ID（而非HWND），则手动设置文本
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        nID = ::GetDlgCtrlID((HWND)nID);
        if (nID)
        {
            // 根据控件ID匹配提示文本
            //switch (nID)
            //{
            //case IDC_BUTTON_TOP_ACTIVITY:
            //	pTTT->lpszText = (LPTSTR)L"adb shell dumpsys \"activity top | grep ACTIVITY | tail -n 1\"";
            //	break;
            //case IDC_EDIT1:
            //	pTTT->lpszText = (LPTSTR)L"请输入用户名";
            //	break;
            //default:
            //	pTTT->lpszText = L"未知控件";
            //}
            pTTT->hinst = AfxGetResourceHandle();
            return TRUE;
        }
    }
    return FALSE;
}

// 预处理消息，让工具提示捕获鼠标消息
BOOL AndroidPcToolDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_LBUTTONDOWN ||
        pMsg->message == WM_RBUTTONDOWN || pMsg->message == WM_MBUTTONDOWN)
    {
        m_tooltip.RelayEvent(pMsg);
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

void AndroidPcToolDlg::OnOpenWeb(UINT nID)
{
    // 托盘：常用目录打开（动态生成）
    if (nID >= ID_TRAY_COMMON_PATH_BASE && nID < (ID_TRAY_COMMON_PATH_BASE + USE_PATH_COUNT)) {
        const int idx = (int)(nID - ID_TRAY_COMMON_PATH_BASE);
        const CString path = pApp->GetProfileString(_T("Settings"), USE_PATH_KEYS[idx], _T(""));
        if (!path.IsEmpty()) {
            ShellExecute(NULL, _T("open"), path, _T(""), _T(""), SW_SHOWNORMAL);
        }
        return;
    }

    // “获取配置项”弹出的下拉选择（动态生成）
    if (nID >= ID_CONFIG_ITEM_BASE && nID < (ID_CONFIG_ITEM_BASE + CONFIG_PICK_COUNT + 1)) {
        const int idx = (int)(nID - ID_CONFIG_ITEM_BASE);
        CString key;
        CString display;
        bool isInt = false;

        if (idx == CONFIG_PICK_COUNT) {
            // 自定义：剪贴板里放配置 Key（如 CONFIG_SHOT_PIC_PATH）
            key = ReadClipboardText(GetSafeHwnd());
            key.Trim();
            display = _T("自定义");
        }
        else {
            key = CONFIG_PICK_ITEMS[idx].key;
            display = CONFIG_PICK_ITEMS[idx].displayName;
            isInt = CONFIG_PICK_ITEMS[idx].isInt;
        }

        if (key.IsEmpty()) {
            MessageBox(_T("未获取到配置 Key（请先把 Key 复制到剪贴板）。"), _T("获取配置项"), MB_OK | MB_ICONINFORMATION);
            return;
        }

        CString value;
        if (isInt) {
            value.Format(_T("%d"), pApp->GetProfileInt(_T("Settings"), key, 0));
        }
        else {
            value = pApp->GetProfileString(_T("Settings"), key, _T(""));
        }

        CString out;
        out.Format(_T("[%s]\nKey=%s\nValue=%s"), display.GetString(), key.GetString(), value.GetString());
        m_editShowResut = out;
        UpdateData(FALSE);

        CopyToClipboardText(GetSafeHwnd(), value);
        return;
    }

    // 根据不同的ID打开对应的网页
    switch (nID)
    {
        case ID_WX_CODE:
            // 打开微信探索页面
            openWeb("https://git.weixin.qq.com/explore");
            break;
        case ID_GITHUP:
            // 打开GitHub上的RNStudyNotes项目页面
            openWeb("https://github.com/crazycodeboy/RNStudyNotes");
            break;
        case ID_GITCODE:
            // 打开Gitcode首页
            openWeb("https://gitcode.com/");
            break;
        case ID_GITEE:
            // 打开Gitee首页
            openWeb("https://gitee.com/");
            break;
        case ID_UI_ALIBABA_PNG:
            // 打开阿里云图标库
            openWeb("https://www.iconfont.cn/collections/index?spm=a313x.7781069.1998910419.5&type=2");
            break;
        case ID_CAINIAO:
            // 打开菜鸟教程的Android高级编程页面
            openWeb("https://www.runoob.com/w3cnote_genre/android-advance");
            break;
        case ID_AI_GITCODE_DEEPSEEK:
            // 打开Gitcode的AI页面
            openWeb("https://gitcode.com/ai/?utm_source=sidebar");
            break;
        case ID_HF:
            // 打开HF镜像站点
            openWeb("https://hf-mirror.com/");
            break;
        case ID_AOSP:
            openWeb("https://cs.android.com/android/platform/superproject/main");
            break;
        case ID_HENLEYLEE:
            // 打开Henleylee的GitHub主页
            openWeb("https://henleylee.github.io/");
            break;
        case ID_TO_ICON:
            // 打开在线文件转换工具网站
            openWeb("https://convertio.co/zh/");
            break;
        case ID_PICTURE_COMPRESS:
            // 打开图片压缩工具网站
            openWeb("https://tinypng.com/");
            break;
        case ID_Android_CODE:
            // 打开Android源代码资源网站
            openWeb("https://www.androidos.net.cn/sourcecode");
            break;
        case ID_Mp4ToMp3:
            // 打开MP4转MP3工具网站
            openWeb("https://mp4tomp3.org/");
            break;
        case ID_LANHU_UI:
            // 打开兰湖设计协作平台网站
            openWeb("https://lanhuapp.com/dashboard/#/item");
            break;
        case ID_GET_PACK:
            // 打开获取打包资源教程博客
            openWeb("https://blog.csdn.net/c10WTiybQ1Ye3/article/details/78098632");
            break;
        case ID_AI_NAMI_SO:
            // 打开AI智能搜索工具网站
            openWeb("https://www.n.cn/?fromsou=1");
            break;
        case ID_AI_DOUBAO:
            // 打开AI豆包网站
            openWeb("https://www.doubao.com/chat/");
            break;
        case ID_AI_QIANWEN:
            // 打开AI千问网站
            openWeb("https://www.qianwen.com/?ch=webtongyi@sem_bdsempinzhuan");
            break;
        case ID_GIT_CODE_SELF:
            openWeb("https://gitcode.com/wanghuanlong/AndroidPcTool");
            break;
        case ID_VERSION_UPDATE:
            CheckForUpdateGitCode();
            break;
        case ID_ACTIVATE:
            ActivateByClipboard();
            break;
        case ID_WU_AI_PO_JIE:
            openWeb("https://www.52pojie.cn/");
            break;
        case ID_DIR_SHOT:
            ShellExecute(NULL, L"open", pApp->GetProfileString(_T("Settings"), CONFIG_SHOT_PIC_PATH, _T("")), L"", L"", SW_SHOWNORMAL);
            break;
        case ID_DIR_LOG:
            MessageBoxA(NULL, "请选日志保存目录", "提示", MB_OK);
            break;
        case ID_DIR_APK:
            MessageBoxA(NULL, "请选择APK保存目录", "提示", MB_OK);
            break;
        case IDC_BUTTON_TOP_ACTIVITY:
            // 显示当前活动的顶部信息
            cmdAndShowEdit("adb shell dumpsys \"activity top | grep ACTIVITY | tail -n 1\"", true);
            break;
        case IDC_BUTTON_OPEN_SCR_CPY:
            // 执行scrcpy脚本以显示设备屏幕
            ShellExecuteA(NULL, "open", "scrcpy-noconsole.vbs", "", "scrcpy-win64-v3.2", SW_SHOWNORMAL);
            break;
        case IDC_BUTTON_OPEN_JADX:
            // 打开JADX反编译工具
            ShellExecuteA(NULL, "open", "jadx.exe", "", "", SW_SHOWNORMAL);
            break;
        case IDC_BUTTON_OPNE_FSCapture:
            // 打开FSCapture屏幕捕捉工具
            ShellExecuteA(NULL, "open", "FSCapture.exe", "", "FSCapture", SW_SHOWNORMAL);
            break;
        case IDC_BUTTON_PX_COOK:
            // 打开PxCook
            ShellExecuteA(NULL, "open", "PxCook.exe", "", "PxCook", SW_SHOWNORMAL);
            break;
        case IDC_BUTTON_ADB_REBOOT:
            // 通过ADB命令重启设备
            ShellExecuteA(NULL, "open", "adb", "reboot", "", SW_HIDE);
            break;
        case IDC_BUTTON_REBOOT_P:
            // 通过ADB命令重启设备
            ShellExecuteA(NULL, "open", "adb", "reboot", "-p", SW_HIDE);
            break;
        case IDC_BUTTON_REBOOT_EDL:
            // 通过ADB命令重启设备
            ShellExecuteA(NULL, "open", "adb", "reboot", "edl", SW_HIDE);
            break;
        case IDC_BUTTON_FASTBOOT_REBOOT:
            // 通过Fastboot命令重启设备
            ShellExecuteA(NULL, "open", "fastboot", "reboot", "", SW_HIDE);
            break;
        case IDC_BUTTON_ROOT_REMOUNT:
            // 以root权限重新挂载设备文件系统
            cmdAndShowEdit("adb root && adb remount", true);
            break;
        case IDC_MFCMENUBUTTON_KILL_ABD:
            // 杀死ADB服务器进程和所有ADB客户端进程
            cmdAndShowEdit("adb kill-server && taskkill / F / IM adb.exe");
            break;
        case IDC_MFCMENUBUTTON_KILL_JAVA:
            // 杀死所有Java进程
            cmdAndShowEdit("taskkill / F / IM java.exe");
            break;
        case ID_OPEN_ENV:
            // 打开环境变量编辑界面
            ShellExecute(NULL, _T("open"), _T("rundll32.exe"), _T("sysdm.cpl,EditEnvironmentVariables"), NULL, SW_SHOWNORMAL);
            break;
        case ID_getIpconfig:
            cmdAndShowEdit("ipconfig -all");
            break;
        case ID_CONFIG_GET_ITEM:
            ShowConfigItemPicker();
            break;
        case ID_CONFIG_EXPORT_ALL:
            ExportAllSettingsToFile();
            break;
        case ID_ABOUT:
            CAboutDlg dlgAbout;
            dlgAbout.DoModal();
            dlgAbout.ShowWindow(SW_SHOW);
            break;
    }
}

// 提取包名
CString ExtractPackageName(const CString& output) {
    int nPos = 0;
    CString line, packageName;
    CString lastLine;

    while ((nPos = _ttoi(output.Tokenize(_T("\r\n"), nPos))) != -1) {
        line = output.Tokenize(_T("\r\n"), nPos);
        if (line.Find(_T("ACTIVITY")) != -1) {
            lastLine = line;
        }
    }

    if (!lastLine.IsEmpty()) {
        int start = lastLine.Find(_T(' ')) + 1;
        int end = lastLine.Find(_T('/'), start);
        if (start > 0 && end > start) {
            packageName = lastLine.Mid(start, end - start);
        }
    }
    return packageName;
}

// 提取安装路径
CStringA ExtractInstallPath(const CStringA& output) {
    int nPos = output.Find(_T(':'));
    if (nPos != -1) {
        return output.Mid(nPos + 1).Trim();
    }
    return ("");
}

CString getTopPackageName()
{
    std::string raw = RunHiddenCommand(
            "adb shell dumpsys \"activity top | grep ACTIVITY | tail -n 1\"");

    CString output(raw.c_str());
    int nStart = output.Find(_T("ACTIVITY"));
    if (nStart == -1) return L"";
    nStart += 9;
    int nSlash = output.Find(_T('/'), nStart);
    if (nSlash == -1) return L"";
    return output.Mid(nStart, nSlash - nStart);
}

CString AndroidPcToolDlg::getAndChekTopPackageName()
{
    // 执行命令获取当前置顶应用的包名
    CString packageName = getTopPackageName();
    if (packageName.IsEmpty()) {
        m_editShowResut = _T("无法获取包名");
        UpdateData(FALSE);
    }
    return packageName;

}

void AndroidPcToolDlg::OnBnClickedButtonTopPath()
{
    CString packageName = getAndChekTopPackageName();
    if (packageName.IsEmpty()) {
        return;
    }
    // 获取安装路径
    CString pathCmd;
    pathCmd.Format(L"adb shell pm path %s", packageName.GetString());
    CStringA pathOutput = cmdAndShowEdit(CStringA(pathCmd));

    // 解析路径
    if (!pathOutput.IsEmpty()) {
        m_editShowResut = CString(pathOutput);
    }
    else {
        m_editShowResut = _T("无法获取安装路径");
    }
    UpdateData(FALSE);
}

void AndroidPcToolDlg::OnBnClickedButtonTopApkVersion()
{
    CString packageName = getAndChekTopPackageName();
    if (packageName.IsEmpty()) {
        return;
    }

    // 执行命令获取应用的版本信息
    std::string command = "adb shell dumpsys package " + CStringA(packageName) + " | findstr version";
    cmdAndShowEdit(command.c_str(), true);
}

void AndroidPcToolDlg::OnBnClickedMfcmenubuttonPullLog()
{
    // 默认导出目录（在“路径配置”里配置的“导出日志路径”）
    const CString defaultPcDir = pApp ? pApp->GetProfileString(_T("Settings"), CONFIG_LOG_PATH, _T("")) : _T("");

    CFolderPickerDialog folderDlg(defaultPcDir);
    folderDlg.m_ofn.lpstrTitle = _T("选择日志另存为到的PC目录");
    if (folderDlg.DoModal() != IDOK) {
        return;
    }

    const CString pcDir = folderDlg.GetFolderPath();
    if (pcDir.IsEmpty()) return;

    const bool isCommon = (m_radionCommonLogs.GetCheck() == BST_CHECKED);

    // 常用日志：尽量只拉取常用目录（不依赖root）
    // 所有日志：在常用日志基础上，额外尝试拉取更多常见日志目录（可能因权限失败）
    std::vector<CStringA> deviceLogDirs;
    deviceLogDirs.emplace_back("sdcard/log");
    deviceLogDirs.emplace_back("sdcard/debuglogger");

    if (!isCommon) {
        deviceLogDirs.emplace_back("data/anr");
        deviceLogDirs.emplace_back("data/tombstones");
        deviceLogDirs.emplace_back("data/system/dropbox");
    }

    CStringA cmd = "cmd.exe /c ";
    for (size_t i = 0; i < deviceLogDirs.size(); i++) {
        cmd.AppendFormat("adb pull \"%s\" \"%s\"", deviceLogDirs[i].GetString(), CStringA(pcDir).GetString());
        if (i + 1 < deviceLogDirs.size()) cmd += " & ";
    }

    cmdAndShowEdit(cmd);

    if (m_isAutoOpenPullDir) {
        ShellExecute(NULL, _T("open"), pcDir, _T(""), _T(""), SW_SHOWNORMAL);
    }
}

void AndroidPcToolDlg::OnConifPathAndUse()
{
    PathAndUseConfigDlg dlg;
    dlg.DoModal();
}

void AndroidPcToolDlg::OnBnClickedButtonPullTopApk()
{
    OnBnClickedButtonTopPath();
    m_editShowResut.Trim();  // 去除首尾空白字符
    m_editShowResut.Replace(_T("\r\n"), _T(""));  // 去除换行符
    m_editShowResut.Replace(_T("package:"), _T(""));// 去除多余的前缀
    std::string command = "adb pull " + CStringA(m_editShowResut);
    cmdAndShowEdit(command.c_str());
}

void AndroidPcToolDlg::OnBnClickedButtonClearApp()
{
    CString packageName = getAndChekTopPackageName();
    if (packageName.IsEmpty()) {
        return;
    }
    std::string command = "adb shell pm clear " + CStringA(packageName);
    cmdAndShowEdit(command.c_str(), true);
}


void AndroidPcToolDlg::OnBnClickedButtonInstallApk()
{
    UpdateData(TRUE);

    std::string g_Str = "";
    if (m_install_g) {
        g_Str = " -g ";
    }

    std::string command = "adb install " + g_Str;
    command.append(CStringA(m_editInputPath));
    cmdAndShowEdit(command.c_str());
}

void AndroidPcToolDlg::setStringMd5()
{
    UpdateData(TRUE);
    std::string command = "powershell Get-FileHash -Path " + CStringA(m_editInputPath) + " -Algorithm MD5";
    cmdAndShowEdit(command.c_str());
}

void AndroidPcToolDlg::OnBnClickedButtonApkInSettings()
{
    CString packageName = getAndChekTopPackageName();
    if (packageName.IsEmpty()) {
        return;
    }
    std::string command = "adb shell am start -a android.settings.APPLICATION_DETAILS_SETTINGS -d package:" + CStringA(packageName);
    cmdAndShowEdit(command.c_str());
}


void AndroidPcToolDlg::OnBnClickedButtonPush()
{
    UpdateData(TRUE);
    std::string command = "adb push " + CStringA(m_editInputPath)+" "+ CStringA(m_deviceDIr);
    cmdAndShowEdit(command.c_str());
}


void AndroidPcToolDlg::OnBnClickedButtonLs()
{
    Sleep(300);
    UpdateData(TRUE);
    Sleep(100);
    std::string command = "adb shell ls -l "  + CStringA(m_deviceDIr);
    cmdAndShowEdit(command.c_str());
}

UINT upDataLs(LPVOID lParam) {
    Sleep(300);
    AndroidPcToolDlg* pWnd = (AndroidPcToolDlg*)lParam;
    pWnd->OnBnClickedButtonLs();
    return 0;
}

void AndroidPcToolDlg::OnCbnSelchangeComboDeviceDir()
{
    AfxBeginThread(upDataLs, (LPVOID)this);//启动新的线程去设置更新
}

void AndroidPcToolDlg::OnBnClickedButtonPull()
{
    UpdateData(TRUE);

    m_deviceDIr.Trim();
    if (m_deviceDIr.IsEmpty()) {
        MessageBox(_T("请先在设备目录下拉框中选择/输入要导出的设备路径"), _T("提示"), MB_OK | MB_ICONINFORMATION);
        return;
    }

    CFolderPickerDialog folderDlg;
    folderDlg.m_ofn.lpstrTitle = _T("选择导出到的PC目录");
    if (folderDlg.DoModal() != IDOK) {
        return;
    }

    const CString folderPath = folderDlg.GetFolderPath();
    if (folderPath.IsEmpty()) return;

    CStringA cmd;
    cmd.Format("adb pull \"%s\" \"%s\"", CStringA(m_deviceDIr).GetString(), CStringA(folderPath).GetString());
    cmdAndShowEdit(cmd);

    if (m_isAutoOpenPullDir) {
        ShellExecute(NULL, _T("open"), folderPath, _T(""), _T(""), SW_SHOWNORMAL);
    }
}

void AndroidPcToolDlg::CheckForUpdateGitCode()
{
    // GitCode API 需要 private-token：优先从环境变量获取，避免把 token 明文写入配置
    const CString token = GetEnvVarW(L"GITCODE_PRIVATE_TOKEN");
    if (token.IsEmpty()) {
        MessageBox(_T("检测更新需要设置环境变量 GITCODE_PRIVATE_TOKEN（GitCode Personal Access Token）。\n")
        _T("设置后重新打开工具再点击“版本更新”。"), _T("提示"), MB_OK | MB_ICONINFORMATION);
        return;
    }

    CString caption;
    GetWindowText(caption);
    const CString currentVer = ExtractVersionFromCaption(caption);

    CString tokenEsc = token;
    tokenEsc.Replace(_T("'"), _T("''")); // PowerShell 单引号转义

    const CStringA apiUrl = "https://gitcode.com/api/v5/repos/wanghuanlong/AndroidPcTool/releases?per_page=1&page=1";
    CStringA cmd;
    cmd.Format(
            "powershell -NoProfile -ExecutionPolicy Bypass -Command \""
            "$h=@{'private-token'='%s'}; "
            "$r=Invoke-RestMethod -Uri '%s' -Method Get -Headers $h -TimeoutSec 15; "
            "$r[0] | ConvertTo-Json -Depth 12 -Compress"
            "\"",
            CStringA(tokenEsc).GetString(),
            apiUrl.GetString()
    );

    const std::string out = RunHiddenCommand(cmd.GetString());
    if (out.empty()) {
        MessageBox(_T("检测更新失败：未获取到返回数据（请检查网络/Token 权限）。"), _T("提示"), MB_OK | MB_ICONWARNING);
        return;
    }

    const std::string latestTag = ExtractJsonStringValue(out, "tag_name");
    const std::string htmlUrl = ExtractJsonStringValue(out, "html_url");
    std::string downloadUrl = ExtractJsonStringValue(out, "browser_download_url");
    if (downloadUrl.empty()) downloadUrl = ExtractJsonStringValue(out, "download_url");

    if (latestTag.empty()) {
        m_editShowResut = CString(out.c_str());
        UpdateData(FALSE);
        MessageBox(_T("检测更新失败：无法解析最新版本信息（已将原始返回写入结果框）。"), _T("提示"), MB_OK | MB_ICONWARNING);
        return;
    }

    CString latestVer(latestTag.c_str());
    latestVer.Trim();
    if (!latestVer.IsEmpty() && (latestVer[0] == _T('v') || latestVer[0] == _T('V'))) {
        latestVer = latestVer.Mid(1);
    }

    const int cmp = CompareSemver(currentVer, latestVer);
    if (cmp >= 0) {
        CString msg;
        msg.Format(_T("当前版本：%s\n最新版本：%s\n已是最新版本。"),
                   currentVer.IsEmpty() ? _T("(未知)") : currentVer.GetString(),
                   latestVer.GetString());
        MessageBox(msg, _T("版本更新"), MB_OK | MB_ICONINFORMATION);
        return;
    }

    CString msg;
    msg.Format(_T("发现新版本：%s\n当前版本：%s\n\n是否下载更新？"),
               latestVer.GetString(),
               currentVer.IsEmpty() ? _T("(未知)") : currentVer.GetString());

    if (MessageBox(msg, _T("版本更新"), MB_YESNO | MB_ICONQUESTION) != IDYES) {
        if (!htmlUrl.empty()) {
            openWeb(htmlUrl.c_str());
        }
        return;
    }

    // 尝试下载资产；如果没有下载链接则打开发布页
    if (downloadUrl.empty()) {
        if (!htmlUrl.empty()) openWeb(htmlUrl.c_str());
        MessageBox(_T("未找到可下载的更新包链接，已为你打开发布页。"), _T("提示"), MB_OK | MB_ICONINFORMATION);
        return;
    }

    CStringW downloadUrlW(downloadUrl.c_str());
    WCHAR tmpDir[MAX_PATH] = { 0 };
    GetTempPathW(MAX_PATH, tmpDir);

    CStringW fileNameW = PathFindFileNameW(downloadUrlW);
    if (fileNameW.IsEmpty()) fileNameW = L"AndroidPcToolUpdate.exe";

    CStringW savePathW(tmpDir);
    if (savePathW.Right(1) != L"\\") savePathW += L"\\";
    savePathW += fileNameW;

    HRESULT hr = URLDownloadToFileW(NULL, downloadUrlW, savePathW, 0, NULL);
    if (SUCCEEDED(hr)) {
        CString done;
        done.Format(_T("已下载更新包到：\n%s\n\n是否打开所在文件夹？"), CString(savePathW));
        if (MessageBox(done, _T("版本更新"), MB_YESNO | MB_ICONINFORMATION) == IDYES) {
            ShellExecuteW(NULL, L"open", CStringW(tmpDir), L"", L"", SW_SHOWNORMAL);
        }
    }
    else {
        if (!htmlUrl.empty()) openWeb(htmlUrl.c_str());
        MessageBox(_T("下载失败，已为你打开发布页（可手动下载）。"), _T("提示"), MB_OK | MB_ICONWARNING);
    }
}

void AndroidPcToolDlg::ActivateByClipboard()
{
    const CString machineCode = GetMachineCode();
    if (machineCode.IsEmpty()) {
        MessageBox(_T("获取机器码失败：未找到有效网卡 MAC 地址。"), _T("激活"), MB_OK | MB_ICONWARNING);
        return;
    }

    const CString actTime = TodayYmd();
    const std::string expected = Md5HexLower(std::string(CStringA(machineCode + actTime).GetString()));
    if (expected.empty()) {
        MessageBox(_T("生成校验码失败。"), _T("激活"), MB_OK | MB_ICONWARNING);
        return;
    }

    // 复制“机器码|激活日期”方便发给生成激活码的一方
    CopyToClipboardText(GetSafeHwnd(), machineCode + _T("|") + actTime);

    CString tip;
    tip.Format(
            _T("机器码(=MAC的MD5)：\n%s\n\n激活日期：%s\n\n")
    _T("已将“机器码|激活日期”复制到剪贴板。\n")
    _T("请把激活码复制到剪贴板后，点击“确定”进行验证。"),
            machineCode.GetString(),
            actTime.GetString()
    );

    if (MessageBox(tip, _T("激活"), MB_OKCANCEL | MB_ICONINFORMATION) != IDOK) {
        return;
    }

    CString code = ReadClipboardText(GetSafeHwnd());
    code.Trim();
    code.MakeLower();
    if (code.IsEmpty()) {
        MessageBox(_T("剪贴板为空：请先复制激活码。"), _T("激活"), MB_OK | MB_ICONWARNING);
        return;
    }

    const CString expectedW(expected.c_str());
    if (code.CompareNoCase(expectedW) != 0) {
        CString msg;
        msg.Format(_T("激活码校验失败。\n\n期望：%s\n实际：%s"), expectedW.GetString(), code.GetString());
        MessageBox(msg, _T("激活"), MB_OK | MB_ICONERROR);
        return;
    }

    pApp->WriteProfileString(_T("Settings"), CONFIG_ACTIVATION_TIME, actTime);
    pApp->WriteProfileString(_T("Settings"), CONFIG_ACTIVATION_CODE, code);

    MessageBox(_T("激活成功！建议重启工具以确保所有功能恢复。"), _T("激活"), MB_OK | MB_ICONINFORMATION);
}

void AndroidPcToolDlg::ShowConfigItemPicker()
{
    CMenu menu;
    menu.CreatePopupMenu();

    for (int i = 0; i < CONFIG_PICK_COUNT; i++) {
        menu.AppendMenu(MF_STRING, ID_CONFIG_ITEM_BASE + i, CONFIG_PICK_ITEMS[i].displayName);
    }
    menu.AppendMenu(MF_SEPARATOR);
    menu.AppendMenu(MF_STRING, ID_CONFIG_ITEM_BASE + CONFIG_PICK_COUNT, _T("自定义(从剪贴板读取Key)"));

    CPoint pos;
    GetCursorPos(&pos);
    SetForegroundWindow();
    menu.TrackPopupMenu(TPM_RIGHTBUTTON, pos.x, pos.y, this);
    PostMessage(WM_NULL);
}

static std::string WideToUtf8(const CString& ws)
{
    if (ws.IsEmpty()) return {};
    int len = WideCharToMultiByte(CP_UTF8, 0, ws, ws.GetLength(), nullptr, 0, nullptr, nullptr);
    if (len <= 0) return {};
    std::string out;
    out.resize(len);
    //WideCharToMultiByte(CP_UTF8, 0, ws, ws.GetLength(), out.data(), len, nullptr, nullptr);
    return out;
}

void AndroidPcToolDlg::ExportAllSettingsToFile()
{
    CFileDialog dlg(FALSE, _T("ini"), _T("AndroidPcToolSettings.ini"),
                    OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
                    _T("INI 文件 (*.ini)|*.ini|文本文件 (*.txt)|*.txt||"),
                    this);

    if (dlg.DoModal() != IDOK) return;
    const CString path = dlg.GetPathName();
    if (path.IsEmpty()) return;

    CString content;
    content += _T("[Settings]\r\n");

    for (int i = 0; i < CONFIG_PICK_COUNT; i++) {
        CString value;
        if (CONFIG_PICK_ITEMS[i].isInt) {
            value.Format(_T("%d"), pApp->GetProfileInt(_T("Settings"), CONFIG_PICK_ITEMS[i].key, 0));
        }
        else {
            value = pApp->GetProfileString(_T("Settings"), CONFIG_PICK_ITEMS[i].key, _T(""));
        }
        content += CONFIG_PICK_ITEMS[i].key;
        content += _T("=");
        content += value;
        content += _T("\r\n");
    }

    const std::string utf8 = WideToUtf8(content);
    if (utf8.empty()) {
        MessageBox(_T("导出失败：编码转换失败。"), _T("导出配置"), MB_OK | MB_ICONWARNING);
        return;
    }

    try {
        CFile file(path, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
        const BYTE bom[3] = { 0xEF, 0xBB, 0xBF };
        file.Write(bom, 3);
        file.Write(utf8.data(), (UINT)utf8.size());
        file.Close();
    }
    catch (...) {
        MessageBox(_T("导出失败：无法写入文件。"), _T("导出配置"), MB_OK | MB_ICONWARNING);
        return;
    }

    MessageBox(_T("导出配置成功。"), _T("导出配置"), MB_OK | MB_ICONINFORMATION);
}

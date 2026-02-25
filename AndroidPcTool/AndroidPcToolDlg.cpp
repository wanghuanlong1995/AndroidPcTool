
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
	//
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

}

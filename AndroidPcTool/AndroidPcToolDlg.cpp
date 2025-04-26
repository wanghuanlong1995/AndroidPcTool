
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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void AndroidPcToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_TOP_SELFT, m_isTopSelft);
	DDX_Check(pDX, IDC_CHECK_AUTO_OPEN_DIR, m_isAutoOpenPullDir);
	DDX_Text(pDX, IDC_EDIT_SHOW_RESULT, m_editShowResut);
	DDX_Check(pDX, IDC_CHECK_SCECPY_TOP, m_isScrcpyTop);
	DDX_Control(pDX, IDC_RADIO_Common_Logs, m_radionCommonLogs);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_dragInputEdit);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_editInputPath);
	DDX_Check(pDX, IDC_CHECK_IS_AUTO_INSTALL, m_isAutoInstallApk);
	DDX_Control(pDX, IDC_COMBO_DEVICE_DIR, m_comboBoxDeviceDir);
	DDX_CBString(pDX, IDC_COMBO_DEVICE_DIR, m_deviceDIr);
}

BEGIN_MESSAGE_MAP(AndroidPcToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK_TOP_SELFT, &AndroidPcToolDlg::OnBnClickedCheckTopSelft)
	ON_COMMAND(ID_WX_CODE, &AndroidPcToolDlg::OnWxCode)
	ON_COMMAND(ID_GITHUP, &AndroidPcToolDlg::OnGithup)
	ON_COMMAND(ID_GITCODE, &AndroidPcToolDlg::OnGitcode)
	ON_COMMAND(ID_GITEE, &AndroidPcToolDlg::OnGitee)
	ON_COMMAND(ID_UI_ALIBABA_PNG, &AndroidPcToolDlg::OnUiAlibabaPng)
	ON_COMMAND(ID_CAINIAO, &AndroidPcToolDlg::OnCainiao)
	ON_COMMAND(ID_AI_GITCODE_DEEPSEEK, &AndroidPcToolDlg::OnAiGitcodeDeepseek)
	ON_COMMAND(ID_TO_ICON, &AndroidPcToolDlg::OnToIcon)
	ON_BN_CLICKED(IDC_BUTTON_TOP_ACTIVITY, &AndroidPcToolDlg::OnBnClickedButtonTopActivity)
	ON_BN_CLICKED(IDC_BUTTON_TOP_PATH, &AndroidPcToolDlg::OnBnClickedButtonTopPath)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_SCRCPY, &AndroidPcToolDlg::OnBnClickedButtonOpenScrcpy)
	ON_BN_CLICKED(IDC_CHECK_SCECPY_TOP, &AndroidPcToolDlg::OnBnClickedCheckScecpyTop)
	ON_BN_CLICKED(IDC_BUTTON_TOP_APK_VERSION, &AndroidPcToolDlg::OnBnClickedButtonTopApkVersion)
	ON_COMMAND(ID_PICTURE_COMPRESS, &AndroidPcToolDlg::OnPictureCompress)
	ON_COMMAND(ID_Android_CODE, &AndroidPcToolDlg::OnAndroidCode)
	ON_COMMAND(ID_Mp4ToMp3, &AndroidPcToolDlg::OnMp4tomp3)
	ON_COMMAND(ID_LanHU_UI, &AndroidPcToolDlg::OnLanhuUi)
	ON_COMMAND(ID_henleylee, &AndroidPcToolDlg::Onhenleylee)
	ON_COMMAND(ID_GET_PACK, &AndroidPcToolDlg::OnGetPack)
	ON_BN_CLICKED(IDC_MFCMENUBUTTON_PULL_LOG, &AndroidPcToolDlg::OnBnClickedMfcmenubuttonPullLog)
	ON_COMMAND(ID_CONIF_PATH_AND_USE, &AndroidPcToolDlg::OnConifPathAndUse)
	ON_BN_CLICKED(IDC_BUTTON_PULL_TOP_APK, &AndroidPcToolDlg::OnBnClickedButtonPullTopApk)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_JADX, &AndroidPcToolDlg::OnBnClickedButtonOpenJadx)
	ON_BN_CLICKED(IDC_BUTTON_OPNE_FSCapture, &AndroidPcToolDlg::OnBnClickedButtonOpneFscapture)
	ON_BN_CLICKED(IDC_BUTTON_ADB_REBOOT, &AndroidPcToolDlg::OnBnClickedButtonAdbReboot)
	ON_BN_CLICKED(IDC_BUTTON_FASTBOOT_REBOOT, &AndroidPcToolDlg::OnBnClickedButtonFastbootReboot)
	ON_BN_CLICKED(IDC_BUTTON_ROOT_REMOUNT, &AndroidPcToolDlg::OnBnClickedButtonRootRemount)
	ON_BN_CLICKED(IDC_MFCMENUBUTTON1, &AndroidPcToolDlg::OnBnClickedMfcmenubuttonKillAdb)
	ON_BN_CLICKED(IDC_MFCMENUBUTTON2, &AndroidPcToolDlg::OnBnClickedMfcmenubuttonKillJava)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_APP, &AndroidPcToolDlg::OnBnClickedButtonClearApp)
	ON_COMMAND(ID_32809, &AndroidPcToolDlg::OnHf)
	ON_BN_CLICKED(IDC_BUTTON_INSTALL_APK, &AndroidPcToolDlg::OnBnClickedButtonInstallApk)
	ON_BN_CLICKED(IDC_BUTTON8, &AndroidPcToolDlg::OnBnClickedButtonApkInSettings)
	ON_BN_CLICKED(IDC_BUTTON_PUSH, &AndroidPcToolDlg::OnBnClickedButtonPush)
	ON_BN_CLICKED(IDC_BUTTON_LS, &AndroidPcToolDlg::OnBnClickedButtonLs)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE_DIR, &AndroidPcToolDlg::OnCbnSelchangeComboDeviceDir)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_radionCommonLogs.SetCheck(TRUE);

	m_comboBoxDeviceDir.InsertString(0, L"system/app/HwLauncher6/");
	m_comboBoxDeviceDir.InsertString(0, L"system/app/Bluetooth/");
	m_comboBoxDeviceDir.InsertString(0, L"system/framework/");
	m_comboBoxDeviceDir.InsertString(0, L"system/priv-app/SystemUI/");
	m_comboBoxDeviceDir.InsertString(0, L"system/priv-app/TeleService/");
	m_comboBoxDeviceDir.InsertString(0, L"system/app/HwLauncher3/");
	m_comboBoxDeviceDir.InsertString(0, L"system/priv-app/Settings/");

	m_comboBoxDeviceDir.SetCurSel(0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void AndroidPcToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
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

CStringA AndroidPcToolDlg::cmdAndShowEdit(CStringA cmd,bool isNeedShowDefalutMsg)
{
	FILE* pipe;
	char buffer[1024];
	std::string command = cmd.GetString();
	pipe = _popen(command.c_str(), "r");
	if (!pipe) {
		AfxMessageBox(_T("Failed to execute command"));
		return "";
	}

	//adb shell dumpsys package tv.danmaku.bili | findstr version

	std::string installPath = "";
	while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
		installPath = installPath + buffer + "\r\n";
	}
	_pclose(pipe);

	// 将结果显示在编辑框中
	m_editShowResut = installPath.c_str();
	if (installPath.empty() && isNeedShowDefalutMsg) {
		m_editShowResut = L"请检查设备连接或者是否解锁";
	}
	UpdateData(FALSE);

	return installPath.c_str();
}

CStringA AndroidPcToolDlg::cmdAndShowTopApkEdit(CStringA cmd)
{
	return CStringA();
}

void AndroidPcToolDlg::OnWxCode()
{
	openWeb("https://git.weixin.qq.com/explore");
}


void AndroidPcToolDlg::OnGithup()
{
	openWeb("https://github.com/crazycodeboy/RNStudyNotes");
}


void AndroidPcToolDlg::OnGitcode()
{
	openWeb("https://gitcode.com/");
}


void AndroidPcToolDlg::OnGitee()
{
	openWeb("https://gitee.com/");
}


void AndroidPcToolDlg::OnUiAlibabaPng()
{
	openWeb("https://www.iconfont.cn/collections/index?spm=a313x.7781069.1998910419.5&type=2");
}


void AndroidPcToolDlg::OnCainiao()
{
	openWeb("https://www.runoob.com/w3cnote_genre/android-advance");
}


void AndroidPcToolDlg::OnAiGitcodeDeepseek()
{
	openWeb("https://gitcode.com/ai/?utm_source=sidebar");
}


void AndroidPcToolDlg::OnToIcon()
{
	openWeb("https://convertio.co/zh/");
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

void AndroidPcToolDlg::OnBnClickedButtonTopActivity()
{
	cmdAndShowEdit("adb shell dumpsys \"activity top | grep ACTIVITY | tail -n 1\"",true);
}

CString getTopPackageName()
{
	// 执行命令获取当前置顶应用的包名
	FILE* pipe = _popen("adb shell dumpsys \"activity top | grep ACTIVITY | tail -n 1\"", "r");
	if (!pipe) {
		//AfxMessageBox(_T("Failed to execute command"));
		return L"";
	}

	// 读取包名
	char buffer[128];
	std::string packageAndActivityName = "";
	if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
		packageAndActivityName = buffer;
		// 去掉换行符
		packageAndActivityName.erase(packageAndActivityName.find_last_not_of("\r\n") + 1);
	}
	_pclose(pipe);
	CString output = CString(packageAndActivityName.c_str());
	CString packageName;
	// 查找 "com." 起始位置，避免截取到前面的无关内容
	int nStartPos = output.Find(_T("ACTIVITY")) + 9;
	if (nStartPos != -1) {
		// 从 "com." 开始查找第一个 '/'
		int nSlashPos = output.Find(_T('/'), nStartPos);
		if (nSlashPos != -1) {
			packageName = output.Mid(nStartPos, nSlashPos - nStartPos);
		}
	}
	return packageName;
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


void AndroidPcToolDlg::OnBnClickedButtonOpenScrcpy()
{
	ShellExecuteA(NULL, "open", "scrcpy-noconsole.vbs", "", "scrcpy-win64-v3.2", SW_SHOWNORMAL);
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


void AndroidPcToolDlg::OnPictureCompress()
{
	openWeb("https://tinypng.com/");
}


void AndroidPcToolDlg::OnAndroidCode()
{
	openWeb("https://www.androidos.net.cn/sourcecode");
}


void AndroidPcToolDlg::OnMp4tomp3()
{
	openWeb("https://mp4tomp3.org/");
}


void AndroidPcToolDlg::OnLanhuUi()
{
	openWeb("https://lanhuapp.com/dashboard/#/item");
}


void AndroidPcToolDlg::Onhenleylee()
{
	openWeb("https://henleylee.github.io/");
}


void AndroidPcToolDlg::OnGetPack()
{
	openWeb("https://blog.csdn.net/c10WTiybQ1Ye3/article/details/78098632");
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


void AndroidPcToolDlg::OnBnClickedButtonOpenJadx()
{
	ShellExecuteA(NULL, "open", "jadx.exe", "", "", SW_SHOWNORMAL);
}


void AndroidPcToolDlg::OnBnClickedButtonOpneFscapture()
{
	ShellExecuteA(NULL, "open", "FSCapture.exe", "FSCapture", "", SW_SHOWNORMAL);
}


void AndroidPcToolDlg::OnBnClickedButtonAdbReboot()
{
	ShellExecuteA(NULL, "open", "adb", "", "reboot", SW_HIDE);
}


void AndroidPcToolDlg::OnBnClickedButtonFastbootReboot()
{
	ShellExecuteA(NULL, "open", "fastboot", "", "reboot", SW_HIDE);
}


void AndroidPcToolDlg::OnBnClickedButtonRootRemount()
{
	cmdAndShowEdit("adb root && adb remount", true);
}


void AndroidPcToolDlg::OnBnClickedMfcmenubuttonKillAdb()
{
	cmdAndShowEdit("adb kill-server && taskkill / F / IM adb.exe");
}


void AndroidPcToolDlg::OnBnClickedMfcmenubuttonKillJava()
{
	cmdAndShowEdit("taskkill / F / IM java.exe");
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


void AndroidPcToolDlg::OnHf()
{
	openWeb("https://hf-mirror.com/");
}

void AndroidPcToolDlg::OnBnClickedButtonInstallApk()
{
	UpdateData(TRUE);
	std::string command = "adb install " + CStringA(m_editInputPath);
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

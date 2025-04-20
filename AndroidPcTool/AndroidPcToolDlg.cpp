
// AndroidPcToolDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "AndroidPcTool.h"
#include "AndroidPcToolDlg.h"
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


void AndroidPcToolDlg::OnBnClickedButtonTopActivity()
{
	//CoInitialize(NULL);

	//// 创建管道和安全属性
	//SECURITY_ATTRIBUTES sa;
	//ZeroMemory(&sa, sizeof(sa));
	//sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	//sa.lpSecurityDescriptor = NULL;
	//sa.bInheritHandle = TRUE;

	//// 创建管道
	//HANDLE hRead, hWrite;
	//CreatePipe(&hRead, &hWrite, &sa, 0);

	//// 创建子进程执行命令
	//STARTUPINFO si;
	//PROCESS_INFORMATION pi;
	//ZeroMemory(&si, sizeof(si));
	//si.cb = sizeof(si);
	//si.hStdError = hWrite;
	//si.hStdOutput = hWrite;
	//si.dwFlags |= STARTF_USESTDHANDLES;
	//si.wShowWindow = SW_SHOWNORMAL;

	//// 创建进程执行adb命令
	//wchar_t cmd[] = L"adb shell dumpsys activity | grep “mResumedActivity”";
	//BOOL bSuccess = CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	//if (bSuccess) {
	//	CloseHandle(hWrite); // 不再需要写入句柄
	//	CHAR buffer[1024] = { 0 }; // 缓冲区来读取输出
	//	DWORD bytesRead;
	//	ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL); // 读取输出到缓冲区
	//	CloseHandle(hRead); // 关闭读取句柄
	//	CloseHandle(pi.hProcess); // 关闭进程句柄
	//	CloseHandle(pi.hThread); // 关闭线程句柄
	//	CoUninitialize(); // 反初始化COM库
	//	CString strOutput = CA2T(buffer); // 将C风格字符串转换为CString对象

	//	strOutput.Left(strOutput.Find(_T("\n\n")));
	//	m_editShowResut = strOutput;
	//	UpdateData(FALSE);
	//}
	//else {
	//	// 处理错误情况，例如显示错误信息到编辑框或日志中
	//	MessageBox(_T("无法执行ADB命令"), _T("错误"), MB_ICONERROR);
	//}

	 // 执行adb命令
	FILE* pipe = _popen("adb shell dumpsys activity | findstr mResumedActivity", "r");
	if (!pipe) {
		AfxMessageBox(_T("Failed to execute command"));
		return;
	}

	// 读取命令输出
	char buffer[1024];
	std::string result = "";
	while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
		result += buffer;
	}
	_pclose(pipe);

	// 将结果显示在编辑框中
	m_editShowResut = result.c_str();
	if (result.empty()) {
		m_editShowResut = L"请检查设备连接或者是否解锁";
	}
	UpdateData(FALSE);
}


std::string getTopPackageName()
{
	// 执行命令获取当前置顶应用的包名
	FILE* pipe = _popen("adb shell \"dumpsys activity | grep 'mResumedActivity' | awk -F ' ' '{print $4}' | cut -d '/' -f 1\"", "r");
	if (!pipe) {
		AfxMessageBox(_T("Failed to execute command"));
		return "";
	}

	// 读取包名
	char buffer[128];
	std::string packageName = "";
	if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
		packageName = buffer;
		// 去掉换行符
		packageName.erase(packageName.find_last_not_of("\r\n") + 1);
	}
	_pclose(pipe);
	return packageName;
}

void AndroidPcToolDlg::OnBnClickedButtonTopPath()
{
	// 执行命令获取当前置顶应用的包名
	FILE* pipe;

	// 读取包名
	char buffer[128];
	std::string packageName = getTopPackageName();
	if (packageName.empty()) {
		AfxMessageBox(_T("No active application found"));
		return;
	}

	// 执行命令获取应用的安装路径
	std::string command = "adb shell pm path " + packageName;
	pipe = _popen(command.c_str(), "r");
	if (!pipe) {
		AfxMessageBox(_T("Failed to execute command"));
		return;
	}

	// 读取安装路径
	std::string installPath = "";
	if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
		installPath = buffer;
		// 去掉换行符
		installPath.erase(installPath.find_last_not_of("\r\n") + 1);
	}
	_pclose(pipe);

	// 将结果显示在编辑框中
	m_editShowResut = installPath.c_str();
	if (installPath.empty()) {
		m_editShowResut = L"请检查设备连接或者是否解锁";
	}
	UpdateData(FALSE);
}


void AndroidPcToolDlg::OnBnClickedButtonOpenScrcpy()
{
	ShellExecuteA(NULL, "open", "scrcpy-noconsole.vbs", "", "scrcpy-win64-v3.2", SW_SHOWNORMAL);
}



void AndroidPcToolDlg::OnBnClickedButtonTopApkVersion()
{
	// 执行命令获取当前置顶应用的包名
	FILE* pipe;

	// 读取包名
	char buffer[1024];
	std::string packageName = getTopPackageName();
	if (packageName.empty()) {
		AfxMessageBox(_T("No active application found"));
		return;
	}

	// 执行命令获取应用的安装路径
	std::string command = "adb shell dumpsys package " + packageName + " | findstr version";
	pipe = _popen(command.c_str(), "r");
	if (!pipe) {
		AfxMessageBox(_T("Failed to execute command"));
		return;
	}

	//adb shell dumpsys package tv.danmaku.bili | findstr version

	// 读取安装路径
	std::string installPath = "";
	while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
		installPath = installPath + buffer + "\r\n";
	}
	_pclose(pipe);

	// 将结果显示在编辑框中
	m_editShowResut = installPath.c_str();
	if (installPath.empty()) {
		m_editShowResut = L"请检查设备连接或者是否解锁";
	}
	UpdateData(FALSE);
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

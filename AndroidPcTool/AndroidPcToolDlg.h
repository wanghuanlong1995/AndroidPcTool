
// AndroidPcToolDlg.h: 头文件
//

#pragma once
#include"DragEdit.h"

// AndroidPcToolDlg 对话框
class AndroidPcToolDlg : public CDialogEx
{
// 构造
public:
	AndroidPcToolDlg(CWnd* pParent = nullptr);	// 标准构造函数

	void openWeb(const char* url);

	void setViewHide(int viewId); // 隐藏控件

	CStringA cmdAndShowEdit(CStringA cmd, bool isNeedShowDefalutMsg = false); // 执行cmd命令并且显示到编辑框

	CStringA cmdAndShowTopApkEdit(CStringA cmd);  // 获取顶部apk后执行adb 和顶部apk相关的 执行cmd命令并且显示到编辑框

	CString getAndChekTopPackageName();  // 检查获取顶部apk后执行adb是否正常

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANDROIDPCTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckTopSelft();
	// 是否将本应用置顶
	BOOL m_isTopSelft;
	// 是否自动打开导出的文件的目录
	BOOL m_isAutoOpenPullDir;
	afx_msg void OnWxCode();
	afx_msg void OnGithup();
	afx_msg void OnGitcode();
	afx_msg void OnGitee();
	afx_msg void OnUiAlibabaPng();
	afx_msg void OnCainiao();
	afx_msg void OnAiGitcodeDeepseek();
	afx_msg void OnToIcon();
	afx_msg void OnBnClickedButtonTopActivity();
	// 执行结果输出
	CString m_editShowResut;
	afx_msg void OnBnClickedButtonTopPath();
	afx_msg void OnBnClickedButtonOpenScrcpy();
	// 是否将投屏工具置顶
	BOOL m_isScrcpyTop;
	afx_msg void OnBnClickedCheckScecpyTop();
	afx_msg void OnBnClickedButtonTopApkVersion();
	afx_msg void OnPictureCompress();
	afx_msg void OnAndroidCode();
	afx_msg void OnMp4tomp3();
	afx_msg void OnLanhuUi();
	afx_msg void Onhenleylee();
	afx_msg void OnGetPack();
	// 是否导出常用日志
	CButton m_radionCommonLogs;
	afx_msg void OnBnClickedMfcmenubuttonPullLog();
	afx_msg void OnConifPathAndUse();
	afx_msg void OnBnClickedButtonPullTopApk();
	afx_msg void OnBnClickedButtonOpenJadx();
	afx_msg void OnBnClickedButtonOpneFscapture();
	afx_msg void OnBnClickedButtonAdbReboot();
	afx_msg void OnBnClickedButtonFastbootReboot();
	afx_msg void OnBnClickedButtonRootRemount();
	afx_msg void OnBnClickedMfcmenubuttonKillAdb();
	afx_msg void OnBnClickedMfcmenubuttonKillJava();
	afx_msg void OnBnClickedButtonClearApp();
	afx_msg void OnHf();
	// 可以拓展输入的路径的编辑框
	DragEdit m_dragInputEdit;
	afx_msg void OnBnClickedButtonInstallApk();
	// 可以拓展输入的路径的编辑框的文字
	CString m_editInputPath;
	// 是否拖拽后自动安装apk
	BOOL m_isAutoInstallApk;
	afx_msg void OnBnClickedButtonApkInSettings();
	// 设备目录选择
	CComboBox m_comboBoxDeviceDir;
	afx_msg void OnBnClickedButtonPush();
	// 选择的设备中的目录
	CString m_deviceDIr;
	afx_msg void OnBnClickedButtonLs();
	afx_msg void OnCbnSelchangeComboDeviceDir();
	afx_msg void OnAiNamiSo();
	afx_msg void Ongetipconfig();
	afx_msg void OnSystemEnv();
};


// AndroidPcToolDlg.h: 头文件
//

#pragma once


// AndroidPcToolDlg 对话框
class AndroidPcToolDlg : public CDialogEx
{
// 构造
public:
	AndroidPcToolDlg(CWnd* pParent = nullptr);	// 标准构造函数

	void openWeb(const char* url);

	void setViewHide(int viewId); // 隐藏控件

	CStringA cmdAndShowEdit(CStringA cmd); // 执行cmd命令并且显示到编辑框

	CStringA cmdAndShowTopApkEdit(CStringA cmd);  // 获取顶部apk后执行adb 和顶部apk相关的 执行cmd命令并且显示到编辑框

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
};

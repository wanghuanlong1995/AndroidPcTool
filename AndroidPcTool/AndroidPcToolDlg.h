
// AndroidPcToolDlg.h: 头文件
//

#pragma once


// CAndroidPcToolDlg 对话框
class CAndroidPcToolDlg : public CDialogEx
{
// 构造
public:
	CAndroidPcToolDlg(CWnd* pParent = nullptr);	// 标准构造函数

	void openWeb(const char* url);

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
};

// DragEdit.cpp: 实现文件
//

#include "pch.h"
#include "AndroidPcTool.h"
#include "AndroidPcToolDlg.h"
#include "DragEdit.h"


// DragEdit

IMPLEMENT_DYNAMIC(DragEdit, CWnd)

DragEdit::DragEdit()
{

}

DragEdit::~DragEdit()
{
}


BEGIN_MESSAGE_MAP(DragEdit, CWnd)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

// DragEdit 消息处理程序

void DragEdit::OnDropFiles(HDROP hDropInfo)
{
	// 获取拖放文件数量
	UINT nFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	CString strFiles; // 存储所有文件路径

	for (UINT i = 0; i < nFileCount; i++)
	{
		TCHAR szPath[MAX_PATH];
		DragQueryFile(hDropInfo, i, szPath, MAX_PATH);
		strFiles += szPath;
		
		//strFiles += _T("\r\n"); // 换行分隔多个文件
	}

	// 显示到编辑框
	SetWindowText(strFiles);

	AndroidPcToolDlg* dlg = (AndroidPcToolDlg*)GetParent();
	dlg->UpdateData(TRUE);
	if (dlg->m_isAutoInstallApk) {
		dlg->OnBnClickedButtonInstallApk();
	}

	DragFinish(hDropInfo); // 释放资源
	CEdit::OnDropFiles(hDropInfo);
}

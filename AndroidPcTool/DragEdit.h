#pragma once


// DragEdit

class DragEdit : public CEdit
{
	DECLARE_DYNAMIC(DragEdit)

public:
	DragEdit();
	virtual ~DragEdit();

	//void OnDropFiles(HDROP hDropInfo);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
};



#pragma once


// Rection_Dialog 对话框

class Rection_Dialog : public CDialogEx
{
	DECLARE_DYNAMIC(Rection_Dialog)

public:
	Rection_Dialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Rection_Dialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG14 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString Edit_Rection;
	void get_relocation_infomation(char* buff);
protected:
	afx_msg LRESULT OnUser_Rection(WPARAM wParam, LPARAM lParam);
};

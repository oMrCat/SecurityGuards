#pragma once


// Unload_Dialog 对话框

class Unload_Dialog : public CDialogEx
{
	DECLARE_DYNAMIC(Unload_Dialog)

public:
	Unload_Dialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Unload_Dialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG17 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString Edit_Unload;
	void GetUnloadInfo(char* buff);
protected:
	afx_msg LRESULT OnUser_Unload(WPARAM wParam, LPARAM lParam);
};

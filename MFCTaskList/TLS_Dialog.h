#pragma once


// TLS_Dialog 对话框

class TLS_Dialog : public CDialogEx
{
	DECLARE_DYNAMIC(TLS_Dialog)

public:
	TLS_Dialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~TLS_Dialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG16 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString Edit_TLS;
	void DispatchTLS(char* buff);
protected:
	afx_msg LRESULT OnUser_TLS(WPARAM wParam, LPARAM lParam);
};

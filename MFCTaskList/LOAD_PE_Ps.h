#pragma once


// LOAD_PE_Ps 对话框

class LOAD_PE_Ps : public CDialogEx
{
	DECLARE_DYNAMIC(LOAD_PE_Ps)

public:
	LOAD_PE_Ps(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~LOAD_PE_Ps();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG11 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
public:
//	afx_msg void OnBnClickedButton1();
//	CEdit edit_FileHeader;
//	virtual BOOL OnInitDialog();
	void get_file_header_information(char* buff);
	CString edit_Fileheader;
	void rva_to_vafoa(char* buf);
	void va_to_rvafoa(char* buf);
	void foa_to_rvava(char* buf);
	CString Edit_Va;
	CString Edit_Rva;
	CString Edit_Foa;
	afx_msg void OnBnClickedButton2();
	CEdit Edit_Control_Foa;
	CEdit Edit_Control_Va;
	CEdit Edit_Control_Rva;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton8();
};

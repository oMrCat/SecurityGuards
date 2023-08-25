#pragma once
#include"Resource.h"
#include<vector>
using std::vector;

// CMyDialog2 对话框

class CMyDialog2 : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDialog2)

public:
	CMyDialog2(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyDialog2();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString w_file;
	void FileAttribute();
	afx_msg void OnBnClickedButton2();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	CListCtrl m_filelist;
};

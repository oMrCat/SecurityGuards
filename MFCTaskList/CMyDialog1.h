#pragma once
#include"Resource.h"
#include<vector>
using std::vector;

// CMyDialog1 对话框

class CMyDialog1 : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDialog1)

public:
	CMyDialog1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyDialog1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	CListCtrl m_filelist;
	CString m_fileedit;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	void GetFilePath(vector<CString>& vFilePathList, CString strDir);
	afx_msg void OnBnClickedButton2();
//	afx_msg void OnBnClickedButton3();
};

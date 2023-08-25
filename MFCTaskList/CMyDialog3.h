#pragma once
#include"Resource.h"
#include<tlhelp32.h>
#include<vector>
using std::vector;
// CMyDialog3 对话框

class CMyDialog3 : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDialog3)

public:
	CMyDialog3(CWnd* pParent = nullptr);   // 标准构造函数
	
	virtual ~CMyDialog3();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL EnumProcess();
	CListCtrl m_proList;
	vector<DWORD> m_vecPid;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	void ProcessList();
	afx_msg void OnProcesslist();
	afx_msg void OnModul();
	afx_msg void OnModule();
	afx_msg void OncloseProcess();
};

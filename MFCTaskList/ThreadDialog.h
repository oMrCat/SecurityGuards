#pragma once
#include"resource.h"
#include"CMyDialog3.h"
#include<vector>
using std::vector;
// ThreadDialog 对话框

class ThreadDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ThreadDialog)

public:
	ThreadDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ThreadDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
public:
	CListCtrl m_modullist;
	BOOL EnumThread();
	void ThreadList();
	CListCtrl m_threadlist;
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSuspendThread();
	afx_msg void OnResumeThread();
	afx_msg void OnCloseThread();
};

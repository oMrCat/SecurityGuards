#pragma once
#include"Resource.h"
#include<vector>
using std::vector;

// HeapDialog 对话框

class HeapDialog : public CDialogEx
{
	DECLARE_DYNAMIC(HeapDialog)

public:
	HeapDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~HeapDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL HeapList(DWORD dwID);
	CListCtrl m_heaplist;
protected:
	afx_msg LRESULT OnUser_Heap(WPARAM wParam, LPARAM lParam);
	void ModuleList();
};

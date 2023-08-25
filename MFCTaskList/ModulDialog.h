#pragma once
#include"Resource.h"
#include<vector>
using std::vector;

// ModulDialog 对话框

class ModulDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ModulDialog)

public:
	ModulDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ModulDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUser_Modul(WPARAM wParam, LPARAM lParam);
	BOOL EnumModule();
	void ModuleList();
public:
	CListCtrl m_modullist;
};

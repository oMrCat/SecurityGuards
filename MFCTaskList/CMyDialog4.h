#pragma once
#include<vector>
using std::vector;

// CMyDialog4 对话框

class CMyDialog4 : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDialog4)

public:
	CMyDialog4(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyDialog4();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListCtrl ser_List;
	void GetServiceInfo();
	void UpDataSerInfo();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void On32781();
	afx_msg void On32782();
};

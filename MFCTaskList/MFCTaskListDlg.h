﻿
// MFCTaskListDlg.h: 头文件
//

#pragma once
#include"CMyTabCtrl.h"
// CMFCTaskListDlg 对话框
class CMFCTaskListDlg : public CDialogEx
{
// 构造
public:
	CMFCTaskListDlg(CWnd* pParent = nullptr);	// 标准构造函数
	BOOL m_IsWindowsHide = TRUE;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCTASKLIST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMyTabCtrl m_tab;
	CStatusBar m_status;
	CString cs;
	CString cp;
	void GetCpuState(int& nStr);
	DWORD GetMemState();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);

	afx_msg void OnBnClickedButton6();
};

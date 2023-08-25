#pragma once
#include<vector>
#include"CMyDialog3.h"
using std::vector;

// CMyTabCtrl

class CMyTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTabCtrl)

public:
	CMyTabCtrl();
	virtual ~CMyTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()

private:
	vector<CDialogEx*>m_vecDlg;
public:
	void InitTabCtrl(int count, ...);
	afx_msg void OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
};



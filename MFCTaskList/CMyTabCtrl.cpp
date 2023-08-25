// CMyTabCtrl.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "CMyTabCtrl.h"


// CMyTabCtrl

IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{

}

CMyTabCtrl::~CMyTabCtrl()
{
}

void CMyTabCtrl::InitTabCtrl(int count, ...)
{
	va_list vl;
	va_start(vl, count);

	for (int i = 0; i < count; i++)
	{
		TCHAR* name = va_arg(vl, TCHAR*);
		CDialogEx* dlg = va_arg(vl, CDialogEx*);
		DWORD id = va_arg(vl, DWORD);

		// 添加选项名称
		InsertItem(i, name);
		// 创建窗口
		dlg->Create(id, this);
		// 移动子窗口
		CRect rc = {};
		GetClientRect(&rc);
		rc.DeflateRect(1, 26, 1, 1);
		dlg->MoveWindow(rc);
		// 隐藏窗口
		dlg->ShowWindow(SW_HIDE);
		// 将窗口指针 push 到 vector 中
		m_vecDlg.push_back(dlg);
	}
	m_vecDlg[0]->ShowWindow(SW_SHOW);
	va_end(vl);
}

BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMyTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()



// CMyTabCtrl 消息处理程序




void CMyTabCtrl::OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int select = GetCurSel();
	for (int i = 0; i < m_vecDlg.size(); i++)
	{
		if (i == select)
		{
			m_vecDlg[i]->ShowWindow(SW_SHOW);
		}
		else
		{
			m_vecDlg[i]->ShowWindow(SW_HIDE);
		}
	}
	*pResult = 0;
}


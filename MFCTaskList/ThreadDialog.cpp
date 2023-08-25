// ThreadDialog.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "ThreadDialog.h"
#include "afxdialogex.h"

// ThreadDialog 对话框
//CString tmp;
DWORD dwProcessId;
vector<DWORD>threa;
vector<CString>threa1;
vector<CString>threa2;
IMPLEMENT_DYNAMIC(ThreadDialog, CDialogEx)

ThreadDialog::ThreadDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG6, pParent)
{

}

ThreadDialog::~ThreadDialog()
{
}

void ThreadDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_threadlist);
}


BEGIN_MESSAGE_MAP(ThreadDialog, CDialogEx)
	ON_MESSAGE(WM_USER + 1, &ThreadDialog::OnUser)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &ThreadDialog::OnRclickList1)
	ON_COMMAND(ID_32778, &ThreadDialog::OnSuspendThread)
	ON_COMMAND(ID_32779, &ThreadDialog::OnResumeThread)
	ON_COMMAND(ID_32780, &ThreadDialog::OnCloseThread)
END_MESSAGE_MAP()


// ThreadDialog 消息处理程序


BOOL ThreadDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//ThreadList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


afx_msg LRESULT ThreadDialog::OnUser(WPARAM wParam, LPARAM lParam)
{
	dwProcessId = (DWORD)lParam;
	//tmp.Format(L"%d", dwProcessId);
	ThreadList();
	return 0;
}

BOOL ThreadDialog::EnumThread()
{
	threa.clear();
	threa1.clear();
	m_threadlist.DeleteAllItems();
	//DWORD pro;
	CString pro1;
	CString pro2;
	HANDLE hThreadSnapshot = 0;
	THREADENTRY32 te = { 0 };
	te.dwSize = sizeof(THREADENTRY32);
	hThreadSnapshot = CreateToolhelp32Snapshot(
		TH32CS_SNAPTHREAD,  //创建的是线程快照
		dwProcessId            //进程ID，只有在创建模块，堆快照的时候，指定进程
	);
	if (hThreadSnapshot == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	//3. 获取第一个线程的信息，如果获取成功，就可以继续往后获取
	BOOL bSuccess = Thread32First(
		hThreadSnapshot,//快照的句柄
		&te     //第一个进程的信息
	);
	//4. 循环遍历，所有的进程信息
	if (bSuccess == TRUE)
	{
		do
		{
			//HANDLE hThread = OpenProcess(THREAD_QUERY_INFORMATION | THREAD_QUERY_LIMITED_INFORMATION, FALSE, te.th32ThreadID);
			//pro.Format(L"%8d", te.th32ThreadID);
			pro1.Format(L"%d", te.th32OwnerProcessID);
			pro2.Format(L"%d", te.tpDeltaPri);
			threa.push_back(te.th32ThreadID);
			threa1.push_back(pro1);
			threa2.push_back(pro2);
		} while (Thread32Next(hThreadSnapshot, &te));
	}
	//关闭句柄
	CloseHandle(hThreadSnapshot);
	return 0;
}
void ThreadDialog::ThreadList()
{
	CRect rc = {};
	m_threadlist.GetClientRect(&rc);
	// 通过调用 InsertColumn 函数添加列
	m_threadlist.InsertColumn(0, _T("序号"), 0, rc.right / 4);
	m_threadlist.InsertColumn(1, _T("线程ID"), 0, rc.right / 4);
	m_threadlist.InsertColumn(2, _T("tpBasePri"), 0, rc.right / 4);
	m_threadlist.InsertColumn(3, _T("tpDeltaPri"), 0, rc.right / 4);
	m_threadlist.SetExtendedStyle(LVS_EX_CHECKBOXES |
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	if (EnumThread() == 0)
	{
		static int index = 0;
		CString temp2;
		CString temp3;
		for (int i = 0; i < threa1.size(); i++)
		{
			if (threa1[i].GetLength() != 0)
			{
				temp2.Format(_T("%d"), threa[i]);
				temp3.Format(_T("%d"), i);
				// 通过 InsertItem 函数添加行
				m_threadlist.InsertItem(i, temp3);
				// 通过 SetItemText 函数给每一行设置内容
				m_threadlist.SetItemText(i, 1, temp2);
				m_threadlist.SetItemText(i, 2, threa1[i]);
				m_threadlist.SetItemText(i, 3, threa2[i]);
				index++;
			}
		}
	}

}

void ThreadDialog::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CMenu menu;
	menu.LoadMenu(IDR_MENU2);
	CMenu* pSub = menu.GetSubMenu(0);

	POINT pt;
	GetCursorPos(&pt);
	pSub->TrackPopupMenu(TPM_RIGHTALIGN, pt.x, pt.y, this);
	*pResult = 0;
}



void ThreadDialog::OnSuspendThread()
{
	// TODO: 在此添加命令处理程序代码
	DWORD dwIndex = m_threadlist.GetSelectionMark();
	//THREADENTRY32 te = { sizeof(THREADENTRY32) };
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, threa[dwIndex]);
	SuspendThread(hThread);
	CloseHandle(hThread);
}


void ThreadDialog::OnResumeThread()
{
	// TODO: 在此添加命令处理程序代码
	DWORD dwIndex = m_threadlist.GetSelectionMark();
	//THREADENTRY32 te = { sizeof(THREADENTRY32) };
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, threa[dwIndex]);
	ResumeThread(hThread);
	CloseHandle(hThread);
}


void ThreadDialog::OnCloseThread()
{
	// TODO: 在此添加命令处理程序代码
	DWORD dwIndex = m_threadlist.GetSelectionMark();
	//THREADENTRY32 te = { sizeof(THREADENTRY32) };
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, threa[dwIndex]);
	TerminateThread(hThread,0);
	CloseHandle(hThread);
}

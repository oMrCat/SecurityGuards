// CMyDialog3.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "CMyDialog3.h"
#include "afxdialogex.h"
#include"CDialogMod.h"
#include"ThreadDialog.h"
#include"ModulDialog.h"
#include"HeapDialog.h"

vector<DWORD>m_pro;
vector<CString>m_pro1;
vector<CString>m_pro2;

// CMyDialog3 对话框

IMPLEMENT_DYNAMIC(CMyDialog3, CDialogEx)

CMyDialog3::CMyDialog3(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

CMyDialog3::~CMyDialog3()
{
}

void CMyDialog3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_proList);
}


BEGIN_MESSAGE_MAP(CMyDialog3, CDialogEx)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMyDialog3::OnRclickList1)
	ON_COMMAND(ID_32773, &CMyDialog3::OnProcesslist)
	ON_COMMAND(ID_32774, &CMyDialog3::OnModul)
	ON_COMMAND(ID_32775, &CMyDialog3::OnModule)
	ON_COMMAND(ID_32776, &CMyDialog3::OncloseProcess)
END_MESSAGE_MAP()


// CMyDialog3 消息处理程序

BOOL CMyDialog3::EnumProcess()
{
	m_pro.clear();
	m_pro1.clear();
	m_pro.clear();
	m_proList.DeleteAllItems();
	CString pro;
	CString pro1;
	CString pro2;
	HANDLE hProcessSnapshot = 0;
	PROCESSENTRY32 pe = { 0 };
	pe.dwSize = sizeof(PROCESSENTRY32);
	hProcessSnapshot = CreateToolhelp32Snapshot(
		TH32CS_SNAPPROCESS,  //创建的是进程快照
		0            //进程ID，只有在创建模块，堆快照的时候，指定进程
	);
	if (hProcessSnapshot == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	//3. 获取第一个进程的信息，如果获取成功，就可以继续往后获取
	BOOL bSuccess = Process32First(
		hProcessSnapshot,//快照的句柄
		&pe              //第一个进程的信息
	);
	TCHAR szExePath[MAX_PATH] = { 0 };
	DWORD dwSize = MAX_PATH;
	//4. 循环遍历，所有的进程信息
	if (bSuccess == TRUE)
	{
		do
		{
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe.th32ProcessID);
			dwSize = MAX_PATH;
			QueryFullProcessImageName(hProcess, 0, szExePath, &dwSize);
			//pro.Format(L"%8d", pe.th32ProcessID);
			pro1.Format(L"%s", pe.szExeFile);
			pro2.Format(L"%s", szExePath);
			m_pro.push_back(pe.th32ProcessID);
			m_pro1.push_back(pro1);
			m_pro2.push_back(pro2);
		} while (Process32Next(hProcessSnapshot, &pe));
	}
	//关闭句柄
	CloseHandle(hProcessSnapshot);
	return 0;
}

void CMyDialog3::ProcessList()
{
	CRect rc = {};
	m_proList.GetClientRect(&rc);
	// 通过调用 InsertColumn 函数添加列
	m_proList.InsertColumn(0, _T("PID"), 0, rc.right / 3);
	m_proList.InsertColumn(1, _T("进程名称"), 0, rc.right / 3);
	m_proList.InsertColumn(2, _T("进程路径"), 0, rc.right / 3);
	m_proList.SetExtendedStyle(LVS_EX_CHECKBOXES |
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	if (EnumProcess() == 0)
	{
		static int index = 0;
		CString temp2;
		for (int i = 0; i < m_pro.size(); i++)
		{
			if (m_pro1[i].GetLength() != 0)
			{
				temp2.Format(_T("%d"), m_pro[i]);
				// 通过 InsertItem 函数添加行
				m_proList.InsertItem(i, temp2);
				// 通过 SetItemText 函数给每一行设置内容
				//m_proList.SetItemText(i, 0, m_pro[i]);
				m_proList.SetItemText(i, 1, m_pro1[i]);
				m_proList.SetItemText(i, 2, m_pro2[i]);
				index++;
			}
		}
	}

}

BOOL CMyDialog3::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ProcessList();
	SetTimer(
		2,
		100000,
		NULL
	);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CMyDialog3::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 2)
	{
		CRect rc = {};
		m_proList.GetClientRect(&rc);
		// 通过调用 InsertColumn 函数添加列
		m_proList.InsertColumn(0, _T("PID"), 0, rc.right / 3);
		m_proList.InsertColumn(1, _T("进程名称"), 0, rc.right / 3);
		m_proList.InsertColumn(2, _T("进程路径"), 0, rc.right / 3);
		m_proList.SetExtendedStyle(LVS_EX_CHECKBOXES |
			LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
		if (EnumProcess() == 0)
		{
			static int index = 0;
			CString temp;
			for (int i = 0; i < m_pro.size(); i++)
			{
				if (m_pro1[i].GetLength() != 0)
				{
					temp.Format(_T("%d"), m_pro[i]);
					// 通过 InsertItem 函数添加行
					m_proList.InsertItem(i, temp);
					// 通过 SetItemText 函数给每一行设置内容
					//m_proList.SetItemText(i, 0, m_pro[i]);
					m_proList.SetItemText(i, 1, m_pro1[i]);
					m_proList.SetItemText(i, 2, m_pro2[i]);
					index++;
				}
			}
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}





void CMyDialog3::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);
	CMenu* pSub = menu.GetSubMenu(0);

	POINT pt;
	GetCursorPos(&pt);
	pSub->TrackPopupMenu(TPM_RIGHTALIGN, pt.x, pt.y, this);
	*pResult = 0;
}


void CMyDialog3::OnProcesslist()
{
	// TODO: 在此添加命令处理程序代码
	//MessageBox(_T("hello"));
	DWORD dwIndex = m_proList.GetSelectionMark();
	//::SendMessage(::GetDlgItem(this->GetSafeHwnd(), IDD_DIALOG6),WM_USER+1,NULL,m_pro[dwIndex]);
	ThreadDialog* tdl = new ThreadDialog();
	tdl->Create(IDD_DIALOG6,this);
	tdl->SendMessage(WM_USER+1,NULL,m_pro[dwIndex]);
	tdl->ShowWindow(SW_SHOW);
}


void CMyDialog3::OnModul()
{
	// TODO: 在此添加命令处理程序代码
	DWORD dwIndex = m_proList.GetSelectionMark();
	//::SendMessage(::GetDlgItem(this->GetSafeHwnd(), IDD_DIALOG6),WM_USER+1,NULL,m_pro[dwIndex]);
	ModulDialog* mdl = new ModulDialog();
	mdl->Create(IDD_DIALOG7, this);
	mdl->SendMessage(WM_USER + 2, NULL, m_pro[dwIndex]);
	mdl->ShowWindow(SW_SHOW);
}


void CMyDialog3::OnModule()
{
	// TODO: 在此添加命令处理程序代码
	DWORD dwIndex = m_proList.GetSelectionMark();
	//::SendMessage(::GetDlgItem(this->GetSafeHwnd(), IDD_DIALOG6),WM_USER+1,NULL,m_pro[dwIndex]);
	HeapDialog* hdl = new HeapDialog();
	hdl->Create(IDD_DIALOG8, this);
	hdl->SendMessage(WM_USER + 3, NULL, m_pro[dwIndex]);
	hdl->ShowWindow(SW_SHOW);
}


void CMyDialog3::OncloseProcess()
{
	// TODO: 在此添加命令处理程序代码
	DWORD dwIndex = m_proList.GetSelectionMark();
	HANDLE hProcess = OpenProcess(
		PROCESS_ALL_ACCESS,
		FALSE,
		m_pro[dwIndex]
	);
	//结束进程
	TerminateProcess(hProcess, 0);
	//关闭句柄
	CloseHandle(hProcess);
}

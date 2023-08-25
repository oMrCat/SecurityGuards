// ModulDialog.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "ModulDialog.h"
#include "afxdialogex.h"
#include<TlHelp32.h>
#include<iostream>

vector<CString>Mo;
vector<CString>Mo1;
vector<CString>Mo2;
// ModulDialog 对话框
DWORD dwModulId;
IMPLEMENT_DYNAMIC(ModulDialog, CDialogEx)

ModulDialog::ModulDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG7, pParent)
{

}

ModulDialog::~ModulDialog()
{
}

void ModulDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_modullist);
}


BEGIN_MESSAGE_MAP(ModulDialog, CDialogEx)
	ON_MESSAGE(WM_USER + 2, &ModulDialog::OnUser_Modul)
END_MESSAGE_MAP()


// ModulDialog 消息处理程序


afx_msg LRESULT ModulDialog::OnUser_Modul(WPARAM wParam, LPARAM lParam)
{
	dwModulId = (DWORD)lParam;
	ModuleList();
	return 0;
}

BOOL ModulDialog::EnumModule()
{
	Mo.clear();
	Mo1.clear();
	Mo2.clear();
	m_modullist.DeleteAllItems();
	CString mo;
	CString mo1;
	CString mo2;
	HANDLE hModulSnapshot = 0;
	MODULEENTRY32 me = { 0 };
	me.dwSize = sizeof(MODULEENTRY32);
	hModulSnapshot = CreateToolhelp32Snapshot(
		TH32CS_SNAPMODULE,  //创建的是线程快照
		dwModulId            //进程ID，只有在创建模块，堆快照的时候，指定进程
	);
	if (hModulSnapshot == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	//3. 获取第一个线程的信息，如果获取成功，就可以继续往后获取
	BOOL bSuccess = Module32First(
		hModulSnapshot,//快照的句柄
		&me     //第一个进程的信息
	);
	//4. 循环遍历，所有的进程信息
	if (bSuccess == TRUE)
	{
		do
		{
			//HANDLE hThread = OpenProcess(THREAD_QUERY_INFORMATION | THREAD_QUERY_LIMITED_INFORMATION, FALSE, te.th32ThreadID);
			mo.Format(L"%8d", me.th32ModuleID);
			mo1.Format(L"%d", me.szModule);
			mo2.Format(L"%d", me.szExePath);
			Mo.push_back(mo);
			Mo1.push_back(mo1);
			Mo2.push_back(mo2);
		} while (Module32Next(hModulSnapshot,&me));
	}
	//关闭句柄
	CloseHandle(hModulSnapshot);
	return 0;
}
void ModulDialog::ModuleList()
{
	CRect rc = {};
	m_modullist.GetClientRect(&rc);
	// 通过调用 InsertColumn 函数添加列
	m_modullist.InsertColumn(0, _T("模块ID"), 0, rc.right / 3);
	m_modullist.InsertColumn(1, _T("Module"), 0, rc.right / 3);
	m_modullist.InsertColumn(2, _T("szExePath"), 0, rc.right / 3);
	m_modullist.SetExtendedStyle(LVS_EX_CHECKBOXES |
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	if (EnumModule() == 0)
	{
		static int index = 0;
		CString temp2;
		for (int i = 0; i < Mo.size(); i++)
		{
			if (Mo[i].GetLength() != 0)
			{
				temp2.Format(_T("%d"), Mo[i]);
				// 通过 InsertItem 函数添加行
				m_modullist.InsertItem(i, temp2);
				// 通过 SetItemText 函数给每一行设置内容
				m_modullist.SetItemText(i, 0, Mo[i]);
				m_modullist.SetItemText(i, 1, Mo1[i]);
				m_modullist.SetItemText(i, 2, Mo2[i]);
				index++;
			}
		}
	}

}
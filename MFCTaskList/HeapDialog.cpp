// HeapDialog.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "HeapDialog.h"
#include "afxdialogex.h"
#include<TlHelp32.h>
#include<iostream>


// HeapDialog 对话框
vector<CString>Hp;
vector<CString>Hp1;
DWORD dwHeapID;
IMPLEMENT_DYNAMIC(HeapDialog, CDialogEx)

HeapDialog::HeapDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG8, pParent)
{

}

HeapDialog::~HeapDialog()
{
}

void HeapDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_heaplist);
}


BEGIN_MESSAGE_MAP(HeapDialog, CDialogEx)
	ON_MESSAGE(WM_USER + 3, &HeapDialog::OnUser_Heap)
END_MESSAGE_MAP()


// HeapDialog 消息处理程序

BOOL HeapDialog::HeapList(DWORD dwID)
{
	Hp.clear();
	Hp1.clear();
	m_heaplist.DeleteAllItems();
	CString hp;
	CString hp1;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, dwID);

	HEAPLIST32 hlist = { sizeof(HEAPLIST32) };
	if (Heap32ListFirst(hSnap, &hlist))
	{

		//do
		//{
			HEAPENTRY32 hHeapEntry;
			ZeroMemory(&hHeapEntry, sizeof(HEAPENTRY32));
			hHeapEntry.dwSize = sizeof(HEAPENTRY32);
			if (Heap32First(&hHeapEntry, dwID, hlist.th32HeapID))
			{
				hp.Format(L"%8d", hHeapEntry.th32HeapID);
				Hp.push_back(hp);
				//do
				//{
					//hp1.Format(L"%d", hHeapEntry.dwBlockSize);
					//Hp1.push_back(hp1);
					//hHeapEntry.dwSize = sizeof(HEAPENTRY32);
				//} while (Heap32Next(&hHeapEntry));
			}
			//hHeapEntry.dwSize = sizeof(HEAPENTRY32);
		//} while (Heap32ListNext(hSnap, &hlist));
	}
	CloseHandle(hSnap);
	return TRUE;
}

void HeapDialog::ModuleList()
{
	CRect rc = {};
	m_heaplist.GetClientRect(&rc);
	// 通过调用 InsertColumn 函数添加列
	m_heaplist.InsertColumn(0, _T("HeapID"), 0, rc.right / 2);
	m_heaplist.InsertColumn(1, _T("BlockSize"), 0, rc.right / 2);
	m_heaplist.SetExtendedStyle(LVS_EX_CHECKBOXES |
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	if (HeapList(dwHeapID) == 0)
	{
		static int index = 0;
		CString temp2;
		for (int i = 0; i < Hp.size(); i++)
		{
			if (Hp[i].GetLength() != 0)
			{
				temp2.Format(_T("%d"), Hp[i]);
				// 通过 InsertItem 函数添加行
				m_heaplist.InsertItem(i, temp2);
				// 通过 SetItemText 函数给每一行设置内容
				m_heaplist.SetItemText(i, 0, Hp[i]);
				m_heaplist.SetItemText(i, 1, Hp1[i]);
				index++;
			}
		}
	}

}
afx_msg LRESULT HeapDialog::OnUser_Heap(WPARAM wParam, LPARAM lParam)
{
	dwHeapID = (DWORD)lParam;
	ModuleList();
	return 0;
}

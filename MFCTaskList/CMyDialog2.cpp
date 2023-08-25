 // CMyDialog2.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "CMyDialog2.h"
#include "afxdialogex.h"
#include<iostream>

// CMyDialog2 对话框
vector<CString>m_Filebuff;
vector<CString>m_Filebuff1;
vector<CString>m_Filebuff2;
vector<CString>m_Filebuff3;
CString tempok;
IMPLEMENT_DYNAMIC(CMyDialog2, CDialogEx)

CMyDialog2::CMyDialog2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
	, w_file(_T(""))
{

}

CMyDialog2::~CMyDialog2()
{
}

void CMyDialog2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, w_file);
	DDX_Control(pDX, IDC_LIST2, m_filelist);
}


BEGIN_MESSAGE_MAP(CMyDialog2, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyDialog2::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog2::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMyDialog2 消息处理程序
void EnumFile(const TCHAR* szPath)
{
	CString fn;
	CString fs;
	CString ft;
	WIN32_FIND_DATA  wfd = {};
	setlocale(LC_ALL, "CHS"); // 设置字符编码格式
	//找到第一个文件
	TCHAR szDirtoryPath[MAX_PATH] = { 0 };
	_stprintf_s(szDirtoryPath, _T("%s\\%s"), szPath, _T("*"));
	HANDLE hFindFile = FindFirstFile(szDirtoryPath, &wfd);
	if (hFindFile != INVALID_HANDLE_VALUE) {
		do {
			fn.Format(TEXT("%s:  \n"), wfd.cFileName);
			fs.Format(L"文件大小为:%d字节  ", wfd.nFileSizeLow);
			FILETIME  Time = { 0 };
			FileTimeToLocalFileTime(&wfd.ftCreationTime, &Time);
			SYSTEMTIME sysTime = { 0 };
			FileTimeToSystemTime(&Time, &sysTime);
			ft.Format(L"创建时间:%d年%d月%d日%d时%d分%d秒\n",
				sysTime.wYear,
				sysTime.wMonth, sysTime.wDay, sysTime.wHour,
				sysTime.wMinute, sysTime.wSecond);
			m_Filebuff.push_back(fn);
			m_Filebuff1.push_back(fs);
			m_Filebuff2.push_back(ft);
		} while (FindNextFile(hFindFile, &wfd));
	}

}
void CMyDialog2::FileAttribute()
{
	CRect rc = {};
	m_filelist.GetClientRect(&rc);
	// 通过调用 InsertColumn 函数添加列
	m_filelist.InsertColumn(0, _T("序号"), 0, rc.right / 5);
	m_filelist.InsertColumn(1, _T("文件名称"), 0, rc.right / 5);
	m_filelist.InsertColumn(2, _T("文件大小"), 0, rc.right / 5);
	m_filelist.InsertColumn(3, _T("文件创建时间"), 0, rc.right / 5);
	m_filelist.InsertColumn(4, _T("文件属性"), 0, rc.right / 5);
	m_filelist.SetExtendedStyle(LVS_EX_CHECKBOXES |
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
}

void CMyDialog2::OnBnClickedButton2()
{
	m_Filebuff.clear();
	m_Filebuff1.clear();
	m_Filebuff2.clear();
	UpdateData(TRUE);
	tempok.Format(L"%s", w_file);
	if (tempok.GetLength() != 0)
	{
		MessageBox(_T("seccess"));
		EnumFile(tempok);
	}
	m_filelist.DeleteAllItems();
	static int index = 0;
	CString temp2;
	for (int i = 0; i < m_Filebuff.size(); i++)
	{
		if (m_Filebuff[i].GetLength() != 0)
		{
			temp2.Format(_T("%d"), i);
			// 通过 InsertItem 函数添加行
			m_filelist.InsertItem(i, temp2);
			// 通过 SetItemText 函数给每一行设置内容
			m_filelist.SetItemText(i, 1, m_Filebuff[i]);
			m_filelist.SetItemText(i, 2, m_Filebuff1[i]);
			m_filelist.SetItemText(i, 3, m_Filebuff2[i]);
			index++;
		}
	}

	// TODO: 在此添加控件通知处理程序代码
}

void CMyDialog2::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Filebuff.clear();
	m_Filebuff1.clear();
	m_Filebuff2.clear();
	m_filelist.DeleteAllItems();
	EnumFile(_T("C:"));
	EnumFile(_T("D:"));
	static int index = 0;
	CString temp1;
	for (int i = 0; i < m_Filebuff.size(); i++)
	{
		if (m_Filebuff[i].GetLength() != 0)
		{
			temp1.Format(_T("%d"), i);
			// 通过 InsertItem 函数添加行
			m_filelist.InsertItem(i, temp1);
			// 通过 SetItemText 函数给每一行设置内容
			m_filelist.SetItemText(i, 1, m_Filebuff[i]);
			m_filelist.SetItemText(i, 2, m_Filebuff1[i]);
			m_filelist.SetItemText(i, 3, m_Filebuff2[i]);
			index++;
		}
	}
}

BOOL CMyDialog2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	FileAttribute();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

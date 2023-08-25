// CMyDialog.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "CMyDialog.h"
#include "afxdialogex.h"

// CMyDialog 对话框
vector<CString>m_buff;
IMPLEMENT_DYNAMIC(CMyDialog, CDialogEx)

CMyDialog::CMyDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CMyDialog::~CMyDialog()
{
}

void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_objList);
}

BOOL CALLBACK EnumWinProc(HWND hWnd, LPARAM lParam)
{
	//根据窗口句柄获取窗口名
	TCHAR buff[200] = {};
	GetWindowText(hWnd, buff, 200);//得到窗口名，可显示到界面中
	//判断窗口是否被隐藏
	if (IsWindowVisible(hWnd) == TRUE && wcslen(buff) != 0)
	{
		//窗口没有被隐藏且窗口标题长度不为0，则将窗口信息显示到界面中
		m_buff.push_back(buff);
	}
	return TRUE;
}

void CMyDialog::InsertList()
{
	CRect rc = {};
	m_objList.GetClientRect(&rc);
	// 通过调用 InsertColumn 函数添加列
	m_objList.InsertColumn(0, _T("序号"), 0, rc.right / 2);
	m_objList.InsertColumn(1, _T("窗口信息"), 0, rc.right / 2);
	m_objList.SetExtendedStyle(LVS_EX_CHECKBOXES |
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
}


void CMyDialog::InitListCtrl()
{

}

BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMyDialog 消息处理程序


//入口函数
BOOL CMyDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InsertList();
	//InitListCtrl();
	SetTimer(
		1,
		500, NULL
	);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



void CMyDialog::OnTimer(UINT_PTR nIDEvent)
{
	static int index = 0;
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		m_objList.DeleteAllItems();
		m_buff.clear();
		EnumWindows(EnumWinProc, 0);
		CString temp;
		for (int i = 0; i < m_buff.size(); i++)
		{
			if (m_buff[i].GetLength() != 0)
			{
				temp.Format(_T("%d"), index);
				// 通过 InsertItem 函数添加行
				m_objList.InsertItem(index, temp);
				// 通过 SetItemText 函数给每一行设置内容
				m_objList.SetItemText(index, 1, m_buff[i]);
				index++;
			}
		}
	}
	index = 0;
	CDialogEx::OnTimer(nIDEvent);
}

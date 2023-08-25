// CMyDialog1.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "CMyDialog1.h"
#include "afxdialogex.h"


// CMyDialog1 对话框
vector<CString>Fl;
IMPLEMENT_DYNAMIC(CMyDialog1, CDialogEx)

CMyDialog1::CMyDialog1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_fileedit(_T(""))
{

}

CMyDialog1::~CMyDialog1()
{
}

void CMyDialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_filelist);
	DDX_Text(pDX, IDC_EDIT1, m_fileedit);
}


BEGIN_MESSAGE_MAP(CMyDialog1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog1::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyDialog1::OnBnClickedButton2)
//	ON_BN_CLICKED(IDC_BUTTON3, &CMyDialog1::OnBnClickedButton3)
END_MESSAGE_MAP()


// CMyDialog1 消息处理程序

void CMyDialog1::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_filelist.DeleteAllItems();
	Fl.clear();
	CString fl;
	UpdateData(TRUE);
	fl.Format(L"%s", m_fileedit);
	GetFilePath(Fl, fl);
	CString tmpla;
	for (int i = 0; i < Fl.size(); i++)
	{
		tmpla.Format(_T("%d"), i);
		m_filelist.InsertItem(i, tmpla);
		m_filelist.SetItemText(i, 1, Fl[i]);
	}
	UpdateData(FALSE);
}


BOOL CMyDialog1::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	CRect rc1 = {};
	m_filelist.GetClientRect(&rc1);
	m_filelist.InsertColumn(0, _T("序号"), 0, rc1.right / 2);
	m_filelist.InsertColumn(1, _T("文件信息"), 0, rc1.right / 2);
	m_filelist.SetExtendedStyle(LVS_EX_CHECKBOXES |
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
void CMyDialog1::GetFilePath(vector<CString>& vFilePathList, CString strDir)
{
	CFileFind finder;
	BOOL isNotEmpty = finder.FindFile(strDir + _T("*.*"));//总文件夹，开始遍历 
	while (isNotEmpty)
	{
		isNotEmpty = finder.FindNextFile();//查找文件 
		CString filename = finder.GetFilePath();//获取文件的路径，可能是文件夹，可能是文件 
		if (!(finder.IsDirectory()))
		{
			//如果是文件则加入文件列表 
			vFilePathList.push_back(filename);//将一个文件路径加入容器 
		}
		else
		{
			//递归遍历用户文件夹，跳过非用户文件夹 
			if (!(finder.IsDots() || finder.IsHidden() || finder.IsSystem() || finder.IsTemporary() || finder.IsReadOnly()))
			{
				GetFilePath(vFilePathList, filename + _T("/"));
			}
		}
	}
}

void CMyDialog1::OnBnClickedButton2()
{
	for (int i = 0; i < Fl.size(); i++)
	{
		LPWSTR FileSuffix = PathFindExtension(Fl[i]);
		if (!lstrcmp(FileSuffix,L".txt")|| !lstrcmp(FileSuffix, L".tlog") || !lstrcmp(FileSuffix, L".obj") || 
			!lstrcmp(FileSuffix, L".log") || !lstrcmp(FileSuffix, L".pch") || !lstrcmp(FileSuffix, L".ilk") ||
			!lstrcmp(FileSuffix, L".pdb") )
		{
			DeleteFile(Fl[i]);
		}
	}
}



//void CMyDialog1::OnBnClickedButton3()
//{
//	// TODO: 在此添加控件通知处理程序代码
//
//}

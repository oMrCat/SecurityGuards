// CDialogMod.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "CDialogMod.h"
#include "afxdialogex.h"


// CDialogMod 对话框

IMPLEMENT_DYNAMIC(CDialogMod, CDialogEx)

CDialogMod::CDialogMod(int p,CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDialogMod, pParent),pid(p)
{

}

CDialogMod::~CDialogMod()
{
}

void CDialogMod::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogMod, CDialogEx)
END_MESSAGE_MAP()


// CDialogMod 消息处理程序


BOOL CDialogMod::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString temp;
	temp.Format(L"%d", pid);
	MessageBox(temp);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

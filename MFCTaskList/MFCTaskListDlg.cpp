
// MFCTaskListDlg.cpp: 实现文件
//

#include "pch.h"
#include <Psapi.h>
#include <powrprof.h>
#include "framework.h"
#include "MFCTaskList.h"
#include "MFCTaskListDlg.h"
#include "afxdialogex.h"
#include"CMyDialog.h"
#include"CMyDialog1.h"
#include"CMyDialog2.h"
#include"CMyDialog3.h"
#include"CMyDialog4.h"
#include"CMyAntivrusDlg.h"
#include"LOAD_PE.h"
#pragma comment(lib, "powrprof.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
public:
	//	afx_msg void OnCloseThread();
//	afx_msg void On32781();
//	afx_msg void OnDropFiles(HDROP hDropInfo);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	//	ON_COMMAND(ID_32780, &CAboutDlg::OnCloseThread)
//	ON_COMMAND(ID_32781, &CAboutDlg::On32781)
//ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CMFCTaskListDlg 对话框



CMFCTaskListDlg::CMFCTaskListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCTASKLIST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCTaskListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CMFCTaskListDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//	ON_WM_TIMER()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCTaskListDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCTaskListDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCTaskListDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCTaskListDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCTaskListDlg::OnBnClickedButton5)
//	ON_WM_HOTKEY()
ON_BN_CLICKED(IDC_BUTTON6, &CMFCTaskListDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CMFCTaskListDlg 消息处理程序

BOOL CMFCTaskListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_tab.InitTabCtrl(7,
		_T("窗口信息"), new CMyDialog, IDD_DIALOG1,
		_T("VS扫描清理"), new CMyDialog1, IDD_DIALOG2,
		_T("文件信息"), new CMyDialog2, IDD_DIALOG3,
		_T("进程"), new CMyDialog3, IDD_DIALOG4,
		_T("服务"), new CMyDialog4, IDD_DIALOG5,
		_T("病毒查杀"), new CMyAntivrusDlg, IDD_DIALOG9,
		_T("LOAD_PE"), new LOAD_PE, IDD_DIALOG10
	);
	SetTimer(
		22, 500, NULL
	);
	SetTimer(
		33, 500, NULL
	);
	//初始化状态栏
	UINT buf[4] = { 1, 2, 3 ,4 };
	m_status.Create(this);
	m_status.SetIndicators(buf, 4);
	m_status.SetPaneInfo(0, 1, 0, 150);
	m_status.SetPaneInfo(1, 2, 0, 200);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	
	//注册全局热键
	::RegisterHotKey(this->GetSafeHwnd(),//获取当前句柄的窗口
		0x1234,								//自定义快捷键标识
		MOD_CONTROL | MOD_SHIFT,			//同时按下CTRL，shift
		'H'									//ctrl+shift+H
	);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//cpu 占有率
void CMFCTaskListDlg::GetCpuState(int& nStr)
{
	static LONGLONG nFree{}, nNt{}, nUser{};
	static LONGLONG nFreet{}, nNtt{}, nUsert{};
	static DOUBLE nHave{};
	GetSystemTimes((PFILETIME)&nFree, (PFILETIME)&nNt, (PFILETIME)&nUser);
	nHave = (DOUBLE)(nFree - nFreet) / (DOUBLE)(nNt - nNtt + nUser - nUsert);
	nStr = DWORD(100.0 - nHave * 100);
	nFreet = nFree;
	nNtt = nNt;
	nUsert = nUser;
}

//物理内存占有率
DWORD CMFCTaskListDlg::GetMemState()
{
	MEMORYSTATUSEX nStatex;

	nStatex.dwLength = sizeof(nStatex);

	GlobalMemoryStatusEx(&nStatex);

	DOUBLE nAll = (DOUBLE)nStatex.ullTotalPhys / 1048576;
	DOUBLE nHave = (DOUBLE)nStatex.ullAvailPhys / 1048576;
	return (DWORD)((nAll - nHave) / nAll * 100);

}
void CMFCTaskListDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCTaskListDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCTaskListDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//void CMFCTaskListDlg::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	if
//
//	CDialogEx::OnTimer(nIDEvent);
//}


void CMFCTaskListDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 22)
	{
		int nRandom = 0;
		GetCpuState(nRandom);
		cp.Format(_T("cpu的占有率：%d"), nRandom);
		//DWORD nNum = GetMemState();
		m_status.SetPaneText(0, cp);

	}
	if (nIDEvent == 33)
	{
		int nRandom = 0;
		cs.Format(_T("物理内存的占有率：%d"), GetMemState());
		//DWORD nNum = GetMemState();

		m_status.SetPaneText(1, cs);

	}
	CDialogEx::OnTimer(nIDEvent);
}




//void CAboutDlg::OnCloseThread()
//{
//	// TODO: 在此添加命令处理程序代码
//}


void CMFCTaskListDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取当前的内存状态
	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG preUsedMem =
		stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	//清理内存
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA,
			false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);
	}
}


void CMFCTaskListDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//关机
	HANDLE hToken = NULL;
	HANDLE hProcess = GetCurrentProcess(); // 这个函数是得到当前进程的伪句柄
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp = { 0 };
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	// 调用函数提升权限
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
	ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, SHTDN_REASON_MAJOR_APPLICATION);
}


void CMFCTaskListDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	//重启
	HANDLE hToken = NULL;
	HANDLE hProcess = GetCurrentProcess(); // 这个函数是得到当前进程的伪句柄
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp = { 0 };
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	// 调用函数提升权限
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, NULL);
}


void CMFCTaskListDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	//注销
	HANDLE hToken = NULL;
	HANDLE hProcess = GetCurrentProcess(); // 这个函数是得到当前进程的伪句柄
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp = { 0 };
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	// 调用函数提升权限
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
	ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, NULL);
}


void CMFCTaskListDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	//休眠
	SetSuspendState(TRUE, FALSE, FALSE);
}


//void CAboutDlg::On32781()
//{
//	// TODO: 在此添加命令处理程序代码
//	
//}




//注册全局热键
BOOL CMFCTaskListDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//判断是全局热键的同时也是当前注册的键
	if ((pMsg->message==WM_HOTKEY)&&(pMsg->wParam==0x1234))
	{
		//隐藏窗口的代码
		if (m_IsWindowsHide==TRUE)
		{
			ShowWindow(SW_HIDE);
			m_IsWindowsHide = FALSE;
		}
		//显示窗口的代码
		else
		{
			ShowWindow(SW_SHOW);
			m_IsWindowsHide = TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


//void CMFCTaskListDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
//}


void CMFCTaskListDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	//初始化SHQUERYRBINFO结构体
	SHQUERYRBINFO RecycleBinInfo = {};
	RecycleBinInfo.cbSize = sizeof(RecycleBinInfo);
	//查询回收站信息
	SHQueryRecycleBin(NULL, &RecycleBinInfo);
	//清空回收站
	SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
}

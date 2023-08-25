// CMyDialog4.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "CMyDialog4.h"
#include "afxdialogex.h"
#include<winsvc.h>
#pragma comment(lib,"Advapi32.lib")

// CMyDialog4 对话框
vector<ENUM_SERVICE_STATUS>m_vecSerInfo;
CString m_ServiceName;
DWORD m_SelectNum;

IMPLEMENT_DYNAMIC(CMyDialog4, CDialogEx)

CMyDialog4::CMyDialog4(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{

}

CMyDialog4::~CMyDialog4()
{
}

void CMyDialog4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, ser_List);
}


BEGIN_MESSAGE_MAP(CMyDialog4, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog4::OnBnClickedButton1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMyDialog4::OnRclickList1)
	ON_COMMAND(ID_32781, &CMyDialog4::On32781)
	ON_COMMAND(ID_32782, &CMyDialog4::On32782)
END_MESSAGE_MAP()


// CMyDialog4 消息处理程序

void CMyDialog4::GetServiceInfo()
{
	//1.打开远程计算机服务控制管理器
	ser_List.DeleteAllItems();
	m_vecSerInfo.clear();
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//2.第一次调用，获取需要的内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,  //所有服务状态
		NULL,        //缓冲区
		0,          //缓冲区大小
		&dwSize,      //需要的大小
		&dwServiceNum,    //缓冲区中的服务个数
		NULL);
	//3.申请需要的内存，第二次调用
	LPENUM_SERVICE_STATUS lpEnumService =
		(LPENUM_SERVICE_STATUS)LocalAlloc(LPTR, dwSize);
	//4.第二次枚举
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,  //所有服务状态
		lpEnumService,    //缓冲区
		dwSize,        //缓冲区大小
		&dwSize,      //需要的大小
		&dwServiceNum,    //缓冲区中的服务个数
		NULL);
	//5.遍历信息
	SC_HANDLE hService = nullptr;
	CString Temp;
	CString Temp1;
	CString Temp2;
	CString Temp3;
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		static int index = 0;
		//获取基础信息
		//1.服务名称
		//2.服务描述
		//3.服务状态(根据得到的值手动输出字符串)
		//“已停止” “正在运行" "正在暂停"...
		m_vecSerInfo.push_back(lpEnumService[i]);
		//获取更多信息
		//1.打开服务
		hService = OpenService(hSCM,
			lpEnumService[i].lpServiceName, SERVICE_QUERY_CONFIG);
		// 2.第一次调用获取需要的缓冲区大小
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		//分配内存
		LPQUERY_SERVICE_CONFIG pServiceConfig =
			(LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, dwSize);
		//3.第二次调用,获取信息
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);
		Temp.Format(L"%d", index);
		Temp1.Format(L"%d", m_vecSerInfo[i].ServiceStatus.dwCurrentState);
		Temp2.Format(L"%d", pServiceConfig->dwStartType);
		Temp3.Format(L"%d", m_vecSerInfo[i].ServiceStatus.dwServiceType);
		//获取服务信息完毕，开始插入List
		ser_List.InsertItem(i, Temp);
		ser_List.SetItemText(i,1,m_vecSerInfo[i].lpServiceName);
		ser_List.SetItemText(i,2, m_vecSerInfo[i].lpDisplayName);
		ser_List.SetItemText(i,3, Temp1);
		ser_List.SetItemText(i,4, Temp2);
		ser_List.SetItemText(i,5, Temp3);
		LocalFree(pServiceConfig);
		index++;
	}
	LocalFree(lpEnumService);
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hService);
}


BOOL CMyDialog4::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rc = {};
	ser_List.GetClientRect(&rc);
	// 通过调用 InsertColumn 函数添加列
	ser_List.InsertColumn(0, _T("number"), 0, rc.right / 6);
	ser_List.InsertColumn(1, _T("ServiceName"), 0, rc.right / 6);
	ser_List.InsertColumn(2, _T("DisplayName"), 0, rc.right / 6);
	ser_List.InsertColumn(3, _T("ServiceStatus"), 0, rc.right / 6);
	ser_List.InsertColumn(4, _T("StartType"), 0, rc.right / 6);
	ser_List.InsertColumn(5, _T("ServiceType"), 0, rc.right / 6);
	ser_List.SetExtendedStyle(LVS_EX_CHECKBOXES |
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	GetServiceInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CMyDialog4::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	GetServiceInfo();
}


void CMyDialog4::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//防止在空白区点击弹出菜单  
	int n = pNMItemActivate->iItem;//选择了第几行
	if (n != -1)
	{
		//保存所选服务的 服务名
		m_ServiceName = ser_List.GetItemText(n, 1);
		m_SelectNum = n;
		//下面的这段代码, 不单单适应于ListCtrl  
		CMenu menu, * pPopup;
		menu.LoadMenu(IDR_MENU3);
		pPopup = menu.GetSubMenu(0);
		CPoint myPoint;
		ClientToScreen(&myPoint);
		GetCursorPos(&myPoint);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, myPoint.x, myPoint.y, this);
	}
	*pResult = 0;
}

void CMyDialog4::UpDataSerInfo()
{
	//1.打开远程计算机服务控制管理器
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//2.第一次调用，获取需要的内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//所有服务状态
		NULL,				//缓冲区
		0,					//缓冲区大小
		&dwSize,			//需要的大小
		&dwServiceNum,		//缓冲区中的服务个数
		NULL);
	//3.申请需要的内存，第二次调用
	LPENUM_SERVICE_STATUS lpEnumService =
		(LPENUM_SERVICE_STATUS)LocalAlloc(LPTR, dwSize);
	//4.第二次枚举
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//所有服务状态
		lpEnumService,		//缓冲区
		dwSize,				//缓冲区大小
		&dwSize,			//需要的大小
		&dwServiceNum,		//缓冲区中的服务个数
		NULL);
	//5.遍历信息
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		//如果是要更新的服务
		if (!_tcscmp(lpEnumService[i].lpServiceName, m_ServiceName))
		{
			//更新list中该服务的状态信息
			switch (lpEnumService[i].ServiceStatus.dwCurrentState)
			{
			case SERVICE_STOPPED:
				ser_List.SetItemText(m_SelectNum, 3, _T("已停止"));
				break;
			case SERVICE_STOP_PENDING:
				ser_List.SetItemText(m_SelectNum, 3, _T("正在停止"));
				break;
			case SERVICE_START_PENDING:
				ser_List.SetItemText(m_SelectNum, 3, _T("正在启动"));
				break;
			case SERVICE_RUNNING:
				ser_List.SetItemText(m_SelectNum, 3, _T("正在运行"));
				break;
			case SERVICE_PAUSED:
				ser_List.SetItemText(m_SelectNum, 3, _T("已暂停"));
				break;
			case SERVICE_PAUSE_PENDING:
				ser_List.SetItemText(m_SelectNum, 3, _T("正在暂停"));
				break;
			case SERVICE_CONTINUE_PENDING:
				ser_List.SetItemText(m_SelectNum, 3, _T("准备继续"));
				break;
			}
		}
	}
	LocalFree(lpEnumService);
	CloseServiceHandle(hSCM);
}
void CMyDialog4::On32781()
{
	// TODO: 在此添加命令处理程序代码
	//启动服务
	SC_HANDLE hSCM = OpenSCManager(			//打开服务控制管理器
		NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE hService = OpenService(hSCM,	//打开服务
		m_ServiceName, SERVICE_START);
	StartService(hService, 0, 0);			//启动服务
	//更新信息
	UpDataSerInfo();
	//m_IsUpDateSer = TRUE;
	//m_UpDataTimes = 5;
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hService);
}


void CMyDialog4::On32782()
{
	// TODO: 在此添加命令处理程序代码
	//停止服务
	SC_HANDLE hSCM = OpenSCManager(			//打开服务控制管理器
		NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE hService = OpenService(hSCM,	//打开服务
		m_ServiceName, SERVICE_STOP);
	SERVICE_STATUS status;
	ControlService(hService,				//结束服务
		SERVICE_CONTROL_STOP,
		&status);
	//更新信息
	UpDataSerInfo();
	//m_IsUpDateSer = TRUE;
	//m_UpDataTimes = 5;
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hService);
}

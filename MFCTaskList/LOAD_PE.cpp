// LOAD_PE.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "LOAD_PE.h"
#include "afxdialogex.h"
#include "LOAD_PE_Ps.h"


// LOAD_PE 对话框

IMPLEMENT_DYNAMIC(LOAD_PE, CDialogEx)

LOAD_PE::LOAD_PE(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG10, pParent)
{

}

LOAD_PE::~LOAD_PE()
{
}

void LOAD_PE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LOAD_PE, CDialogEx)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// LOAD_PE 消息处理程序

//判断是不是PE文件
BOOL LOAD_PE::IsPE_File(char* lpImage)
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpImage;
	if (pDos->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return FALSE;
	}
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + lpImage);
	if (pNt->Signature != IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}
	return TRUE;
}


//相应文件拖拽
void LOAD_PE::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 存放文件路径
	wchar_t filePath[MAX_PATH] = {};

	// 获取拖拽文件的路径
	DragQueryFile(hDropInfo, 0, filePath, MAX_PATH);

	//WCHAR* wFilePath = filePath;

	//1 打开文件
	HANDLE hFile = CreateFile(
		filePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	//2 获取文件大小
	DWORD dwFileSize = GetFileSize(hFile, NULL);

	//3 申请空间并初始化
	char*buf = new char[dwFileSize] {0};
	//4 读取文件
	DWORD dwRealSize = 0;
	ReadFile(hFile, buf, dwFileSize, &dwRealSize, NULL);
	CloseHandle(hFile);
	//5 判断是不是PE文件
	if (IsPE_File(buf) == TRUE)
	{
		LOAD_PE_Ps* LOAD_PE_Show = new LOAD_PE_Ps;
		LOAD_PE_Show->Create(IDD_DIALOG11,this);
		LOAD_PE_Show->SendMessage(WM_USER + 4, NULL, (LPARAM)buf);
		LOAD_PE_Show->ShowWindow(SW_SHOW);
	}
	else
	{
		MessageBox(L"错误的");
	}
	CDialogEx::OnDropFiles(hDropInfo);
}

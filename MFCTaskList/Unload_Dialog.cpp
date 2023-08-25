// Unload_Dialog.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "Unload_Dialog.h"
#include "afxdialogex.h"


// Unload_Dialog 对话框
char* Ul_Buff;
IMPLEMENT_DYNAMIC(Unload_Dialog, CDialogEx)

Unload_Dialog::Unload_Dialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG17, pParent)
	, Edit_Unload(_T(""))
{

}

Unload_Dialog::~Unload_Dialog()
{
}

void Unload_Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Edit_Unload);
}


BEGIN_MESSAGE_MAP(Unload_Dialog, CDialogEx)
	ON_MESSAGE(WM_USER + 10, &Unload_Dialog::OnUser_Unload)
END_MESSAGE_MAP()


// Unload_Dialog 消息处理程序


DWORD Ul_rva_to_foa(char* buff, DWORD rva)
{
	// 先将缓冲区转换成 dos 头
	PIMAGE_DOS_HEADER dos_head = (PIMAGE_DOS_HEADER)buff;
	// 根据 dos 头找到 n t 头
	PIMAGE_NT_HEADERS nt_head =
		PIMAGE_NT_HEADERS(buff + dos_head->e_lfanew);
	// 获取文件头
	PIMAGE_FILE_HEADER file_head = &nt_head->FileHeader;
	// 区段的数量
	DWORD section_count = file_head->NumberOfSections;
	// 获取到区段头表
	PIMAGE_SECTION_HEADER section_head =
		IMAGE_FIRST_SECTION(nt_head);
	for (int i = 0; i < section_count; i++)
	{
		// 如果 rva 在本区段，就计算段内偏移
		// 加上文件中的偏移,
		// 为什么用 内存的起始地址 + 文件大小，
		// 因为部分数据，在文件中没有，用内存大小计算这个范围，
		// 可能出现问题
		if (rva >= section_head->VirtualAddress &&
			rva <= section_head->VirtualAddress +
			section_head->SizeOfRawData
			)
		{
			// 根据公式计算 FOA 
			// RVA - 区段起始地址 = FOA - 区段文件起始地址
			DWORD foa = rva - section_head->VirtualAddress
				+ section_head->PointerToRawData;
			return foa;
		}
		section_head++;
	}
	return 0;
}


void Unload_Dialog::GetUnloadInfo(char* buff)
{
	//先将缓冲区转换成dos头
	PIMAGE_DOS_HEADER dos_head = (PIMAGE_DOS_HEADER)buff;
	//根据dos头找到nt头
	PIMAGE_NT_HEADERS nt_head =
		PIMAGE_NT_HEADERS(buff + dos_head->e_lfanew);
	//获取文件头
	PIMAGE_FILE_HEADER file_head = &nt_head->FileHeader;
	//获取扩展头
	PIMAGE_OPTIONAL_HEADER optional_head = &nt_head->OptionalHeader;
	//获取数据目录表
	PIMAGE_DATA_DIRECTORY DataDirectory = optional_head->DataDirectory;

	DWORD dwFOA = Ul_rva_to_foa(buff, DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress);
	PIMAGE_DELAYLOAD_DESCRIPTOR pUnload = (PIMAGE_DELAYLOAD_DESCRIPTOR)(dwFOA + buff);
	CString cs;
	cs.Format(L"DllNameRVA:%08x\r\n", pUnload->DllNameRVA);
	Edit_Unload += cs;
	cs.Format(L"ModuleHandleRVA:%08x\r\n", pUnload->ModuleHandleRVA);
	Edit_Unload += cs;
	cs.Format(L"ImportAddressTableRVA:%08x\r\n", pUnload->ImportAddressTableRVA);
	Edit_Unload += cs;
	cs.Format(L"ImportNameTableRVA:%08x\r\n", pUnload->ImportNameTableRVA);
	Edit_Unload += cs;
	cs.Format(L"BoundImportAddressTableRVA:%08x\r\n", pUnload->BoundImportAddressTableRVA);
	Edit_Unload += cs;
	cs.Format(L"UnloadInformationTableRVA:%08x\r\n", pUnload->UnloadInformationTableRVA);
	Edit_Unload += cs;

	UpdateData(FALSE);
}


afx_msg LRESULT Unload_Dialog::OnUser_Unload(WPARAM wParam, LPARAM lParam)
{
	Ul_Buff = (char*)lParam;
	GetUnloadInfo(Ul_Buff);
	return 0;
}

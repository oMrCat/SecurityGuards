// Rection_Dialog.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "Rection_Dialog.h"
#include "afxdialogex.h"


// Rection_Dialog 对话框
char* R_Buff;

IMPLEMENT_DYNAMIC(Rection_Dialog, CDialogEx)

Rection_Dialog::Rection_Dialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG14, pParent)
	, Edit_Rection(_T(""))
{

}

Rection_Dialog::~Rection_Dialog()
{
}

void Rection_Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Edit_Rection);
}


BEGIN_MESSAGE_MAP(Rection_Dialog, CDialogEx)
	ON_MESSAGE(WM_USER + 7, &Rection_Dialog::OnUser_Rection)
END_MESSAGE_MAP()


// Rection_Dialog 消息处理程序


// RVA 转换成 FOA 
DWORD r_rva_to_foa(char* buff, DWORD rva)
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

typedef struct _TYPEOFFSET {
	WORD Offset : 12;  // (1) 大小为12Bit的重定位偏移
	WORD Type : 4;   // (2) 大小为4Bit的重定位信息类型值
}TYPEOFFSET, * PTYPEOFFSET;

//解析重定位表
void Rection_Dialog::get_relocation_infomation(char* buff)
{
	CString Base_Rva;
	CString Count;
	CString Index;
	CString FOA;
	CString r_data;
	CString split;
	//先将缓冲区转换成dos头
	PIMAGE_DOS_HEADER dos_head = (PIMAGE_DOS_HEADER)buff;
	//根据dos头找到nt头
	PIMAGE_NT_HEADERS nt_head =
		PIMAGE_NT_HEADERS(buff + dos_head->e_lfanew);
	//获取扩展头
	PIMAGE_OPTIONAL_HEADER optional_head = &nt_head->OptionalHeader;

	//在数据目录表的第6项获取资源
	DWORD reloc_rva = optional_head->DataDirectory[5].VirtualAddress;
	DWORD reloc_foa = r_rva_to_foa(buff, reloc_rva);

	// 获取重定位表的第一项
	PIMAGE_BASE_RELOCATION rel =
		(PIMAGE_BASE_RELOCATION)(reloc_foa + (DWORD)buff);

	while (rel->SizeOfBlock != 0)
	{
		DWORD count = (rel->SizeOfBlock - 8) / 2;
		DWORD base_rva = rel->VirtualAddress;
		Base_Rva.Format(L"基址:%08X\r\n", base_rva);
		Count.Format(L"需要重定位的项数:%x\r\n",count);
		Edit_Rection += Base_Rva;
		Edit_Rection += Count;
		UpdateData(FALSE);
		PTYPEOFFSET first_table = (PTYPEOFFSET)(rel + 1);
		// 开始遍历每一项
		for (int i = 0; i < count; i++)
		{
			// 如果类型为 3 ，就是需要重定位的数据
			if (first_table[i].Type == 3)
			{
				// 计算出真正需要重定位的数据 RAV 
				DWORD real_rva =
					rel->VirtualAddress + first_table[i].Offset;
				DWORD real_foa = r_rva_to_foa(buff, real_rva);

				PDWORD data = PDWORD(real_foa + (DWORD)buff);

				Index.Format(L"index:%d\r\n",i);
				FOA.Format(L"FOA:%X\r\n",real_foa);
				r_data.Format(L"data:%x\r\n",*data);
				Edit_Rection += Index;
				Edit_Rection += FOA;
				Edit_Rection += r_data;
				UpdateData(FALSE);
			}
		}
		split.Format(L"------------------------\r\n");
		Edit_Rection += split;
		UpdateData(FALSE);
		// 找到下一页
		rel = PIMAGE_BASE_RELOCATION((DWORD)rel + rel->SizeOfBlock);
	}

}

afx_msg LRESULT Rection_Dialog::OnUser_Rection(WPARAM wParam, LPARAM lParam)
{
	R_Buff = (char*)lParam;
	get_relocation_infomation(R_Buff);
	return 0;
}

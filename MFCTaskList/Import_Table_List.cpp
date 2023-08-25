// Import_Table_List.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "Import_Table_List.h"
#include "afxdialogex.h"


// Import_Table_List 对话框
char* Buff;

IMPLEMENT_DYNAMIC(Import_Table_List, CDialogEx)

Import_Table_List::Import_Table_List(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG12, pParent)
	, Import_List(_T(""))
{

}

Import_Table_List::~Import_Table_List()
{
}

void Import_Table_List::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Import_List);
}


BEGIN_MESSAGE_MAP(Import_Table_List, CDialogEx)
	ON_MESSAGE(WM_USER + 5, &Import_Table_List::OnUser_Import)
END_MESSAGE_MAP()


// Import_Table_List 消息处理程序

// RVA 转换成 FOA 
DWORD I_rva_to_foa(char* buff, DWORD rva)
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
// 解析导入表
void Import_Table_List::get_import_information(char* import_buff)
{
	// 先将缓冲区转换成 dos 头
	PIMAGE_DOS_HEADER dos_head = (PIMAGE_DOS_HEADER)import_buff;
	// 根据 dos 头找到 n t 头
	PIMAGE_NT_HEADERS nt_head =
		PIMAGE_NT_HEADERS(import_buff + dos_head->e_lfanew);
	// 获取文件头
	PIMAGE_OPTIONAL_HEADER optional_head = &nt_head->OptionalHeader;

	// 导出表在数据目录表的第 0 项
	DWORD import_rva = optional_head->DataDirectory[1].VirtualAddress;
	DWORD import_foa = I_rva_to_foa(import_buff, import_rva);

	// 获取到导出表的数据
	PIMAGE_IMPORT_DESCRIPTOR import_table =
		(PIMAGE_IMPORT_DESCRIPTOR)(import_foa + (DWORD)import_buff);

	CString Import_Name;
	CString Hint;
	CString Name;
	CString Ordinal;
	while (import_table->Name != 0)
	{
		// 先获取到导入模块的名称
		DWORD name_foa = I_rva_to_foa(import_buff, import_table->Name);
		PCHAR name = (PCHAR)(name_foa + (DWORD)import_buff);
		Import_Name.Format(L"Name:%S\r\n", name);
		Import_List += Import_Name;
		UpdateData(FALSE);
		// 获取到导入地址表 IMAGE_THUNK_DATA
		DWORD iat_foa = I_rva_to_foa(import_buff, import_table->FirstThunk);
		PIMAGE_THUNK_DATA iat =
			(PIMAGE_THUNK_DATA)(iat_foa + (DWORD)import_buff);
		while (iat->u1.AddressOfData != 0)
		{
			BOOL is_only_ordinal = IMAGE_SNAP_BY_ORDINAL(iat->u1.Ordinal);
			if (!is_only_ordinal)
			{
				// 否则就是名称导入
				DWORD name_table_foa =
					I_rva_to_foa(import_buff, iat->u1.AddressOfData);
				PIMAGE_IMPORT_BY_NAME name_table =
					(PIMAGE_IMPORT_BY_NAME)(name_table_foa + (DWORD)import_buff);
				Hint.Format(L"序号：%X\r\n",name_table->Hint);
				Import_List += Hint;
				Name.Format(L"函数名 :%S\r\n",name_table->Name);
				Import_List += Name;
				UpdateData(FALSE);
			}
			// 判断最高位是否为1，如果为1就是序号导入
			else
			{
				Ordinal.Format(L"导入序号为 %X\r\n",iat->u1.Ordinal & 0x7FFFFFFF);
				UpdateData(FALSE);
			}
			iat++;
		}
		import_table++;
	}

}



afx_msg LRESULT Import_Table_List::OnUser_Import(WPARAM wParam, LPARAM lParam)
{
	Buff = (char*)lParam;
	get_import_information(Buff);
	return 0;
}


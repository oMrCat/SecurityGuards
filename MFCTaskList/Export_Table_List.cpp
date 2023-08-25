// Export_Table_List.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "Export_Table_List.h"
#include "afxdialogex.h"


// Export_Table_List 对话框
char* E_Buff;
IMPLEMENT_DYNAMIC(Export_Table_List, CDialogEx)

Export_Table_List::Export_Table_List(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG13, pParent)
	, Edit_Export(_T(""))
{

}

Export_Table_List::~Export_Table_List()
{
}

void Export_Table_List::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Edit_Export);
}


BEGIN_MESSAGE_MAP(Export_Table_List, CDialogEx)
	ON_MESSAGE(WM_USER + 6, &Export_Table_List::OnUser_Export)
END_MESSAGE_MAP()


// Export_Table_List 消息处理程序

// RVA 转换成 FOA 
DWORD E_rva_to_foa(char* buff, DWORD rva)
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


// 解析导出表
void Export_Table_List::get_export_information(char* buff)
{
	CString Name,Address,E_Name,Number;
	// 先将缓冲区转换成 dos 头
	PIMAGE_DOS_HEADER dos_head = (PIMAGE_DOS_HEADER)buff;
	// 根据 dos 头找到 n t 头
	PIMAGE_NT_HEADERS nt_head =
		PIMAGE_NT_HEADERS(buff + dos_head->e_lfanew);
	// 获取文件头
	PIMAGE_OPTIONAL_HEADER optional_head = &nt_head->OptionalHeader;

	// 导出表在数据目录表的第 0 项
	DWORD export_rva = optional_head->DataDirectory[0].VirtualAddress;
	DWORD export_foa = E_rva_to_foa(buff, export_rva);

	// 获取到导出表的数据
	PIMAGE_EXPORT_DIRECTORY export_table =
		(PIMAGE_EXPORT_DIRECTORY)(export_foa + (DWORD)buff);

	// 解析内容
	DWORD name_foa = E_rva_to_foa(buff, export_table->Name);
	PCHAR name = (PCHAR)(name_foa + (DWORD)buff);
	Name.Format(L"导出模块的名称：%S\r\n", name);
	Edit_Export += Name;
	UpdateData(FALSE);
	// 获取到序号的基址
	DWORD base = export_table->Base;
	// 获取到两张表的数量
	DWORD address_count = export_table->NumberOfFunctions;
	DWORD name_count = export_table->NumberOfNames;
	// 遍历三张表，获取数据，如果哪张表没有内容了代表遍历完成？
	// 【地址表】，名称表，名称序号表
	// 得到函数地址表
	DWORD func_table_rva = export_table->AddressOfFunctions;
	DWORD func_table_foa = E_rva_to_foa(buff, func_table_rva);
	PDWORD func_table = PDWORD(func_table_foa + DWORD(buff));
	// 得到函数名称表
	DWORD name_table_rva = export_table->AddressOfNames;
	DWORD name_table_foa = E_rva_to_foa(buff, name_table_rva);
	PDWORD name_table = PDWORD(name_table_foa + DWORD(buff));
	// 得到名称序号表// 我们可以自己实现 GetProcAddress 这个函数
	DWORD ordinal_table_rva = export_table->AddressOfNameOrdinals;
	DWORD ordinal_table_foa = E_rva_to_foa(buff, ordinal_table_rva);
	PWORD ordinal_table = PWORD(ordinal_table_foa + DWORD(buff));

	// 开始遍历,因为地址表中的内容多，所以用他遍历
	for (int i = 0; i < address_count; i++)
	{
		if (func_table[i] == 0)
		{
			// 如果地址表的内容为 0 就是无效函数
			continue;
		}
		// 判断函数是否在名称表中，如果在就是名称导出，否则就是序号导出
		// 标志位，如果为假，就说明不在名称表中
		bool is_find = false;
		for (int j = 0; j < name_count; j++)
		{
			// i 是从 0 开始遍历，序号表也就不需要加 base 了
			// 如果地址表的索引和名称序号表的内容一致，就说明名称导出
			if (i == ordinal_table[j])
			{
				is_find = true;
				PCHAR fun_name =
					(PCHAR)(E_rva_to_foa(buff, name_table[j]) + (DWORD)buff);
				Address.Format(L"函数地址: %p\r\n",func_table[i]);
				E_Name.Format(L"函数名称: %S\r\n",fun_name);
				Number.Format(L"名称序号: %d\r\n",ordinal_table[j] + base);
				Edit_Export += Address;
				Edit_Export += E_Name;
				Edit_Export += Number;
				UpdateData(FALSE);
				break;
			}
		}
		if (is_find == false)
		{
			Address.Format(L"函数地址: %p\r\n", func_table[i]);
			E_Name.Format(L"函数名称: NULL\r\n");
			Number.Format(L"函数序号: %d\r\n", i + base);
			Edit_Export += Address;
			Edit_Export += E_Name;
			Edit_Export += Number;
			UpdateData(FALSE);
		}

	}

}



afx_msg LRESULT Export_Table_List::OnUser_Export(WPARAM wParam, LPARAM lParam)
{
	E_Buff = (char*)lParam;
	get_export_information(E_Buff);
	return 0;
}

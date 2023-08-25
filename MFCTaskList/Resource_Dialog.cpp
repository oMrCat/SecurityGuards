// Resource_Dialog.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "Resource_Dialog.h"
#include "afxdialogex.h"


// Resource_Dialog 对话框
char* Re_Buff;
IMPLEMENT_DYNAMIC(Resource_Dialog, CDialogEx)

Resource_Dialog::Resource_Dialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG15, pParent)
	, Edit_Resource(_T(""))
{

}

Resource_Dialog::~Resource_Dialog()
{
}

void Resource_Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Edit_Resource);
}


BEGIN_MESSAGE_MAP(Resource_Dialog, CDialogEx)
	ON_MESSAGE(WM_USER + 8, &Resource_Dialog::OnUser_Resource)
END_MESSAGE_MAP()


// Resource_Dialog 消息处理程序

// RVA 转换成 FOA 
DWORD re_rva_to_foa(char* buff, DWORD rva)
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

//解析资源表
void Resource_Dialog::get_resource_infomation(char* buff)
{
	CString Id;
	CString NameString;
	CString OnceId;
	CString OffsetToData;
	CString DataSize;
	//先将缓冲区转换成dos头
	PIMAGE_DOS_HEADER dos_head = (PIMAGE_DOS_HEADER)buff;
	//根据dos头找到nt头
	PIMAGE_NT_HEADERS nt_head =
		PIMAGE_NT_HEADERS(buff + dos_head->e_lfanew);
	//获取扩展头
	PIMAGE_OPTIONAL_HEADER optional_head = &nt_head->OptionalHeader;

	//在数据目录表的第3项获取资源
	DWORD resource_rva = optional_head->DataDirectory[2].VirtualAddress;
	DWORD resource_foa = re_rva_to_foa(buff, resource_rva);

	// 资源表的基址
	DWORD res_base = resource_foa + (DWORD)buff;
	PIMAGE_RESOURCE_DIRECTORY res_table =
		(PIMAGE_RESOURCE_DIRECTORY)res_base;

	// 资源的种类
	DWORD name_count = res_table->NumberOfNamedEntries;
	DWORD id_count = res_table->NumberOfIdEntries;
	DWORD count = name_count + id_count;

	// 第一层遍历 entry
	PIMAGE_RESOURCE_DIRECTORY_ENTRY first_table =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(res_table + 1);
	for (int i = 0; i < count; i++)
	{
		// 判断是字符串资源还是id资源
		if (first_table->NameIsString != 1)
		{
			// 这里是 ID 资源
			switch (first_table->Id)
			{
			case 0x1:Id.Format(L"Cursor\r\n"); break;
			case 0x2:Id.Format(L"Bitmap\r\n"); break;
			case 0x3:Id.Format(L"Icon\r\n"); break;
			case 0x4:Id.Format(L"Menu\r\n"); break;
			case 0x5:Id.Format(L"Dialog\r\n"); break;
			case 0x6:Id.Format(L"String Table\r\n"); break;
			case 0x7:Id.Format(L"Font Directory\r\n"); break;
			case 0x8:Id.Format(L"Font\r\n"); break;
			case 0x9:Id.Format(L"Accelerators\r\n"); break;
			case 0xA:Id.Format(L"Unformatted\r\n"); break;
			case 0xB:Id.Format(L"Message Table\r\n"); break;
			case 0xC:Id.Format(L"Group Cursor\r\n"); break;
			case 0xE:Id.Format(L"Group Icon\r\n"); break;
			case 0x10:Id.Format(L"Version Information\r\n"); break;
			default:
				Id.Format(L"Undefine\r\n");
			}
			Edit_Resource += Id;
			UpdateData(FALSE);
		}
		else
		{
			// 字符串资源
			PIMAGE_RESOURCE_DIR_STRING_U name_string =
				(PIMAGE_RESOURCE_DIR_STRING_U)(res_base + first_table->NameOffset);
			NameString.Format(L"%S\r\n", name_string->NameString);
			Edit_Resource += NameString;
			UpdateData(FALSE);
		}

		// 判断下一层是否是一个目录
		if (first_table->DataIsDirectory == 1)
		{
			// 获取第二层的信息
			PIMAGE_RESOURCE_DIRECTORY second =
				(PIMAGE_RESOURCE_DIRECTORY)(first_table->OffsetToDirectory + res_base);
			// 获取某一种资源的个数
			DWORD res_count = second->NumberOfNamedEntries + second->NumberOfIdEntries;

			// 先获取到后面跟 entry
			PIMAGE_RESOURCE_DIRECTORY_ENTRY second_table =
				(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(second + 1);

			// 开始遍历第二层的数据
			for (int j = 0; j < res_count; j++)
			{
				// 这里是某种资源的ID
				if (second_table->NameIsString != 1)
				{
					OnceId.Format(L"资源ID:%x\r\n", second_table->Id);
					Edit_Resource += OnceId;
					UpdateData(FALSE);
				}
				// 寻找第三层资源
				if (second_table->DataIsDirectory == 1)
				{
					PIMAGE_RESOURCE_DIRECTORY third =
						(PIMAGE_RESOURCE_DIRECTORY)(second_table->OffsetToDirectory + res_base);
					PIMAGE_RESOURCE_DIRECTORY_ENTRY third_table =
						(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(third + 1);

					PIMAGE_RESOURCE_DATA_ENTRY data =
						(PIMAGE_RESOURCE_DATA_ENTRY)(third_table->OffsetToData + res_base);

					OffsetToData.Format(L"资源的数据RVA:0x%08X\r\n",data->OffsetToData);
					DataSize.Format(L"资源的数据大小:%x\r\n", data->Size);
					Edit_Resource += OffsetToData;
					Edit_Resource += DataSize;
					UpdateData(FALSE);
				}
				second_table++;
			}
		}
		first_table++;
	}

}




afx_msg LRESULT Resource_Dialog::OnUser_Resource(WPARAM wParam, LPARAM lParam)
{
	Re_Buff = (char*)lParam;
	get_resource_infomation(Re_Buff);
	return 0;
}

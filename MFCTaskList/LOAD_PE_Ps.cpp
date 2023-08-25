// LOAD_PE_Ps.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "LOAD_PE_Ps.h"
#include "afxdialogex.h"
#include"Import_Table_List.h"
#include"Export_Table_List.h"
#include"Rection_Dialog.h"
#include"Resource_Dialog.h"
#include"TLS_Dialog.h"
#include"Unload_Dialog.h"


// LOAD_PE_Ps 对话框
CString PE_Ps;
char* buff;
IMPLEMENT_DYNAMIC(LOAD_PE_Ps, CDialogEx)

LOAD_PE_Ps::LOAD_PE_Ps(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG11, pParent)
	, edit_Fileheader(_T(""))
	, Edit_Va(_T(""))
	, Edit_Rva(_T(""))
	, Edit_Foa(_T(""))
{

}

LOAD_PE_Ps::~LOAD_PE_Ps()
{
}

void LOAD_PE_Ps::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_EDIT2, edit_FileHeader);
	DDX_Text(pDX, IDC_EDIT2, edit_Fileheader);
	DDX_Text(pDX, IDC_EDIT1, Edit_Va);
	DDX_Text(pDX, IDC_EDIT3, Edit_Rva);
	DDX_Text(pDX, IDC_EDIT4, Edit_Foa);
	DDX_Control(pDX, IDC_EDIT4, Edit_Control_Foa);
	DDX_Control(pDX, IDC_EDIT1, Edit_Control_Va);
	DDX_Control(pDX, IDC_EDIT3, Edit_Control_Rva);
}


BEGIN_MESSAGE_MAP(LOAD_PE_Ps, CDialogEx)
	ON_MESSAGE(WM_USER + 4, &LOAD_PE_Ps::OnUser)
ON_BN_CLICKED(IDC_BUTTON2, &LOAD_PE_Ps::OnBnClickedButton2)
ON_BN_CLICKED(IDC_BUTTON1, &LOAD_PE_Ps::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON3, &LOAD_PE_Ps::OnBnClickedButton3)
ON_BN_CLICKED(IDC_BUTTON6, &LOAD_PE_Ps::OnBnClickedButton6)
ON_BN_CLICKED(IDC_BUTTON7, &LOAD_PE_Ps::OnBnClickedButton7)
ON_BN_CLICKED(IDC_BUTTON5, &LOAD_PE_Ps::OnBnClickedButton5)
ON_BN_CLICKED(IDC_BUTTON9, &LOAD_PE_Ps::OnBnClickedButton9)
ON_BN_CLICKED(IDC_BUTTON10, &LOAD_PE_Ps::OnBnClickedButton10)
ON_BN_CLICKED(IDC_BUTTON8, &LOAD_PE_Ps::OnBnClickedButton8)
END_MESSAGE_MAP()


// LOAD_PE_Ps 消息处理程序


afx_msg LRESULT LOAD_PE_Ps::OnUser(WPARAM wParam, LPARAM lParam)
{
	buff=(char*)lParam;
	get_file_header_information(buff);
	return 0;
}


// 解析文件头
void LOAD_PE_Ps::get_file_header_information(char* buff)
{
	// 先将缓冲区转换成 dos 头
	PIMAGE_DOS_HEADER dos_head = (PIMAGE_DOS_HEADER)buff;
	// 根据 dos 头找到 n t 头
	PIMAGE_NT_HEADERS nt_head =
		PIMAGE_NT_HEADERS(buff + dos_head->e_lfanew);

	// 获取文件头
	PIMAGE_FILE_HEADER file_head = &nt_head->FileHeader;
	// 获取扩展头
	PIMAGE_OPTIONAL_HEADER optional_head = &nt_head->OptionalHeader;

	CString file_head_NumberOfSection;
	CString Machine;
	CString SizeOfOptionalHeader;
	CString AddressOfEntryPoint;
	CString BaseOfCode;
	CString BaseOfData;
	CString ImageBase;
	CString SectionAlignment;
	CString FileAlignment;
	CString SizeOfImage;
	CString SizeOfHeaders;
	CString NumberOfRvaAndSizes;

	//区段数量
	file_head_NumberOfSection.Format(L"NumberOfSections:%d\r\n", file_head->NumberOfSections);
	//运行平台
	Machine.Format(L"Machine:%x\r\n", file_head->Machine);
	//扩展头的大小
	SizeOfOptionalHeader.Format(L"SizeOfOptionalHeader:%x\r\n", file_head->SizeOfOptionalHeader);
	// EP
	AddressOfEntryPoint.Format(L"AddressOfEntryPoint：%x\r\n", optional_head->AddressOfEntryPoint);
	//代码区块的起始RVA
	BaseOfCode.Format(L"BaseOfCode：%x\r\n", optional_head->BaseOfCode);
	//数据区块的起始RVA
	BaseOfData.Format(L"BaseOfData：%x\r\n", optional_head->BaseOfData);
	//镜像基址
	ImageBase.Format(L"ImageBase：%x\r\n", optional_head->ImageBase);
	//内存的对齐粒度
	SectionAlignment.Format(L"SectionAlignment：%x\r\n", optional_head->SectionAlignment);
	//文件的对齐粒度
	FileAlignment.Format(L"FileAlignment：%x\r\n", optional_head->FileAlignment);
	//映像装入内存后的总大小
	SizeOfImage.Format(L"SizeOfImage：%x\r\n", optional_head->SizeOfImage);
	//从DOS头部+PE头部+区块表的总大小
	SizeOfHeaders.Format(L"SizeOfHeaders：%x\r\n", optional_head->SizeOfHeaders);
	//数据目录表的个数
	NumberOfRvaAndSizes.Format(L"NumberOfRvaAndSizes：%x\r\n", optional_head->NumberOfRvaAndSizes);

	edit_Fileheader += Machine;
	edit_Fileheader += file_head_NumberOfSection;
	edit_Fileheader += SizeOfOptionalHeader;
	edit_Fileheader += AddressOfEntryPoint;
	edit_Fileheader += BaseOfCode;
	edit_Fileheader += BaseOfData;
	edit_Fileheader += ImageBase;
	edit_Fileheader += SectionAlignment;
	edit_Fileheader += FileAlignment;
	edit_Fileheader += SizeOfImage;
	edit_Fileheader += SizeOfHeaders;
	edit_Fileheader += NumberOfRvaAndSizes;
	// 数据目录表信息输出
	CString Data_Info;
	Data_Info.Format(L"数据目录表信息输出：\r\n");
	edit_Fileheader += Data_Info;
	CString VirtulaAddress;
	CString VirtualSize;
	for (int i = 0; i < optional_head->NumberOfRvaAndSizes; i++)
	{
		VirtulaAddress.Format(L"VirtulaAddress : %08X\r\n",
			optional_head->DataDirectory[i].VirtualAddress);
		VirtulaAddress.Format(L"VirtualSize: %08X\r\n",
			optional_head->DataDirectory[i].Size);
		edit_Fileheader += VirtulaAddress;
		edit_Fileheader += VirtualSize;
	}
	//区段的数量
	DWORD section_count = file_head->NumberOfSections;
	//第一个区段的位置
	PIMAGE_SECTION_HEADER section_head = IMAGE_FIRST_SECTION(nt_head);
	CString Section_Info,Section_Name, Section_VirtualAddress, Section_VirtualSize,
		Section_PointerToRawData, Section_SizeOfRawData;
	Section_Info.Format(L"区段表的信息输出：\r\n");
	edit_Fileheader += Section_Info;
	for (int i = 0; i < section_count; i++)
	{
		Section_Name.Format(L"Section_Name：%S \r\n", section_head->Name);
		Section_VirtualAddress.Format(L"Section_VirtualAddress ：%08X\r\n",section_head->VirtualAddress);
		Section_VirtualSize.Format(L"Section_VirtualSize\%08X\r\n",section_head->Misc.VirtualSize);
		Section_PointerToRawData.Format(L"Section_PointerToRawData ：%08X\r\n",section_head->PointerToRawData);
		Section_SizeOfRawData.Format(L"Section_SizeOfRawData\%08X\r\n",section_head->SizeOfRawData);

		edit_Fileheader += Section_Name;
		edit_Fileheader += Section_VirtualAddress;
		edit_Fileheader += Section_VirtualSize;
		edit_Fileheader += Section_PointerToRawData;
		edit_Fileheader += Section_SizeOfRawData;
		// 指向下一个区段头
		section_head++;
	}
	UpdateData(FALSE);
}

//RVA转换成VA、FOA
void LOAD_PE_Ps::rva_to_vafoa(char* buff)
{
	UpdateData(TRUE);
	//将编辑框内的CString值转成16进制
	DWORD rva = _tcstoul(Edit_Rva, NULL, 16);
	//先将缓冲区转换成DOS头
	PIMAGE_DOS_HEADER dos_head = (PIMAGE_DOS_HEADER)buff;
	//根据dos头找到nt头
	PIMAGE_NT_HEADERS nt_head = (PIMAGE_NT_HEADERS)(buff + dos_head->e_lfanew);
	//获取文件头
	PIMAGE_FILE_HEADER file_head = &nt_head->FileHeader;
	//获取扩展头
	PIMAGE_OPTIONAL_HEADER option_head = &nt_head->OptionalHeader;
	//区段的数量
	DWORD section_count = file_head->NumberOfSections;
	//获取到区段头表
	PIMAGE_SECTION_HEADER section_head = IMAGE_FIRST_SECTION(nt_head);
	for (int i = 0; i < section_count; i++)
	{
		//如果rva在本区段，就计算段内偏移
		if (rva >= section_head->VirtualAddress&&
			rva<=section_head->VirtualAddress+section_head->SizeOfRawData)
		{
			//根据公式计算  FOA
			//RVA-区段起始位置=FOA-区段文件起始地址
			DWORD foa = rva - section_head->VirtualAddress
				+ section_head->PointerToRawData;
			CString Foa;
			Foa.Format(L"%x", foa);
			Edit_Foa = Foa;
			//va 虚拟地址= InmageBase(加载基址) + RVA(相对虚拟地址)
			DWORD va = option_head->ImageBase + rva;
			CString Va;
			Va.Format(L"%x", va);
			Edit_Va = Va;
			UpdateData(FALSE);
		}
		section_head++;
	}
	return;
}
//VA转换成RVA、FOA
void LOAD_PE_Ps::va_to_rvafoa(char* buff)
{
	UpdateData(TRUE);
	//将编辑框内的CString值转成16进制
	DWORD va = _tcstoul(Edit_Va, NULL, 16);
	//先将缓冲区转换成DOS头
	PIMAGE_DOS_HEADER dos_head = (PIMAGE_DOS_HEADER)buff;
	//根据dos头找到nt头
	PIMAGE_NT_HEADERS nt_head = (PIMAGE_NT_HEADERS)(buff + dos_head->e_lfanew);
	//获取文件头
	PIMAGE_FILE_HEADER file_head = &nt_head->FileHeader;
	//获取扩展头
	PIMAGE_OPTIONAL_HEADER option_head = &nt_head->OptionalHeader;
	//区段的数量
	DWORD section_count = file_head->NumberOfSections;
	//获取到区段头表
	PIMAGE_SECTION_HEADER section_head = IMAGE_FIRST_SECTION(nt_head);
	//获取Rva
	DWORD Rva = va - option_head->ImageBase;
	CString rva;
	rva.Format(L"%x", Rva);
	Edit_Rva = rva;
	UpdateData(FALSE);
	for (int i = 0; i < section_count; i++)
	{
		//如果rva在本区段，就计算段内偏移
		if (Rva >= section_head->VirtualAddress &&
			Rva <= section_head->VirtualAddress + section_head->SizeOfRawData)
		{
			//根据公式计算  FOA
			//RVA-区段起始位置=FOA-区段文件起始地址
			DWORD foa = Rva - section_head->VirtualAddress
				+ section_head->PointerToRawData;
			CString Foa;
			Foa.Format(L"%x", foa);
			Edit_Foa = Foa;
			UpdateData(FALSE);
		}
		section_head++;
	}
	return;
}
//FOA转换成RVA、VA
void LOAD_PE_Ps::foa_to_rvava(char* buff)
{
	UpdateData(TRUE);
	//将编辑框内的CString值转成16进制
	DWORD foa = _tcstoul(Edit_Foa, NULL, 16);
	//先将缓冲区转换成DOS头
	PIMAGE_DOS_HEADER dos_head = (PIMAGE_DOS_HEADER)buff;
	//根据dos头找到nt头
	PIMAGE_NT_HEADERS nt_head = (PIMAGE_NT_HEADERS)(buff + dos_head->e_lfanew);
	//获取文件头
	PIMAGE_FILE_HEADER file_head = &nt_head->FileHeader;
	//获取扩展头
	PIMAGE_OPTIONAL_HEADER option_head = &nt_head->OptionalHeader;
	//区段的数量
	DWORD section_count = file_head->NumberOfSections;
	//获取到区段头表
	PIMAGE_SECTION_HEADER section_head = IMAGE_FIRST_SECTION(nt_head);

	if (foa < nt_head->OptionalHeader.SizeOfHeaders)
	{
		CString cd;
		cd.Format(L"%x", foa);
		Edit_Rva = cd;
		UpdateData(FALSE);
	}
	//循环判断FOA落在了哪个区段中
	for (int i = 0; i < section_count; i++)
	{
		DWORD dwSectionRva = section_head[i].VirtualAddress;
		DWORD dwSectionEndRva = dwSectionRva + section_head[i].SizeOfRawData;
		DWORD dwSectionFoa = section_head[i].PointerToRawData;
		//FOA=RVA - RVA区段 +FOA区段
		DWORD Rva = foa + dwSectionRva - dwSectionFoa;
		CString rva;
		rva.Format(L"%x", Rva);
		Edit_Rva = rva;
		//VA虚拟地址=ImageBase(加载基址)+RVA（相对虚拟地址）
		DWORD Va = option_head->ImageBase + Rva;
		CString va;
		va.Format(L"%x", Va);
		Edit_Va = va;
		UpdateData(FALSE);
	}
	return;
}

//RVA
void LOAD_PE_Ps::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	Edit_Control_Foa.SetReadOnly(TRUE);
	Edit_Control_Va.SetReadOnly(TRUE);
	Edit_Control_Rva.SetReadOnly(FALSE);
	rva_to_vafoa(buff);
}

//VA
void LOAD_PE_Ps::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	Edit_Control_Foa.SetReadOnly(TRUE);
	Edit_Control_Rva.SetReadOnly(TRUE);
	Edit_Control_Va.SetReadOnly(FALSE);
	va_to_rvafoa(buff);
}


//FOA
void LOAD_PE_Ps::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	Edit_Control_Va.SetReadOnly(TRUE);
	Edit_Control_Rva.SetReadOnly(TRUE);
	Edit_Control_Foa.SetReadOnly(FALSE);
	foa_to_rvava(buff);
}

//导入表
void LOAD_PE_Ps::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	Import_Table_List* import_table = new Import_Table_List;
	import_table->Create(IDD_DIALOG12, this);
	import_table->SendMessage(WM_USER + 5, NULL, (LPARAM)buff);
	import_table->ShowWindow(SW_SHOW);
}

//导出表
void LOAD_PE_Ps::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	Export_Table_List* export_table = new Export_Table_List;
	export_table->Create(IDD_DIALOG13, this);
	export_table->SendMessage(WM_USER + 6, NULL, (LPARAM)buff);
	export_table->ShowWindow(SW_SHOW);
}

//重定位表
void LOAD_PE_Ps::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	Rection_Dialog* rection = new Rection_Dialog;
	rection->Create(IDD_DIALOG14, this);
	rection->SendMessage(WM_USER + 7, NULL, (LPARAM)buff);
	rection->ShowWindow(SW_SHOW);
}

//资源表
void LOAD_PE_Ps::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	Resource_Dialog* resource = new Resource_Dialog;
	resource->Create(IDD_DIALOG15, this);
	resource->SendMessage(WM_USER + 8, NULL, (LPARAM)buff);
	resource->ShowWindow(SW_SHOW);
}

//TLS表
void LOAD_PE_Ps::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	TLS_Dialog* tls = new TLS_Dialog;
	tls->Create(IDD_DIALOG16, this);
	tls->SendMessage(WM_USER + 9, NULL, (LPARAM)buff);
	tls->ShowWindow(SW_SHOW);
}

//延迟表
void LOAD_PE_Ps::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	Unload_Dialog* unload = new Unload_Dialog;
	unload->Create(IDD_DIALOG17, this);
	unload->SendMessage(WM_USER + 10, NULL, (LPARAM)buff);
	unload->ShowWindow(SW_SHOW);
}

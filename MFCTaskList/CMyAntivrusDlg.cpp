// CMyAntivrusDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "CMyAntivrusDlg.h"
#include "afxdialogex.h"

// CMyAntivrusDlg 对话框
CString m_chex;
vector<CString>vecPath1;
vector<CString>vecMd5;
vector<CString>BinDuFile;
IMPLEMENT_DYNAMIC(CMyAntivrusDlg, CDialogEx)

CMyAntivrusDlg::CMyAntivrusDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG9, pParent)
	, m_szStatus(_T(""))
{

}

CMyAntivrusDlg::~CMyAntivrusDlg()
{
}

void CMyAntivrusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST3, m_List_Vir);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
	DDX_Control(pDX, IDC_BUTTON2, m_Button2);
	DDX_Text(pDX, IDC_STATIC11, m_szStatus);
}


BEGIN_MESSAGE_MAP(CMyAntivrusDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyAntivrusDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyAntivrusDlg::OnBnClickedButton2)
	ON_MESSAGE(WM_MYUPDATEDATA, OnUpdateMyData)
END_MESSAGE_MAP()


// CMyAntivrusDlg 消息处理程序





BOOL CMyAntivrusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化窗口
	CRect rc;
	::GetClientRect(AfxGetApp()->m_pMainWnd->m_hWnd, rc);
	rc.DeflateRect(15, 50, 15, 160);
	MoveWindow(rc);

	//初始化List
	m_List_Vir.InsertColumn(0, _T("病毒文件名"), 0, 100);
	m_List_Vir.InsertColumn(1, _T("文件路径"), 0, 350);
	m_List_Vir.InsertColumn(2, _T("文件大小"), 0, 100);
	m_List_Vir.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//扫描线程
DWORD WINAPI ScanThread(LPVOID lpVoid)
{
	CMyAntivrusDlg* pScanCtrl = (CMyAntivrusDlg*)lpVoid;
	//查杀路径判断
	if (pScanCtrl->IsSingleScan)
	{
		//单文件查杀
		pScanCtrl->Scan_Single(pScanCtrl->m_SinglePath);
	}
	else
	{
		//全路径查杀
		//调用路径扫描，传入路径参数
		pScanCtrl->Scam_All(pScanCtrl->m_AllPath);
	}


	pScanCtrl->m_szStatus.Format(
		_T("查 杀 完 毕 ！ 本 次 查 杀 发 现 病 毒 数 ：%d"),
		pScanCtrl->m_vecVirInfo.size());
	pScanCtrl->SendMessage(WM_MYUPDATEDATA, FALSE);

	return 0;
}

//扫描
void CMyAntivrusDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取按钮状态
	IsSelect[0] = ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck();
	IsSelect[1] = ((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck();
	IsSelect[2] = ((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck();
	IsSelect[3] = ((CButton*)GetDlgItem(IDC_RADIO4))->GetCheck();
	IsSelect[4] = ((CButton*)GetDlgItem(IDC_RADIO5))->GetCheck();
	IsSelect[5] = ((CButton*)GetDlgItem(IDC_RADIO6))->GetCheck();
	if (IsSelect[0] == 0 && IsSelect[1] == 0)
	{
		m_szStatus = _T("请 选 择 查 杀 路 径 !");
		UpdateData(FALSE);
		return;
	}
	if (IsSelect[2] == 0 && IsSelect[3] == 0)
	{
		m_szStatus = _T("请 选 择 查 杀 方 式 !");
		UpdateData(FALSE);
		return;
	}
	if (IsSelect[4] == 0 && IsSelect[5] == 0)
	{
		m_szStatus = _T("请 选 择 病 毒 库 !");
		UpdateData(FALSE);
		return;
	}
	//初始化选项设置
	m_vecVirInfo.clear();
	m_LocalMD5.clear();
	m_LocalWhite.clear();
	m_ServerMD5.clear();
	m_ServerWhite.clear();
	TCPClient();
	InitSelect();
	//开始扫描
	StartScan();
}

//清除
void CMyAntivrusDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//先判断病毒Vector是否为空，若为空，则提示没有可以清除的文件
	if (m_vecVirInfo.size() == 0)
	{
		return;
	}
	//删除病毒
	for (DWORD i = 0; i < m_vecVirInfo.size(); i++)
	{
		DeleteFile(m_vecVirInfo[i].szVirPath);
	}
	m_vecVirInfo.clear();
	m_List_Vir.DeleteAllItems();
	m_szStatus = _T("清 除 病 毒 完 毕 !");
	UpdateData(FALSE);
}

//初始化选项设置
void CMyAntivrusDlg::InitSelect()
{
	if (IsSelect[0])
	{
		//单文件查杀
		IsSingleScan = TRUE;
	}
	else
	{
		//全路径查杀
		IsSingleScan = FALSE;
	}
	if (IsSelect[2])
	{
		//MD5
		IsMD5 = TRUE;
	}
	else
	{
		//黑名单
		IsMD5 = FALSE;
	}
	if (IsSelect[4])
	{
		//本地
		IsLocal = TRUE;
	}
	else
	{
		//云端
		IsLocal = FALSE;
	}
}

//开始扫描
void CMyAntivrusDlg::StartScan()
{
	//1.先判断扫描路径
	if (IsSingleScan)
	{
		m_SinglePath.Empty();
		//如果为单个文件查杀，弹出文件选择框，路径保存在m_szSingleFilePath
		CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
			NULL,
			NULL,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			(LPCTSTR)_TEXT("All Files (*.*)|*.*||"),
			NULL);
		if (dlg.DoModal() == IDOK)
		{
			//打开成功，保存文件路径
			m_SinglePath = dlg.GetPathName();
		}
		else
		{
			m_szStatus = _T("打 开 文 件 失 败 ！");
			UpdateData(FALSE);
			return;
		}
		if (m_SinglePath.IsEmpty())
		{
			return;
		}
	}
	else
	{
		//如果为全路径查杀，弹出文件夹选择框，路径保存在m_AllPath
		m_AllPath.Empty();
		m_AllPath = BrowseFolder(this->m_hWnd, _T("请选择查杀路径"));
		if (m_AllPath.IsEmpty())
		{
			return;
		}
	}

	m_List_Vir.DeleteAllItems();
	//2.判断是云端还是本地
	//如果是本地，再判断是MD5还是黑名单，加载本地病毒库信息
	if (IsLocal)
	{
		if (IsMD5)
		{
			//本地MD5	加载本地MD5病毒库
			//先获取病毒库中MD5的个数
			CString Temp;
			GetPrivateProfileString(_T("Num"),
				_T("MD5"), _T("DefaultName"),
				Temp.GetBuffer(MAX_PATH), MAX_PATH,
				_T(".\\LocalVirBase\\Vir.ini"));
			Temp.ReleaseBuffer();
			int n = _ttoi(Temp);
			//再根据个数遍历MD5信息，加载到Vector
			for (int i = 0; i < n; i++)
			{
				CString tempCount;
				tempCount.Format(_T("%d"), i);
				GetPrivateProfileString(_T("MD5"),
					tempCount, _T("DefaultName"),
					Temp.GetBuffer(MAX_PATH), MAX_PATH,
					_T(".\\LocalVirBase\\Vir.ini"));
				Temp.ReleaseBuffer();
				m_LocalMD5.push_back(Temp);
			}
			if (m_LocalMD5.size() == 0)
			{
				m_szStatus = _T("读 取 病 毒 库 错误 ！");
				UpdateData(FALSE);
				return;
			}
		}
		else
		{
			//本地黑名单	加载本地黑名单病毒库
			//先获取病毒库中黑名单的个数
			CString Temp;
			GetPrivateProfileString(_T("Num"),
				_T("White"), _T("DefaultName"),
				Temp.GetBuffer(MAX_PATH), MAX_PATH,
				_T(".\\LocalVirBase\\Vir.ini"));
			Temp.ReleaseBuffer();
			int n = _ttoi(Temp);
			//在根据个数遍历MD5信息，加载到Vector
			for (int i = 0; i < n; i++)
			{
				CString tempCount;
				tempCount.Format(_T("%d"), i);
				GetPrivateProfileString(_T("White"),
					tempCount, _T("DefaultName"),
					Temp.GetBuffer(MAX_PATH), MAX_PATH,
					_T(".\\LocalVirBase\\Vir.ini"));
				Temp.ReleaseBuffer();
				m_LocalWhite.push_back(Temp);
			}
			if (m_LocalWhite.size() == 0)
			{
				m_szStatus = _T("读 取 白 名 单 错误 ！");
				UpdateData(FALSE);
				return;
			}
		}
	}
	//如果是云端，再判断是MD5还是黑名单，下载云端病毒库信息
	else
	{
		if (IsMD5)
		{
			//云端MD5	加载云端MD5病毒库
			//获取服务器数据  TRUE 代表获取 MD5数据库
			//if (!m_ServerObj.SendMyMessage(TRUE))
			//{
			//	m_szStatus = _T("连 接 服 务 器 失 败 ...");
			//	UpdateData(FALSE);
			//	return;
			//}
			//正在连接服务器
			m_szStatus = _T("正 在 连 接 服 务 器 ...");
			UpdateData(FALSE);
			//m_ServerMD5 = m_ServerObj.RetVector();
		}
		else
		{
			//云端黑名单	加载云端黑名单病毒库
			//获取服务器数据  FALSE 代表获取 黑名单数据库
			//if (!m_ServerObj.SendMyMessage(FALSE))
			//{
			//	m_szStatus = _T("连 接 服 务 器 失 败 ...");
			//	UpdateData(FALSE);
			//	return;
			//}
			//正在连接服务器
			m_szStatus = _T("正 在 连 接 服 务 器 ...");
			UpdateData(FALSE);
			//m_ServerWhite = m_ServerObj.RetVector();
		}
	}
	//3.创建启动扫描线程
	//根据以上四项，调用主线程中的函数进行查杀，将结果保存在主线程的Vector中
	CloseHandle(CreateThread(NULL, 0, ScanThread, this, 0, NULL));
}


//打开文件
CString CMyAntivrusDlg::BrowseFolder(HWND hWnd, LPCTSTR lpTitle)
{

	TCHAR szPath[MAX_PATH] = { 0 };
	BROWSEINFO m_bi;

	m_bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	m_bi.hwndOwner = hWnd;
	m_bi.pidlRoot = NULL;
	m_bi.lpszTitle = lpTitle;
	m_bi.lpfn = NULL;
	m_bi.lParam = NULL;
	m_bi.pszDisplayName = szPath;

	LPITEMIDLIST pidl = ::SHBrowseForFolder(&m_bi);
	if (pidl)
	{
		if (!::SHGetPathFromIDList(pidl, szPath)) szPath[0] = 0;

		IMalloc* pMalloc = NULL;
		if (SUCCEEDED(::SHGetMalloc(&pMalloc))) // 取得IMalloc分配器接口
		{
			pMalloc->Free(pidl); // 释放内存
			pMalloc->Release(); // 释放接口
		}
	}
	return szPath;
}


//单个文件查杀
void CMyAntivrusDlg::Scan_Single(LPCTSTR szPath)
{
	WIN32_FIND_DATA wfd;
	CString sFindFilter;
	DWORD dwAttributes = 0;

	sFindFilter = szPath;
	FindFirstFile(sFindFilter, &wfd);

	//判断是否是病毒
	if (IsMD5)
	{
		//MD5查杀
		if (MD5Scan(szPath))
		{
			//插入List
			m_List_Vir.InsertItem(0, wfd.cFileName);	//文件名
			m_List_Vir.SetItemText(0, 1, szPath);		//文件路径
			m_List_Vir.SetItemText(0, 2,				//文件大小
				GetShowSize(wfd.nFileSizeLow));
			//保存到Vector
			VIRINFO VirInfo;
			_tcscpy_s(VirInfo.szVirName, wfd.cFileName);
			_tcscpy_s(VirInfo.szVirPath, szPath);
			_tcscpy_s(VirInfo.szVirSize, GetShowSize(wfd.nFileSizeLow));
			m_vecVirInfo.push_back(VirInfo);
		}
	}
	else
	{
		//黑名单查杀
		//如果是病毒 将文件信息保存到病毒Vector
		if (WhiteScan(szPath))
		{
			//插入List
			m_List_Vir.InsertItem(0, wfd.cFileName);	//文件名
			m_List_Vir.SetItemText(0, 1, szPath);		//文件路径
			m_List_Vir.SetItemText(0, 2,				//文件大小
				GetShowSize(wfd.nFileSizeLow));
			//保存到Vector
			VIRINFO VirInfo;
			_tcscpy_s(VirInfo.szVirName, wfd.cFileName);
			_tcscpy_s(VirInfo.szVirPath, szPath);
			_tcscpy_s(VirInfo.szVirSize, GetShowSize(wfd.nFileSizeLow));
			m_vecVirInfo.push_back(VirInfo);
		}
	}
}

//全路径查杀
void CMyAntivrusDlg::Scam_All(LPCTSTR szPath)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	CString sFullPath;
	CString sFindFilter;
	DWORD dwAttributes = 0;

	sFindFilter = szPath;
	sFindFilter += TEXT("\\*.*");
	if ((hFind = FindFirstFile(sFindFilter, &wfd)) == INVALID_HANDLE_VALUE)
		return;

	do
	{
		if (_tcscmp(wfd.cFileName, TEXT(".")) == 0 ||
			_tcscmp(wfd.cFileName, TEXT("..")) == 0)
		{
			continue;
		}

		//获取完整路径名
		sFullPath = szPath;
		sFullPath += TEXT("\\");
		sFullPath += wfd.cFileName;

		//如果当前路径是文件夹，则需要递归文件夹中的文件
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			Scam_All(sFullPath);
		}
		//否则，这是一个文件
		else
		{
			//输出正在搜索的文件路径界面，让用户直观的看到正在扫描的文件路径
			m_szStatus = _T("正在扫描 : ") + sFullPath;
			SendMessage(WM_MYUPDATEDATA, FALSE);

			//判断是否是病毒
			if (IsMD5)
			{
				//MD5查杀
				//如果是病毒 将文件信息保存到病毒Vector
				if (MD5Scan(sFullPath))
				{
					//插入List中显示
					m_List_Vir.InsertItem(0, wfd.cFileName);	//文件名
					m_List_Vir.SetItemText(0, 1, sFullPath);	//文件路径
					m_List_Vir.SetItemText(0, 2,				//文件大小
						GetShowSize(wfd.nFileSizeLow));
					//保存病毒信息到Vector容器
					VIRINFO VirInfo;
					_tcscpy_s(VirInfo.szVirName, wfd.cFileName);
					_tcscpy_s(VirInfo.szVirPath, sFullPath);
					_tcscpy_s(VirInfo.szVirSize, GetShowSize(wfd.nFileSizeLow));
					m_vecVirInfo.push_back(VirInfo);
				}
			}
			else
			{
				//黑名单查杀
				//如果是病毒 将文件信息保存到病毒Vector
				if (WhiteScan(sFullPath))
				{
					//插入List中显示
					m_List_Vir.InsertItem(0, wfd.cFileName);	//文件名
					m_List_Vir.SetItemText(0, 1, sFullPath);	//文件路径
					m_List_Vir.SetItemText(0, 2,				//文件大小
						GetShowSize(wfd.nFileSizeLow));
					//保存病毒信息到Vector容器
					VIRINFO VirInfo;
					_tcscpy_s(VirInfo.szVirName, wfd.cFileName);
					_tcscpy_s(VirInfo.szVirPath, sFullPath);
					_tcscpy_s(VirInfo.szVirSize, GetShowSize(wfd.nFileSizeLow));
					m_vecVirInfo.push_back(VirInfo);
				}
			}
		}
	} while (FindNextFile(hFind, &wfd));
	FindClose(hFind);
}


//获取病毒大小

CString CMyAntivrusDlg::GetShowSize(DOUBLE dwSize)
{
	CString szSize;
	if (dwSize < 1024)
	{
		//B
		szSize.Format(_T("%d B"), (DWORD)dwSize);
		return szSize;
	}
	else if (dwSize < 1024 * 1024)
	{
		//KB
		szSize.Format(_T("%.2f KB"), ((DOUBLE)dwSize) / 1024);
		return szSize;
	}
	else if (dwSize < 1024 * 1024 * 1024)
	{
		//MB
		szSize.Format(_T("%.2f MB"), (DOUBLE)dwSize / 1024 / 1024);
		return szSize;
	}if (dwSize > 1024 * 1024 * 1024)
	{
		//GB
		szSize.Format(_T("%.2f GB"), (DOUBLE)dwSize / 1024 / 1024 / 1024);
		return szSize;
	}
	return _T("");
}


//MD5查杀
BOOL CMyAntivrusDlg::MD5Scan(LPCTSTR szPath)
{
	//LPCTSTR 转CHAR*
	int num = WideCharToMultiByte(CP_OEMCP, NULL, szPath, -1, NULL, 0, NULL, FALSE);
	char* pchar = new char[num];
	WideCharToMultiByte(CP_OEMCP, NULL, szPath, -1, pchar, num, NULL, FALSE);
	//获取MD5值
	char* md5 = md5FileValue(pchar);
	//CHAR* 转LPCTSTR
	num = MultiByteToWideChar(0, 0, md5, -1, NULL, 0);
	wchar_t* wide = new wchar_t[num];
	MultiByteToWideChar(0, 0, md5, -1, wide, num);
	m_szMD5 = wide;
	delete[]pchar;
	delete[]wide;

	//获取文件MD5信息完毕
	//判断是本地查杀还是云端查杀
	if (IsLocal)
	{
		//本地MD5查杀		与m_LocalMD5作对比
		for (DWORD i = 0; i < m_LocalMD5.size(); i++)
		{
			if (m_LocalMD5[i] == m_szMD5)
			{
				//是病毒 返回真
				return TRUE;
			}
		}
	}
	else
	{
		//云端MD5查杀		与m_ServerMD5作对比
		//本地MD5查杀		与m_LocalMD5作对比
		for (DWORD i = 0; i < m_ServerMD5.size(); i++)
		{
			if (m_ServerMD5[i] == m_szMD5)
			{
				//是病毒 返回真
				return TRUE;
			}
		}
	}
	return FALSE;
}


//黑名单查杀
BOOL CMyAntivrusDlg::WhiteScan(LPCTSTR szPath)
{
	//判断是本地查杀还是云端查杀
	if (IsLocal)
	{
		//本地黑名单查杀		与m_LocalWhite作对比
		for (DWORD i = 0; i < m_LocalWhite.size(); i++)
		{
			if (m_LocalWhite[i] == szPath)
			{
				//是病毒 返回真
				return FALSE;
			}
		}
	}
	else
	{
		//云端MD5查杀		与m_ServerWhite作对比
		for (DWORD i = 0; i < m_ServerWhite.size(); i++)
		{
			if (m_ServerWhite[i] == szPath)
			{
				//是病毒 返回真
				return FALSE;
			}
		}
	}
	return TRUE;
}

//客户端
void CMyAntivrusDlg::TCPClient()
{
	//1. 初始化Winsock库
	WORD wVersion = 0;//要使用的库的版本
	WSADATA wsd = { 0 };//传出参数，告知加载库的信息
	wVersion = MAKEWORD(2, 2);
	int nSuccess = WSAStartup(wVersion, &wsd);
	if (nSuccess != 0)
	{
		printf("初始化失败，从新运行");
	}
	//2. 创建一个socket用于连接服务端
	SOCKET ConnectSocket = socket(
		AF_INET,
		SOCK_STREAM,
		IPPROTO_TCP
	);
	//3. 主动连接服务端
	sockaddr_in addrin = { 0 };
	addrin.sin_family = AF_INET;//使用的协议族
	//主机字节序转网络字节序：小端转大端
	addrin.sin_port = htons(1234); //端口号
	addrin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	connect(
		ConnectSocket,
		(sockaddr*)&addrin,
		sizeof(sockaddr_in)
	);

	//4. 开始收发数据
	char buf[40] = { 0 };
	char buf1[40] = { 0 };
	char buf2[40] = { 0 };
	char buf_black[40] = { 0 };
	char buf_black1[40] = { 0 };
	char buf_black2[40] = { 0 };
	TCHAR buff[40] = { 0 };
	TCHAR buff_Black[40] = { 0 };
	//int count = (int)buff;
	//send(ConnectSocket, "1", 10, 0);

	recv(ConnectSocket, buf, 40, 0);
	recv(ConnectSocket, buf1, 40, 0);
	recv(ConnectSocket, buf2, 40, 0);
	recv(ConnectSocket, buf_black, 40, 0);
	recv(ConnectSocket, buf_black1, 40, 0);
	recv(ConnectSocket, buf_black2, 40, 0);

	for (int i = 0; i < 32; i++)
	{
		buff[i] = buf[i];
	}
	m_ServerMD5.push_back(buff);
	for (int i = 0; i < 32; i++)
	{
		buff[i] = buf1[i];
	}
	m_ServerMD5.push_back(buff);
	for (int i = 0; i < 32; i++)
	{
		buff[i] = buf2[i];
	}
	m_ServerMD5.push_back(buff);
	for (int i = 0; i < 40; i++)
	{
		buff_Black[i] = buf_black[i];
	}
	m_ServerWhite.push_back(buff_Black);
	for (int i = 0; i < 40; i++)
	{
		buff_Black[i] = buf_black1[i];
	}
	m_ServerWhite.push_back(buff_Black);
	for (int i = 0; i < 40; i++)
	{
		buff_Black[i] = buf_black2[i];
	}
	m_ServerWhite.push_back(buff_Black);

	//printf("接到的数据为%s", buf);
	//5. 结束
	closesocket(ConnectSocket);
	WSACleanup();
}



#pragma once
#include<vector>
#include"framework.h"
#include "Md5.h"
//#include "TCPClient.h"
#include "MyButton_Dlg2_1.h"
#include "CMyButton_Dlg2_2.h"
using namespace std;
#define WM_MYUPDATEDATA				WM_USER+100 

//病毒信息结构体
typedef struct _VIRINFO
{
	TCHAR szVirName[50];
	TCHAR szVirPath[MAX_PATH];
	TCHAR szVirSize[50];
}VIRINFO, * PVIRINFO;
// CMyAntivrusDlg 对话框

class CMyAntivrusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyAntivrusDlg)

public:
	CMyAntivrusDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyAntivrusDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG9 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_szStatus;

	CListCtrl m_List_Vir;
	virtual BOOL OnInitDialog();
	//扫描
	afx_msg void OnBnClickedButton1();
	//清除
	afx_msg void OnBnClickedButton2();

	//杀毒选项是否被选中
	BOOL IsSelect[6];
	//初始化查杀选项设置
	BOOL IsSingleScan;	//是否为单个文件查杀	否则为全路径
	BOOL IsMD5;			//是否为MD5查杀		否则为白名单
	BOOL IsLocal;		//是否为本地查杀		否则为云端
	void InitSelect();
	//查杀路径保存
	CString m_SinglePath;
	CString BrowseFolder(HWND hWnd, LPCTSTR lpTitle);
	CString m_AllPath;
	//单个文件查杀
	void Scan_Single(LPCTSTR szPath);
	//全路径查杀
	void Scam_All(LPCTSTR szPath);
	//开始查杀
	void StartScan();
	//获取病毒大小
	CString m_szTemp;
	CString GetShowSize(DOUBLE dwSize);
	//响应子线程更新状态消息
	LRESULT OnUpdateMyData(WPARAM wParam, LPARAM lParam)
	{
		return UpdateData(wParam);
	}
	//MD5查杀
	BOOL MD5Scan(LPCTSTR szPath);
	CString m_szMD5;
	//白名单查杀
	BOOL WhiteScan(LPCTSTR szPath);
	//保存病毒信息Vector
	vector<VIRINFO> m_vecVirInfo;
	//病毒库加载信息
	vector<CString> m_LocalMD5;		//本地MD5库
	vector<CString> m_LocalWhite;	//本地白名单库
	vector<CString> m_ServerMD5;	//云端MD5库
	vector<CString> m_ServerWhite;	//云端白名单库
	//云查杀
	void ServerScan();
	//按钮变量
	MyButton_Dlg2_1 m_Button1;
	CMyButton_Dlg2_2 m_Button2;
	//客户端
	void TCPClient();
};

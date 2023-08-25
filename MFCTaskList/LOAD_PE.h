#pragma once


// LOAD_PE 对话框

class LOAD_PE : public CDialogEx
{
	DECLARE_DYNAMIC(LOAD_PE)

public:
	LOAD_PE(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~LOAD_PE();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG10 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	BOOL IsPE_File(char* lpImage);
};

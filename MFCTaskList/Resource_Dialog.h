#pragma once


// Resource_Dialog 对话框

class Resource_Dialog : public CDialogEx
{
	DECLARE_DYNAMIC(Resource_Dialog)

public:
	Resource_Dialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Resource_Dialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG15 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString Edit_Resource;
	void get_resource_infomation(char* buff);
protected:
	afx_msg LRESULT OnUser_Resource(WPARAM wParam, LPARAM lParam);
};

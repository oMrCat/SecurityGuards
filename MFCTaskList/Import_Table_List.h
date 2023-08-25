#pragma once


// Import_Table_List 对话框

class Import_Table_List : public CDialogEx
{
	DECLARE_DYNAMIC(Import_Table_List)

public:
	Import_Table_List(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Import_Table_List();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG12 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUser_Import(WPARAM wParam, LPARAM lParam);
public:
	void get_import_information(char* import_buff);
	CString Import_List;
};

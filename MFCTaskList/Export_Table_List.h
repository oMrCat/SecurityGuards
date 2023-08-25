#pragma once


// Export_Table_List 对话框

class Export_Table_List : public CDialogEx
{
	DECLARE_DYNAMIC(Export_Table_List)

public:
	Export_Table_List(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Export_Table_List();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG13 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUser_Export(WPARAM wParam, LPARAM lParam);
public:
	void get_export_information(char* buff);

	CString Edit_Export;
};

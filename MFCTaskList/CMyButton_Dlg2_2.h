#pragma once


// CMyButton_Dlg2_2

class CMyButton_Dlg2_2 : public CButton
{
	DECLARE_DYNAMIC(CMyButton_Dlg2_2)

public:
	CMyButton_Dlg2_2();
	virtual ~CMyButton_Dlg2_2();

protected:
	DECLARE_MESSAGE_MAP()
public:
	enum BUTTONSTATE { NORMAL, MOUSEOVER, BUTTONDOWN };
	BUTTONSTATE m_btnState;					//°´Å¥×´Ì¬
	bool m_bIsButtonDown;
	bool m_bIsMouseHower;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};



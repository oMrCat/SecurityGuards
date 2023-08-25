#pragma once


// MyButton_Dlg2_1

class MyButton_Dlg2_1 : public CButton
{
	DECLARE_DYNAMIC(MyButton_Dlg2_1)

public:
	MyButton_Dlg2_1();
	virtual ~MyButton_Dlg2_1();

protected:
	DECLARE_MESSAGE_MAP()
public:
	enum BUTTONSTATE { NORMAL, MOUSEOVER, BUTTONDOWN };
	BUTTONSTATE m_btnState;
	bool m_bIsButtonDown;
	bool m_bIsMouseHower;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};



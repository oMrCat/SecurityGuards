// MyButton_Dlg2_1.cpp: 实现文件
//

#include "pch.h"
#include "MFCTaskList.h"
#include "MyButton_Dlg2_1.h"


// MyButton_Dlg2_1

IMPLEMENT_DYNAMIC(MyButton_Dlg2_1, CButton)

MyButton_Dlg2_1::MyButton_Dlg2_1()
{

}

MyButton_Dlg2_1::~MyButton_Dlg2_1()
{
}


BEGIN_MESSAGE_MAP(MyButton_Dlg2_1, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// MyButton_Dlg2_1 消息处理程序




void MyButton_Dlg2_1::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bIsButtonDown = TRUE;
	m_btnState = BUTTONDOWN;					//按钮状态，被点击状态
	InvalidateRect(NULL, TRUE);
	CButton::OnLButtonDown(nFlags, point);
}


void MyButton_Dlg2_1::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bIsButtonDown = FALSE;
	m_btnState = MOUSEOVER;					//按钮状态，恢复悬停状态
	InvalidateRect(NULL, TRUE);
	CButton::OnLButtonUp(nFlags, point);
}


void MyButton_Dlg2_1::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//一直在按钮上方，则不重复触发
	if (m_bIsMouseHower == TRUE)
	{
		return;
	}
	//第一次上来，触发
	m_bIsMouseHower = TRUE;

	m_btnState = MOUSEOVER;					//按钮状态，鼠标悬停状态
	InvalidateRect(NULL, TRUE);
	CButton::OnMouseHover(nFlags, point);
}


void MyButton_Dlg2_1::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//离开后触发
	m_bIsMouseHower = FALSE;
	m_btnState = NORMAL;					  //按钮状态，正常状态
	InvalidateRect(NULL, TRUE);
	CButton::OnMouseLeave();
}


void MyButton_Dlg2_1::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//需要手动触发TME_LEAVE消息和TME_HOVER消息
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_LEAVE | TME_HOVER;	// 要触发的消息
	tme.hwndTrack = this->m_hWnd;			// 指定为按钮窗口
	tme.dwHoverTime = 20;					// 我觉得应该是检测的间隔，每20毫秒检测一次
	TrackMouseEvent(&tme);
	CButton::OnMouseMove(nFlags, point);
}

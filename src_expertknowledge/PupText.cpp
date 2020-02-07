/*
Copyright (C) 2015, 2016 SoLIM Group, University of Wisconsin-Maison, 
Madison, Wisconsin, USA and Institute of Geographic Sciences and Natural 
Resources Research, Chinese Academy of Sciences, Beijing, China

This program is free software: you can redistribute it and/or 
modify it under the terms of the GNU General Public License 
version 3, 2007 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
GNU General Public License for more details.

A copy of the GNU General Public License version 3 is provided
 in file GPLv3license.txt. This is also available at: 
 https://www.gnu.org/licenses/gpl-3.0.html.

==================additional conditions============================
If the GNU General Public License conditions do not meet your 
needs to use or incorporate this program (or part of it) into 
other software, SoLIM Solutions may also be available under 
alternative licenses (multi licensing). This allows the program 
to be sold or licensed for use in proprietary applications. In 
this case, please contact any of the members of the SoLIM Group 
listed below to discuss a licensing agreement.
SoLIM Group:
(A-Xing Zhu, Cheng-Zhi Qin, Peng Liang, Fang-He Zhao)
Stat Key Laboratory of Resources and Environmental Information System
Institute of Geographic Sciences and Natural Resources Research,
Chinese Academy of Sciences
11A Datun Road, Chaoyang District, Beijing 100101, PR China
Or 
(A-Xing Zhu)
Department of Geography
University of Wisconsin-Maison
550 N. Park St., Madison, WI 53706, USA
https://solim.geography.wisc.edu/index.htm
Email: dsm_solim@yahoo.com

If you wish to use this program (or part of it), please do credit us
either by citing the proper references in your published papers or in 
your program. Related publications on SoLIM can be found in Appendix 
D: SoLIM Publications in the Appendices of the SoLIM Solutions Help 
system at: http://solim.geography.wisc.edu/software/SoLIMSolutions2013/Help_HTML/index.html. 



By downloading this program, you have agreed to conditions of this license.
*/
////////////////////////////////////////////////////////////////
// MSDN Magazine -- November 2003
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio .NET on Windows XP. Tab size=3.
//
#include "stdafx.h"
#include "puptext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CPopupText,CWnd)
BEGIN_MESSAGE_MAP(CPopupText,CWnd)
	ON_WM_NCHITTEST()
	ON_WM_PAINT()
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_WM_TIMER()
END_MESSAGE_MAP()

CPopupText::CPopupText()
{
	CNonClientMetrics ncm;
	m_font.CreateFontIndirect(&ncm.lfMenuFont);
	m_szMargins = CSize(4,4);
}

CPopupText::~CPopupText()
{
}

//////////////////
// Create window. pt is upper left corner
//
int CPopupText::Create(CPoint pt, CWnd* pParentWnd, UINT nID)
{
	return CreateEx(0,
		NULL,
		NULL,
		WS_POPUP|WS_VISIBLE,
		CRect(pt,CSize(0,0)),
		pParentWnd,
		nID);
}

//////////////////
// Text changed: resize window to fit
//
LRESULT CPopupText::OnSetText(WPARAM wp, LPARAM lp)
{
	CRect rc;
	GetWindowRect(&rc);
	int h = rc.Height();
	CClientDC dc(this);
	DrawText(dc, CString((LPCTSTR)lp), rc, DT_CALCRECT);
	rc.InflateRect(m_szMargins);
	if (m_szMargins.cy)
		h = rc.Height();
	SetWindowPos(NULL, 0, 0, rc.Width(), h,
		SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
	return Default();
}

void CPopupText::DrawText(CDC& dc, LPCTSTR lpText, CRect& rc, UINT flags)
{
	CBrush b(GetSysColor(COLOR_INFOBK)); // use tooltip bg color
	dc.FillRect(&rc, &b);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(GetSysColor(COLOR_INFOTEXT)); // tooltip text color
	CFont* pOldFont = dc.SelectObject(&m_font);
	dc.DrawText(lpText, &rc, flags);
	dc.SelectObject(pOldFont);
}

//////////////////
// Paint text using system colors
//
void CPopupText::OnPaint()
{
	CRect rc;
	GetClientRect(&rc);
	CString s;
	GetWindowText(s);
	CPaintDC dc(this);
	DrawText(dc, s, rc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
}

//////////////////
// Register class if needed
//
BOOL CPopupText::PreCreateWindow(CREATESTRUCT& cs) 
{
	static CString sClassName;
	if (sClassName.IsEmpty())
		sClassName = AfxRegisterWndClass(0);
	cs.lpszClass = sClassName;
	cs.style = WS_POPUP|WS_BORDER;
	cs.dwExStyle |= WS_EX_TOOLWINDOW;
	return CWnd::PreCreateWindow(cs);
}

//////////////////
// CPopupText is intended to be used on the stack,
// not heap, so don't auto-delete.
//
void CPopupText::PostNcDestroy()
{
	// don't delete this
}

//////////////////
// Show window with delay. No delay means show now.
//
void CPopupText::ShowDelayed(UINT msec)
{
	if (msec==0) {
		// no delay: show it now
		OnTimer(1);
	} else {
		// delay: set time
		SetTimer(1, msec, NULL);
	}
}

//////////////////
// Cancel text: kill timer and hide window
//
void CPopupText::Cancel()
{
	if (m_hWnd) {
		KillTimer(1);
		ShowWindow(SW_HIDE);
	}
}

//////////////////
// Timer popped: display myself and kill timer
//
void CPopupText::OnTimer(UINT nIDEvent)
{
	ShowWindow(SW_SHOWNA);
	Invalidate();
	UpdateWindow();
	KillTimer(1);
}

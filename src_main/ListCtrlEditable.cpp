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
#include "stdafx.h"
#include "ListCtrlEditable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#if (_WIN32_WINNT < 0x501)
typedef struct tagNMLVSCROLL
{
	NMHDR   hdr;
	int     dx;
	int     dy;
} NMLVSCROLL, *LPNMLVSCROLL;

#define LVN_BEGINSCROLL          (LVN_FIRST-80)
#define LVN_ENDSCROLL            (LVN_FIRST-81)
#endif


#define MAX_COLS	2

#define LV_EDIT_CTRL_ID		1001

#define IDC_COMBO_CHARACTERIZATION 1002

BEGIN_MESSAGE_MAP(CEditableListCtrl, CListCtrl)
	ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_WM_HSCROLL()
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(LV_EDIT_CTRL_ID, OnEnKillFocus)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
	ON_NOTIFY_REFLECT(LVN_ENDSCROLL, OnLvnEndScroll)
	ON_NOTIFY_REFLECT(LVN_BEGINSCROLL, OnLvnBeginScroll)

	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)


	ON_CBN_SELCHANGE(IDC_COMBO_CHARACTERIZATION, OnCbnSelchangeComboCharaccterization)

	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CEditableListCtrl::CEditableListCtrl()
{
	m_pctrlEdit = NULL;
	m_pctrlComboBox = NULL;
	m_iRow = 0;
	m_iCol = 1;

	m_iEditRow = -1;
	m_iEditCol = -1;


	m_iNumOfUsedRow = 9;

	m_colRow = RGB(229,232,239);
}

void CEditableListCtrl::ResetTopPosition()
{
	CWnd* pWnd = NULL;
	if (m_pctrlComboBox)
		pWnd = m_pctrlComboBox;
	else if (m_pctrlEdit)
		pWnd = m_pctrlEdit;
	if (pWnd)
	{
		CRect rcThis;
		GetWindowRect(rcThis);
		CRect rect;
		if (!GetSubItemRect(m_iRow, m_iCol, LVIR_LABEL, rect))
			return;
		ClientToScreen(rect);
		CPoint pt(rect.left, rect.top);
		if (!rcThis.PtInRect(pt))
			SendMessage(WM_VSCROLL, SB_LINEUP, 0);
	}
}

void CEditableListCtrl::ResetBottomPosition()
{
	CWnd* pWnd = NULL;
	if (m_pctrlComboBox)
		pWnd = m_pctrlComboBox;
	else if (m_pctrlEdit)
		pWnd = m_pctrlEdit;
	if (pWnd)
	{
		CRect rcThis;
		GetWindowRect(rcThis);
		CRect rect;
		if (!GetSubItemRect(m_iRow, m_iCol, LVIR_LABEL, rect))
			return;
		ClientToScreen(rect);
		CPoint pt(rect.right, rect.bottom);
		if (!rcThis.PtInRect(pt))
			SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
	}
}

void CEditableListCtrl::VerifyScrollPos()
{
	int nTopRowVal = GetTopIndex();
	int nBottomVal = nTopRowVal + GetCountPerPage();
	if (m_iRow > nTopRowVal-1 && m_iRow < nBottomVal+1)
		;
	else
	{
		if (m_iRow < nTopRowVal)
		{
			for (int j = 0; j < -m_iRow + nTopRowVal; j++)
				SendMessage(WM_VSCROLL, SB_LINEUP, 0);
		}
		if (m_iRow > nBottomVal)
		{
			for (int j = 0; j < m_iRow - nTopRowVal; j++)
				SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
		}
	}
}

BOOL CEditableListCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB)
	{
		CommitEditCtrl();
		m_iEditRow = -1;
		m_iEditCol = -1;
		if (GetKeyState(VK_SHIFT) & 0x8000)
		{
			m_iCol--;
			if (m_iCol < 1)
			{
				m_iCol = MAX_COLS - 1;
				m_iRow--;
				while (m_iRow >= 0 && GetItemData(m_iRow) & 1)
					m_iRow--;

				if (m_iRow < 0)
				{
					m_iCol = 1;
					m_iRow = 0;

					// cycle to prev sibling control
					CWnd* pWnd = GetWindow(GW_HWNDPREV);
					ASSERT( pWnd );
					if (pWnd)
					{
						pWnd->SetFocus();
						return CListCtrl::PreTranslateMessage(pMsg);
					}
				}
			}
		}
		else
		{
			m_iCol++;
			if (m_iCol >= MAX_COLS)
			{
				m_iCol = 1;
				m_iRow++;

				while (m_iRow < GetItemCount() && GetItemData(m_iRow) & 1)
					m_iRow++;

				ResetBottomPosition();
				if (m_iRow > GetItemCount() - 1)
				{
					m_iRow = GetItemCount() - 1;

					// cycle to next sibling control
					CWnd* pWnd = GetNextWindow();
					ASSERT( pWnd );
					if (pWnd)
					{
						pWnd->SetFocus();
						return CListCtrl::PreTranslateMessage(pMsg);
					}
				}
			}
		}

		VerifyScrollPos();
		ResetTopPosition();
		ResetBottomPosition();

		switch (m_iCol)
		{
		case 0:
			break;
		case 1:
			ShowEditCtrl();
			break;
		}
		return TRUE;
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}

void CEditableListCtrl::OnSetFocus(CWnd* pOldWnd)
{
	CListCtrl::OnSetFocus(pOldWnd);

	switch (m_iCol)
	{
	case 0:
		break;
	case 1:
		if (pOldWnd != m_pctrlEdit)
			ShowEditCtrl();
		break;
	}
}

void CEditableListCtrl::CommitEditCtrl()
{
	if (m_iEditCol != -1 && m_iEditRow != -1)
	{
		if (m_pctrlEdit && m_pctrlEdit->IsWindowVisible())
		{
			CString strItem;
			m_pctrlEdit->GetWindowText(strItem);
			strItem.TrimLeft();
			strItem.TrimRight();
			SetItemText(m_iEditRow, m_iEditCol, strItem);
			//			m_iEditRow = -1;
			//			m_iEditCol = -1;
		}
		else if (m_pctrlComboBox && m_pctrlComboBox->IsWindowVisible())
		{
			CString strItem;
			m_pctrlComboBox->GetLBText(m_pctrlComboBox->GetCurSel(), strItem);
			strItem.TrimLeft();
			strItem.TrimRight();

			//SetItemText(m_iRow, m_iCol, strItem);
			//	SetItemText(m_iEditRow, m_iEditCol, strItem);
			//			m_iEditRow = -1;
			//			m_iEditCol = -1;
		}
	}
}

void CEditableListCtrl::ShowEditCtrl()
{
	/*
	if (m_iRow >= GetItemCount())
	return;
	if (GetItemData(m_iRow) & 1)
	{
	if (m_pctrlEdit)
	m_pctrlEdit->ShowWindow(SW_HIDE);
	return;
	}

	CRect rect;
	GetSubItemRect(m_iRow, m_iCol, LVIR_LABEL, rect);
	if (m_pctrlEdit == NULL)
	{
	m_pctrlEdit = new CEdit;
	m_pctrlEdit->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_TABSTOP|WS_BORDER|ES_LEFT|ES_AUTOHSCROLL, rect, this, LV_EDIT_CTRL_ID);
	m_pctrlEdit->SetFont(GetFont());
	m_pctrlEdit->ShowWindow(SW_SHOW);
	}
	else
	{
	m_pctrlEdit->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
	}

	TCHAR szItem[256] = {0};
	LVITEM item;
	item.mask = LVIF_TEXT;
	item.iItem = m_iRow;
	item.iSubItem = m_iCol;
	//item.cchTextMax = ARRSIZE(szItem);
	item.pszText = szItem;
	GetItem(&item);
	m_pctrlEdit->SetWindowText(szItem);
	m_pctrlEdit->SetSel(0, -1);
	if (m_pctrlComboBox)
	m_pctrlComboBox->ShowWindow(SW_HIDE);
	m_pctrlEdit->SetFocus();

	m_iEditRow = m_iRow;
	m_iEditCol = m_iCol;*/
}

void CEditableListCtrl::OnEnKillFocus()
{
	CommitEditCtrl();
	m_iEditRow = -1;
	m_iEditCol = -1;
	if (m_pctrlEdit)
		m_pctrlEdit->ShowWindow(SW_HIDE);
}

void CEditableListCtrl::ShowComboBoxCtrl(CStringList &strItems, CString curStr)
{
	//m_iEditRow = m_iRow;
	//m_iEditCol = m_iCol;

	CRect rect;
	GetSubItemRect(m_iRow, m_iCol, LVIR_LABEL, rect);
	rect.bottom += 100;


	if (!m_pctrlComboBox)
	{
		m_pctrlComboBox = new CComboBox;
		m_pctrlComboBox->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_CLIPSIBLINGS|WS_BORDER|CBS_DROPDOWN|WS_VSCROLL|WS_HSCROLL, rect, this, 1002);
		m_pctrlComboBox->ShowWindow(SW_SHOW);


		m_pctrlComboBox->SetFont(GetFont());

		//m_pctrlComboBox->SetHorizontalExtent(100);
		//m_pctrlComboBox->SetDroppedWidth(10);


		for(int i=0; i<strItems.GetCount(); i++)
		{

			POSITION position; 
			position = strItems.FindIndex(i); 

			m_pctrlComboBox->AddString(strItems.GetAt(position));

		}

		for(int i=0; i<strItems.GetCount(); i++)
		{

			POSITION position; 
			position = strItems.FindIndex(i); 

			if(strItems.GetAt(position)==curStr)

				m_pctrlComboBox->SetCurSel(i);

		}







		//m_pctrlComboBox->SetCurSel(0);

		//	m_pctrlComboBox->SendMessage(CB_SETDROPPEDWIDTH, 600, 0);
	}
	else
	{
		int num = m_pctrlComboBox->GetCount();
		for(int i=0; i<num; i++)
		{
			m_pctrlComboBox->DeleteString(0);
		}

		for(int i=0; i<strItems.GetCount(); i++)
		{

			POSITION position; 
			position = strItems.FindIndex(i); 

			m_pctrlComboBox->AddString(strItems.GetAt(position));

		}

		for(int i=0; i<strItems.GetCount(); i++)
		{

			POSITION position; 
			position = strItems.FindIndex(i); 

			if(strItems.GetAt(position)==curStr)

				m_pctrlComboBox->SetCurSel(i);

		}



		m_pctrlComboBox->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
		//m_pctrlComboBox->SetCurSel(0);
	}
	if (m_pctrlEdit)
		m_pctrlEdit->ShowWindow(SW_HIDE);
	m_pctrlComboBox->SetFocus();


}

void CEditableListCtrl::OnColumnclick(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	if (m_pctrlEdit && m_pctrlEdit->IsWindowVisible())
		m_pctrlEdit->ShowWindow(SW_HIDE);
	if (m_pctrlComboBox && m_pctrlComboBox->IsWindowVisible())
		m_pctrlComboBox->ShowWindow(SW_HIDE);
	*pResult = 0;
}




void CEditableListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{


	CListCtrl::OnLButtonDown(nFlags, point);



	LVHITTESTINFO hti = {0};
	hti.pt = point;
	if (SubItemHitTest(&hti) == -1)
		return;

	int iItem = hti.iItem;
	int iSubItem = hti.iSubItem;
	if (iItem == -1 || iSubItem == -1)
		return;

	CRect rect;
	GetSubItemRect(iItem, iSubItem, LVIR_LABEL, rect);
	ClientToScreen(rect);

	CPoint pt(rect.right, rect.bottom);
	CRect rc;
	GetWindowRect(rc);
	if (!rc.PtInRect(pt))
		SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);

	pt.x = rect.left;
	pt.y = rect.top;
	if (!rc.PtInRect(pt))
		SendMessage(WM_VSCROLL, SB_LINEUP, 0);

	m_iRow = iItem;
	m_iCol = iSubItem;


	m_iCurComboRow = m_iRow;
	m_iCurComboCol = m_iCol;




	if(m_iCol == 3)
	{

		CString curStr = this->GetItemText(m_iRow, m_iCol);

		CStringList strItems;

		strItems.AddTail("Single Value");

		if(atof(this->GetItemText(m_iRow, m_iCol-1).GetBuffer(0)) < m_dCellSize)
			strItems.AddTail("Prob. Density");
		ShowComboBoxCtrl(strItems, curStr);


	}

	else if(m_iCol == 4)
	{


		CStringList strItems;

		CString str = this->GetItemText(iItem, m_iCol-1);
		str.Trim();

		CString curStr = this->GetItemText(m_iRow, m_iCol);
		//AfxMessageBox(str);

		if(str == "Single Value")
		{

			CStringList strItems;
			strItems.AddTail("Gower Distance");
			strItems.AddTail("Boolean");
			ShowComboBoxCtrl(strItems, curStr);
		}
		else if(str == "Prob. Density")
		{
			CStringList strItems;
			strItems.AddTail("Consistency Measure");
			ShowComboBoxCtrl(strItems, curStr);
		}
	}


	else 
	{
		if (m_pctrlComboBox && m_pctrlComboBox->IsWindowVisible())
			m_pctrlComboBox->ShowWindow(SW_HIDE);
	}
	/*switch (m_iCol)
	{
	case 2:

	//m_iCol = 1;
	ShowComboBoxCtrl(strItems);
	break;
	case 3:


	ShowComboBoxCtrl(strItems);
	break;
	}*/

	this->Invalidate();
}

void CEditableListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (nSBCode != SB_LINEDOWN && nSBCode != SB_LINEUP)
	{
		if (nSBCode == SB_ENDSCROLL)
		{
			if (m_pctrlEdit)
				ShowEditCtrl();
		}
		else if (m_pctrlEdit)
		{
			CommitEditCtrl();
			m_iEditRow = -1;
			m_iEditCol = -1;
			m_pctrlEdit->ShowWindow(SW_HIDE);
		}
	}

	if (m_pctrlComboBox)
		m_pctrlComboBox->ShowWindow(SW_HIDE);
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CEditableListCtrl::OnLvnBeginScroll(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	//LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);
	if (m_pctrlEdit)
	{
		CommitEditCtrl();
		m_iEditRow = -1;
		m_iEditCol = -1;
		m_pctrlEdit->ShowWindow(SW_HIDE);
	}
	*pResult = 0;
}

void CEditableListCtrl::OnLvnEndScroll(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	//LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);
	if (m_pctrlEdit)
		ShowEditCtrl();
	*pResult = 0;
}

void CEditableListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (m_pctrlEdit && m_pctrlEdit->IsWindowVisible())
		m_pctrlEdit->ShowWindow(SW_HIDE);
	if (m_pctrlComboBox && m_pctrlComboBox->IsWindowVisible())
		m_pctrlComboBox->ShowWindow(SW_HIDE);
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CEditableListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	switch (((NMHDR*)lParam)->code)
	{
	case HDN_BEGINTRACKW:
	case HDN_BEGINTRACKA:
		if (m_pctrlEdit && m_pctrlEdit->IsWindowVisible())
			m_pctrlEdit->ShowWindow(SW_HIDE);
		if (m_pctrlComboBox && m_pctrlComboBox->IsWindowVisible())
			m_pctrlComboBox->ShowWindow(SW_HIDE);
		*pResult = 0;
		break;
	}

	return CListCtrl::OnNotify(wParam, lParam, pResult);
}

void CEditableListCtrl::OnDestroy()
{
	delete m_pctrlEdit;
	delete m_pctrlComboBox;
	CListCtrl::OnDestroy();
}

void CEditableListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	if (pNMCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;
	}

	if (pNMCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		DWORD dwItemData = pNMCD->nmcd.lItemlParam;
		if (dwItemData & 1)
		{
			pNMCD->clrText = RGB(128,128,128);
			pNMCD->clrTextBk = 0xFF000000;
		}
		else
		{
			pNMCD->clrText = 0xFF000000;
			pNMCD->clrTextBk = 0xFF000000;
		}
	}

	*pResult = CDRF_DODEFAULT;
}




void CEditableListCtrl::OnCbnSelchangeComboCharaccterization()
{  



	if(this->m_iCol == 3)
	{


		CString strItem;
		m_pctrlComboBox->GetLBText(m_pctrlComboBox->GetCurSel(), strItem);
		strItem.TrimLeft();
		strItem.TrimRight();

		SetItemText(m_iRow, m_iCol, strItem);



		if(strItem == "Single Value")

			SetItemText(m_iRow, m_iCol+1, "Gower Distance");
		else if(strItem == "Prob. Density")
			SetItemText(m_iRow, m_iCol+1, "Consistency Measure");
	}



	if(this->m_iCol == 4)
	{


		CString strItem;
		m_pctrlComboBox->GetLBText(m_pctrlComboBox->GetCurSel(), strItem);
		strItem.TrimLeft();
		strItem.TrimRight();

		SetItemText(m_iRow, m_iCol, strItem);




	}


	this->Invalidate();

	//m_pctrlComboBox->GetCurSel()


}


void CEditableListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{

	/*
	*pResult = 0;

	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
	int iRow = lplvcd->nmcd.dwItemSpec;

	switch(lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT :
	{
	*pResult = CDRF_NOTIFYITEMDRAW;
	return;
	}

	// Modify item text and or background
	case CDDS_ITEMPREPAINT:
	{
	lplvcd->clrText = RGB(0,0,0);
	// If you want the sub items the same as the item,
	// set *pResult to CDRF_NEWFONT
	*pResult = CDRF_NOTIFYSUBITEMDRAW;
	return;
	}

	// Modify sub item text and/or background
	case CDDS_SUBITEM | CDDS_PREPAINT | CDDS_ITEM:
	{

	if(iRow >= m_iNumOfUsedRow)

	lplvcd->clrTextBk = m_colRow;
	/*

	pDC->FillRect(&rect,&brush);
	if(iRow %2){
	lplvcd->clrTextBk = m_colRow;
	}
	else{
	lplvcd->clrTextBk = m_colRow;
	}


	*pResult = CDRF_DODEFAULT;
	return;
	}
	}*/




}



BOOL CEditableListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	/*
	CRect rect;
	GetClientRect(rect);


	POINT mypoint;  


	CBrush brush(m_colRow);



	int chunk_height=GetCountPerPage();
	// pDC->FillRect(&rect,&brush1);


	for (int i=0;i<=chunk_height;i++)
	{


	GetItemPosition(i,&mypoint);
	rect.top=mypoint.y ;
	GetItemPosition(i+1,&mypoint);
	rect.bottom =mypoint.y;
	if(i>=this->m_iNumOfUsedRow)
	pDC->FillRect(&rect,&brush);


	}


	brush.DeleteObject();*/

	return FALSE;
}

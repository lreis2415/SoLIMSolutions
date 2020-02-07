/*******************************************************************************
	Author						: Aravindan Premkumar
	Unregistered Copyright 2003	: Aravindan Premkumar
	All Rights Reserved
	
	This piece of code does not have any registered copyright and is free to be 
	used as necessary. The user is free to modify as per the requirements. As a
	fellow developer, all that I expect and request for is to be given the 
	credit for intially developing this reusable code by not removing my name as 
	the author.
*******************************************************************************/

#include "stdafx.h"
#include "CTIDbgrid.h"
#include "InPlaceCombo.h"
#include "InPlaceEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#defines
#define FIRST_COLUMN				0
#define MIN_COLUMN_WIDTH			10
#define MAX_DROP_DOWN_ITEM_COUNT	10

/////////////////////////////////////////////////////////////////////////////
// CCTIDbgrid

CCTIDbgrid::CCTIDbgrid()
{
	m_iColumnCounts = 0;
	m_iRows=0;
	m_ComboSupportColumnsList.RemoveAll();
	m_ReadOnlyColumnsList.RemoveAll();
	m_strValidEditCtrlChars.Empty();
	m_dwEditCtrlStyle = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_LEFT | ES_NOHIDESEL;
	m_dwDropDownCtrlStyle = WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | 
							CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL;

	//tian
	

	

	m_clrText  = GetSysColor(COLOR_WINDOWTEXT);
	m_clrTextBk= GetSysColor(COLOR_WINDOW);
	m_clrBkgnd = GetSysColor(COLOR_WINDOW);
	crBackSel  = GetSysColor(COLOR_HIGHLIGHT);
	crCategoryBck = GetSysColor(COLOR_INACTIVECAPTION);
	cr3dFace = GetSysColor(COLOR_3DFACE);

	m_cxClient = 0;
	gpn = new CPen(PS_SOLID, 1, RGB(192,192,192));//GetSysColor(COLOR_INACTIVECAPTION));
	gcatpn = new CPen(PS_SOLID, 1, RGB(128,128,128));
	m_cxStateImageOffset = 0;

	CFont * ft = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	LOGFONT lf;
	ASSERT(ft);
	ft->GetLogFont(&lf);
	ftNormal.CreateFontIndirect(&lf);
	lf.lfWeight = 700;
	ftBold.CreateFontIndirect(&lf);
	lf.lfWeight = 0;
	lf.lfUnderline = TRUE;
	ftUnderline.CreateFontIndirect(&lf);
	lf.lfUnderline = FALSE;
	lf.lfItalic = TRUE;
	ftItalic.CreateFontIndirect(&lf);

	pReciper = NULL;

	iSubItemFocus = -1;



	iCtrlId = -1;



	iInternal1 = -1;

	iItemHeight = 17;
	iItemHeightCategory = 16;



	iAutoPreviewHeight = 0;

}

CCTIDbgrid::~CCTIDbgrid()
{
	CInPlaceCombo::DeleteInstance();
	CInPlaceEdit::DeleteInstance();  
}


BEGIN_MESSAGE_MAP(CCTIDbgrid, CListCtrl)
	//{{AFX_MSG_MAP(CCTIDbgrid)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndLabelEdit)
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginLabelEdit)
	//}}AFX_MSG_MAP
	ON_WM_DRAWITEM()

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCTIDbgrid message handlers

CInPlaceCombo* CCTIDbgrid::ShowInPlaceList(int iRowIndex, int iColumnIndex, CStringList& rComboItemsList, 
											   CString strCurSelecetion /*= ""*/, int iSel /*= -1*/)
{
	// The returned obPointer should not be saved
	
	// Make sure that the item is visible
	if (!EnsureVisible(iRowIndex, TRUE))
	{
		return NULL;
	}

	// Make sure that iColumnIndex is valid 
	CHeaderCtrl* pHeader = static_cast<CHeaderCtrl*> (GetDlgItem(FIRST_COLUMN));

	int iColumnCount = pHeader->GetItemCount();

	if (iColumnIndex >= iColumnCount || GetColumnWidth(iColumnIndex) < MIN_COLUMN_WIDTH) 
	{
		return NULL;
	}

	// Calculate the rectangle specifications for the combo box
	CRect obCellRect(0, 0, 0, 0);
	CalculateCellRect(iColumnIndex, iRowIndex, obCellRect);

	int iHeight = obCellRect.Height();  
	int iCount = (int )rComboItemsList.GetCount();

	iCount = (iCount < MAX_DROP_DOWN_ITEM_COUNT) ? 
		iCount + MAX_DROP_DOWN_ITEM_COUNT : (MAX_DROP_DOWN_ITEM_COUNT + 1); 

	obCellRect.bottom += iHeight * iCount; 

	// Create the in place combobox
	CInPlaceCombo* pInPlaceCombo = CInPlaceCombo::GetInstance();
	pInPlaceCombo->ShowComboCtrl(m_dwDropDownCtrlStyle, obCellRect, this, 0, iRowIndex, iColumnIndex, &rComboItemsList, 
								 strCurSelecetion, iSel);
	
	return pInPlaceCombo;
}

CInPlaceEdit* CCTIDbgrid::ShowInPlaceEdit(int iRowIndex, int iColumnIndex, CString& rstrCurSelection)
{
	// Create an in-place edit control
	CInPlaceEdit* pInPlaceEdit = CInPlaceEdit::GetInstance();
		
	CRect obCellRect(0, 0, 0, 0);
	CalculateCellRect(iColumnIndex, iRowIndex, obCellRect);
			
	pInPlaceEdit->ShowEditCtrl(m_dwEditCtrlStyle, obCellRect, this, 0, 
							   iRowIndex, iColumnIndex,
							   m_strValidChars[iColumnIndex], rstrCurSelection);

	return pInPlaceEdit;
}

void CCTIDbgrid::OnHScroll(UINT iSBCode, UINT iPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	if (GetFocus() != this)
	{
		SetFocus();
	}

	CListCtrl::OnHScroll(iSBCode, iPos, pScrollBar);
}

void CCTIDbgrid::OnVScroll(UINT iSBCode, UINT iPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	if (GetFocus() != this)
	{
		SetFocus();
	}

	CListCtrl::OnVScroll(iSBCode, iPos, pScrollBar);
}

void CCTIDbgrid::OnLButtonDown(UINT iFlags, CPoint obPoint) 
{
	// TODO: Add your message handler code here and/or call default

	int iColumnIndex = -1;
	int iRowIndex = -1;

	// Get the current column and row
	if (!HitTestEx(obPoint, &iRowIndex, &iColumnIndex))
	{
		return;
	}

	CListCtrl::OnLButtonDown(iFlags, obPoint);
	
	// If column is not read only then
	// If the SHIFT or CTRL key is down call the base class
	// Check the high bit of GetKeyState to determine whether SHIFT or CTRL key is down
	if ((GetKeyState(VK_SHIFT) & 0x80) || (GetKeyState(VK_CONTROL) & 0x80))
	{
		return;
	}

	// Get the current selection before creating the in place combo box
	CString strCurSelection = GetItemText(iRowIndex, iColumnIndex);

	AfxMessageBox(strCurSelection);
	
	if (-1 != iRowIndex)
	{
		UINT flag = LVIS_FOCUSED;
		
		if ((GetItemState(iRowIndex, flag ) & flag) == flag)
		{
			// Add check for LVS_EDITLABELS
			if (true)//GetWindowLong(m_hWnd, GWL_STYLE) & LVS_EDITLABELS)
			{
				// If combo box is supported
				// Create and show the in place combo box
				if (IsCombo(iColumnIndex))
				{
					CStringList obComboItemsList;
										
					//GetParent()->SendMessage(WM_SET_ITEMS, (WPARAM)iColumnIndex, (LPARAM)&obComboItemsList);  
					{
						CComboBox* pInPlaceCombo = static_cast<CComboBox*> (GetFocus());

						// Get the inplace combbox top left
						CRect obWindowRect;

						pInPlaceCombo->GetWindowRect(&obWindowRect);
						
						CPoint obInPlaceComboTopLeft(obWindowRect.TopLeft()); 
						
						// Get the active list
						// Get the control window rect
						// If the inplace combobox top left is in the rect then
						// The control is the active control
						GetWindowRect(&obWindowRect);
						
						int iColIndex = (int )iColumnIndex;
						
						CStringList* pComboList = reinterpret_cast<CStringList*>(&obComboItemsList);
						pComboList->RemoveAll(); 
	
						if (obWindowRect.PtInRect(obInPlaceComboTopLeft)) 
						{				
							
							if(iColIndex==3)
		                    {
			                   pComboList->AddTail("single value");
			                   pComboList->AddTail("density function");		
		                    }			

		                    else if(iColIndex == 4)
		                    {
			                   pComboList->AddTail("Gower Distance");
			                   pComboList->AddTail("Consistency Measurement");
		                    }
						}
					}
					
					CInPlaceCombo* pInPlaceComboBox = ShowInPlaceList(iRowIndex, iColumnIndex, obComboItemsList, strCurSelection);
					ASSERT(pInPlaceComboBox); 
					
					// Set the selection to previous selection
					pInPlaceComboBox->SelectString(-1, strCurSelection);
				}
				// If combo box is not read only
				// Create and show the in place edit control
				else if (!IsReadOnly(iColumnIndex))
				{
					CInPlaceEdit* pInPlaceEdit = ShowInPlaceEdit(iRowIndex, iColumnIndex, strCurSelection);
				}
			}
		}
	}  
}

bool CCTIDbgrid::HitTestEx(CPoint &obPoint, int* pRowIndex, int* pColumnIndex) const
{
	if (!pRowIndex || !pColumnIndex)
	{
		return false;
	}

	// Get the row index
	*pRowIndex = HitTest(obPoint, NULL);

	if (pColumnIndex)
	{
		*pColumnIndex = 0;
	}

	// Make sure that the ListView is in LVS_REPORT
	if ((GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
	{
		return false;
	}

	// Get the number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);

	int iColumnCount = pHeader->GetItemCount();

	// Get bounding rect of item and check whether obPoint falls in it.
	CRect obCellRect;
	GetItemRect(*pRowIndex, &obCellRect, LVIR_BOUNDS);
	
	if (obCellRect.PtInRect(obPoint))
	{
		// Now find the column
		for (*pColumnIndex = 0; *pColumnIndex < iColumnCount; (*pColumnIndex)++)
		{
			int iColWidth = GetColumnWidth(*pColumnIndex);
			
			if (obPoint.x >= obCellRect.left && obPoint.x <= (obCellRect.left + iColWidth))
			{
				return true;
			}
			obCellRect.left += iColWidth;
		}
	}
	return false;
}

void CCTIDbgrid::SetComboColumns(int iColumnIndex, bool bSet /*= true*/)
{
	// If the Column Index is not present && Set flag is false
	// Then do nothing 
	// If the Column Index is present && Set flag is true
	// Then do nothing
	POSITION Pos = m_ComboSupportColumnsList.Find(iColumnIndex);

	// If the Column Index is not present && Set flag is true
	// Then Add to list
	if ((NULL == Pos) && bSet) 
	{
		m_ComboSupportColumnsList.AddTail(iColumnIndex); 
	}

	// If the Column Index is present && Set flag is false
	// Then Remove from list
	if ((NULL != Pos) && !bSet) 
	{
		m_ComboSupportColumnsList.RemoveAt(Pos); 
	}
}

void CCTIDbgrid::SetReadOnlyColumns(int iColumnIndex, bool bSet /*= true*/)
{
	// If the Column Index is not present && Set flag is false
	// Then do nothing 
	// If the Column Index is present && Set flag is true
	// Then do nothing
	POSITION Pos = m_ReadOnlyColumnsList.Find(iColumnIndex);

	// If the Column Index is not present && Set flag is true
	// Then Add to list
	if ((NULL == Pos) && bSet) 
	{
		m_ReadOnlyColumnsList.AddTail(iColumnIndex); 
	}

	// If the Column Index is present && Set flag is false
	// Then Remove from list
	if ((NULL != Pos) && !bSet) 
	{
		m_ReadOnlyColumnsList.RemoveAt(Pos); 
	}
}

bool CCTIDbgrid::IsReadOnly(int iColumnIndex)
{
	if (m_ReadOnlyColumnsList.Find(iColumnIndex))
	{
		return true;
	}
	
	return false;
}

bool CCTIDbgrid::IsCombo(int iColumnIndex)
{
	if (m_ComboSupportColumnsList.Find(iColumnIndex))
	{
		return true;
	}

	return false;
}

void CCTIDbgrid::CalculateCellRect(int iColumnIndex, int iRowIndex, CRect& robCellRect)
{
	GetItemRect(iRowIndex, &robCellRect, LVIR_BOUNDS);
	
	CRect rcClient;
	GetClientRect(&rcClient);

	if (robCellRect.right > rcClient.right) 
	{
		robCellRect.right = rcClient.right;
	}

	ScrollToView(iColumnIndex, robCellRect); 
}

void CCTIDbgrid::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	// Update the item text with the new text
	SetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText);

	GetParent()->SendMessage(WM_VALIDATE, GetDlgCtrlID(), (LPARAM)pDispInfo); 
	
	*pResult = 0;
}

void CCTIDbgrid::SetValidEditCtrlCharacters(CString &rstrValidCharacters)
{
	m_strValidEditCtrlChars = rstrValidCharacters;
}

void CCTIDbgrid::SetColumnValidEditCtrlCharacters(CString &rstrValidCharacters,int column)
{
	if(column>MAX_LISTCTRL_COLUMNS-1)
		return;
	if(column == -1)
	{
		for(int i=0;i<MAX_LISTCTRL_COLUMNS;i++)
		{
			m_strValidChars[i] = rstrValidCharacters;
		}
	}
	else
		m_strValidChars[column] = rstrValidCharacters;
}

void CCTIDbgrid::EnableHScroll(bool bEnable /*= true*/)
{
	if (bEnable)
	{
		m_dwDropDownCtrlStyle |= WS_HSCROLL;
	}
	else
	{
		m_dwDropDownCtrlStyle &= ~WS_HSCROLL;
	}	
}

void CCTIDbgrid::EnableVScroll(bool bEnable /*= true*/)
{
	if (bEnable)
	{
		m_dwDropDownCtrlStyle |= WS_VSCROLL;
	}
	else
	{
		m_dwDropDownCtrlStyle &= ~WS_VSCROLL;
	}
}

void CCTIDbgrid::ScrollToView(int iColumnIndex, /*int iOffSet, */CRect& robCellRect)
{
	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(&rcClient);

	int iColumnWidth = GetColumnWidth(iColumnIndex);

	// Get the column iOffset
	int iOffSet = 0;
	for (int iIndex_ = 0; iIndex_ < iColumnIndex; iIndex_++)
	{
		iOffSet += GetColumnWidth(iIndex_);
	}

	// If x1 of cell rect is < x1 of ctrl rect or
	// If x1 of cell rect is > x1 of ctrl rect or **Should not ideally happen**
	// If the width of the cell extends beyond x2 of ctrl rect then
	// Scroll

	CSize obScrollSize(0, 0);

	if (((iOffSet + robCellRect.left) < rcClient.left) || 
		((iOffSet + robCellRect.left) > rcClient.right))
	{
		obScrollSize.cx = iOffSet + robCellRect.left;
	}
	else if ((iOffSet + robCellRect.left + iColumnWidth) > rcClient.right)
	{
		obScrollSize.cx = iOffSet + robCellRect.left + iColumnWidth - rcClient.right;
	}

	Scroll(obScrollSize);
	robCellRect.left -= obScrollSize.cx;
	
	// Set the width to the column width
	robCellRect.left += iOffSet;
	robCellRect.right = robCellRect.left + iColumnWidth;
}

void CCTIDbgrid::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (IsReadOnly(pDispInfo->item.iSubItem))
	{
		*pResult = 1;
		return;
	}

	*pResult = 0;
}

int CCTIDbgrid::InsertColumn(int nCol,LPCTSTR lpszColumnHeading,int nFormat ,int nWidth,int nSubItem)
{
	m_iColumnCounts++;
	return CListCtrl::InsertColumn( nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
}
int CCTIDbgrid::InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem,  UINT nState,
		UINT nStateMask,int nImage, LPARAM lParam)
{
	m_iRows++;
	return CListCtrl::InsertItem(nMask,  nItem, lpszItem, nState,
		nStateMask, nImage,lParam);

}
int CCTIDbgrid::GetColumnCounts()
{
	return m_iColumnCounts;
}

void CCTIDbgrid::DeleteAllColumn()
{
	for(int i=0;i<m_iColumnCounts;i++)
	{
		DeleteColumn(0);
	}
}

BOOL CCTIDbgrid::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYUP&&pMsg->wParam==VK_SPACE)
	{
		  POSITION   pos   =  GetFirstSelectedItemPosition();   
		  if(pos   ==   NULL)   
		  {   
		  return   CListCtrl::PreTranslateMessage(pMsg);  
		  } 
		  else
		  {
		    
		  int  index =GetNextSelectedItem(pos);

		     if(index<m_iRows-1)  
			return   CListCtrl::PreTranslateMessage(pMsg);  
		  }

		  CString TmpStr;
		  TmpStr.Format(_T("%d"), m_iRows+1);
		    
		  InsertItem(LVIF_TEXT|LVIF_STATE, m_iRows, 
			TmpStr, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);
			SetItemText(m_iRows,1,"");
			SetItemText(m_iRows,2,"");
			SetItemText(m_iRows,3,"");
		
	}
	return CListCtrl::PreTranslateMessage(pMsg);
}

void CCTIDbgrid::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	


	 CListCtrl::OnPaint();
}

void CCTIDbgrid::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC* pDC=CDC::FromHandle(lpDrawItemStruct->hDC);

	CHeaderCtrl * pHdr = (CHeaderCtrl *)GetDlgItem(0);
	if( pHdr == NULL )
	{
		ASSERT(0);
		return;
	}

	// 创建 CDC 临时对象


	if( lpDrawItemStruct->itemAction & (ODA_DRAWENTIRE | ODA_SELECT) )
	{ 	
		COLORREF crOldText;   // 不要永久改变原 CDC 的属性
		COLORREF crOldBack;   // 不要永久改变原 CDC 的属性

		// 高亮度显示被选中的对象	
		if( lpDrawItemStruct->itemState & ODS_SELECTED )
		{
			crOldText = pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT) );
			crOldBack = pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT) );

			// this is the trick! we always know the row number of the selection (the one with highlight)
			// remember this row for later and update box text
			// this code is kinda redundant (dw *is* lpDrawItemStruct->itemData)
			DWORD dw = GetItemData(lpDrawItemStruct->itemData); 
			//m_strSelect.Format("你选中的是第 %d 行", (int)dw);
			UpdateData(FALSE);
		}

	   	// 清除整个背景区，
		pDC->ExtTextOut( lpDrawItemStruct->rcItem.left,
							  lpDrawItemStruct->rcItem.top,
							  ETO_OPAQUE, &(lpDrawItemStruct->rcItem),"", 0, NULL );              

		// 进行一些设置工作
		CString s;
		int x, y;
		x = lpDrawItemStruct->rcItem.left;
		y =lpDrawItemStruct->rcItem.top;

		HD_ITEM hditem;
		hditem.mask = HDI_WIDTH;
		/// why -1?
		pDC->MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom-1 ); 
		// -2 is probably a text metric (frame?)
		pDC->LineTo( lpDrawItemStruct->rcItem.right-2, lpDrawItemStruct->rcItem.bottom -1);

		for( int i = 0; i < m_iColumnCounts; i++)
		{
			s = GetItemText(lpDrawItemStruct->itemID, i);//>itemData

			// 输出文字 
			pDC->TextOut(  x+2, y, s, s.GetLength()  );

			// 根据表头排列各列
			pHdr->GetItem( i, &hditem);
			x += hditem.cxy;

			// 2 is probably a text metric (frame?)
			pDC->MoveTo( x-2, lpDrawItemStruct->rcItem.bottom -1);//-1后加，以消除竖线下部的突出点 
			pDC->LineTo( x-2, lpDrawItemStruct->rcItem.top -1);//-1后加，以消除竖线上部的断点
		}

		// 绘制选中（获得焦点）对象的状态
		if( lpDrawItemStruct->itemState & ODA_FOCUS )
		{
			pDC->DrawFocusRect( &(lpDrawItemStruct->rcItem) );  
		}

		// 恢复设备上下文（CDC）的状态
		if( lpDrawItemStruct->itemState & ODS_SELECTED )
		{
			pDC->SetTextColor( crOldText );
			pDC->SetBkColor( crOldBack );
		}

	} // end of if on	ODA_DRAWENTIRE | ODA_SELECT

   	if( lpDrawItemStruct->itemAction & ODA_FOCUS )
	{
		pDC->DrawFocusRect( &(lpDrawItemStruct->rcItem) );
	}

}














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

#if !defined(AFX_COMBOLISTCTRL_H__9089600F_374F_4BFC_9482_DEAC0E7133E8__INCLUDED_)
#define AFX_COMBOLISTCTRL_H__9089600F_374F_4BFC_9482_DEAC0E7133E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//the max listCtrl columns
#define MAX_LISTCTRL_COLUMNS 100

#include <afxtempl.h>

class CInPlaceCombo;
class CInPlaceEdit;

// User define message 
// This message is posted to the parent
// The message can be handled to make the necessary validations, if any
#define WM_VALIDATE		WM_USER + 0x7FFD

// User define message 
// This message is posted to the parent
// The message should be handled to spcify the items to the added to the combo
#define WM_SET_ITEMS	WM_USER + 0x7FFC

class CCTIDbgrid : public CListCtrl
{
public:
	
// Implementation
	typedef enum {MODE_READONLY,MODE_DIGITAL_EDIT,MODE_TEXT_EDIT,MODE_COMBO} COMBOLISTCTRL_COLUMN_MODE;

	// Constructor
	CCTIDbgrid();

	// Destructor
	virtual ~CCTIDbgrid();

	// Sets/Resets the column which support the in place combo box
	void SetComboColumns(int iColumnIndex, bool bSet = true);
	
	// Sets/Resets the column which support the in place edit control
	void SetReadOnlyColumns(int iColumnIndex, bool bSet = true);

	// Sets the valid characters for the edit ctrl
	void SetValidEditCtrlCharacters(CString& rstrValidCharacters);

	// Sets the vertical scroll
	void EnableVScroll(bool bEnable = true);

	// Sets the horizontal scroll
	void EnableHScroll(bool bEnable = true);

	//insert column
	int InsertColumn(int nCol,LPCTSTR lpszColumnHeading,int nFormat = LVCFMT_LEFT,int nWidth = -1,int nSubItem = -1);
    
	int InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem,  UINT nState,
		 UINT nStateMask, int nImage, LPARAM lParam);
	//Get column counts
	int GetColumnCounts();

	//delete all column
	void DeleteAllColumn();

	//set column Valid char string
	void SetColumnValidEditCtrlCharacters(CString &rstrValidCharacters,int column = -1);
public:
	//tian
		COLORREF m_clrText, crBackSel, m_clrTextBk, m_clrBkgnd, crCategoryBck, cr3dFace;

	int iCtrlId;
	int iSubItemFocus;
	int m_cxClient;
	int m_cxStateImageOffset;

	int iItemHeight;
	int iAutoPreviewHeight;
	int iItemHeightCategory;

	void SetItemHeight(const int iHeight, const bool bCategory = false);
	void SetAutoPreviewHeight(const int iHeight = 0);
	int  GetAutoPreviewHeight() const;

	int iInternal1;

	CPen * gpn, *gcatpn;

	CFont	ftNormal, ftUnderline, ftBold, ftItalic;

	CWnd * pReciper;

	CWnd * GetReciper() const { return pReciper ? pReciper : GetParent(); };





// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCTIDbgrid)
	//}}AFX_VIRTUAL

protected:

// Methods
	// Generated message map functions
	//{{AFX_MSG(CCTIDbgrid)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
			
// Implementation

	// Returns the row & column index of the column on which mouse click event has occured
	bool HitTestEx(CPoint& rHitPoint, int* pRowIndex, int* pColumnIndex) const;

	// Creates and displays the in place combo box
	CInPlaceCombo* ShowInPlaceList(int iRowIndex, int iColumnIndex, CStringList& rComboItemsList, 
								   CString strCurSelecetion = "", int iSel = -1);

	// Creates and displays the in place edit control
	CInPlaceEdit* ShowInPlaceEdit(int iRowIndex, int iColumnIndex, CString& rstrCurSelection);

	// Calculates the cell rect
	void CalculateCellRect(int iColumnIndex, int iRowIndex, CRect& robCellRect);

	// Checks whether column supports in place combo box
	bool IsCombo(int iColumnIndex);

	// Checks whether column is read only
	bool IsReadOnly(int iColumnIndex);

	// Scrolls the list ctrl to bring the in place ctrl to the view
	void ScrollToView(int iColumnIndex, /*int iOffSet, */CRect& obCellRect);

// Attributes
	
	// List of columns that support the in place combo box
	CList<int, int> m_ComboSupportColumnsList;

	// List of columns that are read only
	CList<int, int> m_ReadOnlyColumnsList;

	// Valid characters
	CString m_strValidEditCtrlChars;

	// The window style of the in place edit ctrl
	DWORD m_dwEditCtrlStyle;

	// The window style of the in place combo ctrl
	DWORD m_dwDropDownCtrlStyle;

	//columnCounts
	int m_iColumnCounts;
	int m_iRows;

	//column types
	COMBOLISTCTRL_COLUMN_MODE m_modeColumn[MAX_LISTCTRL_COLUMNS];

	//column 
	CString m_strValidChars[MAX_LISTCTRL_COLUMNS];
	//int m_
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOLISTCTRL_H__9089600F_374F_4BFC_9482_DEAC0E7133E8__INCLUDED_)

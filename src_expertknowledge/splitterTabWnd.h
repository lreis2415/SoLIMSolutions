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
#ifndef __SPLITTAB_H_
#define __SPLITTAB_H_

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CWndTab;

class CSplitterTabWnd : public CSplitterWnd
{
   friend class CWndTab;
	DECLARE_DYNCREATE(CSplitterTabWnd)
public:
	CSplitterTabWnd();           
	virtual ~CSplitterTabWnd();

public:
   CPtrArray    m_ptrarrayRCView;     // Array for runtime classes for views
// Attributes
private:
   UINT         m_uSelectedViewIndex; // Index for current selected view

   CWndTab*     m_pwndTab;            // Tab window that can be used for switching views


   CStringArray m_sarrayViewName;

// Get/Set
public:
   CWndTab* GetTab() { return m_pwndTab; }

// Operations
public:
	UINT AddView( CRuntimeClass* pRuntimeClass, LPCTSTR pszViewName );
	void SelectView( UINT uIndex );
	BOOL Create( CWnd* pwndParent, CCreateContext* pContext );
    BOOL ReplaceView(CRuntimeClass* pViewClass);

// Overrides
public:
   virtual void RecalcLayout();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitterTabWnd)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CSplitterTabWnd)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/*
*************************************************
*************************************************
******                                     ******
****                                         ****
****    class CWndTab                        ****
****                                         ****
******                                     ******
*************************************************
*************************************************
*/

#define PG_WNDTAB_FONT                 0x00010000

class CWndTab : public CWnd
{
	DECLARE_DYNCREATE(CWndTab)

public:
	CWndTab();
	virtual ~CWndTab();

// Attributes
public:
   CSplitterTabWnd* m_psplitterTabWnd;    // pointer to parent (close relationship)

   DWORD            m_dwFlag;             // for various things

   UINT             m_uHeight;            // the height for tabs
   int              m_iOffsetToLeft;      // distance from left side to first tab
   UINT             m_uOffsetFontGap;     // gap between font boundary and tab
   BOOL             m_bTabModified;       // if tabs are modified

   CDWordArray      m_dwarrayTabWidth;    // the size of each tab
   CFont*           m_pFont;              // text font on the tabs

   static CString   m_stringWindowClass;

// Get/Set
public:
   UINT GetHeight() { return m_uHeight; }

   void SetHeight( UINT uHeight );
   void SetModified() { m_bTabModified = TRUE; }
   void SetSplitterTabWnd( CSplitterTabWnd* psplitterTabWnd ) 
           { m_psplitterTabWnd = psplitterTabWnd; }
   void SetFont( CFont* pFont ) { m_pFont = pFont; }

// Operations
public:
	void DrawTab( CDC* pDC );
	void DrawSelectedTab( CDC* pD );
	void UpdateTabWidth( CDC* pDC );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndTab)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CWndTab)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void CreateFont();
};

#endif

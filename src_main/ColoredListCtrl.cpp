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
// ColoredListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ColoredListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColoredListCtrl

CColoredListCtrl::CColoredListCtrl()
{
	//m_colRow1 = RGB(220,220,220);
	//m_colRow2 = RGB(247,247,247);

	//m_colRow1 = RGB(240,247,249);
	//m_colRow2 = RGB(229,232,239);

	m_colRow = RGB(229,232,239);

	m_iNumOfUsedRow = 1000;
}

CColoredListCtrl::~CColoredListCtrl()
{
}


BEGIN_MESSAGE_MAP(CColoredListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CColoredListCtrl)
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColoredListCtrl message handlers

void CColoredListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
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
        }*/
		

        *pResult = CDRF_DODEFAULT;
        return;
    }
  }
   



}

BOOL CColoredListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
  CRect rect;
  CColoredListCtrl::GetClientRect(rect);


  POINT mypoint;  
  
 // CBrush brush0(m_colRow1);
  //CBrush brush1(m_colRow2);

    CBrush brush(m_colRow);


 
 int chunk_height=GetCountPerPage();
 //pDC->FillRect(&rect,&brush1);

 /*
 for (int i=0;i<=chunk_height;i++)
 {
		
	
	GetItemPosition(i,&mypoint);
	rect.top=mypoint.y ;
	GetItemPosition(i+1,&mypoint);
	rect.bottom =mypoint.y;
	if(i>=this->m_iNumOfUsedRow)
	  pDC->FillRect(&rect,&brush);


 }*/

 // brush0.DeleteObject();
 // brush1.DeleteObject();
  brush.DeleteObject();

  return FALSE;
}

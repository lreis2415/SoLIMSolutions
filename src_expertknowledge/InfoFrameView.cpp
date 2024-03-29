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
// DatabaseView.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "InfoFrameView.h"
#include ".\databaseview.h"
#include "MainFrm.h"
#include "SoLIMDoc.h"
#include "PixelInfoView.h"

#include "Utility.h"

#include "DataExplorerView.h"
#include "MetadataView.h"

#include<CMath>
#include ".\infoframeview.h"




// CDatabaseView

IMPLEMENT_DYNCREATE(CInfoFrameView, CView)

CInfoFrameView::CInfoFrameView()
{

}

CInfoFrameView::~CInfoFrameView()
{
}


BEGIN_MESSAGE_MAP(CInfoFrameView, CView)
	ON_WM_SIZE()	
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDatabaseView drawing

void CInfoFrameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());
}


// CDatabaseView diagnostics

#ifdef _DEBUG
void CInfoFrameView::AssertValid() const
{
	CView::AssertValid();
}

void CInfoFrameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CDatabaseView message handlers

void CInfoFrameView::OnSize(UINT nType, int cx, int cy)
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CView::OnSize(nType, cx, cy);

	SwitchView();

}




void CInfoFrameView::OnDraw(CDC* pDC)
{


}


int CInfoFrameView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rect;
	GetClientRect(&rect);
	CCreateContext *pContext = (CCreateContext *) lpCreateStruct->lpCreateParams;
	m_wndSplitter.CreateStatic(this,1,2);  //split this window to 1 row and 2 columns
	m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CPixelInfoView),
		CSize(250, 1),
		pContext);

	m_wndSplitter.CreateView(0,1,
		RUNTIME_CLASS(CMetadataView),
		CSize(1,1),
		pContext);

	CMetadataView *pMetadataView = (CMetadataView *)(m_wndSplitter.GetPane(0,1));	 
	pMetadataView->OnInitialUpdate();
	return 0;

}


void CInfoFrameView::SwitchView()
{
	CRect rect;
	GetClientRect(&rect);
	int cx = rect.Width();
	int cy = rect.Height();
	m_wndSplitter.MoveWindow(-2,-2,cx,cy+3);
	m_wndSplitter.SetRowInfo(0,cy-cy/5,0);
	m_wndSplitter.RecalcLayout();
}



void CInfoFrameView::OnDestroy()
{
	CView::OnDestroy();
}


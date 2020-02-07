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
// KnowledgeView.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "KnowledgeView.h"
#include ".\knowledgeview.h"
#include "MainFrm.h"
#include "SoLIMDoc.h"


// CKnowledgeView

IMPLEMENT_DYNCREATE(CKnowledgeView, CFormView)

CKnowledgeView::CKnowledgeView()
: CFormView(CKnowledgeView::IDD)
{
}

CKnowledgeView::~CKnowledgeView()
{

}

void CKnowledgeView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FRAME, m_staticFrame);
	DDX_Control(pDX, IDC_STATIC_RULE_FRAME, m_staticRuleFrame);
	DDX_Control(pDX, IDC_STATIC_RULE_FOR, m_staticRuleFor);
}

BEGIN_MESSAGE_MAP(CKnowledgeView, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CKnowledgeView diagnostics

#ifdef _DEBUG
void CKnowledgeView::AssertValid() const
{
	CFormView::AssertValid();
}

void CKnowledgeView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CKnowledgeView message handlers

void CKnowledgeView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if(m_staticFrame.GetSafeHwnd())
	{
		CRect rect(0,-5,cx,cy);
		m_staticFrame.MoveWindow(&rect);
	}

	if(m_staticRuleFor.GetSafeHwnd())
	{
		CRect rect(0.5*cx/10,0.4*cy/10,9.5*cx/10,0.4*cy/10+20);
		m_staticRuleFor.MoveWindow(&rect);

	}

	CRect rcFrame(0.5*cx/10,cy/10,9.5*cx/10,9.5*cy/10);
	if(m_staticRuleFrame.GetSafeHwnd())
	{
		m_staticRuleFrame.MoveWindow(&rcFrame);
	}

	if(m_norulePage.GetSafeHwnd())
	{
		m_norulePage.MoveWindow(&rcFrame);
	}
	if(m_FreehandPage.GetSafeHwnd())
	{
		m_FreehandPage.MoveWindow(&rcFrame);
	}
	if(m_rangePage.GetSafeHwnd())
	{
		m_rangePage.MoveWindow(&rcFrame);
	}
	if(m_wordPage.GetSafeHwnd())
	{
		m_wordPage.MoveWindow(&rcFrame);
	}
	if(m_pointPage.GetSafeHwnd())
	{
		m_pointPage.MoveWindow(&rcFrame);
	}
	if(m_enumPage.GetSafeHwnd())
	{
		m_enumPage.MoveWindow(&rcFrame);
	}
	
}


void CKnowledgeView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());

	CRect rect;
	GetClientRect(&rect);

	int cx = rect.right;
	int cy = rect.bottom;

	if(m_staticFrame.GetSafeHwnd())
	{
		m_staticFrame.MoveWindow(&rect);
	}

	if(m_norulePage.GetSafeHwnd())
	{
		m_norulePage.DestroyWindow();
	}
	if(m_FreehandPage.GetSafeHwnd())
	{
		m_FreehandPage.DestroyWindow();
	}
	if(m_rangePage.GetSafeHwnd())
	{
		m_rangePage.DestroyWindow();
	}
	if(m_wordPage.GetSafeHwnd())
	{
		m_wordPage.DestroyWindow();
	}
	if(m_pointPage.GetSafeHwnd())
	{
		m_pointPage.DestroyWindow();
	}
	if(m_enumPage.GetSafeHwnd())
	{
		m_enumPage.DestroyWindow();
	}

	if(m_staticRuleFor.GetSafeHwnd())
	{
		CRect rect(0.5*cx/10,0.4*cy/10,9.5*cx/10,0.4*cy/10+20);
		m_staticRuleFor.MoveWindow(&rect);
	}

	CRect rcFrame(0.5*cx/10,cy/10,9.5*cx/10,9.5*cy/10);	
	if(m_staticRuleFrame.GetSafeHwnd())
	{
		m_staticRuleFrame.MoveWindow(&rcFrame);
	}

	
	pFrame->m_leftBar.ExpandNode(2);
	FillKnowledgeView(pFrame->m_leftBar.m_pCurRule);

}

void CKnowledgeView::FillKnowledgeView(CRule *pRule)
{

	if(m_norulePage.GetSafeHwnd())
	{
		m_norulePage.DestroyWindow();
	}
	if(m_FreehandPage.GetSafeHwnd())
	{
		m_FreehandPage.DestroyWindow();
	}
	if(m_rangePage.GetSafeHwnd())
	{
		m_rangePage.DestroyWindow();
	}
	if(m_wordPage.GetSafeHwnd())
	{
		m_wordPage.DestroyWindow();
	}
	if(m_pointPage.GetSafeHwnd())
	{
		m_pointPage.DestroyWindow();
	}
	if(m_enumPage.GetSafeHwnd())
	{
		m_enumPage.DestroyWindow();
	}

	CRect rect;
	GetClientRect(&rect);
	int cx = rect.right;
	int cy = rect.bottom;

	CRect rcFrame(0.5*cx/10,cy/10,9.5*cx/10,9.5*cy/10);

	if(pRule == NULL)
	{
		m_norulePage.Create(IDD_DIALOG_NORULE,this);
		m_norulePage.MoveWindow(&rcFrame);
		m_norulePage.ShowWindow(SW_SHOW);
		m_staticRuleFor.SetWindowText("No rule is selected.");
		return;

	}

	else if(pRule->getRuleType() == 0)
	{
		m_rangePage.Create(IDD_DIALOG_RANGE,this);
		m_rangePage.MoveWindow(&rcFrame);
		m_rangePage.ShowWindow(SW_SHOW);
	
		m_staticRuleFor.SetWindowText("Rule For Variable: "+pRule->getRuleName());		
		m_rangePage.FillPage();
	}
	else if(pRule->getRuleType()==1)
	{
		m_FreehandPage.Create(IDD_DIALOG_CURVE,this);
		m_FreehandPage.MoveWindow(&rcFrame);
		m_FreehandPage.ShowWindow(SW_SHOW);
		m_staticRuleFor.SetWindowText("Rule For Variable: "+pRule->getRuleName());
		m_FreehandPage.FillPage();
	}
	else if(pRule->getRuleType()==2)
	{
		m_wordPage.Create(IDD_DIALOG_WORD,this);
		m_wordPage.MoveWindow(&rcFrame);
		m_wordPage.ShowWindow(SW_SHOW);
		m_staticRuleFor.SetWindowText("Rule For Variable: "+pRule->getRuleName());
		m_wordPage.FillPage();
	}

	else if(pRule->getRuleType()==3)
	{
		m_pointPage.Create(IDD_DIALOG_POINT,this);
		m_pointPage.MoveWindow(&rcFrame);
		m_pointPage.ShowWindow(SW_SHOW);
		m_staticRuleFor.SetWindowText("Rule For Variable: "+pRule->getRuleName());
		m_pointPage.FillPage();
	}

	else if(pRule->getRuleType()==4)
	{
		m_enumPage.Create(IDD_DIALOG_ENUMERATED,this);
		m_enumPage.MoveWindow(&rcFrame);
		m_enumPage.ShowWindow(SW_SHOW);
		m_staticRuleFor.SetWindowText("Rule For Variable: "+pRule->getRuleName());
		m_enumPage.FillPage();
	}

	Invalidate();



}


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
// ProjectView.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "ProjectView.h"
#include ".\projectview.h"
#include "MainFrm.h"
#include "SoLIMDoc.h"


// CProjectView

IMPLEMENT_DYNCREATE(CProjectView, CFormView)

CProjectView::CProjectView()
	: CFormView(CProjectView::IDD)
{
	//m_pProject= NULL;
}

CProjectView::~CProjectView()
{
}

void CProjectView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PROJECT_NAME, m_staticProjectName);
	DDX_Control(pDX, IDC_EDIT_PROJECT_NAME, m_editProjectName);
	DDX_Control(pDX, IDC_STATIC_FRAME, m_staticFrame);
	DDX_Control(pDX, IDC_EDIT_PROJECT_DIR, m_editProjectDir);
	DDX_Control(pDX, IDC_STATIC_PROJECT_DIR, m_staticProjectDir);
	DDX_Control(pDX, IDC_STATIC_SOIL_NUM, m_staticSoilNum);
	DDX_Control(pDX, IDC_EDIT_SOIL_NUM, m_editSoilNum);
	DDX_Control(pDX, IDC_BTN_RUN_INFERENCE, m_btnRunInference);
	DDX_Control(pDX, IDC_STATIC_LAYER_LIST, m_staticLayerList);
	DDX_Control(pDX, IDC_LIST_LAYER, m_listLayerList);
}

BEGIN_MESSAGE_MAP(CProjectView, CFormView)
	ON_WM_SIZE()	
//	ON_WM_ACTIVATE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_RUN_INFERENCE, OnBnClickedBtnRunInference)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CProjectView diagnostics

#ifdef _DEBUG
void CProjectView::AssertValid() const
{
	CFormView::AssertValid();
}

void CProjectView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CProjectView message handlers

void CProjectView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if(m_staticFrame.GetSafeHwnd())
	{
		CRect rect(0,0,cx,cy);
		m_staticFrame.MoveWindow(&rect);
	}

	if(m_staticProjectName.GetSafeHwnd())
	{
		CRect rect(cx/10,cy/10,cx/10+130,cy/10+20);
		m_staticProjectName.MoveWindow(&rect);
	}


	if(m_editProjectName.GetSafeHwnd())   
	{
		//m_ProjectName.SetWindowText("TestProject");
		 CRect rect(cx/10+150,cy/10,cx/10+7*cx/10,cy/10+20);
		 m_editProjectName.MoveWindow(&rect);
	}	

	
	if(m_staticProjectDir.GetSafeHwnd())
	{
		CRect rect(cx/10,2*cy/10,cx/10+130,2*cy/10+20);
		m_staticProjectDir.MoveWindow(&rect);
	}


	if(m_editProjectDir.GetSafeHwnd())   
	{
	    CRect rect(cx/10+150,2*cy/10,cx/10+7*cx/10,2*cy/10+20);
		 m_editProjectDir.MoveWindow(&rect);
	}	
	
	if(m_staticLayerList.GetSafeHwnd())
	{
		CRect rect(cx/10,3*cy/10,cx/10+130,3*cy/10+20);
		m_staticLayerList.MoveWindow(&rect);
	}

	if(m_listLayerList.GetSafeHwnd())
	{
		CRect rect(cx/10+150,3*cy/10,cx/10+7*cx/10,3*cy/10+4*cy/10);
		m_listLayerList.MoveWindow(&rect);
	}

	if(m_staticSoilNum.GetSafeHwnd())
	{
		//m_staticProjectName.SetWindowText("ProjectName");
		CRect rect(cx/10,8*cy/10,cx/10+190,8*cy/10+20);
		m_staticSoilNum.MoveWindow(&rect);
	}

	

	if(m_editSoilNum.GetSafeHwnd())   
	{
	  CRect rect(cx/10+200,8*cy/10,cx/10+7*cx/10,8*cy/10+20);
		 m_editSoilNum.MoveWindow(&rect);
	}	

	
	if(m_btnRunInference.GetSafeHwnd())   
	{
	     CRect rect(7*cx/10,9*cy/10,7*cx/10+80,9*cy/10+20);
		 m_btnRunInference.MoveWindow(&rect);
	}	
	
	// TODO: Add your message handler code here
}



//void CProjectView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
//{
//	CFormView::OnActivate(nState, pWndOther, bMinimized);
//	MessageBox("dfsd");
//
//	// TODO: Add your message handler code here
//}


void CProjectView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	



	m_btnRunInference.SetCaption("&Run Inference");
	m_btnRunInference.SetStyleBorder(CGuiButton::STYLEXP);

	CRect rect;
	this->GetClientRect(&rect);
	if(m_staticFrame.GetSafeHwnd())
	{
		m_staticFrame.MoveWindow(&rect);
	}
	int cx = rect.right;
	int cy = rect.bottom;

	
	if(m_staticProjectName.GetSafeHwnd())
	{
		CRect rect(cx/10,cy/10,cx/10+130,cy/10+20);
		m_staticProjectName.MoveWindow(&rect);
	}


	if(m_editProjectName.GetSafeHwnd())   
	{
		//m_ProjectName.SetWindowText("TestProject");
		 CRect rect(cx/10+150,cy/10,cx/10+7*cx/10,cy/10+20);
		 m_editProjectName.MoveWindow(&rect);
	}	

	
	if(m_staticProjectDir.GetSafeHwnd())
	{
		CRect rect(cx/10,2*cy/10,cx/10+130,2*cy/10+20);
		m_staticProjectDir.MoveWindow(&rect);
	}


	if(m_editProjectDir.GetSafeHwnd())   
	{
	    CRect rect(cx/10+150,2*cy/10,cx/10+7*cx/10,2*cy/10+20);
		 m_editProjectDir.MoveWindow(&rect);
	}	

	if(m_staticLayerList.GetSafeHwnd())
	{
		CRect rect(cx/10,3*cy/10,cx/10+130,3*cy/10+20);
		m_staticLayerList.MoveWindow(&rect);
	}

	
	if(m_listLayerList.GetSafeHwnd())
	{
		CRect rect(cx/10+150,3*cy/10,cx/10+7*cx/10,3*cy/10+4*cy/10);
		m_listLayerList.MoveWindow(&rect);
	}

	if(m_staticSoilNum.GetSafeHwnd())
	{
		//m_staticProjectName.SetWindowText("ProjectName");
		CRect rect(cx/10,8*cy/10,cx/10+190,8*cy/10+20);
		m_staticSoilNum.MoveWindow(&rect);
	}


	if(m_editSoilNum.GetSafeHwnd())   
	{
	  CRect rect(cx/10+200,8*cy/10,cx/10+7*cx/10,8*cy/10+20);
		 m_editSoilNum.MoveWindow(&rect);
	}	

	
	if(m_btnRunInference.GetSafeHwnd())   
	{
	     CRect rect(7*cx/10,9*cy/10,7*cx/10+80,9*cy/10+20);
		 m_btnRunInference.MoveWindow(&rect);
	}

    CSoLIMDoc *pDoc = (CSoLIMDoc *)(this->GetDocument());
		pDoc->setCurViewType(0);
	
	FillProjectView(pDoc->m_pProject);

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	pFrame->m_LeftBar.m_ctServer.DeleteComponent(0);
	pFrame->m_LeftBar.m_trProjectTree.DeleteAllItems();
	pFrame->m_LeftBar.FillProjectTree(pDoc);
	pFrame->m_LeftBar.m_ctServer.AddComponen(&(pFrame->m_LeftBar.m_trProjectTree));



}

void CProjectView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages
}

void CProjectView::OnBnClickedBtnRunInference()
{
	AfxMessageBox("Run inference here!");
	// TODO: Add your control notification handler code here
}


void CProjectView::FillProjectView(CProject *pProject)
{
	if(pProject!=NULL)
	{		
		m_editProjectName.SetWindowText(pProject->getProjectName());
		m_editProjectDir.SetWindowText(pProject->getProjectDir());
		CString str;
		str.Format("%d",pProject->m_pKnowledgeBase->getSoilTypeNum());
		m_editSoilNum.SetWindowText(str);
	}
	
}

void CProjectView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CFormView::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here
}

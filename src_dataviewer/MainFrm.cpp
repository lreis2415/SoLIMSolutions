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
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SoLIM.h"

#include "MainFrm.h"
#include ".\mainfrm.h"
#include "DatabaseView.h"
#include "KnowledgeView.h"
#include "InferenceView.h"
#include "DataExplorerView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP	
	ON_WM_SIZE()	
	ON_WM_SIZING()

	ON_COMMAND(ID_NAVIGATION_ZOOMIN, OnNavigationZoomin)
	ON_COMMAND(ID_NAVIGATION_ZOOMOUT, OnNavigationZoomout)
	ON_COMMAND(ID_NAVIGATION_ENTIRE_LAYER,OnNavigationEntireLayer)	
	ON_UPDATE_COMMAND_UI(ID_NAVIGATION_ZOOMIN, OnUpdateNavigationZoomin)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATION_ZOOMOUT, OnUpdateNavigationZoomout)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATION_PAN, OnUpdateNavigationPan)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATION_RECT_ZOOM, OnUpdateNavigationRectZoom)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATION_ENTIRE_LAYER, OnUpdateNavigationEntireLayer)
	ON_COMMAND(ID_NAVIGATION_PAN,OnNavigationPan)
	ON_COMMAND(ID_NAVIGATION_RECT_ZOOM,OnNavigationRectZoom)

	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_COMMAND(ID_VIEW_PROJECT_TREE, OnViewProjectTree)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROJECT_TREE, OnUpdateViewProjectTree)



	ON_WM_CLOSE()

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
		ID_INDICATOR_CAPS,
		ID_INDICATOR_NUM,
		ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{


	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	InitMenu(IDR_MAINFRAME);
	InitToolBar(IDR_MAINFRAME);
	//InitStatusBar(indicators,sizeof(indicators)/sizeof(UINT));

	if (!m_leftBar.Create(_T("         Control Panel"), WS_VISIBLE, this, 995))
	{
		TRACE0("Failed to create Control Panel\n");
		return -1;      
	}

	m_leftBar.SetBarStyle(m_leftBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	EnableDocking(CBRS_ALIGN_ANY);
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar,m_dockTop);
	DockControlBar(&m_wndToolBar,m_dockTop);
	m_leftBar.EnableDocking(CBRS_ALIGN_LEFT);
	DockControlBar(&m_leftBar, AFX_IDW_DOCKBAR_LEFT);

	sProfile = _T("SoLIM");

	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)GetActiveDocument();

	

	//this->m_wndStatusBar.SetPaneText(0,"fsdfl");

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	  m_strTitle = "SoLIM Data Viewer"; 
	  cs.x = 40;
	  cs.y = 40;
	  cs.cx = 900; 
      cs.cy = 700; 
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CGuiFrameWnd::OnSize(nType, cx, cy);
	//Invalidate();
}

void CMainFrame::OnViewToolbar()
{
	ShowControlBar(&m_wndToolBar, !m_wndToolBar.IsVisible(), FALSE); 

}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{  
	m_splitterTabWnd.AddView( RUNTIME_CLASS( CDatabaseView ),"GIS Database Panel" );   
	
	m_splitterTabWnd.Create( this, pContext );
	SetActiveView((CView *)m_splitterTabWnd.GetPane(0,0));//default is database view
	return TRUE;
}

void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect)
{
	if   ((pRect->right-pRect->left)<800)   pRect->right=pRect->left+800;   
	if   ((pRect->bottom-pRect->top)<600)   pRect->bottom =pRect->top+600;  
	CGuiFrameWnd::OnSizing(fwSide, pRect);
}


HMENU CMainFrame::getMenuHandle()
{
	return m_wndMenuBar.m_hMenu;
}

void CMainFrame::OnViewProjectTree()
{
	ShowControlBar(&m_leftBar, !m_leftBar.IsVisible(), FALSE); 

}

void CMainFrame::OnUpdateViewProjectTree(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_leftBar.IsVisible());

}

void CMainFrame::OnNavigationZoomin()
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	pApp->m_iIsOriginChanged=1;
	CDataExplorerView *pDataView = (CDataExplorerView *)(((CDatabaseView *)(m_splitterTabWnd.GetPane(0,0)))->m_wndSplitter.GetPane(0,0));
	pDataView->ImageZoom(pDataView ->m_ptMidPt,pDataView ->m_dZoom+0.5);
	pDataView->m_iToolChoice=0;
	pDataView->Invalidate();
}
void CMainFrame::OnUpdateNavigationZoomin(CCmdUI *pCmdUI)
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
	if(pDoc->m_pProjectEdit!=NULL)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);

}

void CMainFrame::OnNavigationZoomout()
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	pApp->m_iIsOriginChanged=1;
	CDataExplorerView *pDataView = (CDataExplorerView *)(((CDatabaseView *)(m_splitterTabWnd.GetPane(0,0)))->m_wndSplitter.GetPane(0,0));
	if(pDataView ->m_dZoom>1)
		pDataView->ImageZoom(pDataView->m_ptMidPt,pDataView->m_dZoom-0.5);
	pDataView->m_iToolChoice=0;
	pDataView->Invalidate();
}
void CMainFrame::OnUpdateNavigationZoomout(CCmdUI *pCmdUI)
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
	if(pDoc->m_pProjectEdit!=NULL)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);

}


void CMainFrame::OnNavigationPan()
{
	CDataExplorerView *pDataView = (CDataExplorerView *)(((CDatabaseView *)(m_splitterTabWnd.GetPane(0,0)))->m_wndSplitter.GetPane(0,0));
	if(pDataView->m_iToolChoice!=1)
	{
		pDataView->m_iToolChoice=1;
	}
	else
	{
		pDataView->m_iToolChoice=0;
	}
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	pApp->m_iIsOriginChanged=1;
}
void CMainFrame::OnUpdateNavigationPan(CCmdUI *pCmdUI)
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
	if(pDoc->m_pProjectEdit!=NULL)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);

}

void CMainFrame::OnNavigationRectZoom()
{
	CDataExplorerView *pDataView = (CDataExplorerView *)(((CDatabaseView *)(m_splitterTabWnd.GetPane(0,0)))->m_wndSplitter.GetPane(0,0));
	if(pDataView->m_iToolChoice!=2)
	{
		pDataView->m_iToolChoice=2;
	}
	else
	{
		pDataView->m_iToolChoice=0;
	}
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	pApp->m_iIsOriginChanged=1;
}

void CMainFrame::OnUpdateNavigationRectZoom(CCmdUI *pCmdUI)
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
	if(pDoc->m_pProjectEdit!=NULL)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);

}


void CMainFrame::OnNavigationEntireLayer()
{
	CDataExplorerView *pDataView = (CDataExplorerView *)(((CDatabaseView *)(m_splitterTabWnd.GetPane(0,0)))->m_wndSplitter.GetPane(0,0));
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	pApp->m_iIsOriginChanged=0;
	pDataView->m_dZoom=1;
	pDataView->m_iToolChoice=0;
	Invalidate(false);
}

void CMainFrame::OnUpdateNavigationEntireLayer(CCmdUI *pCmdUI)
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
	if(pDoc->m_pProjectEdit!=NULL)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);

}


void CMainFrame::OnClose()
{
	CSoLIMDoc *pDoc = (CSoLIMDoc *)GetActiveDocument();
	
	CGuiFrameWnd::OnClose();
}




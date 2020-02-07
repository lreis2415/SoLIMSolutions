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
// DataExplorerView.cpp : implementation file
//
#include "stdafx.h"
#include "SoLIM.h"
#include "DataExplorerView.h"
#include ".\DataExplorerView.h"
#include "MainFrm.h"
#include "SoLIMDoc.h"
#include "Utility.h"
#include "ThreeDR.h"
#include "MetadataView.h"
#include "InfoFrameView.h"
#include "DatabaseView.h"
#include "PixelInfoView.h"

// CDataExplorerView

IMPLEMENT_DYNCREATE(CDataExplorerView, CView)

CDataExplorerView::CDataExplorerView()
{
	//m_iZoom=1;
	m_GLPixelIndex = 0;
	m_hGLContext = NULL;
	m_ptOriginOffset.x=0;
	m_ptOriginOffset.y=0;
	m_iIsLButtonDown=0;
	m_iToolChoice=0;

}

CDataExplorerView::~CDataExplorerView()
{

}


BEGIN_MESSAGE_MAP(CDataExplorerView, CView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CDataExplorerView drawing

void CDataExplorerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}


// CDataExplorerView diagnostics

#ifdef _DEBUG
void CDataExplorerView::AssertValid() const
{
	CView::AssertValid();
}

void CDataExplorerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CDataExplorerView message handlers

void CDataExplorerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,cx,0,cy);		
	glViewport(0,0,cx,cy);
	Invalidate();

}

BOOL CDataExplorerView::OnEraseBkgnd(CDC* pDC)
{
	return CView::OnEraseBkgnd(pDC);

}

void CDataExplorerView::FillDataExplorerView(CEnvDatabase *pDatabase)
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CWnd* pWnd=pFrame->GetActiveWindow();	
	Invalidate();

}

void CDataExplorerView::OnDraw(CDC* pDC)
{

	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CSoLIMDoc* pDoc = (CSoLIMDoc *)GetDocument();
	int iCurrEnvLayerIndex=pApp->m_iCurrEnvLayerIndex;
	CRect rectClientRect;
	GetClientRect(rectClientRect);
	m_ptMidPt.x=rectClientRect.right/2;
	m_ptMidPt.y=rectClientRect.bottom/2;

	
	if(iCurrEnvLayerIndex!=-1)
	{
		m_iRows=pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->getRow();
		m_iCols=pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->getCol();
		DrawDataLayer(iCurrEnvLayerIndex);
	}

	if(m_iIsLButtonDown==1 && m_iToolChoice==2)
	{


		//source codes are modified by Fei Du because of the drawing problem in Windows 7 and Widows Vista
		//HBRUSH Brush=(HBRUSH)GetStockObject(HOLLOW_BRUSH); 
		//pDC->SelectObject(Brush);
		//pDC->SetBkColor(RGB(255, 0, 0));
	
	
		//pDC->SetDCBrushColor(RGB(255,0,0));
		CPen NewPen(PS_SOLID,1,RGB(255,0,0));
	    CPen *pOldPen=pDC->SelectObject(&NewPen);
		//pDC->Rectangle(m_ptMouseDownPt.x,m_ptMouseDownPt.y,m_ptMouseDownPt.x+m_ptOriginOffset.x,m_ptMouseDownPt.y+m_ptOriginOffset.y);

			pDC->MoveTo(m_ptMouseDownPt.x,m_ptMouseDownPt.y);
			pDC->LineTo(m_ptMouseDownPt.x+m_ptOriginOffset.x,m_ptMouseDownPt.y);
		
		pDC->LineTo(m_ptMouseDownPt.x+m_ptOriginOffset.x,m_ptMouseDownPt.y+m_ptOriginOffset.y);
		pDC->LineTo(m_ptMouseDownPt.x,m_ptMouseDownPt.y+m_ptOriginOffset.y);

		pDC->LineTo(m_ptMouseDownPt.x,m_ptMouseDownPt.y);
	    pDC->SelectObject(pOldPen);
		
	}

	

}

BOOL CDataExplorerView::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|
			PFD_DOUBLEBUFFER|PFD_SUPPORT_GDI,
			PFD_TYPE_RGBA,
			24,
			0,0,0,0,0,0,
			0,
			0,
			0,
			0,0,0,0,
			32,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
			0,0,0
	};

	m_GLPixelIndex = ChoosePixelFormat(hDC,&pixelDesc);
	if(m_GLPixelIndex==0)
	{
		m_GLPixelIndex = 1;
		if(DescribePixelFormat(hDC,m_GLPixelIndex,sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0)
		{
			return FALSE;
		}
	}

	if(SetPixelFormat(hDC,m_GLPixelIndex,&pixelDesc)==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CDataExplorerView::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);
	if(m_hGLContext==NULL)
	{//failed
		return FALSE;
	}

	if(wglMakeCurrent(hDC,m_hGLContext)==FALSE)
	{//select as current RC failed
		return FALSE;
	}

	return TRUE;
}
int CDataExplorerView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	HWND hWnd = GetSafeHwnd();    
	HDC hDC = ::GetDC(hWnd);
	if(SetWindowPixelFormat(hDC)==FALSE)
	{
		return 0;
	}
	if(CreateViewGLContext(hDC)==FALSE)
	{
		return 0;
	}
	return 0;

}


void CDataExplorerView::DrawDataLayer(int iIndex)
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();

	CSoLIMDoc* pDoc = (CSoLIMDoc *)GetDocument();
	float fMaxValue=pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(iIndex)->m_pTheData->getDataMax();
	float fMinValue=pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(iIndex)->m_pTheData->getDataMin();
	double dRange=fMaxValue-fMinValue;
	//draw selected environment database layer
	if(pApp->m_iIsOriginChanged==0)
	{
		double dTempXZoom=2*double(m_ptMidPt.x)/m_iCols*4/5;
		double dTempYZoom=2*double(m_ptMidPt.y)/m_iRows*4/5;
		if(dTempXZoom<=dTempYZoom)
		{
			m_dZoom=dTempXZoom;
		}
		else
		{
			m_dZoom=dTempYZoom;
		}
		m_ptImgTopLeftPt.x=m_ptMidPt.x-m_iCols/2*m_dZoom;
		m_ptImgTopLeftPt.y=m_ptMidPt.y-m_iRows/2*m_dZoom;
		m_ptOriginPt=CPoint(m_ptMidPt.x-m_iCols/2*m_dZoom,m_ptMidPt.y-m_iRows/2*m_dZoom);
		m_ptOriginOffset=CPoint(0,0);
	}
	else if(m_iIsLButtonDown==1 && m_iToolChoice==1 )
	{
		m_ptOriginPt.x=(m_ptImgTopLeftPt.x+m_ptOriginOffset.x);
		m_ptOriginPt.y=2*m_ptMidPt.y-(m_ptImgTopLeftPt.y+m_iRows*m_dZoom+m_ptOriginOffset.y);
	}
	else if(m_iToolChoice==2 && m_iIsLButtonDown==0)
	{
		m_ptOriginPt.x=(m_ptImgTopLeftPt.x+m_ptOriginOffset.x);
		m_ptOriginPt.y=2*m_ptMidPt.y-(m_ptImgTopLeftPt.y+m_iRows*m_dZoom+m_ptOriginOffset.y);
	}

	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	int iTempCol =m_iCols;
	if(m_iCols%4 != 0)	
		iTempCol = m_iCols+(4-m_iCols%4);
	GLubyte* image=new GLubyte[m_iRows*iTempCol];
	for(int i=m_iRows-1;i>=0;i--)
		for(int j=0;j<m_iCols;j++)
			image[(m_iRows-i-1)*iTempCol+j]= (GLubyte)((pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(iIndex)->getData(i,j)-fMinValue)/dRange*255);
	glPixelZoom(m_dZoom,m_dZoom); 
	glRasterPos3f(0,0,0);
	glBitmap(0,0,0,0,m_ptOriginPt.x,m_ptOriginPt.y,NULL);
	glDrawPixels(m_iCols,m_iRows,GL_LUMINANCE ,GL_UNSIGNED_BYTE ,image); 
	glFlush();
	SwapBuffers(wglGetCurrentDC());
	delete [] image;
}


void CDataExplorerView::ImageZoom(CPoint ptFocus, double dZoom)
{
	m_ptOriginOffset=CPoint(0,0);
	double dTempZoom=m_dZoom;
	m_dZoom=dZoom;
	CPoint ptTempOffset=m_ptMidPt-ptFocus;
	m_ptImgTopLeftPt+=ptTempOffset;
	m_ptImgTopLeftPt.x=(m_ptImgTopLeftPt.x-m_ptMidPt.x)/dTempZoom*m_dZoom+m_ptMidPt.x;
	m_ptImgTopLeftPt.y=(m_ptImgTopLeftPt.y-m_ptMidPt.y)/dTempZoom*m_dZoom+m_ptMidPt.y;
	m_ptOriginPt.x=m_ptImgTopLeftPt.x;
	m_ptOriginPt.y=2*m_ptMidPt.y-(m_ptImgTopLeftPt.y+m_iRows*m_dZoom);
	Invalidate(false);
}

BOOL CDataExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	return CView::PreCreateWindow(cs);
}


void CDataExplorerView::OnMouseMove(UINT nFlags,CPoint point)
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CSoLIMDoc* pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
	if(pApp->m_iCurrEnvLayerIndex!=-1 && point.x>0)
	{
		int iRow=pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->getRow();
		int iCol=pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->getCol();
		CRect rectClientRect;
		GetClientRect(rectClientRect);
		pApp->m_ptBufDataIndex.y=(point.y-m_ptImgTopLeftPt.y)/m_dZoom;
		pApp->m_ptBufDataIndex.x=(point.x-m_ptImgTopLeftPt.x)/m_dZoom;
		if (m_iToolChoice==1)
		{
			pApp->m_ptBufDataIndex.y=(point.y+12-m_ptImgTopLeftPt.y)/m_dZoom;
			pApp->m_ptBufDataIndex.x=(point.x+10-m_ptImgTopLeftPt.x)/m_dZoom;
		}
		if (m_iToolChoice==2)
		{
			pApp->m_ptBufDataIndex.y=(point.y+12-m_ptImgTopLeftPt.y)/m_dZoom;
			pApp->m_ptBufDataIndex.x=(point.x+12-m_ptImgTopLeftPt.x)/m_dZoom;
		}
		CDatabaseView *pDatabaseView = (CDatabaseView *)(pFrame->m_splitterTabWnd.GetPane(0,0));	 
		CInfoFrameView *pInfoFrameView = (CInfoFrameView *)(pDatabaseView->m_wndSplitter.GetPane(1,0));	 
		CPixelInfoView *pPixelInfoView = (CPixelInfoView *)(pInfoFrameView->m_wndSplitter.GetPane(0,0));	
		pPixelInfoView->Invalidate(1);
		if(m_iIsLButtonDown==1 && m_iToolChoice==1)
		{
			m_ptOriginOffset=point-m_ptMouseDownPt;
			Invalidate(false);
		}
		if(m_iIsLButtonDown==1 && m_iToolChoice==2)
		{
			m_ptOriginOffset=point-m_ptMouseDownPt;
			Invalidate(false);
		}
	}
	CView::OnMouseMove(nFlags, point);
}

void CDataExplorerView::OnDestroy() 
{
	CView::OnDestroy();

	if(wglGetCurrentContext()!=NULL)
	{
		wglMakeCurrent(NULL,NULL);
	}
	if(m_hGLContext!=NULL)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}

	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();

}

void CDataExplorerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_iToolChoice==1 && m_iIsLButtonDown==0)
	{
		m_ptMouseDownPt=point;
		m_iIsLButtonDown=1;
	}
	else if(m_iToolChoice==2 && m_iIsLButtonDown==0)
	{
		m_ptMouseDownPt=point;
		m_iIsLButtonDown=1;
	}

}

void CDataExplorerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	if(m_iToolChoice==1)
	{
		m_iIsLButtonDown=0;
		m_ptImgTopLeftPt.x+=m_ptOriginOffset.x;
		m_ptImgTopLeftPt.y+=m_ptOriginOffset.y;
		m_ptOriginOffset=CPoint(0,0);
		Invalidate(false);


	}
	else if(m_iToolChoice==2)
	{
		if(m_ptOriginOffset.x!=0 && m_ptOriginOffset.y!=0)
		{
			double dTempXZoom=2*m_ptMidPt.x/m_ptOriginOffset.x*m_dZoom;
			double dTempYZoom=2*m_ptMidPt.y/m_ptOriginOffset.y*m_dZoom;
			m_iIsLButtonDown=0;
			CPoint ptTempFocus=CPoint((point.x+m_ptMouseDownPt.x)/2,(point.y+m_ptMouseDownPt.y)/2);
			if(dTempXZoom<=dTempYZoom)
			{
				ImageZoom(ptTempFocus,dTempXZoom);
			}
			else
			{
				ImageZoom(ptTempFocus,dTempYZoom);
			}
		}
		else
		{
			m_iIsLButtonDown=0;
			ImageZoom(m_ptMouseDownPt,m_dZoom+0.5);
		}
	}
}

BOOL CDataExplorerView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{		
	if(m_iToolChoice==1)
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_HAND));
		return true;
	}
	else if(m_iToolChoice==2)
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_CROSS));
		return true;
	}
	return CView::OnSetCursor(pWnd, nHitTest, message);
}

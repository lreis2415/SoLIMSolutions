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
// GraphPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "SoLIM.h"
#include ".\rasterdisplayctrl.h"
#include "MainFrm.h"
#include "SoLIMDoc.h"
#include "Utility.h"


// CRasterDisplayCtrl

IMPLEMENT_DYNAMIC(CRasterDisplayCtrl, CWnd)
CRasterDisplayCtrl::CRasterDisplayCtrl()
{

	m_LayerLoaded = false;
	m_pTheData = NULL;


		m_GLPixelIndex = 0;
	m_hGLContext = NULL;
	m_ptOriginOffset.x=0;
	m_ptOriginOffset.y=0;
	m_iIsLButtonDown=0;
	//m_iToolChoice=0;



}

CRasterDisplayCtrl::~CRasterDisplayCtrl()
{
}


BEGIN_MESSAGE_MAP(CRasterDisplayCtrl, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()

	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()

END_MESSAGE_MAP()


BOOL CRasterDisplayCtrl::OnEraseBkgnd(CDC* pDC)
{
	return CWnd::OnEraseBkgnd(pDC);
}

void CRasterDisplayCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	
	//CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	RECT rect;
	GetClientRect(&rect);
	int cy = rect.bottom-rect.top;
	int cx = rect.right-rect.left;	


    m_ptMidPt.x=rect.right/2;
    m_ptMidPt.y=rect.bottom/2;

	HWND hwnd=GetSafeHwnd();
	HDC hdc=::GetDC(hwnd);
	if(hdc==NULL||hwnd==NULL) return;
	wglMakeCurrent(hdc,m_hGLContext);

		glClearColor(1.0f,1.0f,1.0f,1.0f);



	if(m_LayerLoaded == true)	
	{
		
	    //glClearColor(1.0f,1.0f,1.0f,1.0f);
		
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


		    m_iRows = m_pTheData->getNumberOfRows();
			m_iCols = m_pTheData->getNumberOfColumns();
		    DrawDataLayer(m_pTheData);
			
		//pFrame->m_leftBar.m_pCurRule->DrawGraph(cx,cy,&dc); //call draw function of each rule
	}


}


BOOL CRasterDisplayCtrl::SetWindowPixelFormat(HDC hDC)
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

BOOL CRasterDisplayCtrl::CreateViewGLContext(HDC hDC)
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



int CRasterDisplayCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	HWND hWnd = GetSafeHwnd();    
	HDC hDC = ::GetDC(m_hWnd);

	CClientDC clientDC(this);
	if(SetWindowPixelFormat(clientDC.m_hDC)==FALSE)
	{
		return 0;
	}
	if(CreateViewGLContext(clientDC.m_hDC)==FALSE)
	{
		return 0;
	}
	return 0;


}

void CRasterDisplayCtrl::OnDestroy()
{
	CWnd::OnDestroy();

	if(wglGetCurrentContext()!=NULL)
	{
		wglMakeCurrent(NULL,NULL);
	}
	if(m_hGLContext!=NULL)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}

}

void CRasterDisplayCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,cx,0,cy);	
	glViewport(0,0,cx,cy);
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
	Invalidate();

}



void CRasterDisplayCtrl::DrawDataLayer(CThreeDR *pTheData)
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();



		CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());

	float fMaxValue=m_pTheData->getDataMax();
	float fMinValue=m_pTheData->getDataMin();
	double dRange=fMaxValue-fMinValue;


		
	int width = pTheData->getNumberOfColumns();
	int height = pTheData->getNumberOfRows();



	int *origBuf = new int[width*height];
	for(int i=m_iRows-1;i>=0;i--)
		for(int j=0;j<m_iCols;j++)
		{
			if( fabs(pTheData->getDataBuf()[i*width+j] -  pTheData->getNullDataValue()) < VERYSMALL)
				
				origBuf[i*width+j] = -9999;//no data;
			else
				origBuf[i*width+j] = (pTheData->getDataBuf()[i*width+j] - fMinValue)/dRange * 250;
				//image[(m_iRows-i-1)*iTempCol+j]= (GLubyte)((fMaxValue-fMinValue)/dRange*255);

			
		}

	int *pEnhancedBuf = new int[width * height];
	HistogramEqualize(origBuf, pEnhancedBuf, width, height);






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
		{
			if(pEnhancedBuf[i*width+j] == -9999)
					image[(m_iRows-i-1)*iTempCol+j]= (GLubyte)(255);
			else
				image[(m_iRows-i-1)*iTempCol+j]= (GLubyte)(pEnhancedBuf[i*width+j]);
		}
	glPixelZoom(m_dZoom,m_dZoom); 
	glRasterPos3f(0,0,0);
	glBitmap(0,0,0,0,m_ptOriginPt.x,m_ptOriginPt.y,NULL);





    //glBitmap(0,0,0,0,0,0,NULL);
	glDrawPixels(m_iCols,m_iRows,GL_LUMINANCE ,GL_UNSIGNED_BYTE ,image);


	
   for(int i=0; i<m_vecPoints.size(); i++)
   {

	  float centralX = m_dZoom * (m_vecPoints[i].x- m_pTheData->getXmin())/ m_pTheData->getCellSize() + m_ptImgTopLeftPt.x;

	  float a = m_vecPoints[i].y;
	  float centralY = m_dZoom * (m_pTheData->getNumberOfRows() -(m_vecPoints[i].y- m_pTheData->getYmin())/ m_pTheData->getCellSize()) + m_ptImgTopLeftPt.y;
	

	 centralY = 2*m_ptMidPt.y - centralY;


	  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	  glBegin(GL_QUADS);
      glVertex2f(centralX-2.5, centralY-2.5); 
	  glVertex2f(centralX-2.5, centralY+2.5);
	  glVertex2f(centralX+2.5, centralY+2.5);
	  glVertex2f(centralX+2.5, centralY-2.5);


/*
	       glVertex2f(centralX-2.5, m_ptImgTopLeftPt.y-2.5); 
	  glVertex2f(centralX-2.5, m_ptImgTopLeftPt.y+2.5);
	  glVertex2f(centralX+2.5, m_ptImgTopLeftPt.y+2.5);
	  glVertex2f(centralX+2.5, m_ptImgTopLeftPt.y-2.5);*/
      glEnd();
   }


	//glFlush();
	SwapBuffers(wglGetCurrentDC());
	delete [] image;
}


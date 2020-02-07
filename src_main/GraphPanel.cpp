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
#include ".\graphpanel.h"
#include "MainFrm.h"
#include "RangeRule.h"
#include "RangePage.h"
#include "FreehandRule.h"
#include "FreehandPage.h"
#include "EnumRule.h"
#include "EnumPage.h"
#include "PointRule.h"
#include "PointPage.h"


// CGraphPanel

IMPLEMENT_DYNAMIC(CGraphPanel, CWnd)
CGraphPanel::CGraphPanel()
{
	m_LBtnDownW = false;
	m_LBtnDownV = false;
	m_iPreviousCursorX = 0;
	m_iCurrentCurveNode=-1;
	m_bEnable=true;

}

CGraphPanel::~CGraphPanel()
{
}


BEGIN_MESSAGE_MAP(CGraphPanel, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ENABLE()
END_MESSAGE_MAP()


BOOL CGraphPanel::OnEraseBkgnd(CDC* pDC)
{
	return CWnd::OnEraseBkgnd(pDC);
}

void CGraphPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	RECT rect;
	GetClientRect(&rect);
	int cy = rect.bottom-rect.top;
	int cx = rect.right-rect.left;	

	HWND hwnd=GetSafeHwnd();
	HDC hdc=::GetDC(hwnd);
	if(hdc==NULL||hwnd==NULL) return;

	CBrush brushEnable, brushDisable;
	
	brushEnable.CreateSolidBrush(RGB(0, 0, 0));
    brushDisable.CreateSolidBrush(RGB(100, 100, 100));



	//wglMakeCurrent(hdc,m_hGLContext);

	if(pFrame->m_leftBar.m_pCurRule!=NULL)
	{
		if(m_bEnable)
		{
			
	        dc.FillRect(&rect, &brushEnable);
	
		}
		else
		{
			    
	        dc.FillRect(&rect, &brushDisable);
		}
		//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		pFrame->m_leftBar.m_pCurRule->DrawGraph(cx,cy,&dc); //call draw function of each rule





	}
	else 
	{
		if(m_bEnable)
		{
			
	        dc.FillRect(&rect, &brushEnable);
	
		}
		else
		{
			    
	        dc.FillRect(&rect, &brushDisable);
		}
		//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		//glFinish();




		CPen NewPen(PS_SOLID,4,RGB(255,255,0));
		CPen *pOldPen=dc.SelectObject(&NewPen);
		CFont font;//create font	
		font.CreateFont(12,8,0,0,100,0,0,0,DEFAULT_CHARSET,0,0,2,DEFAULT_PITCH,"1");
		CFont *pOldFont=dc.SelectObject(&font);		
		CString str;
		str = "No graph to be displayed";
		dc.TextOut(10,10,str, str.GetLength());
		dc.SelectObject(pOldFont);
		dc.SelectObject(pOldPen);
	}

}


BOOL CGraphPanel::SetWindowPixelFormat(HDC hDC)
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

BOOL CGraphPanel::CreateViewGLContext(HDC hDC)
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



int CGraphPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

void CGraphPanel::OnDestroy()
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

void CGraphPanel::OnSize(UINT nType, int cx, int cy)
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


void CGraphPanel::OnLButtonDown(UINT nFlags, CPoint point)
{

	CRect rect;
	GetClientRect(&rect);
	int cy = rect.bottom-rect.top;
	int cx = rect.right-rect.left;	

	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	if(pFrame->m_leftBar.m_pCurRule==NULL)
	{
		return;
	}
	else
	{
		if(pFrame->m_leftBar.m_pCurRule->getRuleType() == 0)
		{
			CRangeRule *pRule = (CRangeRule *)(pFrame->m_leftBar.m_pCurRule);
			if(abs(pRule->ComputeScreenCoordY(0.5,cy)-point.y)<10)
			{ //if the cursor is close enough to the cross-point handle
				int x = -1;
				if(pRule->getCurveType() == 1)
				{					
					x = pRule->ComputeScreenCoordX(pRule->getLowCross(),cx);
					m_bLeftOrRight = true;
				}
				else if(pRule->getCurveType() == 2)
				{
					x = pRule->ComputeScreenCoordX(pRule->getHighCross(),cx); 
					m_bLeftOrRight = false;
				}
				else if(point.x<pRule->ComputeScreenCoordX(pRule->getLowUnity(),cx))
				{
					x = pRule->ComputeScreenCoordX(pRule->getLowCross(),cx);
					m_bLeftOrRight = true;
				}

				else if(point.x>pRule->ComputeScreenCoordX(pRule->getHighUnity(),cx))
				{
					x = pRule->ComputeScreenCoordX(pRule->getHighCross(),cx);
					m_bLeftOrRight = false;
				}
				if(abs(x-point.x)<6)
				{
					m_LBtnDownW = true;

					m_iPreviousCursorX = x;
				}
			}
			else if(abs(20-point.y)<10)
			{    //if the cursor is close enough to the top-point handle
				if(pRule->getLowUnity()!=pRule->getHighUnity())
				{
					if(abs(point.x-pRule->ComputeScreenCoordX(pRule->getLowUnity(),cx))<=abs(point.x-pRule->ComputeScreenCoordX(pRule->getHighUnity(),cx))&&abs(point.x-pRule->ComputeScreenCoordX(pRule->getLowUnity(),cx))<6)
					{
						m_LBtnDownV = true;
						m_bLeftOrRight = true;
						m_iPreviousCursorX = pRule->ComputeScreenCoordX(pRule->getLowUnity(),cx);
						m_bIsTopXEqual = false;
					}
					else if(abs(point.x-pRule->ComputeScreenCoordX(pRule->getLowUnity(),cx))>abs(point.x-pRule->ComputeScreenCoordX(pRule->getHighUnity(),cx))&&abs(point.x-pRule->ComputeScreenCoordX(pRule->getHighUnity(),cx))<6)
					{
						m_LBtnDownV = true;
						m_bLeftOrRight = false;
						m_iPreviousCursorX = pRule->ComputeScreenCoordX(pRule->getHighUnity(),cx);
						m_bIsTopXEqual = false;
					}
				}
				else
				{
					m_LBtnDownV = true;

					m_iPreviousCursorX = pRule->ComputeScreenCoordX(pRule->getLowUnity(),cx);
					m_bIsTopXEqual = true;


				}
			}
		}
		else if(pFrame->m_leftBar.m_pCurRule->getRuleType() == 1)
		{
			CurrentCurveNode(point.x,point.y,cx,cy);
		}
		else if (pFrame->m_leftBar.m_pCurRule->getRuleType() == 4)
		{
			CurrentEnumItem(point.x,point.y,cx,cy);
		}
		else if (pFrame->m_leftBar.m_pCurRule->getRuleType() == 3)
		{	
			CPointRule *pRule = (CPointRule *)(pFrame->m_leftBar.m_pCurRule);
			if(abs(pRule->ComputeScreenCoordY(0.5,cy)-point.y)<10)
			{ //if the cursor is close enough to the cross-point handle
				int x = -1;
				if(pRule->getCurveType() == 1)
				{					
					x = pRule->ComputeScreenCoordX(pRule->getLowCross(),cx);
					m_bLeftOrRight = true;
				}
				else if(pRule->getCurveType() == 2)
				{
					x = pRule->ComputeScreenCoordX(pRule->getHighCross(),cx); 
					m_bLeftOrRight = false;
				}
				else if(point.x<pRule->ComputeScreenCoordX(pRule->getCentralValue(),cx))
				{
					x = pRule->ComputeScreenCoordX(pRule->getLowCross(),cx);
					m_bLeftOrRight = true;
				}

				else if(point.x>pRule->ComputeScreenCoordX(pRule->getCentralValue(),cx))
				{
					x = pRule->ComputeScreenCoordX(pRule->getHighCross(),cx);
					m_bLeftOrRight = false;
				}
				if(abs(x-point.x)<6)
				{
					m_LBtnDownW = true;

					m_iPreviousCursorX = x;
				}
			}

		}

	}
	CWnd::OnLButtonDown(nFlags, point);

	return;


}

void CGraphPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_LBtnDownW = false;
	m_LBtnDownV = false;
	m_iCurrentCurveNode=-1;
}

void CGraphPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CRect rect;
	GetClientRect(&rect);
	int cy = rect.bottom-rect.top;
	int cx = rect.right-rect.left;	


	if(pFrame->m_leftBar.m_pCurRule==NULL)
	{
		return;
	}
	else
	{
		if(pFrame->m_leftBar.m_pCurRule->getRuleType() == 0)
		{
			CRangeRule *pRule = (CRangeRule *)(pFrame->m_leftBar.m_pCurRule);
			if(point.x<20 || point.x>cx-20 || point.y<20 || point.y>cy-20)
			{
				m_LBtnDownW = false;
				m_LBtnDownV = false;
			}
			if(m_LBtnDownW && abs(m_iPreviousCursorX-point.x)>0)
			{
				if(m_bLeftOrRight==true && point.x>20 && point.x<pRule->ComputeScreenCoordX(pRule->getLowUnity(),cx))
				{
					pRule->setLowCross(pRule->ComputeGraphCoordX(point.x, cx));
					CString str;  		
					str.Format("%f",pRule->getLowCross());
					str = str.Left(9);
					((CRangePage *)(GetParent()))->m_editLowCross.SetWindowText(str);
				}
				else if(m_bLeftOrRight==false && point.x>pRule->ComputeScreenCoordX(pRule->getHighUnity(),cx) && point.x<cx-20)
				{
					pRule->setHighCross(pRule->ComputeGraphCoordX(point.x, cx));
					CString str;  
					str.Format("%f",pRule->getHighCross());
					str = str.Left(9);
					((CRangePage *)(GetParent()))->m_editHighCross.SetWindowText(str);
				}
				m_iPreviousCursorX = point.x; //for determining if the width value should increase or decrease
				Invalidate();
			}

			else if(m_LBtnDownV && abs(m_iPreviousCursorX-point.x)>0)
			{

				if(m_bIsTopXEqual==false)
				{
					if(pRule->getCurveType()==0&&m_bLeftOrRight==true&&point.x>pRule->ComputeScreenCoordX(pRule->getLowCross(),cx)&&point.x<=pRule->ComputeScreenCoordX(pRule->getHighUnity(),cx))
					{
						pRule->setLowUnity(pRule->ComputeGraphCoordX(point.x, cx));
						CString str;  
						str.Format("%f",pRule->getLowUnity());
						str = str.Left(9);
						((CRangePage *)(GetParent()))->m_editLowUnity.SetWindowText(str);

					}
					else if(pRule->getCurveType()==0&&m_bLeftOrRight==false&&point.x<pRule->ComputeScreenCoordX(pRule->getHighCross(),cx)&&point.x>=pRule->ComputeScreenCoordX(pRule->getLowUnity(),cx))
					{
						pRule->setHighUnity(pRule->ComputeGraphCoordX(point.x, cx));
						CString str;
						str.Format("%f",pRule->getHighUnity());
						str = str.Left(9);
						((CRangePage *)(GetParent()))->m_editHighUnity.SetWindowText(str);

					}
					else if(pRule->getCurveType()==1&&m_bLeftOrRight==true&&point.x>pRule->ComputeScreenCoordX(pRule->getLowCross(),cx))
					{
						pRule->setLowUnity(pRule->ComputeGraphCoordX(point.x, cx));
						CString str;
						str.Format("%f",pRule->getLowUnity());
						str = str.Left(9);
						((CRangePage *)(GetParent()))->m_editLowUnity.SetWindowText(str);

					}
					else if(pRule->getCurveType()==2&&m_bLeftOrRight==false&&point.x<pRule->ComputeScreenCoordX(pRule->getHighCross(),cx))
					{
						pRule->setHighUnity(pRule->ComputeGraphCoordX(point.x, cx));
						CString str;  
						str.Format("%f",pRule->getHighUnity());
						str = str.Left(9);
						((CRangePage *)(GetParent()))->m_editHighUnity.SetWindowText(str);

					}

					m_iPreviousCursorX = point.x;
					Invalidate();

				}
				else 
				{
					if(m_iPreviousCursorX-point.x>0)
					{
						m_bIsTopXEqual=false;
						m_bLeftOrRight=true;
					}
					else if(m_iPreviousCursorX-point.x<0)
					{
						m_bIsTopXEqual=false;
						m_bLeftOrRight=false;
					}
				}

			}

		}
		else if(pFrame->m_leftBar.m_pCurRule->getRuleType() == 1)
		{	
			CFreehandRule *pRule = (CFreehandRule *)(pFrame->m_leftBar.m_pCurRule);
			double fNewGraphX=pRule->ComputeGraphCoordX(point.x,cx);
			double fNewGraphY=pRule->ComputeGraphCoordY(point.y,cy);
			if(m_iCurrentCurveNode!=-1)
			{

				CString strTemp;
				pRule->setKnot(m_iCurrentCurveNode,fNewGraphX,fNewGraphY);
				Invalidate();

			}
			((CFreehandPage *)(GetParent()))->DisplayCurrentCursorPosition(fNewGraphX,fNewGraphY);
		}
		else if (pFrame->m_leftBar.m_pCurRule->getRuleType() == 4)
		{
			CEnumRule *pRule = (CEnumRule *)(pFrame->m_leftBar.m_pCurRule);
			int fNewGraphX=pRule->ComputeGraphCoordX(point.x,cx);
			if(m_iCurrentCurveNode!=-1)
			{

				CString strTemp;
				pRule->setItem(m_iCurrentCurveNode,fNewGraphX);
				strTemp.Format("%d",fNewGraphX);
				((CEnumPage *)(GetParent()))->m_listctrlEnumItem.SetItemText(
					m_iCurrentCurveNode,0,strTemp);
				Invalidate();
			}
			((CEnumPage *)(GetParent()))->DisplayCurrentCursorPosition(fNewGraphX,1);
		}
		else if (pFrame->m_leftBar.m_pCurRule->getRuleType() == 3)
		{
			CPointRule *pRule = (CPointRule *)(pFrame->m_leftBar.m_pCurRule);
			if(point.x<20 || point.x>cx-20 || point.y<20 || point.y>cy-20)
			{
				m_LBtnDownW = false;
				m_LBtnDownV = false;
			}
			if(m_LBtnDownW && abs(m_iPreviousCursorX-point.x)>0)
			{
				if(m_bLeftOrRight==true && point.x>20 && point.x<pRule->ComputeScreenCoordX(pRule->getCentralValue(),cx))
				{
					pRule->setLWidth(pRule->getCentralValue()-pRule->ComputeGraphCoordX(point.x, cx));
					CString str;  		

					str.Format("%f",pRule->getLWidth());
					str = str.Left(9);
					((CPointPage *)(GetParent()))->m_editLWidth.SetWindowText(str);
				}
				else if(m_bLeftOrRight==false && point.x>pRule->ComputeScreenCoordX(pRule->getCentralValue(),cx) && point.x<cx-20)
				{
					pRule->setRWidth(pRule->ComputeGraphCoordX(point.x, cx)-pRule->getCentralValue());
					CString str;  
					str.Format("%f",pRule->getRWidth());
					str = str.Left(9);
					((CPointPage *)(GetParent()))->m_editRWidth.SetWindowText(str);
				}
				m_iPreviousCursorX = point.x; //for determining if the width value should increase or decrease
				Invalidate();
			}

		}
	}

	CWnd::OnMouseMove(nFlags, point);
}



void CGraphPanel::CurrentCurveNode(int pointx, int pointy, int cx,int cy)
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CFreehandRule *pRule = (CFreehandRule *)(pFrame->m_leftBar.m_pCurRule);
	int ScreenCoordX,ScreenCoordY;
	for(int i=0;i<pRule->getKnotNum();i++)
	{
		ScreenCoordX=pRule->ComputeScreenCoordX(pRule->getKnotX(i),cx);
		ScreenCoordY=pRule->ComputeScreenCoordY(pRule->getKnotY(i),cy);
		if(abs(pointx-ScreenCoordX)<=3 && abs(pointy-ScreenCoordY)<=3)
		{
			m_iCurrentCurveNode=i;
			m_LBtnDownW=1;
			break;
		}
	}
}

void CGraphPanel::CurrentEnumItem(int pointx, int pointy,int cx, int cy)
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CEnumRule *pRule = (CEnumRule *)(pFrame->m_leftBar.m_pCurRule);
	int ScreenCoordX;
	for(int i=0;i<pRule->getItemNum();i++)
	{
		ScreenCoordX=pRule->ComputeScreenCoordX(pRule->getItem(i),cx);
		if(abs(pointx-ScreenCoordX)<=2.5 && pointy>=cy*0.2 && pointy<cy-20)
		{
			m_iCurrentCurveNode=i;
			m_LBtnDownW=1;
			break;
		}
	}
}


void CGraphPanel::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CRect rect;
	GetClientRect(&rect);
	int cy = rect.bottom-rect.top;
	int cx = rect.right-rect.left;	

	if(pFrame->m_leftBar.m_pCurRule==NULL)
	{
		return;
	}
	else
	{
		if(pFrame->m_leftBar.m_pCurRule->getRuleType() == 1)
		{
			CurrentCurveNode(point.x,point.y,cx,cy);
			if(m_iCurrentCurveNode==-1)
			{
				CFreehandRule *pRule = (CFreehandRule *)(pFrame->m_leftBar.m_pCurRule);
				double tempx=pRule->ComputeGraphCoordX(point.x,cx);
				double tempy=pRule->ComputeGraphCoordY(point.y,cy);
				pRule->AddKnot(tempx,tempy);
				((CFreehandPage *)(GetParent()))->FillPage();
				m_iCurrentCurveNode=-1;
				Invalidate();
			}
			else
			{
				CFreehandRule *pRule = (CFreehandRule *)(pFrame->m_leftBar.m_pCurRule);
				pRule->DeleteKnot(m_iCurrentCurveNode);
				((CFreehandPage *)(GetParent()))->FillPage();
				m_iCurrentCurveNode=-1;
				Invalidate();
			}
		}
		else if (pFrame->m_leftBar.m_pCurRule->getRuleType() == 4)
		{
			CurrentEnumItem(point.x,point.y,cx,cy);
			if(m_iCurrentCurveNode==-1)
			{
				CEnumRule *pRule = (CEnumRule *)(pFrame->m_leftBar.m_pCurRule);
				int tempx=pRule->ComputeGraphCoordX(point.x,cx);
				pRule->AddItem(tempx);
				((CEnumPage *)(GetParent()))->FillPage();
				m_iCurrentCurveNode=-1;
				Invalidate();
			}
			else
			{
				CEnumRule *pRule = (CEnumRule *)(pFrame->m_leftBar.m_pCurRule);
				pRule->DeleteItem(m_iCurrentCurveNode);
				((CEnumPage *)(GetParent()))->FillPage();
				m_iCurrentCurveNode=-1;
				Invalidate();
			}
		}
	}
}

void CGraphPanel::OnEnable(BOOL bEnable)
{

	CWnd::OnEnable(bEnable);

	m_bEnable=bEnable;
	

}

void CGraphPanel::SetEnable(BOOL bEnable)
{
	m_bEnable= bEnable;

}

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
// PixelInfoView.cpp 
//

#include "stdafx.h"
#include "SoLIM.h"
#include "PixelInfoView.h"
#include "MainFrm.h"

// CPixelInfoView

IMPLEMENT_DYNCREATE(CPixelInfoView, CView)

CPixelInfoView::CPixelInfoView()
{
}

CPixelInfoView::CPixelInfoView(CSoLIMDoc *pDoc )
{
	m_pDoc = pDoc;
}
CPixelInfoView::~CPixelInfoView()
{
}

BEGIN_MESSAGE_MAP(CPixelInfoView, CView)
END_MESSAGE_MAP()


void CPixelInfoView::OnDraw(CDC* pDC)
{
	CSoLIMDoc* pDoc = (CSoLIMDoc*)GetDocument();
	CPaintDC dc(this); // device context for painting
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	if(pDoc!=NULL)
	{
		if(pDoc->m_pProjectEdit==NULL)
			return;

		if(pApp->m_iCurrEnvLayerIndex!=-1&&pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum()>0)
		{
			int iRow=pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->getRow()-1;
			int iCol=pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->getCol()-1;
			CString strX,strY,strValue;
			CRect CurrRect;
			GetClientRect(CurrRect);
			int iDspRow=CurrRect.bottom/30-1,iDspCol=CurrRect.right/80-1;
			int iStartDspIndexRow=pApp->m_ptBufDataIndex.y-iDspRow/2, iStartDspIndexCol=pApp->m_ptBufDataIndex.x-iDspCol/2;
			if(pApp->m_ptBufDataIndex.x>=0 && pApp->m_ptBufDataIndex.x<=iCol && pApp->m_ptBufDataIndex.y>=0 && pApp->m_ptBufDataIndex.y<=iRow )
			{
				float fDataValue=pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->getData(pApp->m_ptBufDataIndex.y,pApp->m_ptBufDataIndex.x);
				int iIndexValue=0;
				for (int i=0;i<iDspRow;i++)
				{
					if(iStartDspIndexRow+i>=0 && iStartDspIndexRow+i<=iRow)
					{
						pDC->SetTextColor(0);
						strValue.Format("%d",iStartDspIndexRow+i);
						pDC->TextOut(0,(i+1)*40,strValue);
					}
				}
				for (int i=0;i<iDspCol;i++)
				{
					if(iStartDspIndexCol+i>=0 && iStartDspIndexCol+i<=iCol)
					{
						pDC->SetTextColor(0);
						strValue.Format("%d",iStartDspIndexCol+i);
						pDC->TextOut((i+1)*80,0,strValue);
					}
				}


				for(int i=0;i<iDspRow;i++)
				{
					for(int j=0;j<iDspCol;j++)
					{
						if(iStartDspIndexRow+i>=0 && iStartDspIndexRow+i<=iRow && iStartDspIndexCol+j>=0 && iStartDspIndexCol+j<=iCol)
						{
							pDC->SetTextColor(0);
							if(iStartDspIndexCol+j==pApp->m_ptBufDataIndex.x && iStartDspIndexRow+i==pApp->m_ptBufDataIndex.y)
							{
								pDC->SetTextColor(255);
							}

						

							//int decimalNum = FormatFloat(pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->getData(iStartDspIndexRow+i,iStartDspIndexCol+j),6);


							strValue.Format("%f",pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->getData(iStartDspIndexRow+i,iStartDspIndexCol+j));
							strValue = strValue.Left(9);
							pDC->TextOut((j+1)*80,(i+1)*40,strValue);
						}
					}
				}
			}
		}
	}
}



#ifdef _DEBUG
void CPixelInfoView::AssertValid() const
{
	CView::AssertValid();
}

void CPixelInfoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG



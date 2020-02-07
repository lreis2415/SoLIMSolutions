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
#include "StdAfx.h"
#include ".\enumrule.h"

CEnumRule::CEnumRule(void)
{
	m_iItemNum = 0;
}

CEnumRule::CEnumRule(CEnumRule &anEnumRule)
{
	m_iItemNum = 0;
	m_strRuleName = anEnumRule.getRuleName();
	m_bIsUsed = anEnumRule.getIsUsed();
	m_dDispMin = anEnumRule.getDispMin();
	m_dDispMax = anEnumRule.getDispMax();
	m_iRuleType = anEnumRule.getRuleType();
	m_strLayerName = anEnumRule.getLayerName();
	for(int i =0;i<anEnumRule.getItemNum();i++)
	{
		AddItem(anEnumRule.getItem(i));
	}
	m_dCurGraphMin = anEnumRule.getCurGraphMin();
	m_dCurGraphMax = anEnumRule.getCurGraphMax();

}

CEnumRule::~CEnumRule(void)
{

}

void CEnumRule::setItem(int iIndex, int iItem)
{
	if(iIndex+1>m_iItemNum) m_iItemNum++;
	m_vecItem[iIndex] = iItem;
	//BubbleSort();
}

int CEnumRule::getItemNum()
{
	return m_iItemNum;
}

int CEnumRule::getItem(int iIndex)
{
	return m_vecItem[iIndex];
}

void  CEnumRule::AddItem(int iItem)
{
	m_vecItem.push_back(iItem);
	m_iItemNum++;
	BubbleSort();
}

void CEnumRule::DeleteItem(int iIndex)
{
	m_vecItem.erase(m_vecItem.begin()+iIndex);
	m_iItemNum--;
	BubbleSort();
}

double CEnumRule::Evaluate(float fEnvValue)
{
	if(fabs(fEnvValue-NOFLOATVALUE)<VERYSMALL)
	{
		return 1;
	}
	if(m_iItemNum<=0)
		return 1;
	for(int i=0;i<m_iItemNum;i++)
	{
		int item = m_vecItem[i];
		if(fabs(fEnvValue-m_vecItem[i])<VERYSMALL)
		{ 
			return 1;
			break;		
		}
	}
	return 0;
}


void CEnumRule::DrawGraph(int cx, int cy, CPaintDC *dc)
{
	//BubbleSort();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,cx,0,cy);	
	glViewport(0,0,cx,cy);
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0f,1.0f,0.0f);
	glEnable(GL_LINE_SMOOTH);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glLineWidth(1.0f);

	//draw axis
	glBegin(GL_LINES);
	glVertex2f(20.0f, 20.0f);
	glVertex2f(cx-10,20.0f);
	glVertex2f(20.0f, 20.0f);
	glVertex2f(20.0f,cy-10);
	glVertex2f(cx-10,20.0f);
	glVertex2f(cx-15,25);
	glVertex2f(cx-10,20.0f);
	glVertex2f(cx-15,15);
	glVertex2f(20.0f,cy-10);
	glVertex2f(15,cy-15);
	glVertex2f(20.0f,cy-10);
	glVertex2f(25,cy-15);
	glVertex2f(20,cy-20);
	glVertex2f(23,cy-20);	
	glEnd();

	//draw bars
	for(int i=0;i<m_vecItem.size();i++)
	{
		glBegin(GL_QUADS);
		glVertex2f((getItem(i)-m_dDispMin)*(cx-40)/(m_dDispMax-m_dDispMin)+20-2.5, 20);		
		glVertex2f((getItem(i)-m_dDispMin)*(cx-40)/(m_dDispMax-m_dDispMin)+20-2.5, cy-20);		
		glVertex2f((getItem(i)-m_dDispMin)*(cx-40)/(m_dDispMax-m_dDispMin)+20+2.5, cy-20);	
		glVertex2f((getItem(i)-m_dDispMin)*(cx-40)/(m_dDispMax-m_dDispMin)+20+2.5, 20);	
		glEnd();	
	}
	glFlush();
	SwapBuffers(wglGetCurrentDC());
	HDC  hdc=wglGetCurrentDC();
	//::SetBkMode( hdc, TRANSPARENT );

	//draw coord axis
	
		//::SetBkMode(hdc, TRANSPARENT );
	::SetTextColor( hdc, RGB(255,255,255) );
	::SetBkColor(hdc, RGB(0, 0, 0));

	HFONT hFont;
	hFont=::CreateFont(10,5,0,0,100,0,0,0,DEFAULT_CHARSET,0,0,2,DEFAULT_PITCH,"1");
	::SelectObject(hdc,hFont);
	CPen NewPen(PS_SOLID,4,RGB(255,255,0));
	CPen *pOldPen=dc->SelectObject(&NewPen);
	CFont font;	
	font.CreateFont(7,5,0,0,100,0,0,0,DEFAULT_CHARSET,0,0,2,DEFAULT_PITCH,"1");	
	CFont *pOldFont=dc->SelectObject(&font);
	CString str;	
	str.Format("%.1f",1.0);	
	::TextOut(hdc,4,20-3,str,str.GetLength());
	for(int i=0;i<getItemNum();i++)
	{
		str.Format("%d",getItem(i));
		::TextOut(hdc,(getItem(i)-m_dDispMin)*(cx-40)/(m_dDispMax-m_dDispMin)+20-2.5,cy-19,str,str.GetLength());
	}
	

		str="Optimality";
	::TextOut(hdc,3,0,str,str.GetLength());
	str=this->getLayerName();
	if(str == "")
		str = "Env. Condition";
	::TextOut(hdc,cx/2-5,cy-10,str,str.GetLength());

	::SelectObject(hdc,pOldFont);
	::SelectObject(hdc,pOldPen);
}

void CEnumRule::BubbleSort()
{
	double tempx;
	for(int i=0;i<getItemNum();i++)
	{
		for(int j=0;j<getItemNum()-1;j++)
		{
			if(m_vecItem[j]>m_vecItem[j+1])
			{
				tempx=m_vecItem[j+1];
				m_vecItem[j+1]=m_vecItem[j];
				m_vecItem[j]=tempx;
			}
		}
	}
}

double CEnumRule::ComputeRuleGraphMin()
{
	if(m_iItemNum<=0)
		return -1000;
	else if(m_iItemNum == 1)
		return getItem(0)-2;
	else 
		return getItem(0)-0.1*(getItem(getItemNum()-1) - getItem(0));
}

double CEnumRule::ComputeRuleGraphMax()
{
	if(m_iItemNum<=0)
		return 1000;
	else if(m_iItemNum == 1)
		return getItem(0)+2;
	else		
		return getItem(getItemNum()-1)+0.1*(getItem(getItemNum()-1) - getItem(0));
}

int CEnumRule::ComputeScreenCoordX(double x, int cx)
{
	return (cx-40)*(x-m_dDispMin)/(m_dDispMax-m_dDispMin)+20;
}
int CEnumRule::ComputeScreenCoordY(double y, int cy)
{
	return (1-y)*cy+40*y-20;
}
double CEnumRule::ComputeGraphCoordX(int x, int cx)
{
	return (x-20)*(m_dDispMax-m_dDispMin)/(cx-40)+m_dDispMin;
}
double CEnumRule::ComputeGraphCoordY(int y, int cy)
{
	double t=(double)(cy-y-20)/(cy-40);
	return t;
}

bool CEnumRule::IsLegal(void)
{
	return true;

}




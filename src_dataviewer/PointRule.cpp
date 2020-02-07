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
#include ".\pointrule.h"

CPointRule::CPointRule(void)
{
	m_dCentralX = 0;
	m_dCentralY = 0;
	m_dLWidth=1000;	
	m_dRWidth=1000;
	m_iCurveType=0;
	m_fCentralValue = NOFLOATVALUE;
}

CPointRule::CPointRule(CPointRule &aPointRule)
{
	m_strRuleName = aPointRule.getRuleName();
	m_bIsUsed = aPointRule.getIsUsed();
	m_dDispMin = aPointRule.getDispMin();
	m_dDispMax = aPointRule.getDispMax();
	m_iRuleType = aPointRule.getRuleType();
	m_strLayerName = aPointRule.getLayerName();
	m_dCentralX = aPointRule.getCentralX();
	m_dCentralY = aPointRule.getCentralY();
	m_dLWidth = aPointRule.getLWidth();
	m_dRWidth = aPointRule.getRWidth();
	m_iCurveType = aPointRule.getCurveType();
	m_fCentralValue = aPointRule.getCentralValue();
	m_dCurGraphMin = aPointRule.getCurGraphMin();
	m_dCurGraphMax = aPointRule.getCurGraphMax();
}

CPointRule::~CPointRule(void)
{
}


void CPointRule::setLWidth(double dWidth)
{
	m_dLWidth = dWidth;

}
double CPointRule::getLWidth()
{
	return m_dLWidth;
}

void CPointRule::setRWidth(double dWidth)
{
	m_dRWidth = dWidth;

}
double CPointRule::getRWidth()
{
	return m_dRWidth;
}
void CPointRule::setCentralX(double dCentralX)
{
	m_dCentralX = dCentralX;
}
double CPointRule::getCentralX()
{
	return m_dCentralX;
}
void CPointRule::setCentralY(double dCentralY)
{
	m_dCentralY = dCentralY;
}
double CPointRule::getCentralY()
{
	return m_dCentralY;
}

double CPointRule::Evaluate(float fEnvValue)
{
	if(fabs(fEnvValue-NOFLOATVALUE)<VERYSMALL)
	{
		return 1;
	}

	if(fabs(m_fCentralValue-NOFLOATVALUE)<VERYSMALL)
	{
		return 1;
	}

	double dLowUnity, dHighUnity,dLowCross,dHighCross;
	dLowUnity = dHighUnity = m_fCentralValue;
	dLowCross = dLowUnity-m_dLWidth;
	dHighCross = dHighUnity+m_dRWidth;	
	double result;
	if(fabs(dLowCross-NODOUBLEVALUE)<VERYSMALL||fabs(dLowUnity-NODOUBLEVALUE)<VERYSMALL)
	{
		if(fEnvValue<dHighUnity)
		{
			result=1;
		}
		else 
		{
			result = exp(pow(fabs(fEnvValue-dHighUnity)/(dHighCross-dHighUnity), 2)*log(0.5));
		}
	}
	else if(fabs(dHighCross-NODOUBLEVALUE)<VERYSMALL||fabs(dHighUnity-NODOUBLEVALUE)<VERYSMALL)
	{
		if(fEnvValue>dLowUnity)
		{
			result=1;
		}
		else
		{
			result = exp(pow(fabs(fEnvValue-dLowUnity)/(dLowUnity-dLowCross), 2)*log(0.5));
		}

	} 
	else
	{
		if(fEnvValue<dHighUnity&&fEnvValue>dLowUnity)
		{
			result=1;
		}
		else if(fEnvValue<=dLowUnity)
		{
			result = exp(pow(fabs(fEnvValue-dLowUnity)/(dLowUnity-dLowCross), 2)*log(0.5));
		}
		else 
		{
			result = exp(pow(fabs(fEnvValue-dHighUnity)/(dHighCross-dHighUnity), 2)*log(0.5));
		}
	}
	return result;
}


void CPointRule::setCurveType(int curveType)
{
	m_iCurveType = curveType;	
	if(curveType == 0)
	{
		if(m_dLWidth == NODOUBLEVALUE)
		{	
			m_dLWidth = m_dRWidth;
		}
		if(m_dRWidth== NODOUBLEVALUE)
		{
			m_dRWidth = m_dLWidth;		
		}	
	}
	if(curveType == 1)
	{		
		if(m_dLWidth == NODOUBLEVALUE)
		{
			m_dLWidth = m_dRWidth;
		}
		m_dRWidth = NODOUBLEVALUE;

	}
	else if(curveType == 2)
	{
		if(m_dRWidth== NODOUBLEVALUE)
		{
			m_dRWidth = m_dLWidth;		
		}
		m_dLWidth = NODOUBLEVALUE;

	}

}

int CPointRule::getCurveType()
{
	return m_iCurveType;
}

void CPointRule::DrawGraph(int cx, int cy, CPaintDC *dc)
{
	double dLowUnity, dHighUnity,dLowCross,dHighCross;
	dLowUnity = dHighUnity = m_fCentralValue;
	dLowCross = dLowUnity-m_dLWidth;
	dHighCross = dHighUnity+m_dRWidth;
	RECT rect;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,cx,0,cy);	
	glViewport(0,0,cx,cy);
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
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
	for(int i=1;i<=10;i++)
	{ 	
		glVertex2f(20,20+i*0.1*(cy-40));
		glVertex2f(23,20+i*0.1*(cy-40));		
	}
	double step1=(m_dDispMax-m_dDispMin)/10;
	for(int i=0;i<=10;i++)
	{
		glVertex2f((cx-40)*(step1*i)/(m_dDispMax-m_dDispMin)+20,20);
		glVertex2f((cx-40)*(step1*i)/(m_dDispMax-m_dDispMin)+20,23);

	}
	glEnd();
	
	//draw curve
	if(fabs(m_fCentralValue-NOFLOATVALUE)>VERYSMALL)
	{		
		glBegin(GL_LINES);	
		double step2=(m_dDispMax-m_dDispMin)/100;	
		double result[101];	
		for(int i=0;i<101;i++)
		{
			result[i]=Evaluate(m_dDispMin+step2*i);

		}
		for(int i=0;i<100;i++)
		{
			glVertex2f((cx-40)*(step2*i)/(m_dDispMax-m_dDispMin)+20, (cy-40)*result[i]+20);		
			glVertex2f((cx-40)*(step2*i+step2)/(m_dDispMax-m_dDispMin)+20, (cy-40)*result[i+1]+20);		
		}
		glEnd();
		if(fabs(dHighCross-NODOUBLEVALUE)<VERYSMALL)
		{
			glBegin(GL_QUADS);
			glVertex2f((cx-40)*(dLowCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, (cy-40)*0.5+20-3);		
			glVertex2f((cx-40)*(dLowCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, (cy-40)*0.5+20+3);	
			glVertex2f((cx-40)*(dLowCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, (cy-40)*0.5+20+3);	
			glVertex2f((cx-40)*(dLowCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, (cy-40)*0.5+20-3);	
			glVertex2f((cx-40)*(dLowUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, (cy-40)*1+20-3);		
			glVertex2f((cx-40)*(dLowUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, (cy-40)*1+20+3);	
			glVertex2f((cx-40)*(dLowUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, (cy-40)*1+20+3);	
			glVertex2f((cx-40)*(dLowUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, (cy-40)*1+20-3);	
			glEnd();		
		}

		else if(fabs(dLowCross-NODOUBLEVALUE)<VERYSMALL)
		{
			glBegin(GL_QUADS);		 
			glVertex2f((cx-40)*(dHighCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, (cy-40)*0.5+20-3);		
			glVertex2f((cx-40)*(dHighCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, (cy-40)*0.5+20+3);	
			glVertex2f((cx-40)*(dHighCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, (cy-40)*0.5+20+3);	
			glVertex2f((cx-40)*(dHighCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, (cy-40)*0.5+20-3);	
			glVertex2f((cx-40)*(dHighUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, (cy-40)*1+20-3);		
			glVertex2f((cx-40)*(dHighUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, (cy-40)*1+20+3);	
			glVertex2f((cx-40)*(dHighUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, (cy-40)*1+20+3);	
			glVertex2f((cx-40)*(dHighUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, (cy-40)*1+20-3);	
			glEnd();		
		}
		else 
		{
			glBegin(GL_QUADS);
			glVertex2f((cx-40)*(dLowCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, (cy-40)*0.5+20-3);		
			glVertex2f((cx-40)*(dLowCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, (cy-40)*0.5+20+3);	
			glVertex2f((cx-40)*(dLowCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, (cy-40)*0.5+20+3);	
			glVertex2f((cx-40)*(dLowCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, (cy-40)*0.5+20-3);	
			glVertex2f((cx-40)*(dHighCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, (cy-40)*0.5+20-3);		
			glVertex2f((cx-40)*(dHighCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, (cy-40)*0.5+20+3);	
			glVertex2f((cx-40)*(dHighCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, (cy-40)*0.5+20+3);	
			glVertex2f((cx-40)*(dHighCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, (cy-40)*0.5+20-3);	
			glVertex2f((cx-40)*(dLowUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, (cy-40)*1+20-3);		
			glVertex2f((cx-40)*(dLowUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, (cy-40)*1+20+3);	
			glVertex2f((cx-40)*(dLowUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, (cy-40)*1+20+3);	
			glVertex2f((cx-40)*(dLowUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, (cy-40)*1+20-3);	
			glVertex2f((cx-40)*(dHighUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, (cy-40)*1+20-3);		
			glVertex2f((cx-40)*(dHighUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, (cy-40)*1+20+3);	
			glVertex2f((cx-40)*(dHighUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, (cy-40)*1+20+3);	
			glVertex2f((cx-40)*(dHighUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, (cy-40)*1+20-3);	
			glEnd();	
		}

	}
	glFlush();
	SwapBuffers(wglGetCurrentDC());
	HDC  hdc=wglGetCurrentDC();
	//::SetBkMode(hdc, TRANSPARENT );
	::SetTextColor( hdc, RGB(255,255,255) );
	::SetBkColor(hdc, RGB(0, 0, 0));

	HFONT hFont,hOldFont;
	hFont=::CreateFont(10,5,0,0,100,0,0,0,DEFAULT_CHARSET,0,0,2,DEFAULT_PITCH,"1");
	::SelectObject(hdc,hFont);
	CPen NewPen(PS_SOLID,4,RGB(255,255,0));
	CPen *pOldPen=dc->SelectObject(&NewPen);
	CFont font;	
	font.CreateFont(7,5,0,0,100,0,0,0,DEFAULT_CHARSET,0,0,2,DEFAULT_PITCH,"1");	
	CFont *pOldFont=dc->SelectObject(&font);

	CString str;	
	for(int i=1;i<=10;i++)
	{   str.Format("%.1f",i*0.1);	
	::TextOut(hdc,4,cy-20-i*0.1*(cy-40)-3,str,str.GetLength());
	}
	for(int i=0;i<=10;i++)
	{
		str.Format("%f",m_dDispMin+i*step1);
		str = str.Left(6);
		::TextOut(hdc,(cx-40)*(step1*i)/(m_dDispMax-m_dDispMin)+12,cy-19,str,str.GetLength());
	}
	str="Optimality";
	::TextOut(hdc,3,0,str,str.GetLength());
	str="Env. Condition";
	::TextOut(hdc,cx/2-5,cy-10,str,str.GetLength());
	::SelectObject(hdc,pOldFont);
	::SelectObject(hdc,pOldPen);

}

double CPointRule::ComputeRuleGraphMin()
{
	if(fabs(m_fCentralValue-NOFLOATVALUE)<VERYSMALL)
	{
		return -1000;
	}
	double dLowUnity, dHighUnity,dLowCross,dHighCross;
	dLowUnity = dHighUnity = m_fCentralValue;
	dLowCross = dLowUnity-m_dLWidth;
	dHighCross = dHighUnity+m_dRWidth;
	if(fabs(m_dLWidth-NODOUBLEVALUE)<VERYSMALL||fabs(dLowUnity-NODOUBLEVALUE)<VERYSMALL)
	{
		double x = (dHighCross-dHighUnity)*pow(13.28771239, 0.5);
		return dHighUnity-x;	  
	}
	else
	{
		double x = (dLowUnity-dLowCross)*pow(13.28771239, 0.5);
		return  dLowUnity-x;
	}	
}

double CPointRule::ComputeRuleGraphMax()
{
	if(fabs(m_fCentralValue-NOFLOATVALUE)<VERYSMALL)
	{
		return 1000;
	}
	double dLowUnity, dHighUnity,dLowCross,dHighCross;
	dLowUnity = dHighUnity = m_fCentralValue;
	dLowCross = dLowUnity-m_dLWidth;
	dHighCross = dHighUnity+m_dRWidth;
	if(fabs(m_dRWidth -NODOUBLEVALUE)<VERYSMALL)
	{
		double x = (dLowUnity-dLowCross)*pow(13.28771239, 0.5);
		return dLowUnity+x;	 
	} 
	else
	{
		double x = (dHighCross-dHighUnity)*pow(13.28771239, 0.5);
		return dHighUnity +x;
	}
	
}

int CPointRule::ComputeScreenCoordX(double x, int cx)
{
	return (cx-40)*(x-m_dDispMin)/(m_dDispMax-m_dDispMin)+20;
}
int CPointRule::ComputeScreenCoordY(double y, int cy)
{
	return (1-y)*cy+40*y-20;
}
double CPointRule::ComputeGraphCoordX(int x, int cx)
{
	return (x-20)*(m_dDispMax-m_dDispMin)/(cx-40)+m_dDispMin;
}
double CPointRule::ComputeGraphCoordY(int y, int cy)
{
	return (cy-y-20)/(cy-40);
}

bool CPointRule::IsLegal(void)
{
	return true;
}

void CPointRule::setCentralValue(float fCentralValue)
{
	m_fCentralValue = fCentralValue;
}
float CPointRule::getCentralValue()
{
	return m_fCentralValue;
}

void CPointRule::ComputeCentralValue(CEnvDatabase *pEnvDatabase)
{
	m_fCentralValue = NOFLOATVALUE;
	int iCurLayer = -1;
	for(int i = 0; i<pEnvDatabase->getLayerNum(); i++)
	{
		CString strLayerName;
		strLayerName = pEnvDatabase->getLayer(i)->getLayerName();
		if(m_strLayerName== strLayerName)
		{
			CEnvLayer *pLayer;
			pLayer = pEnvDatabase->FindDataLayer(strLayerName);
			int iRow=-1,iCol=-1;
			m_fCentralValue =  pLayer->getData(m_dCentralX,m_dCentralY);
			return;

		}
	} 
}

double CPointRule::getHighCross()
{
	return m_fCentralValue+m_dRWidth;
}

double CPointRule::getLowCross()
{
	return m_fCentralValue-m_dLWidth;
}

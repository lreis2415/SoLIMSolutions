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
#include "SoLIM.h"
#include ".\freehandrule.h"

CFreehandRule::CFreehandRule(void)
{	
	m_iKnotNum=0;	
}

CFreehandRule::CFreehandRule(CFreehandRule &aFreehandRule)
{	
	m_iKnotNum = 0;	
	m_strRuleName = aFreehandRule.getRuleName();
	m_bIsUsed = aFreehandRule.getIsUsed();
	m_dDispMin = aFreehandRule.getDispMin();
	m_dDispMax = aFreehandRule.getDispMax();
	m_iRuleType = aFreehandRule.getRuleType();
	m_strLayerName = aFreehandRule.getLayerName();
	for(int i=0;i<aFreehandRule.getKnotNum();i++)
	{
		double x,y;
		aFreehandRule.getKnot(i,&x,&y);
		AddKnot(x,y);
	}
	m_dCurGraphMin = aFreehandRule.getCurGraphMin();
	m_dCurGraphMax = aFreehandRule.getCurGraphMax();
}

CFreehandRule::~CFreehandRule(void)
{
}

int CFreehandRule::getKnotNum()
{
	return m_iKnotNum;
}

void CFreehandRule::getKnot(int iIndex, double *dX, double *dY)
{
	*dX = m_vecKnotX[iIndex];
	*dY = m_vecKnotY[iIndex];
}

void CFreehandRule::setKnot(int iIndex, double dX, double dY)
{
	if(iIndex+1>m_iKnotNum)
		m_iKnotNum++;
	m_vecKnotX[iIndex] = dX;
	m_vecKnotY[iIndex] = dY;
	BubbleSort();
}

void CFreehandRule::AddKnot(double x, double y)
{
	m_vecKnotX.push_back(x);
	m_vecKnotY.push_back(y);
	m_iKnotNum++;
	BubbleSort();
}

void CFreehandRule::DeleteKnot(int iIndex)
{
	m_vecKnotX.erase(m_vecKnotX.begin()+iIndex);
	m_vecKnotY.erase(m_vecKnotY.begin()+iIndex);
	m_iKnotNum--;
	BubbleSort();
}


void CFreehandRule::DrawGraph(int cx, int cy, CPaintDC *dc)
{
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

	//draw axises
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

	//if there are more than 2 knots, draw the curve
	if(getKnotNum()>2)
	{
		double step = (getDispMax()-getDispMin())/100;
		double result[101];	
		for(int i=0;i<101;i++)
		{
			result[i]=Evaluate(m_dDispMin+step*i);

		}

		for(int i=0;i<100;i++)
		{
			glVertex2f((cx-40)*(step*i)/(m_dDispMax-m_dDispMin)+20, (cy-40)*result[i]+20);		
			glVertex2f((cx-40)*(step*i+step)/(m_dDispMax-m_dDispMin)+20, (cy-40)*result[i+1]+20);		
		}
	}
	double step1=(m_dDispMax-m_dDispMin)/10;
	for(int i=0;i<=10;i++)
	{
		glVertex2f((cx-40)*(step1*i)/(m_dDispMax-m_dDispMin)+20,20);
		glVertex2f((cx-40)*(step1*i)/(m_dDispMax-m_dDispMin)+20,23);
	}
	for(int i=1;i<=10;i++)
	{ 	
		glVertex2f(20,20+i*0.1*(cy-40));
		glVertex2f(23,20+i*0.1*(cy-40));		

	}
	glEnd();
	for(int i=0;i<getKnotNum();i++)
	{
		glBegin(GL_QUADS);		
		glVertex2f((getKnotX(i)-m_dDispMin)*(cx-40)/(m_dDispMax-m_dDispMin)+20-3, getKnotY(i)*(cy-40)+20-3);		
		glVertex2f((getKnotX(i)-m_dDispMin)*(cx-40)/(m_dDispMax-m_dDispMin)+20-3, getKnotY(i)*(cy-40)+20+3);	
		glVertex2f((getKnotX(i)-m_dDispMin)*(cx-40)/(m_dDispMax-m_dDispMin)+20+3, getKnotY(i)*(cy-40)+20+3);	
		glVertex2f((getKnotX(i)-m_dDispMin)*(cx-40)/(m_dDispMax-m_dDispMin)+20+3, getKnotY(i)*(cy-40)+20-3);	
		glEnd();	
	}
	glFlush();

	SwapBuffers(wglGetCurrentDC());
	HDC  hdc=wglGetCurrentDC();
		//::SetBkMode(hdc, TRANSPARENT );
	::SetTextColor( hdc, RGB(255,255,255) );
	::SetBkColor(hdc, RGB(0, 0, 0));

	//draw coord axis
	CPen NewPen(PS_SOLID,4,RGB(255,255,0));
	CPen *pOldPen=dc->SelectObject(&NewPen);
	CFont font;	
	font.CreateFont(7,5,0,0,100,0,0,0,DEFAULT_CHARSET,0,0,2,DEFAULT_PITCH,"1");	
	HFONT hFont;
	hFont=::CreateFont(10,5,0,0,100,0,0,0,DEFAULT_CHARSET,0,0,2,DEFAULT_PITCH,"1");
	::SelectObject(hdc,hFont);
	CString str;	
	for(int i=1;i<=10;i++)
	{   
		str.Format("%.1f",i*0.1);	
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
	str=this->getLayerName();
	if(str == "")
		str = "Env. Condition";
	::TextOut(hdc,cx/2-5,cy-10,str,str.GetLength());

}

double CFreehandRule::ComputeRuleGraphMin()
{	
	if(getKnotNum()>2)
	{
		return getKnotX(0)-0.1*(getKnotX(getKnotNum()-1) - getKnotX(0));
	}
	else 
		return -1000;
}

double CFreehandRule::ComputeRuleGraphMax()
{
	if(getKnotNum()>2)
	{
		return getKnotX(getKnotNum()-1)+0.1*(getKnotX(getKnotNum()-1) - getKnotX(0));
	}
	else 
		return 1000;
}

int CFreehandRule::ComputeScreenCoordX(double x, int cx)
{
	return (cx-40)*(x-m_dDispMin)/(m_dDispMax-m_dDispMin)+20;
}
int CFreehandRule::ComputeScreenCoordY(double y, int cy)
{
	return (1-y)*cy+40*y-20;
}
double CFreehandRule::ComputeGraphCoordX(int x, int cx)
{
	return (x-20)*(m_dDispMax-m_dDispMin)/(cx-40)+m_dDispMin;
}
double CFreehandRule::ComputeGraphCoordY(int y, int cy)
{
	double t=(double)(cy-y-20)/(cy-40);
	return t;
}

void CFreehandRule::BubbleSort()
{
	double tempx,tempy;
	for(int i=0;i<m_iKnotNum;i++)
	{
		for(int j=0;j<m_iKnotNum-1;j++)
		{
			if(m_vecKnotX[j]>m_vecKnotX[j+1])
			{
				tempx=m_vecKnotX[j+1];
				m_vecKnotX[j+1]=m_vecKnotX[j];
				m_vecKnotX[j]=tempx;
				tempy=m_vecKnotY[j+1];
				m_vecKnotY[j+1]=m_vecKnotY[j];
				m_vecKnotY[j]=tempy;
			}
		}
	}
}

double CFreehandRule::getKnotX(int iIndex)
{
	return m_vecKnotX[iIndex];
}

double CFreehandRule::getKnotY(int iIndex)
{
	return m_vecKnotY[iIndex];
}

bool CFreehandRule::IsLegal()
{
	return true;
}

double CFreehandRule::Evaluate(float fEnvValue)
{
	if(fabs(fEnvValue-NOFLOATVALUE)<VERYSMALL)
	{
		return 1;
	}
	if(m_iKnotNum<3)
		return 1;
    
	//use spline curve technique to generate the inference value
	double result;
	int i,j;
	double h0,h1,alpha,beta;
	vector <double> vecDY(getKnotNum());
	vector <double> vecDDY(getKnotNum());
	vector <double> vecS(getKnotNum());	
	vecDDY[0] = 0;
	vecDDY[m_iKnotNum-1] = 0;
	vecDY[0] = -0.5;
	h0 = m_vecKnotX[1] - m_vecKnotX[0];
	vecS[0] = 3.0 * (m_vecKnotY[1] - m_vecKnotY[0]) / (2.0 * h0) - vecDDY[0] * h0 / 4.0;
	for(j=1;j<=m_iKnotNum-2; j++)
	{
		h1 = m_vecKnotX[j+1]-m_vecKnotX[j];
		alpha = h0/(h0+h1);
		beta = (1.0-alpha)*(m_vecKnotY[j]-m_vecKnotY[j-1])/h0;
		beta = 3.0*(beta+alpha*(m_vecKnotY[j+1]-m_vecKnotY[j])/h1);
		vecDY[j] = -alpha/(2.0+(1.0-alpha)*vecDY[j-1]);
		vecS[j] = beta-(1.0-alpha)*vecS[j-1];
		vecS[j] = vecS[j]/(2.0+(1.0-alpha)*vecDY[j-1]);
		h0=h1;
	}
	vecDY[m_iKnotNum-1] = (3.0*(m_vecKnotY[m_iKnotNum-1]-m_vecKnotY[m_iKnotNum-2])/h1+vecDDY[m_iKnotNum-1]*h1/2-vecS[m_iKnotNum-2])/(2.0+vecDY[m_iKnotNum-2]);
	for(j=m_iKnotNum-2; j>=0; j--)
		vecDY[j] = vecDY[j]*vecDY[j+1]+vecS[j];

	for(j=0; j<=m_iKnotNum-2; j++)
		vecS[j] = m_vecKnotX[j+1]-m_vecKnotX[j];

	for(j=0;j<=m_iKnotNum-2;j++)
	{
		h1 = vecS[j]*vecS[j];
		vecDDY[j] = 6.0*(m_vecKnotY[j+1]-m_vecKnotY[j])/h1-2.0*(2.0*vecDY[j]+vecDY[j+1])/vecS[j];
	}
	h1 = vecS[m_iKnotNum-2]*vecS[m_iKnotNum-2];
	vecDDY[m_iKnotNum-1] = 6*(m_vecKnotY[m_iKnotNum-2]-m_vecKnotY[m_iKnotNum-1])/h1+2*(2*vecDY[m_iKnotNum-1]+vecDY[m_iKnotNum-2])/vecS[m_iKnotNum-2];

	//compute the final result	
	if(fEnvValue>m_vecKnotX[m_iKnotNum-1]) //if the input number is greater than any X 
		result = vecDY[m_iKnotNum-1]*(fEnvValue-m_vecKnotX[m_iKnotNum-1])+m_vecKnotY[m_iKnotNum-1];	
	else if(fEnvValue<m_vecKnotX[0])  //if the input number is smaller than any X 
		result = vecDY[0]*(fEnvValue-m_vecKnotX[0])+m_vecKnotY[0];
	else
	{
		i = 0;
		while(fEnvValue>m_vecKnotX[i+1])
			i = i+1;

		h1 = (m_vecKnotX[i+1]-fEnvValue)/vecS[i];
		h0 = h1*h1;
		result = (3.0*h0-2.0*h0*h1)*m_vecKnotY[i];
		result = result+vecS[i]*(h0-h0*h1)*vecDY[i];
		h1 = (fEnvValue-m_vecKnotX[i])/vecS[i];
		h0 = h1*h1;
		result = result+(3.0*h0-2.0*h0*h1)*m_vecKnotY[i+1];
		result = result-vecS[i]*(h0-h0*h1)*vecDY[i+1];
	} 

	if(result>1)
		result = 1;
	else if(result<0) 
		result = 0;
	return result;  
}




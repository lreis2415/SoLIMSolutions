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
#include ".\rangerule.h"

CRangeRule::CRangeRule(void)
{
	m_dLowCross=-1000;
	m_dLowUnity=0;
	m_dHighCross=1000;
	m_dHighUnity=0;
	m_iCurveType=0;

}

CRangeRule::CRangeRule(CRangeRule &aRangeRule)
{
	m_strRuleName = aRangeRule.getRuleName();
	m_bIsUsed = aRangeRule.getIsUsed();
	m_dDispMin = aRangeRule.getDispMin();
	m_dDispMax = aRangeRule.getDispMax();
	m_iRuleType = aRangeRule.getRuleType();
	m_strLayerName = aRangeRule.getLayerName();
	m_dCurGraphMin = aRangeRule.getCurGraphMin();
	m_dCurGraphMax = aRangeRule.getCurGraphMax();

	m_iCurveType = aRangeRule.getCurveType();
	m_dLowCross = aRangeRule.getLowCross();
	m_dHighCross = aRangeRule.getHighCross();
	m_dLowUnity = aRangeRule.getLowUnity();
	m_dHighUnity = aRangeRule.getHighUnity();

}


CRangeRule::~CRangeRule(void)
{
}


void CRangeRule::setLowCross(double dLowCross)
{
	m_dLowCross = dLowCross;
}
double CRangeRule::getLowCross()
{
	return m_dLowCross;
}
void CRangeRule::setLowUnity(double dLowUnity)
{
	m_dLowUnity = dLowUnity;
}

double CRangeRule::getLowUnity()
{
	return m_dLowUnity;
}

void CRangeRule::setHighUnity(double dHighUnity)
{
	m_dHighUnity = dHighUnity;
}

double CRangeRule::getHighUnity()
{
	return m_dHighUnity;
}

void CRangeRule::setHighCross(double dHighCross)
{
	m_dHighCross = dHighCross;
}

double CRangeRule::getHighCross()
{
	return m_dHighCross;
}

void CRangeRule::setCurveType(int curveType)
{
	m_iCurveType = curveType;
	if(curveType == 0)
	{
		if(m_dHighCross == NODOUBLEVALUE&&m_dHighUnity == NODOUBLEVALUE)
		{
			m_dHighUnity = m_dLowUnity;
			m_dHighCross = m_dHighUnity + (m_dLowUnity - m_dLowCross);
		}
		else if(m_dLowCross == NODOUBLEVALUE&&m_dLowUnity == NODOUBLEVALUE)
		{
			m_dLowUnity = m_dHighUnity;
			m_dLowCross = m_dLowUnity - (m_dHighCross - m_dHighUnity);

		}
	}


	if(curveType == 1)
	{

		if(m_dLowCross == NODOUBLEVALUE&&m_dLowUnity == NODOUBLEVALUE)
		{
			m_dLowUnity = m_dHighUnity;
			m_dLowCross = m_dLowUnity - (m_dHighCross - m_dHighUnity);

		}
		m_dHighCross = NODOUBLEVALUE;
		m_dHighUnity = NODOUBLEVALUE;
	}
	else if(curveType == 2)
	{
		if(m_dHighCross == NODOUBLEVALUE&&m_dHighUnity == NODOUBLEVALUE)
		{
			m_dHighUnity = m_dLowUnity;
			m_dHighCross = m_dHighUnity + (m_dLowUnity - m_dLowCross);
		}
		m_dLowCross = NODOUBLEVALUE;
		m_dLowUnity = NODOUBLEVALUE;
	}


}
int CRangeRule::getCurveType()
{
	return m_iCurveType;
}


double CRangeRule::Evaluate(float fEnvValue)
{
	if(fabs(fEnvValue-NOFLOATVALUE)<VERYSMALL)
	{
		return 0;
	}

	double result;
	if(fabs(m_dLowCross-NODOUBLEVALUE)<VERYSMALL||fabs(m_dLowUnity-NODOUBLEVALUE)<VERYSMALL)  //Z 
	{
		if(fEnvValue<m_dHighUnity)
		{
			result=1;
		}
		else 
		{
			result = exp(pow(fabs(fEnvValue-m_dHighUnity)/(m_dHighCross-m_dHighUnity), 2)*log(0.5));
		}
	}
	else if(fabs(m_dHighCross-NODOUBLEVALUE)<VERYSMALL||fabs(m_dHighUnity-NODOUBLEVALUE)<VERYSMALL) //S 
	{
		if(fEnvValue>m_dLowUnity)
		{
			result=1;
		}
		else
		{
			result = exp(pow(fabs(fEnvValue-m_dLowUnity)/(m_dLowUnity-m_dLowCross), 2)*log(0.5));

		}

	} 
	else  //bell shape
	{
		if(fEnvValue<m_dHighUnity&&fEnvValue>m_dLowUnity)
		{
			result=1;
		}
		else if(fEnvValue<=m_dLowUnity)
		{
			result = exp(pow(fabs(fEnvValue-m_dLowUnity)/(m_dLowUnity-m_dLowCross), 2)*log(0.5));
		}
		else 
		{
			result = exp(pow(fabs(fEnvValue-m_dHighUnity)/(m_dHighCross-m_dHighUnity), 2)*log(0.5));
		}
	}
	return result;

}


void CRangeRule::DrawGraph(int cx, int cy, CPaintDC *dc)
{
	
	

	CPen NewPen(PS_SOLID,1,RGB(0,255,0));
	CPen *pOldPen=dc->SelectObject(&NewPen);

	
	dc->MoveTo(20.0f, cy-20.0f);
	dc->LineTo(cx-10,cy-20.0f);
	dc->MoveTo(20.0f, cy-20.0f);
	dc->LineTo(20.0f,10);
	dc->MoveTo(cx-10,cy-20.0f);
	dc->LineTo(cx-15,cy-25);
	dc->MoveTo(cx-10,cy-20.0f);
	dc->LineTo(cx-15,cy-15);
	dc->MoveTo(20.0f,10);
	dc->LineTo(15,15);
	dc->MoveTo(20.0f,10);
	dc->LineTo(25,15);



	for(int i=1;i<=10;i++)
	{ 	
		dc->MoveTo(20,cy-(20+i*0.1*(cy-40)));
		dc->LineTo(23,cy-(20+i*0.1*(cy-40)));		

	}
	double step1=(m_dDispMax-m_dDispMin)/10;
	for(int i=0;i<=10;i++)
	{
		dc->MoveTo((cx-40)*(step1*i)/(m_dDispMax-m_dDispMin)+20,cy-20);
		dc->LineTo((cx-40)*(step1*i)/(m_dDispMax-m_dDispMin)+20,cy-23);

	}



	double step2=(m_dDispMax-m_dDispMin)/100;	
	double result[101];	
	for(int i=0;i<101;i++)
	{
		result[i]=Evaluate(m_dDispMin+step2*i);

	}


  

	for(int i=0;i<100;i++)
	{

		DrawAntialiasLine(dc, (cx-40)*(step2*i)/(m_dDispMax-m_dDispMin)+20, cy-((cy-40)*result[i]+20),(cx-40)*(step2*i+step2)/(m_dDispMax-m_dDispMin)+20, cy-((cy-40)*result[i+1]+20), RGB(0, 255, 0) );
	
	}


	CBrush brush;
	
	brush.CreateSolidBrush(RGB(0, 255, 0));


	if(fabs(m_dHighCross-NODOUBLEVALUE)<VERYSMALL)
	{
        CRect rc1((cx-40)*(m_dLowCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, cy-((cy-40)*0.5+20+3), (cx-40)*(m_dLowCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, cy-((cy-40)*0.5+20-3));
		CRect rc2((cx-40)*(m_dLowUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, cy-((cy-40)*1+20+3), (cx-40)*(m_dLowUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, cy-((cy-40)*1+20-3));
		
		dc->FillRect(&rc1, &brush);
		dc->FillRect(&rc2, &brush);
		

		
	}

	else if(fabs(m_dLowCross-NODOUBLEVALUE)<VERYSMALL)
	{
		  CRect rc1((cx-40)*(m_dHighCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, cy-((cy-40)*0.5+20+3), (cx-40)*(m_dHighCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, cy-((cy-40)*0.5+20-3));
		CRect rc2((cx-40)*(m_dHighUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, cy-((cy-40)*1+20+3), (cx-40)*(m_dHighUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, cy-((cy-40)*1+20-3));
		
		dc->FillRect(&rc1, &brush);
		dc->FillRect(&rc2, &brush);
			 
		
				

	}
	else 
	{
		
    CRect rc1((cx-40)*(m_dLowCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, cy-((cy-40)*0.5+20+3), (cx-40)*(m_dLowCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, cy-((cy-40)*0.5+20-3));
		CRect rc2((cx-40)*(m_dLowUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, cy-((cy-40)*1+20+3), (cx-40)*(m_dLowUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, cy-((cy-40)*1+20-3));
		
		 CRect rc3((cx-40)*(m_dHighCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, cy-((cy-40)*0.5+20+3), (cx-40)*(m_dHighCross-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, cy-((cy-40)*0.5+20-3));
		CRect rc4((cx-40)*(m_dHighUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20-3, cy-((cy-40)*1+20+3), (cx-40)*(m_dHighUnity-m_dDispMin)/(m_dDispMax-m_dDispMin)+20+3, cy-((cy-40)*1+20-3));
		
		dc->FillRect(&rc1, &brush);
		dc->FillRect(&rc2, &brush);
			 
		dc->FillRect(&rc3, &brush);
		dc->FillRect(&rc4, &brush);
		
			
	}

	


	CFont font;
	font.CreateFont(10,5,0,0,100,0,0,0,DEFAULT_CHARSET,0,0,2,DEFAULT_PITCH,"1");	
	CFont *pOldFont=dc->SelectObject(&font);

	dc->SetBkMode(TRANSPARENT);

     dc->SetTextColor(RGB(255,255,255) );
	
	//::SetBkColor(hdc, RGB(0, 0, 0));


	CString str;	
	for(int i=1;i<=10;i++)
	{   str.Format("%.1f",i*0.1);	
	    
	dc->TextOut(4,cy-20-i*0.1*(cy-40)-3,str,str.GetLength());
	}

	for(int i=0;i<=10;i++)
	{
		str.Format("%f",m_dDispMin+i*step1);
		str = str.Left(6);
		dc->TextOut((cx-40)*(step1*i)/(m_dDispMax-m_dDispMin)+12,cy-19,str,str.GetLength());

	}

	str="Optimality";
	dc->TextOut(3,0,str,str.GetLength());
	str=this->getLayerName();
	if(str == "")
		str = "Env. Condition";
 dc->TextOut(cx/2-5,cy-10,str,str.GetLength());


	dc->SelectObject(pOldFont);
	dc->SelectObject(pOldPen);

}

double CRangeRule::ComputeRuleGraphMin()
{

	if(m_iCurveType == 0||m_iCurveType ==1)
	{
		if(fabs(m_dLowCross-NODOUBLEVALUE)<VERYSMALL||fabs(m_dLowUnity-NODOUBLEVALUE)<VERYSMALL)
			return -1000;
	}
	else if(m_iCurveType == 2)
	{
		if(fabs(m_dHighCross-NODOUBLEVALUE)<VERYSMALL||fabs(m_dHighUnity-NODOUBLEVALUE)<VERYSMALL)
			return -1000;
	}

	if(fabs(m_dLowCross-NODOUBLEVALUE)<VERYSMALL||fabs(m_dLowUnity-NODOUBLEVALUE)<VERYSMALL)
	{
		double x = (m_dHighCross-m_dHighUnity)*pow(13.28771239, 0.5);
		return m_dHighUnity-x;	  
	}  
	else
	{
		double x = (m_dLowUnity-m_dLowCross)*pow(13.28771239, 0.5); 
		return  m_dLowUnity-x;
	}

}

double CRangeRule::ComputeRuleGraphMax()
{

	if(m_iCurveType == 0||m_iCurveType ==2)
	{
		if(fabs(m_dHighCross-NODOUBLEVALUE)<VERYSMALL||fabs(m_dHighUnity-NODOUBLEVALUE)<VERYSMALL)
			return 1000;
	}
	else if(m_iCurveType == 1)
	{
		if(fabs(m_dLowCross-NODOUBLEVALUE)<VERYSMALL||fabs(m_dLowUnity-NODOUBLEVALUE)<VERYSMALL)
			return 1000;
	}

	if(fabs(m_dHighCross-NODOUBLEVALUE)<VERYSMALL||fabs(m_dHighUnity-NODOUBLEVALUE)<VERYSMALL)
	{
		double x = (m_dLowUnity-m_dLowCross)*pow(13.28771239, 0.5);
		return m_dLowUnity+x;	 

	} 
	else
	{
		double x = (m_dHighCross-m_dHighUnity)*pow(13.28771239, 0.5);
		return m_dHighUnity +x;
	}
	
}

int CRangeRule::ComputeScreenCoordX(double x, int cx)
{
	return (cx-40)*(x-m_dDispMin)/(m_dDispMax-m_dDispMin)+20;
}
int CRangeRule::ComputeScreenCoordY(double y, int cy)
{
	return (1-y)*cy+40*y-20;
}
double CRangeRule::ComputeGraphCoordX(int x, int cx)
{
	return (x-20)*(m_dDispMax-m_dDispMin)/(cx-40)+m_dDispMin;
}
double CRangeRule::ComputeGraphCoordY(int y, int cy)
{
	return (cy-y-20)/(cy-40);
}

bool CRangeRule::IsLegal(void)
{

	if(m_iCurveType == 0)
	{
		if(m_dLowCross>=m_dLowUnity||m_dHighUnity<m_dLowUnity||m_dHighCross<=m_dHighUnity)
			return false;
	}
	else if(m_iCurveType == 1)
	{
		if(m_dLowCross>=m_dLowUnity)
			return false;
	}
	else if(m_iCurveType == 2)
	{
		if(m_dHighCross<=m_dHighUnity)
			return false;
	}

	return true;
}

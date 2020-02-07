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
#include ".\wordcurve.h"

CWordCurve::CWordCurve(void)
{
	m_iKnotNum =0;
}

CWordCurve::CWordCurve(CWordCurve &aWordCurve)
{
	m_iKnotNum =0;
	m_strPhrase = aWordCurve.getPhrase();
	m_strEnvAttri = aWordCurve.getEnvAttri();
	for(int i=0;i<aWordCurve.getKnotNum();i++)
	{
		double x,y;
		aWordCurve.getKnot(i,&x,&y);
		AddKnot(x,y);
	}
}
CWordCurve::~CWordCurve(void)
{
}

CString CWordCurve::getPhrase()
{
	return m_strPhrase;
}

void CWordCurve::setPhrase(CString strPhrase)
{
	m_strPhrase = strPhrase;
}

int CWordCurve::getKnotNum()
{
	return m_iKnotNum;
}

void CWordCurve::getKnot(int iIndex, double *dX, double *dY)
{
	*dX = m_vecKnotX[iIndex];
	*dY = m_vecKnotY[iIndex];
}


void CWordCurve::setKnot(int iIndex, double dX, double dY)
{
	if(iIndex+1>m_iKnotNum) 
		m_iKnotNum++;
	m_vecKnotX[iIndex] = dX;
	m_vecKnotX[iIndex] = dY;
}

void CWordCurve::AddKnot(double x, double y)
{
	m_vecKnotX.push_back(x);
	m_vecKnotY.push_back(y);
	m_iKnotNum++;
}

void CWordCurve::DeleteKnot(int iIndex)
{
	m_vecKnotX.erase(m_vecKnotX.begin()+iIndex);
	m_vecKnotY.erase(m_vecKnotY.begin()+iIndex);
	m_iKnotNum--;

}


double CWordCurve::getKnotX(int iIndex)
{
	return m_vecKnotX[iIndex];
}

double CWordCurve::getKnotY(int iIndex)
{
	return m_vecKnotY[iIndex];
}

CString CWordCurve::getEnvAttri()
{
	return m_strEnvAttri;
}

void CWordCurve::setEnvAttri(CString strEnvAttri)
{
	m_strEnvAttri=strEnvAttri;
}


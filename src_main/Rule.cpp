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
#include ".\rule.h"

CRule::CRule(void)
{
	m_bIsUsed = true;
	m_strRuleName = "Untitled_Rule";
	m_strLayerName = "";
	m_iRuleType = 0;
	m_dCurGraphMin = -1000;
	m_dCurGraphMax = 1000;
	m_dDispMin = -1000;
	m_dDispMax = 1000;	

}

CRule::~CRule(void)
{
}

CString CRule::getRuleName()
{
	return m_strRuleName;
}

void CRule::setRuleName(CString strRuleName)
{
	m_strRuleName = strRuleName;
}
int CRule::getRuleType()
{
	return m_iRuleType;
}

void CRule::setRuleType(int iRuleType)
{
	m_iRuleType = iRuleType;
}

bool CRule::getIsUsed()
{
	return m_bIsUsed;
}

void CRule::setIsUsed(bool bIsUsed)
{
	m_bIsUsed = bIsUsed;
}

void CRule::setLayerName(CString strLayerName)
{
	m_strLayerName = strLayerName;
}

CString CRule::getLayerName()
{
	return m_strLayerName;
}

double CRule::getDispMin()
{
	return m_dDispMin;
}

void CRule::setDispMin(double dDispMin)
{
	m_dDispMin = dDispMin;
}

double CRule::getDispMax()
{
	return m_dDispMax;
}

void CRule::setDispMax(double dDispMax)
{
	m_dDispMax = dDispMax;
}

double CRule::getCurGraphMin()
{
	return m_dCurGraphMin;
}

void CRule::setCurGraphMin(double dCurGraphMin)
{
	m_dCurGraphMin=dCurGraphMin;
}

double CRule::getCurGraphMax()
{
	return m_dCurGraphMax;
}

void CRule::setCurGraphMax(double dCurGraphMax)
{
	m_dCurGraphMax=dCurGraphMax;
}

void CRule::setLayerID(int ID)
{
	m_iLayerID = ID;
}

int CRule::getLayerID()
{
	return m_iLayerID;
}

double CRule::getEvaluateResult(int iRow, int iCol,CEnvDatabase *pEnvDatabase)
{	
	if(m_strLayerName != "")
	{
		float fEnvValue=pEnvDatabase->FindDataLayer(m_strLayerName)->m_pTheData->getData(iRow,iCol);
		return Evaluate(fEnvValue);
	}
	else 
		return 1;

}



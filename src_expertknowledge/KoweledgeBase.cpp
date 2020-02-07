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
#include ".\KnowledgeBase.h"

CKnowledgeBase::CKnowledgeBase(void)
{
	m_iSoilTypeNum = 0;

}

CKnowledgeBase::CKnowledgeBase(CKnowledgeBase &aKnowledgeBase)
{
	m_iSoilTypeNum = 0;
	for(int i = 0;i<aKnowledgeBase.getSoilTypeNum();i++)
	{
		CSoilType *tempSoilType = new CSoilType(*(aKnowledgeBase.getSoilType(i)));
		AddSoilType(tempSoilType);
	}
}

CKnowledgeBase::~CKnowledgeBase(void)
{
	while(m_iSoilTypeNum != 0)
	{
		CSoilType *pTempSoilType = m_vecSoilType[m_iSoilTypeNum-1];
		m_vecSoilType.pop_back();
		if(pTempSoilType!= NULL)
		{
			delete pTempSoilType;
			pTempSoilType= NULL;
		}
		m_iSoilTypeNum--;
	}
}
int CKnowledgeBase::getSoilTypeNum()
{
	return m_iSoilTypeNum;
}

CSoilType* CKnowledgeBase::getSoilType(int iIndex)
{
	return m_vecSoilType[iIndex];
}


bool CKnowledgeBase::AddSoilType(CSoilType* newSoilType)
{
	for(int i=0;i<getSoilTypeNum();i++)
	{
		if(newSoilType->getSoilTypeName()==m_vecSoilType[i]->getSoilTypeName())
		{
			AfxMessageBox("The name of the new soil type is the same with one of existing soil types. Please specify another name.");
			delete newSoilType;
			newSoilType = NULL;
			return false;
		}
	}
	m_vecSoilType.push_back(newSoilType);
	m_iSoilTypeNum++;
	return true;
}

void CKnowledgeBase::DeleteSoilType(int iIndex)
{
	CSoilType *pTempSoilType = m_vecSoilType[iIndex];
	m_vecSoilType.erase(m_vecSoilType.begin()+iIndex);
	if(NULL != pTempSoilType)
	{
		delete pTempSoilType;
		pTempSoilType = NULL;
	}
	m_iSoilTypeNum--;
}



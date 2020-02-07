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
#include ".\instance.h"
#include "RangeRule.h"
#include "FreehandRule.h"
#include "WordRule.h"
#include "PointRule.h"
#include "EnumRule.h"


CInstance::CInstance(void)
{
	m_iRuleNum = 0;
	m_strInstanceName = "Untitled_Instance";

}
CInstance::CInstance(CInstance &anInstance)
{
	m_iRuleNum = 0;
	m_strInstanceName = anInstance.getInstanceName();
	for(int i=0;i<anInstance.getRuleNum();i++)
	{
		if(anInstance.getRule(i)->getRuleType()==0)
		{
			CRangeRule *tempRule = new CRangeRule(*((CRangeRule*)(anInstance.getRule(i))));
			AddRule((CRule*)tempRule);
		}
		else if(anInstance.getRule(i)->getRuleType()==1)
		{
			CFreehandRule *tempRule = new CFreehandRule(*((CFreehandRule*)(anInstance.getRule(i))));
			AddRule((CRule*)tempRule);
		}
		else if(anInstance.getRule(i)->getRuleType()==2)
		{
			CWordRule *tempRule = new CWordRule(*((CWordRule*)(anInstance.getRule(i))));
			AddRule((CRule*)tempRule);
		}
		else if(anInstance.getRule(i)->getRuleType()==3)
		{
			CPointRule *tempRule = new CPointRule(*((CPointRule*)(anInstance.getRule(i))));
			AddRule((CRule*)tempRule);
		}
		else if(anInstance.getRule(i)->getRuleType()==4)
		{
			CEnumRule *tempRule = new CEnumRule(*((CEnumRule*)(anInstance.getRule(i))));
			AddRule((CRule*)tempRule);		
		}

	} 

}

CInstance::~CInstance(void)
{
	while(m_iRuleNum != 0)
	{
		CRule *pTempRule = m_vecRule[m_iRuleNum-1];
		m_vecRule.pop_back();
		if(pTempRule!= NULL)
		{
			delete pTempRule;
			pTempRule = NULL;
		}
		m_iRuleNum--;
	}
}


CRule* CInstance::getRule(int iIndex)
{
	return m_vecRule[iIndex];
}

CString CInstance::getInstanceName()
{
	return m_strInstanceName;
}

void CInstance::setInstanceName(CString strInstanceName)
{
	m_strInstanceName = strInstanceName;
}

int  CInstance::getRuleNum()
{
	return m_iRuleNum;
}

void CInstance::DeleteRule(int iIndex)
{
	CRule *pTempRule = m_vecRule[iIndex];
	m_vecRule.erase(m_vecRule.begin()+iIndex);
	if(NULL != pTempRule)
	{
		delete pTempRule;
		pTempRule = NULL;
	}
	m_iRuleNum--;
}

bool CInstance::AddRule(CRule* newRule)
{
	for(int i=0;i<m_iRuleNum;i++)
	{
		if(newRule->getRuleName()==m_vecRule[i]->getRuleName())
		{
			AfxMessageBox("The name of the new rule is the same with one of existing rules. Please use another name.");
			delete newRule;
			newRule = NULL;
			return false;
		}
	}
	m_vecRule.push_back(newRule);
	m_iRuleNum++;
	return true;
}

double CInstance::getInstanceResult(double dX,double dY, CEnvDatabase *pEnvDatabase,int *iRuleID )
{
	double dMin=1;
	double dSim;

	for(int i=0;i<m_iRuleNum;i++)
	{		
		if(getRule(i)->getLayerID() == -1)
		{
			return 1;
		}
        if(getRule(i)->getIsUsed()==true)
		{	     
			dSim = getRule(i)->Evaluate(pEnvDatabase->getLayer(getRule(i)->getLayerID())->getData(dX,dY));
			if(dSim<=dMin)  //is current similarity is smaller than the current minimum one
			{
				dMin=dSim;
				*iRuleID = i+1;
			}
		}
	}
	return dMin;
}






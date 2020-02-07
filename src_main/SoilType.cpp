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
#include ".\soiltype.h"


CSoilType::CSoilType(void)
{
	m_bIsUsed=true;
	m_iInstanceNum = 0;
	m_iOccurrenceNum = 0;
	m_iExclusionNum = 0;
	m_iSoilTypeID = -1;
	m_strSoilTypeName = "Untitled_SoilType";

}

CSoilType::CSoilType(CSoilType &aSoilType)
{ 
	m_iInstanceNum = 0;
	m_iOccurrenceNum = 0;
	m_iExclusionNum = 0;
	m_bIsUsed = aSoilType.getIsUsed();
	m_iSoilTypeID = aSoilType.getSoilTypeID();
	m_strSoilTypeName = aSoilType.getSoilTypeName();

	for(int i=0;i<aSoilType.getInstanceNum();i++)
	{
		CInstance *tempInstance = new CInstance(*(aSoilType.getInstance(i)));
		AddInstance(tempInstance);
	}
	for(i=0;i<aSoilType.getOccurrenceNum();i++)
	{
		COccurrence *tempOccurrence  = new COccurrence(*(aSoilType.getOccurrence (i)));
		AddOccurrence (tempOccurrence );
	}
	for(i=0;i<aSoilType.getExclusionNum();i++)
	{
		CExclusion *tempExclusion = new CExclusion(*(aSoilType.getExclusion(i)));
		AddExclusion(tempExclusion);
	}
}

CSoilType::~CSoilType(void)
{
	while(m_iInstanceNum != 0)
	{
		CInstance *pTempInstance = m_vecInstance [m_iInstanceNum-1];
		m_vecInstance.pop_back();
		if(pTempInstance != NULL)
		{
			delete pTempInstance ;
			pTempInstance = NULL;
		}
		m_iInstanceNum--;
	}
	while(m_iOccurrenceNum != 0)
	{
		COccurrence *pTempOccurrence = m_vecOccurrence[m_iOccurrenceNum-1];
		m_vecOccurrence.pop_back();
		if(pTempOccurrence != NULL)
		{
			delete pTempOccurrence ;
			pTempOccurrence = NULL;
		}
		m_iOccurrenceNum--;
	}
	while(m_iExclusionNum != 0)
	{
		CExclusion *pTempExclusion = m_vecExclusion[m_iExclusionNum-1];
		m_vecExclusion.pop_back();
		if(pTempExclusion != NULL)
		{
			delete pTempExclusion ;
			pTempExclusion = NULL;
		}
		m_iExclusionNum--;
	}
}
int CSoilType::getInstanceNum(void)
{
	return m_iInstanceNum;
}

int CSoilType::getExclusionNum(void)
{
	return m_iExclusionNum;
}

int CSoilType::getOccurrenceNum(void)
{
	return m_iOccurrenceNum;
}
CInstance* CSoilType::getInstance(int iIndex)
{
	return m_vecInstance[iIndex];
}
COccurrence* CSoilType::getOccurrence(int iIndex)
{
	return m_vecOccurrence[iIndex];
}
CExclusion* CSoilType::getExclusion(int iIndex)
{
	return m_vecExclusion[iIndex];
}

CString CSoilType::getSoilTypeName()
{
	return m_strSoilTypeName;
}
void CSoilType::setSoilTypeName(CString strSoilTypeName)
{
	m_strSoilTypeName = strSoilTypeName;
}

int CSoilType::getSoilTypeID()
{
	return m_iSoilTypeID;
}

void CSoilType::setSoilTypeID(int iSoilTypeID)
{
	m_iSoilTypeID = iSoilTypeID;
}

bool CSoilType::getIsUsed()
{
	return m_bIsUsed;
}

void  CSoilType::setIsUsed(bool bIsUsed)
{
	m_bIsUsed = bIsUsed;
}

bool CSoilType::AddInstance(CInstance* newInstance)
{
	for(int i=0;i<m_iInstanceNum;i++)
	{
		if(newInstance->getInstanceName()==m_vecInstance[i]->getInstanceName())
		{
			AfxMessageBox("The name of the new instance is the same with one of existing instances.Please specify another name.");
			delete newInstance;
			newInstance=NULL;
			return false;
		}
	}

	m_vecInstance.push_back(newInstance);
	m_iInstanceNum++;
	return true;
}

void CSoilType::DeleteInstance(int iIndex)
{
	CInstance *pTempInstance = m_vecInstance [iIndex];
	m_vecInstance.erase(m_vecInstance.begin()+iIndex);

	if(pTempInstance != NULL)
	{
		delete pTempInstance ;
		pTempInstance = NULL;
	}	
	m_iInstanceNum--;
}

bool CSoilType::AddOccurrence(COccurrence* newOccurrence)
{
	for(int i=0;i<m_iOccurrenceNum;i++)
	{
		if(newOccurrence->getOccurrenceName()==m_vecOccurrence[i]->getOccurrenceName())
		{
			AfxMessageBox("The name of the new occurrence is the same with one of existing occurrences.Please specify another name.");
			delete newOccurrence;
			newOccurrence=NULL;
			return false;
		}
	}
	m_vecOccurrence.push_back(newOccurrence);
	m_iOccurrenceNum++;
	return true;

}
void CSoilType::DeleteOccurrence(int iIndex)
{
	COccurrence *pTempOccurrence= m_vecOccurrence[iIndex];
	m_vecOccurrence.erase(m_vecOccurrence.begin()+iIndex);

	if(pTempOccurrence != NULL)
	{
		delete pTempOccurrence;
		pTempOccurrence = NULL;
	}	
	m_iOccurrenceNum--;
}

bool CSoilType::AddExclusion(CExclusion* newExclusion)
{
	for(int i=0;i<m_iExclusionNum;i++)
	{
		if(newExclusion->getExclusionName()==m_vecExclusion[i]->getExclusionName())
		{
			AfxMessageBox("The name of the new exclusion is the same with one of existing exclusions.Please specify another name.");
			delete newExclusion;
			newExclusion=NULL;
			return false;
		}
	}
	m_vecExclusion.push_back(newExclusion);
	m_iExclusionNum++;
	return true;
}
void CSoilType::DeleteExclusion(int iIndex)
{
	CExclusion *pTempExclusion= m_vecExclusion[iIndex];
	m_vecExclusion.erase(m_vecExclusion.begin()+iIndex);

	if(pTempExclusion != NULL)
	{
		delete pTempExclusion ;
		pTempExclusion = NULL;
	}	
	m_iExclusionNum--;
}

void CSoilType::getInferenceResult(float *resultBuf, CEnvDatabase *pEnvDatabase,bool bUseMask, bool *maskBuf, bool bCreateCheckFile, float *chkBuf,CProgressCtrl *ProgressBar, int iCurSoilID,int iSoilNum, double dCellSize)
{
	float nodata = -9999;

	if(pEnvDatabase->getLayerNum()<=0)
	{
		AfxMessageBox("No layer is in environmental database.");
		return;
	}

     double xMin = pEnvDatabase->m_pTheSetting->getWestMin();
	double xMax = pEnvDatabase->m_pTheSetting->getEastMin();
	
	double yMin = pEnvDatabase->m_pTheSetting->getSouthMin();
	double yMax = pEnvDatabase->m_pTheSetting->getNorthMin();




	int iRowNum = (yMax - yMin)/dCellSize;
	int iColNum = (xMax - xMin)/dCellSize;

	//int iRowNum = pEnvDatabase->m_pTheSetting->getRowNum();
	//int iColNum = pEnvDatabase->m_pTheSetting->getColNum();
  
	//these variables are used in check file creation
	int iCurDecisiveFactor=1;
	int iCurDecisiveID = 1;
	int iCurDicisiveRule = 1;

	double dResultSim=0;  //initial similarity 
	double dCurSim;  //temporary similarity
	int iRuleID=1;
	double dDist=0; //the distance between pixel and ocurrence/exclusion

	if(m_iInstanceNum == 0)
	{
		AfxMessageBox(this->getSoilTypeName() + " must contains at least one instance");
		return;
	}

	//if(m_iInstanceNum==0&&m_iOccurrenceNum==0)  //if there are only exclusions
	//	dResultSim=1;  

	for(int k=0;k<m_iInstanceNum;k++)
	{
		int iRuleNum = getInstance(k)->getRuleNum();
		//assign layer id for each rule before running inference
		for(int j=0;j<iRuleNum;j++)
		{
			int iLayerID = pEnvDatabase->FindDataLayerID(getInstance(k)->getRule(j)->getLayerName());
			getInstance(k)->getRule(j)->setLayerID(iLayerID);

		}
	}



	for(int k=0;k<m_iOccurrenceNum;k++)
	{
		int iRuleNum = getOccurrence(k)->getRuleNum();
		//assign layer id for each rule before running inference

		bool hasRuleUsed = false;
		for(int j=0;j<iRuleNum;j++)
		{
			int iLayerID = pEnvDatabase->FindDataLayerID(getOccurrence(k)->getRule(j)->getLayerName());
			getOccurrence(k)->getRule(j)->setLayerID(iLayerID);
			if(getOccurrence(k)->getRule(j)->getIsUsed() == true)
			  hasRuleUsed = true;
		}

		//if the occurrence has no  rule being used,it will not take part in inference
		if(hasRuleUsed == false)
			getOccurrence(k)->setInRange(false);		
		//if the occurrence is out of the range of research area, it will not be used
		else if(getOccurrence(k)->getCentralX()<pEnvDatabase->m_pTheSetting->getWest()||getOccurrence(k)->getCentralX()>pEnvDatabase->m_pTheSetting->getEast()
			||getOccurrence(k)->getCentralY()>pEnvDatabase->m_pTheSetting->getNorth()||getOccurrence(k)->getCentralY()<pEnvDatabase->m_pTheSetting->getSouth())
			getOccurrence(k)->setInRange(false);	
		else
		{
			getOccurrence(k)->setInRange(true);
			getOccurrence(k)->ComputeCentralPointResult(pEnvDatabase);
		}
	}

	for(int k=0;k<m_iExclusionNum;k++)
	{
		int iRuleNum = getExclusion(k)->getRuleNum();
		//assign layer id for each rule before running inference
		bool hasRuleUsed = false;
		for(int j=0;j<iRuleNum;j++)
		{
			int iLayerID = pEnvDatabase->FindDataLayerID(getExclusion(k)->getRule(j)->getLayerName());
			getExclusion(k)->getRule(j)->setLayerID(iLayerID);
			if(getExclusion(k)->getRule(j)->getIsUsed() == true)
			  hasRuleUsed = true;
		}
		
		//if the exclusion has no  rule being used,it will not take part in inference
		if(hasRuleUsed == false)
			getExclusion(k)->setInRange(false);	
		//is the exclusion is out of the range of research area, it will not be used
		else if(getExclusion(k)->getCentralX()<pEnvDatabase->m_pTheSetting->getWest()||getExclusion(k)->getCentralX()>pEnvDatabase->m_pTheSetting->getEast()
			||getExclusion(k)->getCentralY()>pEnvDatabase->m_pTheSetting->getNorth()||getExclusion(k)->getCentralY()<pEnvDatabase->m_pTheSetting->getSouth())
			getExclusion(k)->setInRange(false);

		else
		{
			getExclusion(k)->setInRange(true);
			getExclusion(k)->ComputeCentralPointResult(pEnvDatabase);
		}
	}

	double dX;  //to store the x coordinate of pixel
	double dY;  //to store the y coordinate of pixel

	if(bUseMask==false) //if there is no mask
	{
		for(int i=0;i<iRowNum;i++)
		{
			ProgressBar->SetPos(100*(iCurSoilID+(double)i/(double)iRowNum)/iSoilNum);  //indicate current process
			for(int j=0;j<iColNum;j++)
			{
			

				dX=xMin + dCellSize * j + 0.5 * dCellSize;
				dY=yMax - dCellSize * i - 0.5 * dCellSize; 
			
				dResultSim = 0;
				for(int k=0;k<m_iInstanceNum;k++)
				{
					dCurSim = getInstance(k)->getInstanceResult(dX,dY,pEnvDatabase,&iRuleID);
					if(dCurSim>=dResultSim)  //if current similarity is greater than existing one
					{						
						dResultSim=dCurSim;
						//record decisive factors
						iCurDecisiveFactor = 1;
						iCurDecisiveID = k+1;
						iCurDicisiveRule = iRuleID;
					}
				}

				bool bInOcc = false;
			
				for(int k =0;k<m_iOccurrenceNum;k++)
				{
					if(getOccurrence(k)->getInRange()==false)
						continue;					
					if (getOccurrence(k)->IsInRange(i,j,pEnvDatabase,&dDist))
					{
						bInOcc = true;
						dResultSim = 0;
						dCurSim = getOccurrence(k)->getOccurrenceResult(dX,dY,pEnvDatabase,&iRuleID,dDist, this->m_vecInstance, m_iInstanceNum);
						if(dCurSim>=dResultSim)  //if current similarity is greater than existing one
						{
							dResultSim=dCurSim;
							iCurDecisiveFactor = 2;
							iCurDecisiveID = k+1;
							iCurDicisiveRule = iRuleID;
						}
					}
					else
					{
						continue;
					}
				}

				for(int k =0;k<m_iExclusionNum;k++)
				{
					if(getExclusion(k)->getInRange()==false)
						continue;				
					if (getExclusion(k)->IsInRange(i,j,pEnvDatabase,&dDist))
					{
						if(bInOcc == false)
						   dResultSim = 1;
						dCurSim = getExclusion(k)->getExclusionResult(dX,dY,pEnvDatabase,&iRuleID,dDist, this->m_vecInstance, m_iInstanceNum);
						if(dCurSim<=dResultSim) //if current similarity is smaller than existing one
						{
							dResultSim=dCurSim;
							iCurDecisiveFactor = 3;
							iCurDecisiveID = k+1;
							iCurDicisiveRule = iRuleID;
						}
					}
					else
					{
						continue;
					}

				}
				resultBuf[i*iColNum+j]=dResultSim*100;
				chkBuf[i*iColNum+j]= iCurDecisiveFactor*100+ iCurDecisiveID+0.01*iCurDicisiveRule;
			}
		}
	}

	else  //if there is a mask
	{
		for(int i=0;i<iRowNum;i++)	
		{
			ProgressBar->SetPos(100*(iCurSoilID+(double)i/(double)iRowNum)/iSoilNum);  //indicate current process
			for(int j=0;j<iColNum;j++)
			{
			
			dX=xMin + dCellSize * j + 0.5 * dCellSize;
				dY=yMax - dCellSize * i - 0.5 * dCellSize; 

				if(maskBuf[i*iColNum+j]==false)  //run inference only if mask takes effect
				{					
					dResultSim = 0;
					for(int k=0;k<m_iInstanceNum;k++)
					{
						dCurSim = getInstance(k)->getInstanceResult(dX,dY,pEnvDatabase,&iRuleID);
						if(dCurSim>=dResultSim)
						{
							dResultSim=dCurSim;
							iCurDecisiveFactor = 1;
							iCurDecisiveID = k+1;
							iCurDicisiveRule = iRuleID;
						}
					}

					bool bInOcc = false;
					for(int k =0;k<m_iOccurrenceNum;k++)
					{
						if(getOccurrence(k)->getInRange()==false)
							continue;					
						if (getOccurrence(k)->IsInRange(i,j,pEnvDatabase,&dDist))
						{
							bInOcc = true;
						    dResultSim = 0;
							dCurSim = getOccurrence(k)->getOccurrenceResult(dX,dY,pEnvDatabase,&iRuleID,dDist, this->m_vecInstance, m_iInstanceNum);
							if(dCurSim>=dResultSim)
							{
								dResultSim=dCurSim;
								iCurDecisiveFactor = 2;
								iCurDecisiveID = k+1;
								iCurDicisiveRule = iRuleID;
							}
						}
						else
						{
							continue;
						}
					}
					for(int k =0;k<m_iExclusionNum;k++)
					{
						if(getExclusion(k)->getInRange()==false)
							continue;				
						if (getExclusion(k)->IsInRange(i,j,pEnvDatabase,&dDist))
						{
						   if(bInOcc == false)
						   dResultSim = 1;
							dCurSim = getExclusion(k)->getExclusionResult(dX,dY,pEnvDatabase,&iRuleID,dDist, this->m_vecInstance, m_iInstanceNum);
							if(dCurSim<=dResultSim)
							{
								dResultSim=dCurSim;
								iCurDecisiveFactor = 3;
								iCurDecisiveID = k+1;
								iCurDicisiveRule = iRuleID;
							}
						}
						else
						{
							continue;
						}

					}
					resultBuf[i*iColNum+j]=dResultSim*100;
					chkBuf[i*iColNum+j]= iCurDecisiveFactor*100+ iCurDecisiveID+0.01*iCurDicisiveRule;

				}
				else  //if mask does not take effect, set the resul to be no data
				{
					resultBuf[i*iColNum+j]=nodata;
				}
			}
		}

	}

}



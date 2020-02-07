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
#include ".\exclusion.h"
#include "RangeRule.h"
#include "FreehandRule.h"
#include "WordRule.h"
#include "PointRule.h"
#include "EnumRule.h"


CExclusion::CExclusion(void)
{
	m_dCentralX = 0;
	m_dCentralY = 0;
	m_dSearchDist = 0 ;
	m_bUseDistSim = false;
	m_dDistDecayFactor = 1;
	m_dZFactor = 1;
	m_iRuleNum = 0;
	m_strExclusionName="Untitled_Exclusion";

}

CExclusion::CExclusion(CExclusion &anExclusion)
{
	m_iRuleNum = 0;
	m_strExclusionName = anExclusion.getExclusionName();
	m_dCentralX = anExclusion.getCentralX();
	m_dCentralY = anExclusion.getCentralY();
	m_dSearchDist = anExclusion.getSearchDist();
	m_bUseDistSim = anExclusion.getUseDistSim();
	m_dDistDecayFactor = anExclusion.getDistDecayFactor();
	m_dZFactor = anExclusion.getZFactor();


	for(int i=0;i<anExclusion.getRuleNum();i++)
	{
		if(anExclusion.getRule(i)->getRuleType()==0)
		{
			CRangeRule *tempRule = new CRangeRule(*((CRangeRule*)(anExclusion.getRule(i))));
			AddRule((CRule*)tempRule);
		}
		else if(anExclusion.getRule(i)->getRuleType()==1)
		{
			CFreehandRule *tempRule = new CFreehandRule(*((CFreehandRule*)(anExclusion.getRule(i))));
			AddRule((CRule*)tempRule);
		}
		else if(anExclusion.getRule(i)->getRuleType()==2)
		{
			CWordRule *tempRule = new CWordRule(*((CWordRule*)(anExclusion.getRule(i))));
			AddRule((CRule*)tempRule);
		}
		else if(anExclusion.getRule(i)->getRuleType()==3)
		{
			CPointRule *tempRule = new CPointRule(*((CPointRule*)(anExclusion.getRule(i))));
			AddRule((CRule*)tempRule);
		}
		else if(anExclusion.getRule(i)->getRuleType()==4)
		{
			CEnumRule *tempRule = new CEnumRule(*((CEnumRule*)(anExclusion.getRule(i))));
			AddRule((CRule*)tempRule);
		}

	} 

}

CExclusion::~CExclusion(void)
{
}

CRule* CExclusion::getRule(int iIndex)
{
	return m_vecRule[iIndex];
}



CString CExclusion::getExclusionName()
{
	return m_strExclusionName;
}
void CExclusion::setExclusionName(CString strExclusionName)
{
	m_strExclusionName = strExclusionName;
}



int CExclusion::getRuleNum()
{
	return m_iRuleNum;
}

double CExclusion::getCentralX()
{
	return m_dCentralX;
}
void CExclusion::setCentralX(double dCentralX)
{
	m_dCentralX = dCentralX;
}
double CExclusion::getCentralY()
{
	return m_dCentralY;
}
void CExclusion::setCentralY(double dCentralY)
{
	m_dCentralY = dCentralY;
}

double CExclusion::getSearchDist()
{
	return m_dSearchDist;
}
void CExclusion::setSearchDist(double dSearchDist)
{
	m_dSearchDist = dSearchDist;
}

bool CExclusion::getUseDistSim()
{
	return m_bUseDistSim;
}

void CExclusion::setUseDistSim(bool bUseDistSim)
{
	m_bUseDistSim = bUseDistSim;
}

double CExclusion::getDistDecayFactor()
{
	return m_dDistDecayFactor;
}

void CExclusion::setDistDecayFactor(double dDistDecayFactor)
{
	m_dDistDecayFactor = dDistDecayFactor;
}

double CExclusion::getZFactor()
{
	return m_dZFactor;
}

void CExclusion::setZFactor(double dZFactor)
{
	m_dZFactor = dZFactor;
}


bool CExclusion::AddRule(CRule* newRule)
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

void CExclusion::DeleteRule(int iIndex)
{
	m_vecRule.erase(m_vecRule.begin()+iIndex);
	m_iRuleNum--;
}

bool CExclusion::IsInRange(int iRow,int iCol,CEnvDatabase *pEnvDatabase, double *dDist ) 
{
	bool bFlag;
	int iCentralR = pEnvDatabase->getLayer(0)->m_pTheData->rowOfAPoint(m_dCentralY);
	int iCentralC = pEnvDatabase->getLayer(0)->m_pTheData->colOfAPoint(m_dCentralX); 
	int iIntervalR= m_dSearchDist/pEnvDatabase->m_pTheSetting->getCellSize();
	int iStartR,iStartC,iEndR,iEndC;

	iStartR=iCentralR-iIntervalR;//get the rectangle boundary of an exclusion
	if(iStartR<0)
		iStartR=0;
	iStartC=iCentralC-iIntervalR;
	if(iStartC<0) 
		iStartC=0;
	iEndR=iCentralR+iIntervalR;
	if(iEndR>pEnvDatabase->m_pTheSetting->getRowNum()-1) 
		iEndR=pEnvDatabase->m_pTheSetting->getRowNum()-1;
	iEndC=iCentralC+iIntervalR;
	if(iEndC>pEnvDatabase->m_pTheSetting->getColNum()-1) 
		iEndC=pEnvDatabase->m_pTheSetting->getColNum()-1;


	if (iEndR<iRow||iRow<iStartR || iEndC<iCol||iCol<iStartC) //check if the pixel is not in the rectangle boundary, 
	{
		bFlag=false;
	} 
	else//if the pixel is in the rectangle, then calculate the topodistance see if the pixel is within the range
	{
		double dX=pEnvDatabase->getLayer(0)->m_pTheData->xOfACell(iCol);
		double dY=pEnvDatabase->getLayer(0)->m_pTheData->yOfACell(iRow);

		if(pEnvDatabase->getDEMLayerID()!=-1)
			*dDist=topoDistanceXY(dX,dY,m_dCentralX,m_dCentralY,pEnvDatabase->getLayer(pEnvDatabase->getDEMLayerID())->m_pTheData,m_dZFactor);
		else
			*dDist = sqrt(pow((dX-m_dCentralX),2)+pow((dY-m_dCentralY),2));

	if (*dDist<=m_dSearchDist && *dDist >= 0)
			bFlag=true;	
		else
			bFlag=false ;
	}
	return bFlag;
}

// get the similarity of the central point
void CExclusion::ComputeCentralPointResult(CEnvDatabase *pEnvDatabase)
{
	if(pEnvDatabase->getLayerNum()<=0)
		return;
	int iCentralR = pEnvDatabase->getLayer(0)->m_pTheData->rowOfAPoint(m_dCentralY);
	int iCentralC = pEnvDatabase->getLayer(0)->m_pTheData->colOfAPoint(m_dCentralX); 
	double dCentralSim = 1;
	double *tempSim= new double[m_iRuleNum];

	for(int r=0;r<m_iRuleNum;r++)
	{
		if(getRule(r)->getIsUsed()==false)
			tempSim[r]=1.0;		
		else
			tempSim[r]=getRule(r)->getEvaluateResult(iCentralR,iCentralC,pEnvDatabase);
		if(dCentralSim>tempSim[r])
			dCentralSim=tempSim[r];
	}
	m_dCentralSim = dCentralSim;

}
//get the result of exclusion inference
double CExclusion::getExclusionResult(double dX, double dY,CEnvDatabase *pEnvDatabase,int *iRuleID,double dDist, vector<CInstance *> vecInstance, int iInstNum)
{   
	double dSim = 1.0;

		double fMin =1;
		for(int i=0;i<m_iRuleNum;i++)
		{	
			if(getRule(i)->getLayerID() == -1)
		    {
			  continue;
		    }
			if(getRule(i)->getIsUsed()==true)
			{
				dSim = getRule(i)->Evaluate(pEnvDatabase->getLayer(getRule(i)->getLayerID())->getData(dX,dY));
				if(dSim<fMin)
			 {
				 fMin=dSim;
				 *iRuleID = i+1;
			 }
			}
		}

		dSim = fMin;


	if (m_bUseDistSim)
	{
		int iPointR = pEnvDatabase->getLayer(0)->m_pTheData->rowOfAPoint(dY);
		int iPointC = pEnvDatabase->getLayer(0)->m_pTheData->colOfAPoint(dX); 

		int iCentralR = pEnvDatabase->getLayer(0)->m_pTheData->rowOfAPoint(m_dCentralY);
		int iCentralC = pEnvDatabase->getLayer(0)->m_pTheData->colOfAPoint(m_dCentralX); 
		//search the boundary point

		double cellSize = pEnvDatabase->m_pTheSetting->getCellSize();

		if(fabs(dY-m_dCentralY) < VERYSMALL && fabs(dX - m_dCentralX) < VERYSMALL)  //the coordinates are the same
		{
			return 1 - dSim;
		}




		double dInterval=cellSize;
		double dK=1;
		double dXStart=dX;
		double dYStart=dY;
		double xx = dX;
		double yx = dY;
		double dBoundDist = 0;
		double dBoundSim = 0;

		if (fabs(m_dCentralX-dX)>VERYSMALL)	
			dK=(dY-m_dCentralY)/(dX-m_dCentralX);
		double distToCenter = dDist;
		if(fabs(distToCenter - m_dSearchDist) > cellSize)
		{
			do
			{

				if (fabs(dX-m_dCentralX)>VERYSMALL)
				{
					if (m_dCentralX<dX)
						xx=dXStart+dInterval/sqrt(pow(dK,2)+1);
					else
						xx=dXStart-dInterval/sqrt(pow(dK,2)+1);
					if (m_dCentralY<dY)
						yx=dYStart+fabs(dK*dInterval/sqrt(pow(dK,2)+1));
					else
						yx=dYStart-fabs(dK*dInterval/sqrt(pow(dK,2)+1));
				}
				else
				{
					xx=dXStart;
					if (m_dCentralY<dY)
						yx=dYStart+dInterval;
					else
						yx=dYStart-dInterval;
				}

				if(pEnvDatabase->getDEMLayerID()!=-1)		
					distToCenter = topoDistanceXY(xx,yx,m_dCentralX,m_dCentralY,pEnvDatabase->getLayer(pEnvDatabase->getDEMLayerID())->m_pTheData,m_dZFactor);
				else
					distToCenter = sqrt(pow((xx-m_dCentralX),2)+pow((yx-m_dCentralY),2));
				if(distToCenter == -1) //out of boundary
				{
					distToCenter = topoDistanceXY(dXStart,dYStart,m_dCentralX,m_dCentralY,pEnvDatabase->getLayer(pEnvDatabase->getDEMLayerID())->m_pTheData,m_dZFactor);
					xx = dXStart;
					yx = dYStart;
					break;
				}
				else if(distToCenter == -2)
				{
					distToCenter = topoDistanceXY(dXStart,dYStart,m_dCentralX,m_dCentralY,pEnvDatabase->getLayer(pEnvDatabase->getDEMLayerID())->m_pTheData,m_dZFactor);
					xx = dXStart;
					yx = dYStart;
					break;
				}
				else if(fabs(distToCenter - m_dSearchDist) <= cellSize)
					break;
				dXStart=xx;
				dYStart=yx;
			}
			while(true);


		}

		dBoundSim = 0;
		for(int k=0;k<iInstNum;k++)
		{
			int ruleID;
			double dCurSim = vecInstance[k]->getInstanceResult(xx,yx,pEnvDatabase,&ruleID);
			if(dCurSim>=dBoundSim)  //if current similarity is greater than existing one
			{						
				dBoundSim=dCurSim;						
			}
		}

		dBoundDist = distToCenter - dDist;

		if(dDist > 0 && dBoundDist > 0)		
			dSim=distSimilarity(1-dSim,dDist,dBoundSim, dBoundDist,m_dDistDecayFactor);	
		else if(dDist == 0)
			dSim = 1-dSim;
		else if(dBoundDist == 0)
			dSim =  dBoundSim;

	}

	return dSim;
}




double  CExclusion::topoDistanceXY(double dX1, double dY1, double dX2, double dY2,CThreeDR *theDEM, 
									double dZFactor, double dCellFraction)
{
	double dDist=0;
	float fNoData = theDEM->getNullDataValue();
	int iR1 = theDEM->rowOfAPoint(dY1);
	int iC1 = theDEM->colOfAPoint(dX1);
	int iR2 = theDEM->rowOfAPoint(dY2);
	int iC2 = theDEM->colOfAPoint(dX2);
	if(iR1<0 || iR1+1>theDEM->getNumberOfRows() || iR2<0 || iR2+1>theDEM->getNumberOfRows()
		|| iC1<0 || iC1+1>theDEM->getNumberOfColumns() || iC2<0||iC2+1>theDEM->getNumberOfRows())
	{
		return -1;
	}
	double dH1=theDEM->getData(iR1,iC1);
	double dH2=theDEM->getData(iR2,iC2);	

	double dInterval=theDEM->getCellSize()*dCellFraction;
	int iStep=sqrt(pow((dX1-dX2),2)+pow((dY1-dY2),2))/dInterval;
	double dK=1;

	double dXStart=dX1;
	double dYStart=dY1;
	double dHStart=dH1;
	double xx;
	double yx;

	//if (sqrt(pow((dX1-dX2),2)+pow((dY1-dY2),2))>fThreshold)
	//	return -1;
	if(fabs(dH1-fNoData)<VERYSMALL || fabs(dH2-fNoData)<VERYSMALL)//?"="
		return -2;



	if (fabs(dX2-dX1)>VERYSMALL)
		dK=(dY2-dY1)/(dX2-dX1);
	for (int i=0;i<iStep;i++)
	{
		if (fabs(dX2-dX1)>VERYSMALL)
		{
			if (dX1<dX2)
				xx=dXStart+dInterval/sqrt(pow(dK,2)+1);
			else
				xx=dXStart-dInterval/sqrt(pow(dK,2)+1);
			if (dY1<dY2)
				yx=dYStart+fabs(dK*dInterval/sqrt(pow(dK,2)+1));
			else
				yx=dYStart-fabs(dK*dInterval/sqrt(pow(dK,2)+1));
		}
		else
		{
			xx=dXStart;
			if (dY1<dY2)
				yx=dYStart+dInterval;
			else
				yx=dYStart-dInterval;
		}

		int row = theDEM->rowOfAPoint(yx);
		int col = theDEM->colOfAPoint(xx);
		double h=theDEM->getData(row,col);

		//dDist+=sqrt(pow((dXStart-dX2),2),pow((dYStart-dY2),2)+pow((dHstart-dH2)*fZFactor,2));
		dDist+=sqrt(pow((dXStart-xx),2)+pow((dYStart-yx),2)+pow((dHStart-h)*dZFactor,2));
		dXStart=xx;
		dYStart=yx;
		dHStart=h;
	}
	return dDist;
}
double CExclusion::distSimilarity(double dSim,double dDist,double dBoundSim, double dBoundDist,double dDistDecayFactor)
{
	float w1 = pow(dDist, -dDistDecayFactor);
	float w2 = pow(dBoundDist, -dDistDecayFactor);
	return (dSim * w1 +dBoundSim * w2) / (w1 + w2);
	///double a = exp(pow(dDist/dSearchDist, dDistDecayFactor)*log(VERYSMALL));
	//return dSim*exp(pow(dDist/dSearchDist, dDistDecayFactor)*log(VERYSMALL));
}	

void CExclusion::setInRange(bool bInRange)
{
	m_bInRange=bInRange;
}

bool CExclusion::getInRange()
{
	return m_bInRange;
}





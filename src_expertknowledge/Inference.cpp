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
#include ".\inference.h"
#include "ThreeDR.h"

#include "SoLIM.h"
#include "MainFrm.h"
#include "SoLIMDoc.h"

#include "InferenceView.h"

CInference::CInference(void)
{	
	m_bCreateCheckFile = false;
	m_bUseMask = false;
	m_strMaskFile = "";
	m_strResultDir= "";
	m_pEnvDatabase = NULL;
	m_pKnowledgeBase = NULL;
   	m_iMaskValue = 0;
	nodata = -9999;
}

CInference::CInference(CInference &anInference)
{
	m_bCreateCheckFile = anInference.getCreateCheckFile();
	m_bUseMask = anInference.getUseMask();
	m_strMaskFile = anInference.getMaskFile();
	m_strResultDir= anInference.getResultDir();
	m_iMaskValue = anInference.getMaskValue();
	nodata = -9999;

}

CInference::~CInference(void)
{
}

void CInference::setEnvDatabase(CEnvDatabase *pEnvDatabase)
{
	m_pEnvDatabase = pEnvDatabase;
}

void CInference::setKnowledgeBase(CKnowledgeBase *pKnowledgeBase)
{
	m_pKnowledgeBase = pKnowledgeBase;
}

bool CInference::getCreateCheckFile()
{
	return m_bCreateCheckFile;
}
void CInference::setCreateCheckFile(bool bCreateCheckFile)
{
	m_bCreateCheckFile = bCreateCheckFile;
}
bool CInference::getUseMask()
{
	return m_bUseMask;
}
void CInference::setUseMask(bool bUseMask)
{
	m_bUseMask = bUseMask;
}
CString CInference::getResultDir()
{
	return m_strResultDir;
}
void CInference::setResultDir(CString strResultDir)
{
	m_strResultDir = strResultDir;
}

CString CInference::getMaskFile()
{
	return m_strMaskFile;
}
void CInference::setMaskFile(CString strMaskFile)
{
	m_strMaskFile = strMaskFile;
}

int CInference::getMaskValue()
{
	return m_iMaskValue;
}
void CInference::setMaskValue(int iMaskValue)
{
	m_iMaskValue = iMaskValue;	
}

void CInference::InferSoil()
{	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());

	CInferenceView *pInferenceView = (CInferenceView *)(pFrame->m_splitterTabWnd.GetPane(0,0));	
	int iRowNum=m_pEnvDatabase->m_pTheSetting->getRowNum();
	int iColNum=m_pEnvDatabase->m_pTheSetting->getColNum();
	int iSoilTypeNum=m_pKnowledgeBase->getSoilTypeNum();

	//set mask
	bool *maskBuf;
	if(m_bUseMask==true)
	{  
		CThreeDR *pMaskData = new CThreeDR;
		if(pMaskData->readin(m_strMaskFile.GetBuffer(1), 0)!=0)
		{
			AfxMessageBox("Reading mask file error. The mask will not be used.");
			m_bUseMask = false;
			maskBuf=NULL;
		}
		else if(pMaskData->getNumberOfColumns()!=iColNum||pMaskData->getNumberOfRows()!=iRowNum)
		{
			AfxMessageBox("The number of row or column of mask file do not match with the database.The mask will not be used.");
			m_bUseMask = false;
			maskBuf=NULL;
		}
		else
		{
			maskBuf = new bool[iRowNum*iColNum];
			for(int i=0;i<iRowNum;i++)
				for(int j=0;j<iColNum;j++)
				{
					if(fabs(pMaskData->getData(i,j)-m_iMaskValue)<VERYSMALL)
						maskBuf[i*iColNum+j]=true;
					else
						maskBuf[i*iColNum+j]=false;
				}
		}
	}
	else
	{
		maskBuf=NULL;
	}

    //loop to infer every soil type
	for(int k=0;k<m_pKnowledgeBase->getSoilTypeNum();k++)
	{
		pInferenceView->m_ProgressBar.SetPos(100*k/m_pKnowledgeBase->getSoilTypeNum());
		if(m_pKnowledgeBase->getSoilType(k)->getIsUsed()==false)  //if the soil type is not checked
		{
			continue;
		}
		//initialize the result buffer
		float *simArray = new float[iRowNum*iColNum];
		for(int i=0;i<iRowNum;i++)
			for(int j=0;j<iColNum;j++)
				simArray[i*iColNum+j] = 0;

		//set chkfile buffer    
		float *chkBuf;
		chkBuf = new float[iRowNum*iColNum];
		for(int i=0;i<iRowNum;i++)
			for(int j=0;j<iColNum;j++)
				chkBuf[i*iColNum+j]=nodata; 

		//if a check value is 101.01, 100 means it is decided by instances, 200 means it is decided by occurrences, 300 means it is decied by exclusions
		//1 is the id of instance/occurrece/exclusion
		//0.01 indicate the rule id
	
        //call this function to do actual inference
		m_pKnowledgeBase->getSoilType(k)->getInferenceResult(simArray,m_pEnvDatabase,m_bUseMask,maskBuf,m_bCreateCheckFile,chkBuf,&(pInferenceView->m_ProgressBar),k,m_pKnowledgeBase->getSoilTypeNum());

		//write the result to 3dr file
		CString strOutputFileName = m_strResultDir+"\\"+m_pKnowledgeBase->getSoilType(k)->getSoilTypeName()+".3dr";
		CThreeDR* output = new CThreeDR;
		output->setNumberOfRows(iRowNum);
		output->setNumberOfColumns(iColNum);
		output->setXmin(m_pEnvDatabase->m_pTheSetting->getWest());
		output->setYmin(m_pEnvDatabase->m_pTheSetting->getSouth());
		output->setCellSize(m_pEnvDatabase->m_pTheSetting->getCellSize());
		output->setNullDataValue(nodata);
		output->setDataName(extractFileName(strOutputFileName).GetBuffer(1));
		//output->setDataUnits(m_pEnvDatabase->m_pTheSetting->getDataUnit().GetBuffer());
		output->setGridUnits(m_pEnvDatabase->m_pTheSetting->getGridUnit().GetBuffer());
		output->setDataBuf(simArray);
		output->calcStat();
		output->writeout(strOutputFileName.GetBuffer(),0);
		delete output;			

		//if the user want ti create check file
		if(m_bCreateCheckFile==true)
		{
			strOutputFileName = m_strResultDir+"\\"+m_pKnowledgeBase->getSoilType(k)->getSoilTypeName()+"_chk.3dr";
			output = new CThreeDR;
			output->setNumberOfRows(iRowNum);
			output->setNumberOfColumns(iColNum);
			output->setXmin(m_pEnvDatabase->m_pTheSetting->getWest());
			output->setYmin(m_pEnvDatabase->m_pTheSetting->getSouth());
			output->setCellSize(m_pEnvDatabase->m_pTheSetting->getCellSize());
			output->setNullDataValue(nodata);
			output->setDataName(extractFileName(strOutputFileName).GetBuffer(1));
			output->setDataUnits(m_pEnvDatabase->m_pTheSetting->getDataUnit().GetBuffer());
			output->setGridUnits(m_pEnvDatabase->m_pTheSetting->getGridUnit().GetBuffer());
			output->setDataBuf(chkBuf);
			output->calcStat();
			output->writeout(strOutputFileName.GetBuffer(),0);
			delete output;			
		}

	}

	//destroy mask buffer
	if(maskBuf!=NULL)
	{
		delete []maskBuf;
		maskBuf = NULL;
	}

	pInferenceView->m_ProgressBar.SetPos(100);
}

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

#include  "stdafx.h"
#include "EnvDatabase.h"
#include ".\envdatabase.h"

CEnvDatabase::CEnvDatabase()
{
	m_iLayerNum = 0;
	m_pTheSetting = NULL;
	m_iDEMLayerID = -1;
	m_bIsEditVersion = false;
	m_pBaseEnvDatabase = NULL;
}

//copy construction function
CEnvDatabase::CEnvDatabase(CEnvDatabase & anEnvDatabase)
{
	m_iLayerNum = 0;

	if(anEnvDatabase.m_pTheSetting!=NULL)
		m_pTheSetting = new CSetting(*(anEnvDatabase.m_pTheSetting));
	else
		m_pTheSetting = NULL;	

	for(int i=0;i<anEnvDatabase.m_iLayerNum;i++)
	{
		CEnvLayer *pTempLayer = new CEnvLayer(*(anEnvDatabase.getLayer(i)));
		AddLayer(pTempLayer);
	}
}


void CEnvDatabase::CopyEnvDatabase(CEnvDatabase *anEnvDatabase, bool bIsEditVersion)  
{
	if(anEnvDatabase->m_pTheSetting!=NULL)
		m_pTheSetting = new CSetting(anEnvDatabase->m_pTheSetting);
	else
		m_pTheSetting = NULL;

	m_bIsEditVersion = bIsEditVersion;

	m_iDEMLayerID = anEnvDatabase->m_iDEMLayerID;

	if(m_bIsEditVersion == true)   //if this is edit version
	{
		m_iLayerNum = 0;
		m_pBaseEnvDatabase = anEnvDatabase;  //get base database pointer
		
		for(int i=0;i<anEnvDatabase->m_iLayerNum;i++)
		{
			m_vecEnvLayer.push_back(anEnvDatabase->getLayer(i));
			m_iLayerNum++; 

		}
	}
	else    //this is the base database
	{
		for(int i=0;i<m_iLayerNum;i++)
		{
			bool tagDelete=true;
			//to find if the layer in base database is the same with edit version, if so, preserve it
			//otherwise, delete it 
			//this can synchronize the base database and edit version.
			for(int j=0;j<anEnvDatabase->m_iLayerNum;j++)  
			{
				if(anEnvDatabase->getLayer(j)->getLayerID()==getLayer(i)->getLayerID())
				{ 
					tagDelete = false;
					break;
				}
			}
			if(tagDelete==true)
			{
				DeleteLayer(i);
				i--;
			}

		}
		//re-set layer id(s) since many layers may be deleted
		for(int i=0;i<m_iLayerNum;i++)
		{
			getLayer(i)->setLayerID(i);
		}
		//delete edit version and copy the base version to edit version 
		anEnvDatabase->DeleteAll();        
		for(int i=0;i<m_iLayerNum;i++)
		{
			anEnvDatabase->m_vecEnvLayer.push_back(getLayer(i));
			anEnvDatabase->setLayerNum(i+1); 
		}


	}
}
CEnvDatabase::~CEnvDatabase()
{
	if(m_pTheSetting != NULL)
	{
		delete m_pTheSetting;
		m_pTheSetting = NULL;
	}

	if(m_bIsEditVersion==true)  //if this is edit version, only remove layers, do not destroy them
	{
		while(m_iLayerNum != 0)
		{
			m_vecEnvLayer.pop_back();
			m_iLayerNum--;
		}
	}

	if(m_bIsEditVersion==false)  //if this is base version, destroy layers
	{
		while(m_iLayerNum != 0)
		{
			CEnvLayer *pTempLayer = m_vecEnvLayer[m_iLayerNum-1];
			m_vecEnvLayer.pop_back();
			if(pTempLayer != NULL)
			{
				delete pTempLayer;
				pTempLayer = NULL;
			}
			m_iLayerNum--;
		}
	}    
}

BOOL CEnvDatabase::AddLayer(CEnvLayer* pLayer)
{
	if(pLayer->getLayerName() == "elevation" ||pLayer->getLayerName() == "Elevation" ||pLayer->getLayerName() == "ELEVATION")
		m_iDEMLayerID = m_iLayerNum;

	if(m_iLayerNum == 0)
	{
		m_pTheSetting = new CSetting(pLayer->getFileName());

	}
	else
	{
		/*if(fabs(pLayer->m_pTheData->getCellSize()-m_pTheSetting->getCellSize())> VERYSMALL)
		{ 
			AfxMessageBox("The cell size of the new layer does not match the first layer.The layer will not be added.");
			return false ;
		}*/

		double prevWest,prevEast,prevSouth,prevNorth;	
		prevWest = m_pTheSetting->getWestMin();
		prevEast = m_pTheSetting->getEastMin();
		prevSouth = m_pTheSetting->getSouthMin();
		prevNorth = m_pTheSetting->getNorthMin();

		if(m_pTheSetting->getWestMin()< pLayer->m_pTheData->getXmin())			
			m_pTheSetting->setWestMin(pLayer->m_pTheData->getXmin());

		if(m_pTheSetting->getEastMin() > pLayer->m_pTheData->getXmin()+pLayer->m_pTheData->getCellSize()*pLayer->m_pTheData->getNumberOfColumns())		
			m_pTheSetting->setEastMin(pLayer->m_pTheData->getXmin()+pLayer->m_pTheData->getCellSize()*pLayer->m_pTheData->getNumberOfColumns());

		if(m_pTheSetting->getSouthMin() < pLayer->m_pTheData->getYmin())			
			m_pTheSetting->setSouthMin(pLayer->m_pTheData->getYmin());

		if(m_pTheSetting->getNorthMin() > pLayer->m_pTheData->getYmin()+pLayer->m_pTheData->getCellSize()*pLayer->m_pTheData->getNumberOfRows())
			m_pTheSetting->setNorthMin(pLayer->m_pTheData->getYmin()+pLayer->m_pTheData->getCellSize()*pLayer->m_pTheData->getNumberOfRows());

		if(m_pTheSetting->getWestMin() >= m_pTheSetting->getEastMin() || m_pTheSetting->getSouthMin() > m_pTheSetting->getNorthMin())
		{
			//return to previous state
			m_pTheSetting->setWestMin(prevWest);
			m_pTheSetting->setEastMin(prevEast);
			m_pTheSetting->setSouthMin(prevSouth);
			m_pTheSetting->setNorthMin(prevNorth);
			AfxMessageBox("The spatial extent of the new layer does not have overlap with the previous layers. The layer will not be added.");
			return false;
		}
	}
	pLayer->m_pTheData->calcStat();
	if(m_bIsEditVersion==true)
	{
		m_pBaseEnvDatabase->m_vecEnvLayer.push_back(pLayer);       		
		m_pBaseEnvDatabase->m_iLayerNum++; 
		m_vecEnvLayer.push_back(pLayer);
		m_iLayerNum++; 
	}
	else
	{
		m_vecEnvLayer.push_back(pLayer);
		m_iLayerNum++; 
	}

	return true;
}


void CEnvDatabase::DeleteLayer(int iIndex)
{
	//if the deleleted layer is the first layer, reset the setting
	if(iIndex ==0&&m_iLayerNum>1)
	{
		delete m_pTheSetting;
		m_pTheSetting = NULL;		
		CEnvLayer *pFirstLayer = m_vecEnvLayer[1];
		m_pTheSetting = new CSetting(pFirstLayer->getFileName());
	}


	if(m_bIsEditVersion==true)
	{
		m_vecEnvLayer.erase(m_vecEnvLayer.begin()+iIndex);
		m_iLayerNum--;
	}

	else
	{
		CEnvLayer *pTempLayer = m_vecEnvLayer[iIndex];
		if(pTempLayer != NULL)
		{
			delete pTempLayer;
			pTempLayer = NULL;
		}
		m_vecEnvLayer.erase(m_vecEnvLayer.begin()+iIndex);
		m_iLayerNum--;

	}

	if(m_iLayerNum == 0)
	{
		delete m_pTheSetting;
		m_pTheSetting = NULL;
		return;
	}


	//recalculate the bounding box
	m_pTheSetting->setWest(m_vecEnvLayer[0]->m_pTheData->getXmin());
	m_pTheSetting->setEast(m_pTheSetting->getWest()+m_vecEnvLayer[0]->m_pTheData->getCellSize()*m_vecEnvLayer[0]->m_pTheData->getNumberOfColumns());
	m_pTheSetting->setSouth(m_vecEnvLayer[0]->m_pTheData->getYmin());
	m_pTheSetting->setNorth(m_pTheSetting->getSouth()+m_vecEnvLayer[0]->m_pTheData->getCellSize()*m_vecEnvLayer[0]->m_pTheData->getNumberOfRows());

	m_pTheSetting->setColNum(m_vecEnvLayer[0]->m_pTheData->getNumberOfColumns());
	m_pTheSetting->setRowNum(m_vecEnvLayer[0]->m_pTheData->getNumberOfRows());
	m_pTheSetting->setCellSize(m_vecEnvLayer[0]->m_pTheData->getCellSize());

	CString gridUnit,dataUnit;
	gridUnit.Format("%s",m_vecEnvLayer[0]->m_pTheData->getGridUnits());
	dataUnit.Format("%s",m_vecEnvLayer[0]->m_pTheData->getDataUnits());
	m_pTheSetting->setGridUnit(gridUnit);
	m_pTheSetting->setDataUnit(dataUnit);

	m_pTheSetting->setNodata(m_vecEnvLayer[0]->m_pTheData->getNullDataValue());
	m_pTheSetting->setWestMin(m_vecEnvLayer[0]->m_pTheData->getXmin());
	m_pTheSetting->setEastMin(m_pTheSetting->getWest()+m_vecEnvLayer[0]->m_pTheData->getCellSize()*m_vecEnvLayer[0]->m_pTheData->getNumberOfColumns());
	m_pTheSetting->setSouthMin(m_vecEnvLayer[0]->m_pTheData->getYmin());
	m_pTheSetting->setNorthMin(m_pTheSetting->getSouth()+m_vecEnvLayer[0]->m_pTheData->getCellSize()*m_vecEnvLayer[0]->m_pTheData->getNumberOfRows());


	for(int i=1; i<m_iLayerNum; i++)
	{
		if(m_pTheSetting->getWestMin()< m_vecEnvLayer[i]->m_pTheData->getXmin())
			m_pTheSetting->setWestMin( m_vecEnvLayer[i]->m_pTheData->getXmin());
		if(m_pTheSetting->getEastMin() > m_vecEnvLayer[i]->m_pTheData->getXmin()+m_vecEnvLayer[i]->m_pTheData->getCellSize()*m_vecEnvLayer[i]->m_pTheData->getNumberOfColumns())
			m_pTheSetting->setEastMin(m_vecEnvLayer[i]->m_pTheData->getXmin()+m_vecEnvLayer[i]->m_pTheData->getCellSize()*m_vecEnvLayer[i]->m_pTheData->getNumberOfColumns());
		if(m_pTheSetting->getSouthMin() < m_vecEnvLayer[i]->m_pTheData->getYmin())
			m_pTheSetting->setSouthMin(m_vecEnvLayer[i]->m_pTheData->getYmin());
		if(m_pTheSetting->getNorthMin() > m_vecEnvLayer[i]->m_pTheData->getYmin()+m_vecEnvLayer[i]->m_pTheData->getCellSize()*m_vecEnvLayer[i]->m_pTheData->getNumberOfRows())
			m_pTheSetting->setNorthMin( m_vecEnvLayer[i]->m_pTheData->getYmin()+m_vecEnvLayer[i]->m_pTheData->getCellSize()*m_vecEnvLayer[i]->m_pTheData->getNumberOfRows());
	}

}

void CEnvDatabase::DeleteAll()
{
	if(m_bIsEditVersion==true)
	{
		while(m_iLayerNum != 0)
		{
			m_vecEnvLayer.pop_back();
			m_iLayerNum--;
		}
	}
	else
	{
		while(m_iLayerNum != 0)
		{
			CEnvLayer *pTempLayer = m_vecEnvLayer[m_iLayerNum-1];
			m_vecEnvLayer.pop_back();
			if(pTempLayer != NULL)
			{
				delete pTempLayer;
				pTempLayer = NULL;
			}
			m_iLayerNum--;
		}
	}

}


int CEnvDatabase::getLayerNum()
{
	return m_iLayerNum;
}

void CEnvDatabase::setLayerNum(int iLayerNum)
{
	m_iLayerNum = iLayerNum;
}


CEnvLayer* CEnvDatabase::getLayer(int iIndex)
{
	if(iIndex>=0&&iIndex<m_iLayerNum)
		return m_vecEnvLayer[iIndex];
	else
		return NULL;
}

int CEnvDatabase::getDEMLayerID()
{
	return m_iDEMLayerID;
}


CEnvLayer*  CEnvDatabase::FindDataLayer(CString strLayerName)
{
	int iLayerNum = getLayerNum();
	for(int i=0; i<iLayerNum; i++)
	{
		if(getLayer(i)->getLayerName() == strLayerName)
		{
			return getLayer(i);
			break;
		}
	}
	if(i==iLayerNum)
		return NULL;

	return NULL;

}

int CEnvDatabase::FindDataLayerID(CString strLayerName)
{
	int iLayerNum = getLayerNum();
	for(int i=0; i<iLayerNum; i++)
	{
		if(getLayer(i)->getLayerName() == strLayerName)
		{
			return i;
			break;
		}
	}
	if(i==iLayerNum)
		return -1;

	return -1;

}


void CEnvDatabase::swapLayer(int iIndex1, int iIndex2)
{
	if(iIndex1>=this->getLayerNum()||iIndex2>=this->getLayerNum())
		return;
	CEnvLayer *layer1 = this->getLayer(iIndex1);
	CEnvLayer *layer2 = this->getLayer(iIndex2);

		  this->m_vecEnvLayer.erase(m_vecEnvLayer.begin()+iIndex1);
	  this->m_vecEnvLayer.insert(m_vecEnvLayer.begin()+iIndex1, layer2);
	  this->m_vecEnvLayer.erase(m_vecEnvLayer.begin()+iIndex2);
	  this->m_vecEnvLayer.insert(m_vecEnvLayer.begin()+iIndex2, layer1);
	
/*

	if(iIndex1 < iIndex2)
	{ 
	  this->m_vecEnvLayer.erase(m_vecEnvLayer.begin()+iIndex1);
	  this->m_vecEnvLayer.insert(m_vecEnvLayer.begin()+iIndex1, layer2);
	  this->m_vecEnvLayer.erase(m_vecEnvLayer.begin()+iIndex2);
	  this->m_vecEnvLayer.insert(m_vecEnvLayer.begin()+iIndex2, layer1);
	}
	else
		{
	  this->m_vecEnvLayer.insert(m_vecEnvLayer.begin()+iIndex2, layer1);
	  this->m_vecEnvLayer.insert(m_vecEnvLayer.begin()+iIndex1, layer2);
	}*/


}



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
#include ".\envlayer.h"


CEnvLayer::CEnvLayer()
{
	m_iLayerID = -1;
	m_strLayerName = "";
	m_strFileName = "";
	m_pTheData = NULL;
}

//copy construction function
CEnvLayer::CEnvLayer(CEnvLayer &anEnvLayer) 
{
	m_iLayerID = anEnvLayer.getLayerID();
	m_strLayerName = anEnvLayer.getLayerName();
	m_strFileName = anEnvLayer.getFileName();
	m_pTheData = NULL;
	m_pTheData = new CThreeDR(*(anEnvLayer.m_pTheData),true);
}


CEnvLayer::CEnvLayer(int layerID, CString strLayerName, CString strFileName)
{
	m_iLayerID = layerID;
	m_strLayerName = strLayerName;
	m_strFileName = strFileName;
	m_pTheData = new CThreeDR;
	if(m_pTheData->readin(strFileName.GetBuffer(1), 0)!=0)
		m_iLayerID = -1;

}


CEnvLayer::~CEnvLayer()
{
	if(NULL != m_pTheData)
	{
		delete m_pTheData;
		m_pTheData = NULL;
	}
}

void CEnvLayer::setLayerID(int iLayerID)
{
	m_iLayerID = iLayerID;
}

int CEnvLayer::getLayerID()
{
	return m_iLayerID;
}

void CEnvLayer::setLayerName(CString strLayerName)
{
	m_strLayerName = strLayerName;
}

CString CEnvLayer::getLayerName()
{
	return m_strLayerName;
}

void CEnvLayer::setFileName(CString strFileName)
{
	m_strFileName = strFileName;
}

CString CEnvLayer::getFileName()
{
	return m_strFileName;
}

int CEnvLayer::getRow()
{
	return m_pTheData->getNumberOfRows();
}

int CEnvLayer::getCol()
{
	return m_pTheData->getNumberOfColumns();
}

float CEnvLayer::getData(int iRow, int iCol)
{
	return m_pTheData->getData(iRow,iCol);
}

float CEnvLayer::getData(double x,double y)
{
	return m_pTheData->getData(x,y);	
}
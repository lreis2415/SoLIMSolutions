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
#include "Setting.h"
#include "ThreeDR.h"

//The Setting class
CSetting::CSetting(CString aDataFile)
{
	CThreeDR* theData = new CThreeDR;
	theData->readin(aDataFile.GetBuffer(1), 0);
	m_iColNum = theData->getNumberOfColumns();
	m_iRowNum = theData->getNumberOfRows();
	m_fCellSize = theData->getCellSize(); 
	m_dWest = theData->getXmin();
	m_dEast = m_dWest+m_fCellSize*m_iColNum;
	m_dSouth = theData->getYmin();
	m_dNorth = m_dSouth+m_fCellSize*m_iRowNum;
	m_dWestMin = theData->getXmin();
	m_dEastMin = m_dWest+m_fCellSize*m_iColNum;
	m_dSouthMin = theData->getYmin();
	m_dNorthMin = m_dSouth+m_fCellSize*m_iRowNum;
	m_strGridUnit.Format("%s",theData->getGridUnits());
	m_strDataUnit.Format("%s",theData->getDataUnits());
	m_fNodata = theData->getNullDataValue();
	delete theData;
}

CSetting::CSetting(CSetting *aSetting)
{
	m_iColNum = aSetting->getColNum();
	m_iRowNum = aSetting->getRowNum();
	m_fCellSize = aSetting->getCellSize(); 
	m_dWest = aSetting->getWest();
	m_dEast = aSetting->getEast();
	m_dNorth = aSetting->getNorth();
	m_dSouth = aSetting->getSouth();
	m_dWestMin = aSetting->getWestMin();
	m_dEastMin = aSetting->getEastMin();
	m_dNorthMin = aSetting->getNorthMin();
	m_dSouthMin = aSetting->getSouthMin();
	m_strGridUnit = aSetting->getGridUnit();
	m_strDataUnit = aSetting->getDataUnit();	
	m_fNodata = aSetting->getNodata();
}
CSetting::~CSetting()
{
}

int CSetting::getColNum()
{
	return m_iColNum;
}
void CSetting::setColNum(int iColNum)
{
	m_iColNum = iColNum;
}
int CSetting::getRowNum()
{
	return m_iRowNum;
}
void CSetting::setRowNum(int iRowNum)
{
	m_iRowNum = iRowNum;
}
double CSetting::getWest()
{
	return m_dWest;
}
void CSetting::setWest(double dWest)
{
	m_dWest = dWest;
}
double CSetting::getEast()
{
	return m_dEast;
}
void CSetting::setEast(double dEast)
{
	m_dEast = dEast;
}
double CSetting::getSouth()
{
	return m_dSouth;
}
void CSetting::setSouth(double dSouth)
{
	m_dSouth = dSouth;
}
double CSetting::getNorth()
{
	return m_dNorth;
}
void CSetting::setNorth(double dNorth)
{
	m_dNorth = dNorth;
}
double CSetting::getCellSize()
{
	return m_fCellSize;
}
void CSetting::setCellSize(float fCellSize)
{
	m_fCellSize = fCellSize;
}
CString CSetting::getGridUnit()
{
	return m_strGridUnit;
}
void CSetting::setGridUnit(CString strGridUnit)
{
	m_strGridUnit = strGridUnit;
}
CString CSetting::getDataUnit()
{
	return m_strDataUnit;
}
void CSetting::setDataUnit(CString strDataUnit)
{
	m_strDataUnit = strDataUnit;
}
float CSetting::getNodata()
{
	return m_fNodata;
}
void CSetting::setNodata(float fNodata)
{
	m_fNodata = fNodata;
}
double CSetting::getWestMin()
{
	return m_dWestMin;
}
void CSetting::setWestMin(double dWestMin)
{
	m_dWestMin = dWestMin;
}
double CSetting::getEastMin()
{
	return m_dEastMin;
}
void CSetting::setEastMin(double dEastMin)
{
	m_dEastMin = dEastMin;
}
double CSetting::getSouthMin()
{
	return m_dSouthMin;
}
void CSetting::setSouthMin(double dSouthMin)
{
	m_dSouthMin = dSouthMin;
}
double CSetting::getNorthMin()
{
	return m_dNorthMin;
}
void CSetting::setNorthMin(double dNorthMin)
{
	m_dNorthMin = dNorthMin;
}

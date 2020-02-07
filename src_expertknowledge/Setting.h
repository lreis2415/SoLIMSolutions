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
#pragma once


//this class record basic setting of env database
class CSetting
{
private:
	int m_iColNum;
	int m_iRowNum;
	double m_dWest;
	double m_dEast;
	double m_dSouth;
	double m_dNorth;
	float m_fCellSize; 
	CString m_strGridUnit;
	CString m_strDataUnit;
	float m_fNodata;
    double m_dWestMin;
	double m_dEastMin;
	double m_dSouthMin;
	double m_dNorthMin;


public:
	int getColNum();
	void setColNum(int iColNum);
	int getRowNum();
	void setRowNum(int iRowNum);
	double getWest();
	void setWest(double dWest);
	double getEast();
	void setEast(double dEast);
	double getSouth();
	void setSouth(double dSouth);
	double getNorth();
	void setNorth(double dNorth);
	double getCellSize();
	void setCellSize(float fCellSize);
	CString getGridUnit();
	void setGridUnit(CString strGridUnit);
	CString getDataUnit();
	void setDataUnit(CString strDataUnit);
	float getNodata();
	void setNodata(float fNodataMin);
	double getWestMin();
	void setWestMin(double dWestMin);
	double getEastMin();
	void setEastMin(double dEastMin);
	double getSouthMin();
	void setSouthMin(double dSouthMin);
	double getNorthMin();
	void setNorthMin(double dNorthMin);


public:
	CSetting(CString dataFile);
	CSetting(CSetting* setting);
	~CSetting();
};
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
//ThreeDM.h
//header file of tools for handling 3dm file.
//written by Xun Shi
//Last modified: Aug 6, 2001
#pragma once
#include "ThreeDR.h"
#include "utility.h"

class CThreeDM{
private:
	//fields of 3dm file
	int NumberOfRecords;
	int NumberOfColumns;
	int NumberOfRows;
	char FileType[STRLEN];
	char GridUnits[STRLEN];
	double Xmin;
	double Ymin;
	double CellSize;
	char DataUnits[STRLEN];
	float DataMin;
	float DataMax;
	float NullValue;
	float OriginalNullValue;
	int FirstDataByte;
	float* DataBuf;
	unsigned char* PhotoBuf;

public:
	CThreeDM();
	CThreeDM(const CThreeDM& aThreeDR);
	~CThreeDM();

	//read and write
	int readin(char* inputFile); 
	int writeout(char* outputFile); 
	void copyBasicFields(CThreeDM* aThreeDM); //copy those basic header fields from an old 
											//3dm to a new 3dm.
	void calcStat();						//calc values for the statistical fields
	int rowOfAPoint(double y);    //find out the row of a point, 
											//but the user needs to check if the result is within the range
	int colOfAPoint(double x);    //find out the col of a point
											//but the user needs to check if the result is within the range
	double xOfACell(int col);		//find out the x of a cell
	double yOfACell(int row);		//find out the y of a cell
	bool spatialRangeMatch(CThreeDM* anotherThreeDM);
	bool spatialRangeMatch(CThreeDR* anotherThreeDM);
	void flush();							//reset the object.
	
	//get field values
	int getNumberOfRecords();
	int getNumberOfColumns();
	int getNumberOfRows();
	char* getFileType();
	char* getGridUnits();
	double getXmin();
	double getYmin();
	double getCellSize();
	char* getDataUnits();
	float getDataMin();
	float getDataMax();
	float getNullValue();
	float getOriginalNullValue();
	int getFirstDataByte();
	float* getDataBuf();
	unsigned char* getPhotoBuf();

	//set field values
	void setNumberOfRecords(int aNumberOfRecords);
	void setNumberOfColumns(int aNumberOfColumns);
	void setNumberOfRows(int aNumberOfRows);
	void setFileType(char* aFileType);
	void setGridUnits(char* aGridUnits);
	void setXmin(double anXmin);
	void setYmin(double aYmin);
	void setCellSize(float aCellSize);
	void setDataUnits(char* aDataUnits);
	void setDataMin(float aDataMin);
	void setDataMax(float aDataMax);
	void setNullValue(float aNullDataValue);
	void setOriginalNullValue(float aNullDataValue);
	void setFirstDataByte(int aFirstDataByte);
	void setDataBuf(float* aDataBuf);
	void setPhotoBuf(unsigned char* aPhotoBuf);
};



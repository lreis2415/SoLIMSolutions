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

#include "utility.h"


class CThreeDR{
private:
	//fields of 3dr file
	int NumberOfRecords;
	int NumberOfColumns;
	int NumberOfRows;
	char FileType[STRLEN];
	char DataName[STRLEN];
	char BaseFilename[STRLEN];
	char GridUnits[STRLEN];
	double Xmin;
	double Ymin;
	double CellSize;
	double Neighborhood;
	char DataUnits[STRLEN];
	float DataMin;
	float DataMax;
	float DataMean;
	float DataStd;
	int NumberOfColors;
	char DataType[STRLEN];
	float DataClampMin;
	float DataClampMax;
	int FirstDataByte;
	float NullDataValue;
	float* DataBuf;
	PALENTRY* Palette;

	//for reading and writing
	int Chunk;
	long inCurLoc;
	long outCurLoc;
	long DataEndLoc;
public:
	CThreeDR();
	CThreeDR(CThreeDR& aThreeDR, bool bCopyData);
	~CThreeDR();

	//read and write
	int readin(char* inputFile, int aChunk); //return 0 if reach the file end, otherwise 1;
											//if aChunk is zero, Chunk will be nrow;
	int writeout(char* outputFile, int aChunk); //return 0 if reach the file end, otherwise 1;
												//if aChunk is zero, Chunk will be nrow;
	void copyBasicFields(CThreeDR* aThreeDR); //copy those basic header fields from an old 
											//3dr to a new 3dr.
	void calcStat();						//calc values for the statistical fields
	int rowOfAPoint(double y);    //find out the row of a point, 
											//but the user needs to check if the result is within the range
	int colOfAPoint(double x);    //find out the col of a point
											//but the user needs to check if the result is within the range
	double xOfACell(int col);		//find out the x of a cell
	double yOfACell(int row);		//find out the y of a cell
	bool spatialRangeMatch(CThreeDR* anotherThreeDR);
	void flush();							//reset the object.
	
	//get field values
	int getNumberOfRecords();
	int getNumberOfColumns();
	int getNumberOfRows();
	char* getFileType();
	char* getDataName();
	char* getBaseFilename();
	char* getGridUnits();
	double getXmin();
	double getYmin();
	double getCellSize();
	double getNeighborhood();
	char* getDataUnits();
	float getDataMin();
	float getDataMax();
	float getDataMean();
	float getDataStd();
	int getNumberOfColors();
	char* getDataType();
	float getDataClampMin();
	float getDataClampMax();
	int getFirstDataByte();
	float getNullDataValue();
	float* getDataBuf();
	PALENTRY* getPalette();
	float getData(int row,int col);

	float getData(double x,double y);

	int getChunk();
	long getInCurLoc();
	long getOutCurLoc();
	long getDataEndLoc();

	//set field values
	void setNumberOfRecords(int aNumberOfRecords);
	void setNumberOfColumns(int aNumberOfColumns);
	void setNumberOfRows(int aNumberOfRows);
	void setFileType(char* aFileType);
	void setDataName(char* aDataName);
	void setBaseFilename(char* aBaseFilename);
	void setGridUnits(char* aGridUnits);
	void setXmin(double anXmin);
	void setYmin(double aYmin);
	void setCellSize(float aCellSize);
	void setNeighborhood(float aNeighborhood);
	void setDataUnits(char* aDataUnits);
	void setDataMin(float aDataMin);
	void setDataMax(float aDataMax);
	void setDataMean(float aDataMean);
	void setDataStd(float aDataStd);
	void setNumberOfColors(int aNumberOfColors);
	void setDataType(char* DataType);
	void setDataClampMin(float aDataClampMin);
	void setDataClampMax(float aDataClampMax);
	void setFirstDataByte(int aFirstDataByte);
	void setNullDataValue(float aNullDataValue);
	void setDataBuf(float* aDataBuf);
	void setPalette(PALENTRY* aPalette);

	void setChunk(int aChunk);
	void setInCurLoc(int aCurLoc);
	void setOutCurLoc(int aCurLoc);
	void setDataEndLoc(int aDataEndLoc);
};



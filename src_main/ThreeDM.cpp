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
//ThreeDM.cpp
//written by Xun Shi
//last modified Aug. 6, 2003

#include "StdAfx.h"
#include "SoLIM.h"
#include <stdlib.h>
#include <math.h>
#include "ThreeDM.h"


CThreeDM::CThreeDM(){
	//fields of 3dr file
	NumberOfRecords = NOINTVALUE;
	NumberOfColumns = NOINTVALUE;
	NumberOfRows = NOINTVALUE;
	strcpy(FileType, "");
	strcpy(GridUnits, "");
	Xmin = NODOUBLEVALUE;
	Ymin = NODOUBLEVALUE;
	CellSize = NOFLOATVALUE;
	strcpy(DataUnits, "");
	DataMin = NOFLOATVALUE;
	DataMax = NOFLOATVALUE;
	NullValue = NOFLOATVALUE;
	OriginalNullValue = NOFLOATVALUE;
	FirstDataByte = NOINTVALUE;
	DataBuf = NULL;
	PhotoBuf = NULL;
}

CThreeDM::CThreeDM(const CThreeDM& aThreeDM){
	NumberOfRecords = aThreeDM.NumberOfRecords;
	NumberOfColumns = aThreeDM.NumberOfColumns;
	NumberOfRows = aThreeDM.NumberOfRows;
	strcpy(FileType, aThreeDM.FileType);
	strcpy(GridUnits, aThreeDM.GridUnits);
	Xmin = aThreeDM.Xmin;
	Ymin = aThreeDM.Ymin;
	CellSize = aThreeDM.CellSize;
	strcpy(DataUnits, aThreeDM.DataUnits);
	DataMin = aThreeDM.DataMin;
	DataMax = aThreeDM.DataMax;
	NullValue = aThreeDM.NullValue;
	OriginalNullValue = aThreeDM.OriginalNullValue;
	FirstDataByte = aThreeDM.FirstDataByte;

	if(aThreeDM.DataBuf == NULL)
		DataBuf = NULL;
	else{
		DataBuf = new float[NumberOfColumns*NumberOfRows];
		for(int i = 0; i < NumberOfColumns*NumberOfRows; i++)
			DataBuf[i] = aThreeDM.DataBuf[i];
	}

	if(aThreeDM.PhotoBuf == NULL)
		PhotoBuf = NULL;
	else{
		PhotoBuf = new unsigned char[NumberOfColumns*NumberOfRows];
		for(int i = 0; i < NumberOfColumns*NumberOfRows; i++)
			PhotoBuf[i] = aThreeDM.PhotoBuf[i];
	}
}

CThreeDM::~CThreeDM(){
	if(DataBuf != NULL)
		delete [] DataBuf;
	if(PhotoBuf != NULL)
		delete [] PhotoBuf;
}

//read and write
int CThreeDM::readin(char* inputFile){ 
	FILE *fp;
	char utilityString[50];
	float utilityFloat = 0;
	int utilityInt = 0;

	bool unexpectedFieldFlag = false;
	if ((fp=fopen(inputFile, "r"))==NULL){
		return 9;	//Cannot open inputFile for reading header.
	}
	fscanf(fp, "%s", utilityString);
	if(strcmp(utilityString, "NumberOfRecords:")!=0){
		return 10;	//File format of inputFile has error(s).
	}
	else fscanf(fp, "%d", &NumberOfRecords);
	for(int nl=0; nl<NumberOfRecords; nl++){
		fscanf(fp, "%s", utilityString);
		if(strcmp(utilityString, "NumberOfColumns:")==0){
			fscanf(fp, "%d", &NumberOfColumns);
		}
		else if(strcmp(utilityString, "NumberOfRows:")==0){
			fscanf(fp, "%d", &NumberOfRows);
		}
		else if(strcmp(utilityString, "FileType:")==0){
			fscanf(fp, "%s", FileType);
		}
		else if(strcmp(utilityString, "GridUnits:")==0){
			fscanf(fp, "%s", GridUnits);
		}
		else if(strcmp(utilityString, "Xmin:")==0){
			fscanf(fp, "%lf", &Xmin);
		}
		else if(strcmp(utilityString, "Ymin:")==0){
			fscanf(fp, "%lf", &Ymin);
		}
		else if(strcmp(utilityString, "CellSize:")==0){
			fscanf(fp, "%lf", &CellSize);
		}
		else if(strcmp(utilityString, "DataUnits:")==0){
			fscanf(fp, "%s", DataUnits);
		}
		else if(strcmp(utilityString, "DataMin:")==0){
			fscanf(fp, "%f", &DataMin);
		}
		else if(strcmp(utilityString, "DataMax:")==0){
			fscanf(fp, "%f", &DataMax);
		}
		else if(strcmp(utilityString, "NullValue:")==0){
			fscanf(fp, "%f", &NullValue);
		}
		else if(strcmp(utilityString, "OriginalNullValue:")==0){
			fscanf(fp, "%f", &OriginalNullValue);
		}
		else if(strcmp(utilityString, "FirstDataByte:")==0){
			fscanf(fp, "%d", &FirstDataByte);
		}
		else{
			unexpectedFieldFlag = true; //Unexpected field in inputFile
		}
	}
	fseek(fp, 0, SEEK_END);
	long end = ftell(fp);
	fclose(fp);
	if(DataBuf != NULL)
		delete [] DataBuf;  
	DataBuf = new float[NumberOfRows*NumberOfColumns];
	if(DataBuf == NULL)
		return 14;
	if(PhotoBuf != NULL)
		delete [] PhotoBuf;  
	PhotoBuf = new unsigned char[NumberOfRows*NumberOfColumns];
	if(PhotoBuf == NULL)
		return 14;

	if ((fp=fopen(inputFile, "rb"))==NULL){
		return 15;			//Cannot open inputFile for reading data.
	}
	fseek(fp, FirstDataByte, SEEK_SET);
	fread(PhotoBuf, sizeof(unsigned char), (NumberOfRows-1)*(NumberOfColumns-1), fp);
	int begin = ftell(fp);
	fread(DataBuf, sizeof(float), NumberOfRows*NumberOfColumns, fp);
	fclose(fp);
	return 0; //read successfully.
}

int CThreeDM::writeout(char* outputFile){ 
	FILE* fp;
	if((fp=fopen(outputFile, "w")) == NULL){
		return 9;	//Cannot open outputFile for writing header.
	}
	NumberOfRecords = 0;
	fprintf(fp, "%s %d\n", "NumberOfRecords: ", 99);
	if(NumberOfColumns > 0){
		fprintf(fp, "%s %d\n", "NumberOfColumns: ", NumberOfColumns);
		NumberOfRecords++;
	}
	else{
		return 10;	//Number of columns of outputFile is less than 1. Stop writing.
	}
	if(NumberOfRows > 0){
		fprintf(fp, "%s %d\n", "NumberOfRows: ", NumberOfRows);
		NumberOfRecords++;
	}
	else{
		return 11;	//Number of rows of outputFile is less than 1. Stop writing.
	}
	if(strlen(FileType)>0){
		fprintf(fp, "%s %s\n", "FileType: ", FileType);
		NumberOfRecords++;
	}
	if(strlen(GridUnits)>0){
		fprintf(fp, "%s %s\n", "GridUnits: ", GridUnits);
		NumberOfRecords++;
	}
	if(Xmin>NODOUBLEVALUE){
		fprintf(fp, "%s %lf\n", "Xmin: ", Xmin);
		NumberOfRecords++;
	}
	if(Ymin>NODOUBLEVALUE){
		fprintf(fp, "%s %lf\n", "Ymin: ", Ymin);
		NumberOfRecords++;
	}
	if(CellSize>NOFLOATVALUE){
		fprintf(fp, "%s %lf\n", "CellSize: ", CellSize);
		NumberOfRecords++;
	}
	if(strlen(DataUnits)>0){
		fprintf(fp, "%s %s\n", "DataUnits: ", DataUnits);
		NumberOfRecords++;
	}
	if(DataMin>NOFLOATVALUE){
		fprintf(fp, "%s %f\n", "DataMin: ", DataMin);
		NumberOfRecords++;
	}
	if(DataMax>NOFLOATVALUE){
		fprintf(fp, "%s %f\n", "DataMax: ", DataMax);
		NumberOfRecords++;
	}
	if(NullValue>NOFLOATVALUE){
		fprintf(fp, "%s %f\n", "NullValue: ", NullValue);
		NumberOfRecords++;
	}	
	if(OriginalNullValue>NOFLOATVALUE){
		fprintf(fp, "%s %f\n", "OriginalNullValue: ", OriginalNullValue);
		NumberOfRecords++;
	}
	fprintf(fp, "%s %d\n", "FirstDataByte: ", 999);
	NumberOfRecords++;

	FirstDataByte = ftell(fp); //get the offset location.
	rewind(fp);

	fprintf(fp, "%s %d\n", "NumberOfRecords: ", NumberOfRecords);
	fprintf(fp, "%s %d\n", "NumberOfColumns: ", NumberOfColumns);
	fprintf(fp, "%s %d\n", "NumberOfRows: ", NumberOfRows);
	if(strlen(FileType)>0)	fprintf(fp, "%s %s\n", "FileType: ", FileType);
	if(strlen(GridUnits)>0)		fprintf(fp, "%s %s\n", "GridUnits: ", GridUnits);
	if(Xmin>NODOUBLEVALUE)		fprintf(fp, "%s %f\n", "Xmin: ", Xmin);
	if(Ymin>NODOUBLEVALUE)		fprintf(fp, "%s %f\n", "Ymin: ", Ymin);
	if(CellSize>NOFLOATVALUE)	fprintf(fp, "%s %f\n", "CellSize: ", CellSize);
	if(strlen(DataUnits)>0)		fprintf(fp, "%s %s\n", "DataUnits: ", DataUnits);
	if(DataMin>NOFLOATVALUE)	fprintf(fp, "%s %f\n", "DataMin: ", DataMin);
	if(DataMax>NOFLOATVALUE)	fprintf(fp, "%s %f\n", "DataMax: ", DataMax);
	if(NullValue>NOFLOATVALUE) fprintf(fp, "%s %f\n", "NullValue: ", NullValue);
	if(OriginalNullValue>NOFLOATVALUE)	fprintf(fp, "%s %f\n", "OriginalNullValue: ", OriginalNullValue);
	fprintf(fp, "%s %d\n", "FirstDataByte: ", FirstDataByte);

	fclose(fp);
	fp=fopen(outputFile, "ab");
	fseek(fp, FirstDataByte, SEEK_SET);

	fwrite(PhotoBuf, sizeof(unsigned char), (NumberOfRows-1)*(NumberOfColumns-1), fp);
	fwrite(DataBuf, sizeof(float), NumberOfRows*NumberOfColumns, fp);
	fclose(fp);
	return 0; //write successfully
}

void CThreeDM::copyBasicFields(CThreeDM* aThreeDM){
	setNumberOfColumns(aThreeDM->getNumberOfColumns());
	setNumberOfRows(aThreeDM->getNumberOfRows());
	setFileType(aThreeDM->getFileType());
	setGridUnits(aThreeDM->getGridUnits());
	setXmin(aThreeDM->getXmin());
	setYmin(aThreeDM->getYmin());
	setCellSize(aThreeDM->getCellSize());
	setDataUnits(aThreeDM->getDataUnits());
	setNullValue(aThreeDM->getNullValue());
}

void CThreeDM::calcStat(){
	float minmax[2];
	dataMinMax(DataBuf, NumberOfRows*NumberOfColumns, NullValue, minmax);
	DataMin = minmax[0];
	DataMax = minmax[1];
}

int CThreeDM::rowOfAPoint(double y){ 
	//user needs to check if the result is within the range

	int row = (int)((float)NumberOfRows-(y-Ymin)/CellSize);

	// int row = (int) (floor(((NumberOfRows*CellSize+Ymin)-y)/CellSize));



	if(row<0)
		return 0;
	if(row>NumberOfRows-1)
		return NumberOfRows-1;
	else
		return row;
}

int CThreeDM::colOfAPoint(double x){
	//user needs to check if the result is within the range
	int col = (int)((x-Xmin)/CellSize);

   
	//int col = (int) (floor((x-Xmin)/CellSize));

	if(col<0)
		return 0;
	if(col>NumberOfColumns-1)
		return NumberOfColumns-1;
	else
		return col;
}

double CThreeDM::xOfACell(int col){
	return Xmin+CellSize*(col+0.5);
}

double CThreeDM::yOfACell(int row){
	return Ymin+CellSize*(NumberOfRows-row-0.5);
}

bool CThreeDM::spatialRangeMatch(CThreeDM* anotherThreeDM){
	if(anotherThreeDM == NULL) return false;
	if(fabs(Xmin-anotherThreeDM->getXmin())<VERYSMALL
		&& fabs(Ymin-anotherThreeDM->getYmin())<VERYSMALL
		&& fabs(CellSize-anotherThreeDM->getCellSize())<VERYSMALL)
		return true;
	else
		return false;
}

bool CThreeDM::spatialRangeMatch(CThreeDR* anotherThreeDR){
	if(anotherThreeDR == NULL) return false;
	if(fabs(Xmin-anotherThreeDR->getXmin())<VERYSMALL
		&& fabs(Ymin-anotherThreeDR->getYmin())<VERYSMALL
		&& fabs(CellSize-anotherThreeDR->getCellSize())<VERYSMALL)
		return true;
	else
		return false;
}

void CThreeDM::flush(){
	//fields of 3dr file
	NumberOfRecords = NOINTVALUE;
	NumberOfColumns = NOINTVALUE;
	NumberOfRows = NOINTVALUE;
	strcpy(FileType, "");
	strcpy(GridUnits, "");
	Xmin = NODOUBLEVALUE;
	Ymin = NODOUBLEVALUE;
	CellSize = NOFLOATVALUE;
	strcpy(DataUnits, "");
	DataMin = NOFLOATVALUE;
	DataMax = NOFLOATVALUE;
	NullValue = NOFLOATVALUE;
	OriginalNullValue = NOFLOATVALUE;
	FirstDataByte = NOINTVALUE;

	if(DataBuf != NULL){
		float* temp = DataBuf;
		DataBuf = NULL;
		delete [] temp;
	}
	if(PhotoBuf != NULL){
		unsigned char* temp = PhotoBuf;
		PhotoBuf = NULL;
		delete [] temp;
	}
}

//get field values
int CThreeDM::getNumberOfRecords(){
	return NumberOfRecords;
}

int CThreeDM::getNumberOfColumns(){
	return NumberOfColumns;
}

int CThreeDM::getNumberOfRows(){
	return NumberOfRows;
}

char* CThreeDM::getFileType(){
	return FileType;
}

char* CThreeDM::getGridUnits(){
	return GridUnits;
}

double CThreeDM::getXmin(){
	return Xmin;
}

double CThreeDM::getYmin(){
	return Ymin;
}

double CThreeDM::getCellSize(){
	return CellSize;
}

char* CThreeDM::getDataUnits(){
	return DataUnits;
}

float CThreeDM::getDataMin(){
	return DataMin;
}

float CThreeDM::getDataMax(){
	return DataMax;
}

float CThreeDM::getNullValue(){
	return NullValue;
}

float CThreeDM::getOriginalNullValue(){
	return OriginalNullValue;
}

int CThreeDM::getFirstDataByte(){
	return FirstDataByte;
}

float* CThreeDM::getDataBuf(){
	return DataBuf;
}

unsigned char* CThreeDM::getPhotoBuf(){
	return PhotoBuf;
}

//set field values
void CThreeDM::setNumberOfRecords(int aNumberOfRecords){
	NumberOfRecords = aNumberOfRecords;
}

void CThreeDM::setNumberOfColumns(int aNumberOfColumns){
	NumberOfColumns = aNumberOfColumns;
}

void CThreeDM::setNumberOfRows(int aNumberOfRows){
	NumberOfRows = aNumberOfRows;
}

void CThreeDM::setFileType(char* aFileType){
	strcpy(FileType, aFileType);
}

void CThreeDM::setGridUnits(char* aGridUnits){
	strcpy(GridUnits, aGridUnits);
}

void CThreeDM::setXmin(double anXmin){
	Xmin = anXmin;
}

void CThreeDM::setYmin(double aYmin){
	Ymin = aYmin;
}

void CThreeDM::setCellSize(float aCellSize){
	CellSize = aCellSize;
}

void CThreeDM::setDataUnits(char* aDataUnits){
	strcpy(DataUnits, aDataUnits);
}

void CThreeDM::setDataMin(float aDataMin){
	DataMin = aDataMin;
}

void CThreeDM::setDataMax(float aDataMax){
	DataMax = aDataMax;
}

void CThreeDM::setNullValue(float aNullValue){
	NullValue = aNullValue;
}

void CThreeDM::setOriginalNullValue(float anOriginalNullValue){
	OriginalNullValue = anOriginalNullValue;
}

void CThreeDM::setFirstDataByte(int aFirstDataByte){
	FirstDataByte = aFirstDataByte;
}

void CThreeDM::setDataBuf(float* aDataBuf){
	DataBuf = aDataBuf;
}

void CThreeDM::setPhotoBuf(unsigned char* aPhotoBuf){
	PhotoBuf = aPhotoBuf;
}


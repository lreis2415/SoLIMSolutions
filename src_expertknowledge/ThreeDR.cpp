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
#include "SoLIM.h"
#include <stdlib.h>
#include <math.h>
#include "ThreeDR.h"

CThreeDR::CThreeDR(){
	//fields of 3dr file
	NumberOfRecords = NOINTVALUE;
	NumberOfColumns = NOINTVALUE;
	NumberOfRows = NOINTVALUE;
	strcpy(FileType, "");
	strcpy(DataName, "");
	strcpy(BaseFilename, "");
	strcpy(GridUnits, "");
	Xmin = NODOUBLEVALUE;
	Ymin = NODOUBLEVALUE;
	CellSize = NODOUBLEVALUE;
	Neighborhood = NODOUBLEVALUE;
	strcpy(DataUnits, "");
	DataMin = NOFLOATVALUE;
	DataMax = NOFLOATVALUE;
	DataMean = NOFLOATVALUE;
	DataStd = NOFLOATVALUE;
	NumberOfColors = NOINTVALUE;
	strcpy(DataType, "");
	DataClampMin = NOFLOATVALUE;
	DataClampMax = NOFLOATVALUE;
	FirstDataByte = NOINTVALUE;
	NullDataValue = NOFLOATVALUE;
	DataBuf = NULL;
	Palette = NULL;

	//for reading and writing
	Chunk = 0;
	inCurLoc = 0; //reading point when reading
	outCurLoc = 0; //writing point when writing
	DataEndLoc = 0;
}


CThreeDR::CThreeDR(CThreeDR& aThreeDR,bool bCopyData){
	NumberOfRecords = aThreeDR.NumberOfRecords;
	NumberOfColumns = aThreeDR.NumberOfColumns;
	NumberOfRows = aThreeDR.NumberOfRows;
	strcpy(FileType, aThreeDR.FileType);
	strcpy(DataName, aThreeDR.DataName);
	strcpy(BaseFilename, aThreeDR.BaseFilename);
	strcpy(GridUnits, aThreeDR.GridUnits);
	Xmin = aThreeDR.Xmin;
	Ymin = aThreeDR.Ymin;
	CellSize = aThreeDR.CellSize;
	Neighborhood = aThreeDR.Neighborhood;
	strcpy(DataUnits, aThreeDR.DataUnits);
	DataMin = aThreeDR.DataMin;
	DataMax = aThreeDR.DataMax;
	DataMean = aThreeDR.DataMean;
	DataStd = aThreeDR.DataStd;
	NumberOfColors = aThreeDR.NumberOfColors;
	strcpy(DataType, aThreeDR.DataType);
	DataClampMin = aThreeDR.DataClampMin;
	DataClampMax = aThreeDR.DataClampMax;
	NullDataValue = aThreeDR.NullDataValue;
	FirstDataByte = aThreeDR.FirstDataByte;

	Chunk = aThreeDR.Chunk;
	inCurLoc = aThreeDR.inCurLoc;
	outCurLoc = aThreeDR.outCurLoc;
	DataEndLoc = aThreeDR.DataEndLoc;

	if(bCopyData == true)
	{
		if(aThreeDR.DataBuf != NULL){
			DataBuf = new float[NumberOfColumns*NumberOfRows];
			for(int i = 0; i < NumberOfColumns*NumberOfRows; i++)
				DataBuf[i] = aThreeDR.DataBuf[i];
		}
		else
			DataBuf = NULL;

		if(NumberOfColors > 0 && aThreeDR.Palette != NULL){
			Palette = new PALENTRY[NumberOfColors];
			for(int p = 0; p<NumberOfColors; p++){
				Palette[p].index = aThreeDR.Palette[p].index;
				Palette[p].rasterval = aThreeDR.Palette[p].rasterval;
				Palette[p].r = aThreeDR.Palette[p].r;
				Palette[p].g = aThreeDR.Palette[p].g;
				Palette[p].b = aThreeDR.Palette[p].b;
			}
		}
		else
			Palette = NULL;
	}
	else 
	{
		DataBuf = NULL;
		Palette = NULL;
	}

}



CThreeDR::~CThreeDR(){
	if(DataBuf != NULL)
		delete [] DataBuf;
	if(Palette != NULL)
		delete [] Palette;
}

//read and write
int CThreeDR::readin(char* inputFile, int aChunk){ //return 0 if reach the file end, otherwise 1;
												//if aChunk is 0, Chunk will be set to be nrow.
	FILE *fp;

	char utilityString[50];
	float utilityFloat = 0;
	int utilityInt = 0;

	bool unexpectedFieldFlag = false;
	if(inCurLoc == 0){  //First time open the file
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
			else if(strcmp(utilityString, "DataName:")==0){
				fgets(DataName, FN_LEN, fp);
				if(DataName[strlen(DataName)-1]=='\n')
					DataName[strlen(DataName)-1]='\0';
				trim(DataName, DataName);
			}
			else if(strcmp(utilityString, "BaseFilename:")==0){
				fgets(BaseFilename, FN_LEN, fp);
				if(BaseFilename[strlen(BaseFilename)-1]=='\n')
					BaseFilename[strlen(BaseFilename)-1]='\0';
				trim(BaseFilename, BaseFilename);
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
			else if(strcmp(utilityString, "Neighborhood:")==0){
				fscanf(fp, "%lf", &Neighborhood);
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
			else if(strcmp(utilityString, "DataMean:")==0){
				fscanf(fp, "%f", &DataMean);
			}
			else if(strcmp(utilityString, "DataStd:")==0){
				fscanf(fp, "%f", &DataStd);
			}
			else if(strcmp(utilityString, "NumberOfColors:")==0){
				fscanf(fp, "%d", &NumberOfColors);
			}
			else if(strcmp(utilityString, "DataType:")==0){
				fscanf(fp, "%s", DataType);
			}
			else if(strcmp(utilityString, "DataClampMin:")==0){
				fscanf(fp, "%f", &DataClampMin);
			}
			else if(strcmp(utilityString, "DataClampMax:")==0){
				fscanf(fp, "%f", &DataClampMax);
			}
			else if(strcmp(utilityString, "NullDataValue:")==0){
				fscanf(fp, "%f", &NullDataValue);
			}
			else if(strcmp(utilityString, "NoData:")==0){
				fscanf(fp, "%f", &NullDataValue);
			}
			else if(strcmp(utilityString, "FirstDataByte:")==0){
				fscanf(fp, "%d", &FirstDataByte);
				inCurLoc = FirstDataByte;
			}
			else{
				unexpectedFieldFlag = true; //Unexpected field in inputFile
			}
		}
		fclose(fp);
		inCurLoc = FirstDataByte;
		DataEndLoc = NumberOfRows*NumberOfColumns*sizeof(float) + FirstDataByte;
	}
	
	if(aChunk==0 || aChunk >= NumberOfRows) Chunk = NumberOfRows;
	else Chunk = aChunk;

	if(inCurLoc <= 0){
		return 12;	//The FirstDataByte or inCurLoc of inputFile was not properly read.
	}
	if(inCurLoc > DataEndLoc){
		return 13;	//Intend to read beyond the file inputFile
	}

	if(DataBuf != NULL)
		delete [] DataBuf;   //the "Chunk" might be different for each time
	DataBuf = new float[Chunk*NumberOfColumns];
	if(DataBuf == NULL)
		return 14;

	if ((fp=fopen(inputFile, "rb"))==NULL){
		return 15;			//Cannot open inputFile for reading data.
	}

	fseek(fp, inCurLoc, SEEK_SET);

	if(inCurLoc + Chunk*NumberOfColumns*sizeof(float) >= DataEndLoc){//if already reach the end 
		fread(DataBuf, sizeof(float), (DataEndLoc - inCurLoc)/sizeof(float), fp);
		inCurLoc = ftell(fp); //inCurLoc should be already at the file end

		//read color platte if there is any.
		bool readColorFlag = true;
		if(NumberOfColors > 0){
			int loc1 = inCurLoc + NumberOfColors*sizeof(PALENTRY);
			fseek(fp, 0, SEEK_END);
			int loc2 = ftell(fp);
			if(loc1!=loc2){
				readColorFlag = false; //The color palatte of inputFile was not correctly read. The colors were dropped.
			}
			else{
				fseek(fp, inCurLoc, SEEK_SET);
				Palette = new PALENTRY[NumberOfColors];
				fread(Palette, sizeof(PALENTRY), NumberOfColors, fp);
			}
		}
		inCurLoc = ftell(fp);
		fclose(fp);
		if(unexpectedFieldFlag)
			return 2;
		else if(!readColorFlag)
			return 3;
		else 
			return 0; //the last portion has already been read
	}
	else{
		fread(DataBuf, sizeof(float), Chunk*NumberOfColumns, fp);
		inCurLoc = ftell(fp);
		fclose(fp);
		return 1; //the chunck has been read successfully.
	}
}

int CThreeDR::writeout(char* outputFile, int aChunk){ //return 0 if reach the file end, otherwise 1;
												//if aChunk is zero, Chunk will be nrow;
	FILE* fp;

	if(outCurLoc == 0){ //first time open the file.
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
		if(strlen(DataName)>0){
			fprintf(fp, "%s %s\n", "DataName: ", DataName);
			NumberOfRecords++;
		}
		if(strlen(BaseFilename)>0){
			fprintf(fp, "%s %s\n", "BaseFilename: ", BaseFilename);
			NumberOfRecords++;
		}
		if(strlen(GridUnits)>0){
			fprintf(fp, "%s %s\n", "GridUnits: ", GridUnits);
			NumberOfRecords++;
		}
		if(Xmin>NODOUBLEVALUE){
			fprintf(fp, "%s %.3f\n", "Xmin: ", Xmin);
			NumberOfRecords++;
		}
		if(Ymin>NODOUBLEVALUE){
			fprintf(fp, "%s %.3f\n", "Ymin: ", Ymin);
			NumberOfRecords++;
		}
		if(CellSize>NOFLOATVALUE){
			fprintf(fp, "%s %.3f\n", "CellSize: ", CellSize);
			NumberOfRecords++;
		}
		if(Neighborhood>NOFLOATVALUE){
			fprintf(fp, "%s %.3f\n", "Neighborhood: ", Neighborhood);
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
		if(DataMean>NOFLOATVALUE){
			fprintf(fp, "%s %f\n", "DataMean: ", DataMean);
			NumberOfRecords++;
		}
		if(DataStd>NOFLOATVALUE){
			fprintf(fp, "%s %f\n", "DataStd: ", DataStd);
			NumberOfRecords++;
		}
		if(NumberOfColors>NOINTVALUE){
			fprintf(fp, "%s %d\n", "NumberOfColors: ", NumberOfColors);
			NumberOfRecords++;
		}
		if(strlen(DataType)>0){
			fprintf(fp, "%s %s\n", "DataType: ", DataType);
			NumberOfRecords++;
		}
		if(DataClampMin>NOFLOATVALUE){
			fprintf(fp, "%s %f\n", "DataClampMin: ", DataClampMin);
			NumberOfRecords++;
		}
		if(DataClampMax>NOFLOATVALUE){
			fprintf(fp, "%s %f\n", "DataClampMax: ", DataClampMax);
			NumberOfRecords++;
		}
		if(NullDataValue>NOFLOATVALUE){
            fprintf(fp, "%s %f\n", "NullDataValue: ", NullDataValue);
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
		if(strlen(DataName)>0)	fprintf(fp, "%s %s\n", "DataName: ", DataName);
		if(strlen(BaseFilename)>0)	fprintf(fp, "%s %s\n", "BaseFilename: ", BaseFilename);
		if(strlen(GridUnits)>0)		fprintf(fp, "%s %s\n", "GridUnits: ", GridUnits);
		if(Xmin>NODOUBLEVALUE)		fprintf(fp, "%s %.3f\n", "Xmin: ", Xmin);
		if(Ymin>NODOUBLEVALUE)		fprintf(fp, "%s %.3f\n", "Ymin: ", Ymin);
		if(CellSize>NOFLOATVALUE)	fprintf(fp, "%s %.3f\n", "CellSize: ", CellSize);
		if(Neighborhood>NOFLOATVALUE)	fprintf(fp, "%s %.3f\n", "Neighborhood: ", Neighborhood);
		if(strlen(DataUnits)>0)			fprintf(fp, "%s %s\n", "DataUnits: ", DataUnits);
		if(DataMin>NOFLOATVALUE)		fprintf(fp, "%s %f\n", "DataMin: ", DataMin);
		if(DataMax>NOFLOATVALUE)		fprintf(fp, "%s %f\n", "DataMax: ", DataMax);
		if(DataMean>NOFLOATVALUE)		fprintf(fp, "%s %f\n", "DataMean: ", DataMean);
		if(DataStd>NOFLOATVALUE)		fprintf(fp, "%s %f\n", "DataStd: ", DataStd);
		if(NumberOfColors>NOINTVALUE)	fprintf(fp, "%s %d\n", "NumberOfColors: ", NumberOfColors);
		if(strlen(DataType)>0)			fprintf(fp, "%s %s\n", "DataType: ", DataType);
		if(DataClampMin>NOFLOATVALUE)	fprintf(fp, "%s %f\n", "DataClampMin: ", DataClampMin);
		if(DataClampMax>NOFLOATVALUE)	fprintf(fp, "%s %f\n", "DataClampMax: ", DataClampMax);
	    if(NullDataValue>NOFLOATVALUE)  fprintf(fp, "%s %f\n", "NullDataValue: ", NullDataValue);
		fprintf(fp, "%s %d\n", "FirstDataByte: ", FirstDataByte);
		
		fclose(fp);
		outCurLoc = FirstDataByte;
		DataEndLoc = NumberOfRows*NumberOfColumns*sizeof(float) + FirstDataByte;
	}

	if(outCurLoc <= 0){
		return 12;	//The FirstDataByte or outCurLoc of outputFile was not properly written. Stop writing.
	}
	if(outCurLoc > DataEndLoc){
		return 13;	//Intend to write beyond the file outputFile.
	}
	if(aChunk == 0) Chunk = NumberOfRows;
	else Chunk = aChunk;

	fp=fopen(outputFile, "ab");
	fseek(fp, outCurLoc, SEEK_SET);

	if(outCurLoc + Chunk*NumberOfColumns*sizeof(float) >= DataEndLoc){
		fwrite(DataBuf, sizeof(float), (DataEndLoc - outCurLoc)/sizeof(float), fp);
		outCurLoc = ftell(fp);

		//write color platte if there is any
		if(NumberOfColors > 0 && Palette != NULL){
			fwrite(Palette, sizeof(PALENTRY), NumberOfColors, fp);
		}
		fclose(fp);
		return 0;
	}
	else{
		fwrite(DataBuf, sizeof(float), Chunk*NumberOfColumns, fp);
		outCurLoc = ftell(fp);
		fclose(fp);
		return 1;
	}
}

void CThreeDR::copyBasicFields(CThreeDR* aThreeDR){
	setNumberOfColumns(aThreeDR->getNumberOfColumns());
	setNumberOfRows(aThreeDR->getNumberOfRows());
	setBaseFilename(aThreeDR->getBaseFilename());
	setCellSize(aThreeDR->getCellSize());
	setDataType(aThreeDR->getDataType());
	setDataUnits(aThreeDR->getDataUnits());
	setFileType(aThreeDR->getFileType());
	setGridUnits(aThreeDR->getGridUnits());
	setNeighborhood(aThreeDR->getNeighborhood());
	setXmin(aThreeDR->getXmin());
	setYmin(aThreeDR->getYmin());
	setNullDataValue(aThreeDR->getNullDataValue());
}

void CThreeDR::calcStat(){
	float minmax[2];
	dataMinMax(DataBuf, NumberOfRows*NumberOfColumns, NullDataValue, minmax);
	DataMin = minmax[0];
	DataMax = minmax[1];
	DataMean = dataMean(DataBuf, NumberOfRows*NumberOfColumns, NullDataValue);
	DataStd = calcStdDev(DataBuf, NumberOfRows*NumberOfColumns, NullDataValue);
}

int CThreeDR::rowOfAPoint(double y){ 
	return floor((float)NumberOfRows-(y-Ymin)/CellSize);
}

int CThreeDR::colOfAPoint(double x){
	return floor((x-Xmin)/CellSize);

	//return (int)((x-Xmin)/CellSize);
}

double CThreeDR::xOfACell(int col){
	return Xmin+CellSize*(col+0.5);
}

double CThreeDR::yOfACell(int row){
	return Ymin+CellSize*(NumberOfRows-row-0.5);
}

bool CThreeDR::spatialRangeMatch(CThreeDR* anotherThreeDR){
	if(anotherThreeDR == NULL) return false;
	if(fabs(Xmin-anotherThreeDR->Xmin)<VERYSMALL
		&& fabs(Ymin-anotherThreeDR->Ymin)<VERYSMALL
		&& fabs(CellSize-anotherThreeDR->CellSize)<VERYSMALL)
		return true;
	else
		return false;
}

void CThreeDR::flush(){
	//fields of 3dr file
	NumberOfRecords = NOINTVALUE;
	NumberOfColumns = NOINTVALUE;
	NumberOfRows = NOINTVALUE;
	strcpy(FileType, "");
	strcpy(DataName, "");
	strcpy(BaseFilename, "");
	strcpy(GridUnits, "");
	Xmin = NODOUBLEVALUE;
	Ymin = NODOUBLEVALUE;
	CellSize = NOFLOATVALUE;
	Neighborhood = NOFLOATVALUE;
	strcpy(DataUnits, "");
	DataMin = NOFLOATVALUE;
	DataMax = NOFLOATVALUE;
	DataMean = NOFLOATVALUE;
	DataStd = NOFLOATVALUE;
	NumberOfColors = NOINTVALUE;
	strcpy(DataType, "");
	DataClampMin = NOFLOATVALUE;
	DataClampMax = NOFLOATVALUE;
	NullDataValue = NOFLOATVALUE;
	FirstDataByte = NOINTVALUE;

	if(DataBuf != NULL){
		float* temp = DataBuf;
		DataBuf = NULL;
		delete [] temp;
	}
	if(Palette != NULL){
		PALENTRY* temp = Palette;
		Palette = NULL;
		delete [] temp;
	}

	//for reading and writing
	Chunk = 0;
	inCurLoc = 0; //reading point when reading
	outCurLoc = 0; //writing point when writing
	DataEndLoc = 0;
}

//get field values
int CThreeDR::getNumberOfRecords(){
	return NumberOfRecords;
}

int CThreeDR::getNumberOfColumns(){
	return NumberOfColumns;
}

int CThreeDR::getNumberOfRows(){
	return NumberOfRows;
}

char* CThreeDR::getFileType(){
	return FileType;
}

char* CThreeDR::getDataName(){
	return DataName;
}

char* CThreeDR::getBaseFilename(){
	return BaseFilename;
}

char* CThreeDR::getGridUnits(){
	return GridUnits;
}

double CThreeDR::getXmin(){
	return Xmin;
}

double CThreeDR::getYmin(){
	return Ymin;
}

double CThreeDR::getCellSize(){
	return CellSize;
}

double CThreeDR::getNeighborhood(){
	return Neighborhood;
}

char* CThreeDR::getDataUnits(){
	return DataUnits;
}

float CThreeDR::getDataMin(){
	return DataMin;
}

float CThreeDR::getDataMax(){
	return DataMax;
}

float CThreeDR::getDataMean(){
	return DataMean;
}

float CThreeDR::getDataStd(){
	return DataStd;
}

int CThreeDR::getNumberOfColors(){
	return NumberOfColors;
}

char* CThreeDR::getDataType(){
	return DataType;
}

float CThreeDR::getDataClampMin(){
	return DataClampMin;
}

float CThreeDR::getDataClampMax(){
	return DataClampMax;
}

float CThreeDR::getNullDataValue(){
	return NullDataValue;
}

int CThreeDR::getFirstDataByte(){
	return FirstDataByte;
}

float* CThreeDR::getDataBuf(){
	return DataBuf;
}

PALENTRY* CThreeDR::getPalette(){
	return Palette;
}

int CThreeDR::getChunk(){
	return Chunk;
}

long CThreeDR::getInCurLoc(){
	return inCurLoc;
}

long CThreeDR::getOutCurLoc(){
	return outCurLoc;
}

long CThreeDR::getDataEndLoc(){
	return DataEndLoc;
}


//set field values
void CThreeDR::setNumberOfRecords(int aNumberOfRecords){
	NumberOfRecords = aNumberOfRecords;
}

void CThreeDR::setNumberOfColumns(int aNumberOfColumns){
	NumberOfColumns = aNumberOfColumns;
}

void CThreeDR::setNumberOfRows(int aNumberOfRows){
	NumberOfRows = aNumberOfRows;
}

void CThreeDR::setFileType(char* aFileType){
	strcpy(FileType, aFileType);
}

void CThreeDR::setDataName(char* aDataName){
	strcpy(DataName, aDataName);
}

void CThreeDR::setBaseFilename(char* aBaseFilename){
	strcpy(BaseFilename, aBaseFilename);
}

void CThreeDR::setGridUnits(char* aGridUnits){
	strcpy(GridUnits, aGridUnits);
}

void CThreeDR::setXmin(double anXmin){
	Xmin = anXmin;
}

void CThreeDR::setYmin(double aYmin){
	Ymin = aYmin;
}

void CThreeDR::setCellSize(float aCellSize){
	CellSize = aCellSize;
}

void CThreeDR::setNeighborhood(float aNeighborhood){
	Neighborhood = aNeighborhood;
}

void CThreeDR::setDataUnits(char* aDataUnits){
	strcpy(DataUnits, aDataUnits);
}

void CThreeDR::setDataMin(float aDataMin){
	DataMin = aDataMin;
}

void CThreeDR::setDataMax(float aDataMax){
	DataMax = aDataMax;
}

void CThreeDR::setDataMean(float aDataMean){
	DataMean = aDataMean;
}

void CThreeDR::setDataStd(float aDataStd){
	DataStd = aDataStd;
}

void CThreeDR::setNumberOfColors(int aNumberOfColors){
	NumberOfColors = aNumberOfColors;
}

void CThreeDR::setDataType(char* DataType){
	strcpy(DataType, DataType);
}

void CThreeDR::setDataClampMin(float aDataClampMin){
	DataClampMin = aDataClampMin;
}

void CThreeDR::setDataClampMax(float aDataClampMax){
	DataClampMax = aDataClampMax;
}

void CThreeDR::setNullDataValue(float aNullDataValue){
	NullDataValue = aNullDataValue;
}

void CThreeDR::setFirstDataByte(int aFirstDataByte){
	FirstDataByte = aFirstDataByte;
}

void CThreeDR::setDataBuf(float* aDataBuf){
	DataBuf = aDataBuf;
}

void CThreeDR::setChunk(int aChunk){
	Chunk = aChunk;
}

void CThreeDR::setInCurLoc(int anInCurLoc){
	inCurLoc = anInCurLoc;
}

void CThreeDR::setOutCurLoc(int anOutCurLoc){
	outCurLoc = anOutCurLoc;
}

void CThreeDR::setDataEndLoc(int aDataEndLoc){
	DataEndLoc = aDataEndLoc;
}

void CThreeDR::setPalette(PALENTRY* aPalette){
	if(NumberOfColors<1)
		return;
	Palette = new PALENTRY[NumberOfColors];
	for(int p = 0; p<NumberOfColors; p++){
		Palette[p].index = aPalette[p].index;
		Palette[p].rasterval = aPalette[p].rasterval;
		Palette[p].r = aPalette[p].r;
		Palette[p].g = aPalette[p].g;
		Palette[p].b = aPalette[p].b;
	}
}


float CThreeDR::getData(int row,int col)
{
	return DataBuf[row*NumberOfColumns+col];
}


float CThreeDR::getData(double x,double y)
{
    int irow,icol;
	if(x<getXmin()||x>(getXmin()+getCellSize()*getNumberOfColumns()))
		return NOFLOATVALUE;
	else if(y<getYmin()||y>(getYmin()+getCellSize()*getNumberOfRows()))
		return NOFLOATVALUE;
	else
	{
		irow = (x-getXmin())/getCellSize();
		icol = (y-getYmin())/getCellSize();

		irow = (int)((float)NumberOfRows-(y-Ymin)/CellSize);
 
        icol= (int)((x-Xmin)/CellSize);


		return DataBuf[irow*NumberOfColumns+icol];
	}

	
}


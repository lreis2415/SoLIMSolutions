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

#include "stdafx.h"
#include "ImportOldPrj.h"
#include "shpcasebase.h"
#include "RangeRule.h"
#include "EnumRule.h"
#include "SoilType.h"
#include "Instance.h"
#include "Occurrence.h"
#include "Exclusion.h"
#include "FreehandRule.h"
#include "WordRule.h"
#include "PointRule.h"


#include  <msxml2.h>      
#import   <msxml4.dll>

//---------------------------------------------------------------------------------------------------
//The Setting class

OldSetting::OldSetting(char aWorkDir[], char aPntFile[], char aLabFile[], char aCellnameFile[])
{
  getSetting(aWorkDir, aPntFile, aLabFile, aCellnameFile);
}
//---------------------------------------------------------------------------------------------------
void OldSetting::getSetting(char aWorkDir[], char aPntFile[], char aLabFile[], char aCellnameFile[])
{
	//1. Set the workDir, thePntFile, theLabFile, theCellnameFile, and theMaskFile in the Setting object

	strcpy(thePntFile, extract_filename(aPntFile));
	strcpy(theLabFile, extract_filename(aLabFile));
	strcpy(theCellnameFile, extract_filename(aCellnameFile));

	if(strcmp(thePntFile,".")==0||strcmp(theLabFile,".")==0||strcmp(theCellnameFile,".")==0)
	{
		AfxMessageBox("Error in converting project.");
		return;
	}

	strcpy(workDir, aWorkDir);
	strcat(workDir, SLASH);

	//2. set nodata
	nodata = -9999;

	//3. Get the other settings from the "header" file under header dir.
	char headerfile[FN_LEN]; 
	strcpy(headerfile, workDir);
	strcat(headerfile, "header");
	strcat(headerfile, SLASH);
	strcat(headerfile, "header.txt");

	char utilityString[60];
	ifstream fin;

	fin.open(headerfile);
	if(fin.fail()){
		fin.clear(0);
		strcpy(headerfile, workDir);
		strcat(headerfile, "header");
		strcat(headerfile, SLASH);
		strcat(headerfile, "header");
		fin.open(headerfile);//
		if(fin.fail()){
			AfxMessageBox("Error in converting project.");
			return;
		}
	}
	fin>>utilityString>>nrow;
	if(strlen(trim(utilityString, utilityString))==0){
		AfxMessageBox("Error in converting project.");
		fin.close();
		return;
	} 

	if(!fin.eof()) fin>>utilityString>>ncol;
	if(!fin.eof()) fin>>utilityString>>west;
	if(!fin.eof()) fin>>utilityString>>east;
	if(!fin.eof()) fin>>utilityString>>south;
	if(!fin.eof()) fin>>utilityString>>north;
	if(!fin.eof()) fin>>utilityString>>pixelsize;
	fin.close();
}


//calc the std if not found in header
OldLayerListNode::OldLayerListNode(char lName[], char fName[], OldSetting* theSetting, char typeS[]="default", char widthS[]="default")
{
	char dataDir[STRLEN];
	strcpy(dataDir,theSetting->workDir);
	strcat(dataDir, "data");
	strcat(dataDir, SLASH);
	strcat(dataDir, fName);
	strcpy(layerName, lName); //set layername;
	strcpy(layerFileName, dataDir); //save the corresponding data file name
	
	//this part is to preset the type (but the width will be set after the
	// standard deviation is computed

	if(strcmp(typeS, "default")!=0){
		sscanf(typeS, "%d", &type);
	}
	else {
		if (strcmp(layerName, "Aspect:")==0||strcmp(layerName, "aspect:")==0) {
			type = 2;
		}
		else {
			type = 4;			//default is ratio for any other variables
		}
	}

	//Check settings
	FILE *fp;
	if ((fp=fopen(layerFileName, "r"))==NULL){
		AfxMessageBox("Error in converting project.");
		return;
	}

	char keyword[STRLEN]; //for holding the keyword
	int numLine = 0;   //for holding the number of lines in the header
	int intValue = 0;  //for holding integer value
	float floatValue = 0; //for holding floating value
	char charValue[STRLEN];   //for holding charValue
	int dataStdFlag = 0; //for check if its there

	fscanf(fp, "%s", keyword);
	if(strcmp(keyword, "NumberOfRecords:")!=0){
	    fclose(fp);
		AfxMessageBox("Error in converting project.");
		return;
	}
	else fscanf(fp, "%d", &numLine);
	for(int nl=0; nl<numLine; nl++){
		fscanf(fp, "%s", keyword);
		if(strcmp(keyword, "NumberOfRows:")==0)
		{
			fscanf(fp, "%d", &intValue);
			if((intValue-theSetting->nrow)>=1)
			{
				AfxMessageBox("Error in converting project.");	
				fclose(fp);
				return;
			}
			else
				nrow = theSetting->nrow; //assign the correct value first, then assign CHUNK later
		}
		else if(strcmp(keyword, "NumberOfColumns:")==0){
			fscanf(fp, "%d", &intValue);
			if((intValue-theSetting->ncol)>=1){
				AfxMessageBox("Error in converting project.");		
				fclose(fp);
				return;
			}
			else
				ncol = theSetting->ncol;
		}
		else if(strcmp(keyword, "NumberOfColors:")==0){
			fscanf(fp, "%d", &intValue);
		}
		else if(strcmp(keyword, "FileType:")==0){
			fscanf(fp, "%s", charValue);
			if(strcmp(charValue, "RASTER")!=0){
				AfxMessageBox("Error in converting project.");
				fclose(fp);
				return;
			}
		}
		else if(strcmp(keyword, "DataType:")==0){
			fscanf(fp, "%s", charValue);
		}
		else if(strcmp(keyword, "GridUnits:")==0){
			fscanf(fp, "%s", charValue);
			strcpy(theSetting->gridUnits, charValue);
		}
		else if(strcmp(keyword, "DataUnits:")==0){
			fscanf(fp, "%s", charValue);
		}
		else if(strcmp(keyword, "CellSize:")==0){
			fscanf(fp, "%f", &floatValue);
			if((floatValue-theSetting->pixelsize)>=0.1){
				AfxMessageBox("Error in converting project.");			
				return;
			}
		}
		else if(strcmp(keyword, "Xmin:")==0){
			fscanf(fp, "%f", &floatValue);
			if((floatValue-theSetting->west)>=0.1){
				AfxMessageBox("Error in converting project.");
				fclose(fp);
				return;
			}
		}
		else if(strcmp(keyword, "Ymin:")==0){
			fscanf(fp, "%f", &floatValue);
			if((floatValue-theSetting->south)>=0.1){
				AfxMessageBox("Error in converting project.");
				fclose(fp);			
				return;
			}
		}
		else if(strcmp(keyword, "DataName:")==0){
			fgets(charValue, STRLEN, fp);
		}
		else if(strcmp(keyword, "DataStd:")==0){
			fscanf(fp, "%f", &stdDev);
			dataStdFlag = 1;
		}
		else if(strcmp(keyword, "FirstDataByte:")==0){
			fscanf(fp, "%d", &beginningOfData);
		}
		else {
			fscanf(fp, "%s", charValue);		
		}
	}
	curLoc = beginningOfData;
	fclose(fp);

	//*******calc the stdDev if not in header*******
	if(dataStdFlag == 0){
		float* tempBuf;
		tempBuf = new float[nrow*ncol];
		if(tempBuf==NULL){
		   AfxMessageBox("Error in converting project.");
		   return;
		}
	
		if (type != 4) {
			stdDev = -99.0; 
		}
		else {
			if ((fp=fopen(layerFileName, "rb"))==NULL){
				AfxMessageBox("Error in converting project.");
				return;
			}
			if(beginningOfData != 0)
			fseek(fp, beginningOfData, SEEK_SET);

			fread(tempBuf, sizeof(float), nrow*ncol, fp);
			fclose(fp);

			stdDev = calcStdDev(tempBuf, nrow*ncol, theSetting->nodata);
			if(tempBuf!=NULL){
			  delete [] tempBuf;
			  tempBuf = NULL;
			}
		}
	}

	//nrow = CHUNK;

	layerMatrix = new float[nrow*ncol];
	if(layerMatrix==NULL){
		AfxMessageBox("Error in converting project.");
		return;
	}
	for(int i=0; i<nrow; i++)   //initialize the images
		for(int j=0; j<ncol; j++)
			layerMatrix[i*ncol+j]= -1.0;

	//this part is to set the width
	if(strcmp(widthS, "default")!=0){
		sscanf(widthS, "%f", &width);
	}
	else {
		if (type != 4) {
			width = (-99);		//set the width to Feng's favorite number if the type is not ratio
		}
		else {
			width = WIDTHFACTOR*stdDev;
		}
	}
}


//---------------------------------------------------------------------------------------------------
void OldLayerListNode::read3DFile(char *CopyDataPath){
	FILE *fp;
	if ((fp=fopen(layerFileName, "rb"))==NULL){
		AfxMessageBox("Error in converting project.");
		fclose(fp);
		return;
	}

	fseek(fp, curLoc, SEEK_SET);
	fread(layerMatrix, sizeof(float), nrow*ncol, fp);
	curLoc = ftell(fp);
	fclose(fp);	

	CString oldPath,newPath;
	oldPath.Format("%s",layerFileName);
	newPath.Format("%s",CopyDataPath);
	CString layerName = extractFileName(oldPath);  
	newPath+="\\";
	newPath+=layerName;
	if(!CopyFile(oldPath,newPath,false))  //copy env layer to current project directory
	{
		AfxMessageBox("Error in converting project.");
	}
}

//---------------------------------------------------------------------------------------------------
OldLayerListNode::~OldLayerListNode()
{
	if(layerMatrix!=NULL){
		delete [] layerMatrix;
		layerMatrix = NULL;
	} 
	nextLayer = NULL;
}
//---------------------------------------------------------------------------------------------------
OldLayerList::OldLayerList()
{
  numOfLayers = 0;
  firstLayer = NULL;
}
//---------------------------------------------------------------------------------------------------
OldLayerList::OldLayerList(OldLayerListNode* aNode)
{
  numOfLayers = 1;
  firstLayer = aNode;
  firstLayer->nextLayer = NULL;
}

//---------------------------------------------------------------------------------------------------
OldLayerList::~OldLayerList()
{
	OldLayerListNode* tempNode = firstLayer;
	while(firstLayer!=NULL){
		firstLayer = tempNode->nextLayer;
		delete tempNode;
		tempNode = firstLayer;
	}
}
//---------------------------------------------------------------------------------------------------
void OldLayerList::addNode(OldLayerListNode *aNewNode)
{
	OldLayerListNode* tempNode = firstLayer;
	while(tempNode->nextLayer!=NULL) 
		tempNode = tempNode->nextLayer;
	tempNode->nextLayer = aNewNode;
	aNewNode->nextLayer = NULL;
	numOfLayers++;
}

//---------------------------------------------------------------------------------------------------
OldEnvData::OldEnvData(OldSetting* aSetting) // The constructor of EnvData
{
  numOfLayers=0;
  readEnvData(aSetting);
}
//---------------------------------------------------------------------------------------------------


OldEnvData::~OldEnvData()//The destructor of EnvData
{ 
	if(theLayerList!=NULL){
	   delete theLayerList;
	   theLayerList = NULL;
	}
	
}
//---------------------------------------------------------------------------------------------------
void OldEnvData::readEnvData(OldSetting* aSetting)
{
	theSetting = aSetting;
	char cellFileName[FN_LEN];
	strcpy(cellFileName, theSetting->workDir);
	strcat(cellFileName, "header");
	strcat(cellFileName, SLASH);
	strcat(cellFileName, theSetting->theCellnameFile);
	ifstream cellname;
	cellname.open(cellFileName);
	if(cellname.fail()){
		AfxMessageBox("Error in converting project.");
		return;
	}
	char oneRecord[4*FN_LEN];
	PARTS * fields;
	int numFields;
	
	/******* handle new type of cell_names here *******/
	cellname.getline(oneRecord, 4*FN_LEN,'\n');	//read in the first line

	if(strlen(oneRecord)==0){
		AfxMessageBox("Error in converting project.");
		cellname.close();
		return;
	}
	numFields = countPieces(oneRecord);
	if ((fields = (PARTS *) malloc(numFields*sizeof(PARTS)))==NULL)
	{
		AfxMessageBox("Error in converting project.");
		cellname.close();
		return;
	}
	if (numFields == 2)
	{
		retrievePieces(oneRecord, fields, numFields);
		OldLayerListNode* theFirstLayer = new OldLayerListNode(fields[0].piece, fields[1].piece, theSetting);
		theLayerList = new OldLayerList(theFirstLayer);
		numOfLayers=1;
	}
	else if (numFields == 3)
	{
		retrievePieces(oneRecord,fields,numFields);
		OldLayerListNode* theFirstLayer = new OldLayerListNode(fields[0].piece, fields[1].piece,theSetting, fields[2].piece);
		theLayerList = new OldLayerList(theFirstLayer);
		numOfLayers=1;
	}
	else if (numFields == 4)
	{		
		retrievePieces(oneRecord,fields,numFields);	
		OldLayerListNode* theFirstLayer = new OldLayerListNode(fields[0].piece, fields[1].piece, theSetting, fields[2].piece, fields[3].piece);
		theLayerList = new OldLayerList(theFirstLayer);
		numOfLayers=1;
	}
	else {
		AfxMessageBox("Error in converting project.");
		free(fields);
		cellname.close();
		return;
	}
	free(fields);

	while(!cellname.eof())
	{ //read until end of file
		cellname.getline(oneRecord, 4*FN_LEN,'\n');	//read in A line
		if(strlen(oneRecord)==0) break;
		numFields = countPieces(oneRecord);

		if ((fields = (PARTS *) malloc(numFields*sizeof(PARTS)))==NULL)
	    {
		   AfxMessageBox("Error in converting project.");
		   cellname.close();
		   return;
	    }

		if (numFields == 2)
		{
			retrievePieces(oneRecord, fields, numFields);			
			OldLayerListNode* aNewLayer = new OldLayerListNode(fields[0].piece, fields[1].piece, theSetting);
			theLayerList->addNode(aNewLayer);	
			numOfLayers++;
		}
		else if (numFields == 3)
		{
			retrievePieces(oneRecord,fields,numFields);			
			OldLayerListNode* aNewLayer = new OldLayerListNode(fields[0].piece, fields[1].piece,theSetting, fields[2].piece);
			theLayerList->addNode(aNewLayer);
			numOfLayers++;

		}
		else if (numFields == 4)
		{
			retrievePieces(oneRecord,fields,numFields);			
			OldLayerListNode* aNewLayer = new OldLayerListNode(fields[0].piece, fields[1].piece, theSetting, fields[2].piece, fields[3].piece);
			theLayerList->addNode(aNewLayer);
			numOfLayers++;
		}
		else 
		{
		    AfxMessageBox("Error in converting project.");
			free(fields);
			cellname.close();
			return;
		}
		free(fields);
	}
	
	
	cellname.close();

}
//---------------------------------------------------------------------------------------------------
void OldEnvData::refreshEnvData(char *CopyDataPath)
{
	
	OldLayerListNode* tempNode = theLayerList->firstLayer;
	while(tempNode!=NULL){ 
		tempNode->read3DFile(CopyDataPath);
		tempNode = tempNode->nextLayer;
	}

}
//---------------------------------------------------------------------------------------------------
void OldEnvData::resetBeginning()
{
	OldLayerListNode* tempNode = theLayerList->firstLayer;
	while(tempNode!=NULL){ 
		tempNode->curLoc = tempNode->beginningOfData;
		tempNode = tempNode->nextLayer;
	}

}



//---------------------------------------------------------------------------------------------------
//Width Class
WidthRecord::WidthRecord(char theEnviron[], float theWidth, int theFunctionType,bool theUseOrNot)
{
	strcpy(environVariable, theEnviron);
	width = theWidth;
	functionType = theFunctionType;
	nextRecord = NULL;
	useOrNot = theUseOrNot;
}
WidthRecord::~WidthRecord()
{
}
//---------------------------------------------------------------------------------------------------
WidthList::WidthList()
{
	numWidthRecords = 0;
	firstWidthRecord = NULL;
}
//---------------------------------------------------------------------------------------------------
WidthList::~WidthList()
{
	WidthRecord* tempRecord = firstWidthRecord;
	while(firstWidthRecord!=NULL){
		firstWidthRecord = tempRecord->nextRecord;
		delete tempRecord;
		tempRecord = firstWidthRecord;
	}
}
//---------------------------------------------------------------------------------------------------
void WidthList::addWidthRecord(WidthRecord* aRecord)
{
	if(firstWidthRecord == NULL){
		firstWidthRecord = aRecord;
	}
	else{
		WidthRecord* currRecord = firstWidthRecord;
		while(currRecord->nextRecord!=NULL){
			currRecord = currRecord->nextRecord;
		}
		currRecord->nextRecord = aRecord;
		aRecord->nextRecord = NULL;			//actually no need
	}
	numWidthRecords = numWidthRecords + 1;
}



//---------------------------------------------------------------------------------------------------

//Case Class
OldCaseAttr::~OldCaseAttr()
{
	nextCaseAttr = NULL;
}
OldCaseNode::OldCaseNode()
{
	x = -1.0;
	y = -1.0;
	firstAttr = NULL;
	nextCase = NULL;
}
//---------------------------------------------------------------------------------------------------
OldCaseNode::OldCaseNode(double a, double b)
{
	x = a;
	y = b;
	firstAttr = NULL;
	nextCase = NULL;
}
//---------------------------------------------------------------------------------------------------
OldCaseNode::~OldCaseNode()
{
	OldCaseAttr *tempAttr = firstAttr;
    if(firstAttr!=NULL){
		firstAttr = tempAttr->nextCaseAttr;
		delete tempAttr;
		tempAttr = firstAttr;
	}
	nextCase = NULL;

}
//---------------------------------------------------------------------------------------------------
OldCaseList::OldCaseList()
{
	strcpy(soilType,"");
	soilID = -1;
	length = 0;
	hasEffectiveCase = 0;
	firstCase = NULL;
	nextList = NULL;
}
//---------------------------------------------------------------------------------------------------
OldCaseList::OldCaseList(char theType[], int theID, WidthList* theWidthList)
{
	strcpy(soilType, theType);
	soilID = theID;
	widthList = theWidthList;
	length = 0;
	hasEffectiveCase = 0;
	firstCase = NULL;
	nextList = NULL;
}

//---------------------------------------------------------------------------------------------------
OldCaseList::OldCaseList(const OldCaseList& aCaseList)
{
	strcpy(soilType, aCaseList.soilType);
	soilID = aCaseList.soilID;
	length = aCaseList.length;
	hasEffectiveCase = aCaseList.hasEffectiveCase;
	nextList = aCaseList.nextList;
	OldCaseNode* tempCase = aCaseList.firstCase;
	if(tempCase=NULL)
		firstCase = NULL;
	else{
		OldCaseNode* end = new OldCaseNode;
		if(end == NULL){
			AfxMessageBox("Error in converting project.");
			return;
		}
		end->x = tempCase->x;
		end->y = tempCase->y;
		firstCase = end;

		tempCase = tempCase->nextCase;
		while(tempCase!=NULL){
			end->nextCase = new OldCaseNode;
			if(end->nextCase == NULL){
				AfxMessageBox("Error in converting project.");
				return;
			}
			end = end->nextCase;
			end->x = tempCase->x;
			end->y = tempCase->y;
			tempCase = tempCase->nextCase;
		}
		end->nextCase = NULL;
	}
}
//---------------------------------------------------------------------------------------------------
OldCaseList::~OldCaseList()
{
	OldCaseNode* tempCase = firstCase;
	while(firstCase!=NULL){
		firstCase = tempCase->nextCase;
		delete tempCase;
		tempCase = firstCase;
	}
}
//---------------------------------------------------------------------------------------------------
OldCaseBase::OldCaseBase()
{
	theFirstCaseList = NULL;
	numOfSoilTypes = 0; 
}
//---------------------------------------------------------------------------------------------------
OldCaseBase::OldCaseBase(OldSetting* theSetting, OldEnvData* theEnvData)
{
	theFirstCaseList = NULL;
	flagForTheBigLoop = 0;
	buildCaseBase(theSetting, theEnvData);
}

//---------------------------------------------------------------------------------------------------
OldCaseBase::~OldCaseBase()
{

	  OldCaseList* tempCaseList = theFirstCaseList;
	  while(theFirstCaseList!=NULL){
		 theFirstCaseList = tempCaseList->nextList;
		 delete tempCaseList;
		 tempCaseList = theFirstCaseList;
	 }
	
}
//---------------------------------------------------------------------------------------------------
void OldCaseBase::buildCaseBase(OldSetting* theSetting, OldEnvData* theEnvData)
{
	ifstream fin_pnt, fin_lab;
	int flag = 0;

	IdSoilIndexList theIndex;
  
	char theLabFile[FN_LEN];
	strcpy(theLabFile, theSetting->workDir);
	strcat(theLabFile, "knowledge");
	strcat(theLabFile, SLASH);
	strcat(theLabFile, theSetting->theLabFile);
	fin_lab.open(theLabFile);
	if(fin_lab.fail()){
		AfxMessageBox("Error in converting project.");
		return;
	}
  
	numOfSoilTypes=0;
	while(!fin_lab.eof())
	{
		int id;
		char soilType[STRLEN];
		fin_lab>>id>>soilType;

		if(strcmp(soilType,"")==0 )   //because eof() function has some problem,it will read the last line twice
		{
			break;
		}
		//strcpy(prevSoilType, soilType);

		//build the IdSoilIndex (a copy of lab file)
		IdSoilIndexNode* aNewIndexNode = new IdSoilIndexNode(id, soilType);
		if(aNewIndexNode == NULL)
		{
			AfxMessageBox("Error in converting project.");
			fin_lab.close();
			return;
		}
		if(theIndex.theFirstNode==NULL)
			theIndex.theFirstNode=aNewIndexNode;
		else{
			IdSoilIndexNode* tempNode=theIndex.theFirstNode;
			while(tempNode->nextIndexNode!=NULL)
				tempNode=tempNode->nextIndexNode;
			tempNode->nextIndexNode=aNewIndexNode;
		}
    
		//build the skeleton of the casebase.
		//If the newly read soiltype has existed in the casebase,
		//do nothing, else, create a new CaseList whose soiltype
		//equal to the newly read soiltype.
		OldCaseList* tempList = theFirstCaseList;
    
		while(tempList!=NULL){
			if(strcmp(soilType, tempList->soilType)==0||strcmp(soilType,"")==0){
				flag=1; //this soil type has been in the case base
				break;
			}
			tempList=tempList->nextList;
		}
    
		if(flag==0){

			WidthList* aWidthList = new WidthList;

			//read from the corresponding width file
			FILE *fin_width;
			char theWidthFile[FN_LEN];
			strcpy(theWidthFile, theSetting->workDir);
			strcat(theWidthFile, "relation");
			strcat(theWidthFile, SLASH);
			strcat(theWidthFile, soilType);
			strcat(theWidthFile, ".wth");

			if ((fin_width=fopen(theWidthFile, "r"))==NULL) {//using default width & type
	
				OldLayerListNode* tempLayer = theEnvData->theLayerList->firstLayer;

				while(tempLayer!=NULL){
					WidthRecord* aRecord = new WidthRecord(tempLayer->layerName,tempLayer->width,1,true);
					aWidthList->addWidthRecord(aRecord);
					tempLayer = tempLayer->nextLayer;
				}
			}
			else {
				char oneRecord[4*STRLEN];
				PARTS * fields;
				fields = NULL;
				int numFields;
				int line = 0, len, status;
			
				do {
					status=read_record(fin_width, &len, oneRecord); //read until end of file
					if (len==0) {
						if (line == 0) { 
							AfxMessageBox("Error in converting project.");
							fin_lab.close();
							fclose(fin_width);
							return;
						}
						else {
							continue;
						}
					}
					line ++;
					numFields = countPieces(oneRecord);  //assume 4 fields for each line
					if(numFields != 4){
						AfxMessageBox("Error in converting project.");
						fin_lab.close();
						fclose(fin_width);
						return;
					}
					if ((fields = (PARTS *) malloc(numFields*sizeof(PARTS)))==NULL) {
						AfxMessageBox("Error in converting project.");
						fin_lab.close();
						fclose(fin_width);
						return;
					}
					retrievePieces(oneRecord, fields, numFields);


					if(strcmp(fields[1].piece,"1")==0){ //useOrNot = 1 means "use it"
						float width;
						if(strcmp(fields[2].piece, "default")!=0){ //user specified width
							sscanf(fields[2].piece, "%f", &width);
						}
						else { //go to layerlistNode to find it
							OldLayerListNode* tempLayer = theEnvData->theLayerList->firstLayer;
							while(tempLayer!=NULL&&strcmp(tempLayer->layerName, fields[0].piece)!=0){
								tempLayer = tempLayer->nextLayer;
							}
							if (tempLayer == NULL){
								AfxMessageBox("Error in converting project.");
								fin_lab.close();
								fclose(fin_width);
								free(fields);
								return;
							}
							width = tempLayer->width;
						}
	
						int funcType;
						sscanf(fields[3].piece, "%d", &funcType); //assume no occurence of "default", 0,1,2,3 only
						WidthRecord* aRecord = new WidthRecord(fields[0].piece,width,funcType,true);
						aWidthList->addWidthRecord(aRecord);
					}
					else if(strcmp(fields[1].piece,"1")!=0){ //useOrNot != 1 means "not use it"
						float width;
						if(strcmp(fields[2].piece, "default")!=0){ //user specified width
							sscanf(fields[2].piece, "%f", &width);
						}
						else { //go to layerlistNode to find it
							OldLayerListNode* tempLayer = theEnvData->theLayerList->firstLayer;
							while(tempLayer!=NULL&&strcmp(tempLayer->layerName, fields[0].piece)!=0){
								tempLayer = tempLayer->nextLayer;
							}
							if (tempLayer == NULL){
								AfxMessageBox("Error in converting project.");
								fin_lab.close();
								fclose(fin_width);
								free(fields);
								return;
							}
							width = tempLayer->width;
						}
	
						int funcType;
						sscanf(fields[3].piece, "%d", &funcType); //assume no occurence of "default", 0,1,2,3 only
						WidthRecord* aRecord = new WidthRecord(fields[0].piece,width,funcType,false);
						aWidthList->addWidthRecord(aRecord);
					}


				} while (status != (-1));

				fclose(fin_width);
				
				free(fields);
			}
			tempList = new OldCaseList(soilType, id, aWidthList);
			numOfSoilTypes++;
			if(theFirstCaseList==NULL){
				theFirstCaseList=tempList;
				theFirstCaseList->nextList=NULL;
			}
			else{
				OldCaseList* theLastCaseList = theFirstCaseList;
				while(theLastCaseList->nextList!=NULL)
					theLastCaseList=theLastCaseList->nextList;
				theLastCaseList->nextList=tempList;
				tempList->nextList=NULL;
			}
		}
		
	}

    fin_lab.close();


	char thePntFile[FN_LEN];
	strcpy(thePntFile, theSetting->workDir);
	strcat(thePntFile, "knowledge");
	strcat(thePntFile, SLASH);
	strcat(thePntFile, theSetting->thePntFile);
	fin_pnt.open(thePntFile);
	if(fin_pnt.fail()){
		AfxMessageBox("Error in converting project.");
		return;
	}
	
	//add cases (x and y) to the casebase skeleton.
	//use the index to locate to which CaseList the new
	//case should be appended.
	int prevID=-1;
	double prevX,prevY;
	while(!fin_pnt.eof()){
		int id;
		double x, y;
		fin_pnt>>id>>x>>y;
		if(id==prevID&&x==prevX&&y==prevY)  //because eof() function has some problem,it will read the last line twice
		{
			break;
		}
		prevID = id;
		prevX = x;
		prevY = y;

		if(theIndex.theFirstNode==NULL){
			AfxMessageBox("Error in converting project.");
			fin_pnt.close();
			return;

		}
		else{
			IdSoilIndexNode* tempNode=theIndex.theFirstNode;
			while(tempNode!=NULL){
				if(tempNode->theID==id){
					OldCaseList* tempCaseList = theFirstCaseList;
					if(tempCaseList==NULL){
						AfxMessageBox("Error in converting project.");
						fin_pnt.close();			     
						return;
					}
					while(tempCaseList!=NULL){
						if(strcmp(tempNode->theSoilType,tempCaseList->soilType)==0){ 
						//hook the case to the corresponding case list (soil type)
							OldCaseNode* aNewCase = new OldCaseNode(x, y);
							if(aNewCase==NULL)
							{
								AfxMessageBox("Error in converting project.");
								fin_pnt.close();
								return;
							}
							if(tempCaseList->length==0)
								tempCaseList->firstCase = aNewCase;
							else{
								OldCaseNode* tempCaseNode = tempCaseList->firstCase;
								while(tempCaseNode->nextCase!=NULL)
									tempCaseNode = tempCaseNode->nextCase;
								tempCaseNode->nextCase = aNewCase;
							}
							tempCaseList->length++;
							break;
						}
						tempCaseList=tempCaseList->nextList;
					}
					break;
				}
				tempNode = tempNode->nextIndexNode;
			} 
		}
	//	prevID=id;
	}	

	fin_pnt.close();
}
//---------------------------------------------------------------------------------------------------
void OldCaseBase::fillCaseAttr(OldEnvData* theEnvData)//, int theLine)
{
	double north = theEnvData->theSetting->north;
	double south = theEnvData->theSetting->south;
	double west = theEnvData->theSetting->west;
	double east = theEnvData->theSetting->east;
	int ncol = theEnvData->theSetting->ncol;
	int nrow = theEnvData->theSetting->nrow;
	float pixelsize = theEnvData->theSetting->pixelsize;

	OldCaseList* tempList = theFirstCaseList;
	while(tempList!=NULL){ //scan through the case groups of soil types
	    OldCaseNode* tempCaseNode = tempList->firstCase;
		while(tempCaseNode!=NULL){ //scan through the cases in a group( a case is a instance)
			double x = tempCaseNode->x;
			double y = tempCaseNode->y;
			if(x<west || x>east || y>north || y<south){
				tempCaseNode = tempCaseNode->nextCase;	
				continue;
			}

			int row = (int)((north-y)/pixelsize);
			//if(fabs(north-pixelsize*(theLine+CHUNK)-y)<0.0001)
				//row = CHUNK-1;
			if(fabs(south-y)<0.0001)
				//row = (int)((north-y)/pixelsize-theLine)-1;
             row = (int)((north-y)/pixelsize)-1;

			int col = (int)((x-west)/pixelsize);
   
			if(fabs(east-x)<0.0001)
				col = ncol-1;

			if(row>=0 &&row<nrow&&/*&& row<CHUNK*/col>=0 && col<ncol)
			{
				tempList->hasEffectiveCase = 1;
				OldLayerListNode* tempLayerNode = theEnvData->theLayerList->firstLayer;
				if(tempLayerNode == NULL){
					AfxMessageBox("Error in converting project.");
					return;
				}
				OldCaseAttr* aCaseAttr = new OldCaseAttr;
				aCaseAttr->attrValue = tempLayerNode->layerMatrix[row*ncol+col];
				aCaseAttr->nextCaseAttr = NULL;
				tempCaseNode->firstAttr = aCaseAttr;
				while(tempLayerNode->nextLayer!=NULL)
				{ 
					tempLayerNode = tempLayerNode->nextLayer;
					OldCaseAttr* endAttr = new OldCaseAttr;
					endAttr->attrValue = tempLayerNode->layerMatrix[row*ncol+col];
					endAttr->nextCaseAttr = NULL;
					aCaseAttr->nextCaseAttr = endAttr;
					aCaseAttr = endAttr;
				}
			}
			tempCaseNode = tempCaseNode->nextCase;
		}
		tempList = tempList->nextList;
	}
}
//---------------------------------------------------------------------------------------------------


//SoilIndex Class
IdSoilIndexNode::IdSoilIndexNode()
{
  theID = -1;
  strcpy(theSoilType,"");
}
//---------------------------------------------------------------------------------------------------
IdSoilIndexNode::IdSoilIndexNode(int id, char soilType[])
{
  strcpy(theSoilType, soilType);
  theID = id;
  nextIndexNode = NULL;
  
}
//--------------------------------------------------------------------------------------------------
IdSoilIndexNode::~IdSoilIndexNode()
{
	this->nextIndexNode=NULL;	
}
//---------------------------------------------------------------------------------------------------
IdSoilIndexList::IdSoilIndexList()
{
  theFirstNode = NULL;
}
//---------------------------------------------------------------------------------------------------
IdSoilIndexList::IdSoilIndexList(const IdSoilIndexList& anIdSoilIndexList)
{
  IdSoilIndexNode* tempNode = anIdSoilIndexList.theFirstNode;
  if(tempNode==NULL)
    theFirstNode = NULL;
  else{
    IdSoilIndexNode* end = new IdSoilIndexNode(tempNode->theID,tempNode->theSoilType);
    if(end == NULL){
		AfxMessageBox("Error in converting project.");
        return;
    }
    theFirstNode = end;
    
    tempNode = tempNode->nextIndexNode;
    while(tempNode!=NULL){
      end->nextIndexNode = new IdSoilIndexNode;
      if(end->nextIndexNode == NULL){
         AfxMessageBox("Error in converting project.");
	     return;
      }
      end = end->nextIndexNode;
      strcpy(end->theSoilType, tempNode->theSoilType);
      end->theID = tempNode->theID;
      tempNode = tempNode->nextIndexNode;
    }
    end->nextIndexNode = NULL;
  }
}
//---------------------------------------------------------------------------------------------------
IdSoilIndexList::~IdSoilIndexList(){
  IdSoilIndexNode* tempNode = theFirstNode;
  while(theFirstNode!=NULL){
    theFirstNode = tempNode->nextIndexNode;
    delete tempNode;
    tempNode = theFirstNode;
  }
}


//Convert Class

ImportOldPrj::ImportOldPrj()
{
	theCaseBase = NULL;
	theEnvData = NULL;
	theSetting = NULL;

}

ImportOldPrj::~ ImportOldPrj()
{
	if(theCaseBase!=NULL)
	{
		delete theCaseBase;
		theCaseBase = NULL;
	}
	if(theEnvData!=NULL)
	{
	
		delete theEnvData;
		theEnvData = NULL;
	}
	if(theSetting !=NULL)
	{
		delete theSetting;
		theSetting = NULL;
	}
}


void ImportOldPrj::WriteProject(CString strFileName, CProject *pProject)
{
	MSXML2::IXMLDOMDocumentPtr pDoc;
	HRESULT hr;

	hr=CoInitialize(NULL);
	if(FAILED(hr))
	{
		AfxMessageBox("Error in converting project!");
		return;
	}

	hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));
	if(FAILED(hr))
	{
		AfxMessageBox("Error in converting project!");
		return;
	}
	pDoc->preserveWhiteSpace=TRUE;

	MSXML2::IXMLDOMProcessingInstructionPtr pPI = NULL;
	pPI = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
	_variant_t vNullVal;
	vNullVal.vt = VT_NULL;
	pDoc->insertBefore(pPI, vNullVal);
	pPI.Release();


	//*********** create root node "SoLIMProject"***************************
	MSXML2::IXMLDOMElementPtr pRootElem;
	pRootElem=pDoc->createElement("SoLIMProject");
	//create the attributes of the root node
	MSXML2::IXMLDOMAttributePtr attrib;
	CString attri;
	attrib=pDoc->createAttribute("Version");
	if(attrib!=NULL)
	{
		attri="5.0";
		_variant_t varattri(attri);
		attrib->value=varattri;
		pRootElem->setAttributeNode(attrib);

	}
	attrib.Release();
	attrib=pDoc->createAttribute("Name");
	if(attrib!=NULL)
	{
		attri=pProject->getProjectName();
		_variant_t varattri(attri);
		attrib->value=varattri;
		pRootElem->setAttributeNode(attrib);

	}
	attrib.Release();
	attrib=pDoc->createAttribute("DataType");
	if(attrib!=NULL)
	{
		attrib->value="3dr";
		pRootElem->setAttributeNode(attrib);

	}
	attrib.Release();
	//add the root node to the DOM instance
	pDoc->appendChild(pRootElem);
	MSXML2::IXMLDOMTextPtr   pTextNode; 

	//**********  Create database node ***************
	MSXML2::IXMLDOMElementPtr pDatabaseNode;
	pDatabaseNode=pDoc->createElement("Database");
	if(pDatabaseNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n "); 
		pRootElem->appendChild(pTextNode);
		pTextNode.Release();
		pRootElem->appendChild(pDatabaseNode);
	}
	//append layer node
	MSXML2::IXMLDOMElementPtr pLayernode;
	for(int n1=0;n1<pProject->m_pEnvDatabase->getLayerNum();n1++)
	{
		pLayernode=pDoc->createElement("layer");
		if(pLayernode!=NULL)
		{
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pDatabaseNode->appendChild(pTextNode);
			pTextNode.Release();		
			pDatabaseNode->appendChild(pLayernode);
			pTextNode = pDoc->createTextNode(L"\n   "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();
			MSXML2::IXMLDOMElementPtr pLayerNameNode,pLayerPathNode;
			pLayerNameNode=pDoc->createElement("Name");
			CString layername=pProject->m_pEnvDatabase->getLayer(n1)->getLayerName();
			BSTR layername_bstr=layername.AllocSysString();
			pLayerNameNode->text=layername_bstr;
			pLayernode->appendChild(pLayerNameNode);
			pTextNode = pDoc->createTextNode(L"\n   "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();
			pLayerPathNode=pDoc->createElement("Path");
			CString filename=extractFileName(pProject->m_pEnvDatabase->getLayer(n1)->getFileName());
			BSTR filename_bstr=filename.AllocSysString();
			pLayerPathNode->text=filename_bstr;
			pLayernode->appendChild(pLayerPathNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

		}
		pLayernode.Release();

	}   

	pTextNode = pDoc->createTextNode(L"\n "); 
	pDatabaseNode->appendChild(pTextNode);
	pTextNode.Release();
	pDatabaseNode.Release();


	//************* Create KnowledgeBase node  ****************
	MSXML2::IXMLDOMElementPtr pKnowledgebaseNode;
	pKnowledgebaseNode=pDoc->createElement("Knowledgebase");
	if(pKnowledgebaseNode!=NULL)
	{  
		pTextNode = pDoc->createTextNode(L"\n "); 
		pRootElem->appendChild(pTextNode);
		pTextNode.Release();
		pRootElem->appendChild(pKnowledgebaseNode);
	}

	MSXML2::IXMLDOMElementPtr pSoilNameNode;
	for (int n2=0;n2<pProject->m_pKnowledgeBase->getSoilTypeNum();n2++)
	{
		pSoilNameNode=pDoc->createElement("SoilType");
		if(pSoilNameNode!=NULL)
		{
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pKnowledgebaseNode->appendChild(pTextNode);
			pTextNode.Release();

			attrib=pDoc->createAttribute("Name");
			if(attrib!=NULL)
			{
				CString soiltype=pProject->m_pKnowledgeBase->getSoilType(n2)->getSoilTypeName();
				_variant_t soiltype_var((LPCTSTR)soiltype);
				attrib->value=soiltype_var;
				pSoilNameNode->setAttributeNode(attrib);
				attrib.Release();
			}
			pKnowledgebaseNode->appendChild(pSoilNameNode);

			//instances
			MSXML2::IXMLDOMElementPtr pInstancesNode;
			pInstancesNode=pDoc->createElement("Instances");
			if(pInstancesNode!=NULL)
			{
				pTextNode = pDoc->createTextNode(L"\n   "); 
				pSoilNameNode->appendChild(pTextNode);
				pTextNode.Release();

				pSoilNameNode->appendChild(pInstancesNode);

				MSXML2::IXMLDOMElementPtr pInstance;
				for(int n3=0;n3<pProject->m_pKnowledgeBase->getSoilType(n2)->getInstanceNum();n3++)
				{
					pInstance=pDoc->createElement("Instance");
					if(pInstance!=NULL)
					{
						pTextNode = pDoc->createTextNode(L"\n    "); 
						pInstancesNode->appendChild(pTextNode);
						pTextNode.Release();

						attrib=pDoc->createAttribute("Name");
						if(attrib!=NULL)
						{
							CString instancename=pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getInstanceName();
							_variant_t instancename_var((LPCTSTR)instancename);
							attrib->value=instancename_var;
							pInstance->setAttributeNode(attrib);

						}
						attrib.Release();
						pInstancesNode->appendChild(pInstance);

						MSXML2::IXMLDOMElementPtr pRuleNode;
						for(int n4=0;n4<pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRuleNum();n4++)
						{
							pRuleNode=pDoc->createElement("Rule");
							if(pRuleNode!=NULL)
							{
								pTextNode = pDoc->createTextNode(L"\n     "); 
								pInstance->appendChild(pTextNode);
								pTextNode.Release();

								attrib=pDoc->createAttribute("Name");
								if(attrib!=NULL)
								{
									CString rulename=pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleName();
									_variant_t rulename_var((LPCTSTR)rulename);
									attrib->value=rulename_var;
									pRuleNode->setAttributeNode(attrib);								
								}
								attrib.Release();

								attrib=pDoc->createAttribute("UseOrNot");
								if(attrib!=NULL)
								{
									if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getIsUsed()==true)
										attrib->value="true";
									else 
										attrib->value="false";

									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDoc->createAttribute("Type");
								if(attrib!=NULL)
								{
									int a=pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType();
									switch(a)
									{
									case 0:
										attrib->value="Range";
										break;
									case 1:
										attrib->value="Curve";
										break;
									case 2:
										attrib->value="Word";
										break;
									case 3:
										attrib->value="Point";									
										break;
									case 4:
										attrib->value="Enumerated";										
										break;

									}
									pRuleNode->setAttributeNode(attrib);


								}
								attrib.Release();

								attrib=pDoc->createAttribute("DataLayer");
								if(attrib!=NULL)
								{
									CString datalayername=pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getLayerName();
									_variant_t datalayername_var((LPCSTR)datalayername);
									attrib->value=datalayername_var;
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();
								pInstance->appendChild(pRuleNode);



								//Range
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 0)
								{
									CRangeRule *m_pRange;;
									m_pRange=(CRangeRule *)(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));

									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDoc->createElement("CurveType");
									if(pCurveTypeNode!=NULL)
									{
										int curvT=m_pRange->getCurveType();
										switch(curvT)
										{
										case 0:
											pCurveTypeNode->text="Bell-shape";
											break;
										case 1:
											pCurveTypeNode->text="S-shape";
											break;
										case 2:
											pCurveTypeNode->text="Z-shape";
											break;
										}
										pRuleNode->appendChild(pCurveTypeNode);
									}
									pCurveTypeNode.Release();
									MSXML2::IXMLDOMElementPtr pKeyValueNode;
									pKeyValueNode=pDoc->createElement("KeyValue");
									if(pKeyValueNode!=NULL)
									{
										double KeyValue[4];
										KeyValue[0]=m_pRange->getLowCross();
										KeyValue[1]=m_pRange->getLowUnity();
										KeyValue[2]=m_pRange->getHighUnity();
										KeyValue[3]=m_pRange->getHighCross();
										//char *s=new char[];
										CString str;
										int i2=0;
										for(int j2=0;j2<4;j2++)
										{
											CString tempStr;
											tempStr.Format("%f",KeyValue[j2]);
											if(j2!=3)
												str = str + tempStr+",";
											else 
												str = str + tempStr;

										}
										pKeyValueNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pKeyValueNode);
										//delete []s;
									}
									pKeyValueNode.Release();
								}

								//Curve
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 1)
								{
									CFreehandRule *m_pCurve; 
									m_pCurve= (CFreehandRule *)(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));
									MSXML2::IXMLDOMElementPtr pNodeNum,pCoordinatesNode;

									pNodeNum=pDoc->createElement("NodeNum");
									CString str;
									str.Format("%d",m_pCurve->getKnotNum());								
									pNodeNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pNodeNum);
									pNodeNum.Release();

									pCoordinatesNode=pDoc->createElement("Coordinates");
									if(pCoordinatesNode!=NULL)
									{
										int i1=0;
										CString str;
										for(int j=0;j<m_pCurve->getKnotNum();j++)
										{
											double x, y;
											CString tempString;
											m_pCurve->getKnot(j,&x,&y);
											tempString.Format("%f %f",x,y);
											if(j!=m_pCurve->getKnotNum()-1)
												str = str + tempString + ",";
											else 
												str = str + tempString; 
										}
										pCoordinatesNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pCoordinatesNode);
									}
									pCoordinatesNode.Release();
								}

								//Word
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 2)
								{
									CWordRule *m_pWord=new CWordRule;
									m_pWord=static_cast<CWordRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));
									MSXML2::IXMLDOMElementPtr pPraseNode;
									pPraseNode=pDoc->createElement("Prase");
									if(pPraseNode!=NULL)
									{
										CString phrase=m_pWord->getPhrase();
										_bstr_t phrase_bstr=phrase.AllocSysString();
										pPraseNode->text=phrase_bstr;
										pRuleNode->appendChild(pPraseNode);
									}
									pPraseNode.Release();

									MSXML2::IXMLDOMElementPtr pEnvAttriNode;
									pEnvAttriNode=pDoc->createElement("EnvAttri");
									if(pEnvAttriNode!=NULL)
									{
										CString EnvAttri=m_pWord->getEnvAttri();
										_bstr_t EnvAttri_bstr=EnvAttri.AllocSysString();
										pEnvAttriNode->text=EnvAttri_bstr;
										pRuleNode->appendChild(pEnvAttriNode);
									}
									pEnvAttriNode.Release();

									MSXML2::IXMLDOMElementPtr pWordLibNode;
									pWordLibNode=pDoc->createElement("WordLib");
									if(pWordLibNode!=NULL)
									{
										CString libfile=extractFileName(m_pWord->getLibFileName());
										_bstr_t libfile_bstr=libfile.AllocSysString();
										pWordLibNode->text=libfile_bstr;
										pRuleNode->appendChild(pWordLibNode);
									}
									pWordLibNode.Release();
								}


								//Point
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 3)
								{
									CPointRule *m_pPoint=new CPointRule;
									m_pPoint=static_cast<CPointRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));
									MSXML2::IXMLDOMElementPtr pXYNode;
									pXYNode=pDoc->createElement("XY");
									if(pXYNode!=NULL)
									{
										CString str;
										str.Format("%f %f",m_pPoint->getCentralX(),m_pPoint->getCentralY());
										pXYNode->text= (_bstr_t)str;
										pRuleNode->appendChild(pXYNode);
									}
									pXYNode.Release();
									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDoc->createElement("CurveType");
									if(pCurveTypeNode!=NULL)
									{
										int curvT=m_pPoint->getCurveType();
										switch(curvT)
										{
										case 0:
											pCurveTypeNode->text="Bell-shape";
											break;
										case 1:
											pCurveTypeNode->text="S-shape";
											break;
										case 2:
											pCurveTypeNode->text="Z-shape";
											break;
										}
										pRuleNode->appendChild(pCurveTypeNode);
									}
									pCurveTypeNode.Release();

									MSXML2::IXMLDOMElementPtr pLWidthNode;
									pLWidthNode=pDoc->createElement("LWidth");
							
									if(pLWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getLWidth());
										pLWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pLWidthNode);
									}
									pLWidthNode.Release();

									MSXML2::IXMLDOMElementPtr pRWidthNode;
									pRWidthNode=pDoc->createElement("RWidth");
								
									if(pRWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getRWidth());
										pRWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pRWidthNode);
									}
									pRWidthNode.Release();								
								}     

								//Enumerated
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 4)
								{
									CEnumRule *m_pEnum=new CEnumRule;
									m_pEnum=static_cast<CEnumRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));
									MSXML2::IXMLDOMElementPtr pItemNum,pEnumValueNode;

									pItemNum=pDoc->createElement("ItemNum");
									CString str;
									str.Format("%d",m_pEnum->getItemNum());								
									pItemNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pItemNum);
									pItemNum.Release();

									pEnumValueNode=pDoc->createElement("EnumValue");
									if(pEnumValueNode!=NULL)
									{
										int i3=0;
										CString str = "";
										for(int j3=0;j3<m_pEnum->getItemNum();j3++)
										{
											CString TempStr;
											TempStr.Format("%d",m_pEnum->getItem(j3));
											if(j3!=m_pEnum->getItemNum()-1)
											{
												str+=TempStr;
												str+=",";
											}
											else
												str+=TempStr;

										}
										pEnumValueNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pEnumValueNode);
									}
									pEnumValueNode.Release();
								}

							}
							pRuleNode.Release();
						}
					}
					pTextNode = pDoc->createTextNode(L"\n    "); 
					pInstance->appendChild(pTextNode);
					pTextNode.Release();
					pInstance.Release();
				}
			}
			pTextNode = pDoc->createTextNode(L"\n   "); 
			pInstancesNode->appendChild(pTextNode);
			pTextNode.Release();

			pInstancesNode.Release();


			//Occurrences
			MSXML2::IXMLDOMElementPtr pOccurrencesNode;
			pOccurrencesNode=pDoc->createElement("Occurrences");
			if(pOccurrencesNode!=NULL)
			{

				pTextNode = pDoc->createTextNode(L"\n   "); 
				pSoilNameNode->appendChild(pTextNode);
				pTextNode.Release();

				pSoilNameNode->appendChild(pOccurrencesNode);

				MSXML2::IXMLDOMElementPtr pOccurrence;
				for(int n5=0;n5<pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrenceNum();n5++)
				{
					pOccurrence=pDoc->createElement("Occurrence");
					if(pOccurrence!=NULL)
					{
						pTextNode = pDoc->createTextNode(L"\n    "); 
						pOccurrencesNode->appendChild(pTextNode);
						pTextNode.Release();

						attrib=pDoc->createAttribute("Name");
						if(attrib!=NULL)
						{
							CString occurname=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getOccurrenceName();
							_variant_t occurname_var((LPCSTR)occurname);
							attrib->value=occurname_var;
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDoc->createAttribute("x");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getCentralX();
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDoc->createAttribute("y");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getCentralY();
							pOccurrence->setAttributeNode(attrib);

						}	
						attrib.Release();						
						attrib=pDoc->createAttribute("SearchDist");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getSearchDist();
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDoc->createAttribute("UseDistanceDecay");
						if(attrib!=NULL)
						{
							if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getUseDistSim() == true)
							  attrib->value= "true";
							else
								attrib->value = "false";
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDoc->createAttribute("DistanceDecay");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getDistDecayFactor();
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						pOccurrencesNode->appendChild(pOccurrence);

						MSXML2::IXMLDOMElementPtr pRuleNode;
						for(int n6=0;n6<pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRuleNum();n6++)
						{
							pRuleNode=pDoc->createElement("Rule");
							if(pRuleNode!=NULL)
							{
								pTextNode = pDoc->createTextNode(L"\n     "); 
								pOccurrence->appendChild(pTextNode);
								pTextNode.Release();

								attrib=pDoc->createAttribute("Name");
								if(attrib!=NULL)
								{
									CString rulename=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleName();
									_variant_t rulename_var((LPCSTR)rulename);
									attrib->value=rulename_var;
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDoc->createAttribute("UseOrNot");
								if(attrib!=NULL)
								{
									if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n5)->getRule(n6)->getIsUsed()==true)
										attrib->value="true";
									else 
										attrib->value="false";

									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDoc->createAttribute("Type");
								if(attrib!=NULL)
								{
									int a=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType();
									switch(a)
									{
									case 0:
										attrib->value="Range";
										break;
									case 1:
										attrib->value="Curve";
										break;
									case 2:
										attrib->value="Word";
										break;
									case 3:
										attrib->value="Point";									
										break;
									case 4:
										attrib->value="Enumerated";										
										break;

									}
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDoc->createAttribute("DataLayer");
								if(attrib!=NULL)
								{
									CString layername=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getLayerName();
									_variant_t layername_var((LPCSTR)layername);
									attrib->value=layername_var;
									pRuleNode->setAttributeNode(attrib);

								}	
								attrib.Release();
								pOccurrence->appendChild(pRuleNode);


								//Range
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()==0)
								{
									CRangeRule *m_pRange = new CRangeRule;
									m_pRange=static_cast<CRangeRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));

									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDoc->createElement("CurveType");
									if(pCurveTypeNode!=NULL)
									{
										int curvT=m_pRange->getCurveType();
										switch(curvT)
										{
										case 0:
											pCurveTypeNode->text="Bell-shape";
											break;
										case 1:
											pCurveTypeNode->text="S-shape";
											break;
										case 2:
											pCurveTypeNode->text="Z-shape";
											break;
										}
										pRuleNode->appendChild(pCurveTypeNode);
									}
									pCurveTypeNode.Release();

									MSXML2::IXMLDOMElementPtr pKeyValueNode;
									pKeyValueNode=pDoc->createElement("KeyValue");
									if(pKeyValueNode!=NULL)
									{
										double KeyValue[4];
										KeyValue[0]=m_pRange->getLowCross();
										KeyValue[1]=m_pRange->getLowUnity();
										KeyValue[2]=m_pRange->getHighUnity();
										KeyValue[3]=m_pRange->getHighCross();
										//char *s=new char[];
										CString str;
										int i2=0;
										for(int j2=0;j2<4;j2++)
										{
											CString tempStr;
											tempStr.Format("%f",KeyValue[j2]);
											if(j2!=3)
												str = str + tempStr+",";
											else 
												str = str + tempStr;

										}
										pKeyValueNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pKeyValueNode);
										//delete []s;
									}
									pKeyValueNode.Release();
								}
								//Curve
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()== 1)
								{
									CFreehandRule *m_pCurve = new CFreehandRule;
									m_pCurve= static_cast<CFreehandRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));
									MSXML2::IXMLDOMElementPtr pNodeNum,pCoordinatesNode;
									pNodeNum=pDoc->createElement("NodeNum");
									CString str;
									str.Format("%d",m_pCurve->getKnotNum());								
									pNodeNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pNodeNum);
									pNodeNum.Release();

									pCoordinatesNode=pDoc->createElement("Coordinates");
									if(pCoordinatesNode!=NULL)
									{
										int i1=0;
										CString str;
										for(int j=0;j<m_pCurve->getKnotNum();j++)
										{
											double x, y;
											CString tempString;
											m_pCurve->getKnot(j,&x,&y);
											tempString.Format("%f %f",x,y);
											if(j!=m_pCurve->getKnotNum()-1)
												str = str + tempString + ",";
											else 
												str = str + tempString; 							
										}
										pCoordinatesNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pCoordinatesNode);
									}
									pCoordinatesNode.Release();
								}


								//Word
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()== 2)
								{
									CWordRule *m_pWord=new CWordRule;
									m_pWord=static_cast<CWordRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));
									MSXML2::IXMLDOMElementPtr pPraseNode;
									pPraseNode=pDoc->createElement("Prase");
									if(pPraseNode!=NULL)
									{
										CString phrase=m_pWord->getPhrase();
										_bstr_t phrase_bstr=phrase.AllocSysString();
										pPraseNode->text=phrase_bstr;
										pRuleNode->appendChild(pPraseNode);
									}
									pPraseNode.Release();


									MSXML2::IXMLDOMElementPtr pEnvAttriNode;
									pEnvAttriNode=pDoc->createElement("EnvAttri");
									if(pEnvAttriNode!=NULL)
									{
										CString EnvAttri=m_pWord->getEnvAttri();
										_bstr_t EnvAttri_bstr=EnvAttri.AllocSysString();
										pEnvAttriNode->text=EnvAttri_bstr;
										pRuleNode->appendChild(pEnvAttriNode);
									}
									pEnvAttriNode.Release();

									MSXML2::IXMLDOMElementPtr pWordLibNode;
									pWordLibNode=pDoc->createElement("WordLib");
									if(pWordLibNode!=NULL)
									{
										CString libfile=extractFileName(m_pWord->getLibFileName());
										_bstr_t libfile_bstr=libfile.AllocSysString();
										pWordLibNode->text=libfile_bstr;
										pRuleNode->appendChild(pWordLibNode);
									}
									pWordLibNode.Release();
								}


								//Point
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()== 3)
								{
									CPointRule *m_pPoint=new CPointRule;
									m_pPoint=static_cast<CPointRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));
									MSXML2::IXMLDOMElementPtr pXYNode;
									pXYNode=pDoc->createElement("XY");
									if(pXYNode!=NULL)
									{
										//char *XYNode=new char[];
										CString str;
										str.Format("%f %f",m_pPoint->getCentralX(),m_pPoint->getCentralY());
										pXYNode->text= (_bstr_t)str;
										pRuleNode->appendChild(pXYNode);
									}
									pXYNode.Release();	

									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDoc->createElement("CurveType");
									if(pCurveTypeNode!=NULL)
									{
										int curvT=m_pPoint->getCurveType();
										switch(curvT)
										{
										case 0:
											pCurveTypeNode->text="Bell-shape";
											break;
										case 1:
											pCurveTypeNode->text="S-shape";
											break;
										case 2:
											pCurveTypeNode->text="Z-shape";
											break;
										}
										pRuleNode->appendChild(pCurveTypeNode);
									}
									pCurveTypeNode.Release();

									MSXML2::IXMLDOMElementPtr pLWidthNode;
									pLWidthNode=pDoc->createElement("LWidth");
									
									if(pLWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getLWidth());
										pLWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pLWidthNode);
									}
									pLWidthNode.Release();

									MSXML2::IXMLDOMElementPtr pRWidthNode;
									pRWidthNode=pDoc->createElement("RWidth");
								
									if(pRWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getRWidth());
										pRWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pRWidthNode);
									}
									pRWidthNode.Release();
									//delete Width;
								}


								//Enumerated
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()== 4)
								{
									CEnumRule *m_pEnum=new CEnumRule;
									m_pEnum=static_cast<CEnumRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));
									MSXML2::IXMLDOMElementPtr pItemNum,pEnumValueNode;

									pItemNum=pDoc->createElement("ItemNum");
									CString str;
									str.Format("%d",m_pEnum->getItemNum());								
									pItemNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pItemNum);
									pItemNum.Release();
									pEnumValueNode=pDoc->createElement("EnumValue");
									if(pEnumValueNode!=NULL)
									{
										//char *EnumV=new char[];
										int i3=0;
										CString str = "";
										for(int j3=0;j3<m_pEnum->getItemNum();j3++)
										{
											CString TempStr;
											TempStr.Format("%d",m_pEnum->getItem(j3));
											if(j3!=m_pEnum->getItemNum()-1)
											{
												str+=TempStr;
												str+=",";
											}
											else
												str+=TempStr;

										}
										pEnumValueNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pEnumValueNode);
									}pEnumValueNode.Release();
								}

							}
							pRuleNode.Release();
						}
					}
					pTextNode = pDoc->createTextNode(L"\n    "); 
					pOccurrence->appendChild(pTextNode);
					pTextNode.Release();
					pOccurrence.Release();
				}
			}



			pTextNode = pDoc->createTextNode(L"\n   "); 
			pOccurrencesNode->appendChild(pTextNode);
			pTextNode.Release();

			pOccurrencesNode.Release();
			//Exclusions
			MSXML2::IXMLDOMElementPtr pExclusionsNode;
			pExclusionsNode=pDoc->createElement("Exclusions");
			if(pExclusionsNode!=NULL)
			{
				pTextNode = pDoc->createTextNode(L"\n   "); 
				pSoilNameNode->appendChild(pTextNode);
				pTextNode.Release();
				pSoilNameNode->appendChild(pExclusionsNode);

				MSXML2::IXMLDOMElementPtr pExclusion;
				for(int n7=0;n7<pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusionNum();n7++)
				{
					pExclusion=pDoc->createElement("Exclusion");
					if(pExclusion!=NULL)
					{
						pTextNode = pDoc->createTextNode(L"\n    "); 
						pExclusionsNode->appendChild(pTextNode);
						pTextNode.Release();

						attrib=pDoc->createAttribute("Name");
						if(attrib!=NULL)
						{
							CString excluname=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getExclusionName();
							_variant_t excluname_var((LPCSTR)excluname);
							attrib->value=excluname_var;
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDoc->createAttribute("x");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getCentralX();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDoc->createAttribute("y");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getCentralY();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						
						attrib=pDoc->createAttribute("SearchDist");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getSearchDist();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDoc->createAttribute("UseDistanceDecay");
						if(attrib!=NULL)
						{
							if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getUseDistSim() == true)
								attrib->value = "true";
							else 
								attrib->value = "false";
						
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDoc->createAttribute("DistanceDecay");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getDistDecayFactor();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						pExclusionsNode->appendChild(pExclusion);

						MSXML2::IXMLDOMElementPtr pRuleNode;
						for(int n8=0;n8<pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRuleNum();n8++)
						{
							pRuleNode=pDoc->createElement("Rule");
							if(pRuleNode!=NULL)
							{
								pTextNode = pDoc->createTextNode(L"\n     "); 
								pExclusion->appendChild(pTextNode);
								pTextNode.Release();

								attrib=pDoc->createAttribute("Name");
								if(attrib!=NULL)
								{
									CString rulename=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleName();
									_variant_t rulename_var((LPCSTR)rulename);
									attrib->value=rulename_var;
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDoc->createAttribute("UseOrNot");
								if(attrib!=NULL)
								{
									if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n7)->getRule(n8)->getIsUsed()==true)
										attrib->value="true";
									else 
										attrib->value="false";

									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDoc->createAttribute("Type");
								if(attrib!=NULL)
								{
									int a=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType();
									switch(a)
									{
									case 0:
										attrib->value="Range";
										break;
									case 1:
										attrib->value="Curve";
										break;
									case 2:
										attrib->value="Word";
										break;
									case 3:
										attrib->value="Point";									
										break;
									case 4:
										attrib->value="Enumerated";										
										break;

									}
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDoc->createAttribute("DataLayer");
								if(attrib!=NULL)
								{
									CString datalayername=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getLayerName();
									_variant_t datalayername_var((LPCSTR)datalayername);
									attrib->value=datalayername_var;
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();
								pExclusion->appendChild(pRuleNode);


								//Range
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 0)
								{
									CRangeRule *m_pRange = new CRangeRule;
									m_pRange=static_cast<CRangeRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));

									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDoc->createElement("CurveType");
									if(pCurveTypeNode!=NULL)
									{
										int curvT=m_pRange->getCurveType();
										switch(curvT)
										{
										case 0:
											pCurveTypeNode->text="Bell-shape";
											break;
										case 1:
											pCurveTypeNode->text="S-shape";
											break;
										case 2:
											pCurveTypeNode->text="Z-shape";
											break;

										}
										pRuleNode->appendChild(pCurveTypeNode);
									}
									pCurveTypeNode.Release();

									MSXML2::IXMLDOMElementPtr pKeyValueNode;
									pKeyValueNode=pDoc->createElement("KeyValue");
									if(pKeyValueNode!=NULL)
									{
										double KeyValue[4];
										KeyValue[0]=m_pRange->getLowCross();
										KeyValue[1]=m_pRange->getLowUnity();
										KeyValue[2]=m_pRange->getHighUnity();
										KeyValue[3]=m_pRange->getHighCross();
										//char *s=new char[];
										CString str;
										int i2=0;
										for(int j2=0;j2<4;j2++)
										{
											CString tempStr;
											tempStr.Format("%f",KeyValue[j2]);
											if(j2!=3)
												str = str + tempStr+",";
											else 
												str = str + tempStr;

										}
										pKeyValueNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pKeyValueNode);
										//delete []s;
									}
									pKeyValueNode.Release();
								}

								//Curve
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 1)
								{
									CFreehandRule *m_pCurve = new CFreehandRule;
									m_pCurve= static_cast<CFreehandRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));

									MSXML2::IXMLDOMElementPtr pNodeNum,pCoordinatesNode;
									pNodeNum=pDoc->createElement("NodeNum");
									CString str;
									str.Format("%d",m_pCurve->getKnotNum());								
									pNodeNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pNodeNum);
									pNodeNum.Release();

									pCoordinatesNode=pDoc->createElement("Coordinates");
									if(pCoordinatesNode!=NULL)
									{
										int i1=0;
										CString str;
										for(int j=0;j<m_pCurve->getKnotNum();j++)
										{
											double x, y;
											CString tempString;
											m_pCurve->getKnot(j,&x,&y);
											tempString.Format("%f %f",x,y);
											if(j!=m_pCurve->getKnotNum()-1)
												str = str + tempString + ",";
											else 
												str = str + tempString; 
											//i1+=sprintf(s+i1,"%f,%f ",&x,&y);
										}
										pCoordinatesNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pCoordinatesNode);
									}
									pCoordinatesNode.Release();
								}


								//Word
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 2)
								{
									CWordRule *m_pWord=new CWordRule;
									m_pWord=static_cast<CWordRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));
									MSXML2::IXMLDOMElementPtr pPraseNode;
									pPraseNode=pDoc->createElement("Prase");
									if(pPraseNode!=NULL)
									{		
										CString phrase=m_pWord->getPhrase();
										_bstr_t phrase_bstr=phrase.AllocSysString();
										pPraseNode->text=phrase_bstr;
										pRuleNode->appendChild(pPraseNode);
									}
									pPraseNode.Release();


									MSXML2::IXMLDOMElementPtr pEnvAttriNode;
									pEnvAttriNode=pDoc->createElement("EnvAttri");
									if(pEnvAttriNode!=NULL)
									{
										CString EnvAttri=m_pWord->getEnvAttri();
										_bstr_t EnvAttri_bstr=EnvAttri.AllocSysString();
										pEnvAttriNode->text=EnvAttri_bstr;
										pRuleNode->appendChild(pEnvAttriNode);
									}
									pEnvAttriNode.Release();

									MSXML2::IXMLDOMElementPtr pWordLibNode;
									pWordLibNode=pDoc->createElement("WordLib");
									if(pWordLibNode!=NULL)
									{
										CString libfile=extractFileName(m_pWord->getLibFileName());
										_bstr_t libfile_bstr=libfile.AllocSysString();
										pWordLibNode->text=libfile_bstr;
										pRuleNode->appendChild(pWordLibNode);
									}
									pWordLibNode.Release();
								}



								//Point
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 3)
								{
									CPointRule *m_pPoint=new CPointRule;
									m_pPoint=static_cast<CPointRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));
									MSXML2::IXMLDOMElementPtr pXYNode;
									pXYNode=pDoc->createElement("XY");
									if(pXYNode!=NULL)
									{
										CString str;
										str.Format("%f %f",m_pPoint->getCentralX(),m_pPoint->getCentralY());
										pXYNode->text= (_bstr_t)str;
										pRuleNode->appendChild(pXYNode);
									}
									pXYNode.Release();

									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDoc->createElement("CurveType");
									if(pCurveTypeNode!=NULL)
									{
										int curvT=m_pPoint->getCurveType();
										switch(curvT)
										{
										case 0:
											pCurveTypeNode->text="Bell-shape";
											break;
										case 1:
											pCurveTypeNode->text="S-shape";
											break;
										case 2:
											pCurveTypeNode->text="Z-shape";
											break;
										}
										pRuleNode->appendChild(pCurveTypeNode);
									}
									pCurveTypeNode.Release();


									MSXML2::IXMLDOMElementPtr pLWidthNode;
									pLWidthNode=pDoc->createElement("LWidth");
									//char *Width = new char[];
									if(pLWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getLWidth());
										pLWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pLWidthNode);
									}
									pLWidthNode.Release();

									MSXML2::IXMLDOMElementPtr pRWidthNode;
									pRWidthNode=pDoc->createElement("RWidth");
									//char *Width = new char[];
									if(pRWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getRWidth());
										pRWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pRWidthNode);
									}
									pRWidthNode.Release();
									//delete Width;
								}

								//Enumerated
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 4)
								{
									CEnumRule *m_pEnum=new CEnumRule;
									m_pEnum=static_cast<CEnumRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));
									MSXML2::IXMLDOMElementPtr pItemNum,pEnumValueNode;

									pItemNum=pDoc->createElement("ItemNum");
									CString str;
									str.Format("%d",m_pEnum->getItemNum());								
									pItemNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pItemNum);
									pItemNum.Release();

									pEnumValueNode=pDoc->createElement("EnumValue");
									if(pEnumValueNode!=NULL)
									{
							
										int i3=0;
										CString str = "";
										for(int j3=0;j3<m_pEnum->getItemNum();j3++)
										{
											CString TempStr;
											TempStr.Format("%d",m_pEnum->getItem(j3));
											if(j3!=m_pEnum->getItemNum()-1)
											{
												str+=TempStr;
												str+=",";
											}
											else
												str+=TempStr;										

										}
										pEnumValueNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pEnumValueNode);
									}
									pEnumValueNode.Release();
								}
							}
							pRuleNode.Release();
						}
					}

					pTextNode = pDoc->createTextNode(L"\n    "); 
					pExclusion->appendChild(pTextNode);
					pTextNode.Release();
					pExclusion.Release();
				}
			}
			pTextNode = pDoc->createTextNode(L"\n   "); 
			pExclusionsNode->appendChild(pTextNode);
			pTextNode.Release();
			pExclusionsNode.Release();
		}

		pTextNode = pDoc->createTextNode(L"\n  "); 
		pSoilNameNode->appendChild(pTextNode);
		pTextNode.Release();

		pSoilNameNode.Release();
	}

	pTextNode = pDoc->createTextNode(L"\n "); 
	pKnowledgebaseNode->appendChild(pTextNode);
	pTextNode.Release();

	pKnowledgebaseNode.Release();



	//************** Create Inference¡¡************************
	MSXML2::IXMLDOMElementPtr pInferenceNode;
	pInferenceNode=pDoc->createElement("Inference");
	if(pInferenceNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n "); 
		pRootElem->appendChild(pTextNode);
		pTextNode.Release();

		pRootElem->appendChild(pInferenceNode);
	}
	MSXML2::IXMLDOMElementPtr pUseMaskNode,pInferMaskNode,pMaskValueNode,pInferDirNode,pInferCreateCheckNode;
	pUseMaskNode=pDoc->createElement("UseMask");
	if(pUseMaskNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		if(pProject->m_pInference->getUseMask()==true)
			pUseMaskNode->text="true";
		else
			pUseMaskNode->text="false";
		pInferenceNode->appendChild(pUseMaskNode);
	}
	pInferMaskNode=pDoc->createElement("Mask");
	if(pInferMaskNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();

		pInferMaskNode->text=(_bstr_t)(pProject->m_pInference->getMaskFile());
		pInferenceNode->appendChild(pInferMaskNode);
	}
	pMaskValueNode=pDoc->createElement("MaskValue");
	if(pMaskValueNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		CString str;
		str.Format("%f",pProject->m_pInference->getMaskValue());
		pMaskValueNode->text=(_bstr_t)str;
		pInferenceNode->appendChild(pMaskValueNode);
	}
	pInferDirNode=pDoc->createElement("Directory");
	if(pInferDirNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		CString resultdir=pProject->m_pInference->getResultDir();
		_bstr_t resultdir_bstr=resultdir.AllocSysString();
		pInferDirNode->text=resultdir_bstr;
		pInferenceNode->appendChild(pInferDirNode);
	}

	pInferCreateCheckNode=pDoc->createElement("CreateCheckFile");
	if(pInferCreateCheckNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		if(pProject->m_pInference->getCreateCheckFile())
			pInferCreateCheckNode->text="true";
		else
			pInferCreateCheckNode->text="false";
		pInferenceNode->appendChild(pInferCreateCheckNode);
	}

	pTextNode = pDoc->createTextNode(L"\n  "); 
	pInferenceNode->appendChild(pTextNode);
	pTextNode.Release();

	pUseMaskNode.Release();
	pInferMaskNode.Release();
	pMaskValueNode.Release();
	pInferDirNode.Release();
	pInferCreateCheckNode.Release();
	pInferenceNode.Release();


	//*************** Save the xml document **********************
	pTextNode = pDoc->createTextNode(L"\n "); 
	pRootElem->appendChild(pTextNode);
	pTextNode.Release();


	pRootElem.Release();
	_variant_t varxml(strFileName);
	hr=pDoc->save(varxml);
	if(FAILED(hr))
	{
		AfxMessageBox("Error in converting project!");
		return;
	}
	pDoc.Release();
	CoUninitialize();
}


bool ImportOldPrj::ConvertToNewProject(char *workDir, char *thePntFile, char *theLabFile, char *theCellnameFile,CString NewProjectPath,CString NewProjectName)
{	

	CString DataPath;
	CString ProjectFilePath;

	DataPath.Format("%s", NewProjectPath);
	ProjectFilePath.Format("%s", NewProjectPath);

	DataPath = NewProjectPath + "\\GISData";
	ProjectFilePath = NewProjectPath + "\\" + NewProjectName + ".sip";


	CreateDirectory(DataPath,NULL);		
	theSetting = new OldSetting(workDir, thePntFile, theLabFile, theCellnameFile);  
	theEnvData = new OldEnvData(theSetting);
	theCaseBase = new OldCaseBase(theSetting,theEnvData); //theEnvData here is for the caselist to access layerlist to get the default width


	int nrow = theSetting->nrow;
	int i=0;	
	
	theEnvData->resetBeginning(); 

	//Fill the case attributes.	
	theEnvData->refreshEnvData(DataPath.GetBuffer());
	DataPath.ReleaseBuffer();
	
	theCaseBase->fillCaseAttr(theEnvData);

	CProject *pProject;
	pProject = new CProject();
	if(pProject == NULL)
	{
		AfxMessageBox("Error in converting project.");
		return false;
	}   

	if(pProject->m_pEnvDatabase == NULL)
		pProject->m_pEnvDatabase = new CEnvDatabase();
    
	int count;
	if(theEnvData ==NULL){	
		count = 0;		
	}
	else{
      count= theEnvData->numOfLayers;
	}
	
	OldLayerListNode* templayer =theEnvData->theLayerList->firstLayer;
	
    for(int i=0; i<count; i++)
	{
		char layername[FN_LEN];
		strcpy(layername,templayer->layerName);
		int length = strlen(layername);
		layername[length-1] = '\0';

		char filename[FN_LEN];
		strcpy(filename,templayer->layerFileName);	
		strcpy(filename,extract_filename(filename));

		CString layerName;
		CString fileName;
			
		layerName.Format("%s", layername);
		layerName.Trim();
		fileName.Format(filename);
		fileName.Trim();

		CString strNewFilePath= DataPath + "\\"+ fileName;

		CEnvLayer *pLayer = new CEnvLayer(i,layerName, strNewFilePath);
		if(pLayer->getLayerID() != -1)
		{ 
			pProject->m_pEnvDatabase->AddLayer(pLayer);
		}
		else 
		{
			delete pLayer;
			pLayer = NULL;
			CString msg;
			msg.Format("Source file of layer %s can not be found, this layer will not be added",layerName);
			AfxMessageBox(msg);
			count--;
		}


		templayer = templayer->nextLayer;
	}


		

	if(pProject->m_pKnowledgeBase == NULL)
    {
		pProject->m_pKnowledgeBase = new CKnowledgeBase();
	}

	int SoilNum=theCaseBase->numOfSoilTypes;
	OldCaseList *tempCaseList=theCaseBase->theFirstCaseList;
	for(int i=0;i<SoilNum;i++)
	{
		CSoilType *pSoilType;
		pSoilType = new CSoilType();
		if(pSoilType == NULL)
		{
			AfxMessageBox("Error in converting project.");
			return false;
		}
		CString str;
		str.Format("%s", tempCaseList->soilType);
		pSoilType->setSoilTypeName(str);


		OldCaseNode *tempCaseNode=tempCaseList->firstCase;
        int k=1;
		
		while(tempCaseNode!= NULL)
		{
			CInstance *pInstance = new CInstance();
			if(pInstance == NULL)
			{
				AfxMessageBox("Error in converting project.");
				return false;
			}
			
			CString str;
			str.Format("%s%d", "Instance", k);
			pInstance->setInstanceName(str);
	      		
			k++;	     
		
		    OldCaseAttr *tempCaseAttr=tempCaseNode->firstAttr;
		    WidthList *tempWidthList=tempCaseList->widthList;
		    WidthRecord *tempWidthRecord=tempWidthList->firstWidthRecord;
			

           //int curID =1;
		   while(tempWidthRecord!=NULL&&tempCaseAttr!=NULL)
		   {
			
			  bool useOrNot = tempWidthRecord->useOrNot;
			  float width=tempWidthRecord->width;
			  int functionType=tempWidthRecord->functionType;
			  char environVarialbe[STRLEN];
			  strcpy(environVarialbe,tempWidthRecord->environVariable);

			  int length = strlen(environVarialbe);
		      environVarialbe[length-1] = '\0';
			  CString strEnvName;
			  strEnvName.Format("%s", environVarialbe);
	

			  float attri=tempCaseAttr->attrValue;
			  if(width!=-99)
			  {
				  CRangeRule *pRule;
				  pRule = new CRangeRule();

				  if(pRule == NULL)
				  {
					  AfxMessageBox("Error in converting project.");
					  return false;
				  }
				  pRule->setLayerName(strEnvName);

				  double tag=999999;			     
				  if(functionType==1)
				  {
					  pRule->setCurveType(0);
					  pRule->setIsUsed(useOrNot);
					  pRule->setRuleName(strEnvName);
					  pRule->setLowUnity(attri);
					  pRule->setHighUnity(attri);
					  pRule->setLowCross(attri-width);
					  pRule->setHighCross(attri+width);


				  }
				  else if(functionType==2)
				  {			
					  pRule->setCurveType(0);
					  pRule->setIsUsed(useOrNot);
					  pRule->setRuleName(strEnvName);
					  pRule->setLowUnity(attri);
					  pRule->setLowCross(attri-width);


				  }
				  else 
				  {				
					  pRule->setCurveType(0);
					  pRule->setIsUsed(useOrNot);
					  pRule->setRuleName(strEnvName);
					  pRule->setHighUnity(attri);
					  pRule->setHighCross(attri+width);

				  }	
				  pInstance->AddRule(pRule);

			  }
			  else 
			  {	
				  CEnumRule *pRule;
				  pRule = new CEnumRule();
				  if(pRule == NULL)
				  {
					  AfxMessageBox("Error in converting project.");
					  return false;
				  }

				  pRule->setLayerName(strEnvName);

				  pRule->setIsUsed(useOrNot);
				  pRule->setRuleName(strEnvName);

				  pRule->AddItem((int)attri);
				 // pInstance->AddRule((CRule *)pRule);
				  pInstance->AddRule(pRule);
			  }

			
			  tempWidthRecord=tempWidthRecord->nextRecord;
			  tempCaseAttr=tempCaseAttr->nextCaseAttr;
			  //curID++;			
		   }

		   pSoilType->AddInstance(pInstance);
		   tempCaseNode=tempCaseNode->nextCase;
		}			

		tempCaseList=tempCaseList->nextList;
		pProject->m_pKnowledgeBase->AddSoilType(pSoilType);

	}



	if(pProject->m_pInference == NULL)
    {
		pProject->m_pInference = new CInference();
	}
	
	WriteProject(ProjectFilePath, pProject);
	if(pProject!=NULL)
	{
		delete pProject;
		pProject = NULL;
	}

	return true;


}




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
#include  <fstream> 
#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include <math.h>
#include <string.h>
#include "dbfrulebase.h"
#include "Project.h"

#define SLASH "\\"



// This class specifies (contains) the information about the
// spatial extent of the GIS database.
//--------------------------------------------------------------------------------------------------- 
class OldSetting
{
public:
	int ncol, nrow;
	double west;
	double east;
	double south;
	double north;
	float pixelsize; 
	char gridUnits[STRLEN];
	char dataUnits[STRLEN];
	int nodata;
	char workDir[FN_LEN];
	char thePntFile[FN_LEN];
	char theLabFile[FN_LEN];
	char theCellnameFile[FN_LEN];
	char theMaskFile[FN_LEN];

	OldSetting(char aWorkDir[], char aPntFile[], char aLabFile[], char aCellnameFile[]);
	void getSetting(char aWorkDir[], char aPntFile[], char aLabFile[], char aCellnameFile[]);
};
//---------------------------------------------------------------------------------------------------


class OldLayerListNode
{
public:
	char layerName[STRLEN];
	char layerFileName[FN_LEN];
	int type;	 //to handle the new cell_names fields (08/02) default is 4(ration)
	float width; //to handle the new cell_names fields (08/02) default is -99(using stdev)
	int nrow;
	int ncol;
	float* layerMatrix;
	float stdDev;
	int beginningOfData; 
	int curLoc; //The current reading location in the file
	OldLayerListNode* nextLayer;
	//LayerListNode(int theNrow, int theNcol);
	OldLayerListNode(char lName[], char fName[], OldSetting* theSetting, char typeS[], char widthS[]);
	//LayerListNode(Setting* theSetting); //only used for setting up mask
	//LayerListNode(const LayerListNode& aLayerListNode);
	~OldLayerListNode();
	void read3DFile(char *CopyDataPath);
};

//---------------------------------------------------------------------------------------------------
class OldLayerList
{
public:
	int numOfLayers;
	OldLayerListNode* firstLayer;
	OldLayerList();
	OldLayerList(OldLayerListNode* aNode);
	//OldLayerList(const OldLayerList& aLayerList);
	~OldLayerList();
	void addNode(OldLayerListNode* aNewNode);
};

// The class of environment data (GIS database in the form of raster data layers)
// The database consists of two components (objects): the spatial extent and the specific
// raster layers.
//---------------------------------------------------------------------------------------------------
class OldEnvData
{  
public:
	int numOfLayers;
	OldSetting* theSetting;
	OldLayerList* theLayerList; //a list of data layers
	//OldLayerListNode* theMask; //the mask

	OldEnvData(OldSetting* aSetting);
	//EnvData(const EnvData& anEnvData);
	~OldEnvData();
	void readEnvData(OldSetting* aSetting); // a function to read environment data.
		                // precond: data files and arrays exist
			            // postcond: arrays dem, gradient, aspect
				        //           profile, planform and wetness
					    //           are fulfilled.
	void refreshEnvData(char *CopyDataPath);
	void resetBeginning();
};
//---------------------------------------------------------------------------------------------------

class WidthRecord
{///////////////////////////////////////////////////////////////////////////
public:
	char environVariable[STRLEN];  //or should not harden?
	//int useOrNot; //This is no longer used. However, if the user specifies an environment
					// variable not to be used in the width file, this environment variable
					// will not be included in the width list when the width list being constructed
	float width; //if -99, use default
	int functionType; //if -99, use default
	//int dataType; //be here or using layerlist one?
	bool useOrNot;
	WidthRecord* nextRecord;
	WidthRecord(char theEnviron[], float theWidth, int theFunctionType,bool theUseOrNot);
	~WidthRecord();
};
//---------------------------------------------------------------------------------------------------

class WidthList
{
public:	
	WidthRecord* firstWidthRecord;
	int numWidthRecords;
	WidthList();
	~WidthList();
	void addWidthRecord(WidthRecord* aRecord);
};


//---------------------------------------------------------------------------------------------------
class OldCaseAttr
{
public:
	float attrValue;
	OldCaseAttr* nextCaseAttr;
	~OldCaseAttr();
};
//---------------------------------------------------------------------------------------------------
class OldCaseNode  //an instance
{
public:
	double x;
	double y;
	OldCaseAttr* firstAttr; 
	OldCaseNode* nextCase;
	OldCaseNode();
	OldCaseNode(double a, double b);
	~OldCaseNode();
};
//---------------------------------------------------------------------------------------------------
class OldCaseList  //a soil type
{
public:
	char soilType[STRLEN];
	int soilID;
	WidthList* widthList;    //to handle the new width file under relation directory (09/26)
	int  length;
	int  hasEffectiveCase;
	OldCaseNode* firstCase;
	OldCaseList* nextList;
	OldCaseList();
	OldCaseList(char theType[], int theID, WidthList* theWidths);
//	OldCaseList(char theType[], int theID, WidthList* theWidths, CaseNode aCase);
	OldCaseList(const OldCaseList& aCaseList);
	~OldCaseList();
};
//---------------------------------------------------------------------------------------------------
class OldCaseBase //a knowledgebase
{
public:
	OldCaseList* theFirstCaseList;
	int numOfSoilTypes;
	int flagForTheBigLoop;
	OldCaseBase();
	OldCaseBase(OldSetting* theSetting, OldEnvData* theEnvData);
	//OldCaseBase(const OldCaseBase& aCaseBase);
	~OldCaseBase();
	void buildCaseBase(OldSetting* theSetting, OldEnvData* theEnvData);
	//void fillCaseAttr(OldEnvData* theEnvData, int theLine);

	void fillCaseAttr(OldEnvData* theEnvData);
	//void createLookupTable(Setting* theSetting, OldEnvData* theEnvData);
};
//---------------------------------------------------------------------------------------------------

class IdSoilIndexNode
{
public:
	int theID;
	char theSoilType[STRLEN];
	IdSoilIndexNode* nextIndexNode;
	IdSoilIndexNode();
	IdSoilIndexNode(int id, char soilType[]);
	~IdSoilIndexNode();
};
//---------------------------------------------------------------------------------------------------
class IdSoilIndexList
{
public:
	IdSoilIndexNode* theFirstNode;
	IdSoilIndexList();
	IdSoilIndexList(const IdSoilIndexList& anIdSoilIndexList);
	~IdSoilIndexList();
};


//---------------------------------------------------------------------------------------------------		
class ImportOldPrj
{
public:
	ImportOldPrj();
	~ ImportOldPrj();
	bool ImportOldPrj::ConvertToNewProject(char *workDir, char *thePntFile, char *theLabFile, char *theCellnameFile,CString NewProjectPath,CString NewProjectName);
  //  void ImportOldPrj::Convert2PointcaseProject(char *workDir, char *thePntFile, char *theLabFile, char *theCellnameFile,char *NewProjectPath,char *NewProjectName);

	void WriteProject(CString strFileName, CProject *pProject);
	
private:
	OldSetting* theSetting;
	OldCaseBase* theCaseBase;	
	OldEnvData* theEnvData;
};
//---------------------------------------------------------------------------------------------------


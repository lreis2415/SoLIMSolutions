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
/* knowledge.h
 * prepared by Xun Shi
 * last modified: Sep. 16, 2005
 *
 * Dec 15, 2002: Add integratingAlgo to case, 
 *					move wgtDecayFactor from KnowledgeList to Spatial, 
 *					remove wgtDecayFactor from RasterCase
 *					Chage file name from cbr to knowledge
 *					Move the files to this Library
 *					Make PointCase and rasterCase to be children of Attr (actually attribute rule).
 *Sep 16, 2005: Add AHP_ScaleList
 */

#ifndef KNOWLEDGE_H
#define KNOWLEDGE_H

#include "utility.h"
#include "EnvData.h"
#include "ThreeDR.h"
#include "patch.h"

#define DEFAULTATTRVALUE 0
#define DEFAULTWEIGHT 0
#define DEFAULTK 0.5
#define DEFAULTR 2
//#define STDFACTOR 1.274
#define WIDTHFACTOR 1.33

#define DEFAULTSEARCHDIST 1500
#define DEFAULTVTOHFACTOR 1
#define DEFAULTDISTDECAYFACTOR 2
#define DEFAULTWDTDECAYFACTOR 2

class AHP_ScaleList{ //a single scale list for constructing list of lists for scale matrix
public:
	CString name;
	float weight; 
	List<FCNode>* factorList;
	AHP_ScaleList* before;
	AHP_ScaleList* next;
	AHP_ScaleList();
	AHP_ScaleList(CString aName, EnvDatabase* anEnvDatabase);
	AHP_ScaleList(const AHP_ScaleList& anAHP_ScaleList);
	~AHP_ScaleList();
};


// an attr is an instance
class Attr{
public:
	int id;
	CString name;
	CString algo;
	float memberValue;
	bool positive;
	bool effective;
	bool useMask;
	CString maskFileName;
	CThreeDR* mask;
	bool inverseMaskValue;
	bool needSave;
	Attr* next;

	//attribute values and inference settings
	List<FeatureNode>* theFeatureList; //defined in utility.h

	//tools
	int addADefaultFeature(EnvLayer* anEnvLayer, int rowNo, int colNo);
	int setFeatureDefaultsXY(EnvDatabase* theEnvDatabase, double x, double y);//0 - got values, became effective case
											  //1 - theEnvDatabase is empty
											  //2 - case is out of the spatial range
	int setFeatureDefaultsRW(EnvDatabase* theEnvDatabase, int row, int col);//0 - got values, became effective case
											  //1 - theEnvDatabase is empty
											  //2 - case is out of the spatial range
	void setAFeature(FeatureNode* aFeatureNode, int updateValue); //set values for a feature
	void setFeatureConfig(List<FeatureNode>* aFeatureList, int updateValue); //set values for the whole feature list
															//updateValue: 0: keep original central values
															//		   1: use new central values
	//fill the mean, min, max, and std for all the features in the KB from the current env database
	void fillFeatureStats(EnvDatabase* theEnvDatabase);
	//These two functions read local values for v1 and v2 (v1=v2=the local value)
	//They do not change other parts of the featurelist.
	//readALocalValue is a utility of these two functions.
		//0 - got values, became effective case
		//1 - theEnvDatabase is empty
		//2 - case is out of the spatial range
	int readLocalValuesXY(EnvDatabase* theEnvDatabase, double x, double y); 
	int readLocalValuesRW(EnvDatabase* theEnvDatabase, int row, int col);	
	void readALocalValue(EnvLayer* anEnvLayer, int rowNo, int colNo);
	void setSaved();

	Attr();
	Attr(List<FeatureNode>* aFeatureList);
	Attr(const Attr& aAttr);
	~Attr();
};


//a rule list is related to a soil type   (by DuFei)
class RuleList{
public:
	int id;
	int uid; //for assigning unique id to rules
	int typeID; //id of soil type
	CString name;
	int  numRules;
	bool useMask;
	CString maskFileName;
	CThreeDR* mask;
	bool inverseMaskValue;
	bool needSave;

	Attr* first;
	RuleList* next;

	void setFeatureDefaults(EnvDatabase* theEnvDatabase);
	void setAFeature(FeatureNode* aFeatureNode, int updateValue);
	void setFeatureConfig(List<FeatureNode>* aFeatureList, int updateValue);
												//updateValue: 0: keep original central values
												//		   1: use new central values
	void fillFeatureStats(EnvDatabase* theEnvDatabase);
	void addRule(Attr* anAttr);
	void removeRule(Attr* anAttr);

	void setSaved();	
	void checkSave();

	RuleList();
	RuleList(const RuleList& aRuleList);
	~RuleList();
};

class RuleBase{
public:
	int uid; //for assigning unique id to lists
	CString name;
	CString fileName;
	int  numRuleLists;
	bool needSave;	
	RuleList* first;

	void checkSave();
	void setSaved();

	void setFeatureDefaults(EnvDatabase* theEnvDatabase);
	void addList(RuleList* aRuleList);
	void removeList(RuleList* aRuleList);
	// Checks if the features in KB matches feature in the env database
	int checkFeatureMatch(EnvDatabase* theEnvDatabase);
	void updateFeatures(EnvDatabase* theEnvDatabase);
	//fill the mean, min, max, and std for all the features in the KB from the current env database
	void fillFeatureStats(EnvDatabase* theEnvDatabase);

	RuleBase();
	RuleBase(const RuleBase& anRuleBase);
	~RuleBase();
};

class Spatial{
public:
	//spatial inference setting
	int id;
	double x;
	double y;
	float searchDist;
	bool useDistSim;
	float distDecayFactor;
	float VtoHConvertingFactor;
	int integratingAlgo;	//integrating algo when this case encounters other cases
							//0: supplement
							//1: dominant
							//2: inverse-distance
							//3: similarity-weight
							//4: average
							//5: maximum
	float wgtDecayFactor; //decay factor for weighted combination of mutiple cases
	bool needSave;

	//tools
	void setSpatialSetting(double anX, double aY, 
								float aSearchDist, bool aUseDistSim, float aDistDecayFactor, 
								float aVtoHConvertingFactor, 
								int anIntegratingAlgo, float aWgtDecayFactor);
	void setSpatialSetting(Spatial* aSpatial, int updateValue);

	void setSaved();

	Spatial();
	Spatial(const Spatial& aSaptialKnowledge);
};

class RasterCases{
public:
	CString name;
	CString fileName; //the name of the KB
	CString rasterFileName; //the name of the 3dr file
	CThreeDR* theRaster;
	Attr* theAttr; //need the config
	Spatial* theSpatial;
	bool needSave;

	int resampleRatio;
	int RasterCases::checkFeatureMatch(EnvDatabase* theEnvDatabase);
		// Checks if the features in KB matches feature in the env database
		// Return values:
		// 0: perfect match
		// -1: empty KB
		// -2: empty envDB
		// -3: new envDB
		// -4: feature in KB is not found in envDB
		// -5: feature in envDB is not found in KB
	void updateFeatures(EnvDatabase* theEnvDatabase);
	//fill the mean, min, max, and std for all the features in the KB from the current env database
	void fillFeatureStats(EnvDatabase* theEnvDatabase);

	void checkSave();
	void setSaved();

	RasterCases();
	RasterCases(CString aFileName);
	RasterCases(const RasterCases& aRasterCases);
	~RasterCases();
};

class PointCase{
public:
	Attr* theAttr; 
	CString theCentralValueMethod; //place holder, not used by point case
	float modeTolerance; //place holder, not used by point case
	Spatial* theSpatial;
	PointCase* next;
	bool needSave;

	int setFeatureDefaults(EnvDatabase* theEnvDatabase);

	PointCase();
	PointCase(int anID, CString aname, double x, double y);
	PointCase(const PointCase& aPointCase);
	~PointCase();

	void setSaved();
	void checkSave();
};

class LineCase{
public:
	List<IDDNode>* theLine;
	Attr* theAttr; 
	bool needSave;
	CString theCentralValueMethod; //the method for setting the central value
	float modeTolerance; //tolerance in determining mode value
	List<IDDDDNode>* caseStats; //list of stats of the "case pixels" for all the env layers.  
										//d1: mean; d2: median; d3: mode; d4: std dev.
	List<IntNode>* casePixels; //list of positions of pixels on case
	Spatial* theSpatial;
	LineCase* next;

	int setFeatureDefaults(EnvDatabase* theEnvDatabase);
	int setCasePixels(EnvDatabase* theEnvDatabase); //fills the casePixels and the caseStats.
	void setCentralValue(); //sets the central value according to the specified method


	void setSaved();
	void checkSave();

	LineCase();
	LineCase(int anID, CString aname, List<IDDNode>* aLine);
	LineCase(const LineCase& aLineCase);
	~LineCase();
};

class PolyCase{
public:
	xPolygon* thePoly;
	Attr* theAttr; 
	bool needSave;
	CString theCentralValueMethod; //the method for setting the central value
	float modeTolerance; //tolerance in determining mode value
	List<IDDDDNode>* caseStats; //list of stats of the "case pixels" for all the env layers.  
										//d1: mean; d2: median; d3: mode; d4: std dev.
	List<IntNode>* casePixels; //list of positions of pixels on case
	Spatial* theSpatial;
	PolyCase* next;

	int setFeatureDefaults(EnvDatabase* theEnvDatabase);
	int setCasePixels(EnvDatabase* theEnvDatabase); //fills the casePixels and the caseStats.
	void setCentralValue(); //sets the central value according to the specified method

    void setSaved();
	void checkSave();

	PolyCase();
	PolyCase(int anID, CString aname, xPolygon* thePoly);
	PolyCase(const PolyCase& aPolyCase);
	~PolyCase();
};

/*******************************************
 CaseList Class (case group for a soil type)
 *******************************************/
template<class T>
class CaseList{
public:
	int id;
	int uid;  //for assigning unique id to cases
	int typeID; //id of soil type
	CString name;
	int  numCases;
	bool useMask;
	CString maskFileName;
	CThreeDR* mask;
	bool inverseMaskValue;
	bool needSave;
	T* first;
	CaseList* next;

	
	void setSaved()
	{
		T* tempCase = first;
		while(tempCase!=NULL)
		{
			tempCase->setSaved();
			tempCase=tempCase->next;
		}
		needSave = false;
	}


	CaseList(){
		id = -1;
		uid = 1;
		typeID = -1;
		name = "";
		numCases = 0;
		useMask = false;
		maskFileName = "";
		mask = NULL;
		inverseMaskValue = false;
		needSave = true;
		first = NULL;
		next = NULL;
		needSave = true;
	};
	CaseList(int anID, int aTypeID, CString aname){
		id = anID;
		uid = 1;
		typeID = aTypeID;
		name = aname;
		numCases = 0;
		useMask = false;
		maskFileName = "";
		mask = NULL;
		inverseMaskValue = false;
		needSave = true;
		first = NULL;
		next = NULL;
		needSave = true;
	};
	CaseList(const CaseList& aCaseList){
		id = aCaseList.id;
		uid = aCaseList.uid;
		typeID = aCaseList.typeID;
		name = aCaseList.name;
		numCases = aCaseList.numCases;
		useMask = aCaseList.useMask;
		maskFileName = aCaseList.maskFileName;
		if(aCaseList.mask!=NULL)
			mask = new CThreeDR(*(aCaseList.mask));
		else
			mask = NULL;
		inverseMaskValue = aCaseList.inverseMaskValue;
		needSave = true;
		next = aCaseList.next;

		T* tempCase = aCaseList.first;
		while(tempCase!=NULL){
			addCase(new T(*tempCase));
			tempCase = tempCase->next;
		}
		needSave = true;
	};
	~CaseList(){
		id = -1;
		uid = 1;
		typeID = -1;
		name = "";
		numCases = 0;
		useMask = false;
		maskFileName = "";
		if(mask!=NULL){
			delete mask;
			mask = NULL;
		}
		inverseMaskValue = false;
		T* tempCase = first;
		while(first!=NULL){
			first = tempCase->next;
			delete tempCase;
			tempCase = first;
		}
		first = NULL;
		next = NULL;
	};
	int setFeatureDefaults(EnvDatabase* theEnvDatabase){
	//call setFeatureDefaults function at the case level
		int n = 0; //number of cases that are out of range
	    T* tempCase = first;
		while(tempCase!=NULL){ //scan through the cases in a group
			T* nextCase = tempCase->next;
			if(tempCase->setFeatureDefaults(theEnvDatabase)!=0){
				removeCase(tempCase);
				n++;
			}
			tempCase = nextCase;
		}
		needSave = true;
		return n;
	};
	void setAFeature(FeatureNode* aFeatureNode, int updateValue){
		T* tempCase = first;
		while(tempCase!=NULL){ //scan through the cases in a group
			tempCase->theAttr->setAFeature(aFeatureNode, updateValue);
			tempCase = tempCase->next;
		}
		needSave = true;
	};
	void setFeatureConfig(List<FeatureNode>* aFeatureList, int updateValue){
												//updateValue: 0: keep original central values
												//		   1: use new central values
			//set the whole list (group) with the same config
		T* tempCase = first;
		while(tempCase!=NULL){ //scan through the cases in a group
			tempCase->theAttr->setFeatureConfig(aFeatureList, updateValue);
			tempCase = tempCase->next;
		}
		needSave = true;
	};
	//fill the mean, min, max, and std for all the features in the KB from the current env database
	void fillFeatureStats(EnvDatabase* theEnvDatabase){
		T* tempCase = first;
		while(tempCase!=NULL){ //scan through the cases in a group
			tempCase->theAttr->fillFeatureStats(theEnvDatabase);
			tempCase = tempCase->next;
		}
		needSave = true;
	};
	void fillCasePixels(EnvDatabase* theEnvDatabase){ //only for line/poly case
		T* tempCase = first;
		while(tempCase!=NULL){ //scan through the cases in a group
			tempCase->setCasePixels(theEnvDatabase);
			tempCase = tempCase->next;
		}
		needSave = true;
	}
	void setCentralValue(){ //sets the central value for line/poly case according to the specified method
		T* tempCase = first;
		while(tempCase!=NULL){ //scan through the cases in a group
			tempCase->setCentralValue();
			tempCase = tempCase->next;
		}
		needSave = true;
	}
	void setSpatialSetting(float aSearchDist, bool aUseDistSim, float aDistDecayFactor, 
							float aVtoHConvertingFactor, 
							int anIntegratingAlgo, float aWgtDecayFactor){
		//set the whole list (group) with the same spatial setting
		T* tempCase = first;
		while(tempCase!=NULL){ //scan through the cases in a group
			tempCase->theSpatial->setSpatialSetting(aSearchDist, aUseDistSim, aDistDecayFactor, 
													aVtoHConvertingFactor, 
													anIntegratingAlgo, aWgtDecayFactor);
		tempCase = tempCase->next;
		}
		needSave = true;
	};
	void addCase(T* aCase){
		if(aCase == NULL) return;
		numCases++;
		needSave = true;
		if(first ==NULL){
			first = aCase;
			first->next = NULL;
		}
		else{
			T* tempCase = first;
			while(tempCase->next!=NULL)
				tempCase = tempCase->next;
			tempCase->next = aCase;
			aCase->next = NULL;
		}
	};
	void removeCase(T* aCase){
		if(aCase == NULL) return;
		if(first == NULL) return;
		if(first == aCase)	first = first->next;
		else{
			T* temp = first;
			while(temp!=NULL){
				if(temp->next == aCase){
					temp->next = aCase->next;
					break;
				}
				temp=temp->next;
			}
		}
		delete aCase;
		numCases--;
		needSave = true;
	};
	void checkSave(){
		T* temp = first;
		while(temp != NULL){
			temp->checkSave();
			if(temp->needSave){
				needSave = true;
			}
			temp = temp->next;
		}
	};
};


/*******************************************
 CaseBase Class
 *******************************************/
template<class T>
class CaseBase{
public:
	int uid; //for assigning unique ids to lists
	CString name;
	CString fileName;
	CaseList<T>* first;
	int numOfTypes;
	bool needSave;

	
	void setSaved()
	{
		CaseList<T>* tempCaseList = first;
		while(tempCaseList!=NULL)
		{
			tempCaseList->setSaved();
			tempCaseList=tempCaseList->next;
		}
		needSave = false;
	}



	CaseBase(){
		uid = 1;
		name = "";
		fileName = "";
		first = NULL;
		numOfTypes = 0;
		needSave = true;
	};
	CaseBase(const CaseBase& aCaseBase){
		uid = aCaseBase.uid;
		name = aCaseBase.name;
		fileName = aCaseBase.fileName;
		numOfTypes = aCaseBase.numOfTypes;
		needSave = true;
		first = NULL;
	
		CaseList* tempList = aCaseBase.first;
		while(tempList != NULL){
			addList(new CaseList(*tempList));
			tempList = tempList->next;
		}
	};
	~CaseBase(){
		name = "";
		fileName = "";
		numOfTypes = 0;
		CaseList<T>* tempRuleList = first;
		while(first!=NULL){
			first = tempRuleList->next;
			delete tempRuleList;
			tempRuleList = first;
		}
	};

	int readPointFile(CString fileName, CString fieldName){
		//1. read the point Shapefile
		List<ICDDNode>* pointList = new List<ICDDNode>;
		int flag = readShapePoints (fileName, pointList, fieldName);
		if(flag != 0) return flag;

		//2. build the casebase.
		int caseListID = 1;
		int caseID = 1;
		ICDDNode* tempPoint = pointList->first;
		while(tempPoint!=NULL){
			bool labFlag = true;
			CaseList<T>* tempList = first;
    		while(tempList!=NULL){
				if(tempPoint->i == tempList->typeID){//this soil type has been in the case base
					labFlag=false; 
					T* tempNode = tempList->first;
					bool pntFlag = true;
					while(tempNode != NULL){ //check if duplicate point
						if(fabs(tempNode->theSpatial->x-tempPoint->d1)<VERYSMALL && fabs(tempNode->theSpatial->y-tempPoint->d2)<VERYSMALL){
							pntFlag = false; //duplicated point
							break;
						}
						tempNode = tempNode->next;
					}
					if(pntFlag){ //a new point, add the point to the list
						CString str;
						str.Format("%d", caseID);
						tempList->addCase(new T(caseID++, tempPoint->c+"_"+str, tempPoint->d1, tempPoint->d2));
					}
					break;
				}
				tempList=tempList->next;
			}
			if(labFlag){ //a new soil type, create a new caselist
				CaseList<T>* newList = new CaseList<T>(caseListID++, tempPoint->i, tempPoint->c);
				CString str;
				str.Format("%d", caseID);
				newList->addCase(new T(caseID++, tempPoint->c+"_"+str, tempPoint->d1, tempPoint->d2));
				addList(newList);
			}
			tempPoint = tempPoint->next;
		}
		delete pointList;
		needSave = true;
		return 0;
	};

	int readLineFile(CString LineFile, CString fieldName){
		//1. read the line Shapefile
		List<xPolyline>* lineList = new List<xPolyline>;
		List<ICNode>* typeNames = new List<ICNode>;
		int flag = readShapeLines(LineFile, lineList, typeNames, fieldName);
		if(flag != 0) return flag;

		//2. build the casebase.
		int caseListID = 1;
		int caseID = 1;
		xPolyline* tempLine = lineList->first;
		ICNode* tempName = typeNames->first;
		while(tempLine!=NULL && tempName!=NULL){
			bool labFlag = true;
			CaseList<T>* tempList = first;
    		while(tempList!=NULL){
				if(tempLine->theID == tempList->typeID){//this soil type has been in the case base
					labFlag=false; 
					T* tempNode = tempList->first;
					CString str;
					str.Format("%d", tempList->numCases+1);
					tempList->addCase(new T(caseID++, tempName->c+"_"+str, new List<IDDNode>(*tempLine->vertexList)));
					break;
				}
				tempList=tempList->next;
			}
			if(labFlag){ //a new soil type, create a new caselist
				CaseList<T>* newList = new CaseList<T>(caseListID++, tempName->i, tempName->c);
				CString str;
				str.Format("%d", newList->numCases+1);
				newList->addCase(new T(caseID++, tempName->c+"_"+str, new List<IDDNode>(*tempLine->vertexList)));
				addList(newList);
			}
			tempLine = tempLine->next;
			tempName = tempName->next;
		}
		delete lineList;
		delete typeNames;
		needSave = true;
		return 0;
	};

	int readPolyFile(CString PolyFile, CString fieldName){
		//1. read the polygon Shapefile
		List<xPolygon>* polyList = new List<xPolygon>;
		List<ICNode>* typeNames = new List<ICNode>;
		int flag = readShapePolygons(PolyFile, polyList, typeNames, fieldName);
		if(flag != 0) return flag;

		//2. build the casebase.
		int caseListID = 1;
		int caseID = 1;
		xPolygon* tempPoly = polyList->first;
		ICNode* tempName = typeNames->first;
		while(tempPoly!=NULL && tempName!=NULL){
			bool labFlag = true;
			CaseList<T>* tempList = first;
    		while(tempList!=NULL){
				if(tempPoly->theID == tempList->typeID){//this soil type has been in the case base
					labFlag=false; 
					T* tempNode = tempList->first;
					CString str;
					str.Format("%d", tempList->numCases+1);
					tempList->addCase(new T(caseID++, tempName->c+"_"+str, new xPolygon(*tempPoly)));
					break;
				}
				tempList=tempList->next;
			}
			if(labFlag){ //a new soil type, create a new caselist
				CaseList<T>* newList = new CaseList<T>(caseListID++, tempName->i, tempName->c);
				CString str;
				str.Format("%d", newList->numCases+1);
				newList->addCase(new T(caseID++, tempName->c+"_"+str, new xPolygon(*tempPoly)));
				addList(newList);
			}
			tempPoly = tempPoly->next;
			tempName = tempName->next;
		}
		delete polyList;
		delete typeNames;
		needSave = true;
		return 0;
	};

	void addList(CaseList<T>* aCaseList){
		if(aCaseList == NULL) return;
		numOfTypes++;
		needSave = true;
		if(first==NULL){
			first = aCaseList;
			first->next = NULL;
		}
		else{
			CaseList<T>* tempList = first;
			while(tempList->next!=NULL)
				tempList = tempList->next;
			tempList->next = aCaseList;
			aCaseList->next = NULL;
		}
	};
	void removeList(CaseList<T>* aCaseList){
		if(aCaseList == NULL) return;
		if(first == NULL) return;
		//CaseList<T>* tagList;

		if(first == aCaseList)	
		{
			first = first->next;
			//tagList = first;
		}
		else{
			CaseList<T>* temp = first;
			while(temp!=NULL){
				if(temp->next == aCaseList){
					temp->next = aCaseList->next;
					break;
				}
				temp=temp->next;
			}
			//tagList = temp->next;
		}
		delete aCaseList;

		/*while(tagList!=NULL)
		{
			tagList ->id --;
			tagList = tagList->next;
		}*/

		numOfTypes--;
		needSave = true;
	};
	int fillFeatureDefaults(EnvDatabase* theEnvDatabase){
		int n = 0; //total number of cases that are out of spatial range
		CaseList<T>* tempList = first;
		while(tempList!=NULL){ //scan through the case groups of soil types
			CaseList<T>* nextList = tempList->next;
			n += tempList->setFeatureDefaults(theEnvDatabase);
			if(tempList->numCases==0)
				removeList(tempList);
			tempList = nextList;
		}
		needSave = true;
		return n;
	};
	void fillAFeature(FeatureNode* aFeatureNode, int updateValue){
		//set the whole casebase with the same feature values
		CaseList<T>* tempList = first;
		while(tempList!=NULL){ //scan through the case groups of soil types
			tempList->setAFeature(aFeatureNode, updateValue);
			tempList = tempList->next;
		}
		needSave = true;
	};
	void fillFeatureConfig(List<FeatureNode>* aFeatureList, int updateValue){
									//updateValue: 0: keep original central values
									//		   1: use new central values
		//set the whole casebase with the same config
		CaseList<T>* tempList = first;
		while(tempList!=NULL){ //scan through the case groups of soil types
			tempList->setFeatureConfig(aFeatureList, updateValue);
			tempList = tempList->next;
		}
		needSave = true;
	};
	void fillCasePixels(EnvDatabase* theEnvDatabase){ //fills caseStats and casePixels (for line/poly case)
		CaseList<T>* tempList = first;
		while(tempList!=NULL){ //scan through the case groups of soil types
			tempList->fillCasePixels(theEnvDatabase);
			tempList = tempList->next;
		}
	};
	void setCentralValue(){ //sets the central value for line/poly case according to the specified method
		CaseList<T>* tempList = first;
		while(tempList!=NULL){ //scan through the case groups of soil types
			tempList->setCentralValue();
			tempList = tempList->next;
		}
		needSave = true;
	}
	void fillSpatialSetting(float aSearchDist, bool aUseDistSim, float aDistDecayFactor, 
							float aVtoHConvertingFactor, 
							int anIntegratingAlgo, float aWgtDecayFactor){
		//set the whole casebase with the same spatial setting
		CaseList<T>* tempList = first;
		while(tempList!=NULL){ //scan through the case groups of soil types
			tempList->setSpatialSetting(aSearchDist, aUseDistSim, aDistDecayFactor, 
										aVtoHConvertingFactor,
										anIntegratingAlgo, aWgtDecayFactor);
			tempList = tempList->next;
		}
		needSave = true;
	};

	// Checks if the features in KB matches feature in the env database
	int checkFeatureMatch(EnvDatabase* theEnvDatabase){
		if(first==NULL || first->first==NULL || first->first->theAttr->theFeatureList==NULL) return -1;

		//if theEnvDatabase is empty, return "unmatch"
		if(theEnvDatabase == NULL) return -2;

		//if the env db is a brand new one, replace all the features
		if(theEnvDatabase->newEnv)	return -3;

		//compare the features in the first rule with theEnvDatabase
		//to find out if any feature in KB is not found in envDB
		FeatureNode* firstFeature = first->first->theAttr->theFeatureList->first;
		while(firstFeature!=NULL){
			int id = -9999;
			EnvLayer* tempLayer = theEnvDatabase->firstLayer;
			while(tempLayer != NULL){
				if(firstFeature->id==tempLayer->id)
					id = firstFeature->id;
				tempLayer = tempLayer->nextLayer;
			}
			if(id == -9999) return -4;
			firstFeature = firstFeature->next;
		}

		//compare the features in the first rule with theEnvDatabase
		//to find out if any feature in envDB is not found in KB.
		EnvLayer* tempLayer = theEnvDatabase->firstLayer;
		while(tempLayer != NULL){
			int id = -9999;
			firstFeature = first->first->theAttr->theFeatureList->first;
			while(firstFeature!=NULL){
				if(tempLayer->id==firstFeature->id){
					id = tempLayer->id;
					break;
				}
				firstFeature = firstFeature->next;
			}
			if(id == -9999) return -5;
			tempLayer = tempLayer->nextLayer;
		}
		return 0;
	};

	//Update the features in the KB if theEnvDatabase changes
	void updateFeatures(EnvDatabase* theEnvDatabase){
		if(first==NULL || first->first==NULL || 
			first->first->theAttr==NULL ||first->first->theAttr->theFeatureList==NULL)
			return;

		//if theEnvDatabase is empty, remove all the features in the KB
		if(theEnvDatabase == NULL || theEnvDatabase->numOfLayers==0){ 
			CaseList<T>* tempList = first;
			while(tempList!=NULL){
				T* tempCase = first->first;
				while(tempCase != NULL){
					tempCase->theAttr->theFeatureList->clean();
					tempCase = tempCase->next;
				}
				tempList = tempList->next;
			}
		}

		//if the env db is a brand new one, replace all the features
		if(theEnvDatabase->newEnv){
			CaseList<T>* tempList = first;
			while(tempList!=NULL){
				T* tempCase = first->first;
				while(tempCase != NULL){
					tempCase->theAttr->theFeatureList->clean();
					EnvLayer* tempLayer = theEnvDatabase->firstLayer;
					while(tempLayer != NULL){
						int rowNo = tempLayer->theData->rowOfAPoint(tempCase->theSpatial->y);
						int colNo = tempLayer->theData->colOfAPoint(tempCase->theSpatial->x);
						int nrow = tempLayer->theData->getNumberOfRows();
						int ncol = tempLayer->theData->getNumberOfColumns();
						if(rowNo<0 || rowNo>=nrow || colNo<0 || colNo>=ncol){
							rowNo = -1;
							colNo = -1;
						}
						tempCase->theAttr->addADefaultFeature(tempLayer, rowNo, colNo);
						tempLayer = tempLayer->nextLayer;
					}
					tempCase = tempCase->next;
				}
				tempList = tempList->next;
			}
			return;
		}

		//compare the features in the first rule with theEnvDatabase
		//to find out if any old feature(s) need to be removed from the featureList,
		//then apply the finding to the whole KB.
		FeatureNode* firstFeature = first->first->theAttr->theFeatureList->first;
		while(firstFeature!=NULL){
			int id = -9999;
			EnvLayer* tempLayer = theEnvDatabase->firstLayer;
			while(tempLayer != NULL){
				if(firstFeature->id==tempLayer->id){  //found the matched layer
					id = firstFeature->id;
					//update name in case it has been changed by the user
					CaseList<T>* tempList = first;
					while(tempList != NULL){
						T* tempCase = tempList->first;
						while(tempCase != NULL){
							FeatureNode* tempFeature = tempCase->theAttr->theFeatureList->first;
							while(tempFeature != NULL){
								if(tempFeature->id == id){
									tempFeature->name = tempLayer->layerName;
									break;
								}
								tempFeature = tempFeature->next;
							}
							tempCase = tempCase->next;
						}
						tempList = tempList->next;
					}
					break;
				}
				tempLayer = tempLayer->nextLayer;
			}
			if(id == -9999){
				id = firstFeature->id; //the id of the feature that should be removed
				firstFeature = firstFeature->next; //move to the next before doing removal
				CaseList<T>* tempList = first;
				while(tempList != NULL){
					T* tempCase = tempList->first;
					while(tempCase != NULL){
						FeatureNode* tempFeature = tempCase->theAttr->theFeatureList->first;
						while(tempFeature != NULL){
							if(tempFeature->id == id){
								tempCase->theAttr->theFeatureList->removeNode(tempFeature);
								break;
							}
							tempFeature = tempFeature->next;
						}
						tempCase = tempCase->next;
					}
					tempList = tempList->next;
				}
			}
			else
				firstFeature = firstFeature->next;
		}
		//compare the features in the first rule with theEnvDatabase
		//to find out if any new feature(s) need to be added to the featureList,
		//then apply the finding to the whole KB.
		EnvLayer* tempLayer = theEnvDatabase->firstLayer;
		while(tempLayer != NULL){
			int id = -9999;
			firstFeature = first->first->theAttr->theFeatureList->first;
			while(firstFeature!=NULL){
				if(tempLayer->id==firstFeature->id){
					id = tempLayer->id;
					break;
				}
				firstFeature = firstFeature->next;
			}
			if(id == -9999){
				CaseList<T>* tempList = first;
				while(tempList != NULL){
					T* tempCase = tempList->first;
					while(tempCase!=NULL){
						int rowNo = tempLayer->theData->rowOfAPoint(tempCase->theSpatial->y);
						int colNo = tempLayer->theData->colOfAPoint(tempCase->theSpatial->x);
						int nrow = tempLayer->theData->getNumberOfRows();
						int ncol = tempLayer->theData->getNumberOfColumns();
						if(rowNo<0 || rowNo>=nrow || colNo<0 || colNo>=ncol){
							rowNo = -1;
							colNo = -1;
						}
						tempCase->theAttr->addADefaultFeature(tempLayer, rowNo, colNo);
						tempCase = tempCase->next;
					}
					tempList = tempList->next;
				}
			}
			tempLayer = tempLayer->nextLayer;
		}
		//Reset data stats for the features, in case the user change the data file, even if the feature name is not changed
		fillFeatureStats(theEnvDatabase);
	};
	//fill the mean, min, max, and std for all the features in the KB from the current env database
	void fillFeatureStats(EnvDatabase* theEnvDatabase){
		CaseList<T>* tempList = first;
		while(tempList!=NULL){ //scan through the case groups of soil types
			tempList->fillFeatureStats(theEnvDatabase);
			tempList = tempList->next;
		}
		needSave = true;
	};
	void checkSave(){
		CaseList<T>* temp = first;
		while(temp != NULL){
			temp->checkSave();
			if(temp->needSave){
				needSave = true;
			}
			temp = temp->next;
		}
	};
};


/**************************************
 Functions of inference
 **************************************/

float nominalFeatureSimilarity(float zprime, float z1, float z2);

float ordinalFeatureSimilarity(float zprime, float zmin, float zmax,
							   float z1, float w1, 
							   float z2, float w2);

float continuousFeatureSimilarity(float zprime, 
								  float z1, float w1, float r1, float k1, 
								  float z2, float w2, float r2, float k2);

float cyclicFeatureSimilarity(float zprime, 
								   float z1, float w1, float r1, float k1,
								   float z2, float w2, float r2, float k2);

float limitingFactor(ValueNode** featureValueArray, Attr* theCaseAttr, int& env, float nodata);

float weightedAverage(ValueNode** featureValueArray, Attr* theCaseAttr, int& env, float nodata);

float multiplication(ValueNode** featureValueArray, Attr* theCaseAttr, int& env, float nodata);

float visibility(double x1, double y1, float h1, 
				 double x2, double y2, float h2, 
				 double xx, double yx, float hx);

float topoDistanceRC(int r1, int c1, int r2, int c2, CThreeDR* theDEM, 
					 float VtoCFactor, int numOfIntervals, float threshold, 
					 float upwardFactor, float downwardFactor);

float topoDistanceXY(double x1, double y1, double x2, double y2, CThreeDR* theDEM, 
					 float VtoCFactor, float cellFraction, float threshold, 
					 float upwardFactor, float downwardFactor); 

float distancedSimilarity(float featureSpaceSim, float dist, float searchDist, float decayFactor);

float integratingCases(List<IIIDDDNode>* theSimilarityList);

#endif

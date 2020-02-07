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
/* Knowledge.ccp
 * 
 * prepared by Xun Shi
 * last modified: Dec. 15, 2002
 *
 * Dec 15, 2002: Add integratingAlgo to case, 
 *					move wgtDecayFactor from KnowledgeList to Spatial, 
 *					remove wgtDecayFactor from RasterCase
 *					Chage file name from cbr to knowledge
 *					Move the files to this Library
 *					Make PointCase and rasterCase to be children of Attr (actually attribute rule).
 */
#include "stdafx.h"
#include "knowledge.h"

/***************************
 AHP__ScaleList Class
 ***************************/
AHP_ScaleList::AHP_ScaleList(){
	name = "";
	weight = 1;
	factorList = new List<FCNode>;
	before = NULL;
	next = NULL;
}

AHP_ScaleList::AHP_ScaleList(CString aName, EnvDatabase* anEnvDatabase){
	name = aName;
	weight = 1;
	factorList = new List<FCNode>;
	EnvLayer* temp = anEnvDatabase->firstLayer;
	while(temp!=NULL){
		factorList->addTail(new FCNode(1, temp->layerName));
		temp = temp->nextLayer;
	}
	before = NULL;
	next = NULL;
}

AHP_ScaleList::AHP_ScaleList(const AHP_ScaleList& anAHP_ScaleList){
	name = anAHP_ScaleList.name;
	weight = anAHP_ScaleList.weight;
	factorList = new List<FCNode>(*(anAHP_ScaleList.factorList));
	before = NULL;
	next = NULL;
}

AHP_ScaleList::~AHP_ScaleList(){
	delete factorList;
	before = NULL;
	next = NULL;
}

/***************************
 Attr Class
 ***************************/
Attr::Attr(){
	id = -1;
	name = "";
	algo = "Limiting-Factor";
	memberValue = 1;
	positive = 1;
	effective = false;
	useMask = false;
	maskFileName = "";
	mask = NULL;
	inverseMaskValue = false;
	needSave = true;
	theFeatureList = new List<FeatureNode>();
	next = NULL;
}

Attr::Attr(List<FeatureNode>* aFeatureList){
	id = -1;
	name = "";
	algo = "Limiting-Factor";
	memberValue = 1;
	positive = 1;
	effective = false;
	useMask = false;
	maskFileName = "";
	mask = NULL;
	inverseMaskValue = false;
	needSave = true;
	theFeatureList = aFeatureList;
	next = NULL;
}

Attr::Attr(const Attr& anAttr){
	id = anAttr.id;
	name = anAttr.name;
	effective = anAttr.effective;
	algo = anAttr.algo;
	memberValue = anAttr.memberValue;
	positive = anAttr.positive;
	useMask = anAttr.useMask;
	maskFileName = anAttr.maskFileName;
	if(anAttr.mask!=NULL)
		mask = new CThreeDR(*(anAttr.mask));
	else
		mask = NULL;
	inverseMaskValue = anAttr.inverseMaskValue;
	needSave = true;
	theFeatureList = new List<FeatureNode>(*(anAttr.theFeatureList));
	next = NULL;
}

Attr::~Attr(){
	id = -1;
	name = "";
	useMask = false;
	maskFileName = "";
	if(mask!=NULL){
		delete mask;
		mask = NULL;
	}
	inverseMaskValue = false;
	delete theFeatureList;
	theFeatureList = NULL;
	next = NULL;
}

//adds a new feature to theFeatureList.
//set the values of the feature to be the default values 
//calculated from anEnvLayer.
int Attr::addADefaultFeature(EnvLayer* anEnvLayer, int rowNo, int colNo){
	if(anEnvLayer==NULL) return 1;
	if(theFeatureList==NULL) return 2;
	float value1 = 0;
	float value2 = 0;
	int nrow = anEnvLayer->theData->getNumberOfRows();
	int ncol = anEnvLayer->theData->getNumberOfColumns();
	float mean = anEnvLayer->theData->getDataMean();
	if(rowNo<0 || rowNo>nrow-1|| colNo<0 || colNo>ncol-1)
		value1 = mean;
	else
		value1 = anEnvLayer->theData->getDataBuf()[rowNo*ncol+colNo];
	value2 = value1;
	float std = anEnvLayer->theData->getDataStd();
	double tempW = WIDTHFACTOR*std;
	FeatureNode* newFeature = new FeatureNode(anEnvLayer->id, false, anEnvLayer->layerName, 
											DEFAULTWEIGHT, "Continuous:bell-shape", 
											value1, tempW, DEFAULTK, DEFAULTR, 
											value2, tempW, DEFAULTK, DEFAULTR);
	newFeature->mean = mean;
	newFeature->min = anEnvLayer->theData->getDataMin();
	newFeature->max = anEnvLayer->theData->getDataMax();
	newFeature->std = std;
	theFeatureList->addTail(newFeature);
	needSave = true;
	return 0;
}

int Attr::setFeatureDefaultsXY(EnvDatabase* theEnvDatabase, double x, double y){
	//if theEnvDatabase is empty, return
	if(theEnvDatabase==NULL) return 1;
	EnvLayer* tempLayer = theEnvDatabase->firstLayer;
	if(tempLayer==NULL) return 1;

	int rowNo = 0;
	int colNo = 0;
	int nrow = 0;
	int ncol = 0;
	if(fabs(x-NODOUBLEVALUE)<VERYSMALL || fabs(y-NODOUBLEVALUE)<VERYSMALL){
		rowNo = -1;
		colNo = -1;
	}
	else{	//find the location in the env data layers, if outside the area, return
		rowNo = tempLayer->theData->rowOfAPoint(y);
		colNo = tempLayer->theData->colOfAPoint(x);
		nrow = theEnvDatabase->theSetting->nrow;
		ncol = theEnvDatabase->theSetting->ncol;
		if(rowNo<0 || rowNo>=nrow || colNo<0 || colNo>=ncol)
			return 2;

		if(x<theEnvDatabase->theSetting->west_min || x>theEnvDatabase->theSetting->east_min || y<theEnvDatabase->theSetting->south_min || y>theEnvDatabase->theSetting->north_min)
			return 2;
	}
	//reset the feature list
	if(theFeatureList != NULL)
		delete theFeatureList;
	theFeatureList = new List<FeatureNode>;
	algo = "Limiting-Factor";

	//create new nodes in the feature list, and set the values
	while(tempLayer != NULL){
		addADefaultFeature(tempLayer, rowNo, colNo);
		tempLayer = tempLayer->nextLayer;
	}
	effective = true;
	needSave = true;
	return 0;
}

int Attr::setFeatureDefaultsRW(EnvDatabase* theEnvDatabase, int rowNo, int colNo){
	//if theEnvDatabase is empty, return
	EnvLayer* tempLayer = theEnvDatabase->firstLayer;
	if(tempLayer==NULL) return 1;

	int nrow = 0;
	int ncol = 0;
	if(rowNo == NOINTVALUE || colNo == NOINTVALUE){
		rowNo = -1;
		colNo = -1;
	}
	else{	//find the location in the env data layers, if outside the area, return
		nrow = theEnvDatabase->theSetting->nrow;
		ncol = theEnvDatabase->theSetting->ncol;
		if(rowNo<0 || rowNo>=nrow || colNo<0 || colNo>=ncol)
			return 2;
	}
	//reset the feature list
	if(theFeatureList != NULL)
		delete theFeatureList;
	theFeatureList = new List<FeatureNode>;
	algo = "Limiting-Factor";

	//create new nodes in the feature list, and set the values
	while(tempLayer != NULL){
		addADefaultFeature(tempLayer, rowNo, colNo);
		tempLayer = tempLayer->nextLayer;
	}
	effective = true;
	needSave = true;
	return 0;
}

//update values of an individual feature.
//used when the user change the variable values on the interface
//and wants to apply the change to more than one rule (Attr).
//uses id to match the two features
void Attr::setAFeature(FeatureNode* aFeatureNode, int updateValue){
	FeatureNode* tempFeature = theFeatureList->first;
	while(tempFeature != NULL){
		if(tempFeature->id == aFeatureNode->id){
			tempFeature->useOrNot = aFeatureNode->useOrNot;
			tempFeature->weight = aFeatureNode->weight;
			tempFeature->functionType = aFeatureNode->functionType;
			tempFeature->width1 = aFeatureNode->width1;
			tempFeature->k1 = aFeatureNode->k1;
			tempFeature->r1 = aFeatureNode->r1;
			tempFeature->width2 = aFeatureNode->width2;
			tempFeature->k2 = aFeatureNode->k2;
			tempFeature->r2 = aFeatureNode->r2;
			if(updateValue==1){
				tempFeature->value1 = aFeatureNode->value1;
				tempFeature->value2 = aFeatureNode->value2;
			}
			break;
		}
		tempFeature = tempFeature->next;
	}
	needSave = true;
}

//Actually copy featureList values to another rule
//can be used when want to apply the feature config to the whole rule list
void Attr::setFeatureConfig(List<FeatureNode>* aFeatureList, int updateValue){
										//set values for the whole feature list
										//updateValue: 0: keep original central values
										//			   1: use new central values
	if(theFeatureList->first==aFeatureList->first)
		return; //avoid change if aFeatureList is theFeatureList itself
	FeatureNode* oldFeature = theFeatureList->first;
	while(oldFeature != NULL){
		oldFeature->useOrNot = false;
		oldFeature = oldFeature->next;
	}
	FeatureNode* tempFeature = aFeatureList->first;
	while(tempFeature!=NULL){
		setAFeature(tempFeature, updateValue);
		tempFeature = tempFeature->next;
	}
	needSave = true;
}

//fill the mean, min, max, and std for all the features in the KB from the current env database
void Attr::fillFeatureStats(EnvDatabase* theEnvDatabase){
	//if theEnvDatabase is empty, return
	EnvLayer* tempLayer = theEnvDatabase->firstLayer;
	if(tempLayer==NULL) return;
	FeatureNode* tempFeature = theFeatureList->first;
	while(tempFeature!=NULL){
		tempLayer = theEnvDatabase->firstLayer;
		while(tempLayer!=NULL){
			if(tempFeature->id == tempLayer->id)
				break;
			tempLayer = tempLayer->nextLayer;
		}
		tempFeature->mean = tempLayer->theData->getDataMean();
		tempFeature->max = tempLayer->theData->getDataMax();
		tempFeature->min = tempLayer->theData->getDataMin();
		tempFeature->std = tempLayer->theData->getDataStd();
		tempFeature = tempFeature->next;
	}
	effective = true;
	needSave = true;
}

//These two functions read local values for v1 and v2 (v1=v2=the local value)
int Attr::readLocalValuesXY(EnvDatabase* theEnvDatabase, double x, double y){ 
	if(theEnvDatabase==NULL) return 1;
	EnvLayer* tempLayer = theEnvDatabase->firstLayer;
	if(tempLayer==NULL) return 1;

	int rowNo = 0;
	int colNo = 0;
	int nrow = 0;
	int ncol = 0;
	if(fabs(x-NODOUBLEVALUE)<VERYSMALL || fabs(y-NODOUBLEVALUE)<VERYSMALL){
		rowNo = -1;
		colNo = -1;
	}
	else{	//find the location in the env data layers, if outside the area, return
		rowNo = tempLayer->theData->rowOfAPoint(y);
		colNo = tempLayer->theData->colOfAPoint(x);
		nrow = theEnvDatabase->theSetting->nrow;
		ncol = theEnvDatabase->theSetting->ncol;
		if(rowNo<0 || rowNo>=nrow || colNo<0 || colNo>=ncol)
			return 2;
	}

	//create new nodes in the feature list, and set the values
	while(tempLayer != NULL){
		readALocalValue(tempLayer, rowNo, colNo);
		tempLayer = tempLayer->nextLayer;
	}
	effective = true;
	needSave = true;
	return 0;
}

//They do not change other parts of the featurelist
int Attr::readLocalValuesRW(EnvDatabase* theEnvDatabase, int rowNo, int colNo){	
	EnvLayer* tempLayer = theEnvDatabase->firstLayer;
	if(tempLayer==NULL) return 1;
	int nrow = 0;
	int ncol = 0;
	if(rowNo == NOINTVALUE || colNo == NOINTVALUE){
		rowNo = -1;
		colNo = -1;
	}
	else{	//find the location in the env data layers, if outside the area, return
		nrow = theEnvDatabase->theSetting->nrow;
		ncol = theEnvDatabase->theSetting->ncol;
		if(rowNo<0 || rowNo>=nrow || colNo<0 || colNo>=ncol)
			return 2;
	}
	//create new nodes in the feature list, and set the values
	while(tempLayer != NULL){
		readALocalValue(tempLayer, rowNo, colNo);
		tempLayer = tempLayer->nextLayer;
	}
	effective = true;
	return 0;
}

void Attr::readALocalValue(EnvLayer* anEnvLayer, int rowNo, int colNo){
	if(anEnvLayer==NULL) return;
	if(theFeatureList==NULL) return;
	int nrow = anEnvLayer->theData->getNumberOfRows();
	int ncol = anEnvLayer->theData->getNumberOfColumns();
	float value = 0;
	if(rowNo<0 || rowNo>nrow-1|| colNo<0 || colNo>ncol-1)
		value = anEnvLayer->theData->getDataMean();
	else
		value = anEnvLayer->theData->getDataBuf()[rowNo*ncol+colNo];
	FeatureNode* tempFeature = theFeatureList->first;
	while(tempFeature != NULL){
		if(tempFeature->id == anEnvLayer->id){
			tempFeature->value1 = value;
			tempFeature->value2 = value;
			break;
		}
		tempFeature = tempFeature->next;
	}
	needSave = true;

}


void Attr::setSaved()
{
	needSave = false;
}


/***************************
 RuleList Class
 ***************************/
RuleList::RuleList(){
	id = -1;
	uid = 1;
	typeID = -1;
	name = "";
	numRules = 0;
	useMask = false;
	maskFileName = "";
	mask = NULL;
	inverseMaskValue = false;
	needSave = true;
	first = NULL;
	next = NULL;
}

RuleList::RuleList(const RuleList& aRuleList){
	id = aRuleList.id;
	uid = aRuleList.uid;
	typeID = aRuleList.typeID;
	name = aRuleList.name;
	numRules = aRuleList.numRules;
	useMask = aRuleList.useMask;
	maskFileName = aRuleList.maskFileName;
	if(aRuleList.mask!=NULL)
		mask = new CThreeDR(*(aRuleList.mask));
	else
		mask = NULL;
	inverseMaskValue = aRuleList.inverseMaskValue;
	needSave = true;
	Attr* tempRule = aRuleList.first;
	while(tempRule!=NULL){
		addRule(new Attr(*tempRule));
		tempRule = tempRule->next;
	}
	next = NULL;
}

RuleList::~RuleList(){
	id = -1;
	uid = 1;
	typeID = -1;
	name = "";
	numRules = 0;
	useMask = false;
	maskFileName = "";
	if(mask!=NULL){
		delete mask;
		mask = NULL;
	}
	inverseMaskValue = false;
	Attr* tempRule = first;
	while(first!=NULL){
		first = tempRule->next;
		delete tempRule;
		tempRule = first;
	}
	first = NULL;
}

void RuleList::addRule(Attr* aRule){
	if(aRule == NULL) return;
	if(first ==NULL){
		first = aRule;
		first->next = NULL;
	}
	else{
		Attr* tempRule = first;
		while(tempRule->next!=NULL)
			tempRule = tempRule->next;
		tempRule->next = aRule;
		aRule->next = NULL;
	}
	numRules++;
	needSave = true;
}

void RuleList::removeRule(Attr* aRule){
	if(aRule == NULL) return;
	if(first == NULL) return;
	Attr *tagAttr;
	if(first == aRule)
	{
		first = first->next;
		tagAttr = first;
	}

	else{
		Attr* temp = first;
		while(temp!=NULL){
			if(temp->next == aRule){
				temp->next = aRule->next;
				break;
			}
			temp=temp->next;
		}
		tagAttr = temp->next;
	}
	delete aRule;
	while(tagAttr!=NULL)
	{
		tagAttr->id--;
		tagAttr = tagAttr->next;
	}
	needSave=true;
	numRules--;
}

void RuleList::setFeatureDefaults(EnvDatabase* theEnvDatabase){
	Attr* temp = first;
	while(temp!=NULL){ //scan through the rules
		temp->setFeatureDefaultsXY(theEnvDatabase, NODOUBLEVALUE, NODOUBLEVALUE);
		temp = temp->next;
	}
	needSave = true;
	
}

void RuleList::setAFeature(FeatureNode* aFeatureNode, int updateValue){
	Attr* tempRule = first;
	while(tempRule!=NULL){ //scan through the cases in a group
		tempRule->setAFeature(aFeatureNode, updateValue);
		tempRule = tempRule->next;
	}
	needSave = true;

}

void RuleList::setFeatureConfig(List<FeatureNode>* aFeatureList, int updateValue){
	//set the whole list (group) with the same config
	Attr* tempRule = first;
	while(tempRule!=NULL){ //scan through the cases in a group
		tempRule->setFeatureConfig(aFeatureList, updateValue);
		tempRule = tempRule->next;
	}
	needSave = true;

}

void RuleList::fillFeatureStats(EnvDatabase* theEnvDatabase){
	Attr* tempRule = first;
	while(tempRule != NULL){
		tempRule->fillFeatureStats(theEnvDatabase);
		tempRule = tempRule->next;
	}
	needSave = true;

}

void RuleList::checkSave(){
	Attr* temp = first;
	while(temp != NULL){
		if(temp->needSave){
			needSave = true;
		}
		temp = temp->next;
	}
}

void RuleList::setSaved(){  //if  as rule list is saved,set the Attr in a Rule List to be saved

	Attr *temp = first;
	while(temp!=NULL)
	{
		temp->setSaved();
		temp= temp->next;
	}
	needSave = false;

}

/***************************
 RuleBase Class
 ***************************/
RuleBase::RuleBase(){
	uid = 1;
	name = "";
	fileName = "";
	numRuleLists = 0;
	needSave = true;
	first = NULL;
}

RuleBase::RuleBase(const RuleBase& aRuleBase){
	uid = aRuleBase.uid;
	name = aRuleBase.name;
	fileName = aRuleBase.fileName;
	numRuleLists = aRuleBase.numRuleLists;
	needSave = true;
	RuleList* tempRuleList = aRuleBase.first;
	while(tempRuleList!=NULL){
		addList(new RuleList(*tempRuleList));
		tempRuleList = tempRuleList->next;
	}
}

RuleBase::~RuleBase(){
	name = "";
	fileName = "";
	numRuleLists = 0;
	RuleList* tempRuleList = first;
	while(first!=NULL){
		first = tempRuleList->next;
		delete tempRuleList;
		tempRuleList = first;
	}
	first = NULL;
}

void RuleBase::setFeatureDefaults(EnvDatabase* theEnvDatabase){
	RuleList* tempList = first;
	while(tempList!=NULL){ //scan through the rule list in the base
		tempList->setFeatureDefaults(theEnvDatabase);
		tempList = tempList->next;
	}
	needSave = true;
	
}

void RuleBase::fillFeatureStats(EnvDatabase* theEnvDatabase){
	RuleList* tempList = first;
	while(tempList!=NULL){ //scan through the rule list in the base
		tempList->fillFeatureStats(theEnvDatabase);
		tempList = tempList->next;
	}
	needSave = true;

}

void RuleBase::addList(RuleList* aRuleList){
	if(aRuleList==NULL) return;
	if(first ==NULL){
		first = aRuleList;
		first->next = NULL;
	}
	else{
		RuleList* tempRuleList = first;
		while(tempRuleList->next!=NULL)
			tempRuleList = tempRuleList->next;
		tempRuleList->next = aRuleList;
		aRuleList->next = NULL;
	}
	numRuleLists++;
	needSave = true;
}

void RuleBase::removeList(RuleList* aRuleList){
	if(aRuleList==NULL) return;
	if(first == NULL) return;
	//RuleList *tagList;

	if(first == aRuleList)	
	{
		first = first->next;
		//tagList = first;
	}
	else{
		RuleList* temp = first;
		while(temp!=NULL){
			if(temp->next == aRuleList){
				temp->next = aRuleList->next;
				break;
			}
			temp=temp->next;
		}
		//tagList = temp->next;
	}
	delete aRuleList;
	/*while(tagList!=NULL)
	{
		tagList ->id --;
		tagList = tagList->next;
	}*/
	numRuleLists--;
	needSave = true;
}

void RuleBase::checkSave(){
	RuleList* temp = first;
	while(temp != NULL){
		temp->checkSave();
		if(temp->needSave){
			needSave = true;
			return;
		}
		temp = temp->next;
	}
}

void RuleBase::setSaved(){
	RuleList* temp = first;
	while(temp != NULL){
		temp->setSaved();
		temp = temp->next;
	}
	needSave = false;
}

// Checks if the features in KB matches feature in the env database
// Return values:
// 0: perfect match
// -1: empty KB
// -2: empty envDB
// -3: new envDB
// -4: feature in KB is not found in envDB
// -5: feature in envDB is not found in KB
int RuleBase::checkFeatureMatch(EnvDatabase* theEnvDatabase){
	if(first==NULL || first->first==NULL || first->first->theFeatureList==NULL) return -1;

	//if theEnvDatabase is empty, return "unmatch"
	if(theEnvDatabase == NULL) return -2;

	//if the env db is a brand new one, replace all the features
	if(theEnvDatabase->newEnv)	return -3;

	//compare the features in the first rule with theEnvDatabase
	//to find out if any feature in KB is not found in envDB
	FeatureNode* firstFeature = first->first->theFeatureList->first;
	while(firstFeature!=NULL){
		int id = -9999;
		EnvLayer* tempLayer = theEnvDatabase->firstLayer;
		while(tempLayer != NULL){
			if(firstFeature->name==tempLayer->layerName)
				id = 0;
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
		firstFeature = first->first->theFeatureList->first;
		while(firstFeature!=NULL){
			if(firstFeature->name==tempLayer->layerName){
				id = 0;
				break;
			}
			firstFeature = firstFeature->next;
		}
		if(id == -9999) return -5;
		tempLayer = tempLayer->nextLayer;
	}
	return 0;
}

//Update the features in the KB if theEnvDatabase changes
void RuleBase::updateFeatures(EnvDatabase* theEnvDatabase){
	if(first==NULL || first->first==NULL || first->first->theFeatureList==NULL) return;

	//if theEnvDatabase is empty, remove all the features in the KB
	if(theEnvDatabase == NULL || theEnvDatabase->numOfLayers==0){ 
		RuleList* tempList = first;
		while(tempList!=NULL){
			Attr* tempAttr = first->first;
			while(tempAttr != NULL){
				tempAttr->theFeatureList->clean();
				tempAttr = tempAttr->next;
			}
			tempList = tempList->next;
		}
		return;
	}

	//if the env db is a brand new one, replace all the features
	if(theEnvDatabase->newEnv){
		RuleList* tempList = first;
		while(tempList!=NULL){
			Attr* tempAttr = first->first;
			while(tempAttr != NULL){
				tempAttr->theFeatureList->clean();
				EnvLayer* tempLayer = theEnvDatabase->firstLayer;
				while(tempLayer != NULL){
					tempAttr->addADefaultFeature(tempLayer, -1, -1);
					tempLayer = tempLayer->nextLayer;
				}
				tempAttr = tempAttr->next;
			}
			tempList = tempList->next;
		}
		return;
	}

	//compare the features in the first rule with theEnvDatabase
	//to find out if any old feature(s) need to be removed from the featureList,
	//then apply the finding to the whole KB.
	FeatureNode* firstFeature = first->first->theFeatureList->first;
	while(firstFeature!=NULL){
		int id = -9999;
		EnvLayer* tempLayer = theEnvDatabase->firstLayer;
		while(tempLayer != NULL){
			if(firstFeature->id==tempLayer->id){
				id = firstFeature->id;
				//update name in case it has been changed by the user
				RuleList* tempList = first;
				while(tempList != NULL){
					Attr* tempAttr = tempList->first;
					while(tempAttr != NULL){
						FeatureNode* tempFeature = tempAttr->theFeatureList->first;
						while(tempFeature != NULL){
							if(tempFeature->id == id){
								tempFeature->name = tempLayer->layerName;
								break;
							}
							tempFeature = tempFeature->next;
						}
						tempAttr = tempAttr->next;
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
			RuleList* tempList = first;
			while(tempList != NULL){
				Attr* tempAttr = tempList->first;
				while(tempAttr != NULL){
					FeatureNode* tempFeature = tempAttr->theFeatureList->first;
					while(tempFeature != NULL){
						if(tempFeature->id == id){
							tempAttr->theFeatureList->removeNode(tempFeature);
							break;
						}
						tempFeature = tempFeature->next;
					}
					tempAttr = tempAttr->next;
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
		firstFeature = first->first->theFeatureList->first;
		while(firstFeature!=NULL){
			if(tempLayer->id==firstFeature->id){
				id = tempLayer->id;
				break;
			}
			firstFeature = firstFeature->next;
		}
		if(id == -9999){
			RuleList* tempList = first;
			while(tempList != NULL){
				Attr* tempAttr = tempList->first;
				while(tempAttr!=NULL){
					tempAttr->addADefaultFeature(tempLayer, -1, -1);
					tempAttr = tempAttr->next;
				}
				tempList = tempList->next;
			}
		}
		tempLayer = tempLayer->nextLayer;
	}

	//Reset data stats for the features, in case the user change the data file, even if the feature name is not changed
	fillFeatureStats(theEnvDatabase);
	needSave = true;
}

/***************************
 Spatial Class
 ***************************/
Spatial::Spatial(){
	id = -1;
	x = 0;
	y = 0;
	searchDist = DEFAULTSEARCHDIST;
	useDistSim = false;
	distDecayFactor = DEFAULTDISTDECAYFACTOR;
	VtoHConvertingFactor = DEFAULTVTOHFACTOR;
	integratingAlgo = 1;
	wgtDecayFactor = 1;
	needSave = true;
}

Spatial::Spatial(const Spatial& aSpatial){
	id = aSpatial.id;
	x = aSpatial.x;
	y = aSpatial.y;
	searchDist = aSpatial.searchDist;
	useDistSim = aSpatial.useDistSim;
	distDecayFactor = aSpatial.distDecayFactor;
	VtoHConvertingFactor = aSpatial.VtoHConvertingFactor;
	integratingAlgo = aSpatial.integratingAlgo;
	wgtDecayFactor = aSpatial.wgtDecayFactor;
	needSave = true;
}

void Spatial::setSpatialSetting(double anX, double aY, 
								float aSearchDist, bool aUseDistSim, float aDistDecayFactor, 
								float aVtoHConvertingFactor, 
								int anIntegratingAlgo, float aWgtDecayFactor){
	x = anX;
	y = aY;
	searchDist = aSearchDist;
	useDistSim = aUseDistSim;
	distDecayFactor = aDistDecayFactor;
	VtoHConvertingFactor = aVtoHConvertingFactor;
	integratingAlgo = anIntegratingAlgo;
	wgtDecayFactor = aWgtDecayFactor;
	needSave = true;
}

void Spatial::setSpatialSetting(Spatial* aSpatial, int updateValue){
	if(updateValue==1){
		x = aSpatial->x;
		y = aSpatial->y;
	}
	searchDist = aSpatial->searchDist;
	useDistSim = aSpatial->useDistSim;
	distDecayFactor = aSpatial->distDecayFactor;
	VtoHConvertingFactor = aSpatial->VtoHConvertingFactor;
	integratingAlgo = aSpatial->integratingAlgo;
	wgtDecayFactor = aSpatial->wgtDecayFactor;
	needSave = true;
}

void Spatial::setSaved(){
	needSave = false;
}

/***************************
 RasterCases Class
 ***************************/
RasterCases::RasterCases(){
	name = "";
	fileName = "";
	rasterFileName = "";
	theRaster = new CThreeDR;
	resampleRatio = 1;
	theAttr = new Attr;
	theSpatial = new Spatial;
	needSave = true;
}

RasterCases::RasterCases(CString aFileName){
	fileName = "";
	rasterFileName = aFileName;
	theRaster = new CThreeDR;
	int readFlag = theRaster->readin(aFileName.GetBuffer(1), 0);
	name = theRaster->getDataName();
	resampleRatio = 1;
	theAttr = new Attr;
	theAttr->name = name;
	theSpatial = new Spatial;
	if(readFlag == 0)
		theAttr->effective = true;
	else
		theAttr->effective = false;
	needSave = true;
	
}

RasterCases::RasterCases(const RasterCases& aRasterCases){
	name = aRasterCases.name;
	fileName = aRasterCases.fileName;
	rasterFileName = aRasterCases.rasterFileName;
	theRaster = new CThreeDR(*(aRasterCases.theRaster));
	resampleRatio = aRasterCases.resampleRatio;
	theAttr = new Attr(*(aRasterCases.theAttr));
	theSpatial = new Spatial(*(aRasterCases.theSpatial));
	needSave = true;
}

RasterCases::~RasterCases(){
	name = "";
	fileName = "";
	rasterFileName = "";
	delete theRaster;
	delete theAttr;
	delete theSpatial;
}

// Checks if the features in KB matches feature in the env database
// Return values:
// 0: perfect match
// -1: empty KB
// -2: empty envDB
// -3: new envDB
// -4: feature in KB is not found in envDB
// -5: feature in envDB is not found in KB
int RasterCases::checkFeatureMatch(EnvDatabase* theEnvDatabase){
	if(theAttr==NULL || theAttr->theFeatureList==NULL) return -1;

	//if theEnvDatabase is empty, return "unmatch"
	if(theEnvDatabase == NULL) return -2;

	//if the env db is a brand new one, replace all the features
	if(theEnvDatabase->newEnv)	return -3;

	//compare the features in the first rule with theEnvDatabase
	//to find out if any feature in KB is not found in envDB
	FeatureNode* firstFeature = theAttr->theFeatureList->first;
	while(firstFeature!=NULL){
		int id = -9999;
		EnvLayer* tempLayer = theEnvDatabase->firstLayer;
		while(tempLayer != NULL){
			if(firstFeature->name==tempLayer->layerName)
				id = 0;
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
		FeatureNode* firstFeature = theAttr->theFeatureList->first;
		while(firstFeature!=NULL){
			if(firstFeature->name==tempLayer->layerName){
				id = tempLayer->id;
				break;
			}
			firstFeature = firstFeature->next;
		}
		if(id == -9999) return -5;
		tempLayer = tempLayer->nextLayer;
	}
	return 0;
}

//Update the features in the KB if theEnvDatabase changes
void RasterCases::updateFeatures(EnvDatabase* theEnvDatabase){
	if(theAttr==NULL || theAttr->theFeatureList==NULL) return;

	//if theEnvDatabase is empty, remove all the features in the KB
	if(theEnvDatabase == NULL || theEnvDatabase->numOfLayers==0)
		theAttr->theFeatureList->clean();
	//compare the features in the attr with theEnvDatabase
	//to find out if any old feature(s) need to be removed from the featureList,
	FeatureNode* firstFeature = theAttr->theFeatureList->first;
	while(firstFeature!=NULL){
		int id = -9999;
		EnvLayer* tempLayer = theEnvDatabase->firstLayer;
		while(tempLayer != NULL){
			if(firstFeature->id==tempLayer->id){
				firstFeature->name = tempLayer->layerName; //update name in case it has been changed.
				id = firstFeature->id;
				break;
			}
			tempLayer = tempLayer->nextLayer;
		}
		if(id == -9999){
			FeatureNode* tempFeature = firstFeature;
			firstFeature = firstFeature->next; //move to the next before doing removal
			theAttr->theFeatureList->removeNode(tempFeature);
		}
		else
			firstFeature = firstFeature->next;
	}

	//compare the features in the attr with theEnvDatabase
	//to find out if any new feature(s) need to be added to the featureList,
	EnvLayer* tempLayer = theEnvDatabase->firstLayer;
	while(tempLayer != NULL){
		int id = -9999;
		firstFeature = theAttr->theFeatureList->first;
		while(firstFeature!=NULL){
			if(tempLayer->id==firstFeature->id){
				id = tempLayer->id;
				break;
			}
			firstFeature = firstFeature->next;
		}
		if(id == -9999)
			theAttr->addADefaultFeature(tempLayer, -1, -1);
		tempLayer = tempLayer->nextLayer;
	}
	//Reset data stats for the features, in case the user change the data file, even if the feature name is not changed
	fillFeatureStats(theEnvDatabase);
	needSave = true;

}

void RasterCases::fillFeatureStats(EnvDatabase* theEnvDatabase){
	theAttr->fillFeatureStats(theEnvDatabase);
}

void  RasterCases::checkSave()
{
	if(theAttr->needSave == true)
		needSave = true;
	if(theSpatial->needSave == true)
		needSave =true;
}

void RasterCases::setSaved()
{
	needSave = false;
	theAttr->setSaved();
	theSpatial->setSaved();
}

/***************************
 PointCase Class
 ***************************/
PointCase::PointCase(){
	theAttr = new Attr; 
	theSpatial = new Spatial;
	next = NULL;
	needSave = true;
}

PointCase::PointCase(int anID, CString aName, double x, double y){
	theAttr = new Attr;
	theAttr->id = anID;
	theAttr->name = aName;
	theSpatial = new Spatial;
	theSpatial->x = x;
	theSpatial->y = y;
	next = NULL;
	needSave = true;

}

PointCase::PointCase(const PointCase& aPointCase){
	theAttr = new Attr(*(aPointCase.theAttr));
	theSpatial = new Spatial(*(aPointCase.theSpatial));

	next = NULL;
    needSave = true;
}

PointCase::~PointCase(){
	delete theAttr;
	delete theSpatial;
}

int PointCase::setFeatureDefaults(EnvDatabase *theEnvDatabase){
	int a = theAttr->setFeatureDefaultsXY(theEnvDatabase, theSpatial->x, theSpatial->y);
	if(a!=0)
		return a;
	theSpatial->searchDist = 500;
	theSpatial->useDistSim = false;
	theSpatial->distDecayFactor = 2;
	theSpatial->VtoHConvertingFactor = 1;
	theSpatial->integratingAlgo = 3;
	theSpatial->wgtDecayFactor = 2;
	
	theAttr->effective = true;
	//theAttr->needSave = true;
	needSave = true;
	return 0;
}

void PointCase::setSaved()
{
	needSave = false;
	theAttr->setSaved();
	theSpatial->setSaved();
}
void PointCase::checkSave()
{
	if(theAttr->needSave == true)
		needSave = true;
	if(theSpatial->needSave == true)
		needSave =true;
}

/***************************
 LineCase Class
 ***************************/
LineCase::LineCase(){
	theLine = new List<IDDNode>;
	theAttr = new Attr; 
	theCentralValueMethod = "Value at each location";
	modeTolerance = 0.1; //tolerance in determining mode value
	caseStats = NULL; //list of stats of the "case pixels" for all the env layers.  
										//d1: mean; d2: median; d3: mode; d4: std dev.
	casePixels = NULL; //list of positions of pixels on case

	theSpatial = new Spatial;
	next = NULL;
	needSave = true;
}

LineCase::LineCase(int anID, CString aName, List<IDDNode>* aLine){
	theAttr = new Attr; 
	theAttr->id = anID;
	theAttr->name = aName;
	theCentralValueMethod = "Value at each location";
	modeTolerance = 0.1; //tolerance in determining mode value
	caseStats = NULL; //list of stats of the "case pixels" for all the env layers.  
										//d1: mean; d2: median; d3: mode; d4: std dev.
	casePixels = NULL; //list of positions of pixels on case
	theSpatial = new Spatial;
	theLine = aLine;
	IDDNode* temp = theLine->first;
	int i = 0;
	//set the x, y using the middle node values
	while(temp!=NULL && i<theLine->numOfNodes/2){
		i++;
		temp = temp->next;
	}
	if(temp!=NULL){
		theSpatial->x=temp->d1;
		theSpatial->y=temp->d2;
	}
	next = NULL;
	theAttr->effective = true;
	needSave = true;
}

LineCase::LineCase(const LineCase& aLineCase){
	theLine = new List<IDDNode>(*(aLineCase.theLine));
	theAttr = new Attr(*(aLineCase.theAttr));
	theCentralValueMethod = aLineCase.theCentralValueMethod;
	modeTolerance = aLineCase.modeTolerance; //tolerance in determining mode value
	if(aLineCase.caseStats!=NULL)
		caseStats = new List<IDDDDNode>(*aLineCase.caseStats); 
	else
		caseStats = NULL;
	if(aLineCase.casePixels!=NULL)
		casePixels = new List<IntNode>(*aLineCase.casePixels); //list of positions of pixels on case
	else
		casePixels = NULL;
	theSpatial = new Spatial(*(aLineCase.theSpatial));
	next = NULL;
	needSave = true;
}

LineCase::~LineCase(){
	delete theLine;
	delete theAttr;
	delete caseStats;
	delete casePixels;
	delete theSpatial;
}

int LineCase::setFeatureDefaults(EnvDatabase *theEnvDatabase){
	double x = NODOUBLEVALUE;
	double y = NODOUBLEVALUE;
	if(theLine->first != NULL){
		x = theLine->first->d1;
		y = theLine->first->d2;
	}
	int a = theAttr->setFeatureDefaultsXY(theEnvDatabase, x, y);
	if(a!=0)
		return a;
	theSpatial->searchDist = 500;
	theSpatial->useDistSim = false;
	theSpatial->distDecayFactor = 2;
	theSpatial->VtoHConvertingFactor = 1;
	theSpatial->integratingAlgo = 3;
	theSpatial->wgtDecayFactor = 2;
	
	theAttr->effective = true;

	needSave = true;
	return 0;
}

int LineCase::setCasePixels(EnvDatabase* theEnvDatabase){ //fills the casePixels patch and the caseStatList.
	if(casePixels != NULL) delete casePixels;
	casePixels = new List<IntNode>;

	//RASTERIZE THE LINE
	//(from "rasterizeLine" function)
	double xmin = theEnvDatabase->theSetting->west;
	double ymax = theEnvDatabase->theSetting->north;
	int nrow = theEnvDatabase->theSetting->nrow;
	int ncol = theEnvDatabase->theSetting->ncol;
	float cellsize = theEnvDatabase->theSetting->cellsize;
	EnvLayer* firstEnvLayer = theEnvDatabase->firstLayer;
	int i=0;

	IDDNode* from = theLine->first;
	if(from == NULL) return 1;

	while(from->next!=NULL){
		IDDNode* to = from->next;
		int fromRow = firstEnvLayer->theData->rowOfAPoint(from->d2);
		int fromCol = firstEnvLayer->theData->colOfAPoint(from->d1);
		int toRow = firstEnvLayer->theData->rowOfAPoint(to->d2);
		int toCol = firstEnvLayer->theData->colOfAPoint(to->d1);

		if(fromRow<toRow){
			if(fromCol<toCol){ //from is upper-left
				while(fromRow!=toRow || fromCol!=toCol){
					double crossRX = xmin+cellsize*(fromCol+1);
					double crossRY = (to->d2-from->d2)*(crossRX-from->d1)/(to->d1-from->d1)+from->d2;
					int crossRow = firstEnvLayer->theData->rowOfAPoint(crossRY);
					if(crossRow>toRow) crossRow = toRow;

					double crossCY = ymax-cellsize*(fromRow+1);
					double crossCX = (to->d1-from->d1)*(crossCY-from->d2)/(to->d2-from->d2)+from->d1;
					int crossCol = firstEnvLayer->theData->colOfAPoint(crossCX);
					if(crossCol>toCol) crossCol = toCol;

					if((crossRow-fromRow)==(crossCol-fromCol)){
						casePixels->addTail(new IntNode(fromRow*ncol+fromCol));
						fromRow++;
						fromCol++;
						continue;
					}
					if(crossRow>fromRow){
						i = fromRow; 
						do{
							casePixels->addTail(new IntNode(i*ncol+fromCol));
							i++;
						}while(i<crossRow); 
						if(crossCol<toCol && ymax-cellsize*crossRow-crossRY<cellsize*0.5)
							crossCol++;
					}
					else{
						i = fromCol; 
						do{
							casePixels->addTail(new IntNode(fromRow*ncol+i));
							i++;
						}while(i<crossCol); 
						if(crossRow<toRow && crossCX-xmin-cellsize*crossCol<cellsize*0.5)
							crossRow++;
					}
					fromRow = crossRow;
					fromCol = crossCol;
				}
			}
			else if(fromCol>toCol){	//from is upper-right
				while(fromRow!=toRow || fromCol!=toCol){ 
					double crossRX = xmin+cellsize*fromCol; //check
					double crossRY = (to->d2-from->d2)*(crossRX-from->d1)/(to->d1-from->d1)+from->d2;
					int crossRow = firstEnvLayer->theData->rowOfAPoint(crossRY);
					if(crossRow>toRow) crossRow = toRow; //check

					double crossCY = ymax-cellsize*(fromRow+1); //check
					double crossCX = (to->d1-from->d1)*(crossCY-from->d2)/(to->d2-from->d2)+from->d1;
					int crossCol = firstEnvLayer->theData->colOfAPoint(crossCX);
					if(crossCol<toCol) crossCol = toCol; //check

					if((crossRow-fromRow) == (fromCol-crossCol)){
						casePixels->addTail(new IntNode(fromRow*ncol+fromCol));
						fromRow++;
						fromCol--;
						continue;
					}
					if(crossRow>fromRow){ //check
						i = fromRow; 
						do{
							casePixels->addTail(new IntNode(i*ncol+fromCol));
							i++;
						}while(i<crossRow); //check
						if(crossCol>toCol && ymax-cellsize*crossRow-crossRY<cellsize*0.5) //check
							crossCol--; //check
					}
					else{
						i = fromCol; 
						do{
							casePixels->addTail(new IntNode(fromRow*ncol+i));
							i--;
						}while(i>crossCol); //check
						if(crossRow<toRow && crossCX-xmin-cellsize*crossCol>cellsize*0.5) //check
							crossRow++;
					}
					fromRow = crossRow;
					fromCol = crossCol;
				}
			}
			else{ //from and to are at the same col, but from is upper
				for(i=fromRow; i<toRow; i++)
					casePixels->addTail(new IntNode(i*ncol+fromCol));
			}
		}
		else if(fromRow>toRow){
			if(fromCol<toCol){ //from is lower-left
				while(fromRow!=toRow || fromCol!=toCol){
					double crossRX = xmin+cellsize*(fromCol+1);
					double crossRY = (to->d2-from->d2)*(crossRX-from->d1)/(to->d1-from->d1)+from->d2;
					int crossRow = firstEnvLayer->theData->rowOfAPoint(crossRY);
					if(crossRow<toRow) crossRow = toRow;

					double crossCY = ymax-cellsize*(fromRow);
					double crossCX = (to->d1-from->d1)*(crossCY-from->d2)/(to->d2-from->d2)+from->d1;
					int crossCol = firstEnvLayer->theData->colOfAPoint(crossCX);
					if(crossCol>toCol) crossCol = toCol;

					if((fromRow-crossRow)==(fromCol-crossCol)){
						casePixels->addTail(new IntNode(fromRow*ncol+fromCol));
						fromRow--;
						fromCol++;
						continue;
					}
					if(crossRow<fromRow){
						i = fromRow; 
						do{
							casePixels->addTail(new IntNode(i*ncol+fromCol));
							i--;
						}while(i>crossRow); 
						if(crossCol<toCol && ymax-cellsize*crossRow-crossRY>cellsize*0.5)
							crossCol++;
					}
					else{
						i = fromCol; 
						do{
							casePixels->addTail(new IntNode(fromRow*ncol+i));
							i++;
						}while(i<crossCol); 
						if(crossRow>toRow && crossCX-xmin-cellsize*crossCol<cellsize*0.5)
							crossRow--;
					}
					fromRow = crossRow;
					fromCol = crossCol;
				}
			}
			else if(fromCol>toCol){	//from is lower-right
				while(fromRow!=toRow || fromCol!=toCol){ 
					double crossRX = xmin+cellsize*fromCol; //check
					double crossRY = (to->d2-from->d2)*(crossRX-from->d1)/(to->d1-from->d1)+from->d2;
					int crossRow = firstEnvLayer->theData->rowOfAPoint(crossRY);
					if(crossRow<toRow) crossRow = toRow; //check

					double crossCY = ymax-cellsize*fromRow; //check
					double crossCX = (to->d1-from->d1)*(crossCY-from->d2)/(to->d2-from->d2)+from->d1;
					int crossCol = firstEnvLayer->theData->colOfAPoint(crossCX);
					if(crossCol<toCol) crossCol = toCol; //check

					if((fromRow-crossRow)==(crossCol-fromCol)){
						casePixels->addTail(new IntNode(fromRow*ncol+fromCol));
						fromRow--;
						fromCol--;
						continue;
					}
					if(crossRow<fromRow){ //check
						i = fromRow; 
						do{
							casePixels->addTail(new IntNode(i*ncol+fromCol));
							i--; //check
						}while(i>crossRow); //check
						if(crossCol>toCol && ymax-cellsize*crossRow-crossRY>cellsize*0.5) //check
							crossCol--; //check
					}
					else{
						i = fromCol; 
						do{
							casePixels->addTail(new IntNode(fromRow*ncol+i));
							i--;
						}while(i>crossCol); //check
						if(crossRow>toRow && crossCX-xmin-cellsize*crossCol>cellsize*0.5) //check
							crossRow--;
					}
					fromRow = crossRow;
					fromCol = crossCol;
				}
			}
			else { //from and to are at the same col, but from is lower 
				for(i=fromRow; i>toRow; i--)
					casePixels->addTail(new IntNode(i*ncol+fromCol));
			}
		}
		else{
			if(fromCol<toCol){ //from and to are in the same row, but from is left
				for(i=fromCol; i<toCol; i++)
					casePixels->addTail(new IntNode(fromRow*ncol+i));
			}
			else if(fromCol>toCol){	//from and to are in the same row, but from is right
				for(i=fromCol; i>toCol; i--)
					casePixels->addTail(new IntNode(fromRow*ncol+i));
			}
			else { //from and to are in the same cell
				casePixels->addTail(new IntNode(fromRow*ncol+fromCol));
			}
		}
		from = to;
	}
	casePixels->addTail(new IntNode(firstEnvLayer->theData->rowOfAPoint(from->d2)*ncol+firstEnvLayer->theData->colOfAPoint(from->d1)));

	//CALC STATS
	if(caseStats != NULL) delete caseStats;
	caseStats = new List<IDDDDNode>;
	List<DNode>* sortList = new List<DNode>;
	EnvLayer* tempEnvLayer = theEnvDatabase->firstLayer;
	while(tempEnvLayer!=NULL){
		IntNode* tempPixel = casePixels->first;
		while(tempPixel != NULL){
			DNode* tempSortNode = sortList->first; 
			float v = tempEnvLayer->theData->getDataBuf()[tempPixel->pos];
			//ascending sort
			if(tempSortNode==NULL || tempSortNode->d<=v)
				sortList->addHead(new DNode(v));
			else{
				while(tempSortNode!=NULL){
					if(tempSortNode->d>v){
						sortList->insertBefore(tempSortNode, new DNode(v));
						break;
					}
					tempSortNode=tempSortNode->next;
				}
				if(tempSortNode==NULL) //all existing node are smaller
					sortList->addTail(new DNode(v));
			}
			tempPixel = tempPixel->next;
		}
		double tot = 0;
		double mean = 0;
		double median = 0;
		double std = 0;
		double mode = 0;
		int mid = sortList->numOfNodes/2;
		int n = 0;
		DNode* tempSortNode = sortList->first;
		while(tempSortNode!=NULL){
			tot += tempSortNode->d;
			if(n++==mid)
				median = tempSortNode->d;
			tempSortNode = tempSortNode->next;
		}
		mean = tot/sortList->numOfNodes;
		tot = 0;
		tempSortNode = sortList->first;
		while(tempSortNode!=NULL){
			tot += (tempSortNode->d-mean)*(tempSortNode->d-mean);
			tempSortNode = tempSortNode->next;
		}
		std = sqrt(tot/sortList->numOfNodes);
		double tolerance = std*modeTolerance;
		int modeFreq = 0;
		tempSortNode = sortList->first;
		while(tempSortNode!=NULL){
			int freq = 0;
			DNode* tempNode2 = sortList->first;
			while(tempNode2!=NULL){
				if(tempNode2->d>=tempSortNode->d-tolerance || tempNode2->d<=tempSortNode->d+tolerance)
					freq++;
				if(tempNode2->d>tempSortNode->d+tolerance)
					break;
				tempNode2 = tempNode2->next;
			}
			if(freq>modeFreq){
				modeFreq = freq;
				mode = tempSortNode->d;
			}
			tempSortNode = tempSortNode->next;
		}
		caseStats->addTail(new IDDDDNode(tempEnvLayer->id, mean, median, std, mode));
		sortList->clean();
		tempEnvLayer = tempEnvLayer->nextLayer;
	}
	delete sortList;

	needSave =true;
	return 0;
}

void LineCase::setCentralValue(){ //sets the central value according to the specified method
	FeatureNode* tempFeature = theAttr->theFeatureList->first;
	while(tempFeature != NULL){
		IDDDDNode* tempStat = caseStats->first;
		while(tempStat!=NULL){
			if(tempFeature->id == tempStat->i)
				break;
			tempStat = tempStat->next;
		}
		if(theCentralValueMethod=="Median"){
			if(tempFeature->value1<999999) tempFeature->value1 = tempStat->d2; //preserves the curve
			if(tempFeature->value2<999999) tempFeature->value2 = tempStat->d2; //preserves the curve
		}
		else if(theCentralValueMethod=="Mode"){
			if(tempFeature->value1<999999) tempFeature->value1 = tempStat->d4; //preserves the curve
			if(tempFeature->value2<999999) tempFeature->value2 = tempStat->d4; //preserves the curve
		}
		else{ //"Value at each location" or "Mean", use mean
			if(tempFeature->value1<999999) tempFeature->value1 = tempStat->d1; //preserves the curve
			if(tempFeature->value2<999999) tempFeature->value2 = tempStat->d1; //preserves the curve
		}
		tempFeature = tempFeature->next;
	}

	needSave = true;
}

void LineCase::setSaved()
{
	needSave = false;
	theAttr->setSaved();
	theSpatial->setSaved();
}
void LineCase::checkSave()
{
	if(theAttr->needSave == true)
		needSave = true;
	if(theSpatial->needSave == true)
		needSave =true;
}

/*****************************************
 * Polygon Case
 *****************************************/
PolyCase::PolyCase(){
	thePoly = new xPolygon(0);  // Force xPolygon constructor to allocate vertexList
	theAttr = new Attr; 
	theCentralValueMethod = "Value at each location";
	modeTolerance = 0.1; //tolerance in determining mode value
	caseStats = NULL; //list of stats of the "case pixels" for all the env layers.  
										//d1: mean; d2: median; d3: mode; d4: std dev.
	casePixels = NULL; //list of positions of pixels on case
	theSpatial = new Spatial;
	next = NULL;
	needSave =true;
}

PolyCase::PolyCase(int anID, CString aName, xPolygon* aPoly){
	theAttr = new Attr; 
	theAttr->id = anID;
	theAttr->name = aName;
	theCentralValueMethod = "Value at each location";
	modeTolerance = 0.1; //tolerance in determining mode value
	caseStats = NULL; //list of stats of the "case pixels" for all the env layers.  
										//d1: mean; d2: median; d3: mode; d4: std dev.
	casePixels = NULL; //list of positions of pixels on case
	theSpatial = new Spatial;
	thePoly = aPoly;

	// Set Spatial to be centroid of Polygon parent
	IDDNode* temp = centroidSimplePoly( aPoly );
	theSpatial->x = temp->d1;
	theSpatial->y = temp->d2;
	delete temp;
	next = NULL;
	needSave = true;
}

PolyCase::PolyCase(const PolyCase& aPolyCase){
	thePoly = new xPolygon(*(aPolyCase.thePoly));
	theAttr = new Attr(*(aPolyCase.theAttr));
	theCentralValueMethod = aPolyCase.theCentralValueMethod;
	modeTolerance = aPolyCase.modeTolerance; //tolerance in determining mode value
	if(aPolyCase.caseStats!=NULL)
		caseStats = new List<IDDDDNode>(*aPolyCase.caseStats); 
	else
		caseStats = NULL;
	if(aPolyCase.casePixels!=NULL)
		casePixels = new List<IntNode>(*aPolyCase.casePixels); //list of positions of pixels on case
	else
		casePixels = NULL;
	theSpatial = new Spatial(*(aPolyCase.theSpatial));
    needSave = true;
	next = NULL;
}

PolyCase::~PolyCase(){
	delete thePoly;
	delete theAttr;
	delete caseStats;
	delete casePixels;
	delete theSpatial;
}

int PolyCase::setFeatureDefaults(EnvDatabase* theEnvDatabase){
	double x = NODOUBLEVALUE;
	double y = NODOUBLEVALUE;
	if(thePoly->vertexList->first != NULL){
		x = thePoly->vertexList->first->d1;
		y = thePoly->vertexList->first->d2;
	}
	int a = theAttr->setFeatureDefaultsXY(theEnvDatabase, x, y);
	if(a!=0)
		return a;
	theSpatial->searchDist = 500;
	theSpatial->useDistSim = false;
	theSpatial->distDecayFactor = 2;
	theSpatial->VtoHConvertingFactor = 1;
	theSpatial->integratingAlgo = 3;
	theSpatial->wgtDecayFactor = 2;
	theAttr->effective = true;
    needSave = true;
	return 0;
}

int PolyCase::setCasePixels(EnvDatabase* theEnvDatabase){ //fills the casePixels patch and the caseStatList.
	if(casePixels != NULL) delete casePixels;
	casePixels = new List<IntNode>;

	//RASTERIZE THE POLYGON
	//(from "rasterizeLine" function)
	double xmin = theEnvDatabase->theSetting->west;
	double ymax = theEnvDatabase->theSetting->north;
	int nrow = theEnvDatabase->theSetting->nrow;
	int ncol = theEnvDatabase->theSetting->ncol;
	float cellsize = theEnvDatabase->theSetting->cellsize;

	CThreeDR* raster = theEnvDatabase->firstLayer->theData;

	thePoly->getBound(); 
	int sr = raster->rowOfAPoint(thePoly->Ymax);
	int sc = raster->colOfAPoint(thePoly->Xmin);
	int er = raster->rowOfAPoint(thePoly->Ymin);
	int ec = raster->colOfAPoint(thePoly->Xmax);

	//test if pixels are inside the polygon
	double pixelY = raster->yOfACell(sr);
	double pixelX = raster->xOfACell(sc);
	for(int i=sr; i<er+1; i++){
		for(int j=sc; j<ec+1; j++){
			IDDNode* thePoint = new IDDNode(0, pixelX, pixelY);
			if(pointInPolygon(thePoly, thePoly->children, thePoint)>0)//if inside the polygon
				casePixels->addTail(new IntNode(i*ncol+j));
			delete thePoint;
			pixelX += cellsize;
		}
		pixelY -= cellsize;
	}

	//CALC STATS
	if(caseStats != NULL) delete caseStats;
	caseStats = new List<IDDDDNode>;
	List<DNode>* sortList = new List<DNode>;
	EnvLayer* tempEnvLayer = theEnvDatabase->firstLayer;
	while(tempEnvLayer!=NULL){
		IntNode* tempPixel = casePixels->first;
		while(tempPixel != NULL){
			DNode* tempSortNode = sortList->first; 
			float v = tempEnvLayer->theData->getDataBuf()[tempPixel->pos];
			//ascending sort
			if(tempSortNode==NULL || tempSortNode->d<=v)
				sortList->addHead(new DNode(v));
			else{
				while(tempSortNode!=NULL){
					if(tempSortNode->d>v){
						sortList->insertBefore(tempSortNode, new DNode(v));
						break;
					}
					tempSortNode=tempSortNode->next;
				}
				if(tempSortNode==NULL) //all existing node are smaller
					sortList->addTail(new DNode(v));
			}
			tempPixel = tempPixel->next;
		}
		double tot = 0;
		double mean = 0;
		double median = 0;
		double std = 0;
		double mode = 0;
		int mid = sortList->numOfNodes/2;
		int n = 0;
		DNode* tempSortNode = sortList->first;
		while(tempSortNode!=NULL){
			tot += tempSortNode->d;
			if(n++==mid)
				median = tempSortNode->d;
			tempSortNode = tempSortNode->next;
		}
		mean = tot/sortList->numOfNodes;
		tot = 0;
		tempSortNode = sortList->first;
		while(tempSortNode!=NULL){
			tot += (tempSortNode->d-mean)*(tempSortNode->d-mean);
			tempSortNode = tempSortNode->next;
		}
		std = sqrt(tot/sortList->numOfNodes);
		double tolerance = std*modeTolerance;
		int modeFreq = 0;
		tempSortNode = sortList->first;
		while(tempSortNode!=NULL){
			int freq = 0;
			DNode* tempNode2 = sortList->first;
			while(tempNode2!=NULL){
				if(tempNode2->d>=tempSortNode->d-tolerance || tempNode2->d<=tempSortNode->d+tolerance)
					freq++;
				if(tempNode2->d>tempSortNode->d+tolerance)
					break;
				tempNode2 = tempNode2->next;
			}
			if(freq>modeFreq){
				modeFreq = freq;
				mode = tempSortNode->d;
			}
			tempSortNode = tempSortNode->next;
		}
		caseStats->addTail(new IDDDDNode(tempEnvLayer->id, mean, median, std, mode));
		sortList->clean();
		tempEnvLayer = tempEnvLayer->nextLayer;
	}
	delete sortList;

	needSave = true;
	return 0;
}

void PolyCase::setCentralValue(){ //sets the central value according to the specified method
	FeatureNode* tempFeature = theAttr->theFeatureList->first;
	while(tempFeature != NULL){
		IDDDDNode* tempStat = caseStats->first;
		while(tempStat!=NULL){
			if(tempFeature->id == tempStat->i)
				break;
			tempStat = tempStat->next;
		}
		if(theCentralValueMethod=="Median"){
			if(tempFeature->value1<999999) tempFeature->value1 = tempStat->d2; //preserves the curve
			if(tempFeature->value2<999999) tempFeature->value2 = tempStat->d2; //preserves the curve
		}
		else if(theCentralValueMethod=="Mode"){
			if(tempFeature->value1<999999) tempFeature->value1 = tempStat->d4; //preserves the curve
			if(tempFeature->value2<999999) tempFeature->value2 = tempStat->d4; //preserves the curve
		}
		else{ //"Value at each location" or "Mean", use mean
			if(tempFeature->value1<999999) tempFeature->value1 = tempStat->d1; //preserves the curve
			if(tempFeature->value2<999999) tempFeature->value2 = tempStat->d1; //preserves the curve
		}
		tempFeature = tempFeature->next;
	}
	needSave = true;
}


void PolyCase::setSaved()
{
	needSave = false;
	theAttr->setSaved();
	theSpatial->setSaved();
}
void PolyCase::checkSave()
{
	if(theAttr->needSave == true)
		needSave = true;
	if(theSpatial->needSave == true)
		needSave = true;
}

/*****************************************
 * Inference functions
 *****************************************/
float nominalFeatureSimilarity(float zprime, float z1, float z2){
	if(z1-zprime>VERYSMALL || zprime-z2>VERYSMALL)
		return 0.0;
	else
		return 1.0;
}

float ordinalFeatureSimilarity(float zprime, float zmin, float zmax,
							   float z1, float w1, 
							   float z2, float w2){
	if(fabs(w1)<VERYSMALL) w1 = VERYSMALL;
	if(fabs(w2)<VERYSMALL) w2 = VERYSMALL;

	if(zprime<z1)
		return 1.0-((int)((z1-zprime)*(w1-1)/(z1-zmin)+0.5))/w1;
	else if(zprime>z2)
		return 1.0-((int)((zprime-z2)*(w2-1)/(zmax-z2)+0.5))/w2;
	else
		return 1;
}

float continuousFeatureSimilarity(float zprime, 
								  float z1, float w1, float r1, float c1, 
								  float z2, float w2, float r2, float c2){
	if(c1>1 || c1<=0 || c2>1 || c2<=0) return 0;

	if(fabs(w1)<VERYSMALL) w1 = VERYSMALL;
	if(fabs(w2)<VERYSMALL) w2 = VERYSMALL;
	if(fabs(c1)<VERYSMALL) c1 = VERYSMALL;
	if(fabs(c2)<VERYSMALL) c2 = VERYSMALL;
	
	if(zprime<z1)
		return exp(pow(fabs(zprime-z1)/w1, r1)*log(c1));
	else if(zprime>z2)
		return exp(pow(fabs(zprime-z2)/w2, r2)*log(c2));
	else 
		return 1;
}

float cyclicFeatureSimilarity(float zprime, 
								   float z1, float w1, float r1, float c1,
								   float z2, float w2, float r2, float c2){
	if(c1>1 || c1<=0 || c2>1 || c2<=0) return 0;
	if(zprime<0 || zprime>360) return 0;
	if(fabs(w1)<VERYSMALL) w1 = VERYSMALL;
	if(fabs(w2)<VERYSMALL) w2 = VERYSMALL;
	if(fabs(c1)<VERYSMALL) c1 = VERYSMALL;
	if(fabs(c2)<VERYSMALL) c2 = VERYSMALL;

	//the range should always be set in clockwise direction
	//so w1 should be used in the negative direction
	w1 = fabs(w1);

	//sort the four limits
	//label code: 1: lowerLimitFor0
	//			  2: lowerLimitFor1
	//			  3: upperLimitFor1
	//			  4: upperLimitFor0
	struct Limits{
		int label;
		float degree;
	};
	Limits theLimits[4];
	theLimits[0].label = 1;
	theLimits[0].degree = z1-w1;
	theLimits[1].label = 2;
	theLimits[1].degree = z1;
	theLimits[2].label = 3;
	theLimits[2].degree = z2;
	theLimits[3].label = 4;
	theLimits[3].degree = z2+w2;

	Limits tempLimit;

	//bubble algo. for sorting
	//After sorting, the degrees in [0], ...,[3] will be in an ascending order.
	//Their labels indicate their roles in inference.
	for(int i=0; i<3; i++){ 
		for(int j = i+1; j<4; j++){
			if(theLimits[i].degree > theLimits[j].degree){
				tempLimit.label = theLimits[i].label;
				tempLimit.degree = theLimits[i].degree;
				theLimits[i].label = theLimits[j].label;
				theLimits[i].degree = theLimits[j].degree;
				theLimits[j].label = tempLimit.label;
				theLimits[j].degree = tempLimit.degree;
			}
		}
	}

	//check if the limits are reasonable
	//If the sorting is right, then the possible order would be (in clockwise)
	//0, 1, 2, 3
	//1, 2, 3, 0
	//2, 3, 0, 1
	//3, 0, 1, 2
	//Therefore, label[i+1] - label[i] is either 1 or 3
	for(i=0; i<3; i++)
		if(theLimits[i+1].label-theLimits[i].label != 1
			&& theLimits[i].label-theLimits[i+1].label != 3
			&& w1 != w2)
			return -1;

	//If zprime is betwee 0 and [0] and [0] is the upper limit of 1
	//zprime must be in the full value range
	if(zprime<=theLimits[0].degree && theLimits[0].label == 3)
		return 1;
	
	//If zprime is betwee [3] and 360 and [3] is the lower limit of 1
	//zprime must be in the full value range
	if(zprime>=theLimits[3].degree && theLimits[3].label == 2)
		return 1;

	//If zprime is between label 2 and 3, it has a full value
	for(i = 0; i<3; i++)
		if(theLimits[i].label==2 && zprime>=theLimits[i].degree && zprime<=theLimits[i+1].degree)
			return 1;

	//If zprime falls into the "fuzzy" range, return the bigger value of the two.
	return calMax(exp(pow(fabs(z1-zprime)/w1, r1)*log(c1)), exp(pow(fabs(z2-zprime)/w2, r2)*log(c2)));
}

float limitingFactor(ValueNode** featureValueArray, Attr* theCaseAttr, int& env, float nodata){
	float sim = 1;
	bool flag = false; //for checking if all features are not used
	FeatureNode* AttrFeature = theCaseAttr->theFeatureList->first;
	while(AttrFeature!=NULL){
		if(AttrFeature->useOrNot){
			flag = true;
			for(int k=0; k<theCaseAttr->theFeatureList->numOfNodes; k++){
				if(featureValueArray[k]->num == AttrFeature->id){
					if(fabs(featureValueArray[k]->value-nodata)<VERYSMALL
						|| fabs(AttrFeature->value1-nodata)<VERYSMALL
						|| fabs(AttrFeature->value2-nodata)<VERYSMALL)
						return 0;
					float featureSim = 0;

					if(AttrFeature->functionType == "Nominal")
						featureSim = nominalFeatureSimilarity(featureValueArray[k]->value, AttrFeature->value1, AttrFeature->value2);
					else if(AttrFeature->functionType == "Ordinal")
						featureSim = ordinalFeatureSimilarity(featureValueArray[k]->value, AttrFeature->min, AttrFeature->max,
											AttrFeature->value1, AttrFeature->width1,
											AttrFeature->value2, AttrFeature->width2);
					else if(AttrFeature->functionType == "Cyclic")
						featureSim = cyclicFeatureSimilarity(featureValueArray[k]->value, 
											AttrFeature->value1, AttrFeature->width1, AttrFeature->r1, AttrFeature->k1, 
											AttrFeature->value2, AttrFeature->width2, AttrFeature->r2, AttrFeature->k2);
					else if(AttrFeature->functionType.Find("Continuous", 0)>-1 /*|| AttrFeature->functionType == "Default"*/)
						featureSim = continuousFeatureSimilarity(featureValueArray[k]->value, 
											AttrFeature->value1, AttrFeature->width1, AttrFeature->r1, AttrFeature->k1, 
											AttrFeature->value2, AttrFeature->width2, AttrFeature->r2, AttrFeature->k2);
					else if(AttrFeature->functionType == "Raw")
						featureSim = featureValueArray[k]->value;
					if(featureSim<sim){
						sim = featureSim;
						env = AttrFeature->id;
					}
					if(sim<VERYSMALL)
						return 0;
					break;
				}
			}
		}
		AttrFeature = AttrFeature->next;
	}
	if(!flag) sim = 0; //if no feature is used, set membership to be 0.
	return sim;
}

float weightedAverage(ValueNode** featureValueArray, Attr* theCaseAttr, int& env, float nodata){
	float sim = 0;
	float dominant = 0;
	float totWeight = 0;
	FeatureNode* AttrFeature = theCaseAttr->theFeatureList->first;
	while(AttrFeature!=NULL){
		for(int k=0; k<theCaseAttr->theFeatureList->numOfNodes; k++){
			if(featureValueArray[k]->num == AttrFeature->id){
				if(fabs(featureValueArray[k]->value-nodata)<VERYSMALL 
					|| fabs(AttrFeature->value1-nodata)<VERYSMALL
					|| fabs(AttrFeature->value2-nodata)<VERYSMALL)
					return 0;
				float featureSim = 0;
				if(AttrFeature->functionType == "Nominal")
					featureSim = nominalFeatureSimilarity(featureValueArray[k]->value, AttrFeature->value1, AttrFeature->value2);
				else if(AttrFeature->functionType == "Ordinal")
					featureSim = ordinalFeatureSimilarity(featureValueArray[k]->value, AttrFeature->min, AttrFeature->max,
										AttrFeature->value1, AttrFeature->width1,
										AttrFeature->value2, AttrFeature->width2);
				else if(AttrFeature->functionType == "Cyclic")
					featureSim = cyclicFeatureSimilarity(featureValueArray[k]->value, 
										AttrFeature->value1, AttrFeature->width1, AttrFeature->r1, AttrFeature->k1, 
										AttrFeature->value2, AttrFeature->width2, AttrFeature->r2, AttrFeature->k2);
				else if(AttrFeature->functionType.Find("Continuous", 0)>-1 /*|| AttrFeature->functionType == "Default"*/)
					featureSim = continuousFeatureSimilarity(featureValueArray[k]->value, 
										AttrFeature->value1, AttrFeature->width1, AttrFeature->r1, AttrFeature->k1, 
										AttrFeature->value2, AttrFeature->width2, AttrFeature->r2, AttrFeature->k2);
				if(AttrFeature->functionType == "Raw")
					featureSim = featureValueArray[k]->value;
				float v = featureSim*AttrFeature->weight;
				sim += v;
				totWeight += AttrFeature->weight;
				if(v>dominant){
					dominant = v;
					env = AttrFeature->id;
				}
				break;
			}
		}
		AttrFeature = AttrFeature->next;
	}
	return sim/totWeight;
}

float multiplication(ValueNode** featureValueArray, Attr* theCaseAttr, int& env, float nodata){
	float sim = 1;
	bool flag = false; //for checking if all features are not used
	float dominant = 0;
	FeatureNode* AttrFeature = theCaseAttr->theFeatureList->first;
	while(AttrFeature!=NULL){
		if(AttrFeature->useOrNot){
			flag = true;
			for(int k=0; k<theCaseAttr->theFeatureList->numOfNodes; k++){
				if(featureValueArray[k]->num == AttrFeature->id){
					if(fabs(featureValueArray[k]->value-nodata)<VERYSMALL
						|| fabs(AttrFeature->value1-nodata)<VERYSMALL
						|| fabs(AttrFeature->value2-nodata)<VERYSMALL)
						return 0;
					float featureSim = 0;

					if(AttrFeature->functionType == "Nominal")
						featureSim = nominalFeatureSimilarity(featureValueArray[k]->value, AttrFeature->value1, AttrFeature->value2);
					else if(AttrFeature->functionType == "Ordinal")
						featureSim = ordinalFeatureSimilarity(featureValueArray[k]->value, AttrFeature->min, AttrFeature->max,
											AttrFeature->value1, AttrFeature->width1,
											AttrFeature->value2, AttrFeature->width2);
					else if(AttrFeature->functionType == "Cyclic")
						featureSim = cyclicFeatureSimilarity(featureValueArray[k]->value, 
											AttrFeature->value1, AttrFeature->width1, AttrFeature->r1, AttrFeature->k1, 
											AttrFeature->value2, AttrFeature->width2, AttrFeature->r2, AttrFeature->k2);
					else if(AttrFeature->functionType.Find("Continuous", 0)>-1 /*|| AttrFeature->functionType == "Default"*/)
						featureSim = continuousFeatureSimilarity(featureValueArray[k]->value, 
											AttrFeature->value1, AttrFeature->width1, AttrFeature->r1, AttrFeature->k1, 
											AttrFeature->value2, AttrFeature->width2, AttrFeature->r2, AttrFeature->k2);
					if(AttrFeature->functionType == "Raw")
						featureSim = featureValueArray[k]->value;
					sim *= featureSim;
					if(featureSim>dominant){
						dominant = featureSim;
						env = AttrFeature->id;
					}
					if(sim<VERYSMALL)
						return 0;
					break;
				}
			}
		}
		AttrFeature = AttrFeature->next;
	}
	if(!flag) sim = 0; //if no feature is used, set membership to be 0.
	return sim;
}

float visibility(double x1, double y1, float h1, 
				 double x2, double y2, float h2, 
				 double xx, double yx, float hx){

	if(fabs(x1-x2)<VERYSMALL && fabs(y1-y2)<VERYSMALL) return 0;

	//ho must be lower than ht
	float ho = 0;
	float ht = 0;
	double xo = 0, yo = 0;
	double xt = 0, yt = 0;
	if(h1<h2){
		ho = h1;
		xo = x1;
		yo = y1;
		ht = h2;
		xt = x2;
		yt = y2;
	}
	else{
		ho = h2;
		xo = x2;
		xo = y2;
		ht = h1;
		xt = x1;
		yt = y1;
	}
	double dox = sqrt(pow((xo-xx),2)+pow((yo-yx),2));
	double dot = sqrt(pow((xo-xt),2)+pow((yo-yt),2));;
	return hx - (ht-ho)*dox/dot - ho; //return value > 0, cannot see
}

//Compare with topoDistancXY, this one has problem. Jan. 22, 2005
float topoDistanceRC(int r1, int c1, int r2, int c2, 
				   CThreeDR* theDEM, float VtoCFactor, 
				   int numOfIntervals, float threshold, 
				   float upwardFactor, float downwardFactor){
	double x1 = theDEM->xOfACell(c1);
	double y1 = theDEM->yOfACell(r1);
	double x2 = theDEM->xOfACell(c2);
	double y2 = theDEM->yOfACell(r2);
	if(sqrt(pow((x1-x2),2)+pow((y1-y2),2))>threshold)
		return -1;

	float h1 = theDEM->getDataBuf()[r1*theDEM->getNumberOfColumns()+c1];
	float h2 = theDEM->getDataBuf()[r2*theDEM->getNumberOfColumns()+c2];

	double interval = theDEM->getCellSize()/(float)numOfIntervals;
	int steps = sqrt(pow((x1-x2),2)+pow((y1-y2),2))/interval;
	double k = (y2-y1)/(x2-x1);

	double xStart = x1;
	double yStart = y1;
	double xx = 0;
	double yx = 0;
	double hx = 0;
	double hxprime = 0;

	double dist = 0;
	for(int i=0; i<steps; i++){
		xx = xStart + interval/sqrt(pow(k,2)+1);
		yx = yStart + k*interval/sqrt(pow(k,2)+1);
		int rx = theDEM->rowOfAPoint(yx);
		int cx = theDEM->colOfAPoint(xx);
		hx = theDEM->getDataBuf()[rx*theDEM->getNumberOfColumns()+cx];;
		hxprime = visibility(x1, y1, h1, x2, y2, h2, xx, yx, hx);
		if(hxprime<0) hxprime *= downwardFactor;
		else hxprime *= upwardFactor;
		hxprime = fabs(hxprime);

		dist += sqrt(pow((xStart-xx),2)+pow((yStart-yx),2)+pow(hxprime*VtoCFactor,2));
		if(dist > threshold)
			return -1;

		xStart = xx;
		yStart = yx;
	}
	dist += sqrt(pow((xStart-x2),2)+pow((yStart-y2),2)+pow(hxprime*VtoCFactor,2));
	if(dist>threshold) return -1;
	else return dist;
}

//Calculates distance between two points (x1, y1) and (x2, y2) over a DEM surface (given by theDEM).
//Basic idea: Starting from (x1,y1) and along the direction from (x1, y1) to (x2, y2), 
//calculates the length in 3D space of each small interval, and then sum them up to get the overall distance.
//Interval: The interval in 2D space is defined by cellsize*cellFraction, where cellFraction is a user-specifed 
//constant.  For example, the user can specify to use 1/10 of the cellsize as the interval.
//Length of interval in 3D space: The length of an interval in 3D space is calcualted in an Euclidean way 
//using (x1', y1', z1') to (x2', y2', z2'), where the coordinates are for the two ends of an
//interval and z is elevation.
//Elevation: Elevation (z) will be read from the cell of the DEM in which the end point of an interval 
//falls into. No interpolation is used.
float topoDistanceXY(double x1, double y1, double x2, double y2, CThreeDR* theDEM, 
					 float VtoCFactor, float cellFraction, float threshold, 
					 float upwardFactor, float downwardFactor){
	if(sqrt(pow((x1-x2),2)+pow((y1-y2),2))>threshold)
		return -1;

	float nodata = theDEM->getNullDataValue();
	int r1 = theDEM->rowOfAPoint(y1);
	int c1 = theDEM->colOfAPoint(x1);
	int r2 = theDEM->rowOfAPoint(y2);
	int c2 = theDEM->colOfAPoint(x2);
	float h1 = theDEM->getDataBuf()[r1*theDEM->getNumberOfColumns()+c1];
	float h2 = theDEM->getDataBuf()[r2*theDEM->getNumberOfColumns()+c2];
	if(fabs(h1-nodata)<VERYSMALL || fabs(h2-nodata)<VERYSMALL)
		return -2;

	double interval = theDEM->getCellSize()*cellFraction;
	int steps = sqrt(pow((x1-x2),2)+pow((y1-y2),2))/interval;
	double k = 1;
	if(fabs(x2-x1)>VERYSMALL)
		k = fabs((y2-y1)/(x2-x1));

	double xStart = x1;
	double yStart = y1;
	double xx = 0;
	double yx = 0;
	int rStart = 0;
	int cStart = 0;
	float hStart = 0;
	int rx = 0;
	int cx = 0;
	double hx = 0;
	double hxprime = 0;

	double dist = 0;
	for(int i=0; i<steps; i++){
		if(fabs(x2-x1)>VERYSMALL){
			if(x1<x2)
				xx = xStart + interval/sqrt(pow(k,2)+1);
			else
				xx = xStart - interval/sqrt(pow(k,2)+1);
			if(y1<y2)
				yx = yStart + k*interval/sqrt(pow(k,2)+1);
			else
				yx = yStart - k*interval/sqrt(pow(k,2)+1);
		}
		else{
			xx = xStart;
			if(y1<y2)
				yx = yStart + interval;
			else
				yx = yStart - interval;
		}

		rStart = theDEM->rowOfAPoint(yStart);
		cStart = theDEM->colOfAPoint(xStart);
		hStart = theDEM->getDataBuf()[rStart*theDEM->getNumberOfColumns()+cStart];
		rx = theDEM->rowOfAPoint(yx);
		cx = theDEM->colOfAPoint(xx);
		hx = theDEM->getDataBuf()[rx*theDEM->getNumberOfColumns()+cx];
/* use visibility algo to discriminate upward and downward topo, temporarily turned off
		hxprime = visibility(x1, y1, h1, x2, y2, h2, xx, yx, hx);
		if(hxprime<0) hxprime *= downwardFactor;
		else hxprime *= upwardFactor;
		hxprime = fabs(hxprime);

		dist += sqrt(pow((xStart-xx),2)+pow((yStart-yx),2)+pow(hxprime*VtoCFactor,2));
*/
		if(fabs(hStart-nodata)<VERYSMALL || fabs(hx-nodata)<VERYSMALL) return -3;
		dist += sqrt(pow((xStart-xx),2)+pow((yStart-yx),2)+pow((hStart-hx)*VtoCFactor,2));
		if(dist > threshold)
			return -1;

		xStart = xx;
		yStart = yx;
	}
	rStart = theDEM->rowOfAPoint(yStart);
	cStart = theDEM->colOfAPoint(xStart);
	hStart = theDEM->getDataBuf()[rStart*theDEM->getNumberOfColumns()+cStart];
	//return dist += sqrt(pow((xStart-x2),2)+pow((yStart-y2),2)+pow(hxprime*VtoCFactor,2));
	if(fabs(hStart-nodata)<VERYSMALL) return -3;
	dist += sqrt(pow((xStart-x2),2)+pow((yStart-y2),2)+pow((hStart-h2)*VtoCFactor,2));
	if(dist > threshold)
		return -1;
	else
		return dist;
}

float distancedSimilarity(float featureSpaceSim, float dist, float searchDist, float decayFactor){
	//	Suppose sim at rim of the searching range is VERYSMALL
	return featureSpaceSim*exp(pow(dist/searchDist, decayFactor)*log(VERYSMALL));
}

float integratingCases(List<IIIDDDNode>* theSimilarityList){
	if(theSimilarityList == NULL) return 0;
	int integratingAlgo = 100;
	double wgtDecayFactor = 0;
	double result = 0;
	double wgt = 999999;

	IIIDDDNode* tempNode = theSimilarityList->first;
	while(tempNode != NULL){ //Find the case whose algo has the highest priority and use its algo as the algo and its wgtDecayFactor as the wgtDecayFactor
		if(integratingAlgo>tempNode->i3){
			integratingAlgo = tempNode->i3;
			wgtDecayFactor = tempNode->d3;
		}
		tempNode = tempNode->next;
	}
	
	if(integratingAlgo == 1){ //dominant
		tempNode = theSimilarityList->first;
		while(tempNode != NULL){
			if(tempNode->i3 == 1 && tempNode->d2<wgt){
				result = tempNode->d1;
				wgt = tempNode->d2;
			}
			tempNode = tempNode->next;
		}
		return result;
	}
	else if(integratingAlgo== 2){ //suplement
		List<IIIDDDNode>* supplementCases = new List<IIIDDDNode>;
		List<IIIDDDNode>* otherCases = new List<IIIDDDNode>;
		tempNode = theSimilarityList->first;
		while(tempNode != NULL){
			if(tempNode->i3 == 2)
				supplementCases->addTail(tempNode);
			else
				otherCases->addTail(tempNode);
			tempNode = tempNode->next;
		}
		result = integratingCases(otherCases);
		if(result<VERYSMALL)
			result = integratingCases(supplementCases);
		delete supplementCases;
		delete otherCases;
		return result;
	}
	else if(integratingAlgo == 3){ //maximum
		tempNode = theSimilarityList->first;
		while(tempNode!=NULL){
			if(result<tempNode->d1)
				result = tempNode->d1;
			tempNode = tempNode->next;
		}
		return result;
	}
	else if(integratingAlgo == 4){ //nearest
		tempNode = theSimilarityList->first;
		while(tempNode!=NULL){
			if(wgt>tempNode->d2){
				wgt = tempNode->d2;
				result = tempNode->d1;
			}
			tempNode = tempNode->next;
		}
		return result;
	}
	else if(integratingAlgo == 5){ //average
		tempNode = theSimilarityList->first;
		int num = 0;
		while(tempNode!=NULL){
			result += tempNode->d1;
			num ++;
			tempNode = tempNode->next;
		}
		return (float)(result/num);
	}
	else if(integratingAlgo == 6){ //inverse distance
		tempNode = theSimilarityList->first;
		wgt = VERYSMALL/1000000.0;
		while(tempNode!=NULL){
			result += tempNode->d1/pow(tempNode->d2/1000.0, wgtDecayFactor);
			wgt += 1/pow(tempNode->d2/1000.0, wgtDecayFactor);
			tempNode = tempNode->next;
		}
		return (float)(result/wgt);
	}
	else if(integratingAlgo == 7){ //similarity weight
		tempNode = theSimilarityList->first;
		wgt = VERYSMALL/1000000.0;
		while(tempNode!=NULL){
			result += tempNode->d1/pow(tempNode->d1, wgtDecayFactor);
			wgt += 1/pow(tempNode->d1, wgtDecayFactor);
			tempNode = tempNode->next;
		}
		return (float)(result/wgt);
	}
	else if(integratingAlgo == 8){ //minimum (this is a special case, only used by fuzzy boundary operation in polygon CBR.
		result = 99999;
		tempNode = theSimilarityList->first;
		while(tempNode!=NULL){
			if(result>tempNode->d1)
				result = tempNode->d1;
			tempNode = tempNode->next;
		}
		return result;
	}
	else
		return 0;
}

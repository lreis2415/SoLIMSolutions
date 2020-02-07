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
#include "EnvData.h"
#include "afxwin.h"

//The Setting class
Setting::Setting(CString aDataFile){
	ThreeDR* theData = new ThreeDR;
	theData->readin(aDataFile.GetBuffer(1), 0);
	ncol = theData->getNumberOfColumns();
	nrow = theData->getNumberOfRows();
	cellsize = theData->getCellSize(); 
	west = theData->getXmin();
	east = west+cellsize*ncol;
	south = theData->getYmin();
	north = south+cellsize*nrow;

	west_min = theData->getXmin();
	east_min = west+cellsize*ncol;
	south_min = theData->getYmin();
	north_min = south+cellsize*nrow;

	strcpy(gridUnits,theData->getGridUnits());
	strcpy(dataUnits,theData->getDataUnits());
	nodata = theData->getNullDataValue();
	delete theData;
}

Setting::Setting(const Setting& aSetting){
	ncol = aSetting.ncol;
	nrow = aSetting.nrow;
	cellsize = aSetting.cellsize; 
	west = aSetting.west;
	east = aSetting.east;
	north = aSetting.north;
	south = aSetting.south;

	west_min = aSetting.west_min;
	east_min = aSetting.east_min;
	north_min = aSetting.north_min;
	south_min = aSetting.south_min;

	strcpy(gridUnits,aSetting.gridUnits);
	strcpy(dataUnits,aSetting.dataUnits);
	nodata = aSetting.nodata;
}

EnvLayer::EnvLayer(int anID, CString aLayerName, CString aFileName){
	id = anID;
	layerName = aLayerName;
	fileName = aFileName;
	theData = new ThreeDR;
	if(theData->readin(fileName.GetBuffer(1), 0)!=0)
		id = -1;
	nextLayer = NULL;
}

EnvLayer::EnvLayer(const EnvLayer& aEnvLayer){
	id = aEnvLayer.id;
	layerName = aEnvLayer.layerName;
	fileName = aEnvLayer.fileName;
	theData = new ThreeDR(*(aEnvLayer.theData));
	nextLayer = NULL;
}

EnvLayer::~EnvLayer(){
	delete theData;
}

EnvDatabase::EnvDatabase(){
	NameListFile = "";
	numOfLayers = 0;
	layerID = 0;
	newEnv = true;
	theSetting = NULL;
	firstLayer = NULL;
	lastLayer = NULL;
	needSave = false;
}

EnvDatabase::EnvDatabase(const EnvDatabase& anEnvDatabase){
	NameListFile = anEnvDatabase.NameListFile;
	numOfLayers = anEnvDatabase.numOfLayers;
	layerID = anEnvDatabase.layerID;
	newEnv = anEnvDatabase.newEnv;
	if(anEnvDatabase.theSetting!=NULL)
		theSetting = new Setting(*(anEnvDatabase.theSetting));
	else
		theSetting = NULL;

	if(anEnvDatabase.firstLayer == NULL){
		firstLayer = NULL;
		lastLayer = NULL;
		needSave = false;
	}
	else{
		EnvLayer* tempLayer = anEnvDatabase.firstLayer;
		if(tempLayer != NULL){
			firstLayer = new EnvLayer(*tempLayer);
			lastLayer = firstLayer;
			lastLayer->nextLayer = NULL;
		}
		while(tempLayer->nextLayer != NULL){
			tempLayer = tempLayer->nextLayer;
			EnvLayer* aNewLayer = new EnvLayer(*tempLayer);
			lastLayer->nextLayer = aNewLayer;
			lastLayer = lastLayer->nextLayer;
			lastLayer->nextLayer = NULL;
		}
		needSave = true;
	}
	
}

EnvDatabase::~EnvDatabase(){
	NameListFile.Empty();
	numOfLayers = 0;
	newEnv = true;
	if(theSetting != NULL)
		delete theSetting;

	EnvLayer* tempLayer = firstLayer;
	while(tempLayer != NULL){
		firstLayer = tempLayer->nextLayer;
		delete tempLayer;
		tempLayer = firstLayer;
	}
}

bool EnvDatabase::addLayer(EnvLayer* aLayer){
	numOfLayers++;
	if(firstLayer == NULL){
		theSetting = new Setting(aLayer->fileName);
		firstLayer = aLayer;
		lastLayer = aLayer;
		lastLayer->nextLayer = NULL;
	}
	else{
		if(fabs(aLayer->theData->getCellSize()-theSetting->cellsize)> VERYSMALL)
		{ 
		   AfxMessageBox("The cell size of the new layer does not match the first layer.The layer will not be added!");
		   return false;
		}
		/*else if(strcmp(aLayer->theData->getGridUnits(),theSetting->gridUnits)!=0)
		{
			AfxMessageBox("The unit of the new layer does not match the first layer./n The layer will not be added!");
		    return false;
		}*/

        double prevWest,prevEast,prevSouth,prevNorth;	
		prevWest = theSetting->west_min;
		prevEast = theSetting->east_min;
        prevSouth = theSetting->south_min;
	   prevNorth = theSetting->north_min;

		if(theSetting->west_min < aLayer->theData->getXmin())			
			theSetting->west_min = aLayer->theData->getXmin();			

		if(theSetting->east_min > aLayer->theData->getXmin()+aLayer->theData->getCellSize()*aLayer->theData->getNumberOfColumns())		
			theSetting->east_min = aLayer->theData->getXmin()+aLayer->theData->getCellSize()*aLayer->theData->getNumberOfColumns();
			
		if(theSetting->south_min < aLayer->theData->getYmin())			
			theSetting->south_min = aLayer->theData->getYmin();
		
		if(theSetting->north_min > aLayer->theData->getYmin()+aLayer->theData->getCellSize()*aLayer->theData->getNumberOfRows())
			theSetting->north_min = aLayer->theData->getYmin()+aLayer->theData->getCellSize()*aLayer->theData->getNumberOfRows();
			
	

		if(theSetting->west_min >= theSetting->east_min || theSetting->south_min > theSetting->north_min)
		{
			 //return to previous state
			 theSetting->west_min = prevWest;
			 theSetting->east_min = prevEast;
			 theSetting->south_min = prevSouth;
			 theSetting->north_min = prevNorth;
			 AfxMessageBox("The spatial extent of the new layer does not have overlay with the previous layers. The layer will not be added!");

		     return false;
		}

	
		lastLayer->nextLayer = aLayer;
		lastLayer = lastLayer->nextLayer;
		lastLayer->nextLayer = NULL;
	}
	
	needSave = true;
	return true;
}

void EnvDatabase::deleteLayer(CString aLayerName){
	EnvLayer* theLayer = firstLayer;

	while(theLayer != NULL){
		if(aLayerName == theLayer->layerName)
			break;
		theLayer = theLayer->nextLayer;
	}

	if(firstLayer == NULL) return;

	EnvLayer *tagLayer = NULL;
	if(firstLayer == theLayer && lastLayer == theLayer){ //theLayer is the only node in the list
		firstLayer = NULL;
		lastLayer = NULL;
		delete theSetting;
		theSetting = NULL;
	}
	else if(firstLayer == theLayer)
	{
		firstLayer = firstLayer->nextLayer;
		tagLayer = firstLayer;

		theSetting->west = firstLayer->theData->getXmin();
		theSetting->east = theSetting->west+firstLayer->theData->getCellSize()*firstLayer->theData->getNumberOfColumns();
	    theSetting->south = firstLayer->theData->getYmin();
		theSetting->north = theSetting->south+firstLayer->theData->getCellSize()*firstLayer->theData->getNumberOfRows();

		theSetting->ncol = firstLayer->theData->getNumberOfColumns();
	    theSetting->nrow = firstLayer->theData->getNumberOfRows();
	    theSetting->cellsize = firstLayer->theData->getCellSize(); 
	

	    strcpy(theSetting->gridUnits,firstLayer->theData->getGridUnits());
	    strcpy(theSetting->dataUnits,firstLayer->theData->getDataUnits());
	    theSetting->nodata = firstLayer->theData->getNullDataValue();

	}

	else{
		EnvLayer* beforeLayer = firstLayer;
		while(beforeLayer->nextLayer != NULL){
			if(beforeLayer->nextLayer == theLayer){
				beforeLayer->nextLayer = theLayer->nextLayer;
				if(lastLayer == theLayer)	lastLayer = beforeLayer;
				break;
			}
			beforeLayer = beforeLayer->nextLayer;
		}
		tagLayer = beforeLayer->nextLayer;
	}
	delete theLayer;
	theLayer = NULL;
	while(tagLayer!=NULL)
	{
		//tagLayer->id--;
		tagLayer = tagLayer->nextLayer;
	}

	if(lastLayer != NULL) lastLayer->nextLayer = NULL;
	numOfLayers--;

	if(firstLayer != NULL)
	{
    //recalculate the bounding box
	theSetting->west_min = firstLayer->theData->getXmin();
    theSetting->east_min = theSetting->west+firstLayer->theData->getCellSize()*firstLayer->theData->getNumberOfColumns();
	theSetting->south_min = firstLayer->theData->getYmin();
	theSetting->north_min = theSetting->south+firstLayer->theData->getCellSize()*firstLayer->theData->getNumberOfRows();

	EnvLayer* loopLayer = firstLayer->nextLayer;
	  while(loopLayer!=NULL)
	  {
	    if(theSetting->west_min < loopLayer->theData->getXmin())
			theSetting->west_min = loopLayer->theData->getXmin();
		if(theSetting->east_min > loopLayer->theData->getXmin()+loopLayer->theData->getCellSize()*loopLayer->theData->getNumberOfColumns())
			theSetting->east_min = loopLayer->theData->getXmin()+loopLayer->theData->getCellSize()*loopLayer->theData->getNumberOfColumns();
		if(theSetting->south_min < loopLayer->theData->getYmin())
			theSetting->south_min = loopLayer->theData->getYmin();
		if(theSetting->north_min > loopLayer->theData->getYmin()+loopLayer->theData->getCellSize()*loopLayer->theData->getNumberOfRows())
			theSetting->north_min = loopLayer->theData->getYmin()+loopLayer->theData->getCellSize()*loopLayer->theData->getNumberOfRows();
		loopLayer=loopLayer->nextLayer;
	  }
	}

	needSave = true;
}

void EnvDatabase::deleteAll(){
	EnvLayer* tempLayer = firstLayer;
	while(tempLayer != NULL){
		firstLayer = tempLayer->nextLayer;
		delete tempLayer;
		tempLayer = firstLayer;
	}
	firstLayer = NULL;
	lastLayer = NULL;
	numOfLayers = 0;
	layerID = 0;
	newEnv = true;
	needSave = true;

	
}

void EnvDatabase::setSaved(){
	needSave = false;
}
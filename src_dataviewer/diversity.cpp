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
// diversity.cpp
// Calculates the diversity of pixels (in a 3dr file) 
// within a specified polygon (in an Arc/Info ungenerate file)
// Prepared by Xun Shi
// Last modified: June. 18, 2003

#include "diversity.h"

alien::alien(int aType, int aNumOfPixels){
	type = aType;
	numOfPixels = aNumOfPixels;
	nextA = NULL;
}

float alien::calcArea(float cellsize){
	return cellsize*numOfPixels;
}

diversPatch::diversPatch(int aType){
	type = aType;
	labelX = NODOUBLEVALUE;
	labelY = NODOUBLEVALUE;
	numOfPixels = 0;
	firstA = NULL;
	lastA = NULL;
	nextD = NULL;
}

diversPatch::~diversPatch(){
	alien* tempA;
	while(firstA != NULL){
		tempA = firstA;
		firstA = firstA->nextA;
		delete tempA;
	}
}

void diversPatch::addAlien(alien* anAlien){
	numOfPixels += anAlien->numOfPixels;
	if(firstA == NULL){
		firstA = anAlien;
		lastA = anAlien;
		lastA->nextA = NULL;
		return;
	}
	//If the same type alien alread exits, 
	//add up the area, then delete the new alien.
	alien* tempA = firstA;
	while(tempA != NULL){ 
		if(tempA->type == anAlien->type){
			tempA->numOfPixels += anAlien->numOfPixels;
			delete anAlien;
			return;
		}
		tempA = tempA->nextA;
	}
	//If a real new alien, attach it to the end of 
	//the alien list.
	lastA->nextA = anAlien;
	lastA = lastA->nextA;
	lastA->nextA = NULL;
}

float diversPatch::calcArea(float cellsize){
	return cellsize*numOfPixels;
}

float diversPatch::calcPercentage(alien* anAlien){
	return (float)anAlien->numOfPixels/(float)numOfPixels*100;
}

void diversPatch::sorting(){ //use bubble algo. to sort the aliens according to their areas	
	alien* tempA1 = firstA;
	while(tempA1 != NULL){
		alien* tempA2 = tempA1->nextA;
		while(tempA2 != NULL){
			if(tempA1->numOfPixels < tempA2->numOfPixels){
				switchAliens(tempA1, tempA2);
				alien* tempA = tempA1;
				tempA1 = tempA2;
				tempA2 = tempA;
				if(firstA == tempA2)
					firstA = tempA1;
				if(lastA == tempA1)
					lastA = tempA2;
			}
			tempA2 = tempA2->nextA;
		}
		tempA1 = tempA1->nextA;
	}
}

void diversPatch::switchAliens(alien* a1, alien* a2){ //a1 must be before a2
	if(a1 == firstA && a2 == firstA) return;
	if(a1 == lastA && a2 == lastA) return;
	alien* before1 = findBefore(a1);
	alien* before2 = findBefore(a2);
	alien* next1 = a1->nextA;
	alien* next2 = a2->nextA;
	
	if(before1 != NULL)
		before1->nextA = a2;
	a1->nextA = next2;
	if(before2 != a1){
		before2->nextA = a1;
		a2->nextA = next1;
	}
	else
		a2->nextA = a1;
}

alien* diversPatch::findBefore(alien* a){
	if(a == firstA)
		return NULL;
	else{
		alien* tempA = firstA;
		while(tempA != NULL){
			if(tempA->nextA == a)
				return tempA;
			tempA = tempA->nextA;
		}
	}
	return NULL;
}

void diversPatch::output(int numOfEle, ofstream& fout){
	int i = 0;
	alien* tempA = firstA;
	float percentOfOther = 0;

	fout.setf(ios::fixed);
	fout.precision(2);
	fout<<type<<" "<<labelX<<" "<<labelY<<" ";
	while(i < numOfEle){
		if(tempA != NULL){
			fout<<tempA->type<<" "<<calcPercentage(tempA)<<" ";
			tempA = tempA->nextA;
		}
		else{
			fout<<"NILL 0 ";
		}
		i++;
	}
	while(tempA != NULL){
		percentOfOther += calcPercentage(tempA);	
		tempA = tempA->nextA;
	}
	fout<<"Other "<<percentOfOther<<endl;
}

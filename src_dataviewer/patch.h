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
//patch.h
//header file of tools for handling patches.
//written by Xun Shi
//last modified: Aug. 12, 2001

#ifndef PATCH_H
#define PATCH_H

#include "utility.h"

class PatchPixel{
public:
	int pos;
	bool atBorder;
	bool exit;
	PatchPixel* nextPixel;
	
	PatchPixel(int aPos){
		pos = aPos;
		atBorder = false;
		exit = false;
		nextPixel = NULL;
	};
};

template<class T>
class Patch{
private:
	int testNext(int pos, int nextPosIndex, List<IntNode>* theLink, unsigned char* idBuf);
	int buildBlock(int firstPos, unsigned char* idBuf, bool eightDir, T seperateValue); 
public:
	//Attributes
	T value;
	T* dataBuf;
	int nrow;
	int ncol;
	int numPixels;
	int numOfValues;
	float bottom;
	int bottomPos;
	float lowestEdge;
	int lowestPourPos;
	int lowestPourPosNext;

	int matrixStartPos;
	int matrixNrow;
	int matrixNcol;
	int* matrixBuf;

	PatchPixel* first;
	PatchPixel* last;

	//constructors and destructors
	Patch();
	Patch(int firstPos, T* dataBuf, unsigned char* idBuf, int nrow, int ncol, bool eightDir);
	Patch(int firstPos, T* dataBuf, unsigned char* idBuf, int nrow, int ncol, T seperateValue);
	Patch(const Patch& aPatch);
	~Patch();

	//basic operations
	void findBorder(bool eight);										 
	void buildMatrix(int label);
	void setMatrixValue(int pos, int value);
	int getMatrixValue(int pos);
	int addPixel(PatchPixel* aPixel);
	void removePixel(PatchPixel* aPixel);
	
	//special operations
	float findMajority(float* dataBuf); //find out the majority of another data layer within the patch.
	int doRemoval(float* workingBuf, bool eightDir, bool preserveIsolated, float nodata); 
										//let the patch disapear in the background (set by the dataBuf).
	float findMinDiff(float* datBuf, float nodata);		//find the difference between the bottom and the lowest edge,
										//in terms of the data in another layer (the dataBuf).
	void assignValue(T* aDataBuf, T aValue); //change the values of pixels of aDataBuf that are within the patch to aValue
};

//--------------------------------------------------
// Implementation of Patch
// 
// The declaration and implementation of Patch have
// to be in the same file, because it is a class
// template.
//--------------------------------------------------

template<class T>
Patch<T>::Patch(){
	dataBuf = NULL;
	value = 0;
	nrow = 0;
	ncol = 0;
	numPixels = 0;
	numOfValues = 0;
	bottom = 999999;
	bottomPos = -1;
	lowestEdge = 999999;
	lowestPourPos = -1;
	lowestPourPosNext = -1;
	matrixStartPos = 0;
	matrixNrow = 0;
	matrixNcol = 0;
	matrixBuf = NULL;

	first = NULL;
	last = NULL;
}

template<class T>
Patch<T>::Patch(int firstPos, T* theDataBuf, unsigned char* idBuf, int theNrow, int theNcol, bool eightDir){
	dataBuf = theDataBuf;
	value = dataBuf[firstPos];
	nrow = theNrow;
	ncol = theNcol;
	numPixels = 0;
	numOfValues = 0;
	bottom = 999999;
	bottomPos = -1;
	lowestEdge = 999999;
	lowestPourPos = -1;
	lowestPourPosNext = -1;

	matrixStartPos = 0;
	matrixNrow = 0;
	matrixNcol = 0;
	matrixBuf = NULL;

	first = NULL;
	last = NULL;

	int b = buildBlock(firstPos, idBuf, eightDir, NODOUBLEVALUE);
}

template<class T>
Patch<T>::Patch(int firstPos, T* theDataBuf, unsigned char* idBuf, int theNrow, int theNcol, T seperateValue){ //seperateValue is the value of streamlines, which is for seperating two blocks on the two sides of a streamline.
	dataBuf = theDataBuf;
	value = dataBuf[firstPos];
	nrow = theNrow;
	ncol = theNcol;
	numPixels = 0;
	numOfValues = 0;
	bottom = 999999;
	bottomPos = -1;
	lowestEdge = 999999;
	lowestPourPos = -1;
	lowestPourPosNext = -1;

	matrixStartPos = 0;
	matrixNrow = 0;
	matrixNcol = 0;
	matrixBuf = NULL;

	first = NULL;
	last = NULL;

	int b = buildBlock(firstPos, idBuf, true, seperateValue);
}

template<class T>
Patch<T>::Patch(const Patch<T>& aPatch){
	value = aPatch.value;
	numPixels = aPatch.numPixels;
	numOfValues = aPatch.numOfValues;
	bottom = aPatch.bottom;
	bottomPos = aPatch.bottomPos;
	lowestEdge = aPatch.lowestEdge;
	lowestPourPos = aPatch.lowestPourPos;
	lowestPourPosNext = aPatch.lowestPourPosNext;
	dataBuf = aPatch.dataBuf;
	nrow = aPatch.nrow;
	ncol = aPatch.ncol;

	matrixStartPos = aPatch.matrixStartPos;
	matrixNrow = aPatch.matrixNrow;
	matrixNcol = aPatch.matrixNcol;
	matrixBuf = new int*[matrixNrow*matrixNcol];
	for(int i = 0; i<matrixNrow; i++)
		for(int j = 0; j<matrixNcol; j++)
			matrixBuf[i*matrixNcol+j] = aPatch.matrixBuf[i*matrixNcol+j];

	if(aPatch.first == NULL){
		first = NULL;
		last = NULL;
	}
	else{
		PatchPixel* tempPixel = aPatch.first;
		first = new PatchPixel(tempPixel->pos);
		first->atBorder = tempPixel->atBorder;
		first->exit = tempPixel->exit;
		first->nextPixel = NULL;
		last = first;
		tempPixel = tempPixel->nextPixel;
		while(tempPixel!=NULL){
			PatchPixel* end = new PatchPixel(tempPixel->pos);
			end->atBorder = tempPixel->atBorder;
			end->exit = tempPixel->exit;
			end->nextPixel = NULL;
			last->nextPixel = end;
			last = end;
			tempPixel = tempPixel->nextPixel;
		}
	}
}

template<class T>
Patch<T>::~Patch(){
	if(matrixBuf!=NULL)
		delete [] matrixBuf;
	while(first != NULL){
		PatchPixel* tempPixel = first;
		first = first->nextPixel;
		delete tempPixel;
	}
}

template<class T>
int Patch<T>::addPixel(PatchPixel* aPixel){
	if(aPixel == NULL) return 1; //error: no sufficient memory
	if(first == NULL){
		first = aPixel;
		last = aPixel;
	}
	else{
		last->nextPixel = aPixel;
		last = last->nextPixel;
	}
	numPixels++;
	return 0;
}

template<class T>
void Patch<T>::removePixel(PatchPixel* aPixel){
	PatchPixel* beforePixel = first;
	while(beforePixel != NULL){
		if(beforePixel == aPixel){
			first = beforePixel->nextPixel;
			delete beforePixel;
			numPixels--;
			break;
		}
		if(beforePixel->nextPixel == aPixel){
			beforePixel->nextPixel = aPixel->nextPixel;
			delete aPixel;
			numPixels--;
			break;
		}
		beforePixel = beforePixel->nextPixel;
	}
}

template<class T>
void Patch<T>::findBorder(bool eight){
	PatchPixel* tempPixel = first;
	while(tempPixel != NULL){
		int pos = tempPixel->pos;
		tempPixel->atBorder = false;

		//if at the border of the image, set to be at border of the patch
		if(pos<(ncol-1) || pos%ncol==0 || (pos+1)%ncol==0 || (nrow*ncol-pos)<ncol){
			tempPixel->atBorder = true;
			tempPixel = tempPixel->nextPixel;
			continue;
		}
		if(assertN(pos, nrow, ncol))
			if(dataBuf[getN(pos,ncol)] != value){
				tempPixel->atBorder = true;
				tempPixel = tempPixel->nextPixel;
				continue;
			}
		if(assertW(pos, nrow, ncol))
			if(dataBuf[getW(pos,ncol)] != value){
				tempPixel->atBorder = true;
				tempPixel = tempPixel->nextPixel;
				continue;
			}
		if(assertE(pos, nrow, ncol))
			if(dataBuf[getE(pos,ncol)] != value){
				tempPixel->atBorder = true;
				tempPixel = tempPixel->nextPixel;
				continue;
			}
		if(assertS(pos, nrow, ncol))
			if(dataBuf[getS(pos,ncol)] != value){
				tempPixel->atBorder = true;
				tempPixel = tempPixel->nextPixel;
				continue;
			}
		if(eight){
		if(assertNW(pos, nrow, ncol))
			if(dataBuf[getNW(pos,ncol)] != value){
				tempPixel->atBorder = true;
				tempPixel = tempPixel->nextPixel;
				continue;
			}
		if(assertNE(pos, nrow, ncol))
			if(dataBuf[getNE(pos,ncol)] != value){
				tempPixel->atBorder = true;
				tempPixel = tempPixel->nextPixel;
				continue;
			}
		if(assertSW(pos, nrow, ncol))
			if(dataBuf[getSW(pos,ncol)] != value){
				tempPixel->atBorder = true;
				tempPixel = tempPixel->nextPixel;
				continue;
			}
		if(assertSE(pos, nrow, ncol))
			if(dataBuf[getSE(pos,ncol)] != value){
				tempPixel->atBorder = true;
				tempPixel = tempPixel->nextPixel;
				continue;
			}
		} //end of if(eight)
		tempPixel = tempPixel->nextPixel;
	}
}

template<class T>
int Patch<T>::testNext(int pos, int nextPosIndex, List<IntNode>* theLink, unsigned char* idBuf){
	int a = 0; //indicator of the success of adding node and pixel (0: success; 1: no sufficient memory.)
	if(idBuf[nextPosIndex]==0 && dataBuf[nextPosIndex]==dataBuf[pos]){
		a = theLink->addTail(new IntNode(nextPosIndex));
		a = addPixel(new PatchPixel(nextPosIndex));
		idBuf[nextPosIndex] = 1;
	}
	return a;
}

template<class T>
int Patch<T>::buildBlock(int firstPos, unsigned char* idBuf, bool eightDir, T seperateValue){
	int a = 0; //indicator of the success of adding node and pixel (0: success; 1: no sufficient memory.)
	List<IntNode>* theLink = new List<IntNode>;
	a = theLink->addTail(new IntNode(firstPos));
	a = addPixel(new PatchPixel(firstPos));
	if(a != 0) return a;

	idBuf[firstPos] = 1;
	while(theLink->first!=NULL){
		int pos = theLink->first->pos;

		if(assertN(pos, nrow, ncol)){
			a = testNext(pos, getN(pos, ncol), theLink, idBuf);
			if(a != 0) return a;
		}
		if(assertW(pos, nrow, ncol)){
			a = testNext(pos, getW(pos, ncol), theLink, idBuf);
			if(a != 0) return a;
		}
		if(assertE(pos, nrow, ncol)){
			a = testNext(pos, getE(pos, ncol), theLink, idBuf);
			if(a != 0) return a;
		}
		if(assertS(pos, nrow, ncol)){
			a = testNext(pos, getS(pos, ncol), theLink, idBuf);
			if(a != 0) return a;
		}
		if(eightDir){ //build 8-connected block if needed
			if(assertNW(pos, nrow, ncol)){
				if(dataBuf[pos-1]!=seperateValue || dataBuf[pos-ncol]!=seperateValue){
					a = testNext(pos, getNW(pos, ncol), theLink, idBuf);
					if(a != 0) return a;
				}
			}
			if(assertNE(pos, nrow, ncol)){
				if(dataBuf[pos+1]!=seperateValue || dataBuf[pos-ncol]!=seperateValue){
					a = testNext(pos, getNE(pos, ncol), theLink, idBuf);
					if(a != 0) return a;
				}
			}
			if(assertSW(pos, nrow, ncol)){
				if(dataBuf[pos-1]!=seperateValue || dataBuf[pos+ncol]!=seperateValue){
					a = testNext(pos, getSW(pos, ncol), theLink, idBuf);
					if(a != 0) return a;
				}
			}
			if(assertSE(pos, nrow, ncol)){
				if(dataBuf[pos+1]!=seperateValue || dataBuf[pos+ncol]!=seperateValue){
					a = testNext(pos, getSE(pos, ncol), theLink, idBuf);
					if(a != 0) return a;
				}
			}
		}
		theLink->removeHead();
	}
	delete theLink;
	return 0;
}

template<class T>
void Patch<T>::buildMatrix(int label){
	if(matrixBuf!=NULL)
		delete [] matrixBuf;
	int rowMin = nrow;
	int rowMax = 0;
	int colMin = ncol;
	int colMax = 0;
	PatchPixel* temp = first;
	while(temp!=NULL){
		int row = temp->pos/ncol;
		int col = temp->pos - row*ncol;
		if(rowMin>row)
			rowMin = row;
		if(rowMax<row)
			rowMax = row;
		if(colMin>col)
			colMin = col;
		if(colMax<col)
			colMax = col;
		temp = temp->nextPixel;
	}
	matrixStartPos = rowMin*ncol + colMin;
	matrixNrow = rowMax - rowMin + 1;
	matrixNcol = colMax - colMin + 1;
	matrixBuf = new int[matrixNrow*matrixNcol];
	
	for(int i = 0; i<matrixNrow*matrixNcol; i++)
		matrixBuf[i] = 0;

	temp = first;
	while(temp!=NULL){
		int row = (temp->pos-matrixStartPos)/ncol;
		int col = temp->pos-matrixStartPos-row*ncol;
		matrixBuf[row*matrixNcol+col] = label;
		temp = temp->nextPixel;
	}
}

template<class T>
void Patch<T>::setMatrixValue(int pos, int value){
	if(matrixBuf == NULL) return;
	int row = (pos-matrixStartPos)/ncol;
	int col = pos-matrixStartPos-row*ncol;
	if(row<0 || row>matrixNrow-1 || col<0 || col >matrixNcol-1)
		return;
	matrixBuf[row*matrixNcol+col] = value;
}

template<class T>
int Patch<T>::getMatrixValue(int pos){
	if(matrixBuf == NULL) return NOINTVALUE;
	if(pos<0 || pos > nrow*ncol-1) return NOINTVALUE;
	int row = (pos-matrixStartPos)/ncol;
	int col = pos-matrixStartPos-row*ncol;
	if(row<0 || row>matrixNrow-1 || col<0 || col>matrixNcol-1)
		return 0;
	return matrixBuf[row*matrixNcol+col];
}

template<class T>
float Patch<T>::findMajority(float* dataBuf){
	PatchPixel* tempPixel = first;
	List<ValueNode>* theValueList = new List<ValueNode>;
	while(tempPixel != NULL){
		bool flag = true;
		ValueNode* tempValue = theValueList->first;
		while(tempValue!=NULL){
			if(dataBuf[tempPixel->pos]==tempValue->value){
				tempValue->num++;
				flag=false;
				break;
			}
			tempValue = tempValue->next;
		}
		if(flag)
			theValueList->addTail(new ValueNode(dataBuf[tempPixel->pos], 1));
		tempPixel = tempPixel->nextPixel;
	}

	float theMajority = 0;
	int theMaxNum = 0;
	ValueNode* tempValue = theValueList->first;
	while(tempValue!=NULL){
		if(tempValue->num>theMaxNum)
			theMajority = tempValue->value;
		tempValue = tempValue->next;
	}
	delete theValueList;
	return theMajority;
}

template<class T>
int Patch<T>::doRemoval(float* workingBuf, bool eightDir, bool preserveIsolated, float nodata){
	int removed = 0;
	
	int i, k;
	bool flag = true;

	while(first!=NULL){
		findBorder(false);
		PatchPixel* currentPixel = first;

		while(currentPixel!=NULL){
			if(currentPixel->atBorder){
				float values[8];
				int vote[8];
				for (i = 0; i<8; i++) vote[i] = 0;
				int numOfV = 0;
				bool nodataFlag = false;
				int neighborPos = -1;
				if(assertN(currentPixel->pos, nrow, ncol)){
					neighborPos = getN(currentPixel->pos, ncol);
					if(dataBuf[neighborPos] == 3){
						values[numOfV] = workingBuf[neighborPos];
						vote[numOfV]++;
						numOfV++;
					}
					else if(dataBuf[neighborPos] == 4)
						nodataFlag = true;
				}
				if(assertW(currentPixel->pos, nrow, ncol)){
					neighborPos = getW(currentPixel->pos, ncol);
					if(dataBuf[neighborPos] == 3){
						flag = true;
						for(k = 0; k < numOfV; k++)
							if(fabs(values[k] - workingBuf[neighborPos]) < VERYSMALL){
								vote[k]++;
								flag = false;
								break;
							}
						if(flag){
							values[numOfV] = workingBuf[neighborPos];
							vote[numOfV]++;
							numOfV++;
						}
					}
					else if(dataBuf[neighborPos] == 4)
						nodataFlag = true;
				}
				if(assertE(currentPixel->pos, nrow, ncol)){
					neighborPos = getE(currentPixel->pos, ncol);
					if(dataBuf[neighborPos] == 3){
						flag = true;
						for(k = 0; k < numOfV; k++)
							if(fabs(values[k] - workingBuf[neighborPos]) < VERYSMALL){
								vote[k]++;
								flag = false;
								break;
							}
						if(flag){
							values[numOfV] = workingBuf[neighborPos];
							vote[numOfV]++;
							numOfV++;
						}
					}
					else if(dataBuf[neighborPos] == 4)
						nodataFlag = true;
				}
				if(assertS(currentPixel->pos, nrow, ncol)){
					neighborPos = getS(currentPixel->pos, ncol);
					if(dataBuf[neighborPos] == 3){
						flag = true;
						for(k = 0; k < numOfV; k++)
							if(fabs(values[k] - workingBuf[neighborPos]) < VERYSMALL){
								vote[k]++;
								flag = false;
								break;
							}
						if(flag){
							values[numOfV] = workingBuf[neighborPos];
							vote[numOfV]++;
							numOfV++;
						}
					}
					else if(dataBuf[neighborPos] == 4)
						nodataFlag = true;
				}
				if(eightDir){
				if(assertNW(currentPixel->pos, nrow, ncol)){
					neighborPos = getNW(currentPixel->pos, ncol);
					if(dataBuf[neighborPos] == 3){
						flag = true;
						for(k = 0; k < numOfV; k++)
							if(fabs(values[k] - workingBuf[neighborPos]) < VERYSMALL){
								vote[k]++;
								flag = false;
								break;
							}
						if(flag){
							values[numOfV] = workingBuf[neighborPos];
							vote[numOfV]++;
							numOfV++;
						}
					}
					else if(dataBuf[neighborPos] == 4)
						nodataFlag = true;
				}
				if(assertNE(currentPixel->pos, nrow, ncol)){
					neighborPos = getNE(currentPixel->pos, ncol);
					if(dataBuf[neighborPos] == 3){
						flag = true;
						for(k = 0; k < numOfV; k++)
							if(fabs(values[k] - workingBuf[neighborPos]) < VERYSMALL){
								vote[k]++;
								flag = false;
								break;
							}
						if(flag){
							values[numOfV] = workingBuf[neighborPos];
							vote[numOfV]++;
							numOfV++;
						}
					}
					else if(dataBuf[neighborPos] == 4)
						nodataFlag = true;
				}
				if(assertSW(currentPixel->pos, nrow, ncol)){
					neighborPos = getSW(currentPixel->pos, ncol);
					if(dataBuf[neighborPos] == 3){
						flag = true;
						for(k = 0; k < numOfV; k++)
							if(fabs(values[k] - workingBuf[neighborPos]) < VERYSMALL){
								vote[k]++;
								flag = false;
								break;
							}
						if(flag){
							values[numOfV] = workingBuf[neighborPos];
							vote[numOfV]++;
							numOfV++;
						}
					}
					else if(dataBuf[neighborPos] == 4)
						nodataFlag = true;
				}
				if(assertSE(currentPixel->pos, nrow, ncol)){
					neighborPos = getSE(currentPixel->pos, ncol);
					if(dataBuf[neighborPos] == 3){
						flag = true;
						for(k = 0; k < numOfV; k++)
							if(fabs(values[k] - workingBuf[neighborPos]) < VERYSMALL){
								vote[k]++;
								flag = false;
								break;
							}
						if(flag){
							values[numOfV] = workingBuf[neighborPos];
							vote[numOfV]++;
							numOfV++;
						}
					}
					else if(dataBuf[neighborPos] == 4)
						nodataFlag = true;
				}
				}//end if (eightDir)

				if(numOfV!=0){
					int maxvote = 0;
					int maxindex = 0;
					for(k = 0; k < numOfV; k++)
						if(maxvote < vote[k]){
							maxvote = vote[k];
							maxindex = k;
						}
					workingBuf[currentPixel->pos] = values[maxindex];
					removed++;
				}
				else{
					if(nodataFlag){
						if(!preserveIsolated){
							workingBuf[currentPixel->pos] = nodata;
							removed++;
						}
						else{ //if want to preserve the isolated small patch, set the value
							 //of the patch to be the majority of the patch.
							workingBuf[currentPixel->pos] = findMajority(workingBuf);
						}
					}
					else{ // just at the edge of the image, not real border
						currentPixel->atBorder = false;
					}
				}
			}
			currentPixel = currentPixel->nextPixel;
		}
		currentPixel = first;
		while(currentPixel!=NULL){
			PatchPixel* tempPixel = currentPixel;
			currentPixel = currentPixel->nextPixel;
			if(tempPixel->atBorder){ //remove all the border pixels
				dataBuf[tempPixel->pos] = 3; 
				removePixel(tempPixel);
			}
		}
	}
	return removed;
}

template<class T>
float Patch<T>::findMinDiff(float* DEMBuf, float nodata){
	bottom = 999999;
	bottomPos = -1;
	lowestEdge = 999999;
	lowestPourPos = -1;
	lowestPourPosNext = -1;
	float exit = 999999;

	PatchPixel* temp = first;
	while(temp!=NULL){
		if(DEMBuf[temp->pos]<bottom){
			bottom = DEMBuf[temp->pos];
			bottomPos = temp->pos;
		}
		if(temp->atBorder&&DEMBuf[temp->pos]<lowestEdge){
			lowestEdge = DEMBuf[temp->pos];
			lowestPourPos = temp->pos;
		}
		temp = temp->nextPixel;
	}

	//to find if the lowest outside-patch position that is adjacent to 
	//the lowest position on the patch edge (lowestPourPos) is higher than lowestPourPos.
	//If such a location is found, use its elevation value to calculate minDiff, in order to achieve a real "filling".
	buildMatrix(1);
	bool flag = false;
	int nextPosIndex = -1;
	if(assertN(lowestPourPos, nrow, ncol)){
		nextPosIndex = getN(lowestPourPos, ncol);
		if(fabs(DEMBuf[nextPosIndex]-nodata)>VERYSMALL && getMatrixValue(nextPosIndex)==0) //nextPosIndex is ourside the patch
			if(DEMBuf[nextPosIndex]<exit){
				exit = DEMBuf[nextPosIndex];
				lowestPourPosNext = nextPosIndex;
				flag = true;
			}
	}
	if(assertW(lowestPourPos, nrow, ncol)){
		nextPosIndex = getW(lowestPourPos, ncol);
		if(fabs(DEMBuf[nextPosIndex]-nodata)>VERYSMALL && getMatrixValue(nextPosIndex)==0)
			if(DEMBuf[nextPosIndex]<exit){
				exit = DEMBuf[nextPosIndex];
				lowestPourPosNext = nextPosIndex;
				flag = true;
			}
	}
	if(assertE(lowestPourPos, nrow, ncol)){
		nextPosIndex = getE(lowestPourPos, ncol);
		if(fabs(DEMBuf[nextPosIndex]-nodata)>VERYSMALL && getMatrixValue(nextPosIndex)==0)
			if(DEMBuf[nextPosIndex]<exit){
				exit = DEMBuf[nextPosIndex];
				lowestPourPosNext = nextPosIndex;
				flag = true;
			}
	}
	if(assertS(lowestPourPos, nrow, ncol)){
		nextPosIndex = getS(lowestPourPos, ncol);
		if(fabs(DEMBuf[nextPosIndex]-nodata)>VERYSMALL && getMatrixValue(nextPosIndex)==0)
			if(DEMBuf[nextPosIndex]<exit){
				exit = DEMBuf[nextPosIndex];
				lowestPourPosNext = nextPosIndex;
				flag = true;
			}
	}
	if(assertNW(lowestPourPos, nrow, ncol)){
		nextPosIndex = getNW(lowestPourPos, ncol);
		if(fabs(DEMBuf[nextPosIndex]-nodata)>VERYSMALL && getMatrixValue(nextPosIndex)==0)
			if(DEMBuf[nextPosIndex]<exit){
				exit = DEMBuf[nextPosIndex];
				lowestPourPosNext = nextPosIndex;
				flag = true;
			}
	}
	if(assertNE(lowestPourPos, nrow, ncol)){
		nextPosIndex = getNE(lowestPourPos, ncol);
		if(fabs(DEMBuf[nextPosIndex]-nodata)>VERYSMALL && getMatrixValue(nextPosIndex)==0)
			if(DEMBuf[nextPosIndex]<exit){
				exit = DEMBuf[nextPosIndex];
				lowestPourPosNext = nextPosIndex;
				flag = true;
			}
	}
	if(assertSW(lowestPourPos, nrow, ncol)){
		nextPosIndex = getSW(lowestPourPos, ncol);
		if(fabs(DEMBuf[nextPosIndex]-nodata)>VERYSMALL && getMatrixValue(nextPosIndex)==0)
			if(DEMBuf[nextPosIndex]<exit){
				exit = DEMBuf[nextPosIndex];
				lowestPourPosNext = nextPosIndex;
				flag = true;
			}
	}
	if(assertSE(lowestPourPos, nrow, ncol)){
		nextPosIndex = getSE(lowestPourPos, ncol);
		if(fabs(DEMBuf[nextPosIndex]-nodata)>VERYSMALL && getMatrixValue(nextPosIndex)==0)
			if(DEMBuf[nextPosIndex]<exit){
				exit = DEMBuf[nextPosIndex];
				lowestPourPosNext = nextPosIndex;
				flag = true;
			}
	}
	if(flag && exit > lowestEdge)
		lowestEdge = exit;

	return lowestEdge - bottom;
}

template<class T>
void Patch<T>::assignValue(T* aDataBuf, T aValue){
	if(aDataBuf == NULL) return;
	PatchPixel* tempPixel = first;
	while(tempPixel!=NULL){
		aDataBuf[tempPixel->pos] = aValue;
		tempPixel = tempPixel->nextPixel;
	}
	value = aValue;
}
#endif

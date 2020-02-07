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
//overlay.h
//provides overlay functions (in templates)
//written by Xun Shi
//created Mar 9, 2002
//modified Aug 6, 2003
//modified Feb. 21, 2004.  Add plus, subtract, multiply and divide.
//modified July 6, 2005.  Add nodata, numOfPixels.

//identity
//intersect
//mask
//union (unionOverlay - union is a keyword of C++)
//max (maxOverlay - max is a reserved word)
//min (minOverlay - min is a reserved word)
//supplement

#ifndef OVERLAY_H
#define OVERLAY_H

#include "SoLIM.h"
#include "utility.h"

template<class T>
int createMask(T* inBuf, T* outBuf, int nrow, int ncol, T lowerLimit, T upperLimit, int label, T nodata){
	if(inBuf == NULL) return 1;
	if(outBuf == NULL) return 3;

	int numOfPixels = nrow*ncol;
	T l, u;
	if(lowerLimit<upperLimit){
		l = lowerLimit;
		u = upperLimit;
	}
	else{
		l = upperLimit;
		u = lowerLimit;
	}

	for(int i = 0; i<numOfPixels; i++)
		if(fabs(inBuf[i]-nodata)<VERYSMALL)
			outBuf[i] = nodata;
		else if(inBuf[i]>=l && inBuf[i]<=u)
			outBuf[i] = label;
		else
			outBuf[i] = 0;

	return 0;
}

template<class T1, class T2>
int	identity(T1* first, T1 lowerLimitFor1, T1 upperLimitFor1, 
			  T2* second, T2 lowerLimitFor2, T2 upperLimitFor2,
			  T1* outBuf, 
			  int nrow, int ncol, T1 nodata1, T2 nodata2){
	if(first == NULL) return 1;
	if(second == NULL) return 2;
	if(outBuf == NULL) return 3;

	int numOfPixels = nrow*ncol;
	T1 l1, u1;
	T2 l2, u2;
	if(lowerLimitFor1<upperLimitFor1){
		l1 = lowerLimitFor1;
		u1 = upperLimitFor1;
	}
	else{
		l1 = upperLimitFor1;
		u1 = lowerLimitFor1;
	}
	if(lowerLimitFor2<upperLimitFor2){
		l2 = lowerLimitFor2;
		u2 = upperLimitFor2;
	}
	else{
		l2 = upperLimitFor2;
		u2 = lowerLimitFor2;
	}

	for(int i = 0; i<numOfPixels; i++)
		if(fabs(first[i]-nodata1)<VERYSMALL || fabs(second[i]-nodata2)<VERYSMALL)
			outBuf[i] = nodata1;
		else if(first[i]>=l1 && first[i]<=u1 && second[i]>=l2 && second[i]<=u2)
			outBuf[i] = first[i];
		else
			outBuf[i] = 0;

	return 0;
}

template<class T1, class T2>
int	intersect(T1* first, T1 lowerLimitFor1, T1 upperLimitFor1, 
			  T2* second, T2 lowerLimitFor2, T2 upperLimitFor2,
			  unsigned char* outBuf, 
			  int nrow, int ncol, int label, T1 nodata1, T2 nodata2){
	if(first == NULL) return 1;
	if(second == NULL) return 2;
	if(outBuf == NULL) return 3;

	int numOfPixels = nrow*ncol;
	T1 l1, u1;
	T2 l2, u2;
	if(lowerLimitFor1<upperLimitFor1){
		l1 = lowerLimitFor1;
		u1 = upperLimitFor1;
	}
	else{
		l1 = upperLimitFor1;
		u1 = lowerLimitFor1;
	}
	if(lowerLimitFor2<upperLimitFor2){
		l2 = lowerLimitFor2;
		u2 = upperLimitFor2;
	}
	else{
		l2 = upperLimitFor2;
		u2 = lowerLimitFor2;
	}

	for(i = 0; i<numOfPixels; i++)
		if(fabs(first[i]-nodata1)<VERYSMALL || fabs(second[i]-nodata2)<VERYSMALL)
			outBuf[i] = nodata1;
		else if(first[i]>l1 && first[i]<u1 && second[i]>l2 && second[i]<u2)
			outBuf[i] = label;
		else
			outBuf[i] = 0;

	return 0;
}

template<class T1, class T2>
int maxOverlay(T1* fstInBuf, T2* sndInBuf, T1* outBuf, int nrow, int ncol, T1 nodata1, T2 nodata2){
	if(fstInBuf == NULL) return 1;
	if(sndInBuf == NULL) return 2;
	if(outBuf == NULL) return 3;

	int numOfPixels = nrow*ncol;
	for(int i=0; i<numOfPixels; i++){
		if(fabs(fstInBuf[i]-nodata1)<VERYSMALL || fabs(sndInBuf[i]-nodata2)<VERYSMALL)
			outBuf[i] = nodata1;
		else if(fstInBuf[i]>sndInBuf[i])
			outBuf[i] = fstInBuf[i];
		else
			outBuf[i] = sndInBuf[i];
	}
	return 0;
}

template<class T1, class T2>
int select(T1* fstInBuf, T2* sndInBuf, T1* outBuf, int nrow, int ncol, T1 nodata1, T2 nodata2, T2 value){
	if(fstInBuf == NULL) return 1;
	if(sndInBuf == NULL) return 2;
	if(outBuf == NULL) return 3;

	int numOfPixels = nrow*ncol;
	for(int i=0; i<numOfPixels; i++)
		if(fabs(fstInBuf[i]-nodata1)<VERYSMALL || fabs(sndInBuf[i]-nodata2)<VERYSMALL || fabs(sndInBuf[i]-value)>VERYSMALL)
			outBuf[i] = nodata1;
		else
			outBuf[i] = fstInBuf[i];
	return 0;
}

template<class T1, class T2>
int mask(T1* fstInBuf, T2* sndInBuf, T1* outBuf, int nrow, int ncol, T1 nodata1, T2 nodata2, T2 value){
	if(fstInBuf == NULL) return 1;
	if(sndInBuf == NULL) return 2;
	if(outBuf == NULL) return 3;

	int numOfPixels = nrow*ncol;
	for(int i=0; i<numOfPixels; i++)
		if(fabs(fstInBuf[i]-nodata1)<VERYSMALL || fabs(sndInBuf[i]-nodata2)<VERYSMALL || fabs(sndInBuf[i]-value)<VERYSMALL)
			outBuf[i] = nodata1;
		else
			outBuf[i] = fstInBuf[i];
	return 0;
}

template<class T1, class T2>
int mask3DM(T1* dataBuf, unsigned char* photoBuf, T2* maskBuf, 
			T1* outBuf, unsigned char* outPhotoBuf, T1 nodata, int nrow, int ncol){
	if(dataBuf == NULL) return 1;
	if(photoBuf == NULL) return 2;
	if(maskBuf == NULL) return 3;
	if(outBuf == NULL) return 4;
	if(photoBuf == NULL) return 5;
	for(int i=0; i<nrow; i++){
		for(int j=0; j<ncol; j++){
			if(maskBuf[i*ncol+j]>0){
				outBuf[i*(ncol+1)+j] = dataBuf[i*(ncol+1)+j];
				outPhotoBuf[i*ncol+j] = photoBuf[i*ncol+j];
			}
			else{
				outBuf[i*(ncol+1)+j] = nodata;
				outPhotoBuf[i*ncol+j] = 0;
			}
		}
	}
	for(i=0; i<nrow; i++) //last col
		if(maskBuf[i*ncol+ncol-1]>0)
			outBuf[i*(ncol+1)+ncol] = dataBuf[i*(ncol+1)+ncol];
		else
			outBuf[i*(ncol+1)+ncol] = nodata;
	for(int j=0; j<ncol; j++) //last row
		if(maskBuf[(nrow-1)*ncol+j]>0)
			outBuf[nrow*(ncol+1)+j] = dataBuf[nrow*(ncol+1)+j];
		else
			outBuf[nrow*(ncol+1)+j] = nodata;
	if(maskBuf[nrow*ncol-1]>0) //last pixel
		outBuf[(nrow+1)*(ncol+1)-1] = dataBuf[(nrow+1)*(ncol+1)-1];
	else
		outBuf[(nrow+1)*(ncol+1)-1] = nodata;
	return 0;
}

template<class T1, class T2>
int minOverlay(T1* fstInBuf, T2* sndInBuf, T1* outBuf, int nrow, int ncol, T1 nodata1, T2 nodata2){
	if(fstInBuf == NULL) return 1;
	if(sndInBuf == NULL) return 2;
	if(outBuf == NULL) return 3;

	int numOfPixels = nrow*ncol;
	for(int i=0; i<numOfPixels; i++){
		if(fabs(fstInBuf[i]-nodata1)<VERYSMALL || fabs(sndInBuf[i]-nodata2)<VERYSMALL)
			outBuf[i] = nodata1;
		else if(fstInBuf[i]<sndInBuf[i])
			outBuf[i] = fstInBuf[i];
		else
			outBuf[i] = sndInBuf[i];
	}
	return 0;
}

template<class T1, class T2>
int supplement(T1* fstInBuf, T2* sndInBuf, T1* outBuf, int nrow, int ncol, T1 nodata1, T2 nodata2){
	if(fstInBuf == NULL) return 1;
	if(sndInBuf == NULL) return 2;
	if(outBuf == NULL) return 3;

	int numOfPixels = nrow*ncol;
	for(int i=0; i<numOfPixels; i++){
		if(fabs(fstInBuf[i]-nodata1)<VERYSMALL || fabs(sndInBuf[i]-nodata2)<VERYSMALL)
			outBuf[i] = nodata1;
		else if(fstInBuf[i]>nodata1)
			outBuf[i] = fstInBuf[i];
		else
			outBuf[i] = sndInBuf[i];
	}
	return 0;
}

template<class T1, class T2>
int plusOverlay(T1* fstInBuf, T2* sndInBuf, T1* outBuf, int nrow, int ncol, T1 nodata1, T2 nodata2){
	if(fstInBuf == NULL) return 1;
	if(sndInBuf == NULL) return 2;
	if(outBuf == NULL) return 3;

	int numOfPixels = nrow*ncol;
	for(int i=0; i<numOfPixels; i++){
		if(fabs(fstInBuf[i]-nodata1)<VERYSMALL || fabs(sndInBuf[i]-nodata2)<VERYSMALL)
			outBuf[i] = nodata1;
		else 
			outBuf[i] = fstInBuf[i]+sndInBuf[i];
	}
	return 0;
}

template<class T1, class T2>
int subtractOverlay(T1* fstInBuf, T2* sndInBuf, T1* outBuf, int nrow, int ncol, T1 nodata1, T2 nodata2){
	if(fstInBuf == NULL) return 1;
	if(sndInBuf == NULL) return 2;
	if(outBuf == NULL) return 3;

	int numOfPixels = nrow*ncol;
	for(int i=0; i<numOfPixels; i++){
		if(fabs(fstInBuf[i]-nodata1)<VERYSMALL || fabs(sndInBuf[i]-nodata2)<VERYSMALL)
			outBuf[i] = nodata1;
		else 
			outBuf[i] = fstInBuf[i]-sndInBuf[i];
	}
	return 0;
}

template<class T1, class T2>
int multiplyOverlay(T1* fstInBuf, T2* sndInBuf, T1* outBuf, int nrow, int ncol, T1 nodata1, T2 nodata2){
	if(fstInBuf == NULL) return 1;
	if(sndInBuf == NULL) return 2;
	if(outBuf == NULL) return 3;

	int numOfPixels = nrow*ncol;
	for(int i=0; i<numOfPixels; i++){
		if(fabs(fstInBuf[i]-nodata1)<VERYSMALL || fabs(sndInBuf[i]-nodata2)<VERYSMALL)
			outBuf[i] = nodata1;
		else 
			outBuf[i] = fstInBuf[i]*sndInBuf[i];
	}
	return 0;
}

template<class T1, class T2>
int divideOverlay(T1* fstInBuf, T2* sndInBuf, T1* outBuf, int nrow, int ncol, T1 nodata1, T2 nodata2){
	if(fstInBuf == NULL) return 1;
	if(sndInBuf == NULL) return 2;
	if(outBuf == NULL) return 3;

	int numOfPixels = nrow*ncol;
	for(int i=0; i<numOfPixels; i++){
		if(fabs(fstInBuf[i]-nodata1)<VERYSMALL || fabs(sndInBuf[i]-nodata2)<VERYSMALL || fabs(sndInBuf[i]-0)<VERYSMALL*VERYSMALL)
			outBuf[i] = nodata1;
		else 
			outBuf[i] = fstInBuf[i]/sndInBuf[i];
	}
	return 0;
}

template<class T1, class T2>
int	unionOverlay(T1* first, T1 lowerLimitFor1, T1 upperLimitFor1, 
			  T2* second, T2 lowerLimitFor2, T2 upperLimitFor2,
			  unsigned char* outBuf, 
			  int nrow, int ncol, int label, T1 nodata1, T2 nodata2){
	if(first == NULL) return 1;
	if(second == NULL) return 2;
	if(outBuf == NULL) return 3;

	int numOfPixels = nrow*ncol;
	T1 l1, u1;
	T2 l2, u2;
	if(lowerLimitFor1<upperLimitFor1){
		l1 = lowerLimitFor1;
		u1 = upperLimitFor1;
	}
	else{
		l1 = upperLimitFor1;
		u1 = lowerLimitFor1;
	}
	if(lowerLimitFor2<upperLimitFor2){
		l2 = lowerLimitFor2;
		u2 = upperLimitFor2;
	}
	else{
		l2 = upperLimitFor2;
		u2 = lowerLimitFor2;
	}

	for(i = 0; i<numOfPixels; i++)
		if(fabs(first[i]-nodata1)<VERYSMALL || fabs(second[i]-nodata2)<VERYSMALL)
			outBuf[i] = nodata1;
		else if((first[i]>=l1 && first[i]<=u1) || (second[i]>=l2 && second[i]<=u2))
			outBuf[i] = label;
		else
			outBuf[i] = 0;

	return 0;
}

#endif
 
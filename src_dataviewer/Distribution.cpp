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
// Distribution.cpp: implementation of the CDistribution class.
//
// Prepared by Rongxun Wang, UW-Madison
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Distribution.h"
#include "math.h"
//#include "global.h"
#include ".\distribution.h"
#include <fstream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDistribution::CDistribution()
{
	int i = 0;
	
	//id = -1;

	number = 0;
	value = NULL;
	bandWidth = 0;

	SetUp();
	/*
	min = 9999;
	max = -9999;
	mean = 0;
	mode = 0;
	stdDev = 0;
	interQuartileRange = 0;
	bandWidth = 0;
	for (i = 0; i < NUMOFPDE; i++){
		PDE[i] = 0;
	}//for(i)
	minPDE = 9999;
	maxPDE = -9999;

	valid = false;
	*/
}


CDistribution::CDistribution(CDistribution & aDistribution)
{
		//number = 0;
	//value = NULL;

//	SetUp();

		int i = 0;

	min = aDistribution.min;
	max = aDistribution.max;
	mean = aDistribution.mean;
	mode = aDistribution.mode;
	stdDev = aDistribution.stdDev;
	interQuartileRange = aDistribution.interQuartileRange;
	bandWidth = aDistribution.bandWidth;
	for (i = 0; i < NUMOFPDE; i++){
		PDE[i] = aDistribution.PDE[i];
	}//for(i)
	minPDE = aDistribution.minPDE;
	maxPDE = aDistribution.maxPDE;

	valid = aDistribution.valid;

	number = aDistribution.number;
	if(aDistribution.value == NULL)
		value = NULL;
	else
	{ 
		value = new float[number];

	    for(i=0;i<aDistribution.number;i++)
	    {
		    value[i] = aDistribution.value[i];
   	    }
	}
}


CDistribution::~CDistribution()
{
	if (value != NULL){
		delete []value;
		value = NULL;
	}//if

	valid = false;
}

void CDistribution::SetValue(int num, float *val)
{
	int i = 0;

	number = num;
	if (value != NULL){
		delete []value;
		value = NULL;
	}//if
	value = new float[number];
	for (i = 0; i < number; i++){
		value[i] = val[i];
	}//for(i)

	SetUp();

	quicksort(value, 0, number - 1);
	min = value[0];
	max = value[number - 1];

	rangeMin = min - 2 * bandWidth;
	rangeMax = max + 2 * bandWidth;
}

float CDistribution::KernelFunction(int kernelType, float z)
{
	float k = 0;

	switch (kernelType){
	case 1:	//Triangular Kernel
		if (fabs(z) <= 1){
			k = (float)(1 - fabs(z));
		}//if
		else{
			k = 0;
		}//else
		break;
	case 2:	//Second Degree Polynomial
		if (fabs(z) <= 1){
			k = (1 - z * z) * 3 / 4;
		}//if
		else{
			k = 0;
		}//else
		break;
	case 3:	//Normal Kernel
		k = (float)(exp(- z * z / 2) / sqrt(2 * PI));
		break;
	default:
		break;
	}//switch

	return k;
}

void CDistribution::EstimatePD()
{
	EstimatePD(rangeMin, rangeMax);
}

void CDistribution::EstimatePD(float theMin, float theMax)
{
	rangeMin = theMin;
	rangeMax = theMax;

	int kernelType = 3;
	double tmpValue = 0;

	//divided by zero consideration
	if (number == 0){
		return;
	}//if

	if (bandWidth < MYZERO)
		bandWidth = getBandWidth(0);

	for (int i = 0; i < NUMOFPDE; i++){
		tmpValue = theMin + (theMax - theMin) * i / (NUMOFPDE - 1);
		for (int j = 0; j < number; j++){
			PDE[i] += KernelFunction(kernelType, (tmpValue -value[j]) / bandWidth);
		}//for(j)
		PDE[i] = PDE[i] / (number * bandWidth);

		if (PDE[i] < minPDE){
			minPDE = PDE[i];
		}//if
		
		if (PDE[i] > maxPDE){
			maxPDE = PDE[i];
			mode = tmpValue;
		}//if
	}//for(i)

	if (fabs(maxPDE - minPDE) > MYZERO){
		valid = true;
	}//if
}

void CDistribution::quicksort(float* data, int l, int r)
{
	//using namespace std;
	//stack<int> s;
	MyStack theStack;
	theStack.num = 0;
	theStack.first = NULL;

	MyStackItem *aStackItem = NULL;

	//s.push(l);
	aStackItem = new (struct MyStackItem);
	aStackItem->value = l;
	aStackItem->next = NULL;
	if (theStack.num == 0){
		theStack.first = aStackItem;
		theStack.num += 1;
	}//if
	else{
		aStackItem->next = theStack.first;
		theStack.first = aStackItem;
		theStack.num += 1;
	}//if

	//s.push(r);
	aStackItem = new (struct MyStackItem);
	aStackItem->value = r;
	aStackItem->next = NULL;
	if (theStack.num == 0){
		theStack.first = aStackItem;
		theStack.num += 1;
	}//if
	else{
		aStackItem->next = theStack.first;
		theStack.first = aStackItem;
		theStack.num += 1;
	}//if

	//while (!s.empty()){
	while (theStack.num != 0){
		//r = s.top();
		//s.pop();
		aStackItem = theStack.first;
		r = aStackItem->value;
		theStack.first = aStackItem->next;
		delete aStackItem;
		theStack.num -= 1;

		//l = s.top();
		//s.pop();
		aStackItem = theStack.first;
		l = aStackItem->value;
		theStack.first = aStackItem->next;
		delete aStackItem;
		theStack.num -= 1;

		if (r <= l)
			continue;
		int i = partition(data, l, r);
		if (l-1 > r-i){
			//s.push(l);
			aStackItem = new (struct MyStackItem);
			aStackItem->value = l;
			aStackItem->next = NULL;
			if (theStack.num == 0){
				theStack.first = aStackItem;
				theStack.num += 1;
			}//if
			else{
				aStackItem->next = theStack.first;
				theStack.first = aStackItem;
				theStack.num += 1;
			}//if

			//s.push(i-1);
			aStackItem = new (struct MyStackItem);
			aStackItem->value = i - 1;
			aStackItem->next = NULL;
			if (theStack.num == 0){
				theStack.first = aStackItem;
				theStack.num += 1;
			}//if
			else{
				aStackItem->next = theStack.first;
				theStack.first = aStackItem;
				theStack.num += 1;
			}//if

			//s.push(i+1);
			aStackItem = new (struct MyStackItem);
			aStackItem->value = i + 1;
			aStackItem->next = NULL;
			if (theStack.num == 0){
				theStack.first = aStackItem;
				theStack.num += 1;
			}//if
			else{
				aStackItem->next = theStack.first;
				theStack.first = aStackItem;
				theStack.num += 1;
			}//if

			//s.push(r);
			aStackItem = new (struct MyStackItem);
			aStackItem->value = r;
			aStackItem->next = NULL;
			if (theStack.num == 0){
				theStack.first = aStackItem;
				theStack.num += 1;
			}//if
			else{
				aStackItem->next = theStack.first;
				theStack.first = aStackItem;
				theStack.num += 1;
			}//if
		}
		else{
			//s.push(i+1);
			aStackItem = new (struct MyStackItem);
			aStackItem->value = i + 1;
			aStackItem->next = NULL;
			if (theStack.num == 0){
				theStack.first = aStackItem;
				theStack.num += 1;
			}//if
			else{
				aStackItem->next = theStack.first;
				theStack.first = aStackItem;
				theStack.num += 1;
			}//if

			//s.push(r);
			aStackItem = new (struct MyStackItem);
			aStackItem->value = r;
			aStackItem->next = NULL;
			if (theStack.num == 0){
				theStack.first = aStackItem;
				theStack.num += 1;
			}//if
			else{
				aStackItem->next = theStack.first;
				theStack.first = aStackItem;
				theStack.num += 1;
			}//if

			//s.push(l);
			aStackItem = new (struct MyStackItem);
			aStackItem->value = l;
			aStackItem->next = NULL;
			if (theStack.num == 0){
				theStack.first = aStackItem;
				theStack.num += 1;
			}//if
			else{
				aStackItem->next = theStack.first;
				theStack.first = aStackItem;
				theStack.num += 1;
			}//if

			//s.push(i-1);
			aStackItem = new (struct MyStackItem);
			aStackItem->value = i - 1;
			aStackItem->next = NULL;
			if (theStack.num == 0){
				theStack.first = aStackItem;
				theStack.num += 1;
			}//if
			else{
				aStackItem->next = theStack.first;
				theStack.first = aStackItem;
				theStack.num += 1;
			}//if
		}
	}
}

int CDistribution::partition(float* data, int l, int r)
{
	//float tmp;
	int i(l-1);
	int j(r);
	float v = data[r];
	while(true){
		while (data[++i] < v);
		while (v < data[--j])
			if (j == l)
				break;
		if (i >= j)
			break;
		exchange(data[i], data[j]);
		/*
		tmp = data[i];
		data[i] = data[j];
		data[j] = tmp;
		*/
	}
	exchange(data[i], data[r]);
	/*
	tmp = data[i];
	data[i] = data[j];
	data[j] = tmp;
	*/

	return i;
}

void CDistribution::exchange(float &a, float &b)
{
	float tmp;
	tmp = a;
	a = b;
	b = tmp;
}

void CDistribution::SetUp(void)
{
	int i = 0;

	min = 9999;
	max = -9999;
	mean = 0;
	mode = 0;
	stdDev = 0;
	interQuartileRange = 0;
	
	for (i = 0; i < NUMOFPDE; i++){
		PDE[i] = 0;
	}//for(i)
	minPDE = 9999;
	maxPDE = -9999;

	valid = false;
}


//only "Rule of Thumb" is implemented currently
float CDistribution::getBandWidth(int bandWidthType)
{
	int i = 0, j = 0;
	double sum = 0;
		double sumsq = 0;

	for (i = 0; i < number; i++){
		sum += value[i];
		sumsq += value[i] * value[i];
	}//for(i)

	mean = (float)sum / number;
	stdDev = (float)sqrt(sumsq / number - mean * mean);

	float tmpQ1 = value[number / 4];
	float tmpQ3 = value[number * 3 / 4];

	interQuartileRange = tmpQ3 - tmpQ1;

	return (float)(1.06 * stdDev * pow((double)number, -1.0/5.0));
}

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
// FCM.h: interface for the FCM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FCM_H__58912666_8A3E_11D4_A0BD_0001031FD0BB__INCLUDED_)
#define AFX_FCM_H__58912666_8A3E_11D4_A0BD_0001031FD0BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include <stdlib.h>
#include <stdio.h>			 
#include <string.h>

//#include <fstream.h>
#include <conio.h>
#include "EnvDatabase.h"

//#define MAX_SAMPLE_PIXELS 600000
#define MAX_VARIABLES_NUM 30
#define MAX_CLASSES_NUM 50

class FCM  
{
public:

	FCM(CEnvDatabase *pEnvDatabase, bool* idBuf, int numOfValidPixel, int ksize, double weight, double EPS, int iteration);
 
	virtual ~FCM();

	typedef struct PixelFeatures
	{
		float *vars;
		//int x;
		//int y;
	};

	PixelFeatures *pixelFeatures; //Y(NS,ND)


	//inialized variables
	double EPS; //EPS
	int NumberOfClasses; //NC
	int MaxIteration; //LMAX
	int DistNorm; //ICON
	double WeightExp; //QQ
	int NumberOfVariables; //ND

	bool* maskBuf;


	int numOfValidPixel;

	//variables
	char inputFile[80];
	char outputFile[80];

	int samplingPixelNum; //NS


	double scalingMatrix[MAX_VARIABLES_NUM][MAX_VARIABLES_NUM]; //CC(ND,ND)




	float **membershipMatrix;
	float **updatedMembershipMatrix;



	double classCentroid[MAX_CLASSES_NUM][MAX_VARIABLES_NUM]; //V(NC,ND)

	double entropy; //H(NC), but it is not array here
	double payoff; //VJM(NC)
	double partitionCoef; //F(NC)
	double entropyBound; //DIF(NC)


	int convergedIterNum;
	double convergedErrMax;

	CEnvDatabase *m_pEnvDatabase;
	//double alpha;

	int ksize;
	double ESP;
	CString strResultDir;




	//Functions


	
	void setResultDir(CString strResultDir);
	//void setAlpha(double dAlpha);


	void SamplePixels();

	void AssignScalingMatrix();


	void CalClassesCentroid();
	void IniMembershipMatrix();
	void UpdateClassCenters();
	void UpdateMemberShip();
	void CalEntropy();

	void WriteClassCentroidFile();

	void WriteMembershipFiles();



	void setNumOfClasses(int numOfClasses);	

	void DestroyMatrix();

	void InitiatePixels();

};

#endif // !defined(AFX_FCM_H__58912666_8A3E_11D4_A0BD_0001031FD0BB__INCLUDED_)

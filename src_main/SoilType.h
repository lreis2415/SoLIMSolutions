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
#pragma once

using namespace std;
#include <vector>
#include "Instance.h"
#include "Occurrence.h"
#include "Exclusion.h"

class CSoilType
{
public:
	CSoilType(void);
	CSoilType(CSoilType &aSoilType);
	~CSoilType(void);

private:
	CString m_strSoilTypeName;  //SoilName
	int m_iSoilTypeID;   //ID
	int m_iInstanceNum;  //instance number
	int m_iOccurrenceNum;  //occurrence number
	int m_iExclusionNum;  //exclusion number
	vector<CInstance *> m_vecInstance;  //vector to store instances
	vector<COccurrence *> m_vecOccurrence;  //vector to store occurrences
	vector<CExclusion *> m_vecExclusion;  //vector to store exclusions
	bool m_bIsUsed;  //indicate if the soil type will be used in inference	

	

public:
    int getInstanceNum();
	int getOccurrenceNum();
	int getExclusionNum();
	CInstance* getInstance(int iIndex);
	COccurrence* getOccurrence(int iIndex);
	CExclusion* getExclusion(int iIndex);
	CString getSoilTypeName();
	void setSoilTypeName(CString strSoilTypeName);
	int getSoilTypeID();
	void setSoilTypeID(int iSoilTypeID);
	bool getIsUsed();
	void setIsUsed(bool bIsUsed);
	bool AddInstance(CInstance* newInstance);
	void DeleteInstance(int iIndex);
    bool AddOccurrence(COccurrence* newOccurrence);
	void DeleteOccurrence(int iIndex);
	bool AddExclusion(CExclusion* newExclusion);
	void DeleteExclusion(int iIndex);

	//integrate instances, occurrences, exclusions to get the result
	void getInferenceResult(float *resultBuf, CEnvDatabase *pEnvDatabase,bool bUseMask, bool *maskBuf, bool bCreateCheckFile,float *chkBuf,CProgressCtrl *ProgressBar,int iCurSoilID, int iSoilNum, double dCellSize);

};

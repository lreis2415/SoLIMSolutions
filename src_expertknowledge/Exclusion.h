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

#include "Rule.h"

#include <vector>
//using namespace std;
#include "Instance.h"

class CExclusion
{
public:
	CExclusion(void);
	CExclusion(CExclusion &anExclusion);
	~CExclusion(void);

private:		
	CString m_strExclusionName; //exclusion name
	int m_iRuleNum;  //rule number
	vector <CRule *> m_vecRule;  //vector to store rules

	double m_dCentralX;
    double m_dCentralY;
	double m_dSearchDist;	
	bool m_bUseDistSim;
	double m_dDistDecayFactor;
	double m_dZFactor;

	double m_dCentralSim;
	bool m_bInRange;


public:
    int getRuleNum();  
	CRule* getRule(int nIndex);
	CString getExclusionName();
	void setExclusionName(CString strExclusionName);
	double getCentralX();
	void setCentralX(double dCentralX);
	double getCentralY();
	void setCentralY(double dCentralY);
	double getImpactFactor();
	void setImpactFactor(double dImpactFactor);
	double getSearchDist();
	void setSearchDist(double dSearchDist);
	bool getUseDistSim();
	void setUseDistSim(bool bUseDistSim);
    double getDistDecayFactor();
	void setDistDecayFactor(double dDistDecayFactor);
	double getZFactor();
	void setZFactor(double fZFactor);	
	void setInRange(bool bInRange);
	bool getInRange();
	void getCentralSim();	



    bool AddRule(CRule* pNewRule);
	void DeleteRule(int nIndex);
	bool IsInRange(int iRow,int iCol,CEnvDatabase *pEnvDatabase,double *dDist ); 
	//double distSimilarity(double dSim,double dDist,double dSearchDist,double dDistDecayFactor);
	void ComputeCentralPointResult(CEnvDatabase *pEnvDatabase);
	double  topoDistanceXY(double dX1, double dY1, double dX2, double dY2,CThreeDR *theDEM, 
								 double dZFactor, double dCellFraction=1);
	//get the final inference result of this exclusion
	//double getExclusionResult(double dX,double dY,CEnvDatabase *pEnvDatabase,int *iRuleID,double dDist );

	double getExclusionResult(double dX,double dY,CEnvDatabase *pEnvDatabase,int *iRuleID,double dDist, vector<CInstance *> vecInstance, int iInstNum);
	double distSimilarity(double dSim,double dDist,double dBoundSim, double dBoundDist,double dDistDecayFactor);


	

};

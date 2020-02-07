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
#include "EnvLayer.h"
#include "EnvDatabase.h"

//base class for all kinds of rules
class CRule
{
public:
	CRule(void);
	~CRule(void);


protected:
	CString m_strRuleName;      //rule name
	bool  m_bIsUsed;             //if the rule will take part in inference process
	int m_iRuleType;  //rule type  0.Range Rule 1.Curve Rule 2.Word Rule 3.Point Rule 4.Enum Rule
	CString m_strLayerName;  //the name of attached layer
	double m_dDispMax;     //the max display range of graph
	double m_dDispMin;    //the min display range of graph
	double m_dCurGraphMax;  //the max range of graph
	double m_dCurGraphMin;  //the min range of graph
	int m_iLayerID;   //the id of the attached layer
 
public:
	CString getRuleName();
	void setRuleName(CString strRuleName);
	int getRuleType();
	void setRuleType(int iRuleType);
	bool getIsUsed();
	void setIsUsed(bool bIsUsed);
	CString getLayerName();
	void setLayerName(CString strLayerName);
	double getDispMin();
	void setDispMin(double dDispMin);
	double getDispMax();
	void setDispMax(double dDispMax);
	double getCurGraphMin();
	void setCurGraphMin(double dCurGraphMin);
	double getCurGraphMax();
	void setCurGraphMax(double dCurGraphMax);	
	void setLayerID(int ID);
	int getLayerID();


	virtual double Evaluate(float fEnvValue)  {return 1;}  //virtual function, caculate the result(get fuzzy membership)
	virtual void DrawGraph(int cx, int cy, CPaintDC *dc){} //virtual function, draw the graph of this rule
	virtual double ComputeRuleGraphMin(){return 0;}  //virtual function, compute the min range of graph
	virtual double ComputeRuleGraphMax(){return 100;} //virtual function, compute the max range of graph
	virtual int ComputeScreenCoordX(double x, int cx){return 0;}  //virtual function, compute corresponding screen coordinate of double x
	virtual int ComputeScreenCoordY(double y, int cy){return 0;}
	virtual double ComputeGraphCoordX(int x, int cx){return 0;} //virtual function, compute corresponding graph coordinate of int x
	virtual double ComputeGraphCoordY(int y, int cy){return 0;}
	virtual bool IsLegal(){return false;};  //virtual function, judge if the rule is legal
	double getEvaluateResult(int iRow, int iCol,CEnvDatabase *pEnvDatabase);  //get the evaluation result of the rule

};

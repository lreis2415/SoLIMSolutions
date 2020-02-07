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
#include<vector>
#include <string>
#include "CBRUtility.h"
using namespace std;

class CBRSamplePoint
{
public:
	CBRSamplePoint(void);
	~CBRSamplePoint(void);

public:
	//set attribute
	void setid( int aID) {id = aID;}
	void setX( double ax) {x = ax;}
	void setY(double ay) {y = ay;}

	static void addAttrName(string valueName) { valuesNameVec.push_back(valueName); }
	static void addClimateName(string climateName) { climateNameVec.push_back(climateName); }
	static void addParentName(string parentName) {parentNameVec.push_back(parentName); }
	static void addTopoName(string topoName) { topoNameVec.push_back(topoName); }
	static void addVegName(string vegName) {vegNameVec.push_back(vegName);}

	void addAttrVar(float val) { valuesVec.push_back(val); }
	void addClimateVar(float climateVar) { climateVarVec.push_back(climateVar); }
	void addParentVar(float parentVar) { parentVarVec.push_back(parentVar); }
	void addTopoVar(vector<Point>* topoVar) { topoVarVec.push_back(topoVar); }
	void addTopoVarNoKde(float topoVar) {topoVarVecNoKde.push_back(topoVar);}
	void addVegVar(vector<Point>* vegVar) { vegVarVec.push_back(vegVar); }
	void addVegVarNoKde(float vegVar) {vegVarVecNoKde.push_back((vegVar));}

	//get attribute
	int getid() {return id;}
	double getX() {return x;}
	double getY() {return y;}

	static const string getAttrName(int i) {return valuesNameVec[i];}
	static const string getClimateName(int i) {return climateNameVec[i];}
	static const string getParentName(int i) {return parentNameVec[i];}
	static const string getTopoName(int i) {return topoNameVec[i];}
	static const string getVegName(int i) {return vegNameVec[i];}

	int getAttrCount() { return valuesVec.size(); }
	float getAttribute(int i) {return this->valuesVec[i];}
	float getClimateVariable(int i) {return climateVarVec[i];}
	float getParentVariable(int i) {return parentVarVec[i];}
	vector<Point>*  getTopoVariable(int i) {return topoVarVec[i];}
	vector<Point>* getVegVarVariable(int i) {return vegVarVec[i];}
	float getTopoVariableNoKde(int i) {return topoVarVecNoKde[i];}
	float getVegVariableNoKde(int i) {return vegVarVecNoKde[i];}

private:
	//basic fields
	int id; //id
	double x; //x_position
	double y; //y_position
	
	vector<float> valuesVec; //value
	
	//Environmental variables fields
	//变量首字母最好小写，类名首字母大写
	vector<float> climateVarVec;
	vector<float> parentVarVec;
	vector<float> topoVarVecNoKde;
	vector<float> vegVarVecNoKde;
	vector< vector<Point>* > topoVarVec;
    vector< vector<Point>* > vegVarVec;
public:
	// names of the attributes and values
	static vector<string> valuesNameVec;
	static vector<string> climateNameVec;
	static vector<string> parentNameVec;
	static vector<string> topoNameVec;
	static vector<string> vegNameVec;
};

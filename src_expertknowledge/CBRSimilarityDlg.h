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
#include "afxwin.h"
#include "ThreeDR.h"
#include "CBRSamplePoint.h"

#include <string>
#include <vector>
#include "afxcmn.h"

// CBRSimilarityDlg 对话框

class CBRSimilarityDlg : public CDialog
{
	DECLARE_DYNAMIC(CBRSimilarityDlg)

public:
	CBRSimilarityDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBRSimilarityDlg();

// 对话框数据
	enum { IDD = IDD_CBR_SIMILAR };

	void getAttrNameVector(ifstream& fIn, vector<string>& attrNameVec);
	//计算相似度的函数
	void getSampleVec();
	int getVarVec(vector<CString>& filenames, vector<CThreeDR*>& vec3dr);
	int calcParentSimilarity(vector<CThreeDR*>& in_parentVarVec, vector<CBRSamplePoint*>& in_sampleVec);
	int calcIntergratedSimilarity(int iChoice);
	void limitingIntegrate(float* integratedsimi_tempsample, int i, int j, 
		vector<CThreeDR*>& in_climateSimiVec, vector<CThreeDR*>& in_parentSimiVec, 
		vector<CThreeDR*>& in_topoSimiVec, vector<CThreeDR*>& in_vegSimiVec);
	void ruleIntegrate(float* integratedsimi_tempsample, int i, int j, 
		vector<CThreeDR*>& in_climateSimiVec, vector<CThreeDR*>& in_parentSimiVec, 
		vector<CThreeDR*>& in_topoSimiVec, vector<CThreeDR*>& in_vegSimiVec);
	void averIntegrate(float* integratedsimi_tempsample, int i, int j, 
		vector<CThreeDR*>& in_climateSimiVec, vector<CThreeDR*>& in_parentSimiVec, 
		vector<CThreeDR*>& in_topoSimiVec, vector<CThreeDR*>& in_vegSimiVec);
	int calcTopoSimilarity(vector<CString>& topoFiles, vector<CBRSamplePoint*>& in_sampleVec);
	int calcKdeSimilarity(CString type, vector<CString>& kdeFiles, vector<CBRSamplePoint*>& in_sampleVec, vector<CString>& simiVec);
	int calcGowerSimilarity(CString type, vector<CThreeDR*>& vec3dr, vector<CBRSamplePoint*>& in_sampleVec, vector<CString>& vecOuputSimi);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnRun();
	DECLARE_MESSAGE_MAP()

private:
	//combox中的文本
	CString climSimiRule;
	CString parentSimiRule;
	CString topoSimiRule;
	CString intergratedSimiRule;
	//要输入的气候要素的文件名，以2个为例

	//存储环境要素
	vector<CThreeDR*> climateVarVec; 
	vector<CThreeDR*> parentVarVec; 
	vector<CThreeDR*> topoVarVec;
	vector<CThreeDR*> vegVarVec;

	//存储相似度结果
	vector<CString> climateSimiVec;
	vector<CString> parentSimiVec; 
	vector<CString> topoSimiVec; 
	vector<CString> vegSimiVec;

private:
	CEdit climateCutCtrl;
	CEdit climateWeightCtrl;
	CEdit topoWeightCtrl;
	CEdit vegWeightCtrl;
	CProgressCtrl processCtrl;
	CComboBox comboVegVarCtrl;
	
	double climateCut;
	double climateWeight;
	double topoWeight;
	double vegWeight;

	CString outputFolder;
	//double nProcess;
	int varCount;

	double curProcess;

	CComboBox comboClimateVarCtrl;
	CComboBox comboPmVarCtrl;
	CComboBox comboTopoVarCtrl;
	afx_msg void OnBnClickedButton1();
	CComboBox comboCaseMethod;
	afx_msg void OnCbnSelchangeComboCase();
	
public:
	afx_msg void OnBnClickedBack();
	afx_msg void OnBnClickedCancel();
	void resetProcess();
	void fillKdeCombo();
};

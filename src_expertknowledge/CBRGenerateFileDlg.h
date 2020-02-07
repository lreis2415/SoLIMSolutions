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
#include <vector>
#include <fstream>
#include "afxcmn.h"
#include "SoLIM.h"

// CBRGenerateFileDlg 对话框

class CBRGenerateFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CBRGenerateFileDlg)

public:
	CBRGenerateFileDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBRGenerateFileDlg();

// 对话框数据
	enum { IDD = IDD_CBR_GEN_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox comboGridSize;
	virtual BOOL OnInitDialog();

private:
	int testKDE(void);
	int testThreeDR(void);
	int calKDE(CThreeDR* src3dr, CThreeDR* dst3dr, char* outputfile);
	CString GetFilePath();

	std::vector< CThreeDR* > climateDataVec, materialDataVec, topoDataVec, vegDataVec;
	float biggestRes;
	float dstRes;
	int biggestResIndex;

public:
	afx_msg void OnCbnSelchangeComboGridsize();
	afx_msg void OnBnClickedOk();
	void init();
private:
	CString outputFolder;
	int readThreeDRData(char* type, int curIndex, vector<CString>& inputfileVec, vector<CThreeDR*>& threeDRVec);
	bool generateNew3dr(char* type, vector<CString>& inputfileVec, vector<CThreeDR*>& threeDRVec, 
		vector<CString>& g_vecOutput);
	bool copy3dr(void);
	void saveFileName(vector<CString> vecFileName, CString outputfile);
private:
	CProgressCtrl processCtrl;
	CStatic statusCtrl;
	CString resValue;
	//ofstream testOf;

	BOOL radioSingle;
	BOOL radioKde;

	CButton radioSingleCtrl;
	CButton radioKdeCtrl;
	CButton radioSingleCtrlVeg;
	CButton radioKdeCtrlVeg;
	int inputCount;
	int process;

	afx_msg void OnBnClickedRadioSingleValue();
	afx_msg void OnBnClickedRadioKde();
	afx_msg void OnBnClickedRadioSingleValueVegetation();
	afx_msg void OnBnClickedRadioKdeVegetation();
public:
	afx_msg void OnBnClickedBack();
	afx_msg void OnBnClickedCancel();
	void resetProcess();
};

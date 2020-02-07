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
#include "dbfopen.h"

// CBRSampleDlg 对话框

class CBRSampleDlg : public CDialog
{
	DECLARE_DYNAMIC(CBRSampleDlg)

public:
	CBRSampleDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBRSampleDlg();

// 对话框数据
	enum { IDD = IDD_CBR_SAMPLE };

private:
	CListCtrl m_listctrlSampleData;
	CString GetFilePath();
	void generateAttributeFiles();
	CString generateFiles(CString type, vector<CString>& vecFiles);
	void generateKdeFiles(CString type, vector<CString>& vecOriginalFile, vector<CString>& vecSample);

	
	CString generateFilesForType(CString type, CString typeName, vector<CString>& vecFiles);
	void generateKdeFilesForType(CString type, CString typeName,vector<CString>& vecOriginalFile, vector<CString>& vecSample);
	void generateAttributeFilesForType();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonGetSample();
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonClearSapmleList();
	afx_msg void OnBnClickedOk();

	afx_msg void OnBnClickedButtonAddfield();
	afx_msg void OnBnClickedButtonDeltefield();

private:
	DBFHandle hDBF;
	CComboBox comboFieldX;
	CComboBox comboFieldY;
	// the index of x and y coordinates in the listctrl
	int xIndex, yIndex;
	CString xFieldName;
	CString yFieldName;
	CListBox listAllField;
	CListBox listSelField;
	afx_msg void OnBnClickedButtonAddField();
	afx_msg void OnBnClickedButtonAddField2();

	
	CListBox listAllType;
	CListBox listSelType;


	int typeFieldIndex;

	CString strOutputPath;

	//ofstream testof;
public:
	afx_msg void OnBnClickedBack();
	afx_msg void OnBnClickedCancel();


	afx_msg void OnBnClickedButtonAddType();
	afx_msg void OnBnClickedButtonAddType2();
	CComboBox comboTypeField;
	afx_msg void OnCbnSelchangeComboTypeField();
	CString typeFieldName;

	afx_msg void OnBnClickedRadioType();
	afx_msg void OnBnClickedRadioProperty();
	CButton radioPropertyCtrl;
	CButton radioTypeCtrl;
	CButton m_radioProperty;
	CButton m_radioType;
};



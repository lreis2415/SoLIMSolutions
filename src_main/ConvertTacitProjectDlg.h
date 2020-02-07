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
#include "GetDirDlg.h"

// CConvertTacitProjectDlg dialog

class CConvertTacitProjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CConvertTacitProjectDlg)

public:
	CConvertTacitProjectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConvertTacitProjectDlg();

// Dialog Data
	enum { IDD = IDD_ConvertTacitProjectDlg};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	public:
	CString m_LabelFileName;
	CString m_PntFileName;
	CString m_CellName;
	CString m_OldPrjDir;
	CString m_NewPrjDir;
	CString m_NewPrjName;


	
	
	CGetDirDlg m_getDirDlg1;
	CGetDirDlg m_getDirDlg2;


	afx_msg void OnBnClickedLabelfilebtn();
	afx_msg void OnBnClickedPntfilebtn();
	afx_msg void OnBnClickedCellnamebtn();
	afx_msg void OnBnClickedOk();

	
	void ClearFields();	
	
	afx_msg void OnBnClickedOlddirbtn();
	afx_msg void OnBnClickedNewdirbtn();


	afx_msg void OnBnClickedCancel();


	virtual BOOL OnInitDialog();
		


};

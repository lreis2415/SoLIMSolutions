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
#if !defined(AFX_SHOULDERDLG_H__C3A47E33_BB38_4883_A00E_D2134E54CCFF__INCLUDED_)
#define AFX_SHOULDERDLG_H__C3A47E33_BB38_4883_A00E_D2134E54CCFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShoulderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShoulderDlg dialog

class CShoulderDlg : public CDialog
{
// Construction
public:
	CShoulderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShoulderDlg)
	enum { IDD = IDD_ShoulderDlg };
	CProgressCtrl	m_ProgressBar;
	CString	m_OutputFileName;
	CString	m_PlanformFileName;
	float	m_PlanformWgt;
	CString	m_ProfileFileName;
	float	m_ProfileWgt;
	CString	m_TerrainPartitionFileName;
	float	m_TopPercent;
	float	m_MinArea;
	CString	m_SlopeFileName;
	float	m_SlopeThresh;
	CString	m_RidgeFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShoulderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShoulderDlg)
	afx_msg void OnOutputFileNameBtn();
	virtual void OnOK();
	afx_msg void OnPlanformBtn();
	afx_msg void OnProfileBtn();
	afx_msg void OnTerrainPartitionFileBtn();
	afx_msg void OnSlopeFileBtn();
	afx_msg void OnRidgeFileBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOULDERDLG_H__C3A47E33_BB38_4883_A00E_D2134E54CCFF__INCLUDED_)

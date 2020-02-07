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
#if !defined(AFX_SAMPLEDLG_H__46AA20CB_43D4_4512_A796_BF9AF74C1366__INCLUDED_)
#define AFX_SAMPLEDLG_H__46AA20CB_43D4_4512_A796_BF9AF74C1366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SampleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSampleDlg dialog

class CSampleDlg : public CDialog
{
// Construction
public:
	CSampleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSampleDlg)
	enum { IDD = IDD_SamplingDlg };
	int		m_SampleMethod;
	BOOL	m_SubNoOpt;
	BOOL	m_RangeOpt;
	int		m_SubNo;
	BOOL	m_TotalNoOpt;
	int		m_TotalNo;
	double	m_HorizontalRange;
	double	m_VerticalRange;
	double	m_Xmax;
	double	m_Xmin;
	double	m_Ymax;
	double	m_Ymin;
	CString	m_NumberFileName;
	CString	m_RegionFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSampleDlg)
	virtual void OnOK();
	afx_msg void OnRandomRdo();
	afx_msg void OnSystematicRdo();
	afx_msg void OnStratifiedRdo();
	afx_msg void OnNoInSubChk();
	afx_msg void OnRangeChk();
	afx_msg void OnTotalNoChk();
	virtual BOOL OnInitDialog();
	afx_msg void OnNumberFileBtn();
	afx_msg void OnRegionFileBtn();
	afx_msg void OnRegionalRandomRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void systematicOption();
	void stratifiedOption();
	void rangeOption();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAMPLEDLG_H__46AA20CB_43D4_4512_A796_BF9AF74C1366__INCLUDED_)

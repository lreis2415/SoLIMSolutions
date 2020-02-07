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
#if !defined(AFX_HEADWATERDLG_H__76807B45_9D90_43D1_9FB0_FF7464F260DE__INCLUDED_)
#define AFX_HEADWATERDLG_H__76807B45_9D90_43D1_9FB0_FF7464F260DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HeadwaterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHeadwaterDlg dialog

class CHeadwaterDlg : public CDialog
{
// Construction
public:
	CHeadwaterDlg(CWnd* pParent = NULL);   // standard constructor
	enum { IDD = IDD_HeadwaterDlg };

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeadwaterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHeadwaterDlg)
	afx_msg void OnFlowDirBtn();
	afx_msg void OnStreamFileBtn();
	afx_msg void OnProfileBtn();
	afx_msg void OnPlanformBtn();
	afx_msg void OnOutputFileNameBtn();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CProgressCtrl	m_ProgressBar;
	CString	m_FlowDirFileName;
	CString	m_StreamFileName;
	CString	m_ProfileFileName;
	CString	m_PlanformFileName;
	CString	m_OutputFileName;
	int m_StreamOrder;
	float	m_PlanformWgt;
	float	m_ProfileWgt;
	BOOL m_NodeType;
	int m_UpSearchRange;
	int m_DownSearchRange;
	CString m_UDAFileName;
public:
	afx_msg void OnBnClickedFlowaccfilebtn();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEADWATERDLG_H__76807B45_9D90_43D1_9FB0_FF7464F260DE__INCLUDED_)

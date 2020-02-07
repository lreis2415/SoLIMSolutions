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

#include "RangeSlider.h"
#include "afxwin.h"

#include "GraphPanel.h"


// CWordPage dialog

class CWordPage : public CDialog
{
	DECLARE_DYNAMIC(CWordPage)

public:
	CWordPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWordPage();

	// Dialog Data
	enum { IDD = IDD_DIALOG_WORD };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	LRESULT OnRangeChange(WPARAM wParam, LPARAM /* lParam */);

public:

	CButton m_chkUse;
	CStatic m_staticLayer;
	CStatic m_staticRangeMin;
	CStatic m_staticRangeMax;
	CRangeSlider m_sliderRange;
	CButton m_buttonChooseLayer;
	CEdit m_editLayerName;
	CButton m_buttonMinLeft;
	CButton m_buttonMinRight;
	CButton m_buttonMaxLeft;
	CButton m_buttonMaxRight;
	CStatic m_staticRuleType;
	CGraphPanel m_pGraphPanel;

	CComboBox m_comboCurveName;	
	CComboBox m_comboEnvAttri;
	CButton m_btnChangeToFreehandRule;
	CEdit m_editLibFileName;
	CButton m_buttonChooseLibFile;
	CStatic m_staticLibFile;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void EnablePage(bool bIsEnable);
	void FillPage();
	afx_msg void OnBnClickedCheckUse();
	afx_msg void OnBnClickedButtonBrowseLibfile();
	virtual BOOL PreTranslateMessage( MSG* pMsg );
	afx_msg void OnBnClickedButtonBrowseLayerFile();
	afx_msg void OnBnClickedButtonMinLeft();
	afx_msg void OnBnClickedButtonMinRight();
	afx_msg void OnBnClickedButtonMaxLeft();
	afx_msg void OnBnClickedButtonMaxRight();

	afx_msg void OnCbnSelchangeComboCurveName();
	afx_msg void OnBnClickedButtonChangetocurverule();
	afx_msg void OnCbnSelchangeComboEnvAttri();
};

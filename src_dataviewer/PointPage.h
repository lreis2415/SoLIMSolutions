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


// CPointPage dialog

class CPointPage : public CDialog
{
	DECLARE_DYNAMIC(CPointPage)

public:
	CPointPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPointPage();

	// Dialog Data
	enum { IDD = IDD_DIALOG_POINT };	

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
	double m_dCurGraphMin;
	double m_dCurGraphMax;
	CStatic m_staticRuleType;
    CGraphPanel m_pGraphPanel;
	CButton m_buttonChooseLayer;
	CEdit m_editLayerName;
	CButton m_buttonMinLeft;
	CButton m_buttonMinRight;
	CButton m_buttonMaxLeft;
	CButton m_buttonMaxRight;

	CStatic m_staticCentralX;
	CEdit m_editCentralX;
	CStatic m_staticCentralY;
	CEdit m_editCentralY;
	CStatic m_staticLWidth;
	CEdit m_editLWidth;
	CStatic m_staticRWidth;
	CEdit m_editRWidth;
	CComboBox m_comboCurveType;
	CStatic m_staticCurveType;
	//CButton m_btn3DMapper;
	CButton m_buttonUpdateRule;

		CButton m_btnChangeToFreehandRule;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void EnablePage(bool bIsEnable);
	void FillPage();
	afx_msg void OnBnClickedCheckUse();
	virtual BOOL PreTranslateMessage( MSG* pMsg );
	afx_msg void OnEnKillfocusEditCentralX();
	afx_msg void OnEnKillfocusEditCentralY();
	afx_msg void OnEnKillfocusEditLWidth();
	afx_msg void OnEnKillfocusEditRWidth();
	afx_msg void OnCbnSelchangeComboCurveType();
	afx_msg void OnBnClickedButtonBrowseLayerFile();
	afx_msg void OnBnClickedButtonMinLeft();
	afx_msg void OnBnClickedButtonMinRight();
	afx_msg void OnBnClickedButtonMaxLeft();
	afx_msg void OnBnClickedButtonMaxRight();
	//afx_msg void OnBnClickedButton3dmapper();
	afx_msg void OnBnClickedButtonUpdateRule();
	afx_msg void OnBnClickedButtonChangetocurverule();
};

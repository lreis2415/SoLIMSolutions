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
#include "afxcmn.h"


#include "TimePhasePage.h"

#include <vector>
#include <algorithm>

#include <sstream>
#include <map>
using namespace std;


// CRemoteSensingDlg 对话框

class CRemoteSensingDlg : public CDialog
{
	DECLARE_DYNAMIC(CRemoteSensingDlg)

public:
	CRemoteSensingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRemoteSensingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_REMOTESENSING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSetNumTimePhases();
	
	CTabCtrl m_tabDays;
	virtual BOOL OnInitDialog();

   CTimePhasePage m_page;
   
   int m_iNumOfDays;

   int m_iNumOfBands;

	vector<int> m_daysVec;

    vector<int > m_bandsVec;

	vector<vector<CString> > m_dataFileNameVec;

	vector<vector<CString> > m_dataPathVec;


   CEnvDatabase *m_pData;
   afx_msg void OnTcnSelchangeTabDays(NMHDR *pNMHDR, LRESULT *pResult);

   int m_curSelTab;
   afx_msg void OnBnClickedButtonSelectFolder();
   CString m_strResultFolder;
   afx_msg void OnBnClickedOk();

   void DWT_Once(short** spOriginData, short** spTransData0, short** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H);

};

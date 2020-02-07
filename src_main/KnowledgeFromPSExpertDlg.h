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

#include "GridCtrl_src\GridCtrl.h"

#include "EnvDatabase.h"
#include "KnowledgeBase.h"


#include "FieldSample.h"



// CKnowledgeFromPSExpertDlg ¶Ô»°¿ò

class CKnowledgeFromPSExpertDlg : public CDialog
{
	DECLARE_DYNAMIC(CKnowledgeFromPSExpertDlg)

public:
	CKnowledgeFromPSExpertDlg(CWnd* pParent = NULL);   
	virtual ~CKnowledgeFromPSExpertDlg();

	enum { IDD = IDD_DIALOG_KNOWLEDGE_FROM_PURPOSIVE_SAMPLES_NEED_SPATIAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrl m_gridPatternTable;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	//afx_msg void OnBnClickedButtonAddSoil();
	//afx_msg void OnBnClickedButtonRemoveSoil();
	CListCtrl m_SoilSequenceList;
	afx_msg void OnBnClickedButtonAddLayer();
	afx_msg void OnBnClickedButtonRemoveLayer();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();

	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);


	CListCtrl m_LayerList;
	CString m_listFileName;	
	CEnvDatabase* m_pEnvDatabase;	


	int m_iNumOfPatterns;
	int m_curRow;
	int m_curCol;

	vector<vector<FieldSample> > m_vecPatternSamples;
	vector<CString> m_vecSoilSequence;
	
	vector<CString> m_vecPatternIDs;
	vector<CString> m_vecSoilTypes;
	vector<CString> m_vecStabilities;

	int m_iSoilNum;
	afx_msg void OnBnClickedButtonLoadPattern();
	//afx_msg void OnBnClickedButtonDeletePattern();
	//afx_msg void OnBnClickedButtonInputSample();
	afx_msg void OnBnClickedButtonKbfile();
	CString m_strKBFile;

	void CKnowledgeFromPSExpertDlg::SaveKnowledgeFile(CString strKBFile, CKnowledgeBase *pKnowledgeBase);
};

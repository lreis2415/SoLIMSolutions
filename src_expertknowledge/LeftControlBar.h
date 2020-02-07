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
// LeftControlBar.h: interface for the CLeftControlBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUIFOLDERVISIO_H__1FF06CA7_9692_4721_AA4A_39B001F58236__INCLUDED_)
#define AFX_GUIFOLDERVISIO_H__1FF06CA7_9692_4721_AA4A_39B001F58236__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GuiControlBar.h"
#include "SoLIMDoc.h"
#include "Rule.h"
#include "RangeWizard.h"
#include "FreehandWizard.h"
#include "EnumWizard.h"
#include "WordWizard.h"
#include "PointWizard.h"

class CLeftControlBar : public CGuiControlBar  
{
public:
	CGuiContainer   m_ctServer; //the container
	CTreeCtrl m_trProjectTree;  //the tree
	CImageList	m_imgListProject;  //the image list

	//different nodes of the tree
	HTREEITEM m_hKnowledge;


	HTREEITEM m_hPreviousItem;

    //current soil, instance, occurrence, exclusion, rul
	int m_iCurSoil;
	int m_iCurInstance;
	int m_iCurOccurrence;
	int m_iCurExclusion;
	int m_iCurRule;

	bool m_bRightClick ;  //if the mouse is right clicked
	bool m_bHasNewRule;  //if a new rule is added

	CRule *m_pCurRule;  //current rule pointer
	CRule *m_pClipBoardRule;  //a rule in the clip board (used for copy/paste)


public:
	CLeftControlBar();
	virtual ~CLeftControlBar();
	void CleanMemory();

public:	
	void FillProjectTree(CSoLIMDoc *pDoc);

	void FillKnowledgeTree(CSoLIMDoc *pDoc);

	int getSelectedTreeLevel(CTreeCtrl &m_theTree,  HTREEITEM hItem);
	void ExpandNode(int iNodeType);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnTvnSelchangedProjectTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRClickProjectTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);	

	afx_msg void OnRuleDelete();
	afx_msg void OnInstanceDelete();
	afx_msg void OnOccurrenceDelete();
	afx_msg void OnExclusionDelete();
	afx_msg void OnSoiltypeDelete();
	afx_msg void OnAddruleFreehandrule();
	afx_msg void OnAddruleRangerule();
	afx_msg void OnAddrulePointrule();
	afx_msg void OnAddruleWordrule();
	afx_msg void OnAddruleEnumrule();
	afx_msg void OnInstanceRename();
	afx_msg void OnExclusionRename();
	afx_msg void OnOccurrenceRename();
	afx_msg void OnSoiltypeRename();
	afx_msg void OnRuleRename();
	afx_msg void OnKnowledgebaseAddsoiltype();
	afx_msg void OnInstancesAddinstance();
	afx_msg void OnExclusionsAddexclusion();
	afx_msg void OnOccurrencesAddoccurrence();

	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnRuleCopy();
	afx_msg void OnInstancePaste();
	afx_msg void OnOccurrencePaste();
	afx_msg void OnExclusionPaste();

};

#endif // !defined(AFX_GUIFOLDERVISIO_H__1FF06CA7_9692_4721_AA4A_39B001F58236__INCLUDED_)

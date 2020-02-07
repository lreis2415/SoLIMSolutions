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
	// SoLIMDoc.h : interface of the CSoLIMDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUIVISIODEMODOC_H__E2383086_C2A3_4CA4_ACE4_44C37E821D47__INCLUDED_)
#define AFX_GUIVISIODEMODOC_H__E2383086_C2A3_4CA4_ACE4_44C37E821D47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Project.h"

#include "ConvertTacitProjectDlg.h"
#include "ConvertOldProjectDlg.h"
#include "ConvertSIEDlg.h"

class CSoLIMDoc : public CDocument
{
protected: // create from serialization only
	CSoLIMDoc();
	DECLARE_DYNCREATE(CSoLIMDoc)

	// Attributes
public:

	// Operations
public:
	CProject *m_pProject;  //base project
	CProject *m_pProjectEdit;  //project being edited
	CString m_strProjectDir;  //project directory(without project name)
	CString m_strProjectFileName;  //project name



	CString m_strProjectFullPath;






	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoLIMDoc)
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CSoLIMDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	//{{AFX_MSG(CSoLIMDoc)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	void NewProject(CString strProjectDir, CString strProjectName);
	void ReadProject(CString strFileName);
	void WriteProject(CString strFileName,CProject *pProject);
	void CleanMemory();
	void OpenMruFile(int nMru);
	void CopyProject(CProject *pProjectFrom, CProject *pProjectTo,bool bIsEditVersion);

   // void ConvertSoLIM2Project(CString projectFile,CString newProjectName, CString newProjectDir);

    afx_msg void OnProjectNew();
	afx_msg void OnProjectOpen();
	afx_msg void OnProjectSave();
	afx_msg void OnUpdateProjectSave(CCmdUI *pCmdUI);
	afx_msg void OnProjectSaveAs();
	afx_msg void OnUpdateProjectSaveAs(CCmdUI *pCmdUI);

	//these functions are for recent files
	afx_msg void OnRecentfiles1();
	afx_msg void OnRecentfiles2();
	afx_msg void OnRecentfiles3();
	afx_msg void OnRecentfiles4();
	afx_msg void OnRecentfilesClearrecentfilelist();
	afx_msg void OnUpdateRecentfilesClearrecentfilelist(CCmdUI *pCmdUI);


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIVISIODEMODOC_H__E2383086_C2A3_4CA4_ACE4_44C37E821D47__INCLUDED_)

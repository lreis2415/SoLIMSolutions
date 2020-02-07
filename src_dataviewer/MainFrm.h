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
// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__57C35901_B6A0_45DC_9229_2697767F4931__INCLUDED_)
#define AFX_MAINFRM_H__57C35901_B6A0_45DC_9229_2697767F4931__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LeftControlBar.h"
#include "GuiFrameWnd.h"
#include "splitterTabWnd.h"



class CMainFrame : public CGuiFrameWnd  //parent class is from GuiLib
{	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
	CLeftControlBar m_leftBar;  //left control panel
	CSplitterTabWnd m_splitterTabWnd;  //used to split window
	CStatusBar  m_wndStatusBar;  //status bar
	HMENU getMenuHandle();  //get menu handle, this function is used in realizing recent file menu
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
		protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);  //control the size of the window so it will not become so small
	afx_msg void OnViewToolbar();	
	afx_msg void OnViewProjectTree();
	afx_msg void OnUpdateViewProjectTree(CCmdUI *pCmdUI);


	//navigation
	afx_msg void OnNavigationZoomin();
	afx_msg void OnNavigationZoomout();
	afx_msg void OnNavigationPan();
	afx_msg void OnNavigationRectZoom();
	afx_msg void OnNavigationEntireLayer();
	afx_msg void OnUpdateNavigationZoomin(CCmdUI *pCmdUI);
	afx_msg void OnUpdateNavigationZoomout(CCmdUI *pCmdUI);
	afx_msg void OnUpdateNavigationPan(CCmdUI *pCmdUI);
	afx_msg void OnUpdateNavigationRectZoom(CCmdUI *pCmdUI);
	afx_msg void OnUpdateNavigationEntireLayer(CCmdUI *pCmdUI);

	//functions and utilities
    
	afx_msg void OnValidationErrorMatrixWithMap();
	afx_msg void OnValidationErrorMatrixWithPoints();

	afx_msg void OnProductderivationPropertymap();
	afx_msg void OnProductderivationHardenFuzzymaps();
	afx_msg void OnDataPreparationDemfillandshave();
	afx_msg void OnDataPreparationSurfacederivatives();
	afx_msg void OnDataPreparationCatchment();
	afx_msg void OnDataPreparationFlowaccumulationSingle();
	afx_msg void OnDataPreparationFlowaccumulationMultiple(); 
	afx_msg void OnDataPreparationFlowdirection();
	afx_msg void OnDataPreparationHeadwater();
	afx_msg void OnDataPreparationRidgeline();
	afx_msg void OnDataPreparationBroadnarrowridgeline();
	afx_msg void OnDataPreparationStreamline();
	afx_msg void OnDataPreparationShoulder();
	afx_msg void OnDataPreparationBack();
	afx_msg void OnDataPreparationTerrainpartition();
	afx_msg void OnDataPreparationWetnessindex();
	afx_msg void OnDatapreparationNdvi();
	afx_msg void OnFileConverter3drToGridascii();
	afx_msg void OnFileCalcstatistics();
	afx_msg void OnFileconverterGridasciiTo3dr();
	afx_msg void OnFileconverterGridasciiTo3dm();
    afx_msg void OnFileconverter3drTo3dm();

	afx_msg void OnUtilitiesValuesAtSamplesites();
	afx_msg void OnUtilitiesSample();	
	afx_msg void OnUtilities3drViewer();
	afx_msg void OnUtilitiesClip();
	afx_msg void OnUtilitiesReclass();
	afx_msg void OnUtilitiesOverlay();
	afx_msg void OnUtilitiesStretch();
	//afx_msg void OnUtilitiesSample();
	afx_msg void OnUtilitiesFrequency();
	afx_msg void OnUtilitiesColor();
	afx_msg void OnUtilitiesFilter();	
	afx_msg void OnUtilities3dvisualization();
	afx_msg void OnHelp();


	afx_msg void OnClose();
	afx_msg void OnDatabaseDatalocation();

	afx_msg void OnFileconverterOtherrasterformat();
	afx_msg void OnFileconverterOtherto3dm();
	afx_msg void OnTypevalidataionPointlistvs();
	afx_msg void OnValidationPropertyvalidation();

	afx_msg void OnPurposivesamplingFcmclustering();
	afx_msg void OnPurposivesamplingSampledesign();
	afx_msg void OnMappingWithSamples();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__57C35901_B6A0_45DC_9229_2697767F4931__INCLUDED_)

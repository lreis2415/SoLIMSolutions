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
// SoLIM.h : main header file for the GUIVISIODEMO application
//

#if !defined(AFX_GUIVISIODEMO_H__CFBEAD0E_0D2B_46DA_A58E_069E22DCEC05__INCLUDED_)
#define AFX_GUIVISIODEMO_H__CFBEAD0E_0D2B_46DA_A58E_069E22DCEC05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


#include <vector>
using namespace std;
#include "ThreeDR.h"
#include "ThreeDM.h"
#include "terrain.h"

#include "CBRSamplePoint.h"
#include "CBRSmallScaleDlg.h"
#include "CBRGenerateFileDlg.h"
#include "CBRSampleDlg.h"
#include "CBRSimilarityDlg.h"
#include "CBRUncertaintyDlg.h"
#include "CBRTypeInferenceDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CSoLIMApp:
// See SoLIM.cpp for the implementation of this class
//

class CSoLIMApp : public CWinApp
{
public:
	CSoLIMApp();

	~CSoLIMApp();



	int m_iCurrEnvLayerIndex;		//the index of current environment database layer
	CPoint m_ptBufDataIndex;	//indetify the position of the cursor in 3dr buffer
	int m_iIsOriginChanged;
	CString m_exePath;   //the path of the exe file


	// For small scale inference 
	vector<CString> g_vecstrClimateLayers;	//climate factors
	vector<CString> g_vecstrParentMaterialLayers;	//parent material factors
	vector<CString> g_vecstrTopographicLayers;	//terrain factors
	vector<CString> g_vecstrVegetationLayers;   //Vegetation variable

	// data resampled to the unified resolution
	vector<CString> g_vecClimate;
	vector<CString> g_vecParentMaterial;
	vector<CString> g_vecTopology;
	vector<CString> g_vecVegetation;

	//sample data files for small scale inference
	CString g_climateSample;
	CString g_parentMaterialSample;
	CString g_topoSample;
	CString g_vegSample;
	CString g_attrSample;
	vector<CString> g_vecTopologySample;
	vector<CString> g_vecVegetationSample;

	//final similarity vector
	vector<CString> g_vecSimiFile;
	vector<CBRSamplePoint*> g_vecSamplePoint;
	//destination resolution
	double g_dstResolution;
	//whether or not use kernel density estimation
	bool g_useKde, g_useKdeVegetation;

	//Dialog pointer
	CBRSmallScaleDlg* pSmallScaleDlg;
	CBRGenerateFileDlg* pGenerateFileDlg;
	CBRSampleDlg* pSampleDlg;
	CBRSimilarityDlg* pSimiDlg;
	CBRUncertaintyDlg* pUncertaintyDlg;

	CBRTypeInferenceDlg* pTypeinferDlg;

	int iCBROption;  //added by Fei
	//end



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoLIMApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSoLIMApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIVISIODEMO_H__CFBEAD0E_0D2B_46DA_A58E_069E22DCEC05__INCLUDED_)

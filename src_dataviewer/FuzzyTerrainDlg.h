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

#include "utility.h"
#include "EnvData.h"
#include "knowledge.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "newenvdlg.h"
#include "ApplyMenuDlg.h"

#include "beforeinferencerepdlg.h"
#include "envfactormismatchdlg.h"
#include "checkenvfctofkbdlg.h"
#include "ahpdlg.h"
#include "GetDirDlg.h"



// CFuzzyTerrainDlgdialog

class CFuzzyTerrainDlg: public CDialog
{
	DECLARE_DYNAMIC(CFuzzyTerrainDlg)

public:
	CFuzzyTerrainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFuzzyTerrainDlg();

public:
	//the following varialbes are used outside this class 
	//(mainly for exchanging data with class SoLIMDlg)

	EnvDatabase* theEnvDatabase;
	RasterCases* theRstKnowledge;	
	List<AHP_ScaleList>* theAHP;   
	BOOL	m_UseSpatial;	
	RuleList* m_curRuleList;


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();	

	afx_msg void OnBnClickedNewbtn();
	afx_msg void OnBnClickedLoadbtn();
	afx_msg void OnBnClickedSavebtn();	


	afx_msg void OnLvnEndlabeleditFeaturelistctrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedFeaturelistctrl(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedSpatialsettingbtn();
	afx_msg void OnBnClickedEditenvlistbtn();

	afx_msg void OnBnClickedApplybtn();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnBellMenuitem();
	afx_msg void OnSMenuitem();
	afx_msg void OnZMenuitem();
	afx_msg void OnCyclicMenuitem();
	afx_msg void OnOrdinalMenuitem();
	afx_msg void OnNominalMenuitem();
	//afx_msg void OnDefaultMenuitem();

	afx_msg void OnEnKillfocusLowervaluefld();
	afx_msg void OnEnKillfocusUppervaluefld();
	afx_msg void OnEnKillfocusWidth1fld();
	afx_msg void OnEnKillfocusWidth2fld();
	afx_msg void OnEnKillfocusr1fld();
	afx_msg void OnEnKillfocusr2fld();
	afx_msg void OnEnKillfocusSearchdistfld();
	afx_msg void OnEnKillfocusVtohconvertfactorfld();
	afx_msg void OnEnKillfocusDistdecayfactorfld();
	afx_msg void OnEnKillfocusWgtdecayfactorfld();
	afx_msg void OnCbnKillfocusIntegratingalgocmb();

	//afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNMRclickFeaturelistHeaderctrl(NMHDR *pNMHDR, LRESULT *pResult);

	//Functions called by the right-click menus 
	afx_msg void OnLimitingfactor();
	afx_msg void OnWeightedaverage();
	//afx_msg void ApplyToList();
	//afx_msg void ApplyToKB();


	afx_msg void OnBnClickedAhpbtn();
	afx_msg void OnClose();
	afx_msg void OnMultiplication();
	afx_msg void OnRawValuesMenuItem();
	afx_msg void OnCbnSelchangeViewtypecmb();
	afx_msg void OnBnClickedPositiverdo();
	afx_msg void OnBnClickedNegativerdo();
	afx_msg void OnEnKillfocusMembershipfld();
	//afx_msg void OnBnClickedFuzzybndchk();

	afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedRuninferbtn();

    DECLARE_MESSAGE_MAP()

	// Dialog Data
	enum { IDD = IDD_FuzzyTerrainDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL PreTranslateMessage( MSG* pMsg );	 
	


private:
	CString m_OutputFileName;
	CString m_CaseFileName;
	CString m_FieldName;
	HTREEITEM m_treeRoot;
	CTreeCtrl m_theTree;
	CListCtrl m_theList;
	CNewEnvDlg m_newEnvDlg;
	CApplyMenuDlg m_applyMenuDlg;

	//cur variables
	Attr* m_curAttr;
	Spatial* m_curSpatial;
	FeatureNode* m_curFeature;	
	


	//icons for buttons
	HICON hiconNew;
	HICON hiconLoad;
	HICON hiconOpen;
	HICON hiconSave;
	HICON hiconDB;
	int m_ApplyTo;
	float m_MemberValue;
	CString m_PFunc;

	//for defining curve
	float m_v1;
	float m_w1;
	int m_r1;
	float m_v2;
	float m_w2;
	int m_r2;
	int rightCrossX;
	int leftCrossX;
	int crossY;
	int rightTopX;
	int leftTopX;
	int centralX;
	int rightX;
	int leftX;
	int lowY;
	int highY;
	int previousCursorX;
	CPoint startPnt;
	CRect graphicRegion;
	CRect graphicRegion2; //for lower part of the window
	bool m_LBtnDownW;
	bool m_LBtnDownV;  //Add by DuFei
	int step;
	float unitY;
	float unitX;
	CString m_EFunc;
	float m_LeftValue; //x value at 0.0001 membership value
	float m_RightValue;
	float m_realW1; //for preserving the precision of m_w1, since m_w1 will be trancated
	float m_realW2; //for preserving the precision of m_w2, since m_w2 will be trancated
	//////// Added by DuFei/////////
	float m_realV1;//for preserving the precision of m_v1  
	float m_realV2;//for preserving the precision of m_v2
	float m_prevV1;//for record the previous value of m_v1,m_v2,m_w1,m_w2
	float m_prevV2;
    float m_prevW1;
	float m_prevW2;  
	/////////////////////////////

	CComboBox m_curveTypeCmb;
	CString m_CurveType;

	CString m_ViewType;
	CComboBox m_viewTypeCmb;
	BOOL m_Negative; //indicates the current attr is positive/negative
	float m_TopValue; //the top value on Y-axis 
	float m_MidValue; //the middle value on Y-axis

private:
	//Spatial
	double	m_x;
	double	m_y;
	BOOL	m_UseDistSimilarity;
	float	m_SearchDist;
	float	m_DistDecayFactor;
	float	m_VtoHConvertFactor;
	CString	m_IntegratingAlgStr;
	int m_IntegratingAlgo;
	float	m_WgtDecayFactor;

    BOOL m_isInit;   //Added by DuFei to determing if the dialog is init just now


private:
	void setUnitX();
	void setIntialCurveVariables();
	int	newRstCasebase();
	//int loadForEmptyEnvDatabase(List<FeatureNode>* theFeatureList);
	int removeRstCasebase(void);
	void runInference(); //for handling max/min membership value and positive/negative issus.  Called by DoThis and DoBatch.  Calls specific inference function listed below.
	void RstInference(float* soilmap, float* checkmap);
	CThreeDR* CheckDEM(); //Check if the DEM layer is available for calculating search range and/or distance similarity
	void CheckNodata(float* soilmap, bool* idBuf); //Check if any pixels should be assigned nodata
	
	void output(float* soilmap, float* checkmap); //Output inference results
	void updateFields();
	void setCurve(); //set curve shape according to M_EFunc
	void updateKnowledge(); 
	
	void fillTree();
	void fillList();
	void fillOthers();
	void setInitialForCur();
	int integratingAlgCodeStr(bool flag);
	void enableDisableItems();
	void openSpatialPanel();//open the spatial panel
	void closeSpatialPanel();//close the spatial panel
	void clearFields();

	// indicate if a click on the tree change the page in the list.  used by the itemchanged notification of the list.  don't want to check the change if the page changes.
	Attr* m_oldAttr;
	CBeforeInferenceRepDlg m_beforeInferenceRepDlg;
	CProgressCtrl m_ProgressBar;
	CString m_RunningType;
	CCheckEnvFctOfKBDlg m_chkEnvFctOfKBDlg;
	CEnvFactorMismatchDlg m_envFactorMismatchDlg;
	CAHPDlg m_AHPDlg;
	CGetDirDlg m_getDirDlg;   //Added by DuFei


public:
	void ClearDialog(void);  //Added by to set the variables of this class to be null
	float m_OriginValue;
	afx_msg void OnCbnSelchangecurvetypecmb();
	CToolTipCtrl *m_tooltip;
	//void RBInference(float* soilmap, float* checkmap, RuleList* theRuleList);

};

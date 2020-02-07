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
// InferenceView.cpp : implementation file
//
#include <stdio.h>
#include <afx.h>
#include "stdafx.h"
#include "SoLIM.h"
#include "CBInferenceView.h"
#include ".\cbinferenceview.h"
#include "MainFrm.h"
#include "SoLIMDoc.h"
#include "Distribution.h"

#include "CBUserDefinedParaDlg.h"

#include <fstream>
#include <iomanip>
#include <vector>
#include <string>

#include "ThreeDR.h"
#include "CBRSamplePoint.h"

#include "Utility.h"
#include "gdal_priv.h"

#include <sstream>
#include <map>
using namespace std;

#include "DataFile.h"
#include "FileManager.h"



// CCBInferenceView


const string sep = "\\";


IMPLEMENT_DYNCREATE(CCBInferenceView, CFormView)

CCBInferenceView::CCBInferenceView()
: CFormView(CCBInferenceView::IDD)
, m_dFinalRes(0)
{

	climateCut = 0.5;
	climateWeight  = 0.5;
	topoWeight = 0.5;
	varCount = 0;
	vegWeight = 0;

	//outputFolderSimi = getCurrentPath() + "\\similarity";
	curProcess = 0.0;

	m_curSelTab = 0;
}

CCBInferenceView::~CCBInferenceView()
{
}

void CCBInferenceView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);




	/*
	DDX_Control(pDX, IDC_EDIT_CLIMATE_CUT, climateCutCtrl);
	DDX_Control(pDX, IDC_EDIT_CLIMATE_WEIGHT, climateWeightCtrl);
	DDX_Control(pDX, IDC_EDIT_TERRAIN_WEIGHT, topoWeightCtrl);	
	DDX_Control(pDX, IDC_EDIT_VEGETAION_WEIGHT, vegWeightCtrl);

	DDX_Text(pDX, IDC_EDIT_CLIMATE_CUT, climateCut);
	DDX_Text(pDX, IDC_EDIT_CLIMATE_WEIGHT, climateWeight);
	DDX_Text(pDX, IDC_EDIT_TERRAIN_WEIGHT, topoWeight);
	DDX_Text(pDX, IDC_EDIT_VEGETAION_WEIGHT, vegWeight);*/



	DDX_Control(pDX, IDC_COMBO_CASE, m_comboSampleMethod);




	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);



	DDX_Control(pDX, IDC_LIST_LAYER_PARA_SETTING, m_ListLayerSetting);


	DDX_Control(pDX, IDC_TAB_PROPERTY_TYPE, m_tabPropertyType);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_editProgress);
	DDX_Control(pDX, IDOK, m_btnRunInference);
	DDX_Control(pDX, IDC_FRAME_LAYER_SETTING, m_frameLayerSetting);
	DDX_Control(pDX, IDC_FRAME_SAMPLE_METHOD, m_frameSampleMethod);
	DDX_Control(pDX, IDC_STATIC_SAMPLE_METHOD, m_staticSampleMethod);
	DDX_Control(pDX, IDC_BUTTON_CASEMETHOD_SETTING, m_btnSampleMethod);
	DDX_Control(pDX, IDC_FRAME_ITEM_TO_INFER, m_frameItemInfer);
	DDX_Control(pDX, IDC_FRAME, m_frame);









	DDX_Control(pDX, IDC_LIST_ALL_FIELD, listAllField);

	DDX_Text(pDX, IDC_EDIT_Uncertaintyfilename, m_OutputUncertaintyFilename);
	DDX_Text(pDX, IDC_EDIT_Valuesfoldername, m_OutputPredictvaluesFoldername);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD_PROPERTY, m_thresholdProperty);
	DDX_Control(pDX, IDC_EDIT_Uncertaintyfilename, uncertaintyCtrl);
	DDX_Control(pDX, IDC_EDIT_Valuesfoldername, propertyFolderCtrl);
	DDX_Control(pDX, IDC_EDIT_THRESHOLD_PROPERTY, thresholdPropertyCtrl);


	DDX_Control(pDX, IDC_FRAME_RESULT, m_frameResult);
	DDX_Control(pDX, IDC_STATIC_SAVE_MAP, m_staticSaveMap);
	DDX_Control(pDX, IDC_BUTTON_SaveValues, m_btnSaveMap);
	DDX_Control(pDX, IDC_STATIC_THESHOLD, m_staticThreshold);
	DDX_Control(pDX, IDC_STATIC_SAVE_UNCERTAINTY, m_staticSaveUncertainty);
	DDX_Control(pDX, IDC_BUTTON_SaveUncertainty, m_btnSaveUncertainty);



	DDX_Control(pDX, IDC_LIST_ALL_TYPE, listAllType);

	DDX_Control(pDX, IDC_COMBO_TYPE_FIELD, comboTypeField);
	DDX_CBString(pDX, IDC_COMBO_TYPE_FIELD, typeFieldName);


	DDX_Text(pDX, IDC_EDIT_MEMBERSHIP, m_strMaxSimiFolder);






	DDX_Control(pDX, IDC_STATIC_TYPE_FIELD, m_staticTypeField);
	DDX_Control(pDX, IDC_STATIC_SELECT_TYPE, m_staticSelectType);
	DDX_Control(pDX, IDC_FRAME_RESULT_TYPE, m_frameResultType);
	DDX_Control(pDX, IDC_STATIC_SAVE_RESULT_TYPE, m_staticSaveTypeResult);
	DDX_Control(pDX, IDC_EDIT_MEMBERSHIP, m_editTypeResultFolder);
	DDX_Control(pDX, IDC_BUTTON_SAVE_TYPERESULT, m_btnSaveTypeResult);
	DDX_Control(pDX, IDC_STATIC_SELECT_PROPERTY, m_staticSelectProperty);
	DDX_Text(pDX, IDC_EDIT_FINAL_RESOLUTION, m_dFinalRes);
	DDX_Control(pDX, IDC_STATIC_FINAL_RESOLUTION, m_staticResolution);
	DDX_Control(pDX, IDC_EDIT_FINAL_RESOLUTION, m_editResolution);

	DDX_Control(pDX, IDC_STATIC_RESULT_TYPE, m_staticSaveType);
	DDX_Control(pDX, IDC_COMBO_SAVE_TYPE, m_comboSaveType);
	
}

BEGIN_MESSAGE_MAP(CCBInferenceView, CFormView)
	ON_WM_SIZE()

	ON_BN_CLICKED(IDOK, OnBnClickedOk)


	//ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO_CASE, OnCbnSelchangeComboCase)




	ON_BN_CLICKED(IDC_BUTTON_CASEMETHOD_SETTING, OnBnClickedButtonCasemethodSetting)


	//ON_NOTIFY(NM_CLICK, IDC_LIST_LAYER_PARA_SETTING, OnNMClickListLayerParaSetting)


	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PROPERTY_TYPE, OnTcnSelchangeTabPropertyType)



	ON_BN_CLICKED(IDC_BUTTON_SaveUncertainty, OnBnClickedButtonSaveuncertainty)
	ON_BN_CLICKED(IDC_BUTTON_SaveValues, OnBnClickedButtonSavevalues)


	ON_BN_CLICKED(IDC_BUTTON_SAVE_TYPERESULT, OnBnClickedButtonSaveTyperesult)	
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_FIELD, OnCbnSelchangeComboTypeField)

	ON_EN_KILLFOCUS(IDC_EDIT_FINAL_RESOLUTION, OnEnKillfocusEditFinalResolution)

END_MESSAGE_MAP()


// CCBInferenceView diagnostics

#ifdef _DEBUG
void CCBInferenceView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCBInferenceView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CCBInferenceView message handlers

void CCBInferenceView::OnSize(UINT nType, int cx, int cy)
{

	CFormView::OnSize(nType, cx, cy);

	if(cx < 800 || cy <600)
		return;





	if(m_frame.GetSafeHwnd())
	{
		CRect rect(0,-5,cx,cy);
		m_frame.MoveWindow(&rect);
	}


	if(m_staticResolution.GetSafeHwnd())
	{
		CRect rect(cx/10, 0.5 *cy/10,  cx/10 + 200, 0.5*cy/10 + 20);
		m_staticResolution.MoveWindow(&rect);
	}


	if(m_editResolution.GetSafeHwnd())
	{
		CRect rect(cx/10 + 220, 0.5*cy/10, cx/10 + 320, 0.5*cy/10 + 20);
		m_editResolution.MoveWindow(&rect);
	}


	if(this->m_frameLayerSetting.GetSafeHwnd())
	{
		CRect rect(cx/10,0.9*cy/10,9*cx/10,3.5*cy/10);
		m_frameLayerSetting.MoveWindow(&rect);
	}
	if(m_ListLayerSetting.GetSafeHwnd())
	{
		CRect rect(1.1*cx/10,1.3*cy/10,8.9*cx/10,3.4*cy/10);
		m_ListLayerSetting.MoveWindow(&rect);

		m_ListLayerSetting.Invalidate();

		m_ListLayerSetting.SetColumnWidth(0, (0.88*cx - 80)*0.12);
		m_ListLayerSetting.SetColumnWidth(1, (0.88*cx-80)*0.13);
		m_ListLayerSetting.SetColumnWidth(2, (0.88*cx-80)*0.12);
		m_ListLayerSetting.SetColumnWidth(3, (0.88*cx-80)*0.28);
		m_ListLayerSetting.SetColumnWidth(4, (0.88*cx-80)*0.35);
		//	m_ListLayerSetting.SetColumnWidth(4, 0.82*cx*0.35);
	}





	if(m_frameSampleMethod.GetSafeHwnd())
	{
		CRect rect(cx/10,3.8*cy/10,9*cx/10,3.8*cy/10+60);
		m_frameSampleMethod.MoveWindow(&rect);
	}
	if(this->m_staticSampleMethod.GetSafeHwnd())
	{
		CRect rect(1.5*cx/10,3.8*cy/10+20,3*cx/10,3.8*cy/10+40);
		m_staticSampleMethod.MoveWindow(&rect);
	}

	if(m_comboSampleMethod.GetSafeHwnd())
	{
		CRect rect(3*cx/10+10,3.8*cy/10+20,3*cx/10 + 200,3.8*cy/10+40);
		m_comboSampleMethod.MoveWindow(&rect);
	}

	if(this->m_btnSampleMethod.GetSafeHwnd())
	{
		CRect rect(7*cx/10,3.8*cy/10+20,7*cx/10+80,3.8*cy/10+40);
		m_btnSampleMethod.MoveWindow(&rect);
	}




	

	if(this->m_frameItemInfer.GetSafeHwnd())
	{
		//CRect rect(cx/10,4.9*cy/10,9*cx/10,9.6*cy/10);
		CRect rect(cx/10,4.9*cy/10,9*cx/10,9.2*cy/10);
		m_frameItemInfer.MoveWindow(&rect);
	}

	if(m_tabPropertyType.GetSafeHwnd())
	{
		//CRect rect(1.1*cx/10,5.15*cy/10,8.9*cx/10,9.4*cy/10+8);
		CRect rect(1.1*cx/10,5.15*cy/10,8.9*cx/10,9.1*cy/10);
		m_tabPropertyType.MoveWindow(&rect);

	}



	if(this->m_staticSelectProperty.GetSafeHwnd())
	{
		CRect rect(1.15 * cx/10,5.5*cy/10,8.85* cx/10,5.5*cy/10 + 20);
		this->m_staticSelectProperty.MoveWindow(&rect);
	}	


	if(this->listAllField.GetSafeHwnd())
	{
		CRect rect(1.15 * cx/10,5.5*cy/10+25,8.85* cx/10,6.7*cy/10);
		this->listAllField.MoveWindow(&rect);
	}	


	if(this->m_frameResult.GetSafeHwnd())
	{
		//CRect rect(1.15 * cx/10,6.8*cy/10,8.85* cx/10,9.5*cy/10);
		CRect rect(1.15 * cx/10,6.8*cy/10,8.85* cx/10,9.05*cy/10);
		this->m_frameResult.MoveWindow(&rect);
	}


	if(this->m_staticSaveMap.GetSafeHwnd())
	{
		CRect rect(1.2 * cx/10,6.8*cy/10+15,8.8* cx/10,6.8*cy/10+35);
		this->m_staticSaveMap.MoveWindow(&rect);
	}

	if(this->propertyFolderCtrl.GetSafeHwnd())
	{
		CRect rect(1.2 * cx/10,6.8*cy/10 + 40,7.6* cx/10,6.8*cy/10+60);
		this->propertyFolderCtrl.MoveWindow(&rect);
	}

	if(this->m_btnSaveMap.GetSafeHwnd())
	{
		CRect rect(7.7 * cx/10,6.8*cy/10+40,8.8* cx/10,6.8*cy/10+60);
		this->m_btnSaveMap.MoveWindow(&rect);
	}

	if(this->m_staticThreshold.GetSafeHwnd())
	{
		CRect rect(1.2 * cx/10,6.8*cy/10+65,1.2* cx/10+ 200,6.8*cy/10+85);
		this->m_staticThreshold.MoveWindow(&rect);
	}
	if(this->thresholdPropertyCtrl.GetSafeHwnd())
	{
		//CRect rect(1.2 * cx/10+ 210,6.8*cy/10+65,8.8* cx/10,6.8*cy/10+85);
		CRect rect(3.2* cx/10,6.8*cy/10+65,8.8* cx/10,6.8*cy/10+85);
		this->thresholdPropertyCtrl.MoveWindow(&rect);
	}

	if(this->m_staticSaveUncertainty.GetSafeHwnd())
	{
		//CRect rect(1.2 * cx/10,6.8*cy/10+90,8.8* cx/10,6.8*cy/10+110);
		CRect rect(0,0,1,1);
		this->m_staticSaveUncertainty.MoveWindow(&rect);
	}
	if(this->uncertaintyCtrl.GetSafeHwnd())
	{
		//CRect rect(1.2 * cx/10,6.8*cy/10+115,7.6* cx/10,6.8*cy/10+135);
		CRect rect(0,0,1,1);
		this->uncertaintyCtrl.MoveWindow(&rect);
	}

	if(this->m_btnSaveUncertainty.GetSafeHwnd())
	{
		//CRect rect(7.7 * cx/10,6.8*cy/10+115,8.8* cx/10,6.8*cy/10+135);
		CRect rect(0,0,1,1);
		this->m_btnSaveUncertainty.MoveWindow(&rect);
	}



	//type 

	if(this->m_staticTypeField.GetSafeHwnd())
	{
		CRect rect(1.15 * cx/10,5.5*cy/10+5,1.15* cx/10+ 100,5.5*cy/10 + 25);
		this->m_staticTypeField.MoveWindow(&rect);
	}	




	if(this->comboTypeField.GetSafeHwnd())
	{
		CRect rect(1.15 * cx/10+110,5.5*cy/10+10,8.8* cx/10,5.5*cy/10 + 80);
		this->comboTypeField.MoveWindow(&rect);
	}	







	if(this->m_staticSelectType.GetSafeHwnd())
	{
		CRect rect(1.15 * cx/10,6.0*cy/10,8.85* cx/10,6.0*cy/10+ 20);
		this->m_staticSelectType.MoveWindow(&rect);
	}

	if(listAllType.GetSafeHwnd())
	{
		CRect rect(1.15 * cx/10,6.0*cy/10 + 25,8.85* cx/10,7.3*cy/10);
		listAllType.MoveWindow(&rect);
	}


	if(m_frameResultType.GetSafeHwnd())
	{
		CRect rect(1.15 * cx/10,7.4*cy/10,8.85* cx/10,9.0*cy/10);
		m_frameResultType.MoveWindow(&rect);
	}



	if(this->m_staticSaveTypeResult.GetSafeHwnd())
	{
		CRect rect(1.2 * cx/10,7.7*cy/10,8.8* cx/10,7.7*cy/10+20);
		m_staticSaveTypeResult.MoveWindow(&rect);
	}
	if(m_editTypeResultFolder.GetSafeHwnd())
	{
		CRect rect(1.2 * cx/10,7.7*cy/10+30,7.6* cx/10,7.7*cy/10+50);
		m_editTypeResultFolder.MoveWindow(&rect);
	}

	if(m_btnSaveTypeResult.GetSafeHwnd())
	{
		CRect rect(7.7 * cx/10,7.7*cy/10+30,8.8* cx/10,7.7*cy/10+50);
		m_btnSaveTypeResult.MoveWindow(&rect);
	}



	// save format as
	if(this->m_staticSaveType.GetSafeHwnd())
	{
		//CRect rect(1.2 * cx/10, 6.8*cy/10+140, 3* cx/10, 6.8*cy/10+155);
		CRect rect(1.2 * cx/10,6.8*cy/10+110,8.8* cx/10,6.8*cy/10+130);
		this->m_staticSaveType.MoveWindow(&rect);
	}
	if(this->m_comboSaveType.GetSafeHwnd())
	{
		//CRect rect(3.2 * cx/10, 6.8*cy/10+140, 6.2* cx/10, 6.8*cy/10+155);
		CRect rect(3.2 * cx/10, 6.8*cy/10+110, 6.2* cx/10,6.8*cy/10+195);
		this->m_comboSaveType.MoveWindow(&rect);
	}







	if(m_btnRunInference.GetSafeHwnd())
	{
		CRect rect(cx/10,9.3*cy/10,3*cx/10,9.3*cy/10+30);
		m_btnRunInference.MoveWindow(&rect);
	} 
	if(m_ProgressBar.GetSafeHwnd())
	{
		CRect rect(3*cx/10+10,9.3*cy/10,9*cx/10,9.3*cy/10+20);
		m_ProgressBar.MoveWindow(&rect);
	}

	if(this->m_editProgress.GetSafeHwnd())
	{
		CRect rect(3*cx/10+10,9.3*cy/10 + 30,9*cx/10,9.3*cy/10+50);
		m_editProgress.MoveWindow(&rect);

	}

	m_staticSaveUncertainty.ShowWindow(SW_HIDE);
	m_btnSaveUncertainty.ShowWindow(SW_HIDE);
	uncertaintyCtrl.ShowWindow(SW_HIDE);

	this->Invalidate();








}


void CCBInferenceView::OnInitialUpdate()
{



	CFormView::OnInitialUpdate();

	this->Invalidate();

	CSoLIMApp* pApp=(CSoLIMApp*)AfxGetApp();

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());




	pDoc->setCurViewType(4);
	pFrame->m_leftBar.ExpandNode(5);



	/*

	m_ListLayerSetting.InsertColumn(0, "Category", LVCFMT_LEFT, 100);
	m_ListLayerSetting.InsertColumn(1, "Layer Name", LVCFMT_LEFT,100);
	m_ListLayerSetting.InsertColumn(2, "Resolution", LVCFMT_LEFT, 150);
	m_ListLayerSetting.InsertColumn(3, "Characterization Method", LVCFMT_LEFT, 150);
	m_ListLayerSetting.InsertColumn(4, "Similarity Calculation Method", LVCFMT_LEFT, 400);*/



	m_ListLayerSetting.InsertColumn(0, "Category", LVCFMT_LEFT);
	m_ListLayerSetting.InsertColumn(1, "Layer Name", LVCFMT_LEFT);
	m_ListLayerSetting.InsertColumn(2, "Resolution", LVCFMT_LEFT);
	m_ListLayerSetting.InsertColumn(3, "Characterization Method", LVCFMT_LEFT);
	m_ListLayerSetting.InsertColumn(4, "Similarity Calculation Method", LVCFMT_LEFT);



	this->m_ListLayerSetting.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);



	/*

	this->m_ListLayerSetting.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	this->m_ListLayerSetting.InsertColumn(1,CString("Layer Name"));
	this->m_ListLayerSetting.SetColumnWidth(0,80);


	m_ListLayerSetting.InsertColumn(2, CString("Resolution"));
	m_ListLayerSetting.SetColumnWidth(1, 80);

	m_ListLayerSetting.InsertColumn(3, CString("single value"));
	m_ListLayerSetting.SetColumnWidth(2, 80);



	m_ListLayerSetting.InsertColumn(4, CString("Similarity Computation"));
	//m_ListLayerSetting.SetColumnWidth(3, 80);

	m_ListLayerSetting.InsertItem(1, "sff");*/


	//const char* content = DBFReadStringAttribute(pDoc->m_pProjectCaseEdit->hDBF, i, j);
	//	m_ListLayerSetting.SetItemText(m_ListLayerSetting.GetItemCount()-1, j+1, content);



	//为Tab Control增加两个页面
	m_tabPropertyType.InsertItem(0, _T("Property Inference"));
	m_tabPropertyType.InsertItem(1, _T("Type Inference"));





	FillInferenceView();

	CRect rect;
	GetClientRect(&rect);

	int cx = rect.right;
	int cy = rect.bottom;




	if(m_frame.GetSafeHwnd())
	{
		CRect rect(0,-5,cx,cy);
		m_frame.MoveWindow(&rect);
	}


	if(m_staticResolution.GetSafeHwnd())
	{
		CRect rect(cx/10, 0.5 *cy/10, cx/10 + 200 , 0.5*cy/10 + 20);
		m_staticResolution.MoveWindow(&rect);
	}


	if(m_editResolution.GetSafeHwnd())
	{
		CRect rect(cx/10 + 220, 0.5*cy/10, cx/10 + 320, 0.5*cy/10 + 20);
		m_editResolution.MoveWindow(&rect);
	}


	if(this->m_frameLayerSetting.GetSafeHwnd())
	{
		CRect rect(cx/10,0.9*cy/10,9*cx/10,3.5*cy/10);
		m_frameLayerSetting.MoveWindow(&rect);
	}
	if(m_ListLayerSetting.GetSafeHwnd())
	{
		CRect rect(1.1*cx/10,1.3*cy/10,8.9*cx/10,3.4*cy/10);
		m_ListLayerSetting.MoveWindow(&rect);

		m_ListLayerSetting.Invalidate();

		m_ListLayerSetting.SetColumnWidth(0, (0.88*cx - 80)*0.12);
		m_ListLayerSetting.SetColumnWidth(1, (0.88*cx-80)*0.13);
		m_ListLayerSetting.SetColumnWidth(2, (0.88*cx-80)*0.12);
		m_ListLayerSetting.SetColumnWidth(3, (0.88*cx-80)*0.28);
		m_ListLayerSetting.SetColumnWidth(4, (0.88*cx-80)*0.35);
		//	m_ListLayerSetting.SetColumnWidth(4, 0.82*cx*0.35);
	}







	if(m_frameSampleMethod.GetSafeHwnd())
	{
		CRect rect(cx/10,3.8*cy/10,9*cx/10,3.8*cy/10+60);
		m_frameSampleMethod.MoveWindow(&rect);
	}
	if(this->m_staticSampleMethod.GetSafeHwnd())
	{
		CRect rect(1.5*cx/10,3.8*cy/10+20,3*cx/10,3.8*cy/10+40);
		m_staticSampleMethod.MoveWindow(&rect);
	}

	if(m_comboSampleMethod.GetSafeHwnd())
	{
		CRect rect(3*cx/10+10,3.8*cy/10+20,3*cx/10 + 200,3.8*cy/10+40);
		m_comboSampleMethod.MoveWindow(&rect);
	}

	if(this->m_btnSampleMethod.GetSafeHwnd())
	{
		CRect rect(7*cx/10,3.8*cy/10+20,7*cx/10+80,3.8*cy/10+40);
		m_btnSampleMethod.MoveWindow(&rect);
	}




	if(this->m_frameItemInfer.GetSafeHwnd())
	{
		//CRect rect(cx/10,4.9*cy/10,9*cx/10,9.6*cy/10);
		CRect rect(cx/10,4.9*cy/10,9*cx/10,9.2*cy/10);
		m_frameItemInfer.MoveWindow(&rect);
	}

	if(m_tabPropertyType.GetSafeHwnd())
	{
		//CRect rect(1.1*cx/10,5.15*cy/10,8.9*cx/10,9.4*cy/10+8);
		CRect rect(1.1*cx/10,5.15*cy/10,8.9*cx/10,9.1*cy/10);
		m_tabPropertyType.MoveWindow(&rect);

	}



	if(this->m_staticSelectProperty.GetSafeHwnd())
	{
		CRect rect(1.15 * cx/10,5.5*cy/10,8.85* cx/10,5.5*cy/10 + 20);
		this->m_staticSelectProperty.MoveWindow(&rect);
	}	


	if(this->listAllField.GetSafeHwnd())
	{
		CRect rect(1.15 * cx/10,5.5*cy/10+25,8.85* cx/10,6.7*cy/10);
		this->listAllField.MoveWindow(&rect);
	}	

	if(this->m_frameResult.GetSafeHwnd())
	{
		//CRect rect(1.15 * cx/10,6.8*cy/10,8.85* cx/10,9.5*cy/10);
		CRect rect(1.15 * cx/10,6.8*cy/10,8.85* cx/10,9.05*cy/10);
		this->m_frameResult.MoveWindow(&rect);
	}


	if(this->m_staticSaveMap.GetSafeHwnd())
	{
		CRect rect(1.2 * cx/10,6.8*cy/10+15,8.8* cx/10,6.8*cy/10+35);
		this->m_staticSaveMap.MoveWindow(&rect);
	}

	if(this->propertyFolderCtrl.GetSafeHwnd())
	{
		CRect rect(1.2 * cx/10,6.8*cy/10 + 40,7.6* cx/10,6.8*cy/10+60);
		this->propertyFolderCtrl.MoveWindow(&rect);
	}

	if(this->m_btnSaveMap.GetSafeHwnd())
	{
		CRect rect(7.7 * cx/10,6.8*cy/10+40,8.8* cx/10,6.8*cy/10+60);
		this->m_btnSaveMap.MoveWindow(&rect);
	}

	if(this->m_staticThreshold.GetSafeHwnd())
	{
		CRect rect(1.2 * cx/10,6.8*cy/10+65,1.2* cx/10+ 200,6.8*cy/10+85);
		this->m_staticThreshold.MoveWindow(&rect);
	}
	if(this->thresholdPropertyCtrl.GetSafeHwnd())
	{
		//CRect rect(1.2 * cx/10+ 210,6.8*cy/10+65,8.8* cx/10,6.8*cy/10+85);
		CRect rect(3.2* cx/10,6.8*cy/10+65,8.8* cx/10,6.8*cy/10+85);
		this->thresholdPropertyCtrl.MoveWindow(&rect);
	}

	if(this->m_staticSaveUncertainty.GetSafeHwnd())
	{
		//CRect rect(1.2 * cx/10,6.8*cy/10+90,8.8* cx/10,6.8*cy/10+110);
		CRect rect(0,0,1,1);
		this->m_staticSaveUncertainty.MoveWindow(&rect);
	}
	if(this->uncertaintyCtrl.GetSafeHwnd())
	{
		//CRect rect(1.2 * cx/10,6.8*cy/10+115,7.6* cx/10,6.8*cy/10+135);
		CRect rect(0,0,1,1);
		this->uncertaintyCtrl.MoveWindow(&rect);
	}

	if(this->m_btnSaveUncertainty.GetSafeHwnd())
	{
		//CRect rect(7.7 * cx/10,6.8*cy/10+115,8.8* cx/10,6.8*cy/10+135);
		CRect rect(0,0,1,1);
		this->m_btnSaveUncertainty.MoveWindow(&rect);
	}



	//type 
	if(this->m_staticTypeField.GetSafeHwnd())
	{
		CRect rect(1.15 * cx/10,5.5*cy/10+5,1.15* cx/10+ 100,5.5*cy/10 + 25);
		this->m_staticTypeField.MoveWindow(&rect);
	}	


	if(this->comboTypeField.GetSafeHwnd())
	{
		CRect rect(1.15 * cx/10+110,5.5*cy/10+10,8.8* cx/10,5.5*cy/10 + 80);
		this->comboTypeField.MoveWindow(&rect);
	}	







	if(this->m_staticSelectType.GetSafeHwnd())
	{
		CRect rect(1.15 * cx/10,6.0*cy/10,8.85* cx/10,6.0*cy/10+ 20);
		this->m_staticSelectType.MoveWindow(&rect);
	}

	if(listAllType.GetSafeHwnd())
	{
		CRect rect(1.15 * cx/10,6.0*cy/10 + 25,8.85* cx/10,7.3*cy/10);
		listAllType.MoveWindow(&rect);
	}


	if(m_frameResultType.GetSafeHwnd())
	{
		CRect rect(1.15 * cx/10,7.4*cy/10,8.85* cx/10,9.0*cy/10);
		m_frameResultType.MoveWindow(&rect);
	}



	if(this->m_staticSaveTypeResult.GetSafeHwnd())
	{
		CRect rect(1.2 * cx/10,7.7*cy/10,8.8* cx/10,7.7*cy/10+20);
		m_staticSaveTypeResult.MoveWindow(&rect);
	}
	if(m_editTypeResultFolder.GetSafeHwnd())
	{
		CRect rect(1.2 * cx/10,7.7*cy/10+30,7.6* cx/10,7.7*cy/10+50);
		m_editTypeResultFolder.MoveWindow(&rect);
	}

	if(m_btnSaveTypeResult.GetSafeHwnd())
	{
		CRect rect(7.7 * cx/10,7.7*cy/10+30,8.8* cx/10,7.7*cy/10+50);
		m_btnSaveTypeResult.MoveWindow(&rect);
	}


	// save format as
	if(this->m_staticSaveType.GetSafeHwnd())
	{
		//CRect rect(1.2 * cx/10, 6.8*cy/10+140, 3* cx/10, 6.8*cy/10+155);
		CRect rect(1.2 * cx/10,6.8*cy/10+110,8.8* cx/10,6.8*cy/10+130);
		this->m_staticSaveType.MoveWindow(&rect);
	}
	if(this->m_comboSaveType.GetSafeHwnd())
	{
		//CRect rect(3.2 * cx/10, 6.8*cy/10+140, 6.2* cx/10, 6.8*cy/10+155);
		CRect rect(3.2 * cx/10, 6.8*cy/10+110, 6.2* cx/10,6.8*cy/10+195);
		this->m_comboSaveType.MoveWindow(&rect);
	}





	if(m_btnRunInference.GetSafeHwnd())
	{
		CRect rect(cx/10,9.3*cy/10,3*cx/10,9.3*cy/10+30);
		m_btnRunInference.MoveWindow(&rect);
	} 
	if(m_ProgressBar.GetSafeHwnd())
	{
		CRect rect(3*cx/10+10,9.3*cy/10,9*cx/10,9.3*cy/10+20);
		m_ProgressBar.MoveWindow(&rect);
	}

	if(this->m_editProgress.GetSafeHwnd())
	{
		CRect rect(3*cx/10+10,9.3*cy/10 + 30,9*cx/10,9.3*cy/10+50);
		m_editProgress.MoveWindow(&rect);

	}


	// comboxBox output file type
	m_comboSaveType.AddString("3DR  (*.3dr)");
	m_comboSaveType.AddString("SAGA GIS Binary format  (*.sdat)");
	//m_comboSaveType.AddString("Arc/Info ASCII Grid  (*.acs)");
	m_comboSaveType.AddString("Erdas Imagine  (*.img)");
	m_comboSaveType.AddString("TIFF / BigTIFF / GeoTIFF  (*.tif)");
	m_comboSaveType.SetCurSel(0);


	m_staticSaveUncertainty.ShowWindow(SW_HIDE);
	m_btnSaveUncertainty.ShowWindow(SW_HIDE);
	uncertaintyCtrl.ShowWindow(SW_HIDE);
	m_staticSaveUncertainty.EnableWindow(false);
	m_btnSaveUncertainty.EnableWindow(false);
	uncertaintyCtrl.EnableWindow(false);


	this->Invalidate();


}

CString CCBInferenceView::GetOutputExt()
{
	int nIndex = m_comboSaveType.GetCurSel();
	CString selectedText;
	m_comboSaveType.GetLBText(nIndex, selectedText);

	CString ext = strlwr(strrchr(selectedText.GetBuffer(0), '.'));
	ext = ext.Left(ext.GetLength()-1); 
	return ext;
}


void CCBInferenceView::FillInferenceView()
{

	this->Invalidate();

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());



	/*

	climateDataVec.clear();
	materialDataVec.clear();
	topoDataVec.clear();
	vegDataVec.clear();


	int curIndex = 0;
	biggestRes = -1;
	curIndex = readThreeDRData("climate", curIndex, pDoc->m_pProjectCaseEdit->g_vecstrClimateLayers, climateDataVec);
	curIndex = readThreeDRData("geology", curIndex, pDoc->m_pProjectCaseEdit->g_vecstrParentMaterialLayers, materialDataVec);
	curIndex = readThreeDRData("terrain", curIndex,pDoc->m_pProjectCaseEdit->g_vecstrTopographicLayers, topoDataVec);
	curIndex = readThreeDRData("vegetation", curIndex, pDoc->m_pProjectCaseEdit->g_vecstrVegetationLayers, vegDataVec);

	curIndex = readThreeDRData("other", curIndex, pDoc->m_pProjectCaseEdit->g_vecstrOtherLayers, otherDataVec);

	inputCount = curIndex;*/


	CString str;



	biggestRes = 0;


	int curIndex = 0;

	for (int i = 0; i <  pDoc->m_pProjectCaseEdit->g_vecstrClimateLayers.size(); i++)
	{
		m_ListLayerSetting.InsertItem(LVIF_TEXT|LVIF_STATE, curIndex, "climate", LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);

		CString str2 = pDoc->m_pProjectCaseEdit->vecClimateLayerNames[i];


		m_ListLayerSetting.SetItemText(curIndex,1, pDoc->m_pProjectCaseEdit->vecClimateLayerNames[i]);

		if(pDoc->m_pProjectCaseEdit->vecClimateLayerResolutions[i] > biggestRes)
		{
			biggestRes = pDoc->m_pProjectCaseEdit->vecClimateLayerResolutions[i];
		}

		str.Format("%f", pDoc->m_pProjectCaseEdit->vecClimateLayerResolutions[i]);

		m_ListLayerSetting.SetItemText(curIndex,2,str);

		m_ListLayerSetting.SetItemText(curIndex,3,pDoc->m_pProjectCaseEdit->vecClimateLayerSimiTypes[i]);

		m_ListLayerSetting.SetItemText(curIndex,4,pDoc->m_pProjectCaseEdit->vecClimateLayerSimiMethods[i]);

		curIndex ++;
	}

	for (int i = 0; i < pDoc->m_pProjectCaseEdit->g_vecstrGeologyLayers.size(); i++)
	{
		m_ListLayerSetting.InsertItem(LVIF_TEXT|LVIF_STATE, curIndex, "parent material", LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);

		m_ListLayerSetting.SetItemText(curIndex,1, pDoc->m_pProjectCaseEdit->vecGeologyLayerNames[i]);

		if(pDoc->m_pProjectCaseEdit->vecGeologyLayerResolutions[i] > biggestRes)
		{
			biggestRes = pDoc->m_pProjectCaseEdit->vecGeologyLayerResolutions[i];
		}

		str.Format("%f", pDoc->m_pProjectCaseEdit->vecGeologyLayerResolutions[i]);

		m_ListLayerSetting.SetItemText(curIndex,2,str);

		m_ListLayerSetting.SetItemText(curIndex,3,pDoc->m_pProjectCaseEdit->vecGeologyLayerSimiTypes[i]);

		m_ListLayerSetting.SetItemText(curIndex,4,pDoc->m_pProjectCaseEdit->vecGeologyLayerSimiMethods[i]);

		curIndex ++;
	}




	for (int i = 0; i < pDoc->m_pProjectCaseEdit->g_vecstrTerrainLayers.size(); i++)
	{
		m_ListLayerSetting.InsertItem(LVIF_TEXT|LVIF_STATE, curIndex, "terrain", LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);

		m_ListLayerSetting.SetItemText(curIndex,1,pDoc->m_pProjectCaseEdit->vecTerrainLayerNames[i]);

		str.Format("%f", pDoc->m_pProjectCaseEdit->vecTerrainLayerResolutions[i]);

		if(pDoc->m_pProjectCaseEdit->vecTerrainLayerResolutions[i] > biggestRes)
		{
			biggestRes = pDoc->m_pProjectCaseEdit->vecTerrainLayerResolutions[i];
		}

		m_ListLayerSetting.SetItemText(curIndex,2,str);

		m_ListLayerSetting.SetItemText(curIndex,3,pDoc->m_pProjectCaseEdit->vecTerrainLayerSimiTypes[i]);

		m_ListLayerSetting.SetItemText(curIndex,4,pDoc->m_pProjectCaseEdit->vecTerrainLayerSimiMethods[i]);
		curIndex ++;
	}




	for (int i = 0; i < pDoc->m_pProjectCaseEdit->g_vecstrVegetationLayers.size(); i++)
	{
		m_ListLayerSetting.InsertItem(LVIF_TEXT|LVIF_STATE, curIndex, "vegetation", LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);

		m_ListLayerSetting.SetItemText(curIndex,1,pDoc->m_pProjectCaseEdit->vecVegetationLayerNames[i]);

		if(pDoc->m_pProjectCaseEdit->vecVegetationLayerResolutions[i] > biggestRes)
		{
			biggestRes = pDoc->m_pProjectCaseEdit->vecVegetationLayerResolutions[i];
		}

		str.Format("%f", pDoc->m_pProjectCaseEdit->vecVegetationLayerResolutions[i]);

		m_ListLayerSetting.SetItemText(curIndex,2,str);

		m_ListLayerSetting.SetItemText(curIndex,3,pDoc->m_pProjectCaseEdit->vecVegetationLayerSimiTypes[i]);

		m_ListLayerSetting.SetItemText(curIndex,4,pDoc->m_pProjectCaseEdit->vecVegetationLayerSimiMethods[i]);

		curIndex ++;
	}




	for (int i = 0; i <  pDoc->m_pProjectCaseEdit->g_vecstrOtherLayers.size(); i++)
	{
		m_ListLayerSetting.InsertItem(LVIF_TEXT|LVIF_STATE, curIndex, "other", LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);

		m_ListLayerSetting.SetItemText(curIndex,1, pDoc->m_pProjectCaseEdit->vecOtherLayerNames[i]);

		if(pDoc->m_pProjectCaseEdit->vecOtherLayerResolutions[i] > biggestRes)
		{
			biggestRes = pDoc->m_pProjectCaseEdit->vecOtherLayerResolutions[i];
		}

		str.Format("%f", pDoc->m_pProjectCaseEdit->vecOtherLayerResolutions[i]);

		m_ListLayerSetting.SetItemText(curIndex,2,str);

		m_ListLayerSetting.SetItemText(curIndex,3,pDoc->m_pProjectCaseEdit->vecOtherLayerSimiTypes[i]);

		m_ListLayerSetting.SetItemText(curIndex,4,pDoc->m_pProjectCaseEdit->vecOtherLayerSimiMethods[i]);

		curIndex ++;
	}


	this->m_dFinalRes = biggestRes;
	this->dstRes = this->m_dFinalRes;
	m_ListLayerSetting.m_dCellSize = biggestRes;

	m_ListLayerSetting.Invalidate(true);
	m_ListLayerSetting.Invalidate(false);




	// int nFields =DBFGetFieldCount( pDoc->m_pProjectCaseEdit->hDBF );

	int nFields = pDoc->m_pProjectCaseEdit->hCSV->GetNumberOfVariables();
	//int nRec = DBFGetRecordCount( pDoc->m_pProjectCaseEdit->hDBF );

	int nRec = pDoc->m_pProjectCaseEdit->hCSV->GetNumberOfSamples(0) -1;

	//int iID = DBFGetFieldIndex(pDoc->m_pProjectCaseEdit->hDBF, "ID");

	//int iID = pDoc->m_pProjectCaseEdit->hCSV->GetVariableIndex("ID");


	// add field names to combobox and listctrl
	char fieldName[100];
	int width, deci;


	while (listAllField.GetCount() > 0)
		listAllField.DeleteString(0);


	//added by Fei
	while (listAllType.GetCount() > 0)
		listAllType.DeleteString(0);




	int index = 1;
	for (int j = 0; j < nFields; ++j)
	{
		//DBFGetFieldInfo(pDoc->m_pProjectCaseEdit->hDBF, j, fieldName, &width, &deci);

		pDoc->m_pProjectCaseEdit->hCSV->GetVariableName(j, fieldName);



		listAllField.AddString(fieldName);
		comboTypeField.AddString(fieldName);
		UpdateData(False);
	}




	if(pDoc->m_pProjectCaseEdit->iCBROption == 1)
	{  

		this->m_curSelTab = 0;
		this->m_tabPropertyType.SetCurSel(0);


		//uncertaintyCtrl.ShowWindow(SW_SHOW);
		uncertaintyCtrl.ShowWindow(SW_HIDE);
		propertyFolderCtrl.ShowWindow(SW_SHOW);
		thresholdPropertyCtrl.ShowWindow(SW_SHOW);

		m_staticSelectProperty.ShowWindow(SW_SHOW);

		m_staticSaveType.ShowWindow(SW_SHOW);
		m_comboSaveType.ShowWindow(SW_SHOW);



		listAllField.ShowWindow(SW_SHOW);

		m_frameResult.ShowWindow(SW_SHOW);
		m_staticSaveMap.ShowWindow(SW_SHOW);
		m_btnSaveMap.ShowWindow(SW_SHOW);
		m_staticThreshold.ShowWindow(SW_SHOW);
		//m_staticSaveUncertainty.ShowWindow(SW_SHOW);
		m_staticSaveUncertainty.ShowWindow(SW_HIDE);
		//m_btnSaveUncertainty.ShowWindow(SW_SHOW);
		m_btnSaveUncertainty.ShowWindow(SW_HIDE);


		comboTypeField.ShowWindow(SW_HIDE);
		listAllType.ShowWindow(SW_HIDE);

		m_staticTypeField.ShowWindow(SW_HIDE);
		m_staticSelectType.ShowWindow(SW_HIDE);
		m_frameResultType.ShowWindow(SW_HIDE);
		m_staticSaveTypeResult.ShowWindow(SW_HIDE);
		m_editTypeResultFolder.ShowWindow(SW_HIDE);
		m_btnSaveTypeResult.ShowWindow(SW_HIDE);

		//m_radioProperty.SetCheck(TRUE);
		//m_radioType.SetCheck(FALSE);
		/*GetDlgItem(IDC_LIST_ALL_FIELD)->EnableWindow(true);


		GetDlgItem(IDC_COMBO_TYPE_FIELD)->EnableWindow(false);
		GetDlgItem(IDC_LIST_ALL_TYPE)->EnableWindow(false);



		this->GetDlgItem(IDC_EDIT_Uncertaintyfilename)->EnableWindow(true);
		this->GetDlgItem(IDC_EDIT_Valuesfoldername)->EnableWindow(true);
		this->GetDlgItem(IDC_EDIT_THRESHOLD_PROPERTY)->EnableWindow(true);*/



	}
	else
	{
		this->m_curSelTab = 1;
		this->m_tabPropertyType.SetCurSel(1);

		uncertaintyCtrl.ShowWindow(SW_HIDE);
		propertyFolderCtrl.ShowWindow(SW_HIDE);
		thresholdPropertyCtrl.ShowWindow(SW_HIDE);


		listAllField.ShowWindow(SW_HIDE);


		m_frameResult.ShowWindow(SW_HIDE);
		m_staticSaveMap.ShowWindow(SW_HIDE);
		m_btnSaveMap.ShowWindow(SW_HIDE);
		m_staticThreshold.ShowWindow(SW_HIDE);
		m_staticSaveUncertainty.ShowWindow(SW_HIDE);
		m_btnSaveUncertainty.ShowWindow(SW_HIDE);

		m_staticSaveType.ShowWindow(SW_HIDE);
		m_comboSaveType.ShowWindow(SW_HIDE);



		m_staticSelectProperty.ShowWindow(SW_HIDE);

		comboTypeField.ShowWindow(SW_SHOW);
		listAllType.ShowWindow(SW_SHOW);

		m_staticTypeField.ShowWindow(SW_SHOW);
		m_staticSelectType.ShowWindow(SW_SHOW);
		m_frameResultType.ShowWindow(SW_SHOW);
		m_staticSaveTypeResult.ShowWindow(SW_SHOW);
		m_editTypeResultFolder.ShowWindow(SW_SHOW);
		m_btnSaveTypeResult.ShowWindow(SW_SHOW);


		/*GetDlgItem(IDC_COMBO_TYPE_FIELD)->EnableWindow(true);
		GetDlgItem(IDC_LIST_ALL_TYPE)->EnableWindow(true);


		GetDlgItem(IDC_LIST_ALL_FIELD)->EnableWindow(false);




		this->GetDlgItem(IDC_EDIT_Uncertaintyfilename)->EnableWindow(false);
		this->GetDlgItem(IDC_EDIT_Valuesfoldername)->EnableWindow(false);
		this->GetDlgItem(IDC_EDIT_THRESHOLD_PROPERTY)->EnableWindow(false);*/



	}




	m_comboSampleMethod.AddString("Limiting Factor");
	m_comboSampleMethod.AddString("Average Weighed");
	m_comboSampleMethod.AddString("User Defined");



	if(pDoc->m_pProjectCaseEdit->m_iCaseInteMethod == 0)
	{
		m_comboSampleMethod.SetCurSel(0);


		GetDlgItem(IDC_BUTTON_CASEMETHOD_SETTING)->ShowWindow(SW_HIDE);

	}
	else if(pDoc->m_pProjectCaseEdit->m_iCaseInteMethod == 1)
	{

		m_comboSampleMethod.SetCurSel(1);

		GetDlgItem(IDC_BUTTON_CASEMETHOD_SETTING)->ShowWindow(SW_HIDE);


	}
	else
	{
		m_comboSampleMethod.SetCurSel(2);

		GetDlgItem(IDC_BUTTON_CASEMETHOD_SETTING)->ShowWindow(SW_SHOW);

		bool hasClimate = true, hasVeg = true, hasOther = true;


		if(pDoc->m_pProjectCaseEdit->g_vecstrClimateLayers.size() == 0)
		{

			pDoc->m_pProjectCaseEdit->m_dClimateCut = 0;

			pDoc->m_pProjectCaseEdit ->m_dClimateWeight = 0;
			hasClimate = false;

		}
		else
		{
			if(fabs(pDoc->m_pProjectCaseEdit->m_dClimateCut - 0) < VERYSMALL)
				pDoc->m_pProjectCaseEdit->m_dClimateCut = 0.5;
		}



		if(pDoc->m_pProjectCaseEdit->g_vecstrVegetationLayers.size() == 0)
		{
			pDoc->m_pProjectCaseEdit ->m_dVegWeight = 0;
			hasVeg = false;
		}



		if(pDoc->m_pProjectCaseEdit->g_vecstrOtherLayers.size() == 0)
		{
			pDoc->m_pProjectCaseEdit ->m_dOtherWeight = 0;
			hasOther = false;
		}


		if( fabs(pDoc->m_pProjectCaseEdit ->m_dClimateWeight + pDoc->m_pProjectCaseEdit->m_dTopoWeight + pDoc->m_pProjectCaseEdit ->m_dVegWeight + pDoc->m_pProjectCaseEdit ->m_dOtherWeight - 1) > VERYSMALL)
		{
			if(hasClimate == false && hasVeg == false && hasOther == false)
			{
				pDoc->m_pProjectCaseEdit ->m_dClimateWeight = 0;
				pDoc->m_pProjectCaseEdit->m_dTopoWeight = 1;
				pDoc->m_pProjectCaseEdit ->m_dVegWeight = 0;
				pDoc->m_pProjectCaseEdit ->m_dOtherWeight = 0;
			}
			else if(hasClimate == true && hasVeg == true && hasOther == true)
			{
				pDoc->m_pProjectCaseEdit ->m_dClimateWeight = 0.25;
				pDoc->m_pProjectCaseEdit->m_dTopoWeight = 0.25;
				pDoc->m_pProjectCaseEdit ->m_dVegWeight = 0.25;
				pDoc->m_pProjectCaseEdit ->m_dOtherWeight = 0.25;
			}
			else if(hasClimate == true && hasVeg == true && hasOther == false)
			{
				pDoc->m_pProjectCaseEdit ->m_dClimateWeight = 0.33333333;
				pDoc->m_pProjectCaseEdit->m_dTopoWeight = 0.33333333;
				pDoc->m_pProjectCaseEdit ->m_dVegWeight = 0.33333333;
				pDoc->m_pProjectCaseEdit ->m_dOtherWeight = 0;
			}

			else if(hasClimate == true && hasVeg == false && hasOther == true)
			{
				pDoc->m_pProjectCaseEdit ->m_dClimateWeight = 0.33333333;
				pDoc->m_pProjectCaseEdit->m_dTopoWeight = 0.33333333;
				pDoc->m_pProjectCaseEdit ->m_dVegWeight = 0;
				pDoc->m_pProjectCaseEdit ->m_dOtherWeight = 0.33333333;
			}

			else if(hasClimate == false && hasVeg == true && hasOther == true)
			{
				pDoc->m_pProjectCaseEdit ->m_dClimateWeight = 0;
				pDoc->m_pProjectCaseEdit->m_dTopoWeight = 0.33333333;
				pDoc->m_pProjectCaseEdit ->m_dVegWeight = 0.33333333;
				pDoc->m_pProjectCaseEdit ->m_dOtherWeight = 0.33333333;
			}

			else if(hasClimate == false && hasVeg == false && hasOther == true)
			{
				pDoc->m_pProjectCaseEdit ->m_dClimateWeight = 0;
				pDoc->m_pProjectCaseEdit->m_dTopoWeight = 0.5;
				pDoc->m_pProjectCaseEdit ->m_dVegWeight = 0.0;
				pDoc->m_pProjectCaseEdit ->m_dOtherWeight = 0.5;
			}

			else if(hasClimate == false && hasVeg == true && hasOther == false)
			{
				pDoc->m_pProjectCaseEdit ->m_dClimateWeight = 0;
				pDoc->m_pProjectCaseEdit->m_dTopoWeight = 0.5;
				pDoc->m_pProjectCaseEdit ->m_dVegWeight = 0.5;
				pDoc->m_pProjectCaseEdit ->m_dOtherWeight = 0;
			}

			else if(hasClimate == true && hasVeg == false && hasOther == false)
			{
				pDoc->m_pProjectCaseEdit ->m_dClimateWeight = 0.5;
				pDoc->m_pProjectCaseEdit->m_dTopoWeight = 0.5;
				pDoc->m_pProjectCaseEdit ->m_dVegWeight = 0;
				pDoc->m_pProjectCaseEdit ->m_dOtherWeight = 0;
			}
		}



	}



	this->m_thresholdProperty = pDoc->m_pProjectCaseEdit->m_thresholdProperty;


	UpdateData(false);
	m_ListLayerSetting.Invalidate();


}



CString CCBInferenceView::GetFilePath()
{
	CString strPath = "";
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.hwndOwner = NULL; //m_hWnd;
	bInfo.lpszTitle = _T("Select Path: ");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;    

	LPITEMIDLIST lpDlist; 
	lpDlist = SHBrowseForFolder(&bInfo) ; 
	if(lpDlist != NULL) 
	{
		TCHAR chPath[255]; 
		SHGetPathFromIDList(lpDlist, chPath); 
		strPath = chPath; 
	}
	return strPath;
}



/*

void CCBInferenceView::generateAttributeFilesForType()
{

CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();

CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());

CString filename = strOutputPath + "\\attribute.txt";
pDoc->m_pProjectCaseEdit->g_attrSample = filename;
ofstream output(filename.GetBuffer());



output << "Count:  " << 0 << endl;
output << "AttributeCount:  " << 0<< endl;



output.close();
}



CString CCBInferenceView::generateFilesForType(CString type, CString typeName,vector<CString>& vecFiles)
{

CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());

double X=0;
double Y=0;	//坐标
int iRow=0;
int iCol=0;	//行列号
int iTotalSampleCount =  pDoc->m_pProjectCaseEdit->vecSampleData.size();
int iSampleCount = 0;
for(int i = 0; i < iTotalSampleCount; i ++)
{
if(pDoc->m_pProjectCaseEdit->vecSampleData[i][typeFieldIndex] == typeName)
iSampleCount ++;

}

ofstream fileOutput;
int layerCount = vecFiles.size();
vector<vector<float> > vecData;
vecData.resize(iSampleCount);
for (int k=0; k<iSampleCount; k++)
{
vecData[k].resize(layerCount);
}
CString oFileName;
oFileName.Format("%s\\%s.txt", strOutputPath,type);
fileOutput.open(oFileName);

CString strLayerName;
fileOutput << "AttributeCount: " << vecFiles.size() << endl;
fileOutput<<"ID"<<'\t'<<"X"<<'\t'<<"Y"<<'\t';
for (int i=0; i<vecFiles.size(); i++)
{
CThreeDR threedr;
strLayerName = vecFiles[i];
threedr.readin(strLayerName.GetBuffer(),0);
fileOutput<<strLayerName.Mid(strLayerName.ReverseFind('\\')+1,strLayerName.ReverseFind('.')-strLayerName.ReverseFind('\\')-1)<<'\t';
CString strTmp;
int curNum = 0;
for (int j=0;j<pDoc->m_pProjectCaseEdit->vecSampleData.size();j++)
{
if(pDoc->m_pProjectCaseEdit->vecSampleData[j][typeFieldIndex] == typeName)
{
Y=atof(pDoc->m_pProjectCaseEdit->vecSampleData[j][pDoc->m_pProjectCaseEdit->yIndex].GetBuffer());
iRow=(Y-threedr.getYmin())/threedr.getCellSize();
strTmp = pDoc->m_pProjectCaseEdit->vecSampleData[j][pDoc->m_pProjectCaseEdit->xIndex];
X=atof(strTmp.GetBuffer());
iCol=(X-threedr.getXmin())/threedr.getCellSize();
if (iRow>=0 && iRow<threedr.getNumberOfRows() && iCol>=0 && iCol<threedr.getNumberOfColumns())
{
vecData[curNum][i] = threedr.getData(X, Y);
}
else
{
vecData[curNum][i] = -9999;
}
curNum ++;
}
}
}
fileOutput<<endl;
fileOutput << "SampleCount: " << iSampleCount << endl; 

int curNum = 0;

for (int i=0;i<iTotalSampleCount;i++)
{
if(pDoc->m_pProjectCaseEdit->vecSampleData[i][typeFieldIndex].GetBuffer() == typeName)
{
fileOutput<<pDoc->m_pProjectCaseEdit->vecSampleData[i][0]<<'\t'
//<<this->m_listctrlSampleData.GetItemText(i,1)<<'\t'
<<pDoc->m_pProjectCaseEdit->vecSampleData[i][pDoc->m_pProjectCaseEdit->xIndex]<<'\t'
<<pDoc->m_pProjectCaseEdit->vecSampleData[i][pDoc->m_pProjectCaseEdit->yIndex]<<'\t';
for (int j=0;j<layerCount;j++)
{
fileOutput << setprecision(6) <<vecData[curNum][j]<<'\t';
}
fileOutput<<endl;
curNum ++;
}
}
fileOutput.close();

return oFileName;
}





void CCBInferenceView::generateKdeFilesForType(CString type, CString typeName,vector<CString>& vecOriginalFile, vector<CString>& vecSample)
{
CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());

CString dic;
dic.Format("%s\\%s", strOutputPath, type);
CreateDirectory(dic.GetString(), NULL);

int iTotalSampleCount = pDoc->m_pProjectCaseEdit->vecSampleData.size();
int iSampleCount = 0;
for(int i = 0; i < iTotalSampleCount; i ++)
{
if(pDoc->m_pProjectCaseEdit->vecSampleData[i][typeFieldIndex].GetBuffer() == typeName)
iSampleCount ++;

}

CString outputfile;
for (unsigned int i = 0; i < vecOriginalFile.size(); ++i)
{
CThreeDR data;
data.readin(vecOriginalFile[i].GetBuffer(), 0);

CString dataname = getCoreFilename(vecOriginalFile[i]);
outputfile.Format("%s\\%s.txt", dic, dataname);

//testof << dataname << endl;

CDistribution kde;
ofstream output(outputfile);
output << dataname << endl;
output << "NumberOfPointsPerLine:\t" << kde.getNumOfPDE() << endl;
output << "SampleCount:\t" << iSampleCount << endl;
output << "ID	X	Y	value" << endl;
//calculate the stdDev of the whole raster
float* srcArray = data.getDataBuf();
int num = data.getNumberOfColumns() * data.getNumberOfRows();
float stdDev = calcStdDev(srcArray, num, data.getNullDataValue());
int count = num;
for(int i=0; i<num; i++)
{
if(srcArray[i] == data.getNullDataValue())
--count;
}
float bandWidth = (float)(1.06 * stdDev * pow((double)count, -1.0/5.0));
kde.setBandWidth(bandWidth);

//testof << "bandwidth: " << bandWidth << endl;

CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
int winSize = (int)(pDoc->m_pProjectCaseEdit->g_dstResolution / data.getCellSize()) + 1;
for (int j=0; j < pDoc->m_pProjectCaseEdit->vecSampleData.size(); j++)
{

if(pDoc->m_pProjectCaseEdit->vecSampleData[j][typeFieldIndex ].GetBuffer() == typeName)
{
double x = atof(pDoc->m_pProjectCaseEdit->vecSampleData[j][pDoc->m_pProjectCaseEdit->xIndex].GetBuffer());
double y = atof(pDoc->m_pProjectCaseEdit->vecSampleData[j][pDoc->m_pProjectCaseEdit->yIndex].GetBuffer());
//find the neigbourhood in the source data
vector<float> neighbors;
findNeighborhood(&data, x, y, winSize, neighbors);

//testof << "size:\t" << neighbors.size() << endl;
//for (int m = 0; m < neighbors.size(); ++m)
//{
//	testof << neighbors[m] << "\t";
//}

if (neighbors.size() > 0)
{
int num = neighbors.size();
float* pData = new float[num];
for(int m = 0; m < num; ++m)
pData[m] = neighbors[m];
kde.SetValue(neighbors.size(), pData);
kde.EstimatePD();
output << setprecision(6) <<pDoc->m_pProjectCaseEdit->vecSampleData[j][0]<<'\t'
<< x << "\t" << y << endl;
for( int i = 0; i < kde.getNumOfPDE(); i++ )
output << kde.getRangeAt(i) << "\t";
output << endl;
for( int i = 0; i < kde.getNumOfPDE(); i++ )
output << kde.getPDVal(i) << "\t";
output << endl;
delete pData;
}
else
output << endl << endl;
}

//testof << endl;
}
output.close();
vecSample.push_back(outputfile);
}
}


*/






void CCBInferenceView::OnCbnSelchangeComboCase()
{

	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());

	if (m_comboSampleMethod.GetCurSel() == 0)
	{

		pDoc->m_pProjectCaseEdit->m_iCaseInteMethod = 0;

		GetDlgItem(IDC_BUTTON_CASEMETHOD_SETTING)->ShowWindow(SW_HIDE);
	}

	else if(m_comboSampleMethod.GetCurSel() == 1)
	{
		pDoc->m_pProjectCaseEdit->m_iCaseInteMethod = 1;

		GetDlgItem(IDC_BUTTON_CASEMETHOD_SETTING)->ShowWindow(SW_HIDE);
	}
	else
	{

		pDoc->m_pProjectCaseEdit->m_iCaseInteMethod = 2;

		GetDlgItem(IDC_BUTTON_CASEMETHOD_SETTING)->ShowWindow(SW_SHOW);
	}
}



void CCBInferenceView::SaveData()
{

	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());
	UpdateData(true);

	int curIndex = 0;

	for(int i=0; i<pDoc->m_pProjectCaseEdit->g_vecstrClimateLayers.size(); i++)
	{
		pDoc->m_pProjectCaseEdit->vecClimateLayerSimiTypes[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 3);
		pDoc->m_pProjectCaseEdit->vecClimateLayerSimiMethods[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 4);	   
		curIndex ++;
	}

	for(int i=0; i<pDoc->m_pProjectCaseEdit->g_vecstrGeologyLayers.size(); i++)
	{
		pDoc->m_pProjectCaseEdit->vecGeologyLayerSimiTypes[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 3);

		CString str = pDoc->m_pProjectCaseEdit->vecGeologyLayerSimiTypes[i];
		pDoc->m_pProjectCaseEdit->vecGeologyLayerSimiMethods[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 4);	 


		str = pDoc->m_pProjectCaseEdit->vecGeologyLayerSimiMethods[i];
		curIndex ++;
	}


	for(int i=0; i<pDoc->m_pProjectCaseEdit->g_vecstrTerrainLayers.size(); i++)
	{
		pDoc->m_pProjectCaseEdit->vecTerrainLayerSimiTypes[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 3);
		pDoc->m_pProjectCaseEdit->vecTerrainLayerSimiMethods[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 4);	   
		curIndex ++;
	}

	for(int i=0; i<pDoc->m_pProjectCaseEdit->g_vecstrVegetationLayers.size(); i++)
	{
		pDoc->m_pProjectCaseEdit->vecVegetationLayerSimiTypes[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 3);
		pDoc->m_pProjectCaseEdit->vecVegetationLayerSimiMethods[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 4);	   
		curIndex ++;
	}

	for(int i=0; i<pDoc->m_pProjectCaseEdit->g_vecstrOtherLayers.size(); i++)
	{
		pDoc->m_pProjectCaseEdit->vecOtherLayerSimiTypes[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 3);
		pDoc->m_pProjectCaseEdit->vecOtherLayerSimiMethods[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 4);	   
		curIndex ++;
	}







	pDoc->m_pProjectCaseEdit->m_strUncertaintyFile = m_OutputUncertaintyFilename;

	pDoc->m_pProjectCaseEdit->m_strAttributeFolder = m_OutputPredictvaluesFoldername;

	pDoc->m_pProjectCaseEdit->m_thresholdProperty = m_thresholdProperty;

	pDoc->m_pProjectCaseEdit->m_strMaxSimiFolder = m_strMaxSimiFolder;





	pDoc->m_pProjectCaseEdit->vecPropertyToInfer.clear();


}



// ****************************** EXCUTE ******************************** //
void CCBInferenceView::OnBnClickedOk()
{


	CSoLIMApp* pApp=(CSoLIMApp*)AfxGetApp();


	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());


	UpdateData(true);

	int curIndex = 0;

	for(int i=0; i<pDoc->m_pProjectCaseEdit->g_vecstrClimateLayers.size(); i++)
	{
		pDoc->m_pProjectCaseEdit->vecClimateLayerSimiTypes[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 3);
		pDoc->m_pProjectCaseEdit->vecClimateLayerSimiMethods[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 4);	   
		curIndex ++;
	}

	for(int i=0; i<pDoc->m_pProjectCaseEdit->g_vecstrGeologyLayers.size(); i++)
	{
		pDoc->m_pProjectCaseEdit->vecGeologyLayerSimiTypes[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 3);

		CString str = pDoc->m_pProjectCaseEdit->vecGeologyLayerSimiTypes[i];
		pDoc->m_pProjectCaseEdit->vecGeologyLayerSimiMethods[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 4);	 


		str = pDoc->m_pProjectCaseEdit->vecGeologyLayerSimiMethods[i];
		curIndex ++;
	}


	for(int i=0; i<pDoc->m_pProjectCaseEdit->g_vecstrTerrainLayers.size(); i++)
	{
		pDoc->m_pProjectCaseEdit->vecTerrainLayerSimiTypes[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 3);
		pDoc->m_pProjectCaseEdit->vecTerrainLayerSimiMethods[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 4);	   
		curIndex ++;
	}

	for(int i=0; i<pDoc->m_pProjectCaseEdit->g_vecstrVegetationLayers.size(); i++)
	{
		pDoc->m_pProjectCaseEdit->vecVegetationLayerSimiTypes[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 3);
		pDoc->m_pProjectCaseEdit->vecVegetationLayerSimiMethods[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 4);	   
		curIndex ++;
	}

	for(int i=0; i<pDoc->m_pProjectCaseEdit->g_vecstrOtherLayers.size(); i++)
	{
		pDoc->m_pProjectCaseEdit->vecOtherLayerSimiTypes[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 3);
		pDoc->m_pProjectCaseEdit->vecOtherLayerSimiMethods[i] =  this->m_ListLayerSetting.GetItemText(curIndex, 4);	   
		curIndex ++;
	}







	pDoc->m_pProjectCaseEdit->m_strUncertaintyFile = m_OutputUncertaintyFilename;

	pDoc->m_pProjectCaseEdit->m_strAttributeFolder = m_OutputPredictvaluesFoldername;

	pDoc->m_pProjectCaseEdit->m_thresholdProperty = m_thresholdProperty;

	pDoc->m_pProjectCaseEdit->m_strMaxSimiFolder = m_strMaxSimiFolder;





	pDoc->m_pProjectCaseEdit->vecPropertyToInfer.clear();




	int numOfProperties=listAllField.GetCount();
	int state;
	bool bHasProperty=false;
	for(int i=0;i<numOfProperties;i++)
	{
		state=listAllField.GetCheck(i);
		if(state!=0)	
		{
			CString strItem;
			listAllField.GetText(i,strItem);
			pDoc->m_pProjectCaseEdit->vecPropertyToInfer.push_back(strItem);
			bHasProperty=true;
		} 
	}


	if(pDoc->m_pProjectCaseEdit->iCBROption == 1 && bHasProperty == false)
	{
		MessageBox("At least one property should be selected.");
		return;
	}



	pDoc->m_pProjectCaseEdit->vecTypeToInfer.clear();




	int numOfTypes=listAllType.GetCount();

	bool bHasType=false;
	for(int i=0;i<numOfTypes;i++)
	{
		state=listAllType.GetCheck(i);
		if(state!=0)	
		{
			CString strItem;
			listAllType.GetText(i,strItem);
			pDoc->m_pProjectCaseEdit->vecTypeToInfer.push_back(strItem);
			bHasType=true;
		} 
	}





	if(pDoc->m_pProjectCaseEdit->iCBROption == 2 && bHasType == false)
	{
		MessageBox("At least one type should be selected.");
		return;
	}





	m_ProgressBar.SetRange(0, 100);





	ProjectInfo info;

	bool folderExists = info.fillProjectInfo(pDoc->m_pProjectCaseEdit);

	if(folderExists == true)
	{
		int s = MessageBox("The folder you specified contains interim results from previous inference. If you didn't make any change to the project, choose yes to continue the inference. Otherwise, please choose another folder to hold the results.", "Continue Previous Inference?",MB_ICONQUESTION|MB_YESNO);//6: yes; 7: no;
		if(s == 7)  //no
			return;	 
	}





	m_ProgressBar.SetPos(10);


	vector< vector<EnViriable>* > vecvec;
	vecvec.reserve(5);
	vecvec.push_back(&info.climateVec);
	vecvec.push_back(&info.materialVec);
	vecvec.push_back(&info.topoVec);
	vecvec.push_back(&info.vegeVec);
	vecvec.push_back(&info.otherVec);

	vector<string> vecDescription;
	vecDescription.reserve(5);
	vecDescription.push_back("Climate");
	vecDescription.push_back("ParentMaterial");
	vecDescription.push_back("Topography");
	vecDescription.push_back("Vegetation");
	vecDescription.push_back("Others");

	//*******************************************************************************
	//environment characterization
	//characterization files
	vector<string> climateCharVec, materialCharVec, topoCharVec, vegeCharVec, otherCharVec;
	vector< vector<string>* > vecvecChar;
	vecvecChar.reserve(5);
	vecvecChar.push_back(&climateCharVec);
	vecvecChar.push_back(&materialCharVec);
	vecvecChar.push_back(&topoCharVec);
	vecvecChar.push_back(&vegeCharVec);
	vecvecChar.push_back(&otherCharVec);
	//count of the total input files
	int totalCount = 0;
	for (int i = 0; i < vecvec.size(); ++i)
		totalCount += vecvec[i]->size();


	envChar(info, totalCount, vecvec, vecDescription, vecvecChar);

	m_ProgressBar.SetPos(10);



	if(pDoc->m_pProjectCaseEdit->iCBROption == 1) //infer soil properties
	{


		//*******************************************************************************
		//generating sample files
		vector<string> climateSampleVec, materialSampleVec, topoSampleVec, vegeSampleVec, otherSampleVec;
		vector< vector<string>* > vecvecSamplePointFile;
		vecvecSamplePointFile.reserve(5);
		vecvecSamplePointFile.push_back(&climateSampleVec);
		vecvecSamplePointFile.push_back(&materialSampleVec);
		vecvecSamplePointFile.push_back(&topoSampleVec);
		vecvecSamplePointFile.push_back(&vegeSampleVec);
		vecvecSamplePointFile.push_back(&otherSampleVec);


		CBRSamplePoint::attrNameVec.clear();


		vector< CBRSamplePoint* > vecSamplePoint;
		generateSampleData(info, totalCount, vecvec, vecDescription, 
			vecvecSamplePointFile, vecSamplePoint);
		m_ProgressBar.SetPos(10);


		//*******************************************************************************
		//similarity calculation
		vector< vector< vector<string>* >* > vvvSimi;
		generateSimiFiles(info, totalCount, vecvec, vecDescription, vecvecChar, vecSamplePoint, vvvSimi);

		m_ProgressBar.SetPos(80);

		//integrate similarity files
		vector< vector<string>* > vecTypeSimi;
		integrateSimiByType(info, vecSamplePoint.size(), vecvec, vecDescription, vvvSimi, vecTypeSimi,vecSamplePoint);

		m_ProgressBar.SetPos(87);
		//integrate similarity files of various types
		vector<string> vecSimi;
		integrateSimiBySample(info, vecSamplePoint.size(), vecvec, vecDescription, vecTypeSimi, vecSimi, vecSamplePoint);


		m_ProgressBar.SetPos(95);






		//*******************************************************************************
		//uncertainty and output
		calcUncertainty(vecSimi, vecSamplePoint, info.output.uncertaintyFile, info.output.attributeFold);
		calcValues(vecSimi, vecSamplePoint, info.output.threshold, 
			info.output.uncertaintyFile, info.output.attributeFold);

		//free memory
		for (int i = 0; i < vvvSimi.size(); ++i)
		{
			vector< vector<string>* >* vvSimi = vvvSimi[i];
			for (int j = 0; j < vvSimi->size(); ++j)
			{
				vector<string>* vSimi = vvSimi->at(j);
				delete vSimi;
			}
			delete vvSimi;
		}

		for (int i = 0; i < vecTypeSimi.size(); ++i)
		{
			delete vecTypeSimi[i];
		}

		for (int i = 0; i < vecSamplePoint.size(); ++i)
		{
			delete vecSamplePoint[i];
		}


		m_ProgressBar.SetPos(100);


		MessageBox("Inference is finished.");
		m_ProgressBar.SetPos(0);
		this->m_editProgress.SetWindowText("");
	}


	else if(pDoc->m_pProjectCaseEdit->iCBROption == 2) //infer soil type
	{

		for(int i=0; i<pDoc->m_pProjectCaseEdit->vecTypeToInfer.size(); i++)
		{
			m_ProgressBar.SetPos(30 + i *70/pDoc->m_pProjectCaseEdit->vecTypeToInfer.size());


			//*******************************************************************************
			//generating sample files
			vector<string> climateSampleVec, materialSampleVec, topoSampleVec, vegeSampleVec, otherSampleVec;
			vector< vector<string>* > vecvecSamplePointFile;
			vecvecSamplePointFile.reserve(5);
			vecvecSamplePointFile.push_back(&climateSampleVec);
			vecvecSamplePointFile.push_back(&materialSampleVec);
			vecvecSamplePointFile.push_back(&topoSampleVec);
			vecvecSamplePointFile.push_back(&vegeSampleVec);
			vecvecSamplePointFile.push_back(&otherSampleVec);

			vector< CBRSamplePoint* > vecSamplePoint;

			string strTypeField = pDoc->m_pProjectCaseEdit-> m_strCurTypeField;
			string strType = pDoc->m_pProjectCaseEdit->vecTypeToInfer[i];

			generateSampleDataForType(info, totalCount, vecvec, vecDescription, 
				vecvecSamplePointFile, vecSamplePoint,strTypeField, strType );


			//*******************************************************************************
			//similarity calculation
			vector< vector< vector<string>* >* > vvvSimi;
			generateSimiFiles(info, totalCount, vecvec, vecDescription, vecvecChar, vecSamplePoint, vvvSimi);

			//integrate similarity files
			vector< vector<string>* > vecTypeSimi;
			integrateSimiByType(info, vecSamplePoint.size(), vecvec, vecDescription, vvvSimi, vecTypeSimi, vecSamplePoint);
			//integrate similarity files of various types
			vector<string> vecSimi;
			integrateSimiBySample(info, vecSamplePoint.size(), vecvec, vecDescription, vecTypeSimi, vecSimi , vecSamplePoint);



			//*******************************************************************************
			//uncertainty and output

			//string sampleFolder = info.output.tempFolder + sep + "sample";
			calcMaxSimilarityFile(vecSimi, vecSamplePoint, info.output.maxSimiFolder + sep + pDoc->m_pProjectCaseEdit->vecTypeToInfer[i].GetBuffer() + ".3dr");



			//free memory
			for (int i = 0; i < vvvSimi.size(); ++i)
			{
				vector< vector<string>* >* vvSimi = vvvSimi[i];
				for (int j = 0; j < vvSimi->size(); ++j)
				{
					vector<string>* vSimi = vvSimi->at(j);
					delete vSimi;
				}
				delete vvSimi;
			}

			for (int i = 0; i < vecTypeSimi.size(); ++i)
			{
				delete vecTypeSimi[i];
			}

			for (int i = 0; i < vecSamplePoint.size(); ++i)
			{
				delete vecSamplePoint[i];
			}

		}

		m_ProgressBar.SetPos(100);

		MessageBox("Inference is finished.");
		m_ProgressBar.SetPos(0);
		this->m_editProgress.SetWindowText("");

	}
















}







void CCBInferenceView::OnBnClickedButtonCasemethodSetting()
{
	// TODO: Add your control notification handler code here

	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());

	CCBUserDefinedParaDlg dlg;
	dlg.m_dClimateSimilarity = pDoc->m_pProjectCaseEdit->m_dClimateCut;
	dlg.m_dClimateWeight = pDoc->m_pProjectCaseEdit->m_dClimateWeight;
	dlg.m_dTerrainWeight = pDoc->m_pProjectCaseEdit->m_dTopoWeight;
	dlg.m_dVegetationWeight = pDoc->m_pProjectCaseEdit->m_dVegWeight;
	dlg.m_dOtherWeight = pDoc->m_pProjectCaseEdit->m_dOtherWeight;


	if(dlg.DoModal()== IDOK)
	{
		pDoc->m_pProjectCaseEdit->m_dClimateCut = dlg.m_dClimateSimilarity;
		pDoc->m_pProjectCaseEdit->m_dClimateWeight= dlg.m_dClimateWeight;
		pDoc->m_pProjectCaseEdit->m_dTopoWeight= dlg.m_dTerrainWeight;
		pDoc->m_pProjectCaseEdit->m_dVegWeight = dlg.m_dVegetationWeight;
		pDoc->m_pProjectCaseEdit->m_dOtherWeight = dlg.m_dOtherWeight;

	}
}







void CCBInferenceView::getThreeDRHeaderInfo(const char* filename, ThreeDRHeaderInfo* info)
{
	CThreeDR threedr;
	threedr.readHeader(filename);
	info->width = threedr.getNumberOfColumns();
	info->height = threedr.getNumberOfRows();
	info->noDataValue = threedr.getNullDataValue();
	info->cellSize = (float)threedr.getCellSize();
	info->xMin = (float)threedr.getXmin();
	info->yMin = (float)threedr.getYmin();
	info->dataMin = threedr.getDataMin();
	info->dataMax = threedr.getDataMax();
}

void CCBInferenceView::getThreeDRData(const char* filename, float* data)
{
	CThreeDR threedr;
	threedr.readin(filename, 0);
	size_t n = threedr.getNumberOfColumns() * threedr.getNumberOfRows() * sizeof(float);
	memcpy(data, threedr.getDataBuf(), n);
}


double CCBInferenceView::threeDR2Ascii(const char* filename, const char* outputfile)
{
	CThreeDR input;
	int readFlag = input.readin(filename, 0);
	double res = input.getCellSize();

	int nrow = input.getNumberOfRows();
	int ncol = input.getNumberOfColumns();
	int numOfPixels = nrow*ncol;

	FILE *fp;
	fp=fopen(outputfile, "w");

	fprintf(fp, "%s %d\n", "ncols        ", ncol);
	fprintf(fp, "%s %d\n", "nrows        ", nrow);
	fprintf(fp, "%s %f\n", "xllcorner    ", input.getXmin());
	fprintf(fp, "%s %f\n", "yllcorner    ", input.getYmin());
	fprintf(fp, "%s %f\n", "cellsize     ", input.getCellSize());
	fprintf(fp, "%s %f\n", "NODATA_value ", input.getNullDataValue()); 


	for(int i=0;i<nrow;i++)
	{
		int m = 100*i/nrow;
		for(int j=0;j<ncol;j++)
			fprintf(fp, "%f ", input.getDataBuf()[i*ncol+j]);
		fprintf(fp, "\n");
	}
	fclose(fp);

	return res;
}





bool CCBInferenceView::resampleAndClipThreeDR(const char* filename, float resolution, const char* outputfile, double west, int numOfCol, double north, int numOfRow)
{
	CThreeDR src3dr;
	src3dr.readin(filename, 0);



	float *pDataOutput = new float[numOfCol * numOfRow];


	for(int i=0; i<numOfRow; i++)
		for(int j=0; j<numOfCol; j++)
		{
			pDataOutput[i*numOfCol + j] = src3dr.getData(west + (j+0.5) *resolution, north -(i+0.5)*resolution);
		}








		/*
		double xMin = src3dr.getXmin();
		double yMin = src3dr.getYmin();
		double xMax = src3dr.getXmin() + src3dr.getNumberOfColumns() * src3dr.getCellSize();
		double yMax = src3dr.getYmin() + src3dr.getNumberOfRows() * src3dr.getCellSize();

		int colMin = src3dr.colOfAPoint(xMin);
		int colMax = src3dr.colOfAPoint(xMax);
		int rowMax = src3dr.rowOfAPoint(yMin);
		int rowMin = src3dr.rowOfAPoint(yMax);

		long newNumRows = (xMax - xMin)/resolution;
		long newNumCols = (yMax - yMin)/resolution;
		*/



		//long newNumRows = long( src3dr.getNumberOfRows() * src3dr.getCellSize() / resolution );
		//long newNumCols = long( src3dr.getNumberOfColumns() * src3dr.getCellSize() / resolution);

		//float *pDataOutput = new float[newNumCols * newNumRows];


		//	src3dr.resample(newNumCols, newNumRows, resolution, pDataOutput);

		CThreeDR *dst3dr = new CThreeDR();
		dst3dr->setCellSize(resolution);
		dst3dr->setNumberOfRows(numOfRow);
		dst3dr->setNumberOfColumns(numOfCol);


		dst3dr->setXmin(west);
		double newYMin = north - numOfRow * resolution;
		dst3dr->setYmin(newYMin);


		//dst3dr->setXmin(src3dr.getXmin());

		//double newYMin = src3dr.getYmin() + src3dr.getCellSize() * src3dr.getNumberOfRows()
		//	- dst3dr->getCellSize() * dst3dr->getNumberOfRows();
		//dst3dr->setYmin(newYMin);

		dst3dr->setNullDataValue(src3dr.getNullDataValue());
		dst3dr->setDataBuf(pDataOutput);
		dst3dr->setDataName(src3dr.getDataName());
		dst3dr->calcStat();
		dst3dr->writeout(outputfile, 0);
		delete dst3dr;

		return true;
}




bool CCBInferenceView::calKDEAndClip(const char* srcfile, float resolution, const char* outputfile, double west, int numOfCol, double north, int numOfRow)
{
	CThreeDR *src3dr = new CThreeDR();
	src3dr->readin(srcfile, 0);

	//ThreeDRHeaderInfo refHeader;
	//getThreeDRHeaderInfo(reffile, &refHeader);

	int dstNumRows = numOfRow;
	int dstNumCols = numOfCol;
	double dstCellSize = resolution;
	double dstXMin = west;
	double dstYMin = north - numOfRow * resolution;
	double dstYMax = north;

	double srcCellSize = src3dr->getCellSize();
	int srcNumRows = src3dr->getNumberOfRows();
	int srcNumCols = src3dr->getNumberOfColumns();

	//calculate kde
	CDistribution kde;
	ofstream output(outputfile);
	output << "NumberOfPointsPerLine:\t" << kde.getNumOfPDE() << endl;
	//calculate the stdDev of the whole raster
	float* srcArray = src3dr->getDataBuf();
	int num = srcNumRows * srcNumCols;
	float stdDev = calcStdDev(srcArray, num, src3dr->getNullDataValue());
	int count = num;
	for(int i=0; i<num; i++)
	{
		if(abs(srcArray[i] - src3dr->getNullDataValue()) < CBRZERO)
			--count;
	}
	src3dr->calcStat();
	float test = src3dr->getDataStd();
	float bandWidth = (float)(1.06 * stdDev * pow((double)count, -1.0/5.0));
	kde.setBandWidth(bandWidth);

	output << "NumberOfColumns:\t" << dstNumCols << endl;
	output << "NumberOfRows:\t" << dstNumRows << endl;
	output << "Xmin:\t" << dstXMin << endl;
	output << "Ymin:\t" << dstYMin << endl;
	output << "CellSize:\t" << dstCellSize << endl;

	int winSize = (int)(dstCellSize / srcCellSize) + 1;
	for (int i = 0; i < dstNumRows; ++i)
	{
		for (int j = 0; j < dstNumCols; ++j)
		{
			MSG msg;

			while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			double dstx = dstXMin + dstCellSize * (j + 0.5);
			double dsty = dstYMax - dstCellSize * (i + 0.5);
			//find the neigbourhood in the source data
			vector<float> neighbors;
			src3dr->findNeighborhood(dstx, dsty, winSize, neighbors);

			if (neighbors.size() > 0)
			{
				int num = neighbors.size();
				float* pData = new float[num];
				for(int m = 0; m < num; ++m)
					pData[m] = neighbors[m];
				kde.SetValue(neighbors.size(), pData);
				kde.EstimatePD();
				for( int i = 0; i < kde.getNumOfPDE(); i++ )
					output << kde.getRangeAt(i) << "\t";
				output << endl;
				for( int i = 0; i < kde.getNumOfPDE(); i++ )
					output << kde.getPDVal(i) << "\t";
				output << endl;
				delete pData;
			}
			else
				output << endl << endl;
		}
	}
	output.close();

	delete src3dr;

	return true;
}

/*****************************************************************
** Description:         generate a kde file according to the coordinates of sample points 
** Input: 
**        originalFile: the input 3dr file
**        dataname:     data description
**        dstRes:       destination resolution
**        sampleCount:  count of sample points
**        sampleX:      X coordinate of sample points
**        sampleY:      Y coordinate of sample points
** Output: 
**        outputfile:   the output kde file
****************************************************************/
void CCBInferenceView::generateKdeFile(const char* originalFile, const char* dataname, 
									   double dstRes, int sampleCount, 
									   double *sampleX, double *sampleY, const char* outputfile,
									   vector< CBRSamplePoint* >& vecSamplePoint, int iType, int iVar)
{
	CThreeDR data;
	data.readin(originalFile, 0);

	CDistribution kde;
	ofstream output(outputfile);
	output << dataname << endl;
	output << "NumberOfPointsPerLine:\t" << kde.getNumOfPDE() << endl;
	output << "SampleCount:\t" << sampleCount << endl;
	output << "ID	X	Y	value" << endl;
	//calculate the stdDev of the whole raster
	float* srcArray = data.getDataBuf();
	int num = data.getNumberOfColumns() * data.getNumberOfRows();
	float stdDev = calcStdDev(srcArray, num, data.getNullDataValue());
	int count = num;
	for(int i=0; i<num; i++)
	{
		if(abs(srcArray[i] - data.getNullDataValue()) < CBRZERO)
			--count;
	}
	float bandWidth = (float)(1.06 * stdDev * pow((double)count, -1.0/5.0));
	kde.setBandWidth(bandWidth);

	int winSize = (int)(dstRes / data.getCellSize()) + 1;
	for (int j=0; j < sampleCount; j++)
	{
		MSG msg;
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		double x = sampleX[j];
		double y = sampleY[j];
		//find the neighborhood in the source data
		vector<float> neighbors;
		data.findNeighborhood(x, y, winSize, neighbors);

		vector<Point>* vecPt = new vector<Point>();

		if (neighbors.size() > 0)
		{
			int num = neighbors.size();
			float* pData = new float[num];
			for(int m = 0; m < num; ++m)
				pData[m] = neighbors[m];
			kde.SetValue(neighbors.size(), pData);
			kde.EstimatePD();

			//char sId[255];
			//itoa(j+1, sId, 10);

			output << setprecision(6) << vecSamplePoint[j]->id << '\t' << x << "\t" << y << endl;
			for( int i = 0; i < kde.getNumOfPDE(); i++ )
				output << kde.getRangeAt(i) << "\t";
			output << endl;
			for( int i = 0; i < kde.getNumOfPDE(); i++ )
				output << kde.getPDVal(i) << "\t";
			output << endl;
			delete pData;

			//vecSamplePoint
			for( int i = 0; i < kde.getNumOfPDE(); i++ )
			{
				Point pt;
				pt.x = kde.getRangeAt(i);
				pt.y = kde.getPDVal(i);
				vecPt->push_back(pt);
			}
		}
		else
			output << endl << endl;

		//vecSamplePoint
		vecSamplePoint[j]->vecvecEnvInfo[iType]->at(iVar).vecPt = vecPt;
	}
	output.close();
}

//iType: the type index, for example, i of climate is 0
//iVar : the variable index in a type, for p_ann in climate type is 0
void CCBInferenceView::generateSingleValueFile(const char* originalFile, const char* dataname, 
											   int sampleCount, double *sampleX, double *sampleY, 
											   const char* outputfile, 
											   vector< CBRSamplePoint* >& vecSamplePoint, int iType, int iVar)

{
	CThreeDR data;
	data.readin(originalFile, 0);
	double dstRes = data.getCellSize();

	ofstream output(outputfile);
	output << dataname << endl;
	output << "SampleCount:\t" << sampleCount << endl;
	output << "ID	X	Y	value" << endl;

	for (int i = 0; i < sampleCount; i++)
	{
		MSG msg;
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		double x = sampleX[i];
		double y = sampleY[i];

		int iRow = (y - data.getYmin()) / dstRes;
		int iCol = (x - data.getXmin()) / dstRes;
		double val;
		if (iRow >= 0 && iRow < data.getNumberOfRows() 
			&& iCol >= 0 && iCol < data.getNumberOfColumns())
		{
			val = data.getData(x, y);
		}
		else
		{
			val = -9999;
		}
		//vecSamplePoint
		vecSamplePoint[i]->vecvecEnvInfo[iType]->at(iVar).value = val;

		output << setprecision(6) << vecSamplePoint[i]->id << "\t" << x << "\t" <<y << "\t" << val << endl;
	}
	output.close();
}




int CCBInferenceView::calcBooleanSimilarity(vector< CBRSamplePoint* >& vecSamplePoint, int iType, int iVar, 
											string& dataName, string& inputFile, string& outputFolder, 
											vector<string>* outputSimiVec)
{
	recursiveDeleteDir(outputFolder.c_str());
	::CreateDirectory(outputFolder.c_str(), NULL);
	string outputFilePre = outputFolder + sep + dataName;

	int nSample = vecSamplePoint.size();

	CThreeDR input3dr;
	input3dr.readin(inputFile.c_str(), 0);
	float* inputData = input3dr.getDataBuf();

	int nrow = input3dr.getNumberOfRows();
	int ncol = input3dr.getNumberOfColumns();

	for (int j = 0; j < nSample; j++)
	{
		MSG msg;
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

        CBRSamplePoint* pSample = vecSamplePoint[j];

	//	stringstream ss;
	//	ss << (j+1);

	    stringstream ss;
		ss << pSample->id;

		string outputFile = outputFilePre + ss.str() + ".3dr";

		outputSimiVec->push_back(outputFile);

		//if the file with the same name exists, do nothing
		CFileStatus status;
		if(CFile::GetStatus(outputFile.c_str(), status) == true) 
			continue;


		
		float sampleValue = pSample->vecvecEnvInfo[iType]->at(iVar).value;
		float* outputData = new float[nrow * ncol];

		for (int k = 0; k < nrow * ncol; k++)
		{
			if(abs(inputData[k] - sampleValue) < CBRZERO)
				outputData[k] = 1.0;
			else if(abs(inputData[k] + 9999) < CBRZERO)
				outputData[k] = -9999;
			else
				outputData[k] = 0.0;
		}

		CThreeDR* output3dr = new CThreeDR;
		output3dr->copyBasicFields(&input3dr);
		output3dr->setDataBuf(outputData);

		output3dr->calcStat();
		output3dr->setXmin(output3dr->getXmin());
		output3dr->setYmin(output3dr->getYmin());

		//output

		output3dr->setDataName((char*)dataName.c_str());
		output3dr->writeout(outputFile.c_str(), 0);
		delete output3dr;



		/*string asciiFilename = outputFile + ".txt";
		threeDR2Ascii(outputFile.c_str(), asciiFilename.c_str());*/

	}	

	return 0;
}

int CCBInferenceView::calcGowerSimilarity(vector< CBRSamplePoint* >& vecSamplePoint, int iType, int iVar, 
										  string& dataName, string& inputFile, string& outputFolder,
										  vector<string>* outputSimiVec)
{
	recursiveDeleteDir(outputFolder.c_str());
	::CreateDirectory(outputFolder.c_str(), NULL);
	string outputFilePre = outputFolder + sep + dataName;

	int nSample = vecSamplePoint.size();

	CThreeDR input3dr;
	input3dr.readin(inputFile.c_str(), 0);


	float max = input3dr.getDataMax(); 
	float min = input3dr.getDataMin();
	float range = max - min;

	float* inputData = input3dr.getDataBuf();

	int nrow = input3dr.getNumberOfRows();
	int ncol = input3dr.getNumberOfColumns();

	for (int j = 0; j < nSample; j++)
	{
		MSG msg;
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		CBRSamplePoint* pSample = vecSamplePoint[j];

		stringstream ss;
		ss << pSample->id;
		string outputFile = outputFilePre + ss.str() + ".3dr";

		outputSimiVec->push_back(outputFile);

		//if the file with the same name exists, do nothing
		CFileStatus status;
		if(CFile::GetStatus(outputFile.c_str(), status) == true) 
			continue;


		float sampleValue = pSample->vecvecEnvInfo[iType]->at(iVar).value;
		float* outputData = new float[nrow * ncol];

		for (int k = 0; k < nrow * ncol; k++)
		{

			if(abs(inputData[k] + 9999) < CBRZERO)
				outputData[k] = -9999;
			else
				outputData[k] = (1 - abs(inputData[k] - sampleValue) / range);
		}

		CThreeDR* output3dr = new CThreeDR;
		output3dr->copyBasicFields(&input3dr);
		output3dr->setDataBuf(outputData);

		output3dr->calcStat();
		output3dr->setXmin(output3dr->getXmin());
		output3dr->setYmin(output3dr->getYmin());

		//output



		output3dr->setDataName((char*)dataName.c_str());
		output3dr->writeout(outputFile.c_str(), 0);
		delete output3dr;



		/*string asciiFilename = outputFile + ".txt";
		threeDR2Ascii(outputFile.c_str(), asciiFilename.c_str());*/

	}	

	return 0;

}

int CCBInferenceView::calcCMSimilarity(vector< CBRSamplePoint* >& vecSamplePoint, int iType, int iVar, 
									   string& dataName, string& kdeFile/*, string& refFile*/, string& outputFolder,
									   vector<string>* outputSimiVec)
{

	recursiveDeleteDir(outputFolder.c_str());
	::CreateDirectory(outputFolder.c_str(), NULL);
	string outputFilePre = outputFolder + sep + dataName;

	int nCols, nRows;
	double xmin, ymin;
	float cellsize;

	//the each topoFile there is nSample similarity files
	ifstream fIn(kdeFile.c_str());
	string tmp;
	int numPoints;

	fIn >> tmp >> numPoints;
	fIn >> tmp >> nCols;
	fIn >> tmp >> nRows;
	fIn >> tmp >> xmin;
	fIn >> tmp >> ymin;
	fIn >> tmp >> cellsize;

	getline(fIn, tmp);

	string tmp1, tmp2;
	int nAll = nRows * nCols;

	//read data from file
	vector< vector<Point>* > vecData;
	vecData.resize(nAll);
	for (int j = 0; j < nAll; ++j)
	{
		MSG msg;
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		getline(fIn, tmp1);
		getline(fIn, tmp2);
		if (tmp1.empty())
			vecData[j] = NULL;
		else
		{
			stringstream parser1(tmp1);
			stringstream parser2(tmp2);

			vector<Point> *curVecPt = new vector<Point>();
			curVecPt->resize(numPoints);
			for (int k = 0; k < numPoints; ++k)
			{
				Point pt;
				parser1 >> pt.x;
				parser2 >> pt.y;
				(*curVecPt)[k] = pt;
			}
			vecData[j] = curVecPt;
		}
	}

	fIn.close();

	for (int t = 0; t < vecSamplePoint.size(); ++t)
	{
		MSG msg;
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		CBRSamplePoint* pSample = vecSamplePoint[t];

		stringstream ss;
		ss << pSample->id;
		string outputFile = outputFilePre + ss.str() + ".3dr";

		outputSimiVec->push_back(outputFile);



		//if the file with the same name exists, do nothing
		CFileStatus status;
		if(CFile::GetStatus(outputFile.c_str(), status) == true) 
			continue;


		vector<Point>* pVec = pSample->vecvecEnvInfo[iType]->at(iVar).vecPt;

		double s1 = CurveArea(*pVec);

		float* pSimi = new float[nAll];

		for (int j = 0; j < nAll; ++j)
		{
			MSG msg;
			while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			vector<Point>* curVecPt = vecData[j];
			if (curVecPt)
			{
				double s2 = CurveArea(*curVecPt);
				double ss = CrossArea(*curVecPt, *pVec);
				pSimi[j] = 2 * ss / (s1 + s2);

			}
			else
				pSimi[j] = -9999;
		}

		//CThreeDR input3dr;
		//input3dr.readin(refFile.c_str(), 0);

		CThreeDR* output3dr = new CThreeDR;
		//	output3dr->copyBasicFields(&input3dr);


		output3dr->setNumberOfColumns(nCols);
		output3dr->setNumberOfRows(nRows);
		output3dr->setCellSize(cellsize);
		output3dr->setXmin(xmin);
		output3dr->setYmin(ymin);





		output3dr->setDataBuf(pSimi);

		output3dr->calcStat();
		//output3dr->setXmin(output3dr->getXmin());
		//	output3dr->setYmin(output3dr->getYmin());

		//output


		output3dr->setDataName((char*)dataName.c_str());
		output3dr->writeout(outputFile.c_str(), 0);
		delete output3dr;


		/*string asciiFilename = outputFile + ".txt";
		threeDR2Ascii(outputFile.c_str(), asciiFilename.c_str());*/

	}

	for (int i = 0; i < vecData.size(); ++i)
	{
		delete vecData[i];
	}
	return 0;
}

int CCBInferenceView::recursiveDeleteDir(const char* sDirName)
{

	/*
	CFileFind tempFind; 
	char sTempFileFind[200] ;

	sprintf(sTempFileFind,"%s\\*.*",sDirName); 
	BOOL IsFinded = tempFind.FindFile(sTempFileFind); 
	while (IsFinded) 
	{ 
	IsFinded = tempFind.FindNextFile(); 

	if (!tempFind.IsDots()) 
	{ 
	char sFoundFileName[200]; 
	strcpy(sFoundFileName,tempFind.GetFileName().GetBuffer(200)); 

	if (tempFind.IsDirectory()) 
	{ 
	char sTempDir[200]; 
	sprintf(sTempDir,"%s\\%s",sDirName,sFoundFileName); 
	recursiveDeleteDir(sTempDir); 
	} 
	else 
	{ 
	char sTempFileName[200]; 
	sprintf(sTempFileName,"%s\\%s",sDirName,sFoundFileName); 
	DeleteFile(sTempFileName); 
	} 
	} 
	} 
	tempFind.Close(); 
	if(!RemoveDirectory(sDirName)) 
	{ 
	return FALSE; 
	}  */
	return TRUE;
}



void CCBInferenceView::envChar(ProjectInfo& info, int totalCount, vector< vector<EnViriable>* >& vecvec,
							   vector<string>& vecDescription, vector< vector<string>* >& vecvecChar)
{
	string charFolder = info.output.tempFolder + sep + "characterization";
	recursiveDeleteDir(charFolder.c_str());
	::CreateDirectory(charFolder.c_str(), NULL);





	//first get the boundary of common area

	double west, east, north, south;

	for (int i = 0; i < vecvec.size(); ++i)
	{

		MSG msg;

		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//for each variable type, such as climate, topography and so on
		vector<EnViriable>* pVec = vecvec[i];
		vector<string>* pVecChar = vecvecChar[i];
		string typeFolder = charFolder + sep + vecDescription[i];
		::CreateDirectory(typeFolder.c_str(), NULL);

		bool findFirstLayer = false;

		for (int j = 0; j < pVec->size(); ++j)
		{
			MSG msg;

			while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			EnViriable& enVar = pVec->at(j);

			CThreeDR src3dr;
			src3dr.readin(enVar.file.c_str(), 0);
			if(findFirstLayer == false)
			{
				west = src3dr.getXmin();			
				south = src3dr.getYmin();
				east = src3dr.getXmin() + src3dr.getCellSize() * src3dr.getNumberOfColumns();
				north = src3dr.getYmin() + src3dr.getCellSize() * src3dr.getNumberOfRows();

				enVar.numOfCol = src3dr.getNumberOfColumns();
				enVar.numOfRow = src3dr.getNumberOfRows();
				enVar.xMin = src3dr.getXmin();
				enVar.xMax = src3dr.getXmin() + src3dr.getCellSize() * src3dr.getNumberOfColumns();
				enVar.yMin = src3dr.getYmin();
				enVar.yMax = src3dr.getYmin() + src3dr.getCellSize() * src3dr.getNumberOfRows();

				findFirstLayer = true;

			}
			else
			{
				if(src3dr.getXmin() > west)
					west = src3dr.getXmin();
				if(src3dr.getYmin() > south)
					south = src3dr.getYmin();
				if(src3dr.getXmin() + src3dr.getCellSize() * src3dr.getNumberOfColumns() < east)
					east = src3dr.getXmin() + src3dr.getCellSize() * src3dr.getNumberOfColumns();
				if(src3dr.getYmin() + src3dr.getCellSize() * src3dr.getNumberOfRows() < north)
					north = src3dr.getYmin() + src3dr.getCellSize() * src3dr.getNumberOfRows();

				enVar.numOfCol = src3dr.getNumberOfColumns();
				enVar.numOfRow = src3dr.getNumberOfRows();

				enVar.xMin = src3dr.getXmin();
				enVar.xMax = src3dr.getXmin() + src3dr.getCellSize() * src3dr.getNumberOfColumns();
				enVar.yMin = src3dr.getYmin();
				enVar.yMax = src3dr.getYmin() + src3dr.getCellSize() * src3dr.getNumberOfRows();


				if(west >= east)
				{
					AfxMessageBox("The spatial extent of the layers in GIS database do not have overlap. Inference cannot be executed.");
					return;
				}
				if(south >= north)
				{
					AfxMessageBox("The spatial extent of the layers in GIS database do not have overlap. Inference cannot be executed.");
					return;
				}

			}
		}
	}





	int numOfCol;
	float xTimes = (east - west) / dstRes;
	if(fabs(xTimes - floor(xTimes)) < VERYSMALL)
		numOfCol = floor(xTimes);
	else
		numOfCol = floor(xTimes) + 1;



	int numOfRow;
	float yTimes = (north - south) / dstRes;
	if(fabs(yTimes - floor(yTimes)) < VERYSMALL)
		numOfRow = floor(yTimes);
	else
		numOfRow = floor(yTimes) + 1;









	int counter = 0;
	for (int i = 0; i < vecvec.size(); ++i)
	{

		MSG msg;

		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//for each variable type, such as climate, topography and so on
		vector<EnViriable>* pVec = vecvec[i];
		vector<string>* pVecChar = vecvecChar[i];
		string typeFolder = charFolder + sep + vecDescription[i];
		::CreateDirectory(typeFolder.c_str(), NULL);
		for (int j = 0; j < pVec->size(); ++j)
		{
			MSG msg;

			while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			EnViriable& enVar = pVec->at(j);
			string outputfile = "";
			if(enVar.simiCalInfo.simiType == SIMI_TYPE_SINGLEVALUE)
			{
				//for single value, resample or simply copy
				outputfile = typeFolder + sep + enVar.name + ".3dr";

				//if the file with the same name exists, do nothing
				CFileStatus status;
				if(CFile::GetStatus(outputfile.c_str(), status) != true) 
				{			   

					if (abs(enVar.resolution - dstRes) > RESZERO || abs(enVar.xMin - west) > VERYSMALL || enVar.numOfCol != numOfCol || abs(enVar.yMax - north) > VERYSMALL|| enVar.numOfRow != numOfRow )
					{
						resampleAndClipThreeDR(enVar.file.c_str(), dstRes, outputfile.c_str(), west, numOfCol, north, numOfRow);//, east, south, north);
					}
					else
					{
						::CopyFile(enVar.file.c_str(), outputfile.c_str(), 0);
					}
				}

			}
			else//pdf
			{
				//for pdf, calculate pdf by kde
				outputfile = typeFolder + sep + enVar.name + ".txt";
				//if the file with the same name exists, do nothing

				CFileStatus status;
				if(CFile::GetStatus(outputfile.c_str(), status) != true) 
				{
					calKDEAndClip(enVar.file.c_str(), dstRes, outputfile.c_str(), west, numOfCol, north, numOfRow);
				}
			}
			++counter;
			cout << "Step 1 of 4: Characterizing environmental factors... "
				<< int(counter*100.0/totalCount) << "%\n"; 

			CString str;
			str.Format("Characterizing environmental factors...%d%%", int(counter*100.0/totalCount) );
			this->m_editProgress.SetWindowText(str);
			pVecChar->push_back(outputfile);
		}
	}
	cout << "Step 1 of 4: Characterizing environmental factors is finished.\n\n";

	CString str;
	str.Format("Characterizing environmental factors is finished." );
	this->m_editProgress.SetWindowText(str);
}

void CCBInferenceView::generateSampleData(ProjectInfo& info, int totalCount, vector< vector<EnViriable>* >& vecvec,
										  vector<string>& vecDescription, 
										  vector< vector<string>* >& vecvecSamplePointFile,
										  vector< CBRSamplePoint* >& vecSamplePoint)
{
	string sampleFolder = info.output.tempFolder + sep + "sample";
	recursiveDeleteDir(sampleFolder.c_str());
	::CreateDirectory(sampleFolder.c_str(), NULL);

	//get sample point coordinates
	//DBFHandle hDBF = DBFOpen( info.samplePoint.file.c_str(), "rb" );

	CDataFile *hCSV = new CDataFile(info.samplePoint.file.c_str(), DF::RF_READ_AS_STRING);





	int nFields = hCSV->GetNumberOfVariables();
	//int nFields = hCSV
	//int sampleCount = DBFGetRecordCount( hDBF );

	int sampleCount = hCSV->GetNumberOfSamples(0)-1;
	double *sampleX = new double[sampleCount];
	double *sampleY = new double[sampleCount];



	for (int i = 0; i < sampleCount; ++i)
	{

		int ix = hCSV->GetVariableIndex(info.samplePoint.xField.c_str());
		int iy = hCSV->GetVariableIndex(info.samplePoint.yField.c_str());

		char strX[100];
		char strY[100];

		hCSV->GetData(ix, i, strX);
		hCSV->GetData(iy, i, strY);
		//sampleX[i] = DBFReadDoubleAttribute(hDBF, i, ix);
		//sampleY[i] = DBFReadDoubleAttribute(hDBF, i, iy);

		sampleX[i] = atof(strX);
		sampleY[i] = atof(strY);

		//build the vecSamplePoint framework
		CBRSamplePoint* pSamplePt = new CBRSamplePoint();
		//pSamplePt->id = i + 1;
		pSamplePt->x = sampleX[i];
		pSamplePt->y = sampleY[i];

		//store id

		int iID = hCSV->GetVariableIndex(info.samplePoint.IDField.c_str());
		char strID[100];
		hCSV->GetData(iID, i, strID);
		pSamplePt->id = atoi(strID);

		vecSamplePoint.push_back(pSamplePt);

	}

	//generate attribute files
	string attrFile = sampleFolder + sep + "attribute.txt";
	ofstream attrOutput(attrFile.c_str());
	attrOutput << "Count:  " << sampleCount << endl;
	attrOutput << "AttributeCount:  " << info.samplePoint.attributeVec.size() << endl;
	for (int i = 0; i < info.samplePoint.attributeVec.size(); ++i)
	{
		attrOutput << info.samplePoint.attributeVec[i] << "\t";
		//vecSamplePoint
		CBRSamplePoint::attrNameVec.push_back(info.samplePoint.attributeVec[i]);
	}
	attrOutput << endl;

	vector<int> indexVec;
	for (int i = 0; i < info.samplePoint.attributeVec.size(); ++i)
	{
		//int iField = DBFGetFieldIndex(hDBF, info.samplePoint.attributeVec[i].c_str());

		int iField =hCSV->GetVariableIndex(info.samplePoint.attributeVec[i].c_str());
		indexVec.push_back(iField);
	}

	for (int k = 0; k < sampleCount; ++k)
	{	
		attrOutput << vecSamplePoint[k]->id << "\t" << sampleX[k] << "\t" << sampleY[k] << "\t";
		for (int i = 0; i < info.samplePoint.attributeVec.size(); ++i)
		{
			char strVal[100];
			hCSV->GetData(indexVec[i], k, strVal);
			double val = atof(strVal);
			//double val = DBFReadDoubleAttribute(hDBF, k, indexVec[i]);
			attrOutput << val << "\t";
			//vecSamplePoint
			vecSamplePoint[k]->vecAttribute.push_back(val);
		}
		attrOutput << endl;
	}

	attrOutput.close();

	//DBFClose(hDBF);

	delete hCSV;
	hCSV = NULL;

	//build the framework of vecSamplePoint
	for (int iSample = 0; iSample < sampleCount; ++iSample)
	{
		for (int i = 0; i < vecvec.size(); ++i)
		{
			vector<EnvInfo>* pVecEnv = new vector<EnvInfo>();
			vector<EnViriable>* pVec = vecvec[i];
			for (int j = 0; j < pVec->size(); ++j)
			{
				EnvInfo envInfo;
				pVecEnv->push_back(envInfo);
			}
			vecSamplePoint[iSample]->vecvecEnvInfo.push_back(pVecEnv);
		}
	}
	int counter = 0;
	for (int i = 0; i < vecvec.size(); ++i)
	{
		//for each variable type, such as climate, topography and so on
		vector<EnViriable>* pVec = vecvec[i];
		vector<string>* pVecSample = vecvecSamplePointFile[i];
		string typeFolder = sampleFolder + sep + vecDescription[i];
		::CreateDirectory(typeFolder.c_str(), NULL);

		vector<string> singleVarVec;
		for (int j = 0; j < pVec->size(); ++j)
		{
			EnViriable& enVar = pVec->at(j);
			string outputfile = typeFolder + sep + enVar.name + ".txt";

			//CFileStatus status;
			//if(CFile::GetStatus(outputfile.c_str(), status) == true) 
			//continue;
			//vecSamplePoint
			singleVarVec.push_back(enVar.name);

			if(enVar.simiCalInfo.simiType == SIMI_TYPE_SINGLEVALUE)
			{
				generateSingleValueFile(enVar.file.c_str(), enVar.name.c_str(), sampleCount, 
					sampleX, sampleY, outputfile.c_str(), vecSamplePoint, i, j);
			}
			else
			{
				generateKdeFile(enVar.file.c_str(), enVar.name.c_str(), dstRes, 
					sampleCount, sampleX, sampleY, outputfile.c_str(), vecSamplePoint, i, j);
			}
			++counter;
			cout << "Step 2 of 4: Generating sample points... "
				<< int(counter*100.0/totalCount) << "%\n"; 

			CString str;
			str.Format("Generating sample points...%d%%", int(counter*100.0/totalCount) );
			this->m_editProgress.SetWindowText(str);

			pVecSample->push_back(outputfile);
		}
		CBRSamplePoint::varNameVec.push_back(singleVarVec);
	}
	delete sampleX;
	delete sampleY;
	cout << "Step 2 of 4: Generating sample points is finished.\n\n";

	CString str;
	str.Format("Generating sample points is finished." );
	this->m_editProgress.SetWindowText(str);
}

void CCBInferenceView::generateSampleDataForType(ProjectInfo& info, int totalCount, vector< vector<EnViriable>* >& vecvec,
												 vector<string>& vecDescription, 
												 vector< vector<string>* >& vecvecSamplePointFile,
												 vector< CBRSamplePoint* >& vecSamplePoint,string typeField, string typeName)
{
	string sampleFolder = info.output.tempFolder + sep + "sample_" + typeName;
	recursiveDeleteDir(sampleFolder.c_str());
	::CreateDirectory(sampleFolder.c_str(), NULL);



	CDataFile *hCSV = new CDataFile(info.samplePoint.file.c_str(), DF::RF_READ_AS_STRING);

	int nFields = hCSV->GetNumberOfVariables();

	int iTotalSampleCount = hCSV-> GetNumberOfSamples(0) - 1;
	int sampleCount = 0;
	for(int i = 0; i < iTotalSampleCount; i ++)
	{

		int index = hCSV->GetVariableIndex(typeField.c_str());


		char itemText[100];
		hCSV->GetData(index, i, itemText);
		if(strcmp(itemText,typeName.c_str()) == 0)
			sampleCount ++;

	}



	double *sampleX = new double[sampleCount];
	double *sampleY = new double[sampleCount];

	int curRec = 0;
	for (int i = 0; i < iTotalSampleCount; ++i)
	{


		int index = hCSV->GetVariableIndex(typeField.c_str());

		char itemText[100];
		hCSV->GetData(index, i, itemText);
		if(strcmp(itemText,typeName.c_str()) == 0)
		{	

			CBRSamplePoint* pSamplePt = new CBRSamplePoint();		

			int ix = hCSV->GetVariableIndex(info.samplePoint.xField.c_str());
			int iy = hCSV->GetVariableIndex(info.samplePoint.yField.c_str());

			char strX[100];
			char strY[100];

			hCSV->GetData(ix, i, strX);
			hCSV->GetData(iy, i, strY);

			sampleX[curRec] = atof(strX);
			sampleY[curRec] = atof(strY);

			//int iID = hCSV->GetVariableIndex("ID");

			int iID = hCSV->GetVariableIndex(info.samplePoint.IDField.c_str());
			char strID[100];
			hCSV->GetData(iID, i, strID);
			pSamplePt->id = atoi(strID);


			//build the vecSamplePoint framework

			//pSamplePt->id = curRec + 1;
			pSamplePt->x = sampleX[curRec];
			pSamplePt->y = sampleY[curRec];
			vecSamplePoint.push_back(pSamplePt);
			curRec ++;
		}
	}


	//generate attribute files
	string attrFile = sampleFolder + sep + "attribute.txt";
	ofstream attrOutput(attrFile.c_str());
	attrOutput << "Count:  " << 0 << endl;
	attrOutput << "AttributeCount:  " << 0 << endl;

	attrOutput.close();


	delete hCSV;
	hCSV = NULL;


	//build the framework of vecSamplePoint
	for (int iSample = 0; iSample < sampleCount; ++iSample)
	{
		for (int i = 0; i < vecvec.size(); ++i)
		{
			vector<EnvInfo>* pVecEnv = new vector<EnvInfo>();
			vector<EnViriable>* pVec = vecvec[i];
			for (int j = 0; j < pVec->size(); ++j)
			{
				EnvInfo envInfo;
				pVecEnv->push_back(envInfo);
			}
			vecSamplePoint[iSample]->vecvecEnvInfo.push_back(pVecEnv);
		}
	}
	int counter = 0;
	for (int i = 0; i < vecvec.size(); ++i)
	{
		//for each variable type, such as climate, topography and so on
		vector<EnViriable>* pVec = vecvec[i];
		vector<string>* pVecSample = vecvecSamplePointFile[i];
		string typeFolder = sampleFolder + sep + vecDescription[i];
		::CreateDirectory(typeFolder.c_str(), NULL);

		vector<string> singleVarVec;
		for (int j = 0; j < pVec->size(); ++j)
		{
			EnViriable& enVar = pVec->at(j);
			string outputfile = typeFolder + sep + enVar.name + ".txt";

			//CFileStatus status;
			//if(CFile::GetStatus(outputfile.c_str(), status) == true) 
			//continue;
			//vecSamplePoint
			singleVarVec.push_back(enVar.name);

			if(enVar.simiCalInfo.simiType == SIMI_TYPE_SINGLEVALUE)
			{
				generateSingleValueFile(enVar.file.c_str(), enVar.name.c_str(), sampleCount, 
					sampleX, sampleY, outputfile.c_str(), vecSamplePoint, i, j);
			}
			else
			{
				generateKdeFile(enVar.file.c_str(), enVar.name.c_str(), dstRes, 
					sampleCount, sampleX, sampleY, outputfile.c_str(), vecSamplePoint, i, j);
			}
			++counter;
			cout << "Step 2 of 4: Generating sample points... "
				<< int(counter*100.0/totalCount) << "%\n"; 

			CString str;
			str.Format("Generating sample points...%d%%", int(counter*100.0/totalCount) );
			this->m_editProgress.SetWindowText(str);



			pVecSample->push_back(outputfile);
		}
		CBRSamplePoint::varNameVec.push_back(singleVarVec);
	}
	delete sampleX;
	sampleX = NULL;
	delete sampleY;
	sampleY = NULL;
	cout << "Step 2 of 4: Generating sample points is finished.\n\n";

	CString str;
	str.Format("Generating sample points is finished." );
	this->m_editProgress.SetWindowText(str);

}


void CCBInferenceView::generateSimiFiles(ProjectInfo& info, int totalCount, 
										 vector< vector<EnViriable>* >& vecvec, vector<string>& vecDescription, vector< vector<string>* > vecvecChar,
										 vector< CBRSamplePoint* >& vecSamplePoint, 
										 vector< vector< vector<string>* >* >& vvvSimi)
{

	string simiFolder = info.output.tempFolder + sep + "similarity";
	recursiveDeleteDir(simiFolder.c_str());
	::CreateDirectory(simiFolder.c_str(), NULL);


	int size = vecvec.size();

	int counter = 0;
	for (int i = 0; i < vecvec.size(); ++i)
	{

		MSG msg;

		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}




		//for each variable type, such as climate, topography and so on
		vector<EnViriable>* pVec = vecvec[i];
		string typeFolder = simiFolder + sep + vecDescription[i];
		::CreateDirectory(typeFolder.c_str(), NULL);

		vector< vector<string>* >* vvSimi = new vector< vector<string>* >();
		for (int j = 0; j < pVec->size(); ++j)
		{

			cout << "Step 3 of 4: Generating similarity files... "
				<< int(counter*100.0/totalCount) << "%\n"; 

			CString str;

			str.Format("Generating similarity files...%d%%", int(counter*100.0/totalCount) );
			this->m_editProgress.SetWindowText(str);


			MSG msg;
			while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			vector<string>* vSimi = new vector<string>();
			EnViriable& enVar = pVec->at(j);
			string inputFile = vecvecChar[i]->at(j);
			string outputFolder = typeFolder + sep + enVar.name;
			if (enVar.simiCalInfo.simiMethod == SIMI_METHOD_GOWER)
			{
				calcGowerSimilarity(vecSamplePoint, i, j, enVar.name, inputFile, 
					outputFolder, vSimi);
			}
			else if (enVar.simiCalInfo.simiMethod == SIMI_METHOD_BOOLEAN)
			{
				calcBooleanSimilarity(vecSamplePoint, i, j, enVar.name, inputFile, 
					outputFolder, vSimi);
			}
			else if (enVar.simiCalInfo.simiMethod == SIMI_METHOD_CM)
			{
				calcCMSimilarity(vecSamplePoint, i, j, enVar.name, inputFile, 
					/*info.refFile,*/ outputFolder, vSimi);
			}
			vvSimi->push_back(vSimi);

			++counter;
			m_ProgressBar.SetPos(10 + counter*70.0/(float)(totalCount));



		}
		vvvSimi.push_back(vvSimi);
	}

}

void CCBInferenceView::integrateSimiLimitingForOneType(vector< vector<string>* >& vec3drFileName, int sampleCount,
													   string& outputFolder, vector<string>* pVecSimi,vector< CBRSamplePoint* >& vecSamplePoint)
{

	CThreeDR *p3drFirst = new CThreeDR();
	p3drFirst->readin(vec3drFileName[0]->at(0).c_str(), 0);
	int nrow = p3drFirst->getNumberOfRows();
	int ncol = p3drFirst->getNumberOfColumns();
	int nAll = nrow * ncol;

	for (int iSample = 0; iSample < sampleCount; ++iSample)
	{
		MSG msg;
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		CBRSamplePoint* pSample = vecSamplePoint[iSample];
		stringstream ss;
		ss << pSample->id;

		string dataName = "similarity" + ss.str();
		string outputFile = outputFolder + sep + dataName + ".3dr";


		float *outputData = new float[nAll];
		for (int iCell = 0; iCell < nAll; ++iCell)
		{
			outputData[iCell] = 1;
		}

		//loop the variables to get type level similarity
		for (int j = 0; j <vec3drFileName.size(); ++j)
		{
			CThreeDR *p3dr = new CThreeDR();
			p3dr->readin(vec3drFileName[j]->at(iSample).c_str(), 0);
			float* data = p3dr->getDataBuf();

			//limiting factor
			for (int iCell = 0; iCell < nAll; ++iCell)
			{
				if (data[iCell] < outputData[iCell])
					outputData[iCell] = data[iCell];
			}

			delete p3dr;
			p3dr = NULL;

		}
		//output file
		CThreeDR* output3dr = new CThreeDR;
		output3dr->copyBasicFields(p3drFirst);
		output3dr->setDataBuf(outputData);
		output3dr->calcStat();
		output3dr->setXmin(output3dr->getXmin());
		output3dr->setYmin(output3dr->getYmin());
		output3dr->setDataName(dataName.c_str());
		output3dr->writeout(outputFile.c_str(), 0);
		delete output3dr;

		pVecSimi->push_back(outputFile);

		/*string asciiFilename = outputFile + ".txt";
		threeDR2Ascii(outputFile.c_str(), asciiFilename.c_str());*/
	}
}

void CCBInferenceView::integrateSimiLimiting(vector< vector<string>* >& vec3drFileName, int sampleCount,
											 string& outputFolder, vector<string>* pVecSimi,vector< CBRSamplePoint* >& vecSamplePoint)
{

	CThreeDR *p3drFirst = new CThreeDR();
	p3drFirst->readin(vec3drFileName[0]->at(0).c_str(), 0);
	int nrow = p3drFirst->getNumberOfRows();
	int ncol = p3drFirst->getNumberOfColumns();
	int nAll = nrow * ncol;

	for (int iSample = 0; iSample < sampleCount; ++iSample)
	{
		MSG msg;
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		CBRSamplePoint* pSample = vecSamplePoint[iSample];
		stringstream ss;
		ss << pSample->id;

		string dataName = "final_similarity" + ss.str();
		string outputFile = outputFolder + sep + dataName + ".3dr";


		float *outputData = new float[nAll];
		for (int iCell = 0; iCell < nAll; ++iCell)
		{
			outputData[iCell] = 1;
		}

		//loop the variables to get type level similarity
		for (int j = 0; j <vec3drFileName.size(); ++j)
		{
			CThreeDR *p3dr = new CThreeDR();
			p3dr->readin(vec3drFileName[j]->at(iSample).c_str(), 0);
			float* data = p3dr->getDataBuf();

			//limiting factor
			for (int iCell = 0; iCell < nAll; ++iCell)
			{
				if (data[iCell] < outputData[iCell])
					outputData[iCell] = data[iCell];
			}

			delete p3dr;
			p3dr = NULL;

		}
		//output file
		CThreeDR* output3dr = new CThreeDR;
		output3dr->copyBasicFields(p3drFirst);
		output3dr->setDataBuf(outputData);
		output3dr->calcStat();
		output3dr->setXmin(output3dr->getXmin());
		output3dr->setYmin(output3dr->getYmin());
		output3dr->setDataName(dataName.c_str());
		output3dr->writeout(outputFile.c_str(), 0);
		delete output3dr;

		pVecSimi->push_back(outputFile);

		/*string asciiFilename = outputFile + ".txt";
		threeDR2Ascii(outputFile.c_str(), asciiFilename.c_str());*/
	}
}

void CCBInferenceView::integrateSimiAverage(vector< vector<string>* >& vec3drFileName, int sampleCount, 
											string& outputFolder, vector<string>* pVecSimi, int indexGeology, vector< CBRSamplePoint* >& vecSamplePoint)
{

	CThreeDR *p3drFirst = new CThreeDR();
	p3drFirst->readin(vec3drFileName[0]->at(0).c_str(), 0);
	int nrow = p3drFirst->getNumberOfRows();
	int ncol = p3drFirst->getNumberOfColumns();
	int nAll = nrow * ncol;


	for (int iSample = 0; iSample < sampleCount; ++iSample)
	{
		MSG msg;
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		CBRSamplePoint* pSample = vecSamplePoint[iSample];
		stringstream ss;
		ss << pSample->id;

		string dataName = "final_similarity" + ss.str();
		string outputFile = outputFolder + sep + dataName + ".3dr";


		float *outputData = new float[nAll];
		bool *isNoData = new bool[nAll];
		bool *isGeologySame = new bool[nAll];
		for (int iCell = 0; iCell < nAll; ++iCell)
		{
			outputData[iCell] = 0;
			isNoData[iCell] = false;
			isGeologySame[iCell] = true;
		}

		//loop the variables to get type level similarity

		for (int j = 0; j < vec3drFileName.size(); ++j)
		{
			CThreeDR *p3dr = new CThreeDR();
			p3dr->readin(vec3drFileName[j]->at(iSample).c_str(), 0);
			float* data = p3dr->getDataBuf();


			for (int iCell = 0; iCell < nAll; ++iCell)
			{


				if (abs(data[iCell] + 9999) < CBRZERO)
				{
					isNoData[iCell] = true;
					//break;
				}

				if(j == indexGeology)
				{
					if(fabs(data[iCell] - 1) < VERYSMALL)
					{
						isGeologySame[iCell] = false;
						//break;
					}
				}

				outputData[iCell] += data[iCell];

			}

			delete p3dr;
			p3dr = NULL;


		}



		for (int iCell = 0; iCell < nAll; ++iCell)
		{

			if (isNoData[iCell])
				outputData[iCell] = -9999;
			else if(isGeologySame[iCell] == false)
				outputData[iCell] = 0;
			else 
			{
				if(indexGeology >= 0)
					outputData[iCell] /= (vec3drFileName.size() - 1);
				else
					outputData[iCell] /= vec3drFileName.size();
			}
		}



		//output file
		CThreeDR* output3dr = new CThreeDR;
		output3dr->copyBasicFields(p3drFirst);
		output3dr->setDataBuf(outputData);
		output3dr->calcStat();
		output3dr->setXmin(output3dr->getXmin());
		output3dr->setYmin(output3dr->getYmin());
		output3dr->setDataName(dataName.c_str());
		output3dr->writeout(outputFile.c_str(), 0);
		delete output3dr;

		delete []isNoData;
		delete []isGeologySame;
		pVecSimi->push_back(outputFile);

		/*string asciiFilename = outputFile + ".txt";
		threeDR2Ascii(outputFile.c_str(), asciiFilename.c_str());*/
	}

	delete p3drFirst;
	p3drFirst = NULL;
}


void CCBInferenceView::integrateSimiUserDefined(vector< vector<string>* >& vec3drFileName, 
												double climateCut, vector<double>& vecWeight,
												int sampleCount,
												string& outputFolder, vector<string>* pVecSimi, int indexClimate, int indexGeology, vector< CBRSamplePoint* >& vecSamplePoint)
{

	CThreeDR *p3drFirst = new CThreeDR();
	p3drFirst->readin(vec3drFileName[0]->at(0).c_str(), 0);
	int nrow = p3drFirst->getNumberOfRows();
	int ncol = p3drFirst->getNumberOfColumns();
	int nAll = nrow * ncol;


	for (int iSample = 0; iSample < sampleCount; ++iSample)
	{
		MSG msg;
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}


		CBRSamplePoint* pSample = vecSamplePoint[iSample];
		stringstream ss;
		ss << pSample->id;

		string dataName = "simi" + ss.str();
		string outputFile = outputFolder + sep + dataName + ".3dr";

		float *outputData = new float[nAll];
		for (int iCell = 0; iCell < nAll; ++iCell)
		{
			outputData[iCell] = 0;
		}

		vector <CThreeDR *> p3drForASample;

		for (int k = 0; k < vec3drFileName.size(); ++k)
		{
			CThreeDR *p3dr = new CThreeDR();
			p3dr->readin(vec3drFileName[k]->at(iSample).c_str(), 0);
			p3drForASample.push_back(p3dr);
		}


		for (int iCell = 0; iCell < nAll; ++iCell)
		{
			int flag = 0;
			for (int j = 0; j < vec3drFileName.size(); ++j)
			{
				CThreeDR *p3dr = p3drForASample[j];
				float* data = p3dr->getDataBuf();			

				if (abs(data[iCell] + 9999) < CBRZERO)
				{
					flag = 1;
					break;
				}

				else if(j == indexClimate)
				{
					if (data[iCell] < climateCut)
					{
						flag = 3;
						break;
					}
				}
				else if (j == indexGeology)
				{
					if(abs(data[iCell]) < CBRZERO )
					{
						flag = 2;
						break;
					}
				}
				outputData[iCell] += data[iCell] * vecWeight[j];
			}
			if (flag == 1)
				outputData[iCell] = -9999;
			else if (flag == 2)
				outputData[iCell] = 0.0;
			else if (flag == 3)
				outputData[iCell] = 0.0;
		}

		//output file
		CThreeDR* output3dr = new CThreeDR;
		output3dr->copyBasicFields(p3drFirst);
		output3dr->setDataBuf(outputData);
		output3dr->calcStat();
		output3dr->setXmin(output3dr->getXmin());
		output3dr->setYmin(output3dr->getYmin());
		output3dr->setDataName(dataName.c_str());
		output3dr->writeout(outputFile.c_str(), 0);
		delete output3dr;

		pVecSimi->push_back(outputFile);


		for (int k = 0; k < vec3drFileName.size(); ++k)
		{
			delete p3drForASample[k];

		}

	}

	delete p3drFirst;
	p3drFirst = NULL;
}




//output: vecIntergratedSimi
void CCBInferenceView::integrateSimiByType(ProjectInfo& info, int sampleCount, 
										   vector< vector<EnViriable>* >& vecvec, vector<string>& vecDescription,
										   vector< vector< vector<string>* >* >& vvvSimi, 
										   vector< vector<string>* >& vecTypeSimi, vector< CBRSamplePoint* >& vecSamplePoint)
{
	string simiFolder = info.output.tempFolder + sep + "similarity";
	//for each variable type, such as climate, topography and so on


	/*vector< vector<EnViriable>* > vecvecs;
	vecvecs.reserve(5);
	vecvecs.push_back(&info.climateVec);
	vecvecs.push_back(&info.materialVec);
	vecvecs.push_back(&info.topoVec);
	vecvecs.push_back(&info.vegeVec);
	vecvecs.push_back(&info.otherVec);*/

	CString str;
	str.Format("Integrating similarity files by type....." );
	this->m_editProgress.SetWindowText(str);


	//int a = vecvecs.size();
	for (int i = 0; i < vecvec.size(); i++)
	{
		//vector<EnViriable>* pVec = vecvec[i];
		//no layers in this category
		if (vecvec[i]->size() == 0)
			continue;

		vector< vector<string>* >* vvSimi = vvvSimi[i];

		//read the files to memory
		//int nRow;
		//int nCol;
		vector< vector<string>* > vec3drFileName;
		for (int m = 0; m < vvSimi->size(); ++m)
		{
			vector<string>* pVec3drName = new vector<string>();
			vector<string>* vSimi = vvSimi->at(m);
			for (int n = 0; n < vSimi->size(); ++n)
			{
				string fileName = vSimi->at(n);
				pVec3drName->push_back(fileName);

			}
			vec3drFileName.push_back(pVec3drName);
		}

		//int nAll = nRow * nCol;
		vector<string>* pVecSimi = new vector<string>();
		string outputFolder = simiFolder + sep + vecDescription[i];

		integrateSimiLimitingForOneType(vec3drFileName, sampleCount, outputFolder, pVecSimi, vecSamplePoint);


		vecTypeSimi.push_back(pVecSimi);

		//free vec3drFileName
		for (int m = 0; m < vec3drFileName.size(); ++m)
		{
			delete vec3drFileName[m];
		}
	}
}


void CCBInferenceView::integrateSimiBySample(ProjectInfo& info, int sampleCount, 
											 vector< vector<EnViriable>* >& vecvec, vector<string>& vecDescription,
											 vector< vector<string>* >& vvSimi, 
											 vector<string>& vecSimi, vector< CBRSamplePoint* >& vecSamplePoint)
{
	string simiFolder = info.output.tempFolder + sep + "similarity" + sep + "integrated";
	recursiveDeleteDir(simiFolder.c_str());
	::CreateDirectory(simiFolder.c_str(), NULL);

	CString str;
	str.Format("Integrating similarity files by sample......" );
	this->m_editProgress.SetWindowText(str);


	vector< vector<string>* > vec3drFileName;

	for (int m = 0; m < vvSimi.size(); ++m)
	{
		vector<string>* pVec3drFileName = new vector<string>();

		vector<string> vSimi;

		for(int t=0; t< vvSimi[m]->size(); t++)
		{
			vSimi.push_back(vvSimi[m]->at(t));
		}

		//vector<string>* vSimi = vvSimi[m];

		for (int n = 0; n < vSimi.size(); ++n)
		{
			string fileName = vSimi[n];
			pVec3drFileName->push_back(fileName);

		}
		vec3drFileName.push_back(pVec3drFileName);
	}


	int indexGeology;

	if(vecvec[0]->size() > 0 && vecvec[1]->size() > 0)	
		indexGeology = 1;
	else if(vecvec[0]->size() <= 0 && vecvec[1]->size() > 0)
		indexGeology = 0;
	else
		indexGeology = -1;

	int indexClimate;

	if(vecvec[0]->size() > 0)		
		indexClimate = 0;
	else		
		indexClimate = -1;




	if (info.sampleLevelMethod.type == 0)//limiting
	{
		integrateSimiLimiting(vec3drFileName, sampleCount, simiFolder, &vecSimi, vecSamplePoint);
	}
	else if (info.sampleLevelMethod.type == 1)//average
	{
		integrateSimiAverage(vec3drFileName, sampleCount, simiFolder, &vecSimi, indexGeology,vecSamplePoint);
	}
	else
	{

		vector<double> vecWeight;

		if(indexClimate == 0)		
			vecWeight.push_back(info.sampleLevelMethod.climateWeight);	
		if(vecvec[1]->size() > 0)
			vecWeight.push_back(0.0);
		vecWeight.push_back(info.sampleLevelMethod.topoWeight);
		if(vecvec[3]->size() > 0)
			vecWeight.push_back(info.sampleLevelMethod.vegeWeight);
		if(vecvec[4]->size() > 0)
			vecWeight.push_back(info.sampleLevelMethod.otherWeight);

		double climateCut = info.sampleLevelMethod.climateCut;

		integrateSimiUserDefined(vec3drFileName, climateCut, vecWeight, sampleCount, simiFolder, &vecSimi, indexClimate,indexGeology, vecSamplePoint);
	}


	//free vec3drFileName
	for (int m = 0; m < vec3drFileName.size(); ++m)
	{
		delete vec3drFileName[m];
	}




	cout << "Step 3 of 4: Generating similarity files is finished.\n\n";


	str.Format("Generating similarity files is finished." );
	this->m_editProgress.SetWindowText(str);
}

/*
int CCBInferenceView::findMaxSum(vector<float>& inputarray, int &flag, float &max, float &sum)
{	
flag = 0; max = inputarray[0]; sum = 0;
for (int i = 0; i < inputarray.size(); i++)
{
if(inputarray[i] >= max)
{	
max = inputarray[i];
flag = i;
}
sum = sum + inputarray[i];
}
return 0;
}*/

int CCBInferenceView::calcUncertainty(vector<string>& vecSimi, vector< CBRSamplePoint* >& vecSamplePoint,
									  string& uncertaintyFile, string& outputFolder)
{
	cout << "Step 4 of 4: Calculating uncertainty and values..." << endl;

	CString str;
	str.Format("Calculating uncertainty and values...");
	this->m_editProgress.SetWindowText(str);


	int nsample = vecSimi.size();							//NO. of sample
	CThreeDR* simi3drFirst = new CThreeDR();				    //get similarity to sample one; all the similarities are dimension matched 

	simi3drFirst->readin(vecSimi[0].c_str(), 0);
	int nrow = simi3drFirst->getNumberOfRows();						//get the NO. of rows
	int ncol = simi3drFirst->getNumberOfColumns();					//get the NO. of columes

	float* Uncertainty_dataBuf = new float[nrow * ncol];
	int nAll = nrow * ncol;

	for (int iCell = 0; iCell < nAll; ++iCell)
	{
		Uncertainty_dataBuf[iCell] = -9999;
	}


	for (int j = 0; j < vecSimi.size(); ++j)
	{
		CThreeDR *threedr = new CThreeDR();
		threedr->readin(vecSimi[j].c_str(), 0);
		for (int i = 0; i < nAll; i++) 
		{
			if(threedr->getDataBuf()[i] > Uncertainty_dataBuf[i])
				Uncertainty_dataBuf[i] = threedr->getDataBuf()[i];
		}
		delete threedr;
	}


	for (int i = 0; i < nAll; i++) 
	{
		if (abs(Uncertainty_dataBuf[i] + 9999) < CBRZERO)
			Uncertainty_dataBuf[i] = -9999;
		else		 
			Uncertainty_dataBuf[i] = 1 -  Uncertainty_dataBuf[i];
	}
	CThreeDR* uncertainty = new CThreeDR;
	uncertainty->copyBasicFields(simi3drFirst);
	uncertainty->setDataBuf(Uncertainty_dataBuf);
	uncertainty->setDataName(extractFileName(uncertaintyFile.c_str()).GetBuffer(1));
	uncertainty->calcStat();
	uncertainty->setXmin(uncertainty->getXmin());
	uncertainty->setYmin(uncertainty->getYmin());
	uncertainty->setDataUnits(uncertainty->getDataUnits());

	CMainFrame* pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
	//uncertainty->setProjection(pDoc->m_pProjectCaseEdit->m_projection);

	/*CString outputFilename = uncertaintyFile.c_str();
	if (m_OutputUncertaintyFilename.Right(4) == ".3dr") {
		uncertainty->writeout(uncertaintyFile.c_str(),0);
	}
	else if (m_OutputUncertaintyFilename.Right(5) == ".sdat") {
		CString filename_3dr = m_OutputUncertaintyFilename.GetBuffer(0);
		filename_3dr.Replace(".sdat", ".3dr");
		uncertainty->writeout(filename_3dr.GetBuffer(0), 0);
		this->Convert_3drToGdal(uncertainty, outputFilename, "SAGA");

		// write the projection info of saga file
		char *projection = pDoc->m_pProjectCaseEdit->m_projection;
		if (projection != "") {
			CString filename_prj = m_OutputUncertaintyFilename.GetBuffer(0);
			filename_prj.Replace(".sdat", ".prj");
			FILE *pf = fopen(filename_prj, "w");
			fwrite(projection, 1, strlen(projection), pf);
			fflush(pf);
			fclose(pf);
		}
	}*/


	// write out
	CFileManager* fm = new CFileManager;
	string strExt = GetOutputExt();
	//m_OutputPredictvaluesFoldername
	string outvalfilename = outputFolder + "\\" + "uncertainty" + strExt;
	CString outputFullname = outvalfilename.c_str();
	fm->Writeout(uncertainty, outputFullname, GetOutputExt());

	
	delete uncertainty;
	delete simi3drFirst;
	simi3drFirst = NULL;


	return 0;
}


int CCBInferenceView::calcMaxSimilarityFile(vector<string>& vecSimi, vector< CBRSamplePoint* >& vecSamplePoint,
											string& maxSimilarityFile)
{
	cout << "Step 4 of 4: Calculating max similarity..." << endl;

	CString str;
	str.Format("Calculating max similarity...");
	this->m_editProgress.SetWindowText(str);

	int nsample = vecSimi.size();							//NO. of sample
	CThreeDR* simi3drFirst = new CThreeDR();				    //get similarity to sample one; all the similarities are dimension matched 

	simi3drFirst->readin(vecSimi[0].c_str(), 0);
	int nrow = simi3drFirst->getNumberOfRows();						//get the NO. of rows
	int ncol = simi3drFirst->getNumberOfColumns();					//get the NO. of columes


	float* MaxSimi_dataBuf = new float[nrow * ncol];

	int nAll = nrow * ncol;

	for (int iCell = 0; iCell < nAll; ++iCell)
	{
		MaxSimi_dataBuf[iCell] = -9999;
	}

	for (int j = 0; j < vecSimi.size(); ++j)
	{
		CThreeDR *threedr = new CThreeDR();
		threedr->readin(vecSimi[j].c_str(), 0);
		for (int i = 0; i < nAll; i++) 
		{
			if(threedr->getDataBuf()[i] > MaxSimi_dataBuf[i])
				MaxSimi_dataBuf[i] = threedr->getDataBuf()[i];
		}
		delete threedr;
	}


	for (int i = 0; i < nAll; i++) 
	{
		if (abs(MaxSimi_dataBuf[i] + 9999) < CBRZERO)
			MaxSimi_dataBuf[i] = -9999;
		else		 
			MaxSimi_dataBuf[i] = MaxSimi_dataBuf[i] * 100;
	}



	CThreeDR* maxSimi = new CThreeDR;
	maxSimi->copyBasicFields(simi3drFirst);
	maxSimi->setDataBuf(MaxSimi_dataBuf);
	maxSimi->calcStat();
	maxSimi->setXmin(maxSimi->getXmin());
	maxSimi->setYmin(maxSimi->getYmin());
	maxSimi->setDataUnits(maxSimi->getDataUnits());

	maxSimi->setDataName(extractFileName(maxSimilarityFile.c_str()).GetBuffer(1));

	maxSimi->writeout(maxSimilarityFile.c_str(),0);	
	delete maxSimi;


	delete simi3drFirst;
	simi3drFirst = NULL;

	return 0;
}

int CCBInferenceView::calcValues(vector<string>& vecSimi, vector< CBRSamplePoint* >& vecSamplePoint,
								 double threshold, string& uncertaintyFile, string& outputFolder)
{

	CFileManager* fm = new CFileManager;

	int nsample = vecSimi.size();							//NO. of sample
	CThreeDR* simi3drFirst = new CThreeDR();				    //get similarity to sample one; all the similarities are dimension matched 

	simi3drFirst->readin(vecSimi[0].c_str(), 0);
	int nrow = simi3drFirst->getNumberOfRows();						//get the NO. of rows
	int ncol = simi3drFirst->getNumberOfColumns();					//get the NO. of columes


	CThreeDR *uncertainty3dr = new CThreeDR;
	string strExt = GetOutputExt();
	string uncertaintyFilename_str = outputFolder + "\\" + "uncertainty" + strExt;
	CString uncertaintyFilename = uncertaintyFilename_str.c_str();
	uncertainty3dr = fm->Readin(uncertaintyFilename);

	/*if (m_OutputUncertaintyFilename.Right(4) == ".3dr") {
		uncertainty3dr->readin((char*)uncertaintyFile.c_str(), 0);
	}
	if (m_OutputUncertaintyFilename.Right(5) == ".sdat") {
		CString filename_3dr = m_OutputUncertaintyFilename.GetBuffer(0);
		filename_3dr.Replace(".sdat", ".3dr");
		uncertainty3dr->readin(filename_3dr.GetBuffer(0), 0);
		remove(filename_3dr.GetBuffer(0));
	}*/


	float No_Data = simi3drFirst->getNullDataValue();
	float* Uncertainty_dataBuf = uncertainty3dr->getDataBuf();		//Uncertainty


	const int nval = CBRSamplePoint::attrNameVec.size();                 //the NO. of values

	int nAll = nrow * ncol;


	for(int k = 0; k < nval; k++)  //for every soil property
	{
		CThreeDR* tempPredictvalue;

		string valuename;
		string outvalfilename;

		float* predicttemp = new float[nAll];
		for (int i = 0; i < nAll; i++) 
		{

			if(Uncertainty_dataBuf[i] >= threshold || abs(Uncertainty_dataBuf[i] + 9999) < CBRZERO)
			{
				predicttemp[i] = No_Data;
			}
		}

		float* maxS_dataBuf = new float[nAll];
		float* sumS_dataBuf = new float[nAll];
		float* sumSV_dataBuf = new float[nAll];
		int* flag_dataBuf = new int[nAll];

		for (int iCell = 0; iCell < nAll; ++iCell)
		{
			maxS_dataBuf[iCell] = -9999;
		}

		for (int iCell = 0; iCell < nAll; ++iCell)
		{
			sumS_dataBuf[iCell] = 0;
		}
		for (int iCell = 0; iCell < nAll; ++iCell)
		{
			sumSV_dataBuf[iCell] = 0;
		}

		for (int iCell = 0; iCell < nAll; ++iCell)
		{
			flag_dataBuf[iCell] = 0;
		}


		for (int j = 0; j < vecSimi.size(); ++j)
		{
			CThreeDR *threedr = new CThreeDR();
			threedr->readin(vecSimi[j].c_str(), 0);
			for (int i = 0; i < nAll; i++) 
			{
				if(abs(predicttemp[i] - No_Data) < CBRZERO)
					continue;
				if(threedr->getDataBuf()[i] > maxS_dataBuf[i])
				{
					maxS_dataBuf[i] = threedr->getDataBuf()[i];
					flag_dataBuf[i] = j;
				}
				sumS_dataBuf[i] += threedr->getDataBuf()[i];

				float val = vecSamplePoint[j]->vecAttribute[k];
				sumSV_dataBuf[i] = sumSV_dataBuf[i] + threedr->getDataBuf()[i] * val;
			}
			delete threedr;
		}

		for (int i = 0; i < nAll; i++) 
		{

			float mValue = vecSamplePoint[flag_dataBuf[i]]->vecAttribute[k];

			if(abs(predicttemp[i] - No_Data) < CBRZERO)
				predicttemp[i] = No_Data;
			else if(abs(sumS_dataBuf[i] - maxS_dataBuf[i]) < CBRZERO)
				predicttemp[i] = mValue;
			else
				predicttemp[i] = mValue * maxS_dataBuf[i] + (1.0 - maxS_dataBuf[i]) * (sumSV_dataBuf[i] - mValue * maxS_dataBuf[i]) / (sumS_dataBuf[i] - maxS_dataBuf[i]);
		}

		valuename = CBRSamplePoint::attrNameVec[k].c_str();
		outvalfilename = outputFolder + "\\" + "predicted_" + valuename + strExt;
		tempPredictvalue = new CThreeDR;
		tempPredictvalue->copyBasicFields(simi3drFirst);
		tempPredictvalue->setDataBuf(predicttemp);
		tempPredictvalue->calcStat();
		tempPredictvalue->setXmin(tempPredictvalue->getXmin());
		tempPredictvalue->setYmin(tempPredictvalue->getYmin());
		tempPredictvalue->setDataUnits(tempPredictvalue->getDataUnits());
		tempPredictvalue->setDataName(extractFileName(outvalfilename.c_str()).GetBuffer(1));
		

		CMainFrame* pFrame   =   (CMainFrame*)AfxGetMainWnd();
		CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
		tempPredictvalue->setProjection(pDoc->m_pProjectCaseEdit->m_projection);

		CString outputFilename = uncertaintyFile.c_str();
		/*if (m_OutputUncertaintyFilename.Right(4) == ".3dr") {
			outvalfilename = outputFolder + "\\" + "predicted_" + valuename + ".3dr";
            tempPredictvalue->writeout(outvalfilename.c_str(), 0);
		}
		else if (m_OutputUncertaintyFilename.Right(5) == ".sdat") {
			CString filename_3dr = m_OutputUncertaintyFilename.GetBuffer(0);
			filename_3dr.Replace(".sdat", ".3dr");
			remove(filename_3dr);
			outvalfilename = outputFolder + "\\" + "predicted_" + valuename + ".sdat";
			this->Convert_3drToGdal(tempPredictvalue, outvalfilename.c_str(), "SAGA");

			// write the projection info of saga file
			char *projection = pDoc->m_pProjectCaseEdit->m_projection;
			if (projection != "") {
				CString filename_prj = outvalfilename.c_str();
				filename_prj.Replace(".sdat", ".prj");
				FILE *pf = fopen(filename_prj, "w");
				fwrite(projection, 1, strlen(projection), pf);
				fflush(pf);
				fclose(pf);
			}
		}*/

		
		
		CString outputFullname = outvalfilename.c_str();
		fm->Writeout(tempPredictvalue, outputFullname, GetOutputExt());

		delete tempPredictvalue;

		delete maxS_dataBuf, sumS_dataBuf,sumSV_dataBuf, flag_dataBuf;


	}


	delete simi3drFirst;
	simi3drFirst = NULL;

	delete uncertainty3dr;

	cout << "Step 4 of 4: Calculating uncertainty and values is finished.\n\n";

	return 0;
}






void CCBInferenceView::OnTcnSelchangeTabPropertyType(NMHDR *pNMHDR, LRESULT *pResult)
{

	//CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());


	if(this->m_curSelTab == 0)
	{



		uncertaintyCtrl.ShowWindow(SW_HIDE);
		propertyFolderCtrl.ShowWindow(SW_HIDE);
		thresholdPropertyCtrl.ShowWindow(SW_HIDE);

		m_staticSelectProperty.ShowWindow(SW_HIDE);

		listAllField.ShowWindow(SW_HIDE);


		m_frameResult.ShowWindow(SW_HIDE);
		m_staticSaveMap.ShowWindow(SW_HIDE);
		m_btnSaveMap.ShowWindow(SW_HIDE);
		m_staticThreshold.ShowWindow(SW_HIDE);
		m_staticSaveUncertainty.ShowWindow(SW_HIDE);
		m_btnSaveUncertainty.ShowWindow(SW_HIDE);

		m_staticSaveType.ShowWindow(SW_HIDE);
		m_comboSaveType.ShowWindow(SW_HIDE);
	}


	else 
	{

		comboTypeField.ShowWindow(SW_HIDE);
		listAllType.ShowWindow(SW_HIDE);

		m_staticTypeField.ShowWindow(SW_HIDE);
		m_staticSelectType.ShowWindow(SW_HIDE);
		m_frameResultType.ShowWindow(SW_HIDE);
		m_staticSaveTypeResult.ShowWindow(SW_HIDE);
		m_editTypeResultFolder.ShowWindow(SW_HIDE);
		m_btnSaveTypeResult.ShowWindow(SW_HIDE);
	}

	this->m_curSelTab = m_tabPropertyType.GetCurSel();


	if(this->m_curSelTab == 0)
	{

		//uncertaintyCtrl.ShowWindow(SW_HIDE);
		uncertaintyCtrl.ShowWindow(SW_SHOW);
		propertyFolderCtrl.ShowWindow(SW_SHOW);
		thresholdPropertyCtrl.ShowWindow(SW_SHOW);

		m_staticSaveType.ShowWindow(SW_SHOW);
		m_comboSaveType.ShowWindow(SW_SHOW);


		m_staticSelectProperty.ShowWindow(SW_SHOW);


		listAllField.ShowWindow(SW_SHOW);


		m_frameResult.ShowWindow(SW_SHOW);
		m_staticSaveMap.ShowWindow(SW_SHOW);
		m_btnSaveMap.ShowWindow(SW_SHOW);
		m_staticThreshold.ShowWindow(SW_SHOW);
		//m_staticSaveUncertainty.ShowWindow(SW_SHOW);
		m_staticSaveUncertainty.ShowWindow(SW_HIDE);
		//m_btnSaveUncertainty.ShowWindow(SW_SHOW);
		m_btnSaveUncertainty.ShowWindow(SW_HIDE);
		pDoc->m_pProjectCaseEdit->iCBROption = 1;
	}
	else 
	{
		comboTypeField.ShowWindow(SW_SHOW);

		listAllType.ShowWindow(SW_SHOW);

		m_staticTypeField.ShowWindow(SW_SHOW);
		m_staticSelectType.ShowWindow(SW_SHOW);
		m_frameResultType.ShowWindow(SW_SHOW);
		m_staticSaveTypeResult.ShowWindow(SW_SHOW);
		m_editTypeResultFolder.ShowWindow(SW_SHOW);
		m_btnSaveTypeResult.ShowWindow(SW_SHOW);
		pDoc->m_pProjectCaseEdit->iCBROption = 2;
	}

	*pResult = 0;

}





void CCBInferenceView::OnBnClickedButtonSaveuncertainty()
{

	UpdateData(true);
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());

	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|Saga Files (*.sdat)|*.sdat|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(FALSE, "3dr", ".3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	

	m_ldFile.m_ofn.lpstrInitialDir= pDoc->m_strProjectDir; 

	if(m_ldFile.DoModal() == IDOK)
	{
		m_OutputUncertaintyFilename = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CCBInferenceView::OnBnClickedButtonSavevalues()
{
	UpdateData(true);

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());	
	CGetDirDlg dirDlg;   
	m_OutputPredictvaluesFoldername = dirDlg.GetDirectory(this, pDoc->m_strProjectDir, "Choose a directory to save the predicted values:");

	// 更改存储文件名称
	string outputFolder(m_OutputPredictvaluesFoldername.GetBuffer());
	string strExt = GetOutputExt();
	string uncertaintyFilename = outputFolder + "\\" + "uncertainty" + strExt;
	m_OutputUncertaintyFilename = uncertaintyFilename.c_str();

	UpdateData(FALSE);
}




void CCBInferenceView::OnBnClickedButtonSaveTyperesult()
{

	UpdateData(true);

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());

	CGetDirDlg dirDlg;   
	m_strMaxSimiFolder = dirDlg.GetDirectory(this, pDoc->m_strProjectDir, "Choose a directory to save the membership files:");;   
	UpdateData(FALSE);

}



void CCBInferenceView::OnCbnSelchangeComboTypeField()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());


	UpdateData();
	// TODO: Add your control notification handler code here
	//added by Fei
	while (listAllType.GetCount() > 0)
		listAllType.DeleteString(0);



	//const char* strType = DBFReadStringAttribute(pDoc->m_pProjectCaseEdit->hDBF, 0, DBFGetFieldIndex(pDoc->m_pProjectCaseEdit->hDBF, typeFieldName.GetBuffer()));
	//CString ex(strType);


	CString strSel;
	comboTypeField.GetLBText(comboTypeField.GetCurSel(), strSel);




	pDoc->m_pProjectCaseEdit-> m_strCurTypeField = strSel;

	//typeFieldIndex = DBFGetFieldIndex(pDoc->m_pProjectCaseEdit->hDBF, strSel.GetBuffer());

	typeFieldIndex = pDoc->m_pProjectCaseEdit->hCSV->GetVariableIndex(strSel.GetBuffer());

	//int nRec = DBFGetRecordCount( pDoc->m_pProjectCaseEdit->hDBF );

	int nRec = pDoc->m_pProjectCaseEdit->hCSV->GetNumberOfSamples(0) -1;



	//only add new types to the list
	for (int k = 0; k < nRec; ++k)
	{
	
		string s;
		pDoc->m_pProjectCaseEdit->hCSV->GetData(typeFieldIndex, k, s);//vecSampleData[k][typeFieldIndex];

		CString newName(s.c_str());


		bool bExists = false;
		for(int m = 0; m < listAllType.GetCount(); m++)
		{
			CString tempName;
			listAllType.GetText(m,tempName);

			if(newName.Trim() == tempName.Trim())
			{
				bExists = true;
				break;
			}
		}
		if(bExists == false)  //this is totally a new type
			listAllType.AddString(newName);
			
		newName.ReleaseBuffer();
	}



}









void CCBInferenceView::OnEnKillfocusEditFinalResolution()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	this->m_ListLayerSetting.m_dCellSize = this->m_dFinalRes;

	this->dstRes = this->m_dFinalRes;




	for(int i=0; i<this->m_ListLayerSetting.GetItemCount(); i++)
	{
		if(atof(m_ListLayerSetting.GetItemText(i, 2).GetBuffer(0)) >= m_dFinalRes)
			if(m_ListLayerSetting.GetItemText(i, 3) == "Prob. Density")
			{
				m_ListLayerSetting.SetItemText(i, 3, "Single Value");
				m_ListLayerSetting.SetItemText(i, 4, "Gower Distance");
			}
	}

	this->m_ListLayerSetting.Invalidate();



}

void CCBInferenceView::Convert_3drToGdal(CThreeDR *input, CString output, char *type)
{
	// get the basic info from source
	int width = input->getNumberOfColumns();
	int height = input->getNumberOfRows();
	char *dataUnits = input->getGridUnits();
	int nBands = 1;
	float *dataBuffer = input->getDataBuf();
	double geoTransform[6];
	char *projection = input->getProjection();

	geoTransform[0] = input->getXmin();
	//geoTransform[3] = input->getYmin();
	geoTransform[3] = input->getYmin() + height * input->getCellSize();
	geoTransform[1] = input->getCellSize();
	geoTransform[5] = -1 * input->getCellSize();
	geoTransform[2] = 0;
	geoTransform[4] = 0;
	float noDataValue = input->getNullDataValue();

	//GDALAllRegister();
	// create gdal file
	GDALDriver *pDriver = NULL;
    pDriver = GetGDALDriverManager()->GetDriverByName(type);
    if ( pDriver == NULL ) { return; }
  
    GDALDataset *pDataset = pDriver->Create(output, width, height, nBands, GDT_Float32, NULL);
	if( pDataset == NULL ) { return; }
	
	// write to the output file
	pDataset->SetGeoTransform(geoTransform);
	//pDataset->SetProjection(projection);
	GDALRasterBand *pBand = NULL;
	//pBand->SetUnitType(dataUnits);
	pBand = pDataset->GetRasterBand(1);
	pBand->SetNoDataValue(NOFLOATVALUE);//noDataValue);
    pBand->RasterIO(GF_Write,
                    0,
                    0,
                    width,
                    height,
                    dataBuffer,
                    width,
                    height,
                    GDT_Float32,
                    0,
                    0);
	pBand->SetNoDataValue(-9999.0);//noDataValue);
	double a = pBand->GetNoDataValue();

	/* Once we're done, close properly the dataset */
    if( pDataset != NULL )
        GDALClose( (GDALDatasetH) pDataset );
}





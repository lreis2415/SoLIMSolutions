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
// PropertyInferPage.cpp : 实现文件
//

#include "stdafx.h"
#include "SoLIM.h"
#include "PropertyInferPage.h"

#include "MainFrm.h"
#include "SoLIMDoc.h"
#include ".\propertyinferpage.h"


// CPropertyInferPage 对话框

IMPLEMENT_DYNAMIC(CPropertyInferPage, CDialog)
CPropertyInferPage::CPropertyInferPage(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyInferPage::IDD, pParent)
{
}

CPropertyInferPage::~CPropertyInferPage()
{
}

void CPropertyInferPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

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
}


BEGIN_MESSAGE_MAP(CPropertyInferPage, CDialog)	
	ON_BN_CLICKED(IDC_BUTTON_SaveUncertainty, OnBnClickedButtonSaveuncertainty)
	ON_BN_CLICKED(IDC_BUTTON_SaveValues, OnBnClickedButtonSavevalues)

	ON_WM_SIZE()
END_MESSAGE_MAP()




	

void CPropertyInferPage::OnBnClickedButtonSaveuncertainty()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取保存Uncertainty的文件名
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());

	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(FALSE, "3dr", ".3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	

	m_ldFile.m_ofn.lpstrInitialDir= pDoc->m_strProjectDir; 

	if(m_ldFile.DoModal() == IDOK)
	{
		m_OutputUncertaintyFilename = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CPropertyInferPage::OnBnClickedButtonSavevalues()
{

		CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());	
	CGetDirDlg dirDlg;   
	m_OutputPredictvaluesFoldername = dirDlg.GetDirectory(this, pDoc->m_strProjectDir, "Choose a directory to save the predicted values:");;   
	UpdateData(FALSE);
}








// CPropertyInferPage 消息处理程序

void CPropertyInferPage::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
		
	

	if(listAllField.GetSafeHwnd())
	{
		//CRect rect(0.5 * cx/10,1.0*cy/10,9.5* cx/10,5*cy/10);


		CRect rect(0, 0, 200, 200);
		listAllField.MoveWindow(&rect);

		listAllField.ShowWindow(SW_SHOW);
	}
	
	if(this->m_frameResult.GetSafeHwnd())
	{
		CRect rect(0.5 * cx/10,5.1*cy/10,9.5* cx/10,9.9*cy/10);
		m_frameResult.MoveWindow(&rect);
		
		
	}

    

	if(this->m_staticSaveMap.GetSafeHwnd())
	{
		CRect rect(0.5 * cx/10,5.2*cy/10,9.5* cx/10,5.2*cy/10+20);
		m_staticSaveMap.MoveWindow(&rect);
	}

	if(this->propertyFolderCtrl.GetSafeHwnd())
	{
		CRect rect(0.5 * cx/10,5.2*cy/10 + 30,9.0* cx/10,5.2*cy/10+50);
		propertyFolderCtrl.MoveWindow(&rect);
	}

	if(this->m_btnSaveMap.GetSafeHwnd())
	{
		CRect rect(9.1 * cx/10,5.2*cy/10+30,9.5* cx/10,5.2*cy/10+50);
		m_btnSaveMap.MoveWindow(&rect);
	}

	if(this->m_staticThreshold.GetSafeHwnd())
	{
		CRect rect(0.5 * cx/10,6*cy/10,0.5* cx/10+ 50,6*cy/10+20);
		m_staticThreshold.MoveWindow(&rect);
	}
	if(this->thresholdPropertyCtrl.GetSafeHwnd())
	{
		CRect rect(0.5 * cx/10+ 60,6.0*cy/10,9.0* cx/10,6*cy/10+30);
		thresholdPropertyCtrl.MoveWindow(&rect);
	}

	if(this->m_staticSaveUncertainty.GetSafeHwnd())
	{
		CRect rect(0.5 * cx/10,7*cy/10,9.5* cx/10,7*cy/10+20);
		m_staticSaveUncertainty.MoveWindow(&rect);
	}
	if(this->uncertaintyCtrl.GetSafeHwnd())
	{
		CRect rect(0.5 * cx/10,7*cy/10+30,9.0* cx/10,7*cy/10+50);
		uncertaintyCtrl.MoveWindow(&rect);
	}

		if(this->m_btnSaveUncertainty.GetSafeHwnd())
	{
		CRect rect(9.1 * cx/10,7.0*cy/10,9.5* cx/10,7*cy/10+50);
		m_btnSaveUncertainty.MoveWindow(&rect);
	}

	this->Invalidate();
	
	// TODO: Add your message handler code here
}

BOOL CPropertyInferPage::OnInitDialog()
{
	CDialog::OnInitDialog();

		CRect rect;
	GetClientRect(&rect);

	/*

	int cx = rect.right;
	int cy = rect.bottom;



	if(listAllField.GetSafeHwnd())
	{
		CRect rect(0.5 * cx/10,1.0*cy/10,9.5* cx/10,5*cy/10);
		listAllField.MoveWindow(&rect);
	}	
	
	if(this->m_frameResult.GetSafeHwnd())
	{
		CRect rect(0.5 * cx/10,5.1*cy/10,9.5* cx/10,9.9*cy/10);
		m_frameResult.MoveWindow(&rect);
	}

	if(this->m_staticSaveMap.GetSafeHwnd())
	{
		CRect rect(0.5 * cx/10,5.2*cy/10,9.5* cx/10,5.2*cy/10+20);
		m_staticSaveMap.MoveWindow(&rect);
	}

	if(this->propertyFolderCtrl.GetSafeHwnd())
	{
		CRect rect(0.5 * cx/10,5.2*cy/10 + 30,9.0* cx/10,5.2*cy/10+50);
		propertyFolderCtrl.MoveWindow(&rect);
	}

	if(this->m_btnSaveMap.GetSafeHwnd())
	{
		CRect rect(9.1 * cx/10,5.2*cy/10+30,9.5* cx/10,5.2*cy/10+50);
		m_btnSaveMap.MoveWindow(&rect);
	}

	if(this->m_staticThreshold.GetSafeHwnd())
	{
		CRect rect(0.5 * cx/10,6*cy/10,0.5* cx/10+ 50,6*cy/10+20);
		m_staticThreshold.MoveWindow(&rect);
	}
	if(this->thresholdPropertyCtrl.GetSafeHwnd())
	{
		CRect rect(0.5 * cx/10+ 60,6.0*cy/10,9.0* cx/10,6*cy/10+30);
		thresholdPropertyCtrl.MoveWindow(&rect);
	}

	if(this->m_staticSaveUncertainty.GetSafeHwnd())
	{
		CRect rect(0.5 * cx/10,7*cy/10,9.5* cx/10,7*cy/10+20);
		m_staticSaveUncertainty.MoveWindow(&rect);
	}
	if(this->uncertaintyCtrl.GetSafeHwnd())
	{
		CRect rect(0.5 * cx/10,7*cy/10+30,9.0* cx/10,7*cy/10+50);
		uncertaintyCtrl.MoveWindow(&rect);
	}

		if(this->m_btnSaveUncertainty.GetSafeHwnd())
	{
		CRect rect(9.1 * cx/10,7.0*cy/10,9.5* cx/10,7*cy/10+50);
		m_btnSaveUncertainty.MoveWindow(&rect);
	}
	*/

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

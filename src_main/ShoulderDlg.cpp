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
// ShoulderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "ShoulderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShoulderDlg dialog


CShoulderDlg::CShoulderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShoulderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShoulderDlg)
	m_OutputFileName = _T("");
	m_PlanformFileName = _T("");
	m_PlanformWgt = 1.0f;
	m_ProfileFileName = _T("");
	m_ProfileWgt = 1.0f;
	m_TerrainPartitionFileName = _T("");
	m_TopPercent = 0.0f;
	m_MinArea = 0.0f;
	m_SlopeFileName = _T("");
	m_SlopeThresh = 0.0f;
	m_RidgeFileName = _T("");
	//}}AFX_DATA_INIT
}


void CShoulderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShoulderDlg)
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
	DDX_Text(pDX, IDC_OutputFileNameFld, m_OutputFileName);
	DDX_Text(pDX, IDC_PlanformFileFld, m_PlanformFileName);
	DDX_Text(pDX, IDC_PlanformWgtFld, m_PlanformWgt);
	DDV_MinMaxFloat(pDX, m_PlanformWgt, 0.f, 1.f);
	DDX_Text(pDX, IDC_ProfileFileFld, m_ProfileFileName);
	DDX_Text(pDX, IDC_ProfileWgtFld, m_ProfileWgt);
	DDV_MinMaxFloat(pDX, m_ProfileWgt, 0.f, 1.f);
	DDX_Text(pDX, IDC_TerrainPartitionFileFld, m_TerrainPartitionFileName);
	DDX_Text(pDX, IDC_TopPercentFld, m_TopPercent);
	DDV_MinMaxFloat(pDX, m_TopPercent, 0.f, 100.f);
	DDX_Text(pDX, IDC_MinAreaFld, m_MinArea);
	DDX_Text(pDX, IDC_SlopeFileFld, m_SlopeFileName);
	DDX_Text(pDX, IDC_SlopeThreshFld, m_SlopeThresh);
	DDX_Text(pDX, IDC_RidgeFileFld, m_RidgeFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShoulderDlg, CDialog)
	//{{AFX_MSG_MAP(CShoulderDlg)
	ON_BN_CLICKED(IDC_OutputFileNameBtn, OnOutputFileNameBtn)
	ON_BN_CLICKED(IDC_PlanformBtn, OnPlanformBtn)
	ON_BN_CLICKED(IDC_ProfileBtn, OnProfileBtn)
	ON_BN_CLICKED(IDC_TerrainPartitionFileBtn, OnTerrainPartitionFileBtn)
	ON_BN_CLICKED(IDC_SlopeFileBtn, OnSlopeFileBtn)
	ON_BN_CLICKED(IDC_RidgeFileBtn, OnRidgeFileBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShoulderDlg message handlers

void CShoulderDlg::OnTerrainPartitionFileBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_TerrainPartitionFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void CShoulderDlg::OnPlanformBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_PlanformFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}		
}

void CShoulderDlg::OnProfileBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_ProfileFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}		
}

void CShoulderDlg::OnSlopeFileBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_SlopeFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}			
}

void CShoulderDlg::OnRidgeFileBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_RidgeFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}				
}

void CShoulderDlg::OnOutputFileNameBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(FALSE, "3dr", ".3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK){
		m_OutputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void CShoulderDlg::OnOK(){
	// TODO: Add extra validation here
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

	if(m_TerrainPartitionFileName.IsEmpty()){
		MessageBox("No terrain partition file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if(m_PlanformFileName.IsEmpty()){
		MessageBox("No planform file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if(m_ProfileFileName.IsEmpty()){
		MessageBox("No profile file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if(m_SlopeFileName.IsEmpty()){
		MessageBox("No slope file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if(m_RidgeFileName.IsEmpty()){
		MessageBox("No ridge file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if(m_OutputFileName.IsEmpty()){
		MessageBox("No output file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}

	CThreeDR* theTerrainPartition = new CThreeDR;
	int readFlag = theTerrainPartition->readin(m_TerrainPartitionFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the terrain partition file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		delete theTerrainPartition;
		return;
	}
	CThreeDR* thePlanform = new CThreeDR;
	readFlag = thePlanform->readin(m_PlanformFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the planform file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		delete theTerrainPartition;
		delete thePlanform;
		return;
	}
	CThreeDR* theProfile = new CThreeDR;
	readFlag = theProfile->readin(m_ProfileFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the profile file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		delete theTerrainPartition;
		delete thePlanform;
		delete theProfile;
		return;
	}
	CThreeDR* theSlope = new CThreeDR;
	readFlag = theSlope->readin(m_SlopeFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the slope file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		delete theTerrainPartition;
		delete thePlanform;
		delete theProfile;
		delete theSlope;
		return;
	}	
	CThreeDR* theRidge = new CThreeDR;
	readFlag = theRidge->readin(m_RidgeFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the ridgeline file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		delete theTerrainPartition;
		delete thePlanform;
		delete theProfile;
		delete theSlope;
		delete theRidge;
		return;
	}	
	float* terrainPartition = theTerrainPartition->getDataBuf();
	float* planform = thePlanform->getDataBuf();
	float* profile = theProfile->getDataBuf();
	float* slope = theSlope->getDataBuf();
	float* ridge = theRidge->getDataBuf();
	int nrow = theTerrainPartition->getNumberOfRows();
	int ncol = theTerrainPartition->getNumberOfColumns();
	float cellsize = theTerrainPartition->getCellSize();
	float nodata = theTerrainPartition->getNullDataValue();
	float* outBuf = new float[nrow*ncol];
	m_ProgressBar.SetPos(30);

	int opFlag = shoulder(terrainPartition, planform, profile, slope, ridge, outBuf, 
							m_PlanformWgt, m_ProfileWgt, 
							m_SlopeThresh, (int)(m_MinArea/(cellsize*cellsize)), m_TopPercent, nrow, ncol, nodata);

	if(opFlag != 0){
		MessageBox("Error in shoulder function.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		delete theTerrainPartition;
		delete thePlanform;
		delete theProfile;
		delete theSlope;
		delete theRidge;
		delete [] outBuf;
		return;
	}

	m_ProgressBar.SetPos(100);
	
	CThreeDR* theOutput = new CThreeDR;
	theOutput->setDataBuf(outBuf);
	theOutput->setDataName(extractFileName(m_OutputFileName).GetBuffer(1));
	theOutput->copyBasicFields(theTerrainPartition);
	theOutput->calcStat();
	int writeFlag = theOutput->writeout(m_OutputFileName.GetBuffer(1), 0);

	//clean up
	delete theTerrainPartition;
	delete thePlanform;
	delete theProfile;
	delete theSlope;
	delete theRidge;
	delete theOutput;

	GetDlgItem(IDOK)->EnableWindow(TRUE);

	CDialog::OnOK();
}

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
// StreamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "StreamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStreamDlg dialog


CStreamDlg::CStreamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStreamDlg::IDD, pParent)
	, m_InputFileName(_T(""))
	, m_OutputFileName(_T(""))
	, m_Threshold(100000.0)
	, m_CodingAlg(_T("Simple mark"))
	, m_StreamAlg(0)
	, m_SkidmoreLower(2.0f)
	, m_SkidmoreUpper(2.0f)
{
}

void CStreamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
	DDX_Text(pDX, IDC_InputFileNameFld, m_InputFileName);
	DDX_Text(pDX, IDC_OutputFileNameFld, m_OutputFileName);
	DDX_Text(pDX, IDC_ThresholdFld, m_Threshold);
	DDX_CBString(pDX, IDC_CodingAlgCmb, m_CodingAlg);
	DDX_Radio(pDX, IDC_OCallaghanRdo, m_StreamAlg);
	DDX_Text(pDX, IDC_SkidmoreLowerFld, m_SkidmoreLower);
	DDX_Text(pDX, IDC_SkidmoreUpperFld, m_SkidmoreUpper);
}


BEGIN_MESSAGE_MAP(CStreamDlg, CDialog)
	//{{AFX_MSG_MAP(CStreamDlg)
	ON_BN_CLICKED(IDC_InputFileNameBtn, OnInputFileNameBtn)
	ON_BN_CLICKED(IDC_OutputFileNameBtn, OnOutputFileNameBtn)
	ON_BN_CLICKED(IDC_OCallaghanRdo, OnOCallaghanRdo)
	ON_BN_CLICKED(IDC_PeukerRdo, OnPeukerRdo)
	ON_BN_CLICKED(IDC_SkidmoreRdo, OnSkidmoreRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStreamDlg message handlers

BOOL CStreamDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	algoParameters();
	m_ProgressBar.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStreamDlg::OnInputFileNameBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_InputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void CStreamDlg::OnOutputFileNameBtn(){
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

void CStreamDlg::OnOCallaghanRdo(){
	// TODO: Add your control notification handler code here
	m_StreamAlg = 0;
	algoParameters();	
}

void CStreamDlg::OnSkidmoreRdo(){
	// TODO: Add your control notification handler code here
	m_StreamAlg = 1;
	algoParameters();	
}

void CStreamDlg::OnPeukerRdo(){
	// TODO: Add your control notification handler code here
	m_StreamAlg = 2;
	algoParameters();	
}

void CStreamDlg::algoParameters(){
	switch(m_StreamAlg){
	case 0:
		GetDlgItem(IDC_OCallaghanGrp)->EnableWindow(TRUE);
		GetDlgItem(IDC_ThresholdLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_ThresholdFld)->EnableWindow(TRUE);
		GetDlgItem(IDC_CodingAlgLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_CodingAlgCmb)->EnableWindow(TRUE);
		GetDlgItem(IDC_SkidmoreGrp)->EnableWindow(FALSE);
		GetDlgItem(IDC_SkidmoreUpperLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_SkidmoreUpperFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_SkidmoreLowerLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_SkidmoreLowerFld)->EnableWindow(FALSE);
		break;
	case 1:
		GetDlgItem(IDC_OCallaghanGrp)->EnableWindow(FALSE);
		GetDlgItem(IDC_ThresholdLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_ThresholdFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_CodingAlgLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_CodingAlgCmb)->EnableWindow(FALSE);
		GetDlgItem(IDC_SkidmoreGrp)->EnableWindow(TRUE);
		GetDlgItem(IDC_SkidmoreUpperLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_SkidmoreUpperFld)->EnableWindow(TRUE);
		GetDlgItem(IDC_SkidmoreLowerLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_SkidmoreLowerFld)->EnableWindow(TRUE);
		break;
	case 2:
		GetDlgItem(IDC_OCallaghanGrp)->EnableWindow(FALSE);
		GetDlgItem(IDC_ThresholdLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_ThresholdFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_CodingAlgLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_CodingAlgCmb)->EnableWindow(FALSE);
		GetDlgItem(IDC_SkidmoreGrp)->EnableWindow(FALSE);
		GetDlgItem(IDC_SkidmoreUpperLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_SkidmoreUpperFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_SkidmoreLowerLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_SkidmoreLowerFld)->EnableWindow(FALSE);
		break;
	}
}

void CStreamDlg::OnOK(){
	// TODO: Add extra validation here
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.ShowWindow(SW_SHOW);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

	if(m_InputFileName.IsEmpty()){
		MessageBox("No input file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_OutputFileName.IsEmpty()){
		MessageBox("No output file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_Threshold<=0){
		MessageBox("Not a valid threshold.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	CThreeDR* theInput = new CThreeDR;
	int readFlag = theInput->readin(m_InputFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the input file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	int nrow = theInput->getNumberOfRows();
	int ncol = theInput->getNumberOfColumns();
	float nodata = theInput->getNullDataValue();
	float* dem = theInput->getDataBuf();
	float* outBuf = new float[nrow*ncol];
	int opFlag = 0;

	if(m_StreamAlg == 0){ //O'Callaghan and Mark's
		float cellsize = theInput->getCellSize();
		float unitArea = cellsize*cellsize;

		float* cachment = new float[nrow*ncol];
		float* flowDir = new float[nrow*ncol];
		float* filledDEM = new float[nrow*ncol];
		float* gradBuf = new float[nrow*ncol];
		float* flowAcc = new float[nrow*ncol];

		opFlag = fillDEM(dem, cachment, flowDir, filledDEM, 10000, nrow, ncol, nodata);
		if(opFlag != 0){
			MessageBox("Error in filling DEM.");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			m_ProgressBar.SetPos(0);
			return;
		}
		m_ProgressBar.SetPos(20);

		opFlag = flowDirection(filledDEM, flowDir, nrow, ncol, nodata);
		if(opFlag != 0){
			MessageBox("Error in computing flow direction.");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			m_ProgressBar.SetPos(0);
			return;
		}
		m_ProgressBar.SetPos(40);

		opFlag = flowAccumulation(flowDir, flowAcc, nrow, ncol, unitArea, nodata);
		if(opFlag != 0){
			MessageBox("Error in computing flow accumulation.");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			m_ProgressBar.SetPos(0);
			return;
		}
		m_ProgressBar.SetPos(60);

		opFlag = streamlineOCallaghan(flowDir, flowAcc, outBuf, nrow, ncol, m_Threshold, 1, m_CodingAlg, nodata);
		if(opFlag != 0){
			MessageBox("Error in labeling streamlines.");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			m_ProgressBar.SetPos(0);
			return;
		}
		m_ProgressBar.SetPos(80);

		delete [] filledDEM;
		delete [] flowDir;
		delete [] flowAcc;
	}		
	else if(m_StreamAlg == 1){ //modified Skidmore's
		opFlag = streamlineSkidmore(dem, outBuf, m_SkidmoreUpper, m_SkidmoreLower, nrow, ncol, 1, nodata);
		if(opFlag != 0){
			MessageBox("Error in calculation with Skidmore's Alg.");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			m_ProgressBar.SetPos(0);
			return;
		}
	}
	else{ //Peuker and Douglas'
		opFlag = streamlinePeuker(dem, outBuf, nrow, ncol, 1, nodata);
		if(opFlag != 0){
			MessageBox("Error in calculation with Peuker and Douglas' Alg.");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			m_ProgressBar.SetPos(0);
			return;
		}
	}

	m_ProgressBar.SetPos(100);

	CThreeDR* theOutput = new CThreeDR;
	theOutput->setDataBuf(outBuf);
	theOutput->setDataName(extractFileName(m_OutputFileName).GetBuffer(1));
	theOutput->copyBasicFields(theInput);
	theOutput->calcStat();
	int writeFlag = theOutput->writeout(m_OutputFileName.GetBuffer(1), 0);

	delete theInput;
	delete theOutput;

	GetDlgItem(IDOK)->EnableWindow(TRUE);
	CDialog::OnOK();
}

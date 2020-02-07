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
// BrdNrwRidgeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "BrdNrwRidgeDlg.h"
#include ".\brdnrwridgedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBrdNrwRidgeDlg dialog


CBrdNrwRidgeDlg::CBrdNrwRidgeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBrdNrwRidgeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBrdNrwRidgeDlg)
	m_FlatArea = 0.0f;
	m_Grad = 6;
	m_InputRidge = _T("");
	m_Label = 1;
	m_Lower = 0.0f;
	m_OutputFileName = _T("");
	m_Upper = 99999;
	m_InputGrad = _T("");
	//}}AFX_DATA_INIT
}

void CBrdNrwRidgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrdNrwRidgeDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
	DDX_Text(pDX, IDC_FlatAreaFld, m_FlatArea);
	DDX_Text(pDX, IDC_GradFld, m_Grad);
	DDX_Text(pDX, IDC_InputRidgeFld, m_InputRidge);
	DDX_Text(pDX, IDC_LabelFld, m_Label);
	DDX_Text(pDX, IDC_LowerFld, m_Lower);
	DDX_Text(pDX, IDC_OutputFileNameFld, m_OutputFileName);
	DDX_Text(pDX, IDC_UpperFld, m_Upper);
	DDX_Text(pDX, IDC_InputGradientFld, m_InputGrad);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBrdNrwRidgeDlg, CDialog)
	//{{AFX_MSG_MAP(CBrdNrwRidgeDlg)
	ON_BN_CLICKED(IDC_InputRidgelineBtn, OnInputRidgelineBtn)
	ON_BN_CLICKED(IDC_OutputFileNameBtn, OnOutputFileNameBtn)
	ON_BN_CLICKED(IDC_InputGradientBtn, OnInputGradientBtn)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_OutputFileNameLbl, OnStnClickedOutputfilenamelbl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrdNrwRidgeDlg message handlers

void CBrdNrwRidgeDlg::OnInputRidgelineBtn(){
	// TODO: Add your control notification handler code here
 	UpdateData(TRUE);
 	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
  	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
 	if(m_ldFile.DoModal() == IDOK){
 		m_InputRidge = m_ldFile.GetPathName();
 		UpdateData(FALSE);
 	}	
}

void CBrdNrwRidgeDlg::OnInputGradientBtn(){
	// TODO: Add your control notification handler code here
 	UpdateData(TRUE);
 	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
  	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
 	if(m_ldFile.DoModal() == IDOK){
 		m_InputGrad = m_ldFile.GetPathName();
 		UpdateData(FALSE);
 	}	
}

void CBrdNrwRidgeDlg::OnOutputFileNameBtn(){
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

void CBrdNrwRidgeDlg::OnOK(){
	// TODO: Add extra validation here
	UpdateData(TRUE);	
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

	//1. read in data
	if(m_InputRidge.IsEmpty()){
		MessageBox("No ridgeline file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_InputGrad.IsEmpty()){
		MessageBox("No slope gradient file.");
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

	CThreeDR* theRidgeline = new CThreeDR;
	int readFlag = theRidgeline->readin(m_InputRidge.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the ridgeline file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	CThreeDR* theGrad = new CThreeDR;
	readFlag = theGrad->readin(m_InputGrad.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the slope gradient file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	int nrow = theRidgeline->getNumberOfRows();
	int ncol = theRidgeline->getNumberOfColumns();
	float cellsize = theRidgeline->getCellSize();
	float nodata = theRidgeline->getNullDataValue();

	float* grad = theGrad->getDataBuf();
	float* ridgeline = theRidgeline->getDataBuf();
	float* outBuf = new float[nrow*ncol];
	m_ProgressBar.SetPos(20);

	int opFlag = brdNrwRidge(ridgeline, m_Label, grad, outBuf, nrow, ncol, cellsize, m_Upper, m_Lower, m_Grad, m_FlatArea, nodata);
	if(opFlag != 0){
		MessageBox("Error in deriving broad/narrow ridges.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	m_ProgressBar.SetPos(100);

	CThreeDR* theOutput = new CThreeDR;
	theOutput->setDataBuf(outBuf);
	theOutput->setDataName(extractFileName(m_OutputFileName).GetBuffer(1));
	theOutput->copyBasicFields(theRidgeline);
	theOutput->calcStat();
	int writeFlag = theOutput->writeout(m_OutputFileName.GetBuffer(1), 0);
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	delete theRidgeline;
	delete theGrad;
	delete theOutput;
		
	CDialog::OnOK();
}

void CBrdNrwRidgeDlg::OnStnClickedOutputfilenamelbl()
{
	// TODO: 在此添加控件通知处理程序代码
}

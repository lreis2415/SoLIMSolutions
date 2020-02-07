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
// FillShaveDEMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "FillShaveDEMDlg.h"
#include ".\fillshavedemdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFillShaveDEMDlg dialog


CFillShaveDEMDlg::CFillShaveDEMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFillShaveDEMDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFillShaveDEMDlg)
	m_InputFileName = _T("");
	m_OutputFileName = _T("");
	m_PeakHeight = 2;
	m_PitDepth = 10000;
	//m_ElevThreshhold = 0.01;
	//}}AFX_DATA_INIT
	m_Method = 1;
}


void CFillShaveDEMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFillShaveDEMDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_ProgressBar);
	DDX_Text(pDX, IDC_InputFileNameFld, m_InputFileName);
	DDX_Text(pDX, IDC_OutputFileNameFld, m_OutputFileName);
	DDX_Text(pDX, IDC_PeakHeightFld, m_PeakHeight);
	DDX_Text(pDX, IDC_PitDepthFld, m_PitDepth);
	//DDX_Text(pDX, IDC_ElevThreshholdFld, m_ElevThreshhold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFillShaveDEMDlg, CDialog)
	//{{AFX_MSG_MAP(CFillShaveDEMDlg)
	ON_BN_CLICKED(IDC_InputFileNameBtn, OnInputFileNameBtn)
	ON_BN_CLICKED(IDC_OutputFileNameBtn, OnOutputFileNameBtn)
	//}}AFX_MSG_MAP
	//ON_BN_CLICKED(IDC_RADIO_Default, OnBnClickedRadioDefault)
	//ON_BN_CLICKED(IDC_RADIO_PD, OnBnClickedRadioPd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFillShaveDEMDlg message handlers


BOOL CFillShaveDEMDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//m_ProgressBar.ShowWindow(SW_HIDE);
	//((CButton*)GetDlgItem(IDC_RADIO_Default))->SetCheck(1);
//	m_Method = 1;
	//GetDlgItem(IDC_ElevThreshholdFld)->EnableWindow(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CFillShaveDEMDlg::OnInputFileNameBtn(){
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

void CFillShaveDEMDlg::OnOutputFileNameBtn(){
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

void CFillShaveDEMDlg::OnOK(){
	// TODO: Add extra validation here
	m_ProgressBar.ShowWindow(SW_SHOW);
	UpdateData(TRUE);	
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);

	if(m_InputFileName.IsEmpty()){
		MessageBox("No input file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if(m_OutputFileName.IsEmpty()){
		MessageBox("No output file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	m_ProgressBar.SetPos(5);

	CThreeDR* theInput = new CThreeDR;
	int readFlag = theInput->readin(m_InputFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	int nrow = theInput->getNumberOfRows();
	int ncol = theInput->getNumberOfColumns();
	float nodata = theInput->getNullDataValue();
	float* opDEM = theInput->getDataBuf();	
	

   // if(m_Method == 1)
	//{
	   if(m_PitDepth>0 || m_PeakHeight>0){
		 int opFlag = 0;
		 float* cachment = new float[nrow*ncol];
		 float* flowDir = new float[nrow*ncol];
		 float* filledDEM = NULL;
		 float* shavedDEM = NULL;
		 float maxEle = theInput->getDataMax();
		 m_ProgressBar.SetPos(10);

		 //fill the pits
		 if(m_PitDepth>0){
			filledDEM = new float[nrow*ncol];
			opFlag = fillDEM(opDEM, cachment, flowDir, filledDEM, m_PitDepth, nrow, ncol, nodata);
			if(opFlag != 0){
				MessageBox("Error in filling pits.");
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				m_ProgressBar.SetPos(0);
				return;
			 }
			opDEM = filledDEM;
		 }
		 m_ProgressBar.SetPos(50);

		 //shave the peaks
		 if(m_PeakHeight>0){
			 shavedDEM = new float[nrow*ncol];
			 opFlag = shaveDEM(opDEM, cachment, flowDir, shavedDEM, maxEle, m_PeakHeight, nrow, ncol, nodata);
			 if(opFlag != 0){
			 	MessageBox("Error in shaving peaks.");
			 	GetDlgItem(IDOK)->EnableWindow(TRUE);
				m_ProgressBar.SetPos(0);
				return;
			 }
			 opDEM = shavedDEM;
		  }
		 if(opDEM != filledDEM) delete filledDEM;
		 delete [] cachment;
		 delete [] flowDir;
	
	   }
	//}

	/*if(m_Method == 2)
	{
	    float* filledDEM = new float[nrow*ncol];
		bool result;
	    result = FillDEM_PD(opDEM ,filledDEM ,nrow,ncol,theInput->getCellSize(),theInput->getNullDataValue(),m_ElevThreshhold);
		if(result == false)
		    AfxMessageBox("The iteration time is greater than 10000,\nbut the program didn't get the final result.\nPlease change the threshold and try again.");

	    opDEM = filledDEM ;
	 }
 */

	CThreeDR* theOutput = new CThreeDR;
	theOutput->copyBasicFields(theInput);
	theOutput->setDataBuf(opDEM);
	theOutput->calcStat();
	theOutput->setDataName(extractFileName(m_OutputFileName).Left(extractFileName(m_OutputFileName).FindOneOf(".")).GetBuffer(1));
	theOutput->writeout(m_OutputFileName.GetBuffer(1), 0);
	delete theOutput;
	delete theInput;
	m_ProgressBar.SetPos(100);
		
	CDialog::OnOK();
}

/*
void CFillShaveDEMDlg::OnBnClickedRadioDefault()
{
    m_Method = 1;
	GetDlgItem(IDC_ElevThreshholdFld)->EnableWindow(false);
	GetDlgItem(IDC_PeakHeightFld)->EnableWindow(true);
	GetDlgItem(IDC_PitDepthFld)->EnableWindow(true);
	// TODO: Add your control notification handler code here
}

void CFillShaveDEMDlg::OnBnClickedRadioPd()
{
	m_Method = 2;
	GetDlgItem(IDC_ElevThreshholdFld)->EnableWindow(true);
	GetDlgItem(IDC_PeakHeightFld)->EnableWindow(false);
	GetDlgItem(IDC_PitDepthFld)->EnableWindow(false);
	// TODO: Add your control notification handler code here
}*/

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
// StretchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "StretchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStretchDlg dialog


CStretchDlg::CStretchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStretchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStretchDlg)
	m_InputFileName = _T("");
	m_OutputFileName = _T("");
	m_InputMax = 0.0f;
	m_InputMin = 0.0f;
	m_OutputMin = 0.0f;
	m_OutputMax = 0.0f;
	m_Customize = FALSE;
	//}}AFX_DATA_INIT
}


void CStretchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStretchDlg)
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
	DDX_Text(pDX, IDC_InputFileNameFld, m_InputFileName);
	DDX_Text(pDX, IDC_OutputFileNameFld, m_OutputFileName);
	DDX_Text(pDX, IDC_InputMaxFld, m_InputMax);
	DDX_Text(pDX, IDC_InputMinFld, m_InputMin);
	DDX_Text(pDX, IDC_OutputMinFld, m_OutputMin);
	DDX_Text(pDX, IDC_OutputMaxFld, m_OutputMax);
	DDX_Check(pDX, IDC_CustomizeChk, m_Customize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStretchDlg, CDialog)
	//{{AFX_MSG_MAP(CStretchDlg)
	ON_BN_CLICKED(IDC_OutputFileNameBtn, OnOutputFileNameBtn)
	ON_BN_CLICKED(IDC_InputFileNameBtn, OnInputFileNameBtn)
	ON_BN_CLICKED(IDC_CustomizeChk, OnCustomizeChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStretchDlg message handlers

BOOL CStretchDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	OnCustomizeChk();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStretchDlg::OnInputFileNameBtn(){
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

void CStretchDlg::OnOutputFileNameBtn(){
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

void CStretchDlg::OnCustomizeChk(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_Customize){
		GetDlgItem(IDC_InputMinLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_InputMaxLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_InputMinFld)->EnableWindow(TRUE);
		GetDlgItem(IDC_InputMaxFld)->EnableWindow(TRUE);
	}
	else{
		GetDlgItem(IDC_InputMinLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_InputMaxLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_InputMinFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_InputMaxFld)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}

void CStretchDlg::OnOK(){
	// TODO: Add extra validation here
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(1);

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
	theInput->calcStat();
	float dataMin = theInput->getDataMin();
	float dataMax = theInput->getDataMax();
	if(m_Customize){
		dataMin = m_InputMin;
		dataMax = m_InputMax;
	}
	float nodata = theInput->getNullDataValue();
	int numOfPixels = nrow*ncol;
	float d = dataMax-dataMin;
	if(d<VERYSMALL) d = VERYSMALL;
	float r = (m_OutputMax-m_OutputMin)/d;


	for(int i=0;i<nrow;i++)
	{
		m_ProgressBar.SetPos(100*i/nrow);
		for(int j=0;j<ncol;j++)
		{
			if(fabs(theInput->getDataBuf()[i*ncol+j]-nodata)<VERYSMALL) continue;
		     // m_ProgressBar.SetPos(i*100/numOfPixels);
		    if(theInput->getDataBuf()[i*ncol+j]<dataMin)
			  theInput->getDataBuf()[i*ncol+j] = m_OutputMin;
		    else if(theInput->getDataBuf()[i*ncol+j]>dataMax)
			theInput->getDataBuf()[i*ncol+j] = m_OutputMax;
		   else
			 theInput->getDataBuf()[i*ncol+j] = (theInput->getDataBuf()[i*ncol+j]-dataMin)*r+m_OutputMin;
		}
	}
/*
	for(int i = 0; i<numOfPixels; i++){
		if(fabs(theInput->getDataBuf()[i]-nodata)<VERYSMALL) continue;
		m_ProgressBar.SetPos(i*100/numOfPixels);
		if(theInput->getDataBuf()[i]<dataMin)
			theInput->getDataBuf()[i] = m_OutputMin;
		else if(theInput->getDataBuf()[i]>dataMax)
			theInput->getDataBuf()[i] = m_OutputMax;
		else
			theInput->getDataBuf()[i] = (theInput->getDataBuf()[i]-dataMin)*r+m_OutputMin;
	}*/
	m_ProgressBar.SetPos(100);

	theInput->calcStat();
	theInput->setDataName(extractFileName(m_OutputFileName).GetBuffer(1));
	int writeFlag = theInput->writeout(m_OutputFileName.GetBuffer(1), 0);

	//clean up
	delete theInput;
	GetDlgItem(IDOK)->EnableWindow(TRUE);	
	CDialog::OnOK();
}

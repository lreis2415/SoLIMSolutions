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
// MFDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "MFDDlg.h"
#include ".\mfddlg.h"


// CMFDDlg dialog

IMPLEMENT_DYNAMIC(CMFDDlg, CDialog)
CMFDDlg::CMFDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFDDlg::IDD, pParent)
	, m_InputFileName(_T(""))
	, m_OutputFileName(_T(""))
	, m_P(1)
{
}

CMFDDlg::~CMFDDlg()
{
}

void CMFDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_InputFileNameFld, m_InputFileName);
	DDX_Text(pDX, IDC_OutputFileNameFld, m_OutputFileName);
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
	DDX_Text(pDX, IDC_EDIT_P, m_P);
	//DDX_Text(pDX, IDC_EDIT_Prange, m_Prange);
	//DDX_Text(pDX, IDC_EDIT_UB, m_UB);
	//DDV_MinMaxDouble(pDX, m_UB, 0, 90);

}


BEGIN_MESSAGE_MAP(CMFDDlg, CDialog)
	ON_BN_CLICKED(IDC_InputFileNameBtn, OnBnClickedInputfilenamebtn)
	ON_BN_CLICKED(IDC_OutputFileNameBtn, OnBnClickedOutputfilenamebtn)
	ON_BN_CLICKED(IDC_RADIO_Quinn, OnBnClickedRadioQuinn)
	//ON_BN_CLICKED(IDC_RADIO_fg, OnBnClickedRadiofg)
	ON_BN_CLICKED(IDC_RADIO_md, OnBnClickedRadiomd)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CMFDDlg message handlers

BOOL CMFDDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//m_ProgressBar.ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO_Quinn))->SetCheck(1);
	m_Method = 1;
	GetDlgItem(IDC_EDIT_P)->EnableWindow(true);
	//GetDlgItem(IDC_EDIT_Prange)->EnableWindow(false);
	
	//GetDlgItem(IDC_EDIT_UB)->EnableWindow(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMFDDlg::OnBnClickedInputfilenamebtn()
{
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_InputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}			
		// TODO: Add your control notification handler code here
}

void CMFDDlg::OnBnClickedOutputfilenamebtn()
{
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(FALSE, "3dr", ".3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK){
		m_OutputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}		
	// TODO: Add your control notification handler code here
}

void CMFDDlg::OnBnClickedRadioQuinn()
{
	m_Method = 0;
	GetDlgItem(IDC_EDIT_P)->EnableWindow(true);
	//GetDlgItem(IDC_EDIT_Prange)->EnableWindow(false);
	//GetDlgItem(IDC_EDIT_UB)->EnableWindow(false);
	
	// TODO: Add your control notification handler code here
}



void CMFDDlg::OnBnClickedRadiomd()
{
	m_Method = 1;
	GetDlgItem(IDC_EDIT_P)->EnableWindow(false);
	//GetDlgItem(IDC_EDIT_Prange)->EnableWindow(true);
	//GetDlgItem(IDC_EDIT_UB)->EnableWindow(true);
	// TODO: Add your control notification handler code here
}

void CMFDDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

	if(m_InputFileName.IsEmpty()){
		MessageBox("No DEM file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if(m_OutputFileName.IsEmpty()){
		MessageBox("No output file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}

	CThreeDR* theInput = new CThreeDR;
	int readFlag = theInput->readin(m_InputFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Error in openning the DEM file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	m_ProgressBar.SetPos(20);


	float* AccRes= new float[theInput->getNumberOfRows()*theInput->getNumberOfColumns()];

	int nrow = theInput->getNumberOfRows();
	int ncol = theInput->getNumberOfColumns();

		 for(int y=0;y<nrow;y++)
	    for(int x=0;x<ncol;x++)
	    {
		 if(theInput->getDataBuf()[y*ncol+x] == theInput->getNullDataValue())
			 AccRes[y*ncol+x]=theInput->getNullDataValue();
		 else
			 AccRes[y*ncol+x] =0;
	    }


	int opFlag = MFD(theInput->getDataBuf(), AccRes, theInput->getNumberOfRows(), theInput->getNumberOfColumns(),theInput->getCellSize(),theInput->getNullDataValue(),m_Method,m_P);
	if(opFlag != 0){
		MessageBox("Error in computing MFD accumulation.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	m_ProgressBar.SetPos(80);

	CThreeDR* theOutput = new CThreeDR;
	theOutput->setDataBuf(AccRes);
	theOutput->setDataName(extractFileName(m_OutputFileName).GetBuffer(1));
	theOutput->copyBasicFields(theInput);
	theOutput->calcStat();
	opFlag = theOutput->writeout(m_OutputFileName.GetBuffer(1), 0);

	delete theInput;
	delete theOutput;

	GetDlgItem(IDOK)->EnableWindow(TRUE);
	OnOK();
}




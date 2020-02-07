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
// FilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "FilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterDlg dialog


CFilterDlg::CFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFilterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterDlg)
	m_FilterSize = 3;
	m_InputFileName = _T("");
	m_OutputFileName = _T("");
	//}}AFX_DATA_INIT
}


void CFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterDlg)
	DDX_Text(pDX, IDC_FilterSizeFld, m_FilterSize);
	DDX_Text(pDX, IDC_InputFld, m_InputFileName);
	DDX_Text(pDX, IDC_OutputFld, m_OutputFileName);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
}


BEGIN_MESSAGE_MAP(CFilterDlg, CDialog)
	//{{AFX_MSG_MAP(CFilterDlg)
	ON_BN_CLICKED(IDC_InputBtn, OnInputBtn)
	ON_BN_CLICKED(IDC_OutputBtn, OnOutputBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterDlg message handlers

BOOL CFilterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//m_ProgressBar.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CFilterDlg::OnInputBtn(){
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

void CFilterDlg::OnOutputBtn(){
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

void CFilterDlg::OnOK(){
	// TODO: Add extra validation here
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.ShowWindow(SW_SHOW);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(1);

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
	if(m_FilterSize<3){
		MessageBox("Fiter size must be greater than or equal to 3.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}

	CThreeDR* theInput = new CThreeDR;
	int readFlag = theInput->readin(m_InputFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the input file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}

	float* dataBuf = theInput->getDataBuf();
	int nrow = theInput->getNumberOfRows();
	int ncol = theInput->getNumberOfColumns();
	float nodata = theInput->getNullDataValue();
	float* outBuf = new float[nrow*ncol];

	for(int i=0; i<nrow; i++){
		m_ProgressBar.SetPos(100*i/nrow);
		int startRow = i-m_FilterSize/2;
		if(startRow<0) startRow = 0;
		int endRow = i+m_FilterSize/2;
		if(endRow>nrow-1) endRow = nrow-1;
		for(int j=0; j<ncol; j++){
			if(fabs(dataBuf[i*ncol+j]-nodata)<VERYSMALL){
				outBuf[i*ncol+j] = nodata;
				continue;
			}
			int startCol = j-m_FilterSize/2;
			if(startCol<0) startCol = 0;
			int endCol = j+m_FilterSize/2;
			if(endCol>ncol-1) endCol = ncol-1;
			float sum = 0;
			int num = 0;
			for(int p=startRow; p<=endRow; p++){
				for(int q=startCol; q<=endCol; q++){
					if(fabs(dataBuf[p*ncol+q]-nodata)>VERYSMALL){
						sum += dataBuf[p*ncol+q];
						num++;
					}
				}
			}
			outBuf[i*ncol+j] = sum/num;
		}
	}
	CThreeDR* theOutput = new CThreeDR;
	theOutput->setDataBuf(outBuf);
	theOutput->setDataName(extractFileName(m_OutputFileName).GetBuffer(1));
	theOutput->copyBasicFields(theInput);
	theOutput->calcStat();
	int writeFlag = theOutput->writeout(m_OutputFileName.GetBuffer(1), 0);

	//clean up
	delete theInput;
	delete theOutput;

	GetDlgItem(IDOK)->EnableWindow(TRUE);

	CDialog::OnOK();
}

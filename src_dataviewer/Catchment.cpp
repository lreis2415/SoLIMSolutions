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
// Catchment.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "Catchment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCatchment dialog


CCatchment::CCatchment(CWnd* pParent /*=NULL*/)
	: CDialog(CCatchment::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCatchment)
	m_InputFileName = _T("");
	m_OutputFileName = _T("");
	//}}AFX_DATA_INIT
}


void CCatchment::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCatchment)
	DDX_Text(pDX, IDC_InputFld, m_InputFileName);
	DDX_Text(pDX, IDC_OutputFld, m_OutputFileName);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
}


BEGIN_MESSAGE_MAP(CCatchment, CDialog)
	//{{AFX_MSG_MAP(CCatchment)
	ON_BN_CLICKED(IDC_InputBtn, OnInputBtn)
	ON_BN_CLICKED(IDC_OutputBtn, OnOutputBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatchment message handlers

void CCatchment::OnInputBtn(){
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

void CCatchment::OnOutputBtn(){
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

void CCatchment::OnOK(){
	// TODO: Add extra validation here
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

	if(m_InputFileName.IsEmpty()){
		MessageBox("No flow direction file.");
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
		MessageBox("Cannot open the input file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}

	m_ProgressBar.SetPos(20);

	float* outBuf = new float[theInput->getNumberOfRows()*theInput->getNumberOfColumns()];
	int opFlag = catchment(theInput->getDataBuf(),	outBuf,	theInput->getNumberOfRows(), theInput->getNumberOfColumns(), theInput->getNullDataValue());
	if(opFlag != 0){
		MessageBox("Error in catchment function.");
		delete theInput;
		delete outBuf;
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	m_ProgressBar.SetPos(80);
	CThreeDR* theOutput = new CThreeDR;
	theOutput->setDataBuf(outBuf);
	theOutput->setDataName(extractFileName(m_OutputFileName).GetBuffer(1));
	theOutput->copyBasicFields(theInput);
	theOutput->calcStat();
	int writeFlag = theOutput->writeout(m_OutputFileName.GetBuffer(1), 0);

	m_ProgressBar.SetPos(100);

	//clean up
	delete theInput;
	delete theOutput;

	GetDlgItem(IDOK)->EnableWindow(TRUE);

	CDialog::OnOK();
}

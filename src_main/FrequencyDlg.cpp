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
// FrequencyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "FrequencyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrequencyDlg dialog


CFrequencyDlg::CFrequencyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFrequencyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFrequencyDlg)
	m_InputFileName = _T("");
	m_OutputFileName = _T("");
	//}}AFX_DATA_INIT
}


void CFrequencyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFrequencyDlg)
	DDX_Text(pDX, IDC_InputFileFld, m_InputFileName);
	DDX_Text(pDX, IDC_OutputFileFld, m_OutputFileName);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
}


BEGIN_MESSAGE_MAP(CFrequencyDlg, CDialog)
	//{{AFX_MSG_MAP(CFrequencyDlg)
	ON_BN_CLICKED(IDC_InputFileBtn, OnInputFileBtn)
	ON_BN_CLICKED(IDC_OutputFileBtn, OnOutputFileBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrequencyDlg message handlers

void CFrequencyDlg::OnInputFileBtn(){
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

void CFrequencyDlg::OnOutputFileBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "Text Document (*.txt)|*.txt|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(FALSE, "txt", ".txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK){
		m_OutputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CFrequencyDlg::OnOK(){
	// TODO: Add extra validation here
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

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

	CThreeDR* theInput = new CThreeDR;
	int readFlag = theInput->readin(m_InputFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Error in openning the input file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}

	m_ProgressBar.SetPos(20);

	float* dataBuf = theInput->getDataBuf();
	int nrow = theInput->getNumberOfRows();
	int ncol = theInput->getNumberOfColumns();
	int numOfPixels = nrow*ncol;
	float nodata = theInput->getNullDataValue();

	List<ValueNode>* theList = new List<ValueNode>;

	theList->addHead(new ValueNode(dataBuf[0], 1));
	for(int i = 1; i<numOfPixels; i++){
		if(fabs(dataBuf[i]-nodata)<VERYSMALL) continue;
		ValueNode* temp = theList->first;
		bool greatest = true;
		while(temp != NULL){
			float a = dataBuf[i];
			float b = temp->value;
			if(fabs(a-b)<VERYSMALL){
				temp->num++;
				greatest = false;
				break;
			}
			if(a<b){
				theList->insertBefore(temp, new ValueNode(dataBuf[i], 1));
				greatest = false;
				break;
			}
			temp = temp->next;
		}
		if(greatest)
			theList->insertAfter(theList->last, new ValueNode(dataBuf[i], 1));
	}

	m_ProgressBar.SetPos(80);

	ofstream fFrequencyList;
	fFrequencyList.open(m_OutputFileName);
	if(fFrequencyList.fail()){
		MessageBox("Cannot open the output file for writing.");
		GetDlgItem(IDErrorMatrixOK)->EnableWindow(TRUE);
		return;
	}
	ValueNode* temp = theList->first;
	while(temp!=NULL){
		fFrequencyList<<temp->value<<"	"<<temp->num<<endl;
		temp = temp->next;
	}
	fFrequencyList.close();
    
	delete theList;
	delete theInput;
	m_ProgressBar.SetPos(100);

	CDialog::OnOK();
}

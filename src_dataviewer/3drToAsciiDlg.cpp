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
// 3drToAsciiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "3drToAsciiDlg.h"
#include ".\3drtoasciidlg.h"


// C3drToAsciiDlg dialog

IMPLEMENT_DYNAMIC(C3drToAsciiDlg, CDialog)
C3drToAsciiDlg::C3drToAsciiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C3drToAsciiDlg::IDD, pParent)
	, m_InputFileName(_T(""))
	, m_OutputFileName(_T(""))
{
}

C3drToAsciiDlg::~C3drToAsciiDlg()
{
}

void C3drToAsciiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_InputFileFld, m_InputFileName);
	DDX_Text(pDX, IDC_OutputFileFld, m_OutputFileName);
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
}


BEGIN_MESSAGE_MAP(C3drToAsciiDlg, CDialog)
	ON_BN_CLICKED(IDC_InputFileBtn, OnBnClickedInputfilebtn)
	ON_BN_CLICKED(IDC_OutputFileBtn, OnBnClickedOutputfilebtn)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// C3drToAsciiDlg message handlers

void C3drToAsciiDlg::OnBnClickedInputfilebtn()
{
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	

	if(m_ldFile.DoModal() == IDOK){
		m_InputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void C3drToAsciiDlg::OnBnClickedOutputfilebtn()
{
	static char BASED_CODE szFilter[] = "Aacii Files (*.asc)|*.asc|All Files (*.*)|*.*||";
		CFileDialog m_ldFile(FALSE, "asc", ".asc", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
		m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK){
		m_OutputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void C3drToAsciiDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);
	//m_ProgressBar.SetPos(1);

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
	int numOfPixels = nrow*ncol;

	FILE *fp;
	fp=fopen(m_OutputFileName.GetBuffer(1), "w");

	fprintf(fp, "%s %d\n", "ncols        ", ncol);
	fprintf(fp, "%s %d\n", "nrows        ", nrow);
	fprintf(fp, "%s %f\n", "xllcorner    ", theInput->getXmin());
	fprintf(fp, "%s %f\n", "yllcorner    ", theInput->getYmin());
	fprintf(fp, "%s %f\n", "cellsize     ", theInput->getCellSize());
	fprintf(fp, "%s %f\n", "NODATA_value ", theInput->getNullDataValue()); 


	for(int i=0;i<nrow;i++)
	{
		int m = 100*i/nrow;
		m_ProgressBar.SetPos(m);
		for(int j=0;j<ncol;j++)
		   fprintf(fp, "%f ", theInput->getDataBuf()[i*ncol+j]);
		fprintf(fp, "\n");
	}


	fclose(fp);
	
	m_ProgressBar.SetPos(100);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	delete theInput;
	CDialog::OnOK();
}

void C3drToAsciiDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

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
// ColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "ColorDlg.h"
#include "utility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog


CColorDlg::CColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorDlg)
	m_ColorFileName = _T("");
	m_InputFileName = _T("");
	m_OutputFileName = _T("");
	//}}AFX_DATA_INIT
}


void CColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorDlg)
	DDX_Text(pDX, IDC_ColorFileNameFld, m_ColorFileName);
	DDX_Text(pDX, IDC_InputFileNameFld, m_InputFileName);
	DDX_Text(pDX, IDC_OutputFileNameFld, m_OutputFileName);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
}


BEGIN_MESSAGE_MAP(CColorDlg, CDialog)
	//{{AFX_MSG_MAP(CColorDlg)
	ON_BN_CLICKED(IDC_ColorFileNameBtn, OnColorFileNameBtn)
	ON_BN_CLICKED(IDC_InputFileNameBtn, OnInputFileNameBtn)
	ON_BN_CLICKED(IDC_OutputFileNameBtn, OnOutputFileNameBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorDlg message handlers
void CColorDlg::OnInputFileNameBtn(){
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

void CColorDlg::OnColorFileNameBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "Color Palette Files (*.pal)|*.pal|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.pal", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_ColorFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}				
}

void CColorDlg::OnOutputFileNameBtn(){
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

void CColorDlg::OnOK(){
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);


	if(m_InputFileName.IsEmpty()){
		MessageBox("No data file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if(m_ColorFileName.IsEmpty()){
		MessageBox("No data file.");
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
		MessageBox("Error in openning the data file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	float* dataBuf = theInput->getDataBuf();

	ifstream fin;
	fin.open(m_ColorFileName);
	if(fin.fail()){
		MessageBox("Cannot open the color file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	float tempVal;
	int tempR, tempG, tempB;
	int NumberOfColors = 0;
	while(!fin.eof()){
		fin>>tempVal>>tempR>>tempG>>tempB;
		NumberOfColors++;
	}
	fin.close();
	PALENTRY* thePalette = new PALENTRY[NumberOfColors];

	fin.open(m_ColorFileName);
	for(int p=0; p<NumberOfColors; p++){
		thePalette[p].index = p;
		fin>>tempVal>>tempR>>tempG>>tempB; //since r, g, b are unsigned char,
		thePalette[p].rasterval = tempVal;  //cannot directly take read-in values
		thePalette[p].r = tempR;
		thePalette[p].g = tempG;
		thePalette[p].b = tempB;
	}
	fin.close();
    m_ProgressBar.SetPos(50);


	theInput->setNumberOfColors(NumberOfColors);
	theInput->setPalette(thePalette);
	int writeFlag = theInput->writeout(m_OutputFileName.GetBuffer(1), 0);

	delete theInput;
	m_ProgressBar.SetPos(100);

	GetDlgItem(IDOK)->EnableWindow(TRUE);	

	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

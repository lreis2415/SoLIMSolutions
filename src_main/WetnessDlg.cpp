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
// WetnessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "WetnessDlg.h"
#include "ThreeDR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWetnessDlg dialog


CWetnessDlg::CWetnessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWetnessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWetnessDlg)
	m_GradientFileName = _T("");
	m_OutputFileName = _T("");
	m_UDAFileName = _T("");
	//}}AFX_DATA_INIT
}


void CWetnessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWetnessDlg)
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
	DDX_Text(pDX, IDC_GradientFileNameFld, m_GradientFileName);
	DDX_Text(pDX, IDC_OutputFileNameFld, m_OutputFileName);
	DDX_Text(pDX, IDC_UDAFileNameFld, m_UDAFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWetnessDlg, CDialog)
	//{{AFX_MSG_MAP(CWetnessDlg)
	ON_BN_CLICKED(IDC_GradientFileNameBtn, OnGradientFileNameBtn)
	ON_BN_CLICKED(IDC_OutputFileNameBtn, OnOutputFileNameBtn)
	ON_BN_CLICKED(IDC_UDAFileNameBtn, OnUDAFileNameBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWetnessDlg message handlers
void CWetnessDlg::OnUDAFileNameBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_UDAFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}		
}

void CWetnessDlg::OnGradientFileNameBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_GradientFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}		
}

void CWetnessDlg::OnOutputFileNameBtn(){
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

void CWetnessDlg::OnOK(){
	// TODO: Add extra validation here
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

	if(m_UDAFileName.IsEmpty()){
		MessageBox("No input file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_GradientFileName.IsEmpty()){
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

	CThreeDR* theUDA = new CThreeDR;
	int readFlag = theUDA->readin(m_UDAFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the UDA file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	CThreeDR* theGradient = new CThreeDR;
	readFlag = theGradient->readin(m_GradientFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the gradient file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	int nrow = theUDA->getNumberOfRows();
	int ncol = theUDA->getNumberOfColumns();
	int numOfPixels = nrow*ncol;
	float nodata = theUDA->getNullDataValue();
	float cellsize = theUDA->getCellSize();


	for(int i=0;i<nrow;i++)
	{
		m_ProgressBar.SetPos(100*i/nrow);
		for(int j=0;j<ncol;j++)
		{
		    if(fabs(theUDA->getDataBuf()[i*ncol+j]-nodata)<VERYSMALL || fabs(theGradient->getDataBuf()[i*ncol+j]-nodata)<VERYSMALL){
			theUDA->getDataBuf()[i*ncol+j] = nodata;
			continue;
		   }

	      float a = theUDA->getDataBuf()[i*ncol+j]/cellsize;
		  if(a<0.1) a = 0.1; //avoid negative values
		  float s = theGradient->getDataBuf()[i*ncol+j];
		  if(s<0.1/cellsize) s=0.1/cellsize;
		  theUDA->getDataBuf()[i*ncol+j] = log(a/s);
		}
	}

/*

	for(int i = 0; i<numOfPixels; i++){
		m_ProgressBar.SetPos(i*100/numOfPixels);
		if(fabs(theUDA->getDataBuf()[i]-nodata)<VERYSMALL || fabs(theGradient->getDataBuf()[i]-nodata)<VERYSMALL){
			theUDA->getDataBuf()[i] = nodata;
			continue;
		}

	    float a = theUDA->getDataBuf()[i]/cellsize;
		if(a<0.1) a = 0.1; //avoid negative values
		float s = theGradient->getDataBuf()[i];
		if(s<0.1/cellsize) s=0.1/cellsize;
		theUDA->getDataBuf()[i] = log(a/s);
	}*/
	m_ProgressBar.SetPos(100);

	theUDA->setDataName(extractFileName(m_OutputFileName).GetBuffer(1));
	theUDA->calcStat();
	int writeFlag = theUDA->writeout(m_OutputFileName.GetBuffer(1), 0);

	//clean up
	delete theUDA;
	delete theGradient;
	GetDlgItem(IDOK)->EnableWindow(TRUE);		
	CDialog::OnOK();
}

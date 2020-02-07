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
// RidgeLineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "RidgeLineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRidgeLineDlg dialog


CRidgeLineDlg::CRidgeLineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRidgeLineDlg::IDD, pParent)
	, m_Proportion(0.25)
{
	//{{AFX_DATA_INIT(CRidgeLineDlg)
	m_InputFileName = _T("");
	m_OutputFileName = _T("");
	m_Label = 1;
	m_Alg = 0;
	m_Gradient = 100;
	m_Elevation = 0.0f;
	m_GradientFileName = _T("");
	m_TerrainPartitionFileName = _T("");
	m_TPEleDiff = 0.0f;
	//}}AFX_DATA_INIT
}


void CRidgeLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRidgeLineDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
	DDX_Text(pDX, IDC_InputFileNameFld, m_InputFileName);
	DDX_Text(pDX, IDC_OutputFileNameFld, m_OutputFileName);
	DDX_Text(pDX, IDC_LabelFld, m_Label);
	DDX_Radio(pDX, IDC_PDAlgRdo, m_Alg);
	DDX_Text(pDX, IDC_GradientFld, m_Gradient);
	DDX_Text(pDX, IDC_ElevationFld, m_Elevation);
	DDX_Text(pDX, IDC_GradientFileFld, m_GradientFileName);
	DDX_Text(pDX, IDC_TerrainPartitionFileFld, m_TerrainPartitionFileName);
	DDX_Text(pDX, IDC_TPEleDiffFld, m_TPEleDiff);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_TopProportionFld, m_Proportion);
}


BEGIN_MESSAGE_MAP(CRidgeLineDlg, CDialog)
	//{{AFX_MSG_MAP(CRidgeLineDlg)
	ON_BN_CLICKED(IDC_InputFileNameBtn, OnInputFileNameBtn)
	ON_BN_CLICKED(IDC_OutputFileNameBtn, OnOutputFileNameBtn)
	ON_BN_CLICKED(IDC_GradientFileBtn, OnGradientFileBtn)
	ON_BN_CLICKED(IDC_TerrainPartitionFileBtn, OnTerrainPartitionFileBtn)
	ON_BN_CLICKED(IDC_OMAlgRdo, OnOMAlgRdo)
	ON_BN_CLICKED(IDC_PDAlgRdo, OnPDAlgRdo)
	ON_BN_CLICKED(IDC_ShiAlgRdo, OnShiAlgRdo)
	ON_BN_CLICKED(IDC_SkidmoreAlgRdo, OnSkidmoreAlgRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRidgeLineDlg message handlers

BOOL CRidgeLineDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//m_ProgressBar.ShowWindow(SW_HIDE);
	itemAvailability();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRidgeLineDlg::OnInputFileNameBtn(){
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

void CRidgeLineDlg::OnOutputFileNameBtn(){
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

void CRidgeLineDlg::OnGradientFileBtn(){
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

void CRidgeLineDlg::OnTerrainPartitionFileBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_TerrainPartitionFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}		
}

void CRidgeLineDlg::OnOMAlgRdo(){
	// TODO: Add your control notification handler code here
	itemAvailability();
}

void CRidgeLineDlg::OnPDAlgRdo(){
	// TODO: Add your control notification handler code here
	itemAvailability();	
}

void CRidgeLineDlg::OnShiAlgRdo(){
	// TODO: Add your control notification handler code here
	itemAvailability();	
}

void CRidgeLineDlg::OnSkidmoreAlgRdo(){
	// TODO: Add your control notification handler code here
	itemAvailability();	
}

void CRidgeLineDlg::itemAvailability(){
	UpdateData(TRUE);
	if(m_Alg!=3){
		GetDlgItem(IDC_TerrainPartitionFileLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_TerrainPartitionFileFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_TerrainPartitionFileBtn)->EnableWindow(FALSE);
		GetDlgItem(IDC_TPEleDiffLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_TPEleDiffFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_TopProportionLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_TopProportionFld)->EnableWindow(FALSE);
	}
	else{
		GetDlgItem(IDC_TerrainPartitionFileLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_TerrainPartitionFileFld)->EnableWindow(TRUE);
		GetDlgItem(IDC_TerrainPartitionFileBtn)->EnableWindow(TRUE);
		GetDlgItem(IDC_TPEleDiffLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_TPEleDiffFld)->EnableWindow(TRUE);
		GetDlgItem(IDC_TopProportionLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_TopProportionFld)->EnableWindow(TRUE);
	}
}

void CRidgeLineDlg::OnOK(){
	// TODO: Add extra validation here
	UpdateData(TRUE);	
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.ShowWindow(SW_SHOW);
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
		MessageBox("Cannot open the input file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	int nrow = theInput->getNumberOfRows();
	int ncol = theInput->getNumberOfColumns();
	float cellsize = theInput->getCellSize();
	float* dem = theInput->getDataBuf();
	float nodata = theInput->getNullDataValue();
	int opFlag = 0;
	m_ProgressBar.SetPos(10);

	float* outBuf = new float[nrow*ncol];
	switch(m_Alg){
	case 0:
		opFlag = ridgelines1(dem, outBuf, nrow, ncol, m_Label, nodata);
		if(opFlag != 0){
			MessageBox("Error in calculation with Peucker's Alg.");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			m_ProgressBar.SetPos(0);
			return;
		}
		break;
	case 1:
		opFlag = ridgelines2(dem, outBuf, nrow, ncol, m_Label, nodata);
		if(opFlag != 0){
			MessageBox("Error in calculation with O'C. and Mark's Alg.");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			m_ProgressBar.SetPos(0);
			return;
		}
		break;
	case 2:
		opFlag = ridgelines3(dem, outBuf, nrow, ncol, m_Label, nodata);
		if(opFlag != 0){
			MessageBox("Error in calculation with Skidmore's Alg.");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			m_ProgressBar.SetPos(0);
			return;
		}
		break;
	case 3:
		if(m_TerrainPartitionFileName.IsEmpty()){
			MessageBox("No terrain partition file.");
			delete [] outBuf;
			delete theInput;
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			m_ProgressBar.SetPos(0);
			return;
		}
		CThreeDR* theTP = new CThreeDR;
		readFlag = theTP->readin(m_TerrainPartitionFileName.GetBuffer(1), 0);
		if(readFlag != 0){
			MessageBox("Cannot open the slope file or file format has errors.");
			delete theTP;
			delete [] outBuf;
			delete theInput;
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			m_ProgressBar.SetPos(0);
			return;
		}
		float* tpBuf = theTP->getDataBuf();
		opFlag = ridgelineShi(tpBuf, dem, outBuf, m_TPEleDiff, m_Proportion, nrow, ncol, m_Label, nodata);
		if(opFlag != 0){
			MessageBox("Error in calculation with Shi's Alg.");
			delete theTP;
			delete [] outBuf;
			delete theInput;
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			m_ProgressBar.SetPos(0);
			return;
		}
		delete theTP;
		break;
	}
	m_ProgressBar.SetPos(70);

	int i = 0;
	if(m_Elevation>0)
		for(i=0; i<nrow*ncol; i++)
			if(outBuf[i]==(float)m_Label && dem[i]<m_Elevation)
				outBuf[i] = 0;
	m_ProgressBar.SetPos(80);
	
	if(m_Gradient<100){
		if(m_GradientFileName.IsEmpty()){
			MessageBox("No slope gradient file. Will compute slope with the input DEM.");
			float* gradBuf = new float[nrow*ncol];
			opFlag = gradientSqrEvans(dem, gradBuf, nrow, ncol, DEFAULTNEIGHBOR, cellsize, nodata);
			for(i=0; i<nrow*ncol; i++)
				if(outBuf[i]==m_Label && gradBuf[i]>=m_Gradient)
					outBuf[i] = 0;
			delete [] gradBuf;
		}
		else{
			CThreeDR* theGradient = new CThreeDR;
			readFlag = theGradient->readin(m_GradientFileName.GetBuffer(1), 0);
			if(readFlag != 0){
				MessageBox("Cannot open the slope file or file format has errors. Will compute slope with the input DEM.");
				float* gradBuf = new float[nrow*ncol];
				opFlag = gradientSqrEvans(dem, gradBuf, nrow, ncol, DEFAULTNEIGHBOR, cellsize, nodata);
				for(i=0; i<nrow*ncol; i++)
					if(outBuf[i]==m_Label && gradBuf[i]>=m_Gradient)
						outBuf[i] = 0;
				delete [] gradBuf;
			}
			else{
				for(i=0; i<nrow*ncol; i++)
					if(outBuf[i]==m_Label && theGradient->getDataBuf()[i]>=m_Gradient)
						outBuf[i] = 0;
			}
			delete theGradient;
		}
	}
	m_ProgressBar.SetPos(100);

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

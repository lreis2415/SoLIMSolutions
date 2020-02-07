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
// 3drTo3dmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "3drTo3dmDlg.h"


// C3drTo3dmDlg dialog

IMPLEMENT_DYNAMIC(C3drTo3dmDlg, CDialog)
C3drTo3dmDlg::C3drTo3dmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C3drTo3dmDlg::IDD, pParent)
	, m_InputFileName(_T(""))
	, m_OutputFileName(_T(""))
	, m_DataUnit(_T("Meter"))
	, m_GridUnit(_T("Meter"))
{
}

C3drTo3dmDlg::~C3drTo3dmDlg()
{
}

void C3drTo3dmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_InputFileFld, m_InputFileName);
	DDX_Text(pDX, IDC_OutputFileFld, m_OutputFileName);
	DDX_CBString(pDX, IDC_DataUnitCmb, m_DataUnit);
	DDX_CBString(pDX, IDC_GridUnitCmb, m_GridUnit);
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
}


BEGIN_MESSAGE_MAP(C3drTo3dmDlg, CDialog)
	ON_BN_CLICKED(IDC_InputFileBtn, OnBnClickedInputfilebtn)
	ON_BN_CLICKED(IDC_OutputFileBtn, OnBnClickedOutputfilebtn)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// C3drTo3dmDlg message handlers

void C3drTo3dmDlg::OnBnClickedInputfilebtn(){
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

void C3drTo3dmDlg::OnBnClickedOutputfilebtn(){
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "3dm Files (*.3dm)|*.3dm|All Files (*.*)|*.*||";
		CFileDialog m_ldFile(FALSE, "3dm", ".3dm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
		m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK){
		m_OutputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void C3drTo3dmDlg::OnBnClickedOk(){
	// TODO: Add your control notification handler code here
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
	}

	int nrow = theInput->getNumberOfRows();
	int ncol = theInput->getNumberOfColumns();
	float cellsize = theInput->getCellSize();
	double xmin = theInput->getXmin();
	double ymin = theInput->getYmin();
	CString dataunit = theInput->getDataUnits();
	CString gridunit = theInput->getGridUnits();
	float nodata = theInput->getNullDataValue();
	theInput->calcStat();
	float dataMin = theInput->getDataMin();
	float dataMax = theInput->getDataMax();
	float* dataBuf = theInput->getDataBuf();

	int numOfPixels = nrow*ncol;
	//Create the photo buffer
	unsigned char* photoBuf = new unsigned char[nrow*ncol];
	//Strech the buf to 0-255
	for(int i=0; i<numOfPixels; i++)
		photoBuf[i] = (dataBuf[i]-dataMin)*255/(dataMax-dataMin);
	
	//create the dem buffer
	int nrow2 = nrow+1;
	int ncol2 = ncol+1;
	float* demBuf = new float[nrow2*ncol2];
	//first row
	demBuf[0] = dataBuf[0];
	demBuf[ncol2-1] = dataBuf[ncol-1];
	for(int j=1; j<ncol2-1; j++)
		demBuf[j] = (dataBuf[j-1]+dataBuf[j])/2;
	//middle rows
	for(i=1; i<nrow2-1; i++){
		m_ProgressBar.SetPos(100*i/nrow);
		demBuf[i*ncol2] = (dataBuf[(i-1)*ncol]+dataBuf[i*ncol])/2;
		demBuf[(i+1)*ncol2-1] = (dataBuf[i*ncol-1]+dataBuf[(i+1)*ncol-1])/2;
		for(j=1; j<ncol2-1; j++)
			demBuf[i*ncol2+j] = (dataBuf[(i-1)*ncol+j-1]+dataBuf[(i-1)*ncol+j]+dataBuf[i*ncol+j-1]+dataBuf[i*ncol+j])/4;
	}
	//last row
	demBuf[(nrow2-1)*ncol2] = dataBuf[(nrow-1)*ncol];
	demBuf[nrow2*ncol2-1]=dataBuf[nrow*ncol-1];
	for(j=1; j<ncol2-1; j++)
		demBuf[(nrow2-1)*ncol2+j] = (dataBuf[(nrow-1)*ncol+j-1]+dataBuf[(nrow-1)*ncol+j])/2;
	
	CThreeDM* output = new CThreeDM;
	output->setNumberOfRows(nrow+1);
	output->setNumberOfColumns(ncol+1);
	output->setFileType("3DMAPPER");
	output->setGridUnits(m_GridUnit.GetBuffer(1));
	output->setXmin(xmin);
	output->setYmin(ymin);
	output->setCellSize(cellsize);
	output->setDataUnits(m_DataUnit.GetBuffer(1));
	output->setDataMin(dataMin);
	output->setDataMax(dataMax);
	output->setNullValue(nodata);
	output->setOriginalNullValue(nodata);
	output->setPhotoBuf(photoBuf);
	output->setDataBuf(demBuf);
	output->writeout(m_OutputFileName.GetBuffer(1));

	delete theInput;
	delete output;

	OnOK();
}

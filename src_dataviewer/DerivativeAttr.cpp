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
// DerivativeAttr.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "DerivativeAttr.h"


// CDerivativeAttr dialog

IMPLEMENT_DYNAMIC(CDerivativeAttr, CDialog)
CDerivativeAttr::CDerivativeAttr(CWnd* pParent /*=NULL*/)
	: CDialog(CDerivativeAttr::IDD, pParent)
	, m_InputFileName(_T(""))
	, m_OutputFileName(_T(""))
	, m_TerrainAttrName(_T("Slope"))
	, m_Algo(_T("Evans-Young"))
	, m_NeighborSize(100)
{
}

CDerivativeAttr::~CDerivativeAttr()
{
}

void CDerivativeAttr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_InputFileFld, m_InputFileName);
	DDX_Text(pDX, IDC_OutputFileFld, m_OutputFileName);
	DDX_CBString(pDX, IDC_TerrainAttrCmb, m_TerrainAttrName);
	DDX_CBString(pDX, IDC_AlgoCmb, m_Algo);
	DDX_Text(pDX, IDC_NeighborSizeFld, m_NeighborSize);
}


BEGIN_MESSAGE_MAP(CDerivativeAttr, CDialog)
	ON_BN_CLICKED(IDC_InputFileBtn, OnBnClickedInputfilebtn)
	ON_BN_CLICKED(IDC_OutputFileBtn, OnBnClickedOutputfilebtn)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDerivativeAttr message handlers
void CDerivativeAttr::OnBnClickedInputfilebtn(){
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

void CDerivativeAttr::OnBnClickedOutputfilebtn(){
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

void CDerivativeAttr::OnBnClickedOk(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);

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
		MessageBox("Cannot open the input file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}

	int nrow = theInput->getNumberOfRows();
	int ncol = theInput->getNumberOfColumns();
	float cellsize = theInput->getCellSize();
	float nodata = theInput->getNullDataValue();
	float* DEMBuf = theInput->getDataBuf();
	float* outBuf = new float[nrow*ncol];

	int type = -1;
	if(m_TerrainAttrName == "Slope")
		type = 2;
	else if(m_TerrainAttrName == "Aspect")
		type = 4;
	else if(m_TerrainAttrName == "Mean curvature")
		type = 5;
	else if(m_TerrainAttrName == "Unsphericity")
		type = 6;
	else if(m_TerrainAttrName == "Difference curvature")
		type = 7;
	else if(m_TerrainAttrName == "Horizontal curvature")
		type = 8;
	else if(m_TerrainAttrName == "Planform curvature")
		type = 9;
	else if(m_TerrainAttrName == "Profile curvature")
		type = 10;
	else if(m_TerrainAttrName == "Roter")
		type = 11;
	else if(m_TerrainAttrName == "Horizontal excess curvature")
		type = 12;
	else if(m_TerrainAttrName == "Vertical excess curvature")
		type = 13;
	else if(m_TerrainAttrName == "Min curvature")
		type = 14;
	else if(m_TerrainAttrName == "Max curvature")
		type = 15;
	else if(m_TerrainAttrName == "Total Gaussian curvature")
		type = 16;
	else if(m_TerrainAttrName == "Total ring curvature")
		type = 17;
	else if(m_TerrainAttrName == "Total accumulation curvature")
		type = 18;
	else if(m_TerrainAttrName == "Curvature")
		type = 19;
	else if(m_TerrainAttrName == "Tangent curvature")
		type = 20;

	int flag = -1;
	if(type==2){
		/*if(m_Algo=="Shi"){
			if(m_NeighborShape=="Circular")
				flag = gradientCirShi(DEMBuf,outBuf,nrow, ncol, cellsize, m_NeighborSize, nodata);
			else if(m_NeighborShape=="Square")
				flag = gradientSqrShi(DEMBuf,outBuf,nrow, ncol, cellsize, m_NeighborSize, nodata);
		}*/
		if(m_Algo=="Evans-Young"){
				flag = gradientSqrEvans(DEMBuf,outBuf,nrow, ncol, cellsize, m_NeighborSize, nodata);
		}
		else if(m_Algo=="Horn"){
				flag = gradientHorn(DEMBuf,outBuf,nrow, ncol, cellsize, m_NeighborSize, nodata);
		}
		else if(m_Algo=="Zevenbergen-Thorne"){
			flag = gradientZT(DEMBuf,outBuf,nrow, ncol, cellsize, m_NeighborSize, nodata);
		}
	}
	if(type==4){
	
		if(m_Algo=="Evans-Young"){

				flag = aspectSqrEvans(DEMBuf,outBuf,nrow, ncol, cellsize, m_NeighborSize, nodata);
		}
		else if(m_Algo=="Horn"){

				flag = aspectHorn(DEMBuf,outBuf,nrow, ncol, cellsize, m_NeighborSize, nodata);
		}
		else if(m_Algo=="Zevenbergen-Thorne"){
			flag = aspectZT(DEMBuf,outBuf,nrow, ncol, cellsize, m_NeighborSize, nodata);
		}
	}
	if(type>4){
		if(m_Algo == "Evans-Young" || m_Algo == "Horn"){

				flag = curvatureSqrEvans(DEMBuf, outBuf, nrow, ncol, cellsize, m_NeighborSize, type, nodata);
		}
		else{

				flag = curvatureSqrZT(DEMBuf, outBuf, nrow, ncol, cellsize, m_NeighborSize, type, nodata);
		}
	}
	
	if(flag==-1){
		delete [] outBuf;
	}
	else if(flag != 0){
		MessageBox("Error in calculation.");
		delete [] outBuf;
	}
	else{
		CThreeDR* theOutput = new CThreeDR;
		theOutput->setDataBuf(outBuf);
		theOutput->setDataName(extractFileName(m_OutputFileName).GetBuffer(1));
		theOutput->copyBasicFields(theInput);
		theOutput->calcStat();
		int writeFlag = theOutput->writeout(m_OutputFileName.GetBuffer(1), 0);
		delete theOutput;
	}

	//clean up
	delete theInput;

	GetDlgItem(IDOK)->EnableWindow(TRUE);

	OnOK();
}

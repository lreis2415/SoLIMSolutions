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
// ClipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "ClipDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClipDlg dialog


CClipDlg::CClipDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClipDlg::IDD, pParent)
	, m_SpecifyExt(FALSE)
	, m_3dm(FALSE)
	, m_ExistingFileName(_T(""))
{
	//{{AFX_DATA_INIT(CClipDlg)
	m_InputFileName = _T("");
	m_NumOfCols = 0;
	m_NumOfRows = 0;
	m_OutputFileName = _T("");
	m_Xmax = 0.0;
	m_Xmin = 0.0;
	m_Ymax = 0.0;
	m_Ymin = 0.0;
	m_XYorRC = 0;
	//}}AFX_DATA_INIT
}

void CClipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClipDlg)
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
	DDX_Text(pDX, IDC_InputFileNameFld, m_InputFileName);
	DDX_Text(pDX, IDC_NumOfColsFld, m_NumOfCols);
	DDV_MinMaxUInt(pDX, m_NumOfCols, 0, 10000000);
	DDX_Text(pDX, IDC_NumOfRowsFld, m_NumOfRows);
	DDV_MinMaxUInt(pDX, m_NumOfRows, 0, 10000000);
	DDX_Text(pDX, IDC_OutputFileNameFld, m_OutputFileName);
	DDX_Text(pDX, IDC_XmaxFld, m_Xmax);
	DDX_Text(pDX, IDC_XminFld, m_Xmin);
	DDX_Text(pDX, IDC_YmaxFld, m_Ymax);
	DDX_Text(pDX, IDC_YminFld, m_Ymin);
	DDX_Radio(pDX, IDC_XYMaxRdo, m_XYorRC);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_ExsitingFileRdo, m_SpecifyExt);
	DDX_Radio(pDX, IDC_3drRdo, m_3dm);
	DDX_Text(pDX, IDC_ExistingFileNameFld, m_ExistingFileName);
}


BEGIN_MESSAGE_MAP(CClipDlg, CDialog)
	//{{AFX_MSG_MAP(CClipDlg)
	ON_BN_CLICKED(IDC_InputFileNameBtn, OnInputFileNameBtn)
	ON_BN_CLICKED(IDC_OutputFileNameBtn, OnOutputFileNameBtn)
	ON_BN_CLICKED(IDC_XYMaxRdo, OnXYMaxRdo)
	ON_BN_CLICKED(IDC_NoRowColRdo, OnNoRowColRdo)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ExsitingFileRdo, OnBnClickedExsitingfilerdo)
	ON_BN_CLICKED(IDC_SpecifyExtRdo, OnBnClickedSpecifyextrdo)
	ON_BN_CLICKED(IDC_ExistingFileNameBtn, OnBnClickedExistingfilenamebtn)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClipDlg message handlers

BOOL CClipDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//m_ProgressBar.ShowWindow(SW_HIDE);
	extType();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CClipDlg::OnInputFileNameBtn(){
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

void CClipDlg::OnOutputFileNameBtn(){
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

void CClipDlg::OnBnClickedExistingfilenamebtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	if(m_3dm){ //3dm
		static char BASED_CODE szFilter[] = "3dm Files (*.3dm)|*.3dm|All Files (*.*)|*.*||";
		CFileDialog m_ldFile(TRUE, NULL, "*.3dm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
		m_ldFile.m_ofn.lpstrTitle="Open";	
		if(m_ldFile.DoModal() == IDOK){
			m_ExistingFileName = m_ldFile.GetPathName();
			UpdateData(FALSE);
		}	
	}
	else{ //3dr
		static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
		CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
			m_ldFile.m_ofn.lpstrTitle="Open";	
		if(m_ldFile.DoModal() == IDOK){
			m_ExistingFileName = m_ldFile.GetPathName();
			UpdateData(FALSE);
		}	
	}
}

void CClipDlg::OnOK(){
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_ProgressBar.ShowWindow(SW_SHOW);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

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
	double cellsize = theInput->getCellSize();
	double xmin = theInput->getXmin();
	double ymin = theInput->getYmin();
	float* dataBuf = theInput->getDataBuf();
	float nodata = theInput->getNullDataValue();

	CThreeDR* theOutput = new CThreeDR;
	theOutput->copyBasicFields(theInput);
	theOutput->setDataName(theInput->getDataName());
	if(theInput->getPalette()!=NULL && theInput->getNumberOfColors()>0){
		theOutput->setNumberOfColors(theInput->getNumberOfColors());
		theOutput->setPalette(theInput->getPalette());
	}

	if(m_SpecifyExt==FALSE){ //use existing file
		if(m_ExistingFileName.IsEmpty()){
			MessageBox("No existing file specified.");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			m_ProgressBar.SetPos(0);
			return;
		}
		int existNRow = 0;
		int existNCol = 0;
		double existCellsize = 0;
		double existXmin = 0;
		double existYmin = 0;
		float existNodata = -9999;
		bool extMatch = true;
		if(m_3dm){ //file type is 3dm
			CThreeDM* exist3DM = new CThreeDM;
			exist3DM->readin(m_ExistingFileName.GetBuffer(1));
			existCellsize = exist3DM->getCellSize();
			if(fabs(cellsize-existCellsize)<VERYSMALL){
				existNRow = exist3DM->getNumberOfRows()-1;
				existNCol = exist3DM->getNumberOfColumns()-1;
				existXmin = exist3DM->getXmin();
				existYmin = exist3DM->getYmin();
			}
			else
				extMatch = false;
		}
		else{ //file type is 3dr
			CThreeDR* exist3DR = new CThreeDR;
			exist3DR->readin(m_ExistingFileName.GetBuffer(1), 0);
			existCellsize = exist3DR->getCellSize();
			if(fabs(cellsize-existCellsize)<VERYSMALL){
				existNRow = exist3DR->getNumberOfRows();
				existNCol = exist3DR->getNumberOfColumns();
				existXmin = exist3DR->getXmin();
				existYmin = exist3DR->getYmin();
			}
			else
				extMatch = false;
		}
		if(extMatch){
			int er = theInput->rowOfAPoint(existYmin);
			int sr = theInput->rowOfAPoint(existYmin+existCellsize*existNRow);
			int sc = theInput->colOfAPoint(existXmin);
			int ec = theInput->colOfAPoint(existXmin+existCellsize*existNCol);
			float* outBuf = new float[existNRow*existNCol];
			theOutput->setDataBuf(outBuf);
			int k=0;
			for(int i=sr; i<er; i++){
				m_ProgressBar.SetPos(100*i/nrow);
				for(int j=sc; j<ec; j++)
					if(i<0 || i>=nrow || j<0 || j>=ncol)
						outBuf[k++] = nodata;
					else
						outBuf[k++] = dataBuf[i*ncol+j];
			}
			theOutput->setNumberOfRows(existNRow);
			theOutput->setNumberOfColumns(existNCol);
			theOutput->setCellSize(existCellsize);
			theOutput->setXmin(existXmin);
			theOutput->setYmin(existYmin);
			theOutput->calcStat();
			readFlag = theOutput->writeout(m_OutputFileName.GetBuffer(1), 0);
		}
		else
			MessageBox("Cellsizes of the input file and the existing file do not match.", "Stop", MB_ICONSTOP);
	}
	else{					//use specified extent
		int colMin = theInput->colOfAPoint(m_Xmin);
		int rowMax = 0;
		int colMax = 0;
		int rowMin = 0;
		if(fabs(m_Ymin-ymin)<VERYSMALL*VERYSMALL) //right on the border
			rowMax = nrow - 1;
		else{
			rowMax = theInput->rowOfAPoint(m_Ymin);
			if(fabs((nrow-rowMax)*cellsize + ymin - m_Ymin)<VERYSMALL)
				rowMax -= 1;
		}
		if(colMin < 0 || rowMax > nrow){
			MessageBox("Out of the extent of the data.");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			m_ProgressBar.SetPos(0);
			return;
		}

		switch(m_XYorRC){
		case 0:
			colMax = theInput->colOfAPoint(m_Xmax);
			if(fabs(xmin + colMax*cellsize - m_Xmax)<VERYSMALL)
				colMax -= 1;
			rowMin = theInput->rowOfAPoint(m_Ymax);
			m_NumOfRows = rowMax - rowMin + 1;
			m_NumOfCols = colMax - colMin + 1;
			break;
		case 1:
			rowMin = rowMax - m_NumOfRows + 1;
			colMax = colMin + m_NumOfCols - 1;
			break;
		}

		if(m_NumOfRows>0 && m_NumOfCols>0){
			theOutput->setNumberOfColumns(m_NumOfCols);
			theOutput->setNumberOfRows(m_NumOfRows);
			theOutput->setXmin(theInput->xOfACell(colMin)-cellsize/2);
			theOutput->setYmin(theInput->yOfACell(rowMax)-cellsize/2);
			theOutput->setDataName(theInput->getDataName());
			float* outBuf = new float[m_NumOfRows*m_NumOfCols];
			theOutput->setDataBuf(outBuf);
			int k=0;
			for(int i=0; i<nrow; i++){
				m_ProgressBar.SetPos(100*i/nrow);
				if(i>=rowMin && i<=rowMax)
					for(int j=0; j<ncol; j++)
						if(j>=colMin && j<=colMax){
							outBuf[k]=dataBuf[i*ncol+j];
							k++;
						}
			}
			theOutput->calcStat();
			readFlag = theOutput->writeout(m_OutputFileName.GetBuffer(1), 0);
		}
		else
			MessageBox("Invalid spatial extent.");
	}
	m_ProgressBar.SetPos(100);
	delete theInput;
	delete theOutput;

	GetDlgItem(IDOK)->EnableWindow(TRUE);
	CDialog::OnOK();
}

void CClipDlg::OnBnClickedExsitingfilerdo(){
	// TODO: Add your control notification handler code here
	extType();
}

void CClipDlg::OnBnClickedSpecifyextrdo(){
	// TODO: Add your control notification handler code here
	extType();
}

void CClipDlg::OnXYMaxRdo(){
	// TODO: Add your control notification handler code here
	setXYorRC();	
}

void CClipDlg::OnNoRowColRdo(){
	// TODO: Add your control notification handler code here
	setXYorRC();	
}

void CClipDlg::extType(void){
	UpdateData(TRUE);
	switch(m_SpecifyExt){
		case 0: 
			GetDlgItem(IDC_ExistingFileGrp)->EnableWindow(TRUE);
			GetDlgItem(IDC_ExistingFileNameFld)->EnableWindow(TRUE);
			GetDlgItem(IDC_ExistingFileNameBtn)->EnableWindow(TRUE);
			GetDlgItem(IDC_3drRdo)->EnableWindow(TRUE);
			GetDlgItem(IDC_3dmRdo)->EnableWindow(TRUE);
			GetDlgItem(IDC_ScissorExtentGbx)->EnableWindow(FALSE);
			GetDlgItem(IDC_XminLbl)->EnableWindow(FALSE);
			GetDlgItem(IDC_XminFld)->EnableWindow(FALSE);
			GetDlgItem(IDC_YminLbl)->EnableWindow(FALSE);
			GetDlgItem(IDC_YminFld)->EnableWindow(FALSE);
			GetDlgItem(IDC_XYMaxRdo)->EnableWindow(FALSE);
			GetDlgItem(IDC_NoRowColRdo)->EnableWindow(FALSE);
			GetDlgItem(IDC_XmaxLbl)->EnableWindow(FALSE);
			GetDlgItem(IDC_XmaxFld)->EnableWindow(FALSE);
			GetDlgItem(IDC_YmaxLbl)->EnableWindow(FALSE);
			GetDlgItem(IDC_YmaxFld)->EnableWindow(FALSE);
			GetDlgItem(IDC_NumOfRowsLbl)->EnableWindow(FALSE);
			GetDlgItem(IDC_NumOfRowsFld)->EnableWindow(FALSE);
			GetDlgItem(IDC_NumOfColsLbl)->EnableWindow(FALSE);
			GetDlgItem(IDC_NumOfColsFld)->EnableWindow(FALSE);
			break;
		case 1:
			GetDlgItem(IDC_ExistingFileGrp)->EnableWindow(FALSE);
			GetDlgItem(IDC_ExistingFileNameFld)->EnableWindow(FALSE);
			GetDlgItem(IDC_ExistingFileNameBtn)->EnableWindow(FALSE);
			GetDlgItem(IDC_3drRdo)->EnableWindow(FALSE);
			GetDlgItem(IDC_3dmRdo)->EnableWindow(FALSE);
			GetDlgItem(IDC_ScissorExtentGbx)->EnableWindow(TRUE);
			GetDlgItem(IDC_XminLbl)->EnableWindow(TRUE);
			GetDlgItem(IDC_XminFld)->EnableWindow(TRUE);
			GetDlgItem(IDC_YminLbl)->EnableWindow(TRUE);
			GetDlgItem(IDC_YminFld)->EnableWindow(TRUE);
			GetDlgItem(IDC_XYMaxRdo)->EnableWindow(TRUE);
			GetDlgItem(IDC_NoRowColRdo)->EnableWindow(TRUE);
			setXYorRC();
			break;
	}
}

void CClipDlg::setXYorRC(){
	UpdateData(TRUE);
	switch(m_XYorRC){
	case 0:
		GetDlgItem(IDC_XmaxLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_XmaxFld)->EnableWindow(TRUE);
		GetDlgItem(IDC_YmaxLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_YmaxFld)->EnableWindow(TRUE);
		GetDlgItem(IDC_NumOfRowsLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_NumOfRowsFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_NumOfColsLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_NumOfColsFld)->EnableWindow(FALSE);
		break;
	case 1:
		GetDlgItem(IDC_NumOfRowsLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_NumOfRowsFld)->EnableWindow(TRUE);
		GetDlgItem(IDC_NumOfColsLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_NumOfColsFld)->EnableWindow(TRUE);
		GetDlgItem(IDC_XmaxLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_XmaxFld)->EnableWindow(FALSE);
 		GetDlgItem(IDC_YmaxLbl)->EnableWindow(FALSE);
 		GetDlgItem(IDC_YmaxFld)->EnableWindow(FALSE);
		break;
	}
}

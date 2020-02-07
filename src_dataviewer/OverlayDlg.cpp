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
// OverlayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OverlayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COverlayDlg dialog

COverlayDlg::COverlayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COverlayDlg::IDD, pParent)
	,m_FileType(0)
	,m_FstInputFileName(_T(""))
	,m_OutputFileName(_T(""))
	,m_SndInputFileName(_T(""))
	,m_Alg(_T(""))
	,m_Value(0)
{
}

void COverlayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_ThreeDRRdo, m_FileType);
	DDX_Text(pDX, IDC_FstInputFld, m_FstInputFileName);
	DDX_Text(pDX, IDC_OutputFld, m_OutputFileName);
	DDX_Text(pDX, IDC_SndInputFld, m_SndInputFileName);
	DDX_CBString(pDX, IDC_AlgCmb, m_Alg);
	DDX_Text(pDX, IDC_ValueFld, m_Value);
	DDX_Control(pDX, IDC_AlgCmb, m_AlgList);
}


BEGIN_MESSAGE_MAP(COverlayDlg, CDialog)
	//{{AFX_MSG_MAP(COverlayDlg)
	ON_BN_CLICKED(IDC_FstInputBtn, OnFstInputBtn)
	ON_BN_CLICKED(IDC_OutputBtn, OnOutputBtn)
	ON_BN_CLICKED(IDC_SndInputBtn, OnSndInputBtn)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_AlgCmb, OnCbnSelchangeAlgcmb)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COverlayDlg message handlers

BOOL COverlayDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_Alg == "Select" || m_Alg == "Mask"){
		GetDlgItem(IDC_ValueLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_ValueFld)->EnableWindow(TRUE);
	}
	else{
		GetDlgItem(IDC_ValueLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_ValueFld)->EnableWindow(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COverlayDlg::OnFstInputBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_FileType==0){
		static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 		CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
		m_ldFile.m_ofn.lpstrTitle="Open";	
		if(m_ldFile.DoModal() == IDOK){
			m_FstInputFileName = m_ldFile.GetPathName();
			UpdateData(FALSE);
		}
	}
	else if(m_FileType==1){
		static char BASED_CODE szFilter[] = "3dm Files (*.3dm)|*.3dm|All Files (*.*)|*.*||";
 		CFileDialog m_ldFile(TRUE, NULL, "*.3dm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
		m_ldFile.m_ofn.lpstrTitle="Open";	
		if(m_ldFile.DoModal() == IDOK){
			m_FstInputFileName = m_ldFile.GetPathName();
			UpdateData(FALSE);
		}
	}
}

void COverlayDlg::OnSndInputBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_SndInputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void COverlayDlg::OnOutputBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_FileType==0){
		static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 		CFileDialog m_ldFile(FALSE, "3dr", ".3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
		m_ldFile.m_ofn.lpstrTitle="Save As";	
		if(m_ldFile.DoModal() == IDOK){
			m_OutputFileName = m_ldFile.GetPathName();
			UpdateData(FALSE);
		}
	}
	else if(m_FileType==1){
		static char BASED_CODE szFilter[] = "3dm Files (*.3dm)|*.3dm|All Files (*.*)|*.*||";
 		CFileDialog m_ldFile(FALSE, "3dm", ".3dm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
		m_ldFile.m_ofn.lpstrTitle="Save As";	
		if(m_ldFile.DoModal() == IDOK){
			m_OutputFileName = m_ldFile.GetPathName();
			UpdateData(FALSE);
		}
	}
}

void COverlayDlg::OnOK(){
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if(m_FileType==1 && m_Alg != "Mask"){
		MessageBox("This operation cannot be performed on 3dm file.");
		return;
	}
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	if(m_FstInputFileName.IsEmpty()){
		MessageBox("No first input file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if(m_SndInputFileName.IsEmpty()){
		MessageBox("No second input file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if(m_OutputFileName.IsEmpty()){
		MessageBox("No output file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}

	CThreeDR* theSecondInput = new CThreeDR;
	int readFlag = theSecondInput->readin(m_SndInputFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the second input file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	int nrow = theSecondInput->getNumberOfRows();
	int ncol = theSecondInput->getNumberOfColumns();
	float nodata2 = theSecondInput->getNullDataValue();

	if(m_FileType==0){
		CThreeDR* theFirstInput = new CThreeDR;
		readFlag = theFirstInput->readin(m_FstInputFileName.GetBuffer(1), 0);
		if(readFlag != 0){
			MessageBox("Cannot open the first input file or file format has errors.");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			delete theFirstInput;
			delete theSecondInput;
			return;
		}
		if(!theFirstInput->spatialRangeMatch(theSecondInput)){
			MessageBox("Spatial ranges of the first and second files do not match.");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			delete theFirstInput;
			delete theSecondInput;
			return;
		}
		float nodata = theFirstInput->getNullDataValue();
		int flag = 0;
		float* outBuf=new float[nrow*ncol];
		int s = m_AlgList.GetCurSel();
		m_AlgList.GetLBText(s, m_Alg);
		if(m_Alg == "Max") //These functions are in overlay.h
			flag = maxOverlay(theFirstInput->getDataBuf(), theSecondInput->getDataBuf(), outBuf, nrow, ncol, nodata, nodata2);
		else if(m_Alg == "Min")
			flag = minOverlay(theFirstInput->getDataBuf(), theSecondInput->getDataBuf(), outBuf, nrow, ncol, nodata, nodata2);
		else if(m_Alg == "Supplement")
			flag = supplement(theFirstInput->getDataBuf(), theSecondInput->getDataBuf(), outBuf, nrow, ncol, nodata, nodata2);
		else if(m_Alg == "Select")
			flag = select(theFirstInput->getDataBuf(), theSecondInput->getDataBuf(), outBuf, nrow, ncol, nodata, nodata2, m_Value);
		else if(m_Alg == "Mask")
			flag = mask(theFirstInput->getDataBuf(), theSecondInput->getDataBuf(), outBuf, nrow, ncol, nodata, nodata2, m_Value);
		else if(m_Alg == "+")
			flag = plusOverlay(theFirstInput->getDataBuf(), theSecondInput->getDataBuf(), outBuf, nrow, ncol, nodata, nodata2);
		else if(m_Alg == "-")
			flag = subtractOverlay(theFirstInput->getDataBuf(), theSecondInput->getDataBuf(), outBuf, nrow, ncol, nodata, nodata2);
		else if(m_Alg == "*")
			flag = multiplyOverlay(theFirstInput->getDataBuf(), theSecondInput->getDataBuf(), outBuf, nrow, ncol, nodata, nodata2);
		else if(m_Alg == "/")
			flag = divideOverlay(theFirstInput->getDataBuf(), theSecondInput->getDataBuf(), outBuf, nrow, ncol, nodata, nodata2);

		if(flag != 0){
			MessageBox("Error in overlay operation.");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			delete theFirstInput;
			delete theSecondInput;
			delete [] outBuf;
			return;
		}
		CThreeDR* theOutput = new CThreeDR;
		theOutput->copyBasicFields(theFirstInput);
		theOutput->setDataName(extractFileName(m_OutputFileName).GetBuffer(1));
		theOutput->setDataBuf(outBuf);
		theOutput->calcStat();
		theOutput->setNumberOfColors(theFirstInput->getNumberOfColors());
		theOutput->setPalette(theFirstInput->getPalette());
		theOutput->writeout(m_OutputFileName.GetBuffer(1), 0);
		delete theFirstInput;
		delete theSecondInput;
		delete theOutput;
	}
	else if(m_FileType==1){
		CThreeDM* theFirstInput = new CThreeDM;
		readFlag = theFirstInput->readin(m_FstInputFileName.GetBuffer(1));
		if(readFlag != 0){
			MessageBox("Cannot open the first input file or file format has errors.");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			delete theFirstInput;
			delete theSecondInput;
			return;
		}
		if(!theFirstInput->spatialRangeMatch(theSecondInput)){
			MessageBox("Spatial ranges of the first and second files do not match.");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			delete theFirstInput;
			delete theSecondInput;
			return;
		}
		float* outBuf=new float[(nrow+1)*(ncol+1)];
		unsigned char* outPhotoBuf = new unsigned char[nrow*ncol];
		float nodata = theFirstInput->getNullValue();
		int flag = mask3DM(theFirstInput->getDataBuf(), theFirstInput->getPhotoBuf(), 
							theSecondInput->getDataBuf(), 
							outBuf, outPhotoBuf, nodata, nrow, ncol);
		if(flag != 0){
			MessageBox("Error in overlay operation.");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			delete theFirstInput;
			delete theSecondInput;
			delete [] outBuf;
			delete [] outPhotoBuf;
			return;
		}
		CThreeDM* theOutput = new CThreeDM;
		theOutput->setDataBuf(outBuf);
		theOutput->setPhotoBuf(outPhotoBuf);
		theOutput->copyBasicFields(theFirstInput);
		theOutput->calcStat();
		theOutput->setNullValue(nodata);
		theOutput->setOriginalNullValue(theFirstInput->getOriginalNullValue());
		theOutput->writeout(m_OutputFileName.GetBuffer(1));
		delete theFirstInput;
		delete theSecondInput;
		delete theOutput;
	}
	else{
		MessageBox("Error in the file type.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		delete theSecondInput;
		return;
	}

	GetDlgItem(IDOK)->EnableWindow(TRUE);
	CDialog::OnOK();
}

void COverlayDlg::OnCbnSelchangeAlgcmb(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int s = m_AlgList.GetCurSel();
	m_AlgList.GetLBText(s, m_Alg);
	if(m_Alg == "Select" || m_Alg == "Mask"){
		GetDlgItem(IDC_ValueLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_ValueFld)->EnableWindow(TRUE);
	}
	else{
		GetDlgItem(IDC_ValueLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_ValueFld)->EnableWindow(FALSE);
	}
}

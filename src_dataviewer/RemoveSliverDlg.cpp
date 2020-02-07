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
// RemoveSliverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "RemoveSliverDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemoveSliverDlg dialog


CRemoveSliverDlg::CRemoveSliverDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoveSliverDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRemoveSliverDlg)
	m_CuttingArea = 0.0f;
	m_InputFileName = _T("");
	m_Nodata = -9999;
	m_OutputFileName = _T("");
	m_EightDir = 0;
	m_SpecificValue = 0.0f;
	m_PreserveIsolated = FALSE;
	m_RemoveSpcValue = FALSE;
	//}}AFX_DATA_INIT
}


void CRemoveSliverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemoveSliverDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_progressBar);
	DDX_Text(pDX, IDC_REMSLVCUTAREAFLD, m_CuttingArea);
	DDV_MinMaxFloat(pDX, m_CuttingArea, 0.f, 1.e+011f);
	DDX_Text(pDX, IDC_REMSLVINPUTFILEFLD, m_InputFileName);
	DDX_Text(pDX, IDC_REMSLVNODATAFLD, m_Nodata);
	DDX_Text(pDX, IDC_REMSLVOUTFILEFLD, m_OutputFileName);
	DDX_Radio(pDX, IDC_REMSLV4CONNECTRDO, m_EightDir);
	DDX_Text(pDX, IDC_SpcValueFld, m_SpecificValue);
	DDX_Check(pDX, IDC_PreserveIsolatedChk, m_PreserveIsolated);
	DDX_Check(pDX, IDC_RemoveSpcValueChk, m_RemoveSpcValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRemoveSliverDlg, CDialog)
	//{{AFX_MSG_MAP(CRemoveSliverDlg)
	ON_BN_CLICKED(IDC_REMSLVINFILEBTN, OnRemslvinfilebtn)
	ON_BN_CLICKED(IDC_REMSLVOUTFILEBTN, OnRemslvoutfilebtn)
	ON_BN_CLICKED(IDREMSLVOK, OnRemSlvOK)
	ON_BN_CLICKED(IDREMSLVCANCEL, OnRemSlvCancel)
	ON_BN_CLICKED(IDC_RemoveSpcValueChk, OnRemoveSpcValueChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoveSliverDlg message handlers


BOOL CRemoveSliverDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_SpcValueLbl)->EnableWindow(FALSE);
	GetDlgItem(IDC_SpcValueFld)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRemoveSliverDlg::OnRemslvinfilebtn(){
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_InputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CRemoveSliverDlg::OnRemslvoutfilebtn(){
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(FALSE, "3dr", ".3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK){
		m_OutputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CRemoveSliverDlg::OnRemoveSpcValueChk(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_RemoveSpcValue){
		GetDlgItem(IDC_SpcValueLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_SpcValueFld)->EnableWindow(TRUE);
	}
	else{
		GetDlgItem(IDC_SpcValueLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_SpcValueFld)->EnableWindow(FALSE);
	}
}

void CRemoveSliverDlg::OnRemSlvOK(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDREMSLVOK)->EnableWindow(FALSE);
	m_progressBar.SetRange(1, 100);
	m_progressBar.SetPos(5);

	if(m_InputFileName.IsEmpty()){
		MessageBox("No input file.");
		GetDlgItem(IDREMSLVOK)->EnableWindow(TRUE);
		m_progressBar.SetPos(0);
		return;
	}
	if(m_OutputFileName.IsEmpty()){
		MessageBox("No output file.");
		GetDlgItem(IDREMSLVOK)->EnableWindow(TRUE);
		m_progressBar.SetPos(0);
		return;
	}

	CThreeDR* theInput = new CThreeDR;
	int readFlag = theInput->readin(m_InputFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the input file or file format has errors.");
		GetDlgItem(IDREMSLVOK)->EnableWindow(TRUE);
		m_progressBar.SetPos(0);
		return;
	}

	int nrow = theInput->getNumberOfRows();
	int ncol = theInput->getNumberOfColumns();
	if(m_CuttingArea < theInput->getCellSize()*theInput->getCellSize()){
		MessageBox("The cutting area is smaller than a pixel.");
		GetDlgItem(IDREMSLVOK)->EnableWindow(TRUE);
		m_progressBar.SetPos(0);
		return;
	}
	bool eightDir = false;
	if(m_EightDir == 1)
		eightDir = true;

	int numOfSlivers = 0;
	int threshNum = (int)(m_CuttingArea/(theInput->getCellSize()*theInput->getCellSize()));

	float* workingBuf = theInput->getDataBuf();
	
	do{
		//looking for slivers
		//Building the ID buffer
		numOfSlivers = 0;
		unsigned char* idBuf = new unsigned char[nrow*ncol];
		if(idBuf == NULL){
			MessageBox("No sufficient memory to create ID buffer.");
			GetDlgItem(IDREMSLVOK)->EnableWindow(TRUE);
			m_progressBar.SetPos(0);
			return;
		}

		for(int i=0; i<nrow*ncol; i++){
			if(fabs(workingBuf[i]-m_Nodata)<VERYSMALL)  //if nodata
				idBuf[i] = 4;	//0: not visited
			else{				//1: visited, but not decided if is sliver
				idBuf[i] = 0;	//2: sliver
								//3: not sliver
								//4: nodata
				if(m_RemoveSpcValue && fabs(workingBuf[i]-m_SpecificValue)>VERYSMALL)
					idBuf[i] = 3;
			}
		}
		for(i=0; i<nrow*ncol; i++){
			if(i>0 && i%(200*ncol) == 0){
				m_progressBar.SetPos(i*100/(nrow*ncol));
			}
			if(idBuf[i] == 0){
				Patch<float>* thePatch = new Patch<float>(i, workingBuf, idBuf, nrow, ncol, eightDir);

				PatchPixel* tempPixel = thePatch->first;
				if(thePatch->numPixels<=threshNum){//sliver
					numOfSlivers++;
					while(tempPixel!=NULL){
						idBuf[tempPixel->pos] = 2;
						tempPixel = tempPixel->nextPixel;
					}
				}
				else{								//not a sliver
					while(tempPixel!=NULL){
						idBuf[tempPixel->pos] = 3;
						tempPixel = tempPixel->nextPixel;
					}
				}
				delete thePatch;
			}
		}

		if(numOfSlivers == 0) break;

		//removing slivers
		numOfSlivers = 0;
		unsigned char* idBuf2 = new unsigned char[nrow*ncol];
		if(idBuf2 == NULL){
			MessageBox("No sufficient memory to create ID buffer.");
			GetDlgItem(IDREMSLVOK)->EnableWindow(TRUE);
			m_progressBar.SetPos(0);
			return;
		}
		for(i=0; i<nrow*ncol; i++)
			idBuf2[i] = 0;

		for(i=0; i<nrow*ncol; i++){
			if(i%(200*ncol) == 0)
				m_progressBar.SetPos(i*100/(nrow*ncol));

			if(idBuf[i] == 2){ //a sliver
				Patch<unsigned char>* thePatch = new Patch<unsigned char>(i, idBuf, idBuf2, nrow, ncol, eightDir);
				if(thePatch->doRemoval(workingBuf, eightDir, m_PreserveIsolated, m_Nodata)>0)
					numOfSlivers++;
				delete thePatch;
			}
		}
		m_progressBar.SetPos(100);

		//clean up
		delete [] idBuf2;
		delete [] idBuf;
	}while(numOfSlivers>0);


	//writing out
	theInput->calcStat();
	int writeFlag = theInput->writeout(m_OutputFileName.GetBuffer(1), 0);
	if(writeFlag != 0){
		MessageBox("Error in writing the result.");
		GetDlgItem(IDREMSLVOK)->EnableWindow(TRUE);
		m_progressBar.SetPos(0);
		return;
	}

	delete theInput;

	GetDlgItem(IDREMSLVOK)->EnableWindow(TRUE);
	CDialog::OnOK();
}

void CRemoveSliverDlg::OnRemSlvCancel() {
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

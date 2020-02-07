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
// TerrainPartitionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "TerrainPartitionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTerrainPartitionDlg dialog


CTerrainPartitionDlg::CTerrainPartitionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTerrainPartitionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTerrainPartitionDlg)
	m_FlowDirFileName = _T("");
	m_OutputFileName = _T("");
	m_StreamlineFileName = _T("");
	m_AngleDiff = 0.0f;
	m_CombinePartitions = FALSE;
	//}}AFX_DATA_INIT
}


void CTerrainPartitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTerrainPartitionDlg)
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
	DDX_Text(pDX, IDC_FlowDirFileFld, m_FlowDirFileName);
	DDX_Text(pDX, IDC_OutputFld, m_OutputFileName);
	DDX_Text(pDX, IDC_StreamlineFileFld, m_StreamlineFileName);
	DDX_Text(pDX, IDC_AngleFld, m_AngleDiff);
	DDX_Check(pDX, IDC_CombinePartitionChk, m_CombinePartitions);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTerrainPartitionDlg, CDialog)
	//{{AFX_MSG_MAP(CTerrainPartitionDlg)
	ON_BN_CLICKED(IDC_StreamlineFileBtn, OnStreamlineFileBtn)
	ON_BN_CLICKED(IDC_FlowDirFileBtn, OnFlowDirFileBtn)
	ON_BN_CLICKED(IDC_OutputBtn, OnOutputBtn)
	ON_BN_CLICKED(IDC_CombinePartitionChk, OnCombinePartitionChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerrainPartitionDlg message handlers
BOOL CTerrainPartitionDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	OnCombinePartitionChk();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTerrainPartitionDlg::OnStreamlineFileBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_StreamlineFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void CTerrainPartitionDlg::OnFlowDirFileBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_FlowDirFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void CTerrainPartitionDlg::OnOutputBtn(){
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

void CTerrainPartitionDlg::OnOK(){
	// TODO: Add extra validation here
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

	if(m_StreamlineFileName.IsEmpty()){
		MessageBox("No streamline file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_FlowDirFileName.IsEmpty()){
		MessageBox("No flow direction file.");
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

	CThreeDR* theFlowDir = new CThreeDR;
	int readFlag = theFlowDir->readin(m_FlowDirFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the flow direction file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	CThreeDR* theStreamline = new CThreeDR;
	readFlag = theStreamline->readin(m_StreamlineFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the streamline file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	
	int nrow = theStreamline->getNumberOfRows();
	int ncol = theStreamline->getNumberOfColumns();	
	float nodata = theStreamline->getNullDataValue();
	float* flowDir = theFlowDir->getDataBuf();
	float* streamline = theStreamline->getDataBuf();
	float* outBuf = new float[nrow*ncol];

	m_ProgressBar.SetPos(30);
	float theAngle = 0;
	if(m_CombinePartitions)
		theAngle = m_AngleDiff;
	int opFlag = terrainPartition(streamline, flowDir, outBuf, nrow, ncol, theAngle, nodata);
	
	if(opFlag != 0){
		MessageBox("Error in terrainPartition function.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	m_ProgressBar.SetPos(100);
	
	CThreeDR* theOutput = new CThreeDR;
	theOutput->setDataBuf(outBuf);
	theOutput->setDataName(extractFileName(m_OutputFileName).GetBuffer(1));
	theOutput->copyBasicFields(theStreamline);
	theOutput->calcStat();
	int writeFlag = theOutput->writeout(m_OutputFileName.GetBuffer(1), 0);

	//clean up
	delete theFlowDir;
	delete theStreamline;
	delete theOutput;

	GetDlgItem(IDOK)->EnableWindow(TRUE);
	CDialog::OnOK();
}

void CTerrainPartitionDlg::OnCombinePartitionChk(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_CombinePartitions)
		GetDlgItem(IDC_AngleFld)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_AngleFld)->EnableWindow(FALSE);
}

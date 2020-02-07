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
// HeadwaterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "HeadwaterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeadwaterDlg dialog


CHeadwaterDlg::CHeadwaterDlg(CWnd* pParent /*=NULL*/)
: CDialog(CHeadwaterDlg::IDD, pParent)
	, m_FlowDirFileName(_T(""))
	, m_StreamFileName(_T(""))
	, m_ProfileFileName(_T(""))
	, m_PlanformFileName(_T(""))
	, m_OutputFileName(_T(""))
	, m_StreamOrder(2)
	, m_NodeType(FALSE)
	, m_ProfileWgt(1)
	, m_PlanformWgt(0)
	, m_UpSearchRange(5)
	, m_DownSearchRange(20)
	, m_UDAFileName(_T(""))
{
}


void CHeadwaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FlowDirFileFld, m_FlowDirFileName);
	DDX_Text(pDX, IDC_StreamFileFld, m_StreamFileName);
	DDX_Text(pDX, IDC_ProfileFileFld, m_ProfileFileName);
	DDX_Text(pDX, IDC_PlanformFileFld, m_PlanformFileName);
	DDX_Text(pDX, IDC_OutputFileNameFld, m_OutputFileName);
	DDX_Text(pDX, IDC_StreamOrderFld, m_StreamOrder);
	DDX_Radio(pDX, IDC_JumpNodesRdo, m_NodeType);
	DDX_Text(pDX, IDC_ProfileWgtFld, m_ProfileWgt);
	DDX_Text(pDX, IDC_PlanforWgtFld, m_PlanformWgt);
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
	DDX_Text(pDX, IDC_UpSearchRangeFld, m_UpSearchRange);
	DDX_Text(pDX, IDC_DownSearchRangeFld, m_DownSearchRange);
	DDX_Text(pDX, IDC_FlowAccFileFld, m_UDAFileName);
}


BEGIN_MESSAGE_MAP(CHeadwaterDlg, CDialog)
	//{{AFX_MSG_MAP(CHeadwaterDlg)
	ON_BN_CLICKED(IDC_FlowDirBtn, OnFlowDirBtn)
	ON_BN_CLICKED(IDC_StreamFileBtn, OnStreamFileBtn)
	ON_BN_CLICKED(IDC_ProfileBtn, OnProfileBtn)
	ON_BN_CLICKED(IDC_PlanformBtn, OnPlanformBtn)
	ON_BN_CLICKED(IDC_OutputFileNameBtn, OnOutputFileNameBtn)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_FlowAccFileBtn, OnBnClickedFlowaccfilebtn)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeadwaterDlg message handlers
BOOL CHeadwaterDlg::OnInitDialog(){
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//m_ProgressBar.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CHeadwaterDlg::OnFlowDirBtn(){
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

void CHeadwaterDlg::OnStreamFileBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_StreamFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}		
}

void CHeadwaterDlg::OnProfileBtn(){
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_ProfileFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void CHeadwaterDlg::OnPlanformBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_PlanformFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void CHeadwaterDlg::OnBnClickedFlowaccfilebtn(){
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

void CHeadwaterDlg::OnOutputFileNameBtn(){
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

void CHeadwaterDlg::OnOK(){
	// TODO: Add extra validation here
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.ShowWindow(SW_SHOW);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

	if(m_FlowDirFileName.IsEmpty()){
		MessageBox("No flow direction file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_StreamFileName.IsEmpty()){
		MessageBox("No stream file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_ProfileFileName.IsEmpty()){
		MessageBox("No profile curvature file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_PlanformFileName.IsEmpty()&& m_PlanformWgt != 0){
		MessageBox("No planform curvature file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_UDAFileName.IsEmpty()&& m_PlanformWgt != 0){
		MessageBox("No flow accumulation file.");
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
		delete theFlowDir;
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	CThreeDR* theStream = new CThreeDR;
	readFlag = theStream->readin(m_StreamFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the stream file or file format has errors.");
		delete theFlowDir;
		delete theStream;
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	CThreeDR* theProfile = new CThreeDR;
	readFlag = theProfile->readin(m_ProfileFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the profile file or file format has errors.");
		delete theFlowDir;
		delete theStream;
		delete theProfile;
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	CThreeDR* thePlanform = NULL;
	if(!m_PlanformFileName.IsEmpty()){
		thePlanform = new CThreeDR;
		readFlag = thePlanform->readin(m_PlanformFileName.GetBuffer(1), 0);
		if(readFlag != 0){
			MessageBox("Cannot open the planform file or file format has errors.");
			delete theFlowDir;
			delete theStream;
			delete theProfile;
			delete thePlanform;
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			m_ProgressBar.SetPos(0);
			return;
		}
	}
	
	CThreeDR* theUDA = NULL;
	if(!m_UDAFileName.IsEmpty()){
		theUDA = new CThreeDR;
		readFlag = theUDA->readin(m_UDAFileName.GetBuffer(1), 0);
		if(readFlag != 0){
			MessageBox("Cannot open the flow accumulation file or file format has errors.");
			delete theFlowDir;
			delete theStream;
			delete theProfile;
			delete thePlanform;
			delete theUDA;
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			m_ProgressBar.SetPos(0);
			return;
		}
	}

	int nrow = theFlowDir->getNumberOfRows();
	int ncol = theFlowDir->getNumberOfColumns();	
	float nodata = theFlowDir->getNullDataValue();
	float* flowDir = theFlowDir->getDataBuf();
	float* stream = theStream->getDataBuf();
	float* profile = theProfile->getDataBuf();
	float* planform = NULL;
	if(thePlanform!=NULL)
		planform = thePlanform->getDataBuf();
	float* flowAcc = theUDA->getDataBuf();
	float* outBuf = new float[nrow*ncol];

	int opFlag = headwater(flowDir, stream, planform, profile, flowAcc, outBuf,
							m_StreamOrder, m_NodeType, m_PlanformWgt, m_ProfileWgt, m_UpSearchRange, m_DownSearchRange,
							nrow, ncol, nodata);
	
	if(opFlag != 0){
		MessageBox("Error in headwater function.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	m_ProgressBar.SetPos(100);
	
	CThreeDR* theOutput = new CThreeDR;
	theOutput->setDataBuf(outBuf);
	theOutput->setDataName(extractFileName(m_OutputFileName).GetBuffer(1));
	theOutput->copyBasicFields(theFlowDir);
	theOutput->calcStat();
	int writeFlag = theOutput->writeout(m_OutputFileName.GetBuffer(1), 0);

	//clean up
	delete theFlowDir;
	delete theStream;
	delete theProfile;
	if(thePlanform != NULL)
		delete thePlanform;
	delete theOutput;

	GetDlgItem(IDOK)->EnableWindow(TRUE);
	
	CDialog::OnOK();
}

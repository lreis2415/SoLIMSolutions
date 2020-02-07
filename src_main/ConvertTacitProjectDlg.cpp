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
// ConvertTacitProjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "ConvertTacitProjectDlg.h"
#include "ImportOldPrj.h"
#include ".\converttacitprojectdlg.h"


// CConvertTacitProjectDlg dialog

IMPLEMENT_DYNAMIC(CConvertTacitProjectDlg, CDialog)
CConvertTacitProjectDlg::CConvertTacitProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConvertTacitProjectDlg::IDD, pParent)
	, m_OldPrjDir(_T(""))
	, m_LabelFileName(_T(""))
	, m_PntFileName(_T(""))
	, m_CellName(_T(""))
	, m_NewPrjDir(_T(""))
	, m_NewPrjName(_T(""))
{
	
}

CConvertTacitProjectDlg::~CConvertTacitProjectDlg()
{
}

void CConvertTacitProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LabelFileNameFld, m_LabelFileName);
	DDX_Text(pDX, IDC_PntFileNameFld, m_PntFileName);
	DDX_Text(pDX, IDC_CellNameEdit, m_CellName);
	DDX_Text(pDX, IDC_OldPrjDirFld, m_OldPrjDir);
	DDX_Text(pDX, IDC_NewPrjDirFld, m_NewPrjDir);
	DDX_Text(pDX, IDC_NewPrjNameFld, m_NewPrjName);
}


BEGIN_MESSAGE_MAP(CConvertTacitProjectDlg, CDialog)
	ON_BN_CLICKED(IDC_LabelFileBtn, OnBnClickedLabelfilebtn)
	ON_BN_CLICKED(IDC_PntFileBtn, OnBnClickedPntfilebtn)
	ON_BN_CLICKED(IDC_CellNameBtn, OnBnClickedCellnamebtn)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_OldDirBtn, OnBnClickedOlddirbtn)
	ON_BN_CLICKED(IDC_NewDirBtn, OnBnClickedNewdirbtn)
	//ON_BN_CLICKED(IDC_RADIO_RBR, OnBnClickedRadioRbr)
	//ON_BN_CLICKED(IDC_RADIO_CBR, OnBnClickedRadioCbr)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	
END_MESSAGE_MAP()



BOOL CConvertTacitProjectDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
 //  ((CButton*)GetDlgItem(IDC_RADIO_RBR))->SetCheck(1);

	//m_PrjType = 1;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


// CConvertTacitProjectDlg message handlers

void CConvertTacitProjectDlg::OnBnClickedLabelfilebtn()
{
	UpdateData(TRUE);
	static char BASED_CODE szFilter[] = "Label Files (*.lab)|*.lab|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.lab", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if(m_OldPrjDir != "")
	{
	    CString str = m_OldPrjDir + "\\knowledge"; 	
		char str1[FN_LEN];   	  
		strcpy(str1,str);   
		str1[strlen(str1)+1]=0;   
		m_ldFile.m_ofn.lpstrInitialDir  =   (LPCSTR)str1;  


	}

		//m_ldFile.m_ofn.lpstrInitialDir = "c:\\test";
	   //m_ldFile.m_ofn.lpstrInitialDir = m_OldPrjDir + "\\knowledge";

	//CString  k= m_ldFile.m_ofn.lpstrInitialDir;


	if(m_ldFile.DoModal() == IDOK)
	{
		m_LabelFileName = m_ldFile.GetFileName();
		//m_PntFileName = m_LabelFileName.Left(m_LabelFileName.GetLength() - 3) + "pnt";
		//m_PathName = m_ldFile.GetPathName();
		//m_PrjDirName = m_PathName.Left(m_PathName.GetLength() - LENOFKNOWLEDGE - 2 - m_LabelFileName.GetLength());
		UpdateData(FALSE);
	}	

}



void CConvertTacitProjectDlg::OnBnClickedPntfilebtn()
{
	UpdateData(TRUE);
	static char BASED_CODE szFilter[] = "Point Files (*.pnt)|*.pnt|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.pnt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	if(m_OldPrjDir != "")
	{
		CString str = m_OldPrjDir + "\\knowledge"; 	
		char str1[FN_LEN];   	  
		strcpy(str1,str);   
		str1[strlen(str1)+1]=0;   
		m_ldFile.m_ofn.lpstrInitialDir  =   (LPCSTR)str1;  


	}

	if(m_ldFile.DoModal() == IDOK)
	{
		m_PntFileName = m_ldFile.GetFileName();
		//m_LabelFileName = m_PntFileName.Left(m_PntFileName.GetLength() - 3) + "lab";
		//m_PathName = m_ldFile.GetPathName();
		//m_PrjDirName = m_PathName.Left(m_PathName.GetLength() - LENOFKNOWLEDGE - 2 - m_PntFileName.GetLength());
		UpdateData(FALSE);
	}	

}

void CConvertTacitProjectDlg::OnBnClickedCellnamebtn()
{
	UpdateData(TRUE);
	static char BASED_CODE szFilter[] = "CellName Files (*.txt)|*.txt|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if(m_OldPrjDir != "")
	{
		CString str = m_OldPrjDir + "\\header"; 	
		char str1[FN_LEN];   	  
		strcpy(str1,str);   
		str1[strlen(str1)+1]=0;   
		m_ldFile.m_ofn.lpstrInitialDir  =   (LPCSTR)str1;  
	}
	if(m_ldFile.DoModal() == IDOK)
	{
		m_CellName = m_ldFile.GetFileName();
		UpdateData(FALSE);
	}
}

void CConvertTacitProjectDlg::OnBnClickedOk()
{
 	UpdateData(TRUE);


	if (m_LabelFileName.IsEmpty())
		MessageBox("You must specify the Label File!");
	else if (m_PntFileName.IsEmpty())
		MessageBox("You must specify the Point File!");
	else if (m_CellName.IsEmpty())
		MessageBox("You must specify the CellName File!");
	else if (m_NewPrjName.IsEmpty())
		MessageBox("You must specify the New Project Name!");
	else if (m_OldPrjDir.IsEmpty())
		MessageBox("You must specify the Old Project Directory!");
	else if (m_NewPrjDir.IsEmpty())
		MessageBox("You must specify the New Project Directory!");
	else
	{	
		ImportOldPrj *theChange = new ImportOldPrj();
	
		 char projDir[STRLEN];
	     char point_file[FN_LEN];
	     char label_file[FN_LEN];
	     char cellname_file[FN_LEN];
	     char output_path[FN_LEN];
	     char output_name[FN_LEN];

	    strcpy(projDir, m_OldPrjDir.GetBuffer());
	    strcpy(point_file, m_PntFileName.GetBuffer());
	    strcpy(label_file,m_LabelFileName.GetBuffer());
        strcpy(cellname_file,m_CellName.GetBuffer());


	  m_OldPrjDir.ReleaseBuffer();
	  m_PntFileName.ReleaseBuffer();
	  m_LabelFileName.ReleaseBuffer();
	   m_CellName.ReleaseBuffer();

	bool success = theChange->ConvertToNewProject(projDir,point_file,label_file,cellname_file,m_NewPrjDir, m_NewPrjName);
    

	if(theChange!=NULL)
	{
		delete theChange;
		theChange = NULL;
	}
	
	if(success == true)
	{
		MessageBox("Convert project successfully!");
	}

	   
	     CDialog::OnOK();
	}

}



void CConvertTacitProjectDlg::OnBnClickedOlddirbtn()
{
			// TODO: Add your control notification handler code here
	UpdateData(true);
	m_OldPrjDir = m_getDirDlg1.GetDirectory(this, "", "Choose old project directory :");
	UpdateData(false);

	// TODO: Add your control notification handler code here
}

void CConvertTacitProjectDlg::OnBnClickedNewdirbtn()
{
		// TODO: Add your control notification handler code here
	UpdateData(true);
	m_NewPrjDir = m_getDirDlg2.GetDirectory(this, m_OldPrjDir, "Choose a directory to hold the new project:");
	UpdateData(false);

}


	// TODO: Add your control notification handler code her




void CConvertTacitProjectDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}




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
// PropertyMapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "PropertyMapDlg.h"
#include ".\propertymapdlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyMapDlg dialog


CPropertyMapDlg::CPropertyMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyMapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyMapDlg)
	m_InputFileName = _T("");
	m_OutputFileName = _T("");
	m_WorkingDir = _T("");
	m_Mask = FALSE;
	m_MaskFileName = _T("");
	m_MaskValue = 0.0f;
    //}}AFX_DATA_INIT
	nodata = -9999;
}


void CPropertyMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyMapDlg)
	DDX_Control(pDX, IDC_PropertyMapProBar, m_ProgressBar);
	DDX_Text(pDX, IDC_PropertyMapInFileFld, m_InputFileName);
	DDX_Text(pDX, IDC_PropertyMapOutFileFld, m_OutputFileName);
	DDX_Text(pDX, IDC_WorkingDirFld, m_WorkingDir);
	DDX_Check(pDX, IDC_MaskChk, m_Mask);
	DDX_Text(pDX, IDC_MaskFileFld, m_MaskFileName);
	DDX_Text(pDX, IDC_MaskValueFld, m_MaskValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyMapDlg, CDialog)
	//{{AFX_MSG_MAP(CPropertyMapDlg)
	ON_BN_CLICKED(IDC_PropertyMapInFileBtn, OnPropertyMapInFileBtn)
	ON_BN_CLICKED(IDC_PropertyMapOutFileBtn, OnPropertyMapOutFileBtn)
	ON_BN_CLICKED(IDPropertyMapCancel, OnPropertyMapCancel)
	ON_BN_CLICKED(IDPropertyMapOK, OnPropertyMapOK)
	ON_BN_CLICKED(IDC_WorkingDirBtn, OnWorkingDirBtn)
	ON_EN_KILLFOCUS(IDC_WorkingDirFld, OnKillfocusWorkingDirFld)
	ON_BN_CLICKED(IDC_MaskFileBtn, OnMaskFileBtn)
	ON_BN_CLICKED(IDC_MaskChk, OnMaskChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyMapDlg message handlers

void CPropertyMapDlg::OnKillfocusWorkingDirFld(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CPropertyMapDlg::OnWorkingDirBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	CString dir;	
	
	dir = m_getDirDlg.GetDirectory(this, "", "Please specify the result directory");	
	  if(dir == "")
		 return;
	  else
	    m_WorkingDir = dir;
	    UpdateData(FALSE);

}

void CPropertyMapDlg::OnPropertyMapInFileBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "Lookup Table Files (*.lkt)|*.lkt|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.lkt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_InputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CPropertyMapDlg::OnPropertyMapOutFileBtn(){
	// TODO: Add your control notification handler code here

	//The input is a lookup table containing two columns, 
	//one is the soil name (also the file name of fuzzy 
	//membership map), the other is property of of this soil.
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(FALSE, "3dr", ".3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK){
		m_OutputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CPropertyMapDlg::OnPropertyMapCancel(){
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CPropertyMapDlg::OnPropertyMapOK() {
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDPropertyMapOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

	if(m_InputFileName.IsEmpty()){
		MessageBox("No input file.");
		GetDlgItem(IDPropertyMapOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_OutputFileName.IsEmpty()){
		MessageBox("No output file.");
		GetDlgItem(IDPropertyMapOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	CThreeDR* PropertyMap = new CThreeDR;
	char FileName[STRLEN];
	char FuzzyMapFileName[STRLEN];
	float SoilProperty = 0;
	CThreeDR* FuzzyMap = new CThreeDR;
	int i = 0;

	//read first fuzzy map (for getting file header)
	ifstream fin;
	fin.open(m_InputFileName);
	if(fin.fail()){
		MessageBox("Cannot open lookup table file.");
		GetDlgItem(IDPropertyMapOK)->EnableWindow(TRUE);
		return;
	}
	strcpy(FuzzyMapFileName, m_WorkingDir.GetBuffer(1));
	fin>>FileName>>SoilProperty;
	strcat(FuzzyMapFileName,"\\");
	strcat(FuzzyMapFileName, FileName);
	strcat(FuzzyMapFileName, ".3dr");
	int readflag = FuzzyMap->readin(FuzzyMapFileName, 0);
	if(readflag != 0){
		CString msg = "Cannot open ";
		msg += FuzzyMapFileName;
		msg += " or file format has errors.";
		MessageBox(msg);
		GetDlgItem(IDPropertyMapOK)->EnableWindow(TRUE);
		return;
	}

     
	//extract property map name(without .3dr from the full path)
	char *MapName;
	MapName = extract_filename(m_OutputFileName.GetBuffer(1));
    int length = strlen(MapName);
	for (i=0; i<length;i++)
	{
		if(MapName[i] == '.')
		{
			MapName[i] = '\0';
			break;
		}
	} 

    PropertyMap->setDataName(MapName);


	PropertyMap->copyBasicFields(FuzzyMap);

	PropertyMap->setNullDataValue(nodata);

	int nrow = PropertyMap->getNumberOfRows();
	int ncol = PropertyMap->getNumberOfColumns();
	PropertyMap->setDataBuf(new float[nrow*ncol]);
	float* weightBuf = new float[nrow*ncol];
	bool *idBuf = new bool[nrow*ncol];
	for(i=0; i<nrow*ncol; i++)
	{
		idBuf[i]= true;
	}
	if(m_Mask == true)
	{
	   CheckMask(idBuf, nrow, ncol, FuzzyMap);
	}
    

	for(i=0; i<nrow*ncol; i++){
		PropertyMap->getDataBuf()[i] = FuzzyMap->getDataBuf()[i] * SoilProperty;
		weightBuf[i] = FuzzyMap->getDataBuf()[i];
	}

	while(!fin.eof()){
		m_ProgressBar.OffsetPos(3);

		strcpy(FuzzyMapFileName, m_WorkingDir.GetBuffer(1));
		strcat(FuzzyMapFileName,"\\");
		fin>>FileName>>SoilProperty;
		trim(FileName, FileName);
		if(strlen(FileName)==0) continue;
		strcat(FuzzyMapFileName, FileName);
		strcat(FuzzyMapFileName, ".3dr");
		FuzzyMap->flush();
		readflag = FuzzyMap->readin(FuzzyMapFileName, 0);
		if(readflag != 0){
			CString msg = "Cannot open ";
			msg += FuzzyMapFileName;
			msg += " or file format has errors.";
			MessageBox(msg);
			continue;
		}

		for(i=0; i<nrow*ncol; i++){
			PropertyMap->getDataBuf()[i] += FuzzyMap->getDataBuf()[i] * SoilProperty;
			weightBuf[i] += FuzzyMap->getDataBuf()[i];
		}
	}
	fin.close();
	m_ProgressBar.SetPos(100);

	for(i=0; i<nrow*ncol; i++)
	{
		if(weightBuf[i] <= VERYSMALL)
		  PropertyMap->getDataBuf()[i] = nodata;

		
		else if(m_Mask == true)
		{ 
			if(idBuf[i] == false)
			   PropertyMap->getDataBuf()[i] = nodata;
			else 
			  PropertyMap->getDataBuf()[i] /= weightBuf[i];
		}
		else
		  PropertyMap->getDataBuf()[i] /= weightBuf[i];		
	}
	
	PropertyMap->calcStat();

	int writeFlag = PropertyMap->writeout(m_OutputFileName.GetBuffer(1), 0);

	//clean up
	delete PropertyMap;
	delete FuzzyMap;
	delete []idBuf;

	GetDlgItem(IDPropertyMapOK)->EnableWindow(TRUE);
	CDialog::OnOK();
}


void CPropertyMapDlg::OnMaskChk(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_Mask){
		GetDlgItem(IDC_MaskFileFld)->EnableWindow(TRUE);
		GetDlgItem(IDC_MaskFileBtn)->EnableWindow(TRUE);
		GetDlgItem(IDC_MaskValueLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_MaskValueFld)->EnableWindow(TRUE);
	}
	else{
		GetDlgItem(IDC_MaskFileFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_MaskFileBtn)->EnableWindow(FALSE);
		GetDlgItem(IDC_MaskValueLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_MaskValueFld)->EnableWindow(FALSE);
	}
}

void CPropertyMapDlg::OnMaskFileBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_MaskFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CPropertyMapDlg::CheckMask(bool* idBuf, int nrow, int ncol,CThreeDR *theData){ //Check if mask should be used
	//int nrow = theEnvDatabase->theSetting->nrow;
	//int ncol = theEnvDatabase->theSetting->ncol;
	int numPixels = nrow*ncol;
	if(m_MaskFileName.IsEmpty()){
		//MessageBox("No mask file specified.");
		m_Mask = false;
	}
	else{
		CThreeDR* maskFile = new CThreeDR;
		int readflag = maskFile->readin(m_MaskFileName.GetBuffer(100), 0);
		if(readflag > 0){
			MessageBox("Error in reading mask file. No mask used.");
			m_Mask = false;
		}
		else{
			if(!maskFile->spatialRangeMatch(theData)){
				MessageBox("The mask file and the first layer do not spatially matched . No mask used.");
				m_Mask = false;
			}
			else{
				for(int i=0; i<numPixels; i++)
					if(fabs(maskFile->getDataBuf()[i]-m_MaskValue)<VERYSMALL)
						idBuf[i] = false;
			}
		}
		delete maskFile;
	}
}



BOOL CPropertyMapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

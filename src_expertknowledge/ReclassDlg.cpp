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
// ReclassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "ReclassDlg.h"
#include "UncertainColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReclassDlg dialog


CReclassDlg::CReclassDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReclassDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReclassDlg)
	m_InputFileName = _T("");
	m_LookTBName = _T("");
	m_OutputFileName = _T("");
	//}}AFX_DATA_INIT
}


void CReclassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReclassDlg)
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
	DDX_Text(pDX, IDC_InputFileNameFld, m_InputFileName);
	DDX_Text(pDX, IDC_LookTBFileFld, m_LookTBName);
	DDX_Text(pDX, IDC_OutFileNameFld, m_OutputFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReclassDlg, CDialog)
	//{{AFX_MSG_MAP(CReclassDlg)
	ON_BN_CLICKED(IDC_LookTBFileBtn, OnLookTBFileBtn)
	ON_BN_CLICKED(IDC_InputFileNameBtn, OnInputFileNameBtn)
	ON_BN_CLICKED(IDC_OutFileNameBtn, OnOutFileNameBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReclassDlg message handlers

BOOL CReclassDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	clearFields();
	//m_ProgressBar.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReclassDlg::OnInputFileNameBtn(){
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

void CReclassDlg::OnLookTBFileBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "Lookup Table Files (*.lkt)|*.lkt|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.lkt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_LookTBName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CReclassDlg::OnOutFileNameBtn(){
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

void CReclassDlg::OnOK(){
	// TODO: Add extra validation here
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.ShowWindow(SW_SHOW);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

	if(m_InputFileName.IsEmpty()){
		MessageBox("No input file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_LookTBName.IsEmpty()){
		MessageBox("No lookup table file.");
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
	float nodata = theInput->getNullDataValue();
	float* dataBuf = theInput->getDataBuf();
	int numOfPixels = nrow*ncol;

	float* tempBuf = new float[numOfPixels];

	//1. read in the data.
	ifstream fin;
	fin.open(m_LookTBName.GetBuffer(1));
	if(fin.fail()){
		AfxMessageBox("cannot open the lookup table file");
		return;

	}
	double tempInVal = 0;
	double tempOutVal = 0;
	List<DDNode>* theList = new List<DDNode>;
	bool tagDouble = false;
	bool hasEqual;
	int numOfValues = 0;

	while(!fin.eof()){
		fin>>tempInVal>>tempOutVal;
		if((tempOutVal - int(tempOutVal)) != 0)
			tagDouble = true;
        DDNode* tempNode = theList->first;
		hasEqual = false;
		while(tempNode != NULL)
		{
			if (tempOutVal == tempNode->d2)
			{
				hasEqual = true;
				break;
			}
			tempNode = tempNode->next;
		}
		if(hasEqual == false)
		{
			numOfValues ++;
		}

		theList->addTail(new DDNode(tempInVal, tempOutVal));
	}
	fin.close();

	if(tagDouble == false)  //if the output value is integer
	{
		UncertainColors* uncertaincolors = new UncertainColors;
	    int numBaseValues = numOfValues;
	    int* baseID = new int[numOfValues];
	    int i=0;
		DDNode* tempNode = theList->first;
	    while(tempNode != NULL){
			hasEqual = false;
			for(int j=0;j<i;j++)
			{
				if(tempNode->d2 == baseID[j])
				{
					hasEqual = true;
					break;
				}				
			}
		   if(hasEqual == false)
		   {
		      baseID[i] = tempNode->d2;
		      i++;
		   }
		   tempNode = tempNode->next;
		   
	    }

	  	uncertaincolors->SetGamma(false);
	    int ncolors = 0;
	   
		if((ncolors=uncertaincolors->BuildHSVPalette(baseID,numBaseValues))<0)
	    {
			   MessageBox("Error in allocating the color palette");
			   GetDlgItem(IDOK)->EnableWindow(TRUE);
		       return;
	   	}



		for(i=0;i<nrow;i++)
	    {
		    m_ProgressBar.SetPos(100*i/nrow);
		    for(int j=0;j<ncol;j++)
			{
				if(fabs(dataBuf[i*ncol+j]-nodata)<VERYSMALL)
			    tempBuf[i*ncol+j] = nodata;
		        else{
			        DDNode* tempNode = theList->first;
			        while(tempNode->next!=NULL){
				    if(theInput->getDataBuf()[i*ncol+j] >= tempNode->d1 && theInput->getDataBuf()[i*ncol+j] < tempNode->next->d1)
					{
					   tempBuf[i*ncol+j] = tempNode->d2;
					   break;
				    }
				    tempNode = tempNode->next;
			        }
			     if(theInput->getDataBuf()[i*ncol+j] >= theList->last->d1)
				   tempBuf[i*ncol+j] = theList->last->d2;
		
		       }
		   }
		}
	

	  m_ProgressBar.SetPos(100);

	  CThreeDR* theOutput = new CThreeDR;
	  theOutput->setDataBuf(tempBuf);
	  
	  //extract property map name(without .3dr from the full path)
	  char *MapName;
	  MapName = extract_filename(m_OutputFileName.GetBuffer(1)); //extractFileName(m_OutputFileName).GetBuffer(1);
      int length = strlen(MapName);
	  for (i=0; i<length;i++)
	  {
		  if(MapName[i] == '.')
		  {
			MapName[i] = '\0';
			break;
		 }
      } 
      theOutput->setDataName(MapName);

	  theOutput->copyBasicFields(theInput);
	  theOutput->calcStat();
	  theOutput->setNumberOfColors(ncolors);

      theOutput->writeout(m_OutputFileName.GetBuffer(1), 0);
      uncertaincolors->WritePalette(m_OutputFileName.GetBuffer(1));

	  //clean up
	  delete theList;
	  delete theInput;
	  delete theOutput;
	
	}
	else
	{
		for(int i=0;i<nrow;i++)
		{
			m_ProgressBar.SetPos(100*i/nrow);
			for(int j=0;j<ncol;j++)
			{
				if(fabs(dataBuf[i*ncol+j]-nodata)<VERYSMALL)
					tempBuf[i*ncol+j] = nodata;
				else{
					DDNode* tempNode = theList->first;
					while(tempNode->next!=NULL)
					{
						if(theInput->getDataBuf()[i*ncol+j] >= tempNode->d1 && theInput->getDataBuf()[i*ncol+j] < tempNode->next->d1)
						{
							tempBuf[i*ncol+j] = tempNode->d2;
							break;
						}
						tempNode = tempNode->next;
					}
					if(theInput->getDataBuf()[i*ncol+j] >= theList->last->d1)
						tempBuf[i*ncol+j] = theList->last->d2;

				}
			}
		}


	    m_ProgressBar.SetPos(100);

	    CThreeDR* theOutput = new CThreeDR;
	    theOutput->setDataBuf(tempBuf);
	
	   //extract property map name(without .3dr from the full path)
	    char *MapName;
	    MapName = extract_filename(m_OutputFileName.GetBuffer(1)); //extractFileName(m_OutputFileName).GetBuffer(1);
        int length = strlen(MapName);
	    for (i=0; i<length;i++)
	    {
		  if(MapName[i] == '.')
		  {
			MapName[i] = '\0';
			break;
		 }
        } 

        theOutput->setDataName(MapName);
      
	    theOutput->copyBasicFields(theInput);
	    theOutput->calcStat();

        theOutput->writeout(m_OutputFileName.GetBuffer(1), 0);
		 //clean up
	    delete theList;
	    delete theInput;
	    delete theOutput;	
	
	}


	GetDlgItem(IDOK)->EnableWindow(TRUE);
	CDialog::OnOK();
}




void CReclassDlg::clearFields(){
	m_InputFileName = "";
	m_LookTBName = "";
	m_OutputFileName = "";
	m_ProgressBar.SetPos(0);
	UpdateData(FALSE);
}

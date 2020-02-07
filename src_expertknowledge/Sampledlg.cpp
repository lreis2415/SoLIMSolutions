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
// SampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "SampleDlg.h"
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSampleDlg dialog


CSampleDlg::CSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSampleDlg)
	m_SampleMethod = 0;
	m_SubNoOpt = FALSE;
	m_RangeOpt = FALSE;
	m_SubNo = 0;
	m_TotalNoOpt = TRUE;
	m_TotalNo = 0;
	m_HorizontalRange = 0.0;
	m_VerticalRange = 0.0;
	m_Xmax = 0.0;
	m_Xmin = 0.0;
	m_Ymax = 0.0;
	m_Ymin = 0.0;
	m_NumberFileName = _T("");
	m_RegionFileName = _T("");
	//}}AFX_DATA_INIT
}


void CSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSampleDlg)
	DDX_Radio(pDX, IDC_RandomRdo, m_SampleMethod);
	DDX_Check(pDX, IDC_NoInSubChk, m_SubNoOpt);
	DDX_Check(pDX, IDC_RangeChk, m_RangeOpt);
	DDX_Text(pDX, IDC_NoInSubFld, m_SubNo);
	DDX_Check(pDX, IDC_TotalNoChk, m_TotalNoOpt);
	DDX_Text(pDX, IDC_TotalNoFld, m_TotalNo);
	DDX_Text(pDX, IDC_HorizontalRangeFld, m_HorizontalRange);
	DDX_Text(pDX, IDC_VerticalRangeFld, m_VerticalRange);
	DDX_Text(pDX, IDC_XmaxFld, m_Xmax);
	DDX_Text(pDX, IDC_XminFld2, m_Xmin);
	DDX_Text(pDX, IDC_YmaxFld, m_Ymax);
	DDX_Text(pDX, IDC_YminFld, m_Ymin);
	DDX_Text(pDX, IDC_NumberFileFld, m_NumberFileName);
	DDX_Text(pDX, IDC_RegionFileFld, m_RegionFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSampleDlg, CDialog)
	//{{AFX_MSG_MAP(CSampleDlg)
	ON_BN_CLICKED(IDC_RandomRdo, OnRandomRdo)
	ON_BN_CLICKED(IDC_SystematicRdo, OnSystematicRdo)
	ON_BN_CLICKED(IDC_StratifiedRdo, OnStratifiedRdo)
	ON_BN_CLICKED(IDC_NoInSubChk, OnNoInSubChk)
	ON_BN_CLICKED(IDC_RangeChk, OnRangeChk)
	ON_BN_CLICKED(IDC_TotalNoChk, OnTotalNoChk)
	ON_BN_CLICKED(IDC_NumberFileBtn, OnNumberFileBtn)
	ON_BN_CLICKED(IDC_RegionFileBtn, OnRegionFileBtn)
	ON_BN_CLICKED(IDC_RegionalRandomRdo, OnRegionalRandomRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleDlg message handlers
BOOL CSampleDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	rangeOption();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSampleDlg::OnRandomRdo(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_TotalNoOpt = TRUE;
	rangeOption();	
}

void CSampleDlg::OnStratifiedRdo(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_TotalNoOpt = TRUE;
	rangeOption();
}


void CSampleDlg::OnSystematicRdo(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_TotalNoOpt = TRUE;
	rangeOption();
}

void CSampleDlg::OnRegionalRandomRdo(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	rangeOption();
}

void CSampleDlg::OnTotalNoChk(){
	// TODO: Add your control notification handler code here
 	UpdateData(TRUE);
	switch(m_SampleMethod){
	case 0:
		break;
	case 1:
		stratifiedOption();
		break;
	case 2:
		systematicOption();
		break;
	}
}

void CSampleDlg::OnNoInSubChk(){
	// TODO: Add your control notification handler code here
 	UpdateData(TRUE);
	switch(m_SampleMethod){
	case 0:
		break;
	case 1:
		stratifiedOption();
		break;
	case 2:
		systematicOption();
		break;
	}	
}

void CSampleDlg::OnRangeChk(){
	// TODO: Add your control notification handler code here
 	UpdateData(TRUE);
	switch(m_SampleMethod){
	case 0:
		break;
	case 1:
		stratifiedOption();
		break;
	case 2:
		systematicOption();
		break;
	}	
}

void CSampleDlg::rangeOption(){
	switch(m_SampleMethod){
	case 0:
		m_TotalNoOpt = true;
 		GetDlgItem(IDC_TotalNoChk)->EnableWindow(TRUE);
		GetDlgItem(IDC_TotalNoFld)->EnableWindow(TRUE);
 		GetDlgItem(IDC_NumberFileLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_NumberFileFld)->EnableWindow(FALSE);
 		GetDlgItem(IDC_NumberFileBtn)->EnableWindow(FALSE);
		GetDlgItem(IDC_RegionFileLbl)->EnableWindow(FALSE);
 		GetDlgItem(IDC_RegionFileFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_RegionFileBtn)->EnableWindow(FALSE);
 		GetDlgItem(IDC_NoInSubChk)->EnableWindow(FALSE);
 		GetDlgItem(IDC_NoInSubFld)->EnableWindow(FALSE);
 		GetDlgItem(IDC_RangeChk)->EnableWindow(FALSE);
		GetDlgItem(IDC_VerticalRangeLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_VerticalRangeFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_HorizontalRangeLbl)->EnableWindow(FALSE);
 		GetDlgItem(IDC_HorizontalRangeFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_RegionLbl)->EnableWindow(TRUE);
 		GetDlgItem(IDC_XminLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_XminFld2)->EnableWindow(TRUE);
		GetDlgItem(IDC_XmaxLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_XmaxFld)->EnableWindow(TRUE);
 		GetDlgItem(IDC_YminLbl)->EnableWindow(TRUE);
 		GetDlgItem(IDC_YminFld)->EnableWindow(TRUE);
 		GetDlgItem(IDC_YmaxLbl)->EnableWindow(TRUE);
 		GetDlgItem(IDC_YmaxFld)->EnableWindow(TRUE);
		break;
	case 1:
		stratifiedOption();
		break;
	case 2:
		systematicOption();
		break;
	case 3:
		m_TotalNoOpt = false;
		m_SubNoOpt = false;
		m_RangeOpt = false;
 		GetDlgItem(IDC_NumberFileLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_NumberFileFld)->EnableWindow(TRUE);
 		GetDlgItem(IDC_NumberFileBtn)->EnableWindow(TRUE);
		GetDlgItem(IDC_RegionFileLbl)->EnableWindow(TRUE);
 		GetDlgItem(IDC_RegionFileFld)->EnableWindow(TRUE);
		GetDlgItem(IDC_RegionFileBtn)->EnableWindow(TRUE);
 		GetDlgItem(IDC_SampleGrp)->EnableWindow(FALSE);
 		GetDlgItem(IDC_TotalNoChk)->EnableWindow(FALSE);
		GetDlgItem(IDC_TotalNoFld)->EnableWindow(FALSE);
 		GetDlgItem(IDC_NoInSubChk)->EnableWindow(FALSE);
 		GetDlgItem(IDC_NoInSubFld)->EnableWindow(FALSE);
 		GetDlgItem(IDC_RangeChk)->EnableWindow(FALSE);
		GetDlgItem(IDC_VerticalRangeLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_VerticalRangeFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_HorizontalRangeLbl)->EnableWindow(FALSE);
 		GetDlgItem(IDC_HorizontalRangeFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_RegionLbl)->EnableWindow(FALSE);
 		GetDlgItem(IDC_XminLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_XminFld2)->EnableWindow(FALSE);
		GetDlgItem(IDC_XmaxLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_XmaxFld)->EnableWindow(FALSE);
 		GetDlgItem(IDC_YminLbl)->EnableWindow(FALSE);
 		GetDlgItem(IDC_YminFld)->EnableWindow(FALSE);
 		GetDlgItem(IDC_YmaxLbl)->EnableWindow(FALSE);
 		GetDlgItem(IDC_YmaxFld)->EnableWindow(FALSE);
		break;
	}
	UpdateData(FALSE);
}

void CSampleDlg::stratifiedOption(){
	GetDlgItem(IDC_NumberFileLbl)->EnableWindow(FALSE);
	GetDlgItem(IDC_NumberFileFld)->EnableWindow(FALSE);
	GetDlgItem(IDC_NumberFileBtn)->EnableWindow(FALSE);
	GetDlgItem(IDC_RegionFileLbl)->EnableWindow(FALSE);
	GetDlgItem(IDC_RegionFileFld)->EnableWindow(FALSE);
	GetDlgItem(IDC_RegionFileBtn)->EnableWindow(FALSE);
	GetDlgItem(IDC_TotalNoChk)->EnableWindow(TRUE);
	GetDlgItem(IDC_TotalNoFld)->EnableWindow(TRUE);
 	GetDlgItem(IDC_NoInSubChk)->EnableWindow(TRUE);
 	GetDlgItem(IDC_NoInSubFld)->EnableWindow(TRUE);
 	GetDlgItem(IDC_RangeChk)->EnableWindow(TRUE);
	GetDlgItem(IDC_VerticalRangeLbl)->EnableWindow(TRUE);
	GetDlgItem(IDC_VerticalRangeFld)->EnableWindow(TRUE);
	GetDlgItem(IDC_HorizontalRangeLbl)->EnableWindow(TRUE);
 	GetDlgItem(IDC_HorizontalRangeFld)->EnableWindow(TRUE);
	GetDlgItem(IDC_RegionLbl)->EnableWindow(TRUE);
	GetDlgItem(IDC_XminLbl)->EnableWindow(TRUE);
	GetDlgItem(IDC_XminFld2)->EnableWindow(TRUE);
	GetDlgItem(IDC_XmaxLbl)->EnableWindow(TRUE);
	GetDlgItem(IDC_XmaxFld)->EnableWindow(TRUE);
	GetDlgItem(IDC_YminLbl)->EnableWindow(TRUE);
	GetDlgItem(IDC_YminFld)->EnableWindow(TRUE);
	GetDlgItem(IDC_YmaxLbl)->EnableWindow(TRUE);
	GetDlgItem(IDC_YmaxFld)->EnableWindow(TRUE);
	if(m_TotalNoOpt){
		if(m_SubNoOpt){
			m_RangeOpt = false;
	 		GetDlgItem(IDC_RangeChk)->EnableWindow(FALSE);
			GetDlgItem(IDC_VerticalRangeLbl)->EnableWindow(FALSE);
			GetDlgItem(IDC_VerticalRangeFld)->EnableWindow(FALSE);
			GetDlgItem(IDC_HorizontalRangeLbl)->EnableWindow(FALSE);
 			GetDlgItem(IDC_HorizontalRangeFld)->EnableWindow(FALSE);
		}
		else if(m_RangeOpt){
			m_SubNoOpt = false;
	 		GetDlgItem(IDC_NoInSubChk)->EnableWindow(FALSE);
 			GetDlgItem(IDC_NoInSubFld)->EnableWindow(FALSE);
		}
	}
	else if(m_SubNoOpt&&m_RangeOpt){
		GetDlgItem(IDC_TotalNoChk)->EnableWindow(FALSE);
		GetDlgItem(IDC_TotalNoFld)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);	
}

void CSampleDlg::systematicOption(){
	GetDlgItem(IDC_NumberFileLbl)->EnableWindow(FALSE);
	GetDlgItem(IDC_NumberFileFld)->EnableWindow(FALSE);
 	GetDlgItem(IDC_NumberFileBtn)->EnableWindow(FALSE);
	GetDlgItem(IDC_RegionFileLbl)->EnableWindow(FALSE);
 	GetDlgItem(IDC_RegionFileFld)->EnableWindow(FALSE);
	GetDlgItem(IDC_RegionFileBtn)->EnableWindow(FALSE);
	GetDlgItem(IDC_TotalNoChk)->EnableWindow(TRUE);
	GetDlgItem(IDC_TotalNoFld)->EnableWindow(TRUE);
 	GetDlgItem(IDC_NoInSubChk)->EnableWindow(FALSE);
 	GetDlgItem(IDC_NoInSubFld)->EnableWindow(FALSE);
 	GetDlgItem(IDC_RangeChk)->EnableWindow(TRUE);
	GetDlgItem(IDC_VerticalRangeLbl)->EnableWindow(TRUE);
	GetDlgItem(IDC_VerticalRangeFld)->EnableWindow(TRUE);
	GetDlgItem(IDC_HorizontalRangeLbl)->EnableWindow(TRUE);
 	GetDlgItem(IDC_HorizontalRangeFld)->EnableWindow(TRUE);
	GetDlgItem(IDC_RegionLbl)->EnableWindow(TRUE);
	GetDlgItem(IDC_XminLbl)->EnableWindow(TRUE);
	GetDlgItem(IDC_XminFld2)->EnableWindow(TRUE);
	GetDlgItem(IDC_XmaxLbl)->EnableWindow(TRUE);
	GetDlgItem(IDC_XmaxFld)->EnableWindow(TRUE);
	GetDlgItem(IDC_YminLbl)->EnableWindow(TRUE);
	GetDlgItem(IDC_YminFld)->EnableWindow(TRUE);
	GetDlgItem(IDC_YmaxLbl)->EnableWindow(TRUE);
	GetDlgItem(IDC_YmaxFld)->EnableWindow(TRUE);
	m_SubNoOpt = false;
	if(m_TotalNoOpt){
		m_RangeOpt = false;
 		GetDlgItem(IDC_RangeChk)->EnableWindow(FALSE);
		GetDlgItem(IDC_VerticalRangeLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_VerticalRangeFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_HorizontalRangeLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_HorizontalRangeFld)->EnableWindow(FALSE);
	}
	else if(m_RangeOpt){
		GetDlgItem(IDC_TotalNoChk)->EnableWindow(FALSE);
		GetDlgItem(IDC_TotalNoFld)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);	
}

void CSampleDlg::OnNumberFileBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_NumberFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void CSampleDlg::OnRegionFileBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "Polygon Files (*.ply)|*.ply|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.ply", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_RegionFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void CSampleDlg::OnOK(){
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CString outputFile;
	static char BASED_CODE szFilter[] = "Point File (*.pnt)|*.pnt|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(FALSE, "pnt", ".pnt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK){
		outputFile = m_ldFile.GetPathName();
	}
	ofstream out;
	out.open(outputFile);
	if(out.fail()){
		MessageBox("Cannot open the output file");
		return;
	}
	out.precision(2);
	out.setf(ios::fixed);
	out.setf(ios::showpoint);

	int id = 0;
	if(m_SampleMethod==2){ //systematic
		if(m_TotalNoOpt){
			int NoOfRow = sqrt(m_TotalNo*(m_Ymax-m_Ymin)/(m_Xmax-m_Xmin+VERYSMALL))+0.5;
			int NoOfCol = sqrt(m_TotalNo*(m_Xmax-m_Xmin)/(m_Ymax-m_Ymin+VERYSMALL))+0.5;
			m_HorizontalRange = (m_Xmax-m_Xmin)/(NoOfCol+1);
			m_VerticalRange = (m_Ymax-m_Ymin)/(NoOfRow+1);
		}
		double curY = m_Ymin+m_VerticalRange;
		while(curY<m_Ymax){
			double curX = m_Xmin+m_HorizontalRange;
			while(curX<m_Xmax){
				id++;
				out<<id<<" "<<curX<<" "<<curY<<endl;
				curX+=m_HorizontalRange;
			}
			curY+=m_VerticalRange;
		}
		out.close();
	}
	else if(m_SampleMethod==0 || m_SampleMethod==1){ //random or stratified
		if(m_SampleMethod==0){ //random
			m_HorizontalRange = m_Xmax-m_Xmin;
			m_VerticalRange = m_Ymax-m_Ymin;
			m_SubNo = m_TotalNo;
		}
		else{ //stratified
			if(m_TotalNoOpt && m_SubNoOpt){
				int tot = m_TotalNo/m_SubNo;
				int NoOfRow = sqrt(tot*(m_Ymax-m_Ymin)/(m_Xmax-m_Xmin+VERYSMALL))+0.5;
				int NoOfCol = sqrt(tot*(m_Xmax-m_Xmin)/(m_Ymax-m_Ymin+VERYSMALL))+0.5;
				m_HorizontalRange = (m_Xmax-m_Xmin)/NoOfCol;
				m_VerticalRange = (m_Ymax-m_Ymin)/NoOfRow;
			}
			else if(m_TotalNoOpt && m_RangeOpt){
				m_SubNo = m_TotalNo/(((m_Xmax-m_Xmin)/(m_HorizontalRange+VERYSMALL))*((m_Ymax-m_Ymin)/(m_VerticalRange+VERYSMALL)));
			}
			else if(m_SubNoOpt && m_RangeOpt){
			}
			else{
				MessageBox("Conditions are not sufficient for stratified sampling.");
				return;
			}
		}
		double curXmin = m_Xmin;
		double curYmin = m_Ymin;
		double curXmax = m_Xmin+m_HorizontalRange;
		double curYmax = m_Ymin+m_VerticalRange;
		srand( (unsigned)time( NULL ) );
		while(curYmax<m_Ymax+VERYSMALL){
			while(curXmax<m_Xmax+VERYSMALL){
				double curX = 0;
				double curY = 0;
				for(int i=0; i<m_SubNo; i++){
					id++;
					do{
						curX = rand()*rand()/(rand()+VERYSMALL);
					}while(curX<curXmin||curX>curXmax);
					do{
						curY = rand()*rand()/(rand()+VERYSMALL);
					}while(curY<curYmin||curY>curYmax);

					out<<id<<" "<<curX<<" "<<curY<<endl;
				}
				curXmin = curXmax;
				curXmax+=m_HorizontalRange;
			}
			curYmin = curYmax;
			curYmax+=m_VerticalRange;
			curXmin = m_Xmin;
			curXmax = m_Xmin+m_HorizontalRange;
		}
		out.close();
	}
	else if(m_SampleMethod==3){ //regional random
		//read in number of samples in each region
		if(m_NumberFileName.IsEmpty()){
			MessageBox("No number-of-sample file.");
			return;
		}
		if(m_RegionFileName.IsEmpty()){
			MessageBox("No region file.");
			return;
		}
		ifstream fin_number;
		fin_number.open(m_NumberFileName);
		if(fin_number.fail()){
			MessageBox("Cannot open the number file.");
			return;
		}
		List<IINode>* numberList = new List<IINode>;
		CString tempString;
		while(!fin_number.eof()){
			fin_number.getline(tempString.GetBuffer(120), 100);
			tempString.ReleaseBuffer(-1);
			tempString.TrimLeft();
			tempString.TrimRight();
			if(tempString.IsEmpty())
				continue;
			int regionID = atoi(tempString.Left(tempString.FindOneOf(" ")));
			int number = atoi(tempString.Right(tempString.GetLength()-tempString.FindOneOf(" ")));
			numberList->addTail(new IINode(regionID, number));
		}
		fin_number.close();

		//read in the region polygons
		int pointID = 0;
		ifstream fin_poly;
		fin_poly.open(m_RegionFileName);
		if(fin_poly.fail()){
			MessageBox("Cannot open the region file.");
			return;
		}
		bool fend = false; //file end
		fin_poly.getline(tempString.GetBuffer(120), 100);
		tempString.ReleaseBuffer(-1);
		tempString.TrimLeft();
		tempString.TrimRight();
		if(tempString == "END" || tempString == "End")
			return;
		int polyID = atoi(tempString.Left(tempString.FindOneOf(" ")));
		while(!fin_poly.eof()){
			xPolygon* curPoly = new xPolygon(polyID);
			List<xPolygon>* islands = new List<xPolygon>;
			tempString.Empty();
			fin_poly.getline(tempString.GetBuffer(120), 100);
			tempString.ReleaseBuffer(-1);
			tempString.TrimLeft();
			tempString.TrimRight();
			int vid = 0;
			double x = 0;
			double y = 0;
			while(tempString != "END" && tempString != "End"){
				x = atof(tempString.Left(tempString.FindOneOf(" ")));
				y = atof(tempString.Right(tempString.GetLength()-tempString.FindOneOf(" ")));
				if(curPoly->vertexList->first==NULL||
					fabs(x-curPoly->vertexList->last->d1)>VERYSMALL||
					fabs(y-curPoly->vertexList->last->d2)>VERYSMALL)
					curPoly->vertexList->addTail(new IDDNode(vid, x, y));
				tempString.Empty();
				fin_poly.getline(tempString.GetBuffer(120), 100);
				tempString.ReleaseBuffer(-1);
				tempString.TrimLeft();
				tempString.TrimRight();
				vid++;
			}
			if(!fin_poly.eof()){
				fin_poly.getline(tempString.GetBuffer(120), 100);
				tempString.ReleaseBuffer(-1);
				tempString.TrimLeft();
				tempString.TrimRight();
				if(tempString == "END" || tempString == "End")
					fend = true;
				if(!fend){
					if(tempString.FindOneOf(" ")>0)
						polyID = atoi(tempString.Left(tempString.FindOneOf(" ")));
					else
						polyID = atoi(tempString);
				}
			}

			while(!fend && polyID == -99999){ //island
				xPolygon* newIsland = new xPolygon(polyID);
				tempString.Empty();
				fin_poly.getline(tempString.GetBuffer(120), 100);
				tempString.ReleaseBuffer(-1);
				tempString.TrimLeft();
				tempString.TrimRight();
				int vIsland = 0;
				double xIsland = 0;
				double yIsland = 0;
				while(tempString != "END" && tempString != "End"){
					xIsland = atof(tempString.Left(tempString.FindOneOf(" ")));
					yIsland = atof(tempString.Right(tempString.GetLength()-tempString.FindOneOf(" ")));
					if(newIsland->vertexList->first==NULL||
						fabs(x-newIsland->vertexList->last->d1)>VERYSMALL||
						fabs(y-newIsland->vertexList->last->d2)>VERYSMALL)
						newIsland->vertexList->addTail(new IDDNode(vIsland, xIsland, yIsland));
					tempString.Empty();
					fin_poly.getline(tempString.GetBuffer(120), 100);
					tempString.ReleaseBuffer(-1);
					tempString.TrimLeft();
					tempString.TrimRight();
					vIsland++;
				}
				if(fabs(newIsland->vertexList->first->d1-newIsland->vertexList->last->d1)>VERYSMALL || 
					fabs(newIsland->vertexList->first->d2-newIsland->vertexList->last->d2)>VERYSMALL) //not closed
					delete newIsland;
				else
					islands->addTail(newIsland);
				if(!fin_poly.eof()){
					fin_poly.getline(tempString.GetBuffer(120), 100);
					tempString.ReleaseBuffer(-1);
					tempString.TrimLeft();
					tempString.TrimRight();
					if(tempString == "END" || tempString == "End")
						fend = true;
					if(!fend){
						if(tempString.FindOneOf(" ")>0)
							polyID = atoi(tempString.Left(tempString.FindOneOf(" ")));
						else
							polyID = atoi(tempString);
					}
				}
			}	

			if(fabs(curPoly->vertexList->first->d1-curPoly->vertexList->last->d1)>VERYSMALL || 
				fabs(curPoly->vertexList->first->d2-curPoly->vertexList->last->d2)>VERYSMALL){ //check closure
				delete curPoly;
				delete islands;
				if(fend)
					break;
				else
					continue;
			}
	
			//find out the number in this region
			IINode* temp = numberList->first;
			while(temp!=NULL){
				if(temp->i1==curPoly->theID)
					break;
				temp = temp->next;
			}
			curPoly->getBound();	
			
			if(temp!=NULL){ //the number in the current region is greater than 0
				int id = 0;
				while(id<temp->i2){
					id++;
					pointID++;
					double curX = 0;
					double curY = 0;
					srand( (unsigned)time( NULL ) + rand());
					while(true){
						curX = rand()*(curPoly->Xmax-curPoly->Xmin)/RAND_MAX+curPoly->Xmin;
						curY = rand()*(curPoly->Ymax-curPoly->Ymin)/RAND_MAX+curPoly->Ymin;
						IDDNode* thePoint = new IDDNode(id, curX, curY);
						if(pointInPolygon(curPoly, islands, thePoint)){
							delete thePoint;
							break;
						}
						delete thePoint;
					}
					out<<pointID<<" "<<curX<<" "<<curY<<" "<<curPoly->theID<<endl;
				}
			}
		
			//clean up
			delete curPoly;
			delete islands;
			if(fend)
				break;
		}
		delete numberList;
		fin_poly.close();
		out.close();
	}

	CDialog::OnOK();
}

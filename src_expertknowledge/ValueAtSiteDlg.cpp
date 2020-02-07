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
// ValueAtSiteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "ValueAtSiteDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CValueAtSiteDlg dialog


CValueAtSiteDlg::CValueAtSiteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CValueAtSiteDlg::IDD, pParent)
	, m_Bilinear(FALSE)
{
	//{{AFX_DATA_INIT(CValueAtSiteDlg)
	m_DataFileName = _T("");
	m_OutputFileName = _T("");
	m_PntFileName = _T("");
	//}}AFX_DATA_INIT
}


void CValueAtSiteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CValueAtSiteDlg)
	DDX_Control(pDX, IDC_ValueAtSiteProBar, m_ProgressBar);
	DDX_Text(pDX, IDC_ValueAtSiteDataFld, m_DataFileName);
	DDX_Text(pDX, IDC_ValueAtSiteOutFld, m_OutputFileName);
	DDX_Text(pDX, IDC_ValueAtSitePntFld, m_PntFileName);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_NearestNeighborRdo, m_Bilinear);
}


BEGIN_MESSAGE_MAP(CValueAtSiteDlg, CDialog)
	//{{AFX_MSG_MAP(CValueAtSiteDlg)
	ON_BN_CLICKED(IDC_ValueAtSiteDataBtn, OnValueAtSiteDataBtn)
	ON_BN_CLICKED(IDC_ValueAtSiteOutBtn, OnValueAtSiteOutBtn)
	ON_BN_CLICKED(IDC_ValueAtSitePntBtn, OnValueAtSitePntBtn)
	ON_BN_CLICKED(IDValueAtSiteCancel, OnValueAtSiteCancel)
	ON_BN_CLICKED(IDValueAtSiteOK, OnValueAtSiteOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CValueAtSiteDlg message handlers

void CValueAtSiteDlg::OnValueAtSitePntBtn(){
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "Point Files (*.pnt)|*.pnt|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.pnt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_PntFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CValueAtSiteDlg::OnValueAtSiteDataBtn(){
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_DataFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CValueAtSiteDlg::OnValueAtSiteOutBtn() {
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "Text Document (*.txt)|*.txt|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(FALSE, "txt", ".txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK){
		m_OutputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CValueAtSiteDlg::OnValueAtSiteCancel(){
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CValueAtSiteDlg::OnValueAtSiteOK(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDValueAtSiteOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

	if(m_PntFileName.IsEmpty()){
		MessageBox("No point file.");
		GetDlgItem(IDValueAtSiteOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_DataFileName.IsEmpty()){
		MessageBox("No data file.");
		GetDlgItem(IDValueAtSiteOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_OutputFileName.IsEmpty()){
		MessageBox("No output file.");
		GetDlgItem(IDValueAtSiteOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	int numOfPoints = numOfRecords(m_PntFileName.GetBuffer(1));
	if(numOfPoints == 0){
		CString msg = "Cannot open ";
		msg += m_PntFileName;
		msg += " or file is empty.";
		MessageBox(msg);
		GetDlgItem(IDValueAtSiteOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	CThreeDR* theData = new CThreeDR;
	int readflag = theData->readin(m_DataFileName.GetBuffer(1), 0);
	if(readflag != 0){
		CString msg = "Cannot open ";
		msg += m_DataFileName;
		msg += " or file format has errors.";
		MessageBox(msg);
		GetDlgItem(IDValueAtSiteOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	int nrow = theData->getNumberOfRows();
	int ncol = theData->getNumberOfColumns();
	float nodata = theData->getNullDataValue();
	float cellsize = theData->getCellSize();

	ofstream fout;
	fout.open(m_OutputFileName);
	if(fout.fail()){
		MessageBox("Cannot open the output file");
		GetDlgItem(IDValueAtSiteOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	fout.precision(5);
	fout.setf(ios::fixed);
	fout.setf(ios::showpoint);

	ifstream pnt;
	pnt.open(m_PntFileName);
	if(pnt.fail()){
		MessageBox("Cannot open the point file");
		GetDlgItem(IDValueAtSiteOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	m_ProgressBar.ShowWindow(SW_SHOW);
	for(int i=0; i<numOfPoints; i++){
		m_ProgressBar.SetPos(i*100/numOfPoints);

		int id = 0;
		double x = 0.0;
		double y = 0.0;
		
		pnt>>id>>x>>y;
		fout.width(4);
		fout<<id;
		fout.width(15);
		fout<<x;
		fout.width(15);
		fout<<y;

		//to skip the rest of a record (a look up table may 
		//contain more than two columns.)
		char c;
		char utilityString[STRLEN];
		pnt.get(c);
		if(c != '\n'){
			pnt.getline(utilityString, 100);
		}

		//calc row and col of the point
		int row = theData->rowOfAPoint(y);
		int col = theData->colOfAPoint(x);
		if(row<0 || row>=nrow-1 ||
			col<0 || col>=ncol-1){
			fout<<"Out of the extent"<<endl;
		}
		else{
			float v = theData->getDataBuf()[row*ncol+col];
			if(fabs(v-nodata)<VERYSMALL) 
				fout<<"nodata"<<endl;
			else{
				fout.width(10);
				if(!m_Bilinear) //nearest neighbor
					fout<<theData->getDataBuf()[row*ncol+col]<<endl;
				else{ //bilinear
					double px = theData->xOfACell(col);
					double py = theData->yOfACell(row);
					double xleft = 0;
					double xright = 0;
					double yupper = 0;
					double ylower = 0;
					int cleft = 0;
					int cright = 0;
					int rupper = 0;
					int rlower = 0;
					if(px>x){
						xright = px;
						cright = col;
						xleft = px-cellsize;
						cleft = col-1;
					}
					else{
						xleft = px;
						cleft = col;
						xright = px+cellsize;
						cright = col+1;
					}
					if(py>y){
						yupper = py;
						rupper = row;
						ylower = py-cellsize;
						rlower = row+1;
					}
					else{
						ylower = py;
						rlower = row;
						yupper = py+cellsize;
						rupper = row-1;
					}
					if(cright>ncol-1) cright = ncol-1;
					if(cleft<0) cleft = 0;
					if(rupper<0) rupper = 0;
					if(rlower>nrow-1) rlower = rlower-1;
					float vul = theData->getDataBuf()[rupper*ncol+cleft];
					float vll = theData->getDataBuf()[rlower*ncol+cleft];
					float vur = theData->getDataBuf()[rupper*ncol+cright];
					float vlr = theData->getDataBuf()[rlower*ncol+cright];
					float wx1 = (x-xleft)/cellsize;
					float wx2 = 1-wx1;
					float wy1 = (y-ylower)/cellsize;
					float wy2 = 1-wy1;
					fout<<wx1*(wy1*vur+wy2*vlr)+wx2*(wy1*vul+wy2*vll)<<endl;
				}
			}
		}
	}
	delete theData;
	pnt.close();
	fout.close();
	m_ProgressBar.SetPos(100);

	GetDlgItem(IDPropertyMapOK)->EnableWindow(TRUE);
	CDialog::OnOK();
}

BOOL CValueAtSiteDlg::OnInitDialog(){
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//m_ProgressBar.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

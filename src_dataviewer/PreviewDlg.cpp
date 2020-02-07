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
// PreviewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "PreviewDlg.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreviewDlg dialog


CPreviewDlg::CPreviewDlg(CWnd* pParent)
: CDialog(CPreviewDlg::IDD, pParent), m_CurFileName(_T(""))
, m_X(0)
, m_Y(0)
, m_Row(0)
, m_Col(0)
, m_Pixel(0)
, m_Header(_T(""))
, m_InputFileName(_T(""))
, m_DecimalDigits(4)
{
	//{{AFX_DATA_INIT(CPreviewDlg)
		// NOTE: the ClassWizard will add member initialization here

	//}}AFX_DATA_INIT
	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_nID = CPreviewDlg::IDD;

	m_CurFileName = _T("");
	m_CurPixel = 0;
	m_CurX = 0;
	m_CurY = 0;

	m_theInput = NULL;
	m_bCaptured = false;
	m_curBtn = 0; //no button pushed
}

void CPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_InputFileFld, m_CurFileName);
	DDX_Text(pDX, IDC_XFld, m_X);
	DDX_Text(pDX, IDC_YFld, m_Y);
	DDX_Text(pDX, IDC_RowFld, m_Row);
	DDX_Text(pDX, IDC_ColFld, m_Col);
	DDX_Text(pDX, IDC_PixelFld, m_Pixel);
	DDX_Text(pDX, IDC_DecimalDigitsFld, m_DecimalDigits);
}


BEGIN_MESSAGE_MAP(CPreviewDlg, CDialog)
	//{{AFX_MSG_MAP(CPreviewDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_InputFileBtn, OnBnClickedInputfilebtn)
	ON_BN_CLICKED(IDC_RightBtn, OnBnClickedRightbtn)
	ON_BN_CLICKED(IDC_DownBtn, OnBnClickedDownbtn)
	ON_BN_CLICKED(IDC_LeftBtn, OnBnClickedLeftbtn)
	ON_BN_CLICKED(IDC_UpBtn, OnBnClickedUpbtn)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreviewDlg message handlers
BOOL CPreviewDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPreviewDlg::Create(){
	return CDialog::Create(m_nID, m_pParent);
}

void CPreviewDlg::OnPaint(){
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}

void CPreviewDlg::OnClose(){
	// TODO: Add your message handler code here and/or call default
	//((CMainFrame*)m_pParent)->PreviewDone();
	//DestroyWindow();	
}



void CPreviewDlg::OnOK(){
	// TODO: Add extra validation here
	UpdateData(TRUE);
	go();
}

void CPreviewDlg::OnBnClickedInputfilebtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_CurFileName = m_ldFile.GetPathName();
		m_InputFileName = m_CurFileName;
		UpdateData(FALSE);
	}
	else return;
	readFile();
	go();
}

void CPreviewDlg::readFile(){
	UpdateData(TRUE);
	if(m_CurFileName.IsEmpty()){
		MessageBox("No input file.");
		return;
	}
	int readflag = 0;
	int error = 0;

	m_theInput = new CThreeDR;
	readflag = m_theInput->readin(m_CurFileName.GetBuffer(1), 0);
	if(readflag != 0){
		MessageBox("Errors in openning the input file.");
		return;
	}	
}

void CPreviewDlg::OnBnClickedUpbtn(){
	// TODO: Add your control notification handler code here
	if(m_Row>0){
		m_Row--;
		go();
	}
}

void CPreviewDlg::OnBnClickedDownbtn(){
	// TODO: Add your control notification handler code here
	if(m_theInput!=NULL && m_Row<m_theInput->getNumberOfRows()-1){
		m_Row++;
		go();
	}
}

void CPreviewDlg::OnBnClickedRightbtn(){
	// TODO: Add your control notification handler code here
	if(m_theInput!=NULL && m_Col<m_theInput->getNumberOfColumns()-1){
		m_Col++;
		go();
	}
}

void CPreviewDlg::OnBnClickedLeftbtn(){
	// TODO: Add your control notification handler code here
	if(m_Col>0){
		m_Col--;
		go();
	}
}

void CPreviewDlg::go(){
	if(m_CurFileName != m_InputFileName){
		m_CurFileName = m_InputFileName;
		readFile();
	}
	if(m_theInput == NULL){
		MessageBox("No input file or reading error.");
		return;
	}

	if(m_CurPixel != m_Pixel){
		m_CurPixel = m_Pixel;
		m_Row = m_CurPixel/m_theInput->getNumberOfColumns();
		m_Col = m_CurPixel - m_Row*m_theInput->getNumberOfColumns(); 
		m_X = m_theInput->xOfACell(m_Col);
		m_Y = m_theInput->yOfACell(m_Row);
		m_CurX = m_X;
		m_CurY = m_Y;
	}
	else if(fabs(m_CurX-m_X)>VERYSMALL || fabs(m_CurY-m_Y)>VERYSMALL){
		m_CurX = m_X;
		m_CurY = m_Y;
		m_Row = m_theInput->rowOfAPoint(m_CurY);
		m_Col = m_theInput->colOfAPoint(m_CurX);
		m_CurPixel = m_Row*m_theInput->getNumberOfColumns() + m_Col; 
		m_Pixel = m_CurPixel;
	}
	else{
		m_CurPixel = m_Row*m_theInput->getNumberOfColumns() + m_Col; 
		m_Pixel = m_CurPixel;
		m_X = m_theInput->xOfACell(m_Col);
		m_Y = m_theInput->yOfACell(m_Row);
		m_CurX = m_X;
		m_CurY = m_Y;
	}
	UpdateData(FALSE);

	CString hd = "NumberOfRecords: ";
	char string[STRLEN];
	_ultoa(m_theInput->getNumberOfRecords(), string, 10);
	hd += string;
	hd += "\r\n";

	if(m_theInput->getNumberOfColumns() > 0){
		hd += "NumberOfColumns: ";
		_ultoa(m_theInput->getNumberOfColumns(), string, 10);
		hd += string;
		hd += "\r\n";
	}
	if(m_theInput->getNumberOfRows() > 0){
		hd += "NumberOfRows: ";
		_ultoa(m_theInput->getNumberOfRows(), string, 10);
		hd += string;
		hd += "\r\n";
	}
	if(strlen(m_theInput->getFileType())>0){
		hd += "FileType: ";
		hd += m_theInput->getFileType();
		hd += "\r\n";
	}
	if(strlen(m_theInput->getDataName())>0){
		hd += "DataName: ";
		hd += m_theInput->getDataName();
		hd += "\r\n";
	}
	if(strlen(m_theInput->getBaseFilename())>0){
		hd += "BaseFilename: ";
		hd += m_theInput->getBaseFilename();
		hd += "\r\n";
	}
	if(strlen(m_theInput->getGridUnits())>0){
		hd += "GridUnits: ";
		hd += m_theInput->getGridUnits();
		hd += "\r\n";
	}
	if(m_theInput->getXmin()>NODOUBLEVALUE){
		hd += "Xmin: ";
		_gcvt(m_theInput->getXmin(), 10, string);
		hd += string;
		hd += "\r\n";
	}
	if(m_theInput->getYmin()>NODOUBLEVALUE){
		hd += "Ymin: ";
		_gcvt(m_theInput->getYmin(), 10, string);
		hd += string;
		hd += "\r\n";
	}
	if(m_theInput->getCellSize()>NOFLOATVALUE){
		hd += "CellSize: ";
		_gcvt(m_theInput->getCellSize(), 10, string);
		hd += string;
		hd += "\r\n";
	}
	if(m_theInput->getNeighborhood()>NOFLOATVALUE){
		hd += "Neighborhood: ";
		_gcvt(m_theInput->getNeighborhood(), 10, string);
		hd += string;
		hd += "\r\n";
	}
	if(strlen(m_theInput->getDataUnits())>0){
		hd += "DataUnits: ";
		hd += m_theInput->getDataUnits();
		hd += "\r\n";
	}
	if(m_theInput->getNullDataValue()>NOFLOATVALUE){
		hd += "NullDataValue: ";
		_gcvt(m_theInput->getNullDataValue(), 10, string);
		hd += string;
		hd += "\r\n";
	}
	if(m_theInput->getDataMin()>NOFLOATVALUE){
		hd += "DataMin: ";
		_gcvt(m_theInput->getDataMin(), 10, string);
		hd += string;
		hd += "\r\n";
	}
	if(m_theInput->getDataMax()>NOFLOATVALUE){
		hd += "DataMax: ";
		_gcvt(m_theInput->getDataMax(), 10, string);
		hd += string;
		hd += "\r\n";
	}
	if(m_theInput->getDataMean()>NOFLOATVALUE){
		hd += "DataMean: ";
		_gcvt(m_theInput->getDataMean(), 10, string);
		hd += string;
		hd += "\r\n";
	}
	if(m_theInput->getDataStd()>NOFLOATVALUE){
		hd += "DataStd: ";
		_gcvt(m_theInput->getDataStd(), 10, string);
		hd += string;
		hd += "\r\n";
	}
	if(m_theInput->getNumberOfColors() > 0){
		hd += "NumberOfColors: ";
		_ultoa(m_theInput->getNumberOfColors(), string, 10);
		hd += string;
		hd += "\r\n";
	}
	if(strlen(m_theInput->getDataType())>0){
		hd += "DataType: ";
		hd += m_theInput->getDataType();
		hd += "\r\n";
	}
	if(m_theInput->getDataClampMin()>NOFLOATVALUE){
		hd += "DataClampMin: ";
		_gcvt(m_theInput->getDataClampMin(), 10, string);
		hd += string;
		hd += "\r\n";
	}
	if(m_theInput->getDataClampMax()>NOFLOATVALUE){
		hd += "DataClampMax: ";
		_gcvt(m_theInput->getDataClampMax(), 10, string);
		hd += string;
		hd += "\r\n";
	}
	if(m_theInput->getFirstDataByte() > 0){
		hd += "FirstDataByte: ";
		_ultoa(m_theInput->getFirstDataByte(), string, 10);
		hd += string;
	}
	GetDlgItem(IDC_FileHeaderFld)->SetWindowText(hd);

	//display data
	int nrow = m_theInput->getNumberOfRows();
	int ncol = m_theInput->getNumberOfColumns();
	char s2[STRLEN];

	CString dt = "	";
	for(int i = m_Col; i< m_Col+16; i++){
		if(i>ncol-1) break;
		_ultoa(i, s2, 10);
		dt += s2;
		dt += "	";
	}
	dt += "\r\n";
	dt += "\r\n";
	CString format = "%-8.";
	_ultoa(m_DecimalDigits, s2, 10);
	format = format + s2;
	format = format + "f";
	for(i = m_Row; i< m_Row+31; i++){
		if(i>nrow-1) break;
		_ultoa(i, s2, 10);
		dt += s2;
		dt += "	";
		for(int j = m_Col; j< m_Col+16; j++){
			if(j>ncol-1) break;
			int count = sprintf(s2,format.GetBuffer(1),m_theInput->getDataBuf()[i*ncol+j]);
			dt += s2;
			dt += "	";
		}
		dt += "\r\n";
	}
	GetDlgItem(IDC_DataFld)->SetWindowText(dt);
}

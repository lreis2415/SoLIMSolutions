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
// 3drToSagaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SoLIM.h"
#include "3drToSagaDlg.h"
#include ".\3drtosagadlg.h"
#include "gdal_priv.h"


IMPLEMENT_DYNAMIC(C3drToSagaDlg, CDialog)
C3drToSagaDlg::C3drToSagaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C3drToSagaDlg::IDD, pParent)
	, m_InputFileName(_T(""))
	, m_OutputFileName(_T(""))
{
}

C3drToSagaDlg::~C3drToSagaDlg()
{
}

void C3drToSagaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_InputFileFld, m_InputFileName);
	DDX_Text(pDX, IDC_OutputFileFld, m_OutputFileName);
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
}


BEGIN_MESSAGE_MAP(C3drToSagaDlg, CDialog)
	ON_BN_CLICKED(IDC_InputFileBtn, OnBnClickedInputfilebtn)
	ON_BN_CLICKED(IDC_OutputFileBtn, OnBnClickedOutputfilebtn)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


void C3drToSagaDlg::OnBnClickedInputfilebtn()
{
	UpdateData(TRUE);
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";

	if(m_ldFile.DoModal() == IDOK){
		m_InputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void C3drToSagaDlg::OnBnClickedOutputfilebtn()
{
	static char BASED_CODE szFilter[] = "Saga Files (*.sdat)|*.sdat|All Files (*.*)|*.*||";
		CFileDialog m_ldFile(FALSE, "sdat", ".sdat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
		m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK){
		m_OutputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void C3drToSagaDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);
	//m_ProgressBar.SetPos(1);

	if(m_InputFileName.IsEmpty()){
		MessageBox("No input file.");
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
	CThreeDR *input = new CThreeDR;
	int readFlag = input->readin(m_InputFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the input file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	// get the basic info from source
	int width = input->getNumberOfColumns();
	int height = input->getNumberOfRows();
	char *dataUnits = input->getGridUnits();
	int nBands = 1;
	float *dataBuffer = input->getDataBuf();
	double geoTransform[6];

	geoTransform[0] = input->getXmin();
	//geoTransform[3] = input->getYmin();
	geoTransform[3] = input->getYmin() + height * input->getCellSize();
	geoTransform[1] = input->getCellSize();
	geoTransform[5] = -1 * input->getCellSize();
	geoTransform[2] = 0;
	geoTransform[4] = 0;
	float noDataValue = input->getNullDataValue();

	//GDALAllRegister();
	// create gdal file
	GDALDriver *pDriver = NULL;
    pDriver = GetGDALDriverManager()->GetDriverByName("SAGA");
    if ( pDriver == NULL ) { return; }
  
    GDALDataset *pDataset = pDriver->Create(m_OutputFileName, width, height, nBands, GDT_Float32, NULL);
	if( pDataset == NULL ) { return; }
	
	// write to the output file
	pDataset->SetGeoTransform(geoTransform);
	GDALRasterBand *pBand = NULL;
	//pBand->SetUnitType(dataUnits);
	pBand = pDataset->GetRasterBand(1);
	pBand->SetNoDataValue(NOFLOATVALUE);//noDataValue);
    pBand->RasterIO(GF_Write,
                    0,
                    0,
                    width,
                    height,
                    dataBuffer,
                    width,
                    height,
                    GDT_Float32,
                    0,
                    0);
	pBand->SetNoDataValue(-9999.0);//noDataValue);

	/* Once we're done, close properly the dataset */
    if( pDataset != NULL )
        GDALClose( (GDALDatasetH) pDataset );

	m_ProgressBar.SetPos(100);

	OnOK();
}

void C3drToSagaDlg::OnBnClickedCancel()
{
	OnCancel();
}

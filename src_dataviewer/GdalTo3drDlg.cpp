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
// GdalTo3drDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SoLIM.h"
#include "GdalTo3drDlg.h"
#include "gdal_priv.h"


// CGdalTo3drDlg 对话框

IMPLEMENT_DYNAMIC(CGdalTo3drDlg, CDialog)
CGdalTo3drDlg::CGdalTo3drDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGdalTo3drDlg::IDD, pParent)
	, m_InputFileName(_T(""))
	, m_OutputFileName(_T(""))
	, m_DataUnit(_T("Meter"))
	, m_GridUnit(_T("Meter"))
{
}

CGdalTo3drDlg::~CGdalTo3drDlg()
{
}

void CGdalTo3drDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_InputFileFld, m_InputFileName);
	DDX_Text(pDX, IDC_OutputFileFld, m_OutputFileName);
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
	DDX_CBString(pDX, IDC_DataUnitCmb, m_DataUnit);
	DDX_CBString(pDX, IDC_GridUnitCmb, m_GridUnit);
}


BEGIN_MESSAGE_MAP(CGdalTo3drDlg, CDialog)
	ON_BN_CLICKED(IDC_InputFileBtn, OnBnClickedInputfilebtn)
	ON_BN_CLICKED(IDC_OutputFileBtn, OnBnClickedOutputfilebtn)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

void CGdalTo3drDlg::OnBnClickedInputfilebtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	static char BASED_CODE szFilter[] = "All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.*", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_InputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CGdalTo3drDlg::OnBnClickedOutputfilebtn(){
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

void CGdalTo3drDlg::OnBnClickedOk(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

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


	GDALDataset *poDataset;
	poDataset = (GDALDataset *) GDALOpen(m_InputFileName.GetBuffer(1), GA_ReadOnly);
	if(poDataset != NULL)
	{
		long imgWidth; //影响的高度，宽度
		long imgHeight;
		long bandNum;//波段的数目
		double GeoLeft;//地理范围
		double GeoTop;
		double GeoRight;
		double GeoBottom;
		double ResolutionY ;//分辨率
		double ResolutionX ;

		CString Projection ;//投影信息 
		CString DataType;//数据类型 
		double AdfGeoTransform[6];//投影转换参数	 
		CString FileName;//文件名字 ]
		int CurrentBand;

		imgWidth=poDataset->GetRasterXSize(); //影响的高度，宽度
		imgHeight=poDataset->GetRasterYSize();
		bandNum=poDataset->GetRasterCount();//波段的数目
		m_ProgressBar.SetPos(30);

		if(bandNum > 1)
		{
			AfxMessageBox("There are more than one band. Only the first one will be converted. ");
		}


		if( poDataset->GetProjectionRef()  != NULL )
		{
			Projection=poDataset->GetProjectionRef(); 
		}

		if( poDataset->GetGeoTransform( AdfGeoTransform ) == CE_None )
		{		
			GeoLeft=AdfGeoTransform[0];
			GeoTop=AdfGeoTransform[3] ;
			ResolutionX=AdfGeoTransform[1];
			ResolutionY=-AdfGeoTransform[5] ;
		}


		if( poDataset->GetGeoTransform( AdfGeoTransform ) == CE_None )
		{ 
			GeoRight = AdfGeoTransform[0] + imgWidth*AdfGeoTransform[1] + imgHeight*AdfGeoTransform[2];
			GeoBottom = AdfGeoTransform[3] + imgWidth*AdfGeoTransform[4] + imgHeight*AdfGeoTransform[5];

		}

		GDALRasterBand  *poBand=NULL;
			m_ProgressBar.SetPos(50);

		CurrentBand=1;
		poBand = poDataset->GetRasterBand(1);	
		if(poBand)
		{
			DataType=GDALGetDataTypeName(poBand->GetRasterDataType());//数据类型
			float nodataValue = poBand->GetNoDataValue();
			CString str = poBand->GetDescription();
			str = poBand->GetUnitType();
			int dataType=0;
			if(DataType=="Byte")dataType=0;
			else dataType = 1;

			m_ProgressBar.SetPos(80);


			byte* ByteBuff;
			//float* m_FloatBuff;
			byte* buf =NULL;
			float* buffFloat=NULL;

			buffFloat = new float[imgWidth*imgHeight];
			//m_FloatBuff = new float[width*height*3];

			if (CE_None==poBand->RasterIO( GF_Read, 0,0, imgWidth, imgHeight, buffFloat, imgWidth,imgHeight, GDT_Float32, 0, 0 ))
			{
				//CString strOutputFileName = m_OutputFileName.GetBuffer(1)
				CThreeDR* output = new CThreeDR;
				output->setNumberOfRows(imgHeight);
				output->setNumberOfColumns(imgWidth);
				output->setXmin(GeoLeft);
				output->setYmin(GeoBottom);
				output->setCellSize(ResolutionX);
				output->setNullDataValue(nodataValue);
				output->setDataName(extractFileName(m_OutputFileName).GetBuffer(1));

				output->setDataUnits(m_DataUnit.GetBuffer(1));
	           output->setGridUnits(m_GridUnit.GetBuffer(1));
				output->setDataBuf(buffFloat);
				output->calcStat();
				output->writeout(m_OutputFileName.GetBuffer(1),0);
				delete output;	
		
			}
				m_ProgressBar.SetPos(100);

		}  	

	}

	

	OnOK();
}

// CGdalTo3drDlg 消息处理程序

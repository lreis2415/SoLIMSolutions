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


#include "stdafx.h"
#include "SoLIM.h"
#include "GdalTo3dmDlg.h"
#include "gdal_priv.h"



IMPLEMENT_DYNAMIC(CGdalTo3dmDlg, CDialog)
CGdalTo3dmDlg::CGdalTo3dmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGdalTo3dmDlg::IDD, pParent)
	, m_InputFileName(_T(""))
	, m_OutputFileName(_T(""))
	, m_DataUnit(_T("Meter"))
	, m_GridUnit(_T("Meter"))
{
}

CGdalTo3dmDlg::~CGdalTo3dmDlg()
{
}

void CGdalTo3dmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_InputFileFld, m_InputFileName);
	DDX_Text(pDX, IDC_OutputFileFld, m_OutputFileName);
	DDX_CBString(pDX, IDC_DataUnitCmb, m_DataUnit);
	DDX_CBString(pDX, IDC_GridUnitCmb, m_GridUnit);
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
}


BEGIN_MESSAGE_MAP(CGdalTo3dmDlg, CDialog)
	ON_BN_CLICKED(IDC_InputFileBtn, OnBnClickedInputfilebtn)
	ON_BN_CLICKED(IDC_OutputFileBtn, OnBnClickedOutputfilebtn)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


void CGdalTo3dmDlg::OnBnClickedInputfilebtn(){
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

void CGdalTo3dmDlg::OnBnClickedOutputfilebtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	static char BASED_CODE szFilter[] = "3dm Files (*.3dm)|*.3dm|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(FALSE, "3dm", ".3dm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK){
		m_OutputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CGdalTo3dmDlg::OnBnClickedOk(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(1);

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


	     if(m_InputFileName.Right(4) == ".aux")
     m_InputFileName = dropFileSuffix(m_InputFileName);


	GDALDataset *poDataset;
	poDataset = (GDALDataset *) GDALOpen(m_InputFileName.GetBuffer(1), GA_ReadOnly);
	if(poDataset != NULL)
	{
		long ncol; 
		long nrow;
		long bandNum;
		double GeoLeft;
		double GeoTop;
		double GeoRight;
		double GeoBottom;
		double ResolutionY ;
		double ResolutionX ;

		CString Projection ;
		CString DataType; 
		double AdfGeoTransform[6];	 
		CString FileName;
		int CurrentBand;

		ncol=poDataset->GetRasterXSize();
		nrow=poDataset->GetRasterYSize();
		bandNum=poDataset->GetRasterCount();

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
			GeoRight = AdfGeoTransform[0] + ncol*AdfGeoTransform[1] + nrow*AdfGeoTransform[2];
			GeoBottom = AdfGeoTransform[3] + ncol*AdfGeoTransform[4] + nrow*AdfGeoTransform[5];

		}

		GDALRasterBand  *poBand=NULL;

		CurrentBand=1;
		poBand = poDataset->GetRasterBand(1);	
		m_ProgressBar.SetPos(50);
		if(poBand)
		{
			DataType=GDALGetDataTypeName(poBand->GetRasterDataType());
			float nodata = poBand->GetNoDataValue();
			CString str = poBand->GetDescription();
			str = poBand->GetUnitType();
			int dataType=0;
			if(DataType=="Byte")dataType=0;
			else dataType = 1;


			byte* ByteBuff;
			//float* m_FloatBuff;
			byte* buf =NULL;
			float* dataBuf=NULL;

			dataBuf = new float[nrow*ncol];
			//m_FloatBuff = new float[width*height*3];

			m_ProgressBar.SetPos(60);

			if (CE_None==poBand->RasterIO( GF_Read, 0,0, ncol, nrow, dataBuf, ncol, nrow, GDT_Float32, 0, 0 ))
			{


				
				for(int i=0; i<nrow; i++)
					for(int j=0; j<ncol; j++)
					{
						if (fabs(dataBuf[i*ncol+ j] - nodata) < VERYSMALL)
                           dataBuf[i*ncol + j] = NOFLOATVALUE;
					}
				

				int numOfPixels = nrow*ncol;
				//Create the photo buffer
				unsigned char* photoBuf = new unsigned char[nrow*ncol];
				//Strech the buf to 0-255
				float minmax[2];
				dataMinMax(dataBuf, numOfPixels, NOFLOATVALUE, minmax);
				for(int i=0; i<numOfPixels; i++)
					photoBuf[i] = (dataBuf[i]-minmax[0])*255/(minmax[1]-minmax[0]);

				//create the dem buffer
				int nrow2 = nrow+1;
				int ncol2 = ncol+1;
				float* demBuf = new float[nrow2*ncol2];
				//first row
				demBuf[0] = dataBuf[0];
				demBuf[ncol2-1] = dataBuf[ncol-1];
				for(int j=1; j<ncol2-1; j++)
					demBuf[j] = (dataBuf[j-1]+dataBuf[j])/2;
				//middle rows
				for(i=1; i<nrow2-1; i++){
					m_ProgressBar.SetPos(100*i/nrow);
					demBuf[i*ncol2] = (dataBuf[(i-1)*ncol]+dataBuf[i*ncol])/2;
					demBuf[(i+1)*ncol2-1] = (dataBuf[i*ncol-1]+dataBuf[(i+1)*ncol-1])/2;
					for(j=1; j<ncol2-1; j++)
						demBuf[i*ncol2+j] = (dataBuf[(i-1)*ncol+j-1]+dataBuf[(i-1)*ncol+j]+dataBuf[i*ncol+j-1]+dataBuf[i*ncol+j])/4;
				}
				//last row
				demBuf[(nrow2-1)*ncol2] = dataBuf[(nrow-1)*ncol];
				demBuf[nrow2*ncol2-1]=dataBuf[nrow*ncol-1];
				for(j=1; j<ncol2-1; j++)
					demBuf[(nrow2-1)*ncol2+j] = (dataBuf[(nrow-1)*ncol+j-1]+dataBuf[(nrow-1)*ncol+j])/2;

				CThreeDM* output = new CThreeDM;

				m_ProgressBar.SetPos(80);

				output->setNumberOfRows(nrow+1);
				output->setNumberOfColumns(ncol+1);
				output->setFileType("3DMAPPER");
				output->setGridUnits(m_GridUnit.GetBuffer(1));
				output->setXmin(GeoLeft);
				output->setYmin(GeoBottom);
				output->setCellSize(ResolutionX);
				output->setDataUnits(m_DataUnit.GetBuffer(1));
				output->setDataMin(minmax[0]);
				output->setDataMax(minmax[1]);
				output->setNullValue(NOFLOATVALUE);
				output->setOriginalNullValue(NOFLOATVALUE);
				output->setPhotoBuf(photoBuf);
				output->setDataBuf(demBuf);
				output->writeout(m_OutputFileName.GetBuffer(1));
				delete output;

			}

		}  

			GDALClose( (GDALDatasetH) poDataset);

	}



	m_ProgressBar.SetPos(100);

	OnOK();
}





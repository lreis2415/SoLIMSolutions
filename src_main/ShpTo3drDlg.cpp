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
// ShpTo3drDlg.cpp
//

#include "stdafx.h"
#include "SoLIM.h"
#include "ShpTo3drDlg.h"
#include "gdal_priv.h"

#include "ogrsf_frmts.h"

#include "cpl_string.h"



// CShpTo3drDlg 

IMPLEMENT_DYNAMIC(CShpTo3drDlg, CDialog)
CShpTo3drDlg::CShpTo3drDlg(CWnd* pParent /*=NULL*/)
: CDialog(CShpTo3drDlg::IDD, pParent)
, m_InputFileName(_T(""))
, m_OutputFileName(_T(""))
, m_GridUnit(_T("Meter"))
, m_cellSize(1)
{
	m_strSelectedField = "";

	m_numOfFields = 0;
}

CShpTo3drDlg::~CShpTo3drDlg()
{
}

void CShpTo3drDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_InputFileFld, m_InputFileName);
	DDX_Text(pDX, IDC_OutputFileFld, m_OutputFileName);
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
	DDX_Text(pDX, IDC_EDIT_DATA_UNIT, m_DataUnit);
	//DDX_CBString(pDX, IDC_DataUnitCmb, m_DataUnit);
	DDX_CBString(pDX, IDC_GridUnitCmb, m_GridUnit);
	DDX_Text(pDX, IDC_EDIT_CELL_SIZE, m_cellSize);
	DDX_Control(pDX, IDC_COMBO_FIELDS, m_comboField);
}


BEGIN_MESSAGE_MAP(CShpTo3drDlg, CDialog)
	ON_BN_CLICKED(IDC_InputFileBtn, OnBnClickedInputfilebtn)
	ON_BN_CLICKED(IDC_OutputFileBtn, OnBnClickedOutputfilebtn)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

void CShpTo3drDlg::OnBnClickedInputfilebtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString msg;
	static char BASED_CODE szFilter[] = "shapefile (*.shp)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.shp", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_InputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
		OGRDataSource  *poDS;
		poDS = OGRSFDriverRegistrar::Open(m_InputFileName, FALSE);
		if( poDS == NULL )
		{
			msg.Format("Error in reading %s.",m_InputFileName);
			AfxMessageBox(msg);
			return;
		}
		else
		{

			OGRLayer *poLayer = poDS->GetLayer(0);



			OGRFeatureDefn *source_layer_def = poLayer->GetLayerDefn();




			int numOfFields = source_layer_def->GetFieldCount();

			for(int i=0; i<numOfFields; i++)
			{
				const char *field = source_layer_def->GetFieldDefn(i)->GetNameRef();

				CString oneField;
				oneField.Format("%s",field );

				this->m_comboField.AddString(oneField);
			}



			this->m_numOfFields = numOfFields;

		}

	}
}

void CShpTo3drDlg::OnBnClickedOutputfilebtn(){
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

void CShpTo3drDlg::OnBnClickedOk(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);

	CString msg;


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

	if(this->m_cellSize <= 0)
	{
		MessageBox("Invalide cell size!");
		return;
	}


	if(m_comboField.GetCurSel()<0)
	{
		MessageBox("No value field is selected.");
		return;
	}
	m_comboField.GetLBText(m_comboField.GetCurSel(), m_strSelectedField);


	if(this->m_DataUnit.IsEmpty()){
		MessageBox("Please specify data unit (e.g.meters, feet, degrees). You can specify \"none\" if it is unitless");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}


	m_ProgressBar.SetPos(5);



	CString layerFileName = m_InputFileName;       


	OGRDataSource  *poDS;
	poDS = OGRSFDriverRegistrar::Open(layerFileName, FALSE);
	if( poDS == NULL )
	{
		msg.Format("Error in reading %s.",layerFileName);
		AfxMessageBox(msg);
		return;
	}
	else
	{

		OGRLayer *poLayer = poDS->GetLayer(0);
		OGRSpatialReference *source_srs = poLayer->GetSpatialRef();
		OGREnvelope *pExtent = new OGREnvelope();
		poLayer->GetExtent(pExtent);
		double x_min = pExtent->MinX;
		double x_max = pExtent->MaxX;
		double y_min = pExtent->MinY;
		double y_max = pExtent->MaxY;
		double pixel_size = this->m_cellSize;

		int numOfFeatures = poLayer->GetFeatureCount();


		CString strField; 


		strField =this->m_strSelectedField;



		int ncol = (x_max - x_min)/pixel_size;
		int nrow = (y_max - y_min)/pixel_size;


		const char *pszFormat = "HFA";
		GDALDriver *poDriver;

		poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);

		if(poDriver == NULL )
		{
			msg.Format("Error in reading %s. The layer will not be added",layerFileName);
			AfxMessageBox(msg);
			return;
		}


		GDALDataset *poDstDS;   


		CString strTempFileNameNoSuffix = extractFileName(layerFileName);

		strTempFileNameNoSuffix = dropFileSuffix(strTempFileNameNoSuffix);


		CString strTempFileName =  dropFileSuffix(this->m_OutputFileName); 



		strTempFileName = strTempFileName + ".img";


		poDstDS = poDriver->Create(strTempFileName.GetBuffer(1),ncol, nrow, 1, GDT_Float32, NULL );


		m_ProgressBar.SetPos(30);

		double adfGeoTransform[6] = {x_min, pixel_size, 0, y_max, 0, -pixel_size};


		poDstDS->SetGeoTransform(adfGeoTransform);


		char *pszSRS_WKT = NULL;

		if(source_srs != NULL)
		{

			source_srs->exportToWkt(&pszSRS_WKT);
			poDstDS->SetProjection(pszSRS_WKT);

			CPLFree( pszSRS_WKT );


		}


		poDstDS->GetRasterBand(1)->Fill(-9999);
		poDstDS->GetRasterBand(1)->SetNoDataValue(-9999);										

		GDALClose( (GDALDatasetH) poDstDS );


		CSoLIMApp* app = (CSoLIMApp *)AfxGetApp();	//pointer of the executable program

		CString parameters = "-a " + strField + " -l " + strTempFileNameNoSuffix + " " + "\""+ layerFileName + "\"" + " " + "\"" + strTempFileName + "\"";

		int result=(int)(ShellExecute(m_hWnd,_T("open"),_T("gdal_rasterize.exe"),parameters.GetBuffer(1),_T(app->m_exePath),SW_HIDE));



		if(result > 32)
		{	
			layerFileName.Format("%s",strTempFileName);
		}
		else
		{
			msg.Format("Error in converting file.");
			AfxMessageBox(msg);
			return;

		}

		Sleep(numOfFeatures * 100);

		m_ProgressBar.SetPos(70);



		GDALDataset *poDataset;
		poDataset = (GDALDataset *) GDALOpen(layerFileName.GetBuffer(1), GA_ReadOnly);
		if(poDataset == NULL)
		{
			msg.Format("Error in converting file.");
			AfxMessageBox(msg);
			return;
		}

		else
		{
			long imgWidth; 
			long imgHeight;
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

			imgWidth=poDataset->GetRasterXSize(); 
			imgHeight=poDataset->GetRasterYSize();
			bandNum=poDataset->GetRasterCount();



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

			CurrentBand=1;
			poBand = poDataset->GetRasterBand(1);	
			if(poBand)
			{
				float nodataValue = poBand->GetNoDataValue();
				//byte* ByteBuff;

				//byte* buf =NULL;
				float* buffFloat=NULL;
				buffFloat = new float[imgWidth*imgHeight];					
				if (CE_None==poBand->RasterIO( GF_Read, 0,0, imgWidth, imgHeight, buffFloat, imgWidth,imgHeight, GDT_Float32, 0, 0 ))
				{
					CThreeDR* output = new CThreeDR;
					output->setNumberOfRows(imgHeight);
					output->setNumberOfColumns(imgWidth);
					output->setXmin(GeoLeft);
					output->setYmin(GeoBottom);
					output->setCellSize(ResolutionX);
					output->setNullDataValue(nodataValue);

					CString strLayerFileName = extractFileName(this->m_OutputFileName);



					strLayerFileName = dropFileSuffix(strLayerFileName);

					//CString strNewLayerFilePath= pDoc->m_strProjectDir+"\\GISData\\"+strLayerFileName + ".3dr";

					output->setDataName(strLayerFileName.GetBuffer(1));
					output->setDataBuf(buffFloat);
					output->calcStat();
					output->writeout(this->m_OutputFileName.GetBuffer(1),0);
					delete output;	
				}


				else 
				{
					msg.Format("Error in converting file.");
					AfxMessageBox(msg);
					return;
				}
			}  	

			else 
			{
				msg.Format("Error in converting file.");
				AfxMessageBox(msg);
				return;
			}
		}


		GDALClose( (GDALDatasetH) poDataset);

		m_ProgressBar.SetPos(100);
		DeleteFile(strTempFileName.GetBuffer(1));


	}



	OnOK();
}


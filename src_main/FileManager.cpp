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
#include "SoLIMDoc.h"
#include "FileManager.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include "SHPSelectFieldDlg.h"
#include "Shp2RasterCellSizeDlg.h"

CFileManager::CFileManager()
{
}

CFileManager::CFileManager(CSoLIMDoc *pDoc, HWND hWnd)
{
	_pDoc = pDoc;
	_hWnd = hWnd;
}

CFileManager::~CFileManager()
{
}

CString CFileManager::GetFileTypeByExt(CString ext)
{
    char *type = NULL;  
    if      (0 == strcmp(ext,".3dr")) type = "3dr";  
    else if (0 == strcmp(ext,".sdat")) type = "SAGA";  
	else if (0 == strcmp(ext,".acs")) type = "AAIGrid"; 
    else if (0 == strcmp(ext,".img")) type = "HFA";  
    else if (0 == strcmp(ext,".tif")) type = "GTiff";
    else type = "3dr";
    return type;  
}

CThreeDR* CFileManager::ReadinShp(CString layerFileName)
{
	CThreeDR* output = new CThreeDR;
	CString layerFileFullName;
	CString strTempFileName;
	CString msg = "";
	OGRDataSource *poDS;
	poDS = OGRSFDriverRegistrar::Open(layerFileName, FALSE);
	if( poDS == NULL )
	{
		msg.Format("Error in reading %s. The layer will not be added",layerFileName);
		AfxMessageBox(msg);
		return NULL;
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
		int numOfFeatures = poLayer->GetFeatureCount();
		double pixel_size;
		if(_pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum() == 0)
		{
			CShp2RasterCellSizeDlg dlg;
			dlg.m_strText = "Shapefile " + layerFileName + " will be converted to .3dr file (raster format). Please specify cell size:";
			if(dlg.DoModal() == IDOK)
			{
				pixel_size = dlg.m_cellSize;
			}
			else
			{
				msg.Format("Cell size is not specified. The layer %s will not be added",layerFileName);
				AfxMessageBox(msg);
				return NULL;
			}
		}			 
		else
		{
			pixel_size = _pDoc->m_pProjectEdit->m_pEnvDatabase->m_pTheSetting->getCellSize();
		}
		OGRFeatureDefn *source_layer_def = poLayer->GetLayerDefn();
		CSHPSelectFieldDlg dlg;
		int numOfFields = source_layer_def->GetFieldCount();
		dlg.fields = new CString[numOfFields];
		dlg.numOfFields = numOfFields;
		dlg.m_strText = "Please specify the value field which will be used to rasterize shapefile " + layerFileName + ":";
		for(int i=0; i<numOfFields; i++)
		{
			const char *field = source_layer_def->GetFieldDefn(i)->GetNameRef();
			CString oneField;
			oneField.Format("%s",field );
			dlg.fields[i] = oneField;		
		}
		CString strField; 
		if(dlg.DoModal() == IDOK)
		{
			strField =dlg.strSelectedField;
		}
		else
		{
			msg.Format("Value field is not specified. The layer %s will not be added",layerFileName);
			AfxMessageBox(msg);
			return NULL;
		}
		int ncol = (x_max - x_min)/pixel_size;
		int nrow = (y_max - y_min)/pixel_size;
		const char *pszFormat = "HFA";
		GDALDriver *poDriver;
		poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
		if(poDriver == NULL )
		{
			msg.Format("Error in reading %s. The layer will not be added",layerFileName);
			AfxMessageBox(msg);
			return NULL;
		}
		GDALDataset *poDstDS;       
		strTempFileName = extractFileName(layerFileName);						
		CString strTempFileNameNoSuffix = dropFileSuffix(strTempFileName);
		strTempFileName= _pDoc->m_strProjectDir+"\\GISData\\"+strTempFileNameNoSuffix + ".img";
		poDstDS = poDriver->Create(strTempFileName.GetBuffer(1),ncol, nrow, 1, GDT_Float32, NULL );
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
		CString parameters = "-a " + strField + " -l " + strTempFileNameNoSuffix + " " + layerFileName + " " + strTempFileName;
		int result=(int)(ShellExecute(_hWnd,_T("open"),_T("gdal_rasterize.exe"),parameters.GetBuffer(1),_T(app->m_exePath),SW_SHOW));
		if(result > 32)
		{	
			layerFileName.Format("%s",strTempFileName);
		}
		else
		{
			msg.Format("Error in reading %s. The layer will not be added",layerFileName);
			AfxMessageBox(msg);
			return NULL;
		}
		Sleep(numOfFeatures * 100);
	}
	GDALDataset *poDataset;
	poDataset = (GDALDataset *) GDALOpen(layerFileName.GetBuffer(1), GA_ReadOnly);
	if(poDataset == NULL)
	{
		msg.Format("Error in converting shapefile to 3dr file.");
		AfxMessageBox(msg);
		return NULL;
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
		if(bandNum > 1)
		{
			msg.Format("There are more than one band in %s. Only the first one will be converted.",layerFileName);
			AfxMessageBox(msg);
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
		CurrentBand=1;
		poBand = poDataset->GetRasterBand(1);	
		if(poBand)
		{
			float nodataValue = poBand->GetNoDataValue();
			byte* ByteBuff;
			byte* buf =NULL;
			float* buffFloat=NULL;
			buffFloat = new float[imgWidth*imgHeight];					
			if (CE_None==poBand->RasterIO( GF_Read, 0,0, imgWidth, imgHeight, buffFloat, imgWidth, imgHeight, GDT_Float32, 0, 0 ))
			{
				output->setNumberOfRows(imgHeight);
				output->setNumberOfColumns(imgWidth);
				output->setXmin(GeoLeft);
				output->setYmin(GeoBottom);
				output->setCellSize(ResolutionX);
				output->setNullDataValue(nodataValue);
				CString strLayerFileName = extractFileName(layerFileName);							
				strLayerFileName = dropFileSuffix(strLayerFileName);
				CString strNewLayerFilePath= _pDoc->m_strProjectDir+"\\GISData\\"+strLayerFileName + ".3dr";
				output->setDataName(strLayerFileName.GetBuffer(1));
				output->setDataBuf(buffFloat);
				output->calcStat();
				//output->writeout(strNewLayerFilePath.GetBuffer(1),0);
			}
			else 
			{
				msg.Format("Error in converting shapefile to 3dr file.");
				AfxMessageBox(msg);
				return NULL;
			}
		}  	
		else 
		{
			msg.Format("Error in converting shapefile to 3dr file.");
			AfxMessageBox(msg);
			return NULL;
		}
	}

	GDALClose( (GDALDatasetH) poDataset);
	DeleteFile(strTempFileName.GetBuffer(1));
	return output;
}




CThreeDR* CFileManager::ReadinGDAL(CString layerFileName)
{
	CString msg = "";
	CThreeDR *output = new CThreeDR();
	// GDAL-Supported files
	
	if(layerFileName.Right(4) == ".aux")
		layerFileName = dropFileSuffix(layerFileName);

	GDALDataset *poDataset;
	poDataset = (GDALDataset *) GDALOpen(layerFileName.GetBuffer(1), GA_ReadOnly);
	if(poDataset == NULL)
	{
		msg.Format("Error in reading %s. The layer will not be added",layerFileName);
		AfxMessageBox(msg);
		return false;
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
		char *projection = "";
		CString DataType;
		double AdfGeoTransform[6];	 
		CString FileName;
		int CurrentBand;
		imgWidth=poDataset->GetRasterXSize(); 
		imgHeight=poDataset->GetRasterYSize();
		bandNum=poDataset->GetRasterCount();
		if(bandNum > 1)
		{
			msg.Format("There are more than one band in %s. Only the first one will be converted.",layerFileName);
			AfxMessageBox(msg);
		}
		if( poDataset->GetProjectionRef()  != NULL )
		{
			Projection=poDataset->GetProjectionRef();
		}

		// get projection from saga file
		if (layerFileName.Right(5) == ".sdat")
		{
			CString filename_prj = layerFileName.GetBuffer(0);
			filename_prj.Replace(".sdat", ".prj");
			
			FILE *pFile = fopen(filename_prj, "r");
			if(pFile != NULL)
			{
				char *pBuf;
				fseek(pFile, 0, SEEK_END);
				int len = ftell(pFile);
				pBuf = new char[len+1];
				rewind(pFile);
				fread(pBuf, 1, len, pFile);
				pBuf[len] = 0;		
				projection = pBuf;
				fclose(pFile);
			}
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
			float nodataValue = NOFLOATVALUE;//poBand->GetNoDataValue();
			byte* ByteBuff;
			byte* buf =NULL;
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
				output->setDataUnits("Meters");
				output->setGridUnits("Meters");

				//output->setProjection(projection);

				CString strLayerFileName = extractFileName(layerFileName);							

				strLayerFileName = dropFileSuffix(strLayerFileName);

				//CString strNewLayerFilePath= _pDoc->m_strProjectDir+"\\GISData\\" + strCategory + "\\"+strLayerFileName + ".3dr";

				output->setDataName(strLayerFileName.GetBuffer(1));
				output->setDataBuf(buffFloat);
				output->calcStat();
				return output;
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			return NULL;
		}
	}
}


CThreeDR* CFileManager::Readin3dr(CString filename)
{
	CThreeDR *output = new CThreeDR();
	output->readin(filename.GetBuffer(1), 0);
	return output;
}

CThreeDR* CFileManager::Readin(CString filename)
{
	CThreeDR* newLayer = NULL;
	if(filename == "")
	{
		AfxMessageBox("No input file.");
		return NULL;
	}
	if(filename.Right(4) == ".shp")
	{
		newLayer = this->ReadinShp(filename);
	}
	else if(filename.Right(4) != ".3dr") // GDAL-Supported
	{
		newLayer = this->ReadinGDAL(filename);
	}
	else // 3dr
	{
		newLayer = this->Readin3dr(filename);
	}
	return newLayer;
}


bool CFileManager::Writeout3dr(CThreeDR* pTheData, CString outputFilename)
{
	pTheData->writeout(outputFilename.GetBuffer(1), 0);
	return true;
}

bool CFileManager::WriteoutGDAL(CThreeDR* input, CString outputFilename, CString fileType)
{
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
    pDriver = GetGDALDriverManager()->GetDriverByName(fileType);
    if ( pDriver == NULL ) { return false; }
  
    GDALDataset *pDataset = pDriver->Create(outputFilename, width, height, nBands, GDT_Float32, NULL);
	if( pDataset == NULL ) { return false; }
	
	// write to the output file
	pDataset->SetGeoTransform(geoTransform);
	GDALRasterBand *pBand = NULL;
	//pBand->SetUnitType(dataUnits);
	pBand = pDataset->GetRasterBand(1);
	pBand->SetNoDataValue(noDataValue);
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

	/* Once we're done, close properly the dataset */
    if( pDataset != NULL )
        GDALClose( (GDALDatasetH) pDataset );

	return true;
}

bool CFileManager::Writeout(CThreeDR* pTheData, CString outputFilename, CString outputExt)
{
	if(outputExt == ".3dr")
	{
		this->Writeout3dr(pTheData, outputFilename);
		return true;
	}
	else // gdal-supported raster data
	{
		WriteoutGDAL(pTheData, outputFilename, GetFileTypeByExt(outputExt));
		return true;
	}
}


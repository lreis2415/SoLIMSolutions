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
#include "ThreeDR.h"
#include "gdal_priv.h"
#include "gdalwarper.h"
#include "CBRUtility.h"

using namespace std;

int calcBasicKde( int n, double * x, int nTarget, double * y, double h, double * g )
{
	for(int i=0; i<nTarget; ++i)
	{
		g[i] = 0.0;
		for (int j=0; j<n; ++j)
		{
			double tmp = (y[i]-x[j]) / h;
			g[i] += exp(-tmp*tmp/2);
		}
		g[i] /= n * h * sqrt(2*PI);
	}
	return 0;
}

double threeDR2Ascii(char* filename, const char* outputfile)
{
	CThreeDR input;
	int readFlag = input.readin(filename, 0);
	double res = input.getCellSize();

	int nrow = input.getNumberOfRows();
	int ncol = input.getNumberOfColumns();
	int numOfPixels = nrow*ncol;

	FILE *fp;
	fp=fopen(outputfile, "w");

	fprintf(fp, "%s %d\n", "ncols        ", ncol);
	fprintf(fp, "%s %d\n", "nrows        ", nrow);
	fprintf(fp, "%s %f\n", "xllcorner    ", input.getXmin());
	fprintf(fp, "%s %f\n", "yllcorner    ", input.getYmin());
	fprintf(fp, "%s %f\n", "cellsize     ", input.getCellSize());
	fprintf(fp, "%s %f\n", "NODATA_value ", input.getNullDataValue()); 


	for(int i=0;i<nrow;i++)
	{
		int m = 100*i/nrow;
		for(int j=0;j<ncol;j++)
			fprintf(fp, "%f ", input.getDataBuf()[i*ncol+j]);
		fprintf(fp, "\n");
	}
	fclose(fp);
	
	return res;
}

bool resampleThreeDR(char* filename, float resolution, char* outputfile)
{
	//Build GDAL data structure from 3dr
	double srcRes = threeDR2Ascii(filename, "tmpascii.txt");

	GDALDataset *poDataset = (GDALDataset *) GDALOpen("tmpascii.txt", GA_ReadOnly);
	if(poDataset != NULL)
	{
		int nSrcXSize = poDataset->GetRasterXSize();
		int nSrcYSize = poDataset->GetRasterYSize();
		double GeoLeft;//地理范围
		double GeoTop;
		double GeoRight;
		double GeoBottom;
		double ResolutionY ;//分辨率
		double ResolutionX ;
		double AdfGeoTransform[6];//投影转换参数	
		if( poDataset->GetGeoTransform( AdfGeoTransform ) == CE_None )
		{		
			GeoLeft=AdfGeoTransform[0];
			GeoTop=AdfGeoTransform[3] ;
			ResolutionX=AdfGeoTransform[1];
			ResolutionY=-AdfGeoTransform[5] ;
			GeoRight = AdfGeoTransform[0] + nSrcXSize*AdfGeoTransform[1] + nSrcYSize*AdfGeoTransform[2];
			GeoBottom = AdfGeoTransform[3] + nSrcXSize*AdfGeoTransform[4] + nSrcYSize*AdfGeoTransform[5];
		}

		int nDstXSize = int(nSrcXSize * srcRes / resolution);
		int nDstYSize = int(nSrcYSize * srcRes / resolution);
		
		float *buffFloat = new float[nDstXSize*nDstYSize];
		GDALRasterBand  *poBand = poDataset->GetRasterBand(1);


		if (CE_None == poBand->RasterIO( GF_Read, 0, 0, 
			nSrcXSize, nSrcYSize, buffFloat, nDstXSize,nDstYSize, GDT_Float32, 0, 0 ))
		{
			//CString strOutputFileName = m_OutputFileName.GetBuffer(1)
			CThreeDR* output = new CThreeDR;
			output->setNumberOfRows(nDstYSize);
			output->setNumberOfColumns(nDstXSize);
			output->setXmin(GeoLeft);
			output->setYmin(GeoBottom);
			output->setCellSize(resolution);
			output->setNullDataValue((float)poBand->GetNoDataValue());
			output->setDataName(outputfile);
			output->setDataBuf(buffFloat);
			output->calcStat();
			output->writeout(outputfile, 0);
			delete output;	
		}
		GDALClose( (GDALDatasetH) poDataset );
	}

	CFile tmpFile;
	tmpFile.Remove("tmpascii.txt");

	return true;
}

//int main()	{
//	fstream file1;
//	fstream file2;
//	file1.open("file1.txt");
//	file2.open("file2.txt");
//
//	vector<Point> vecFunc1;
//	vector<Point> vecFunc2;
//	Point temppoint={0,0};
//
//	while(!file1.eof())	{
//		file1>>temppoint.x;
//		file1>>temppoint.y;
//		vecFunc1.push_back(temppoint);
//	}
//	while(!file2.eof())	{
//		file2>>temppoint.x;
//		file2>>temppoint.y;
//		vecFunc2.push_back(temppoint);
//	}
//
//	CrossArea(vecFunc1,vecFunc2);
//	CurveArea(vecFunc2);
//
//
//	return 0;
//}

double CrossArea(vector<Point>& func1, vector<Point>& func2)	{

	int iFunc1Size=func1.size();
	int iFunc2Size=func2.size();
	double dMinX=0, dMaxX=0;
	
	if ((func1[0].x > func2[iFunc2Size-1].x) || (func2[0].x > func1[iFunc1Size-1].x) )
		return 0;
	
	if(func1[0].x >= func2[0].x)	{
		dMinX = func1[0].x;
	}
	else{
		dMinX = func2[0].x;
	}
	if(func1[iFunc1Size-1].x <= func2[iFunc2Size-1].x)	{
		dMaxX = func1[iFunc1Size-1].x;
	}
	else{
		dMaxX = func2[iFunc2Size-1].x;
	}

	vector<Point>::iterator iterFunc1=func1.begin();
	vector<Point>::iterator iterFunc2=func2.begin();
	double pFunc1Eval;
	double pFunc2Eval;
	double *pFuncEval=new double[10000];
	double dInterval=(dMaxX-dMinX)/10000;

	for(iterFunc1; iterFunc1!=func1.end(); iterFunc1++)	 {
		if((*iterFunc1).x >= dMinX)	{
			break;
		}
	}

	for(iterFunc2; iterFunc2!=func1.end(); iterFunc2++)	 {
		if((*iterFunc2).x >= dMinX)	{
			break;
		}
	}

	if((*iterFunc1).x == dMinX)	{
		iterFunc1++;
	}
	else{
		iterFunc2++;
	}

	double dCurrentX=dMinX;
	for(int i=0;i<10000;i++){

		if((dMinX+i*dInterval) > (*iterFunc1).x){
			iterFunc1++;
		}
		if((dMinX+i*dInterval) > (*iterFunc2).x){
			iterFunc2++;
		}

		pFunc1Eval=(*(iterFunc1-1)).y+
			(dMinX+i*dInterval-(*(iterFunc1-1)).x)/((*iterFunc1).x-(*(iterFunc1-1)).x)*((*iterFunc1).y-(*(iterFunc1-1)).y);
		pFunc2Eval=(*(iterFunc2-1)).y+
			(dMinX+i*dInterval-(*(iterFunc2-1)).x)/((*iterFunc2).x-(*(iterFunc2-1)).x)*((*iterFunc2).y-(*(iterFunc2-1)).y);

		if(pFunc1Eval <= pFunc2Eval){
			pFuncEval[i]=pFunc1Eval;
		}
		else{
			pFuncEval[i] = pFunc2Eval;
		}
	}

	double dCrossArea=0;
	for(int i=1;i<10000;i++){
		dCrossArea += (pFuncEval[i]+pFuncEval[i-1])*dInterval/2;
	}
	delete pFuncEval;

	return dCrossArea;
}

double CurveArea(vector<Point>& func1)	{

	int iFunc1Size=func1.size();
	double dMinX=func1[0].x, dMaxX=func1[iFunc1Size-1].x;

	vector<Point>::iterator iterFunc1=func1.begin();
	double pFunc1Eval;
	double *pFuncEval=new double[10000];
	double dInterval=(dMaxX-dMinX)/10000;

	iterFunc1++;

	double dCurrentX=dMinX;
	for(int i=0;i<10000;i++){

		if((dMinX+i*dInterval) > (*iterFunc1).x)	 {
			iterFunc1++;
		}

		pFunc1Eval=(*(iterFunc1-1)).y+
			(dMinX+i*dInterval-(*(iterFunc1-1)).x)/((*iterFunc1).x-(*(iterFunc1-1)).x)*((*iterFunc1).y-(*(iterFunc1-1)).y);

		pFuncEval[i]=pFunc1Eval;

	}

	double dCurveArea=0;
	for(int i=1;i<10000;i++)	 {
		dCurveArea+=(pFuncEval[i]+pFuncEval[i-1])*dInterval/2;
	}
	delete pFuncEval;

	return dCurveArea;
}

bool isNumeric(CString str)
{
	str.Replace(" ", "");

	bool bIsDigit = true;
	int nCount = str.GetLength(); // 获得字符个数
	for ( int i = 0; i < nCount; i ++ )
	{
		char ch = str.GetAt(i);
		if (ch < 0)
		{
			bIsDigit = false;
			break;// 退出
		}
		if ( 0 == isdigit(ch) && '.' != ch  && 'e' != ch && 'E' != ch && '+' != ch && '-' != ch) // 不是数字和小数点就置标志位
		{
			bIsDigit = false;
			break;// 退出
		}
	}
	return bIsDigit;

} 

void findNeighborhood(CThreeDR* src3dr, double x, double y, int winSize, vector<float>& neighbors)
{
	
	double xmin = src3dr->getXmin();
	double ymin = src3dr->getYmin();
	int nRows = src3dr->getNumberOfRows();
	int nCols = src3dr->getNumberOfColumns();
	double cellsize = src3dr->getCellSize();
	float nodataValue = src3dr->getNullDataValue();

	// row and column number in the point (x, y)
	int rowCentor = nRows - (y - ymin) / cellsize - 1; 
	int colCentor = (x - xmin) / cellsize;

	if (rowCentor < 0 || rowCentor > src3dr->getNumberOfRows() - 1 
		|| colCentor < 0 || colCentor > src3dr->getNumberOfColumns() - 1)
		return;

	if(abs(src3dr->getDataBuf()[rowCentor*nCols + colCentor] + 9999) < CBRZERO)
		return;

	int offset = winSize / 2;
	int rowBegin = (rowCentor - offset > 0) ? (rowCentor - offset) : 0;
	int rowEnd = (rowCentor + offset < nRows) ? (rowCentor + offset) : (nRows - 1);
	int colBegin = (colCentor - offset > 0) ? (colCentor - offset) : 0;
	int colEnd = (colCentor + offset < nCols) ? (colCentor + offset) : (nCols - 1);

	for (int i = rowBegin; i <= rowEnd; ++i)
	{
		for (int j = colBegin; j <= colEnd; ++j)
		{
			float curVal = src3dr->getDataBuf()[i*nCols + j];
			if (abs(curVal - nodataValue) > CBRZERO )
				neighbors.push_back(curVal);
		}
	}
}

CString getCoreFilename(CString fullFileName)
{
	CString coreFileName;
	if (fullFileName.Find("\\") != -1)
		coreFileName = fullFileName.Mid(fullFileName.ReverseFind('\\')+1,
		fullFileName.ReverseFind('.') - fullFileName.ReverseFind('\\')-1);
	else
		coreFileName = fullFileName.Mid(fullFileName.ReverseFind('/')+1,
		fullFileName.ReverseFind('.') - fullFileName.ReverseFind('/')-1);
	return coreFileName;
}

CString getCurrentPath()
{
	CString path;  
	GetModuleFileName(NULL,path.GetBufferSetLength(261),260);   
	path.ReleaseBuffer();  
	int pos = path.ReverseFind('\\');  
	path=path.Left(pos);   
	return path;
}

int cbrDeleteDir(char* sDirName)
{
	CFileFind tempFind; 
	char sTempFileFind[200] ;

	sprintf(sTempFileFind,"%s\\*.*",sDirName); 
	BOOL IsFinded = tempFind.FindFile(sTempFileFind); 
	while (IsFinded) 
	{ 
		IsFinded = tempFind.FindNextFile(); 

		if (!tempFind.IsDots()) 
		{ 
			char sFoundFileName[200]; 
			strcpy(sFoundFileName,tempFind.GetFileName().GetBuffer(200)); 

			if (tempFind.IsDirectory()) 
			{ 
				char sTempDir[200]; 
				sprintf(sTempDir,"%s\\%s",sDirName,sFoundFileName); 
				cbrDeleteDir(sTempDir); 
			} 
			else 
			{ 
				char sTempFileName[200]; 
				sprintf(sTempFileName,"%s\\%s",sDirName,sFoundFileName); 
				DeleteFile(sTempFileName); 
			} 
		} 
	} 
	tempFind.Close(); 
	if(!RemoveDirectory(sDirName)) 
	{ 
		return FALSE; 
	} 
	return TRUE; 
}
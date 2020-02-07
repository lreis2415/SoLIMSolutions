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
// CBRGenerateFileDlg.cpp : 实现文件
//
#include <fstream>
#include <iomanip>
#include "stdafx.h"
#include "SoLIM.h"
#include "CBRGenerateFileDlg.h"
#include "ThreeDR.h"
#include "CBRUtility.h"
#include "distribution.h"
#include ".\cbrgeneratefiledlg.h"
#include "CBRSmallScaleDlg.h"
#include "CBRSampleDlg.h"

using namespace std;

// CBRGenerateFileDlg 对话框

IMPLEMENT_DYNAMIC(CBRGenerateFileDlg, CDialog)
CBRGenerateFileDlg::CBRGenerateFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBRGenerateFileDlg::IDD, pParent), biggestRes(-1), biggestResIndex(0)
	, resValue(_T(""))
	, radioSingle(TRUE)
	, radioKde(FALSE)
	, inputCount(0)
	, process(0)
{
	outputFolder = getCurrentPath() + "\\characterization";
	//testOf.open("F:\\bandwidth.txt");
}

CBRGenerateFileDlg::~CBRGenerateFileDlg()
{
	unsigned int i = 0;
	for(i = 0; i < climateDataVec.size(); ++i)
		delete climateDataVec[i];
	for(i = 0; i < materialDataVec.size(); ++i)
		delete materialDataVec[i];
	for(i = 0; i < topoDataVec.size(); ++i)
		delete topoDataVec[i];
	for(i = 0; i < vegDataVec.size(); ++i)
		delete vegDataVec[i];
	//testOf.close();
}

void CBRGenerateFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_GRIDSIZE, comboGridSize);
	DDX_Control(pDX, IDC_PROGRESS_GENERATE, processCtrl);
	DDX_Control(pDX, IDC_STATIC_STATUS, statusCtrl);
	DDX_CBString(pDX, IDC_COMBO_GRIDSIZE, resValue);
	DDX_Radio(pDX, IDC_RADIO_SINGLE_VALUE, radioSingle);
	DDX_Radio(pDX, IDC_RADIO_KDE, radioKde);
	DDX_Control(pDX, IDC_RADIO_SINGLE_VALUE, radioSingleCtrl);
	DDX_Control(pDX, IDC_RADIO_KDE, radioKdeCtrl);
	DDX_Control(pDX, IDC_RADIO_SINGLE_VALUE_VEGETATION, radioSingleCtrlVeg);
	DDX_Control(pDX, IDC_RADIO_KDE_VEGETATION, radioKdeCtrlVeg);
}


BEGIN_MESSAGE_MAP(CBRGenerateFileDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_GRIDSIZE, OnCbnSelchangeComboGridsize)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_SINGLE_VALUE, OnBnClickedRadioSingleValue)
	ON_BN_CLICKED(IDC_RADIO_KDE, OnBnClickedRadioKde)
	ON_BN_CLICKED(IDC_RADIO_SINGLE_VALUE_VEGETATION, OnBnClickedRadioSingleValueVegetation)
	ON_BN_CLICKED(IDC_RADIO_KDE_VEGETATION, OnBnClickedRadioKdeVegetation)
	ON_BN_CLICKED(ID_BACK, OnBnClickedBack)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CBRGenerateFileDlg 消息处理程序


BOOL CBRGenerateFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//init();

	CSoLIMApp* pApp=(CSoLIMApp*)AfxGetApp();	
	pApp->g_useKde = false;
	radioKdeCtrl.SetCheck(FALSE);
	radioKdeCtrlVeg.SetCheck(FALSE);
	radioSingleCtrl.SetCheck(TRUE);
	radioSingleCtrlVeg.SetCheck(TRUE);

	return true;
}

void CBRGenerateFileDlg::init()
{
	CSoLIMApp* pApp=(CSoLIMApp*)AfxGetApp();	

	while( comboGridSize.GetCount() > 0)
		comboGridSize.DeleteString(0);

	int curIndex = 0;
	biggestRes = -1;
	curIndex = readThreeDRData("climate", curIndex, pApp->g_vecstrClimateLayers, climateDataVec);
	curIndex = readThreeDRData("geology", curIndex, pApp->g_vecstrParentMaterialLayers, materialDataVec);
	curIndex = readThreeDRData("topology", curIndex, pApp->g_vecstrTopographicLayers, topoDataVec);
	curIndex = readThreeDRData("vegetation", curIndex, pApp->g_vecstrVegetationLayers, vegDataVec);

	inputCount = curIndex;

	comboGridSize.SetCurSel(biggestResIndex);
}

int CBRGenerateFileDlg::testKDE(void)
{
	CDistribution kde;
	float x[121] = {6.67355 , 6.8544 , 6.99365 , 7.00764 , 6.70375 , 6.6681 , 7.01342 , 7.14464 , 7.39674 , 12.9625 , 13.1004 , 7.03679 , 6.72883 , 6.64648 , 6.76294 , 7.2721 , 12.7083 , 13.4713 , 13.2439 , 12.8707 , 10.543 , 7.10651 , 6.35413 , 6.6729 , 11.7366 , 13.4474 , 13.2262 , 11.6038 , 7.17042 , 6.83722 , 6.8547 , 7.04752 , 7.10776 , 12.0509 , 13.4844 , 12.4588 , 7.28808 , 6.89272 , 6.9234 , 6.84196 , 6.84953 , 6.901 , 7.00341 , 6.80954 , 12.2811 , 7.68397 , 6.81235 , 6.69733 , 6.43086 , 6.47655 , 6.61357 , 6.58866 , 6.57707 , 6.60965 , 6.50531 , 7.73044 , 7.1666 , 6.89816 , 6.37784 , 6.22698 , 6.27466 , 6.63238 , 6.72589 , 6.70027 , 6.42348 , 7.07647 , 7.48127 , 7.08588 , 6.83548 , 6.41545 , 6.50012 , 6.68621 , 7.18383 , 7.17106 , 7.15143 , 7.46758 , 13.1737 , 7.38752 , 7.14703 , 6.65678 , 6.43666 , 6.82878 , 8.06978 , 8.50092 , 8.16694 , 7.93804 , 12.2326 , 10.2878 , 6.97364 , 7.02099 , 6.57917 , 6.63524 , 7.77297 , 8.39446 , 8.01314 , 7.82639 , 8.36963 , 12.1016 , 6.75144 , 6.34478 , 6.47598 , 6.37482 , 7.00125 , 7.61035 , 7.16629 , 6.29756 , 6.92114 , 12.304 , 6.52306 , 6.03155 , 5.76745 , 6.05606 , 6.47895 , 7.04292 , 6.70587 , 6.25726 , 6.42766 , 9.8185 , 11.6671 , 5.94428 , 5.99804};
	kde.SetValue(121, x);
	kde.setBandWidth(0.250834);
	kde.EstimatePD();

	ofstream output("plot.py");
	output << "from pylab import *" << endl;
	output << "x = [" << endl;
	for( int i = 0; i < kde.getNumOfPDE(); i++ )
		output << kde.getRangeAt(i) << ", ";
	output << "]" << endl;

	output << "y = [" << endl;
	for( int i = 0; i < kde.getNumOfPDE(); i++ )
		output << kde.getPDVal(i) << ", ";
	output << "]" << endl;
	output << "plot(x,y)" << endl;
	output << "show()" << endl;
	output.close();
	return 0;
}

int CBRGenerateFileDlg::testThreeDR(void)
{
	CThreeDR threeDR;
	threeDR.readin("G:\\solim\\TestSoLIMPro\\GISData\\elevation.3dr", 0);
	int nRows = threeDR.getNumberOfRows();
	int nCols = threeDR.getNumberOfColumns();
	ofstream output("data.txt");
	for(int i = 0; i < nRows; ++i)
	{
		for (int j = 0; j < nCols; ++j)
		{
			float f = threeDR.getDataBuf()[i*nCols + j];
			output << threeDR.getDataBuf()[i*nCols + j] << "\t";
		}
		output << endl;
	}
	output.close();
	return 0;
}

int CBRGenerateFileDlg::calKDE(CThreeDR* src3dr, CThreeDR* dst3dr, char* outputfile)
{
	int dstNumRows = dst3dr->getNumberOfRows();
	int dstNumCols = dst3dr->getNumberOfColumns();
	double dstCellSize = dst3dr->getCellSize();
	double dstXMin = dst3dr->getXmin();
	double dstYMin = dst3dr->getYmin();
	double dstYMax = dstYMin + dstCellSize * dstNumRows;

	double srcCellSize = src3dr->getCellSize();
	int srcNumRows = src3dr->getNumberOfRows();
	int srcNumCols = src3dr->getNumberOfColumns();

	//calculate kde
	CDistribution kde;
	ofstream output(outputfile);
	output << "NumberOfPointsPerLine:\t" << kde.getNumOfPDE() << endl;
	//calculate the stdDev of the whole raster
	float* srcArray = src3dr->getDataBuf();
	int num = srcNumRows * srcNumCols;
	float stdDev = calcStdDev(srcArray, num, src3dr->getNullDataValue());
	int count = num;
	for(int i=0; i<num; i++)
	{
		if(abs(srcArray[i] - src3dr->getNullDataValue()) < CBRZERO)
			--count;
	}
	src3dr->calcStat();
	float test = src3dr->getDataStd();
	float bandWidth = (float)(1.06 * stdDev * pow((double)count, -1.0/5.0));
	//testOf << src3dr->getDataName() << "\t" << bandWidth << endl;
	kde.setBandWidth(bandWidth);

	output << "NumberOfColumns:\t" << dstNumCols << endl;
	output << "NumberOfRows:\t" << dstNumRows << endl;
	output << "Xmin:\t" << dstXMin << endl;
	output << "Ymin:\t" << dstYMin << endl;
	output << "CellSize:\t" << dstCellSize << endl;

	int winSize = (int)(dstRes / srcCellSize) + 1;
	for (int i = 0; i < dstNumRows; ++i)
	{
		for (int j = 0; j < dstNumCols; ++j)
		{
			double dstx = dstXMin + dstCellSize * (j + 0.5);
			double dsty = dstYMax - dstCellSize * (i + 0.5);
			//find the neigbourhood in the source data
			vector<float> neighbors;
			findNeighborhood(src3dr, dstx, dsty, winSize, neighbors);
			//if(i*dstNumCols + j == 494)
			//{
			//	ofstream testof("e:\\neighbor494.txt");
			//	for (int c = 0; c < neighbors.size(); ++c)
			//		testof << neighbors[c] << ",";
			//	testof.close();
			//}

			if (neighbors.size() > 0)
			{
				//ofstream testof("e:\\neighbor.txt");
				//for (int c = 0; c < neighbors.size(); ++c)
				//	testof << neighbors[c] << endl;
				//testof.close();

				int num = neighbors.size();
				float* pData = new float[num];
				for(int m = 0; m < num; ++m)
					pData[m] = neighbors[m];
				kde.SetValue(neighbors.size(), pData);
				kde.EstimatePD();
				for( int i = 0; i < kde.getNumOfPDE(); i++ )
					output << kde.getRangeAt(i) << "\t";
				output << endl;
				for( int i = 0; i < kde.getNumOfPDE(); i++ )
					output << kde.getPDVal(i) << "\t";
				output << endl;
				delete pData;
			}
			else
				output << endl << endl;
		}
	}
	output.close();

	return 0;
}
void CBRGenerateFileDlg::OnCbnSelchangeComboGridsize()
{
	// TODO: 在此添加控件通知处理程序代码
}

/////////////////////////////////////////////////////////////////////
/*****************************************************************
** 函数名:GetFilePath
** 输 入: 无
** 输 出: CString strPath
**        strPath非空, 表示用户选择的文件夹路径
**        strPath为空, 表示用户点击了“取消”键，取消选择
** 功能描述:显示“选择文件夹”对话框，让用户选择文件夹
****************************************************************/
CString CBRGenerateFileDlg::GetFilePath()
{
	CString strPath = "";
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.hwndOwner = NULL; //m_hWnd;
	bInfo.lpszTitle = _T("请选择路径: ");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;    

	LPITEMIDLIST lpDlist; //用来保存返回信息的IDList
	lpDlist = SHBrowseForFolder(&bInfo) ; //显示选择对话框
	if(lpDlist != NULL)  //用户按了确定按钮
	{
		TCHAR chPath[255]; //用来存储路径的字符串
		SHGetPathFromIDList(lpDlist, chPath); //把项目标识列表转化成字符串
		strPath = chPath; //将TCHAR类型的字符串转换为CString类型的字符串
	}
	return strPath;
}

void CBRGenerateFileDlg::OnBnClickedOk()
{
	UpdateData(true);
	//删除文件夹 
	cbrDeleteDir(outputFolder.GetBuffer());
	CreateDirectory(outputFolder, NULL);
	
	if (isNumeric(resValue) )
		dstRes = (float)atof(resValue);
	else
		dstRes = biggestRes;

	CSoLIMApp* pApp = (CSoLIMApp*)AfxGetApp();
	pApp->g_dstResolution = dstRes;

	pApp->g_vecClimate.clear();
	pApp->g_vecParentMaterial.clear();
	pApp->g_vecTopology.clear();
	pApp->g_vecVegetation.clear();

	processCtrl.SetRange(0, 100);
	process = 0;
	if (!pApp->g_vecstrClimateLayers.empty())
		generateNew3dr("climate", pApp->g_vecstrClimateLayers, climateDataVec, pApp->g_vecClimate);

	if (!pApp->g_vecstrParentMaterialLayers.empty())
		generateNew3dr("geology", pApp->g_vecstrParentMaterialLayers, materialDataVec, 
		pApp->g_vecParentMaterial);
	
	//generate pdf curve using kde
	if (!pApp->g_vecstrTopographicLayers.empty())
	{
		statusCtrl.SetWindowText("Generating topology files......");
		UpdateData(false);

		//if not use kde, resample the data
		if (!pApp->g_useKde)
			generateNew3dr("topology", pApp->g_vecstrTopographicLayers, topoDataVec, 
			pApp->g_vecTopology);
		//if use kde, generate the kde curve
		else
		{
			CString dic;
			dic.Format("%s\\%s", outputFolder.GetString(), "topology");
			CreateDirectory(dic.GetString(), NULL);
			CString outputfile;
			CThreeDR* dst3dr = new CThreeDR();   //the file used to determine the center of cells in the destination raster
			dst3dr->readin(pApp->g_vecClimate[0].GetBuffer(), 0);
			for (unsigned int i = 0; i < pApp->g_vecstrTopographicLayers.size(); ++i)
			{
				process += int(100 / inputCount);
				processCtrl.SetPos(process);
				UpdateData(FALSE);

				CString dataname = topoDataVec[i]->getDataName();
				if (dataname.Right(4).CompareNoCase(".3dr") == 0)
				{
					dataname = dataname.Left(dataname.GetLength()-4);
				}
				outputfile.Format("%s\\%s.txt", dic, dataname);

				calKDE(topoDataVec[i], dst3dr, (char*)outputfile.GetString());
				pApp->g_vecTopology.push_back(outputfile);
			}
			delete dst3dr;
		}
	}


	if (!pApp->g_vecstrVegetationLayers.empty())
	{
		statusCtrl.SetWindowText("Generating vegetation files......");
		UpdateData(false);

		if(!pApp->g_useKdeVegetation)
			generateNew3dr("vegetation", pApp->g_vecstrVegetationLayers, vegDataVec, 
			pApp->g_vecVegetation);
		else
		{
			CString dic;
			dic.Format("%s\\%s", outputFolder.GetString(), "vegetation");
			CreateDirectory(dic.GetString(), NULL);
			CString outputfile;
			CThreeDR* dst3dr = new CThreeDR();   //the file used to determine the center of cells in the destination raster
			dst3dr->readin(pApp->g_vecClimate[0].GetBuffer(), 0);
			for (unsigned int i = 0; i < pApp->g_vecstrVegetationLayers.size(); ++i)
			{
				process += int(100 / inputCount);;
				processCtrl.SetPos(process);
				UpdateData(FALSE);

				CString dataname = vegDataVec[i]->getDataName();
				if (dataname.Right(4).CompareNoCase(".3dr") == 0)
				{
					dataname = dataname.Left(dataname.GetLength()-4);
				}
				outputfile.Format("%s\\%s.txt", dic, dataname);

				calKDE(vegDataVec[i], dst3dr, (char*)outputfile.GetString());
				pApp->g_vecVegetation.push_back(outputfile);
			}
			delete dst3dr;
		}
	}

	processCtrl.SetPos(100);
	UpdateData(FALSE);

	//OnOK();

	this->ShowWindow(SW_HIDE);
	if (!pApp->pSampleDlg)
	{
		pApp->pSampleDlg = new CBRSampleDlg();
		pApp->pSampleDlg->Create(IDD_CBR_SAMPLE, NULL);
	}
	pApp->pSampleDlg->ShowWindow(SW_SHOW);
}

void CBRGenerateFileDlg::saveFileName(vector<CString> vecFileName, CString outputfile)
{
	ofstream testof(outputfile.GetBuffer());
	for (unsigned int i = 0; i < vecFileName.size(); ++i)
		testof << vecFileName[i] << endl;
	testof.close();
}

int CBRGenerateFileDlg::readThreeDRData(char* type, int curIndex, vector<CString>& inputfileVec, vector<CThreeDR*>& threeDRVec)
{
	for (int i = 0; i < threeDRVec.size(); ++i)
	{
		delete threeDRVec[i];
	}
	threeDRVec.clear();

	CString tmp;
	//read climate data
	for (unsigned int i = 0; i < inputfileVec.size(); ++i)
	{
		CThreeDR* threeDR = new CThreeDR();
		threeDR->readin((char*)inputfileVec[i].GetString(), 0);
		threeDRVec.push_back(threeDR);
		if (threeDR->getCellSize() > biggestRes)
		{
			biggestRes = (float)threeDR->getCellSize();
			biggestResIndex = curIndex;
		}
		tmp.Format("%s: %s  resolution: %f", type, threeDR->getDataName(), threeDR->getCellSize());
		comboGridSize.AddString(tmp);
		++curIndex;
	}
	return curIndex;
}

bool CBRGenerateFileDlg::generateNew3dr(char* type, vector<CString>& inputfileVec, vector<CThreeDR*>& threeDRVec, 
										vector<CString>& g_vecOutput)
{
	CString status;
	status.Format("Generating %s files......", type);
	statusCtrl.SetWindowText(status);
	UpdateData(false);

	CString dic;
	dic.Format("%s\\%s", outputFolder.GetString(), type);
	CreateDirectory(dic.GetString(), NULL);

	for (unsigned int i = 0; i < threeDRVec.size(); ++i)
	{

		process += int(100 / inputCount);
		processCtrl.SetPos(process);
		UpdateData(FALSE);

		CString outputfile;
		outputfile.Format("%s\\%s\\%s", outputFolder.GetString(), type, threeDRVec[i]->getDataName());
		if (outputfile.Right(4).CompareNoCase(".3dr"))
			outputfile.Append(".3dr");
		LPCTSTR input = (LPCTSTR)inputfileVec[i];
		
		//if the current data's resolution is as same as the destination resolution,
		//simply copy the file to the destination folder
		if (abs(threeDRVec[i]->getCellSize() - dstRes) < RESZERO)
		{
			CopyFile(input, outputfile, 0);
			g_vecOutput.push_back(outputfile);
			continue;
		}

		//resample
		resampleThreeDR((char*)input, dstRes, (char*)outputfile.GetString());

		g_vecOutput.push_back(outputfile);
	}
	return true;
}


void CBRGenerateFileDlg::OnBnClickedRadioSingleValue()
{
	radioKdeCtrl.SetCheck(FALSE);
	radioSingleCtrl.SetCheck(TRUE);
	UpdateData();
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	if (radioKdeCtrl.GetCheck())
		pApp->g_useKde = true;
	else
		pApp->g_useKde = false;
}

void CBRGenerateFileDlg::OnBnClickedRadioKde()
{
	radioKdeCtrl.SetCheck(TRUE);
	radioSingleCtrl.SetCheck(FALSE);
	
	UpdateData();
	
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	if (radioKdeCtrl.GetCheck())
		pApp->g_useKde = true;
	else
		pApp->g_useKde = false;
}

void CBRGenerateFileDlg::OnBnClickedRadioSingleValueVegetation()
{
	radioKdeCtrlVeg.SetCheck(FALSE);
	radioSingleCtrlVeg.SetCheck(TRUE);
	UpdateData();

	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	if (radioKdeCtrlVeg.GetCheck())
		pApp->g_useKdeVegetation = true;
	else
		pApp->g_useKdeVegetation = false;
}

void CBRGenerateFileDlg::OnBnClickedRadioKdeVegetation()
{
	radioKdeCtrlVeg.SetCheck(TRUE);
	radioSingleCtrlVeg.SetCheck(FALSE);
	UpdateData();

	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	if (radioKdeCtrlVeg.GetCheck())
		pApp->g_useKdeVegetation = true;
	else
		pApp->g_useKdeVegetation = false;
}

void CBRGenerateFileDlg::OnBnClickedBack()
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	this->ShowWindow(SW_HIDE);
	pApp->pSmallScaleDlg->ShowWindow(SW_SHOW);
}

void CBRGenerateFileDlg::OnBnClickedCancel()
{
	OnCancel();
	CSoLIMApp* pApp = (CSoLIMApp*)AfxGetApp();
	if(pApp->pSmallScaleDlg)
	{
		delete pApp->pSmallScaleDlg;
		pApp->pSmallScaleDlg = NULL;
	}
	if(pApp->pGenerateFileDlg)
	{
		delete pApp->pGenerateFileDlg;
		pApp->pGenerateFileDlg = NULL;
	}
	if(pApp->pSampleDlg)
	{
		delete pApp->pSampleDlg;
		pApp->pSampleDlg = NULL;
	}
	if(pApp->pSimiDlg)
	{
		delete pApp->pSimiDlg;
		pApp->pSimiDlg = NULL;
	}
	if(pApp->pUncertaintyDlg)
	{
		delete pApp->pUncertaintyDlg;
		pApp->pUncertaintyDlg = NULL;
	}
		if(pApp->pTypeinferDlg)
	{
		delete pApp->pTypeinferDlg;
		pApp->pTypeinferDlg = NULL;
	}

	
}

void CBRGenerateFileDlg::resetProcess()
{
	processCtrl.SetPos(0);
}
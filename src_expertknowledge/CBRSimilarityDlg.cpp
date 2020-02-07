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
// CBRSimilarityDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SoLIM.h"
#include "CBRSimilarityDlg.h"
#include ".\cbrsimilaritydlg.h"

#include "ThreeDR.h"
#include "CBRSamplePoint.h"
#include <string>
#include <fstream>
#include <sstream>
#include <map>

// CBRSimilarityDlg 对话框

IMPLEMENT_DYNAMIC(CBRSimilarityDlg, CDialog)
CBRSimilarityDlg::CBRSimilarityDlg(CWnd* pParent /*=NULL*/)
: CDialog(CBRSimilarityDlg::IDD, pParent), 
	climateCut(0.5),
	climateWeight(0.5),
	topoWeight(0.5),
	varCount(0),
	vegWeight(0)
{
	outputFolder = getCurrentPath() + "\\similarity";
	curProcess = 0.0;
}

CBRSimilarityDlg::~CBRSimilarityDlg()
{
	for (int i = 0; i < climateVarVec.size(); i++)
		delete climateVarVec[i];

	for (int i = 0; i < parentVarVec.size(); i++)
		delete parentVarVec[i];

	for (int i = 0; i < topoVarVec.size(); i++)
		delete topoVarVec[i];

	for (int i = 0; i < vegVarVec.size(); ++i)
		delete vegVarVec[i];
}

void CBRSimilarityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_COMBO1, comboTest);
	DDX_Control(pDX, IDC_EDIT_CLIMATE_CUT, climateCutCtrl);
	DDX_Control(pDX, IDC_EDIT_CLIMATE_WEIGHT, climateWeightCtrl);
	DDX_Control(pDX, IDC_EDIT_TOPOLOGY_WEIGHT, topoWeightCtrl);
	DDX_Control(pDX, IDC_PROGRESS1, processCtrl);
	DDX_Text(pDX, IDC_EDIT_CLIMATE_CUT, climateCut);
	DDX_Text(pDX, IDC_EDIT_CLIMATE_WEIGHT, climateWeight);
	DDX_Text(pDX, IDC_EDIT_TOPOLOGY_WEIGHT, topoWeight);
	DDX_Control(pDX, IDC_COMBO_Climate, comboClimateVarCtrl);
	DDX_Control(pDX, IDC_COMBO_PM2, comboPmVarCtrl);
	DDX_Control(pDX, IDC_COMBO_TOPOLOGY, comboTopoVarCtrl);
	DDX_Control(pDX, IDC_COMBO_CASE, comboCaseMethod);
	DDX_Control(pDX, IDC_COMBO_VEGETATION, comboVegVarCtrl);
	DDX_Control(pDX, IDC_EDIT_VEGETAION_WEIGHT, vegWeightCtrl);
	DDX_Text(pDX, IDC_EDIT_VEGETAION_WEIGHT, vegWeight);
}


BEGIN_MESSAGE_MAP(CBRSimilarityDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedBtnRun)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO_CASE, OnCbnSelchangeComboCase)
	ON_BN_CLICKED(ID_BACK, OnBnClickedBack)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

//事件处理
void CBRSimilarityDlg::OnBnClickedBtnRun()
{
	UpdateData(TRUE);

	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CString strMethod;
	comboCaseMethod.GetWindowText(strMethod);
	if ( strMethod.CompareNoCase("Rules") == 0 && 
		(pApp->g_vecstrClimateLayers.empty() || pApp->g_vecstrParentMaterialLayers.empty() || pApp->g_vecstrTopographicLayers.empty()) )
	{
		MessageBox("The current rule needs climate, topological and parent material data,\nbut one or more of these data are not available.\nPlease select another method.");
		return;
	}

	cbrDeleteDir(outputFolder.GetBuffer());
	CreateDirectory(outputFolder, NULL);

	if(abs(climateWeight + topoWeight + vegWeight - 1) > CBRZERO )
	{
		MessageBox("The sum of three weights must be 1.");
		return;
	}

	
	varCount = 0;
	//if(!pApp->g_vecstrClimateLayers.empty())
	//	++varCount;
	//if(!pApp->g_vecstrParentMaterialLayers.empty())
	//	++varCount;
	if(!pApp->g_vecstrTopographicLayers.empty())
		++varCount;
	if(!pApp->g_vecstrVegetationLayers.empty())
		++varCount;

	//nProcess = 0;
	processCtrl.SetRange(0, 100);
	//generate sample structure
	getSampleVec(); 

	//CString str;
	//str.Format("char: %d\t%d\t%d\t%d\t", pApp->g_vecClimate.size(), pApp->g_vecParentMaterial.size(), 
	//	pApp->g_vecTopology.size(), pApp->g_vecVegetation.size());
	//MessageBox(str);

	//str.Format("%d", pApp->g_vecSamplePoint.size());
	//MessageBox(str);

	if(pApp->g_vecstrClimateLayers.size() > 0)
	{
		if( getVarVec(pApp->g_vecClimate, climateVarVec) == 1)
			return;
		calcGowerSimilarity("climate", climateVarVec, pApp->g_vecSamplePoint, climateSimiVec);
	}

	if(pApp->g_vecstrParentMaterialLayers.size() > 0)
	{
		if( getVarVec(pApp->g_vecParentMaterial, parentVarVec) )
			return;
		calcParentSimilarity(parentVarVec, pApp->g_vecSamplePoint);
	}

	if(pApp->g_vecstrTopographicLayers.size() > 0)
	{
		if(pApp->g_useKde)
		{
			calcKdeSimilarity("topology", pApp->g_vecTopology, pApp->g_vecSamplePoint, topoSimiVec);
		}
		else
		{
			if( getVarVec(pApp->g_vecTopology, topoVarVec) )
				return;
			calcGowerSimilarity("topology", topoVarVec, pApp->g_vecSamplePoint, topoSimiVec);
		}
	}


	if (pApp->g_vecstrVegetationLayers.size() > 0)
	{
		if(pApp->g_useKdeVegetation)
		{
			calcKdeSimilarity("vegetation", pApp->g_vecVegetation, pApp->g_vecSamplePoint, vegSimiVec);
		}
		else
		{
			if( getVarVec(pApp->g_vecVegetation, vegVarVec) == 1)
				return;
			calcGowerSimilarity("vegetation", vegVarVec, pApp->g_vecSamplePoint, vegSimiVec);
		}
	}

	calcIntergratedSimilarity(comboCaseMethod.GetCurSel());

	//OnOK();
	this->ShowWindow(SW_HIDE);

	if(pApp->iCBROption == 1)
	{	
	  if (!pApp->pUncertaintyDlg)
	  {
		pApp->pUncertaintyDlg = new CBRUncertaintyDlg();
		pApp->pUncertaintyDlg->Create(IDD_CBR_Uncertainty, NULL);
	  }
	  pApp->pUncertaintyDlg->ShowWindow(SW_SHOW);
	}
	else
	{
	    if (!pApp->pTypeinferDlg)
	  {
		pApp->pTypeinferDlg = new CBRTypeInferenceDlg();
		pApp->pTypeinferDlg->Create(IDD_CBR_TYPE_INFER, NULL);
	  }
	  pApp->pTypeinferDlg->ShowWindow(SW_SHOW);
	}
	
}

void CBRSimilarityDlg::getAttrNameVector(ifstream& fIn, vector<string>& attrNameVec)
{
	string tmp;
	int numVal;
	fIn >> tmp >> numVal;
	fIn >> tmp >> tmp >> tmp; 
	attrNameVec.clear();
	attrNameVec.resize(numVal);
	for (int i = 0; i < numVal; ++i)
	{
		fIn >> tmp;
		attrNameVec[i] = (tmp);
	}
}

void CBRSimilarityDlg::getSampleVec()
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();

	ifstream attrIn(pApp->g_attrSample);
	int sampleCount;
	int id;
	double x, y;
	string tmp;
	int attrCount;
	float fTmp;
	getline(attrIn, tmp);
	attrIn >> tmp >> attrCount;
	CBRSamplePoint::valuesNameVec.clear();
	for(int i = 0; i < attrCount; ++i)
	{
		attrIn >> tmp;
		CBRSamplePoint::addAttrName(tmp);
	}

	//loop to construct samples
	for (int i = 0; i < pApp->g_vecSamplePoint.size(); ++i)
		delete pApp->g_vecSamplePoint[i];
	pApp->g_vecSamplePoint.clear();
	//topological data is necessary by default
	if(!pApp->g_vecstrTopographicLayers.empty())
	{
		ifstream topoSampleIn;
		if(!pApp->g_useKde)
		{
			topoSampleIn.open(pApp->g_topoSample);
			getAttrNameVector(topoSampleIn, CBRSamplePoint::topoNameVec);
			int topoAttrSize = CBRSamplePoint::topoNameVec.size();
			topoSampleIn >> tmp >> sampleCount;

			for (int i = 0; i < sampleCount; ++i)
			{
				CBRSamplePoint* pSample = new CBRSamplePoint();
				attrIn >> tmp;
				for(int j = 0; j < attrCount; ++j)
				{
					attrIn >> fTmp;
					pSample->addAttrVar(fTmp);
				}
				
				topoSampleIn >> id >> x >> y;
				pSample->setid(id);
				pSample->setX(x);
				pSample->setY(y);
				
				for (int j = 0; j < topoAttrSize; ++j)
				{
					topoSampleIn >> fTmp;
					pSample->addTopoVarNoKde(fTmp);
				}
				pApp->g_vecSamplePoint.push_back(pSample);
			}
			topoSampleIn.close();
		}
		else
		{
			//topofiles instream
			string lineTmp;
			//change to global virable
			string tmp1, tmp2;
			CBRSamplePoint::topoNameVec.clear();
			
			for(int i = 0; i < pApp->g_vecTopologySample.size(); ++i)
			{
				int numPoints;
				string topoName = pApp->g_vecTopologySample[i].GetBuffer();
				ifstream topoIn(pApp->g_vecTopologySample[i].GetBuffer());

				//read header
				getline(topoIn, lineTmp);
				stringstream nameParser(lineTmp);
				nameParser >> tmp;
				CBRSamplePoint::addTopoName(tmp);

				getline(topoIn, lineTmp);
				stringstream ptParser(lineTmp);
				ptParser >> tmp >> numPoints;
				
				getline(topoIn, lineTmp);
				stringstream ssParser(lineTmp);
				ssParser >> tmp >> sampleCount;
				
				getline(topoIn, lineTmp);

				for (int j = 0; j < sampleCount; ++j)
				{
					getline(topoIn, lineTmp);
					stringstream parser(lineTmp);
					parser >> id >> x >> y;
					getline(topoIn, tmp1);
					stringstream parser1(tmp1);
					getline(topoIn, tmp2);
					stringstream parser2(tmp2);
					
					if (i == 0)
					{
						CBRSamplePoint* pSample = new CBRSamplePoint();
						attrIn >> tmp;
						for(int j = 0; j < attrCount; ++j)
						{
							attrIn >> fTmp;
							pSample->addAttrVar(fTmp);
						}
						pSample->setid(id);
						pSample->setX(x);
						pSample->setY(y);
						pApp->g_vecSamplePoint.push_back(pSample);
					}

					vector<Point>* pVecPt = new vector<Point>();
					pVecPt->resize(numPoints);
					for (int m = 0; m < numPoints; ++m)
					{
						parser1 >> x;
						parser2 >> y;
						Point pt;
						pt.x = x;
						pt.y = y;
						(*pVecPt)[m] = pt;
					}
					pApp->g_vecSamplePoint[j]->addTopoVar(pVecPt);
				}
				topoIn.close();
			}
		}
	}

	//get climate attribute name
	if(!pApp->g_vecstrClimateLayers.empty())
	{	
		ifstream climateSampleIn(pApp->g_climateSample);
		getAttrNameVector(climateSampleIn, CBRSamplePoint::climateNameVec);
		int climateAttrSize = CBRSamplePoint::climateNameVec.size();
		climateSampleIn >> tmp >> tmp;
		for (int i = 0; i < sampleCount; ++i)
		{
			CBRSamplePoint* pSample = pApp->g_vecSamplePoint[i];
			climateSampleIn >> id >> x >> y;

			for (int j = 0; j < climateAttrSize; ++j)
			{
				climateSampleIn >> fTmp;
				pSample->addClimateVar(fTmp);
			}
		}
		climateSampleIn.close();
	}
	
	//get parent material attribute name
	if(!pApp->g_vecstrParentMaterialLayers.empty())
	{	
		ifstream parentMaterialSampleIn(pApp->g_parentMaterialSample);
		getAttrNameVector(parentMaterialSampleIn, CBRSamplePoint::parentNameVec);
		int pmAttrSize = CBRSamplePoint::parentNameVec.size();
		parentMaterialSampleIn >> tmp >> tmp;
		for (int i = 0; i < sampleCount; ++i)
		{
			CBRSamplePoint* pSample = pApp->g_vecSamplePoint[i];
			parentMaterialSampleIn >> id >> x >> y;

			for (int j = 0; j < pmAttrSize; ++j)
			{
				parentMaterialSampleIn >> fTmp;
				pSample->addParentVar(fTmp);
			}
		}
		parentMaterialSampleIn.close();
	}

	if (!pApp->g_vecstrVegetationLayers.empty())
	{
		ifstream vegSampleIn;
		if(!pApp->g_useKdeVegetation)
		{
			vegSampleIn.open(pApp->g_vegSample);
			getAttrNameVector(vegSampleIn, CBRSamplePoint::vegNameVec);
			int vegAttrSize = CBRSamplePoint::vegNameVec.size();
			vegSampleIn >> tmp >> tmp;
			for (int i = 0; i < sampleCount; ++i)
			{
				CBRSamplePoint* pSample = pApp->g_vecSamplePoint[i];
				vegSampleIn >> id >> x >> y;
				for (int j = 0; j < vegAttrSize; ++j)
				{
					vegSampleIn >> fTmp;
					pSample->addVegVarNoKde(fTmp);
				}
			}
			vegSampleIn.close();
		}
		else
		{
			//topofiles instream
			string lineTmp;
			//change to global virable
			string tmp1, tmp2;
			CBRSamplePoint::vegNameVec.clear();
			for(int i = 0; i < pApp->g_vecVegetationSample.size(); ++i)
			{
				int numPoints;
				string vegName = pApp->g_vecVegetationSample[i].GetBuffer();
				ifstream vegIn(pApp->g_vecVegetationSample[i].GetBuffer());

				//read header
				getline(vegIn, lineTmp);
				stringstream nameParser(lineTmp);
				nameParser >> tmp;
				CBRSamplePoint::addVegName(tmp);

				getline(vegIn, lineTmp);
				stringstream ptParser(lineTmp);
				ptParser >> tmp >> numPoints;

				getline(vegIn, lineTmp);
				getline(vegIn, lineTmp);

				for (int j = 0; j < sampleCount; ++j)
				{
					getline(vegIn, lineTmp);
					stringstream parser(lineTmp);
					parser >> id >> x >> y;
					getline(vegIn, tmp1);
					stringstream parser1(tmp1);
					getline(vegIn, tmp2);
					stringstream parser2(tmp2);

					vector<Point>* pVecPt = new vector<Point>();
					pVecPt->resize(numPoints);
					for (int m = 0; m < numPoints; ++m)
					{
						parser1 >> x;
						parser2 >> y;
						Point pt;
						pt.x = x;
						pt.y = y;
						(*pVecPt)[m] = pt;
					}
					pApp->g_vecSamplePoint[j]->addVegVar(pVecPt);
				}
				vegIn.close();
			}
		}
	}
}
int CBRSimilarityDlg::getVarVec(vector<CString>& filenames, vector<CThreeDR*>& vec3dr)
{	
	for (int i = 0; i < vec3dr.size(); ++i)
		delete vec3dr[i];
	vec3dr.clear();

	for (int i =0; i < filenames.size(); i++)
	{
		CThreeDR* threedr = new CThreeDR;
		int readFlag = threedr->readin(filenames[i].GetBuffer(1), 0);
		if(readFlag != 0)
		{
			CString str;
			str.Format("Error happens when reading the characterization files.\nPlease return to the last step and regenerate characterization files.");
			MessageBox(str);
			this->processCtrl.SetPos(0);
			return 1;
		}
		vec3dr.push_back(threedr);
	}
	return 0;
}

int CBRSimilarityDlg::calcGowerSimilarity(CString type, vector<CThreeDR*>& vec3dr, vector<CBRSamplePoint*>& in_sampleVec, vector<CString>& vecOuputSimi){
	//获取基本信息
	int nFiles = vec3dr.size();
	if (nFiles == 0)
		return 1;

	int nSample = in_sampleVec.size();
	CThreeDR* threedr = vec3dr[0];
	int nrow = threedr->getNumberOfRows();
	int ncol = threedr->getNumberOfColumns();

	float* dataBuf;
	float* simi;
	float sum, max, min, range, nodataCount;
	vecOuputSimi.clear();
	for (int i = 0; i < nSample; i++)
	{
		curProcess += (double)20 / (double)nSample;
		processCtrl.SetPos((int)curProcess);

		CBRSamplePoint* tempSample = in_sampleVec[i];
		int nAll = nrow * ncol;
		simi = new float[nAll];
		for (int j = 0; j < nAll; j++)
		{
			sum = 0;
			nodataCount = 0;
			//the sequence of the vec3dr and the attributes in the sample file should be the same
			for (int k = 0; k < nFiles; k++)
			{
				dataBuf = vec3dr[k]->getDataBuf();
				float t = dataBuf[j];
				if( abs(dataBuf[j] - vec3dr[k]->getNullDataValue()) < CBRZERO )
				{
					++nodataCount;
					continue;
				}
				max = vec3dr[k]->getDataMax(); 
				min = vec3dr[k]->getDataMin();
				range = max - min;
				double x = dataBuf[j];
				double y = 0;
				if(type == "climate")
					y = tempSample->getClimateVariable(k);
				else if(type == "topology")
					y = tempSample->getTopoVariableNoKde(k);
				else if(type == "vegetation")
					y = tempSample->getVegVariableNoKde(k);
				else
				{
					MessageBox("In function calcGowerSimilarity: type parameter has an invalid input.");
					return 1;
				}
				sum = sum + (1 - abs(dataBuf[j] - y) / range);
			}
			if (nodataCount == 0)
				simi[j] = sum / nFiles;
			else
				simi[j] = -9999;
		}	

		CThreeDR* output_threedr = new CThreeDR;
		output_threedr->copyBasicFields(threedr);
		output_threedr->setDataBuf(simi);
		output_threedr->calcStat();
		output_threedr->setXmin(threedr->getXmin());
		output_threedr->setYmin(threedr->getYmin());

		//output
		CString dic;
		dic.Format("%s\\%s", outputFolder.GetString(), type);
		CreateDirectory(dic.GetString(), NULL);

		CString fullfilename;
		fullfilename.Format("%s\\simi%d.3dr", dic, i+1);
		output_threedr->setDataName(extractFileName(fullfilename).GetBuffer(1));
		output_threedr->writeout(fullfilename.GetBuffer(1), 0);
		delete output_threedr;

		//CString asciiFilename = fullfilename.Left(fullfilename.GetLength()-4) + ".txt";
		//threeDR2Ascii(fullfilename.GetBuffer(1), asciiFilename.GetBuffer(1));

		//========================================================================
		vecOuputSimi.push_back(fullfilename);
	}

	return 0;
}

int CBRSimilarityDlg::calcParentSimilarity(vector<CThreeDR*>& in_parentVarVec, vector<CBRSamplePoint*>& in_sampleVec)
{	
	//获取基本信息
	int nParentVar = in_parentVarVec.size();
	if (nParentVar == 0)
		return 1;
	int nSample = in_sampleVec.size();
	CThreeDR* ParentVar1 = in_parentVarVec[0];
	int nrow = ParentVar1->getNumberOfRows();
	int ncol = ParentVar1->getNumberOfColumns();

	float* Parent_DataBuf;
	float* parentsimi_tempsample;

	parentSimiVec.clear();
	for (int j = 0; j < nSample; j++)
	{
		curProcess += (double)20 / (double)nSample;
		processCtrl.SetPos((int)curProcess);

		CBRSamplePoint* tempSample = in_sampleVec[j];
		
		parentsimi_tempsample = new float[nrow * ncol];

		for (int i = 0; i < nParentVar; i++)
		{	
			float tempsamplevalue = tempSample->getParentVariable(i);

			CThreeDR* tempParentVar = in_parentVarVec[i];
			Parent_DataBuf = tempParentVar->getDataBuf();
			for (int k = 0; k < nrow * ncol; k++)
			{
				if(abs(Parent_DataBuf[k] - tempsamplevalue) < CBRZERO)
					parentsimi_tempsample[k] = 1.0;
				else if(abs(Parent_DataBuf[k] + 9999) < CBRZERO)
					parentsimi_tempsample[k] = -9999;
				else
					parentsimi_tempsample[k] = 0.0;
			}
		}
		CThreeDR* parentsimi_tempsample_threedr = new CThreeDR;
		parentsimi_tempsample_threedr->copyBasicFields(ParentVar1);
		parentsimi_tempsample_threedr->setDataBuf(parentsimi_tempsample);
	
		parentsimi_tempsample_threedr->calcStat();
		parentsimi_tempsample_threedr->setXmin(parentsimi_tempsample_threedr->getXmin());
		parentsimi_tempsample_threedr->setYmin(parentsimi_tempsample_threedr->getYmin());

		//output
		CString dic;
		dic.Format("%s\\%s", outputFolder.GetString(), "geology");
		CreateDirectory(dic.GetString(), NULL);
		CString fullfilename;
		fullfilename.Format("%s\\simi%d.3dr", dic, j+1);
	    parentsimi_tempsample_threedr->setDataName(extractFileName(fullfilename).GetBuffer(1));
		parentsimi_tempsample_threedr->writeout(fullfilename.GetBuffer(1), 0);
		delete parentsimi_tempsample_threedr;
		//CString asciiFilename = fullfilename.Left(fullfilename.GetLength()-4) + ".txt";
		//threeDR2Ascii(fullfilename.GetBuffer(1), asciiFilename.GetBuffer(1));
		
		//========================================================================

		parentSimiVec.push_back(fullfilename);
	}	
	return 0;
}
// i is the index of sample, j is the index of cells
void CBRSimilarityDlg::averIntegrate(float* integratedsimi_tempsample, int i, int j, 
									 vector<CThreeDR*>& in_climateSimiVec, vector<CThreeDR*>& in_parentSimiVec, 
									 vector<CThreeDR*>& in_topoSimiVec, vector<CThreeDR*>& in_vegSimiVec)
{
	integratedsimi_tempsample[j] = 0;
	int counter = 0;
	if(i < in_topoSimiVec.size())
	{
		float* data = in_topoSimiVec[i]->getDataBuf();
		if(abs(data[j] + 9999) < CBRZERO)
		{
			integratedsimi_tempsample[j] = -9999;
			return;
		}
		integratedsimi_tempsample[j] += data[j];
		++counter;
	}


	if(i < in_parentSimiVec.size())
	{
		float* data = in_parentSimiVec[i]->getDataBuf();
		if(abs(data[j] + 9999) < CBRZERO)
		{
			integratedsimi_tempsample[j] = -9999;
			return;
		}
		else if(data[j] < CBRZERO)
		{
			integratedsimi_tempsample[j] = 0.0;
			return;
		}
	}

	if(i < in_climateSimiVec.size())
	{
		float* data = in_climateSimiVec[i]->getDataBuf();
		if(abs(data[j] + 9999) < CBRZERO)
		{
			integratedsimi_tempsample[j] = -9999;
			return;
		}
		integratedsimi_tempsample[j] += data[j];
		++counter;
	}

	if(i < in_vegSimiVec.size())
	{
		float* data = in_vegSimiVec[i]->getDataBuf();
		if(abs(data[j] + 9999) < CBRZERO)
		{
			integratedsimi_tempsample[j] = -9999;
			return;
		}
		integratedsimi_tempsample[j] += data[j];
		++counter;
	}

	integratedsimi_tempsample[j] /= counter;
}

// i is the index of sample, j is the index of cells
void CBRSimilarityDlg::limitingIntegrate(float* integratedsimi_tempsample, int i, int j, 
									 vector<CThreeDR*>& in_climateSimiVec, vector<CThreeDR*>& in_parentSimiVec, 
									 vector<CThreeDR*>& in_topoSimiVec, vector<CThreeDR*>& in_vegSimiVec)
{
	integratedsimi_tempsample[j] = 9999;
	if(i < in_topoSimiVec.size())
	{
		float* data = in_topoSimiVec[i]->getDataBuf();
		if(abs(data[j] + 9999) < CBRZERO)
		{
			integratedsimi_tempsample[j] = -9999;
			return;
		}
		if (data[j] < integratedsimi_tempsample[j])
			integratedsimi_tempsample[j] = data[j];
	}

	if(i < in_parentSimiVec.size())
	{
		float* data = in_parentSimiVec[i]->getDataBuf();
		if(abs(data[j] + 9999) < CBRZERO)
		{
			integratedsimi_tempsample[j] = -9999;
			return;
		}
		else if(data[j] < CBRZERO)
		{
			integratedsimi_tempsample[j] = 0.0;
			return;
		}
	}

	if(i < in_climateSimiVec.size())
	{
		float* data = in_climateSimiVec[i]->getDataBuf();
		if(abs(data[j] + 9999) < CBRZERO)
		{
			integratedsimi_tempsample[j] = -9999;
			return;
		}

		if (data[j] < integratedsimi_tempsample[j])
			integratedsimi_tempsample[j] = data[j];
	}

	if(i < in_vegSimiVec.size())
	{
		float* data = in_vegSimiVec[i]->getDataBuf();
		if(abs(data[j] + 9999) < CBRZERO)
		{
			integratedsimi_tempsample[j] = -9999;
			return;
		}
		if (data[j] < integratedsimi_tempsample[j])
			integratedsimi_tempsample[j] = data[j];
	}
}

// i is the index of sample, j is the index of cells
void CBRSimilarityDlg::ruleIntegrate(float* integratedsimi_tempsample, int i, int j, 
									 vector<CThreeDR*>& in_climateSimiVec, vector<CThreeDR*>& in_parentSimiVec, 
									 vector<CThreeDR*>& in_topoSimiVec, vector<CThreeDR*>& in_vegSimiVec)
{
	float f1, f2;
	integratedsimi_tempsample[j] = 0;
	if(i < in_topoSimiVec.size())
	{
		float* data = in_topoSimiVec[i]->getDataBuf();
		if(abs(data[j] + 9999) < CBRZERO)
		{
			integratedsimi_tempsample[j] = -9999;
			return;
		}
		f1 = data[j];
		integratedsimi_tempsample[j] += topoWeight * data[j];
	}


	if(i < in_parentSimiVec.size())
	{
		float* data = in_parentSimiVec[i]->getDataBuf();
		if(abs(data[j] + 9999) < CBRZERO)
		{
			integratedsimi_tempsample[j] = -9999;
			return;
		}
		else if(data[j] < CBRZERO)
		{
			integratedsimi_tempsample[j] = 0.0;
			return;
		}
	}

	if(i < in_climateSimiVec.size())
	{
		float* data = in_climateSimiVec[i]->getDataBuf();
		if(abs(data[j] + 9999) < CBRZERO)
		{
			integratedsimi_tempsample[j] = -9999;
			return;
		}
		else if(data[j] <= climateCut)
		{
			integratedsimi_tempsample[j] = 0.0;
			return;
		}
		f2 = data[j];
		integratedsimi_tempsample[j] += climateWeight * data[j];
	}

	if(i < in_vegSimiVec.size())
	{
		float* data = in_vegSimiVec[i]->getDataBuf();
		if(abs(data[j] + 9999) < CBRZERO)
		{
			integratedsimi_tempsample[j] = -9999;
			return;
		}
		integratedsimi_tempsample[j] += vegWeight * data[j];
	}
	float x = integratedsimi_tempsample[j];
}

int CBRSimilarityDlg::calcIntergratedSimilarity(int iChoice)
{	
	int nSample = topoSimiVec.size(); //sample的个数

	vector<CThreeDR*> in_climateSimiVec, in_parentSimiVec, in_topoSimiVec, in_vegSimiVec;
	//CString str;
	//str.Format("simi: %d\t%d\t%d\t%d\t", climateSimiVec.size(), parentSimiVec.size(), 
	//	topoSimiVec.size(), vegSimiVec.size());
	//MessageBox(str);

	for (int i = 0; i < nSample; ++i)
	{
		

		if(i < topoSimiVec.size())
		{
			CThreeDR* topoData = new CThreeDR();
			topoData->readin(topoSimiVec[i].GetBuffer(), 0);
			in_topoSimiVec.push_back(topoData);
		}

		if(i < climateSimiVec.size())
		{
			CThreeDR* climateData = new CThreeDR();
			climateData->readin(climateSimiVec[i].GetBuffer(), 0);
			in_climateSimiVec.push_back(climateData);
		}

		if(i < parentSimiVec.size())
		{
			CThreeDR* geoData = new CThreeDR();
			geoData->readin(parentSimiVec[i].GetBuffer(), 0);
			in_parentSimiVec.push_back(geoData);
		}

		if(i < vegSimiVec.size())
		{
			CThreeDR* vegData = new CThreeDR();
			vegData->readin(vegSimiVec[i].GetBuffer(), 0);
			in_vegSimiVec.push_back(vegData);
		}
	}

	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	pApp->g_vecSimiFile.clear();
	//get basic information
	CThreeDR* simi = in_topoSimiVec[0]; 
	int nrow = simi->getNumberOfRows();
	int ncol = simi->getNumberOfColumns();

	for (int i = 0; i < nSample; i++)
	{
		curProcess += (double)20 / (double)nSample;
		processCtrl.SetPos((int)curProcess);

		float* integratedsimi_tempsample = new float[nrow * ncol];
		for (int j = 0; j < nrow * ncol; j++)
		{
			integratedsimi_tempsample[j] = 0;
			switch (iChoice)
			{
			case 0:
				averIntegrate(integratedsimi_tempsample, i, j, in_climateSimiVec, in_parentSimiVec,
					in_topoSimiVec, in_vegSimiVec);
				break;
			case 1:
				limitingIntegrate(integratedsimi_tempsample, i, j, in_climateSimiVec, in_parentSimiVec,
					in_topoSimiVec, in_vegSimiVec);
				break;
			case 2:
				ruleIntegrate(integratedsimi_tempsample, i, j, in_climateSimiVec, in_parentSimiVec,
					in_topoSimiVec, in_vegSimiVec);
				break;
			}
		}

		CThreeDR* intergratedtempsimi = new CThreeDR;
		intergratedtempsimi->copyBasicFields(simi);
		intergratedtempsimi->setDataBuf(integratedsimi_tempsample);
		
		intergratedtempsimi->calcStat();
		intergratedtempsimi->setXmin(intergratedtempsimi->getXmin());
		intergratedtempsimi->setYmin(intergratedtempsimi->getYmin());
		
		CString dic;
		dic.Format("%s\\%s", outputFolder.GetString(), "integrated");
		CreateDirectory(dic.GetString(), NULL);
		CString fullfilename;
		fullfilename.Format("%s\\simi%d.3dr", dic, i+1);
		intergratedtempsimi->setDataName(extractFileName(fullfilename).GetBuffer(1));
		intergratedtempsimi->writeout(fullfilename.GetBuffer(1), 0);
		delete intergratedtempsimi;

		//CString asciiFilename = fullfilename.Left(fullfilename.GetLength()-4) + ".txt";
		//threeDR2Ascii(fullfilename.GetBuffer(1), asciiFilename.GetBuffer(1));
		
		//========================================================================
		pApp->g_vecSimiFile.push_back(fullfilename);
	}

	for (int i = 0; i < in_climateSimiVec.size(); ++i)
		delete in_climateSimiVec[i];
	for(int i = 0; i < in_parentSimiVec.size(); ++i)
		delete in_parentSimiVec[i];
	for(int i = 0; i < in_topoSimiVec.size(); ++i)
		delete in_topoSimiVec[i];
	for(int i = 0; i < in_vegSimiVec.size(); ++i)
		delete in_vegSimiVec[i];

	processCtrl.SetPos(100);
	curProcess =0;

	return 0;
}

int CBRSimilarityDlg::calcKdeSimilarity(CString type, vector<CString>& kdeFiles, vector<CBRSamplePoint*>& in_sampleVec, vector<CString>& simiVec)
{

	CString rootDic;
	rootDic.Format("%s\\%s", outputFolder.GetString(), type);
	CreateDirectory(rootDic.GetString(), NULL);

	int nCols, nRows;
	double xmin, ymin;
	float cellsize;
	int nSample = kdeFiles.size();
	if (nSample == 0)
		return 1;

	for(int i = 0; i < nSample; ++i)
	{


		//the each topoFile there is nSample similarity files
		ifstream fIn(kdeFiles[i]);
		string tmp;
		int numPoints;

		fIn >> tmp >> numPoints;
		fIn >> tmp >> nCols;
		fIn >> tmp >> nRows;
		fIn >> tmp >> xmin;
		fIn >> tmp >> ymin;
		fIn >> tmp >> cellsize;

		getline(fIn, tmp);

		string tmp1, tmp2;
		int nAll = nRows * nCols;

		//read data from file
		vector< vector<Point>* > vecData;
		vecData.resize(nAll);
		for (int j = 0; j < nAll; ++j)
		{
			getline(fIn, tmp1);
			getline(fIn, tmp2);
			if (tmp1.empty())
				vecData[j] = NULL;
			else
			{
				stringstream parser1(tmp1);
				stringstream parser2(tmp2);

				vector<Point> *curVecPt = new vector<Point>();
				curVecPt->resize(numPoints);
				for (int k = 0; k < numPoints; ++k)
				{
					Point pt;
					parser1 >> pt.x;
					parser2 >> pt.y;
					(*curVecPt)[k] = pt;
				}
				vecData[j] = curVecPt;
			}
		}
		fIn.close();

		for (int t = 0; t < in_sampleVec.size(); ++t)
		{
			/*nProcess += 100.0 / (varCount * nSample * in_sampleVec.size());
			processCtrl.SetPos(int(nProcess));*/
			curProcess += (double)20 / (double)in_sampleVec.size();
		    processCtrl.SetPos((int)curProcess);

			vector<Point>* pVec;
			if(type == "topology")
				pVec = in_sampleVec[t]->getTopoVariable(i);
			else if(type == "vegetation")
				pVec = in_sampleVec[t]->getVegVarVariable(i);
			else
			{
				MessageBox("In function calcKdeSimilarity: type parameter has an invalid input.");
				return 1;
			}
			double s1 = CurveArea(*pVec);

			float* pSimi = new float[nAll];

			for (int j = 0; j < nAll; ++j)
			{
				vector<Point>* curVecPt = vecData[j];
				//to be modified
				if (curVecPt)
				{
					double s2 = CurveArea(*curVecPt);
					double ss = CrossArea(*curVecPt, *pVec);
					//if(j == 494)
					//{
					//	ofstream testof("e:\\curve.txt");
					//	for (int c = 0; c < curVecPt->size(); ++c)
					//		testof << curVecPt->at(c).x << ",";
					//	testof << endl;
					//	for (int c = 0; c < curVecPt->size(); ++c)
					//		testof << curVecPt->at(c).y << ",";
					//	testof.close();
					//}
					pSimi[j] = 2 * ss / (s1 + s2);
					//testof << j << "\t" << pSimi[j] << "\t" << ss << "\t" << s1 << "\t" << s2 << endl;
				}
				else
					pSimi[j] = -9999;
			}


			CThreeDR* p3dr = new CThreeDR();
			p3dr->setNumberOfRows(nRows);
			p3dr->setNumberOfColumns(nCols);
			p3dr->setXmin(xmin);
			p3dr->setYmin(ymin);
			p3dr->setCellSize(cellsize);

			p3dr->setDataBuf(pSimi);
			p3dr->setNullDataValue(-9999);

			CString coreFileName = getCoreFilename(kdeFiles[i]);
			CString dic;
			dic.Format("%s\\%s\\%s", outputFolder.GetString(), type, coreFileName);
			CreateDirectory(dic.GetString(), NULL);

			CString fullfilename;
			fullfilename.Format("%s\\simi%d.3dr", dic, t+1);
			p3dr->setDataName(extractFileName(fullfilename).GetBuffer(1));
			p3dr->writeout(fullfilename.GetBuffer(1), 0);
			delete p3dr;

			//CString asciiFilename = fullfilename.Left(fullfilename.GetLength()-4) + ".txt";
			//threeDR2Ascii(fullfilename.GetBuffer(1), asciiFilename.GetBuffer(1));
		}
		for (int i = 0; i < vecData.size(); ++i)
			delete vecData[i];
}

	//average the attributes to a single attribute
	int nAll = nCols * nRows;

	simiVec.clear();
	for (int i = 0; i < in_sampleVec.size(); ++i)
	{
		float* pData = new float[nAll];
		for (int k = 0; k < nAll; ++k)
			pData[k] = 0.0f;

		for(int j = 0; j < kdeFiles.size(); ++j)
		{
			CString coreFileName = getCoreFilename(kdeFiles[j]);
			CString fullName;
			fullName.Format("%s\\%s\\simi%d.3dr", rootDic, coreFileName, i+1);
			CThreeDR threeDR;
			threeDR.readin(fullName.GetBuffer(), 0);

			for (int iCell = 0; iCell < nAll; ++iCell)
			{
				pData[iCell] += threeDR.getDataBuf()[iCell];
			}	
		}

		for (int k = 0; k < nAll; ++k)
			pData[k] /= kdeFiles.size();

		CThreeDR* p3dr = new CThreeDR();
		p3dr->setNumberOfRows(nRows);
		p3dr->setNumberOfColumns(nCols);
		p3dr->setXmin(xmin);
		p3dr->setYmin(ymin);
		p3dr->setCellSize(cellsize);
		p3dr->setDataBuf(pData);

		CString fullfilename;
		fullfilename.Format("%s\\simi%d.3dr", rootDic, i+1);
		p3dr->setDataName(extractFileName(fullfilename).GetBuffer(1));
		p3dr->writeout(fullfilename.GetBuffer(1), 0);
		delete p3dr;

		//CString asciiFilename = fullfilename.Left(fullfilename.GetLength()-4) + ".txt";
		//threeDR2Ascii(fullfilename.GetBuffer(1), asciiFilename.GetBuffer(1));

		simiVec.push_back(fullfilename);
	}

	return 0;
}

BOOL CBRSimilarityDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//comboClimateVarCtrl.AddString("Euclidean distance");
	comboClimateVarCtrl.AddString("Gower distance");
	comboClimateVarCtrl.SetCurSel(0);

	comboPmVarCtrl.AddString("Boolean");
	comboPmVarCtrl.SetCurSel(0);
	
	fillKdeCombo();

	comboCaseMethod.AddString("Average weighed");
	comboCaseMethod.AddString("Limiting Factor");
	comboCaseMethod.AddString("Rules");
	comboCaseMethod.SetCurSel(2);

	return True;
}

void CBRSimilarityDlg::fillKdeCombo()
{
	while(comboTopoVarCtrl.GetCount() > 0)
		comboTopoVarCtrl.DeleteString(0);
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	if (pApp->g_useKde)
	{
		comboTopoVarCtrl.AddString("CM for each variable's PDF and Mean for integration");
		comboTopoVarCtrl.SetCurSel(0);
	}
	else
	{
		comboTopoVarCtrl.AddString("Gower distance");
		//comboTopoVarCtrl.AddString("Euclidean distance");
		comboTopoVarCtrl.SetCurSel(0);
	}


	while(comboVegVarCtrl.GetCount() > 0)
		comboVegVarCtrl.DeleteString(0);
	if (pApp->g_useKdeVegetation)
	{
		comboVegVarCtrl.AddString("CM for each variable's PDF and Mean for integration");
		comboVegVarCtrl.SetCurSel(0);
	}
	else
	{
		comboVegVarCtrl.AddString("Gower distance");
		//comboVegVarCtrl.AddString("Euclidean distance");
		comboVegVarCtrl.SetCurSel(0);
	}
}
void CBRSimilarityDlg::OnBnClickedButton1()
{
	BROWSEINFO      bi;     
	TCHAR			szDisplayName[MAX_PATH];     
	LPITEMIDLIST    pidl;     
	LPMALLOC        pMalloc   =   NULL;       
	ZeroMemory(&bi,   sizeof(bi));               
	bi.hwndOwner   =   GetSafeHwnd();     
	bi.pszDisplayName   =   szDisplayName;     
	bi.lpszTitle   =   TEXT("Please select a folder:");     
	bi.ulFlags   =   BIF_RETURNONLYFSDIRS;       
	pidl   =   SHBrowseForFolder(&bi);     
	//CCString   sPath;   
	if   (pidl)     
	{     
		SHGetPathFromIDList(pidl, szDisplayName);   //   set   the   directory   name.     
		outputFolder = szDisplayName;   
		UpdateData(FALSE);
	}
}

void CBRSimilarityDlg::OnCbnSelchangeComboCase()
{
	if (comboCaseMethod.GetCurSel() != 2)
	{
		climateCutCtrl.EnableWindow(FALSE);
		climateWeightCtrl.EnableWindow(FALSE);
		topoWeightCtrl.EnableWindow(FALSE);
		vegWeightCtrl.EnableWindow(FALSE);
	}
	else
	{
		climateCutCtrl.EnableWindow(TRUE);
		climateWeightCtrl.EnableWindow(TRUE);
		topoWeightCtrl.EnableWindow(TRUE);
		vegWeightCtrl.EnableWindow(TRUE);
	}
}

void CBRSimilarityDlg::OnBnClickedBack()
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	this->ShowWindow(SW_HIDE);
	pApp->pSampleDlg->ShowWindow(SW_SHOW);
}

void CBRSimilarityDlg::OnBnClickedCancel()
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

void CBRSimilarityDlg::resetProcess()
{
	processCtrl.SetPos(0);
}
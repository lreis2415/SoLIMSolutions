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
// CBRUncertaintyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SoLIM.h"
#include "CBRUncertaintyDlg.h"
#include ".\cbruncertaintydlg.h"
#include "ThreeDR.h"
#include "GetDirDlg.h"
#include <vector>
using namespace std;


// CBRUncertaintyDlg 对话框

IMPLEMENT_DYNAMIC(CBRUncertaintyDlg, CDialog)
CBRUncertaintyDlg::CBRUncertaintyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBRUncertaintyDlg::IDD, pParent)
	, m_OutputUncertaintyFilename(_T(""))
	, m_OutputPredictvaluesFoldername(_T(""))
	, threshold(0.5)
{
	
}

CBRUncertaintyDlg::~CBRUncertaintyDlg()
{

}

void CBRUncertaintyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Uncertaintyfilename, m_OutputUncertaintyFilename);
	DDX_Text(pDX, IDC_EDIT_Valuesfoldername, m_OutputPredictvaluesFoldername);
	DDX_Text(pDX, IDC_EDIT_Threshold, threshold);
	DDX_Control(pDX, IDC_EDIT_Uncertaintyfilename, uncertaintyCtrl);
	DDX_Control(pDX, IDC_EDIT_Valuesfoldername, propertyFolderCtrl);
	DDX_Control(pDX, IDC_EDIT_Threshold, thresholdCtrl);
}


BEGIN_MESSAGE_MAP(CBRUncertaintyDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SaveUncertainty, OnBnClickedButtonSaveuncertainty)
	ON_BN_CLICKED(IDC_BUTTON_SaveValues, OnBnClickedButtonSavevalues)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(ID_BACK, OnBnClickedBack)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CBRUncertaintyDlg 消息处理程序

void CBRUncertaintyDlg::OnBnClickedButtonSaveuncertainty()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取保存Uncertainty的文件名
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(FALSE, "3dr", ".3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK)
	{
		m_OutputUncertaintyFilename = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CBRUncertaintyDlg::OnBnClickedButtonSavevalues()
{
	CGetDirDlg dirDlg;   
	m_OutputPredictvaluesFoldername = dirDlg.GetDirectory(this, "", "Choose a directory to save the predicted values:");;   
	UpdateData(FALSE);
}

int CBRUncertaintyDlg::getSimilarityVec(vector<CThreeDR*>& similarityVec)
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	similarityVec.clear();
	for (int i = 0; i < pApp->g_vecSimiFile.size(); ++i)
	{
		CThreeDR* data = new CThreeDR;
		int flag = data->readin(pApp->g_vecSimiFile[i].GetBuffer(1), 0);
		if(flag != 0)
		{
			MessageBox("Error happens when reading the similarity files.\nPlease return to the last step and recalculate the similarities.");
			return 1;
		}
		similarityVec.push_back(data);
	}
	return 0;
}

int CBRUncertaintyDlg::calcValues(vector<CThreeDR*>& in_similarities, vector<CBRSamplePoint*>& in_samples, CThreeDR* in_uncertainty, float in_threshold)
{
	const int nsample = in_samples.size();							//NO. of samples
	CThreeDR* simi_sample1 = in_similarities[0];					//get similarity to sample one; all the similarities are dimension matched 
	const int nrow = simi_sample1->getNumberOfRows();				//get the NO. of rows
	const int ncol = simi_sample1->getNumberOfColumns();			//get the NO. of columes
	float No_Data = simi_sample1->getNullDataValue();
	float* Uncertainty_dataBuf = in_uncertainty->getDataBuf();		//Uncertainty
	
	CString valuename;
	CString outvalfilename;

	const int nval = in_samples[0]->getAttrCount();                 //the NO. of values

	//begin to calculate the uncertainty of each cell
	vector<float> pointi_similarities;								//similarities between point i and all samples
	float pointi_samplej_similarity;								//similarity between point i and sample j	
	int flag;                                                       //to record the sum,
	float maxS, sumS, sumSV;				                        // Max value and its flag similarity vector on this point
	CThreeDR* tempPredictvalue;
	int nAll = nrow * ncol;

	for(int k = 0; k < nval; k++)  //对每一种土壤属性
	{
		float* predicttemp = new float[nAll];
		for (int i = 0; i < nAll; i++) 
		{
			sumSV = 0.0;
			float u = Uncertainty_dataBuf[i];
			if(Uncertainty_dataBuf[i] >= in_threshold || abs(Uncertainty_dataBuf[i] + 9999) < CBRZERO)
			{
				predicttemp[i] = No_Data;
			}
			else
			{
				//get the similarity vector on this point
				for(int j = 0; j < nsample; j++)
				{	
					pointi_samplej_similarity = in_similarities[j]->getDataBuf()[i];
					pointi_similarities.push_back(pointi_samplej_similarity);
					float val = in_samples[j]->getAttribute(k);
					sumSV = sumSV + pointi_samplej_similarity * val;
				}		
				findMaxSum(pointi_similarities, flag, maxS, sumS);		//to find the max similarity
				float mValue = in_samples[flag]->getAttribute(k);
				if( abs(sumS - maxS) < CBRZERO)
					predicttemp[i] = mValue;
				else
				    predicttemp[i] = mValue * maxS + (1.0 - maxS) * (sumSV - mValue * maxS) / (sumS - maxS);	
			}

			pointi_similarities.clear();
		}
		
		tempPredictvalue = new CThreeDR;
		tempPredictvalue->copyBasicFields(simi_sample1);
		tempPredictvalue->setDataBuf(predicttemp);
		tempPredictvalue->calcStat();
		tempPredictvalue->setXmin(tempPredictvalue->getXmin());
		tempPredictvalue->setYmin(tempPredictvalue->getYmin());
		tempPredictvalue->setDataUnits(tempPredictvalue->getDataUnits());
		valuename = CBRSamplePoint::getAttrName(k).c_str();
		outvalfilename = m_OutputPredictvaluesFoldername + "/" + "predicted_" + valuename + ".3dr";
		tempPredictvalue->writeout(outvalfilename.GetBuffer(1), 0);
		delete tempPredictvalue;

		CString asciiFile;
		asciiFile.Format("%s.txt", outvalfilename.Left(outvalfilename.GetLength()-4));
		threeDR2Ascii(outvalfilename.GetBuffer(), asciiFile.GetBuffer());
	}

	return 0;
}

CThreeDR* CBRUncertaintyDlg::calcUncertainty(vector<CThreeDR*>& in_similarities)
{
	int nsample = in_similarities.size();							//NO. of sample
	CThreeDR* simi_sample1 = in_similarities[0];					//get similarity to sample one; all the similarities are dimension matched 
	int nrow = simi_sample1->getNumberOfRows();						//get the NO. of rows
	int ncol = simi_sample1->getNumberOfColumns();					//get the NO. of columes
	float* Uncertainty_dataBuf = new float[nrow * ncol];

	//begin to calculate the uncertainty of each cell
	CThreeDR* similarity_CThreeDR;
	vector<float> pointi_similarities;								//similarities between point i and all samples
	float* samplej_similarities;									//all points' similarities to sample j
	float pointi_samplej_similarity;								//similarity between point i and sample j

	int flag; float max; float sum;									//to record the sum, Max value and its flag similarity vector on this point

	for (int i = 0; i < nrow * ncol; i++) 
	{
		//first, we need to get the similarity vector on this point
		sum = 0.0;
		pointi_similarities.clear();
		for(int j = 0; j < nsample; j++)
		{
			similarity_CThreeDR = in_similarities[j];
			samplej_similarities = similarity_CThreeDR->getDataBuf();
			pointi_samplej_similarity = samplej_similarities[i];
			pointi_similarities.push_back(pointi_samplej_similarity);
		}
		//then we can calculate the uncertainty on this point Uncertainty = 1 - max(pointi_similarities)
		findMaxSum(pointi_similarities, flag, max, sum);			//to find the max similarity
		if (abs(max + 9999) < CBRZERO)
			Uncertainty_dataBuf[i] = -9999;
		else
			Uncertainty_dataBuf[i] = 1 - max;
	}
	CThreeDR* uncertainty = new CThreeDR;
	uncertainty->copyBasicFields(simi_sample1);
	uncertainty->setDataBuf(Uncertainty_dataBuf);
	uncertainty->calcStat();
	uncertainty->setXmin(uncertainty->getXmin());
	uncertainty->setYmin(uncertainty->getYmin());
	uncertainty->setDataUnits(uncertainty->getDataUnits());
	uncertainty->writeout(m_OutputUncertaintyFilename.GetBuffer(1),0);	

	CString asciiFile;
	asciiFile.Format("%s.txt", m_OutputUncertaintyFilename.Left(m_OutputUncertaintyFilename.GetLength()-4));
	threeDR2Ascii(m_OutputUncertaintyFilename.GetBuffer(), asciiFile);

	return uncertainty;
}
//找出一个数组中的最大值和下标,并求和
int CBRUncertaintyDlg::findMaxSum(vector<float>& inputarray, int &flag, float &max, float &sum)
{	
	flag = 0; max = inputarray[0]; sum = 0;
	for (int i = 0; i < inputarray.size(); i++)
	{
		if(inputarray[i] >= max)
		{	
			max = inputarray[i];
			flag = i;
		}
		sum = sum + inputarray[i];
	}
	return 0;
}

void CBRUncertaintyDlg::OnBnClickedOk()
{
	UpdateData();
	if(m_OutputUncertaintyFilename == "")
	{
		MessageBox("Uncertainty file name must be provided.");
		uncertaintyCtrl.SetFocus();
		return;
	}
	if(m_OutputPredictvaluesFoldername == "")
	{
		MessageBox("Output folder must be provided.");
		propertyFolderCtrl.SetFocus();
		return;
	}

	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	vector<CBRSamplePoint*> sampleVec = pApp->g_vecSamplePoint;
	vector<CThreeDR*> similarityVec;
	if( getSimilarityVec(similarityVec) == 1)
		return;

	CThreeDR* in_uncertainty = calcUncertainty(similarityVec);

	calcValues(similarityVec, sampleVec, in_uncertainty, threshold);

	delete in_uncertainty;
	for (int i = 0; i < similarityVec.size(); i++)
	{
		delete similarityVec[i];
	}

	MessageBox("Attributes are calculated!");

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

	OnOK();

	delete this;
	pApp->pUncertaintyDlg = NULL;
}

void CBRUncertaintyDlg::OnBnClickedBack()
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	this->ShowWindow(SW_HIDE);
	pApp->pSimiDlg->ShowWindow(SW_SHOW);
	pApp->pSimiDlg->resetProcess();
	pApp->pSimiDlg->fillKdeCombo();
}

void CBRUncertaintyDlg::OnBnClickedCancel()
{
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

		if(pApp->pTypeinferDlg)
	{
		delete pApp->pTypeinferDlg;
		pApp->pTypeinferDlg = NULL;
	}
	OnCancel();
}

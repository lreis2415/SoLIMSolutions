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
// BRTypeInferenceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "CBRTypeInferenceDlg.h"
#include ".\cbrtypeinferencedlg.h"


// CBRTypeInferenceDlg dialog

IMPLEMENT_DYNAMIC(CBRTypeInferenceDlg, CDialog)
CBRTypeInferenceDlg::CBRTypeInferenceDlg(CWnd* pParent /*=NULL*/)
: CDialog(CBRTypeInferenceDlg::IDD, pParent)
, m_threshold(0.5)
{
	//this->threshold = 0.5;
}

CBRTypeInferenceDlg::~CBRTypeInferenceDlg()
{
}

void CBRTypeInferenceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);


	DDX_Text(pDX,IDC_EDIT_MAXIMUM_SIMI, m_OutputMaxSimFilename);
	DDX_Text(pDX, IDC_EDIT_TIMES_FILENAME, m_OutputTimesFilename);
	//DDX_Text(pDX, IDC_EDIT_SIM_THRESHOLD, threshold);
	DDX_Text(pDX, IDC_EDIT_Threshold, m_threshold);
}


BEGIN_MESSAGE_MAP(CBRTypeInferenceDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_Save_SIM, OnBnClickedButtonSaveSim)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_TIMES, OnBnClickedButtonSaveTimes)
	ON_BN_CLICKED(ID_BACK, OnBnClickedBack)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CBRTypeInferenceDlg message handlers

void CBRTypeInferenceDlg::OnBnClickedButtonSaveSim()
{
	UpdateData(true);
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(FALSE, "3dr", ".3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK)
	{
		m_OutputMaxSimFilename = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CBRTypeInferenceDlg::OnBnClickedButtonSaveTimes()
{
    UpdateData(true);
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(FALSE, "3dr", ".3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK)
	{
		m_OutputTimesFilename = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CBRTypeInferenceDlg::OnBnClickedBack()
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	this->ShowWindow(SW_HIDE);
	pApp->pSimiDlg->ShowWindow(SW_SHOW);
	pApp->pSimiDlg->resetProcess();
	pApp->pSimiDlg->fillKdeCombo();
}


void CBRTypeInferenceDlg::OnBnClickedCancel()
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

	if(pApp->pUncertaintyDlg)
	{
		delete pApp->pUncertaintyDlg;
		pApp->pUncertaintyDlg = NULL;
	}
	OnCancel();
}







int CBRTypeInferenceDlg::getSimilarityVec(vector<CThreeDR*>& similarityVec)
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	similarityVec.clear();
	for (int i = 0; i < pApp->g_vecSimiFile.size(); ++i)
	{
		CThreeDR* data = new CThreeDR;
		int flag = data->readin(pApp->g_vecSimiFile[i].GetBuffer(1), 0);
		if(flag != 0){
			MessageBox("Cannot open the input file or file format has errors.");
			return 1;
		}
		similarityVec.push_back(data);
	}
	return 0;
}




void CBRTypeInferenceDlg::calcMaxSimAndTime(vector<CThreeDR*>& in_similarities)
{
	int nsample = in_similarities.size();							//NO. of sample
	CThreeDR* simi_sample1 = in_similarities[0];					//get similarity to sample one; all the similarities are dimension matched 
	int nrow = simi_sample1->getNumberOfRows();						//get the NO. of rows
	int ncol = simi_sample1->getNumberOfColumns();					//get the NO. of columes
	float* MaxSim_dataBuf = new float[nrow * ncol];

	float* Times_dataBuf = new float[nrow * ncol];


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

		float simMax = 0;
		int times = 0;
		for(int j = 0; j < nsample; j++)
		{
			similarity_CThreeDR = in_similarities[j];
			samplej_similarities = similarity_CThreeDR->getDataBuf();
			pointi_samplej_similarity = samplej_similarities[i];

			if(pointi_samplej_similarity > simMax)
			{
				simMax = pointi_samplej_similarity;
			}

			if(pointi_samplej_similarity >= m_threshold)
			{
				times ++;
			}
			//pointi_similarities.push_back(pointi_samplej_similarity);
		}
	
		MaxSim_dataBuf[i] = simMax;
		Times_dataBuf[i] = times;
	}
	CThreeDR* maxSim = new CThreeDR;
	maxSim->copyBasicFields(simi_sample1);
	maxSim->setDataBuf(MaxSim_dataBuf);
	maxSim->setDataName(extractFileName(m_OutputMaxSimFilename).GetBuffer(1));
	maxSim->calcStat();
	maxSim->setXmin(maxSim->getXmin());
	maxSim->setYmin(maxSim->getYmin());
	maxSim->setDataUnits(maxSim->getDataUnits());
	maxSim->writeout(this->m_OutputMaxSimFilename.GetBuffer(1),0);	

	CString asciiFile;
	asciiFile.Format("%s.txt", m_OutputMaxSimFilename.Left(m_OutputMaxSimFilename.GetLength()-4));
	threeDR2Ascii(m_OutputMaxSimFilename.GetBuffer(), asciiFile);


	CThreeDR* time = new CThreeDR;
	time->copyBasicFields(simi_sample1);
	time->setDataBuf(Times_dataBuf);
	time->setDataName(extractFileName(m_OutputTimesFilename).GetBuffer(1));
	time->calcStat();
	time->setXmin(time->getXmin());
	time->setYmin(time->getYmin());
	time->setDataUnits(time->getDataUnits());
	time->writeout(this->m_OutputTimesFilename.GetBuffer(1),0);	

	//CString asciiFile;
	asciiFile.Format("%s.txt", m_OutputTimesFilename.Left(m_OutputTimesFilename.GetLength()-4));
	threeDR2Ascii(m_OutputTimesFilename.GetBuffer(), asciiFile);


	delete maxSim;
	delete time;

	//return uncertainty;
	return;
}



void CBRTypeInferenceDlg::OnBnClickedOk()
{
	UpdateData(true);

	if(m_OutputMaxSimFilename == "")
	{
		MessageBox("Maximum similarity file name must be provided.");
		//uncertaintyCtrl.SetFocus();
		return;
	}
	if(m_OutputTimesFilename == "")
	{
		MessageBox("The name of file recording times occurred must be provided.");
		//propertyFolderCtrl.SetFocus();
		return;
	}

	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	vector<CBRSamplePoint*> sampleVec = pApp->g_vecSamplePoint;
	vector<CThreeDR*> similarityVec;
	getSimilarityVec(similarityVec);

	calcMaxSimAndTime(similarityVec);


	for (int i = 0; i < similarityVec.size(); i++)
	{
		delete similarityVec[i];
	}

	MessageBox("Inference is finished!");

	/*if(pApp->pSmallScaleDlg)
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

	pApp->pTypeinferDlg = NULL;*/
}



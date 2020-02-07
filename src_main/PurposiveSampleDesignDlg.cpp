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
// PurposiveSampleDesignDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SoLIM.h"
#include "PurposiveSampleDesignDlg.h"
#include ".\PurposiveSampleDesigndlg.h"
#include "FCM.h"
#include "NewClusterNumDlg.h"
#include "GetDirDlg.h"

// CPurposiveSampleDesignDlg 对话框

IMPLEMENT_DYNAMIC(CPurposiveSampleDesignDlg, CDialog)
CPurposiveSampleDesignDlg::CPurposiveSampleDesignDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPurposiveSampleDesignDlg::IDD, pParent)
, m_iKernel(1)
, m_dWeight(2)
, m_iIteration(50)
, m_strResultDir(_T(""))
, m_dEPS(0.01)
, m_MaskValue(0)
, m_dAlpha(0.7)
, m_iItemNum(0)
, m_iNumOfPnt(3)
, m_dDistThreshold(100)
, m_OutputFileName(_T(""))
, m_iMinClusterNum(2)
, m_iMaxClusterNum(20)
{
	this->m_iItemNum = 0;
	m_pEnvDatabase = NULL;

}

CPurposiveSampleDesignDlg::~CPurposiveSampleDesignDlg()
{
}

void CPurposiveSampleDesignDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_LAYER, m_LayerList);

	DDX_Check(pDX, IDC_MaskChk, m_Mask);
	DDX_Text(pDX, IDC_MaskFileFld, m_MaskFileName);
	DDX_Text(pDX, IDC_MaskValueFld, m_MaskValue);


	DDX_Text(pDX, IDC_EDIT_KENEL2, m_iKernel);
	DDX_Text(pDX, IDC_EDIT_M, m_dWeight);
	DDX_Text(pDX, IDC_EDIT_ITERATION, m_iIteration);

	DDX_Text(pDX, IDC_EDIT_RESULT_DIRECTORY, m_strResultDir);
	DDX_Text(pDX, IDC_EDIT_EPS, m_dEPS);


     DDX_Text(pDX, IDC_EDIT_MIN_CLUSTER_NUM, m_iMinClusterNum);
	DDX_Text(pDX, IDC_EDIT_MAX_CLUSTER_NUM, m_iMaxClusterNum);



	DDX_Text(pDX, IDC_EDIT_ALPHA, m_dAlpha);
	DDX_Text(pDX, IDC_EDIT_RESULT_DIRECTORY, m_strResultDir);

	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);

	DDX_Text(pDX, IDC_EDIT1, m_iNumOfPnt);
	DDX_Text(pDX, IDC_EDIT2, m_dDistThreshold);
	DDX_Text(pDX, IDC_EDIT_FIELD_SAMPLE, m_OutputFileName);
}


BEGIN_MESSAGE_MAP(CPurposiveSampleDesignDlg, CDialog)


	ON_BN_CLICKED(IDC_BUTTON_ADD_LAYER, OnBnClickedButtonAddLayer)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_LAYER, OnBnClickedButtonRemoveLayer)
	ON_BN_CLICKED(IDC_MaskFileBtn, OnMaskFileBtn)
	ON_BN_CLICKED(IDC_MaskChk, OnMaskChk)



	ON_BN_CLICKED(IDC_BUTTON_DIR, OnBnClickedButtonDir)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SAMPLE_TABLE, OnBnClickedButtonSampleTable)
END_MESSAGE_MAP()



BOOL CPurposiveSampleDesignDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here




	CDC* pDC = GetDC();
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	this->m_LayerList.InsertColumn(0, "Name", LVCFMT_LEFT, 20*tm.tmAveCharWidth, 0);
	this->m_LayerList.InsertColumn(1, "File", LVCFMT_LEFT, 70*tm.tmAveCharWidth, 1);
	this->m_LayerList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);



	if(m_pEnvDatabase==NULL)
		m_pEnvDatabase = new CEnvDatabase();
	m_LayerList.DeleteAllItems();




	

	this->m_iItemNum = 0;
	m_vecItem.clear();
	return TRUE;  

}





void CPurposiveSampleDesignDlg::OnMaskChk(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_Mask){
		GetDlgItem(IDC_MaskFileFld)->EnableWindow(TRUE);
		GetDlgItem(IDC_MaskFileBtn)->EnableWindow(TRUE);
		GetDlgItem(IDC_MaskValueLbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_MaskValueFld)->EnableWindow(TRUE);
	}
	else{
		GetDlgItem(IDC_MaskFileFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_MaskFileBtn)->EnableWindow(FALSE);
		GetDlgItem(IDC_MaskValueLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_MaskValueFld)->EnableWindow(FALSE);
	}
}

void CPurposiveSampleDesignDlg::OnMaskFileBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_MaskFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}




void CPurposiveSampleDesignDlg::OnBnClickedButtonAddLayer()
{
	UpdateData(TRUE);


	CString layerFileName;
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_ALLOWMULTISELECT |OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Add Layer(s)";
	CString   strFileNames;  
	m_ldFile.m_ofn.lpstrFile=strFileNames.GetBuffer(4000); 
	m_ldFile.m_ofn.nMaxFile= 4000 ;  
	if(m_ldFile.DoModal() == IDOK)
	{
		POSITION mPos= m_ldFile.GetStartPosition(); 
		while(mPos!=NULL) 
		{         
			layerFileName = m_ldFile.GetNextPathName(mPos);         
			if(layerFileName == "")
			{
				MessageBox("No input file.");
				return;
			}
			CString strLayerName = "untitled";
			CEnvLayer *pLayer = new CEnvLayer(m_pEnvDatabase->getLayerNum(),strLayerName,  layerFileName );
			if(pLayer->getLayerID() != -1)
			{ 
				strLayerName.Format("%s",pLayer->m_pTheData->getDataName());
				pLayer->setLayerName(strLayerName);
				int i=0;
				for(i=0;i<m_pEnvDatabase->getLayerNum();i++) 
				{
					if((m_pEnvDatabase->getLayer(i)->getLayerName())==strLayerName)
					{
						AfxMessageBox("The name of the layer is the same with one existing layer. Please use another layer.");
						delete pLayer;
						pLayer = NULL;
						break;
					}
				}
				if(i!=m_pEnvDatabase->getLayerNum())
					continue;

				if(m_pEnvDatabase->AddLayer(pLayer))   //if add layer successfully
				{
					int count = m_LayerList.GetItemCount();
					m_LayerList.InsertItem(LVIF_TEXT|LVIF_STATE, count, strLayerName, LVIS_SELECTED, LVIS_SELECTED, 0, 0);
					m_LayerList.SetItemText(count, 1, layerFileName);
				}
			}
			else 
			{
				delete pLayer;
				pLayer = NULL;
				CString msg;
				msg.Format("Error in reading %s. The layer will not be added",layerFileName);
				AfxMessageBox(msg);
			}	

		}

	}




}

void CPurposiveSampleDesignDlg::OnBnClickedButtonRemoveLayer()
{
	UpdateData(TRUE);
	POSITION pos = m_LayerList.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nItem = m_LayerList.GetNextSelectedItem(pos);
		CString strLayerName = m_LayerList.GetItemText(nItem, 0);
		for(int i=0;i<m_pEnvDatabase->getLayerNum();i++) 	
			if((m_pEnvDatabase->getLayer(i)->getLayerName())==strLayerName)
				m_pEnvDatabase->DeleteLayer(i);
		m_LayerList.DeleteItem(nItem);
	}
}





int  CPurposiveSampleDesignDlg::CheckMask(bool* idBuf, int nrow, int ncol,CThreeDR *theData){ //Check if mask should be used
	//int nrow = theEnvDatabase->theSetting->nrow;
	//int ncol = theEnvDatabase->theSetting->ncol;
	int numOfValidPixel = 0;
	int numPixels = nrow*ncol;
	if(m_MaskFileName.IsEmpty()){
		//MessageBox("No mask file specified.");
		m_Mask = false;
		return numPixels;
	}
	else{
		CThreeDR* maskFile = new CThreeDR;
		int readflag = maskFile->readin(m_MaskFileName.GetBuffer(100), 0);
		if(readflag > 0){
			MessageBox("Error in reading mask file. No mask used.");
			m_Mask = false;
			return numPixels;
		}
		else{
			if(!maskFile->spatialRangeMatch(theData)){
				MessageBox("The mask file and the first layer do not match. No mask used.");
				m_Mask = false;
				return numPixels;
			}
			else{
				for(int i=0; i<numPixels; i++)
				{
					if(fabs(maskFile->getDataBuf()[i]-m_MaskValue)<VERYSMALL)
						idBuf[i] = false;
					else if(fabs(theData->getDataBuf()[i] - theData->getNullDataValue()) < VERYSMALL)
                        idBuf[i] = false;
					else
						numOfValidPixel ++;
				}
			}
		}
		delete maskFile;
	}
	return numOfValidPixel;
}




void CPurposiveSampleDesignDlg::OnBnClickedButtonDir()
{
	UpdateData(true);

	CGetDirDlg m_getDirDlg;

	CString str = m_getDirDlg.GetDirectory(this, "", "Choose Result Directory :");

	m_strResultDir = str;
	UpdateData(false);
}




void CPurposiveSampleDesignDlg::OnBnClickedOk()
{

	UpdateData(true);



	m_ProgressBar.ShowWindow(SW_SHOW);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);


	CString str;
	str.Format("%s%s", m_strResultDir, "\\fcm_result");
	CreateDirectory(str, NULL);



	int nrow,ncol;
	nrow = m_pEnvDatabase->m_pTheSetting->getRowNum();
	ncol = m_pEnvDatabase->m_pTheSetting->getColNum();


	int NumOfValidPixel = nrow * ncol;

	bool* idBuf = new bool[nrow*ncol];
	for(int i=0; i<nrow*ncol; i++)
	{
		idBuf[i]= true;
	}
	if(m_Mask == true)
	{	
		NumOfValidPixel = CheckMask(idBuf, nrow, ncol,m_pEnvDatabase->getLayer(0)->m_pTheData);
	}



	
	this->m_iItemNum = m_iMaxClusterNum - m_iMinClusterNum +1;
	for(int i=this->m_iMinClusterNum; i<=this->m_iMaxClusterNum; i++)
	{
		this->m_vecItem.push_back(i);
	}

		FCM* fcm = new FCM(m_pEnvDatabase,idBuf, NumOfValidPixel, this->m_iKernel,m_dWeight,this->m_dEPS, this->m_iIteration);
    
	fcm->InitiatePixels();
    fcm -> SamplePixels();
    fcm -> AssignScalingMatrix();	

	for(int i=0;i<m_iItemNum;i++)
	{
	    fcm->setNumOfClasses(this->m_vecItem[i]);   

		fcm->IniMembershipMatrix();

		CString str;
		str.Format("%s%s%d%s", m_strResultDir, "\\fcm_result\\", m_vecItem[i], "_clusters");
		CreateDirectory(str, NULL);
		fcm->setResultDir(str);

		
		fcm->CalClassesCentroid();
		fcm->CalEntropy();		
		fcm->WriteClassCentroidFile();
		fcm->WriteMembershipFiles(); //write 3dr 

	

		fcm->DestroyMatrix();
		m_ProgressBar.SetPos(5+ (i+1)*55/m_iItemNum);

	}

	delete fcm;

	if(idBuf != NULL)
	{
		delete idBuf;
		idBuf = NULL;
	}


	m_ProgressBar.SetPos(60);


	str.Format("%s%s", m_strResultDir, "\\alpha_cut");
	CreateDirectory(str, NULL);

	str.Format("%s%s", m_strResultDir, "\\patterns");
	CreateDirectory(str, NULL);

	str.Format("%s%s", m_strResultDir, "\\patterns_all");
	CreateDirectory(str, NULL);

	//int nrow,ncol;
	double XMin, YMin,
	float CellSize;
	char *DataUnit;
	char *GridUnit;


	
	CThreeDR* memberFile = new CThreeDR();

	CString strMemberFile;
	strMemberFile.Format("%s%s%d%s%d%s", this->m_strResultDir, "\\fcm_result\\", m_vecItem[0], "_clusters\\class", 1, ".3dr");


	int readflag = memberFile->readin(strMemberFile.GetBuffer(100), 0);
	if(readflag > 0)
	{
		MessageBox("Error in reading file.");
		return;
	}
	else
	{			
		nrow = memberFile->getNumberOfRows();
		ncol = memberFile->getNumberOfColumns();
		CellSize = memberFile->getCellSize();
		XMin = memberFile->getXmin();
		YMin = memberFile->getYmin();
		DataUnit = memberFile->getDataUnits();
		GridUnit = memberFile->getGridUnits();
	}


	for(int i=0; i<m_iItemNum; i++)
	{
		float *alphaCutIntegrationBuf = new float[nrow*ncol];
		for(int irow=0;irow<nrow;irow++)
			for(int icol=0;icol<ncol;icol++)		 	 
			{		
				alphaCutIntegrationBuf[irow*ncol+icol] = 0;
			}
			for(int j=0; j<m_vecItem[i]; j++)
			{

				CThreeDR* memberFile = new CThreeDR();

				CString strMemberFile;
				strMemberFile.Format("%s%s%d%s%d%s", this->m_strResultDir, "\\fcm_result\\", m_vecItem[i], "_clusters\\class", j+1, ".3dr");


				int readflag = memberFile->readin(strMemberFile.GetBuffer(100), 0);
				if(readflag > 0)
			    {
				   MessageBox("Error in reading file.");
				   return;
			    }
				else
			    {	
				   for(int irow=0;irow<nrow;irow++)
					 for(int icol=0;icol<ncol;icol++)		 	 
					 {

						 if( memberFile->getDataBuf()[irow*ncol+icol]>=m_dAlpha)
						 {
							 alphaCutIntegrationBuf[irow*ncol+icol] += 1*(j+1);
						 }
						 else 
							 alphaCutIntegrationBuf[irow*ncol+icol] += 0*(j+1);			  
					 }  

			 }

			 delete memberFile;
			}


			//set integrated alpha cut file
			CString strOutputFileName;
			strOutputFileName.Format("%s%s%d%s", m_strResultDir,"\\alpha_cut\\", m_vecItem[i], "_alpha_cut.3dr");
			CThreeDR* output = new CThreeDR;
			output->setNumberOfRows(nrow);
			output->setNumberOfColumns(ncol);
			output->setXmin(XMin);
			output->setYmin(YMin);
			output->setCellSize(CellSize);
			output->setNullDataValue(NOFLOATVALUE);
			output->setDataName(extractFileName(strOutputFileName).GetBuffer(1));
			output->setDataUnits(DataUnit);
			output->setGridUnits(GridUnit);
			output->setDataBuf(alphaCutIntegrationBuf);
			output->calcStat();
			output->writeout(strOutputFileName.GetBuffer(),0);
			delete output;			



	}



	CEnvDatabase *pAlphaCutDatabase = new CEnvDatabase();
	for(int i=0; i<m_iItemNum; i++)
	{

		CString strLayerName;
		strLayerName.Format("%d%s",m_vecItem[i],"_alpha_cut");		

		CString strLayerPath;
		strLayerPath.Format("%s%s%d%s",m_strResultDir,"\\alpha_cut\\", m_vecItem[i], "_alpha_cut.3dr");	

		CEnvLayer *pLayer = new CEnvLayer(i,strLayerName, strLayerPath);
		if(pLayer->getLayerID() != -1)
		{ 
			pAlphaCutDatabase->AddLayer(pLayer);
		}
		else 
		{
			delete pLayer;
			pLayer = NULL;
			CString msg;
			msg.Format("Error in reading %s.",strLayerPath);
			AfxMessageBox(msg);
			return;
			//layerNum--;
		}
	}



	m_ProgressBar.SetPos(65);



	int pattern[MAX_ITEM_NUM];	
	//int orderedPattern[60000];
	int stability[MAX_ITEM_NUM];
	int patternNum=0;
	for(int i=0;i<MAX_ITEM_NUM;i++)
	{
		pattern[i]=-3;  
	}
	for(int i=0;i<MAX_ITEM_NUM;i++)
	{
		stability[i] = 0;
	}
	//int layerNum = pAlphaCutDatabase->getLayerNum();

	//int col=pAlphaCutDatabase->m_pTheSetting->getColNum();
	//int row=pAlphaCutDatabase->m_pTheSetting->getRowNum();


	//	float *resultBuf = new float[nrow*ncol];
	int num = 0;
	for(int irow=0;irow<nrow;irow++)
		for(int icol=0;icol<ncol;icol++)		 	 
		{	


			m_ProgressBar.SetPos(65+num*30.0f/(float)(nrow*ncol));

			num++;

			int iDifferent=0;
			for(int n=0;n<patternNum;n++)
			{
				//bool tag=true;
				int matchNum = 0;
				int zeroNum = 0;
				int mismatchNum1 = 0;
				int mismatchNum2 = 0;
				int m;
				for(int i=0;i<m_iItemNum;i++)
				{			
					/*if(pattern[n*m_iItemNum+i] == 0)					  
					existingZeroNum ++;					  
					*/
					if(fabs(pAlphaCutDatabase->getLayer(i)->m_pTheData->getData(irow,icol) - 0) <VERYSMALL && pattern[n*m_iItemNum+i] == 0 )					  
						zeroNum ++;
					else if(fabs(pAlphaCutDatabase->getLayer(i)->m_pTheData->getData(irow,icol) - pattern[n*m_iItemNum+i]) < VERYSMALL)
						matchNum ++;



					else if(fabs(pAlphaCutDatabase->getLayer(i)->m_pTheData->getData(irow,icol) - 0 ) >= VERYSMALL && pattern[n*m_iItemNum+i] == 0)				     
						mismatchNum1 ++;	

					else if(fabs(pAlphaCutDatabase->getLayer(i)->m_pTheData->getData(irow,icol) - 0) < VERYSMALL && pattern[n*m_iItemNum+i] != 0)
						mismatchNum2 ++;

					/*if(pAlphaCutDatabase->getLayer(i)->m_pTheData->getData(irow,icol) != pattern[n*m_iItemNum+i] && pattern[n*m_iItemNum+i] == 0)
					mismatch1 ++;

					if(pAlphaCutDatabase->getLayer(i)->m_pTheData->getData(irow,icol) != pattern[n*m_iItemNum+i] && pattern[n*m_iItemNum+i] == 0)
					mismatch2 ++;*/

				}

				if(zeroNum + matchNum + mismatchNum1 == m_iItemNum && mismatchNum1 > 0 )  //this is higher level pattern
				{

					for(int i=0;i<m_iItemNum;i++)	   
						pattern[n*m_iItemNum+i] = int(pAlphaCutDatabase->getLayer(i)->m_pTheData->getData(irow,icol));			  
					stability[n] = matchNum + mismatchNum1;

				}
				else if(zeroNum + matchNum + mismatchNum2 == m_iItemNum && mismatchNum2 > 0 )  //this is lower level pattern
					m = 0; //do nothing


				else if(zeroNum + matchNum == m_iItemNum)
					m = 0;  //do nothing
				else
					iDifferent++;
			}

			if(iDifferent == patternNum)  //this is totally different pattern
			{
				for(int i=0;i<m_iItemNum;i++)	
				{
					pattern[patternNum*m_iItemNum+i] = int(pAlphaCutDatabase->getLayer(i)->m_pTheData->getData(irow,icol));	
					if(pattern[patternNum*m_iItemNum+i] != 0)
						stability[patternNum]++; 
				}

				patternNum++;
			}
		}


	
	for(int i=0; i<patternNum; i++)
	    for(int j=i+1; j<patternNum;j++)
		{
			bool tag = true;
			for(int m=0;m<m_iItemNum;m++)	
			{
				if(pattern[i*m_iItemNum+m] != pattern[j*m_iItemNum+m])
				{ 
					tag = false;
					break;
				}

			}
			if(tag == true)  //the same pattern found, move
			{
				for(int k=j;k<patternNum-1;k++)
				{
					for(int m=0;m<m_iItemNum;m++)	
					{
						pattern[k*m_iItemNum+m] = pattern[(k+1)*m_iItemNum+m];

					}
					stability[k] = stability[k+1]; 


				}
				patternNum--;
				j--;


			}
		}


	m_ProgressBar.SetPos(95);

    int area[MAX_ITEM_NUM];



	vector<CString > vecAllDesignedPnts;
	vector<int > vecAllDesignedPntPattenID;
	

	for(int i=0; i<patternNum; i++)
	{
	

		float *resultBuf = new float[nrow*ncol];

		float *resultBufAll = new float[nrow*ncol];


		int *patchBuf = new int[nrow*ncol];


		area[i] = 0;
		CEnvDatabase *pFuzzyDatabase = new CEnvDatabase();
		//open fuzzy membership maps for this pattern
		for(int j=0; j<m_iItemNum;j++)
		{
			if(pattern[i*m_iItemNum+j]!=0)
			{
				CString strLayerName;
				strLayerName.Format("%d%s", m_vecItem[j],"_membership_map");		

				CString strLayerPath;
				strLayerPath.Format("%s%s%d%s%d%s", m_strResultDir, "\\fcm_result\\", m_vecItem[j], "_clusters\\class",pattern[i*m_iItemNum+j], ".3dr");

				CEnvLayer *pLayer = new CEnvLayer(i,strLayerName, strLayerPath);
				if(pLayer->getLayerID() != -1)
				{ 
					pFuzzyDatabase->AddLayer(pLayer);
				}
				else 
				{
					delete pLayer;
					pLayer = NULL;
					CString msg;
					msg.Format("Error in reading %s.",strLayerPath);
					AfxMessageBox(msg);
					return;
					//layerNum--;
				}
			}

		}


		for(int irow=0; irow<nrow; irow++)
			for(int icol=0; icol<ncol; icol++)
			{	
				resultBufAll[irow*ncol+icol]=0;
				int temp = 0;
				for(int j=0; j<m_iItemNum; j++)
			    {  
					if(pattern[i*m_iItemNum+j]!=0)
				    {
						resultBufAll[irow*ncol+icol]+= pFuzzyDatabase->getLayer(temp)->getData(irow,icol);

					    temp ++; 
					}
				}

		        resultBufAll[irow*ncol+icol] = resultBufAll[irow*ncol+icol]/stability[i];


			}
		//output .3dr for every pattern 	  
		CString strOutputFileName;
		strOutputFileName.Format("%s%s%d%s", m_strResultDir, "\\patterns_all\\pattern_all", i+1, ".3dr");
		CThreeDR* outputAll = new CThreeDR;
		outputAll ->setNumberOfRows(nrow);
		outputAll ->setNumberOfColumns(ncol);
		outputAll ->setXmin(XMin);
		outputAll ->setYmin(YMin);
		outputAll ->setCellSize(CellSize);
		outputAll ->setNullDataValue(NOFLOATVALUE);
		outputAll ->setDataName(extractFileName(strOutputFileName).GetBuffer(1));
		outputAll ->setDataUnits(DataUnit);
		outputAll ->setGridUnits(GridUnit);
		outputAll ->setDataBuf(resultBufAll);
		outputAll ->calcStat();
		outputAll ->writeout(strOutputFileName.GetBuffer(),0);
		delete outputAll;			//write 3dr file




		bool tag;
        //assign 
		for(int irow=0;irow<nrow;irow++)
			for(int icol=0;icol<ncol;icol++)	
			{
				tag = true;
				resultBuf[irow*ncol+icol]=0;

				int temp = 0;

				for(int j=0;j<m_iItemNum;j++)	
				{

					if(pAlphaCutDatabase->getLayer(j)->m_pTheData->getData(irow,icol)!=  pattern[i*m_iItemNum+j])
					{
						tag = false;
						break;
					}
					else if(pattern[i*m_iItemNum+j]!=0)
					{
						resultBuf[irow*ncol+icol]+= pFuzzyDatabase->getLayer(temp)->getData(irow,icol);

						temp ++; 
					}
				}
				if(tag == true)  //the pixel match with the pattern
				{
					area[i] ++;
					resultBuf[irow*ncol+icol] = resultBuf[irow*ncol+icol]/stability[i];

				}
				else 
				{
					resultBuf[irow*ncol+icol] = NOFLOATVALUE;

				}
			}     




        //compute the statistics patches
		int patchID = 0;

		for(int irow=0;irow<nrow;irow++)
			for(int icol=0;icol<ncol;icol++)
			{
				patchBuf[irow*ncol+icol] =0;
			}

		for(int irow=0;irow<nrow;irow++)
			for(int icol=0;icol<ncol;icol++)
			{
				if(resultBuf[irow*ncol+icol] > NOFLOATVALUE && patchBuf[irow*ncol+icol] == 0)  //8 neighborhood
				{
					patchID++;
					patchBuf[irow*ncol+icol] = patchID;
					connectPatch(irow, icol, resultBuf, patchBuf, patchID, nrow, ncol);

				}

			}


		float *patchArea = new float[patchID];
		for(int n=0; n<patchID; n++)
		{
		    patchArea[n] = 0;
			for(int irow=0;irow<nrow;irow++)
				for(int icol=0;icol<ncol;icol++)
				  if(patchBuf[irow*ncol+icol] == n+1)							
					patchArea[n] ++;
		}

		float minArea = nrow * ncol;
		float maxArea = 0;

		for(int n=0; n<patchID; n++)
		{
			if(patchArea[n] < minArea)					
				minArea = patchArea[n];					
			if(patchArea[n] > maxArea)
			    maxArea = patchArea[n];
		}




        float *highestValues = new float[m_iNumOfPnt];
		int *rowsHighest = new int[m_iNumOfPnt];
		int *colsHighest = new int[m_iNumOfPnt];



		for(int k=0; k<m_iNumOfPnt; k++)
		{
			float max = 0;
			int curRow = -1;
			int curCol = -1;


			for(int irow=0;irow<nrow;irow++)
				for(int icol=0;icol<ncol;icol++)
				{
					bool visited = false;

					for(int m=0; m<k; m++)
					{

						if(sqrt(pow(irow - rowsHighest[m], 2) + pow(icol-colsHighest[m], 2)) * CellSize < this->m_dDistThreshold)
						{
							visited = true;
							break;
						}
					}
					if(visited == false)
					{
						if(resultBuf[irow*ncol+icol] > max)
						{

							max = resultBuf[irow*ncol+icol];
							curRow = irow;
							curCol = icol;
						}
					}
				}



				highestValues[k]  = max;
				rowsHighest[k] = curRow;
				colsHighest[k] = curCol;


				

				if(curRow != -1)
				{

					CString onePnt;
					onePnt.Format("%d,%d,%d,%f,%f,%f,%f,%d,%f,%f", stability[i], i+1 ,area[i], XMin + CellSize * (colsHighest[k]+0.5), YMin + CellSize * (nrow - rowsHighest[k]-0.5),  highestValues[k], patchArea[patchBuf[curRow*ncol + curCol]-1], patchID, maxArea, minArea);
				    
				    vecAllDesignedPnts.push_back(onePnt);
					vecAllDesignedPntPattenID.push_back(i+1);
				}

	

		
			    //fprintf(fp, "\n%d, %d, %d,  %f, %f, %f, %f, %d, %f, %f",stability[i], i+1 ,area[i], XMin + CellSize * colsHighest[k], YMin + CellSize * rowsHighest[k],  highestValues[k], patchArea[patchBuf[curRow*ncol + curCol]-1], patchID, maxArea, minArea);



		}



		//output .3dr for every pattern 	  
		//CString strOutputFileName;
		strOutputFileName.Format("%s%s%d%s", m_strResultDir, "\\patterns\\pattern", i+1, ".3dr");
		CThreeDR* output = new CThreeDR;
		output->setNumberOfRows(nrow);
		output->setNumberOfColumns(ncol);
		output->setXmin(XMin);
		output->setYmin(YMin);
		output->setCellSize(CellSize);
		output->setNullDataValue(NOFLOATVALUE);
		output->setDataName(extractFileName(strOutputFileName).GetBuffer(1));
		output->setDataUnits(DataUnit);
		output->setGridUnits(GridUnit);
		output->setDataBuf(resultBuf);
		output->calcStat();
		output->writeout(strOutputFileName.GetBuffer(),0);
		delete output;			//write 3dr file



		if(pFuzzyDatabase != NULL)
		{
			delete pFuzzyDatabase;
			pFuzzyDatabase = NULL;
		}

	    if(patchBuf != NULL)
	   {
		delete patchBuf;
		patchBuf = NULL;
	   }



	}

	//fclose(fp);




	if(pAlphaCutDatabase != NULL)
	{
		delete pAlphaCutDatabase;
		pAlphaCutDatabase = NULL;
	}


	m_ProgressBar.SetPos(98);

		


	int patternIDs[1000];

	for(int i=0; i<patternNum; i++)
	{
		patternIDs[i] = i+1;
	}

	int r;

	int temp;

	for(int i=0; i<patternNum-1; i++)
	{
		r=i;
		for(int j=i+1; j<patternNum; j++)
		{
			if(stability[r]<stability[j])
				r = j;
			else if(stability[r]==stability[j] && area[r]<area[j])
				r = j;
		}
		if(r!=i)  //exchange
		{
			for(int m=0; m<m_iItemNum; m++)
			{
				temp = pattern[i*m_iItemNum+m];
				pattern[i*m_iItemNum+m] = pattern[r*m_iItemNum+m];
				pattern[r*m_iItemNum+m]= temp;

			}
			temp=stability[i];
			stability[i] = stability[r];
			stability[r]= temp;

			temp=area[i];
			area[i] = area[r];
			area[r]= temp;


			temp=patternIDs[i];
			patternIDs[i] = patternIDs[r];
			patternIDs[r]= temp;


		}
	}


	CString strReportFileName;
	strReportFileName = m_strResultDir + "\\pattern_List.csv";;


	FILE* fp;

	//output csv file 
	if ((fp=fopen(strReportFileName.GetBuffer(), "w"))==NULL) 
	{
		MessageBox("Can't open result file.");
		return;

	}

	//fprintf(fp, "Pattern List ");
	fprintf(fp, "Stability,ID,Name,Area,Average Fuzzy Membership Map");


	for (i=0; i<patternNum; i++)
	{
		CString name = "";

		for(int m=0; m<m_iItemNum; m++)
		{
			if(pattern[i*m_iItemNum+m]!=0)
			{
				if(name!="")				 //this first
				{
					CString str;
					str.Format("%s%d%s%d", "-", m_vecItem[m], "Class", pattern[i*m_iItemNum+m]);
					name += str;
				}
				else
				{
					CString str;
					str.Format("%d%s%d", m_vecItem[m], "Class", pattern[i*m_iItemNum+m]);
					name += str;
				}

			}
		}


		CString averageFuzzyFile;
		averageFuzzyFile.Format("%s%d%s","pattern",patternIDs[i],".3dr");

		fprintf(fp, "\n%d,%d,%s,%d,%s", stability[i],patternIDs[i] ,name ,area[i], averageFuzzyFile);


	}

	fclose(fp);





	//FILE* fp;
	CString strPntFileName = m_OutputFileName;



	//output csv file 
	if ((fp=fopen(strPntFileName.GetBuffer(), "w"))==NULL) 
	{
		MessageBox("Can't open file to write.");
		return;

	}

	//fprintf(fp, "The Point List ");

	fprintf(fp, "Stability,Pattern ID,Total Area,Recommended X,Recommended Y,Ave. Membership,Area of Patch it Belongs to,No. of Patches in The Pattern,Max Area,Min Area,Actual X,Actual Y,Soil Type");



	for (i=0; i<patternNum; i++)
	{

		for(int k = 0; k < vecAllDesignedPntPattenID.size(); k++)
		{
			if (vecAllDesignedPntPattenID[k] == patternIDs[i])     
			{

				CString str = "\n" + vecAllDesignedPnts[k];

				fprintf(fp,str.GetBuffer() );
			}

		}

	}


	fclose(fp);



	m_ProgressBar.SetPos(100);

//	MessageBox("Sampling strategy design is done!");



	//CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();	
	int s =MessageBox("Samples have been designed and saved. Do you want to view the designed sample file?", "View Sample File",MB_ICONQUESTION|MB_YESNOCANCEL);//6: yes; 7: no; 2: cancel
	if(s == 6)  //yes
	{
		CString openResult = "Start Excel " + strPntFileName;

		system(openResult.GetBuffer());

	}


	if(m_pEnvDatabase != NULL)
	{
		delete m_pEnvDatabase;
		m_pEnvDatabase = NULL;
	}


	OnOK();

	//m_ProgressBar.SetPos(0);


}


void CPurposiveSampleDesignDlg::OnBnClickedCancel()
{

	if(m_pEnvDatabase != NULL)
	{
		delete m_pEnvDatabase;
		m_pEnvDatabase = NULL;
	}

	OnCancel();
}


void CPurposiveSampleDesignDlg::BubbleSort()
{
	int temp;
	for(int i=0;i<this->m_iItemNum;i++)
	{
		for(int j=0;j<m_iItemNum-1;j++)
		{
			if(m_vecItem[j]>m_vecItem[j+1])
			{
				temp=m_vecItem[j+1];
				m_vecItem[j+1]=m_vecItem[j];
				m_vecItem[j]=temp;
			}
		}
	}
}



void CPurposiveSampleDesignDlg::connectPatch(int irow, int icol, float *resultBuf, int *patchBuf, int patchID, int nrow, int ncol)
{
	int deltaRow[8] = {1, -1, 1, -1, 1, 0, -1, 0};

	int deltaCol[8] = {1, -1,-1, 1, 0, 1, 0, -1};

	for(int i=0; i<8; i++)
	{
		if(irow + deltaRow[i] >=0 && irow + deltaRow[i]< nrow && icol+deltaCol[i] >=0 && icol+deltaCol[i]<ncol)
		{
			if(resultBuf[(irow + deltaRow[i])*ncol+icol + deltaCol[i]] > NOFLOATVALUE && patchBuf[(irow + deltaRow[i])*ncol+ icol + deltaCol[i]] == 0)  //8 neighborhood
			{
				patchBuf[(irow + deltaRow[i])*ncol + icol+deltaCol[i]] = patchID;
				connectPatch(irow + deltaRow[i],icol+deltaCol[i], resultBuf, patchBuf, patchID, nrow, ncol);

			}
		}
	}

}







void CPurposiveSampleDesignDlg::OnBnClickedButtonSampleTable()
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "csv Files (*.csv)|*.csv|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(FALSE, "csv", ".csv", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK){
		m_OutputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

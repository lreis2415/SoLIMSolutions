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
// PurposiveSampleFCMDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SoLIM.h"
#include "PurposiveSampleFCMDlg.h"
#include ".\PurposiveSampleFCMDlg.h"
#include "FCM.h"
#include "NewClusterNumDlg.h"
#include "GetDirDlg.h"

// CPurposiveSampleFCMDlg 对话框

IMPLEMENT_DYNAMIC(CPurposiveSampleFCMDlg, CDialog)
CPurposiveSampleFCMDlg::CPurposiveSampleFCMDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPurposiveSampleFCMDlg::IDD, pParent)
, m_iKernel(1)
, m_dWeight(2)
, m_iIteration(50)
, m_strResultDir(_T(""))
, m_dEPS(0.01)
, m_MaskValue(0)
, m_iMinClusterNum(2)
, m_iMaxClusterNum(20)
{
	this->m_iItemNum = 0;
	m_pEnvDatabase = NULL;
}

CPurposiveSampleFCMDlg::~CPurposiveSampleFCMDlg()
{
}

void CPurposiveSampleFCMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LAYER, m_LayerList);

	DDX_Check(pDX, IDC_MaskChk, m_Mask);
	DDX_Text(pDX, IDC_MaskFileFld, m_MaskFileName);
	DDX_Text(pDX, IDC_MaskValueFld, m_MaskValue);


	DDX_Text(pDX, IDC_EDIT_KENEL, m_iKernel);
	DDX_Text(pDX, IDC_EDIT_WEIGHT, m_dWeight);
	DDX_Text(pDX, IDC_EDIT_ITERATION, m_iIteration);

	DDX_Text(pDX, IDC_EDIT_RESULT_DIRECTORY, m_strResultDir);
	DDX_Text(pDX, IDC_EDIT_EPS, m_dEPS);
	DDX_Text(pDX, IDC_EDIT_MIN_CLUSTER_NUMBER, m_iMinClusterNum);
	DDX_Text(pDX, IDC_EDIT_CLUSTER_NUMBER, m_iMaxClusterNum);

		DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
}


BEGIN_MESSAGE_MAP(CPurposiveSampleFCMDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_LAYER, OnBnClickedButtonAddLayer)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_LAYER, OnBnClickedButtonRemoveLayer)
	ON_BN_CLICKED(IDC_MaskFileBtn, OnMaskFileBtn)
	ON_BN_CLICKED(IDC_MaskChk, OnMaskChk)


	ON_BN_CLICKED(IDC_BUTTON_DIR, OnBnClickedButtonDir)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()



BOOL CPurposiveSampleFCMDlg::OnInitDialog()
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


	return TRUE;  

}



void CPurposiveSampleFCMDlg::OnMaskChk(){
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

void CPurposiveSampleFCMDlg::OnMaskFileBtn(){
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




void CPurposiveSampleFCMDlg::OnBnClickedButtonAddLayer()
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

void CPurposiveSampleFCMDlg::OnBnClickedButtonRemoveLayer()
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


void CPurposiveSampleFCMDlg::OnBnClickedButtonDir()
{
	UpdateData(TRUE);

	CGetDirDlg m_getDirDlg;

	CString str = m_getDirDlg.GetDirectory(this, "", "Choose Result Directory :");

	m_strResultDir = str;
	UpdateData(false);
}



int  CPurposiveSampleFCMDlg::CheckMask(bool* idBuf, int nrow, int ncol,CThreeDR *theData){ //Check if mask should be used
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
					else
						numOfValidPixel ++;
				}
			}
		}
		delete maskFile;
	}
	return numOfValidPixel;
}


void CPurposiveSampleFCMDlg::OnBnClickedOk()
{

	UpdateData(true);

	m_ProgressBar.ShowWindow(SW_SHOW);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

	this->m_iItemNum = m_iMaxClusterNum - m_iMinClusterNum +1;
	for(int i=this->m_iMinClusterNum; i<=this->m_iMaxClusterNum; i++)
	{
		this->m_vecItem.push_back(i);
	}

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



	FCM* fcm = new FCM(m_pEnvDatabase,idBuf, NumOfValidPixel, this->m_iKernel,m_dWeight,this->m_dEPS, this->m_iIteration);

	
	fcm -> AssignScalingMatrix();

	for(int i=1;i<this->m_vecItem[0];i++)
	{
	
		fcm->setNumOfClasses(i);	
		fcm->IniMembershipMatrix();
		fcm->DestroyMatrix();
	}
		


     fcm->InitiatePixels();
	for(int i=0;i<m_iItemNum;i++)
	{
			fcm -> SamplePixels();
		CString str;
		//str.Format("%s%s%d%s, strResultDir, "\\",this->NumOfClasses, "_Centroid");
		//str =  + "fcm_result\\" + "class" + classes[i];
		str.Format("%s%s%d%s", m_strResultDir, "\\fcm_result\\", m_vecItem[i], "_clusters");
		CreateDirectory(str, NULL);
		fcm->setResultDir(str);
		fcm->setNumOfClasses(this->m_vecItem[i]);		

		fcm->CalClassesCentroid();
		fcm->CalEntropy();		
		fcm->WriteClassCentroidFile();
		fcm->WriteMembershipFiles(); //write 3dr 

		fcm->DestroyMatrix();
		m_ProgressBar.SetPos(5+ (i+1)*80/m_iItemNum);
	}

	delete fcm;

	if(idBuf != NULL)
	{
		delete idBuf;
		idBuf = NULL;
	}

	m_ProgressBar.SetPos(100);

	 MessageBox("FCM clustering is done!");
		m_ProgressBar.SetPos(0);



}


void CPurposiveSampleFCMDlg::OnBnClickedCancel()
{

	if(m_pEnvDatabase != NULL)
	{
		delete m_pEnvDatabase;
		m_pEnvDatabase = NULL;
	}

	OnCancel();
}






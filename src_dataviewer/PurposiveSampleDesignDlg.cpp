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
, m_dAlpha(0.7)
, m_strResultDir(_T(""))
, m_iItemNum(0)
{


}

CPurposiveSampleDesignDlg::~CPurposiveSampleDesignDlg()
{
}

void CPurposiveSampleDesignDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_CLUSTER_NUM, m_ClusterNumList);


	DDX_Text(pDX, IDC_EDIT_ALPHA, m_dAlpha);
	DDX_Text(pDX, IDC_EDIT_RESULT_DIRECTORY, m_strResultDir);

		DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);

}


BEGIN_MESSAGE_MAP(CPurposiveSampleDesignDlg, CDialog)

	ON_BN_CLICKED(IDC_BUTTON_ADD_CLUSTER_NUMBER, OnBnClickedButtonAddClusterNumber)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_CLUSTER_NUMBER, OnBnClickedButtonRemoveClusterNumber)
	ON_BN_CLICKED(IDC_BUTTON_DIR, OnBnClickedButtonDir)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()



BOOL CPurposiveSampleDesignDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here

	m_ClusterNumList.InsertColumn(1,CString("Cluster Number"));
	m_ClusterNumList.SetColumnWidth(0,500);
	m_ClusterNumList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);


	m_ClusterNumList.DeleteAllItems();
	return TRUE;  

}




void CPurposiveSampleDesignDlg::OnBnClickedButtonAddClusterNumber()
{

	int itemCount = m_ClusterNumList.GetItemCount();
	CNewClusterNumDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		//CString strTempX;
		//strTempX.Format("%d",dlg.m_iClusterNum);
		//m_ClusterNumList.InsertItem(LVIF_TEXT|LVIF_STATE,itemCount, strTempX, 0, LVIS_SELECTED,0, 0);
		this->m_vecItem.push_back(dlg.m_iClusterNum);

		this->m_iItemNum++;
		this->BubbleSort();
		m_ClusterNumList.DeleteAllItems();

		for(int i=0;i<m_iItemNum;i++)
		{
			CString strTemp;

			strTemp.Format("%d",m_vecItem[i]); 
			m_ClusterNumList.InsertItem(LVIF_TEXT|LVIF_STATE,i, strTemp, 0, LVIS_SELECTED,0, 0);
		}

	}

}

void CPurposiveSampleDesignDlg::OnBnClickedButtonRemoveClusterNumber()
{
	if(m_ClusterNumList.GetItemCount()>0)
	{

		POSITION pos=m_ClusterNumList.GetFirstSelectedItemPosition();
		int iDeleteIndex=m_ClusterNumList.GetNextSelectedItem(pos);

		m_vecItem.erase(m_vecItem.begin()+iDeleteIndex);
		m_iItemNum--;
		BubbleSort();

		m_ClusterNumList.DeleteAllItems();

		for(int i=0;i<m_iItemNum;i++)
		{
			CString strTemp;

			strTemp.Format("%d",m_vecItem[i]); 
			m_ClusterNumList.InsertItem(LVIF_TEXT|LVIF_STATE,i, strTemp, 0, LVIS_SELECTED,0, 0);
		}



	}
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


	str.Format("%s%s", m_strResultDir, "\\alpha_cut");
	CreateDirectory(str, NULL);

	str.Format("%s%s", m_strResultDir, "\\patterns");
	CreateDirectory(str, NULL);

	int nrow,ncol;
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
			delete output;			//write 3dr file*/



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



	m_ProgressBar.SetPos(10);



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
			

			m_ProgressBar.SetPos(10+num*60.0f/(float)(nrow*ncol));
			
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


				m_ProgressBar.SetPos(75);




		int area[MAX_ITEM_NUM];


		for(int i=0; i<patternNum; i++)
		{
			float *resultBuf = new float[nrow*ncol];

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





				//output .3dr for every pattern 	  
				CString strOutputFileName;
				strOutputFileName.Format("%s%s%d%s", m_strResultDir, "\\patterns\\pattern_all", i+1, ".3dr");
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
		}


		if(pAlphaCutDatabase != NULL)
		{
			delete pAlphaCutDatabase;
			pAlphaCutDatabase = NULL;
		}


		m_ProgressBar.SetPos(90);

		//out put pattern list
		FILE* fp;


		str = m_strResultDir + "\\list.txt";
		if((fp=fopen(str.GetBuffer(), "w")) == NULL)
		{
			AfxMessageBox("Write file error");
			return;	
		}

		for(int i=0;i<patternNum;i++)
		{
			fprintf(fp, "%d   ",i+1);
			for(int k=0;k<m_iItemNum;k++)
			{
				fprintf(fp, "%d ", pattern[i*m_iItemNum+k]);
			}
			fprintf(fp,"\n");
		}
		fclose(fp);



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
		strReportFileName = m_strResultDir + "\\sample_design.csv";;


		//output csv file 
		if ((fp=fopen(strReportFileName.GetBuffer(), "w"))==NULL) 
		{
			MessageBox("Can't open result file.");
			return;

		}

		fprintf(fp, "The result List ");
		fprintf(fp, "\n    Stability,ID, Name,Area,Average Fuzzy Membership Map");


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

			fprintf(fp, "\n%d, %d, %s, %d, %s", stability[i],patternIDs[i] ,name ,area[i], averageFuzzyFile);


		}

		fclose(fp);

			m_ProgressBar.SetPos(100);

	   MessageBox("Sampling strategy design is done!");

	   	m_ProgressBar.SetPos(0);
		// OnOK();
		//return false;

}


void CPurposiveSampleDesignDlg::OnBnClickedCancel()
{

	/*if(m_pEnvDatabase != NULL)
	{
	delete m_pEnvDatabase;
	m_pEnvDatabase = NULL;
	}*/

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






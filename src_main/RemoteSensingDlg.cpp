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
// RemoteSensingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SoLIM.h"
#include "RemoteSensingDlg.h"
#include ".\remotesensingdlg.h"
#include "GetDirDlg.h"




// CRemoteSensingDlg 对话框

IMPLEMENT_DYNAMIC(CRemoteSensingDlg, CDialog)
CRemoteSensingDlg::CRemoteSensingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRemoteSensingDlg::IDD, pParent)
, m_iNumOfDays(0)
, m_strResultFolder(_T(""))
{
	m_pData = NULL;
	m_curSelTab = 0;

}

CRemoteSensingDlg::~CRemoteSensingDlg()
{
	if(m_pData!=NULL)
	{
		delete []m_pData;
		m_pData = NULL;
	}
}

void CRemoteSensingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TAB_TIME_PHASES, m_tabDays);
	DDX_Text(pDX, IDC_EDIT_FOLDER, m_strResultFolder);
}


BEGIN_MESSAGE_MAP(CRemoteSensingDlg, CDialog)

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TIME_PHASES, OnTcnSelchangeTabDays)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_FOLDER, OnBnClickedButtonSelectFolder)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CRemoteSensingDlg 消息处理程序



BOOL CRemoteSensingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	

	
	CRect rc;
	m_tabDays.GetClientRect(rc);
	rc.top += 20;
	rc.bottom -= 8;
	rc.left += 8;
	rc.right -= 8; 

	m_tabDays.DeleteAllItems();

	m_page.DestroyWindow();

	m_page.m_iNumOfBands = m_bandsVec.size();


	m_page.Create(IDD_DIALOG_TIMEPHASE, &m_tabDays);
	m_page.MoveWindow(&rc);
	m_page.ShowWindow(SW_SHOW);

	m_pData = new CEnvDatabase[m_daysVec.size()];

	for (int i = 0; i< m_daysVec.size(); i++)
		for (int j = 0; j < m_bandsVec.size(); j++)
		{

			CEnvLayer *pLayer = new CEnvLayer(j, m_dataFileNameVec[i][j],  m_dataPathVec[i][j]);

			if(pLayer->getLayerID() != -1)
			{ 
				m_pData[i].AddLayer(pLayer);  

			}

			else 
			{
				delete pLayer;
				pLayer = NULL;
				CString msg;
				msg.Format("Error in reading %s. The layer will not be added",m_page.m_pEnvDatabase->getLayer(i)->getFileName());
				AfxMessageBox(msg);
			}	

		}







	for(int i=0; i<m_daysVec.size(); i++)
	{
		CString str;
		str.Format("Day %d",m_daysVec[i]); 
		this->m_tabDays.InsertItem(i, _T(str));

	}


		
	if(this->m_page.m_pEnvDatabase != NULL)
	{
		delete m_page.m_pEnvDatabase;
		m_page.m_pEnvDatabase = NULL;

	}

	for(int i=0; i<this->m_iNumOfBands; i++)
	{
		m_page.m_LayerList.SetItemText(i, 0, "");

		m_page.m_LayerList.SetItemText(i, 1, "");
					
	}

	m_page.m_iCurLayer = 0;

	m_page.m_pEnvDatabase = new CEnvDatabase();


	this->m_curSelTab = m_tabDays.GetCurSel();

	for(int i=0; i<m_pData[m_curSelTab].getLayerNum(); i++)
	{
		CEnvLayer *pLayer = new CEnvLayer(i,m_pData[m_curSelTab].getLayer(i)->getLayerName(), m_pData[m_curSelTab].getLayer(i)->getFileName());
		if(pLayer->getLayerID() != -1)
		{ 

			if(m_page.m_pEnvDatabase->AddLayer(pLayer))   //if add layer successfully
			{
				
				m_page.m_LayerList.SetItemText(m_page.m_iCurLayer, 0, m_pData[m_curSelTab].getLayer(i)->getLayerName());
				m_page.m_LayerList.SetItemText(m_page.m_iCurLayer, 1, m_pData[m_curSelTab].getLayer(i)->getFileName());
					m_page.m_iCurLayer ++;
			}
		}
		else 
		{
			delete pLayer;
			pLayer = NULL;
			CString msg;
			msg.Format("Error in reading %s. The layer will not be added",m_pData[m_curSelTab].getLayer(i)->getFileName());
			AfxMessageBox(msg);
		}	

	}



	return true;


}

void CRemoteSensingDlg::OnTcnSelchangeTabDays(NMHDR *pNMHDR, LRESULT *pResult)
{


	m_pData[m_curSelTab].DeleteAll(); 


	for(int i=0; i<m_page.m_pEnvDatabase->getLayerNum(); i++)
	{
		CEnvLayer *pLayer = new CEnvLayer(i,m_page.m_pEnvDatabase->getLayer(i)->getLayerName(), m_page.m_pEnvDatabase->getLayer(i)->getFileName());
		if(pLayer->getLayerID() != -1)
		{ 

			m_pData[m_curSelTab].AddLayer(pLayer);
		//	m_page.m_iCurLayer ++;

		}
		else 
		{
			delete pLayer;
			pLayer = NULL;
			CString msg;
			msg.Format("Error in reading %s. The layer will not be added",m_page.m_pEnvDatabase->getLayer(i)->getFileName());
			AfxMessageBox(msg);
		}	

	}





	this->m_page.m_pEnvDatabase->DeleteAll();


		
	if(this->m_page.m_pEnvDatabase != NULL)
	{
		delete m_page.m_pEnvDatabase;
		m_page.m_pEnvDatabase = NULL;

	}

	for(int i=0; i<this->m_iNumOfBands; i++)
	{
		m_page.m_LayerList.SetItemText(i, 0, "");


		m_page.m_LayerList.SetItemText(i, 1, "");
					
	}

//	m_page.m_LayerList.DeleteAllItems();
	m_page.m_iCurLayer = 0;

	m_page.m_pEnvDatabase = new CEnvDatabase();


	this->m_curSelTab = m_tabDays.GetCurSel();

	for(int i=0; i<m_pData[m_curSelTab].getLayerNum(); i++)
	{
		CEnvLayer *pLayer = new CEnvLayer(i,m_pData[m_curSelTab].getLayer(i)->getLayerName(), m_pData[m_curSelTab].getLayer(i)->getFileName());
		if(pLayer->getLayerID() != -1)
		{ 

			if(m_page.m_pEnvDatabase->AddLayer(pLayer))   //if add layer successfully
			{
				
				m_page.m_LayerList.SetItemText(m_page.m_iCurLayer, 0, m_pData[m_curSelTab].getLayer(i)->getLayerName());
				m_page.m_LayerList.SetItemText(m_page.m_iCurLayer, 1, m_pData[m_curSelTab].getLayer(i)->getFileName());
					m_page.m_iCurLayer ++;
			}
		}
		else 
		{
			delete pLayer;
			pLayer = NULL;
			CString msg;
			msg.Format("Error in reading %s. The layer will not be added",m_pData[m_curSelTab].getLayer(i)->getFileName());
			AfxMessageBox(msg);
		}	

	}



	*pResult = 0;
}






void CRemoteSensingDlg::OnBnClickedButtonSelectFolder()
{


	CGetDirDlg dirDlg;   
	m_strResultFolder = dirDlg.GetDirectory(this, NULL, "Choose a directory to save the membership files:");;   
	UpdateData(FALSE);
}

void CRemoteSensingDlg::OnBnClickedOk()
{

	OnOK();


	m_pData[m_curSelTab].DeleteAll();

	for(int i=0; i<m_page.m_pEnvDatabase->getLayerNum(); i++)
	{
		CEnvLayer *pLayer = new CEnvLayer(i,m_page.m_pEnvDatabase->getLayer(i)->getLayerName(), m_page.m_pEnvDatabase->getLayer(i)->getFileName());
		if(pLayer->getLayerID() != -1)
		{ 

			m_pData[m_curSelTab].AddLayer(pLayer);

		}
		else 
		{
			delete pLayer;
			pLayer = NULL;
			CString msg;
			msg.Format("Error in reading %s. The layer will not be added",m_page.m_pEnvDatabase->getLayer(i)->getFileName());
			AfxMessageBox(msg);
		}	

	}

	int row = m_pData[0].getLayer(0)->getRow();
	int col = m_pData[0].getLayer(0)->getCol();

	int numOfDays = this->m_iNumOfDays;

	int numOfBands = m_pData[0].getLayerNum();


	float* meanLL_Buf = new float[row * col];
	float* stdLL_Buf = new float[row * col];
	float* meanHL_Buf = new float[row * col];
	float* stdHL_Buf = new float[row * col];
	float* meanLH_Buf = new float[row * col];
	float* stdLH_Buf = new float[row * col];
	float* meanHH_Buf = new float[row * col];
	float* stdHH_Buf = new float[row * col];



	for(int i=0; i<row; i++)
		for(int j=0; j<col; j++)
		{
			if(numOfDays%2 == 0 && numOfBands%2 == 0)
			{
				float **image;
				image = new float*[numOfDays];

				for(int s=0; s<numOfDays; s++)
					image[s] = new float[numOfBands];


				float min = m_pData[0].getLayer(0)->getData(i, j);
				float max = m_pData[0].getLayer(0)->getData(i, j);

				for(int k=0; k<numOfDays; k++)
					for(int t=0; t<numOfBands; t++)
					{
						image[k][t] = m_pData[k].getLayer(t)->getData(i, j);
						if(image[k][t] < min)
							min = image[k][t];
						if(image[k][t] > max)
							max = image[k][t];
					}


					short **imageNew, **imageResult0, **imageResult;
					imageNew = new short*[numOfDays];

					imageResult0 = new short*[numOfDays];
					imageResult = new short*[numOfDays];

					for(int s=0; s<numOfDays; s++)
					{

						imageNew[s] = new short[numOfBands];
						imageResult0[s] = new short[numOfBands];		
						imageResult[s] = new short[numOfBands];
					}



					for(int k=0; k<numOfDays; k++)
						for(int t=0; t<numOfBands; t++)
						{
							imageNew[k][t] = (image[k][t] - min)*255/(max-min);
						}




						DWT_Once(imageNew, imageResult0, imageResult, numOfDays, numOfDays/2, numOfBands, numOfBands/2);

						float mean = 0; 
						float std = 0;

						for(int m=0; m<numOfDays/2; m++)
							for(int n=0; n<numOfBands/2; n++)
							{
								mean += imageResult[m][n];
								std += pow(imageResult[m][n], 2);
							}

							meanLL_Buf[i*col+j] = mean/(numOfDays*numOfBands/4);
							stdLL_Buf[i*col+j] = sqrt(std/(numOfDays*numOfBands/4) - pow(meanLL_Buf[i*col+j], 2));

							mean = 0; 
							std = 0;

							for(int m=0; m<numOfDays/2; m++)
								for(int n=numOfBands/2; n<numOfBands; n++)
								{
									mean += imageResult[m][n];
									std += pow(imageResult[m][n], 2);
								}

								meanHL_Buf[i*col+j] = mean/(numOfDays*numOfBands/4);
								stdHL_Buf[i*col+j] = sqrt(std/(numOfDays*numOfBands/4) - pow(meanHL_Buf[i*col+j], 2));

								mean = 0; 
								std = 0;

								for(int m=numOfDays/2; m<numOfDays ; m++)
									for(int n=0; n<numOfBands/2; n++)
									{
										mean += imageResult[m][n];
										std += pow(imageResult[m][n], 2);
									}

									meanLH_Buf[i*col+j] = mean/(numOfDays*numOfBands/4);
									stdLH_Buf[i*col+j] = sqrt(std/(numOfDays*numOfBands/4) - pow(meanLH_Buf[i*col+j], 2));


									mean = 0; 
									std = 0;

									for(int m=numOfDays/2; m<numOfDays ; m++)
										for(int n=numOfBands/2; n<numOfBands; n++)
										{
											mean += imageResult[m][n];
											std += pow(imageResult[m][n], 2);
										}

										meanHH_Buf[i*col+j] = mean/(numOfDays*numOfBands/4);
										stdHH_Buf[i*col+j] = sqrt(std/(numOfDays*numOfBands/4) - pow(meanHH_Buf[i*col+j], 2));



			}

		    else if(numOfDays%2 != 0 && numOfBands%2 == 0)
			{
				float **image;
				image = new float*[numOfDays + 1];

				for(int s=0; s<numOfDays + 1; s++)
					image[s] = new float[numOfBands];


				float min = m_pData[0].getLayer(0)->getData(i, j);
				float max = m_pData[0].getLayer(0)->getData(i, j);

				for(int k=0; k<numOfDays; k++)
					for(int t=0; t<numOfBands; t++)
					{
						image[k][t] = m_pData[k].getLayer(t)->getData(i, j);
						if(image[k][t] < min)
							min = image[k][t];
						if(image[k][t] > max)
							max = image[k][t];
					}

			    for(int t=0; t<numOfBands; t++)
					{
						image[numOfDays][t] = m_pData[numOfDays-1].getLayer(t)->getData(i, j);
					
					}


					short **imageNew, **imageResult0, **imageResult;
					imageNew = new short*[numOfDays+1];

					imageResult0 = new short*[numOfDays+1];
					imageResult = new short*[numOfDays+1];

					for(int s=0; s<numOfDays+1; s++)
					{

						imageNew[s] = new short[numOfBands];
						imageResult0[s] = new short[numOfBands];		
						imageResult[s] = new short[numOfBands];
					}



					for(int k=0; k<numOfDays+1; k++)
						for(int t=0; t<numOfBands; t++)
						{
							imageNew[k][t] = (image[k][t] - min)*255/(max-min);
						}




						DWT_Once(imageNew, imageResult0, imageResult, numOfDays + 1, (numOfDays + 1)/2, numOfBands, numOfBands/2);

						float mean = 0; 
						float std = 0;

						for(int m=0; m<(numOfDays+1)/2; m++)
							for(int n=0; n<numOfBands/2; n++)
							{
								mean += imageResult[m][n];
								std += pow(imageResult[m][n], 2);
							}

							meanLL_Buf[i*col+j] = mean/((numOfDays + 1)*numOfBands/4);
							stdLL_Buf[i*col+j] = sqrt(std/((numOfDays+1)*numOfBands/4) - pow(meanLL_Buf[i*col+j], 2));

							mean = 0; 
							std = 0;

							for(int m=0; m<(numOfDays+1)/2; m++)
								for(int n=numOfBands/2; n<numOfBands; n++)
								{
									mean += imageResult[m][n];
									std += pow(imageResult[m][n], 2);
								}

								meanHL_Buf[i*col+j] = mean/((numOfDays+1)*numOfBands/4);
								stdHL_Buf[i*col+j] = sqrt(std/((numOfDays+1)*numOfBands/4) - pow(meanHL_Buf[i*col+j], 2));

								mean = 0; 
								std = 0;

								for(int m=(numOfDays +1)/2; m<numOfDays + 1 ; m++)
									for(int n=0; n<numOfBands/2; n++)
									{
										mean += imageResult[m][n];
										std += pow(imageResult[m][n], 2);
									}

									meanLH_Buf[i*col+j] = mean/((numOfDays+1)*numOfBands/4);
									stdLH_Buf[i*col+j] = sqrt(std/((numOfDays+1)*numOfBands/4) - pow(meanLH_Buf[i*col+j], 2));


									mean = 0; 
									std = 0;

									for(int m=(numOfDays+1)/2; m<numOfDays+1 ; m++)
										for(int n=numOfBands/2; n<numOfBands; n++)
										{
											mean += imageResult[m][n];
											std += pow(imageResult[m][n], 2);
										}

										meanHH_Buf[i*col+j] = mean/((numOfDays+1)*numOfBands/4);
										stdHH_Buf[i*col+j] = sqrt(std/((numOfDays+1)*numOfBands/4) - pow(meanHH_Buf[i*col+j], 2));

			}
			else if(numOfDays%2 == 0 && numOfBands%2 != 0)
			{
					float **image;
				image = new float*[numOfDays];

				for(int s=0; s<numOfDays; s++)
					image[s] = new float[numOfBands + 1];


				float min = m_pData[0].getLayer(0)->getData(i, j);
				float max = m_pData[0].getLayer(0)->getData(i, j);

				for(int k=0; k<numOfDays; k++)
					for(int t=0; t<numOfBands; t++)
					{
						image[k][t] = m_pData[k].getLayer(t)->getData(i, j);
						if(image[k][t] < min)
							min = image[k][t];
						if(image[k][t] > max)
							max = image[k][t];
					}
			       for(int k=0; k<numOfDays; k++)
				
					{
						image[k][numOfBands] = m_pData[k].getLayer(numOfBands-1)->getData(i, j);
						
					}


					short **imageNew, **imageResult0, **imageResult;
					imageNew = new short*[numOfDays];
					imageResult0 = new short*[numOfDays];
					imageResult = new short*[numOfDays];

					for(int s=0; s<numOfDays; s++)
					{

						imageNew[s] = new short[numOfBands+1];
						imageResult0[s] = new short[numOfBands+1];		
						imageResult[s] = new short[numOfBands+1];
					}



					for(int k=0; k<numOfDays; k++)
						for(int t=0; t<numOfBands+1; t++)
						{
							imageNew[k][t] = (image[k][t] - min)*255/(max-min);
						}




						DWT_Once(imageNew, imageResult0, imageResult, numOfDays, numOfDays/2, numOfBands+1, (numOfBands+1)/2);

						float mean = 0; 
						float std = 0;

						for(int m=0; m<numOfDays/2; m++)
							for(int n=0; n<(numOfBands+1)/2; n++)
							{
								mean += imageResult[m][n];
								std += pow(imageResult[m][n], 2);
							}

							meanLL_Buf[i*col+j] = mean/(numOfDays*(numOfBands+1)/4);
							stdLL_Buf[i*col+j] = sqrt(std/(numOfDays*(numOfBands+1)/4) - pow(meanLL_Buf[i*col+j], 2));

							mean = 0; 
							std = 0;

							for(int m=0; m<numOfDays/2; m++)
								for(int n=(numOfBands+1)/2; n<numOfBands+1; n++)
								{
									mean += imageResult[m][n];
									std += pow(imageResult[m][n], 2);
								}

								meanHL_Buf[i*col+j] = mean/(numOfDays*(numOfBands+1)/4);
								stdHL_Buf[i*col+j] = sqrt(std/(numOfDays*(numOfBands+1)/4) - pow(meanHL_Buf[i*col+j], 2));

								mean = 0; 
								std = 0;

								for(int m=numOfDays/2; m<numOfDays ; m++)
									for(int n=0; n<(numOfBands+1)/2; n++)
									{
										mean += imageResult[m][n];
										std += pow(imageResult[m][n], 2);
									}

									meanLH_Buf[i*col+j] = mean/(numOfDays*(numOfBands+1)/4);
									stdLH_Buf[i*col+j] = sqrt(std/(numOfDays*(numOfBands+1)/4) - pow(meanLH_Buf[i*col+j], 2));


									mean = 0; 
									std = 0;

									for(int m=numOfDays/2; m<numOfDays ; m++)
										for(int n=(numOfBands+1)/2; n<numOfBands+1; n++)
										{
											mean += imageResult[m][n];
											std += pow(imageResult[m][n], 2);
										}

										meanHH_Buf[i*col+j] = mean/(numOfDays*(numOfBands+1)/4);
										stdHH_Buf[i*col+j] = sqrt(std/(numOfDays*(numOfBands+1)/4) - pow(meanHH_Buf[i*col+j], 2));


			}
			else
			{
				float **image;
				image = new float*[numOfDays + 1];

				for(int s=0; s<numOfDays + 1; s++)
					image[s] = new float[numOfBands + 1];


				float min = m_pData[0].getLayer(0)->getData(i, j);
				float max = m_pData[0].getLayer(0)->getData(i, j);

				for(int k=0; k<numOfDays; k++)
					for(int t=0; t<numOfBands; t++)
					{
						image[k][t] = m_pData[k].getLayer(t)->getData(i, j);
						if(image[k][t] < min)
							min = image[k][t];
						if(image[k][t] > max)
							max = image[k][t];
					}

			    for(int t=0; t<numOfBands; t++)
					{
						image[numOfDays][t] = m_pData[numOfDays-1].getLayer(t)->getData(i, j);
					
					}

					for(int k=0; k<numOfDays; k++)
					{
						image[k][numOfBands] = m_pData[k].getLayer(numOfBands-1)->getData(i, j);
					
					}

					image[numOfDays][numOfBands] = m_pData[numOfDays-1].getLayer(numOfBands-1)->getData(i, j);


					short **imageNew, **imageResult0, **imageResult;
					imageNew = new short*[numOfDays+1];

					imageResult0 = new short*[numOfDays+1];
					imageResult = new short*[numOfDays+1];

					for(int s=0; s<numOfDays+1; s++)
					{

						imageNew[s] = new short[numOfBands+1];
						imageResult0[s] = new short[numOfBands+1];		
						imageResult[s] = new short[numOfBands+1];
					}



					for(int k=0; k<numOfDays+1; k++)
						for(int t=0; t<numOfBands+1; t++)
						{
							imageNew[k][t] = (image[k][t] - min)*255/(max-min);
						}




						DWT_Once(imageNew, imageResult0, imageResult, numOfDays + 1, (numOfDays + 1)/2, numOfBands+1, (numOfBands+1)/2);

						float mean = 0; 
						float std = 0;

						for(int m=0; m<(numOfDays+1)/2; m++)
							for(int n=0; n<(numOfBands+1)/2; n++)
							{
								mean += imageResult[m][n];
								std += pow(imageResult[m][n], 2);
							}

							meanLL_Buf[i*col+j] = mean/((numOfDays + 1)*(numOfBands+1)/4);
							stdLL_Buf[i*col+j] = sqrt(std/((numOfDays+1)*(numOfBands+1)/4) - pow(meanLL_Buf[i*col+j], 2));

							mean = 0; 
							std = 0;

							for(int m=0; m<(numOfDays+1)/2; m++)
								for(int n=(numOfBands+1)/2; n<numOfBands+1; n++)
								{
									mean += imageResult[m][n];
									std += pow(imageResult[m][n], 2);
								}

								meanHL_Buf[i*col+j] = mean/((numOfDays+1)*(numOfBands+1)/4);
								stdHL_Buf[i*col+j] = sqrt(std/((numOfDays+1)*(numOfBands+1)/4) - pow(meanHL_Buf[i*col+j], 2));

								mean = 0; 
								std = 0;

								for(int m=(numOfDays +1)/2; m<numOfDays + 1 ; m++)
									for(int n=0; n<(numOfBands+1)/2; n++)
									{
										mean += imageResult[m][n];
										std += pow(imageResult[m][n], 2);
									}

									meanLH_Buf[i*col+j] = mean/((numOfDays+1)*(numOfBands+1)/4);
									stdLH_Buf[i*col+j] = sqrt(std/((numOfDays+1)*(numOfBands+1)/4) - pow(meanLH_Buf[i*col+j], 2));


									mean = 0; 
									std = 0;

									for(int m=(numOfDays+1)/2; m<numOfDays+1 ; m++)
										for(int n=(numOfBands+1)/2; n<numOfBands+1; n++)
										{
											mean += imageResult[m][n];
											std += pow(imageResult[m][n], 2);
										}

										meanHH_Buf[i*col+j] = mean/((numOfDays+1)*(numOfBands+1)/4);
										stdHH_Buf[i*col+j] = sqrt(std/((numOfDays+1)*(numOfBands+1)/4) - pow(meanHH_Buf[i*col+j], 2));


			   
			}

		}




		CString str;
		CString strName;

		CThreeDR* meanLL = new CThreeDR;
		meanLL->copyBasicFields(m_pData[0].getLayer(0)->m_pTheData);
		meanLL->setDataBuf(meanLL_Buf);
		meanLL->calcStat();	
		str = this->m_strResultFolder + "\\meanLL.3dr";
		strName = "meanLL";
		meanLL->setDataName(strName.GetBuffer());
		meanLL->writeout(str.GetBuffer(),0);	
		delete meanLL;

		CThreeDR* stdLL = new CThreeDR;
		stdLL->copyBasicFields(m_pData[0].getLayer(0)->m_pTheData);
		stdLL->setDataBuf(stdLL_Buf);
		stdLL->calcStat();	
		str = this->m_strResultFolder + "\\stdLL.3dr";	
			strName = "stdLL";
		stdLL->setDataName(strName.GetBuffer());
		stdLL->writeout(str.GetBuffer(),0);	
		delete stdLL;


		CThreeDR* meanHL = new CThreeDR;
		meanHL->copyBasicFields(m_pData[0].getLayer(0)->m_pTheData);
		meanHL->setDataBuf(meanHL_Buf);
		meanHL->calcStat();	
		str = this->m_strResultFolder + "\\meanHL.3dr";	
			strName = "meanHL";
		meanHL->setDataName(strName.GetBuffer());
		meanHL->writeout(str.GetBuffer(),0);	
		delete meanHL;

		CThreeDR* stdHL = new CThreeDR;
		stdHL->copyBasicFields(m_pData[0].getLayer(0)->m_pTheData);
		stdHL->setDataBuf(stdHL_Buf);
		stdHL->calcStat();	
		str = this->m_strResultFolder + "\\stdHL.3dr";	
			strName = "stdHL";
		stdHL->setDataName(strName.GetBuffer());
		stdHL->writeout(str.GetBuffer(),0);	
		delete stdHL;


		CThreeDR* meanLH = new CThreeDR;
		meanLH->copyBasicFields(m_pData[0].getLayer(0)->m_pTheData);
		meanLH->setDataBuf(meanLH_Buf);
		meanLH->calcStat();	
		str = this->m_strResultFolder + "\\meanLH.3dr";
			strName = "meanLH";
		meanLH->setDataName(strName.GetBuffer());
		meanLH->writeout(str.GetBuffer(),0);	
		delete meanLH;

		CThreeDR* stdLH = new CThreeDR;
		stdLH->copyBasicFields(m_pData[0].getLayer(0)->m_pTheData);
		stdLH->setDataBuf(stdLH_Buf);
		stdLH->calcStat();	
		str = this->m_strResultFolder + "\\stdLH.3dr";
			strName = "stdLH";
		stdLH->setDataName(strName.GetBuffer());
		stdLH->writeout(str.GetBuffer(),0);	
		delete stdLH;


		CThreeDR* meanHH = new CThreeDR;
		meanHH->copyBasicFields(m_pData[0].getLayer(0)->m_pTheData);
		meanHH->setDataBuf(meanHH_Buf);
		meanHH->calcStat();	
		str = this->m_strResultFolder + "\\meanHH.3dr";
			strName = "meanHH";
		meanHH->setDataName(strName.GetBuffer());
		meanHH->writeout(str.GetBuffer(),0);	
		delete meanHH;

		CThreeDR* stdHH = new CThreeDR;
		stdHH->copyBasicFields(m_pData[0].getLayer(0)->m_pTheData);
		stdHH->setDataBuf(stdHH_Buf);
		stdHH->calcStat();	
		str = this->m_strResultFolder + "\\stdHH.3dr";
			strName = "stdHH";
		stdHH->setDataName(strName.GetBuffer());
		stdHH->writeout(str.GetBuffer(),0);	
		delete stdHH;


			   MessageBox("Environmental variable extraction from multitemporal remotesensing data is done!");
}





/********************************************************************************
*函数描述：	DWT_Once完成一次图像的小波变换										*
*函数参数：	short **spOriginData：二维指针，指向原始的图像数据					*
*			short **spTransData0：小波变换系数，存放一次水平变换后的小波系数	*
*			short **spTransData1：小波变换系数，存放一次竖直变换后的小波系数	*
*			int   nHeight		：图像属性参数，数值为原始图像的高度值			*
*			int	  nHeight_H		：图像属性参数，数值为原始图像高度值的一半		*
*			int   nWidth		：图像属性参数，数值为原始图像的宽度值			*
*			int	  nWidth_H		：图像属性参数，数值为原始图像宽度值的一半		*
*			int   layer			：小波变换的层数，数值为1层						*
*			float fRadius		：小波变换因子，在调用时候已指定数值为1.414		*
********************************************************************************/

void CRemoteSensingDlg::DWT_Once(short** spOriginData, short** spTransData0, short** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H)
{
	int Trans_W,				//图像扫描线控制：横坐标
		Trans_H,				//图像扫描线控制：纵坐标
		Trans_M,				//图像矩阵的横坐标
		Trans_N;				//图像矩阵的纵坐标
	short Trans_Coeff0;			//小波变换系数
	signed short Trans_Coeff1;
	float fRadius=1.414;				//变换滤波系数

	int layer = 1;
	//本模块完成变换系数的赋值采样
	//行变换,第一次（layer=1时）时nHeight即为原始图像的高度值
	for(Trans_H=0; Trans_H<nHeight; Trans_H++)            
	{
		if(layer == 1)
			//layer=1时，nWidth_H为原始图像宽度值的一半
			for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)          
			{
				Trans_W=Trans_N<<1;
				if (fRadius==2)
				{
					spTransData0[Trans_H][Trans_N] = (spOriginData[Trans_H][Trans_W]);
					spTransData0[Trans_H][nWidth_H+Trans_N] = (spOriginData[Trans_H][Trans_W+1]);
				}
				else
				{
					spTransData0[Trans_H][Trans_N] = (spOriginData[Trans_H][Trans_W]-128);		
					spTransData0[Trans_H][nWidth_H+Trans_N] = (spOriginData[Trans_H][Trans_W+1]-128);	
				}
			}
			//若变换层数大于1,则仅采样低频的小波系数
			if(layer > 1)
				for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
				{
					Trans_W=Trans_N<<1;
					spTransData0[Trans_H][Trans_N] = spTransData1[Trans_H][Trans_W];
					spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData1[Trans_H][Trans_W+1];
				}
	}
	for(Trans_H=0; Trans_H<nHeight; Trans_H++)
	{
		for(Trans_N=0; Trans_N<nWidth_H-1; Trans_N++)
		{
			//奇偶数值和的一半
			Trans_Coeff1 = ((spTransData0[Trans_H][Trans_N]+spTransData0[Trans_H][Trans_N+1])>>1);	
			//逻辑非操作后数值加1
			Trans_Coeff1=~Trans_Coeff1+1;	
			//系数预测
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N]+Trans_Coeff1;	
		}
		//完成一个偶系数的边界处理
		Trans_Coeff1 = ((spTransData0[Trans_H][nWidth_H-1]+spTransData0[Trans_H][nWidth_H-2])>>1);
		Trans_Coeff1=~Trans_Coeff1+1;
		spTransData0[Trans_H][nWidth-1] = spTransData0[Trans_H][nWidth-1]+Trans_Coeff1;
		//完成一个奇系数的边界处理
		Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H]+spTransData0[Trans_H][nWidth_H+1])>>2);
		spTransData0[Trans_H][0] = spTransData0[Trans_H][0]+Trans_Coeff0;
		//提升，整数到整数的变换
		for(Trans_N=1; Trans_N<nWidth_H; Trans_N++)
		{
			Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H+Trans_N]+spTransData0[Trans_H][nWidth_H+Trans_N-1])>>2);
			spTransData0[Trans_H][Trans_N] = spTransData0[Trans_H][Trans_N]+Trans_Coeff0;
		}

	}//水平方向的变换结束
	//竖直方向的变换开始，数据源未水平变换后的小波系数
	for(Trans_M=0; Trans_M<nHeight; Trans_M++)
	{
		for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
		{
			spTransData0[Trans_M][Trans_N]*=fRadius;
			spTransData0[Trans_M][Trans_N+nWidth_H]/=fRadius;
		}
	}
	//行提升后的数据在spTransData0中，spTransData0中的数据自然奇偶有序
	for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
	{
		//列变换
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_H =Trans_M<<1;
			//频带LL部分
			spTransData1[Trans_M][Trans_N] = spTransData0[Trans_H][Trans_N];
			//频带HL部分
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData0[Trans_H+1][Trans_N];
			//频带LH部分
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N];	
			//频带HH部分
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData0[Trans_H+1][nWidth_H+Trans_N];
		}
		//第一次提升奇数坐标系数
		for(Trans_M=0; Trans_M<nHeight_H-1; Trans_M++)
		{
			//竖直方向的变换 
			Trans_Coeff1 = ((spTransData1[Trans_M][Trans_N]+spTransData1[Trans_M+1][Trans_N])>>1);
			Trans_Coeff1=~Trans_Coeff1+1;
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData1[nHeight_H+Trans_M][Trans_N]+Trans_Coeff1;
			Trans_Coeff1 = ((spTransData1[Trans_M][nWidth_H+Trans_N]+spTransData1[Trans_M+1][nWidth_H+Trans_N])>>1);
			Trans_Coeff1=~Trans_Coeff1+1;
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+Trans_Coeff1;
		}
		Trans_Coeff1 = ((spTransData1[nHeight_H-1][Trans_N]+spTransData1[nHeight_H-2][Trans_N])>>1);
		Trans_Coeff1=~Trans_Coeff1+1;
		spTransData1[nHeight-1][Trans_N] = spTransData1[nHeight-1][Trans_N]+Trans_Coeff1;
		Trans_Coeff1 = ((spTransData1[nHeight_H-1][nWidth_H+Trans_N]+spTransData1[nHeight_H-2][nWidth_H+Trans_N])>>1);
		Trans_Coeff1=~Trans_Coeff1+1;
		//边界处理
		spTransData1[nHeight-1][nWidth_H+Trans_N] = spTransData1[nHeight-1][nWidth_H+Trans_N]+Trans_Coeff1;

		Trans_Coeff0 = ((spTransData1[nHeight_H][Trans_N]+spTransData1[nHeight_H+1][Trans_N])>>2);
		spTransData1[0][Trans_N] = spTransData1[0][Trans_N]+Trans_Coeff0;
		Trans_Coeff0 = ((spTransData1[nHeight_H][nWidth_H+Trans_N]+spTransData1[nHeight_H+1][nWidth_H+Trans_N])>>2);
		//边界处理
		spTransData1[0][nWidth_H+Trans_N] = spTransData1[0][nWidth_H+Trans_N]+Trans_Coeff0;
		//第一次提升偶数坐标系数
		for(Trans_M=1; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_Coeff0 = ((spTransData1[nHeight_H+Trans_M][Trans_N]+spTransData1[nHeight_H+Trans_M-1][Trans_N])>>2);
			spTransData1[Trans_M][Trans_N] = spTransData1[Trans_M][Trans_N]+Trans_Coeff0;
			Trans_Coeff0 = ((spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+spTransData1[nHeight_H+Trans_M-1][nWidth_H+Trans_N])>>2);
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N]+Trans_Coeff0;
		}
	}
	//存放小波系数，LL频带的系数进行幅值增强处理，其它高频频带的系数则削弱其幅值
	for(Trans_N=0; Trans_N<nWidth; Trans_N++)
	{
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			spTransData1[Trans_M][Trans_N]*=fRadius;
			spTransData1[Trans_M+nHeight_H][Trans_N]/=fRadius;
		}
	}
}

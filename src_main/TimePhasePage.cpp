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
// TimePhasePage.cpp : 实现文件
//

#include "stdafx.h"
#include "SoLIM.h"
#include "TimePhasePage.h"
#include ".\timephasepage.h"


// CTimePhasePage 对话框

IMPLEMENT_DYNAMIC(CTimePhasePage, CDialog)
CTimePhasePage::CTimePhasePage(CWnd* pParent /*=NULL*/)
	: CDialog(CTimePhasePage::IDD, pParent)
{
	m_pEnvDatabase = NULL;
	m_iCurLayer = 0;
}

CTimePhasePage::~CTimePhasePage()
{
}

void CTimePhasePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

		DDX_Control(pDX, IDC_LIST_LAYER, m_LayerList);


}


BEGIN_MESSAGE_MAP(CTimePhasePage, CDialog)

	ON_BN_CLICKED(IDC_BUTTON_ADD_LAYER, OnBnClickedButtonAddLayer)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_LAYER, OnBnClickedButtonRemoveLayer)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnBnClickedButtonDown)
END_MESSAGE_MAP()


// CTimePhasePage 消息处理程序

BOOL CTimePhasePage::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	CDC* pDC = GetDC();
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

		m_LayerList.m_iNumOfUsedRow = m_iNumOfBands;
	this->m_LayerList.InsertColumn(0, "Name", LVCFMT_LEFT, 20*tm.tmAveCharWidth, 0);
	this->m_LayerList.InsertColumn(1, "File", LVCFMT_LEFT, 70*tm.tmAveCharWidth, 1);
	this->m_LayerList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);


	for(int i=0;i<m_iNumOfBands; i++)
	{

		int count = m_LayerList.GetItemCount();
		m_LayerList.InsertItem(LVIF_TEXT|LVIF_STATE, count, "", LVIS_SELECTED, LVIS_SELECTED, 0, 0);
	}





	if(m_pEnvDatabase==NULL)
		m_pEnvDatabase = new CEnvDatabase();

	m_iCurLayer = 0;
	//m_LayerList.DeleteAllItems();

	return TRUE;  
}



void CTimePhasePage::OnBnClickedButtonAddLayer()
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
			if(m_iCurLayer > this->m_iNumOfBands)
				return;
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
					//int count = m_LayerList.GetItemCount();
					m_LayerList.SetItemText(m_iCurLayer, 0, strLayerName);
					m_LayerList.SetItemText(m_iCurLayer, 1, layerFileName);
					m_iCurLayer ++;
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

void CTimePhasePage::OnBnClickedButtonRemoveLayer()
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
		m_iCurLayer --;
	}

	
	/*for(int i=0; i<m_pEnvDatabase->getLayerNum();i++)
	{
		 m_LayerList.SetItemText(i, 0, m_pEnvDatabase->getLayer(i)->getLayerName());
		m_LayerList.SetItemText(i, 1, m_pEnvDatabase->getLayer(i)->getFileName());
		
	}*/

	m_LayerList.InsertItem(LVIF_TEXT|LVIF_STATE, m_pEnvDatabase->getLayerNum(), "", LVIS_SELECTED, LVIS_SELECTED, 0, 0);

	//m_LayerList.SetItemText(m_pEnvDatabase->getLayerNum(), 0, "aa");
	//m_LayerList.SetItemText(m_pEnvDatabase->getLayerNum(), 1, "bb");

	this->Invalidate();
}



void CTimePhasePage::OnBnClickedButtonUp()
{
	POSITION pos=this->m_LayerList.GetFirstSelectedItemPosition();


		if(m_LayerList.GetSelectedCount() > 1)
		{
			MessageBox("Only one band can be moved down each time.");
			return;
		}
		else if(m_LayerList.GetSelectedCount() < 1)
		{
			MessageBox("No band is selected .");
			return;
		}
		else

	{
		int nItem = m_LayerList.GetNextSelectedItem(pos);
		if(nItem == 0)
			return;

		int index1 = -1;
		int index2 = -1;

		CString strLayerName = m_LayerList.GetItemText(nItem, 0);
		for(int i=0;i<m_pEnvDatabase->getLayerNum();i++) 	
			if((m_pEnvDatabase->getLayer(i)->getLayerName())==strLayerName)
			{
				index1 = i;
		        break;
			}
		
		strLayerName = m_LayerList.GetItemText(nItem-1, 0);
		for(int i=0;i<m_pEnvDatabase->getLayerNum();i++) 	
			if((m_pEnvDatabase->getLayer(i)->getLayerName())==strLayerName)
			{
				index2 = i;
				break;
			}

		m_pEnvDatabase->swapLayer(index1, index2);


   


	for(int i=0; i<m_pEnvDatabase->getLayerNum();i++)
	{

		 m_LayerList.SetItemText(i, 0, m_pEnvDatabase->getLayer(i)->getLayerName());
		m_LayerList.SetItemText(i, 1, m_pEnvDatabase->getLayer(i)->getFileName());
		
	}
	}







}

	void CTimePhasePage::OnBnClickedButtonDown()
	{
		POSITION pos=this->m_LayerList.GetFirstSelectedItemPosition();

		if(m_LayerList.GetSelectedCount() > 1)
		{
			MessageBox("Only one band can be moved down each time.");
			return;
		}
		else if(m_LayerList.GetSelectedCount() < 1)
		{
			MessageBox("No band is selected .");
			return;
		}
		else
		{
			int nItem = m_LayerList.GetNextSelectedItem(pos);

			if(nItem == m_pEnvDatabase->getLayerNum())
				return;

			int index1 = -1;
			int index2 = -1;

			int i=0;

         	CString strLayerName1 = m_LayerList.GetItemText(nItem, 0);
			CString strLayerName2 = m_LayerList.GetItemText(nItem+1, 0);

			for(i=0;i<m_pEnvDatabase->getLayerNum();i++) 	
			{
				CString str = m_pEnvDatabase->getLayer(i)->getLayerName();
				if(str==strLayerName1)
				{
					index1 = i;
				}
                if(str==strLayerName2)
				{
					index2 = i;
				}
			}
	

		

			m_pEnvDatabase->swapLayer(index1, index2);


			for(int i=0; i<m_pEnvDatabase->getLayerNum();i++)
			{

				m_LayerList.SetItemText(i, 0, m_pEnvDatabase->getLayer(i)->getLayerName());
				m_LayerList.SetItemText(i, 1, m_pEnvDatabase->getLayer(i)->getFileName());

			}
		}
	}

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
// MetadataView.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "MetadataView.h"
#include ".\metadataview.h"
#include "SoLIMDoc.h"

// CMetadataView

IMPLEMENT_DYNCREATE(CMetadataView, CFormView)

CMetadataView::CMetadataView()
	: CFormView(CMetadataView::IDD)
{
}

CMetadataView::~CMetadataView()
{
}

void CMetadataView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_METADATA, m_listctrlMetadata);
}

BEGIN_MESSAGE_MAP(CMetadataView, CFormView)
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CMetadataView diagnostics

#ifdef _DEBUG
void CMetadataView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMetadataView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CMetadataView message handlers

void CMetadataView::OnStnClickedStaticNor()
{
	
}

void CMetadataView::FillMetadataView(CProject *pProject)
{
	if(pProject!=NULL)
	{
		if(m_listctrlMetadata.GetItemCount()!=0)
		{
			m_listctrlMetadata.DeleteAllItems();
		}
		CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
		if(pProject->m_pEnvDatabase->getLayerNum()>0)
		{
		CString strTemp;
		/*strTemp.Format("Number Of Records");  
		m_listctrlMetadata.InsertItem(LVIF_TEXT|LVIF_STATE,0, strTemp, 0, LVIS_SELECTED,0, 0);
		strTemp.Format("%d",pProject->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->m_pTheData->getNumberOfRecords());
		m_listctrlMetadata.SetItemText(0,1,strTemp);*/

		strTemp.Format("Number Of Rows");  
		m_listctrlMetadata.InsertItem(LVIF_TEXT|LVIF_STATE,0, strTemp, 0, LVIS_SELECTED,0, 0);
		strTemp.Format("%d",pProject->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->m_pTheData->getNumberOfRows());
		m_listctrlMetadata.SetItemText(0,1,strTemp);

		strTemp.Format("Number Of Columns");  
		m_listctrlMetadata.InsertItem(LVIF_TEXT|LVIF_STATE,1, strTemp, 0, LVIS_SELECTED,0, 0);
		strTemp.Format("%d",pProject->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->m_pTheData->getNumberOfColumns());
		m_listctrlMetadata.SetItemText(1,1,strTemp);

		strTemp.Format("File Type");  
		m_listctrlMetadata.InsertItem(LVIF_TEXT|LVIF_STATE,2, strTemp, 0, LVIS_SELECTED,0, 0);
		strTemp.Format(pProject->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->m_pTheData->getFileType());
		m_listctrlMetadata.SetItemText(2,1,strTemp);

		strTemp.Format("Data Name");  
		m_listctrlMetadata.InsertItem(LVIF_TEXT|LVIF_STATE,3, strTemp, 0, LVIS_SELECTED,0, 0);
		strTemp.Format(pProject->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->m_pTheData->getDataName());
		m_listctrlMetadata.SetItemText(3,1,strTemp);

		strTemp.Format("Base Filename");  
		m_listctrlMetadata.InsertItem(LVIF_TEXT|LVIF_STATE,4, strTemp, 0, LVIS_SELECTED,0, 0);
		strTemp.Format(pProject->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->m_pTheData->getBaseFilename());
		m_listctrlMetadata.SetItemText(4,1,strTemp);

		strTemp.Format("Grid Units");  
		m_listctrlMetadata.InsertItem(LVIF_TEXT|LVIF_STATE,5, strTemp, 0, LVIS_SELECTED,0, 0);
		strTemp.Format(pProject->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->m_pTheData->getGridUnits());
		m_listctrlMetadata.SetItemText(5,1,strTemp);

		strTemp.Format("X min");  
		m_listctrlMetadata.InsertItem(LVIF_TEXT|LVIF_STATE,6, strTemp, 0, LVIS_SELECTED,0, 0);
		strTemp.Format("%f",pProject->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->m_pTheData->getXmin());
		m_listctrlMetadata.SetItemText(6,1,strTemp);

		strTemp.Format("Y min");  
		m_listctrlMetadata.InsertItem(LVIF_TEXT|LVIF_STATE,7, strTemp, 0, LVIS_SELECTED,0, 0);
		strTemp.Format("%f",pProject->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->m_pTheData->getYmin());
		m_listctrlMetadata.SetItemText(7,1,strTemp);

		strTemp.Format("Cell Size");  
		m_listctrlMetadata.InsertItem(LVIF_TEXT|LVIF_STATE,8, strTemp, 0, LVIS_SELECTED,0, 0);
		strTemp.Format("%f",pProject->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->m_pTheData->getCellSize());
		m_listctrlMetadata.SetItemText(8,1,strTemp);

		strTemp.Format("Neighborhood");  
		m_listctrlMetadata.InsertItem(LVIF_TEXT|LVIF_STATE,9, strTemp, 0, LVIS_SELECTED,0, 0);
		strTemp.Format("%f",pProject->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->m_pTheData->getNeighborhood());
		m_listctrlMetadata.SetItemText(9,1,strTemp);

		strTemp.Format("Data Units");  
		m_listctrlMetadata.InsertItem(LVIF_TEXT|LVIF_STATE,10, strTemp, 0, LVIS_SELECTED,0, 0);
		strTemp.Format(pProject->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->m_pTheData->getDataUnits());
		m_listctrlMetadata.SetItemText(10,1,strTemp);

		strTemp.Format("Data Min");  
		m_listctrlMetadata.InsertItem(LVIF_TEXT|LVIF_STATE,11, strTemp, 0, LVIS_SELECTED,0, 0);
		strTemp.Format("%f",pProject->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->m_pTheData->getDataMin());
		m_listctrlMetadata.SetItemText(11,1,strTemp);

		strTemp.Format("Data Max");  
		m_listctrlMetadata.InsertItem(LVIF_TEXT|LVIF_STATE,12, strTemp, 0, LVIS_SELECTED,0, 0);
		strTemp.Format("%f",pProject->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->m_pTheData->getDataMax());
		m_listctrlMetadata.SetItemText(12,1,strTemp);

		strTemp.Format("Data Mean");  
		m_listctrlMetadata.InsertItem(LVIF_TEXT|LVIF_STATE,13, strTemp, 0, LVIS_SELECTED,0, 0);
		strTemp.Format("%f",pProject->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->m_pTheData->getDataMean());
		m_listctrlMetadata.SetItemText(13,1,strTemp);

		strTemp.Format("Data Std");  
		m_listctrlMetadata.InsertItem(LVIF_TEXT|LVIF_STATE,14, strTemp, 0, LVIS_SELECTED,0, 0);
		strTemp.Format("%f",pProject->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->m_pTheData->getDataStd());
		m_listctrlMetadata.SetItemText(14,1,strTemp);

		/*strTemp.Format("First Data Byte");  
		m_listctrlMetadata.InsertItem(LVIF_TEXT|LVIF_STATE,16, strTemp, 0, LVIS_SELECTED,0, 0);
		strTemp.Format("%d",pProject->m_pEnvDatabase->getLayer(pApp->m_iCurrEnvLayerIndex)->m_pTheData->getFirstDataByte());
		m_listctrlMetadata.SetItemText(16,1,strTemp);*/
	  }

	}
}

void CMetadataView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	CRect rect(0,0,cx,cy);
	if(m_listctrlMetadata.GetSafeHwnd())
	{

		m_listctrlMetadata.MoveWindow(&rect);	
		m_listctrlMetadata.SetColumnWidth(0,rect.Width()*1/3);
     	m_listctrlMetadata.SetColumnWidth(1,rect.Width()*2/3);
	}

	
}

int CMetadataView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetDocument()->UpdateAllViews(this);

	return 0;
}

void CMetadataView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class
}

void CMetadataView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CRect rect;
	GetClientRect(&rect);
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());

	SetScrollSizes(MM_TEXT, CSize(0, 0));

	if(m_listctrlMetadata.GetSafeHwnd())
	{	
		m_listctrlMetadata.DeleteColumn(0);
		m_listctrlMetadata.DeleteColumn(1);		
		m_listctrlMetadata.InsertColumn(1,CString("Value"));
	    m_listctrlMetadata.InsertColumn(0,CString("Item"));
		m_listctrlMetadata.MoveWindow(&rect);	
		m_listctrlMetadata.SetColumnWidth(0,rect.Width()*1/3);
     	m_listctrlMetadata.SetColumnWidth(1,rect.Width()*2/3);	
	}

	FillMetadataView(pDoc->m_pProjectEdit);

}

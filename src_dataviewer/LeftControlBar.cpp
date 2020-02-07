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
// LeftControlBar.cpp: implementation of the CLeftControlBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SoLIM.h"
#include "LeftControlBar.h"
#include ".\leftcontrolbar.h"
#include "MainFrm.h"
#include "SoLIMDoc.h"
#include "DatabaseView.h"
#include "InfoFrameView.h"
#include "MetadataView.h"



#define  HTPIN			323

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLeftControlBar::CLeftControlBar()
{
	
	m_hPreviousItem = NULL;
	m_bRightClick = false;
	m_bHasNewRule = false;

}

CLeftControlBar::~CLeftControlBar()
{

}
BEGIN_MESSAGE_MAP(CLeftControlBar, CGuiControlBar)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_NOTIFY(TVN_SELCHANGED, 1, OnTvnSelchangedProjectTree)
	ON_NOTIFY(NM_RCLICK, 1, OnRClickProjectTree)
	ON_WM_SIZE()
	ON_COMMAND(ID_DATABASE_ADDLAYER, OnDatabaseAddlayer)
	ON_COMMAND(ID_DATALAYER_DELETE, OnDatalayerDelete)

	
	ON_WM_NCHITTEST()

	

END_MESSAGE_MAP()

int CLeftControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//to erase the close button
	if (CControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (m_pDockContext==NULL)
		m_pDockContext=new CGuiDockContext(this);
	ASSERT(m_pDockContext);
	m_CloseBtn.SetData(6,_T("Close"));
	m_CloseBtn.SetImageList(IDB_GUI_DOCKBAR,7,0,RGB(255,0,255));  //to hide the close button
	m_AutoHideBtn.SetData(12,_T("Auto Hide"));
	m_AutoHideBtn.SetImageList(IDB_GUI_DOCKBAR,9,15,RGB(255,0,255));
	if (!m_ctServer.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,124))
		return -1;
	if (!m_trProjectTree.Create(WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS |TVS_SHOWSELALWAYS,	CRect(0,0,0,0), &m_ctServer, 1))
		return -1;
	m_imgListProject.Create (IDB_TREE_IMAGE, 16, 45, RGB (255, 0, 0));
	m_trProjectTree.SetImageList(&m_imgListProject,TVSIL_STATE);
	m_hDatabase = m_trProjectTree.InsertItem (_T("Layers"), 0, 0);
	m_trProjectTree.SetItemState(m_hDatabase, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK );
	
	m_trProjectTree.Expand (m_hDatabase, TVE_EXPAND);
	m_ctServer.AddComponen(&m_trProjectTree);
	m_hPreviousItem = m_hDatabase;
	return 0;
}

void CLeftControlBar::OnClose()
{
}

void CLeftControlBar::OnRClickProjectTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());

	//if(pDoc->m_pProjectEdit==NULL)
	//	return;

	HTREEITEM   hSelect   =   m_trProjectTree.GetSelectedItem();
	HTREEITEM   hLight   =    m_trProjectTree.GetDropHilightItem();
	m_bRightClick = true; 
	CPoint point;
	::GetCursorPos(&point);
	POINT tempPoint;
	tempPoint.x=point.x;
	tempPoint.y=point.y;

	if(hLight   !=   NULL   &&   hSelect   !=   hLight)   
	{
		hSelect   =   hLight;   
		m_trProjectTree.SelectItem(hSelect);
	}
	else 
	{
		RECT itemRect;
		RECT windowRect;
		::GetWindowRect(m_trProjectTree.m_hWnd,&windowRect);
		m_trProjectTree.GetItemRect(hSelect,&itemRect,TRUE);
		if(tempPoint.x>itemRect.left+windowRect.left&&
			tempPoint.x<itemRect.right+windowRect.left&& tempPoint.y>itemRect.top+windowRect.top&&
			tempPoint.y<itemRect.bottom+windowRect.top )
		{
			hLight = hSelect;
		}
	}


	if(hLight != NULL)
	{
		int level = getSelectedTreeLevel(m_trProjectTree, hLight);
		if(level==0)
		{	
			CMenu   menu;
			menu.LoadMenu(IDR_MENU_DATABASE); 
			CMenu *pMenu;
			pMenu=menu.GetSubMenu(0);   
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,tempPoint.x+5,tempPoint.y+5,this,NULL);  

		}

		else if(level == 1)
		{
			CMenu   menu;
			menu.LoadMenu(IDR_MENU_DATALAYER); 
			CMenu *pMenu;
			pMenu=menu.GetSubMenu(0);   
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,tempPoint.x+5,tempPoint.y+5,this,NULL); 
		}
		
	}
	m_bRightClick = false;  //set the click tag to be false
}

void CLeftControlBar::OnTvnSelchangedProjectTree(NMHDR *pNMHDR, LRESULT *pResult)
{	
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());

	HTREEITEM hItem = m_trProjectTree.GetSelectedItem();

	if(hItem != NULL)
	{
		if(hItem == m_hPreviousItem)
		{
			return;
		}
		if(hItem!=m_hPreviousItem)
		{	

		}

		m_hPreviousItem = hItem;	

		//begin to find which node is clicked and invoke corresponding operations
		int level = getSelectedTreeLevel(m_trProjectTree, hItem );
		
		if(level==0)  //project
		{
			m_trProjectTree.Expand (m_hDatabase, TVE_EXPAND);
		
		}
		
		else if(level == 1)  //layer
		{
			CString strCurrDbLayerName=m_trProjectTree.GetItemText(hItem);
			for(int i=0;i<pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum();i++) 
			{
				if((pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(i)->getLayerName())==strCurrDbLayerName)
					pApp->m_iCurrEnvLayerIndex=i;
			}
		
			pDoc->UpdateAllViews(NULL);
			CDatabaseView *pDatabaseView = (CDatabaseView *)(pFrame->m_splitterTabWnd.GetPane(0,0));	
			CInfoFrameView *pInfoFrameView = (CInfoFrameView *)(pDatabaseView->m_wndSplitter.GetPane(1,0));	
			CMetadataView *pMetadataView = (CMetadataView *)(pInfoFrameView->m_wndSplitter.GetPane(0,0));	
			pMetadataView->FillMetadataView(pDoc->m_pProjectEdit);
		}
		
		}

	
}

void CLeftControlBar::FillProjectTree(CSoLIMDoc *pDoc)
{
	if(pDoc->m_pProjectEdit!=NULL)
	{
			CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	//m_trProjectTree.DeleteItem(m_hDatabase);
	//m_hDatabase = m_trProjectTree.InsertItem (_T("Layers"), 1, 1, m_hRoot);
	//m_trProjectTree.SetItemState(m_hDatabase, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK );
	if(pDoc->m_pProjectEdit!=NULL)
	{
		for(int i=0;i<pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum();i++) 
		{
			HTREEITEM SubHti = m_trProjectTree.InsertItem(pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(i)->getLayerName(),1,1,m_hDatabase);
			m_trProjectTree.SetItemState(SubHti, INDEXTOSTATEIMAGEMASK(4), TVIS_STATEIMAGEMASK );
			if(i==0)
			{
				pApp->m_iCurrEnvLayerIndex=0;
			}

		}
	}
	}
	m_trProjectTree.Expand (m_hDatabase, TVE_EXPAND);
	
}



void CLeftControlBar::OnSize(UINT nType, int cx, int cy)
{
	CGuiControlBar::OnSize(nType, cx, cy);
}


//get the selected level of the tree
int CLeftControlBar::getSelectedTreeLevel(CTreeCtrl &m_theTree, HTREEITEM hItem)
{
	CString pstr=m_theTree.GetItemText(hItem); 
	if(hItem == NULL) return -1; 
	else if(hItem == m_theTree.GetRootItem()) return 0; 

	else if(m_theTree.GetParentItem(hItem) == m_theTree.GetRootItem()) return 1;

	return -1;
}



void CLeftControlBar::CleanMemory()
{
	m_hPreviousItem = m_hDatabase;
	//m_pCurRule = NULL;


	
}

void CLeftControlBar::OnDatabaseAddlayer()
{	
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
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
			CEnvLayer *pLayer = new CEnvLayer(pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum(),strLayerName,  layerFileName );
			if(pLayer->getLayerID() != -1)
			{ 
				strLayerName.Format("%s",pLayer->m_pTheData->getDataName());
				pLayer->setLayerName(strLayerName);
				int i=0;
				for(i=0;i<pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum();i++) 
				{
					if((pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(i)->getLayerName())==strLayerName)
					{
						AfxMessageBox("The name of the layer is the same with that of one existing layer. The layer will not be added");
						delete pLayer;
						pLayer = NULL;
						break;
					}
				}
				if(i!=pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum())
					continue;

				//CString strNewLayerFilePath;
				///CString strLayerFileName = extractFileName( layerFileName);
				//strNewLayerFilePath= pDoc->m_strProjectDir+"\\GISData\\"+strLayerFileName;
				//CopyFile( layerFileName,strNewLayerFilePath,false);  //copy env layer to current project directory
				//pLayer->setFileName(strNewLayerFilePath);
				if(pDoc->m_pProjectEdit->m_pEnvDatabase->AddLayer(pLayer))   //if add layer successfully
				{
					HTREEITEM SubHti = m_trProjectTree.InsertItem(strLayerName,1,1,m_hDatabase);
					m_trProjectTree.SetItemState(SubHti, INDEXTOSTATEIMAGEMASK(4), TVIS_STATEIMAGEMASK );
					m_trProjectTree.Expand (m_hDatabase, TVE_EXPAND);
					pApp->m_iCurrEnvLayerIndex=pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum()-1;
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
		pDoc->UpdateAllViews(NULL);
	}

}


void CLeftControlBar::OnDatalayerDelete()
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
	CString strCurrDbLayerName=m_trProjectTree.GetItemText(m_trProjectTree.GetSelectedItem());
	for(int i=0;i<pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum();i++) 
	{
		if((pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(i)->getLayerName())==strCurrDbLayerName)
		{

			pDoc->m_pProjectEdit->m_pEnvDatabase->DeleteLayer(i);
			
			m_trProjectTree.DeleteItem(m_trProjectTree.GetSelectedItem());			
			if(pApp->m_iCurrEnvLayerIndex==i)
			{
				pApp->m_iCurrEnvLayerIndex = 0;
			}
			if(pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum()==0)
			{
				pApp->m_iCurrEnvLayerIndex = -1;

			}
			break;
		}
	}

	pDoc->UpdateAllViews(NULL);

}



UINT CLeftControlBar::OnNcHitTest(CPoint point)
{
	CRect rcWindow;
	GetWindowRect(rcWindow);
	int it=0;
	CRect rcT;
	CPoint pt;

	rcT=m_rcAutoHideBtn;
	ClientToScreen(rcT);
	pt=point;
	pt.y+=23;
	pt.x+=5;
	if (rcT.PtInRect(pt))
		return HTPIN;

	CRect rcTemp;
	for (int i=0; i< 4; i++)
	{
		rcTemp=rcWindow;
		if (i== 0)		//left
		{
			it=	rcTemp.left;
			it+=4;
			rcTemp.right=it;
			m_rcBorder=rcTemp;
			if (rcTemp.PtInRect(point))
				if(IsLegal(HTLEFT)) return m_SideMove=HTLEFT;
		}
		if (i==1)  //top
		{
			it=	rcTemp.top;
			it+=4;
			rcTemp.bottom=it;
			m_rcBorder=rcTemp;
			if (rcTemp.PtInRect(point))
				if(IsLegal(HTTOP)) return m_SideMove=HTTOP ;
		}
		if (i==2)  //right
		{
			it=	rcTemp.right;
			it-=4;
			rcTemp.left=it;
			m_rcBorder=rcTemp;
			if (rcTemp.PtInRect(point))
				if (IsLegal(HTRIGHT)) return m_SideMove=HTRIGHT;
		}
		if (i==3)  //bottom
		{
			it=	rcTemp.bottom;
			it-=4;
			rcTemp.top=it;
			m_rcBorder=rcTemp;
			if (rcTemp.PtInRect(point))
				if (IsLegal(HTBOTTOM))return m_SideMove=HTBOTTOM;
		}

	}
	
	return CControlBar::OnNcHitTest(point);
}



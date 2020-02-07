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
#include "KnowledgeView.h"
#include "InferenceView.h"
#include "ProjectSummaryDlg.h"
#include "SpatialSettingDlg.h"
#include "FreehandRule.h"
#include "RangeRule.h"
#include "WordRule.h"
#include "PointRule.h"
#include "EnumRule.h"
#include "RenameDlg.h"
#include "SetVarTypeDlg.h"
#include "KnowledgeSaveAsDlg.h"
#include "KnowledgeNewDlg.h"

#include "CBInferenceView.h"

#include "SHPSelectFieldDlg.h"
#include "Shp2RasterCellSizeDlg.h"

//#include "gdal_alg.h"

#include "gdal_priv.h"



#include "ogrsf_frmts.h"

#include "cpl_string.h"

#include "FileManager.h"




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
	m_pCurRule = NULL;
	m_hPreviousItem = NULL;
	m_bRightClick = false;
	m_bHasNewRule = false;
	m_pClipBoardRule=NULL;
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
	ON_COMMAND(ID_RULE_DELETE, OnRuleDelete)
	ON_COMMAND(ID_INSTANCE_DELETE, OnInstanceDelete)
	ON_COMMAND(ID_OCCURRENCE_DELETE, OnOccurrenceDelete)
	ON_COMMAND(ID_EXCLUSION_DELETE, OnExclusionDelete)
	ON_COMMAND(ID_SOILTYPE_DELETE, OnSoiltypeDelete)
	ON_COMMAND(ID_ADDRULE_RANGERULE, OnAddruleRangerule)
	ON_COMMAND(ID_ADDRULE_FREEHANDRULE, OnAddruleFreehandrule)	
	ON_COMMAND(ID_ADDRULE_WORDRULE, OnAddruleWordrule)
	ON_COMMAND(ID_ADDRULE_POINTRULE, OnAddrulePointrule)
	ON_COMMAND(ID_ADDRULE_ENUMERATEDRULE, OnAddruleEnumrule)
	ON_COMMAND(ID_INSTANCE_RENAME, OnInstanceRename)
	ON_COMMAND(ID_EXCLUSION_RENAME, OnExclusionRename)
	ON_COMMAND(ID_OCCURRENCE_RENAME, OnOccurrenceRename)
	ON_COMMAND(ID_SOILTYPE_RENAME, OnSoiltypeRename)
	ON_COMMAND(ID_RULE_RENAME, OnRuleRename)
	ON_COMMAND(ID_KNOWLEDGEBASE_ADDSOILTYPE, OnKnowledgebaseAddsoiltype)
	ON_COMMAND(ID_KNOWLEDGEBASE_CREATENEW, OnKnowledgebaseCreatenew)
	ON_COMMAND(ID_KNOWLEDGEBASE_LOAD, OnKnowledgebaseLoad)
	ON_COMMAND(ID_KNOWLEDGEBASE_SAVE, OnKnowledgebaseSave)
	ON_COMMAND(ID_KNOWLEDGEBASE_SAVEAS, OnKnowledgebaseSaveas)

	ON_COMMAND(ID_KNOWLEDGEBASE_CURRENTKNOWLEDGEFILE, OnKnowledgebaseCurrentFile)




	ON_COMMAND(ID_INSTANCES_ADDINSTANCE, OnInstancesAddinstance)
	ON_COMMAND(ID_EXCLUSIONS_ADDEXCLUSION, OnExclusionsAddexclusion)
	ON_COMMAND(ID_OCCURRENCES_ADDOCCURRENCE, OnOccurrencesAddoccurrence)
	ON_COMMAND(ID_RULE_VIEWATTACHEDLAYER, OnRuleViewattachedlayer)
	ON_WM_NCHITTEST()
	ON_COMMAND(ID_RULE_COPY, OnRuleCopy)
	ON_COMMAND(ID_INSTANCE_PASTE, OnInstancePaste)
	ON_COMMAND(ID_OCCURRENCE_PASTE, OnOccurrencePaste)
	ON_COMMAND(ID_EXCLUSION_PASTE, OnExclusionPaste)

	ON_COMMAND(ID_DATABASE_CASE_ADDLAYER, OnDatabaseCaseAddlayer)
	ON_COMMAND(ID_DATALAYER_CASE_DELETE, OnDatalayerCaseDelete)

	ON_COMMAND(ID_DATALAYER_SETVARIABLETYPE, OnDatalayerSetvariabletype)

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
	m_hRootRule = m_trProjectTree.InsertItem (_T("Rule-based Project"), 0, 0);

	m_hRootCase = m_trProjectTree.InsertItem (_T("Sample-based Project"), 0, 0);




	m_trProjectTree.SetItemState(m_hRootRule, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK );
	m_hDatabase = m_trProjectTree.InsertItem (_T("GIS Database"), 1, 1, m_hRootRule);
	m_trProjectTree.SetItemState(m_hDatabase, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK );
	m_hKnowledge = m_trProjectTree.InsertItem (_T("Knowledge Base"), 1, 1, m_hRootRule);
	m_trProjectTree.SetItemState(m_hKnowledge, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK );
	m_hInference = m_trProjectTree.InsertItem (_T("Inference"), 1, 1, m_hRootRule);			
	m_trProjectTree.SetItemState(m_hInference, INDEXTOSTATEIMAGEMASK(3), TVIS_STATEIMAGEMASK );
	m_trProjectTree.Expand (m_hRootRule, TVE_EXPAND);



	m_trProjectTree.SetItemState(m_hRootCase, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK );
	m_hDatabaseCase = m_trProjectTree.InsertItem (_T("GIS Database"), 1, 1, m_hRootCase);
	m_trProjectTree.SetItemState(m_hDatabaseCase, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK );
	m_hKnowledgeCase = m_trProjectTree.InsertItem (_T("Field Samples"), 1, 1, m_hRootCase);
	m_trProjectTree.SetItemState(m_hKnowledgeCase, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK );
	m_hInferenceCase = m_trProjectTree.InsertItem (_T("Inference"), 1, 1, m_hRootCase);			
	m_trProjectTree.SetItemState(m_hInferenceCase, INDEXTOSTATEIMAGEMASK(3), TVIS_STATEIMAGEMASK );
	m_trProjectTree.Expand (m_hRootCase, TVE_EXPAND);


	m_hClimate= m_trProjectTree.InsertItem("Climate Layers",m_hDatabaseCase);
	m_trProjectTree.SetItemState(m_hClimate, INDEXTOSTATEIMAGEMASK(5), TVIS_STATEIMAGEMASK );
	m_hGeology= m_trProjectTree.InsertItem("Parent Material Layers",m_hDatabaseCase);
	m_trProjectTree.SetItemState(m_hGeology, INDEXTOSTATEIMAGEMASK(5), TVIS_STATEIMAGEMASK );
	m_hTerrain= m_trProjectTree.InsertItem("Topographic Layers",m_hDatabaseCase);
	m_trProjectTree.SetItemState(m_hTerrain, INDEXTOSTATEIMAGEMASK(5), TVIS_STATEIMAGEMASK );
	m_hVegetation= m_trProjectTree.InsertItem("Vegetation Layers",m_hDatabaseCase);
	m_trProjectTree.SetItemState(m_hVegetation, INDEXTOSTATEIMAGEMASK(5), TVIS_STATEIMAGEMASK );
	m_hOther= m_trProjectTree.InsertItem("Other Layers",m_hDatabaseCase);
	m_trProjectTree.SetItemState(m_hOther, INDEXTOSTATEIMAGEMASK(5), TVIS_STATEIMAGEMASK );



	m_ctServer.AddComponen(&m_trProjectTree);
	m_hPreviousItem = m_hRootRule;
	return 0;
}

void CLeftControlBar::OnClose()
{
}


/*
void CLeftControlBar::OnHoverProjectTree(NMHDR* pNMHDR, LRESULT* pResult) 
{


if(pDoc->m_iProjectType == 0)
{

if(pDoc->m_pProjectEdit==NULL)
return;
}
else if(pDoc->m_iProjectType == 1)
{
return;
}


CPoint point;
::GetCursorPos(&point);
POINT tempPoint;
tempPoint.x=point.x;
tempPoint.y=point.y;



RECT itemRect;
RECT windowRect;
::GetWindowRect(m_trProjectTree.m_hWnd,&windowRect);
m_trProjectTree.GetItemRect(m_hKnowledge,&itemRect,TRUE);

if(tempPoint.x>itemRect.left+windowRect.left&&
tempPoint.x<itemRect.right+windowRect.left&& tempPoint.y>itemRect.top+windowRect.top&&
tempPoint.y<itemRect.bottom+windowRect.top )
{


}
}*/


void CLeftControlBar::OnRClickProjectTree(NMHDR* pNMHDR, LRESULT* pResult) 
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());

	if(pDoc->m_iProjectType == 0)
	{

		if(pDoc->m_pProjectEdit==NULL)
			return;
	}
	else if(pDoc->m_iProjectType == 1)
	{
		if(pDoc->m_pProjectCaseEdit ==NULL)
			return;
	}

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

		if(pDoc->m_iProjectType == 0)
		{
			if(level==0)
			{

			}
			else if(level==1)
			{
				CMenu   menu;
				menu.LoadMenu(IDR_MENU_DATABASE); 
				CMenu *pMenu;
				pMenu=menu.GetSubMenu(0);   
				pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,tempPoint.x+5,tempPoint.y+5,this,NULL);  
			}
			else if(level==2)
			{
				CMenu   menu;
				menu.LoadMenu(IDR_MENU_KNOWLEDGEBASE); 
				CMenu *pMenu;
				pMenu=menu.GetSubMenu(0);   
				pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,tempPoint.x+5,tempPoint.y+5,this,NULL);  
			}
			else if(level==3)  //inference node
			{
			}
			else if(level == 11)
			{
				CMenu   menu;
				menu.LoadMenu(IDR_MENU_DATALAYER); 
				CMenu *pMenu;
				pMenu=menu.GetSubMenu(0);   
				pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,tempPoint.x+5,tempPoint.y+5,this,NULL); 
			}

			else if(level==21)
			{		
				CMenu   menu;
				menu.LoadMenu(IDR_MENU_SOILTYPE); 
				CMenu *pMenu;
				pMenu=menu.GetSubMenu(0);   
				pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,tempPoint.x+5,tempPoint.y+5,this,NULL);  
			}
			else if(level==31)
			{
			}
			else if(level==211||level==212||level==213)
			{	
				if(level==211)
				{
					CMenu   menu;
					menu.LoadMenu(IDR_MENU_INSTANCES); 
					CMenu *pMenu;
					pMenu=menu.GetSubMenu(0);   
					pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,tempPoint.x+5,tempPoint.y+5,this,NULL);  
				}
				if(level==212)
				{
					CMenu   menu;
					menu.LoadMenu(IDR_MENU_OCCURRENCES); 
					CMenu *pMenu;
					pMenu=menu.GetSubMenu(0);   
					pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,tempPoint.x+5,tempPoint.y+5,this,NULL);  
				}
				if(level==213)
				{
					CMenu   menu;
					menu.LoadMenu(IDR_MENU_EXCLUSIONS); 
					CMenu *pMenu;
					pMenu=menu.GetSubMenu(0);   
					pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,tempPoint.x+5,tempPoint.y+5,this,NULL);  
				}


			}
			else if(level==2111||level==2121||level==2131)
			{
				if(level==2111)
				{
					CMenu   menu;
					menu.LoadMenu(IDR_MENU_INSTANCE); 
					CMenu *pMenu;
					pMenu=menu.GetSubMenu(0);   
					if(m_pClipBoardRule==NULL)
						pMenu->EnableMenuItem(ID_INSTANCE_PASTE,MF_GRAYED);
					else
						pMenu->EnableMenuItem(ID_INSTANCE_PASTE, MF_ENABLED);
					pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,tempPoint.x+5,tempPoint.y+5,this,NULL);  


				}
				if(level==2121)
				{
					CMenu   menu;
					menu.LoadMenu(IDR_MENU_OCCURRENCE); 
					CMenu *pMenu;
					pMenu=menu.GetSubMenu(0);   
					if(m_pClipBoardRule==NULL)
						pMenu->EnableMenuItem(ID_OCCURRENCE_PASTE,MF_GRAYED);
					else
						pMenu->EnableMenuItem(ID_OCCURRENCE_PASTE, MF_ENABLED);
					pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,tempPoint.x+5,tempPoint.y+5,this,NULL);  

				}
				if(level==2131)
				{
					CMenu   menu;
					menu.LoadMenu(IDR_MENU_EXCLUSION); 
					CMenu *pMenu;
					pMenu=menu.GetSubMenu(0);    
					if(m_pClipBoardRule==NULL)
						pMenu->EnableMenuItem(ID_EXCLUSION_PASTE,MF_GRAYED);
					else
						pMenu->EnableMenuItem(ID_EXCLUSION_PASTE, MF_ENABLED);
					pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,tempPoint.x+5,tempPoint.y+5,this,NULL);  

				}
			}
			else if(level == 21111||level==21211||level==21311)  //rule level
			{
				CMenu   menu;
				menu.LoadMenu(IDR_MENU_RULE); 
				CMenu *pMenu;
				pMenu=menu.GetSubMenu(0);   
				pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,tempPoint.x+5,tempPoint.y+5,this,NULL); 

			}
		}
		else if(pDoc->m_iProjectType == 1)
		{

			if(level==51)  //CBR Layer category
			{
				CMenu   menu;
				menu.LoadMenu(IDR_MENU_DATABASE_CASE); 
				CMenu *pMenu;
				pMenu=menu.GetSubMenu(0);   
				pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,tempPoint.x+5,tempPoint.y+5,this,NULL);  
			}


			else if(level==511||level==512||level==513||level==514||level==515)  //CBR Layer
			{
				CMenu   menu;
				menu.LoadMenu(IDR_MENU_DATALAYER_CASE); 
				CMenu *pMenu;
				pMenu=menu.GetSubMenu(0);   
				pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,tempPoint.x+5,tempPoint.y+5,this,NULL); 
			}
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
		if(pApp->pCurLayer!=NULL)
		{
			delete(pApp->pCurLayer);
			pApp->pCurLayer = NULL;
		}

		if(hItem == m_hPreviousItem)
		{
			return;
		}
		if(hItem!=m_hPreviousItem)
		{	
			if(m_pCurRule!=NULL)
			{
				if (m_pCurRule->IsLegal()==false)
				{

					if((AfxMessageBox("The defination of current rule is illegal.Do you want to drop this rule?",MB_YESNO))==IDYES)
					{	
						m_trProjectTree.SelectItem(m_hPreviousItem);				
						OnRuleDelete();	
						m_trProjectTree.SelectItem(hItem);

					}
					else
					{
						m_trProjectTree.SelectItem(m_hPreviousItem);
						return;
					}
				}
			}
		}

		m_hPreviousItem = hItem;	

		//begin to find which node is clicked and invoke corresponding operations
		int level = getSelectedTreeLevel(m_trProjectTree, hItem );
		m_pCurRule=NULL;
		if(level==0)  //project
		{
			if(pDoc->m_pProjectEdit!=NULL)
			{
				CProjectSummaryDlg dlg;
				dlg.m_strProjectName = pDoc->m_pProjectEdit->getProjectName();
				dlg.m_iLayerNum = pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum();
				dlg.m_iSoilNum = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilTypeNum();
				dlg.DoModal();
			}
		}
		else if(level==1)  //database
		{
			m_trProjectTree.Expand (m_hDatabase, TVE_EXPAND);
			if(pDoc->getCurViewType()!=0)
			{
				pFrame->m_splitterTabWnd.SelectView(0);
			}	
		}
		else if(level==2)  //knowledge base
		{
			m_trProjectTree.Expand(m_hKnowledge,TVE_EXPAND);

			if(pDoc->m_pProjectEdit!=NULL)
			{
				if(pDoc->getCurViewType()!=2)
				{
					pFrame->m_splitterTabWnd.SelectView(1);
				}
			}
		}
		else if(level==3)  //inference
		{
			m_trProjectTree.Expand(m_hInference,TVE_EXPAND );	
		}
		else if(level == 11)  //layer
		{
			CString strCurrDbLayerName=m_trProjectTree.GetItemText(hItem);
			for(int i=0;i<pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum();i++) 
			{
				if((pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(i)->getLayerName())==strCurrDbLayerName)
					pApp->m_iCurrEnvLayerIndex=i;
			}
			if(pDoc->getCurViewType()!=0)
			{
				pFrame->m_splitterTabWnd.SelectView(0);
			}
			pDoc->UpdateAllViews(NULL);
			CDatabaseView *pDatabaseView = (CDatabaseView *)(pFrame->m_splitterTabWnd.GetPane(0,0));	
			CInfoFrameView *pInfoFrameView = (CInfoFrameView *)(pDatabaseView->m_wndSplitter.GetPane(1,0));	
			CMetadataView *pMetadataView = (CMetadataView *)(pInfoFrameView->m_wndSplitter.GetPane(0,0));	
			pMetadataView->FillMetadataView();
		}
		else if(level==21)  //soil type
		{		
			if(pDoc->getCurViewType()!=1)
			{
				pFrame->m_splitterTabWnd.SelectView(1);
			}
			CKnowledgeView *pKnowledgeView = (CKnowledgeView *)(pFrame->m_splitterTabWnd.GetPane(0,0));	 
			pKnowledgeView->FillKnowledgeView(m_pCurRule);

			CString  strSoilTypeName;
			strSoilTypeName = m_trProjectTree.GetItemText(hItem);

			m_trProjectTree.Expand(hItem,TVE_EXPAND );	

			for(int i=0;i<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilTypeNum();i++) 
			{
				CSoilType *pCurSoilType = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i);
				if(strSoilTypeName == pCurSoilType->getSoilTypeName())
				{
					m_iCurSoil = i;
					break;
				}
			}

		}
		else if(level==31)  //sub inference 
		{
			if(pDoc->getCurViewType()!=2)
			{
				pFrame->m_splitterTabWnd.SelectView(2);
			}	
		}
		else if(level==211||level==212||level==213)  //instances, occurrences, exclusions
		{		
			if(pDoc->getCurViewType()!=1)
			{
				pFrame->m_splitterTabWnd.SelectView(1);
			}



			CString strItemName,strSoilTypeName;
			strItemName = m_trProjectTree.GetItemText(hItem);

			m_trProjectTree.Expand(hItem,TVE_EXPAND );	
			HTREEITEM  pItem= m_trProjectTree.GetParentItem(hItem);//Get the parent item
			strSoilTypeName = m_trProjectTree.GetItemText(pItem);
			for(int i=0;i<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilTypeNum();i++) 
			{
				CSoilType *pCurSoilType = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i);
				if(strSoilTypeName == pCurSoilType->getSoilTypeName())
				{
					m_iCurSoil = i;
				}
			}

			CKnowledgeView *pKnowledgeView = (CKnowledgeView *)(pFrame->m_splitterTabWnd.GetPane(0,0));	 
			pKnowledgeView->OnInitialUpdate();

		}
		else if(level==2111||level==2121||level==2131)  //instance, occrrence, exclusion
		{
			if(pDoc->getCurViewType()!=1)
			{
				pFrame->m_splitterTabWnd.SelectView(1);
			}
			CKnowledgeView *pKnowledgeView = (CKnowledgeView *)(pFrame->m_splitterTabWnd.GetPane(0,0));	 
			pKnowledgeView->FillKnowledgeView(m_pCurRule);
			CString strItemName,strSoilTypeName;
			strItemName = m_trProjectTree.GetItemText(hItem);


			m_trProjectTree.Expand(hItem,TVE_EXPAND );	

			HTREEITEM  pItem= m_trProjectTree.GetParentItem(hItem);//Get the parent item
			HTREEITEM  ppItem= m_trProjectTree.GetParentItem(pItem);//Get the parent item
			strSoilTypeName = m_trProjectTree.GetItemText(ppItem);
			if(level == 2111)
			{
				for(int i=0;i<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilTypeNum();i++) 
				{
					CSoilType *pCurSoilType = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i);
					if(strSoilTypeName == pCurSoilType->getSoilTypeName())
					{
						m_iCurSoil = i;
						for(int ii=0; ii<pCurSoilType->getInstanceNum(); ii++)
						{
							CInstance *pInstance = pCurSoilType->getInstance(ii);
							if(strItemName == pInstance->getInstanceName())
							{
								m_iCurInstance = ii;
								m_iCurOccurrence = -1;
								m_iCurExclusion = -1;
								break;
							}
						}
						break;
					}
				}
			}

			if(level==2121)
			{
				for(int i=0;i<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilTypeNum();i++) 
				{
					CSoilType *pCurSoilType = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i);
					if(strSoilTypeName == pCurSoilType->getSoilTypeName())
					{
						m_iCurSoil = i;
						for(int ii=0; ii<pCurSoilType->getOccurrenceNum(); ii++)
						{
							COccurrence *pOccurrence= pCurSoilType->getOccurrence(ii);
							if(strItemName == pOccurrence->getOccurrenceName())
							{
								m_iCurInstance = -1;
								m_iCurOccurrence = ii;
								m_iCurExclusion = -1;
								break;
							}
						}
						break;
					}
				}

				if(m_bRightClick == false)
				{
					CSpatialSettingDlg dlg;
					dlg.m_dCentralX =pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->getCentralX();
					dlg.m_dCentralY = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->getCentralY();
					dlg.m_fSearchDist = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->getSearchDist();
					dlg.m_bUseDistSim =  pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->getUseDistSim();
					//dlg.m_dZFactor =  pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->getZFactor();
					dlg.m_fDistDecayFactor = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->getDistDecayFactor();
					if(dlg.DoModal()==IDOK)
					{
						pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->setCentralX(dlg.m_dCentralX);
						pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->setCentralY(dlg.m_dCentralY);
						pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->setSearchDist(dlg.m_fSearchDist);
						pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->setUseDistSim(dlg.m_bUseDistSim);
						//pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->setZFactor(dlg.m_dZFactor);
						pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->setDistDecayFactor(dlg.m_fDistDecayFactor);

					}
				}
			}
			if(level==2131)
			{
				for(int i=0;i<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilTypeNum();i++) 
				{
					CSoilType *pCurSoilType = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i);
					if(strSoilTypeName == pCurSoilType->getSoilTypeName())
					{
						m_iCurSoil = i;
						for(int ii=0; ii<pCurSoilType->getExclusionNum(); ii++)
						{
							CExclusion *pExclusion = pCurSoilType->getExclusion(ii);
							if(strItemName == pExclusion->getExclusionName())
							{
								m_iCurInstance = -1;
								m_iCurOccurrence = -1;
								m_iCurExclusion = ii;
								break;
							}
						}
						break;
					}
				}
				if(m_bRightClick == false)
				{
					CSpatialSettingDlg dlg;

					dlg.m_dCentralX =pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->getCentralX();
					dlg.m_dCentralY = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->getCentralY();
					dlg.m_fSearchDist = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->getSearchDist();
					//dlg.m_dZFactor =  pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->getZFactor();
					dlg.m_bUseDistSim =  pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->getUseDistSim();
					dlg.m_fDistDecayFactor = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->getDistDecayFactor();
					if(dlg.DoModal()==IDOK)
					{
						pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->setCentralX(dlg.m_dCentralX);
						pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->setCentralY(dlg.m_dCentralY);
						pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->setSearchDist(dlg.m_fSearchDist);
						pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->setUseDistSim(dlg.m_bUseDistSim);
						//pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->setZFactor(dlg.m_dZFactor);
						pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->setDistDecayFactor(dlg.m_fDistDecayFactor);

					}
				}
			}
		}
		else if(level == 21111||level==21211||level==21311)  //rule level
		{
			if(pDoc->getCurViewType()!=1)
			{
				pFrame->m_splitterTabWnd.SelectView(1);
			}
			CKnowledgeView *pKnowledgeView = (CKnowledgeView *)(pFrame->m_splitterTabWnd.GetPane(0,0));	
			CString  strRuleName, strPName, strPPName,strSoilTypeName;
			strRuleName = m_trProjectTree.GetItemText(hItem);
			HTREEITEM  pItem= m_trProjectTree.GetParentItem(hItem);
			strPName = m_trProjectTree.GetItemText(pItem);
			HTREEITEM  ppItem= m_trProjectTree.GetParentItem(pItem);
			strPPName = m_trProjectTree.GetItemText(ppItem);
			HTREEITEM  pppItem= m_trProjectTree.GetParentItem(ppItem);
			strSoilTypeName = m_trProjectTree.GetItemText(pppItem);
			for(int i=0;i<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilTypeNum();i++) 
			{
				CSoilType *pCurSoilType = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i);
				if(strSoilTypeName == pCurSoilType->getSoilTypeName())
				{
					m_iCurSoil = i;
					if(strPPName == "Instances")
					{
						for(int ii=0; ii<pCurSoilType->getInstanceNum(); ii++)
						{
							CInstance *pInstance = pCurSoilType->getInstance(ii);
							if(strPName == pInstance->getInstanceName())
							{
								m_iCurInstance = ii;
								m_iCurOccurrence = -1;
								m_iCurExclusion = -1;
								for(int iii=0;iii<pInstance->getRuleNum();iii++)
								{
									if(strRuleName == pInstance->getRule(iii)->getRuleName())
									{
										m_pCurRule = pInstance->getRule(iii);
										m_iCurRule = iii;
										break;
									}
								}
								break;
							}
						}
						break;
					}
					if(strPPName == "Occurrences")
					{		
						for(int ii=0; ii<pCurSoilType->getOccurrenceNum(); ii++)
						{
							COccurrence *pOccurrence = pCurSoilType->getOccurrence(ii);
							if(strPName == pOccurrence->getOccurrenceName())
							{
								m_iCurOccurrence = ii;
								m_iCurInstance = -1;
								m_iCurExclusion = -1;
								for(int iii=0;iii<pOccurrence->getRuleNum();iii++)
								{
									if(strRuleName == pOccurrence->getRule(iii)->getRuleName())
									{
										m_pCurRule = pOccurrence->getRule(iii);
										m_iCurRule = iii;
										break;
									}
								}
								break;
							}
						}
						break;
					}
					if(strPPName == "Exclusions")
					{		
						for(int ii=0; ii<pCurSoilType->getExclusionNum(); ii++)
						{
							CExclusion *pExclusion = pCurSoilType->getExclusion(ii);
							if(strPName == pExclusion->getExclusionName())
							{
								m_iCurExclusion = ii;
								m_iCurInstance = -1;
								m_iCurOccurrence = -1;
								for(int iii=0;iii<pExclusion->getRuleNum();iii++)
								{
									if(strRuleName == pExclusion->getRule(iii)->getRuleName())
									{
										m_pCurRule = pExclusion->getRule(iii);
										m_iCurRule = iii;
										break;
									}
								}
								break;
							}
						}
						break;
					}
					break;				
				}

			}
			pKnowledgeView->FillKnowledgeView(m_pCurRule);
		}

		else if(level==4)  //CBR project
		{
			if(pDoc->m_pProjectCaseEdit!=NULL)
			{
				CProjectSummaryDlg dlg;
				dlg.m_strProjectName = pDoc->m_pProjectEdit->getProjectName();
				//dlg.m_iLayerNum = pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum();
				//dlg.m_iSoilNum = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilTypeNum();
				dlg.DoModal();



				if(pDoc->getCurViewType()== 4)
				{
					CCBInferenceView *pCBInferenceView = (CCBInferenceView *)(pFrame->m_splitterTabWnd.GetPane(0,0));	

					pCBInferenceView->SaveData();
				}

			}



		}
		else if(level==5)  //database
		{
			if(pDoc->m_pProjectCaseEdit!=NULL)
			{
				this->m_trProjectTree.Expand (m_hDatabaseCase, TVE_EXPAND);
				if(pDoc->getCurViewType()!=0)
				{
					pFrame->m_splitterTabWnd.SelectView(0);
				}	

				if(pDoc->getCurViewType()== 4)
				{
					CCBInferenceView *pCBInferenceView = (CCBInferenceView *)(pFrame->m_splitterTabWnd.GetPane(0,0));	

					pCBInferenceView->SaveData();
				}

			}
		}
		else if(level==6)  //case base
		{
			m_trProjectTree.Expand(m_hKnowledgeCase,TVE_EXPAND);


			if(pDoc->m_pProjectCaseEdit!=NULL)
			{

				if(pDoc->getCurViewType()!=3)
				{
					pFrame->m_splitterTabWnd.SelectView(3);
				}


				if(pDoc->getCurViewType()== 4)
				{
					CCBInferenceView *pCBInferenceView = (CCBInferenceView *)(pFrame->m_splitterTabWnd.GetPane(0,0));	

					pCBInferenceView->SaveData();
				}

			}
		}
		else if(level==7)  //inference
		{
			m_trProjectTree.Expand(m_hInferenceCase,TVE_EXPAND );	
		}



		else if(level==71)  //sub inference 
		{
			if(pDoc->getCurViewType()!=4)
			{
				//if(pDoc->m_pProjectCaseEdit->hDBF!=NULL)
				if(pDoc->m_pProjectCaseEdit->hCSV!=NULL)
					pFrame->m_splitterTabWnd.SelectView(4);
				else
					MessageBox("Please specify field samples first.");
			}	
		}

		else if(level==511||level==512||level==513||level==514||level==515)
		{

			if(pDoc->getCurViewType()== 4)
			{
				CCBInferenceView *pCBInferenceView = (CCBInferenceView *)(pFrame->m_splitterTabWnd.GetPane(0,0));	

				pCBInferenceView->SaveData();
			}


			CString strCurrDbLayerName=m_trProjectTree.GetItemText(hItem);

			CString strCurCategory = m_trProjectTree.GetItemText(m_trProjectTree.GetParentItem(hItem));

			if(strCurCategory == "Climate Layers")
			{
				for(int i=0; i<pDoc->m_pProjectCaseEdit->vecClimateLayerNames.size(); i++)
				{
					if(pDoc->m_pProjectCaseEdit->vecClimateLayerNames[i] ==strCurrDbLayerName)
					{


						CString layerFileName = pDoc->m_strProjectDir + "\\GISData\\Climate\\" + pDoc->m_pProjectCaseEdit->vecClimateLayerFiles[i];
						if(pApp->pCurLayer!=NULL)
							delete pApp->pCurLayer;
						pApp->pCurLayer = new CEnvLayer(0,strCurrDbLayerName,  layerFileName);



						//three DR 
					}
				}
			}
			else if(strCurCategory == "Parent Material Layers")
			{
				for(int i=0; i<pDoc->m_pProjectCaseEdit->vecGeologyLayerNames.size(); i++)
				{
					if(pDoc->m_pProjectCaseEdit->vecGeologyLayerNames[i] ==strCurrDbLayerName)
					{



						CString layerFileName = pDoc->m_strProjectDir + "\\GISData\\Geology\\" + pDoc->m_pProjectCaseEdit->vecGeologyLayerFiles[i];
						if(pApp->pCurLayer!=NULL)
							delete pApp->pCurLayer;                      
						pApp->pCurLayer = new CEnvLayer(0,strCurrDbLayerName,  layerFileName);


						//three DR 
					}
				}
			}
			else if(strCurCategory == "Topographic Layers")
			{
				for(int i=0; i<pDoc->m_pProjectCaseEdit->vecTerrainLayerNames.size(); i++)
				{
					if(pDoc->m_pProjectCaseEdit->vecTerrainLayerNames[i] ==strCurrDbLayerName)
					{

						CString layerFileName =pDoc->m_strProjectDir + "\\GISData\\Terrain\\" + pDoc->m_pProjectCaseEdit->vecTerrainLayerFiles[i]; 
						if(pApp->pCurLayer!=NULL)
							delete pApp->pCurLayer;
						pApp->pCurLayer = new CEnvLayer(0,strCurrDbLayerName,  layerFileName);


						//three DR 
					}
				}
			}
			else if(strCurCategory == "Vegetation Layers")
			{
				for(int i=0; i<pDoc->m_pProjectCaseEdit->vecVegetationLayerNames.size(); i++)
				{
					if(pDoc->m_pProjectCaseEdit->vecVegetationLayerNames[i] ==strCurrDbLayerName)
					{

						CString layerFileName = pDoc->m_strProjectDir + "\\GISData\\Vegetation\\" + pDoc->m_pProjectCaseEdit->vecVegetationLayerFiles[i]; 
						if(pApp->pCurLayer!=NULL)
							delete pApp->pCurLayer;
						pApp->pCurLayer = new CEnvLayer(0,strCurrDbLayerName,  layerFileName);


						//three DR 
					}
				}
			}
			else if(strCurCategory == "Other Layers")
			{
				for(int i=0; i<pDoc->m_pProjectCaseEdit->vecOtherLayerNames.size(); i++)
				{
					if(pDoc->m_pProjectCaseEdit->vecOtherLayerNames[i] ==strCurrDbLayerName)
					{

						CString layerFileName = pDoc->m_strProjectDir + "\\GISData\\Other\\" + pDoc->m_pProjectCaseEdit->vecOtherLayerFiles[i]; 
						if(pApp->pCurLayer!=NULL)
							delete pApp->pCurLayer;
						pApp->pCurLayer = new CEnvLayer(0,strCurrDbLayerName,  layerFileName);

						//three DR 
					}
				}
			}




			if(pDoc->getCurViewType()!=0)
			{
				pFrame->m_splitterTabWnd.SelectView(0);
			}
			pDoc->UpdateAllViews(NULL);
			CDatabaseView *pDatabaseView = (CDatabaseView *)(pFrame->m_splitterTabWnd.GetPane(0,0));	
			CInfoFrameView *pInfoFrameView = (CInfoFrameView *)(pDatabaseView->m_wndSplitter.GetPane(1,0));	
			CMetadataView *pMetadataView = (CMetadataView *)(pInfoFrameView->m_wndSplitter.GetPane(0,0));	
			pMetadataView->FillMetadataView();
		}
	}
}

void CLeftControlBar::FillProjectTree(CSoLIMDoc *pDoc)
{
	if(pDoc->m_iProjectType == 0)
	{

		if(pDoc->m_pProjectEdit!=NULL)
		{
			FillDatabaseTree(pDoc);
			FillKnowledgeTree(pDoc);
			FillInferenceTree(pDoc);
		}
		m_trProjectTree.Expand (m_hRootRule, TVE_EXPAND);
		int curViewType = pDoc->getCurViewType();
		if(curViewType == 0)
			m_trProjectTree.Expand (m_hDatabase, TVE_EXPAND);
		else if(curViewType == 1)
			m_trProjectTree.Expand (m_hKnowledge, TVE_EXPAND);
		else if(curViewType == 2)
			m_trProjectTree.Expand (m_hInference, TVE_EXPAND);


		m_trProjectTree.DeleteItem(m_hDatabaseCase);
		m_trProjectTree.DeleteItem(m_hKnowledgeCase);
		m_trProjectTree.DeleteItem(m_hInferenceCase);

		m_trProjectTree.SetItemState(m_hRootCase, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK );
		m_hDatabaseCase = m_trProjectTree.InsertItem (_T("GIS Database"), 1, 1, m_hRootCase);
		m_trProjectTree.SetItemState(m_hDatabaseCase, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK );
		m_hKnowledgeCase = m_trProjectTree.InsertItem (_T("Field Samples"), 1, 1, m_hRootCase);
		m_trProjectTree.SetItemState(m_hKnowledgeCase, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK );
		m_hInferenceCase = m_trProjectTree.InsertItem (_T("Inference"), 1, 1, m_hRootCase);			
		m_trProjectTree.SetItemState(m_hInferenceCase, INDEXTOSTATEIMAGEMASK(3), TVIS_STATEIMAGEMASK );
		m_trProjectTree.Expand (m_hRootCase, TVE_EXPAND);


		m_hClimate= m_trProjectTree.InsertItem("Climate Layers",m_hDatabaseCase);
		m_trProjectTree.SetItemState(m_hClimate, INDEXTOSTATEIMAGEMASK(5), TVIS_STATEIMAGEMASK );
		m_hGeology= m_trProjectTree.InsertItem("Parent Material Layers",m_hDatabaseCase);
		m_trProjectTree.SetItemState(m_hGeology, INDEXTOSTATEIMAGEMASK(5), TVIS_STATEIMAGEMASK );
		m_hTerrain= m_trProjectTree.InsertItem("Topographic Layers",m_hDatabaseCase);
		m_trProjectTree.SetItemState(m_hTerrain, INDEXTOSTATEIMAGEMASK(5), TVIS_STATEIMAGEMASK );
		m_hVegetation= m_trProjectTree.InsertItem("Vegetation Layers",m_hDatabaseCase);
		m_trProjectTree.SetItemState(m_hVegetation, INDEXTOSTATEIMAGEMASK(5), TVIS_STATEIMAGEMASK );
		m_hOther= m_trProjectTree.InsertItem("Other Layers",m_hDatabaseCase);
		m_trProjectTree.SetItemState(m_hOther, INDEXTOSTATEIMAGEMASK(5), TVIS_STATEIMAGEMASK );
	}
	else if(pDoc->m_iProjectType == 1)
	{

		if(pDoc->m_pProjectCaseEdit!=NULL)
		{

			m_trProjectTree.DeleteItem(m_hDatabase);
			m_trProjectTree.DeleteItem(m_hKnowledge);
			m_trProjectTree.DeleteItem(m_hInference);

			m_trProjectTree.SetItemState(m_hRootRule, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK );
			m_hDatabase = m_trProjectTree.InsertItem (_T("GIS Database"), 1, 1, m_hRootRule);
			m_trProjectTree.SetItemState(m_hDatabase, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK );
			m_hKnowledge = m_trProjectTree.InsertItem (_T("Knowledge Base"), 1, 1, m_hRootRule);
			m_trProjectTree.SetItemState(m_hKnowledge, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK );
			m_hInference = m_trProjectTree.InsertItem (_T("Inference"), 1, 1, m_hRootRule);			
			m_trProjectTree.SetItemState(m_hInference, INDEXTOSTATEIMAGEMASK(3), TVIS_STATEIMAGEMASK );
			m_trProjectTree.Expand (m_hRootRule, TVE_EXPAND);


			FillDatabaseCaseTree(pDoc);
			FillKnowledgeCaseTree(pDoc);
			FillInferenceCaseTree(pDoc);
		}
		m_trProjectTree.Expand (m_hRootRule, TVE_EXPAND);
		int curViewType = pDoc->getCurViewType();
		if(curViewType == 0)
			m_trProjectTree.Expand (m_hDatabaseCase, TVE_EXPAND);
		else if(curViewType == 1)
			m_trProjectTree.Expand (m_hKnowledgeCase, TVE_EXPAND);
		else if(curViewType == 2)
			m_trProjectTree.Expand (m_hInferenceCase, TVE_EXPAND);
	}
}


void CLeftControlBar::FillDatabaseTree(CSoLIMDoc *pDoc)
{	
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	m_trProjectTree.DeleteItem(m_hDatabase);
	m_hDatabase = m_trProjectTree.InsertItem (_T("GIS Database"), 1, 1, m_hRootRule);
	m_trProjectTree.SetItemState(m_hDatabase, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK );
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

void CLeftControlBar::FillDatabaseCaseTree(CSoLIMDoc *pDoc)
{	
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	m_trProjectTree.DeleteItem(m_hDatabaseCase);
	m_hDatabaseCase = m_trProjectTree.InsertItem (_T("GIS Database"), 1, 1, m_hRootCase);
	m_trProjectTree.SetItemState(m_hDatabase, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK );

	m_hClimate= m_trProjectTree.InsertItem("Climate Layers",m_hDatabaseCase);
	m_trProjectTree.SetItemState(m_hClimate, INDEXTOSTATEIMAGEMASK(5), TVIS_STATEIMAGEMASK );
	m_hGeology= m_trProjectTree.InsertItem("Parent Material Layers",m_hDatabaseCase);
	m_trProjectTree.SetItemState(m_hGeology, INDEXTOSTATEIMAGEMASK(5), TVIS_STATEIMAGEMASK );
	m_hTerrain= m_trProjectTree.InsertItem("Topographic Layers",m_hDatabaseCase);
	m_trProjectTree.SetItemState(m_hTerrain, INDEXTOSTATEIMAGEMASK(5), TVIS_STATEIMAGEMASK );
	m_hVegetation= m_trProjectTree.InsertItem("Vegetation Layers",m_hDatabaseCase);
	m_trProjectTree.SetItemState(m_hVegetation, INDEXTOSTATEIMAGEMASK(5), TVIS_STATEIMAGEMASK );
	m_hOther= m_trProjectTree.InsertItem("Other Layers",m_hDatabaseCase);
	m_trProjectTree.SetItemState(m_hOther, INDEXTOSTATEIMAGEMASK(5), TVIS_STATEIMAGEMASK );


	if(pDoc->m_pProjectCaseEdit!=NULL)
	{
		for(int i=0;i<pDoc->m_pProjectCaseEdit->vecClimateLayerNames.size();i++) 
		{
			HTREEITEM SubHti = m_trProjectTree.InsertItem(pDoc->m_pProjectCaseEdit->vecClimateLayerNames[i],1,1,m_hClimate);
			m_trProjectTree.SetItemState(SubHti, INDEXTOSTATEIMAGEMASK(4), TVIS_STATEIMAGEMASK );
			/*if(i==0)
			{
			pApp->m_iCurrEnvLayerIndex=0;
			}*/

		}
		for(int i=0;i<pDoc->m_pProjectCaseEdit->vecGeologyLayerNames.size();i++) 
		{
			HTREEITEM SubHti = m_trProjectTree.InsertItem(pDoc->m_pProjectCaseEdit->vecGeologyLayerNames[i],1,1,m_hGeology);
			m_trProjectTree.SetItemState(SubHti, INDEXTOSTATEIMAGEMASK(4), TVIS_STATEIMAGEMASK );
			/*if(i==0)
			{
			pApp->m_iCurrEnvLayerIndex=0;
			}*/

		}
		for(int i=0;i<pDoc->m_pProjectCaseEdit->vecTerrainLayerNames.size();i++) 
		{
			HTREEITEM SubHti = m_trProjectTree.InsertItem(pDoc->m_pProjectCaseEdit->vecTerrainLayerNames[i],1,1,m_hTerrain);
			m_trProjectTree.SetItemState(SubHti, INDEXTOSTATEIMAGEMASK(4), TVIS_STATEIMAGEMASK );
			/*if(i==0)
			{
			pApp->m_iCurrEnvLayerIndex=0;
			}*/

		}
		for(int i=0;i<pDoc->m_pProjectCaseEdit->vecVegetationLayerNames.size();i++) 
		{
			HTREEITEM SubHti = m_trProjectTree.InsertItem(pDoc->m_pProjectCaseEdit->vecVegetationLayerNames[i],1,1,m_hVegetation);
			m_trProjectTree.SetItemState(SubHti, INDEXTOSTATEIMAGEMASK(4), TVIS_STATEIMAGEMASK );
			/*if(i==0)
			{
			pApp->m_iCurrEnvLayerIndex=0;
			}*/

		}
		for(int i=0;i<pDoc->m_pProjectCaseEdit->vecOtherLayerNames.size();i++) 
		{
			HTREEITEM SubHti = m_trProjectTree.InsertItem(pDoc->m_pProjectCaseEdit->vecOtherLayerNames[i],1,1,m_hOther);
			m_trProjectTree.SetItemState(SubHti, INDEXTOSTATEIMAGEMASK(4), TVIS_STATEIMAGEMASK );
			/*if(i==0)
			{
			pApp->m_iCurrEnvLayerIndex=0;
			}*/

		}
	}
}

void CLeftControlBar::FillKnowledgeTree(CSoLIMDoc *pDoc)
{	
	m_trProjectTree.DeleteItem(m_hKnowledge);
	m_hKnowledge = m_trProjectTree.InsertItem (_T("Knowledge Base"), 1, 1, m_hRootRule);
	m_trProjectTree.SetItemState(m_hKnowledge, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK );
	if(pDoc->m_pProjectEdit == NULL)
		return;
	else if(pDoc->m_pProjectEdit->m_pKnowledgeBase != NULL)
	{
		int iSoilTypeNum= pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilTypeNum();
		for(int i=0;i<iSoilTypeNum;i++)
		{
			CString strSoilTypeName;
			strSoilTypeName = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->getSoilTypeName();
			HTREEITEM SubHti1= m_trProjectTree.InsertItem(strSoilTypeName,m_hKnowledge);
			m_trProjectTree.SetItemState(SubHti1, INDEXTOSTATEIMAGEMASK(5), TVIS_STATEIMAGEMASK );
			HTREEITEM hInstances= m_trProjectTree.InsertItem("Instances",SubHti1);
			m_trProjectTree.SetItemState(hInstances, INDEXTOSTATEIMAGEMASK(6), TVIS_STATEIMAGEMASK );
			HTREEITEM hOccurrences= m_trProjectTree.InsertItem("Occurrences",SubHti1);
			m_trProjectTree.SetItemState(hOccurrences, INDEXTOSTATEIMAGEMASK(6), TVIS_STATEIMAGEMASK );
			HTREEITEM hExclusions= m_trProjectTree.InsertItem("Exclusions",SubHti1);
			m_trProjectTree.SetItemState(hExclusions, INDEXTOSTATEIMAGEMASK(6), TVIS_STATEIMAGEMASK );

			for(int j=0;j<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->getInstanceNum();j++)
			{
				CString strInstanceName;     
				strInstanceName = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->getInstance(j)->getInstanceName();	          
				HTREEITEM SubHti2= m_trProjectTree.InsertItem(_T(strInstanceName),hInstances);
				m_trProjectTree.SetItemState(SubHti2, INDEXTOSTATEIMAGEMASK(7), TVIS_STATEIMAGEMASK );
				for(int k=0;k<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->getInstance(j)->getRuleNum(); k++)
				{
					CString strRuleName;
					strRuleName = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->getInstance(j)->getRule(k)->getRuleName();
					HTREEITEM SubHti3= m_trProjectTree.InsertItem(_T(strRuleName),SubHti2);				 
					if(pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->getInstance(j)->getRule(k)->getIsUsed()==true)   		          
						m_trProjectTree.SetItemState( SubHti3, INDEXTOSTATEIMAGEMASK(8), TVIS_STATEIMAGEMASK );
					else                   
						m_trProjectTree.SetItemState( SubHti3, INDEXTOSTATEIMAGEMASK(9), TVIS_STATEIMAGEMASK );
				}		
				m_trProjectTree.Expand(SubHti2,TVE_COLLAPSE);
			}

			for(int j=0;j<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->getOccurrenceNum(); j++)
			{
				CString strOccurrenceName;   
				strOccurrenceName = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->getOccurrence(j)->getOccurrenceName();	          	
				HTREEITEM SubHti2= m_trProjectTree.InsertItem(_T(strOccurrenceName),hOccurrences);
				m_trProjectTree.SetItemState(SubHti2, INDEXTOSTATEIMAGEMASK(7), TVIS_STATEIMAGEMASK );
				for(int k=0;k<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->getOccurrence(j)->getRuleNum(); k++)
				{  
					CString strRuleName;
					strRuleName = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->getOccurrence(j)->getRule(k)->getRuleName();
					HTREEITEM SubHti3= m_trProjectTree.InsertItem(_T(strRuleName),SubHti2);				 
					if(pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->getOccurrence(j)->getRule(k)->getIsUsed()==true)   		          
						m_trProjectTree.SetItemState( SubHti3, INDEXTOSTATEIMAGEMASK(8), TVIS_STATEIMAGEMASK );
					else                   
						m_trProjectTree.SetItemState( SubHti3, INDEXTOSTATEIMAGEMASK(9), TVIS_STATEIMAGEMASK );
				}		
				m_trProjectTree.Expand( SubHti2,TVE_COLLAPSE);
			}

			for(int j=0;j<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->getExclusionNum(); j++)
			{
				CString strExclusionName;   
				strExclusionName = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->getExclusion(j)->getExclusionName();	          	
				HTREEITEM SubHti2= m_trProjectTree.InsertItem(_T(strExclusionName),hExclusions);
				m_trProjectTree.SetItemState(SubHti2, INDEXTOSTATEIMAGEMASK(7), TVIS_STATEIMAGEMASK );
				for(int k=0;k<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->getExclusion(j)->getRuleNum(); k++)
				{ 
					CString strRuleName;
					strRuleName = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->getExclusion(j)->getRule(k)->getRuleName();
					HTREEITEM SubHti3= m_trProjectTree.InsertItem(_T(strRuleName),SubHti2);				 
					if(pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->getExclusion(j)->getRule(k)->getIsUsed()==true)   		          
						m_trProjectTree.SetItemState( SubHti3, INDEXTOSTATEIMAGEMASK(8), TVIS_STATEIMAGEMASK );
					else                   
						m_trProjectTree.SetItemState( SubHti3, INDEXTOSTATEIMAGEMASK(9), TVIS_STATEIMAGEMASK );
				}		
				m_trProjectTree.Expand( SubHti2,TVE_COLLAPSE);
			}
			m_trProjectTree.Expand( SubHti1,TVE_EXPAND);
			m_trProjectTree.Expand( hInstances,TVE_EXPAND);
			m_trProjectTree.Expand( hOccurrences,TVE_EXPAND);
			m_trProjectTree.Expand( hExclusions,TVE_EXPAND);
		}
	}

}


void CLeftControlBar::FillKnowledgeCaseTree(CSoLIMDoc *pDoc)
{	
	m_trProjectTree.DeleteItem(m_hKnowledgeCase);
	m_hKnowledgeCase = m_trProjectTree.InsertItem (_T("Field Samples"), 1, 1, m_hRootCase);
	m_trProjectTree.SetItemState(m_hKnowledgeCase, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK );
	if(pDoc->m_pProjectCaseEdit == NULL)
		return;
	/*else if(pDoc->m_pProjectEdit->m_pKnowledgeBase != NULL)
	{

	}*/

}


void CLeftControlBar::FillInferenceTree(CSoLIMDoc *pDoc)
{
	m_trProjectTree.DeleteItem(m_hInference);
	m_hInference = m_trProjectTree.InsertItem (_T("Inference"), 1, 1, m_hRootRule);	
	m_trProjectTree.SetItemState(m_hInferenceCase, INDEXTOSTATEIMAGEMASK(3), TVIS_STATEIMAGEMASK );
	HTREEITEM hRunInference= m_trProjectTree.InsertItem("Inference",m_hInference);
	m_trProjectTree.SetItemState(hRunInference, INDEXTOSTATEIMAGEMASK(3), TVIS_STATEIMAGEMASK );
}

void CLeftControlBar:: FillInferenceCaseTree(CSoLIMDoc *pDoc)
{
	m_trProjectTree.DeleteItem(m_hInferenceCase);
	m_hInferenceCase = m_trProjectTree.InsertItem (_T("Inference"), 1, 1, m_hRootCase);	
	m_trProjectTree.SetItemState(m_hInferenceCase, INDEXTOSTATEIMAGEMASK(3), TVIS_STATEIMAGEMASK );
	HTREEITEM hRunInference= m_trProjectTree.InsertItem("Inference",m_hInferenceCase);
	m_trProjectTree.SetItemState(hRunInference, INDEXTOSTATEIMAGEMASK(3), TVIS_STATEIMAGEMASK );
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
	//else if(hItem == m_theTree.GetRootItem()) return 0; 
	else if(hItem == this->m_hRootRule) return 0;
	else if(hItem == m_hDatabase) return 1;
	else if(hItem == m_hKnowledge) return 2;
	else if(hItem == m_hInference) return 3;
	else if(m_theTree.GetParentItem(hItem) == m_hDatabase) return 11;	
	else if(m_theTree.GetParentItem(hItem) == m_hKnowledge) return 21;
	else if(m_theTree.GetParentItem(hItem) == m_hInference) return 31;
	else if(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem))== m_hKnowledge && m_theTree.GetItemText(hItem)=="Instances") return 211;
	else if(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem))== m_hKnowledge && m_theTree.GetItemText(hItem)=="Occurrences") return 212;
	else if(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem))== m_hKnowledge && m_theTree.GetItemText(hItem)=="Exclusions") return 213;
	else if(m_theTree.GetParentItem(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem)))== m_hKnowledge&&m_theTree.GetItemText(m_theTree.GetParentItem(hItem)) == "Instances") return 2111;
	else if(m_theTree.GetParentItem(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem)))== m_hKnowledge&&m_theTree.GetItemText(m_theTree.GetParentItem(hItem)) == "Occurrences") return 2121;
	else if(m_theTree.GetParentItem(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem)))== m_hKnowledge&&m_theTree.GetItemText(m_theTree.GetParentItem(hItem)) == "Exclusions") return 2131;
	else if(m_theTree.GetParentItem(m_theTree.GetParentItem(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem))))== m_hKnowledge && m_theTree.GetItemText(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem)))=="Instances") return 21111;
	else if(m_theTree.GetParentItem(m_theTree.GetParentItem(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem))))== m_hKnowledge && m_theTree.GetItemText(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem)))=="Occurrences") return  21211;
	else if(m_theTree.GetParentItem(m_theTree.GetParentItem(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem))))== m_hKnowledge && m_theTree.GetItemText(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem)))=="Exclusions") return 21311;


	else if(hItem == this->m_hRootCase) return 4;
	else if(hItem == m_hDatabaseCase) return 5;
	else if(hItem == m_hKnowledgeCase) return 6;
	else if(hItem == m_hInferenceCase) return 7;
	else if(m_theTree.GetParentItem(hItem) == m_hDatabaseCase) return 51;	
	else if(m_theTree.GetParentItem(hItem) == m_hKnowledgeCase) return 61;
	else if(m_theTree.GetParentItem(hItem) == m_hInferenceCase) return 71;

	else if(m_theTree.GetParentItem(hItem)== m_hClimate) return 511;
	else if(m_theTree.GetParentItem(hItem)== m_hGeology) return 512;
	else if(m_theTree.GetParentItem(hItem)== m_hTerrain) return 513;
	else if(m_theTree.GetParentItem(hItem)== m_hVegetation) return 514;
	else if(m_theTree.GetParentItem(hItem)== m_hOther) return 515;

	else return -1; 
	return -1;
}

void CLeftControlBar::ExpandNode(int iNodeType)
{
	if(iNodeType == 0)
		m_trProjectTree.Expand (m_hDatabase, TVE_EXPAND);	 
	else if(iNodeType == 1)
		m_trProjectTree.Expand(m_hKnowledge,TVE_EXPAND);     
	else if(iNodeType == 2)
		m_trProjectTree.Expand(m_hInference, TVE_EXPAND);

	else if(iNodeType == 3)
		m_trProjectTree.Expand (m_hDatabaseCase, TVE_EXPAND);	 
	else if(iNodeType == 4)
		m_trProjectTree.Expand(m_hKnowledgeCase,TVE_EXPAND);     
	else if(iNodeType == 5)
		m_trProjectTree.Expand(m_hInferenceCase, TVE_EXPAND);
}

void CLeftControlBar::CleanMemory()
{
	m_hPreviousItem = m_hRootRule;
	m_pCurRule = NULL;

	m_trProjectTree.DeleteItem(m_hDatabase);
	m_hDatabase = m_trProjectTree.InsertItem (_T("GIS Database"), 1, 1, m_hRootRule);
	m_trProjectTree.SetItemState(m_hDatabase, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK );

	m_trProjectTree.DeleteItem(m_hKnowledge);
	m_hKnowledge = m_trProjectTree.InsertItem (_T("Knowledge Base"), 1, 1, m_hRootRule);
	m_trProjectTree.SetItemState(m_hKnowledge, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK );

	m_trProjectTree.DeleteItem(m_hInference);
	m_hInference = m_trProjectTree.InsertItem (_T("Inference"), 1, 1, m_hRootRule);	
	m_trProjectTree.SetItemState(m_hInference, INDEXTOSTATEIMAGEMASK(3), TVIS_STATEIMAGEMASK );


	m_trProjectTree.DeleteItem(m_hDatabaseCase);
	m_hDatabaseCase = m_trProjectTree.InsertItem (_T("GIS Database"), 1, 1, m_hRootCase);
	m_trProjectTree.SetItemState(m_hDatabaseCase, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK );

	m_trProjectTree.DeleteItem(m_hKnowledgeCase);
	m_hKnowledgeCase = m_trProjectTree.InsertItem (_T("Field Samples"), 1, 1, m_hRootCase);
	m_trProjectTree.SetItemState(m_hKnowledgeCase, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK );

	m_trProjectTree.DeleteItem(m_hInferenceCase);
	m_hInferenceCase = m_trProjectTree.InsertItem (_T("Inference"), 1, 1, m_hRootCase);	
	m_trProjectTree.SetItemState(m_hInferenceCase, INDEXTOSTATEIMAGEMASK(3), TVIS_STATEIMAGEMASK );
	//HTREEITEM hRunInference= m_trProjectTree.InsertItem("Inference",m_hInference);
	//m_trProjectTree.SetItemState(hRunInference, INDEXTOSTATEIMAGEMASK(3), TVIS_STATEIMAGEMASK );

}






void CLeftControlBar::OnDatabaseAddlayer()
{	

	CFileManager* fm = new CFileManager();

	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
	CString layerFileName;
	//static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
	//CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_ALLOWMULTISELECT |OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|GDAL-Supported Raster Formats (*.*)|*.*|shapefile (*.shp)|*.shp|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.*", OFN_ALLOWMULTISELECT |OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	m_ldFile.m_ofn.lpstrTitle="Add Layer(s)";
	CString   strFileNames;  
	m_ldFile.m_ofn.lpstrFile=strFileNames.GetBuffer(4000); 
	m_ldFile.m_ofn.nMaxFile= 4000 ;  

	CString msg;

	if(m_ldFile.DoModal() == IDOK)
	{

		CString strTempFileName;
		POSITION mPos= m_ldFile.GetStartPosition(); 
		while(mPos!=NULL) 
		{         
			layerFileName = m_ldFile.GetNextPathName(mPos);         
			
			// readin...
			CFileManager* fm = new CFileManager(pDoc, m_hWnd);
			CThreeDR* newLayer = fm->Readin(layerFileName);
			if(newLayer == NULL) { return; }
			CString LayerName = newLayer->getDataName();  
			CString strLayerFileName = extractFileName(layerFileName);							
			strLayerFileName = dropFileSuffix(strLayerFileName);
			//CString outputPath = pDoc->m_strProjectDir+"\\GISData\\" + strCategory + "\\"+strLayerFileName + ".3dr";

			strLayerFileName = extractFileName(layerFileName);
			strLayerFileName = dropFileSuffix(strLayerFileName);
			CString strNewLayerFilePath= pDoc->m_strProjectDir+"\\GISData\\"+strLayerFileName + ".3dr";

			//CString layerFileFullName = outputPath;
			//CString FileName = extractFileName(layerFileFullName);
			fm->Writeout3dr(newLayer, strNewLayerFilePath);

			CString strLayerName = "untitled";
			CEnvLayer *pLayer = new CEnvLayer(pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum(), strLayerName, strNewLayerFilePath );
			if(pLayer->getLayerID() != -1)
			{ 
				strLayerName.Format("%s",pLayer->m_pTheData->getDataName());
				pLayer->setLayerName(strLayerName);
				int i=0;
				for(i=0;i<pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum();i++) 
				{
					if((pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(i)->getLayerName())==strLayerName)
					{
						AfxMessageBox("The name of the layer is the same with one layer the database already has. Please use another layer.");
						delete pLayer;
						pLayer = NULL;
						break;
					}
				}
				if(i!=pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum())
					continue;


				pLayer->setFileName(strNewLayerFilePath);
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
			//scan the knowledge base to disable rules that use this layer
			for(int m=0;m<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilTypeNum();m++)
			{
				for(int n1=0;n1<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getInstanceNum();n1++)				
					for(int k=0;k<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getInstance(n1)->getRuleNum();k++)					
						if(strCurrDbLayerName== pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getInstance(n1)->getRule(k)->getLayerName())
						{
							pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getInstance(n1)->getRule(k)->setLayerName("");

							if(pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getInstance(n1)->getRule(k)->getRuleType()==3)  //point rule
							{
								((CPointRule *)(pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getInstance(n1)->getRule(k)))->ComputeCentralValue(pDoc->m_pProjectEdit->m_pEnvDatabase);
								pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getInstance(n1)->getRule(k)->setCurGraphMin(
									pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getInstance(n1)->getRule(k)->ComputeRuleGraphMin());
								pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getInstance(n1)->getRule(k)->setCurGraphMax(
									pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getInstance(n1)->getRule(k)->ComputeRuleGraphMax());

								pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getInstance(n1)->getRule(k)->setDispMin(
									pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getInstance(n1)->getRule(k)->ComputeRuleGraphMin());
								pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getInstance(n1)->getRule(k)->setDispMax(
									pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getInstance(n1)->getRule(k)->ComputeRuleGraphMax());

							}
						}
						for(int n2=0;n2<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getOccurrenceNum();n2++)				
							for(int k=0;k<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getOccurrence(n2)->getRuleNum();k++)					
								if(strCurrDbLayerName== pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getOccurrence(n2)->getRule(k)->getLayerName())
								{
									pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getOccurrence(n2)->getRule(k)->setLayerName("");

									if(pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getOccurrence(n2)->getRule(k)->getRuleType()==3)  //point rule
									{
										((CPointRule *)(pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getOccurrence(n2)->getRule(k)))->ComputeCentralValue(pDoc->m_pProjectEdit->m_pEnvDatabase);
										pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getOccurrence(n2)->getRule(k)->setCurGraphMin(
											pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getOccurrence(n2)->getRule(k)->ComputeRuleGraphMin());
										pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getOccurrence(n2)->getRule(k)->setCurGraphMax(
											pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getOccurrence(n2)->getRule(k)->ComputeRuleGraphMax());

										pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getOccurrence(n2)->getRule(k)->setDispMin(
											pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getOccurrence(n2)->getRule(k)->ComputeRuleGraphMin());
										pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getOccurrence(n2)->getRule(k)->setDispMax(
											pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getOccurrence(n2)->getRule(k)->ComputeRuleGraphMax());

									}
								}
								for(int n3=0;n3<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getExclusionNum();n3++)				
									for(int k=0;k<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getExclusion(n3)->getRuleNum();k++)					
										if(strCurrDbLayerName== pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getExclusion(n3)->getRule(k)->getLayerName())
										{
											pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getExclusion(n3)->getRule(k)->setLayerName("");

											if(pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getExclusion(n3)->getRule(k)->getRuleType()==3)  //point rule
											{
												((CPointRule *)(pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getExclusion(n3)->getRule(k)))->ComputeCentralValue(pDoc->m_pProjectEdit->m_pEnvDatabase);

												pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getExclusion(n3)->getRule(k)->setCurGraphMin(
													pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getExclusion(n3)->getRule(k)->ComputeRuleGraphMin());
												pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getExclusion(n3)->getRule(k)->setCurGraphMax(
													pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getExclusion(n3)->getRule(k)->ComputeRuleGraphMax());

												pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getExclusion(n3)->getRule(k)->setDispMin(
													pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getExclusion(n3)->getRule(k)->ComputeRuleGraphMin());
												pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getExclusion(n3)->getRule(k)->setDispMax(
													pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m)->getExclusion(n3)->getRule(k)->ComputeRuleGraphMax());

											}
										}
			}


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



void CLeftControlBar::OnDatalayerCaseDelete()
{
	MessageBox("Delete Layer");
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
	CString strCurrDbLayerName=m_trProjectTree.GetItemText(m_trProjectTree.GetSelectedItem());
	CString strCategory = m_trProjectTree.GetItemText(m_trProjectTree.GetParentItem(m_trProjectTree.GetSelectedItem()));


	if(strCategory == "Climate Layers");
	{

		for(int i=0; i<pDoc->m_pProjectCaseEdit->vecClimateLayerNames.size();i++)
		{
			if(pDoc->m_pProjectCaseEdit->vecClimateLayerNames[i] == strCurrDbLayerName)
			{
				pDoc->m_pProjectCaseEdit->vecClimateLayerNames.erase(pDoc->m_pProjectCaseEdit->vecClimateLayerNames.begin() + i);
				pDoc->m_pProjectCaseEdit->vecClimateLayerFiles.erase(pDoc->m_pProjectCaseEdit->vecClimateLayerFiles.begin() + i);
				pDoc->m_pProjectCaseEdit->g_vecstrClimateLayers.erase(pDoc->m_pProjectCaseEdit->g_vecstrClimateLayers.begin() + i);
				pDoc->m_pProjectCaseEdit->vecClimateLayerResolutions.erase(pDoc->m_pProjectCaseEdit->vecClimateLayerResolutions.begin() + i);
				pDoc->m_pProjectCaseEdit->vecClimateLayerSimiTypes.erase(pDoc->m_pProjectCaseEdit->vecClimateLayerSimiTypes.begin() + i);
				pDoc->m_pProjectCaseEdit->vecClimateLayerSimiMethods.erase(pDoc->m_pProjectCaseEdit->vecClimateLayerSimiMethods.begin() + i);
				break;
			}
		}
	}

	if(strCategory == "Parent Material Layers");
	{

		for(int i=0; i<pDoc->m_pProjectCaseEdit->vecGeologyLayerNames.size();i++)
		{
			if(pDoc->m_pProjectCaseEdit->vecGeologyLayerNames[i] == strCurrDbLayerName)
			{
				pDoc->m_pProjectCaseEdit->vecGeologyLayerNames.erase(pDoc->m_pProjectCaseEdit->vecGeologyLayerNames.begin() + i);
				pDoc->m_pProjectCaseEdit->vecGeologyLayerFiles.erase(pDoc->m_pProjectCaseEdit->vecGeologyLayerFiles.begin() + i);
				pDoc->m_pProjectCaseEdit->g_vecstrGeologyLayers.erase(pDoc->m_pProjectCaseEdit->g_vecstrGeologyLayers.begin() + i);
				pDoc->m_pProjectCaseEdit->vecGeologyLayerResolutions.erase(pDoc->m_pProjectCaseEdit->vecGeologyLayerResolutions.begin() + i);
				pDoc->m_pProjectCaseEdit->vecGeologyLayerSimiTypes.erase(pDoc->m_pProjectCaseEdit->vecGeologyLayerSimiTypes.begin() + i);
				pDoc->m_pProjectCaseEdit->vecGeologyLayerSimiMethods.erase(pDoc->m_pProjectCaseEdit->vecGeologyLayerSimiMethods.begin() + i);
				break;
			}
		}
	}

	if(strCategory == "Topographic Layers");
	{

		for(int i=0; i<pDoc->m_pProjectCaseEdit->vecTerrainLayerNames.size();i++)
		{
			if(pDoc->m_pProjectCaseEdit->vecTerrainLayerNames[i] == strCurrDbLayerName)
			{
				pDoc->m_pProjectCaseEdit->vecTerrainLayerNames.erase(pDoc->m_pProjectCaseEdit->vecTerrainLayerNames.begin() + i);
				pDoc->m_pProjectCaseEdit->vecTerrainLayerFiles.erase(pDoc->m_pProjectCaseEdit->vecTerrainLayerFiles.begin() + i);
				pDoc->m_pProjectCaseEdit->g_vecstrTerrainLayers.erase(pDoc->m_pProjectCaseEdit->g_vecstrTerrainLayers.begin() + i);
				pDoc->m_pProjectCaseEdit->vecTerrainLayerResolutions.erase(pDoc->m_pProjectCaseEdit->vecTerrainLayerResolutions.begin() + i);
				pDoc->m_pProjectCaseEdit->vecTerrainLayerSimiTypes.erase(pDoc->m_pProjectCaseEdit->vecTerrainLayerSimiTypes.begin() + i);
				pDoc->m_pProjectCaseEdit->vecTerrainLayerSimiMethods.erase(pDoc->m_pProjectCaseEdit->vecTerrainLayerSimiMethods.begin() + i);

				break;
			}
		}
	}

	if(strCategory == "Vegetation Layers");
	{

		for(int i=0; i<pDoc->m_pProjectCaseEdit->vecVegetationLayerNames.size();i++)
		{
			if(pDoc->m_pProjectCaseEdit->vecClimateLayerNames[i] == strCurrDbLayerName)
			{
				pDoc->m_pProjectCaseEdit->vecVegetationLayerNames.erase(pDoc->m_pProjectCaseEdit->vecVegetationLayerNames.begin() + i);
				pDoc->m_pProjectCaseEdit->vecVegetationLayerFiles.erase(pDoc->m_pProjectCaseEdit->vecVegetationLayerFiles.begin() + i);				
				pDoc->m_pProjectCaseEdit->g_vecstrVegetationLayers.erase(pDoc->m_pProjectCaseEdit->g_vecstrVegetationLayers.begin() + i);
				pDoc->m_pProjectCaseEdit->vecVegetationLayerResolutions.erase(pDoc->m_pProjectCaseEdit->vecVegetationLayerResolutions.begin() + i);
				pDoc->m_pProjectCaseEdit->vecVegetationLayerSimiTypes.erase(pDoc->m_pProjectCaseEdit->vecVegetationLayerSimiTypes.begin() + i);
				pDoc->m_pProjectCaseEdit->vecVegetationLayerSimiMethods.erase(pDoc->m_pProjectCaseEdit->vecVegetationLayerSimiMethods.begin() + i);
				break;
			}
		}
	}

	if(strCategory == "Other Layers");
	{

		for(int i=0; i<pDoc->m_pProjectCaseEdit->vecOtherLayerNames.size();i++)
		{
			if(pDoc->m_pProjectCaseEdit->vecClimateLayerNames[i] == strCurrDbLayerName)
			{
				pDoc->m_pProjectCaseEdit->vecOtherLayerNames.erase(pDoc->m_pProjectCaseEdit->vecOtherLayerNames.begin() + i);
				pDoc->m_pProjectCaseEdit->vecOtherLayerFiles.erase(pDoc->m_pProjectCaseEdit->vecOtherLayerFiles.begin() + i);

				pDoc->m_pProjectCaseEdit->g_vecstrOtherLayers.erase(pDoc->m_pProjectCaseEdit->g_vecstrOtherLayers.begin() + i);
				pDoc->m_pProjectCaseEdit->vecOtherLayerResolutions.erase(pDoc->m_pProjectCaseEdit->vecOtherLayerResolutions.begin() + i);
				pDoc->m_pProjectCaseEdit->vecOtherLayerSimiTypes.erase(pDoc->m_pProjectCaseEdit->vecOtherLayerSimiTypes.begin() + i);
				pDoc->m_pProjectCaseEdit->vecOtherLayerSimiMethods.erase(pDoc->m_pProjectCaseEdit->vecOtherLayerSimiMethods.begin() + i);
				break;
			}
		}
	}


	m_trProjectTree.DeleteItem(m_trProjectTree.GetSelectedItem());	


	pApp->m_iCurrEnvLayerIndex = -1;


	pDoc->UpdateAllViews(NULL);

}

void CLeftControlBar::OnRuleDelete()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());

	HTREEITEM selItem = m_trProjectTree.GetSelectedItem() ;

	if(m_iCurInstance!=-1)
	{
		pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getInstance(m_iCurInstance)->DeleteRule(m_iCurRule);
		m_pCurRule = NULL;
		m_trProjectTree.DeleteItem(m_trProjectTree.GetSelectedItem());
	}
	else if(m_iCurOccurrence!=-1)
	{
		HTREEITEM item = m_trProjectTree.GetParentItem(selItem) ;
		HTREEITEM itemChild = m_trProjectTree.GetChildItem(item);


		pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->DeleteRule(m_iCurRule);
		m_pCurRule = NULL;
		m_trProjectTree.SelectItem(itemChild);
		m_trProjectTree.DeleteItem(selItem);



	}
	else if(m_iCurExclusion!=-1)
	{
		HTREEITEM item = m_trProjectTree.GetParentItem(selItem) ;
		HTREEITEM itemChild = m_trProjectTree.GetChildItem(item);

		pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->DeleteRule(m_iCurRule);
		m_pCurRule = NULL;
		m_trProjectTree.SelectItem(itemChild);
		m_trProjectTree.DeleteItem(selItem);
	}
	pDoc->UpdateAllViews(NULL);
}

void CLeftControlBar::OnInstanceDelete()
{	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());

	if(m_iCurInstance!=-1)
	{
		pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->DeleteInstance(m_iCurInstance);
		m_trProjectTree.DeleteItem(m_trProjectTree.GetSelectedItem());
	}
}

void CLeftControlBar::OnOccurrenceDelete()
{	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());

	if(m_iCurOccurrence!=-1)
	{
		pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->DeleteOccurrence(m_iCurOccurrence);
		m_trProjectTree.DeleteItem(m_trProjectTree.GetSelectedItem());
	}
}

void CLeftControlBar::OnExclusionDelete()
{	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());

	if(m_iCurExclusion!=-1)
	{
		pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->DeleteExclusion(m_iCurExclusion);
		m_trProjectTree.DeleteItem(m_trProjectTree.GetSelectedItem());

	}

}

void CLeftControlBar::OnSoiltypeDelete()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());

	if(m_iCurSoil!=-1)
	{
		pDoc->m_pProjectEdit->m_pKnowledgeBase->DeleteSoilType(m_iCurSoil);
		m_trProjectTree.DeleteItem(m_trProjectTree.GetSelectedItem());
	}
}

void CLeftControlBar::OnAddruleRangerule()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	CRangeRule *pTempRule=new CRangeRule();	
	CRangeWizard dlg;
	dlg.m_pRule = pTempRule;
	if(dlg.DoModal()==IDOK)
	{
		pTempRule=dlg.m_pRule;
		pTempRule->setCurGraphMin(pTempRule->ComputeRuleGraphMin());
		pTempRule->setCurGraphMax(pTempRule->ComputeRuleGraphMax());
		pTempRule->setDispMin(pTempRule->getCurGraphMin());
		pTempRule->setDispMax(pTempRule->getCurGraphMax());	
		CString strNewName = pTempRule->getRuleName();
		pTempRule->setRuleType(0);
		bool IsDone = true;
		if(m_iCurInstance!=-1)
		{
			IsDone = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getInstance(m_iCurInstance)->AddRule((CRule*)pTempRule);
		}
		else if(m_iCurOccurrence!=-1)
		{
			IsDone = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->AddRule((CRule*)pTempRule);
		}
		else if(m_iCurExclusion!=-1)
		{
			IsDone = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->AddRule((CRule*)pTempRule);
		}
		if(IsDone==true)
		{

			HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();
			HTREEITEM hNewItem=m_trProjectTree.InsertItem(_T(strNewName),hCurrent);
			m_trProjectTree.SetItemState(hNewItem, INDEXTOSTATEIMAGEMASK(8), TVIS_STATEIMAGEMASK );
			m_trProjectTree.Select(hNewItem,TVGN_CARET);    
			m_bHasNewRule=true;
		}
	}
}

void CLeftControlBar::OnAddruleFreehandrule()
{	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	CFreehandRule *pTempRule=new CFreehandRule();
	CFreehandWizard dlg;
	dlg.m_pRule = pTempRule;
	if(dlg.DoModal()==IDOK)
	{
		pTempRule=dlg.m_pRule;
		pTempRule->setCurGraphMin(pTempRule->getCurGraphMin());
		pTempRule->setCurGraphMax(pTempRule->getCurGraphMax());
		pTempRule->setDispMin(pTempRule->getCurGraphMin());
		pTempRule->setDispMax(pTempRule->getCurGraphMax());	
		CString strNewName = pTempRule->getRuleName();
		pTempRule->setRuleType(1);
		bool IsDone = true;
		if(m_iCurInstance!=-1)
		{
			IsDone = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getInstance(m_iCurInstance)->AddRule((CRule*)pTempRule);

		}
		else if(m_iCurOccurrence!=-1)
		{
			IsDone = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->AddRule((CRule*)pTempRule);

		}
		else if(m_iCurExclusion!=-1)
		{
			IsDone = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->AddRule((CRule*)pTempRule);

		}

		if(IsDone==true)
		{

			HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();
			HTREEITEM hNewItem=m_trProjectTree.InsertItem(_T(strNewName),hCurrent);
			m_trProjectTree.SetItemState(hNewItem, INDEXTOSTATEIMAGEMASK(8), TVIS_STATEIMAGEMASK );
			m_trProjectTree.Select(hNewItem,TVGN_CARET);    
			m_bHasNewRule=true;
		}
	}
}

void CLeftControlBar::OnAddruleWordrule()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	CWordRule *pTempRule=new CWordRule();
	CWordWizard dlg;
	dlg.m_pRule = pTempRule;
	if(dlg.DoModal()==IDOK)
	{
		pTempRule=dlg.m_pRule;
		pTempRule->setCurGraphMin(pTempRule->ComputeRuleGraphMin());
		pTempRule->setCurGraphMax(pTempRule->ComputeRuleGraphMax());
		pTempRule->setDispMin(pTempRule->getCurGraphMin());
		pTempRule->setDispMax(pTempRule->getCurGraphMax());	
		CString strNewName = pTempRule->getRuleName();
		pTempRule->setRuleType(2);
		bool IsDone = true;
		if(m_iCurInstance!=-1)
		{
			IsDone = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getInstance(m_iCurInstance)->AddRule((CRule*)pTempRule);
		}
		else if(m_iCurOccurrence!=-1)
		{
			IsDone = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->AddRule((CRule*)pTempRule);
		}
		else if(m_iCurExclusion!=-1)
		{
			IsDone = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->AddRule((CRule*)pTempRule);
		}
		if(IsDone==true)
		{
			HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();
			HTREEITEM hNewItem=m_trProjectTree.InsertItem(_T(strNewName),hCurrent);
			m_trProjectTree.SetItemState(hNewItem, INDEXTOSTATEIMAGEMASK(8), TVIS_STATEIMAGEMASK );
			m_trProjectTree.Select(hNewItem,TVGN_CARET);    
			m_bHasNewRule=true;
		}
	}
}

void CLeftControlBar::OnAddrulePointrule()
{	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	CPointRule *pTempRule=new CPointRule();
	CPointWizard dlg;
	dlg.m_pRule = pTempRule;
	if(dlg.DoModal()==IDOK)
	{
		pTempRule=dlg.m_pRule;
		pTempRule->ComputeCentralValue(pDoc->m_pProjectEdit->m_pEnvDatabase);
		pTempRule->setCurGraphMin(pTempRule->ComputeRuleGraphMin());
		pTempRule->setCurGraphMax(pTempRule->ComputeRuleGraphMax());
		pTempRule->setDispMin(pTempRule->getCurGraphMin());
		pTempRule->setDispMax(pTempRule->getCurGraphMax());	
		CString strNewName = pTempRule->getRuleName();
		pTempRule->setRuleType(3);
		bool IsDone = true;
		if(m_iCurInstance!=-1)
		{
			IsDone = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getInstance(m_iCurInstance)->AddRule((CRule*)pTempRule);
		}
		else if(m_iCurOccurrence!=-1)
		{
			IsDone = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->AddRule((CRule*)pTempRule);
		}
		else if(m_iCurExclusion!=-1)
		{
			IsDone = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->AddRule((CRule*)pTempRule);
		}
		if(IsDone==true)
		{
			HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();
			HTREEITEM hNewItem=m_trProjectTree.InsertItem(_T(strNewName),hCurrent);
			m_trProjectTree.SetItemState(hNewItem, INDEXTOSTATEIMAGEMASK(8), TVIS_STATEIMAGEMASK );
			m_trProjectTree.Select(hNewItem,TVGN_CARET);    
			m_bHasNewRule=true;
		}
	}
}

void CLeftControlBar::OnAddruleEnumrule()
{	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	CEnumRule *pTempRule=new CEnumRule();
	CEnumWizard dlg;
	dlg.m_pRule = pTempRule;
	if(dlg.DoModal()==IDOK)
	{
		pTempRule=dlg.m_pRule;

		pTempRule->setCurGraphMin(pTempRule->getCurGraphMin());
		pTempRule->setCurGraphMax(pTempRule->getCurGraphMax());
		pTempRule->setDispMin(pTempRule->getCurGraphMin());
		pTempRule->setDispMax(pTempRule->getCurGraphMax());	
		CString strNewName = pTempRule->getRuleName();
		pTempRule->setRuleType(4);
		bool IsDone = true;
		if(m_iCurInstance!=-1)
		{
			IsDone = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getInstance(m_iCurInstance)->AddRule((CRule*)pTempRule);
		}
		else if(m_iCurOccurrence!=-1)
		{
			IsDone = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->AddRule((CRule*)pTempRule);
		}
		else if(m_iCurExclusion!=-1)
		{
			IsDone = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->AddRule((CRule*)pTempRule);
		}
		if(IsDone==true)
		{
			HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();
			HTREEITEM hNewItem=m_trProjectTree.InsertItem(_T(strNewName),hCurrent);
			m_trProjectTree.SetItemState(hNewItem, INDEXTOSTATEIMAGEMASK(8), TVIS_STATEIMAGEMASK );
			m_trProjectTree.Select(hNewItem,TVGN_CARET);    
			m_bHasNewRule=true;
		}
	}
}

void CLeftControlBar::OnInstanceRename()
{	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	if(m_iCurInstance!=-1)
	{
		CRenameDlg dlg;
		dlg.m_strNewItem = "instance:";
		if(dlg.DoModal()==IDOK)
		{
			CString strNewName = dlg.m_strNewName;
			pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getInstance(m_iCurInstance)->setInstanceName(strNewName);
			HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();
			m_trProjectTree.SetItemText(hCurrent,strNewName);
		}

	}
}

void CLeftControlBar::OnExclusionRename()
{	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();
	if(m_iCurExclusion!=-1)
	{
		CRenameDlg dlg;
		dlg.m_strNewItem = "exclusion:";
		if(dlg.DoModal()==IDOK)
		{
			CString strNewName = dlg.m_strNewName;
			pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->setExclusionName(strNewName);
			HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();
			m_trProjectTree.SetItemText(hCurrent,strNewName);
		}
	}
}

void CLeftControlBar::OnOccurrenceRename()
{	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	if(m_iCurOccurrence!=-1)
	{
		CRenameDlg dlg;
		dlg.m_strNewItem = "occurrence:";
		if(dlg.DoModal()==IDOK)
		{
			CString strNewName = dlg.m_strNewName;
			pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->setOccurrenceName(strNewName);
			HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();
			m_trProjectTree.SetItemText(hCurrent,strNewName);
		}
	}
}

void CLeftControlBar::OnSoiltypeRename()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	if(m_iCurSoil!=-1)
	{
		CRenameDlg dlg;
		dlg.m_strNewItem = "soil type:";
		if(dlg.DoModal()==IDOK)
		{
			CString strNewName = dlg.m_strNewName;

			pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->setSoilTypeName(strNewName);
			HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();

			m_trProjectTree.SetItemText(hCurrent,strNewName);

		}

	}
}

void CLeftControlBar::OnRuleRename()
{	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	if(m_iCurInstance!=-1)
	{
		CRenameDlg dlg;
		if(dlg.DoModal()==IDOK)
		{
			CString strNewName = dlg.m_strNewName;
			for(int i=0;i<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getInstance(m_iCurInstance)->getRuleNum();i++)
			{
				if(strNewName ==pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getInstance(m_iCurInstance)->getRule(i)->getRuleName())
				{
					AfxMessageBox("The name is the same with one of existing rules. Please use another name.");
					return;
				}
			}
			pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getInstance(m_iCurInstance)->getRule(m_iCurRule)->setRuleName(strNewName);
			HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();
			m_trProjectTree.SetItemText(hCurrent,strNewName);

			CKnowledgeView *pKnowledgeView = (CKnowledgeView *)(pFrame->m_splitterTabWnd.GetPane(0,0));	 
			pKnowledgeView->m_staticRuleFor.SetWindowText("Rule For: "+strNewName);



		}

	}
	else if(m_iCurOccurrence!=-1)
	{
		CRenameDlg dlg;
		if(dlg.DoModal()==IDOK)
		{
			CString strNewName = dlg.m_strNewName;
			for(int i=0;i<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->getRuleNum();i++)
			{
				if(strNewName ==pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->getRule(i)->getRuleName())
				{
					AfxMessageBox("The name is the same with one of existing rules. Please use another name.");
					return;
				}
			}
			pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->getRule(m_iCurRule)->setRuleName(strNewName);
			HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();
			m_trProjectTree.SetItemText(hCurrent,strNewName);

		}

	}
	else if(m_iCurExclusion!=-1)
	{
		CRenameDlg dlg;
		if(dlg.DoModal()==IDOK)
		{
			CString strNewName = dlg.m_strNewName;
			for(int i=0;i<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->getRuleNum();i++)
			{
				if(strNewName ==pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->getRule(i)->getRuleName())
				{
					AfxMessageBox("The name is the same with one of existing rules. Please use another name.");
					return;
				}
			}
			pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->getRule(m_iCurRule)->setRuleName(strNewName);
			HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();
			m_trProjectTree.SetItemText(hCurrent,strNewName);
		}
	}
}

void CLeftControlBar::OnKnowledgebaseAddsoiltype()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	CRenameDlg dlg;
	dlg.m_strNewItem = "soil type:";
	if(dlg.DoModal()==IDOK)
	{
		CString strNewName = dlg.m_strNewName;
		CSoilType *pSoilType = new CSoilType;
		pSoilType->setSoilTypeName(strNewName);
		if(true == pDoc->m_pProjectEdit->m_pKnowledgeBase->AddSoilType(pSoilType))
		{
			HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();
			HTREEITEM hNewItem=m_trProjectTree.InsertItem(_T(strNewName),hCurrent);
			m_trProjectTree.SetItemState(hNewItem, INDEXTOSTATEIMAGEMASK(5), TVIS_STATEIMAGEMASK );
			HTREEITEM hInstances= m_trProjectTree.InsertItem("Instances",hNewItem);
			m_trProjectTree.SetItemState(hInstances, INDEXTOSTATEIMAGEMASK(6), TVIS_STATEIMAGEMASK );
			HTREEITEM hOccurrences= m_trProjectTree.InsertItem("Occurrences",hNewItem);
			m_trProjectTree.SetItemState(hOccurrences, INDEXTOSTATEIMAGEMASK(6), TVIS_STATEIMAGEMASK );
			HTREEITEM hExclusions= m_trProjectTree.InsertItem("Exclusions",hNewItem);
			m_trProjectTree.SetItemState(hExclusions, INDEXTOSTATEIMAGEMASK(6), TVIS_STATEIMAGEMASK );
			m_trProjectTree.Select(hNewItem,TVGN_CARET); 

			m_trProjectTree.Expand (hNewItem, TVE_EXPAND);
		}

	}
}


void CLeftControlBar::OnKnowledgebaseCreatenew()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();



	int s =pFrame->MessageBox("Save edits to current knowledge base file?", "Save File",MB_ICONQUESTION|MB_YESNOCANCEL);//6: yes; 7: no; 2: cancel
	if(s == 6)  //yes
		OnKnowledgebaseSave();
	else if(s == 2) //cancel
		return;	 

	CKnowledgeNewDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString strKnowledge;
		strKnowledge = pDoc->m_pProjectEdit->getProjectDir() + "\\" + dlg.m_strDiffKnowledgeName + ".xml";

		if(pDoc->m_pProjectEdit->m_pKnowledgeBase !=NULL)
			delete pDoc->m_pProjectEdit->m_pKnowledgeBase;
		pDoc->m_pProjectEdit->m_pKnowledgeBase = new CKnowledgeBase();
		pDoc->SaveKnowledgeBase(strKnowledge, pDoc->m_pProjectEdit);
		pDoc->m_pProjectEdit->m_strKnowledgeBaseFile = dlg.m_strDiffKnowledgeName + ".xml";
		this->FillProjectTree(pDoc);


	}
}


void CLeftControlBar::OnKnowledgebaseLoad()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();



	//ask to save

	int s =pFrame->MessageBox("Save edits to current knowledge base file?", "Save File",MB_ICONQUESTION|MB_YESNOCANCEL);//6: yes; 7: no; 2: cancel
	if(s == 6)  //yes
		OnKnowledgebaseSave();
	else if(s == 2) //cancel
		return;	 





	static char BASED_CODE szFilter[] = "Knowledge Base File (*.xml)|*.xml|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.xml", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open Knowledge Base File";
	if(m_ldFile.DoModal() == IDOK)
	{   


		CString strKnowledge = m_ldFile.GetPathName();	

		pDoc->m_pProjectEdit->m_strKnowledgeBaseFile = extractFileName(strKnowledge);


		CString strNewKnowledgeFilePath= pDoc->m_strProjectDir + "\\" +  pDoc->m_pProjectEdit->m_strKnowledgeBaseFile;
		CopyFile(strKnowledge,strNewKnowledgeFilePath,false);  //copy knowledge file to current project directory


		pDoc->LoadKnowledgeBase(strNewKnowledgeFilePath, pDoc->m_pProjectEdit);

		this->FillProjectTree(pDoc);


	}


}

void CLeftControlBar::OnKnowledgebaseSave()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	pDoc->SaveKnowledgeBase(pDoc->m_pProjectEdit->m_strKnowledgeBaseFile, pDoc->m_pProjectEdit);

	this->FillProjectTree(pDoc);


}

void CLeftControlBar::OnKnowledgebaseSaveas()
{	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();


	int s =pFrame->MessageBox("Save edits to current knowledge base file?", "Save File",MB_ICONQUESTION|MB_YESNOCANCEL);//6: yes; 7: no; 2: cancel
	if(s == 6)  //yes
		OnKnowledgebaseSave();
	else if(s == 2) //cancel
		return;	 

	CKnowledgeSaveAsDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString strKnowledge;
		strKnowledge = pDoc->m_pProjectEdit->getProjectDir() + "\\" + dlg.m_strDiffKnowledgeName + ".xml";
		pDoc->SaveKnowledgeBase(strKnowledge, pDoc->m_pProjectEdit);
		pDoc->m_pProjectEdit->m_strKnowledgeBaseFile = dlg.m_strDiffKnowledgeName + ".xml";

		this->FillProjectTree(pDoc);

	}

}



void CLeftControlBar::OnKnowledgebaseCurrentFile()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
	MessageBox(pDoc->m_pProjectEdit->m_strKnowledgeBaseFile);

}


void CLeftControlBar::OnInstancesAddinstance()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	CRenameDlg dlg;
	dlg.m_strNewItem = "instance:";
	if(dlg.DoModal()==IDOK)
	{
		CString strNewName = dlg.m_strNewName;
		CInstance *pInstance = new CInstance;
		pInstance->setInstanceName(strNewName);
		if(true==pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->AddInstance(pInstance))
		{
			HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();
			HTREEITEM hNewItem=m_trProjectTree.InsertItem(_T(strNewName),hCurrent);
			m_trProjectTree.SetItemState(hNewItem, INDEXTOSTATEIMAGEMASK(7), TVIS_STATEIMAGEMASK );
			m_trProjectTree.Select(hNewItem,TVGN_CARET);  
		}
	}
}

void CLeftControlBar::OnExclusionsAddexclusion()
{	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	CRenameDlg dlg;
	dlg.m_strNewItem = "exclusion:";
	if(dlg.DoModal()==IDOK)
	{
		CString strNewName = dlg.m_strNewName;
		CExclusion *pExclusion = new CExclusion;
		pExclusion->setExclusionName(strNewName);
		if(true ==pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->AddExclusion(pExclusion))
		{
			HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();
			HTREEITEM hNewItem=m_trProjectTree.InsertItem(_T(strNewName),hCurrent);
			m_trProjectTree.SetItemState(hNewItem, INDEXTOSTATEIMAGEMASK(7), TVIS_STATEIMAGEMASK );
			m_trProjectTree.Select(hNewItem,TVGN_CARET);  
		}
	}
}

void CLeftControlBar::OnOccurrencesAddoccurrence()
{	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();
	CRenameDlg dlg;
	dlg.m_strNewItem = "occurrence:";
	if(dlg.DoModal()==IDOK)
	{
		CString strNewName = dlg.m_strNewName;
		COccurrence *pOccurrence = new COccurrence;
		pOccurrence->setOccurrenceName(strNewName);
		if(true ==pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->AddOccurrence(pOccurrence))
		{
			HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();
			HTREEITEM hNewItem=m_trProjectTree.InsertItem(_T(strNewName),hCurrent);
			m_trProjectTree.SetItemState(hNewItem, INDEXTOSTATEIMAGEMASK(7), TVIS_STATEIMAGEMASK );
			m_trProjectTree.Select(hNewItem,TVGN_CARET); 
		}
	}
}


void CLeftControlBar::OnRuleViewattachedlayer()
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();
	CString strCurrDbLayerName = m_pCurRule->getLayerName();
	for(int i=0;i<pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum();i++) 
	{
		if((pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(i)->getLayerName())==strCurrDbLayerName)
		{
			pApp->m_iCurrEnvLayerIndex=i;
			break;
		}
	}
	if(pDoc->getCurViewType()!=0)
	{
		pFrame->m_splitterTabWnd.SelectView(0);
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

void CLeftControlBar::OnRuleCopy()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	m_pClipBoardRule=NULL;
	CRule *pTempRule;
	if(m_iCurInstance!=-1)
	{
		pTempRule = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getInstance(m_iCurInstance)->getRule(m_iCurRule);
	}
	else if(m_iCurOccurrence!=-1)
	{
		pTempRule = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->getRule(m_iCurRule);
	}
	else if(m_iCurExclusion!=-1)
	{
		pTempRule = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->getRule(m_iCurRule);
	}
	if(pTempRule->getRuleType()==0)
	{
		CRangeRule *pNewRule = new CRangeRule(*((CRangeRule *)pTempRule));
		m_pClipBoardRule = pNewRule;
	}

	if(pTempRule->getRuleType()==1)
	{
		CFreehandRule *pNewRule = new CFreehandRule(*((CFreehandRule *)pTempRule));
		m_pClipBoardRule = pNewRule;
	}
	if(pTempRule->getRuleType()==2)
	{
		CWordRule *pNewRule = new CWordRule(*((CWordRule *)pTempRule));
		m_pClipBoardRule = pNewRule;
	}
	if(pTempRule->getRuleType()==3)
	{
		CPointRule *pNewRule = new CPointRule(*((CPointRule *)pTempRule));
		m_pClipBoardRule = pNewRule;
	}
	if(pTempRule->getRuleType()==4)
	{
		CEnumRule *pNewRule = new CEnumRule(*((CEnumRule *)pTempRule));
		m_pClipBoardRule = pNewRule;
	}

}

void CLeftControlBar::OnInstancePaste()
{	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	if(m_iCurInstance!=-1)
	{
		pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getInstance(m_iCurInstance)->AddRule(m_pClipBoardRule);

	}
	HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();
	HTREEITEM hNewItem=m_trProjectTree.InsertItem(_T(m_pClipBoardRule->getRuleName()),hCurrent);
	m_trProjectTree.SetItemState(hNewItem, INDEXTOSTATEIMAGEMASK(8), TVIS_STATEIMAGEMASK );
	m_trProjectTree.Select(hNewItem,TVGN_CARET);   
}

void CLeftControlBar::OnOccurrencePaste()
{	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();
	if(m_iCurOccurrence!=-1)
	{
		pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getOccurrence(m_iCurOccurrence)->AddRule(m_pClipBoardRule);
	}
	HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();
	HTREEITEM hNewItem=m_trProjectTree.InsertItem(_T(m_pClipBoardRule->getRuleName()),hCurrent);
	m_trProjectTree.SetItemState(hNewItem, INDEXTOSTATEIMAGEMASK(8), TVIS_STATEIMAGEMASK );
	m_trProjectTree.Select(hNewItem,TVGN_CARET);   
}

void CLeftControlBar::OnExclusionPaste()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();
	if(m_iCurExclusion!=-1)
	{
		pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(m_iCurSoil)->getExclusion(m_iCurExclusion)->AddRule(m_pClipBoardRule);
	}
	HTREEITEM hCurrent = m_trProjectTree.GetSelectedItem();
	HTREEITEM hNewItem=m_trProjectTree.InsertItem(_T(m_pClipBoardRule->getRuleName()),hCurrent);
	m_trProjectTree.SetItemState(hNewItem, INDEXTOSTATEIMAGEMASK(8), TVIS_STATEIMAGEMASK );
	m_trProjectTree.Select(hNewItem,TVGN_CARET);   
}



bool CLeftControlBar:: AddOneLayerCase(CString layerFileName, CString strCategory, HTREEITEM hCurItem)
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());

	CFileManager* fm = new CFileManager(pDoc, m_hWnd);
	CThreeDR* newLayer = fm->Readin(layerFileName);
	if(newLayer == NULL) { return false; }
	CString LayerName = newLayer->getDataName();  
	CString strLayerFileName = extractFileName(layerFileName);							
	strLayerFileName = dropFileSuffix(strLayerFileName);
	CString outputPath = pDoc->m_strProjectDir+"\\GISData\\" + strCategory + "\\"+strLayerFileName + ".3dr";
	CString layerFileFullName = outputPath;
	CString FileName = extractFileName(layerFileFullName);
	fm->Writeout3dr(newLayer, outputPath);

	double CellSize = newLayer->getCellSize();
	if(strCategory == "Climate")
	{
		pDoc->m_pProjectCaseEdit->vecClimateLayerNames.push_back(LayerName);
		pDoc->m_pProjectCaseEdit->vecClimateLayerFiles.push_back(FileName);
		pDoc->m_pProjectCaseEdit->vecClimateLayerResolutions.push_back(CellSize);
		pDoc->m_pProjectCaseEdit->vecClimateLayerSimiTypes.push_back("Single Value");
		pDoc->m_pProjectCaseEdit->vecClimateLayerSimiMethods.push_back("Gower Distance");
		pDoc->m_pProjectCaseEdit->g_vecstrClimateLayers.push_back(outputPath);
	}

	else if(strCategory == "Geology")
	{
		pDoc->m_pProjectCaseEdit->vecGeologyLayerNames.push_back(LayerName);
		pDoc->m_pProjectCaseEdit->vecGeologyLayerFiles.push_back(FileName);
		pDoc->m_pProjectCaseEdit->vecGeologyLayerResolutions.push_back(CellSize);
		pDoc->m_pProjectCaseEdit->vecGeologyLayerSimiTypes.push_back("Single Value");
		pDoc->m_pProjectCaseEdit->vecGeologyLayerSimiMethods.push_back("Boolean");
		pDoc->m_pProjectCaseEdit->g_vecstrGeologyLayers.push_back(outputPath);
	}

	else if(strCategory == "Terrain")
	{
		pDoc->m_pProjectCaseEdit->vecTerrainLayerNames.push_back(LayerName);
		pDoc->m_pProjectCaseEdit->vecTerrainLayerFiles.push_back(FileName);
		pDoc->m_pProjectCaseEdit->vecTerrainLayerResolutions.push_back(CellSize);
		pDoc->m_pProjectCaseEdit->vecTerrainLayerSimiTypes.push_back("Single Value");
		pDoc->m_pProjectCaseEdit->vecTerrainLayerSimiMethods.push_back("Gower Distance");
		pDoc->m_pProjectCaseEdit->g_vecstrTerrainLayers.push_back(outputPath);
	}

	else if(strCategory == "Vegetation")
	{
		pDoc->m_pProjectCaseEdit->vecVegetationLayerNames.push_back(LayerName);
		pDoc->m_pProjectCaseEdit->vecVegetationLayerFiles.push_back(FileName);
		pDoc->m_pProjectCaseEdit->vecVegetationLayerResolutions.push_back(CellSize);
		pDoc->m_pProjectCaseEdit->vecVegetationLayerSimiTypes.push_back("Single Value");
		pDoc->m_pProjectCaseEdit->vecVegetationLayerSimiMethods.push_back("Gower Distance");
		pDoc->m_pProjectCaseEdit->g_vecstrVegetationLayers.push_back(outputPath);
	}

	else 
	{
		pDoc->m_pProjectCaseEdit->vecOtherLayerNames.push_back(LayerName);
		pDoc->m_pProjectCaseEdit->vecOtherLayerFiles.push_back(FileName);
		pDoc->m_pProjectCaseEdit->vecOtherLayerResolutions.push_back(CellSize);
		pDoc->m_pProjectCaseEdit->vecOtherLayerSimiTypes.push_back("Single Value");
		pDoc->m_pProjectCaseEdit->vecOtherLayerSimiMethods.push_back("Gower Distance");
		pDoc->m_pProjectCaseEdit->g_vecstrOtherLayers.push_back(outputPath);
	}
	//pDoc->m_pProjectCaseEdit->m_projection = projection;

	HTREEITEM SubHti = m_trProjectTree.InsertItem(LayerName,1,1,hCurItem);
	m_trProjectTree.SetItemState(SubHti, INDEXTOSTATEIMAGEMASK(4), TVIS_STATEIMAGEMASK );
	m_trProjectTree.Expand (hCurItem, TVE_EXPAND);
	delete(newLayer);


	return true;
}

// addlayer (case)
void CLeftControlBar::OnDatabaseCaseAddlayer()
{	

	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());


	CString strCurCategory = m_trProjectTree.GetItemText(m_trProjectTree.GetSelectedItem());


	//CString layerFileFullName;

	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|GDAL-Supported Raster Formats (*.*)|*.*|shapefile (*.shp)|*.shp|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.*", OFN_ALLOWMULTISELECT |OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);


	CString msg;

	CString layerFileName;

	CString strTempFileName;

	m_ldFile.m_ofn.lpstrTitle="Add Layer(s)";
	CString   strFileNames;  
	m_ldFile.m_ofn.lpstrFile=strFileNames.GetBuffer(4000); 
	m_ldFile.m_ofn.nMaxFile= 4000; 



	if(strCurCategory == "Climate Layers")
	{
		if(m_ldFile.DoModal() == IDOK)
		{
			POSITION mPos= m_ldFile.GetStartPosition(); 
			while(mPos!=NULL) 
			{         

				layerFileName = m_ldFile.GetNextPathName(mPos);     
				if(!AddOneLayerCase(layerFileName, "Climate", m_hClimate))
					continue;

			}

		}

	}
	if(strCurCategory == "Parent Material Layers")
	{
		if(m_ldFile.DoModal() == IDOK)
		{
			POSITION mPos= m_ldFile.GetStartPosition(); 
			while(mPos!=NULL) 
			{         
				layerFileName = m_ldFile.GetNextPathName(mPos);     

				if(!AddOneLayerCase(layerFileName, "Geology", m_hGeology))
					continue;


			}

		}

	}
	if(strCurCategory == "Topographic Layers")
	{
		if(m_ldFile.DoModal() == IDOK)
		{
			POSITION mPos= m_ldFile.GetStartPosition(); 
			while(mPos!=NULL) 
			{         
				layerFileName = m_ldFile.GetNextPathName(mPos);         
				if(!AddOneLayerCase(layerFileName, "Terrain", m_hTerrain))
					continue;


			}

		}

	}
	if(strCurCategory == "Vegetation Layers")
	{
		if(m_ldFile.DoModal() == IDOK)
		{
			POSITION mPos= m_ldFile.GetStartPosition(); 
			while(mPos!=NULL) 
			{         
				layerFileName = m_ldFile.GetNextPathName(mPos); 
				if(!AddOneLayerCase(layerFileName, "Vegetation",m_hVegetation))
					continue;

			}

		}

	}
	if(strCurCategory == "Other Layers")
	{
		if(m_ldFile.DoModal() == IDOK)
		{
			POSITION mPos= m_ldFile.GetStartPosition(); 
			while(mPos!=NULL) 
			{         
				layerFileName = m_ldFile.GetNextPathName(mPos);    
				if(!AddOneLayerCase(layerFileName, "Other",m_hOther))
					continue;


			}

		}

	}
	pDoc->UpdateAllViews(NULL);
}

void CLeftControlBar::OnDatalayerSetvariabletype()
{
	CSetVarTypeDlg dialog;

	if(dialog.DoModal() == IDOK)
	{
		int varType = dialog.m_iVarType;
	}

}






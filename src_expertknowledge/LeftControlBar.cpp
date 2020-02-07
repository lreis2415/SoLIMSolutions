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
	ON_COMMAND(ID_INSTANCES_ADDINSTANCE, OnInstancesAddinstance)
	ON_COMMAND(ID_EXCLUSIONS_ADDEXCLUSION, OnExclusionsAddexclusion)
	ON_COMMAND(ID_OCCURRENCES_ADDOCCURRENCE, OnOccurrencesAddoccurrence)

	ON_WM_NCHITTEST()
	ON_COMMAND(ID_RULE_COPY, OnRuleCopy)
	ON_COMMAND(ID_INSTANCE_PASTE, OnInstancePaste)
	ON_COMMAND(ID_OCCURRENCE_PASTE, OnOccurrencePaste)
	ON_COMMAND(ID_EXCLUSION_PASTE, OnExclusionPaste)

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
	m_hKnowledge= m_trProjectTree.InsertItem (_T("Knowledge Base"), 0, 0);
	m_trProjectTree.SetItemState(m_hKnowledge, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK );
	
	m_trProjectTree.Expand (m_hKnowledge, TVE_EXPAND);
	m_ctServer.AddComponen(&m_trProjectTree);
	m_hPreviousItem = m_hKnowledge;
	return 0;
}

void CLeftControlBar::OnClose()
{
}

void CLeftControlBar::OnRClickProjectTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());

	if(pDoc->m_pProjectEdit==NULL)
		return;

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
	
		if(level==2)
		{
			CMenu   menu;
			menu.LoadMenu(IDR_MENU_KNOWLEDGEBASE); 
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
	
		else if(level==2)  //knowledge base
		{
			m_trProjectTree.Expand(m_hKnowledge,TVE_EXPAND);
		
		}
		
	
		else if(level==21)  //soil type
		{		
		
			CKnowledgeView *pKnowledgeView = (CKnowledgeView *)(pFrame->m_splitterTabWnd.GetPane(0,0));	 
			pKnowledgeView->FillKnowledgeView(m_pCurRule);

			CString  strSoilTypeName;
			strSoilTypeName = m_trProjectTree.GetItemText(hItem);

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
		
		else if(level==211||level==212||level==213)  //instances, occurrences, exclusions
		{		
		

			CString strItemName,strSoilTypeName;
			strItemName = m_trProjectTree.GetItemText(hItem);
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
		
			CKnowledgeView *pKnowledgeView = (CKnowledgeView *)(pFrame->m_splitterTabWnd.GetPane(0,0));	 
			pKnowledgeView->FillKnowledgeView(m_pCurRule);
			CString strItemName,strSoilTypeName;
			strItemName = m_trProjectTree.GetItemText(hItem);
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

	}
}

void CLeftControlBar::FillProjectTree(CSoLIMDoc *pDoc)
{
	if(pDoc->m_pProjectEdit!=NULL)
	{
		
		FillKnowledgeTree(pDoc);
		
	}
	m_trProjectTree.Expand (m_hKnowledge, TVE_EXPAND);

		m_trProjectTree.Expand (m_hKnowledge, TVE_EXPAND);
	
}


void CLeftControlBar::FillKnowledgeTree(CSoLIMDoc *pDoc)
{	

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


void CLeftControlBar::OnSize(UINT nType, int cx, int cy)
{
	CGuiControlBar::OnSize(nType, cx, cy);
}


//get the selected level of the tree
int CLeftControlBar::getSelectedTreeLevel(CTreeCtrl &m_theTree, HTREEITEM hItem)
{
	CString pstr=m_theTree.GetItemText(hItem); 
	if(hItem == NULL) return -1; 

	else if(hItem == m_hKnowledge) return 2;

		
	else if(m_theTree.GetParentItem(hItem) == m_hKnowledge) return 21;
	
	else if(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem))== m_hKnowledge && m_theTree.GetItemText(hItem)=="Instances") return 211;
	else if(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem))== m_hKnowledge && m_theTree.GetItemText(hItem)=="Occurrences") return 212;
	else if(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem))== m_hKnowledge && m_theTree.GetItemText(hItem)=="Exclusions") return 213;
	else if(m_theTree.GetParentItem(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem)))== m_hKnowledge&&m_theTree.GetItemText(m_theTree.GetParentItem(hItem)) == "Instances") return 2111;
	else if(m_theTree.GetParentItem(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem)))== m_hKnowledge&&m_theTree.GetItemText(m_theTree.GetParentItem(hItem)) == "Occurrences") return 2121;
	else if(m_theTree.GetParentItem(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem)))== m_hKnowledge&&m_theTree.GetItemText(m_theTree.GetParentItem(hItem)) == "Exclusions") return 2131;
	else if(m_theTree.GetParentItem(m_theTree.GetParentItem(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem))))== m_hKnowledge && m_theTree.GetItemText(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem)))=="Instances") return 21111;
	else if(m_theTree.GetParentItem(m_theTree.GetParentItem(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem))))== m_hKnowledge && m_theTree.GetItemText(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem)))=="Occurrences") return  21211;
	else if(m_theTree.GetParentItem(m_theTree.GetParentItem(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem))))== m_hKnowledge && m_theTree.GetItemText(m_theTree.GetParentItem(m_theTree.GetParentItem(hItem)))=="Exclusions") return 21311;
	else return -1; 
	return -1;
}

void CLeftControlBar::ExpandNode(int iNodeType)
{
	 
	if(iNodeType == 1)
		m_trProjectTree.Expand(m_hKnowledge,TVE_EXPAND);     

}

void CLeftControlBar::CleanMemory()
{
	m_hPreviousItem = m_hKnowledge;
	m_pCurRule = NULL;



	m_trProjectTree.DeleteItem(m_hKnowledge);

	m_hKnowledge= m_trProjectTree.InsertItem (_T("Knowledge Base"), 0, 0);
	m_trProjectTree.SetItemState(m_hKnowledge, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK );


	//m_hKnowledge = m_trProjectTree.InsertItem (_T("Knowledge Base"), 1, 1, m_hRootRule);
	//m_trProjectTree.SetItemState(m_hKnowledge, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK );



	//m_hKnowledge = m_trProjectTree.InsertItem (_T("Knowledge Base"), 1, 1, m_hRoot);
	//m_trProjectTree.SetItemState(m_hKnowledge, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK );


	//HTREEITEM hRunInference= m_trProjectTree.InsertItem("Inference",m_hInference);
	//m_trProjectTree.SetItemState(hRunInference, INDEXTOSTATEIMAGEMASK(3), TVIS_STATEIMAGEMASK );
	
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



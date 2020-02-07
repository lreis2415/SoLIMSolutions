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
// RangePage.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "WordPage.h"
#include ".\wordpage.h"
#include "MainFrm.h"
#include "SoLIMDoc.h"
#include "WordRule.h"
#include "WordCurve.h"
#include "FreehandRule.h"
#include "utility.h"

#include "MainFrm.h"
#include "SoLIMDoc.h"

#include   <msxml2.h>      
#import   <msxml4.dll>

// CWordPage dialog
IMPLEMENT_DYNAMIC(CWordPage, CDialog)

CWordPage::CWordPage(CWnd* pParent /*=NULL*/)
: CDialog(CWordPage::IDD, pParent)
{
}

CWordPage::~CWordPage()
{
}

void CWordPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK_USE, m_chkUse);
	DDX_Control(pDX, IDC_STATIC_LAYER, m_staticLayer);
	DDX_Control(pDX, IDC_STATIC_MIN, m_staticRangeMin);
	DDX_Control(pDX, IDC_STATIC_MAX, m_staticRangeMax);
	//DDX_Control(pDX, IDC_BUTTON_BROWSE_LAYER_FILE2, m_buttonChooseLayer);
	//DDX_Control(pDX, IDC_EDIT_LAYER, m_editLayerName);
	DDX_Control(pDX, IDC_BUTTON_MIN_LEFT2, m_buttonMinLeft);
	DDX_Control(pDX, IDC_BUTTON_MIN_RIGHT,m_buttonMinRight);
	DDX_Control(pDX, IDC_BUTTON_MAX_LEFT, m_buttonMaxLeft);
	DDX_Control(pDX, IDC_BUTTON_MAX_RIGHT,m_buttonMaxRight);
	DDX_Control(pDX, IDC_STATIC_RULE_TYPE, m_staticRuleType);

	DDX_Control(pDX,IDC_BUTTON_CHANGETOCURVERULE,m_btnChangeToFreehandRule);
	DDX_Control(pDX,IDC_EDIT_LIBFILENAME,m_editLibFileName);
	DDX_Control(pDX,IDC_COMBO_CURVENAME,m_comboCurveName);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_LIBFILE, m_buttonChooseLibFile);
	DDX_Control(pDX, IDC_STATIC_LIBNAME, m_staticLibFile);
	DDX_Control(pDX, IDC_COMBO_TERRAIN_ATTRI, m_comboEnvAttri);

	DDX_Control(pDX, IDC_COMBO_LAYER_SEL, m_comboAttachLayer);
	DDX_Control(pDX,IDC_BUTTON_APPLY_THE_RULE, m_btnApplyTheRule);

}


BEGIN_MESSAGE_MAP(CWordPage, CDialog)
	ON_REGISTERED_MESSAGE(RANGE_CHANGED, OnRangeChange)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_CURVENAME,OnCbnSelchangeComboCurveName)
	ON_BN_CLICKED(IDC_CHECK_USE, OnBnClickedCheckUse)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_LIBFILE, OnBnClickedButtonBrowseLibfile)
	ON_BN_CLICKED(IDC_BUTTON_CHANGETOCURVERULE, OnBnClickedButtonChangetocurverule)
	//ON_BN_CLICKED(IDC_BUTTON_BROWSE_LAYER_FILE2, OnBnClickedButtonBrowseLayerFile)	
	ON_BN_CLICKED(IDC_BUTTON_MIN_LEFT, OnBnClickedButtonMinLeft)
	ON_BN_CLICKED(IDC_BUTTON_MIN_RIGHT, OnBnClickedButtonMinRight)
	ON_BN_CLICKED(IDC_BUTTON_MAX_LEFT, OnBnClickedButtonMaxLeft)
	ON_BN_CLICKED(IDC_BUTTON_MAX_RIGHT, OnBnClickedButtonMaxRight)
	ON_CBN_SELCHANGE(IDC_COMBO_TERRAIN_ATTRI, OnCbnSelchangeComboEnvAttri)
	ON_CBN_SELCHANGE(IDC_COMBO_LAYER_SELECT2, OnCbnSelchangeComboLayerSelect2)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_THE_RULE, OnBnClickedButtonApplyRule)
END_MESSAGE_MAP()


// CWordPage message handlers

BOOL CWordPage::OnInitDialog()
{
	CDialog::OnInitDialog();	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	CRect rect;
	GetClientRect(&rect);
	int cx = rect.right;
	int cy = rect.bottom;		
	CRect graphRect(0.5*cx/10,1.5*cy/10,7*cx/10,6.5*cy/10);
	m_pGraphPanel.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,graphRect,this,1000);
	CRect rcSlider (5*cx/10,7*cy/10,9*cx/10,7*cy/10+20);
	m_sliderRange.Create(WS_CHILD |WS_VISIBLE|PBS_SMOOTH| WS_TABSTOP, rcSlider, this, IDC_SLIDER);
	if(m_sliderRange.GetSafeHwnd())
	{	
		if(pFrame->m_leftBar.m_pCurRule!=NULL)
		{    
			m_sliderRange.SetMinMax(pFrame->m_leftBar.m_pCurRule->getCurGraphMin(),pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
			m_sliderRange.SetRange(pFrame->m_leftBar.m_pCurRule->getDispMin(),pFrame->m_leftBar.m_pCurRule->getDispMax());
		}
	}


	int iItemCount = m_comboAttachLayer.GetCount();
	for(int i =0; i<iItemCount;i++)
	{
		m_comboAttachLayer.DeleteString(0);
	}
   // int iCurLayer = -1;
	for(i = 0; i<pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum(); i++)
	{
		CString strLayerName;
		strLayerName = pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(i)->getLayerName();

	//	if(strLayerName == 	pFrame->m_leftBar.m_pCurRule->getLayerName())
	//		iCurLayer = i;
		m_comboAttachLayer.AddString(strLayerName);
	} 
	//if(iCurLayer == -1)
	   m_comboAttachLayer.AddString("------");

	m_comboAttachLayer.AddString("Choose One Layer...");

	return TRUE;  
}

void CWordPage::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);	

	if(m_staticRuleType.GetSafeHwnd())
	{
		CRect rect(0.5*cx/10,0.2*cy/10,7*cx/10,0.2*cy/10+20);
		m_staticRuleType.MoveWindow(&rect);
	}
	if(m_chkUse.GetSafeHwnd())
	{
		CRect rect(0.5*cx/10,0.8*cy/10,5*cx/10,0.8*cy/10+20);
		m_chkUse.MoveWindow(&rect);
	}
	if(m_pGraphPanel.GetSafeHwnd())
	{ 
		CRect graphRect(0.5*cx/10,1.5*cy/10,7*cx/10,6.5*cy/10);
		m_pGraphPanel.MoveWindow(&graphRect);
	}
	if(m_sliderRange.GetSafeHwnd())
	{
		CRect rect(cx/10,7*cy/10,6.5*cx/10,7*cy/10+20);
		m_sliderRange.MoveWindow(&rect);
	}
	if(m_buttonMinLeft.GetSafeHwnd())
	{
		CRect rect(0.5*cx/10,7*cy/10,0.75*cx/10,7*cy/10+20);
		m_buttonMinLeft.MoveWindow(&rect);
	}
	if(m_buttonMinRight.GetSafeHwnd())
	{
		CRect rect(0.75*cx/10,7*cy/10,cx/10,7*cy/10+20);
		m_buttonMinRight.MoveWindow(&rect);
	}
	if(m_buttonMaxLeft.GetSafeHwnd())
	{
		CRect rect(6.5*cx/10,7*cy/10,6.75*cx/10,7*cy/10+20);
		m_buttonMaxLeft.MoveWindow(&rect);
	}

	if(m_buttonMaxRight.GetSafeHwnd())
	{
		CRect rect(6.75*cx/10,7*cy/10,7*cx/10,7*cy/10+20);
		m_buttonMaxRight.MoveWindow(&rect);
	}
	if(m_staticRangeMin.GetSafeHwnd())
	{
		CRect rect(0.2*cx/10,7*cy/10+30,cx/10+50,7*cy/10+50);
		m_staticRangeMin.MoveWindow(&rect);
	}
	if(m_staticRangeMax.GetSafeHwnd())
	{
		CRect rect(6.5*cx/10,7*cy/10+30,7.3*cx/10+50,7*cy/10+50);
		m_staticRangeMax.MoveWindow(&rect);
	}
	if(m_staticLayer.GetSafeHwnd())
	{
		CRect rect(cx/10,9*cy/10,3*cx/10,9*cy/10+40);
		m_staticLayer.MoveWindow(&rect);
	}
	/*if(m_editLayerName.GetSafeHwnd())
	{
		CRect rect(3*cx/10,9*cy/10,6*cx/10,9*cy/10+20);
		m_editLayerName.MoveWindow(&rect);
	}
	if(m_buttonChooseLayer.GetSafeHwnd())
	{
		CRect rect(6.5*cx/10,9*cy/10,7.5*cx/10,9*cy/10+20);
		m_buttonChooseLayer.MoveWindow(&rect);
	}*/

	if(m_comboAttachLayer.GetSafeHwnd())
	{
		CRect rect(3*cx/10,9*cy/10,6*cx/10,9*cy/10+20);
		m_comboAttachLayer.MoveWindow(&rect);
	}

		if(m_btnApplyTheRule.GetSafeHwnd())
	{
		CRect rect(6.8*cx/10,9*cy/10,9.5*cx/10,9*cy/10+20);
		m_btnApplyTheRule.MoveWindow(&rect);
	}



	if(m_staticLibFile.GetSafeHwnd())
	{
		CRect rect(cx*7.2/10,cy*1.5/10,cx*9/10,cy*1.5/10+20);
		m_staticLibFile.MoveWindow(&rect);
	}
	if(m_editLibFileName.GetSafeHwnd())
	{
		CRect rect(cx*7.2/10,cy*2.5/10,cx*8.8/10,cy*2.5/10+20);
		m_editLibFileName.MoveWindow(&rect);
	}	
	if(m_buttonChooseLibFile.GetSafeHwnd())
	{
		CRect rect(cx*9.0/10,cy*2.5/10,cx*9.5/10,cy*2.5/10+20);
		m_buttonChooseLibFile.MoveWindow(&rect);
	}	
	if(m_comboEnvAttri.GetSafeHwnd())
	{
		CRect rect(cx*7.2/10,cy*3.5/10,cx*9.5/10,cy*3.5/10+20);
		m_comboEnvAttri.MoveWindow(&rect);
	}	
	if(m_comboCurveName.GetSafeHwnd())
	{
		CRect rect(cx*7.2/10,cy*4.5/10,cx*9.5/10,cy*4.5/10+20);
		m_comboCurveName.MoveWindow(&rect);
	}	
	if(m_btnChangeToFreehandRule.GetSafeHwnd())
	{
		CRect rect(cx*7.2/10,cy*6.5/10-20,9.5*cx/10,cy*6.5/10);
		m_btnChangeToFreehandRule.MoveWindow(&rect);
	}
	
	if(m_staticRuleType.GetSafeHwnd())
	{
		FillPage();
	}
}

void CWordPage::EnablePage(bool bIsEnable)
{
	if(m_staticLayer.GetSafeHwnd())
	{
		m_staticLayer.EnableWindow(bIsEnable); 
	}
	if(m_staticRangeMin.GetSafeHwnd())
	{
		m_staticRangeMin.EnableWindow(bIsEnable);
	}
	if(m_staticRangeMax.GetSafeHwnd())
	{
		m_staticRangeMax.EnableWindow(bIsEnable);
	}
	if(m_pGraphPanel.GetSafeHwnd())
	{
		m_pGraphPanel.EnableWindow(bIsEnable);
		m_pGraphPanel.SetEnable(bIsEnable);
	}
	if(m_sliderRange.GetSafeHwnd())
	{
		m_sliderRange.EnableWindow(bIsEnable);
	}
	if(m_buttonMinLeft.GetSafeHwnd())
	{
		m_buttonMinLeft.EnableWindow(bIsEnable);
	}
	if(m_buttonMinRight.GetSafeHwnd())
	{
		m_buttonMinRight.EnableWindow(bIsEnable);
	}
	if(m_buttonMaxLeft.GetSafeHwnd())
	{
		m_buttonMaxLeft.EnableWindow(bIsEnable);
	}
	if(m_buttonMaxRight.GetSafeHwnd())
	{
		m_buttonMaxRight.EnableWindow(bIsEnable);
	}	
	/*if(m_editLayerName.GetSafeHwnd())
	{
		m_editLayerName.EnableWindow(bIsEnable);
	}
	if(m_buttonChooseLayer.GetSafeHwnd())
	{
		m_buttonChooseLayer.EnableWindow(bIsEnable);
	}*/

		if(m_comboAttachLayer.GetSafeHwnd())
	{		
		m_comboAttachLayer.EnableWindow(bIsEnable);
	}

	if(m_btnChangeToFreehandRule.GetSafeHwnd())
	{
		m_btnChangeToFreehandRule.EnableWindow(bIsEnable);
	}

	if(m_staticLibFile.GetSafeHwnd())
	{	
		m_staticLibFile.EnableWindow(bIsEnable);
	}

	if(m_editLibFileName.GetSafeHwnd())
	{
		m_editLibFileName.EnableWindow(bIsEnable);
	}	

	if(m_buttonChooseLibFile.GetSafeHwnd())
	{
		m_buttonChooseLibFile.EnableWindow(bIsEnable);
	}	

	if(m_comboCurveName.GetSafeHwnd())
	{
		m_comboCurveName.EnableWindow(bIsEnable);
	}	
	if(m_comboCurveName.GetSafeHwnd())
	{
		m_comboEnvAttri.EnableWindow(bIsEnable);
	}	

	m_pGraphPanel.Invalidate();
}

void CWordPage::FillPage()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();
    CString str;
	if(pFrame->m_leftBar.m_pCurRule!=NULL)
	{
		m_staticRuleType.SetWindowText("Rule Type: Word");
		m_sliderRange.SetMinMax(pFrame->m_leftBar.m_pCurRule->getCurGraphMin(),pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
		str.Format("%f",pFrame->m_leftBar.m_pCurRule->getCurGraphMin());
		str=str.Left(9);
		m_staticRangeMin.SetWindowText(str);
		str.Format("%f", pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
		str=str.Left(9);
		m_staticRangeMax.SetWindowText(str);	
		m_sliderRange.SetRange(pFrame->m_leftBar.m_pCurRule->getDispMin(),pFrame->m_leftBar.m_pCurRule->getDispMax());
		CString strFileName = extractFileName(((CWordRule*)(pFrame->m_leftBar.m_pCurRule))->getLibFileName());
		m_editLibFileName.SetWindowText(strFileName);
		int iEnvAttriCount = m_comboEnvAttri.GetCount();
		for(int i =0; i<iEnvAttriCount;i++)
		{
			m_comboEnvAttri.DeleteString(0);
		}
		int iCurEnvAttri = -1;
		CString strCurEnvAttri;
		if(((CWordRule*)(pFrame->m_leftBar.m_pCurRule))->getWordLib()!=NULL)
		{
			for(int i=0;i<((CWordRule*)(pFrame->m_leftBar.m_pCurRule))->getWordLib()->getEnvAttriNum();i++)
			{	
				CString strEnvAttri = ((CWordRule*)(pFrame->m_leftBar.m_pCurRule))->getWordLib()->getEnvAttri(i);
				m_comboEnvAttri.AddString(strEnvAttri);
				if(((CWordRule*)(pFrame->m_leftBar.m_pCurRule))->getEnvAttri()==strEnvAttri)
				{
					iCurEnvAttri = i;
					strCurEnvAttri = strEnvAttri;
				}
			}
			if(iCurEnvAttri !=-1)
				m_comboEnvAttri.SetCurSel(iCurEnvAttri);	
		}


		int iItemCount = m_comboCurveName.GetCount();
		for(int i =0; i<iItemCount;i++)
		{
			m_comboCurveName.DeleteString(0);
		}
		int iPhraseNum = 0;
		int iCurPhrase = -1;
		if(((CWordRule*)(pFrame->m_leftBar.m_pCurRule))->getWordLib()!=NULL)
		{
			for(int i=0;i<((CWordRule*)(pFrame->m_leftBar.m_pCurRule))->getWordLib()->getWordCurveNum();i++)
			{	
				CString strPhrase = ((CWordRule*)(pFrame->m_leftBar.m_pCurRule))->getWordLib()->getWordCurve(i)->getPhrase();
				if(((CWordRule*)(pFrame->m_leftBar.m_pCurRule))->getWordLib()->getWordCurve(i)->getEnvAttri() == strCurEnvAttri)
				{
					m_comboCurveName.AddString(strPhrase);
					iPhraseNum++;
					if(((CWordRule*)(pFrame->m_leftBar.m_pCurRule))->getPhrase()==strPhrase&&((CWordRule*)(pFrame->m_leftBar.m_pCurRule))->getEnvAttri()==strCurEnvAttri)
						iCurPhrase = iPhraseNum-1;
				}

			}
			if(iCurPhrase !=-1)
				m_comboCurveName.SetCurSel(iCurPhrase);	
		}
		m_chkUse.SetCheck(pFrame->m_leftBar.m_pCurRule->getIsUsed());
		//set layer linked
		if(pFrame->m_leftBar.m_pCurRule->getLayerName()!="")
		{
			m_staticLayer.SetWindowText("Data layer linked: ");
			//m_editLayerName.SetWindowText(pFrame->m_leftBar.m_pCurRule->getLayerName());
			//m_buttonChooseLayer.ShowWindow(SW_HIDE);

			for(int i=0; i<m_comboAttachLayer.GetCount(); i++)
			{
				CString str;
				m_comboAttachLayer.GetLBText(i, str);

			    if(str == pFrame->m_leftBar.m_pCurRule->getLayerName())
				{		
			         m_comboAttachLayer.SetCurSel(i);
			          break;
				}
			}
		}
		else 
		{
			str = "No data layer linked.\nPlease set data layer: ";
			m_staticLayer.SetWindowText(str);
			//m_editLayerName.SetWindowText("");
			//m_buttonChooseLayer.ShowWindow(SW_SHOW);

			for(int i=0; i<m_comboAttachLayer.GetCount(); i++)
			{

						CString str;
				m_comboAttachLayer.GetLBText(i, str);

			    if(str == "------")
				{
		
			         m_comboAttachLayer.SetCurSel(i);
			          break;
				}
			}
		}


		EnablePage(pFrame->m_leftBar.m_pCurRule->getIsUsed());
		Invalidate();
	}
}

LRESULT CWordPage::OnRangeChange(WPARAM /* wParam */, LPARAM /* lParam */)
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	UpdateData();
	double left, right;
	m_sliderRange.GetRange(left, right);  

	if(pFrame->m_leftBar.m_pCurRule!=NULL)
	{  
		pFrame->m_leftBar.m_pCurRule->setDispMin(left);
		pFrame->m_leftBar.m_pCurRule->setDispMax(right);	
	}
	m_pGraphPanel.Invalidate();
	return static_cast<LRESULT>(0);
}

void CWordPage::OnBnClickedCheckUse()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	pFrame->m_leftBar.m_pCurRule->setIsUsed(!pFrame->m_leftBar.m_pCurRule->getIsUsed());
	HTREEITEM SelItem = pFrame->m_leftBar.m_trProjectTree.GetSelectedItem();
	if(pFrame->m_leftBar.m_pCurRule->getIsUsed()==true)   	
	{
		pFrame->m_leftBar.m_trProjectTree.SetItemState( SelItem, INDEXTOSTATEIMAGEMASK(8), TVIS_STATEIMAGEMASK );
		EnablePage(true);
	}
	else                   
	{
		pFrame->m_leftBar.m_trProjectTree.SetItemState( SelItem, INDEXTOSTATEIMAGEMASK(9), TVIS_STATEIMAGEMASK );
		EnablePage(false);
	}
}


BOOL CWordPage::PreTranslateMessage( MSG *pMsg )
{
	if (pMsg->message == WM_KEYDOWN)   
	{   
		if   (pMsg->wParam == VK_ESCAPE)   
			return   TRUE;   
		if   (pMsg->wParam == VK_RETURN)   
		{
			return   TRUE;   
		}
	}
	return   CDialog::PreTranslateMessage(pMsg);   
}

/*
void CWordPage::OnBnClickedButtonBrowseLayerFile()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	UpdateData(true);
	static char BASED_CODE szFilter[] = "Layer Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Choose Layer File";

	if(m_ldFile.DoModal() == IDOK)
	{   
		CString strLayerName = "untitled";
		CString strLayerFilePath =  m_ldFile.GetPathName(); 
		CEnvLayer *pLayer = new CEnvLayer(pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum(),strLayerName, strLayerFilePath);
		if(pLayer->getLayerID() != -1)
		{ 
			strLayerName.Format("%s",pLayer->m_pTheData->getDataName());
			pLayer->setLayerName(strLayerName);
			for(int i=0;i<pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum();i++) 
			{
				if((pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(i)->getLayerName())==strLayerName)
				{
					if(AfxMessageBox("The name of the layer is the same with one layer the database already has. Do you want to replace the existing layer with the new one?",MB_YESNO)==IDYES)
					{
						CString strNewLayerFilePath;
						CString strLayerFileName = extractFileName(strLayerFilePath);
						strNewLayerFilePath= pDoc->m_strProjectDir+"\\GISData\\"+strLayerFileName;
						CopyFile(strLayerFilePath,strNewLayerFilePath,false);  //copy env layer to current project directory
						pLayer->setFileName(strNewLayerFilePath);
						//delete the duplicate layer first
						pDoc->m_pProjectEdit->m_pEnvDatabase->DeleteLayer(i);
						pDoc->m_pProjectEdit->m_pEnvDatabase->AddLayer(pLayer);					
						m_buttonChooseLayer.ShowWindow(SW_HIDE);
						pFrame->m_leftBar.m_pCurRule->setLayerName(strLayerName);
						pFrame->m_leftBar.FillProjectTree(pDoc);
						FillPage();
						pDoc->UpdateAllViews(NULL);
						return;
					}
					else
					{
						delete pLayer;
						pLayer = NULL;
						return;
					}
					break;
			 }
			}

			if(pDoc->m_pProjectEdit->m_pEnvDatabase->AddLayer(pLayer))   //if add layer successfully
			{
				CString strNewLayerFilePath;
				CString strLayerFileName = extractFileName(strLayerFilePath);
				strNewLayerFilePath= pDoc->m_strProjectDir+"\\GISData\\"+strLayerFileName;
				CopyFile(strLayerFilePath,strNewLayerFilePath,false);  //copy env layer to current project directory
				pLayer->setFileName(strNewLayerFilePath);
				m_buttonChooseLayer.ShowWindow(SW_HIDE);
				pFrame->m_leftBar.m_pCurRule->setLayerName(strLayerName);
				pFrame->m_leftBar.FillProjectTree(pDoc);
				FillPage();
				pDoc->UpdateAllViews(NULL);
				return;
			}
		}
		else 
		{
			delete pLayer;
			pLayer = NULL;
			CString msg;
			msg.Format("Error in reading %s. The layer will not be added",strLayerFilePath);
			AfxMessageBox(msg);
			return;

		}	

	}

}*/


void CWordPage::OnBnClickedButtonMinLeft()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();
	double dGraphMin=pFrame->m_leftBar.m_pCurRule->ComputeRuleGraphMin();
	double dGraphMax=pFrame->m_leftBar.m_pCurRule->ComputeRuleGraphMax();
	double range= dGraphMax-dGraphMin;
	pFrame->m_leftBar.m_pCurRule->setCurGraphMin(pFrame->m_leftBar.m_pCurRule->getCurGraphMin()-range/50);

	m_sliderRange.SetMinMax(pFrame->m_leftBar.m_pCurRule->getCurGraphMin(),pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
	m_sliderRange.SetRange(pFrame->m_leftBar.m_pCurRule->getDispMin(),pFrame->m_leftBar.m_pCurRule->getDispMax());
	m_pGraphPanel.Invalidate();
	CString str;
	str.Format("%f",pFrame->m_leftBar.m_pCurRule->getCurGraphMin());
	str=str.Left(9);
	m_staticRangeMin.SetWindowText(str);
	str.Format("%f", pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
	str=str.Left(9);
	m_staticRangeMax.SetWindowText(str);


}

void CWordPage::OnBnClickedButtonMinRight()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();
	double dGraphMin=pFrame->m_leftBar.m_pCurRule->ComputeRuleGraphMin();
	double dGraphMax=pFrame->m_leftBar.m_pCurRule->ComputeRuleGraphMax();
	double range= dGraphMax-dGraphMin;
	if(pFrame->m_leftBar.m_pCurRule->getCurGraphMin()+range/50<pFrame->m_leftBar.m_pCurRule->getCurGraphMax())
		pFrame->m_leftBar.m_pCurRule->setCurGraphMin(pFrame->m_leftBar.m_pCurRule->getCurGraphMin()+range/50);
	else
		return;

	if(pFrame->m_leftBar.m_pCurRule->getCurGraphMin()>pFrame->m_leftBar.m_pCurRule->getDispMin())
		pFrame->m_leftBar.m_pCurRule->setDispMin(pFrame->m_leftBar.m_pCurRule->getCurGraphMin());
	if(pFrame->m_leftBar.m_pCurRule->getCurGraphMin()>pFrame->m_leftBar.m_pCurRule->getDispMax())
		pFrame->m_leftBar.m_pCurRule->setDispMax(pFrame->m_leftBar.m_pCurRule->getCurGraphMin());



	m_sliderRange.SetMinMax(pFrame->m_leftBar.m_pCurRule->getCurGraphMin(),pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
	m_sliderRange.SetRange(pFrame->m_leftBar.m_pCurRule->getDispMin(),pFrame->m_leftBar.m_pCurRule->getDispMax());
	m_pGraphPanel.Invalidate();
	CString str;
	str.Format("%f",pFrame->m_leftBar.m_pCurRule->getCurGraphMin());
	str=str.Left(9);
	m_staticRangeMin.SetWindowText(str);
	str.Format("%f", pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
	str=str.Left(9);
	m_staticRangeMax.SetWindowText(str);
}

void CWordPage::OnBnClickedButtonMaxLeft()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();
	double dGraphMin=pFrame->m_leftBar.m_pCurRule->ComputeRuleGraphMin();
	double dGraphMax=pFrame->m_leftBar.m_pCurRule->ComputeRuleGraphMax();
	double range= dGraphMax-dGraphMin;
	if(pFrame->m_leftBar.m_pCurRule->getCurGraphMax()-range/50>pFrame->m_leftBar.m_pCurRule->getCurGraphMin())
		pFrame->m_leftBar.m_pCurRule->setCurGraphMax(pFrame->m_leftBar.m_pCurRule->getCurGraphMax()-range/50);
	else
		return;

	if(pFrame->m_leftBar.m_pCurRule->getCurGraphMax()<pFrame->m_leftBar.m_pCurRule->getDispMax())
		pFrame->m_leftBar.m_pCurRule->setDispMax(pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
	if(pFrame->m_leftBar.m_pCurRule->getCurGraphMax()<pFrame->m_leftBar.m_pCurRule->getDispMin())
		pFrame->m_leftBar.m_pCurRule->setDispMin(pFrame->m_leftBar.m_pCurRule->getCurGraphMax());


	m_sliderRange.SetMinMax(pFrame->m_leftBar.m_pCurRule->getCurGraphMin(),pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
	m_sliderRange.SetRange(pFrame->m_leftBar.m_pCurRule->getDispMin(),pFrame->m_leftBar.m_pCurRule->getDispMax());
	m_pGraphPanel.Invalidate();
	CString str;
	str.Format("%f",pFrame->m_leftBar.m_pCurRule->getCurGraphMin());
	str=str.Left(9);
	m_staticRangeMin.SetWindowText(str);
	str.Format("%f", pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
	str=str.Left(9);
	m_staticRangeMax.SetWindowText(str);
}

void CWordPage::OnBnClickedButtonMaxRight()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();
	double dGraphMin=pFrame->m_leftBar.m_pCurRule->ComputeRuleGraphMin();
	double dGraphMax=pFrame->m_leftBar.m_pCurRule->ComputeRuleGraphMax();
	double range= dGraphMax-dGraphMin;
	pFrame->m_leftBar.m_pCurRule->setCurGraphMax(pFrame->m_leftBar.m_pCurRule->getCurGraphMax()+range/50);
	m_sliderRange.SetMinMax(pFrame->m_leftBar.m_pCurRule->getCurGraphMin(),pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
	m_sliderRange.SetRange(pFrame->m_leftBar.m_pCurRule->getDispMin(),pFrame->m_leftBar.m_pCurRule->getDispMax());
	m_pGraphPanel.Invalidate();
	CString str;
	str.Format("%f",pFrame->m_leftBar.m_pCurRule->getCurGraphMin());
	str=str.Left(9);
	m_staticRangeMin.SetWindowText(str);
	str.Format("%f", pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
	str=str.Left(9);
	m_staticRangeMax.SetWindowText(str);
}

void CWordPage::OnBnClickedButtonBrowseLibfile()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	static char BASED_CODE szFilter[] = "Word Rule Library File (*.wdl)|*.wdl|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.wdl", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (m_ldFile.DoModal()==IDOK)
	{
		CString strFileName = extractFileName(m_ldFile.GetFileName());
		CString strNewFilePath= pDoc->m_strProjectDir+"\\"+strFileName;
		CopyFile(m_ldFile.GetFileName(),strNewFilePath,false);  //copy word lib file to current project directory
		m_editLibFileName.SetWindowText(strFileName);
		((CWordRule *)(pFrame->m_leftBar.m_pCurRule))->setLibFileName(strNewFilePath);
		((CWordRule *)(pFrame->m_leftBar.m_pCurRule))->LoadWordLib();
		CString strPhrase = ((CWordRule *)(pFrame->m_leftBar.m_pCurRule))->getWordLib()->getWordCurve(0)->getPhrase();        
		((CWordRule *)(pFrame->m_leftBar.m_pCurRule))->setPhrase(strPhrase);
		CString strEnvAttri = ((CWordRule *)(pFrame->m_leftBar.m_pCurRule))->getWordLib()->getWordCurve(0)->getEnvAttri();        
		((CWordRule *)(pFrame->m_leftBar.m_pCurRule))->setEnvAttri(strEnvAttri);
		pFrame->m_leftBar.m_pCurRule->setCurGraphMin(pFrame->m_leftBar.m_pCurRule->ComputeRuleGraphMin());
		pFrame->m_leftBar.m_pCurRule->setCurGraphMax(pFrame->m_leftBar.m_pCurRule->ComputeRuleGraphMax());
		pFrame->m_leftBar.m_pCurRule->setDispMin(pFrame->m_leftBar.m_pCurRule->getCurGraphMin());
		pFrame->m_leftBar.m_pCurRule->setDispMax(pFrame->m_leftBar.m_pCurRule->getCurGraphMax());	
		FillPage();
	}
}

void CWordPage::OnBnClickedButtonChangetocurverule()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();
	CWordRule* pCRCurr=(CWordRule*)(pFrame->m_leftBar.m_pCurRule);
	if(pCRCurr->IsLegal()==false)
	{
		AfxMessageBox("Please finish the defination of this word rule.");
		return;
	}
	CFreehandRule *curveruleTempRule=new CFreehandRule();
	curveruleTempRule->setRuleName(pCRCurr->getRuleName());
	curveruleTempRule->setRuleType(1);
	curveruleTempRule->setLayerName(pCRCurr->getLayerName());
	CWordCurve *pWordCurve;
	pWordCurve = pCRCurr->getWordLib()->getWordCurve(pCRCurr->getPhrase(),pCRCurr->getEnvAttri());
	for (int i=0;i<pWordCurve->getKnotNum();i++)
	{
		curveruleTempRule->AddKnot(pWordCurve->getKnotX(i),pWordCurve->getKnotY(i));
	}
	curveruleTempRule->setCurGraphMin(curveruleTempRule->ComputeRuleGraphMin());
	curveruleTempRule->setCurGraphMax(curveruleTempRule->ComputeRuleGraphMax());
	curveruleTempRule->setDispMin(curveruleTempRule->getCurGraphMin());
	curveruleTempRule->setDispMax(curveruleTempRule->getCurGraphMax());
	HTREEITEM hCurrent = pFrame->m_leftBar.m_trProjectTree.GetSelectedItem();
	HTREEITEM hRootItem= pFrame->m_leftBar.m_trProjectTree.GetParentItem(hCurrent);
	HTREEITEM hNewItem=pFrame->m_leftBar.m_trProjectTree.InsertItem(_T(pCRCurr->getRuleName()),hRootItem);
	if (pCRCurr->getIsUsed()==true)
	{
		curveruleTempRule->setIsUsed(true);
		pFrame->m_leftBar.m_trProjectTree.SetItemState( hNewItem, INDEXTOSTATEIMAGEMASK(8), TVIS_STATEIMAGEMASK );
	}
	else
	{
		curveruleTempRule->setIsUsed(false);
		pFrame->m_leftBar.m_trProjectTree.SetItemState( hNewItem, INDEXTOSTATEIMAGEMASK(9), TVIS_STATEIMAGEMASK );
	}
	CString  strRuleName, strPName, strPPName,strSoilTypeName;
	strRuleName=pCRCurr->getRuleName();
	HTREEITEM  pItem= pFrame->m_leftBar.m_trProjectTree.GetParentItem(hCurrent);//Get the parent item
	strPName = pFrame->m_leftBar.m_trProjectTree.GetItemText(pItem);
	HTREEITEM  ppItem= pFrame->m_leftBar.m_trProjectTree.GetParentItem(pItem);
	strPPName = pFrame->m_leftBar.m_trProjectTree.GetItemText(ppItem);
	HTREEITEM  pppItem= pFrame->m_leftBar.m_trProjectTree.GetParentItem(ppItem);//Get the parent item
	strSoilTypeName = pFrame->m_leftBar.m_trProjectTree.GetItemText(pppItem);
	int iCurSoil,iCurInstance,iCurOccurrence,iCurExclusion;
	for(int i=0;i<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilTypeNum();i++) 
	{
		CSoilType *pCurSoilType = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i);
		if(strSoilTypeName == pCurSoilType->getSoilTypeName())
		{
			iCurSoil = i;
			if(strPPName == "Instances")
			{
				for(int ii=0; ii<pCurSoilType->getInstanceNum(); ii++)
				{
					CInstance *pInstance = pCurSoilType->getInstance(ii);
					if(strPName == pInstance->getInstanceName())
					{
						iCurInstance = ii;
						for(int iii=0;iii<pInstance->getRuleNum();iii++)
						{
							if(strRuleName == pInstance->getRule(iii)->getRuleName())
							{
								pInstance->DeleteRule(iii);
								pInstance->AddRule((CRule*)curveruleTempRule);
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
						iCurOccurrence = ii;
						for(int iii=0;iii<pOccurrence->getRuleNum();iii++)
						{
							if(strRuleName == pOccurrence->getRule(iii)->getRuleName())
							{
								pOccurrence->DeleteRule(iii);
								pOccurrence->AddRule((CRule*)curveruleTempRule);
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
						iCurExclusion = ii;
						for(int iii=0;iii<pExclusion->getRuleNum();iii++)
						{
							if(strRuleName == pExclusion->getRule(iii)->getRuleName())
							{
								pExclusion->DeleteRule(iii);
								pExclusion->AddRule((CRule*)curveruleTempRule);
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

	pFrame->m_leftBar.m_pCurRule = NULL;
	pFrame->m_leftBar.m_trProjectTree.DeleteItem(hCurrent);
	pFrame->m_leftBar.m_trProjectTree.Select(hNewItem,TVGN_CARET);
}



void CWordPage::OnCbnSelchangeComboEnvAttri()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();
	int iCurSel = m_comboEnvAttri.GetCurSel();
	CString str;
	m_comboEnvAttri.GetLBText(iCurSel, str);
	((CWordRule *)(pFrame->m_leftBar.m_pCurRule))->setEnvAttri(str);
	for(int i=0;i<((CWordRule*)(pFrame->m_leftBar.m_pCurRule))->getWordLib()->getWordCurveNum();i++)
	{	
		CString strPhrase = ((CWordRule*)(pFrame->m_leftBar.m_pCurRule))->getWordLib()->getWordCurve(i)->getPhrase();
		if(((CWordRule*)(pFrame->m_leftBar.m_pCurRule))->getWordLib()->getWordCurve(i)->getEnvAttri() == str)
		{
			((CWordRule *)(pFrame->m_leftBar.m_pCurRule))->setPhrase(strPhrase);
			break;
		}
	}
	pFrame->m_leftBar.m_pCurRule->setCurGraphMin(pFrame->m_leftBar.m_pCurRule->ComputeRuleGraphMin());
	pFrame->m_leftBar.m_pCurRule->setCurGraphMax(pFrame->m_leftBar.m_pCurRule->ComputeRuleGraphMax());
	pFrame->m_leftBar.m_pCurRule->setDispMin(pFrame->m_leftBar.m_pCurRule->getCurGraphMin());
	pFrame->m_leftBar.m_pCurRule->setDispMax(pFrame->m_leftBar.m_pCurRule->getCurGraphMax());	
	FillPage();
	m_pGraphPanel.Invalidate();
	SetFocus();
}

void CWordPage::OnCbnSelchangeComboCurveName()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	int iCurSel = m_comboCurveName.GetCurSel();
	CString str;
	m_comboCurveName.GetLBText(iCurSel, str);
	((CWordRule *)(pFrame->m_leftBar.m_pCurRule))->setPhrase(str);
	pFrame->m_leftBar.m_pCurRule->setCurGraphMin(pFrame->m_leftBar.m_pCurRule->ComputeRuleGraphMin());
	pFrame->m_leftBar.m_pCurRule->setCurGraphMax(pFrame->m_leftBar.m_pCurRule->ComputeRuleGraphMax());
	pFrame->m_leftBar.m_pCurRule->setDispMin(pFrame->m_leftBar.m_pCurRule->getCurGraphMin());
	pFrame->m_leftBar.m_pCurRule->setDispMax(pFrame->m_leftBar.m_pCurRule->getCurGraphMax());	
	FillPage();
	m_pGraphPanel.Invalidate();
	SetFocus();
}

void CWordPage::OnCbnSelchangeComboLayerSelect2()
{
	UpdateData(true);
	// TODO: Add your control notification handler code here
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	CString strSelected;

	int curSel = this->m_comboAttachLayer.GetCurSel();
	m_comboAttachLayer.GetLBText(curSel, strSelected);





	if(strSelected == "Choose One Layer...")
	{
			

	
	static char BASED_CODE szFilter[] = "Layer Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Choose Layer File";
	if(m_ldFile.DoModal() == IDOK)
	{   
		CString strLayerName = "untitled";
		CString strLayerFilePath =  m_ldFile.GetPathName(); 
		//create the new layer
		CEnvLayer *pLayer = new CEnvLayer(pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum(),strLayerName, strLayerFilePath);
		if(pLayer->getLayerID() != -1)
		{ 
			strLayerName.Format("%s",pLayer->m_pTheData->getDataName());
			pLayer->setLayerName(strLayerName);
			for(int i=0;i<pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum();i++) 
			{
				if((pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(i)->getLayerName())==strLayerName) //if there is a layer with the same name alread
				{
					if(AfxMessageBox("The name of the layer is the same with one layer the database already has. Do you want to replace the existing layer with the new one?",MB_YESNO)==IDYES)
					{
						CString strNewLayerFilePath;
						CString strLayerFileName = extractFileName(strLayerFilePath);
						strNewLayerFilePath= pDoc->m_strProjectDir+"\\GISData\\"+strLayerFileName;
						CopyFile(strLayerFilePath,strNewLayerFilePath,false);  //copy env layer to current project directory
						pLayer->setFileName(strNewLayerFilePath);
						//delete the duplicate layer first
						pDoc->m_pProjectEdit->m_pEnvDatabase->DeleteLayer(i);
						pDoc->m_pProjectEdit->m_pEnvDatabase->AddLayer(pLayer);
				//		m_buttonChooseLayer.ShowWindow(SW_HIDE);
						pFrame->m_leftBar.m_pCurRule->setLayerName(strLayerName);
							m_staticLayer.SetWindowText("Data layer linked: ");
						pFrame->m_leftBar.FillProjectTree(pDoc);
						FillPage();
						//pDoc->UpdateAllViews(NULL);
						return;
					}
					else  //do not replace
					{
						delete pLayer;
						pLayer = NULL;
						FillPage();
						return;
					}
					break;
			 }
			}

			if(pDoc->m_pProjectEdit->m_pEnvDatabase->AddLayer(pLayer))   //if this is a new layer and it is added successfully
			{
				CString strNewLayerFilePath;
				CString strLayerFileName = extractFileName(strLayerFilePath);
				strNewLayerFilePath= pDoc->m_strProjectDir+"\\GISData\\"+strLayerFileName;
				CopyFile(strLayerFilePath,strNewLayerFilePath,false);  //copy env layer to current project directory
				pLayer->setFileName(strNewLayerFilePath);
			//	m_buttonChooseLayer.ShowWindow(SW_HIDE);
				pFrame->m_leftBar.m_pCurRule->setLayerName(strLayerName);

				m_staticLayer.SetWindowText("Data layer linked: ");

				pFrame->m_leftBar.FillProjectTree(pDoc);
				FillPage();
				//pDoc->UpdateAllViews(NULL);
				return;
			}
			
		}
		else 
		{
			delete pLayer;
			pLayer = NULL;
			CString msg;
				msg.Format("Error in reading %s. The layer will not be added",strLayerFilePath);
			AfxMessageBox(msg);
			return;
		}
	}
	else
		this->FillPage();
	}

	else if(strSelected == "------")
	{
		m_staticLayer.SetWindowText("No data layer linked.\nPlease set data layer: ");
	}
	else
	{
		pFrame->m_leftBar.m_pCurRule->setLayerName(strSelected);
		m_staticLayer.SetWindowText("Data layer linked: ");


	}

		this->m_pGraphPanel.Invalidate();

}



void CWordPage::OnBnClickedButtonApplyRule()
{
	// TODO: Add your control notification handler code here



	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	CString strLayerName=  pFrame->m_leftBar.m_pCurRule->getLayerName();


	if(strLayerName!="")
	{
		CEnvLayer *pLayer = pDoc->m_pProjectEdit->m_pEnvDatabase->FindDataLayer(strLayerName);

		CThreeDR *pNewData = new CThreeDR();

		int row = pLayer->m_pTheData->getNumberOfRows();
		int col = pLayer->m_pTheData->getNumberOfColumns();



		pNewData->setNullDataValue(NOFLOATVALUE);

		pNewData->setNumberOfColumns(col);
		pNewData->setNumberOfRows(row);

		float *dataBuf = new float[row * col];
		for(int i=0; i<row; i++)
			for(int j=0; j<col; j++)
			{
				dataBuf[i*col + j] = pFrame->m_leftBar.m_pCurRule->Evaluate(pLayer->m_pTheData->getData(i, j));
			}
			
			pNewData->setDataBuf(dataBuf);


			pNewData->calcStat();
			
			
	        C2dViewerDlg dlg;

			dlg.m_theInput = pNewData;

	       dlg.DoModal();
		  

	}
	else 
	{
		CString str = "No data layer linked.\nPlease set data layer first.";
		MessageBox(str);

	}



}

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
#include "RangePage.h"
#include ".\rangepage.h"
#include "MainFrm.h"
#include "RangeRule.h"

#include "2dViewerDlg.h"

// CRangePage dialog
IMPLEMENT_DYNAMIC(CRangePage, CDialog)

CRangePage::CRangePage(CWnd* pParent /*=NULL*/)
: CDialog(CRangePage::IDD, pParent)
{

}

CRangePage::~CRangePage()
{
}

void CRangePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_USE, m_chkUse);
	DDX_Control(pDX, IDC_STATIC_LAYER, m_staticLayer);
	DDX_Control(pDX, IDC_STATIC_MIN, m_staticRangeMin);
	DDX_Control(pDX, IDC_STATIC_MAX, m_staticRangeMax);
	//DDX_Control(pDX, IDC_BUTTON_BROWSE_LAYER_FILE2, m_buttonChooseLayer);
	//DDX_Control(pDX, IDC_EDIT_LAYER, m_editLayerName);
	DDX_Control(pDX, IDC_BUTTON_MIN_LEFT, m_buttonMinLeft);
	DDX_Control(pDX, IDC_BUTTON_MIN_RIGHT,m_buttonMinRight);
	DDX_Control(pDX, IDC_BUTTON_MAX_LEFT, m_buttonMaxLeft);
	DDX_Control(pDX, IDC_BUTTON_MAX_RIGHT,m_buttonMaxRight);
	DDX_Control(pDX, IDC_STATIC_RULE_TYPE, m_staticRuleType);


	DDX_Control(pDX, IDC_STATIC_LOW_UNITY, m_staticLowUnity);
	DDX_Control(pDX, IDC_EDIT_LOW_UNITY, m_editLowUnity);
	DDX_Control(pDX, IDC_STATIC_HIGH_UNITY, m_staticHighUnity);
	DDX_Control(pDX, IDC_EDIT_HIGH_UNITY, m_editHighUnity);
	DDX_Control(pDX, IDC_STATIC_LOW_CROSS, m_staticLowCross);
	DDX_Control(pDX, IDC_EDIT_LOW_CROSS, m_editLowCross);
	DDX_Control(pDX, IDC_STATIC_HIGH_CROSS, m_staticHighCross);
	DDX_Control(pDX, IDC_EDIT_HIGH_CROSS, m_editHighCross);
	DDX_Control(pDX, IDC_COMBO_CURVE_TYPE, m_comboCurveType);
	DDX_Control(pDX, IDC_STATIC_CURVE_TYPE, m_staticCurveType);
	DDX_Control(pDX, IDC_BUTTON_UPDATE, m_buttonUpdate);

	DDX_Control(pDX,IDC_BUTTON_CHANGETOFREEHAND,m_btnChangeToFreehandRule);

	DDX_Control(pDX, IDC_COMBO_LAYER_SELECT, m_comboAttachLayer);

	DDX_Control(pDX,IDC_BUTTON_APPLY_THE_RULE, m_btnApplyTheRule);
}


BEGIN_MESSAGE_MAP(CRangePage, CDialog)
	ON_REGISTERED_MESSAGE(RANGE_CHANGED, OnRangeChange)
	ON_WM_SIZE()
	//ON_BN_CLICKED(IDC_BUTTON_BROWSE_LAYER_FILE2, OnBnClickedButtonBrowseLayerFile)
	ON_BN_CLICKED(IDC_BUTTON_MIN_LEFT, OnBnClickedButtonMinLeft)
	ON_BN_CLICKED(IDC_BUTTON_MIN_RIGHT, OnBnClickedButtonMinRight)
	ON_BN_CLICKED(IDC_BUTTON_MAX_LEFT, OnBnClickedButtonMaxLeft)
	ON_BN_CLICKED(IDC_BUTTON_MAX_RIGHT, OnBnClickedButtonMaxRight)
	ON_BN_CLICKED(IDC_CHECK_USE, OnBnClickedCheckUse)

	ON_EN_KILLFOCUS(IDC_EDIT_LOW_UNITY, OnEnKillfocusEditLowUnity)
	ON_EN_KILLFOCUS(IDC_EDIT_HIGH_UNITY, OnEnKillfocusEditHighUnity)
	ON_EN_KILLFOCUS(IDC_EDIT_LOW_CROSS, OnEnKillfocusEditLowCross)
	ON_EN_KILLFOCUS(IDC_EDIT_HIGH_CROSS, OnEnKillfocusEditHighCross)
	ON_CBN_SELCHANGE(IDC_COMBO_CURVE_TYPE, OnCbnSelchangeComboCurveType)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, OnBnClickedButtonUpdate)

	ON_BN_CLICKED(IDC_BUTTON_CHANGETOFREEHAND, OnBnClickedButtonChangetofreehand)
	ON_CBN_SELCHANGE(IDC_COMBO_LAYER_SELECT, OnCbnSelchangeComboLayerSelect)

	ON_BN_CLICKED(IDC_BUTTON_APPLY_THE_RULE, OnBnClickedButtonApplyRule)
END_MESSAGE_MAP()


// CRangePage message handlers

BOOL CRangePage::OnInitDialog()
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






	return TRUE;  // return TRUE unless you set the focus to a control
}

//the size of contols will change according to the window size
void CRangePage::OnSize(UINT nType, int cx, int cy)
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
		m_pGraphPanel.Invalidate();
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

	/*
	if(m_editLayerName.GetSafeHwnd())
	{
	CRect rect(3*cx/10,9*cy/10,6*cx/10,9*cy/10+20);
	m_editLayerName.MoveWindow(&rect);
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

	


	/*if(m_buttonChooseLayer.GetSafeHwnd())
	{
	CRect rect(6.5*cx/10,9*cy/10,7.5*cx/10,9*cy/10+20);
	m_buttonChooseLayer.MoveWindow(&rect);
	}*/
	if(m_staticCurveType.GetSafeHwnd())
	{
		CRect rect(7.2*cx/10,1.5*cy/10,7.2*cx/10+60,1.5*cy/10+20);
		m_staticCurveType.MoveWindow(&rect);
	}

	if(m_comboCurveType.GetSafeHwnd())
	{
		CRect rect(7.2*cx/10+70,1.5*cy/10,9.5*cx/10,1.5*cy/10+20);
		m_comboCurveType.MoveWindow(&rect);
	}
	if(m_staticLowUnity.GetSafeHwnd())
	{
		CRect rect(7.2*cx/10,2*cy/10+20,8.2*cx/10,2*cy/10+40);	  
		m_staticLowUnity.MoveWindow(&rect);
	} 
	if(m_editLowUnity.GetSafeHwnd())
	{
		CRect rect(7.2*cx/10,2*cy/10+45,8.2*cx/10,2*cy/10+65);	  
		m_editLowUnity.MoveWindow(&rect);
	} 
	if(m_staticLowCross.GetSafeHwnd())
	{
		CRect rect(7.2*cx/10,4*cy/10,8.2*cx/10,4*cy/10+20);	  
		m_staticLowCross.MoveWindow(&rect);

	} 
	if(m_editLowCross.GetSafeHwnd())
	{
		CRect rect(7.2*cx/10,4*cy/10+25,8.2*cx/10,4*cy/10+45);	  
		m_editLowCross.MoveWindow(&rect);
	} 

	if(m_staticHighUnity.GetSafeHwnd())
	{
		CRect rect(8.5*cx/10,2*cy/10+20,9.5*cx/10,2*cy/10+40);	  
		m_staticHighUnity.MoveWindow(&rect);
	} 
	if(m_editHighUnity.GetSafeHwnd())
	{
		CRect rect(8.5*cx/10,2*cy/10+45,9.5*cx/10,2*cy/10+65);	  
		m_editHighUnity.MoveWindow(&rect);
	} 

	if(m_staticHighCross.GetSafeHwnd())
	{
		CRect rect(8.5*cx/10,4*cy/10,9.5*cx/10,4*cy/10+20);	  
		m_staticHighCross.MoveWindow(&rect);
	} 
	if(m_editHighCross.GetSafeHwnd())
	{
		CRect rect(8.5*cx/10,4*cy/10+25,9.5*cx/10,4*cy/10+45);	  
		m_editHighCross.MoveWindow(&rect);
	} 

	if(m_buttonUpdate.GetSafeHwnd())
	{
		CRect rect(7.2*cx/10,6.5*cy/10-20,9.5*cx/10,6.5*cy/10);	  
		m_buttonUpdate.MoveWindow(&rect);
	} 

	if(m_btnChangeToFreehandRule.GetSafeHwnd())
	{
		CRect rect(cx*7.2/10,7*cy/10,9.5*cx/10,7*cy/10+20);
		m_btnChangeToFreehandRule.MoveWindow(&rect);
	}


	if(m_staticRuleType.GetSafeHwnd())
	{
		FillPage();
	}


}

void CRangePage::EnablePage(bool bIsEnable)
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();


	if(m_pGraphPanel.GetSafeHwnd())
	{
		m_pGraphPanel.EnableWindow(bIsEnable);
		m_pGraphPanel.SetEnable(bIsEnable);
		m_pGraphPanel.Invalidate();
	}

	if(m_staticRangeMin.GetSafeHwnd())
	{
		m_staticRangeMin.EnableWindow(bIsEnable);
	}
	if(m_staticRangeMax.GetSafeHwnd())
	{
		m_staticRangeMax.EnableWindow(bIsEnable);
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



	if(m_staticLayer.GetSafeHwnd())
	{
		m_staticLayer.EnableWindow(bIsEnable); 
	}

	if(m_comboAttachLayer.GetSafeHwnd())
	{		
		m_comboAttachLayer.EnableWindow(bIsEnable);
	}


	/*if(m_editLayerName.GetSafeHwnd())
	{
	m_editLayerName.EnableWindow(bIsEnable);
	}

	if(m_buttonChooseLayer.GetSafeHwnd())
	{
	m_buttonChooseLayer.EnableWindow(bIsEnable);
	}*/



	if(m_staticCurveType.GetSafeHwnd())
	{
		m_staticCurveType.EnableWindow(bIsEnable);
	}
	if(m_comboCurveType.GetSafeHwnd())
	{
		m_comboCurveType.EnableWindow(bIsEnable);
	}
	if(m_staticLowUnity.GetSafeHwnd())
	{
		m_staticLowUnity.EnableWindow(bIsEnable);
	}
	if(m_editLowUnity.GetSafeHwnd())
	{
		m_editLowUnity.EnableWindow(bIsEnable);
	}
	if(m_staticHighUnity.GetSafeHwnd())
	{
		m_staticHighUnity.EnableWindow(bIsEnable);
	}
	if(m_editHighUnity.GetSafeHwnd())
	{
		m_editHighUnity.EnableWindow(bIsEnable);
	}
	if(m_staticLowCross.GetSafeHwnd())
	{
		m_staticLowCross.EnableWindow(bIsEnable);
	}
	if(m_editLowCross.GetSafeHwnd())
	{
		m_editLowCross.EnableWindow(bIsEnable);
	}
	if(m_staticHighCross.GetSafeHwnd())
	{
		m_staticHighCross.EnableWindow(bIsEnable);
	}
	if(m_editHighCross.GetSafeHwnd())
	{
		m_editHighCross.EnableWindow(bIsEnable);
	}
	if(m_buttonUpdate.GetSafeHwnd())
	{
		m_buttonUpdate.EnableWindow(bIsEnable);
	} 

	if(m_btnChangeToFreehandRule.GetSafeHwnd())
	{
		m_btnChangeToFreehandRule.EnableWindow(bIsEnable);
	}

	if(((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getCurveType()==1)
	{
		m_editHighUnity.EnableWindow(false);
		m_editHighCross.EnableWindow(false);
	}
	if(((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getCurveType()==2)
	{
		m_editLowUnity.EnableWindow(false);
		m_editLowCross.EnableWindow(false);
	}


}

LRESULT CRangePage::OnRangeChange(WPARAM /* wParam */, LPARAM /* lParam */)
{
	UpdateData();

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();	
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


void CRangePage::FillPage()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();
	CString str;



	if(pFrame->m_leftBar.m_pCurRule!=NULL)
	{
		m_staticRuleType.SetWindowText("Rule Type: Range");
		if(((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getCurveType()==0) //bell shape
		{
			m_editHighUnity.EnableWindow(true);
			m_editHighCross.EnableWindow(true);
			m_editLowUnity.EnableWindow(true);
			m_editLowCross.EnableWindow(true);
			m_comboCurveType.SetCurSel(0);
			str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getLowUnity());
			str = str.Left(9);		
			m_editLowUnity.SetWindowText(str);
			str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getHighUnity());
			str = str.Left(9);	
			m_editHighUnity.SetWindowText(str);
			str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getLowCross());
			str = str.Left(9);	
			m_editLowCross.SetWindowText(str);
			str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getHighCross());
			str = str.Left(9);	
			m_editHighCross.SetWindowText(str);
		}

		else if(((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getCurveType()==1)  //s shape
		{ 
			m_editHighUnity.EnableWindow(false);
			m_editHighCross.EnableWindow(false);
			m_editLowUnity.EnableWindow(true);
			m_editLowCross.EnableWindow(true);
			m_comboCurveType.SetCurSel(1);
			str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getLowUnity());
			str = str.Left(9);	
			m_editLowUnity.SetWindowText(str);
			str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getLowCross());
			str = str.Left(9);	
			m_editLowCross.SetWindowText(str);
		} 
		else   //z shape
		{	
			m_editHighUnity.EnableWindow(true);
			m_editHighCross.EnableWindow(true);
			m_editLowUnity.EnableWindow(false);
			m_editLowCross.EnableWindow(false);
			m_comboCurveType.SetCurSel(2);
			str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getHighUnity());
			str = str.Left(9);	
			m_editHighUnity.SetWindowText(str);
			str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getHighCross());
			str = str.Left(9);	
			m_editHighCross.SetWindowText(str);
		}    

		//set slider 
		m_sliderRange.SetMinMax(pFrame->m_leftBar.m_pCurRule->getCurGraphMin(),pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
		str.Format("%f",pFrame->m_leftBar.m_pCurRule->getCurGraphMin());
		str = str.Left(9);	
		m_staticRangeMin.SetWindowText(str);
		str.Format("%f", pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
		str = str.Left(9);	
		m_staticRangeMax.SetWindowText(str);
		m_sliderRange.SetRange(pFrame->m_leftBar.m_pCurRule->getDispMin(),pFrame->m_leftBar.m_pCurRule->getDispMax());

		//set check use
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

		m_pGraphPanel.Invalidate();
	}

}

void CRangePage::OnBnClickedCheckUse()
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

/*
void CRangePage::OnBnClickedButtonBrowseLayerFile()
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
m_buttonChooseLayer.ShowWindow(SW_HIDE);
pFrame->m_leftBar.m_pCurRule->setLayerName(strLayerName);
pFrame->m_leftBar.FillProjectTree(pDoc);
FillPage();
pDoc->UpdateAllViews(NULL);
return;
}
else  //do not replace
{
delete pLayer;
pLayer = NULL;
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


void CRangePage::OnBnClickedButtonMinLeft()
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
	str = str.Left(9);
	m_staticRangeMin.SetWindowText(str);
	str.Format("%f", pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
	str = str.Left(9);
	m_staticRangeMax.SetWindowText(str);

}

void CRangePage::OnBnClickedButtonMinRight()
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
	str = str.Left(9);
	m_staticRangeMin.SetWindowText(str);
	str.Format("%f", pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
	str = str.Left(9);
	m_staticRangeMax.SetWindowText(str);

}

void CRangePage::OnBnClickedButtonMaxLeft()
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
	str = str.Left(9);
	m_staticRangeMin.SetWindowText(str);
	str.Format("%f", pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
	str = str.Left(9);
	m_staticRangeMax.SetWindowText(str);
}

void CRangePage::OnBnClickedButtonMaxRight()
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
	str = str.Left(9);
	m_staticRangeMin.SetWindowText(str);
	str.Format("%f", pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
	str = str.Left(9);
	m_staticRangeMax.SetWindowText(str);

}

void CRangePage::OnCancel()
{
	DestroyWindow();
	CDialog::OnCancel();
}

//if the return key is pressed, do sth. 
BOOL CRangePage::PreTranslateMessage( MSG *pMsg )
{

	if (pMsg->message   ==   WM_KEYDOWN)   
	{   
		if   (pMsg->wParam   ==   VK_ESCAPE)   
			return   TRUE;   
		if   (pMsg->wParam   ==   VK_RETURN)   
		{
			SetFocus();
			OnBnClickedButtonUpdate();
			Invalidate();
			return   TRUE;   
		}
	}
	return   CDialog::PreTranslateMessage(pMsg); 
}

void CRangePage::OnEnKillfocusEditLowUnity()
{	

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	UpdateData(TRUE);
	CString str;
	m_editLowUnity.GetWindowText(str);
	if(!IsFloat(str))  //judge if the input string is a number
	{
		AfxMessageBox("You must input a floating number.");
		str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getLowUnity());
		m_editLowUnity.SetWindowText(str);		
	}
}

void CRangePage::OnEnKillfocusEditHighUnity()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	UpdateData(TRUE);
	CString str;
	m_editHighUnity.GetWindowText(str);
	if(!IsFloat(str))
	{
		AfxMessageBox("You must input a floating number.");
		str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getHighUnity());
		str = str.Left(9);
		m_editHighUnity.SetWindowText(str);		
	}
}

void CRangePage::OnEnKillfocusEditLowCross()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	UpdateData(TRUE);
	CString str;
	m_editLowCross.GetWindowText(str);
	if(!IsFloat(str))
	{
		AfxMessageBox("You must input a floating number.");
		str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getLowCross());
		str = str.Left(9);
		m_editLowCross.SetWindowText(str);		
	}
}

void CRangePage::OnEnKillfocusEditHighCross()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	UpdateData(TRUE);
	CString str;
	m_editHighCross.GetWindowText(str);
	if(!IsFloat(str))
	{
		AfxMessageBox("You must input a floating number.");
		str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getHighCross());
		str = str.Left(9);
		m_editHighCross.SetWindowText(str);		
	}
}

void CRangePage::OnCbnSelchangeComboCurveType()
{
	UpdateData(TRUE);


	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	int s = m_comboCurveType.GetCurSel();
	if(s == 0)
	{
		((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->setCurveType(0);
	}
	else if(s==1)
	{
		((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->setCurveType(1);
	}
	else if(s==2)
	{
		((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->setCurveType(2);
	}
	if(((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getCurveType()==0)
	{
		m_editHighUnity.EnableWindow(true);
		m_editHighCross.EnableWindow(true);
		m_editLowUnity.EnableWindow(true);
		m_editLowCross.EnableWindow(true);
		m_comboCurveType.SetCurSel(0);
		CString str;
		str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getLowUnity());
		str = str.Left(9);
		m_editLowUnity.SetWindowText(str);
		str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getHighUnity());
		str = str.Left(9);
		m_editHighUnity.SetWindowText(str);
		str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getLowCross());
		str = str.Left(9);
		m_editLowCross.SetWindowText(str);
		str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getHighCross());
		str = str.Left(9);
		m_editHighCross.SetWindowText(str);
	}

	else if (((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getCurveType()==1)
	{ 
		m_editHighUnity.EnableWindow(false);
		m_editHighCross.EnableWindow(false);
		m_editLowUnity.EnableWindow(true);
		m_editLowCross.EnableWindow(true);
		m_comboCurveType.SetCurSel(1);
		CString str;
		str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getLowUnity());
		str = str.Left(9);
		m_editLowUnity.SetWindowText(str);
		str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getLowCross());
		str = str.Left(9);
		m_editLowCross.SetWindowText(str);
	}
	else 
	{	
		m_editHighUnity.EnableWindow(true);
		m_editHighCross.EnableWindow(true);
		m_editLowUnity.EnableWindow(false);
		m_editLowCross.EnableWindow(false);
		m_comboCurveType.SetCurSel(2);
		CString str;
		str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getHighUnity());
		str = str.Left(9);
		m_editHighUnity.SetWindowText(str);
		str.Format("%f",((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getHighCross());
		str = str.Left(9);
		m_editHighCross.SetWindowText(str);
	}  

	Invalidate();
	SetFocus();
}


//If the update button is pressed, update the rule
void CRangePage::OnBnClickedButtonUpdate()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();	

	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	double dLowCross;
	double dLowUnity;
	double dHighUnity;
	double dHighCross;
	CString str;
	if(((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getCurveType()==0)
	{
		m_editLowCross.GetWindowText(str);
		dLowCross = atof(str);
		m_editLowUnity.GetWindowText(str);
		dLowUnity = atof(str);
		m_editHighUnity.GetWindowText(str);
		dHighUnity = atof(str);
		m_editHighCross.GetWindowText(str);
		dHighCross = atof(str);
		if(dLowCross>= dLowUnity)
		{
			MessageBox("Low Cross must be smaller than Low Unity.");
			return;
		}
		else if(dLowUnity> dHighUnity)
		{
			MessageBox("Low Unity must be smaller than or equal to High Unity.");
			return;
		}
		else if(dHighUnity>=dHighCross)
		{
			MessageBox("High Unity must be smaller than High Cross.");

		}
		else
		{
			((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->setLowCross(dLowCross);
			((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->setLowUnity(dLowUnity);
			((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->setHighUnity(dHighUnity);
			((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->setHighCross(dHighCross);
		}


	}
	else if(((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getCurveType()==1)
	{
		m_editLowCross.GetWindowText(str);
		dLowCross = atof(str);
		m_editLowUnity.GetWindowText(str);
		dLowUnity = atof(str);
		if(dLowCross>= dLowUnity)
		{
			MessageBox("Low Cross must be smaller than Low Unity.");
			return;
		}
		else
		{
			((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->setLowCross(dLowCross);
			((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->setLowUnity(dLowUnity);
		}


	}
	else if(((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->getCurveType()==2)
	{
		m_editHighUnity.GetWindowText(str);
		dHighUnity = atof(str);
		m_editHighCross.GetWindowText(str);
		dHighCross = atof(str);
		if(dHighUnity>=dHighCross)
		{
			MessageBox("High Unity must be smaller than High Cross.");
		}
		else
		{
			((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->setHighUnity(dHighUnity);
			((CRangeRule *)(pFrame->m_leftBar.m_pCurRule))->setHighCross(dHighCross);
		}

	}



	pFrame->m_leftBar.m_pCurRule->setCurGraphMin(pFrame->m_leftBar.m_pCurRule->ComputeRuleGraphMin());
	pFrame->m_leftBar.m_pCurRule->setCurGraphMax(pFrame->m_leftBar.m_pCurRule->ComputeRuleGraphMax());
	pFrame->m_leftBar.m_pCurRule->setDispMin(pFrame->m_leftBar.m_pCurRule->ComputeRuleGraphMin());
	pFrame->m_leftBar.m_pCurRule->setDispMax(pFrame->m_leftBar.m_pCurRule->ComputeRuleGraphMax());


	FillPage();
	SetFocus();

}


void CRangePage::OnBnClickedButtonChangetofreehand()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();
	CRangeRule* pCRCurr=(CRangeRule*)(pFrame->m_leftBar.m_pCurRule);
	if(pCRCurr->IsLegal()==false)
	{
		AfxMessageBox("Please finish the defination of this word rule.");
		return;
	}
	CFreehandRule *curveruleTempRule=new CFreehandRule();
	curveruleTempRule->setRuleName(pCRCurr->getRuleName());
	curveruleTempRule->setRuleType(1);
	curveruleTempRule->setLayerName(pCRCurr->getLayerName());

	if(pCRCurr->getCurveType() == 0)
	{
		double dLowUnity,dHighUnity,dLowCross,dHighCross;
		dLowUnity = pCRCurr->getLowUnity();	
		dHighUnity = pCRCurr->getHighUnity();		
		dLowCross = pCRCurr->getLowCross();
		dHighCross = pCRCurr->getHighCross();

		double dLWidth = dLowUnity - dLowCross;
		double dRWidth = dHighCross - dHighUnity;


		curveruleTempRule->AddKnot(dLowCross-dLWidth, pCRCurr->Evaluate(dLowCross-dLWidth));
		curveruleTempRule->AddKnot(dLowCross, 0.5);

		curveruleTempRule->AddKnot(dLowUnity, 1);

		if(fabs(dLowUnity-dHighUnity) > VERYSMALL)
			curveruleTempRule->AddKnot(dHighUnity, 1);
		curveruleTempRule->AddKnot(dHighCross, 0.5);
		curveruleTempRule->AddKnot(dHighCross+dRWidth, pCRCurr->Evaluate(dHighCross+dRWidth));

	}

	else if(pCRCurr->getCurveType() == 1)
	{
		double dLowUnity,dLowCross;
		dLowUnity = pCRCurr->getLowUnity();	

		dLowCross = pCRCurr->getLowCross();
		double dLWidth = dLowUnity - dLowCross;


		curveruleTempRule->AddKnot(dLowCross-dLWidth, pCRCurr->Evaluate(dLowCross-dLWidth));
		curveruleTempRule->AddKnot(dLowCross, 0.5);
		curveruleTempRule->AddKnot(dLowUnity, 1);
		curveruleTempRule->AddKnot(dLowUnity+dLWidth, 1);
		curveruleTempRule->AddKnot(dLowUnity+2*dLWidth, 1);


	}

	else if(pCRCurr->getCurveType() == 2)
	{
		double dHighUnity,dHighCross;
		dHighUnity = pCRCurr->getHighUnity();		

		dHighCross = pCRCurr->getHighCross();

		double dRWidth = dHighCross - dHighUnity;
		curveruleTempRule->AddKnot(dHighUnity-2*dRWidth, 1);

		curveruleTempRule->AddKnot(dHighUnity-dRWidth, 1);

		curveruleTempRule->AddKnot(dHighUnity, 1);
		curveruleTempRule->AddKnot(dHighCross, 0.5);
		curveruleTempRule->AddKnot(dHighCross+dRWidth, pCRCurr->Evaluate(dHighCross+dRWidth));

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

void CRangePage::OnCbnSelchangeComboLayerSelect()
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

void CRangePage::OnBnClickedButtonApplyRule()
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
		   //{
			   //delete(pNewData->d);
			   //pNewData->DataBuf = NULL;
			  //delete(pNewData);
			  //pNewData = NULL;
		  // }


	}
	else 
	{
		CString str = "No data layer linked.\nPlease set data layer first.";
		MessageBox(str);

	}



}

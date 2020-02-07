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
// EnumPage.cpp 
//

#include "stdafx.h"
#include "SoLIM.h"
#include "EnumPage.h"
#include ".\enumpage.h"
#include "EnumRule.h"
#include "MainFrm.h"
#include "SoLIMDoc.h"
#include "NewEnumItemDlg.h"


IMPLEMENT_DYNAMIC(CEnumPage, CDialog)
CEnumPage::CEnumPage(CWnd* pParent /*=NULL*/)
: CDialog(CEnumPage::IDD, pParent)
{

}

CEnumPage::~CEnumPage()
{
}

void CEnumPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_USE, m_chkUse);

	DDX_Control(pDX, IDC_STATIC_MIN, m_staticRangeMin);
	DDX_Control(pDX, IDC_STATIC_MAX, m_staticRangeMax);	
	DDX_Control(pDX, IDC_BUTTON_MIN_LEFT, m_buttonMinLeft);
	DDX_Control(pDX, IDC_BUTTON_MIN_RIGHT, m_buttonMinRight);
	DDX_Control(pDX, IDC_BUTTON_MAX_LEFT, m_buttonMaxLeft);
	DDX_Control(pDX, IDC_BUTTON_MAX_RIGHT, m_buttonMaxRight);
	
	DDX_Control(pDX, IDC_STATIC_RULE_TYPE, m_staticRuleType);

	DDX_Control(pDX, IDC_LIST_ITEM, m_listctrlEnumItem);
	DDX_Control(pDX, IDC_BUTTON_ADD_ITEM, m_btnAddItem);
	DDX_Control(pDX, IDC_BUTTON_REMOVE_ITEM, m_btnDeleteItem);
	DDX_Control(pDX, IDC_STATIC_CURRENTY, m_staticCurrentY);
	DDX_Control(pDX, IDC_STATIC_CURRENTX, m_staticCurrentX);
}


BEGIN_MESSAGE_MAP(CEnumPage, CDialog)
	
	ON_BN_CLICKED(IDC_BUTTON_MIN_LEFT, OnBnClickedButtonMinLeft)
	ON_BN_CLICKED(IDC_BUTTON_MIN_RIGHT, OnBnClickedButtonMinRight)
	ON_BN_CLICKED(IDC_BUTTON_MAX_LEFT, OnBnClickedButtonMaxLeft)
	ON_BN_CLICKED(IDC_BUTTON_MAX_RIGHT, OnBnClickedButtonMaxRight)
	ON_BN_CLICKED(IDC_CHECK_USE, OnBnClickedCheckUse)
	ON_REGISTERED_MESSAGE(RANGE_CHANGED, OnRangeChange)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_ADD_ITEM, OnBnClickedButtonAddItem)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_ITEM, OnBnClickedButtonRemoveItem)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ITEM, OnNMDblclkListItem)
END_MESSAGE_MAP()


BOOL CEnumPage::OnInitDialog()
{
	CDialog::OnInitDialog();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();
	CRect rect;
	GetClientRect(&rect);
	int cx = rect.right;
	int cy = rect.bottom;
	m_listctrlEnumItem.InsertColumn(1,CString("Item"));
	m_listctrlEnumItem.SetColumnWidth(0,120);

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
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void CEnumPage::OnSize(UINT nType, int cx, int cy)
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
	if(m_btnAddItem.GetSafeHwnd())
	{
		CRect rect(cx*7.2/10,cy*1.5/10,cx*8.2/10,cy*1.5/10+20);
		m_btnAddItem.MoveWindow(&rect);
	}
	if(m_btnDeleteItem.GetSafeHwnd())
	{
		CRect rect(cx*8.5/10,cy*1.5/10,cx*9.5/10,cy*1.5/10+20);
		m_btnDeleteItem.MoveWindow(&rect);
	}
	if(m_listctrlEnumItem.GetSafeHwnd())
	{		
		CRect rect(cx*7.2/10,cy*1.5/10+25,cx*9.5/10,cy*5/10);
		m_listctrlEnumItem.MoveWindow(&rect);
	}
	if (m_staticCurrentX.GetSafeHwnd())
	{
		CRect rect(7.2*cx/10,6.5*cy/10-45,9.5*cx/10,6.5*cy/10-25);
		m_staticCurrentX.MoveWindow(rect);
	}
	if (m_staticCurrentY.GetSafeHwnd())
	{
		CRect rect(7.2*cx/10,6.5*cy/10-20,9.5*cx/10,6.5*cy/10);
		m_staticCurrentY.MoveWindow(rect);
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


}

void CEnumPage::EnablePage(bool bIsEnable)
{

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


	if (m_staticCurrentX.GetSafeHwnd())
	{
		m_staticCurrentX.EnableWindow(bIsEnable);
	}
	if (m_staticCurrentY.GetSafeHwnd())
	{
		m_staticCurrentY.EnableWindow(bIsEnable);
	}
	if(m_btnAddItem.GetSafeHwnd())
	{
		m_btnAddItem.EnableWindow(bIsEnable);
	}
	if(m_btnDeleteItem.GetSafeHwnd())
	{
		m_btnDeleteItem.EnableWindow(bIsEnable);
	}

	if(m_listctrlEnumItem.GetSafeHwnd())
	{		
		m_listctrlEnumItem.EnableWindow(bIsEnable);
	}
	m_pGraphPanel.Invalidate();
}

void CEnumPage::FillPage()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	CEnumRule* pCRCurr=(CEnumRule*)(pFrame->m_leftBar.m_pCurRule);
	CString str;
	m_listctrlEnumItem.DeleteAllItems();
	if(pFrame->m_leftBar.m_pCurRule!=NULL)
	{
		m_staticRuleType.SetWindowText("Rule Type: Enumerated");
		int tempItem;
		CString strTemp;
		for(int i=0;i<((CEnumRule *)(pFrame->m_leftBar.m_pCurRule))->getItemNum();i++)
		{
			tempItem=pCRCurr->getItem(i);
			strTemp.Format("%d",tempItem); 
			m_listctrlEnumItem.InsertItem(LVIF_TEXT|LVIF_STATE,i, strTemp, 0, LVIS_SELECTED,0, 0);
		}
		m_sliderRange.SetMinMax(pFrame->m_leftBar.m_pCurRule->getCurGraphMin(),pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
		str.Format("%f",pFrame->m_leftBar.m_pCurRule->getCurGraphMin());
		str=str.Left(9);
		m_staticRangeMin.SetWindowText(str);
		str.Format("%f", pFrame->m_leftBar.m_pCurRule->getCurGraphMax());
		str=str.Left(9);
		m_staticRangeMax.SetWindowText(str);
		m_sliderRange.SetRange(pFrame->m_leftBar.m_pCurRule->getDispMin(),pFrame->m_leftBar.m_pCurRule->getDispMax());
		m_chkUse.SetCheck(pFrame->m_leftBar.m_pCurRule->getIsUsed());
		

		EnablePage(pFrame->m_leftBar.m_pCurRule->getIsUsed());
		Invalidate();
	}
}

void CEnumPage::DisplayCurrentCursorPosition(double x, double y)
{
	CString strX,strY;
	strX.Format("%.0f",x);
	strY.Format("%.1f",y);
	strX="Env.Value:"+strX;
	strY="Optimality:"+strY;
	m_staticCurrentX.SetWindowText(strX);
	m_staticCurrentY.SetWindowText(strY);
}


LRESULT CEnumPage::OnRangeChange(WPARAM /* wParam */, LPARAM /* lParam */)
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

BOOL CEnumPage::PreTranslateMessage( MSG *pMsg )
{
	if (pMsg->message   ==   WM_KEYDOWN)   
	{   
		if (pMsg->wParam   ==   VK_ESCAPE)   
			return   TRUE;   
		if (pMsg->wParam   ==   VK_RETURN)   
		{
			return   TRUE;   
		}
	}
	return   CDialog::PreTranslateMessage(pMsg);   
}


void CEnumPage::OnBnClickedButtonMinLeft()
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

void CEnumPage::OnBnClickedButtonMinRight()
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

void CEnumPage::OnBnClickedButtonMaxLeft()
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

void CEnumPage::OnBnClickedButtonMaxRight()
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


void CEnumPage::OnBnClickedCheckUse()
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


void CEnumPage::OnBnClickedButtonAddItem()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();
	CEnumRule* pCRCurr=(CEnumRule*)(pFrame->m_leftBar.m_pCurRule);

	int itemCount = m_listctrlEnumItem.GetItemCount();
	CNewEnumItemDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		CString strTempX;
		strTempX.Format("%d",dlg.m_iEnumItem);
		m_listctrlEnumItem.InsertItem(LVIF_TEXT|LVIF_STATE,itemCount, strTempX, 0, LVIS_SELECTED,0, 0);
		pCRCurr->AddItem(dlg.m_iEnumItem);
		m_pGraphPanel.Invalidate(false);
		FillPage();
	}

}

void CEnumPage::OnBnClickedButtonRemoveItem()
{
	if(m_listctrlEnumItem.GetItemCount()>0)
	{
		CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
		CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();
		CEnumRule* pCRCurr=(CEnumRule*)(pFrame->m_leftBar.m_pCurRule);
		POSITION pos=m_listctrlEnumItem.GetFirstSelectedItemPosition();
		int iDeleteIndex=m_listctrlEnumItem.GetNextSelectedItem(pos);
		pCRCurr->DeleteItem(iDeleteIndex);
		FillPage();
		m_pGraphPanel.Invalidate(true);
	}
}

void CEnumPage::OnNMDblclkListItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	CEnumRule* pCRCurr=(CEnumRule*)(pFrame->m_leftBar.m_pCurRule);
	NM_LISTVIEW*   pNMListView   =   (NM_LISTVIEW*)pNMHDR;   
	if(pNMListView->iItem   !=   -1)       
	{   
		CNewEnumItemDlg dlg;
		if(dlg.DoModal()==IDOK)
		{
			pCRCurr->setItem(pNMListView->iItem,dlg.m_iEnumItem);
			m_pGraphPanel.Invalidate(false);
			FillPage();
		}
	}  
	*pResult = 0;
}

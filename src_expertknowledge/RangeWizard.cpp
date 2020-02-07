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
// RangeWizard.cpp
//

#include "stdafx.h"
#include "SoLIM.h"
#include "RangeWizard.h"
#include "MainFrm.h"
#include "SoLIMDoc.h"


IMPLEMENT_DYNAMIC(CRangeWizard, CDialog)
CRangeWizard::CRangeWizard(CWnd* pParent /*=NULL*/)
: CDialog(CRangeWizard::IDD, pParent)
{
	pPageLink=NULL;
	nPageCount=0;
	nCurrentPage=0;
	m_bAttachLayer=false;
}

CRangeWizard::~CRangeWizard()
{
}

void CRangeWizard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRangeWizard, CDialog)
	ON_BN_CLICKED(IDC_PREV, OnPrev)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_FINISH, OnFinish)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CRangeWizard::AddPage(CDialog* pDialog, UINT nID)
{
	struct PAGELINK* pTemp = pPageLink;
	//insert new page
	struct PAGELINK* pNewPage = new PAGELINK;
	pNewPage->pDialog = pDialog;
	pNewPage->pDialog->Create(nID,this);

	// Is window created
	ASSERT(::IsWindow(pNewPage->pDialog->m_hWnd));

	DWORD dwStyle = pNewPage->pDialog->GetStyle();
	ASSERT((dwStyle & WS_CHILD) != 0); 
	ASSERT((dwStyle & WS_BORDER) == 0); 

	pNewPage->pDialog->ShowWindow(SW_HIDE);
	pNewPage->pDialog->MoveWindow(rectPage);

	pNewPage->Next=NULL;
	pNewPage->nNum=++nPageCount; 
	if (pTemp)
	{
		while (pTemp->Next) pTemp=pTemp->Next; 
		pTemp->Next=pNewPage;

	}
	else
		pPageLink=pNewPage; 
}



void CRangeWizard::OnCancel() 
{
	// TODO: Add your control notification handler code here
	if (AfxMessageBox("Do you want to give up defination?",MB_OKCANCEL|MB_ICONQUESTION)==IDCANCEL)
		return;   
	CDialog::OnCancel();
}

void CRangeWizard::OnPrev() 
{
	// TODO: Add your control notification handler code here
	
		ShowPage(--nCurrentPage);
}

void CRangeWizard::OnNext() 
{
	// TODO: Add your control notification handler code here


	if(nCurrentPage==1)
	{
		m_pCurveTypeDlg->UpdateData(true);
		int iCurSel =m_pCurveTypeDlg->m_comboCurveType.GetCurSel();
		if(iCurSel==0)
		{
			m_pRule->setCurveType(0);
			m_pRangePointsDlg->GetDlgItem(IDC_EDIT_LOW_UNITY)->EnableWindow(true);
			m_pRangePointsDlg->GetDlgItem(IDC_EDIT_HIGH_UNITY)->EnableWindow(true);
			m_pRangePointsDlg->GetDlgItem(IDC_EDIT_LOW_CROSS)->EnableWindow(true);
			m_pRangePointsDlg->GetDlgItem(IDC_EDIT_HIGH_CROSS)->EnableWindow(true);
		}
		else if(iCurSel==1)
		{
			m_pRule->setCurveType(1);
			m_pRangePointsDlg->GetDlgItem(IDC_EDIT_LOW_UNITY)->EnableWindow(true);
			m_pRangePointsDlg->GetDlgItem(IDC_EDIT_HIGH_UNITY)->EnableWindow(false);
			m_pRangePointsDlg->GetDlgItem(IDC_EDIT_LOW_CROSS)->EnableWindow(true);
			m_pRangePointsDlg->GetDlgItem(IDC_EDIT_HIGH_CROSS)->EnableWindow(false);
		}
		else if(iCurSel==2)
		{
			m_pRule->setCurveType(2);
			m_pRangePointsDlg->GetDlgItem(IDC_EDIT_LOW_UNITY)->EnableWindow(false);
			m_pRangePointsDlg->GetDlgItem(IDC_EDIT_HIGH_UNITY)->EnableWindow(true);
			m_pRangePointsDlg->GetDlgItem(IDC_EDIT_LOW_CROSS)->EnableWindow(false);
			m_pRangePointsDlg->GetDlgItem(IDC_EDIT_HIGH_CROSS)->EnableWindow(true);
		}

		else
		{
			return;
		}

		
			nCurrentPage++;
			ShowPage(2);

	
	}
	else if(nCurrentPage==2)
	{
		m_pRangePointsDlg->UpdateData(true);
		if(m_pRule->getCurveType()==0)
		{
			if(m_pRangePointsDlg->m_dLowCross>= m_pRangePointsDlg->m_dLowUnity)
			{
				MessageBox("Low Cross must be smaller than Low Unity.");
				return;
			}
			else if(m_pRangePointsDlg->m_dLowUnity> m_pRangePointsDlg->m_dHighUnity)
			{
				MessageBox("Low Unity must be smaller than or equal to High Unity.");
				return;
			}
			else if(m_pRangePointsDlg->m_dHighUnity>=m_pRangePointsDlg->m_dHighCross)
			{
				MessageBox("High Unity must be smaller than High Cross.");

			}
			else
			{
				m_pRule->setLowCross(m_pRangePointsDlg->m_dLowCross);
				m_pRule->setLowUnity(m_pRangePointsDlg->m_dLowUnity);
				m_pRule->setHighUnity(m_pRangePointsDlg->m_dHighUnity);
				m_pRule->setHighCross(m_pRangePointsDlg->m_dHighCross);
			}


		}
		else if(m_pRule->getCurveType()==1)
		{
			if(m_pRangePointsDlg->m_dLowCross>= m_pRangePointsDlg->m_dLowUnity)
			{
				MessageBox("Low Cross must be smaller than Low Unity.");
				return;
			}
			else 
			{
				m_pRule->setLowCross(m_pRangePointsDlg->m_dLowCross);
				m_pRule->setLowUnity(m_pRangePointsDlg->m_dLowUnity);
			}
		}
		else if(m_pRule->getCurveType()==2)
		{
			if(m_pRangePointsDlg->m_dHighUnity>=m_pRangePointsDlg->m_dHighCross)
			{
				MessageBox("High Unity must be smaller than High Cross.");

			}
			else
			{
				m_pRule->setHighUnity(m_pRangePointsDlg->m_dHighUnity);
				m_pRule->setHighCross(m_pRangePointsDlg->m_dHighCross);

			}


		}

		nCurrentPage=3;
		ShowPage(3);

	}


}

void CRangeWizard::OnFinish() 
{
	// TODO: Add your control notification handler code here
	
	m_pNameDlg->UpdateData(true);
	if(m_pNameDlg->m_strRuleName=="")
	{
		MessageBox("The name should not be empty.");
		return;
	}
	else
	{
		m_pRule->setRuleName(m_pNameDlg->m_strRuleName);
	}

	CDialog::OnOK();
}



void CRangeWizard::ShowPage(UINT nPos)
{
	struct PAGELINK* pTemp=pPageLink;
	while(pTemp)
	{
		if (pTemp->nNum==nPos)
		{
			pTemp->pDialog->ShowWindow(SW_SHOW);
		}
		else
			pTemp->pDialog->ShowWindow(SW_HIDE);
		pTemp=pTemp->Next;
	}

	if(nPos==4)
	{
		m_pNameDlg->m_strRuleName = m_pRule->getLayerName();
		m_pNameDlg->UpdateData(false);
	}

	if (nPos>=nPageCount)  //last page
	{
		nCurrentPage=nPageCount;
		SetWizButton(2);
		return;
	}
	if (nPos<=1) //first page
	{
		nCurrentPage=1;
		SetWizButton(0);
		return;
	}
	//middle
	SetWizButton(1);
}

void CRangeWizard::SetWizButton(UINT uFlag)
{
	GetDlgItem(IDC_CANCEL)->EnableWindow(TRUE);
	GetDlgItem(IDC_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_NEXT)->EnableWindow(TRUE);
	GetDlgItem(IDC_FINISH)->EnableWindow(FALSE);
	switch(uFlag)
	{
	case 0: //first step
		GetDlgItem(IDC_PREV)->EnableWindow(FALSE);
		break;
	case 1: //intermedia steps
		break;
	case 2://last step
		GetDlgItem(IDC_NEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_FINISH)->EnableWindow(TRUE);
		break;
	}
}

BOOL CRangeWizard::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect Rect1;
	GetWindowRect(&Rect1); 
	int nCaption = ::GetSystemMetrics(SM_CYCAPTION);
	int nXEdge = ::GetSystemMetrics(SM_CXEDGE);
	int nYEdge = ::GetSystemMetrics(SM_CYEDGE);
	CRect Rect2;
	GetDlgItem(IDC_POS)->GetWindowRect(&Rect2); 
	Rect1.top=Rect1.top+nCaption+nYEdge;
	Rect1.left=Rect1.left+2*nXEdge;

	rectPage.top=Rect2.top-Rect1.top;
	rectPage.left=Rect2.left-Rect1.left;
	rectPage.bottom=Rect2.bottom-Rect1.top;
	rectPage.right=Rect2.right-Rect1.left;

	m_pNameDlg= new CWizardRuleName();
	//m_pLayerDlg = new CWizardChooseLayer();
	m_pCurveTypeDlg = new CWizardCurveType();
	m_pRangePointsDlg = new CWizardRangePoints();


	//AddPage(m_pLayerDlg , IDD_WIZARD_CHOOSE_LAYER);
	AddPage(m_pCurveTypeDlg, IDD_WIZARD_CURVETYPE);
	AddPage(m_pRangePointsDlg, IDD_WIZARD_RANGE_KEY_POINTS);
	AddPage(m_pNameDlg, IDD_WIZARD_RULE_NAME);


	ShowPage(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRangeWizard::OnDestroy() 
{
	CDialog::OnDestroy();
	struct PAGELINK* pTemp = pPageLink;
	while(pTemp)
	{
		struct PAGELINK* pNextTemp = pTemp->Next;
		pTemp->pDialog->DestroyWindow();
		delete pTemp->pDialog;
		delete pTemp;
		pTemp = pNextTemp;
	}

}



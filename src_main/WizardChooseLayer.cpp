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
// WizardChooseLayer.cpp
//

#include "stdafx.h"
#include "SoLIM.h"
#include "WizardChooseLayer.h"
#include ".\wizardchooselayer.h"
#include "MainFrm.h"
#include  "SoLIMDoc.h"
#include "FreehandWizard.h"
#include "EnumWizard.h" 


IMPLEMENT_DYNAMIC(CWizardChooseLayer, CDialog)
CWizardChooseLayer::CWizardChooseLayer(int iRuleType, CWnd* pParent /*=NULL*/)
: CDialog(CWizardChooseLayer::IDD, pParent)
, m_bAttachLayer(TRUE)
{
	m_iRuleType = iRuleType;
}

CWizardChooseLayer::~CWizardChooseLayer()
{
}

void CWizardChooseLayer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
}



BEGIN_MESSAGE_MAP(CWizardChooseLayer, CDialog)
	ON_BN_CLICKED(IDC_RADIO_ATTACH_LAYER, OnBnClickedRadioAttachLayer)
	ON_BN_CLICKED(IDC_RADIO_SKIP,OnBnClickedRadioSkip)
END_MESSAGE_MAP()



BOOL CWizardChooseLayer::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	int iItemCount = m_comboLayer.GetCount();
	for(int i =0; i<iItemCount;i++)
	{
		m_comboLayer.DeleteString(0);
	}
	int iCurLayer = -1;
	for(i = 0; i<pDoc->m_pProjectEdit->m_pEnvDatabase->getLayerNum(); i++)
	{
		CString strLayerName;
		strLayerName = pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(i)->getLayerName();
		m_comboLayer.AddString(strLayerName);
	} 
	//m_chkAttachLayer.SetCheck(true);	
	return TRUE;  // return TRUE unless you set the focus to a control
}




void CWizardChooseLayer::OnBnClickedRadioAttachLayer()
{
	m_bAttachLayer = true;

	m_comboLayer.EnableWindow(m_bAttachLayer);
	if(m_iRuleType == 0)

	{
		((CRangeWizard *)(GetParent()))->SetWizButton(1);
	}



	if(m_iRuleType==1)
	{
	
		if(m_bAttachLayer)
			((CFreehandWizard *)(GetParent()))->SetWizButton(2);
		else
			((CFreehandWizard *)(GetParent()))->SetWizButton(1);
	}

		if(m_iRuleType == 2)

	{
		((CWordWizard *)(GetParent()))->SetWizButton(1);
	}

		if(m_iRuleType == 3)

	{
		((CPointWizard *)(GetParent()))->SetWizButton(1);
	}





	if(m_iRuleType==4)
	{
		if(m_bAttachLayer)
			((CEnumWizard *)(GetParent()))->SetWizButton(2);
		else
			((CEnumWizard *)(GetParent()))->SetWizButton(1);
	}
}

void CWizardChooseLayer::OnBnClickedRadioSkip()
{
	m_bAttachLayer = false;

	m_comboLayer.EnableWindow(m_bAttachLayer);

		if(m_iRuleType == 0)

	{
		((CRangeWizard *)(GetParent()))->SetWizButton(1);
	}



	if(m_iRuleType==1)
	{
		if(m_bAttachLayer)
			((CFreehandWizard *)(GetParent()))->SetWizButton(2);
		else
			((CFreehandWizard *)(GetParent()))->SetWizButton(1);
	}

	
		if(m_iRuleType == 2)

	{
		((CWordWizard *)(GetParent()))->SetWizButton(1);
	}

		if(m_iRuleType == 3)

	{
		((CPointWizard *)(GetParent()))->SetWizButton(1);
	}

	if(m_iRuleType==4)
	{
		if(m_bAttachLayer)
			((CEnumWizard *)(GetParent()))->SetWizButton(2);
		else
			((CEnumWizard *)(GetParent()))->SetWizButton(1);
	}
}

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
// WizardWordLib.cpp 
//

#include "stdafx.h"
#include "SoLIM.h"
#include "WizardWordLib.h"
#include ".\wizardwordlib.h"
#include "MainFrm.h"
#include "SoLIMDoc.h"



IMPLEMENT_DYNAMIC(CWizardWordLib, CDialog)
CWizardWordLib::CWizardWordLib(CWnd* pParent /*=NULL*/)
: CDialog(CWizardWordLib::IDD, pParent)
{
	pWordRule = NULL;
}

CWizardWordLib::~CWizardWordLib()
{
}

void CWizardWordLib::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_EDIT_LIBFILENAME,m_editLibFileName);
	DDX_Control(pDX,IDC_COMBO_CURVENAME,m_comboCurveName);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_LIBFILE, m_buttonChooseLibFile);
	DDX_Control(pDX, IDC_STATIC_LIBNAME, m_staticLibFile);
	DDX_Control(pDX, IDC_COMBO_TERRAIN_ATTRI, m_comboEnvAttri);
}


BEGIN_MESSAGE_MAP(CWizardWordLib, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_LIBFILE, OnBnClickedButtonBrowseLibfile)
	ON_CBN_SELCHANGE(IDC_COMBO_CURVENAME, OnCbnSelchangeComboCurvename)
	ON_CBN_SELCHANGE(IDC_COMBO_TERRAIN_ATTRI, OnCbnSelchangeComboEnvAttri)
END_MESSAGE_MAP()



void CWizardWordLib::OnBnClickedButtonBrowseLibfile()
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
		pWordRule->setLibFileName(strNewFilePath);
		pWordRule->LoadWordLib();
		CString strPhrase = pWordRule->getWordLib()->getWordCurve(0)->getPhrase();        
		pWordRule->setPhrase(strPhrase);
		CString strEnvAttri = pWordRule->getWordLib()->getWordCurve(0)->getEnvAttri();        
		pWordRule->setEnvAttri(strEnvAttri);
		int iEnvAttriCount = m_comboEnvAttri.GetCount();
		for(int i =0; i<iEnvAttriCount;i++)
		{
			m_comboEnvAttri.DeleteString(0);
		}

		int iCurEnvAttri = -1;
		CString strCurEnvAttri;

		if(pWordRule->getWordLib()!=NULL)
		{
			for(int i=0;i<pWordRule->getWordLib()->getEnvAttriNum();i++)
			{	
				CString strEnvAttri = pWordRule->getWordLib()->getEnvAttri(i);
				m_comboEnvAttri.AddString(strEnvAttri);
				if(pWordRule->getEnvAttri()==strEnvAttri)
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

		if(pWordRule->getWordLib()!=NULL)
		{
			for(int i=0;i<pWordRule->getWordLib()->getWordCurveNum();i++)
			{	
				CString strPhrase = pWordRule->getWordLib()->getWordCurve(i)->getPhrase();
				if(pWordRule->getWordLib()->getWordCurve(i)->getEnvAttri() == strCurEnvAttri)
				{
					m_comboCurveName.AddString(strPhrase);
					iPhraseNum++;
					if(pWordRule->getPhrase()==strPhrase&&pWordRule->getEnvAttri()==strCurEnvAttri)
						iCurPhrase = iPhraseNum-1;
				}

			}
			if(iCurPhrase !=-1)
				m_comboCurveName.SetCurSel(iCurPhrase);	
		}


		pWordRule->setCurGraphMin(pWordRule->ComputeRuleGraphMin());
		pWordRule->setCurGraphMax(pWordRule->ComputeRuleGraphMax());
		pWordRule->setDispMin(pWordRule->getCurGraphMin());
		pWordRule->setDispMax(pWordRule->getCurGraphMax());

	}
}

void CWizardWordLib::OnCbnSelchangeComboCurvename()
{	
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	int iCurSel = m_comboCurveName.GetCurSel();
	CString str;
	m_comboCurveName.GetLBText(iCurSel, str);
	pWordRule->setPhrase(str);

	pWordRule->setCurGraphMin(pWordRule->ComputeRuleGraphMin());
	pWordRule->setCurGraphMax(pWordRule->ComputeRuleGraphMax());
	pWordRule->setDispMin(pWordRule->getCurGraphMin());
	pWordRule->setDispMax(pWordRule->getCurGraphMax());


}

void CWizardWordLib::OnCbnSelchangeComboEnvAttri()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)pFrame->GetActiveDocument();

	int iCurSel = m_comboEnvAttri.GetCurSel();
	CString strCurEnvAttri;
	m_comboEnvAttri.GetLBText(iCurSel, strCurEnvAttri);
	pWordRule->setEnvAttri(strCurEnvAttri);
	int iItemCount = m_comboCurveName.GetCount();
	for(int i =0; i<iItemCount;i++)
	{
		m_comboCurveName.DeleteString(0);
	}
	int iPhraseNum = 0;
	int iCurPhrase = -1;

	if(pWordRule->getWordLib()!=NULL)
	{
		for(int i=0;i<pWordRule->getWordLib()->getWordCurveNum();i++)
		{	
			CString strPhrase = pWordRule->getWordLib()->getWordCurve(i)->getPhrase();
			if(pWordRule->getWordLib()->getWordCurve(i)->getEnvAttri() == strCurEnvAttri)
			{
				m_comboCurveName.AddString(strPhrase);
			}

		}
		m_comboCurveName.SetCurSel(0);	
		CString strCurPhrase;
		m_comboCurveName.GetLBText(0, strCurPhrase);
		pWordRule->setPhrase(strCurPhrase);

	}


	pWordRule->setCurGraphMin(pWordRule->ComputeRuleGraphMin());
	pWordRule->setCurGraphMax(pWordRule->ComputeRuleGraphMax());
	pWordRule->setDispMin(pWordRule->getCurGraphMin());
	pWordRule->setDispMax(pWordRule->getCurGraphMax());

}

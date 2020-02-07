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
// TypeInferPage.cpp : 实现文件
//

#include "stdafx.h"
#include "SoLIM.h"
#include "TypeInferPage.h"

#include "MainFrm.h"
#include "SoLIMDoc.h"


// CTypeInferPage 对话框

IMPLEMENT_DYNAMIC(CTypeInferPage, CDialog)
CTypeInferPage::CTypeInferPage(CWnd* pParent /*=NULL*/)
	: CDialog(CTypeInferPage::IDD, pParent)
{
}

CTypeInferPage::~CTypeInferPage()
{
}

void CTypeInferPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	
	DDX_Control(pDX, IDC_LIST_ALL_TYPE, listAllType);

	DDX_Control(pDX, IDC_COMBO_TYPE_FIELD, comboTypeField);
	DDX_CBString(pDX, IDC_COMBO_TYPE_FIELD, typeFieldName);


	DDX_Text(pDX, IDC_EDIT_MEMBERSHIP, m_strMaxSimiFolder);

}


BEGIN_MESSAGE_MAP(CTypeInferPage, CDialog)	
	
	ON_BN_CLICKED(IDC_BUTTON_SAVE_TYPERESULT, OnBnClickedButtonSaveTyperesult)	
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_FIELD, OnCbnSelchangeComboTypeField)
END_MESSAGE_MAP()


// CTypeInferPage 消息处理程序


void CTypeInferPage::OnBnClickedButtonSaveTyperesult()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
	
	CGetDirDlg dirDlg;   
	m_strMaxSimiFolder = dirDlg.GetDirectory(this, pDoc->m_strProjectDir, "Choose a directory to save the membership files:");;   
	UpdateData(FALSE);
	
}



void CTypeInferPage::OnCbnSelchangeComboTypeField()
{

		CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());


	UpdateData();
	// TODO: Add your control notification handler code here
	//added by Fei
	while (listAllType.GetCount() > 0)
		listAllType.DeleteString(0);


	//const char* strType = DBFReadStringAttribute(pDoc->m_pProjectCaseEdit->hDBF, 0, DBFGetFieldIndex(pDoc->m_pProjectCaseEdit->hDBF, typeFieldName.GetBuffer()));
	//CString ex(strType);


	CString strSel;
	comboTypeField.GetLBText(comboTypeField.GetCurSel(), strSel);




	pDoc->m_pProjectCaseEdit-> m_strCurTypeField = strSel;

	//typeFieldIndex = DBFGetFieldIndex(pDoc->m_pProjectCaseEdit->hDBF, strSel.GetBuffer());

	typeFieldIndex = pDoc->m_pProjectCaseEdit->hCSV->GetVariableIndex(strSel.GetBuffer());

	//int nRec = DBFGetRecordCount( pDoc->m_pProjectCaseEdit->hDBF );

	int nRec = pDoc->m_pProjectCaseEdit->hCSV->GetNumberOfSamples(0) - 1;




	//only add new types to the list
	for (int k = 0; k < nRec; ++k)
	{
		CString newName;		
		pDoc->m_pProjectCaseEdit->hCSV->GetData(typeFieldIndex, k, newName.GetBuffer());//vecSampleData[k][typeFieldIndex];
		bool bExists = false;
		for(int m = 0; m < listAllType.GetCount(); m++)
		{
			CString tempName;
			listAllType.GetText(m,tempName);

			if(newName.Trim() == tempName.Trim())
			{
				bExists = true;
				break;
			}
		}
		if(bExists == false)  //this is totally a new type
			listAllType.AddString(newName);
	}



}




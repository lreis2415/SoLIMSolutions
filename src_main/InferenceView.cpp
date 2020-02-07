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
// InferenceView.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "InferenceView.h"
#include ".\inferenceview.h"
#include "MainFrm.h"
#include "SoLIMDoc.h"
#include "FileManager.h"

// CInferenceView

IMPLEMENT_DYNCREATE(CInferenceView, CFormView)

CInferenceView::CInferenceView()
: CFormView(CInferenceView::IDD)
, m_iMaskValue(0)
, m_bCreateCheckFile(FALSE)
, m_bUseMask(FALSE)
, m_strMaskFileName(_T(""))
, m_strInferenceResultDir(_T(""))
, m_dFinalResolution(0)
{
}

CInferenceView::~CInferenceView()
{
}

void CInferenceView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FRAME, m_staticFrame);
	DDX_Control(pDX, IDC_CHECK_CHECK_FILE, m_chkCheckFile);
	DDX_Control(pDX, IDC_CHECK_USE_MASK, m_chkUseMask);
	DDX_Control(pDX, IDC_EDIT_MASK, m_editMaskFile);
	DDX_Control(pDX, IDC_BTN_BROWSE_MASK, m_btnBrowseMask);
	DDX_Control(pDX, IDC_STATIC_MASK_VALUE, m_staticMaskValue);
	DDX_Control(pDX, IDC_EDIT_MASK_VALUE, m_editMaskValue);
	DDX_Control(pDX, IDC_STATIC_MASK_FRAME, m_frameMask);
	DDX_Control(pDX, IDC_STATIC_RESULT_DIR, m_staticResultDir);
	DDX_Control(pDX, IDC_EDIT_RESULT_DIR, m_editResultDir);
	DDX_Control(pDX, IDC_BTN_RESULT_DIR, m_btnResultDir);
	DDX_Control(pDX, IDC_LIST_CHOOSE_SOIL, m_listChooseSoil);
	DDX_Control(pDX, IDC_BTN_RUN_INFERENCE, m_btnRunInference);
	DDX_Check(pDX, IDC_CHECK_CHECK_FILE, m_bCreateCheckFile);
	DDX_Check(pDX, IDC_CHECK_USE_MASK, m_bUseMask);
	DDX_Control(pDX, IDC_EDIT_MASK_VALUE, m_editMaskValue);
	DDX_Text(pDX, IDC_EDIT_MASK_VALUE, m_iMaskValue);
	DDX_Text(pDX, IDC_EDIT_MASK, m_strMaskFileName);
	DDX_Text(pDX, IDC_EDIT_RESULT_DIR, m_strInferenceResultDir);
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
	DDX_Control(pDX, IDC_STATIC_SOIL_TO_INFER, m_frameSoilToInfer);
	DDX_Control(pDX, IDC_STATIC_INFER_RESULT_FRAME, m_frameInferResult);
	DDX_Control(pDX, IDC_STATIC_MASK_VALUE_INSTRUCTION, m_staticMaskValueInstruction);
	DDX_Control(pDX, IDC_STATIC_MASK_FILE, m_staticMaskFile);
	DDX_Text(pDX, IDC_EDIT_FINAL_RES, m_dFinalResolution);
	DDX_Control(pDX, IDC_STATIC_RESOLUTION, m_staticResolution);
	DDX_Control(pDX, IDC_EDIT_FINAL_RES, m_editResolution);
	
	DDX_Control(pDX, IDC_STATIC_RESULT_TYPE, m_staticSaveType);
	DDX_Control(pDX, IDC_COMBO_SAVE_TYPE, m_comboSaveType);
}

BEGIN_MESSAGE_MAP(CInferenceView, CFormView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_RESULT_DIR, OnBnClickedBtnResultDir)
	ON_BN_CLICKED(IDC_BNT_CHOOSE_MASK, OnBnClickedBntChooseMask)
	ON_BN_CLICKED(IDC_CHECK_CHECK_FILE, OnBnClickedCheckCheckFile)
	ON_BN_CLICKED(IDC_CHECK_USE_MASK, OnBnClickedCheckUseMask)
	ON_BN_CLICKED(IDC_BTN_RUN_INFERENCE, OnBnClickedBtnRunInference)
END_MESSAGE_MAP()


// CInferenceView diagnostics

#ifdef _DEBUG
void CInferenceView::AssertValid() const
{
	CFormView::AssertValid();
}

void CInferenceView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CInferenceView message handlers

void CInferenceView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if(m_staticFrame.GetSafeHwnd())
	{
		CRect rect(0,-5,cx,cy);
		m_staticFrame.MoveWindow(&rect);
	}

	if(m_staticResolution.GetSafeHwnd())
	{
		CRect rect(cx/10, 0.5 *cy/10, cx/10 + 200 , 0.5*cy/10 + 20);
		m_staticResolution.MoveWindow(&rect);
	}

	
	 if(m_editResolution.GetSafeHwnd())
	{
		CRect rect(cx/10 + 220, 0.5*cy/10, cx/10 + 320, 0.5*cy/10 + 20);
		m_editResolution.MoveWindow(&rect);
	}

	if(m_frameSoilToInfer.GetSafeHwnd())
	{
		CRect rect(cx/10,0.9*cy/10,9*cx/10,3.5*cy/10);
		m_frameSoilToInfer.MoveWindow(&rect);
	}
	if( m_listChooseSoil.GetSafeHwnd())
	{
		CRect rect(1.1*cx/10,1.3*cy/10,8.9*cx/10,3.4*cy/10);
		m_listChooseSoil.MoveWindow(&rect);
	}
	if(m_frameMask.GetSafeHwnd())
	{
		CRect rect(cx/10,4*cy/10,9*cx/10,6*cy/10);
		m_frameMask.MoveWindow(&rect);
	}
	if(m_chkUseMask.GetSafeHwnd())
	{
		CRect rect(2*cx/10,4.5*cy/10,4*cx/10,4.5*cy/10+20);
		m_chkUseMask.MoveWindow(&rect);
	}

	if(m_staticMaskFile.GetSafeHwnd())
	{
		CRect rect(1.5*cx/10,5*cy/10,3*cx/10,5*cy/10+20);
		m_staticMaskFile.MoveWindow(&rect);
	}
	if(m_editMaskFile.GetSafeHwnd())
	{
		CRect rect(3*cx/10+10,5*cy/10,8*cx/10,5*cy/10+20);
		m_editMaskFile.MoveWindow(&rect);
	}
	if(m_btnBrowseMask.GetSafeHwnd())
	{
		CRect rect(8*cx/10+10,5*cy/10,9*cx/10-5,5*cy/10+20);
		m_btnBrowseMask.MoveWindow(&rect);
	}
	if(m_staticMaskValue.GetSafeHwnd())
	{
		CRect rect(1.5*cx/10,5.5*cy/10,3*cx/10,5.5*cy/10+20);
		m_staticMaskValue.MoveWindow(&rect);
	}
	if(m_editMaskValue.GetSafeHwnd())
	{
		CRect rect(3*cx/10+10,5.5*cy/10,3*cx/10+60,5.5*cy/10+20);
		m_editMaskValue.MoveWindow(&rect);
	}
	if(m_staticMaskValueInstruction.GetSafeHwnd())
	{
		CRect rect(4*cx/10,5.5*cy/10,8.8*cx/10,5.5*cy/10+20);
		m_staticMaskValueInstruction.MoveWindow(&rect);
	}
	if(m_chkCheckFile.GetSafeHwnd())
	{
		CRect rect(cx/10,6*cy/10+30,8*cx/10,6*cy/10+50);
		m_chkCheckFile.MoveWindow(&rect);
	}	
	if(m_frameInferResult.GetSafeHwnd())
	{
		CRect rect(cx/10,7*cy/10,9*cx/10,9*cy/10);
		m_frameInferResult.MoveWindow(&rect);
	}
	if(m_staticResultDir.GetSafeHwnd())
	{
		CRect rect(1.5*cx/10,7.5*cy/10,3*cx/10,7.5*cy/10+20);
		m_staticResultDir.MoveWindow(&rect);
	}
	if(m_editResultDir.GetSafeHwnd())
	{
		CRect rect(3*cx/10+10,7.5*cy/10,8*cx/10,7.5*cy/10+20);
		m_editResultDir.MoveWindow(&rect);
	}
	if(	m_btnResultDir.GetSafeHwnd())
	{
		CRect rect(8*cx/10+5,7.5*cy/10,9*cx/10-5,7.5*cy/10+20);
		m_btnResultDir.MoveWindow(&rect);
	}

	// save as type
	if(m_staticSaveType.GetSafeHwnd())
	{
		CRect rect(1.5*cx/10,8*cy/10,3*cx/10,8.5*cy/10+20);
		m_staticSaveType.MoveWindow(&rect);
	}
	if(m_comboSaveType.GetSafeHwnd())
	{
		CRect rect(3*cx/10+10,8*cy/10,6*cx/10,8.5*cy/10+50);
		m_comboSaveType.MoveWindow(&rect);
	}

	if(m_btnRunInference.GetSafeHwnd())
	{
		CRect rect(cx/10,9.3*cy/10,3*cx/10,9.3*cy/10+30);
		m_btnRunInference.MoveWindow(&rect);
	} 
	if(m_ProgressBar.GetSafeHwnd())
	{
		CRect rect(3*cx/10+10,9.3*cy/10,9*cx/10,9.3*cy/10+20);
		m_ProgressBar.MoveWindow(&rect);
	}

}

void CInferenceView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());

	pDoc->setCurViewType(2);
	pFrame->m_leftBar.ExpandNode(2);

	CRect rect;
	GetClientRect(&rect);

	int cx = rect.right;
	int cy = rect.bottom;

	if(m_staticFrame.GetSafeHwnd())
	{
		m_staticFrame.MoveWindow(&rect);
	}

    if(m_staticResolution.GetSafeHwnd())
	{
		CRect rect(cx/10, 0.5 *cy/10, cx/10 + 200 , 0.5*cy/10 + 20);
		m_staticResolution.MoveWindow(&rect);
	}

	
	 if(m_editResolution.GetSafeHwnd())
	{
		CRect rect(cx/10 + 220, 0.5*cy/10, cx/10 + 320, 0.5*cy/10 + 20);
		m_editResolution.MoveWindow(&rect);
	}

	if(m_frameSoilToInfer.GetSafeHwnd())
	{
		CRect rect(cx/10,0.9*cy/10,9*cx/10,3.5*cy/10);
		m_frameSoilToInfer.MoveWindow(&rect);
	}
	if( m_listChooseSoil.GetSafeHwnd())
	{
		CRect rect(1.1*cx/10,1.3*cy/10,8.9*cx/10,3.4*cy/10);
		m_listChooseSoil.MoveWindow(&rect);
	}
	if(m_frameMask.GetSafeHwnd())
	{
		CRect rect(cx/10,4*cy/10,9*cx/10,6*cy/10);
		m_frameMask.MoveWindow(&rect);
	}
	if(m_chkUseMask.GetSafeHwnd())
	{
		CRect rect(2*cx/10,4.5*cy/10,4*cx/10,4.5*cy/10+20);
		m_chkUseMask.MoveWindow(&rect);
	}

	if(m_staticMaskFile.GetSafeHwnd())
	{
		CRect rect(1.5*cx/10,5*cy/10,3*cx/10,5*cy/10+20);
		m_staticMaskFile.MoveWindow(&rect);
	}
	if(m_editMaskFile.GetSafeHwnd())
	{
		CRect rect(3*cx/10+10,5*cy/10,8*cx/10,5*cy/10+20);
		m_editMaskFile.MoveWindow(&rect);
	}
	if(m_btnBrowseMask.GetSafeHwnd())
	{
		CRect rect(8*cx/10+10,5*cy/10,9*cx/10-5,5*cy/10+20);
		m_btnBrowseMask.MoveWindow(&rect);
	}
	if(m_staticMaskValue.GetSafeHwnd())
	{
		CRect rect(1.5*cx/10,5.5*cy/10,3*cx/10,5.5*cy/10+20);
		m_staticMaskValue.MoveWindow(&rect);
	}
	if(m_editMaskValue.GetSafeHwnd())
	{
		CRect rect(3*cx/10+10,5.5*cy/10,3*cx/10+60,5.5*cy/10+20);
		m_editMaskValue.MoveWindow(&rect);
	}
	if(m_staticMaskValueInstruction.GetSafeHwnd())
	{
		CRect rect(4*cx/10,5.5*cy/10,8.8*cx/10,5.5*cy/10+20);
		m_staticMaskValueInstruction.MoveWindow(&rect);
	}
	if(m_chkCheckFile.GetSafeHwnd())
	{
		CRect rect(cx/10,6*cy/10+30,8*cx/10,6*cy/10+50);
		m_chkCheckFile.MoveWindow(&rect);
	}	
	if(m_frameInferResult.GetSafeHwnd())
	{
		CRect rect(cx/10,7*cy/10,9*cx/10,9*cy/10);
		m_frameInferResult.MoveWindow(&rect);
	}
	if(m_staticResultDir.GetSafeHwnd())
	{
		CRect rect(1.5*cx/10,7.5*cy/10,3*cx/10,7.5*cy/10+20);
		m_staticResultDir.MoveWindow(&rect);
	}
	if(m_editResultDir.GetSafeHwnd())
	{
		CRect rect(3*cx/10+10,7.5*cy/10,8*cx/10,7.5*cy/10+20);
		m_editResultDir.MoveWindow(&rect);
	}
	if(	m_btnResultDir.GetSafeHwnd())
	{
		CRect rect(8*cx/10+5,7.5*cy/10,9*cx/10-5,7.5*cy/10+20);
		m_btnResultDir.MoveWindow(&rect);
	}

	// save as type
	if(m_staticSaveType.GetSafeHwnd())
	{
		CRect rect(1.5*cx/10,8*cy/10,3*cx/10,8.5*cy/10+20);
		m_staticSaveType.MoveWindow(&rect);
	}
	if(m_comboSaveType.GetSafeHwnd())
	{
		CRect rect(3*cx/10+10,8*cy/10,6*cx/10,8.5*cy/10+50);
		m_comboSaveType.MoveWindow(&rect);
	}

	if(m_btnRunInference.GetSafeHwnd())
	{
		CRect rect(cx/10,9.3*cy/10,3*cx/10,9.3*cy/10+30);
		m_btnRunInference.MoveWindow(&rect);
	} 
	if(m_ProgressBar.GetSafeHwnd())
	{
		CRect rect(3*cx/10+10,9.3*cy/10,9*cx/10,9.3*cy/10+20);
		m_ProgressBar.MoveWindow(&rect);
	}

	// init comboBox
	m_comboSaveType.AddString("3DR  (*.3dr)");
	m_comboSaveType.AddString("SAGA GIS Binary format  (*.sdat)");
	//m_comboSaveType.AddString("Arc/Info ASCII Grid  (*.acs)");
	m_comboSaveType.AddString("Erdas Imagine  (*.img)");
	m_comboSaveType.AddString("TIFF / BigTIFF / GeoTIFF  (*.tif)");
	m_comboSaveType.SetCurSel(0);

	if(pDoc->m_pProjectEdit!=NULL)
	{
		EnableView(true);
		FillInferenceView(pDoc->m_pProjectEdit->m_pInference);
	}
	else
	{
		EnableView(false);
	}

}

void CInferenceView::FillInferenceView(CInference *pInference)
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();	
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());


	this->m_dFinalResolution = pDoc->m_pProjectEdit->m_pEnvDatabase->getLayer(0)->m_pTheData->getCellSize();


	m_bCreateCheckFile = pInference->getCreateCheckFile();
	m_bUseMask= pInference->getUseMask();
	m_strMaskFileName = pInference->getMaskFile();
	m_iMaskValue = pInference->getMaskValue();
	if(m_bUseMask)
	{
		pDoc->m_pProjectEdit->m_pInference->setUseMask(true);
		if(m_editMaskFile.GetSafeHwnd())
		{
			m_editMaskFile.EnableWindow(true);
		}
		if(m_btnBrowseMask.GetSafeHwnd())
		{
			m_btnBrowseMask.EnableWindow(true);
		}
		if(m_editMaskValue.GetSafeHwnd())
		{
			m_editMaskValue.EnableWindow(true);
		}

	}
	else
	{
		pDoc->m_pProjectEdit->m_pInference->setUseMask(false);
		if(m_editMaskFile.GetSafeHwnd())
		{
			m_editMaskFile.EnableWindow(false);
		}
		if(m_btnBrowseMask.GetSafeHwnd())
		{
			m_btnBrowseMask.EnableWindow(false);
		}
		if(m_editMaskValue.GetSafeHwnd())
		{
			m_editMaskValue.EnableWindow(false);
		}
	}


	//set check for soil types
	for(int i=0; i<pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilTypeNum();i++)
	{
		CString strSoilName = pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->getSoilTypeName();
		m_listChooseSoil.AddString(strSoilName);
		if(pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->getIsUsed())
			m_listChooseSoil.SetCheck(i,1);
		else
			m_listChooseSoil.SetCheck(i,0);
	}
	m_strInferenceResultDir = pInference->getResultDir();
	UpdateData(false);


}

void CInferenceView::OnBnClickedBtnResultDir()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();	
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());
	CString str = m_getDirDlg.GetDirectory(this, "", "Choose Result Directory :");
	pDoc->m_pProjectEdit->m_pInference->setResultDir(str);
	m_strInferenceResultDir = str;
	UpdateData(false);
}

void CInferenceView::OnBnClickedBntChooseMask()
{
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());
	UpdateData(TRUE);
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK)
	{
		CString str = m_ldFile.GetPathName();
		pDoc->m_pProjectEdit->m_pInference->setMaskFile(str);
		this->m_strMaskFileName = str;
		UpdateData(FALSE);
	}
}

void CInferenceView::OnBnClickedCheckCheckFile()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();	
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());
	m_bCreateCheckFile = !m_bCreateCheckFile;
	if(pDoc->m_pProjectEdit!=NULL)
	{
		if(m_bCreateCheckFile)
			pDoc->m_pProjectEdit->m_pInference->setCreateCheckFile(true);
		else
			pDoc->m_pProjectEdit->m_pInference->setCreateCheckFile(false);
	}

}

void CInferenceView::OnBnClickedCheckUseMask()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();	
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());
	m_bUseMask=!m_bUseMask;
	if(pDoc->m_pProjectEdit!=NULL)
	{
		if(m_bUseMask)
		{
			pDoc->m_pProjectEdit->m_pInference->setUseMask(true);
			if(m_editMaskFile.GetSafeHwnd())
			{
				m_editMaskFile.EnableWindow(true);
			}
			if(m_btnBrowseMask.GetSafeHwnd())
			{
				m_btnBrowseMask.EnableWindow(true);
			}
			if(m_editMaskValue.GetSafeHwnd())
			{
				m_editMaskValue.EnableWindow(true);
			}

		}
		else
		{
			pDoc->m_pProjectEdit->m_pInference->setUseMask(false);
			if(m_editMaskFile.GetSafeHwnd())
			{
				m_editMaskFile.EnableWindow(false);
			}
			if(m_btnBrowseMask.GetSafeHwnd())
			{
				m_btnBrowseMask.EnableWindow(false);
			}
			if(m_editMaskValue.GetSafeHwnd())
			{
				m_editMaskValue.EnableWindow(false);
			}
		}
	}


}

void CInferenceView::OnBnClickedBtnRunInference()
{
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());
	UpdateData(true);

	if(pDoc->m_pProjectEdit!=NULL)
	{
		if(this->m_dFinalResolution <= 0)
		{
				MessageBox("The resolution you specified should be greater than 0.");
				return;
		}
		else
			pDoc->m_pProjectEdit->m_pInference->setCellSize(this->m_dFinalResolution);

		if(m_bUseMask)
		{
			if(this->m_strMaskFileName == "")
			{
				MessageBox("You must specify mask file.");
				return;
			}
			
			CString str;

			
		    m_editMaskValue.GetWindowText(str);
			if(!IsInteger(str))
			{
				MessageBox("You must input an integer for masking value.");
				return;
			}
			else
			{
				pDoc->m_pProjectEdit->m_pInference->setMaskValue(atoi(str));
			}
	


		}
	
		int num=pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilTypeNum();
		int state;
		bool bHasSoil=false;
		for(int i=0;i<num;i++)
		{


			state=m_listChooseSoil.GetCheck(i);
			if(state==0)
			{
				pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->setIsUsed(false);
			}
			else
			{
				pDoc->m_pProjectEdit->m_pKnowledgeBase->getSoilType(i)->setIsUsed(true);
				bHasSoil=true;
			} 
		}

		if(bHasSoil==false)
		{
			AfxMessageBox("No soil is selected.");
			return;
		}

		if(m_strInferenceResultDir=="")
		{	
			MessageBox("Please specify result directory.");
			return;
		}



		//if the specified directory does not exist, this function will create it 
		CreateDirectory(pDoc->m_pProjectEdit->m_pInference->getResultDir(), NULL);

        //run inference
		m_ProgressBar.SetRange(1, 100);        

		pDoc->m_pProjectEdit->m_pInference->setResultExt(GetOutputExt());
		pDoc->m_pProjectEdit->m_pInference->InferSoil();

		MessageBox("Inference is finished.");
		m_ProgressBar.SetPos(0);
	}
}

CString CInferenceView::GetOutputExt()
{
	int nIndex = m_comboSaveType.GetCurSel();
	CString selectedText;
	m_comboSaveType.GetLBText(nIndex, selectedText);

	CString ext = strlwr(strrchr(selectedText.GetBuffer(0), '.'));
	ext = ext.Left(ext.GetLength()-1); 
	return ext;
}


void CInferenceView::EnableView(bool bIsEnable)
{
	if(m_chkCheckFile.GetSafeHwnd())
	{
		m_chkCheckFile.EnableWindow(bIsEnable); 
	}
	if(m_chkUseMask.GetSafeHwnd())
	{
		m_chkUseMask.EnableWindow(bIsEnable); 
	}
	if(m_editMaskFile.GetSafeHwnd())
	{
		m_editMaskFile.EnableWindow(bIsEnable);
	}
	if(m_btnBrowseMask.GetSafeHwnd())
	{
		m_btnBrowseMask.EnableWindow(bIsEnable);
	}

	if(m_editMaskValue.GetSafeHwnd())
	{
		m_editMaskValue.EnableWindow(bIsEnable);
	}

	if(m_editResultDir.GetSafeHwnd())
	{
		m_editResultDir.EnableWindow(bIsEnable);
	}
	if(m_btnResultDir.GetSafeHwnd())
	{
		m_btnResultDir.EnableWindow(bIsEnable);
	}
	if(m_listChooseSoil.GetSafeHwnd())
	{
		m_listChooseSoil.EnableWindow(bIsEnable);
	}
	if(m_btnRunInference.GetSafeHwnd())
	{
		m_btnRunInference.EnableWindow(bIsEnable);
	}

}

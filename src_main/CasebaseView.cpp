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
// KnowledgeView.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "CasebaseView.h"
#include ".\casebaseview.h"
#include "MainFrm.h"
#include "SoLIMDoc.h"

#include "FieldSample.h"


// CCasebaseView

IMPLEMENT_DYNCREATE(CCasebaseView, CFormView)

CCasebaseView::CCasebaseView()
: CFormView(CCasebaseView::IDD)
, xFieldName(_T(""))
, yFieldName(_T(""))

{
	m_numOfCol = 0;

	m_iCurView = 0;
}

CCasebaseView::~CCasebaseView()
{

}

void CCasebaseView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_Sample, m_listctrlSampleData);
	DDX_Control(pDX, IDC_COMBO_FIELD_X, comboFieldX);
	DDX_Control(pDX, IDC_COMBO_FIELD_Y, comboFieldY);
	DDX_Control(pDX, IDC_COMBO_FIELD_ID, comboFieldID);
	DDX_CBString(pDX, IDC_COMBO_FIELD_X, xFieldName);
	DDX_CBString(pDX, IDC_COMBO_FIELD_Y, yFieldName);
	DDX_CBString(pDX, IDC_COMBO_FIELD_ID, idFieldName);



	DDX_Control(pDX, IDC_BUTTON_GET_SAMPLE, m_btnImport);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_SAPMLE_LIST, m_btnClear);
	DDX_Control(pDX, IDC_BUTTON_SWITCH_BETWEEN_TWO_VIEWS, m_btnSwitchView);


	DDX_Control(pDX, IDC_OK, m_btnSubmit);
	DDX_Control(pDX, IDC_STATIC_X, m_staticX);
	DDX_Control(pDX, IDC_STATIC_Y, m_staticY);
	DDX_Control(pDX, IDC_STATIC_ID, m_staticID);
}

BEGIN_MESSAGE_MAP(CCasebaseView, CFormView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_GET_SAMPLE, OnBnClickedButtonGetSample)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_SAPMLE_LIST, OnBnClickedButtonClearSapmleList)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD_X, OnCbnSelchangeComboFieldX)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD_Y, OnCbnSelchangeComboFieldY)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD_ID, OnCbnSelchangeComboFieldID)
	ON_BN_CLICKED(IDC_OK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SWITCH_BETWEEN_TWO_VIEWS, OnBnClickedButtonSwitchBetweenTwoViews)
END_MESSAGE_MAP()


// CCasebaseView diagnostics

#ifdef _DEBUG
void CCasebaseView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCasebaseView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CCasebaseView message handlers

void CCasebaseView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());

	if(m_listctrlSampleData.GetSafeHwnd() )
	{

		if(m_iCurView == 0)
		{
			CRect rect(0.2*cx/10,1*cy/10,9.8*cx/10,8* cy/10);
			m_listctrlSampleData.MoveWindow(&rect);

			//if( pDoc->m_pProjectCaseEdit->hDBF!=NULL)
			if( pDoc->m_pProjectCaseEdit->hCSV!=NULL)
			{

				//int nFields = DBFGetFieldCount( pDoc->m_pProjectCaseEdit->hDBF );

				int nFields = pDoc->m_pProjectCaseEdit->hCSV->GetNumberOfVariables();


				if(nFields != 0)
				{
					for (int j = 0; j < nFields; ++j)
					{

						m_listctrlSampleData.SetColumnWidth(j, (0.96*cx - 80)/nFields);


					}
				}

			}
		}

		else 
		{
			CRect rect(0, 0, 0, 0);

			m_listctrlSampleData.MoveWindow(&rect);
		}
	}


	if(this->m_pImagePanel.GetSafeHwnd())
	{
		if(m_iCurView == 1)
		{
			CRect rect(0.2*cx/10,1*cy/10,9.8*cx/10,8* cy/10);

			m_pImagePanel.MoveWindow(&rect);
		}

		else 
		{
			CRect rect(0, 0, 0, 0);

			m_pImagePanel.MoveWindow(&rect);
		}

	}

	if(m_btnImport.GetSafeHwnd())
	{
		CRect rect(0.2*cx/10,0.1*cy/10 ,0.2*cx/10 + 170,0.1* cy/10 + 30);
		m_btnImport.MoveWindow(&rect);
	}

	if(m_btnClear.GetSafeHwnd())
	{
		CRect rect(0.2 *cx/10 + 200,0.1*cy/10 ,0.2*cx/10 + 370,0.1* cy/10 + 30);
		m_btnClear.MoveWindow(&rect);
	}

	if(this->m_btnSwitchView.GetSafeHwnd())
	{
		CRect rect(0.2*cx/10 + 400,0.1*cy/10 ,0.2*cx/10 + 650,0.1* cy/10 + 30);
		m_btnSwitchView.MoveWindow(&rect);
	}


	if(m_staticX.GetSafeHwnd())
	{
		CRect rect(cx/10,8*cy/10 + 30,cx/10 + 150,8*cy/10+60);
		m_staticX.MoveWindow(&rect);
	}


	if(m_staticX.GetSafeHwnd())
	{
		CRect rect(0.5* cx/10,8*cy/10 + 30,0.5*cx/10 + 100,8*cy/10+60);
		m_staticX.MoveWindow(&rect);
	}


	if(comboFieldX.GetSafeHwnd())
	{
		CRect rect(0.5* cx/10 + 110,8*cy/10 + 30,0.5*cx/10 + 210,8*cy/10+50);
		comboFieldX.MoveWindow(&rect);
	}

	if(m_staticY.GetSafeHwnd())
	{
		CRect rect(4.0*cx/10,8*cy/10 + 30,4.0*cx/10 + 100,8*cy/10+60);
		m_staticY.MoveWindow(&rect);
	}


	if(comboFieldY.GetSafeHwnd())
	{
		CRect rect(4.0*cx/10+110,8*cy/10 + 30,4.0*cx/10 + 220,8*cy/10+50);
		comboFieldY.MoveWindow(&rect);
	}

	if(m_staticID.GetSafeHwnd())
	{
		CRect rect(7.0*cx/10,8*cy/10 + 30,7.0*cx/10 + 100,8*cy/10+60);
		m_staticID.MoveWindow(&rect);
	}


	if(comboFieldID.GetSafeHwnd())
	{
		CRect rect(7.0*cx/10+110,8*cy/10 + 30,7.0*cx/10 + 220,8*cy/10+50);
		comboFieldID.MoveWindow(&rect);
	}


	if(m_btnSubmit.GetSafeHwnd())
	{
		CRect rect(4*cx/10,9*cy/10 + 20,4*cx/10 + 100,9*cy/10+60);
		m_btnSubmit.MoveWindow(&rect);
	}






}


void CCasebaseView::OnInitialUpdate()
{

	CFormView::OnInitialUpdate();

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());


	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();

	this->m_listctrlSampleData.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);


	this->m_listctrlSampleData.InsertColumn(1,CString("Sample"));
	this->m_listctrlSampleData.SetColumnWidth(0,80);




	CRect rect;
	GetClientRect(&rect);

	int cx = rect.right;
	int cy = rect.bottom;

	/*
	if(m_listctrlSampleData.GetSafeHwnd())
	{
	CRect rect(0.2*cx/10,0.2*cy/10,9.8*cx/10,7* cy/10);

	m_listctrlSampleData.MoveWindow(&rect);
	}*/

	if(m_listctrlSampleData.GetSafeHwnd() )
	{

		if(m_iCurView == 0)
		{
			CRect rect(0.2*cx/10,1*cy/10,9.8*cx/10,8* cy/10);
			m_listctrlSampleData.MoveWindow(&rect);
		}

		else 
		{
			CRect rect(0, 0, 0, 0);

			m_listctrlSampleData.MoveWindow(&rect);
		}
	}


	if(this->m_pImagePanel.GetSafeHwnd())
	{
		if(m_iCurView == 1)
		{
			CRect rect(0.2*cx/10,1*cy/10,9.8*cx/10,8* cy/10);

			m_pImagePanel.MoveWindow(&rect);
		}

		else 
		{
			CRect rect(0, 0, 0, 0);

			m_pImagePanel.MoveWindow(&rect);
		}

	}

	if(m_btnImport.GetSafeHwnd())
	{
		CRect rect(0.2*cx/10,0.1*cy/10 ,0.2*cx/10 + 170,0.1* cy/10 + 30);
		m_btnImport.MoveWindow(&rect);
	}

	if(m_btnClear.GetSafeHwnd())
	{
		CRect rect(0.2 *cx/10 + 200,0.1*cy/10 ,0.2*cx/10 + 370,0.1* cy/10 + 30);
		m_btnClear.MoveWindow(&rect);
	}

	if(this->m_btnSwitchView.GetSafeHwnd())
	{
		CRect rect(0.2*cx/10 + 400,0.1*cy/10 ,0.2*cx/10 + 650,0.1* cy/10 + 30);
		m_btnSwitchView.MoveWindow(&rect);
	}




	if(m_staticX.GetSafeHwnd())
	{
		CRect rect(0.5* cx/10,8*cy/10 + 30,0.5*cx/10 + 100,8*cy/10+60);
		m_staticX.MoveWindow(&rect);
	}


	if(comboFieldX.GetSafeHwnd())
	{
		CRect rect(0.5* cx/10 + 110,8*cy/10 + 30,0.5*cx/10 + 210,8*cy/10+50);
		comboFieldX.MoveWindow(&rect);
	}

	if(m_staticY.GetSafeHwnd())
	{
		CRect rect(4.0*cx/10,8*cy/10 + 30,4.0*cx/10 + 100,8*cy/10+60);
		m_staticY.MoveWindow(&rect);
	}


	if(comboFieldY.GetSafeHwnd())
	{
		CRect rect(4.0*cx/10+110,8*cy/10 + 30,4.0*cx/10 + 220,8*cy/10+50);
		comboFieldY.MoveWindow(&rect);
	}

	if(m_staticID.GetSafeHwnd())
	{
		CRect rect(7.0*cx/10,8*cy/10 + 30,7.0*cx/10 + 100,8*cy/10+60);
		m_staticID.MoveWindow(&rect);
	}


	if(comboFieldID.GetSafeHwnd())
	{
		CRect rect(7.0*cx/10+110,8*cy/10 + 30,7.0*cx/10 + 220,8*cy/10+50);
		comboFieldID.MoveWindow(&rect);
	}

	if(m_btnSubmit.GetSafeHwnd())
	{
		CRect rect(4*cx/10,9*cy/10 + 20,4*cx/10 + 100,9*cy/10+60);
		m_btnSubmit.MoveWindow(&rect);
	}



	CRect graphRect(0,0, 0, 0);
	m_pImagePanel.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,graphRect,this,1000);	

	m_pImagePanel.Invalidate();




	/*

	if(m_staticFrame.GetSafeHwnd())
	{
	m_staticFrame.MoveWindow(&rect);
	}

	if(m_norulePage.GetSafeHwnd())
	{
	m_norulePage.DestroyWindow();
	}
	if(m_FreehandPage.GetSafeHwnd())
	{
	m_FreehandPage.DestroyWindow();
	}
	if(m_rangePage.GetSafeHwnd())
	{
	m_rangePage.DestroyWindow();
	}
	if(m_wordPage.GetSafeHwnd())
	{
	m_wordPage.DestroyWindow();
	}
	if(m_pointPage.GetSafeHwnd())
	{
	m_pointPage.DestroyWindow();
	}
	if(m_enumPage.GetSafeHwnd())
	{
	m_enumPage.DestroyWindow();
	}

	if(m_staticRuleFor.GetSafeHwnd())
	{
	CRect rect(0.5*cx/10,0.4*cy/10,9.5*cx/10,0.4*cy/10+20);
	m_staticRuleFor.MoveWindow(&rect);
	}

	CRect rcFrame(0.5*cx/10,cy/10,9.5*cx/10,9.5*cy/10);	
	if(m_staticRuleFrame.GetSafeHwnd())
	{
	m_staticRuleFrame.MoveWindow(&rcFrame);
	}*/

	pDoc->setCurViewType(3);
	pFrame->m_leftBar.ExpandNode(4);

	this->m_btnSwitchView.EnableWindow(false);
	FillKnowledgeView();

}

void CCasebaseView::FillKnowledgeView()
{


	CSoLIMApp* pApp=(CSoLIMApp*)AfxGetApp();

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());

	if(pDoc->m_pProjectCaseEdit->m_strCasebasePath != "")
	{

		CString strSampleDataListName=pDoc->m_pProjectCaseEdit->m_strCasebasePath;


		if(pDoc->m_pProjectCaseEdit->hCSV)
		{
			delete pDoc->m_pProjectCaseEdit->hCSV;		
			pDoc->m_pProjectCaseEdit->hCSV= NULL;
		}



		pDoc->m_pProjectCaseEdit->hCSV = new CDataFile((LPCTSTR) strSampleDataListName, DF::RF_READ_AS_STRING);




		int nFields = pDoc->m_pProjectCaseEdit->hCSV->GetNumberOfVariables();

		//DBFGetFieldCount( pDoc->m_pProjectCaseEdit->hDBF );
		int nRec = pDoc->m_pProjectCaseEdit->hCSV->GetNumberOfSamples(0)-1;













		// add field names to combobox and listctrl
		//CString fieldName;

		char fieldName[100];

		m_listctrlSampleData.DeleteAllItems();

		for(int i=0;i<m_numOfCol; i++)
			m_listctrlSampleData.DeleteColumn(1);

		m_numOfCol = 0;


		//int a = pDoc->m_pProjectCaseEdit->vecSampleData.size();

		//	pDoc->m_pProjectCaseEdit->vecSampleData.clear();

		while(comboFieldY.GetCount())
			comboFieldY.DeleteString(0);
		while(comboFieldX.GetCount())
			comboFieldX.DeleteString(0);

		while(comboFieldID.GetCount())
			comboFieldID.DeleteString(0);
		//while (listAllField.GetCount() > 0)
		//listAllField.DeleteString(0);
		//while(listSelField.GetCount() > 0)
		//listSelField.DeleteString(0);

		//added by Fei
		//while (listAllType.GetCount() > 0)
		//listAllType.DeleteString(0);
		//while(listSelType.GetCount() > 0)
		//	listSelType.DeleteString(0);

		int index = 1;


		int xIndex = 0, yIndex = 0;

		int idIndex = 0;


		CRect rect;
		GetClientRect(&rect);

		int cx = rect.right;
		int cy = rect.bottom;

		if(nFields == 0)
		{
			MessageBox("There are no records in the table.");

			return;
		}







		for (int j = 0; j < nFields; ++j)
		{
			//DBFGetFieldInfo(pDoc->m_pProjectCaseEdit->hDBF, j, fieldName, &width, &deci);


			pDoc->m_pProjectCaseEdit->hCSV->GetVariableName(j, fieldName);

			m_listctrlSampleData.InsertColumn(index, fieldName);

			m_numOfCol ++;
			m_listctrlSampleData.SetColumnWidth(index++, (0.96*cx - 80)/nFields);
			comboFieldX.AddString(fieldName);
			comboFieldY.AddString(fieldName);

			comboFieldID.AddString(fieldName);



			if(pDoc->m_pProjectCaseEdit->m_strXIndex == fieldName)
				xIndex = j;

			if(pDoc->m_pProjectCaseEdit->m_strYIndex == fieldName)
				yIndex = j;

			if(pDoc->m_pProjectCaseEdit->m_strIDIndex == fieldName)
				idIndex = j;



			//listAllField.AddString(fieldName);
			//comboTypeField.AddString(fieldName);
			UpdateData(False);
		}




		comboFieldX.SetCurSel(xIndex);
		comboFieldY.SetCurSel(yIndex);

		comboFieldID.SetCurSel(idIndex);

		//comboTypeField.SetCurSel(0);

		for(int i = 0; i < nRec; ++i)
		{
			CString sID;
			sID.Format("%d", i+1);
			m_listctrlSampleData.InsertItem(i, sID.GetBuffer());

			//std::vector<CString> record;

			for (int j = 0; j < nFields; ++j)
			{
				//   //const char* content = DBFReadStringAttribute(pDoc->m_pProjectCaseEdit->hDBF, i, j);
				char content[100];

				pDoc->m_pProjectCaseEdit->hCSV->GetData(j, i,content); 
				m_listctrlSampleData.SetItemText(i, j+1, content);
				//	record.push_back(content);
			}

			//pDoc->m_pProjectCaseEdit->vecSampleData.push_back(record);
		}
	}


}


void CCasebaseView::OnBnClickedButtonGetSample()
{

	CSoLIMApp* pApp=(CSoLIMApp*)AfxGetApp();

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());


	static char BASED_CODE szFilter[] = "Field Sample File (*.csv)|*.csv|All Files (*.*)|*.*||";
	CFileDialog filedlgBrowseSampleDlg(TRUE, NULL, "*.csv", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	if (filedlgBrowseSampleDlg.DoModal()==IDOK)
	{
		CString strSampleDataListName=filedlgBrowseSampleDlg.GetPathName();

		CString FileName = extractFileName(strSampleDataListName);


		CString strNewCSVFilePath= pDoc->m_strProjectDir + "\\" + FileName;
		CopyFile(strSampleDataListName,strNewCSVFilePath,false);  //copy env layer to current project directory




		pDoc->m_pProjectCaseEdit->m_strCasebasePath = strNewCSVFilePath;

		pDoc->m_pProjectCaseEdit->m_strCasebaseFile = FileName;

		if(pDoc->m_pProjectCaseEdit->hCSV)
		{
			delete pDoc->m_pProjectCaseEdit->hCSV;		
			pDoc->m_pProjectCaseEdit->hCSV= NULL;
		}




		pDoc->m_pProjectCaseEdit->hCSV = new CDataFile((LPCTSTR) strSampleDataListName, DF::RF_READ_AS_STRING);




		int nFields = pDoc->m_pProjectCaseEdit->hCSV->GetNumberOfVariables();

		//DBFGetFieldCount( pDoc->m_pProjectCaseEdit->hDBF );
		int nRec = pDoc->m_pProjectCaseEdit->hCSV->GetNumberOfSamples(0)-1;


		//DBFGetRecordCount( pDoc->m_pProjectCaseEdit->hDBF );

		//int iID = DBFGetFieldIndex(pDoc->m_pProjectCaseEdit->hDBF, "ID");


		// add field names to combobox and listctrl
		char fieldName[100];
		//int width, deci;
		m_listctrlSampleData.DeleteAllItems();

		for(int i=0;i<m_numOfCol; i++)
			m_listctrlSampleData.DeleteColumn(1);

		m_numOfCol = 0;


		while(comboFieldY.GetCount())
			comboFieldY.DeleteString(0);
		while(comboFieldX.GetCount())
			comboFieldX.DeleteString(0);

		while(comboFieldID.GetCount())
			comboFieldID.DeleteString(0);
		//while (listAllField.GetCount() > 0)
		//listAllField.DeleteString(0);
		//while(listSelField.GetCount() > 0)
		//listSelField.DeleteString(0);

		//added by Fei
		//while (listAllType.GetCount() > 0)
		//listAllType.DeleteString(0);
		//while(listSelType.GetCount() > 0)
		//	listSelType.DeleteString(0);




		if(nFields == 0)
		{
			MessageBox("There are no records in the table.");

			return;
		}


		CRect rect(0, 0, 0, 0);
		this->m_pImagePanel.MoveWindow(&rect);

		m_iCurView = 0;

		CRect rectClient;
		GetClientRect(&rectClient);

		int cx = rectClient.right;
		int cy = rectClient.bottom;

		if( this->m_listctrlSampleData.GetSafeHwnd())
		{
			CRect rect(0.2*cx/10,1*cy/10,9.8*cx/10,8* cy/10);

			this->m_listctrlSampleData.MoveWindow(&rect);
		}





		int index = 1;
		for (int j = 0; j < nFields; ++j)
		{
			//DBFGetFieldInfo(pDoc->m_pProjectCaseEdit->hDBF, j, fieldName, &width, &deci);

			//CString fieldName = "";


			pDoc->m_pProjectCaseEdit->hCSV->GetVariableName(j, fieldName);

			m_listctrlSampleData.InsertColumn(index, fieldName);

			m_numOfCol ++;
			m_listctrlSampleData.SetColumnWidth(index++, (0.96*cx - 80)/nFields);
			comboFieldX.AddString(fieldName);
			comboFieldY.AddString(fieldName);

			comboFieldID.AddString(fieldName);


			//listAllField.AddString(fieldName);
			//comboTypeField.AddString(fieldName);
			UpdateData(False);

		}




		comboFieldX.SetCurSel(0);
		comboFieldY.SetCurSel(0);

		comboFieldID.SetCurSel(0);


		//comboTypeField.SetCurSel(0);

		for(int i = 0; i < nRec; ++i)
		{
			CString sID;
			sID.Format("%d", i+1);
			m_listctrlSampleData.InsertItem(i, sID.GetBuffer());

			////std::vector<CString> record;

			for (int j = 0; j < nFields; ++j)
			{
				//const char* content = DBFReadStringAttribute(pDoc->m_pProjectCaseEdit->hDBF, i, j);
				char content[100];

				pDoc->m_pProjectCaseEdit->hCSV->GetData(j, i,content); 
				m_listctrlSampleData.SetItemText(i, j+1, content);
				//	record.push_back(content);
			}

			//pDoc->m_pProjectCaseEdit->vecSampleData.push_back(record);
		}

		this->m_btnSwitchView.EnableWindow(true);
	}



}

void CCasebaseView::OnBnClickedButtonClearSapmleList()
{
	CSoLIMApp* pApp=(CSoLIMApp*)AfxGetApp();

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	CSoLIMDoc *pDoc = (CSoLIMDoc *)(GetDocument());

	this->m_listctrlSampleData.DeleteAllItems();

	for(int i=0;i<m_numOfCol; i++)
		m_listctrlSampleData.DeleteColumn(1);

	m_numOfCol = 0;





	if(pDoc->m_pProjectCaseEdit->hCSV)
	{
		delete pDoc->m_pProjectCaseEdit->hCSV;		
		pDoc->m_pProjectCaseEdit->hCSV= NULL;

		/* if( pDoc->m_pProjectCaseEdit->vecSampleData.empty() != true)
		{
		for(int i=0; i<pDoc->m_pProjectCaseEdit->vecSampleData.size(); i++)
		pDoc->m_pProjectCaseEdit->vecSampleData[i].erase(pDoc->m_pProjectCaseEdit->vecSampleData[i].begin(),pDoc->m_pProjectCaseEdit->vecSampleData[i].end());
		pDoc->m_pProjectCaseEdit->vecSampleData.clear();
		}*/
	}



	this->m_pImagePanel.m_LayerLoaded = false;
	if(this->m_pImagePanel.m_pTheData != NULL)
	{
		delete this->m_pImagePanel.m_pTheData;
		this->m_pImagePanel.m_pTheData = NULL;
	}




	CRect rect(0, 0, 0, 0);
	this->m_pImagePanel.MoveWindow(&rect);

	m_iCurView = 0;

	CRect rectClient;
	GetClientRect(&rectClient);

	int cx = rectClient.right;
	int cy = rectClient.bottom;

	if( this->m_listctrlSampleData.GetSafeHwnd())
	{
		CRect rect(0.2*cx/10,1*cy/10,9.8*cx/10,8* cy/10);

		this->m_listctrlSampleData.MoveWindow(&rect);
	}




	while(comboFieldY.GetCount())
		comboFieldY.DeleteString(0);
	while(comboFieldX.GetCount())
		comboFieldX.DeleteString(0);

	while(comboFieldID.GetCount())
		comboFieldID.DeleteString(0);

	this->m_btnSwitchView.EnableWindow(false);



}

void CCasebaseView::OnCbnSelchangeComboFieldX()
{
	UpdateData(true);

	//CSoLIMApp* pApp=(CSoLIMApp*)AfxGetApp();

	//CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	//CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());

	//CString str;
	//comboFieldX.GetWindowText(str);

	//int i = comboFieldX.GetCurSel();


}

void CCasebaseView::OnCbnSelchangeComboFieldY()
{

	UpdateData(true);



}


void CCasebaseView::OnCbnSelchangeComboFieldID()
{

	UpdateData(true);



}



// ************************************SUBMIT******************************************* //

void CCasebaseView::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CSoLIMApp* pApp=(CSoLIMApp*)AfxGetApp();

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());

	//	if(pDoc->m_pProjectCaseEdit->hDBF == NULL)
	if(pDoc->m_pProjectCaseEdit->hCSV == NULL)
	{
		MessageBox("No sample point table is loaded!");
		return;
	}



	//pDoc->m_pProjectCaseEdit->xIndex = comboFieldX.GetCurSel();//DBFGetFieldIndex(pDoc->m_pProjectCaseEdit->hDBF,str);
	//pDoc->m_pProjectCaseEdit->yIndex = comboFieldY.GetCurSel();//DBFGetFieldIndex(pDoc->m_pProjectCaseEdit->hDBF, yFieldName.GetBuffer());



	CString strItem;
	comboFieldX.GetLBText(comboFieldX.GetCurSel(), strItem);
	pDoc->m_pProjectCaseEdit->m_strXIndex = strItem;

	comboFieldY.GetLBText(comboFieldY.GetCurSel(), strItem);

	pDoc->m_pProjectCaseEdit->m_strYIndex = strItem;

	comboFieldID.GetLBText(comboFieldID.GetCurSel(), strItem);

	pDoc->m_pProjectCaseEdit->m_strIDIndex = strItem;






	if(pDoc->m_pProjectCaseEdit->g_vecstrClimateLayers.size() > 0)
	{


		this->m_pImagePanel.m_pTheData = new CThreeDR;
		CString str = pDoc->m_pProjectCaseEdit->g_vecstrClimateLayers[0];
		int readflag = this->m_pImagePanel.m_pTheData->readin(pDoc->m_pProjectCaseEdit->g_vecstrClimateLayers[0].GetBuffer(1), 0);
		if(readflag != 0){
			MessageBox("Errors in displaying the sample points.");

		}  

		this->m_pImagePanel.m_LayerLoaded = true;

	}	

	else if(pDoc->m_pProjectCaseEdit->g_vecstrGeologyLayers.size() > 0)
	{


		this->m_pImagePanel.m_pTheData = new CThreeDR;
		CString str = pDoc->m_pProjectCaseEdit->g_vecstrGeologyLayers[0];
		int readflag = this->m_pImagePanel.m_pTheData->readin(pDoc->m_pProjectCaseEdit->g_vecstrGeologyLayers[0].GetBuffer(1), 0);
		if(readflag != 0){
			MessageBox("Errors in displaying the sample points.");

		}  

		this->m_pImagePanel.m_LayerLoaded = true;

	}	

	else if(pDoc->m_pProjectCaseEdit->g_vecstrTerrainLayers.size() > 0)
	{


		this->m_pImagePanel.m_pTheData = new CThreeDR;
		CString str = pDoc->m_pProjectCaseEdit->g_vecstrTerrainLayers[0];
		int readflag = this->m_pImagePanel.m_pTheData->readin(pDoc->m_pProjectCaseEdit->g_vecstrTerrainLayers[0].GetBuffer(1), 0);
		if(readflag != 0){
			MessageBox("Errors in displaying the sample points.");

		}  

		this->m_pImagePanel.m_LayerLoaded = true;

	}	

	else if(pDoc->m_pProjectCaseEdit->g_vecstrVegetationLayers.size() > 0)
	{


		this->m_pImagePanel.m_pTheData = new CThreeDR;
		CString str = pDoc->m_pProjectCaseEdit->g_vecstrVegetationLayers[0];
		int readflag = this->m_pImagePanel.m_pTheData->readin(pDoc->m_pProjectCaseEdit->g_vecstrVegetationLayers[0].GetBuffer(1), 0);
		if(readflag != 0){
			MessageBox("Errors in displaying the sample points.");

		}  

		this->m_pImagePanel.m_LayerLoaded = true;

	}	



	else if(pDoc->m_pProjectCaseEdit->g_vecstrOtherLayers.size() > 0)
	{


		this->m_pImagePanel.m_pTheData = new CThreeDR;
		CString str = pDoc->m_pProjectCaseEdit->g_vecstrOtherLayers[0];
		int readflag = this->m_pImagePanel.m_pTheData->readin(pDoc->m_pProjectCaseEdit->g_vecstrOtherLayers[0].GetBuffer(1), 0);
		if(readflag != 0){
			MessageBox("Errors in displaying the sample points.");

		}  

		this->m_pImagePanel.m_LayerLoaded = true;

	}	

	else{
		MessageBox("Errors in displaying the sample points.No Layer is found in the GIS Database");
		return;

	}  
	//this->m_pImagePanel.m_pTheData = this->m_theInput;


	this->m_pImagePanel.m_vecPoints.clear();



	//		int sampleCount = DBFGetRecordCount( pDoc->m_pProjectCaseEdit->hDBF);

	int sampleCount = 	pDoc->m_pProjectCaseEdit->hCSV->GetNumberOfSamples(0) -1;

	for (int i = 0; i < sampleCount; ++i)
	{

		FieldSample pt;


		//int ix = DBFGetFieldIndex(pDoc->m_pProjectCaseEdit->hDBF, pDoc->m_pProjectCaseEdit->m_strXIndex.GetBuffer());

		int ix = pDoc->m_pProjectCaseEdit->hCSV->GetVariableIndex(pDoc->m_pProjectCaseEdit->m_strXIndex.GetBuffer());

		//int iy = DBFGetFieldIndex(pDoc->m_pProjectCaseEdit->hDBF, pDoc->m_pProjectCaseEdit->m_strYIndex.GetBuffer());
		int iy = pDoc->m_pProjectCaseEdit->hCSV->GetVariableIndex(pDoc->m_pProjectCaseEdit->m_strYIndex.GetBuffer());

		char strX[100], strY[100];

		pDoc->m_pProjectCaseEdit->hCSV->GetData(ix, i, strX);

		pDoc->m_pProjectCaseEdit->hCSV->GetData(iy, i, strY);




		//pt.x= DBFReadDoubleAttribute(pDoc->m_pProjectCaseEdit->hDBF, i, ix);
		// pt.y = DBFReadDoubleAttribute(pDoc->m_pProjectCaseEdit->hDBF, i, iy);

		pt.x = atof(strX);
		pt.y = atof(strY);



		this->m_pImagePanel.m_vecPoints.push_back(pt);
	}



	CRect rect(0, 0, 0, 0);
	this->m_listctrlSampleData.MoveWindow(&rect);

	m_iCurView = 1;

	CRect rectClient;
	GetClientRect(&rectClient);

	int cx = rectClient.right;
	int cy = rectClient.bottom;

	if( this->m_pImagePanel.GetSafeHwnd())
	{
		CRect rect(0.2*cx/10,1*cy/10,9.8*cx/10,8* cy/10);

		this->m_pImagePanel.MoveWindow(&rect);
	}


	this->m_pImagePanel.Invalidate();

	this->m_btnSwitchView.EnableWindow(true);






	//DBFGetFieldIndex(pDoc->m_pProjectCaseEdit->hDBF,str);
	//pDoc->m_pProjectCaseEdit->m_strYIndex = comboFieldY.GetCurSel();//DBFGetFieldIndex(pDoc->m_pProjectCaseEdit->hDBF, yFieldName.GetBuffer());

}

void CCasebaseView::OnBnClickedButtonSwitchBetweenTwoViews()
{
	// TODO: Add your control notification handler code here


	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());


	if(pDoc->m_pProjectCaseEdit->hCSV == NULL)
	{
		MessageBox("No sample point table is loaded!");
		return;
	}


	CRect rectClient;
	GetClientRect(&rectClient);

	int cx = rectClient.right;
	int cy = rectClient.bottom;


	if(m_iCurView == 1)
	{
		m_iCurView = 0;	

		if(m_listctrlSampleData.GetSafeHwnd())
		{

			CRect rect(0.2*cx/10,1*cy/10,9.8*cx/10,8* cy/10);
			m_listctrlSampleData.MoveWindow(&rect);
		}
		if(this->m_pImagePanel.GetSafeHwnd())
		{
			CRect rect(0, 0, 0, 0);

			m_pImagePanel.MoveWindow(&rect);
		}
	}
	else
	{
		m_iCurView = 1;
		if(m_listctrlSampleData.GetSafeHwnd())
		{  
			CRect rect(0, 0, 0, 0);


			m_listctrlSampleData.MoveWindow(&rect);
		}
		if(this->m_pImagePanel.GetSafeHwnd())
		{
			CRect rect(0.2*cx/10,1*cy/10,9.8*cx/10,8* cy/10);

			m_pImagePanel.MoveWindow(&rect);
		}

	}


}

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
// SoLIMDoc.cpp : implementation of the CSoLIMDoc class
//

#include "stdafx.h"
#include "SoLIM.h"
#include "SoLIMDoc.h"
#include ".\solimdoc.h"
#include "utility.h"
#include "knowledge.h"
#include "dbfrulebase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include  <msxml2.h>      
#import   <msxml4.dll>
#include "Rule.h"
#include "RangeRule.h"
#include "FreehandRule.h"
#include "WordRule.h"
#include "EnumRule.h"
#include "PointRule.h"
#include "Instance.h"
#include "SoilType.h"
#include "Occurrence.h"
#include "Exclusion.h"
#include "MainFrm.h"
#include "DatabaseView.h"
#include "DataExplorerView.h"
#include "MetadataView.h"
#include "InfoFrameView.h"
#include "KnowledgeView.h"
#include "InferenceView.h"
#include "NewProjectDlg.h"
#include "SaveAsDlg.h"

#include "CBInferenceView.h"





/////////////////////////////////////////////////////////////////////////////
// CSoLIMDoc

IMPLEMENT_DYNCREATE(CSoLIMDoc, CDocument)

BEGIN_MESSAGE_MAP(CSoLIMDoc, CDocument)
	//{{AFX_MSG_MAP(CSoLIMDoc)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_PROJECT_NEW, OnProjectNew)
	ON_COMMAND(ID_PROJECT_OPEN, OnProjectOpen)
	ON_COMMAND(ID_PROJECT_SAVE, OnProjectSave)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_SAVE, OnUpdateProjectSave)
	ON_COMMAND(ID_PROJECT_SAVE_AS, OnProjectSaveAs)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_SAVE_AS, OnUpdateProjectSaveAs)

	ON_COMMAND(ID_RECENTFILES_1, OnRecentfiles1)
	ON_COMMAND(ID_RECENTFILES_2, OnRecentfiles2)
	ON_COMMAND(ID_RECENTFILES_3, OnRecentfiles3)
	ON_COMMAND(ID_RECENTFILES_4, OnRecentfiles4)
	ON_COMMAND(ID_RECENTFILES_CLEARRECENTFILELIST, OnRecentfilesClearrecentfilelist)
	ON_UPDATE_COMMAND_UI(ID_RECENTFILES_CLEARRECENTFILELIST, OnUpdateRecentfilesClearrecentfilelist)
	ON_COMMAND(ID_PROJECT_CONVERTFROMSOLIM4, OnProjectConvertfromSoLIM4)
	ON_COMMAND(ID_PROJECT_CONVERTFROMOLDSOLIM, OnProjectConvertfromoldsolim)
	ON_COMMAND(ID_PROJECT_CONVERTFROMSIE, OnProjectConvertfromsie)
	ON_COMMAND(ID_PROJECT_CONVERTFROSOLIMSOLUTIONS5, OnProjectConvertfrosolimsolutions5)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoLIMDoc construction/destruction

CSoLIMDoc::CSoLIMDoc()
{
	m_pProject = NULL;
	m_pProjectEdit = NULL;
	m_iCurViewType = 0;

	m_pProjectCase = NULL;
	m_pProjectCaseEdit = NULL;

	m_iProjectType =0;

}

CSoLIMDoc::~CSoLIMDoc()
{
}

BOOL CSoLIMDoc::OnNewDocument()
{
	SetTitle("Untitled");
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CSoLIMDoc serialization

void CSoLIMDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) 
	{
	}
	else 
	{	 

	}

}

/////////////////////////////////////////////////////////////////////////////
// CSoLIMDoc diagnostics

#ifdef _DEBUG
void CSoLIMDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSoLIMDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSoLIMDoc commands


//write new project code here
void CSoLIMDoc::OnProjectNew()
{
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	//ask to save
	if(m_pProject!=NULL || m_pProjectCase!=NULL)
	{			
		int s =pFrame->MessageBox("Save edits to current project?", "Save Edits",MB_ICONQUESTION|MB_YESNOCANCEL);//6: yes; 7: no; 2: cancel
		if(s == 6)  //yes
			OnProjectSave();
		else if(s == 2)  //cancel
			return;	 
	}



	CNewProjectDlg NewProDlg;
	if(NewProDlg.DoModal()==IDOK)
	{
		CleanMemory();
		m_strProjectFileName = NewProDlg.m_strProjectName+".sip";
		m_strProjectDir = NewProDlg.m_strProjectDir;

		m_iProjectType = NewProDlg.m_iProjectType;

		//if project is set correctly,create new project
		NewProject(m_strProjectDir,NewProDlg.m_strProjectName, m_iProjectType);	 
		SetTitle(NewProDlg.m_strProjectName);
	}


}

void CSoLIMDoc::OnProjectOpen()
{
	//ask to save
	if(m_pProject!=NULL||m_pProjectCase!=NULL)
	{

		CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();	
		int s =pFrame->MessageBox("Save edits to current project?", "Save Edits",MB_ICONQUESTION|MB_YESNOCANCEL);//6: yes; 7: no; 2: cancel
		if(s == 6)  //yes
			OnProjectSave();
		else if(s == 2) //cancel
			return;	 
	}




	static char BASED_CODE szFilter[] = "Project Files (*.sip)|*.sip|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.sip", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open Project";
	if(m_ldFile.DoModal() == IDOK)
	{   
		CleanMemory();
		ReadProject(m_ldFile.GetPathName());
		if(m_pProject!=NULL)
		{
			SetTitle(m_pProject->getProjectName());

		}
		else if(m_pProjectCase!=NULL)
		{
			SetTitle(m_pProjectCase->getProjectName());			

		}

		CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();	 
		m_CMruFile.pMenu= CMenu::FromHandle(pFrame->getMenuHandle());
		m_CMruFile.AddMru(m_ldFile.GetPathName());
	}

}

void CSoLIMDoc::OnProjectSave()
{
	CString strProjectFullPath= m_strProjectDir+"\\"+m_strProjectFileName;

	if(m_iProjectType == 0)
	{

		CopyProject(m_pProjectEdit, m_pProject, false);

		WriteProject(strProjectFullPath, m_pProject);

	}
	else
	{



		if(getCurViewType()== 4)
		{
			CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
			CCBInferenceView *pCBInferenceView = (CCBInferenceView *)(pFrame->m_splitterTabWnd.GetPane(0,0));	

			pCBInferenceView->SaveData();
		}



		CopyProjectCase(m_pProjectCaseEdit, m_pProjectCase, false);

		WriteProjectCase(strProjectFullPath, m_pProjectCase);
	}
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();	 
	m_CMruFile.pMenu= CMenu::FromHandle(pFrame->getMenuHandle());
	m_CMruFile.AddMru(strProjectFullPath);

}

void CSoLIMDoc::setCurViewType(int iCurViewType)
{
	m_iCurViewType = iCurViewType;
}

int CSoLIMDoc::getCurViewType()
{
	return m_iCurViewType;
}

void CSoLIMDoc::NewProject(CString strProjectDir, CString strProjectName, int iProjectType)
{
	CString strProjectFullPath = strProjectDir+"\\"+strProjectName+".sip";
	CreateDirectory(m_strProjectDir+"\\GISData",NULL);	

	if(iProjectType == 0) ///rule based project
	{

		m_pProject = new CProject;
		m_pProject->setProjectName(strProjectName);
		m_pProject->setProjectDir(strProjectDir);
		m_pProject->setProjectDataType("3dr");

		if(m_pProject->m_pEnvDatabase != NULL)
		{
			delete m_pProject->m_pEnvDatabase;
			m_pProject->m_pEnvDatabase = NULL;
		}
		m_pProject->m_pEnvDatabase = new CEnvDatabase();	
		if(m_pProject->m_pKnowledgeBase != NULL)
		{
			delete m_pProject->m_pKnowledgeBase;
			m_pProject->m_pKnowledgeBase = NULL;
		}
		m_pProject->m_pKnowledgeBase = new CKnowledgeBase();
		if(m_pProject->m_pInference != NULL)
		{
			delete m_pProject->m_pInference;
			m_pProject->m_pInference = NULL;
		}
		m_pProject->m_pInference = new CInference();


		//write the newly created project
		WriteProject(strProjectFullPath,m_pProject);
		CleanMemory();
		//read the project from hard disk
		ReadProject(strProjectFullPath);

		if(m_pProject!=NULL)
			SetTitle(m_pProject->getProjectName());
	}


	else if(iProjectType == 1) ///case based project
	{
		CreateDirectory(m_strProjectDir+"\\GISData\\Climate",NULL);
		CreateDirectory(m_strProjectDir+"\\GISData\\Geology",NULL);
		CreateDirectory(m_strProjectDir+"\\GISData\\Terrain",NULL);
		CreateDirectory(m_strProjectDir+"\\GISData\\Vegetation",NULL);
		CreateDirectory(m_strProjectDir+"\\GISData\\Other",NULL);

		m_pProjectCase = new CProjectCase();
		m_pProjectCase->setProjectName(strProjectName);
		m_pProjectCase->setProjectDir(strProjectDir);
		m_pProjectCase->setProjectDataType("3dr");


		//write the newly created project
		WriteProjectCase(strProjectFullPath,m_pProjectCase);

		ReadProject(strProjectFullPath);

		if(m_pProjectCase!=NULL)
			SetTitle(m_pProjectCase->getProjectName());

	}
}


void CSoLIMDoc::CleanMemory()
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	if(m_pProject != NULL)
	{
		delete m_pProject;
		m_pProject = NULL;
	}
	if(m_pProjectEdit!=NULL)
	{
		m_pProjectEdit->m_pEnvDatabase->setLayerNum(0);
		delete m_pProjectEdit;
		m_pProjectEdit = NULL;
	}



	if(m_pProjectCase != NULL)
	{




		delete m_pProjectCase;
		m_pProjectCase = NULL;
	}
	if(m_pProjectCaseEdit!=NULL)
	{
		//m_pProjectCaseEdit->m_pEnvDatabase->setLayerNum(0);





		delete m_pProjectCaseEdit;
		m_pProjectCaseEdit = NULL;
	}


	pApp->m_iCurrEnvLayerIndex=-1;
	pFrame->m_leftBar.CleanMemory();
	pFrame->Invalidate();

}


void CSoLIMDoc::OnUpdateProjectSave(CCmdUI *pCmdUI)
{
	if(m_pProject != NULL || m_pProjectCase != NULL)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);
}

void CSoLIMDoc::OnProjectSaveAs()
{
	CSaveAsDlg SaveAsDlg;

	if(SaveAsDlg.DoModal() == IDOK)
	{

		CString strDiffProFullPath = m_strProjectDir+"\\"+SaveAsDlg.m_strDiffProName+".sip";

		if(this->m_iProjectType == 0)
			WriteProject(strDiffProFullPath, m_pProjectEdit);

		else
			WriteProjectCase(strDiffProFullPath, m_pProjectCaseEdit);
	}	
}




void CSoLIMDoc::OnUpdateProjectSaveAs(CCmdUI *pCmdUI)
{

	if(m_pProject != NULL || m_pProjectCase != NULL)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);
}


void CSoLIMDoc::CopyProject(CProject *pProjectFrom, CProject *pProjectTo, bool bIsEditVersion)
{

	pProjectTo->setIsEditVersion(bIsEditVersion);
	pProjectTo->setProjectDataType(pProjectFrom->getProjectDataType());
	pProjectTo->setProjectDir(pProjectFrom->getProjectDir());
	pProjectTo->setProjectName(pProjectFrom->getProjectName());

	if(bIsEditVersion==true)  //from back-up project to edit project
	{

		pProjectTo->m_pEnvDatabase = NULL;
		pProjectTo->m_pKnowledgeBase = NULL;
		pProjectTo->m_pInference = NULL;
		pProjectTo->m_pEnvDatabase = new CEnvDatabase();
		pProjectTo->m_pEnvDatabase->CopyEnvDatabase(pProjectFrom->m_pEnvDatabase,true);
		pProjectTo->m_pKnowledgeBase = new CKnowledgeBase(*(pProjectFrom->m_pKnowledgeBase));
		pProjectTo->m_pInference = new CInference(*(pProjectFrom->m_pInference));
		pProjectTo->m_pInference->m_pEnvDatabase= pProjectTo->m_pEnvDatabase;
		pProjectTo->m_pInference->m_pKnowledgeBase = pProjectTo->m_pKnowledgeBase;

		pProjectTo->m_strKnowledgeBaseFile = pProjectFrom->m_strKnowledgeBaseFile;
	}

	else  //from edit project to back-up project
	{
		pProjectTo->m_pKnowledgeBase = NULL;
		pProjectTo->m_pInference = NULL;
		pProjectTo->m_pEnvDatabase->CopyEnvDatabase(pProjectFrom->m_pEnvDatabase,false);
		pProjectTo->m_pKnowledgeBase = new CKnowledgeBase(*(pProjectFrom->m_pKnowledgeBase));
		pProjectTo->m_pInference = new CInference(*(pProjectFrom->m_pInference));
		pProjectTo->m_pInference->m_pEnvDatabase= pProjectTo->m_pEnvDatabase;
		pProjectTo->m_pInference->m_pKnowledgeBase = pProjectTo->m_pKnowledgeBase;


		pProjectTo->m_strKnowledgeBaseFile = pProjectFrom->m_strKnowledgeBaseFile;
	}


}

void CSoLIMDoc::CopyProjectCase(CProjectCase *pProjectFrom, CProjectCase *pProjectTo, bool bIsEditVersion)
{

	pProjectTo->setIsEditVersion(bIsEditVersion);
	pProjectTo->setProjectDataType(pProjectFrom->getProjectDataType());
	pProjectTo->setProjectDir(pProjectFrom->getProjectDir());
	pProjectTo->setProjectName(pProjectFrom->getProjectName());

	pProjectTo->vecClimateLayerNames.clear();
	pProjectTo->vecClimateLayerFiles.clear();
	pProjectTo->g_vecstrClimateLayers.clear();
	pProjectTo->vecGeologyLayerNames.clear();
	pProjectTo->vecGeologyLayerFiles.clear();
	pProjectTo->g_vecstrGeologyLayers.clear();
	pProjectTo->vecTerrainLayerNames.clear();
	pProjectTo->vecTerrainLayerFiles.clear();
	pProjectTo->g_vecstrTerrainLayers.clear();
	pProjectTo->vecVegetationLayerNames.clear();
	pProjectTo->vecVegetationLayerFiles.clear();
	pProjectTo->g_vecstrVegetationLayers.clear();
	pProjectTo->vecOtherLayerNames.clear();
	pProjectTo->vecOtherLayerFiles.clear();
	pProjectTo->g_vecstrOtherLayers.clear();




	pProjectTo->vecClimateLayerResolutions.clear();

	pProjectTo->vecClimateLayerSimiTypes.clear();


	pProjectTo->vecClimateLayerSimiMethods.clear();


	pProjectTo->vecGeologyLayerResolutions.clear();

	pProjectTo->vecGeologyLayerSimiTypes.clear();



	pProjectTo->vecGeologyLayerSimiMethods.clear();


	pProjectTo->vecTerrainLayerResolutions.clear();

	pProjectTo->vecTerrainLayerSimiTypes.clear();


	pProjectTo->vecTerrainLayerSimiMethods.clear();


	pProjectTo->vecVegetationLayerResolutions.clear();

	pProjectTo->vecVegetationLayerSimiTypes.clear();


	pProjectTo->vecVegetationLayerSimiMethods.clear();



	pProjectTo->vecOtherLayerResolutions.clear();

	pProjectTo->vecOtherLayerSimiTypes.clear();


	pProjectTo->vecOtherLayerSimiMethods.clear();

	for(int i=0; i<pProjectFrom->vecClimateLayerNames.size(); i++)
	{
		pProjectTo->vecClimateLayerNames.push_back(pProjectFrom->vecClimateLayerNames[i]);
		pProjectTo->vecClimateLayerFiles.push_back(pProjectFrom->vecClimateLayerFiles[i]);
		pProjectTo->g_vecstrClimateLayers.push_back(pProjectFrom->g_vecstrClimateLayers[i]);

		pProjectTo->vecClimateLayerResolutions.push_back(pProjectFrom->vecClimateLayerResolutions[i]);

		pProjectTo->vecClimateLayerSimiTypes.push_back(pProjectFrom->vecClimateLayerSimiTypes[i]);


		pProjectTo->vecClimateLayerSimiMethods.push_back(pProjectFrom->vecClimateLayerSimiMethods[i]);


	}
	for(int i=0; i<pProjectFrom->vecGeologyLayerNames.size(); i++)
	{
		pProjectTo->vecGeologyLayerNames.push_back(pProjectFrom->vecGeologyLayerNames[i]);
		pProjectTo->vecGeologyLayerFiles.push_back(pProjectFrom->vecGeologyLayerFiles[i]);
		pProjectTo->g_vecstrGeologyLayers.push_back(pProjectFrom->g_vecstrGeologyLayers[i]);


		pProjectTo->vecGeologyLayerResolutions.push_back(pProjectFrom->vecGeologyLayerResolutions[i]);

		pProjectTo->vecGeologyLayerSimiTypes.push_back(pProjectFrom->vecGeologyLayerSimiTypes[i]);


		pProjectTo->vecGeologyLayerSimiMethods.push_back(pProjectFrom->vecGeologyLayerSimiMethods[i]);

	}

	for(int i=0; i<pProjectFrom->vecTerrainLayerNames.size(); i++)
	{
		pProjectTo->vecTerrainLayerNames.push_back(pProjectFrom->vecTerrainLayerNames[i]);
		pProjectTo->vecTerrainLayerFiles.push_back(pProjectFrom->vecTerrainLayerFiles[i]);
		pProjectTo->g_vecstrTerrainLayers.push_back(pProjectFrom->g_vecstrTerrainLayers[i]);

		pProjectTo->vecTerrainLayerResolutions.push_back(pProjectFrom->vecTerrainLayerResolutions[i]);

		pProjectTo->vecTerrainLayerSimiTypes.push_back(pProjectFrom->vecTerrainLayerSimiTypes[i]);


		pProjectTo->vecTerrainLayerSimiMethods.push_back(pProjectFrom->vecTerrainLayerSimiMethods[i]);

	}

	for(int i=0; i<pProjectFrom->vecVegetationLayerNames.size(); i++)
	{
		pProjectTo->vecVegetationLayerNames.push_back(pProjectFrom->vecVegetationLayerNames[i]);
		pProjectTo->vecVegetationLayerFiles.push_back(pProjectFrom->vecVegetationLayerFiles[i]);
		pProjectTo->g_vecstrVegetationLayers.push_back(pProjectFrom->g_vecstrVegetationLayers[i]);


		pProjectTo->vecVegetationLayerResolutions.push_back(pProjectFrom->vecVegetationLayerResolutions[i]);

		pProjectTo->vecVegetationLayerSimiTypes.push_back(pProjectFrom->vecVegetationLayerSimiTypes[i]);


		pProjectTo->vecVegetationLayerSimiMethods.push_back(pProjectFrom->vecVegetationLayerSimiMethods[i]);
	}

	for(int i=0; i<pProjectFrom->vecOtherLayerNames.size(); i++)
	{
		pProjectTo->vecOtherLayerNames.push_back(pProjectFrom->vecOtherLayerNames[i]);
		pProjectTo->vecOtherLayerFiles.push_back(pProjectFrom->vecOtherLayerFiles[i]);
		pProjectTo->g_vecstrOtherLayers.push_back(pProjectFrom->g_vecstrOtherLayers[i]);



		pProjectTo->vecOtherLayerResolutions.push_back(pProjectFrom->vecOtherLayerResolutions[i]);

		pProjectTo->vecOtherLayerSimiTypes.push_back(pProjectFrom->vecOtherLayerSimiTypes[i]);


		pProjectTo->vecOtherLayerSimiMethods.push_back(pProjectFrom->vecOtherLayerSimiMethods[i]);
	}

	//	pProjectTo->g_useKde = pProjectFrom->g_useKde;
	//pProjectTo->g_useKdeVegetation = pProjectFrom->g_useKdeVegetation;

	pProjectTo->m_strCasebaseFile = pProjectFrom->m_strCasebaseFile;

	pProjectTo->m_strCasebasePath = pProjectFrom->m_strCasebasePath;

	pProjectTo->iCBROption = pProjectFrom->iCBROption;


	pProjectTo->m_iCaseInteMethod = pProjectFrom->m_iCaseInteMethod;

	pProjectTo->m_dClimateCut = pProjectFrom->m_dClimateCut;

	pProjectTo->m_dClimateWeight = pProjectTo->m_dClimateWeight;

	pProjectTo->m_dTopoWeight = pProjectTo->m_dTopoWeight;

	pProjectTo->m_dVegWeight = pProjectTo->m_dVegWeight;



	pProjectTo->m_thresholdProperty = pProjectFrom->m_thresholdProperty;

	//	pProjectTo->m_thresholdType = pProjectFrom->m_thresholdType;



	//pProjectTo->hDBF = pProjectFrom->hDBF;

	pProjectTo->hCSV = pProjectFrom->hCSV;

	//if(pProjectTo->hDBF != NULL && pProjectTo->hDBF->fp != NULL) 
	//if(pProjectTo->hCSV != NULL) 
	//{

	//	//int nFields = DBFGetFieldCount(pProjectTo->hDBF );
	//	int nFields = pProjectTo->hCSV->GetNumberOfVariables();
	//	//int nRec = DBFGetRecordCount( pProjectTo->hDBF );
	//	int nRec = pProjectTo->hCSV->GetNumberOfSamples(0) - 1;

	//	//pProjectTo->vecSampleData.clear();

	//	for(int i = 0; i < nRec; ++i)
	//	{

	//		std::vector<CString> record;

	//		for (int j = 0; j < nFields; ++j)
	//		{
	//			//CString content = "";
	//			//const char* content = DBFReadStringAttribute(pProjectTo->hDBF, i, j);

	//			char content[100];

	//			pProjectTo->hCSV->GetData(j, i, content);

	//			record.push_back(content);
	//		}

	//		pProjectTo->vecSampleData.push_back(record);
	//	}
	//}

	/*

	for(int i=0; i<pProjectTo->vecSampleData.size(); i++)
	{
	pProjectTo->vecSampleData[i].clear();
	}


	pProjectTo->vecSampleData.clear();*/


	pProjectTo->m_strXIndex = pProjectFrom->m_strXIndex;
	pProjectTo->m_strYIndex = pProjectFrom->m_strYIndex;

	pProjectTo->m_strIDIndex = pProjectFrom->m_strIDIndex;

	pProjectTo->setProjectDir(pProjectFrom->getProjectDir());






}



void CSoLIMDoc::OpenMruFile(int nMru)
{
	if(m_pProject!=NULL ||m_pProjectCase!=NULL)
	{

		CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();	
		int s =pFrame->MessageBox("Save edits to current project?", "Save Edits",MB_ICONQUESTION|MB_YESNOCANCEL);//6: yes; 7: no; 2: cancel
		if(s == 6)  //yes
			OnProjectSave();
		else if(s == 2)
			return;	 
	}
	if(m_CMruFile.m_PathName[nMru]!="")
	{

		CleanMemory();
		ReadProject(m_CMruFile.m_PathName[nMru]);
		if(m_pProject!=NULL)
		{
			SetTitle(m_pProject->getProjectName());
		}
		else if(m_pProjectCase!=NULL)
		{
			SetTitle(m_pProjectCase->getProjectName());
		}
	}
}

void CSoLIMDoc::OnRecentfiles1()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();	 
	m_CMruFile.pMenu= CMenu::FromHandle(pFrame->getMenuHandle());
	OpenMruFile(0);
}

void CSoLIMDoc::OnRecentfiles2()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();	 
	m_CMruFile.pMenu= CMenu::FromHandle(pFrame->getMenuHandle());
	OpenMruFile(1);
}

void CSoLIMDoc::OnRecentfiles3()
{

	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();	 
	m_CMruFile.pMenu= CMenu::FromHandle(pFrame->getMenuHandle());
	OpenMruFile(2);
}

void CSoLIMDoc::OnRecentfiles4()
{


	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();	 
	m_CMruFile.pMenu= CMenu::FromHandle(pFrame->getMenuHandle());
	OpenMruFile(3);
}

void CSoLIMDoc::OnRecentfilesClearrecentfilelist()
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();	 
	m_CMruFile.pMenu= CMenu::FromHandle(pFrame->getMenuHandle());
	m_CMruFile.ClearMru();
}

void CSoLIMDoc::OnUpdateRecentfilesClearrecentfilelist(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( m_CMruFile.m_CurNum );

}

void CSoLIMDoc::ReadProject(CString strFileName)
{



	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();

	if(strFileName==""||strFileName.Right(4)!=".sip")  
	{
		AfxMessageBox("An invalid SoLIM Solutions project file.");
	}
	else
	{
		int tag = strFileName.ReverseFind('\\');
		m_strProjectDir=strFileName.Left(tag);


		m_strProjectFileName=extractFileName(strFileName);
	}




	HRESULT hr;	 
	MSXML2::IXMLDOMDocumentPtr pDoc;//xml document
	MSXML2::IXMLDOMElementPtr pRootElem;
	MSXML2::IXMLDOMNamedNodeMapPtr pRootAttr;
	MSXML2::IXMLDOMNodeListPtr pRootList;	
	_variant_t VariantValue;
	CString str;

	hr = CoInitialize(NULL);
	if(FAILED(hr))
	{
		AfxMessageBox("Failed to initialize COM");
		return;
	}
	//create instance
	hr=pDoc.CreateInstance(_uuidof(MSXML2::DOMDocument));
	if(FAILED(hr))
	{
		AfxMessageBox("Failed to create DOM Document");
		return;
	}
	//load xml doc
	_variant_t varxml(strFileName);
	VARIANT_BOOL varOut;
	varOut = pDoc->load(varxml);
	if(!varOut)
	{
		AfxMessageBox("Failed to Load xml file");
		return;
	}




	//	//***********parse root node SoLIMProject************************
	pDoc->get_documentElement(&pRootElem);
	if(pRootElem->hasChildNodes() != -1)//-1 is True
	{
		return;
	}
	if(m_pProject!=NULL)
	{
		delete m_pProject;
		m_pProject = NULL;
	}

	if(m_pProjectCase != NULL)
	{
		delete m_pProjectCase;
		m_pProjectCase = NULL;
	}



	pRootElem->get_attributes(&pRootAttr);
	MSXML2::IXMLDOMNodePtr pProjectVersionNode;
	MSXML2::IXMLDOMNodePtr pProjectNameNode;
	MSXML2::IXMLDOMNodePtr pProjectTypeNode;
	MSXML2::IXMLDOMNodePtr pDataTypeNode;



	pRootAttr->get_item(0,&pProjectVersionNode);
	pRootAttr->get_item(1,&pProjectNameNode); 
	pRootAttr->get_item(2,&pProjectTypeNode);
	pRootAttr->get_item(3,&pDataTypeNode);


	pProjectVersionNode->get_nodeTypedValue(&VariantValue);	
	str =(char *)(_bstr_t)VariantValue;
	if(str != "2010")
	{
		AfxMessageBox("Project version is not correct.");
		return;
	}


	pProjectNameNode->get_nodeTypedValue(&VariantValue);
	CString strName = (char *)(_bstr_t)VariantValue;	

	pProjectTypeNode->get_nodeTypedValue(&VariantValue);
	CString strProjectType =(char *)(_bstr_t)VariantValue;
	if(strProjectType == "RBI") 
	{

		m_pProject = new CProject;
		m_pProject->setProjectName(strName);   
		this->m_iProjectType = 0;

		pDataTypeNode->get_nodeTypedValue(&VariantValue);
		str =(char *)(_bstr_t)VariantValue;
		m_pProject->setProjectDataType(str);
		m_pProject->setProjectDir(m_strProjectDir);
		pRootElem->get_childNodes(&pRootList);
	}

	else 
	{

		m_pProjectCase = new CProjectCase;
		m_pProjectCase->setProjectName(strName);  
		this->m_iProjectType = 1;
		pDataTypeNode->get_nodeTypedValue(&VariantValue);
		str =(char *)(_bstr_t)VariantValue;

		m_pProjectCase->setProjectDataType(str);
		m_pProjectCase->setProjectDir(m_strProjectDir);
		pRootElem->get_childNodes(&pRootList);
	}




	if(this->m_iProjectType == 0)
	{

		//*********** Parse database node *************	
		MSXML2::IXMLDOMNodePtr pDatabaseNode;
		if(m_pProject->m_pEnvDatabase != NULL)
		{
			delete m_pProject->m_pEnvDatabase;
			m_pProject->m_pEnvDatabase = NULL;
		}
		m_pProject->m_pEnvDatabase = new CEnvDatabase();	   
		pRootList->get_item(0,&pDatabaseNode);	  
		MSXML2::IXMLDOMNodeListPtr pDatabaseList;
		pDatabaseNode->get_childNodes(&pDatabaseList);
		long layerNum;		
		pDatabaseList->get_length(&layerNum);	
		int tempLayerNum = layerNum;
		//traverse the child nodes of layerlist
		MSXML2::IXMLDOMNodeListPtr pLayerList;
		MSXML2::IXMLDOMNodePtr pLayerNode;
		MSXML2::IXMLDOMNodePtr pLayerNameNode,pLayerPathNode;
		for(long i=0; i<tempLayerNum; i++)
		{
			pDatabaseList->get_item(i,&pLayerNode);
			pLayerNode->get_childNodes(&pLayerList);
			pLayerList->get_item(0,&pLayerNameNode);
			pLayerList->get_item(1,&pLayerPathNode);
			pLayerNameNode->get_nodeTypedValue(&VariantValue);		
			CString strLayerName =(char *)(_bstr_t)VariantValue;		
			pLayerPathNode->get_nodeTypedValue(&VariantValue);		
			CString strLayerPath = m_strProjectDir +"\\GISData\\"+ (char *)(_bstr_t)VariantValue;	

			CEnvLayer *pLayer = new CEnvLayer(i,strLayerName, strLayerPath);
			if(pLayer->getLayerID() != -1)
			{ 
				m_pProject->m_pEnvDatabase->AddLayer(pLayer);
			}
			else 
			{
				delete pLayer;
				pLayer = NULL;
				CString msg;
				msg.Format("Error in reading %s. The layer will not be added",strLayerPath);
				AfxMessageBox(msg);
				layerNum--;
			}
			pLayerList.Release();
			pLayerNode.Release();
			pLayerNameNode.Release();
			pLayerPathNode.Release();

		}
		pDatabaseNode.Release();
		pDatabaseList.Release();

		//***********Parse knowledge base node *************

		MSXML2::IXMLDOMNodePtr pKnowledgeBaseNode;
		MSXML2::IXMLDOMNodeListPtr pKnowledgeBaseList;

		MSXML2::IXMLDOMNodePtr pKnowledgeBaseFileNode;

		pRootList->get_item(1,&pKnowledgeBaseNode);

		pKnowledgeBaseNode->get_childNodes(&pKnowledgeBaseList);
		pKnowledgeBaseList->get_item(0,&pKnowledgeBaseFileNode);	
		pKnowledgeBaseFileNode->get_nodeTypedValue(&VariantValue);	
		CString strKnowledgeBaseFile =(char *)(_bstr_t)VariantValue;	

		this->m_pProject->m_strKnowledgeBaseFile = strKnowledgeBaseFile;


		pKnowledgeBaseNode.Release();
		pKnowledgeBaseList.Release();
		//m_pProjectCase->m_strCasebaseFile = strCasebaseFile;


		HRESULT hrKnowledge;	 
		MSXML2::IXMLDOMDocumentPtr pDocKnowledge;//xml document
		MSXML2::IXMLDOMElementPtr pRootKnowledgeElem;
		//MSXML2::IXMLDOMNamedNodeMapPtr pRootKnowledgeAttr;
		MSXML2::IXMLDOMNodeListPtr pRootKnowledgeBaseList;	
		_variant_t VariantValue;
		CString str;

		hrKnowledge = CoInitialize(NULL);
		if(FAILED(hrKnowledge))
		{
			AfxMessageBox("Failed to initialize COM");
			return;
		}
		//create instance
		hrKnowledge=pDocKnowledge.CreateInstance(_uuidof(MSXML2::DOMDocument));
		if(FAILED(hrKnowledge))
		{
			AfxMessageBox("Failed to create DOM Document");
			return;
		}
		//load xml doc
		_variant_t varxmlKnowledge(this->m_strProjectDir + "\\" + strKnowledgeBaseFile);
		VARIANT_BOOL varOutKnowledge;
		varOutKnowledge = pDocKnowledge->load(varxmlKnowledge);
		if(!varOutKnowledge)
		{
			AfxMessageBox("Failed to Load xml file");
			return;
		}

		pDocKnowledge->get_documentElement(&pRootKnowledgeElem);



		if(m_pProject->m_pKnowledgeBase != NULL)
		{
			delete m_pProject->m_pKnowledgeBase;
			m_pProject->m_pKnowledgeBase = NULL;
		}
		m_pProject->m_pKnowledgeBase = new CKnowledgeBase();
		//pRootKnowledgeList->get_item(1,&pKnowledgeBaseNode);	    
		long soilNum;

		pRootKnowledgeElem->get_childNodes(&pRootKnowledgeBaseList);       
		pRootKnowledgeBaseList->get_length(&soilNum);		
		//traverse child nodes of knowledge base(soiltypes)
		MSXML2::IXMLDOMNodePtr pSoilTypeNode;
		MSXML2::IXMLDOMNodeListPtr pSoilTypeList;
		MSXML2::IXMLDOMNodePtr pSoilNameNode;
		MSXML2::IXMLDOMNamedNodeMapPtr pSoilTypeAttr;
		for(i=0;i<soilNum;i++)
		{
			CSoilType *pSoilType = new CSoilType;
			pRootKnowledgeBaseList->get_item(i,&pSoilTypeNode);
			pSoilTypeNode->get_attributes(&pSoilTypeAttr);   
			pSoilNameNode = pSoilTypeAttr->getNamedItem("Name");
			pSoilNameNode->get_nodeTypedValue(&VariantValue);		
			str =(char *)(_bstr_t)VariantValue;	
			pSoilType->setSoilTypeName(str);
			MSXML2::IXMLDOMNodePtr pInstancesNode,pOccurrencesNode,pExclusionsNode;
			pSoilTypeNode->get_childNodes(&pSoilTypeList);
			pSoilTypeList->get_item(0,&pInstancesNode);
			pSoilTypeList->get_item(1,&pOccurrencesNode);
			pSoilTypeList->get_item(2,&pExclusionsNode);		 
			long instanceNum, occurrenceNum, exclusionNum;	

			MSXML2::IXMLDOMNodeListPtr pInstancesList,pOccurrencesList,pExclusionsList;
			pInstancesNode->get_childNodes(&pInstancesList);
			pOccurrencesNode->get_childNodes(&pOccurrencesList);
			pExclusionsNode->get_childNodes(&pExclusionsList);
			pInstancesList->get_length(&instanceNum);
			pOccurrencesList->get_length(&occurrenceNum);
			pExclusionsList->get_length(&exclusionNum);

			//traverse child nodes of instances (every instance)
			MSXML2::IXMLDOMNodePtr pInstanceNode;
			MSXML2::IXMLDOMNamedNodeMapPtr pInstanceAttr;
			MSXML2::IXMLDOMNodePtr pInstanceNameNode;
			MSXML2::IXMLDOMNodeListPtr pInstanceList;
			for(int n1=0;n1<instanceNum;n1++)
			{
				CInstance *pInstance = new CInstance;			
				pInstancesList->get_item(n1,&pInstanceNode);
				pInstanceNode->get_attributes(&pInstanceAttr);
				pInstanceNameNode = pInstanceAttr->getNamedItem("Name");
				pInstanceNameNode->get_nodeTypedValue(&VariantValue);	
				str =(char *)(_bstr_t)VariantValue;	
				pInstance->setInstanceName(str);
				long ruleNum;			
				pInstanceNode->get_childNodes(&pInstanceList);
				pInstanceList->get_length(&ruleNum);

				//travers child nodes of instance ( rule node )
				CString  strRuleName,strRuleUseOrNot,strRuleType,strRuleLayer;
				for(long n11=0;n11<ruleNum;n11++)
				{
					MSXML2::IXMLDOMNodePtr pRuleNode;
					MSXML2::IXMLDOMNamedNodeMapPtr pRuleAttr;				
					MSXML2::IXMLDOMNodePtr pRuleNameNode,pRuleUseOrNotNode,pRuleTypeNode,pRuleLayerNode;
					MSXML2::IXMLDOMNodeListPtr pRuleList;
					pInstanceList->get_item(n11,&pRuleNode);				
					pRuleNode->get_attributes(&pRuleAttr);
					pRuleNameNode = pRuleAttr->getNamedItem("Name");
					pRuleUseOrNotNode = pRuleAttr->getNamedItem("UseOrNot");
					pRuleTypeNode = pRuleAttr->getNamedItem("Type");
					pRuleLayerNode = pRuleAttr->getNamedItem("DataLayer");
					pRuleNameNode->get_nodeTypedValue(&VariantValue);	
					strRuleName =(char *)(_bstr_t)VariantValue;	
					pRuleUseOrNotNode->get_nodeTypedValue(&VariantValue);	
					strRuleUseOrNot =(char *)(_bstr_t)VariantValue;	
					pRuleTypeNode->get_nodeTypedValue(&VariantValue);	
					strRuleType =(char *)(_bstr_t)VariantValue;	
					pRuleLayerNode->get_nodeTypedValue(&VariantValue);	
					strRuleLayer =(char *)(_bstr_t)VariantValue;				
					pRuleNode->get_childNodes(&pRuleList);
					if(strRuleType == "Range")
					{
						CRangeRule *pRule= new CRangeRule;
						pRule->setRuleType(0);
						pRule->setRuleName(strRuleName);
						if(strRuleUseOrNot == "true")
							pRule->setIsUsed(true);
						else 
							pRule->setIsUsed(false);
						pRule->setLayerName(strRuleLayer);	
						MSXML2::IXMLDOMNodePtr pCurveTypeNode,pKeyValueNode;
						pRuleList->get_item(0,&pCurveTypeNode);
						pRuleList->get_item(1,&pKeyValueNode);
						pCurveTypeNode->get_nodeTypedValue(&VariantValue);	
						CString strCurveType =(char *)(_bstr_t)VariantValue;	
						pKeyValueNode->get_nodeTypedValue(&VariantValue);	
						CString strKeyValue =(char *)(_bstr_t)VariantValue;	
						double KeyValue[4] ;
						split1(strKeyValue,KeyValue);
						pRule->setLowCross(KeyValue[0]);
						pRule->setLowUnity(KeyValue[1]);
						pRule->setHighUnity(KeyValue[2]);
						pRule->setHighCross(KeyValue[3]);   
						if(strCurveType == "Bell-shape")
							pRule->setCurveType(0);
						else if(strCurveType == "S-shape")
							pRule->setCurveType(1);
						else if(strCurveType == "Z-shape")
							pRule->setCurveType(2);
						pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
						pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());
						pRule->setDispMin(pRule->ComputeRuleGraphMin());
						pRule->setDispMax(pRule->ComputeRuleGraphMax());
						pInstance->AddRule((CRule *)pRule);
						pCurveTypeNode.Release();
						pKeyValueNode.Release();
					}
					else if( strRuleType == "Curve")
					{
						CFreehandRule *pRule = new CFreehandRule;
						pRule->setRuleType(1);
						pRule->setRuleName(strRuleName);
						if(strRuleUseOrNot == "true")
							pRule->setIsUsed(true);
						else 
							pRule->setIsUsed(false);

						pRule->setLayerName(strRuleLayer);
						MSXML2::IXMLDOMNodePtr pNodeNum,pCoordinatesNode;
						pRuleList->get_item(0,&pNodeNum);
						pRuleList->get_item(1,&pCoordinatesNode);
						pNodeNum->get_nodeTypedValue(&VariantValue);	
						CString strNodeNum =(char *)(_bstr_t)VariantValue;	
						int NodeNum = atoi(strNodeNum);
						pCoordinatesNode->get_nodeTypedValue(&VariantValue);	
						CString strCoordinates =(char *)(_bstr_t)VariantValue;	
						double NodeX[100];
						double NodeY[100];
						split1(strCoordinates,NodeX, NodeY);
						if(NodeNum>100)
						{
							AfxMessageBox("The number of nodes must be smaller than 100");
							delete pRule;
							pRule = NULL;
							continue;

						}
						for(int i=0;i<NodeNum; i++)				
							pRule->AddKnot(NodeX[i],NodeY[i]);		
						pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
						pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());
						pRule->setDispMin(pRule->ComputeRuleGraphMin());
						pRule->setDispMax(pRule->ComputeRuleGraphMax());
						pInstance->AddRule((CRule *)pRule);
						pNodeNum.Release();
						pCoordinatesNode.Release();
					}
					else if(strRuleType == "Word")
					{
						CWordRule *pRule= new CWordRule;
						pRule->setRuleType(2);
						pRule->setRuleName(strRuleName);
						if(strRuleUseOrNot == "true")
							pRule->setIsUsed(true);
						else 
							pRule->setIsUsed(false);
						pRule->setLayerName(strRuleLayer);	
						MSXML2::IXMLDOMNodePtr pPraseNode, pEnvAttriNode,pWordLibNode;
						pRuleList->get_item(0,&pPraseNode);
						pRuleList->get_item(1,&pEnvAttriNode);
						pRuleList->get_item(2,&pWordLibNode);
						pPraseNode->get_nodeTypedValue(&VariantValue);	
						CString strPrase =(char *)(_bstr_t)VariantValue;	
						pRule->setPhrase(strPrase);
						pEnvAttriNode->get_nodeTypedValue(&VariantValue);	
						CString strEnvAttri =(char *)(_bstr_t)VariantValue;	
						pRule->setEnvAttri(strEnvAttri);
						pWordLibNode->get_nodeTypedValue(&VariantValue);
						CString strWordLib = (char *)(_bstr_t)VariantValue;
						strWordLib = m_strProjectDir +"\\"+strWordLib;
						pRule->setLibFileName(strWordLib);
						pRule->LoadWordLib();
						pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
						pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());
						pRule->setDispMin(pRule->ComputeRuleGraphMin());
						pRule->setDispMax(pRule->ComputeRuleGraphMax());
						pInstance->AddRule((CRule *)pRule);
						pPraseNode.Release();
						pWordLibNode.Release();
					}
					else if(strRuleType == "Point")
					{
						CPointRule *pRule= new CPointRule;
						pRule->setRuleType(3);
						pRule->setRuleName(strRuleName);
						if(strRuleUseOrNot == "true")
							pRule->setIsUsed(true);
						else 
							pRule->setIsUsed(false);
						pRule->setLayerName(strRuleLayer);	
						MSXML2::IXMLDOMNodePtr pXYNode,pCurveTypeNode,pLWidthNode,pRWidthNode;
						pRuleList->get_item(0,&pXYNode);
						pRuleList->get_item(1,&pCurveTypeNode);
						pRuleList->get_item(2,&pLWidthNode);
						pRuleList->get_item(3,&pRWidthNode);
						pXYNode->get_nodeTypedValue(&VariantValue);	
						CString strXY =(char *)(_bstr_t)VariantValue;	
						pCurveTypeNode->get_nodeTypedValue(&VariantValue);	
						CString strCurveType =(char *)(_bstr_t)VariantValue;
						pLWidthNode->get_nodeTypedValue(&VariantValue);	
						CString strLWidth =(char *)(_bstr_t)VariantValue;
						pRWidthNode->get_nodeTypedValue(&VariantValue);	
						CString strRWidth =(char *)(_bstr_t)VariantValue;
						double X[1],Y[1];
						split1(strXY, X,Y);					
						pRule->setCentralX(X[0]);
						pRule->setCentralY(Y[0]);
						if(strCurveType == "Bell-shape")
							pRule->setCurveType(0);
						else if(strCurveType == "S-shape")
							pRule->setCurveType(1);
						else if(strCurveType == "Z-shape")
							pRule->setCurveType(2);
						pRule->setLWidth(atof(strLWidth));
						pRule->setRWidth(atof(strRWidth));
						pRule->ComputeCentralValue(m_pProject->m_pEnvDatabase);
						pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
						pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());
						pRule->setDispMin(pRule->ComputeRuleGraphMin());
						pRule->setDispMax(pRule->ComputeRuleGraphMax());
						pInstance->AddRule((CRule *)pRule);
						pXYNode.Release();
						pCurveTypeNode.Release();
						pLWidthNode.Release();
						pRWidthNode.Release();
					}		
					else if(strRuleType ==  "Enumerated")
					{
						CEnumRule *pRule= new CEnumRule;
						pRule->setRuleType(4);
						pRule->setRuleName(strRuleName);
						if(strRuleUseOrNot == "true")
							pRule->setIsUsed(true);
						else 
							pRule->setIsUsed(false);
						pRule->setLayerName(strRuleLayer);		
						MSXML2::IXMLDOMNodePtr pItemNum, pEnumValueNode;
						pRuleList->get_item(0,&pItemNum);
						pRuleList->get_item(1,&pEnumValueNode);
						pItemNum->get_nodeTypedValue(&VariantValue);	
						CString strItemNum =(char *)(_bstr_t)VariantValue;	
						int ItemNum = atoi(strItemNum);
						if(ItemNum>100)
						{
							AfxMessageBox("The number of items must be smaller than 100");
							delete pRule;
							pRule = NULL;
							continue;
						}
						pEnumValueNode->get_nodeTypedValue(&VariantValue);	
						CString strItems =(char *)(_bstr_t)VariantValue;
						int Items[100];
						split1(strItems, Items);
						for(int i= 0;i<ItemNum;i++)			
							pRule->AddItem(Items[i]);	

						pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
						pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

						pRule->setDispMin(pRule->ComputeRuleGraphMin());
						pRule->setDispMax(pRule->ComputeRuleGraphMax());

						pInstance->AddRule((CRule *)pRule);
						pItemNum.Release();
						pEnumValueNode.Release();
					}	


					pRuleNode.Release();
					pRuleAttr.Release();				
					pRuleNameNode.Release();
					pRuleUseOrNotNode.Release();
					pRuleTypeNode.Release();
					pRuleLayerNode.Release();
					pRuleList.Release();
				}
				pSoilType->AddInstance(pInstance);
				pInstanceNode.Release();
				pInstanceAttr.Release();
				pInstanceNameNode.Release();
				pInstanceList.Release();
			}

			pInstancesList.Release();
			pInstancesNode.Release();


			//traver child node of occurrences(every occurrences)
			MSXML2::IXMLDOMNodePtr pOccurrenceNode;
			MSXML2::IXMLDOMNamedNodeMapPtr pOccurrenceAttr;
			MSXML2::IXMLDOMNodePtr pOccurrenceNameNode,pOccurrenceXNode,pOccurrenceYNode,pOccurrenceUseDistanceDecayNode,pOccurrenceSearchDistNode,pOccurrenceDistanceDecayNode;
			MSXML2::IXMLDOMNodeListPtr pOccurrenceList;
			for(int n2=0;n2<occurrenceNum;n2++)
			{
				COccurrence *pOccurrence = new COccurrence;
				pOccurrencesList->get_item(n2,&pOccurrenceNode);
				pOccurrenceNode->get_attributes(&pOccurrenceAttr);
				pOccurrenceNameNode = pOccurrenceAttr->getNamedItem("Name");
				pOccurrenceXNode = pOccurrenceAttr->getNamedItem("x");
				pOccurrenceYNode = pOccurrenceAttr->getNamedItem("y");			
				pOccurrenceSearchDistNode = pOccurrenceAttr->getNamedItem("SearchDist");
				pOccurrenceUseDistanceDecayNode = pOccurrenceAttr->getNamedItem("UseDistanceDecay");
				pOccurrenceDistanceDecayNode = pOccurrenceAttr->getNamedItem("DistanceDecay");
				pOccurrenceNameNode->get_nodeTypedValue(&VariantValue);	
				CString strOccurrenceName =(char *)(_bstr_t)VariantValue;	
				pOccurrence->setOccurrenceName(strOccurrenceName);	
				pOccurrenceXNode->get_nodeTypedValue(&VariantValue);	
				CString strOccurrenceX =(char *)(_bstr_t)VariantValue;
				pOccurrence->setCentralX(atof(strOccurrenceX));
				pOccurrenceYNode->get_nodeTypedValue(&VariantValue);	
				CString strOccurrenceY =(char *)(_bstr_t)VariantValue;
				pOccurrence->setCentralY(atof(strOccurrenceY));			
				pOccurrenceSearchDistNode->get_nodeTypedValue(&VariantValue);	
				CString strOccurrenceSearchDist =(char *)(_bstr_t)VariantValue;
				pOccurrence->setSearchDist(atof(strOccurrenceSearchDist));
				pOccurrenceUseDistanceDecayNode ->get_nodeTypedValue(&VariantValue);	
				CString strOccurrenceUseDistDecay =(char *)(_bstr_t)VariantValue;
				if(strOccurrenceUseDistDecay == "true")
				{
					pOccurrence->setUseDistSim(true);
				}
				else
				{
					pOccurrence->setUseDistSim(false);
				}
				pOccurrenceDistanceDecayNode->get_nodeTypedValue(&VariantValue);	
				CString strOccurrenceDistanceDecay =(char *)(_bstr_t)VariantValue;
				pOccurrence->setDistDecayFactor(atof(strOccurrenceDistanceDecay));
				pOccurrenceNode->get_childNodes(&pOccurrenceList);
				long ruleNum;
				pOccurrenceList->get_length(&ruleNum);	
				//traver child nodes of occurrence(every rule)
				CString  strRuleName,strRuleUseOrNot,strRuleType,strRuleLayer;			
				for(long n11=0;n11<ruleNum;n11++)
				{
					MSXML2::IXMLDOMNodePtr pRuleNode;
					MSXML2::IXMLDOMNamedNodeMapPtr pRuleAttr;
					MSXML2::IXMLDOMNodePtr pRuleNameNode,pRuleUseOrNotNode,pRuleTypeNode,pRuleLayerNode;
					MSXML2::IXMLDOMNodeListPtr pRuleList;

					pOccurrenceList->get_item(n11,&pRuleNode);				
					pRuleNode->get_attributes(&pRuleAttr);				
					pRuleNameNode = pRuleAttr->getNamedItem("Name");
					pRuleUseOrNotNode = pRuleAttr->getNamedItem("UseOrNot");
					pRuleTypeNode = pRuleAttr->getNamedItem("Type");
					pRuleLayerNode = pRuleAttr->getNamedItem("DataLayer");
					pRuleNameNode->get_nodeTypedValue(&VariantValue);	
					strRuleName =(char *)(_bstr_t)VariantValue;	
					pRuleUseOrNotNode->get_nodeTypedValue(&VariantValue);	
					strRuleUseOrNot =(char *)(_bstr_t)VariantValue;	
					pRuleTypeNode->get_nodeTypedValue(&VariantValue);	
					strRuleType =(char *)(_bstr_t)VariantValue;	
					pRuleLayerNode->get_nodeTypedValue(&VariantValue);	
					strRuleLayer =(char *)(_bstr_t)VariantValue;

					pRuleNode->get_childNodes(&pRuleList);
					if(strRuleType == "Range")
					{
						CRangeRule *pRule= new CRangeRule;
						pRule->setRuleType(0);
						pRule->setRuleName(strRuleName);
						if(strRuleUseOrNot == "true")
							pRule->setIsUsed(true);
						else 
							pRule->setIsUsed(false);
						pRule->setLayerName(strRuleLayer);
						MSXML2::IXMLDOMNodePtr pCurveTypeNode,pKeyValueNode;
						pRuleList->get_item(0,&pCurveTypeNode);
						pRuleList->get_item(1,&pKeyValueNode);
						pCurveTypeNode->get_nodeTypedValue(&VariantValue);	
						CString strCurveType =(char *)(_bstr_t)VariantValue;	
						pKeyValueNode->get_nodeTypedValue(&VariantValue);	
						CString strKeyValue =(char *)(_bstr_t)VariantValue;	
						double KeyValue[4] ;
						split1(strKeyValue,KeyValue);
						pRule->setLowCross(KeyValue[0]);
						pRule->setLowUnity(KeyValue[1]);
						pRule->setHighUnity(KeyValue[2]);
						pRule->setHighCross(KeyValue[3]);   
						if(strCurveType == "Bell-shape")
							pRule->setCurveType(0);
						else if(strCurveType == "S-shape")
							pRule->setCurveType(1);
						else if(strCurveType == "Z-shape")
							pRule->setCurveType(2);

						pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
						pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

						pRule->setDispMin(pRule->ComputeRuleGraphMin());
						pRule->setDispMax(pRule->ComputeRuleGraphMax());
						pOccurrence->AddRule((CRule *)pRule);
						pCurveTypeNode.Release();
						pKeyValueNode.Release();
					}
					else if( strRuleType == "Curve")
					{
						CFreehandRule *pRule = new CFreehandRule;
						pRule->setRuleType(1);
						pRule->setRuleName(strRuleName);
						if(strRuleUseOrNot == "true")
							pRule->setIsUsed(true);
						else 
							pRule->setIsUsed(false);

						pRule->setLayerName(strRuleLayer);	

						MSXML2::IXMLDOMNodePtr pNodeNum,pCoordinatesNode;
						pRuleList->get_item(0,&pNodeNum);
						pRuleList->get_item(1,&pCoordinatesNode);
						pNodeNum->get_nodeTypedValue(&VariantValue);	
						CString strNodeNum =(char *)(_bstr_t)VariantValue;	
						int NodeNum = atoi(strNodeNum);

						pCoordinatesNode->get_nodeTypedValue(&VariantValue);	
						CString strCoordinates =(char *)(_bstr_t)VariantValue;	
						double NodeX[100];
						double NodeY[100];
						split1(strCoordinates,NodeX, NodeY);
						if(NodeNum>100)
						{
							AfxMessageBox("The number of nodes must be smaller than 100");
							delete pRule;
							pRule = NULL;
							continue;

						}
						for(int i=0;i<NodeNum; i++)				
							pRule->AddKnot(NodeX[i],NodeY[i]);

						pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
						pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

						pRule->setDispMin(pRule->ComputeRuleGraphMin());
						pRule->setDispMax(pRule->ComputeRuleGraphMax());

						pOccurrence->AddRule((CRule *)pRule);
						pNodeNum.Release();
						pCoordinatesNode.Release();
					}
					else if(strRuleType == "Word")
					{
						CWordRule *pRule= new CWordRule;
						pRule->setRuleType(2);
						pRule->setRuleName(strRuleName);
						if(strRuleUseOrNot == "true")
							pRule->setIsUsed(true);
						else 
							pRule->setIsUsed(false);
						pRule->setLayerName(strRuleLayer);	

						MSXML2::IXMLDOMNodePtr pPraseNode, pEnvAttriNode,pWordLibNode;

						pRuleList->get_item(0,&pPraseNode);
						pRuleList->get_item(1,&pEnvAttriNode);
						pRuleList->get_item(2,&pWordLibNode);

						pPraseNode->get_nodeTypedValue(&VariantValue);	
						CString strPrase =(char *)(_bstr_t)VariantValue;	
						pRule->setPhrase(strPrase);
						pEnvAttriNode->get_nodeTypedValue(&VariantValue);	
						CString strEnvAttri =(char *)(_bstr_t)VariantValue;	
						pRule->setEnvAttri(strEnvAttri);
						pWordLibNode->get_nodeTypedValue(&VariantValue);
						CString strWordLib = (char *)(_bstr_t)VariantValue;


						strWordLib = m_strProjectDir +"\\"+strWordLib;
						pRule->setLibFileName(strWordLib);
						pRule->LoadWordLib();

						pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
						pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

						pRule->setDispMin(pRule->ComputeRuleGraphMin());
						pRule->setDispMax(pRule->ComputeRuleGraphMax());
						pOccurrence->AddRule((CRule *)pRule);
						pPraseNode.Release();
						pWordLibNode.Release();
					}
					else if(strRuleType == "Point")
					{
						CPointRule *pRule= new CPointRule;
						pRule->setRuleType(3);
						pRule->setRuleName(strRuleName);
						if(strRuleUseOrNot == "true")
							pRule->setIsUsed(true);
						else 
							pRule->setIsUsed(false);
						pRule->setLayerName(strRuleLayer);	

						MSXML2::IXMLDOMNodePtr pXYNode,pCurveTypeNode,pLWidthNode,pRWidthNode;
						pRuleList->get_item(0,&pXYNode);
						pRuleList->get_item(1,&pCurveTypeNode);
						pRuleList->get_item(2,&pLWidthNode);
						pRuleList->get_item(3,&pRWidthNode);
						pXYNode->get_nodeTypedValue(&VariantValue);	
						CString strXY =(char *)(_bstr_t)VariantValue;	
						pCurveTypeNode->get_nodeTypedValue(&VariantValue);	
						CString strCurveType =(char *)(_bstr_t)VariantValue;
						pLWidthNode->get_nodeTypedValue(&VariantValue);	
						CString strLWidth =(char *)(_bstr_t)VariantValue;
						pRWidthNode->get_nodeTypedValue(&VariantValue);	
						CString strRWidth =(char *)(_bstr_t)VariantValue;
						double X[1],Y[1];
						split1(strXY, X,Y);					
						pRule->setCentralX(X[0]);
						pRule->setCentralY(Y[0]);
						if(strCurveType = "Bell-shape")
							pRule->setCurveType(0);
						else if(strCurveType = "S-shape")
							pRule->setCurveType(1);
						else if(strCurveType = "Z-shape")
							pRule->setCurveType(2);
						pRule->setLWidth(atof(strLWidth));
						pRule->setRWidth(atof(strRWidth));

						pRule->ComputeCentralValue(m_pProject->m_pEnvDatabase);

						pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
						pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

						pRule->setDispMin(pRule->ComputeRuleGraphMin());
						pRule->setDispMax(pRule->ComputeRuleGraphMax());

						pOccurrence->AddRule((CRule *)pRule);
						pXYNode.Release();
						pCurveTypeNode.Release();
						pLWidthNode.Release();
						pRWidthNode.Release();

					}		
					else if(strRuleType ==  "Enumerated")
					{
						CEnumRule *pRule= new CEnumRule;
						pRule->setRuleType(4);
						pRule->setRuleName(strRuleName);
						if(strRuleUseOrNot == "true")
							pRule->setIsUsed(true);
						else 
							pRule->setIsUsed(false);
						pRule->setLayerName(strRuleLayer);	

						MSXML2::IXMLDOMNodePtr pItemNum, pEnumValueNode;

						pRuleList->get_item(0,&pItemNum);
						pRuleList->get_item(1,&pEnumValueNode);
						pItemNum->get_nodeTypedValue(&VariantValue);	
						CString strItemNum =(char *)(_bstr_t)VariantValue;	
						int ItemNum = atoi(strItemNum);
						if(ItemNum>100)
						{
							AfxMessageBox("The number of items must be smaller than 100");
							delete pRule;
							pRule = NULL;
							continue;

						}
						pEnumValueNode->get_nodeTypedValue(&VariantValue);	
						CString strItems =(char *)(_bstr_t)VariantValue;
						int Items[100];
						split1(strItems, Items);
						for(int i= 0;i<ItemNum;i++)			
							pRule->AddItem(Items[i]);	

						pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
						pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

						pRule->setDispMin(pRule->ComputeRuleGraphMin());
						pRule->setDispMax(pRule->ComputeRuleGraphMax());					
						pOccurrence->AddRule((CRule *)pRule);
						pItemNum.Release();
						pEnumValueNode.Release();
					}	


					pRuleNode.Release();
					pRuleAttr.Release();
					pRuleNameNode.Release();
					pRuleUseOrNotNode.Release();
					pRuleTypeNode.Release();
					pRuleLayerNode.Release();
					pRuleList.Release();
				}
				pSoilType->AddOccurrence(pOccurrence);
				pOccurrenceNode.Release();
				pOccurrenceAttr.Release();
				pOccurrenceNameNode.Release();
				pOccurrenceXNode.Release();
				pOccurrenceYNode.Release();
				pOccurrenceUseDistanceDecayNode.Release();
				pOccurrenceSearchDistNode.Release();
				pOccurrenceDistanceDecayNode.Release();
				pOccurrenceList.Release();
			}

			pOccurrencesList.Release();
			pOccurrencesNode.Release();

			//traverse child nodes of exclusios(every exclusion)
			MSXML2::IXMLDOMNodePtr pExclusionNode;
			MSXML2::IXMLDOMNamedNodeMapPtr pExclusionAttr;
			MSXML2::IXMLDOMNodePtr pExclusionNameNode,pExclusionXNode,pExclusionYNode,pExclusionUseDistanceDecayNode,pExclusionSearchDistNode,pExclusionDistanceDecayNode;
			MSXML2::IXMLDOMNodeListPtr pExclusionList;
			for(int n2=0;n2<exclusionNum;n2++)
			{
				CExclusion *pExclusion = new CExclusion;
				pExclusionsList->get_item(n2,&pExclusionNode);
				pExclusionNode->get_attributes(&pExclusionAttr);
				pExclusionNameNode = pExclusionAttr->getNamedItem("Name");
				pExclusionXNode = pExclusionAttr->getNamedItem("x");
				pExclusionYNode = pExclusionAttr->getNamedItem("y");
				pExclusionUseDistanceDecayNode = pExclusionAttr->getNamedItem("UseDistanceDecay");
				pExclusionSearchDistNode = pExclusionAttr->getNamedItem("SearchDist");
				pExclusionDistanceDecayNode = pExclusionAttr->getNamedItem("DistanceDecay");
				pExclusionNameNode->get_nodeTypedValue(&VariantValue);	
				CString strExclusionName =(char *)(_bstr_t)VariantValue;	
				pExclusion->setExclusionName(strExclusionName);

				pExclusionXNode->get_nodeTypedValue(&VariantValue);	
				CString strExclusionX =(char *)(_bstr_t)VariantValue;
				pExclusion->setCentralX(atof(strExclusionX));
				pExclusionYNode->get_nodeTypedValue(&VariantValue);	
				CString strExclusionY =(char *)(_bstr_t)VariantValue;
				pExclusion->setCentralY(atof(strExclusionY));			
				pExclusionSearchDistNode->get_nodeTypedValue(&VariantValue);	
				CString strExclusionSearchDist =(char *)(_bstr_t)VariantValue;
				pExclusion->setSearchDist(atof(strExclusionSearchDist));
				pExclusionUseDistanceDecayNode ->get_nodeTypedValue(&VariantValue);	
				CString strExclusionUseDistanceDecay =(char *)(_bstr_t)VariantValue;
				if(strExclusionUseDistanceDecay == "true")
				{
					pExclusion->setUseDistSim(true);
				}
				else
				{
					pExclusion->setUseDistSim(false);
				}
				pExclusionDistanceDecayNode->get_nodeTypedValue(&VariantValue);	
				CString strExclusionDistanceDecay =(char *)(_bstr_t)VariantValue;
				pExclusion->setDistDecayFactor(atof(strExclusionDistanceDecay));


				pExclusionNode->get_childNodes(&pExclusionList);
				long ruleNum;
				pExclusionList->get_length(&ruleNum);			
				//traver child nodes of exclusion( every rule)

				CString  strRuleName,strRuleUseOrNot,strRuleType,strRuleLayer;			
				for(long n11=0;n11<ruleNum;n11++)
				{
					MSXML2::IXMLDOMNodePtr pRuleNode;
					MSXML2::IXMLDOMNamedNodeMapPtr pRuleAttr;
					MSXML2::IXMLDOMNodePtr pRuleNameNode,pRuleUseOrNotNode,pRuleTypeNode,pRuleLayerNode;
					MSXML2::IXMLDOMNodeListPtr pRuleList;

					pExclusionList->get_item(n11,&pRuleNode);				
					pRuleNode->get_attributes(&pRuleAttr);				
					pRuleNameNode = pRuleAttr->getNamedItem("Name");
					pRuleUseOrNotNode = pRuleAttr->getNamedItem("UseOrNot");
					pRuleTypeNode = pRuleAttr->getNamedItem("Type");
					pRuleLayerNode = pRuleAttr->getNamedItem("DataLayer");
					pRuleNameNode->get_nodeTypedValue(&VariantValue);	
					strRuleName =(char *)(_bstr_t)VariantValue;	
					pRuleUseOrNotNode->get_nodeTypedValue(&VariantValue);	
					strRuleUseOrNot =(char *)(_bstr_t)VariantValue;	
					pRuleTypeNode->get_nodeTypedValue(&VariantValue);	
					strRuleType =(char *)(_bstr_t)VariantValue;	
					pRuleLayerNode->get_nodeTypedValue(&VariantValue);	
					strRuleLayer =(char *)(_bstr_t)VariantValue;

					pRuleNode->get_childNodes(&pRuleList);

					if(strRuleType == "Range")
					{
						CRangeRule *pRule= new CRangeRule;
						pRule->setRuleType(0);
						pRule->setRuleName(strRuleName);
						if(strRuleUseOrNot == "true")
							pRule->setIsUsed(true);
						else 
							pRule->setIsUsed(false);
						pRule->setLayerName(strRuleLayer);	

						MSXML2::IXMLDOMNodePtr pCurveTypeNode,pKeyValueNode;
						pRuleList->get_item(0,&pCurveTypeNode);
						pRuleList->get_item(1,&pKeyValueNode);
						pCurveTypeNode->get_nodeTypedValue(&VariantValue);	
						CString strCurveType =(char *)(_bstr_t)VariantValue;	
						pKeyValueNode->get_nodeTypedValue(&VariantValue);	
						CString strKeyValue =(char *)(_bstr_t)VariantValue;	
						double KeyValue[4] ;
						split1(strKeyValue,KeyValue);
						pRule->setLowCross(KeyValue[0]);
						pRule->setLowUnity(KeyValue[1]);
						pRule->setHighUnity(KeyValue[2]);
						pRule->setHighCross(KeyValue[3]);   
						if(strCurveType = "Bell-shape")
							pRule->setCurveType(0);
						else if(strCurveType = "S-shape")
							pRule->setCurveType(1);
						else if(strCurveType = "Z-shape")
							pRule->setCurveType(2);

						pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
						pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

						pRule->setDispMin(pRule->ComputeRuleGraphMin());
						pRule->setDispMax(pRule->ComputeRuleGraphMax());
						pExclusion->AddRule((CRule *)pRule);
						pCurveTypeNode.Release();
						pKeyValueNode.Release();
					}
					else if( strRuleType == "Curve")
					{
						CFreehandRule *pRule = new CFreehandRule;
						pRule->setRuleType(1);
						pRule->setRuleName(strRuleName);
						if(strRuleUseOrNot == "true")
							pRule->setIsUsed(true);
						else 
							pRule->setIsUsed(false);

						pRule->setLayerName(strRuleLayer);	

						MSXML2::IXMLDOMNodePtr pNodeNum,pCoordinatesNode;
						pRuleList->get_item(0,&pNodeNum);
						pRuleList->get_item(1,&pCoordinatesNode);
						pNodeNum->get_nodeTypedValue(&VariantValue);	
						CString strNodeNum =(char *)(_bstr_t)VariantValue;	
						int NodeNum = atoi(strNodeNum);

						pCoordinatesNode->get_nodeTypedValue(&VariantValue);	
						CString strCoordinates =(char *)(_bstr_t)VariantValue;	
						double NodeX[100];
						double NodeY[100];
						split1(strCoordinates,NodeX, NodeY);
						if(NodeNum>100)
						{
							AfxMessageBox("The number of nodes must be smaller than 100");
							delete pRule;
							pRule = NULL;
							continue;

						}
						for(int i=0;i<NodeNum; i++)				
							pRule->AddKnot(NodeX[i],NodeY[i]);

						pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
						pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

						pRule->setDispMin(pRule->ComputeRuleGraphMin());
						pRule->setDispMax(pRule->ComputeRuleGraphMax());
						pExclusion->AddRule((CRule *)pRule);
						pNodeNum.Release();
						pCoordinatesNode.Release();
					}
					else if(strRuleType == "Word")
					{
						CWordRule *pRule= new CWordRule;
						pRule->setRuleType(2);
						pRule->setRuleName(strRuleName);
						if(strRuleUseOrNot == "true")
							pRule->setIsUsed(true);
						else 
							pRule->setIsUsed(false);
						pRule->setLayerName(strRuleLayer);	

						MSXML2::IXMLDOMNodePtr pPraseNode, pEnvAttriNode,pWordLibNode;

						pRuleList->get_item(0,&pPraseNode);
						pRuleList->get_item(1,&pEnvAttriNode);
						pRuleList->get_item(2,&pWordLibNode);

						pPraseNode->get_nodeTypedValue(&VariantValue);	
						CString strPrase =(char *)(_bstr_t)VariantValue;	
						pRule->setPhrase(strPrase);
						pEnvAttriNode->get_nodeTypedValue(&VariantValue);	
						CString strEnvAttri =(char *)(_bstr_t)VariantValue;	
						pRule->setEnvAttri(strEnvAttri);
						pWordLibNode->get_nodeTypedValue(&VariantValue);
						CString strWordLib = (char *)(_bstr_t)VariantValue;

						strWordLib = m_strProjectDir +"\\"+strWordLib;
						pRule->setLibFileName(strWordLib);
						pRule->LoadWordLib();

						pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
						pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

						pRule->setDispMin(pRule->ComputeRuleGraphMin());
						pRule->setDispMax(pRule->ComputeRuleGraphMax());
						pExclusion->AddRule((CRule *)pRule);
						pPraseNode.Release();
						pWordLibNode.Release();
					}
					else if(strRuleType == "Point")
					{
						CPointRule *pRule= new CPointRule;
						pRule->setRuleType(3);
						pRule->setRuleName(strRuleName);
						if(strRuleUseOrNot == "true")
							pRule->setIsUsed(true);
						else 
							pRule->setIsUsed(false);
						pRule->setLayerName(strRuleLayer);	

						MSXML2::IXMLDOMNodePtr pXYNode,pCurveTypeNode,pLWidthNode,pRWidthNode;
						pRuleList->get_item(0,&pXYNode);
						pRuleList->get_item(1,&pCurveTypeNode);
						pRuleList->get_item(2,&pLWidthNode);
						pRuleList->get_item(3,&pRWidthNode);
						pXYNode->get_nodeTypedValue(&VariantValue);	
						CString strXY =(char *)(_bstr_t)VariantValue;	
						pCurveTypeNode->get_nodeTypedValue(&VariantValue);	
						CString strCurveType =(char *)(_bstr_t)VariantValue;
						pLWidthNode->get_nodeTypedValue(&VariantValue);	
						CString strLWidth =(char *)(_bstr_t)VariantValue;
						pRWidthNode->get_nodeTypedValue(&VariantValue);	
						CString strRWidth =(char *)(_bstr_t)VariantValue;
						double X[1],Y[1];
						split1(strXY, X,Y);					
						pRule->setCentralX(X[0]);
						pRule->setCentralY(Y[0]);
						if(strCurveType = "Bell-shape")
							pRule->setCurveType(0);
						else if(strCurveType = "S-shape")
							pRule->setCurveType(1);
						else if(strCurveType = "Z-shape")
							pRule->setCurveType(2);
						pRule->setLWidth(atof(strLWidth));
						pRule->setRWidth(atof(strRWidth));

						pRule->ComputeCentralValue(m_pProject->m_pEnvDatabase);

						pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
						pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

						pRule->setDispMin(pRule->ComputeRuleGraphMin());
						pRule->setDispMax(pRule->ComputeRuleGraphMax());
						pExclusion->AddRule((CRule *)pRule);
						pXYNode.Release();
						pCurveTypeNode.Release();
						pLWidthNode.Release();
						pRWidthNode.Release();
					}		
					else if(strRuleType ==  "Enumerated")
					{
						CEnumRule *pRule= new CEnumRule;
						pRule->setRuleType(4);
						pRule->setRuleName(strRuleName);
						if(strRuleUseOrNot == "true")
							pRule->setIsUsed(true);
						else 
							pRule->setIsUsed(false);
						pRule->setLayerName(strRuleLayer);	

						MSXML2::IXMLDOMNodePtr pItemNum, pEnumValueNode;

						pRuleList->get_item(0,&pItemNum);
						pRuleList->get_item(1,&pEnumValueNode);
						pItemNum->get_nodeTypedValue(&VariantValue);	
						CString strItemNum =(char *)(_bstr_t)VariantValue;	
						int ItemNum = atoi(strItemNum);
						if(ItemNum>100)
						{
							AfxMessageBox("The number of items must be smaller than 100");
							delete pRule;
							pRule = NULL;
							continue;

						}
						pEnumValueNode->get_nodeTypedValue(&VariantValue);	
						CString strItems =(char *)(_bstr_t)VariantValue;
						int Items[100];
						split1(strItems, Items);
						for(int i= 0;i<ItemNum;i++)			
							pRule->AddItem(Items[i]);	

						pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
						pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

						pRule->setDispMin(pRule->ComputeRuleGraphMin());
						pRule->setDispMax(pRule->ComputeRuleGraphMax());					
						pExclusion->AddRule((CRule *)pRule);
						pItemNum.Release();
						pEnumValueNode.Release();

					}
					pRuleNode.Release();
					pRuleAttr.Release();
					pRuleNameNode.Release();
					pRuleUseOrNotNode.Release();
					pRuleTypeNode.Release();
					pRuleLayerNode.Release();
					pRuleList.Release();
				}
				pSoilType->AddExclusion(pExclusion);
				pExclusionNode.Release();
				pExclusionAttr.Release();
				pExclusionNameNode.Release();
				pExclusionXNode.Release();
				pExclusionYNode.Release();
				pExclusionUseDistanceDecayNode.Release();
				pExclusionSearchDistNode.Release();
				pExclusionDistanceDecayNode.Release();
				pExclusionList.Release();
			}

			pExclusionsList.Release();
			pExclusionsNode.Release();
			m_pProject->m_pKnowledgeBase->AddSoilType(pSoilType);

			//release the soil nodes
			pSoilTypeNode.Release();
			pSoilTypeList.Release();
			pSoilNameNode.Release();
			pSoilTypeAttr.Release();
		}




		pRootKnowledgeBaseList.Release();

		pRootKnowledgeElem.Release();
		//	pRootKnowledgeAttr.Release();
		pDocKnowledge.Release();
		CoUninitialize();


		//*********parse inference node ************
		MSXML2::IXMLDOMNodePtr pInferenceNode;
		MSXML2::IXMLDOMNodeListPtr pInferenceList;
		MSXML2::IXMLDOMNodePtr pUseMaskNode,pInferMaskNode,pMaskValueNode,pInferDirNode,pInferCreateCheckNode;
		if(m_pProject->m_pInference != NULL)
		{
			delete m_pProject->m_pInference;
			m_pProject->m_pInference = NULL;
		}
		m_pProject->m_pInference = new CInference();
		pRootList->get_item(2,&pInferenceNode);


		pInferenceNode->get_childNodes(&pInferenceList);

		pInferenceList->get_item(0,&pUseMaskNode);	
		pInferenceList->get_item(1,&pInferMaskNode);
		pInferenceList->get_item(2,&pMaskValueNode);
		pInferenceList->get_item(3,&pInferDirNode);
		pInferenceList->get_item(4,&pInferCreateCheckNode);

		pUseMaskNode->get_nodeTypedValue(&VariantValue);	
		CString strUseMask =(char *)(_bstr_t)VariantValue;		
		if(strUseMask=="false")
			m_pProject->m_pInference->setUseMask(false);
		else
			m_pProject->m_pInference->setUseMask(true);

		pInferMaskNode->get_nodeTypedValue(&VariantValue);	
		CString strMaskFile =(char *)(_bstr_t)VariantValue;	
		m_pProject->m_pInference->setMaskFile(strMaskFile);

		pMaskValueNode->get_nodeTypedValue(&VariantValue);	
		CString strMaskValue =(char *)(_bstr_t)VariantValue;	
		int iMaskValue = atoi(strMaskValue);
		m_pProject->m_pInference->setMaskValue(iMaskValue);

		pInferDirNode->get_nodeTypedValue(&VariantValue);	
		CString strInferDir =(char *)(_bstr_t)VariantValue;
		m_pProject->m_pInference->setResultDir(strInferDir);

		pInferCreateCheckNode->get_nodeTypedValue(&VariantValue);	
		CString strCreateCheck =(char *)(_bstr_t)VariantValue;	
		if(strCreateCheck=="false")	  
			m_pProject->m_pInference->setCreateCheckFile(false);
		else 
			m_pProject->m_pInference->setCreateCheckFile(true);
		m_pProject->m_pInference->setEnvDatabase(m_pProject->m_pEnvDatabase);
		m_pProject->m_pInference->setKnowledgeBase(m_pProject->m_pKnowledgeBase);
		pInferenceNode.Release();
		pInferenceList.Release();
		pUseMaskNode.Release();
		pMaskValueNode.Release();
		pInferMaskNode.Release();
		pInferDirNode.Release();
		pInferCreateCheckNode.Release();

		//pRootList.Release();
		//pDataTypeNode.Release();
		//pProjectNameNode.Release();
		//pProjectVersionNode.Release();
		//pProjectTypeNode.Release();
		//pRootElem.Release();
		//pRootAttr.Release();
		//pDoc.Release();
		//CoUninitialize();



		if(m_pProject->m_pEnvDatabase->getLayerNum()>0)
			pApp->m_iCurrEnvLayerIndex=0;

		//copy the base project to edit project
		if(m_pProjectEdit!=NULL)
		{
			delete m_pProjectEdit;
			m_pProjectEdit = NULL;
		}
		m_pProjectEdit = new CProject();
		CopyProject(m_pProject, m_pProjectEdit, true);

	}

	else    
	{
		//*********** Parse database node *************	
		MSXML2::IXMLDOMNodePtr pDatabaseNode, pClimateNode, pGeologyNode, pTerrainNode, pVegetationNode, pOtherNode;

		pRootList->get_item(0,&pDatabaseNode);	

		//traverse the child nodes of layerlist
		MSXML2::IXMLDOMNodeListPtr pLayerList;
		MSXML2::IXMLDOMNodePtr pLayerNode;
		MSXML2::IXMLDOMNodePtr pLayerNameNode,pLayerPathNode;
		MSXML2::IXMLDOMNodePtr pLayerResolutionNode,pLayerSimiTypeNode, pLayerSimiMethodNode;
		long layerNum;	
		int tempLayerNum;

		MSXML2::IXMLDOMNodeListPtr pDatabaseList;
		pDatabaseNode->get_childNodes(&pDatabaseList);


		pDatabaseList->get_item(0, &pClimateNode);
		MSXML2::IXMLDOMNodeListPtr pClimateList;
		pClimateNode->get_childNodes(&pClimateList);		
		pClimateList->get_length(&layerNum);	
		tempLayerNum = layerNum;
		for(long i=0; i<tempLayerNum; i++)
		{
			pClimateList->get_item(i,&pLayerNode);
			pLayerNode->get_childNodes(&pLayerList);
			pLayerList->get_item(0,&pLayerNameNode);
			pLayerList->get_item(1,&pLayerPathNode);
			pLayerList->get_item(2,&pLayerResolutionNode);
			pLayerList->get_item(3,&pLayerSimiTypeNode);
			pLayerList->get_item(4,&pLayerSimiMethodNode);


			pLayerNameNode->get_nodeTypedValue(&VariantValue);		
			CString strLayerName =(char *)(_bstr_t)VariantValue;		
			pLayerPathNode->get_nodeTypedValue(&VariantValue);		
			CString strLayerFile = (char *)(_bstr_t)VariantValue;	
			this->m_pProjectCase->vecClimateLayerNames.push_back(strLayerName);
			this->m_pProjectCase->vecClimateLayerFiles.push_back(strLayerFile);	


			CString layerPath= m_strProjectDir+"\\GISData\\Climate\\"+strLayerFile;		
			m_pProjectCase->g_vecstrClimateLayers.push_back(layerPath);



			pLayerResolutionNode->get_nodeTypedValue(&VariantValue);
			CString strLayerResolution = (char *)(_bstr_t)VariantValue;	

			pLayerSimiTypeNode->get_nodeTypedValue(&VariantValue);
			CString strLayerSimiType = (char *)(_bstr_t)VariantValue;	

			pLayerSimiMethodNode->get_nodeTypedValue(&VariantValue);
			CString strLayerSimiMethod = (char *)(_bstr_t)VariantValue;	

			double CellSize = atof(strLayerResolution.GetBuffer());				
			this->m_pProjectCase->vecClimateLayerResolutions.push_back(CellSize);
			this->m_pProjectCase->vecClimateLayerSimiTypes.push_back(strLayerSimiType);
			this->m_pProjectCase->vecClimateLayerSimiMethods.push_back(strLayerSimiMethod);



			pLayerList.Release();
			pLayerNode.Release();
			pLayerNameNode.Release();
			pLayerPathNode.Release();
			pLayerResolutionNode.Release();
			pLayerSimiTypeNode.Release();
			pLayerSimiMethodNode.Release();

		}

		pClimateNode.Release();
		pClimateList.Release();


		pDatabaseList->get_item(1, &pGeologyNode);
		MSXML2::IXMLDOMNodeListPtr pGeologyList;
		pGeologyNode->get_childNodes(&pGeologyList);		
		pGeologyList->get_length(&layerNum);	
		tempLayerNum = layerNum;
		for(long i=0; i<tempLayerNum; i++)
		{
			pGeologyList->get_item(i,&pLayerNode);
			pLayerNode->get_childNodes(&pLayerList);
			pLayerList->get_item(0,&pLayerNameNode);
			pLayerList->get_item(1,&pLayerPathNode);
			pLayerList->get_item(2,&pLayerResolutionNode);
			pLayerList->get_item(3,&pLayerSimiTypeNode);
			pLayerList->get_item(4,&pLayerSimiMethodNode);


			pLayerNameNode->get_nodeTypedValue(&VariantValue);		
			CString strLayerName =(char *)(_bstr_t)VariantValue;		
			pLayerPathNode->get_nodeTypedValue(&VariantValue);		
			CString strLayerFile = (char *)(_bstr_t)VariantValue;	
			this->m_pProjectCase->vecGeologyLayerNames.push_back(strLayerName);
			this->m_pProjectCase->vecGeologyLayerFiles.push_back(strLayerFile);	
			CString layerPath= m_strProjectDir+"\\GISData\\Geology\\"+strLayerFile;		
			m_pProjectCase->g_vecstrGeologyLayers.push_back(layerPath);

			pLayerResolutionNode->get_nodeTypedValue(&VariantValue);
			CString strLayerResolution = (char *)(_bstr_t)VariantValue;	

			pLayerSimiTypeNode->get_nodeTypedValue(&VariantValue);
			CString strLayerSimiType = (char *)(_bstr_t)VariantValue;	

			pLayerSimiMethodNode->get_nodeTypedValue(&VariantValue);
			CString strLayerSimiMethod = (char *)(_bstr_t)VariantValue;	

			double CellSize = atof(strLayerResolution.GetBuffer());				
			this->m_pProjectCase->vecGeologyLayerResolutions.push_back(CellSize);
			this->m_pProjectCase->vecGeologyLayerSimiTypes.push_back(strLayerSimiType);
			this->m_pProjectCase->vecGeologyLayerSimiMethods.push_back(strLayerSimiMethod);



			pLayerList.Release();
			pLayerNode.Release();
			pLayerNameNode.Release();
			pLayerPathNode.Release();
			pLayerResolutionNode.Release();
			pLayerSimiTypeNode.Release();
			pLayerSimiMethodNode.Release();

		}

		pGeologyNode.Release();
		pGeologyList.Release();


		pDatabaseList->get_item(2, &pTerrainNode);
		MSXML2::IXMLDOMNodeListPtr pTerrainList;
		pTerrainNode->get_childNodes(&pTerrainList);		
		pTerrainList->get_length(&layerNum);	
		tempLayerNum = layerNum;
		for(long i=0; i<tempLayerNum; i++)
		{
			pTerrainList->get_item(i,&pLayerNode);
			pLayerNode->get_childNodes(&pLayerList);
			pLayerList->get_item(0,&pLayerNameNode);
			pLayerList->get_item(1,&pLayerPathNode);
			pLayerList->get_item(2,&pLayerResolutionNode);
			pLayerList->get_item(3,&pLayerSimiTypeNode);
			pLayerList->get_item(4,&pLayerSimiMethodNode);

			pLayerNameNode->get_nodeTypedValue(&VariantValue);		
			CString strLayerName =(char *)(_bstr_t)VariantValue;		
			pLayerPathNode->get_nodeTypedValue(&VariantValue);		
			CString strLayerFile = (char *)(_bstr_t)VariantValue;	
			this->m_pProjectCase->vecTerrainLayerNames.push_back(strLayerName);
			this->m_pProjectCase->vecTerrainLayerFiles.push_back(strLayerFile);		

			CString layerPath= m_strProjectDir+"\\GISData\\Terrain\\"+strLayerFile;		
			m_pProjectCase->g_vecstrTerrainLayers.push_back(layerPath);

			pLayerResolutionNode->get_nodeTypedValue(&VariantValue);
			CString strLayerResolution = (char *)(_bstr_t)VariantValue;	

			pLayerSimiTypeNode->get_nodeTypedValue(&VariantValue);
			CString strLayerSimiType = (char *)(_bstr_t)VariantValue;	

			pLayerSimiMethodNode->get_nodeTypedValue(&VariantValue);
			CString strLayerSimiMethod = (char *)(_bstr_t)VariantValue;	

			double CellSize = atof(strLayerResolution.GetBuffer());				
			this->m_pProjectCase->vecTerrainLayerResolutions.push_back(CellSize);
			this->m_pProjectCase->vecTerrainLayerSimiTypes.push_back(strLayerSimiType);
			this->m_pProjectCase->vecTerrainLayerSimiMethods.push_back(strLayerSimiMethod);



			pLayerList.Release();
			pLayerNode.Release();
			pLayerNameNode.Release();
			pLayerPathNode.Release();
			pLayerResolutionNode.Release();
			pLayerSimiTypeNode.Release();
			pLayerSimiMethodNode.Release();

		}

		pTerrainNode.Release();
		pTerrainList.Release();


		pDatabaseList->get_item(3, &pVegetationNode);
		MSXML2::IXMLDOMNodeListPtr pVegetationList;
		pVegetationNode->get_childNodes(&pVegetationList);		
		pVegetationList->get_length(&layerNum);	
		tempLayerNum = layerNum;
		for(long i=0; i<tempLayerNum; i++)
		{
			pVegetationList->get_item(i,&pLayerNode);
			pLayerNode->get_childNodes(&pLayerList);
			pLayerList->get_item(0,&pLayerNameNode);
			pLayerList->get_item(1,&pLayerPathNode);
			pLayerList->get_item(2,&pLayerResolutionNode);
			pLayerList->get_item(3,&pLayerSimiTypeNode);
			pLayerList->get_item(4,&pLayerSimiMethodNode);

			pLayerNameNode->get_nodeTypedValue(&VariantValue);		
			CString strLayerName =(char *)(_bstr_t)VariantValue;		
			pLayerPathNode->get_nodeTypedValue(&VariantValue);		
			CString strLayerFile = (char *)(_bstr_t)VariantValue;	
			this->m_pProjectCase->vecVegetationLayerNames.push_back(strLayerName);
			this->m_pProjectCase->vecVegetationLayerFiles.push_back(strLayerFile);

			CString layerPath= m_strProjectDir+"\\GISData\\Vegetation\\"+strLayerFile;		
			m_pProjectCase->g_vecstrVegetationLayers.push_back(layerPath);
			pLayerResolutionNode->get_nodeTypedValue(&VariantValue);
			CString strLayerResolution = (char *)(_bstr_t)VariantValue;	

			pLayerSimiTypeNode->get_nodeTypedValue(&VariantValue);
			CString strLayerSimiType = (char *)(_bstr_t)VariantValue;	

			pLayerSimiMethodNode->get_nodeTypedValue(&VariantValue);
			CString strLayerSimiMethod = (char *)(_bstr_t)VariantValue;	

			double CellSize = atof(strLayerResolution.GetBuffer());				
			this->m_pProjectCase->vecVegetationLayerResolutions.push_back(CellSize);
			this->m_pProjectCase->vecVegetationLayerSimiTypes.push_back(strLayerSimiType);
			this->m_pProjectCase->vecVegetationLayerSimiMethods.push_back(strLayerSimiMethod);



			pLayerList.Release();
			pLayerNode.Release();
			pLayerNameNode.Release();
			pLayerPathNode.Release();
			pLayerResolutionNode.Release();
			pLayerSimiTypeNode.Release();
			pLayerSimiMethodNode.Release();

		}

		pVegetationNode.Release();
		pVegetationList.Release();


		pDatabaseList->get_item(4, &pOtherNode);
		MSXML2::IXMLDOMNodeListPtr pOtherList;
		pOtherNode->get_childNodes(&pOtherList);		
		pOtherList->get_length(&layerNum);	
		tempLayerNum = layerNum;
		for(long i=0; i<tempLayerNum; i++)
		{
			pOtherList->get_item(i,&pLayerNode);
			pLayerNode->get_childNodes(&pLayerList);
			pLayerList->get_item(0,&pLayerNameNode);
			pLayerList->get_item(1,&pLayerPathNode);
			pLayerList->get_item(2,&pLayerResolutionNode);
			pLayerList->get_item(3,&pLayerSimiTypeNode);
			pLayerList->get_item(4,&pLayerSimiMethodNode);

			pLayerNameNode->get_nodeTypedValue(&VariantValue);		
			CString strLayerName =(char *)(_bstr_t)VariantValue;		
			pLayerPathNode->get_nodeTypedValue(&VariantValue);		
			CString strLayerFile = (char *)(_bstr_t)VariantValue;	
			this->m_pProjectCase->vecOtherLayerNames.push_back(strLayerName);
			this->m_pProjectCase->vecOtherLayerFiles.push_back(strLayerFile);		

			CString layerPath= m_strProjectDir+"\\GISData\\Vegetation\\"+strLayerFile;		
			m_pProjectCase->g_vecstrOtherLayers.push_back(layerPath);

			pLayerResolutionNode->get_nodeTypedValue(&VariantValue);
			CString strLayerResolution = (char *)(_bstr_t)VariantValue;	

			pLayerSimiTypeNode->get_nodeTypedValue(&VariantValue);
			CString strLayerSimiType = (char *)(_bstr_t)VariantValue;	

			pLayerSimiMethodNode->get_nodeTypedValue(&VariantValue);
			CString strLayerSimiMethod = (char *)(_bstr_t)VariantValue;	

			double CellSize = atof(strLayerResolution.GetBuffer());				
			this->m_pProjectCase->vecOtherLayerResolutions.push_back(CellSize);
			this->m_pProjectCase->vecOtherLayerSimiTypes.push_back(strLayerSimiType);
			this->m_pProjectCase->vecOtherLayerSimiMethods.push_back(strLayerSimiMethod);



			pLayerList.Release();
			pLayerNode.Release();
			pLayerNameNode.Release();
			pLayerPathNode.Release();
			pLayerResolutionNode.Release();
			pLayerSimiTypeNode.Release();
			pLayerSimiMethodNode.Release();

		}

		pOtherNode.Release();
		pOtherList.Release();

		pDatabaseNode.Release();
		pDatabaseList.Release();

		//***********Parse knowledge base node *************
		MSXML2::IXMLDOMNodePtr pCasebaseNode;
		MSXML2::IXMLDOMNodeListPtr pCasebaseList;

		MSXML2::IXMLDOMNodePtr pCasebaseFileNode;

		pRootList->get_item(1,&pCasebaseNode);

		pCasebaseNode->get_childNodes(&pCasebaseList);
		pCasebaseList->get_item(0,&pCasebaseFileNode);	
		pCasebaseFileNode->get_nodeTypedValue(&VariantValue);	
		CString strCasebaseFile =(char *)(_bstr_t)VariantValue;	


		m_pProjectCase->m_strCasebaseFile = strCasebaseFile;


		if(strCasebaseFile != "")
		{

			m_pProjectCase->m_strCasebasePath = this->m_strProjectDir + "\\" + strCasebaseFile;
			//if(m_pProjectCase->hDBF)
			//  DBFClose(m_pProjectCase->hDBF);
			//m_pProjectCase->hDBF = DBFOpen( (LPCTSTR) m_pProjectCase->m_strCasebasePath, "rb" );

			if(m_pProjectCase->hCSV)
			{
				delete m_pProjectCase->hCSV;
				m_pProjectCase->hCSV = NULL;
			}
			m_pProjectCase->hCSV = new CDataFile( (LPCTSTR) m_pProjectCase->m_strCasebasePath, DF::RF_READ_AS_STRING);



			int nFields = m_pProjectCase->hCSV->GetNumberOfVariables();

			//DBFGetFieldCount( pDoc->m_pProjectCaseEdit->hDBF );
			int nRec = m_pProjectCase->hCSV->GetNumberOfSamples(0)-1;


			// add field names to combobox and listctrl
			//CString fieldName;

			char fieldName[100];


			//m_pProjectCase->vecSampleData.clear();







			//for(int i = 0; i < nRec; ++i)
			//{
			//	CString sID;
			//	sID.Format("%d", i+1);


			//	std::vector<CString> record;

			//	for (int j = 0; j < nFields; ++j)
			//	{
			//		//const char* content = DBFReadStringAttribute(pDoc->m_pProjectCaseEdit->hDBF, i, j);
			//		char content[100];

			//		m_pProjectCase->hCSV->GetData(j, i,content); 
			//		record.push_back(content);
			//	}

			//	m_pProjectCase->vecSampleData.push_back(record);
			//}





		}
		else
		{

			m_pProjectCase->m_strCasebasePath = "";
			// if(m_pProjectCase->hDBF)
			// {
			//  DBFClose(m_pProjectCase->hDBF);
			//  m_pProjectCase->hDBF = NULL;
			//}

			if(m_pProjectCase->hCSV)
			{
				delete m_pProjectCase->hCSV;
				m_pProjectCase->hCSV = NULL;
			}
		}



		//*********parse inference node ************


		MSXML2::IXMLDOMNodePtr pInferenceNode;
		MSXML2::IXMLDOMNodeListPtr pInferenceList;
		//	MSXML2::IXMLDOMNodePtr pUseKdeNode, pUseKdeVegetationNode, pCBROptionNode, pXIndexNode, pYIndexNode;


		MSXML2::IXMLDOMNodePtr pUseKdeNode, pUseKdeVegetationNode, pCaseMethodNode, pClimateCutNode, pClimateWeightNode, pTopoWeightNode, pVegWeightNode, pOtherWeightNode, pCBROptionNode, pXIndexNode, pYIndexNode,pIDIndexNode, pPropertyThresholdNode;// pTypeThresholdNode;



		pRootList->get_item(2,&pInferenceNode);


		pInferenceNode->get_childNodes(&pInferenceList);
		//pInferenceList->get_item(0,&pUseKdeNode);	
		//pInferenceList->get_item(1,&pUseKdeVegetationNode);
		pInferenceList->get_item(0,&pCaseMethodNode);
		pInferenceList->get_item(1,&pClimateCutNode);
		pInferenceList->get_item(2,&pClimateWeightNode);
		pInferenceList->get_item(3,&pTopoWeightNode);
		pInferenceList->get_item(4,&pVegWeightNode);
		pInferenceList->get_item(5,&pOtherWeightNode);
		pInferenceList->get_item(6,&pCBROptionNode);
		pInferenceList->get_item(7,&pXIndexNode);
		pInferenceList->get_item(8,&pYIndexNode);
		pInferenceList->get_item(9,&pIDIndexNode);
		pInferenceList->get_item(10,&pPropertyThresholdNode);
		//pInferenceList->get_item(9,&pTypeThresholdNode);

		/*pUseKdeNode->get_nodeTypedValue(&VariantValue);	
		CString strUseKDE =(char *)(_bstr_t)VariantValue;		
		if(strUseKDE=="false")
		m_pProjectCase->g_useKde = false;
		else
		m_pProjectCase->g_useKde = true;

		pUseKdeVegetationNode->get_nodeTypedValue(&VariantValue);	
		CString strUseKDEVege =(char *)(_bstr_t)VariantValue;		
		if(strUseKDEVege=="false")
		m_pProjectCase->g_useKdeVegetation = false;
		else
		m_pProjectCase->g_useKdeVegetation= true;
		*/

		pCaseMethodNode->get_nodeTypedValue(&VariantValue);	
		CString strCaseMethod =(char *)(_bstr_t)VariantValue;	

		if(strCaseMethod == "LimitingFactor")		
			m_pProjectCase->m_iCaseInteMethod = 0;
		else if(strCaseMethod == "AverageWeighted")
			m_pProjectCase->m_iCaseInteMethod = 1;
		else 
			m_pProjectCase->m_iCaseInteMethod = 2;


		pClimateCutNode->get_nodeTypedValue(&VariantValue);	
		CString strClimateCut =(char *)(_bstr_t)VariantValue;	
		double dClimateCut = atof(strClimateCut);
		m_pProjectCase->m_dClimateCut = dClimateCut;

		pClimateWeightNode->get_nodeTypedValue(&VariantValue);	
		CString strClimateWeight =(char *)(_bstr_t)VariantValue;	
		double dClimateWeight = atof(strClimateWeight);
		m_pProjectCase->m_dClimateWeight = dClimateWeight;

		pTopoWeightNode->get_nodeTypedValue(&VariantValue);	
		CString strTopoWeight =(char *)(_bstr_t)VariantValue;	
		double dTopoWeight = atof(strTopoWeight);
		m_pProjectCase->m_dTopoWeight = dTopoWeight;

		pVegWeightNode->get_nodeTypedValue(&VariantValue);	
		CString strVegWeight =(char *)(_bstr_t)VariantValue;	
		double dVegWeight = atof(strVegWeight);
		m_pProjectCase->m_dVegWeight = dVegWeight;

		pOtherWeightNode->get_nodeTypedValue(&VariantValue);	
		CString strOtherWeight =(char *)(_bstr_t)VariantValue;	
		double dOtherWeight = atof(strOtherWeight);
		m_pProjectCase->m_dOtherWeight = dOtherWeight;




		pCBROptionNode->get_nodeTypedValue(&VariantValue);	
		CString strCBROption =(char *)(_bstr_t)VariantValue;	
		if(strCBROption == "Property")
			m_pProjectCase->iCBROption = 1;
		else
			m_pProjectCase->iCBROption = 2;




		pXIndexNode->get_nodeTypedValue(&VariantValue);	
		CString strXIndex =(char *)(_bstr_t)VariantValue;	
		m_pProjectCase->m_strXIndex = strXIndex;


		pYIndexNode->get_nodeTypedValue(&VariantValue);	
		CString strYIndex =(char *)(_bstr_t)VariantValue;	
		m_pProjectCase->m_strYIndex = strYIndex;


		pIDIndexNode->get_nodeTypedValue(&VariantValue);	
		CString strIDIndex =(char *)(_bstr_t)VariantValue;	
		m_pProjectCase->m_strIDIndex = strIDIndex;

		pPropertyThresholdNode->get_nodeTypedValue(&VariantValue);	
		CString strPropertyThreshold =(char *)(_bstr_t)VariantValue;	
		double dPropertyThreshold = atof(strPropertyThreshold);
		m_pProjectCase->m_thresholdProperty = dPropertyThreshold;

		/*pTypeThresholdNode->get_nodeTypedValue(&VariantValue);	
		CString strTypeThreshold =(char *)(_bstr_t)VariantValue;	
		double dTypeThreshold = atof(strTypeThreshold);
		m_pProjectCase->m_thresholdType = dTypeThreshold;*/



		pInferenceNode.Release();
		pInferenceList.Release();
		//pUseKdeNode.Release();
		//pUseKdeVegetationNode.Release();
		pCBROptionNode.Release();
		pXIndexNode.Release();
		pYIndexNode.Release();
		pIDIndexNode.Release();


		pCaseMethodNode.Release();
		pClimateCutNode.Release();
		pClimateWeightNode.Release();
		pTopoWeightNode.Release();
		pVegWeightNode.Release();
		pOtherWeightNode.Release();
		pPropertyThresholdNode.Release();
		//pTypeThresholdNode.Release();





		//copy the base project to edit project
		if(m_pProjectCaseEdit!=NULL)
		{
			delete m_pProjectCaseEdit;
			m_pProjectCaseEdit = NULL;
		}
		m_pProjectCaseEdit = new CProjectCase();
		CopyProjectCase(m_pProjectCase, m_pProjectCaseEdit, true);


	}





	pRootList.Release();
	pDataTypeNode.Release();
	pProjectNameNode.Release();
	pProjectVersionNode.Release();
	pProjectTypeNode.Release();
	pRootElem.Release();
	pRootAttr.Release();
	pDoc.Release();
	CoUninitialize();





	pFrame->m_leftBar.FillProjectTree(this);
	pFrame->m_splitterTabWnd.SelectView(0);



	CDatabaseView *pDatabaseView = (CDatabaseView *)(pFrame->m_splitterTabWnd.GetPane(0,0));	
	CInfoFrameView *pInfoFrameView = (CInfoFrameView *)(pDatabaseView->m_wndSplitter.GetPane(1,0));	
	CMetadataView *pMetadataView = (CMetadataView *)(pInfoFrameView->m_wndSplitter.GetPane(0,1));	
	pDatabaseView->Invalidate();
	pMetadataView->OnInitialUpdate();
	//	pMetadataView->FillMetadataView();
}


void CSoLIMDoc::LoadKnowledgeBase(CString strFileName, CProject *pProject)
{




	HRESULT hrKnowledge;	 
	MSXML2::IXMLDOMDocumentPtr pDocKnowledge;//xml document
	MSXML2::IXMLDOMElementPtr pRootKnowledgeElem;

	MSXML2::IXMLDOMNodeListPtr pRootKnowledgeBaseList;	
	_variant_t VariantValue;
	CString str;

	hrKnowledge = CoInitialize(NULL);
	if(FAILED(hrKnowledge))
	{
		AfxMessageBox("Failed to initialize COM");
		return;
	}
	//create instance
	hrKnowledge=pDocKnowledge.CreateInstance(_uuidof(MSXML2::DOMDocument));
	if(FAILED(hrKnowledge))
	{
		AfxMessageBox("Failed to create DOM Document");
		return;
	}
	//load xml doc
	_variant_t varxmlKnowledge(strFileName);
	VARIANT_BOOL varOutKnowledge;
	varOutKnowledge = pDocKnowledge->load(varxmlKnowledge);
	if(!varOutKnowledge)
	{
		AfxMessageBox("Failed to Load xml file");
		return;
	}

	pDocKnowledge->get_documentElement(&pRootKnowledgeElem);



	if(pProject->m_pKnowledgeBase != NULL)
	{
		delete pProject->m_pKnowledgeBase;
		pProject->m_pKnowledgeBase = NULL;
	}
	pProject->m_pKnowledgeBase = new CKnowledgeBase();
	//pRootKnowledgeList->get_item(1,&pKnowledgeBaseNode);	    
	long soilNum;

	pRootKnowledgeElem->get_childNodes(&pRootKnowledgeBaseList);       
	pRootKnowledgeBaseList->get_length(&soilNum);		
	//traverse child nodes of knowledge base(soiltypes)
	MSXML2::IXMLDOMNodePtr pSoilTypeNode;
	MSXML2::IXMLDOMNodeListPtr pSoilTypeList;
	MSXML2::IXMLDOMNodePtr pSoilNameNode;
	MSXML2::IXMLDOMNamedNodeMapPtr pSoilTypeAttr;
	for(int i=0;i<soilNum;i++)
	{
		CSoilType *pSoilType = new CSoilType;
		pRootKnowledgeBaseList->get_item(i,&pSoilTypeNode);
		pSoilTypeNode->get_attributes(&pSoilTypeAttr);   
		pSoilNameNode = pSoilTypeAttr->getNamedItem("Name");
		pSoilNameNode->get_nodeTypedValue(&VariantValue);		
		str =(char *)(_bstr_t)VariantValue;	
		pSoilType->setSoilTypeName(str);
		MSXML2::IXMLDOMNodePtr pInstancesNode,pOccurrencesNode,pExclusionsNode;
		pSoilTypeNode->get_childNodes(&pSoilTypeList);
		pSoilTypeList->get_item(0,&pInstancesNode);
		pSoilTypeList->get_item(1,&pOccurrencesNode);
		pSoilTypeList->get_item(2,&pExclusionsNode);		 
		long instanceNum, occurrenceNum, exclusionNum;	

		MSXML2::IXMLDOMNodeListPtr pInstancesList,pOccurrencesList,pExclusionsList;
		pInstancesNode->get_childNodes(&pInstancesList);
		pOccurrencesNode->get_childNodes(&pOccurrencesList);
		pExclusionsNode->get_childNodes(&pExclusionsList);
		pInstancesList->get_length(&instanceNum);
		pOccurrencesList->get_length(&occurrenceNum);
		pExclusionsList->get_length(&exclusionNum);

		//traverse child nodes of instances (every instance)
		MSXML2::IXMLDOMNodePtr pInstanceNode;
		MSXML2::IXMLDOMNamedNodeMapPtr pInstanceAttr;
		MSXML2::IXMLDOMNodePtr pInstanceNameNode;
		MSXML2::IXMLDOMNodeListPtr pInstanceList;
		for(int n1=0;n1<instanceNum;n1++)
		{
			CInstance *pInstance = new CInstance;			
			pInstancesList->get_item(n1,&pInstanceNode);
			pInstanceNode->get_attributes(&pInstanceAttr);
			pInstanceNameNode = pInstanceAttr->getNamedItem("Name");
			pInstanceNameNode->get_nodeTypedValue(&VariantValue);	
			str =(char *)(_bstr_t)VariantValue;	
			pInstance->setInstanceName(str);
			long ruleNum;			
			pInstanceNode->get_childNodes(&pInstanceList);
			pInstanceList->get_length(&ruleNum);

			//travers child nodes of instance ( rule node )
			CString  strRuleName,strRuleUseOrNot,strRuleType,strRuleLayer;
			for(long n11=0;n11<ruleNum;n11++)
			{
				MSXML2::IXMLDOMNodePtr pRuleNode;
				MSXML2::IXMLDOMNamedNodeMapPtr pRuleAttr;				
				MSXML2::IXMLDOMNodePtr pRuleNameNode,pRuleUseOrNotNode,pRuleTypeNode,pRuleLayerNode;
				MSXML2::IXMLDOMNodeListPtr pRuleList;
				pInstanceList->get_item(n11,&pRuleNode);				
				pRuleNode->get_attributes(&pRuleAttr);
				pRuleNameNode = pRuleAttr->getNamedItem("Name");
				pRuleUseOrNotNode = pRuleAttr->getNamedItem("UseOrNot");
				pRuleTypeNode = pRuleAttr->getNamedItem("Type");
				pRuleLayerNode = pRuleAttr->getNamedItem("DataLayer");
				pRuleNameNode->get_nodeTypedValue(&VariantValue);	
				strRuleName =(char *)(_bstr_t)VariantValue;	
				pRuleUseOrNotNode->get_nodeTypedValue(&VariantValue);	
				strRuleUseOrNot =(char *)(_bstr_t)VariantValue;	
				pRuleTypeNode->get_nodeTypedValue(&VariantValue);	
				strRuleType =(char *)(_bstr_t)VariantValue;	
				pRuleLayerNode->get_nodeTypedValue(&VariantValue);	
				strRuleLayer =(char *)(_bstr_t)VariantValue;				
				pRuleNode->get_childNodes(&pRuleList);
				if(strRuleType == "Range")
				{
					CRangeRule *pRule= new CRangeRule;
					pRule->setRuleType(0);
					pRule->setRuleName(strRuleName);
					if(strRuleUseOrNot == "true")
						pRule->setIsUsed(true);
					else 
						pRule->setIsUsed(false);
					pRule->setLayerName(strRuleLayer);	
					MSXML2::IXMLDOMNodePtr pCurveTypeNode,pKeyValueNode;
					pRuleList->get_item(0,&pCurveTypeNode);
					pRuleList->get_item(1,&pKeyValueNode);
					pCurveTypeNode->get_nodeTypedValue(&VariantValue);	
					CString strCurveType =(char *)(_bstr_t)VariantValue;	
					pKeyValueNode->get_nodeTypedValue(&VariantValue);	
					CString strKeyValue =(char *)(_bstr_t)VariantValue;	
					double KeyValue[4] ;
					split1(strKeyValue,KeyValue);
					pRule->setLowCross(KeyValue[0]);
					pRule->setLowUnity(KeyValue[1]);
					pRule->setHighUnity(KeyValue[2]);
					pRule->setHighCross(KeyValue[3]);   
					if(strCurveType == "Bell-shape")
						pRule->setCurveType(0);
					else if(strCurveType == "S-shape")
						pRule->setCurveType(1);
					else if(strCurveType == "Z-shape")
						pRule->setCurveType(2);
					pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
					pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());
					pRule->setDispMin(pRule->ComputeRuleGraphMin());
					pRule->setDispMax(pRule->ComputeRuleGraphMax());
					pInstance->AddRule((CRule *)pRule);
					pCurveTypeNode.Release();
					pKeyValueNode.Release();
				}
				else if( strRuleType == "Curve")
				{
					CFreehandRule *pRule = new CFreehandRule;
					pRule->setRuleType(1);
					pRule->setRuleName(strRuleName);
					if(strRuleUseOrNot == "true")
						pRule->setIsUsed(true);
					else 
						pRule->setIsUsed(false);

					pRule->setLayerName(strRuleLayer);
					MSXML2::IXMLDOMNodePtr pNodeNum,pCoordinatesNode;
					pRuleList->get_item(0,&pNodeNum);
					pRuleList->get_item(1,&pCoordinatesNode);
					pNodeNum->get_nodeTypedValue(&VariantValue);	
					CString strNodeNum =(char *)(_bstr_t)VariantValue;	
					int NodeNum = atoi(strNodeNum);
					pCoordinatesNode->get_nodeTypedValue(&VariantValue);	
					CString strCoordinates =(char *)(_bstr_t)VariantValue;	
					double NodeX[100];
					double NodeY[100];
					split1(strCoordinates,NodeX, NodeY);
					if(NodeNum>100)
					{
						AfxMessageBox("The number of nodes must be smaller than 100");
						delete pRule;
						pRule = NULL;
						continue;

					}
					for(int i=0;i<NodeNum; i++)				
						pRule->AddKnot(NodeX[i],NodeY[i]);		
					pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
					pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());
					pRule->setDispMin(pRule->ComputeRuleGraphMin());
					pRule->setDispMax(pRule->ComputeRuleGraphMax());
					pInstance->AddRule((CRule *)pRule);
					pNodeNum.Release();
					pCoordinatesNode.Release();
				}
				else if(strRuleType == "Word")
				{
					CWordRule *pRule= new CWordRule;
					pRule->setRuleType(2);
					pRule->setRuleName(strRuleName);
					if(strRuleUseOrNot == "true")
						pRule->setIsUsed(true);
					else 
						pRule->setIsUsed(false);
					pRule->setLayerName(strRuleLayer);	
					MSXML2::IXMLDOMNodePtr pPraseNode, pEnvAttriNode,pWordLibNode;
					pRuleList->get_item(0,&pPraseNode);
					pRuleList->get_item(1,&pEnvAttriNode);
					pRuleList->get_item(2,&pWordLibNode);
					pPraseNode->get_nodeTypedValue(&VariantValue);	
					CString strPrase =(char *)(_bstr_t)VariantValue;	
					pRule->setPhrase(strPrase);
					pEnvAttriNode->get_nodeTypedValue(&VariantValue);	
					CString strEnvAttri =(char *)(_bstr_t)VariantValue;	
					pRule->setEnvAttri(strEnvAttri);
					pWordLibNode->get_nodeTypedValue(&VariantValue);
					CString strWordLib = (char *)(_bstr_t)VariantValue;
					strWordLib = m_strProjectDir +"\\"+strWordLib;
					pRule->setLibFileName(strWordLib);
					pRule->LoadWordLib();
					pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
					pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());
					pRule->setDispMin(pRule->ComputeRuleGraphMin());
					pRule->setDispMax(pRule->ComputeRuleGraphMax());
					pInstance->AddRule((CRule *)pRule);
					pPraseNode.Release();
					pWordLibNode.Release();
				}
				else if(strRuleType == "Point")
				{
					CPointRule *pRule= new CPointRule;
					pRule->setRuleType(3);
					pRule->setRuleName(strRuleName);
					if(strRuleUseOrNot == "true")
						pRule->setIsUsed(true);
					else 
						pRule->setIsUsed(false);
					pRule->setLayerName(strRuleLayer);	
					MSXML2::IXMLDOMNodePtr pXYNode,pCurveTypeNode,pLWidthNode,pRWidthNode;
					pRuleList->get_item(0,&pXYNode);
					pRuleList->get_item(1,&pCurveTypeNode);
					pRuleList->get_item(2,&pLWidthNode);
					pRuleList->get_item(3,&pRWidthNode);
					pXYNode->get_nodeTypedValue(&VariantValue);	
					CString strXY =(char *)(_bstr_t)VariantValue;	
					pCurveTypeNode->get_nodeTypedValue(&VariantValue);	
					CString strCurveType =(char *)(_bstr_t)VariantValue;
					pLWidthNode->get_nodeTypedValue(&VariantValue);	
					CString strLWidth =(char *)(_bstr_t)VariantValue;
					pRWidthNode->get_nodeTypedValue(&VariantValue);	
					CString strRWidth =(char *)(_bstr_t)VariantValue;
					double X[1],Y[1];
					split1(strXY, X,Y);					
					pRule->setCentralX(X[0]);
					pRule->setCentralY(Y[0]);
					if(strCurveType == "Bell-shape")
						pRule->setCurveType(0);
					else if(strCurveType == "S-shape")
						pRule->setCurveType(1);
					else if(strCurveType == "Z-shape")
						pRule->setCurveType(2);
					pRule->setLWidth(atof(strLWidth));
					pRule->setRWidth(atof(strRWidth));
					pRule->ComputeCentralValue(m_pProject->m_pEnvDatabase);
					pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
					pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());
					pRule->setDispMin(pRule->ComputeRuleGraphMin());
					pRule->setDispMax(pRule->ComputeRuleGraphMax());
					pInstance->AddRule((CRule *)pRule);
					pXYNode.Release();
					pCurveTypeNode.Release();
					pLWidthNode.Release();
					pRWidthNode.Release();
				}		
				else if(strRuleType ==  "Enumerated")
				{
					CEnumRule *pRule= new CEnumRule;
					pRule->setRuleType(4);
					pRule->setRuleName(strRuleName);
					if(strRuleUseOrNot == "true")
						pRule->setIsUsed(true);
					else 
						pRule->setIsUsed(false);
					pRule->setLayerName(strRuleLayer);		
					MSXML2::IXMLDOMNodePtr pItemNum, pEnumValueNode;
					pRuleList->get_item(0,&pItemNum);
					pRuleList->get_item(1,&pEnumValueNode);
					pItemNum->get_nodeTypedValue(&VariantValue);	
					CString strItemNum =(char *)(_bstr_t)VariantValue;	
					int ItemNum = atoi(strItemNum);
					if(ItemNum>100)
					{
						AfxMessageBox("The number of items must be smaller than 100");
						delete pRule;
						pRule = NULL;
						continue;
					}
					pEnumValueNode->get_nodeTypedValue(&VariantValue);	
					CString strItems =(char *)(_bstr_t)VariantValue;
					int Items[100];
					split1(strItems, Items);
					for(int i= 0;i<ItemNum;i++)			
						pRule->AddItem(Items[i]);	

					pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
					pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

					pRule->setDispMin(pRule->ComputeRuleGraphMin());
					pRule->setDispMax(pRule->ComputeRuleGraphMax());

					pInstance->AddRule((CRule *)pRule);
					pItemNum.Release();
					pEnumValueNode.Release();
				}	


				pRuleNode.Release();
				pRuleAttr.Release();				
				pRuleNameNode.Release();
				pRuleUseOrNotNode.Release();
				pRuleTypeNode.Release();
				pRuleLayerNode.Release();
				pRuleList.Release();
			}
			pSoilType->AddInstance(pInstance);
			pInstanceNode.Release();
			pInstanceAttr.Release();
			pInstanceNameNode.Release();
			pInstanceList.Release();
		}

		pInstancesList.Release();
		pInstancesNode.Release();


		//traver child node of occurrences(every occurrences)
		MSXML2::IXMLDOMNodePtr pOccurrenceNode;
		MSXML2::IXMLDOMNamedNodeMapPtr pOccurrenceAttr;
		MSXML2::IXMLDOMNodePtr pOccurrenceNameNode,pOccurrenceXNode,pOccurrenceYNode,pOccurrenceUseDistanceDecayNode,pOccurrenceSearchDistNode,pOccurrenceDistanceDecayNode;
		MSXML2::IXMLDOMNodeListPtr pOccurrenceList;
		for(int n2=0;n2<occurrenceNum;n2++)
		{
			COccurrence *pOccurrence = new COccurrence;
			pOccurrencesList->get_item(n2,&pOccurrenceNode);
			pOccurrenceNode->get_attributes(&pOccurrenceAttr);
			pOccurrenceNameNode = pOccurrenceAttr->getNamedItem("Name");
			pOccurrenceXNode = pOccurrenceAttr->getNamedItem("x");
			pOccurrenceYNode = pOccurrenceAttr->getNamedItem("y");			
			pOccurrenceSearchDistNode = pOccurrenceAttr->getNamedItem("SearchDist");
			pOccurrenceUseDistanceDecayNode = pOccurrenceAttr->getNamedItem("UseDistanceDecay");
			pOccurrenceDistanceDecayNode = pOccurrenceAttr->getNamedItem("DistanceDecay");
			pOccurrenceNameNode->get_nodeTypedValue(&VariantValue);	
			CString strOccurrenceName =(char *)(_bstr_t)VariantValue;	
			pOccurrence->setOccurrenceName(strOccurrenceName);	
			pOccurrenceXNode->get_nodeTypedValue(&VariantValue);	
			CString strOccurrenceX =(char *)(_bstr_t)VariantValue;
			pOccurrence->setCentralX(atof(strOccurrenceX));
			pOccurrenceYNode->get_nodeTypedValue(&VariantValue);	
			CString strOccurrenceY =(char *)(_bstr_t)VariantValue;
			pOccurrence->setCentralY(atof(strOccurrenceY));			
			pOccurrenceSearchDistNode->get_nodeTypedValue(&VariantValue);	
			CString strOccurrenceSearchDist =(char *)(_bstr_t)VariantValue;
			pOccurrence->setSearchDist(atof(strOccurrenceSearchDist));
			pOccurrenceUseDistanceDecayNode ->get_nodeTypedValue(&VariantValue);	
			CString strOccurrenceUseDistDecay =(char *)(_bstr_t)VariantValue;
			if(strOccurrenceUseDistDecay == "true")
			{
				pOccurrence->setUseDistSim(true);
			}
			else
			{
				pOccurrence->setUseDistSim(false);
			}
			pOccurrenceDistanceDecayNode->get_nodeTypedValue(&VariantValue);	
			CString strOccurrenceDistanceDecay =(char *)(_bstr_t)VariantValue;
			pOccurrence->setDistDecayFactor(atof(strOccurrenceDistanceDecay));
			pOccurrenceNode->get_childNodes(&pOccurrenceList);
			long ruleNum;
			pOccurrenceList->get_length(&ruleNum);	
			//traver child nodes of occurrence(every rule)
			CString  strRuleName,strRuleUseOrNot,strRuleType,strRuleLayer;			
			for(long n11=0;n11<ruleNum;n11++)
			{
				MSXML2::IXMLDOMNodePtr pRuleNode;
				MSXML2::IXMLDOMNamedNodeMapPtr pRuleAttr;
				MSXML2::IXMLDOMNodePtr pRuleNameNode,pRuleUseOrNotNode,pRuleTypeNode,pRuleLayerNode;
				MSXML2::IXMLDOMNodeListPtr pRuleList;

				pOccurrenceList->get_item(n11,&pRuleNode);				
				pRuleNode->get_attributes(&pRuleAttr);				
				pRuleNameNode = pRuleAttr->getNamedItem("Name");
				pRuleUseOrNotNode = pRuleAttr->getNamedItem("UseOrNot");
				pRuleTypeNode = pRuleAttr->getNamedItem("Type");
				pRuleLayerNode = pRuleAttr->getNamedItem("DataLayer");
				pRuleNameNode->get_nodeTypedValue(&VariantValue);	
				strRuleName =(char *)(_bstr_t)VariantValue;	
				pRuleUseOrNotNode->get_nodeTypedValue(&VariantValue);	
				strRuleUseOrNot =(char *)(_bstr_t)VariantValue;	
				pRuleTypeNode->get_nodeTypedValue(&VariantValue);	
				strRuleType =(char *)(_bstr_t)VariantValue;	
				pRuleLayerNode->get_nodeTypedValue(&VariantValue);	
				strRuleLayer =(char *)(_bstr_t)VariantValue;

				pRuleNode->get_childNodes(&pRuleList);
				if(strRuleType == "Range")
				{
					CRangeRule *pRule= new CRangeRule;
					pRule->setRuleType(0);
					pRule->setRuleName(strRuleName);
					if(strRuleUseOrNot == "true")
						pRule->setIsUsed(true);
					else 
						pRule->setIsUsed(false);
					pRule->setLayerName(strRuleLayer);
					MSXML2::IXMLDOMNodePtr pCurveTypeNode,pKeyValueNode;
					pRuleList->get_item(0,&pCurveTypeNode);
					pRuleList->get_item(1,&pKeyValueNode);
					pCurveTypeNode->get_nodeTypedValue(&VariantValue);	
					CString strCurveType =(char *)(_bstr_t)VariantValue;	
					pKeyValueNode->get_nodeTypedValue(&VariantValue);	
					CString strKeyValue =(char *)(_bstr_t)VariantValue;	
					double KeyValue[4] ;
					split1(strKeyValue,KeyValue);
					pRule->setLowCross(KeyValue[0]);
					pRule->setLowUnity(KeyValue[1]);
					pRule->setHighUnity(KeyValue[2]);
					pRule->setHighCross(KeyValue[3]);   
					if(strCurveType == "Bell-shape")
						pRule->setCurveType(0);
					else if(strCurveType == "S-shape")
						pRule->setCurveType(1);
					else if(strCurveType == "Z-shape")
						pRule->setCurveType(2);

					pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
					pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

					pRule->setDispMin(pRule->ComputeRuleGraphMin());
					pRule->setDispMax(pRule->ComputeRuleGraphMax());
					pOccurrence->AddRule((CRule *)pRule);
					pCurveTypeNode.Release();
					pKeyValueNode.Release();
				}
				else if( strRuleType == "Curve")
				{
					CFreehandRule *pRule = new CFreehandRule;
					pRule->setRuleType(1);
					pRule->setRuleName(strRuleName);
					if(strRuleUseOrNot == "true")
						pRule->setIsUsed(true);
					else 
						pRule->setIsUsed(false);

					pRule->setLayerName(strRuleLayer);	

					MSXML2::IXMLDOMNodePtr pNodeNum,pCoordinatesNode;
					pRuleList->get_item(0,&pNodeNum);
					pRuleList->get_item(1,&pCoordinatesNode);
					pNodeNum->get_nodeTypedValue(&VariantValue);	
					CString strNodeNum =(char *)(_bstr_t)VariantValue;	
					int NodeNum = atoi(strNodeNum);

					pCoordinatesNode->get_nodeTypedValue(&VariantValue);	
					CString strCoordinates =(char *)(_bstr_t)VariantValue;	
					double NodeX[100];
					double NodeY[100];
					split1(strCoordinates,NodeX, NodeY);
					if(NodeNum>100)
					{
						AfxMessageBox("The number of nodes must be smaller than 100");
						delete pRule;
						pRule = NULL;
						continue;

					}
					for(int i=0;i<NodeNum; i++)				
						pRule->AddKnot(NodeX[i],NodeY[i]);

					pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
					pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

					pRule->setDispMin(pRule->ComputeRuleGraphMin());
					pRule->setDispMax(pRule->ComputeRuleGraphMax());

					pOccurrence->AddRule((CRule *)pRule);
					pNodeNum.Release();
					pCoordinatesNode.Release();
				}
				else if(strRuleType == "Word")
				{
					CWordRule *pRule= new CWordRule;
					pRule->setRuleType(2);
					pRule->setRuleName(strRuleName);
					if(strRuleUseOrNot == "true")
						pRule->setIsUsed(true);
					else 
						pRule->setIsUsed(false);
					pRule->setLayerName(strRuleLayer);	

					MSXML2::IXMLDOMNodePtr pPraseNode, pEnvAttriNode,pWordLibNode;

					pRuleList->get_item(0,&pPraseNode);
					pRuleList->get_item(1,&pEnvAttriNode);
					pRuleList->get_item(2,&pWordLibNode);

					pPraseNode->get_nodeTypedValue(&VariantValue);	
					CString strPrase =(char *)(_bstr_t)VariantValue;	
					pRule->setPhrase(strPrase);
					pEnvAttriNode->get_nodeTypedValue(&VariantValue);	
					CString strEnvAttri =(char *)(_bstr_t)VariantValue;	
					pRule->setEnvAttri(strEnvAttri);
					pWordLibNode->get_nodeTypedValue(&VariantValue);
					CString strWordLib = (char *)(_bstr_t)VariantValue;


					strWordLib = m_strProjectDir +"\\"+strWordLib;
					pRule->setLibFileName(strWordLib);
					pRule->LoadWordLib();

					pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
					pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

					pRule->setDispMin(pRule->ComputeRuleGraphMin());
					pRule->setDispMax(pRule->ComputeRuleGraphMax());
					pOccurrence->AddRule((CRule *)pRule);
					pPraseNode.Release();
					pWordLibNode.Release();
				}
				else if(strRuleType == "Point")
				{
					CPointRule *pRule= new CPointRule;
					pRule->setRuleType(3);
					pRule->setRuleName(strRuleName);
					if(strRuleUseOrNot == "true")
						pRule->setIsUsed(true);
					else 
						pRule->setIsUsed(false);
					pRule->setLayerName(strRuleLayer);	

					MSXML2::IXMLDOMNodePtr pXYNode,pCurveTypeNode,pLWidthNode,pRWidthNode;
					pRuleList->get_item(0,&pXYNode);
					pRuleList->get_item(1,&pCurveTypeNode);
					pRuleList->get_item(2,&pLWidthNode);
					pRuleList->get_item(3,&pRWidthNode);
					pXYNode->get_nodeTypedValue(&VariantValue);	
					CString strXY =(char *)(_bstr_t)VariantValue;	
					pCurveTypeNode->get_nodeTypedValue(&VariantValue);	
					CString strCurveType =(char *)(_bstr_t)VariantValue;
					pLWidthNode->get_nodeTypedValue(&VariantValue);	
					CString strLWidth =(char *)(_bstr_t)VariantValue;
					pRWidthNode->get_nodeTypedValue(&VariantValue);	
					CString strRWidth =(char *)(_bstr_t)VariantValue;
					double X[1],Y[1];
					split1(strXY, X,Y);					
					pRule->setCentralX(X[0]);
					pRule->setCentralY(Y[0]);
					if(strCurveType = "Bell-shape")
						pRule->setCurveType(0);
					else if(strCurveType = "S-shape")
						pRule->setCurveType(1);
					else if(strCurveType = "Z-shape")
						pRule->setCurveType(2);
					pRule->setLWidth(atof(strLWidth));
					pRule->setRWidth(atof(strRWidth));

					pRule->ComputeCentralValue(m_pProject->m_pEnvDatabase);

					pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
					pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

					pRule->setDispMin(pRule->ComputeRuleGraphMin());
					pRule->setDispMax(pRule->ComputeRuleGraphMax());

					pOccurrence->AddRule((CRule *)pRule);
					pXYNode.Release();
					pCurveTypeNode.Release();
					pLWidthNode.Release();
					pRWidthNode.Release();

				}		
				else if(strRuleType ==  "Enumerated")
				{
					CEnumRule *pRule= new CEnumRule;
					pRule->setRuleType(4);
					pRule->setRuleName(strRuleName);
					if(strRuleUseOrNot == "true")
						pRule->setIsUsed(true);
					else 
						pRule->setIsUsed(false);
					pRule->setLayerName(strRuleLayer);	

					MSXML2::IXMLDOMNodePtr pItemNum, pEnumValueNode;

					pRuleList->get_item(0,&pItemNum);
					pRuleList->get_item(1,&pEnumValueNode);
					pItemNum->get_nodeTypedValue(&VariantValue);	
					CString strItemNum =(char *)(_bstr_t)VariantValue;	
					int ItemNum = atoi(strItemNum);
					if(ItemNum>100)
					{
						AfxMessageBox("The number of items must be smaller than 100");
						delete pRule;
						pRule = NULL;
						continue;

					}
					pEnumValueNode->get_nodeTypedValue(&VariantValue);	
					CString strItems =(char *)(_bstr_t)VariantValue;
					int Items[100];
					split1(strItems, Items);
					for(int i= 0;i<ItemNum;i++)			
						pRule->AddItem(Items[i]);	

					pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
					pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

					pRule->setDispMin(pRule->ComputeRuleGraphMin());
					pRule->setDispMax(pRule->ComputeRuleGraphMax());					
					pOccurrence->AddRule((CRule *)pRule);
					pItemNum.Release();
					pEnumValueNode.Release();
				}	


				pRuleNode.Release();
				pRuleAttr.Release();
				pRuleNameNode.Release();
				pRuleUseOrNotNode.Release();
				pRuleTypeNode.Release();
				pRuleLayerNode.Release();
				pRuleList.Release();
			}
			pSoilType->AddOccurrence(pOccurrence);
			pOccurrenceNode.Release();
			pOccurrenceAttr.Release();
			pOccurrenceNameNode.Release();
			pOccurrenceXNode.Release();
			pOccurrenceYNode.Release();
			pOccurrenceUseDistanceDecayNode.Release();
			pOccurrenceSearchDistNode.Release();
			pOccurrenceDistanceDecayNode.Release();
			pOccurrenceList.Release();
		}

		pOccurrencesList.Release();
		pOccurrencesNode.Release();

		//traverse child nodes of exclusios(every exclusion)
		MSXML2::IXMLDOMNodePtr pExclusionNode;
		MSXML2::IXMLDOMNamedNodeMapPtr pExclusionAttr;
		MSXML2::IXMLDOMNodePtr pExclusionNameNode,pExclusionXNode,pExclusionYNode,pExclusionUseDistanceDecayNode,pExclusionSearchDistNode,pExclusionDistanceDecayNode;
		MSXML2::IXMLDOMNodeListPtr pExclusionList;
		for(int n2=0;n2<exclusionNum;n2++)
		{
			CExclusion *pExclusion = new CExclusion;
			pExclusionsList->get_item(n2,&pExclusionNode);
			pExclusionNode->get_attributes(&pExclusionAttr);
			pExclusionNameNode = pExclusionAttr->getNamedItem("Name");
			pExclusionXNode = pExclusionAttr->getNamedItem("x");
			pExclusionYNode = pExclusionAttr->getNamedItem("y");
			pExclusionUseDistanceDecayNode = pExclusionAttr->getNamedItem("UseDistanceDecay");
			pExclusionSearchDistNode = pExclusionAttr->getNamedItem("SearchDist");
			pExclusionDistanceDecayNode = pExclusionAttr->getNamedItem("DistanceDecay");
			pExclusionNameNode->get_nodeTypedValue(&VariantValue);	
			CString strExclusionName =(char *)(_bstr_t)VariantValue;	
			pExclusion->setExclusionName(strExclusionName);

			pExclusionXNode->get_nodeTypedValue(&VariantValue);	
			CString strExclusionX =(char *)(_bstr_t)VariantValue;
			pExclusion->setCentralX(atof(strExclusionX));
			pExclusionYNode->get_nodeTypedValue(&VariantValue);	
			CString strExclusionY =(char *)(_bstr_t)VariantValue;
			pExclusion->setCentralY(atof(strExclusionY));			
			pExclusionSearchDistNode->get_nodeTypedValue(&VariantValue);	
			CString strExclusionSearchDist =(char *)(_bstr_t)VariantValue;
			pExclusion->setSearchDist(atof(strExclusionSearchDist));
			pExclusionUseDistanceDecayNode ->get_nodeTypedValue(&VariantValue);	
			CString strExclusionUseDistanceDecay =(char *)(_bstr_t)VariantValue;
			if(strExclusionUseDistanceDecay == "true")
			{
				pExclusion->setUseDistSim(true);
			}
			else
			{
				pExclusion->setUseDistSim(false);
			}
			pExclusionDistanceDecayNode->get_nodeTypedValue(&VariantValue);	
			CString strExclusionDistanceDecay =(char *)(_bstr_t)VariantValue;
			pExclusion->setDistDecayFactor(atof(strExclusionDistanceDecay));


			pExclusionNode->get_childNodes(&pExclusionList);
			long ruleNum;
			pExclusionList->get_length(&ruleNum);			
			//traver child nodes of exclusion( every rule)

			CString  strRuleName,strRuleUseOrNot,strRuleType,strRuleLayer;			
			for(long n11=0;n11<ruleNum;n11++)
			{
				MSXML2::IXMLDOMNodePtr pRuleNode;
				MSXML2::IXMLDOMNamedNodeMapPtr pRuleAttr;
				MSXML2::IXMLDOMNodePtr pRuleNameNode,pRuleUseOrNotNode,pRuleTypeNode,pRuleLayerNode;
				MSXML2::IXMLDOMNodeListPtr pRuleList;

				pExclusionList->get_item(n11,&pRuleNode);				
				pRuleNode->get_attributes(&pRuleAttr);				
				pRuleNameNode = pRuleAttr->getNamedItem("Name");
				pRuleUseOrNotNode = pRuleAttr->getNamedItem("UseOrNot");
				pRuleTypeNode = pRuleAttr->getNamedItem("Type");
				pRuleLayerNode = pRuleAttr->getNamedItem("DataLayer");
				pRuleNameNode->get_nodeTypedValue(&VariantValue);	
				strRuleName =(char *)(_bstr_t)VariantValue;	
				pRuleUseOrNotNode->get_nodeTypedValue(&VariantValue);	
				strRuleUseOrNot =(char *)(_bstr_t)VariantValue;	
				pRuleTypeNode->get_nodeTypedValue(&VariantValue);	
				strRuleType =(char *)(_bstr_t)VariantValue;	
				pRuleLayerNode->get_nodeTypedValue(&VariantValue);	
				strRuleLayer =(char *)(_bstr_t)VariantValue;

				pRuleNode->get_childNodes(&pRuleList);

				if(strRuleType == "Range")
				{
					CRangeRule *pRule= new CRangeRule;
					pRule->setRuleType(0);
					pRule->setRuleName(strRuleName);
					if(strRuleUseOrNot == "true")
						pRule->setIsUsed(true);
					else 
						pRule->setIsUsed(false);
					pRule->setLayerName(strRuleLayer);	

					MSXML2::IXMLDOMNodePtr pCurveTypeNode,pKeyValueNode;
					pRuleList->get_item(0,&pCurveTypeNode);
					pRuleList->get_item(1,&pKeyValueNode);
					pCurveTypeNode->get_nodeTypedValue(&VariantValue);	
					CString strCurveType =(char *)(_bstr_t)VariantValue;	
					pKeyValueNode->get_nodeTypedValue(&VariantValue);	
					CString strKeyValue =(char *)(_bstr_t)VariantValue;	
					double KeyValue[4] ;
					split1(strKeyValue,KeyValue);
					pRule->setLowCross(KeyValue[0]);
					pRule->setLowUnity(KeyValue[1]);
					pRule->setHighUnity(KeyValue[2]);
					pRule->setHighCross(KeyValue[3]);   
					if(strCurveType = "Bell-shape")
						pRule->setCurveType(0);
					else if(strCurveType = "S-shape")
						pRule->setCurveType(1);
					else if(strCurveType = "Z-shape")
						pRule->setCurveType(2);

					pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
					pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

					pRule->setDispMin(pRule->ComputeRuleGraphMin());
					pRule->setDispMax(pRule->ComputeRuleGraphMax());
					pExclusion->AddRule((CRule *)pRule);
					pCurveTypeNode.Release();
					pKeyValueNode.Release();
				}
				else if( strRuleType == "Curve")
				{
					CFreehandRule *pRule = new CFreehandRule;
					pRule->setRuleType(1);
					pRule->setRuleName(strRuleName);
					if(strRuleUseOrNot == "true")
						pRule->setIsUsed(true);
					else 
						pRule->setIsUsed(false);

					pRule->setLayerName(strRuleLayer);	

					MSXML2::IXMLDOMNodePtr pNodeNum,pCoordinatesNode;
					pRuleList->get_item(0,&pNodeNum);
					pRuleList->get_item(1,&pCoordinatesNode);
					pNodeNum->get_nodeTypedValue(&VariantValue);	
					CString strNodeNum =(char *)(_bstr_t)VariantValue;	
					int NodeNum = atoi(strNodeNum);

					pCoordinatesNode->get_nodeTypedValue(&VariantValue);	
					CString strCoordinates =(char *)(_bstr_t)VariantValue;	
					double NodeX[100];
					double NodeY[100];
					split1(strCoordinates,NodeX, NodeY);
					if(NodeNum>100)
					{
						AfxMessageBox("The number of nodes must be smaller than 100");
						delete pRule;
						pRule = NULL;
						continue;

					}
					for(int i=0;i<NodeNum; i++)				
						pRule->AddKnot(NodeX[i],NodeY[i]);

					pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
					pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

					pRule->setDispMin(pRule->ComputeRuleGraphMin());
					pRule->setDispMax(pRule->ComputeRuleGraphMax());
					pExclusion->AddRule((CRule *)pRule);
					pNodeNum.Release();
					pCoordinatesNode.Release();
				}
				else if(strRuleType == "Word")
				{
					CWordRule *pRule= new CWordRule;
					pRule->setRuleType(2);
					pRule->setRuleName(strRuleName);
					if(strRuleUseOrNot == "true")
						pRule->setIsUsed(true);
					else 
						pRule->setIsUsed(false);
					pRule->setLayerName(strRuleLayer);	

					MSXML2::IXMLDOMNodePtr pPraseNode, pEnvAttriNode,pWordLibNode;

					pRuleList->get_item(0,&pPraseNode);
					pRuleList->get_item(1,&pEnvAttriNode);
					pRuleList->get_item(2,&pWordLibNode);

					pPraseNode->get_nodeTypedValue(&VariantValue);	
					CString strPrase =(char *)(_bstr_t)VariantValue;	
					pRule->setPhrase(strPrase);
					pEnvAttriNode->get_nodeTypedValue(&VariantValue);	
					CString strEnvAttri =(char *)(_bstr_t)VariantValue;	
					pRule->setEnvAttri(strEnvAttri);
					pWordLibNode->get_nodeTypedValue(&VariantValue);
					CString strWordLib = (char *)(_bstr_t)VariantValue;

					strWordLib = m_strProjectDir +"\\"+strWordLib;
					pRule->setLibFileName(strWordLib);
					pRule->LoadWordLib();

					pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
					pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

					pRule->setDispMin(pRule->ComputeRuleGraphMin());
					pRule->setDispMax(pRule->ComputeRuleGraphMax());
					pExclusion->AddRule((CRule *)pRule);
					pPraseNode.Release();
					pWordLibNode.Release();
				}
				else if(strRuleType == "Point")
				{
					CPointRule *pRule= new CPointRule;
					pRule->setRuleType(3);
					pRule->setRuleName(strRuleName);
					if(strRuleUseOrNot == "true")
						pRule->setIsUsed(true);
					else 
						pRule->setIsUsed(false);
					pRule->setLayerName(strRuleLayer);	

					MSXML2::IXMLDOMNodePtr pXYNode,pCurveTypeNode,pLWidthNode,pRWidthNode;
					pRuleList->get_item(0,&pXYNode);
					pRuleList->get_item(1,&pCurveTypeNode);
					pRuleList->get_item(2,&pLWidthNode);
					pRuleList->get_item(3,&pRWidthNode);
					pXYNode->get_nodeTypedValue(&VariantValue);	
					CString strXY =(char *)(_bstr_t)VariantValue;	
					pCurveTypeNode->get_nodeTypedValue(&VariantValue);	
					CString strCurveType =(char *)(_bstr_t)VariantValue;
					pLWidthNode->get_nodeTypedValue(&VariantValue);	
					CString strLWidth =(char *)(_bstr_t)VariantValue;
					pRWidthNode->get_nodeTypedValue(&VariantValue);	
					CString strRWidth =(char *)(_bstr_t)VariantValue;
					double X[1],Y[1];
					split1(strXY, X,Y);					
					pRule->setCentralX(X[0]);
					pRule->setCentralY(Y[0]);
					if(strCurveType = "Bell-shape")
						pRule->setCurveType(0);
					else if(strCurveType = "S-shape")
						pRule->setCurveType(1);
					else if(strCurveType = "Z-shape")
						pRule->setCurveType(2);
					pRule->setLWidth(atof(strLWidth));
					pRule->setRWidth(atof(strRWidth));

					pRule->ComputeCentralValue(m_pProject->m_pEnvDatabase);

					pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
					pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

					pRule->setDispMin(pRule->ComputeRuleGraphMin());
					pRule->setDispMax(pRule->ComputeRuleGraphMax());
					pExclusion->AddRule((CRule *)pRule);
					pXYNode.Release();
					pCurveTypeNode.Release();
					pLWidthNode.Release();
					pRWidthNode.Release();
				}		
				else if(strRuleType ==  "Enumerated")
				{
					CEnumRule *pRule= new CEnumRule;
					pRule->setRuleType(4);
					pRule->setRuleName(strRuleName);
					if(strRuleUseOrNot == "true")
						pRule->setIsUsed(true);
					else 
						pRule->setIsUsed(false);
					pRule->setLayerName(strRuleLayer);	

					MSXML2::IXMLDOMNodePtr pItemNum, pEnumValueNode;

					pRuleList->get_item(0,&pItemNum);
					pRuleList->get_item(1,&pEnumValueNode);
					pItemNum->get_nodeTypedValue(&VariantValue);	
					CString strItemNum =(char *)(_bstr_t)VariantValue;	
					int ItemNum = atoi(strItemNum);
					if(ItemNum>100)
					{
						AfxMessageBox("The number of items must be smaller than 100");
						delete pRule;
						pRule = NULL;
						continue;

					}
					pEnumValueNode->get_nodeTypedValue(&VariantValue);	
					CString strItems =(char *)(_bstr_t)VariantValue;
					int Items[100];
					split1(strItems, Items);
					for(int i= 0;i<ItemNum;i++)			
						pRule->AddItem(Items[i]);	

					pRule->setCurGraphMin(pRule->ComputeRuleGraphMin());
					pRule->setCurGraphMax(pRule->ComputeRuleGraphMax());

					pRule->setDispMin(pRule->ComputeRuleGraphMin());
					pRule->setDispMax(pRule->ComputeRuleGraphMax());					
					pExclusion->AddRule((CRule *)pRule);
					pItemNum.Release();
					pEnumValueNode.Release();

				}
				pRuleNode.Release();
				pRuleAttr.Release();
				pRuleNameNode.Release();
				pRuleUseOrNotNode.Release();
				pRuleTypeNode.Release();
				pRuleLayerNode.Release();
				pRuleList.Release();
			}
			pSoilType->AddExclusion(pExclusion);
			pExclusionNode.Release();
			pExclusionAttr.Release();
			pExclusionNameNode.Release();
			pExclusionXNode.Release();
			pExclusionYNode.Release();
			pExclusionUseDistanceDecayNode.Release();
			pExclusionSearchDistNode.Release();
			pExclusionDistanceDecayNode.Release();
			pExclusionList.Release();
		}

		pExclusionsList.Release();
		pExclusionsNode.Release();
		pProject->m_pKnowledgeBase->AddSoilType(pSoilType);

		//release the soil nodes
		pSoilTypeNode.Release();
		pSoilTypeList.Release();
		pSoilNameNode.Release();
		pSoilTypeAttr.Release();
	}




	pRootKnowledgeBaseList.Release();

	pRootKnowledgeElem.Release();
	//	pRootKnowledgeAttr.Release();
	pDocKnowledge.Release();
	CoUninitialize();



}




void CSoLIMDoc::WriteProject(CString strFileName, CProject *pProject)
{
	MSXML2::IXMLDOMDocumentPtr pDoc;
	HRESULT hr;

	hr=CoInitialize(NULL);
	if(FAILED(hr))
	{
		AfxMessageBox("Error in saving project!");
		return;
	}

	hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));
	if(FAILED(hr))
	{
		AfxMessageBox("Error in saving project!");
		return;
	}
	pDoc->preserveWhiteSpace=TRUE;

	MSXML2::IXMLDOMProcessingInstructionPtr pPI = NULL;
	pPI = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
	_variant_t vNullVal;
	vNullVal.vt = VT_NULL;
	pDoc->insertBefore(pPI, vNullVal);
	pPI.Release();


	//*********** create root node "SoLIMProject"***************************
	MSXML2::IXMLDOMElementPtr pRootElem;
	pRootElem=pDoc->createElement("SoLIMProject");
	//create the attributes of the root node
	MSXML2::IXMLDOMAttributePtr attrib;
	CString attri;
	attrib=pDoc->createAttribute("Version");
	if(attrib!=NULL)
	{
		attri="2010";
		_variant_t varattri(attri);
		attrib->value=varattri;
		pRootElem->setAttributeNode(attrib);

	}
	attrib.Release();
	attrib=pDoc->createAttribute("Name");
	if(attrib!=NULL)
	{
		attri=pProject->getProjectName();
		_variant_t varattri(attri);
		attrib->value=varattri;
		pRootElem->setAttributeNode(attrib);

	}
	attrib.Release();
	attrib=pDoc->createAttribute("ProjectType");
	if(attrib!=NULL)
	{
		attri="RBI";
		_variant_t varattri(attri);
		attrib->value=varattri;
		pRootElem->setAttributeNode(attrib);

	}
	attrib.Release();
	attrib=pDoc->createAttribute("DataType");
	if(attrib!=NULL)
	{
		attrib->value="3dr";
		pRootElem->setAttributeNode(attrib);

	}
	attrib.Release();
	//add the root node to the DOM instance
	pDoc->appendChild(pRootElem);
	MSXML2::IXMLDOMTextPtr   pTextNode; 

	//**********  Create database node ***************
	MSXML2::IXMLDOMElementPtr pDatabaseNode;
	pDatabaseNode=pDoc->createElement("Database");
	if(pDatabaseNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n "); 
		pRootElem->appendChild(pTextNode);
		pTextNode.Release();
		pRootElem->appendChild(pDatabaseNode);
	}
	//append layer node
	MSXML2::IXMLDOMElementPtr pLayernode;
	for(int n1=0;n1<pProject->m_pEnvDatabase->getLayerNum();n1++)
	{
		pLayernode=pDoc->createElement("layer");
		if(pLayernode!=NULL)
		{
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pDatabaseNode->appendChild(pTextNode);
			pTextNode.Release();		
			pDatabaseNode->appendChild(pLayernode);
			pTextNode = pDoc->createTextNode(L"\n   "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();
			MSXML2::IXMLDOMElementPtr pLayerNameNode,pLayerPathNode;
			pLayerNameNode=pDoc->createElement("Name");
			CString layername=pProject->m_pEnvDatabase->getLayer(n1)->getLayerName();
			BSTR layername_bstr=layername.AllocSysString();
			pLayerNameNode->text=layername_bstr;
			pLayernode->appendChild(pLayerNameNode);
			pTextNode = pDoc->createTextNode(L"\n   "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();
			pLayerPathNode=pDoc->createElement("Path");
			CString filename=extractFileName(pProject->m_pEnvDatabase->getLayer(n1)->getFileName());
			BSTR filename_bstr=filename.AllocSysString();
			pLayerPathNode->text=filename_bstr;
			pLayernode->appendChild(pLayerPathNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

		}
		pLayernode.Release();

	}   

	pTextNode = pDoc->createTextNode(L"\n "); 
	pDatabaseNode->appendChild(pTextNode);
	pTextNode.Release();
	pDatabaseNode.Release();


	//************* Create KnowledgeBase file****************


	MSXML2::IXMLDOMDocumentPtr pDocKnowledge;
	HRESULT hrKnowledge;

	hrKnowledge=CoInitialize(NULL);
	if(FAILED(hrKnowledge))
	{
		AfxMessageBox("Error in saving project!");
		return;
	}

	hrKnowledge=pDocKnowledge.CreateInstance(__uuidof(MSXML2::DOMDocument));
	if(FAILED(hrKnowledge))
	{
		AfxMessageBox("Error in saving project!");
		return;
	}
	pDocKnowledge->preserveWhiteSpace=TRUE;

	MSXML2::IXMLDOMProcessingInstructionPtr pPIKnowledge = NULL;
	pPIKnowledge = pDocKnowledge->createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");

	pDocKnowledge->insertBefore(pPIKnowledge, vNullVal);
	pPIKnowledge.Release();




	MSXML2::IXMLDOMElementPtr pRootKnowledgebaseElem;
	pRootKnowledgebaseElem=pDocKnowledge->createElement("KnowledgeBase");
	if(pRootKnowledgebaseElem!=NULL)
	{  
		//pTextNode = pDocKnowledge->createTextNode(L"\n "); 
		//pRootKnowledgebaseElem->appendChild(pTextNode);
		//pTextNode.Release();
		pDocKnowledge->appendChild(pRootKnowledgebaseElem);
	}

	MSXML2::IXMLDOMElementPtr pSoilNameNode;
	for (int n2=0;n2<pProject->m_pKnowledgeBase->getSoilTypeNum();n2++)
	{
		pSoilNameNode=pDocKnowledge->createElement("SoilType");
		if(pSoilNameNode!=NULL)
		{
			pTextNode = pDocKnowledge->createTextNode(L"\n  "); 
			pRootKnowledgebaseElem->appendChild(pTextNode);
			pTextNode.Release();

			attrib=pDocKnowledge->createAttribute("Name");
			if(attrib!=NULL)
			{
				CString soiltype=pProject->m_pKnowledgeBase->getSoilType(n2)->getSoilTypeName();
				_variant_t soiltype_var((LPCTSTR)soiltype);
				attrib->value=soiltype_var;
				pSoilNameNode->setAttributeNode(attrib);
				attrib.Release();
			}
			pRootKnowledgebaseElem->appendChild(pSoilNameNode);

			//instances
			MSXML2::IXMLDOMElementPtr pInstancesNode;
			pInstancesNode=pDocKnowledge->createElement("Instances");
			if(pInstancesNode!=NULL)
			{
				pTextNode = pDocKnowledge->createTextNode(L"\n   "); 
				pSoilNameNode->appendChild(pTextNode);
				pTextNode.Release();

				pSoilNameNode->appendChild(pInstancesNode);

				MSXML2::IXMLDOMElementPtr pInstance;
				for(int n3=0;n3<pProject->m_pKnowledgeBase->getSoilType(n2)->getInstanceNum();n3++)
				{
					pInstance=pDocKnowledge->createElement("Instance");
					if(pInstance!=NULL)
					{
						pTextNode = pDocKnowledge->createTextNode(L"\n    "); 
						pInstancesNode->appendChild(pTextNode);
						pTextNode.Release();

						attrib=pDocKnowledge->createAttribute("Name");
						if(attrib!=NULL)
						{
							CString instancename=pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getInstanceName();
							_variant_t instancename_var((LPCTSTR)instancename);
							attrib->value=instancename_var;
							pInstance->setAttributeNode(attrib);

						}
						attrib.Release();
						pInstancesNode->appendChild(pInstance);

						MSXML2::IXMLDOMElementPtr pRuleNode;
						for(int n4=0;n4<pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRuleNum();n4++)
						{
							pRuleNode=pDocKnowledge->createElement("Rule");
							if(pRuleNode!=NULL)
							{
								pTextNode = pDocKnowledge->createTextNode(L"\n     "); 
								pInstance->appendChild(pTextNode);
								pTextNode.Release();

								attrib=pDocKnowledge->createAttribute("Name");
								if(attrib!=NULL)
								{
									CString rulename=pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleName();
									_variant_t rulename_var((LPCTSTR)rulename);
									attrib->value=rulename_var;
									pRuleNode->setAttributeNode(attrib);								
								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("UseOrNot");
								if(attrib!=NULL)
								{
									if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getIsUsed()==true)
										attrib->value="true";
									else 
										attrib->value="false";

									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("Type");
								if(attrib!=NULL)
								{
									int a=pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType();
									switch(a)
									{
									case 0:
										attrib->value="Range";
										break;
									case 1:
										attrib->value="Curve";
										break;
									case 2:
										attrib->value="Word";
										break;
									case 3:
										attrib->value="Point";									
										break;
									case 4:
										attrib->value="Enumerated";										
										break;

									}
									pRuleNode->setAttributeNode(attrib);


								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("DataLayer");
								if(attrib!=NULL)
								{
									CString datalayername=pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getLayerName();
									_variant_t datalayername_var((LPCSTR)datalayername);
									attrib->value=datalayername_var;
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();
								pInstance->appendChild(pRuleNode);



								//Range
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 0)
								{
									CRangeRule *m_pRange;;
									m_pRange=(CRangeRule *)(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));

									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDocKnowledge->createElement("CurveType");
									if(pCurveTypeNode!=NULL)
									{
										int curvT=m_pRange->getCurveType();
										switch(curvT)
										{
										case 0:
											pCurveTypeNode->text="Bell-shape";
											break;
										case 1:
											pCurveTypeNode->text="S-shape";
											break;
										case 2:
											pCurveTypeNode->text="Z-shape";
											break;
										}
										pRuleNode->appendChild(pCurveTypeNode);
									}
									pCurveTypeNode.Release();
									MSXML2::IXMLDOMElementPtr pKeyValueNode;
									pKeyValueNode=pDocKnowledge->createElement("KeyValue");
									if(pKeyValueNode!=NULL)
									{
										double KeyValue[4];
										KeyValue[0]=m_pRange->getLowCross();
										KeyValue[1]=m_pRange->getLowUnity();
										KeyValue[2]=m_pRange->getHighUnity();
										KeyValue[3]=m_pRange->getHighCross();
										//char *s=new char[];
										CString str;
										int i2=0;
										for(int j2=0;j2<4;j2++)
										{
											CString tempStr;
											tempStr.Format("%f",KeyValue[j2]);
											if(j2!=3)
												str = str + tempStr+",";
											else 
												str = str + tempStr;

										}
										pKeyValueNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pKeyValueNode);
										//delete []s;
									}
									pKeyValueNode.Release();
								}

								//Curve
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 1)
								{
									CFreehandRule *m_pCurve; 
									m_pCurve= (CFreehandRule *)(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));
									MSXML2::IXMLDOMElementPtr pNodeNum,pCoordinatesNode;

									pNodeNum=pDocKnowledge->createElement("NodeNum");
									CString str;
									str.Format("%d",m_pCurve->getKnotNum());								
									pNodeNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pNodeNum);
									pNodeNum.Release();

									pCoordinatesNode=pDocKnowledge->createElement("Coordinates");
									if(pCoordinatesNode!=NULL)
									{
										int i1=0;
										CString str;
										for(int j=0;j<m_pCurve->getKnotNum();j++)
										{
											double x, y;
											CString tempString;
											m_pCurve->getKnot(j,&x,&y);
											tempString.Format("%f %f",x,y);
											if(j!=m_pCurve->getKnotNum()-1)
												str = str + tempString + ",";
											else 
												str = str + tempString; 
										}
										pCoordinatesNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pCoordinatesNode);
									}
									pCoordinatesNode.Release();
								}

								//Word
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 2)
								{
									CWordRule *m_pWord=new CWordRule;
									m_pWord=static_cast<CWordRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));
									MSXML2::IXMLDOMElementPtr pPraseNode;
									pPraseNode=pDocKnowledge->createElement("Prase");
									if(pPraseNode!=NULL)
									{
										CString phrase=m_pWord->getPhrase();
										_bstr_t phrase_bstr=phrase.AllocSysString();
										pPraseNode->text=phrase_bstr;
										pRuleNode->appendChild(pPraseNode);
									}
									pPraseNode.Release();

									MSXML2::IXMLDOMElementPtr pEnvAttriNode;
									pEnvAttriNode=pDocKnowledge->createElement("EnvAttri");
									if(pEnvAttriNode!=NULL)
									{
										CString EnvAttri=m_pWord->getEnvAttri();
										_bstr_t EnvAttri_bstr=EnvAttri.AllocSysString();
										pEnvAttriNode->text=EnvAttri_bstr;
										pRuleNode->appendChild(pEnvAttriNode);
									}
									pEnvAttriNode.Release();

									MSXML2::IXMLDOMElementPtr pWordLibNode;
									pWordLibNode=pDocKnowledge->createElement("WordLib");
									if(pWordLibNode!=NULL)
									{
										CString libfile=extractFileName(m_pWord->getLibFileName());
										_bstr_t libfile_bstr=libfile.AllocSysString();
										pWordLibNode->text=libfile_bstr;
										pRuleNode->appendChild(pWordLibNode);
									}
									pWordLibNode.Release();
								}


								//Point
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 3)
								{
									CPointRule *m_pPoint=new CPointRule;
									m_pPoint=static_cast<CPointRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));
									MSXML2::IXMLDOMElementPtr pXYNode;
									pXYNode=pDocKnowledge->createElement("XY");
									if(pXYNode!=NULL)
									{
										CString str;
										str.Format("%f %f",m_pPoint->getCentralX(),m_pPoint->getCentralY());
										pXYNode->text= (_bstr_t)str;
										pRuleNode->appendChild(pXYNode);
									}
									pXYNode.Release();
									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDocKnowledge->createElement("CurveType");
									if(pCurveTypeNode!=NULL)
									{
										int curvT=m_pPoint->getCurveType();
										switch(curvT)
										{
										case 0:
											pCurveTypeNode->text="Bell-shape";
											break;
										case 1:
											pCurveTypeNode->text="S-shape";
											break;
										case 2:
											pCurveTypeNode->text="Z-shape";
											break;
										}
										pRuleNode->appendChild(pCurveTypeNode);
									}
									pCurveTypeNode.Release();

									MSXML2::IXMLDOMElementPtr pLWidthNode;
									pLWidthNode=pDocKnowledge->createElement("LWidth");

									if(pLWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getLWidth());
										pLWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pLWidthNode);
									}
									pLWidthNode.Release();

									MSXML2::IXMLDOMElementPtr pRWidthNode;
									pRWidthNode=pDocKnowledge->createElement("RWidth");

									if(pRWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getRWidth());
										pRWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pRWidthNode);
									}
									pRWidthNode.Release();								
								}     

								//Enumerated
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 4)
								{
									CEnumRule *m_pEnum=new CEnumRule;
									m_pEnum=static_cast<CEnumRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));
									MSXML2::IXMLDOMElementPtr pItemNum,pEnumValueNode;

									pItemNum=pDocKnowledge->createElement("ItemNum");
									CString str;
									str.Format("%d",m_pEnum->getItemNum());								
									pItemNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pItemNum);
									pItemNum.Release();

									pEnumValueNode=pDocKnowledge->createElement("EnumValue");
									if(pEnumValueNode!=NULL)
									{
										int i3=0;
										CString str = "";
										for(int j3=0;j3<m_pEnum->getItemNum();j3++)
										{
											CString TempStr;
											TempStr.Format("%d",m_pEnum->getItem(j3));
											if(j3!=m_pEnum->getItemNum()-1)
											{
												str+=TempStr;
												str+=",";
											}
											else
												str+=TempStr;

										}
										pEnumValueNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pEnumValueNode);
									}
									pEnumValueNode.Release();
								}

							}
							pRuleNode.Release();
						}
					}
					pTextNode = pDocKnowledge->createTextNode(L"\n    "); 
					pInstance->appendChild(pTextNode);
					pTextNode.Release();
					pInstance.Release();
				}
			}
			pTextNode = pDocKnowledge->createTextNode(L"\n   "); 
			pInstancesNode->appendChild(pTextNode);
			pTextNode.Release();

			pInstancesNode.Release();


			//Occurrences
			MSXML2::IXMLDOMElementPtr pOccurrencesNode;
			pOccurrencesNode=pDocKnowledge->createElement("Occurrences");
			if(pOccurrencesNode!=NULL)
			{

				pTextNode = pDocKnowledge->createTextNode(L"\n   "); 
				pSoilNameNode->appendChild(pTextNode);
				pTextNode.Release();

				pSoilNameNode->appendChild(pOccurrencesNode);

				MSXML2::IXMLDOMElementPtr pOccurrence;
				for(int n5=0;n5<pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrenceNum();n5++)
				{
					pOccurrence=pDocKnowledge->createElement("Occurrence");
					if(pOccurrence!=NULL)
					{
						pTextNode = pDocKnowledge->createTextNode(L"\n    "); 
						pOccurrencesNode->appendChild(pTextNode);
						pTextNode.Release();

						attrib=pDocKnowledge->createAttribute("Name");
						if(attrib!=NULL)
						{
							CString occurname=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getOccurrenceName();
							_variant_t occurname_var((LPCSTR)occurname);
							attrib->value=occurname_var;
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("x");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getCentralX();
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("y");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getCentralY();
							pOccurrence->setAttributeNode(attrib);

						}	
						attrib.Release();						
						attrib=pDocKnowledge->createAttribute("SearchDist");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getSearchDist();
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("UseDistanceDecay");
						if(attrib!=NULL)
						{
							if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getUseDistSim() == true)
								attrib->value= "true";
							else
								attrib->value = "false";
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("DistanceDecay");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getDistDecayFactor();
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						pOccurrencesNode->appendChild(pOccurrence);

						MSXML2::IXMLDOMElementPtr pRuleNode;
						for(int n6=0;n6<pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRuleNum();n6++)
						{
							pRuleNode=pDocKnowledge->createElement("Rule");
							if(pRuleNode!=NULL)
							{
								pTextNode = pDocKnowledge->createTextNode(L"\n     "); 
								pOccurrence->appendChild(pTextNode);
								pTextNode.Release();

								attrib=pDocKnowledge->createAttribute("Name");
								if(attrib!=NULL)
								{
									CString rulename=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleName();
									_variant_t rulename_var((LPCSTR)rulename);
									attrib->value=rulename_var;
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("UseOrNot");
								if(attrib!=NULL)
								{
									if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getIsUsed()==true)
										attrib->value="true";
									else 
										attrib->value="false";

									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("Type");
								if(attrib!=NULL)
								{
									int a=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType();
									switch(a)
									{
									case 0:
										attrib->value="Range";
										break;
									case 1:
										attrib->value="Curve";
										break;
									case 2:
										attrib->value="Word";
										break;
									case 3:
										attrib->value="Point";									
										break;
									case 4:
										attrib->value="Enumerated";										
										break;

									}
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("DataLayer");
								if(attrib!=NULL)
								{
									CString layername=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getLayerName();
									_variant_t layername_var((LPCSTR)layername);
									attrib->value=layername_var;
									pRuleNode->setAttributeNode(attrib);

								}	
								attrib.Release();
								pOccurrence->appendChild(pRuleNode);


								//Range
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()==0)
								{
									CRangeRule *m_pRange = new CRangeRule;
									m_pRange=static_cast<CRangeRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));

									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDocKnowledge->createElement("CurveType");
									if(pCurveTypeNode!=NULL)
									{
										int curvT=m_pRange->getCurveType();
										switch(curvT)
										{
										case 0:
											pCurveTypeNode->text="Bell-shape";
											break;
										case 1:
											pCurveTypeNode->text="S-shape";
											break;
										case 2:
											pCurveTypeNode->text="Z-shape";
											break;
										}
										pRuleNode->appendChild(pCurveTypeNode);
									}
									pCurveTypeNode.Release();

									MSXML2::IXMLDOMElementPtr pKeyValueNode;
									pKeyValueNode=pDocKnowledge->createElement("KeyValue");
									if(pKeyValueNode!=NULL)
									{
										double KeyValue[4];
										KeyValue[0]=m_pRange->getLowCross();
										KeyValue[1]=m_pRange->getLowUnity();
										KeyValue[2]=m_pRange->getHighUnity();
										KeyValue[3]=m_pRange->getHighCross();
										//char *s=new char[];
										CString str;
										int i2=0;
										for(int j2=0;j2<4;j2++)
										{
											CString tempStr;
											tempStr.Format("%f",KeyValue[j2]);
											if(j2!=3)
												str = str + tempStr+",";
											else 
												str = str + tempStr;

										}
										pKeyValueNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pKeyValueNode);
										//delete []s;
									}
									pKeyValueNode.Release();
								}
								//Curve
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()== 1)
								{
									CFreehandRule *m_pCurve = new CFreehandRule;
									m_pCurve= static_cast<CFreehandRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));
									MSXML2::IXMLDOMElementPtr pNodeNum,pCoordinatesNode;
									pNodeNum=pDocKnowledge->createElement("NodeNum");
									CString str;
									str.Format("%d",m_pCurve->getKnotNum());								
									pNodeNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pNodeNum);
									pNodeNum.Release();

									pCoordinatesNode=pDocKnowledge->createElement("Coordinates");
									if(pCoordinatesNode!=NULL)
									{
										int i1=0;
										CString str;
										for(int j=0;j<m_pCurve->getKnotNum();j++)
										{
											double x, y;
											CString tempString;
											m_pCurve->getKnot(j,&x,&y);
											tempString.Format("%f %f",x,y);
											if(j!=m_pCurve->getKnotNum()-1)
												str = str + tempString + ",";
											else 
												str = str + tempString; 							
										}
										pCoordinatesNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pCoordinatesNode);
									}
									pCoordinatesNode.Release();
								}


								//Word
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()== 2)
								{
									CWordRule *m_pWord=new CWordRule;
									m_pWord=static_cast<CWordRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));
									MSXML2::IXMLDOMElementPtr pPraseNode;
									pPraseNode=pDocKnowledge->createElement("Prase");
									if(pPraseNode!=NULL)
									{
										CString phrase=m_pWord->getPhrase();
										_bstr_t phrase_bstr=phrase.AllocSysString();
										pPraseNode->text=phrase_bstr;
										pRuleNode->appendChild(pPraseNode);
									}
									pPraseNode.Release();


									MSXML2::IXMLDOMElementPtr pEnvAttriNode;
									pEnvAttriNode=pDocKnowledge->createElement("EnvAttri");
									if(pEnvAttriNode!=NULL)
									{
										CString EnvAttri=m_pWord->getEnvAttri();
										_bstr_t EnvAttri_bstr=EnvAttri.AllocSysString();
										pEnvAttriNode->text=EnvAttri_bstr;
										pRuleNode->appendChild(pEnvAttriNode);
									}
									pEnvAttriNode.Release();

									MSXML2::IXMLDOMElementPtr pWordLibNode;
									pWordLibNode=pDocKnowledge->createElement("WordLib");
									if(pWordLibNode!=NULL)
									{
										CString libfile=extractFileName(m_pWord->getLibFileName());
										_bstr_t libfile_bstr=libfile.AllocSysString();
										pWordLibNode->text=libfile_bstr;
										pRuleNode->appendChild(pWordLibNode);
									}
									pWordLibNode.Release();
								}


								//Point
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()== 3)
								{
									CPointRule *m_pPoint=new CPointRule;
									m_pPoint=static_cast<CPointRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));
									MSXML2::IXMLDOMElementPtr pXYNode;
									pXYNode=pDocKnowledge->createElement("XY");
									if(pXYNode!=NULL)
									{
										//char *XYNode=new char[];
										CString str;
										str.Format("%f %f",m_pPoint->getCentralX(),m_pPoint->getCentralY());
										pXYNode->text= (_bstr_t)str;
										pRuleNode->appendChild(pXYNode);
									}
									pXYNode.Release();	

									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDocKnowledge->createElement("CurveType");
									if(pCurveTypeNode!=NULL)
									{
										int curvT=m_pPoint->getCurveType();
										switch(curvT)
										{
										case 0:
											pCurveTypeNode->text="Bell-shape";
											break;
										case 1:
											pCurveTypeNode->text="S-shape";
											break;
										case 2:
											pCurveTypeNode->text="Z-shape";
											break;
										}
										pRuleNode->appendChild(pCurveTypeNode);
									}
									pCurveTypeNode.Release();

									MSXML2::IXMLDOMElementPtr pLWidthNode;
									pLWidthNode=pDocKnowledge->createElement("LWidth");

									if(pLWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getLWidth());
										pLWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pLWidthNode);
									}
									pLWidthNode.Release();

									MSXML2::IXMLDOMElementPtr pRWidthNode;
									pRWidthNode=pDocKnowledge->createElement("RWidth");

									if(pRWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getRWidth());
										pRWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pRWidthNode);
									}
									pRWidthNode.Release();
									//delete Width;
								}


								//Enumerated
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()== 4)
								{
									CEnumRule *m_pEnum=new CEnumRule;
									m_pEnum=static_cast<CEnumRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));
									MSXML2::IXMLDOMElementPtr pItemNum,pEnumValueNode;

									pItemNum=pDocKnowledge->createElement("ItemNum");
									CString str;
									str.Format("%d",m_pEnum->getItemNum());								
									pItemNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pItemNum);
									pItemNum.Release();
									pEnumValueNode=pDocKnowledge->createElement("EnumValue");
									if(pEnumValueNode!=NULL)
									{
										//char *EnumV=new char[];
										int i3=0;
										CString str = "";
										for(int j3=0;j3<m_pEnum->getItemNum();j3++)
										{
											CString TempStr;
											TempStr.Format("%d",m_pEnum->getItem(j3));
											if(j3!=m_pEnum->getItemNum()-1)
											{
												str+=TempStr;
												str+=",";
											}
											else
												str+=TempStr;

										}
										pEnumValueNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pEnumValueNode);
									}pEnumValueNode.Release();
								}

							}
							pRuleNode.Release();
						}
					}
					pTextNode = pDocKnowledge->createTextNode(L"\n    "); 
					pOccurrence->appendChild(pTextNode);
					pTextNode.Release();
					pOccurrence.Release();
				}
			}



			pTextNode = pDocKnowledge->createTextNode(L"\n   "); 
			pOccurrencesNode->appendChild(pTextNode);
			pTextNode.Release();

			pOccurrencesNode.Release();
			//Exclusions
			MSXML2::IXMLDOMElementPtr pExclusionsNode;
			pExclusionsNode=pDocKnowledge->createElement("Exclusions");
			if(pExclusionsNode!=NULL)
			{
				pTextNode = pDocKnowledge->createTextNode(L"\n   "); 
				pSoilNameNode->appendChild(pTextNode);
				pTextNode.Release();
				pSoilNameNode->appendChild(pExclusionsNode);

				MSXML2::IXMLDOMElementPtr pExclusion;
				for(int n7=0;n7<pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusionNum();n7++)
				{
					pExclusion=pDocKnowledge->createElement("Exclusion");
					if(pExclusion!=NULL)
					{
						pTextNode = pDocKnowledge->createTextNode(L"\n    "); 
						pExclusionsNode->appendChild(pTextNode);
						pTextNode.Release();

						attrib=pDocKnowledge->createAttribute("Name");
						if(attrib!=NULL)
						{
							CString excluname=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getExclusionName();
							_variant_t excluname_var((LPCSTR)excluname);
							attrib->value=excluname_var;
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("x");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getCentralX();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("y");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getCentralY();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();

						attrib=pDocKnowledge->createAttribute("SearchDist");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getSearchDist();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("UseDistanceDecay");
						if(attrib!=NULL)
						{
							if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getUseDistSim() == true)
								attrib->value = "true";
							else 
								attrib->value = "false";

							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("DistanceDecay");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getDistDecayFactor();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						pExclusionsNode->appendChild(pExclusion);

						MSXML2::IXMLDOMElementPtr pRuleNode;
						for(int n8=0;n8<pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRuleNum();n8++)
						{
							pRuleNode=pDocKnowledge->createElement("Rule");
							if(pRuleNode!=NULL)
							{
								pTextNode = pDocKnowledge->createTextNode(L"\n     "); 
								pExclusion->appendChild(pTextNode);
								pTextNode.Release();

								attrib=pDocKnowledge->createAttribute("Name");
								if(attrib!=NULL)
								{
									CString rulename=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleName();
									_variant_t rulename_var((LPCSTR)rulename);
									attrib->value=rulename_var;
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("UseOrNot");
								if(attrib!=NULL)
								{
									if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getIsUsed()==true)
										attrib->value="true";
									else 
										attrib->value="false";

									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("Type");
								if(attrib!=NULL)
								{
									int a=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType();
									switch(a)
									{
									case 0:
										attrib->value="Range";
										break;
									case 1:
										attrib->value="Curve";
										break;
									case 2:
										attrib->value="Word";
										break;
									case 3:
										attrib->value="Point";									
										break;
									case 4:
										attrib->value="Enumerated";										
										break;

									}
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("DataLayer");
								if(attrib!=NULL)
								{
									CString datalayername=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getLayerName();
									_variant_t datalayername_var((LPCSTR)datalayername);
									attrib->value=datalayername_var;
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();
								pExclusion->appendChild(pRuleNode);


								//Range
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 0)
								{
									CRangeRule *m_pRange = new CRangeRule;
									m_pRange=static_cast<CRangeRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));

									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDocKnowledge->createElement("CurveType");
									if(pCurveTypeNode!=NULL)
									{
										int curvT=m_pRange->getCurveType();
										switch(curvT)
										{
										case 0:
											pCurveTypeNode->text="Bell-shape";
											break;
										case 1:
											pCurveTypeNode->text="S-shape";
											break;
										case 2:
											pCurveTypeNode->text="Z-shape";
											break;

										}
										pRuleNode->appendChild(pCurveTypeNode);
									}
									pCurveTypeNode.Release();

									MSXML2::IXMLDOMElementPtr pKeyValueNode;
									pKeyValueNode=pDocKnowledge->createElement("KeyValue");
									if(pKeyValueNode!=NULL)
									{
										double KeyValue[4];
										KeyValue[0]=m_pRange->getLowCross();
										KeyValue[1]=m_pRange->getLowUnity();
										KeyValue[2]=m_pRange->getHighUnity();
										KeyValue[3]=m_pRange->getHighCross();
										//char *s=new char[];
										CString str;
										int i2=0;
										for(int j2=0;j2<4;j2++)
										{
											CString tempStr;
											tempStr.Format("%f",KeyValue[j2]);
											if(j2!=3)
												str = str + tempStr+",";
											else 
												str = str + tempStr;

										}
										pKeyValueNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pKeyValueNode);
										//delete []s;
									}
									pKeyValueNode.Release();
								}

								//Curve
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 1)
								{
									CFreehandRule *m_pCurve = new CFreehandRule;
									m_pCurve= static_cast<CFreehandRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));

									MSXML2::IXMLDOMElementPtr pNodeNum,pCoordinatesNode;
									pNodeNum=pDocKnowledge->createElement("NodeNum");
									CString str;
									str.Format("%d",m_pCurve->getKnotNum());								
									pNodeNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pNodeNum);
									pNodeNum.Release();

									pCoordinatesNode=pDocKnowledge->createElement("Coordinates");
									if(pCoordinatesNode!=NULL)
									{
										int i1=0;
										CString str;
										for(int j=0;j<m_pCurve->getKnotNum();j++)
										{
											double x, y;
											CString tempString;
											m_pCurve->getKnot(j,&x,&y);
											tempString.Format("%f %f",x,y);
											if(j!=m_pCurve->getKnotNum()-1)
												str = str + tempString + ",";
											else 
												str = str + tempString; 
											//i1+=sprintf(s+i1,"%f,%f ",&x,&y);
										}
										pCoordinatesNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pCoordinatesNode);
									}
									pCoordinatesNode.Release();
								}


								//Word
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 2)
								{
									CWordRule *m_pWord=new CWordRule;
									m_pWord=static_cast<CWordRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));
									MSXML2::IXMLDOMElementPtr pPraseNode;
									pPraseNode=pDocKnowledge->createElement("Prase");
									if(pPraseNode!=NULL)
									{		
										CString phrase=m_pWord->getPhrase();
										_bstr_t phrase_bstr=phrase.AllocSysString();
										pPraseNode->text=phrase_bstr;
										pRuleNode->appendChild(pPraseNode);
									}
									pPraseNode.Release();


									MSXML2::IXMLDOMElementPtr pEnvAttriNode;
									pEnvAttriNode=pDocKnowledge->createElement("EnvAttri");
									if(pEnvAttriNode!=NULL)
									{
										CString EnvAttri=m_pWord->getEnvAttri();
										_bstr_t EnvAttri_bstr=EnvAttri.AllocSysString();
										pEnvAttriNode->text=EnvAttri_bstr;
										pRuleNode->appendChild(pEnvAttriNode);
									}
									pEnvAttriNode.Release();

									MSXML2::IXMLDOMElementPtr pWordLibNode;
									pWordLibNode=pDocKnowledge->createElement("WordLib");
									if(pWordLibNode!=NULL)
									{
										CString libfile=extractFileName(m_pWord->getLibFileName());
										_bstr_t libfile_bstr=libfile.AllocSysString();
										pWordLibNode->text=libfile_bstr;
										pRuleNode->appendChild(pWordLibNode);
									}
									pWordLibNode.Release();
								}



								//Point
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 3)
								{
									CPointRule *m_pPoint=new CPointRule;
									m_pPoint=static_cast<CPointRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));
									MSXML2::IXMLDOMElementPtr pXYNode;
									pXYNode=pDocKnowledge->createElement("XY");
									if(pXYNode!=NULL)
									{
										CString str;
										str.Format("%f %f",m_pPoint->getCentralX(),m_pPoint->getCentralY());
										pXYNode->text= (_bstr_t)str;
										pRuleNode->appendChild(pXYNode);
									}
									pXYNode.Release();

									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDocKnowledge->createElement("CurveType");
									if(pCurveTypeNode!=NULL)
									{
										int curvT=m_pPoint->getCurveType();
										switch(curvT)
										{
										case 0:
											pCurveTypeNode->text="Bell-shape";
											break;
										case 1:
											pCurveTypeNode->text="S-shape";
											break;
										case 2:
											pCurveTypeNode->text="Z-shape";
											break;
										}
										pRuleNode->appendChild(pCurveTypeNode);
									}
									pCurveTypeNode.Release();


									MSXML2::IXMLDOMElementPtr pLWidthNode;
									pLWidthNode=pDocKnowledge->createElement("LWidth");
									//char *Width = new char[];
									if(pLWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getLWidth());
										pLWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pLWidthNode);
									}
									pLWidthNode.Release();

									MSXML2::IXMLDOMElementPtr pRWidthNode;
									pRWidthNode=pDocKnowledge->createElement("RWidth");
									//char *Width = new char[];
									if(pRWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getRWidth());
										pRWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pRWidthNode);
									}
									pRWidthNode.Release();
									//delete Width;
								}

								//Enumerated
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 4)
								{
									CEnumRule *m_pEnum=new CEnumRule;
									m_pEnum=static_cast<CEnumRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));
									MSXML2::IXMLDOMElementPtr pItemNum,pEnumValueNode;

									pItemNum=pDocKnowledge->createElement("ItemNum");
									CString str;
									str.Format("%d",m_pEnum->getItemNum());								
									pItemNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pItemNum);
									pItemNum.Release();

									pEnumValueNode=pDocKnowledge->createElement("EnumValue");
									if(pEnumValueNode!=NULL)
									{

										int i3=0;
										CString str = "";
										for(int j3=0;j3<m_pEnum->getItemNum();j3++)
										{
											CString TempStr;
											TempStr.Format("%d",m_pEnum->getItem(j3));
											if(j3!=m_pEnum->getItemNum()-1)
											{
												str+=TempStr;
												str+=",";
											}
											else
												str+=TempStr;										

										}
										pEnumValueNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pEnumValueNode);
									}
									pEnumValueNode.Release();
								}
							}
							pRuleNode.Release();
						}
					}

					pTextNode = pDocKnowledge->createTextNode(L"\n    "); 
					pExclusion->appendChild(pTextNode);
					pTextNode.Release();
					pExclusion.Release();
				}
			}
			pTextNode = pDocKnowledge->createTextNode(L"\n   "); 
			pExclusionsNode->appendChild(pTextNode);
			pTextNode.Release();
			pExclusionsNode.Release();
		}

		pTextNode = pDocKnowledge->createTextNode(L"\n  "); 
		pSoilNameNode->appendChild(pTextNode);
		pTextNode.Release();

		pSoilNameNode.Release();
	}

	pTextNode = pDocKnowledge->createTextNode(L"\n "); 
	pRootKnowledgebaseElem->appendChild(pTextNode);
	pTextNode.Release();

	pRootKnowledgebaseElem.Release();



	//*************** Save the xml document **********************
	pTextNode = pDocKnowledge->createTextNode(L"\n "); 
	//pDocKnowledge->appendChild(pTextNode);
	pTextNode.Release();




	//pRootElem.Release();

	if(this->m_pProject->m_strKnowledgeBaseFile == "")
		this->m_pProject->m_strKnowledgeBaseFile= "KnowledgeBase.xml";






	_variant_t varxmlKnowledge(extractPathName(strFileName) + "\\" + m_pProject->m_strKnowledgeBaseFile);
	hrKnowledge = pDocKnowledge->save(varxmlKnowledge);
	if(FAILED(hrKnowledge))
	{
		AfxMessageBox("Error in saving project!");
		return;
	}
	pDocKnowledge.Release();
	CoUninitialize();






	//************* Create KnowledgeBase Node****************









	MSXML2::IXMLDOMElementPtr pKnowledgeBaseNode;
	pKnowledgeBaseNode=pDoc->createElement("KnowledgeBase");
	if(pKnowledgeBaseNode!=NULL)
	{  
		pTextNode = pDoc->createTextNode(L"\n "); 
		pRootElem->appendChild(pTextNode);
		pTextNode.Release();
		pRootElem->appendChild(pKnowledgeBaseNode);
	}

	MSXML2::IXMLDOMElementPtr pKnowledgeBaseFileNode;

	pKnowledgeBaseFileNode=pDoc->createElement("KnowledgeBaseFile");
	if(pKnowledgeBaseFileNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pKnowledgeBaseFileNode->appendChild(pTextNode);
		pTextNode.Release();

		CString knowledgeBaseFile = this->m_pProject->m_strKnowledgeBaseFile;
		_bstr_t knowledgeBaseFile_bstr=knowledgeBaseFile.AllocSysString();
		pKnowledgeBaseFileNode->text=knowledgeBaseFile_bstr;

		pKnowledgeBaseNode->appendChild(pKnowledgeBaseFileNode);

		//pCasebaseFileNode->text= pProjectCase->m_strCasebasePath;

	}

	pTextNode = pDoc->createTextNode(L"\n "); 
	pKnowledgeBaseNode->appendChild(pTextNode);
	pTextNode.Release();

	pKnowledgeBaseNode.Release();

	pKnowledgeBaseFileNode.Release();



	//************** Create Inference¡¡************************
	MSXML2::IXMLDOMElementPtr pInferenceNode;
	pInferenceNode=pDoc->createElement("Inference");
	if(pInferenceNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n "); 
		pRootElem->appendChild(pTextNode);
		pTextNode.Release();

		pRootElem->appendChild(pInferenceNode);
	}
	MSXML2::IXMLDOMElementPtr pUseMaskNode,pInferMaskNode,pMaskValueNode,pInferDirNode,pInferCreateCheckNode;
	pUseMaskNode=pDoc->createElement("UseMask");
	if(pUseMaskNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		if(pProject->m_pInference->getUseMask()==true)
			pUseMaskNode->text="true";
		else
			pUseMaskNode->text="false";
		pInferenceNode->appendChild(pUseMaskNode);
	}
	pInferMaskNode=pDoc->createElement("Mask");
	if(pInferMaskNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();

		pInferMaskNode->text=(_bstr_t)(pProject->m_pInference->getMaskFile());
		pInferenceNode->appendChild(pInferMaskNode);
	}
	pMaskValueNode=pDoc->createElement("MaskValue");
	if(pMaskValueNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		CString str;
		str.Format("%f",pProject->m_pInference->getMaskValue());
		pMaskValueNode->text=(_bstr_t)str;
		pInferenceNode->appendChild(pMaskValueNode);
	}
	pInferDirNode=pDoc->createElement("Directory");
	if(pInferDirNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		CString resultdir=pProject->m_pInference->getResultDir();
		_bstr_t resultdir_bstr=resultdir.AllocSysString();
		pInferDirNode->text=resultdir_bstr;
		pInferenceNode->appendChild(pInferDirNode);
	}

	pInferCreateCheckNode=pDoc->createElement("CreateCheckFile");
	if(pInferCreateCheckNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		if(pProject->m_pInference->getCreateCheckFile())
			pInferCreateCheckNode->text="true";
		else
			pInferCreateCheckNode->text="false";
		pInferenceNode->appendChild(pInferCreateCheckNode);
	}

	pTextNode = pDoc->createTextNode(L"\n  "); 
	pInferenceNode->appendChild(pTextNode);
	pTextNode.Release();

	pUseMaskNode.Release();
	pInferMaskNode.Release();
	pMaskValueNode.Release();
	pInferDirNode.Release();
	pInferCreateCheckNode.Release();
	pInferenceNode.Release();


	//*************** Save the xml document **********************
	pTextNode = pDoc->createTextNode(L"\n "); 
	pRootElem->appendChild(pTextNode);
	pTextNode.Release();


	pRootElem.Release();
	_variant_t varxmlProject(strFileName);
	hr=pDoc->save(varxmlProject);
	if(FAILED(hr))
	{
		AfxMessageBox("Error in saving project!");
		return;
	}
	pDoc.Release();
	CoUninitialize();
}




void CSoLIMDoc::SaveKnowledgeBase(CString strFileName, CProject *pProject)
{


	MSXML2::IXMLDOMDocumentPtr pDocKnowledge;
	HRESULT hrKnowledge;


	MSXML2::IXMLDOMAttributePtr attrib;


	MSXML2::IXMLDOMTextPtr   pTextNode; 

	_variant_t vNullVal;
	vNullVal.vt = VT_NULL;

	hrKnowledge=CoInitialize(NULL);
	if(FAILED(hrKnowledge))
	{
		AfxMessageBox("Error in saving project!");
		return;
	}





	hrKnowledge=CoInitialize(NULL);
	if(FAILED(hrKnowledge))
	{
		AfxMessageBox("Error in saving project!");
		return;
	}

	hrKnowledge=pDocKnowledge.CreateInstance(__uuidof(MSXML2::DOMDocument));
	if(FAILED(hrKnowledge))
	{
		AfxMessageBox("Error in saving project!");
		return;
	}
	pDocKnowledge->preserveWhiteSpace=TRUE;

	MSXML2::IXMLDOMProcessingInstructionPtr pPIKnowledge = NULL;
	pPIKnowledge = pDocKnowledge->createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");

	pDocKnowledge->insertBefore(pPIKnowledge, vNullVal);
	pPIKnowledge.Release();




	MSXML2::IXMLDOMElementPtr pRootKnowledgebaseElem;
	pRootKnowledgebaseElem=pDocKnowledge->createElement("KnowledgeBase");
	if(pRootKnowledgebaseElem!=NULL)
	{  
		//pTextNode = pDocKnowledge->createTextNode(L"\n "); 
		//pRootKnowledgebaseElem->appendChild(pTextNode);
		//pTextNode.Release();
		pDocKnowledge->appendChild(pRootKnowledgebaseElem);
	}

	MSXML2::IXMLDOMElementPtr pSoilNameNode;
	for (int n2=0;n2<pProject->m_pKnowledgeBase->getSoilTypeNum();n2++)
	{
		pSoilNameNode=pDocKnowledge->createElement("SoilType");
		if(pSoilNameNode!=NULL)
		{
			pTextNode = pDocKnowledge->createTextNode(L"\n  "); 
			pRootKnowledgebaseElem->appendChild(pTextNode);
			pTextNode.Release();

			attrib=pDocKnowledge->createAttribute("Name");
			if(attrib!=NULL)
			{
				CString soiltype=pProject->m_pKnowledgeBase->getSoilType(n2)->getSoilTypeName();
				_variant_t soiltype_var((LPCTSTR)soiltype);
				attrib->value=soiltype_var;
				pSoilNameNode->setAttributeNode(attrib);
				attrib.Release();
			}
			pRootKnowledgebaseElem->appendChild(pSoilNameNode);

			//instances
			MSXML2::IXMLDOMElementPtr pInstancesNode;
			pInstancesNode=pDocKnowledge->createElement("Instances");
			if(pInstancesNode!=NULL)
			{
				pTextNode = pDocKnowledge->createTextNode(L"\n   "); 
				pSoilNameNode->appendChild(pTextNode);
				pTextNode.Release();

				pSoilNameNode->appendChild(pInstancesNode);

				MSXML2::IXMLDOMElementPtr pInstance;
				for(int n3=0;n3<pProject->m_pKnowledgeBase->getSoilType(n2)->getInstanceNum();n3++)
				{
					pInstance=pDocKnowledge->createElement("Instance");
					if(pInstance!=NULL)
					{
						pTextNode = pDocKnowledge->createTextNode(L"\n    "); 
						pInstancesNode->appendChild(pTextNode);
						pTextNode.Release();

						attrib=pDocKnowledge->createAttribute("Name");
						if(attrib!=NULL)
						{
							CString instancename=pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getInstanceName();
							_variant_t instancename_var((LPCTSTR)instancename);
							attrib->value=instancename_var;
							pInstance->setAttributeNode(attrib);

						}
						attrib.Release();
						pInstancesNode->appendChild(pInstance);

						MSXML2::IXMLDOMElementPtr pRuleNode;
						for(int n4=0;n4<pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRuleNum();n4++)
						{
							pRuleNode=pDocKnowledge->createElement("Rule");
							if(pRuleNode!=NULL)
							{
								pTextNode = pDocKnowledge->createTextNode(L"\n     "); 
								pInstance->appendChild(pTextNode);
								pTextNode.Release();

								attrib=pDocKnowledge->createAttribute("Name");
								if(attrib!=NULL)
								{
									CString rulename=pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleName();
									_variant_t rulename_var((LPCTSTR)rulename);
									attrib->value=rulename_var;
									pRuleNode->setAttributeNode(attrib);								
								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("UseOrNot");
								if(attrib!=NULL)
								{
									if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getIsUsed()==true)
										attrib->value="true";
									else 
										attrib->value="false";

									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("Type");
								if(attrib!=NULL)
								{
									int a=pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType();
									switch(a)
									{
									case 0:
										attrib->value="Range";
										break;
									case 1:
										attrib->value="Curve";
										break;
									case 2:
										attrib->value="Word";
										break;
									case 3:
										attrib->value="Point";									
										break;
									case 4:
										attrib->value="Enumerated";										
										break;

									}
									pRuleNode->setAttributeNode(attrib);


								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("DataLayer");
								if(attrib!=NULL)
								{
									CString datalayername=pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getLayerName();
									_variant_t datalayername_var((LPCSTR)datalayername);
									attrib->value=datalayername_var;
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();
								pInstance->appendChild(pRuleNode);



								//Range
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 0)
								{
									CRangeRule *m_pRange;;
									m_pRange=(CRangeRule *)(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));

									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDocKnowledge->createElement("CurveType");
									if(pCurveTypeNode!=NULL)
									{
										int curvT=m_pRange->getCurveType();
										switch(curvT)
										{
										case 0:
											pCurveTypeNode->text="Bell-shape";
											break;
										case 1:
											pCurveTypeNode->text="S-shape";
											break;
										case 2:
											pCurveTypeNode->text="Z-shape";
											break;
										}
										pRuleNode->appendChild(pCurveTypeNode);
									}
									pCurveTypeNode.Release();
									MSXML2::IXMLDOMElementPtr pKeyValueNode;
									pKeyValueNode=pDocKnowledge->createElement("KeyValue");
									if(pKeyValueNode!=NULL)
									{
										double KeyValue[4];
										KeyValue[0]=m_pRange->getLowCross();
										KeyValue[1]=m_pRange->getLowUnity();
										KeyValue[2]=m_pRange->getHighUnity();
										KeyValue[3]=m_pRange->getHighCross();
										//char *s=new char[];
										CString str;
										int i2=0;
										for(int j2=0;j2<4;j2++)
										{
											CString tempStr;
											tempStr.Format("%f",KeyValue[j2]);
											if(j2!=3)
												str = str + tempStr+",";
											else 
												str = str + tempStr;

										}
										pKeyValueNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pKeyValueNode);
										//delete []s;
									}
									pKeyValueNode.Release();
								}

								//Curve
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 1)
								{
									CFreehandRule *m_pCurve; 
									m_pCurve= (CFreehandRule *)(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));
									MSXML2::IXMLDOMElementPtr pNodeNum,pCoordinatesNode;

									pNodeNum=pDocKnowledge->createElement("NodeNum");
									CString str;
									str.Format("%d",m_pCurve->getKnotNum());								
									pNodeNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pNodeNum);
									pNodeNum.Release();

									pCoordinatesNode=pDocKnowledge->createElement("Coordinates");
									if(pCoordinatesNode!=NULL)
									{
										int i1=0;
										CString str;
										for(int j=0;j<m_pCurve->getKnotNum();j++)
										{
											double x, y;
											CString tempString;
											m_pCurve->getKnot(j,&x,&y);
											tempString.Format("%f %f",x,y);
											if(j!=m_pCurve->getKnotNum()-1)
												str = str + tempString + ",";
											else 
												str = str + tempString; 
										}
										pCoordinatesNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pCoordinatesNode);
									}
									pCoordinatesNode.Release();
								}

								//Word
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 2)
								{
									CWordRule *m_pWord=new CWordRule;
									m_pWord=static_cast<CWordRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));
									MSXML2::IXMLDOMElementPtr pPraseNode;
									pPraseNode=pDocKnowledge->createElement("Prase");
									if(pPraseNode!=NULL)
									{
										CString phrase=m_pWord->getPhrase();
										_bstr_t phrase_bstr=phrase.AllocSysString();
										pPraseNode->text=phrase_bstr;
										pRuleNode->appendChild(pPraseNode);
									}
									pPraseNode.Release();

									MSXML2::IXMLDOMElementPtr pEnvAttriNode;
									pEnvAttriNode=pDocKnowledge->createElement("EnvAttri");
									if(pEnvAttriNode!=NULL)
									{
										CString EnvAttri=m_pWord->getEnvAttri();
										_bstr_t EnvAttri_bstr=EnvAttri.AllocSysString();
										pEnvAttriNode->text=EnvAttri_bstr;
										pRuleNode->appendChild(pEnvAttriNode);
									}
									pEnvAttriNode.Release();

									MSXML2::IXMLDOMElementPtr pWordLibNode;
									pWordLibNode=pDocKnowledge->createElement("WordLib");
									if(pWordLibNode!=NULL)
									{
										CString libfile=extractFileName(m_pWord->getLibFileName());
										_bstr_t libfile_bstr=libfile.AllocSysString();
										pWordLibNode->text=libfile_bstr;
										pRuleNode->appendChild(pWordLibNode);
									}
									pWordLibNode.Release();
								}


								//Point
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 3)
								{
									CPointRule *m_pPoint=new CPointRule;
									m_pPoint=static_cast<CPointRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));
									MSXML2::IXMLDOMElementPtr pXYNode;
									pXYNode=pDocKnowledge->createElement("XY");
									if(pXYNode!=NULL)
									{
										CString str;
										str.Format("%f %f",m_pPoint->getCentralX(),m_pPoint->getCentralY());
										pXYNode->text= (_bstr_t)str;
										pRuleNode->appendChild(pXYNode);
									}
									pXYNode.Release();
									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDocKnowledge->createElement("CurveType");
									if(pCurveTypeNode!=NULL)
									{
										int curvT=m_pPoint->getCurveType();
										switch(curvT)
										{
										case 0:
											pCurveTypeNode->text="Bell-shape";
											break;
										case 1:
											pCurveTypeNode->text="S-shape";
											break;
										case 2:
											pCurveTypeNode->text="Z-shape";
											break;
										}
										pRuleNode->appendChild(pCurveTypeNode);
									}
									pCurveTypeNode.Release();

									MSXML2::IXMLDOMElementPtr pLWidthNode;
									pLWidthNode=pDocKnowledge->createElement("LWidth");

									if(pLWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getLWidth());
										pLWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pLWidthNode);
									}
									pLWidthNode.Release();

									MSXML2::IXMLDOMElementPtr pRWidthNode;
									pRWidthNode=pDocKnowledge->createElement("RWidth");

									if(pRWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getRWidth());
										pRWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pRWidthNode);
									}
									pRWidthNode.Release();								
								}     

								//Enumerated
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 4)
								{
									CEnumRule *m_pEnum=new CEnumRule;
									m_pEnum=static_cast<CEnumRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));
									MSXML2::IXMLDOMElementPtr pItemNum,pEnumValueNode;

									pItemNum=pDocKnowledge->createElement("ItemNum");
									CString str;
									str.Format("%d",m_pEnum->getItemNum());								
									pItemNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pItemNum);
									pItemNum.Release();

									pEnumValueNode=pDocKnowledge->createElement("EnumValue");
									if(pEnumValueNode!=NULL)
									{
										int i3=0;
										CString str = "";
										for(int j3=0;j3<m_pEnum->getItemNum();j3++)
										{
											CString TempStr;
											TempStr.Format("%d",m_pEnum->getItem(j3));
											if(j3!=m_pEnum->getItemNum()-1)
											{
												str+=TempStr;
												str+=",";
											}
											else
												str+=TempStr;

										}
										pEnumValueNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pEnumValueNode);
									}
									pEnumValueNode.Release();
								}

							}
							pRuleNode.Release();
						}
					}
					pTextNode = pDocKnowledge->createTextNode(L"\n    "); 
					pInstance->appendChild(pTextNode);
					pTextNode.Release();
					pInstance.Release();
				}
			}
			pTextNode = pDocKnowledge->createTextNode(L"\n   "); 
			pInstancesNode->appendChild(pTextNode);
			pTextNode.Release();

			pInstancesNode.Release();


			//Occurrences
			MSXML2::IXMLDOMElementPtr pOccurrencesNode;
			pOccurrencesNode=pDocKnowledge->createElement("Occurrences");
			if(pOccurrencesNode!=NULL)
			{

				pTextNode = pDocKnowledge->createTextNode(L"\n   "); 
				pSoilNameNode->appendChild(pTextNode);
				pTextNode.Release();

				pSoilNameNode->appendChild(pOccurrencesNode);

				MSXML2::IXMLDOMElementPtr pOccurrence;
				for(int n5=0;n5<pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrenceNum();n5++)
				{
					pOccurrence=pDocKnowledge->createElement("Occurrence");
					if(pOccurrence!=NULL)
					{
						pTextNode = pDocKnowledge->createTextNode(L"\n    "); 
						pOccurrencesNode->appendChild(pTextNode);
						pTextNode.Release();

						attrib=pDocKnowledge->createAttribute("Name");
						if(attrib!=NULL)
						{
							CString occurname=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getOccurrenceName();
							_variant_t occurname_var((LPCSTR)occurname);
							attrib->value=occurname_var;
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("x");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getCentralX();
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("y");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getCentralY();
							pOccurrence->setAttributeNode(attrib);

						}	
						attrib.Release();						
						attrib=pDocKnowledge->createAttribute("SearchDist");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getSearchDist();
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("UseDistanceDecay");
						if(attrib!=NULL)
						{
							if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getUseDistSim() == true)
								attrib->value= "true";
							else
								attrib->value = "false";
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("DistanceDecay");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getDistDecayFactor();
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						pOccurrencesNode->appendChild(pOccurrence);

						MSXML2::IXMLDOMElementPtr pRuleNode;
						for(int n6=0;n6<pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRuleNum();n6++)
						{
							pRuleNode=pDocKnowledge->createElement("Rule");
							if(pRuleNode!=NULL)
							{
								pTextNode = pDocKnowledge->createTextNode(L"\n     "); 
								pOccurrence->appendChild(pTextNode);
								pTextNode.Release();

								attrib=pDocKnowledge->createAttribute("Name");
								if(attrib!=NULL)
								{
									CString rulename=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleName();
									_variant_t rulename_var((LPCSTR)rulename);
									attrib->value=rulename_var;
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("UseOrNot");
								if(attrib!=NULL)
								{
									if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getIsUsed()==true)
										attrib->value="true";
									else 
										attrib->value="false";

									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("Type");
								if(attrib!=NULL)
								{
									int a=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType();
									switch(a)
									{
									case 0:
										attrib->value="Range";
										break;
									case 1:
										attrib->value="Curve";
										break;
									case 2:
										attrib->value="Word";
										break;
									case 3:
										attrib->value="Point";									
										break;
									case 4:
										attrib->value="Enumerated";										
										break;

									}
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("DataLayer");
								if(attrib!=NULL)
								{
									CString layername=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getLayerName();
									_variant_t layername_var((LPCSTR)layername);
									attrib->value=layername_var;
									pRuleNode->setAttributeNode(attrib);

								}	
								attrib.Release();
								pOccurrence->appendChild(pRuleNode);


								//Range
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()==0)
								{
									CRangeRule *m_pRange = new CRangeRule;
									m_pRange=static_cast<CRangeRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));

									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDocKnowledge->createElement("CurveType");
									if(pCurveTypeNode!=NULL)
									{
										int curvT=m_pRange->getCurveType();
										switch(curvT)
										{
										case 0:
											pCurveTypeNode->text="Bell-shape";
											break;
										case 1:
											pCurveTypeNode->text="S-shape";
											break;
										case 2:
											pCurveTypeNode->text="Z-shape";
											break;
										}
										pRuleNode->appendChild(pCurveTypeNode);
									}
									pCurveTypeNode.Release();

									MSXML2::IXMLDOMElementPtr pKeyValueNode;
									pKeyValueNode=pDocKnowledge->createElement("KeyValue");
									if(pKeyValueNode!=NULL)
									{
										double KeyValue[4];
										KeyValue[0]=m_pRange->getLowCross();
										KeyValue[1]=m_pRange->getLowUnity();
										KeyValue[2]=m_pRange->getHighUnity();
										KeyValue[3]=m_pRange->getHighCross();
										//char *s=new char[];
										CString str;
										int i2=0;
										for(int j2=0;j2<4;j2++)
										{
											CString tempStr;
											tempStr.Format("%f",KeyValue[j2]);
											if(j2!=3)
												str = str + tempStr+",";
											else 
												str = str + tempStr;

										}
										pKeyValueNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pKeyValueNode);
										//delete []s;
									}
									pKeyValueNode.Release();
								}
								//Curve
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()== 1)
								{
									CFreehandRule *m_pCurve = new CFreehandRule;
									m_pCurve= static_cast<CFreehandRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));
									MSXML2::IXMLDOMElementPtr pNodeNum,pCoordinatesNode;
									pNodeNum=pDocKnowledge->createElement("NodeNum");
									CString str;
									str.Format("%d",m_pCurve->getKnotNum());								
									pNodeNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pNodeNum);
									pNodeNum.Release();

									pCoordinatesNode=pDocKnowledge->createElement("Coordinates");
									if(pCoordinatesNode!=NULL)
									{
										int i1=0;
										CString str;
										for(int j=0;j<m_pCurve->getKnotNum();j++)
										{
											double x, y;
											CString tempString;
											m_pCurve->getKnot(j,&x,&y);
											tempString.Format("%f %f",x,y);
											if(j!=m_pCurve->getKnotNum()-1)
												str = str + tempString + ",";
											else 
												str = str + tempString; 							
										}
										pCoordinatesNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pCoordinatesNode);
									}
									pCoordinatesNode.Release();
								}


								//Word
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()== 2)
								{
									CWordRule *m_pWord=new CWordRule;
									m_pWord=static_cast<CWordRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));
									MSXML2::IXMLDOMElementPtr pPraseNode;
									pPraseNode=pDocKnowledge->createElement("Prase");
									if(pPraseNode!=NULL)
									{
										CString phrase=m_pWord->getPhrase();
										_bstr_t phrase_bstr=phrase.AllocSysString();
										pPraseNode->text=phrase_bstr;
										pRuleNode->appendChild(pPraseNode);
									}
									pPraseNode.Release();


									MSXML2::IXMLDOMElementPtr pEnvAttriNode;
									pEnvAttriNode=pDocKnowledge->createElement("EnvAttri");
									if(pEnvAttriNode!=NULL)
									{
										CString EnvAttri=m_pWord->getEnvAttri();
										_bstr_t EnvAttri_bstr=EnvAttri.AllocSysString();
										pEnvAttriNode->text=EnvAttri_bstr;
										pRuleNode->appendChild(pEnvAttriNode);
									}
									pEnvAttriNode.Release();

									MSXML2::IXMLDOMElementPtr pWordLibNode;
									pWordLibNode=pDocKnowledge->createElement("WordLib");
									if(pWordLibNode!=NULL)
									{
										CString libfile=extractFileName(m_pWord->getLibFileName());
										_bstr_t libfile_bstr=libfile.AllocSysString();
										pWordLibNode->text=libfile_bstr;
										pRuleNode->appendChild(pWordLibNode);
									}
									pWordLibNode.Release();
								}


								//Point
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()== 3)
								{
									CPointRule *m_pPoint=new CPointRule;
									m_pPoint=static_cast<CPointRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));
									MSXML2::IXMLDOMElementPtr pXYNode;
									pXYNode=pDocKnowledge->createElement("XY");
									if(pXYNode!=NULL)
									{
										//char *XYNode=new char[];
										CString str;
										str.Format("%f %f",m_pPoint->getCentralX(),m_pPoint->getCentralY());
										pXYNode->text= (_bstr_t)str;
										pRuleNode->appendChild(pXYNode);
									}
									pXYNode.Release();	

									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDocKnowledge->createElement("CurveType");
									if(pCurveTypeNode!=NULL)
									{
										int curvT=m_pPoint->getCurveType();
										switch(curvT)
										{
										case 0:
											pCurveTypeNode->text="Bell-shape";
											break;
										case 1:
											pCurveTypeNode->text="S-shape";
											break;
										case 2:
											pCurveTypeNode->text="Z-shape";
											break;
										}
										pRuleNode->appendChild(pCurveTypeNode);
									}
									pCurveTypeNode.Release();

									MSXML2::IXMLDOMElementPtr pLWidthNode;
									pLWidthNode=pDocKnowledge->createElement("LWidth");

									if(pLWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getLWidth());
										pLWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pLWidthNode);
									}
									pLWidthNode.Release();

									MSXML2::IXMLDOMElementPtr pRWidthNode;
									pRWidthNode=pDocKnowledge->createElement("RWidth");

									if(pRWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getRWidth());
										pRWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pRWidthNode);
									}
									pRWidthNode.Release();
									//delete Width;
								}


								//Enumerated
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()== 4)
								{
									CEnumRule *m_pEnum=new CEnumRule;
									m_pEnum=static_cast<CEnumRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));
									MSXML2::IXMLDOMElementPtr pItemNum,pEnumValueNode;

									pItemNum=pDocKnowledge->createElement("ItemNum");
									CString str;
									str.Format("%d",m_pEnum->getItemNum());								
									pItemNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pItemNum);
									pItemNum.Release();
									pEnumValueNode=pDocKnowledge->createElement("EnumValue");
									if(pEnumValueNode!=NULL)
									{
										//char *EnumV=new char[];
										int i3=0;
										CString str = "";
										for(int j3=0;j3<m_pEnum->getItemNum();j3++)
										{
											CString TempStr;
											TempStr.Format("%d",m_pEnum->getItem(j3));
											if(j3!=m_pEnum->getItemNum()-1)
											{
												str+=TempStr;
												str+=",";
											}
											else
												str+=TempStr;

										}
										pEnumValueNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pEnumValueNode);
									}pEnumValueNode.Release();
								}

							}
							pRuleNode.Release();
						}
					}
					pTextNode = pDocKnowledge->createTextNode(L"\n    "); 
					pOccurrence->appendChild(pTextNode);
					pTextNode.Release();
					pOccurrence.Release();
				}
			}



			pTextNode = pDocKnowledge->createTextNode(L"\n   "); 
			pOccurrencesNode->appendChild(pTextNode);
			pTextNode.Release();

			pOccurrencesNode.Release();
			//Exclusions
			MSXML2::IXMLDOMElementPtr pExclusionsNode;
			pExclusionsNode=pDocKnowledge->createElement("Exclusions");
			if(pExclusionsNode!=NULL)
			{
				pTextNode = pDocKnowledge->createTextNode(L"\n   "); 
				pSoilNameNode->appendChild(pTextNode);
				pTextNode.Release();
				pSoilNameNode->appendChild(pExclusionsNode);

				MSXML2::IXMLDOMElementPtr pExclusion;
				for(int n7=0;n7<pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusionNum();n7++)
				{
					pExclusion=pDocKnowledge->createElement("Exclusion");
					if(pExclusion!=NULL)
					{
						pTextNode = pDocKnowledge->createTextNode(L"\n    "); 
						pExclusionsNode->appendChild(pTextNode);
						pTextNode.Release();

						attrib=pDocKnowledge->createAttribute("Name");
						if(attrib!=NULL)
						{
							CString excluname=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getExclusionName();
							_variant_t excluname_var((LPCSTR)excluname);
							attrib->value=excluname_var;
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("x");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getCentralX();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("y");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getCentralY();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();

						attrib=pDocKnowledge->createAttribute("SearchDist");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getSearchDist();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("UseDistanceDecay");
						if(attrib!=NULL)
						{
							if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getUseDistSim() == true)
								attrib->value = "true";
							else 
								attrib->value = "false";

							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("DistanceDecay");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getDistDecayFactor();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						pExclusionsNode->appendChild(pExclusion);

						MSXML2::IXMLDOMElementPtr pRuleNode;
						for(int n8=0;n8<pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRuleNum();n8++)
						{
							pRuleNode=pDocKnowledge->createElement("Rule");
							if(pRuleNode!=NULL)
							{
								pTextNode = pDocKnowledge->createTextNode(L"\n     "); 
								pExclusion->appendChild(pTextNode);
								pTextNode.Release();

								attrib=pDocKnowledge->createAttribute("Name");
								if(attrib!=NULL)
								{
									CString rulename=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleName();
									_variant_t rulename_var((LPCSTR)rulename);
									attrib->value=rulename_var;
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("UseOrNot");
								if(attrib!=NULL)
								{
									if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getIsUsed()==true)
										attrib->value="true";
									else 
										attrib->value="false";

									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("Type");
								if(attrib!=NULL)
								{
									int a=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType();
									switch(a)
									{
									case 0:
										attrib->value="Range";
										break;
									case 1:
										attrib->value="Curve";
										break;
									case 2:
										attrib->value="Word";
										break;
									case 3:
										attrib->value="Point";									
										break;
									case 4:
										attrib->value="Enumerated";										
										break;

									}
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("DataLayer");
								if(attrib!=NULL)
								{
									CString datalayername=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getLayerName();
									_variant_t datalayername_var((LPCSTR)datalayername);
									attrib->value=datalayername_var;
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();
								pExclusion->appendChild(pRuleNode);


								//Range
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 0)
								{
									CRangeRule *m_pRange = new CRangeRule;
									m_pRange=static_cast<CRangeRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));

									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDocKnowledge->createElement("CurveType");
									if(pCurveTypeNode!=NULL)
									{
										int curvT=m_pRange->getCurveType();
										switch(curvT)
										{
										case 0:
											pCurveTypeNode->text="Bell-shape";
											break;
										case 1:
											pCurveTypeNode->text="S-shape";
											break;
										case 2:
											pCurveTypeNode->text="Z-shape";
											break;

										}
										pRuleNode->appendChild(pCurveTypeNode);
									}
									pCurveTypeNode.Release();

									MSXML2::IXMLDOMElementPtr pKeyValueNode;
									pKeyValueNode=pDocKnowledge->createElement("KeyValue");
									if(pKeyValueNode!=NULL)
									{
										double KeyValue[4];
										KeyValue[0]=m_pRange->getLowCross();
										KeyValue[1]=m_pRange->getLowUnity();
										KeyValue[2]=m_pRange->getHighUnity();
										KeyValue[3]=m_pRange->getHighCross();
										//char *s=new char[];
										CString str;
										int i2=0;
										for(int j2=0;j2<4;j2++)
										{
											CString tempStr;
											tempStr.Format("%f",KeyValue[j2]);
											if(j2!=3)
												str = str + tempStr+",";
											else 
												str = str + tempStr;

										}
										pKeyValueNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pKeyValueNode);
										//delete []s;
									}
									pKeyValueNode.Release();
								}

								//Curve
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 1)
								{
									CFreehandRule *m_pCurve = new CFreehandRule;
									m_pCurve= static_cast<CFreehandRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));

									MSXML2::IXMLDOMElementPtr pNodeNum,pCoordinatesNode;
									pNodeNum=pDocKnowledge->createElement("NodeNum");
									CString str;
									str.Format("%d",m_pCurve->getKnotNum());								
									pNodeNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pNodeNum);
									pNodeNum.Release();

									pCoordinatesNode=pDocKnowledge->createElement("Coordinates");
									if(pCoordinatesNode!=NULL)
									{
										int i1=0;
										CString str;
										for(int j=0;j<m_pCurve->getKnotNum();j++)
										{
											double x, y;
											CString tempString;
											m_pCurve->getKnot(j,&x,&y);
											tempString.Format("%f %f",x,y);
											if(j!=m_pCurve->getKnotNum()-1)
												str = str + tempString + ",";
											else 
												str = str + tempString; 
											//i1+=sprintf(s+i1,"%f,%f ",&x,&y);
										}
										pCoordinatesNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pCoordinatesNode);
									}
									pCoordinatesNode.Release();
								}


								//Word
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 2)
								{
									CWordRule *m_pWord=new CWordRule;
									m_pWord=static_cast<CWordRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));
									MSXML2::IXMLDOMElementPtr pPraseNode;
									pPraseNode=pDocKnowledge->createElement("Prase");
									if(pPraseNode!=NULL)
									{		
										CString phrase=m_pWord->getPhrase();
										_bstr_t phrase_bstr=phrase.AllocSysString();
										pPraseNode->text=phrase_bstr;
										pRuleNode->appendChild(pPraseNode);
									}
									pPraseNode.Release();


									MSXML2::IXMLDOMElementPtr pEnvAttriNode;
									pEnvAttriNode=pDocKnowledge->createElement("EnvAttri");
									if(pEnvAttriNode!=NULL)
									{
										CString EnvAttri=m_pWord->getEnvAttri();
										_bstr_t EnvAttri_bstr=EnvAttri.AllocSysString();
										pEnvAttriNode->text=EnvAttri_bstr;
										pRuleNode->appendChild(pEnvAttriNode);
									}
									pEnvAttriNode.Release();

									MSXML2::IXMLDOMElementPtr pWordLibNode;
									pWordLibNode=pDocKnowledge->createElement("WordLib");
									if(pWordLibNode!=NULL)
									{
										CString libfile=extractFileName(m_pWord->getLibFileName());
										_bstr_t libfile_bstr=libfile.AllocSysString();
										pWordLibNode->text=libfile_bstr;
										pRuleNode->appendChild(pWordLibNode);
									}
									pWordLibNode.Release();
								}



								//Point
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 3)
								{
									CPointRule *m_pPoint=new CPointRule;
									m_pPoint=static_cast<CPointRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));
									MSXML2::IXMLDOMElementPtr pXYNode;
									pXYNode=pDocKnowledge->createElement("XY");
									if(pXYNode!=NULL)
									{
										CString str;
										str.Format("%f %f",m_pPoint->getCentralX(),m_pPoint->getCentralY());
										pXYNode->text= (_bstr_t)str;
										pRuleNode->appendChild(pXYNode);
									}
									pXYNode.Release();

									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDocKnowledge->createElement("CurveType");
									if(pCurveTypeNode!=NULL)
									{
										int curvT=m_pPoint->getCurveType();
										switch(curvT)
										{
										case 0:
											pCurveTypeNode->text="Bell-shape";
											break;
										case 1:
											pCurveTypeNode->text="S-shape";
											break;
										case 2:
											pCurveTypeNode->text="Z-shape";
											break;
										}
										pRuleNode->appendChild(pCurveTypeNode);
									}
									pCurveTypeNode.Release();


									MSXML2::IXMLDOMElementPtr pLWidthNode;
									pLWidthNode=pDocKnowledge->createElement("LWidth");
									//char *Width = new char[];
									if(pLWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getLWidth());
										pLWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pLWidthNode);
									}
									pLWidthNode.Release();

									MSXML2::IXMLDOMElementPtr pRWidthNode;
									pRWidthNode=pDocKnowledge->createElement("RWidth");
									//char *Width = new char[];
									if(pRWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getRWidth());
										pRWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pRWidthNode);
									}
									pRWidthNode.Release();
									//delete Width;
								}

								//Enumerated
								if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 4)
								{
									CEnumRule *m_pEnum=new CEnumRule;
									m_pEnum=static_cast<CEnumRule *>(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));
									MSXML2::IXMLDOMElementPtr pItemNum,pEnumValueNode;

									pItemNum=pDocKnowledge->createElement("ItemNum");
									CString str;
									str.Format("%d",m_pEnum->getItemNum());								
									pItemNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pItemNum);
									pItemNum.Release();

									pEnumValueNode=pDocKnowledge->createElement("EnumValue");
									if(pEnumValueNode!=NULL)
									{

										int i3=0;
										CString str = "";
										for(int j3=0;j3<m_pEnum->getItemNum();j3++)
										{
											CString TempStr;
											TempStr.Format("%d",m_pEnum->getItem(j3));
											if(j3!=m_pEnum->getItemNum()-1)
											{
												str+=TempStr;
												str+=",";
											}
											else
												str+=TempStr;										

										}
										pEnumValueNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pEnumValueNode);
									}
									pEnumValueNode.Release();
								}
							}
							pRuleNode.Release();
						}
					}

					pTextNode = pDocKnowledge->createTextNode(L"\n    "); 
					pExclusion->appendChild(pTextNode);
					pTextNode.Release();
					pExclusion.Release();
				}
			}
			pTextNode = pDocKnowledge->createTextNode(L"\n   "); 
			pExclusionsNode->appendChild(pTextNode);
			pTextNode.Release();
			pExclusionsNode.Release();
		}

		pTextNode = pDocKnowledge->createTextNode(L"\n  "); 
		pSoilNameNode->appendChild(pTextNode);
		pTextNode.Release();

		pSoilNameNode.Release();
	}

	pTextNode = pDocKnowledge->createTextNode(L"\n "); 
	pRootKnowledgebaseElem->appendChild(pTextNode);
	pTextNode.Release();

	pRootKnowledgebaseElem.Release();



	//*************** Save the xml document **********************
	pTextNode = pDocKnowledge->createTextNode(L"\n "); 
	//pDocKnowledge->appendChild(pTextNode);
	pTextNode.Release();




	//pRootElem.Release();

	//if(this->m_pProject->m_strKnowledgeBaseFile == "")
	//	this->m_pProject->m_strKnowledgeBaseFile= "KnowledgeBase.xml";






	_variant_t varxmlKnowledge(strFileName);
	hrKnowledge = pDocKnowledge->save(varxmlKnowledge);
	if(FAILED(hrKnowledge))
	{
		AfxMessageBox("Error in saving project!");
		return;
	}
	pDocKnowledge.Release();
	CoUninitialize();





}

void CSoLIMDoc::WriteProjectCase(CString strFileName, CProjectCase *pProjectCase)
{





	MSXML2::IXMLDOMDocumentPtr pDoc;
	HRESULT hr;

	hr=CoInitialize(NULL);
	if(FAILED(hr))
	{
		AfxMessageBox("Error in saving project!");
		return;
	}

	hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));
	if(FAILED(hr))
	{
		AfxMessageBox("Error in saving project!");
		return;
	}
	pDoc->preserveWhiteSpace=TRUE;

	MSXML2::IXMLDOMProcessingInstructionPtr pPI = NULL;
	pPI = pDoc->createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
	_variant_t vNullVal;
	vNullVal.vt = VT_NULL;
	pDoc->insertBefore(pPI, vNullVal);
	pPI.Release();


	//*********** create root node "SoLIMProject"***************************
	MSXML2::IXMLDOMElementPtr pRootElem;
	pRootElem=pDoc->createElement("SoLIMProject");
	//create the attributes of the root node
	MSXML2::IXMLDOMAttributePtr attrib;
	CString attri;
	attrib=pDoc->createAttribute("Version");
	if(attrib!=NULL)
	{
		attri="2010";
		_variant_t varattri(attri);
		attrib->value=varattri;
		pRootElem->setAttributeNode(attrib);

	}
	attrib.Release();

	attrib=pDoc->createAttribute("Name");
	if(attrib!=NULL)
	{
		attri=pProjectCase->getProjectName();
		_variant_t varattri(attri);
		attrib->value=varattri;
		pRootElem->setAttributeNode(attrib);

	}
	attrib.Release();
	attrib=pDoc->createAttribute("ProjectType");
	if(attrib!=NULL)
	{
		attri="SBI";
		_variant_t varattri(attri);
		attrib->value=varattri;
		pRootElem->setAttributeNode(attrib);

	}
	attrib.Release();
	attrib=pDoc->createAttribute("DataType");
	if(attrib!=NULL)
	{
		attrib->value="3dr";
		pRootElem->setAttributeNode(attrib);

	}
	attrib.Release();
	//add the root node to the DOM instance
	pDoc->appendChild(pRootElem);
	MSXML2::IXMLDOMTextPtr   pTextNode; 

	//**********  Create database node ***************
	MSXML2::IXMLDOMElementPtr pDatabaseNode;
	pDatabaseNode=pDoc->createElement("Database");
	if(pDatabaseNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n "); 
		pRootElem->appendChild(pTextNode);
		pTextNode.Release();
		pRootElem->appendChild(pDatabaseNode);
	}

	MSXML2::IXMLDOMElementPtr pLayernode;

	MSXML2::IXMLDOMElementPtr pLayerNameNode,pLayerPathNode, pLayerResolutionNode, pLayerSimiTypeNode, pLayerSimiMethodNode;

	MSXML2::IXMLDOMElementPtr pClimateNode;
	pClimateNode =pDoc->createElement("Climate");
	if(pDatabaseNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n "); 
		pDatabaseNode->appendChild(pTextNode);
		pTextNode.Release();
		pDatabaseNode->appendChild(pClimateNode);
	}
	//append layer node


	for(int n1=0;n1<pProjectCase->vecClimateLayerNames.size(); n1++)
	{

		pLayernode=pDoc->createElement("layer");
		if(pLayernode!=NULL)
		{
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pClimateNode->appendChild(pTextNode);
			pTextNode.Release();		
			pClimateNode->appendChild(pLayernode);
			pTextNode = pDoc->createTextNode(L"\n   "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();


			pLayerNameNode=pDoc->createElement("Name");
			CString layername=pProjectCase->vecClimateLayerNames[n1];
			BSTR layername_bstr=layername.AllocSysString();
			pLayerNameNode->text=layername_bstr;
			pLayernode->appendChild(pLayerNameNode);
			pTextNode = pDoc->createTextNode(L"\n   "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerPathNode=pDoc->createElement("File");
			CString filename= pProjectCase->vecClimateLayerFiles[n1];
			BSTR filename_bstr=filename.AllocSysString();
			pLayerPathNode->text=filename_bstr;
			pLayernode->appendChild(pLayerPathNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerResolutionNode=pDoc->createElement("Resolution");
			double resolution= pProjectCase->vecClimateLayerResolutions[n1];
			CString str;
			str.Format("%f", resolution);
			BSTR reso_bstr=str.AllocSysString();
			pLayerResolutionNode->text=reso_bstr;
			pLayernode->appendChild(pLayerResolutionNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerSimiTypeNode=pDoc->createElement("SimilarityType");
			CString simitype= pProjectCase->vecClimateLayerSimiTypes[n1];
			BSTR simitype_bstr=simitype.AllocSysString();
			pLayerSimiTypeNode->text=simitype_bstr;
			pLayernode->appendChild(pLayerSimiTypeNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerSimiMethodNode=pDoc->createElement("SimilarityMethod");
			CString simimethod= pProjectCase->vecClimateLayerSimiMethods[n1];
			BSTR simimethod_bstr=simimethod.AllocSysString();
			pLayerSimiMethodNode->text=simimethod_bstr;
			pLayernode->appendChild(pLayerSimiMethodNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

		}

		pLayerNameNode.Release();
		pLayerPathNode.Release();
		pLayerResolutionNode.Release();
		pLayerSimiTypeNode.Release();
		pLayerSimiMethodNode.Release();
		pLayernode.Release();

	}   

	MSXML2::IXMLDOMElementPtr pGeologyNode;
	pGeologyNode =pDoc->createElement("Geology");
	if(pDatabaseNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n "); 
		pDatabaseNode->appendChild(pTextNode);
		pTextNode.Release();
		pDatabaseNode->appendChild(pGeologyNode);
	}

	for(int n1=0;n1<pProjectCase->vecGeologyLayerNames.size(); n1++)
	{

		pLayernode=pDoc->createElement("layer");
		if(pLayernode!=NULL)
		{
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pGeologyNode->appendChild(pTextNode);
			pTextNode.Release();		
			pGeologyNode->appendChild(pLayernode);
			pTextNode = pDoc->createTextNode(L"\n   "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerNameNode=pDoc->createElement("Name");
			CString layername=pProjectCase->vecGeologyLayerNames[n1];
			BSTR layername_bstr=layername.AllocSysString();
			pLayerNameNode->text=layername_bstr;
			pLayernode->appendChild(pLayerNameNode);
			pTextNode = pDoc->createTextNode(L"\n   "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();
			pLayerPathNode=pDoc->createElement("File");
			CString filename=pProjectCase->vecGeologyLayerFiles[n1];
			BSTR filename_bstr=filename.AllocSysString();
			pLayerPathNode->text=filename_bstr;
			pLayernode->appendChild(pLayerPathNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerResolutionNode=pDoc->createElement("Resolution");
			double resolution= pProjectCase->vecGeologyLayerResolutions[n1];
			CString str;
			str.Format("%f", resolution);
			BSTR reso_bstr=str.AllocSysString();
			pLayerResolutionNode->text=reso_bstr;
			pLayernode->appendChild(pLayerResolutionNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerSimiTypeNode=pDoc->createElement("SimilarityType");
			CString simitype= pProjectCase->vecGeologyLayerSimiTypes[n1];
			BSTR simitype_bstr=simitype.AllocSysString();
			pLayerSimiTypeNode->text=simitype_bstr;
			pLayernode->appendChild(pLayerSimiTypeNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerSimiMethodNode=pDoc->createElement("SimilarityMethod");
			CString simimethod= pProjectCase->vecGeologyLayerSimiMethods[n1];
			BSTR simimethod_bstr=simimethod.AllocSysString();
			pLayerSimiMethodNode->text=simimethod_bstr;
			pLayernode->appendChild(pLayerSimiMethodNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();


		}

		pLayerNameNode.Release();
		pLayerPathNode.Release();
		pLayerResolutionNode.Release();
		pLayerSimiTypeNode.Release();
		pLayerSimiMethodNode.Release();
		pLayernode.Release();

	}   

	MSXML2::IXMLDOMElementPtr pTerrainNode;
	pTerrainNode =pDoc->createElement("Terrain");
	if(pDatabaseNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n "); 
		pDatabaseNode->appendChild(pTextNode);
		pTextNode.Release();
		pDatabaseNode->appendChild(pTerrainNode);
	}
	//append layer node

	for(int n1=0;n1<pProjectCase->vecTerrainLayerNames.size(); n1++)
	{

		pLayernode=pDoc->createElement("layer");
		if(pLayernode!=NULL)
		{
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pTerrainNode->appendChild(pTextNode);
			pTextNode.Release();		
			pTerrainNode->appendChild(pLayernode);
			pTextNode = pDoc->createTextNode(L"\n   "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerNameNode=pDoc->createElement("Name");
			CString layername=pProjectCase->vecTerrainLayerNames[n1];
			BSTR layername_bstr=layername.AllocSysString();
			pLayerNameNode->text=layername_bstr;
			pLayernode->appendChild(pLayerNameNode);
			pTextNode = pDoc->createTextNode(L"\n   "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();
			pLayerPathNode=pDoc->createElement("File");
			CString filename=pProjectCase->vecTerrainLayerFiles[n1];
			BSTR filename_bstr=filename.AllocSysString();
			pLayerPathNode->text=filename_bstr;
			pLayernode->appendChild(pLayerPathNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerResolutionNode=pDoc->createElement("Resolution");
			double resolution= pProjectCase->vecTerrainLayerResolutions[n1];
			CString str;
			str.Format("%f", resolution);
			BSTR reso_bstr=str.AllocSysString();
			pLayerResolutionNode->text=reso_bstr;
			pLayernode->appendChild(pLayerResolutionNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerSimiTypeNode=pDoc->createElement("SimilarityType");
			CString simitype= pProjectCase->vecTerrainLayerSimiTypes[n1];
			BSTR simitype_bstr=simitype.AllocSysString();
			pLayerSimiTypeNode->text=simitype_bstr;
			pLayernode->appendChild(pLayerSimiTypeNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerSimiMethodNode=pDoc->createElement("SimilarityMethod");
			CString simimethod= pProjectCase->vecTerrainLayerSimiMethods[n1];
			BSTR simimethod_bstr=simimethod.AllocSysString();
			pLayerSimiMethodNode->text=simimethod_bstr;
			pLayernode->appendChild(pLayerSimiMethodNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();


		}
		pLayerNameNode.Release();
		pLayerPathNode.Release();
		pLayerResolutionNode.Release();
		pLayerSimiTypeNode.Release();
		pLayerSimiMethodNode.Release();
		pLayernode.Release();

	}   

	MSXML2::IXMLDOMElementPtr pVegetationNode;
	pVegetationNode =pDoc->createElement("Vegetation");
	if(pDatabaseNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n "); 
		pDatabaseNode->appendChild(pTextNode);
		pTextNode.Release();
		pDatabaseNode->appendChild(pVegetationNode);
	}

	for(int n1=0;n1<pProjectCase->vecVegetationLayerNames.size(); n1++)
	{

		pLayernode=pDoc->createElement("layer");
		if(pLayernode!=NULL)
		{
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pVegetationNode->appendChild(pTextNode);
			pTextNode.Release();		
			pVegetationNode->appendChild(pLayernode);
			pTextNode = pDoc->createTextNode(L"\n   "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerNameNode=pDoc->createElement("Name");
			CString layername=pProjectCase->vecVegetationLayerNames[n1];
			BSTR layername_bstr=layername.AllocSysString();
			pLayerNameNode->text=layername_bstr;
			pLayernode->appendChild(pLayerNameNode);
			pTextNode = pDoc->createTextNode(L"\n   "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();
			pLayerPathNode=pDoc->createElement("File");
			CString filename=pProjectCase->vecVegetationLayerFiles[n1];
			BSTR filename_bstr=filename.AllocSysString();
			pLayerPathNode->text=filename_bstr;
			pLayernode->appendChild(pLayerPathNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerResolutionNode=pDoc->createElement("Resolution");
			double resolution= pProjectCase->vecVegetationLayerResolutions[n1];
			CString str;
			str.Format("%f", resolution);
			BSTR reso_bstr=str.AllocSysString();
			pLayerResolutionNode->text=reso_bstr;
			pLayernode->appendChild(pLayerResolutionNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerSimiTypeNode=pDoc->createElement("SimilarityType");
			CString simitype= pProjectCase->vecVegetationLayerSimiTypes[n1];
			BSTR simitype_bstr=simitype.AllocSysString();
			pLayerSimiTypeNode->text=simitype_bstr;
			pLayernode->appendChild(pLayerSimiTypeNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerSimiMethodNode=pDoc->createElement("SimilarityMethod");
			CString simimethod= pProjectCase->vecVegetationLayerSimiMethods[n1];
			BSTR simimethod_bstr=simimethod.AllocSysString();
			pLayerSimiMethodNode->text=simimethod_bstr;
			pLayernode->appendChild(pLayerSimiMethodNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();


		}

		pLayerNameNode.Release();
		pLayerPathNode.Release();
		pLayerResolutionNode.Release();
		pLayerSimiTypeNode.Release();
		pLayerSimiMethodNode.Release();
		pLayernode.Release();

	}   

	MSXML2::IXMLDOMElementPtr pOtherNode;
	pOtherNode =pDoc->createElement("Other");
	if(pDatabaseNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n "); 
		pDatabaseNode->appendChild(pTextNode);
		pTextNode.Release();
		pDatabaseNode->appendChild(pOtherNode);
	}
	//append layer node
	//MSXML2::IXMLDOMElementPtr pLayernode;
	for(int n1=0;n1<pProjectCase->vecOtherLayerNames.size(); n1++)
	{

		pLayernode=pDoc->createElement("layer");
		if(pLayernode!=NULL)
		{
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pOtherNode->appendChild(pTextNode);
			pTextNode.Release();		
			pOtherNode->appendChild(pLayernode);
			pTextNode = pDoc->createTextNode(L"\n   "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerNameNode=pDoc->createElement("Name");
			CString layername=pProjectCase->vecOtherLayerNames[n1];
			BSTR layername_bstr=layername.AllocSysString();
			pLayerNameNode->text=layername_bstr;
			pLayernode->appendChild(pLayerNameNode);
			pTextNode = pDoc->createTextNode(L"\n   "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();
			pLayerPathNode=pDoc->createElement("File");
			CString filename=pProjectCase->vecOtherLayerFiles[n1];
			BSTR filename_bstr=filename.AllocSysString();
			pLayerPathNode->text=filename_bstr;
			pLayernode->appendChild(pLayerPathNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerResolutionNode=pDoc->createElement("Resolution");
			double resolution= pProjectCase->vecOtherLayerResolutions[n1];
			CString str;
			str.Format("%f", resolution);
			BSTR reso_bstr=str.AllocSysString();
			pLayerResolutionNode->text=reso_bstr;
			pLayernode->appendChild(pLayerResolutionNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerSimiTypeNode=pDoc->createElement("SimilarityType");
			CString simitype= pProjectCase->vecOtherLayerSimiTypes[n1];
			BSTR simitype_bstr=simitype.AllocSysString();
			pLayerSimiTypeNode->text=simitype_bstr;
			pLayernode->appendChild(pLayerSimiTypeNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();

			pLayerSimiMethodNode=pDoc->createElement("SimilarityMethod");
			CString simimethod= pProjectCase->vecOtherLayerSimiMethods[n1];
			BSTR simimethod_bstr=simimethod.AllocSysString();
			pLayerSimiMethodNode->text=simimethod_bstr;
			pLayernode->appendChild(pLayerSimiMethodNode);	
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pLayernode->appendChild(pTextNode);
			pTextNode.Release();


		}

		pLayerNameNode.Release();
		pLayerPathNode.Release();
		pLayerResolutionNode.Release();
		pLayerSimiTypeNode.Release();
		pLayerSimiMethodNode.Release();
		pLayernode.Release();

	}   

	pTextNode = pDoc->createTextNode(L"\n "); 
	pDatabaseNode->appendChild(pTextNode);
	pTextNode.Release();


	pClimateNode.Release();
	pGeologyNode.Release();
	pTerrainNode.Release();
	pVegetationNode.Release();

	pOtherNode.Release();

	pDatabaseNode.Release();


	//************* Create KnowledgeBase node  ****************
	MSXML2::IXMLDOMElementPtr pKnowledgebaseNode;
	pKnowledgebaseNode=pDoc->createElement("FieldSamples");
	if(pKnowledgebaseNode!=NULL)
	{  
		pTextNode = pDoc->createTextNode(L"\n "); 
		pRootElem->appendChild(pTextNode);
		pTextNode.Release();
		pRootElem->appendChild(pKnowledgebaseNode);
	}

	MSXML2::IXMLDOMElementPtr pCasebaseFileNode;

	pCasebaseFileNode=pDoc->createElement("FieldSampleFile");
	if(pCasebaseFileNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pCasebaseFileNode->appendChild(pTextNode);
		pTextNode.Release();

		CString casebaseFile =pProjectCase->m_strCasebaseFile;
		_bstr_t casebaseFile_bstr=casebaseFile.AllocSysString();
		pCasebaseFileNode->text=casebaseFile_bstr;

		pKnowledgebaseNode->appendChild(pCasebaseFileNode);

		//pCasebaseFileNode->text= pProjectCase->m_strCasebasePath;

	}

	pTextNode = pDoc->createTextNode(L"\n "); 
	pKnowledgebaseNode->appendChild(pTextNode);
	pTextNode.Release();


	pCasebaseFileNode.Release();
	pKnowledgebaseNode.Release();



	//************** Create Inference¡¡************************
	MSXML2::IXMLDOMElementPtr pInferenceNode;
	pInferenceNode=pDoc->createElement("Inference");
	if(pInferenceNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n "); 
		pRootElem->appendChild(pTextNode);
		pTextNode.Release();

		pRootElem->appendChild(pInferenceNode);
	}



	//MSXML2::IXMLDOMElementPtr pUseMaskNode,pInferMaskNode,pMaskValueNode,pInferDirNode,pInferCreateCheckNode;


	MSXML2::IXMLDOMElementPtr pCaseMethodNode, pClimateCutNode, pClimateWeightNode, pTopoWeightNode, pVegWeightNode, pOtherWeightNode,  pCBROptionNode, pXIndexNode, pYIndexNode,pIDIndexNode, pPropertyThresholdNode;// pTypeThresholdNode;

	/*pUseKdeNode=pDoc->createElement("UseKDEForTerrain");
	if(pUseKdeNode!=NULL)
	{
	pTextNode = pDoc->createTextNode(L"\n  "); 
	pInferenceNode->appendChild(pTextNode);
	pTextNode.Release();
	if(m_pProjectCase->g_useKde ==true)
	pUseKdeNode->text="true";
	else
	pUseKdeNode->text="false";
	pInferenceNode->appendChild(pUseKdeNode);
	}

	pUseKdeVegetationNode=pDoc->createElement("UseKDEForVegetation");
	if(pUseKdeVegetationNode!=NULL)
	{
	pTextNode = pDoc->createTextNode(L"\n  "); 
	pInferenceNode->appendChild(pTextNode);
	pTextNode.Release();
	if(m_pProjectCase->g_useKdeVegetation ==true)
	pUseKdeVegetationNode->text="true";
	else
	pUseKdeVegetationNode->text="false";
	pInferenceNode->appendChild(pUseKdeVegetationNode);
	}*/


	pCaseMethodNode = pDoc->createElement("CaseMethod");

	if(pCaseMethodNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		if(m_pProjectCase->m_iCaseInteMethod == 0)
			pCaseMethodNode->text = "LimitingFactor";			
		else if(m_pProjectCase->m_iCaseInteMethod == 1)
			pCaseMethodNode->text="AverageWeighted";
		else
			pCaseMethodNode->text= "UserDefined";
		pInferenceNode->appendChild(pCaseMethodNode);
	}



	pClimateCutNode=pDoc->createElement("ClimateCut");
	if(pClimateCutNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		CString str;
		str.Format("%f",pProjectCase->m_dClimateCut);
		pClimateCutNode->text=(_bstr_t)str;
		pInferenceNode->appendChild(pClimateCutNode);
	}

	pClimateWeightNode=pDoc->createElement("ClimateWeight");
	if(pClimateWeightNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		CString str;
		str.Format("%f",pProjectCase->m_dClimateWeight);
		pClimateWeightNode->text=(_bstr_t)str;
		pInferenceNode->appendChild(pClimateWeightNode);
	}



	pTopoWeightNode=pDoc->createElement("TerrainWeight");
	if(pTopoWeightNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		CString str;
		str.Format("%f",pProjectCase->m_dTopoWeight);
		pTopoWeightNode->text=(_bstr_t)str;
		pInferenceNode->appendChild(pTopoWeightNode);
	}


	pVegWeightNode=pDoc->createElement("VegetationWeight");
	if(pVegWeightNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		CString str;
		str.Format("%f",pProjectCase->m_dVegWeight);
		pVegWeightNode->text=(_bstr_t)str;
		pInferenceNode->appendChild(pVegWeightNode);
	}


	pOtherWeightNode=pDoc->createElement("OtherWeight");
	if(pOtherWeightNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		CString str;
		str.Format("%f",pProjectCase->m_dOtherWeight);
		pOtherWeightNode->text=(_bstr_t)str;
		pInferenceNode->appendChild(pOtherWeightNode);
	}






	pCBROptionNode=pDoc->createElement("SBIOption");

	if(pCBROptionNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		if(m_pProjectCase->iCBROption ==1)
			pCBROptionNode->text="Property";
		else
			pCBROptionNode->text="Type";
		pInferenceNode->appendChild(pCBROptionNode);
	}

	pXIndexNode=pDoc->createElement("XIndex");
	if(pXIndexNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		CString str = pProjectCase->m_strXIndex;
		pXIndexNode->text=(_bstr_t)str;
		pInferenceNode->appendChild(pXIndexNode);
	}

	pYIndexNode=pDoc->createElement("YIndex");
	if(pYIndexNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		CString str = pProjectCase->m_strYIndex;
		pYIndexNode->text=(_bstr_t)str;
		pInferenceNode->appendChild(pYIndexNode);
	}

	pIDIndexNode=pDoc->createElement("IDIndex");
	if(pIDIndexNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		CString str = pProjectCase->m_strIDIndex;
		pIDIndexNode->text=(_bstr_t)str;
		pInferenceNode->appendChild(pIDIndexNode);
	}




	pPropertyThresholdNode=pDoc->createElement("UncertaintyThreshold");
	if(pPropertyThresholdNode!=NULL)
	{
		pTextNode = pDoc->createTextNode(L"\n  "); 
		pInferenceNode->appendChild(pTextNode);
		pTextNode.Release();
		CString str;
		str.Format("%f",pProjectCase->m_thresholdProperty);
		pPropertyThresholdNode->text=(_bstr_t)str;
		pInferenceNode->appendChild(pPropertyThresholdNode);
	}



	/*pTypeThresholdNode=pDoc->createElement("SimilarityThreshold");
	if(pTypeThresholdNode!=NULL)
	{
	pTextNode = pDoc->createTextNode(L"\n  "); 
	pInferenceNode->appendChild(pTextNode);
	pTextNode.Release();
	CString str;
	str.Format("%f",pProjectCase->m_thresholdType);
	pTypeThresholdNode->text=(_bstr_t)str;
	pInferenceNode->appendChild(pTypeThresholdNode);
	}*/



	pTextNode = pDoc->createTextNode(L"\n  "); 
	pInferenceNode->appendChild(pTextNode);
	pTextNode.Release();

	//pUseKdeNode.Release();
	//pUseKdeVegetationNode.Release();

	pCaseMethodNode.Release();
	pClimateCutNode.Release();
	pClimateWeightNode.Release();
	pTopoWeightNode.Release();
	pVegWeightNode.Release();
	pOtherWeightNode.Release();
	//pPropertyThresholdNode.Release();
	//pTypeThresholdNode.Release();

	pCBROptionNode.Release();
	pXIndexNode.Release();
	pYIndexNode.Release();
	pIDIndexNode.Release();
	pPropertyThresholdNode.Release();


	pInferenceNode.Release();



	//*************** Save the xml document **********************
	pTextNode = pDoc->createTextNode(L"\n "); 
	pRootElem->appendChild(pTextNode);
	pTextNode.Release();


	pRootElem.Release();
	_variant_t varxml(strFileName);
	hr=pDoc->save(varxml);
	if(FAILED(hr))
	{
		AfxMessageBox("Error in saving project!");
		return;
	}
	pDoc.Release();

	CoUninitialize();

}

void CSoLIMDoc::OnProjectConvertfromSoLIM4()
{
	m_convertTacitPrjDlg.DoModal();

}

void CSoLIMDoc::OnProjectConvertfromoldsolim()
{
	m_convertOldPrjDlg.DoModal();
}


void CSoLIMDoc::OnProjectConvertfromsie()
{
	m_convertSIEDlg.DoModal();
}



void CSoLIMDoc::OnProjectConvertfrosolimsolutions5()
{
	m_convert5PrjDlg.DoModal();
}

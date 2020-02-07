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
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SoLIM.h"

#include "MainFrm.h"
#include ".\mainfrm.h"
#include "DatabaseView.h"
#include "KnowledgeView.h"
#include "InferenceView.h"
#include "DataExplorerView.h"

#include "CasebaseView.h"
#include "CBInferenceView.h"

#include "KnowledgeFromPSExpertDlg.h"
#include "RemoteSensingSettingDlg.h"
#include "KnowledgeFromPSDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	ON_WM_SIZE()	
	ON_WM_SIZING()

	ON_COMMAND(ID_NAVIGATION_ZOOMIN, OnNavigationZoomin)
	ON_COMMAND(ID_NAVIGATION_ZOOMOUT, OnNavigationZoomout)
	ON_COMMAND(ID_NAVIGATION_ENTIRE_LAYER,OnNavigationEntireLayer)	
	ON_UPDATE_COMMAND_UI(ID_NAVIGATION_ZOOMIN, OnUpdateNavigationZoomin)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATION_ZOOMOUT, OnUpdateNavigationZoomout)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATION_PAN, OnUpdateNavigationPan)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATION_RECT_ZOOM, OnUpdateNavigationRectZoom)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATION_ENTIRE_LAYER, OnUpdateNavigationEntireLayer)
	ON_COMMAND(ID_NAVIGATION_PAN,OnNavigationPan)
	ON_COMMAND(ID_NAVIGATION_RECT_ZOOM,OnNavigationRectZoom)

	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_COMMAND(ID_VIEW_PROJECT_TREE, OnViewProjectTree)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROJECT_TREE, OnUpdateViewProjectTree)

	ON_COMMAND(ID_PRODUCTDERIVATION_PROPERTYMAP, OnProductderivationPropertymap)
	ON_COMMAND(ID_PRODUCTDERIVATION_HARDENFUZZYMAPS, OnProductderivationHardenFuzzymaps)
	ON_COMMAND(ID_UTILITIES_VALUESATSAMPLESITES, OnUtilitiesValuesAtSamplesites)
	ON_COMMAND(ID_VALIDATION_ERRORMATRIX_WITHMAP, OnValidationErrorMatrixWithMap)
	ON_COMMAND(ID_VALIDATION_ERRORMATRIX_WITHPOINTS, OnValidationErrorMatrixWithPoints)
	ON_COMMAND(ID_UTILITIES_SAMPLE, OnUtilitiesSample)
	ON_COMMAND(ID_DATAPREPARATION_DEMFILLANDSHAVE, OnDataPreparationDemfillandshave)
	ON_COMMAND(ID_DATAPREPARATION_SURFACEDERIVATIVES, OnDataPreparationSurfacederivatives)
	ON_COMMAND(ID_DATAPREPARATION_FLOWDIRECTION, OnDataPreparationFlowdirection)
	ON_COMMAND(ID_DATAPREPARATION_CACHMENT, OnDataPreparationCatchment)
	ON_COMMAND(ID_DATAPREPARATION_FLOWACCUMULATIONSINGLE, OnDataPreparationFlowaccumulationSingle)
	ON_COMMAND(ID_DATAPREPARATION_FLOWACCUMULATIONMULTIPLE, OnDataPreparationFlowaccumulationMultiple)
	ON_COMMAND(ID_DATAPREPARATION_WETNESSINDEX, OnDataPreparationWetnessindex)
	ON_COMMAND(ID_DATAPREPARATION_RIDGELINE, OnDataPreparationRidgeline)
	ON_COMMAND(ID_DATAPREPARATION_BROADNARROWRIDGELINE, OnDataPreparationBroadnarrowridgeline)
	ON_COMMAND(ID_DATAPREPARATION_SHOULDER, OnDataPreparationShoulder)
	ON_COMMAND(ID_DATAPREPARATION_BACK, OnDataPreparationBack)
	ON_COMMAND(ID_DATAPREPARATION_STREAMLINE, OnDataPreparationStreamline)
	ON_COMMAND(ID_DATAPREPARATION_HEADWATER, OnDataPreparationHeadwater)
	ON_COMMAND(ID_DATAPREPARATION_TERRAINPARTITION, OnDataPreparationTerrainpartition)
	ON_COMMAND(ID_DATAPREPARATION_NDVI, OnDatapreparationNdvi)
	ON_COMMAND(ID_FILECONVERTER_3DRGRIDASCII, OnFileConverter3drToGridascii)
	ON_COMMAND(ID_FILECONVERTER_CALCSTATISTICS, OnFileCalcstatistics)
	ON_COMMAND(ID_FILECONVERTER_3DRSAGA, OnFileConverter3drToSaga)

	//ON_COMMAND(ID_FILECONVERTER_GRIDASCII3DR, OnFileconverterGridasciiTo3dr)
	//ON_COMMAND(ID_FILECONVERTER_GRIDASCII3DM, OnFileconverterGridasciiTo3dm)
	ON_COMMAND(ID_FILECONVERTER_3DR3DM, OnFileconverter3drTo3dm)
	ON_COMMAND(ID_UTILITIES_3DRVIEWER, OnUtilities3drViewer)
	ON_COMMAND(ID_UTILITIES_CLIP, OnUtilitiesClip)
	ON_COMMAND(ID_UTILITIES_RECLASS, OnUtilitiesReclass)
	ON_COMMAND(ID_UTILITIES_OVERLAY, OnUtilitiesOverlay)
	ON_COMMAND(ID_UTILITIES_STRETCH, OnUtilitiesStretch)
	ON_COMMAND(ID_UTILITIES_FREQUENCY, OnUtilitiesFrequency)
	ON_COMMAND(ID_UTILITIES_COLOR, OnUtilitiesColor)

	ON_COMMAND(ID_UTILITIES_3BANDSTOCOLORFILE, OnUtilities3BandsToColor)
	ON_COMMAND(ID_UTILITIES_FILTER, OnUtilitiesFilter)  
	ON_COMMAND(ID_UTILITIES_3DVISUALIZATION, OnUtilities3dvisualization)
	ON_COMMAND(ID_HELP_SOLIMHELP, OnHelp)

	ON_WM_CLOSE()
	ON_COMMAND(ID_DATABASE_DATALOCATION, OnDatabaseDatalocation)

	ON_COMMAND(ID_FILECONVERTER_OTHERRASTERFORMAT, OnFileconverterOtherrasterformat)

	ON_COMMAND(ID_FILECONVERTER_SHP, OnFileconverterShapefile)

	ON_COMMAND(ID_FILECONVERTER_OTHERTO3DM, OnFileconverterOtherto3dm)
	ON_COMMAND(ID_TYPEVALIDATAION_POINTLISTVS, OnTypevalidataionPointlistvs)
	ON_COMMAND(ID_VALIDATION_PROPERTYVALIDATION, OnValidationPropertyvalidation)

	ON_COMMAND(ID_PURPOSIVESAMPLING_FCMCLUSTERING, OnPurposivesamplingFcmclustering)
	ON_COMMAND(ID_PURPOSIVESAMPLING_SAMPLEDESIGN, OnPurposivesamplingSampledesign)
	ON_COMMAND(ID_MAPPING_WITH_SAMPLES, OnMappingWithSamples)


	ON_COMMAND(ID_SAMPLEDESIGN_PURPOSIVESAMPLING, OnPurposivesamplingSampledesign)


	ON_COMMAND(ID_KNOWLEDGEACQUISITION_FROMPURPOSIVESAMPLING_EXPERT, OnKnowledgeAquisitionPurposiveSamplingExpert)
	ON_COMMAND(ID_KNOWLEDGEACQUISITION_FROMPURPOSIVESAMPLING_NOVICE, OnKnowledgeAquisitionPurposiveSamplingNovice)
	ON_COMMAND(ID_KNOWLEDGEACQUISITION_FROMEXPERT, OnKnowledgeAquisitionExpert)
	ON_COMMAND(ID_DATAPREPARATION_REMOTESENSING, OnDataPreparationRemoteSensing)
	ON_COMMAND(ID_KNOWLEDGEACQUISITION_FROMMAP, OnKnowledgeAquisitionMap)
	

	//}}AFX_MSG_MAP		

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
		ID_INDICATOR_CAPS,
		ID_INDICATOR_NUM,
		ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{


	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	InitMenu(IDR_MAINFRAME);
	InitToolBar(IDR_MAINFRAME);
	//InitStatusBar(indicators,sizeof(indicators)/sizeof(UINT));

	if (!m_leftBar.Create(_T("         Project Types"), WS_VISIBLE, this, 995))
	{
		TRACE0("Failed to create Control Panel\n");
		return -1;      
	}

	m_leftBar.SetBarStyle(m_leftBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	EnableDocking(CBRS_ALIGN_ANY);
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar,m_dockTop);
	DockControlBar(&m_wndToolBar,m_dockTop);
	m_leftBar.EnableDocking(CBRS_ALIGN_LEFT);
	DockControlBar(&m_leftBar, AFX_IDW_DOCKBAR_LEFT);

	sProfile = _T("SoLIM");

	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)GetActiveDocument();

	pDoc->m_CMruFile.pMenu= CMenu::FromHandle(m_wndMenuBar.m_hMenu); //get menu from handle
	pDoc->m_CMruFile.m_IniFileName = pApp->m_exePath+_T("MRU.ini");	//set ini file name
	pDoc->m_CMruFile.ReadMru();		//read ini file which holds the recent file list


		// Install menu tip manager--that's all!
	m_menuTipManager.Install(this);

	//this->m_wndStatusBar.SetPaneText(0,"fsdfl");

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	  m_strTitle = "SoLIM Solutions"; 
	  cs.x = 10;
	  cs.y = 10;
	  cs.cx = 1000; 
      cs.cy = 750; 
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CGuiFrameWnd::OnSize(nType, cx, cy);
	//Invalidate();
}

void CMainFrame::OnViewToolbar()
{
	ShowControlBar(&m_wndToolBar, !m_wndToolBar.IsVisible(), FALSE); 

}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{  
	m_splitterTabWnd.AddView( RUNTIME_CLASS( CDatabaseView ),"GIS Database Panel" );  

	m_splitterTabWnd.AddView( RUNTIME_CLASS( CKnowledgeView ),"Knowledge Base Panel" );  
	m_splitterTabWnd.AddView( RUNTIME_CLASS( CInferenceView ),"Knowledge Based Inference Panel" ); 

	m_splitterTabWnd.AddView( RUNTIME_CLASS( CCasebaseView ),"Case Base Panel" ); 
	m_splitterTabWnd.AddView( RUNTIME_CLASS( CCBInferenceView ),"Case Based Inference Panel" ); 

	m_splitterTabWnd.Create( this, pContext );
	SetActiveView((CView *)m_splitterTabWnd.GetPane(0,0));//default is database view
	return TRUE;
}

void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect)
{
	if   ((pRect->right-pRect->left)<800)   pRect->right=pRect->left+800;   
	if   ((pRect->bottom-pRect->top)<600)   pRect->bottom =pRect->top+600; 

	CGuiFrameWnd::OnSizing(fwSide, pRect);
}


HMENU CMainFrame::getMenuHandle()
{
	return m_wndMenuBar.m_hMenu;
}

void CMainFrame::OnViewProjectTree()
{
	ShowControlBar(&m_leftBar, !m_leftBar.IsVisible(), FALSE); 

}

void CMainFrame::OnUpdateViewProjectTree(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_leftBar.IsVisible());

}

void CMainFrame::OnNavigationZoomin()
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	pApp->m_iIsOriginChanged=1;
	CDataExplorerView *pDataView = (CDataExplorerView *)(((CDatabaseView *)(m_splitterTabWnd.GetPane(0,0)))->m_wndSplitter.GetPane(0,0));
	pDataView->ImageZoom(pDataView ->m_ptMidPt,pDataView ->m_dZoom+0.5);
	pDataView->m_iToolChoice=0;
	pDataView->Invalidate();
}
void CMainFrame::OnUpdateNavigationZoomin(CCmdUI *pCmdUI)
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
	if(pDoc->getCurViewType()==0&&pDoc->m_pProjectEdit!=NULL)
		pCmdUI->Enable(true);
	else if(pDoc->getCurViewType()==0&&pDoc->m_pProjectCaseEdit!=NULL)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);

}

void CMainFrame::OnNavigationZoomout()
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	pApp->m_iIsOriginChanged=1;
	CDataExplorerView *pDataView = (CDataExplorerView *)(((CDatabaseView *)(m_splitterTabWnd.GetPane(0,0)))->m_wndSplitter.GetPane(0,0));
	if(pDataView ->m_dZoom>1)
		pDataView->ImageZoom(pDataView->m_ptMidPt,pDataView->m_dZoom-0.5);
	pDataView->m_iToolChoice=0;
	pDataView->Invalidate();
}
void CMainFrame::OnUpdateNavigationZoomout(CCmdUI *pCmdUI)
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
	if(pDoc->getCurViewType()==0&&pDoc->m_pProjectEdit!=NULL)
		pCmdUI->Enable(true);
	else if(pDoc->getCurViewType()==0&&pDoc->m_pProjectCaseEdit!=NULL)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);

}


void CMainFrame::OnNavigationPan()
{
	CDataExplorerView *pDataView = (CDataExplorerView *)(((CDatabaseView *)(m_splitterTabWnd.GetPane(0,0)))->m_wndSplitter.GetPane(0,0));
	if(pDataView->m_iToolChoice!=1)
	{
		pDataView->m_iToolChoice=1;
	}
	else
	{
		pDataView->m_iToolChoice=0;
	}
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	pApp->m_iIsOriginChanged=1;
}
void CMainFrame::OnUpdateNavigationPan(CCmdUI *pCmdUI)
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
	if(pDoc->getCurViewType()==0&&pDoc->m_pProjectEdit!=NULL)
		pCmdUI->Enable(true);
	else if(pDoc->getCurViewType()==0&&pDoc->m_pProjectCaseEdit!=NULL)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);

}

void CMainFrame::OnNavigationRectZoom()
{
	CDataExplorerView *pDataView = (CDataExplorerView *)(((CDatabaseView *)(m_splitterTabWnd.GetPane(0,0)))->m_wndSplitter.GetPane(0,0));
	if(pDataView->m_iToolChoice!=2)
	{
		pDataView->m_iToolChoice=2;
	}
	else
	{
		pDataView->m_iToolChoice=0;
	}
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	pApp->m_iIsOriginChanged=1;
}

void CMainFrame::OnUpdateNavigationRectZoom(CCmdUI *pCmdUI)
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
	if(pDoc->getCurViewType()==0&&pDoc->m_pProjectEdit!=NULL)
		pCmdUI->Enable(true);
  	else if(pDoc->getCurViewType()==0&&pDoc->m_pProjectCaseEdit!=NULL)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);

}


void CMainFrame::OnNavigationEntireLayer()
{
	CDataExplorerView *pDataView = (CDataExplorerView *)(((CDatabaseView *)(m_splitterTabWnd.GetPane(0,0)))->m_wndSplitter.GetPane(0,0));
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	pApp->m_iIsOriginChanged=0;
	pDataView->m_dZoom=1;
	pDataView->m_iToolChoice=0;
	Invalidate(false);
}

void CMainFrame::OnUpdateNavigationEntireLayer(CCmdUI *pCmdUI)
{
	CSoLIMApp* pApp=(CSoLIMApp *)AfxGetApp();
	CMainFrame*   pFrame   =   (CMainFrame*)AfxGetMainWnd();
	CSoLIMDoc *pDoc = (CSoLIMDoc *)(pFrame->GetActiveDocument());
	if(pDoc->getCurViewType()==0&&pDoc->m_pProjectEdit!=NULL)
		pCmdUI->Enable(true);
	else if(pDoc->getCurViewType()==0&&pDoc->m_pProjectCaseEdit!=NULL)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);

}


/****************************
Validation Menu
****************************/
void CMainFrame::OnValidationErrorMatrixWithMap()
{
	// TODO: Add your command handler code here
	if( m_errorMatrixPntLabMapDlg.DoModal() == IDOK)
		MessageBox("Creating error matrix using pnt/lab and hardened map is done.");
}

void CMainFrame::OnValidationErrorMatrixWithPoints()
{
	// TODO: Add your command handler code here
	if(m_errorMatrixPointToPointDlg.DoModal() == IDOK)
		AfxMessageBox("Creating error matrix using point list files is done.");
}



void CMainFrame::OnUtilitiesValuesAtSamplesites()
{
	// TODO: Add your command handler code here
	if(m_valueAtSiteDlg.DoModal() == IDOK)
		MessageBox("Capturing values at sample sites is done.");
}

void CMainFrame::OnUtilitiesSample()
{
	// TODO: Add your command handler code here
	if(m_sampleDlg.DoModal() == IDOK)
		AfxMessageBox("Sampling is done.");	
}

/*****************************
Product Derivation Menu
*****************************/
void CMainFrame::OnProductderivationPropertymap()
{
	// TODO: Add your command handler code here
	if(m_PropertyMapDlg.DoModal() == IDOK)
		MessageBox("Creating property map is done.");
}

/****************************
Data Preparation Menu
****************************/
void CMainFrame::OnDataPreparationDemfillandshave()
{
	// TODO: Add your command handler code here
	if(m_fillShaveDEMDlg.DoModal() == IDOK)
		AfxMessageBox("Filling and shaving DEM is done.");
}

void CMainFrame::OnDataPreparationSurfacederivatives()
{
	// TODO: Add your command handler code here
	if(m_surDerivativeDlg.DoModal() == IDOK)
		MessageBox("Calculation surface derivatieve is done.");
}

void CMainFrame::OnDataPreparationFlowdirection()
{
	// TODO: Add your command handler code here
	if(m_flowDirDlg.DoModal() == IDOK)
		AfxMessageBox("Creating flow direction file is done.");	
}

void CMainFrame::OnDataPreparationFlowaccumulationSingle()
{
	// TODO: Add your command handler code here
	if(m_UDADlg.DoModal() == IDOK)
		MessageBox("Creating single direction flowaccumulation file is done.");
}

void CMainFrame::OnDataPreparationFlowaccumulationMultiple()
{
	// TODO: Add your command handler code here
	if(m_MFDDlg.DoModal() == IDOK)
		AfxMessageBox("Creating multiple direction flow accumulation layer is done.");
}

void CMainFrame::OnDataPreparationCatchment()
{
	// TODO: Add your command handler code here
	if(m_catchmentDlg.DoModal() == IDOK)
		MessageBox("Creating catchment file is done.");

}

void CMainFrame::OnDataPreparationWetnessindex()
{
	// TODO: Add your command handler code here
	if(m_wetnessDlg.DoModal() == IDOK)
		MessageBox("Creating wetness index file is done.");
}

void CMainFrame::OnDataPreparationRidgeline()
{
	// TODO: Add your command handler code here
	if(m_ridgelineDlg.DoModal() == IDOK)
		MessageBox("Creating ridgeline file is done.");
}

void CMainFrame::OnDataPreparationBroadnarrowridgeline()
{
	// TODO: Add your command handler code here
	if(m_brdNrwRidgeDlg.DoModal() == IDOK)
		MessageBox("Creating broad/narrow ridgeline file is done.");
}

void CMainFrame::OnDataPreparationShoulder()
{
	// TODO: Add your command handler code here
	if(m_shoulderDlg.DoModal() == IDOK)
		MessageBox("Creating shoulder file is done.");	
}

void CMainFrame::OnDataPreparationBack()
{
	// TODO: Add your command handler code here
	if(m_backDlg.DoModal() == IDOK)
		MessageBox("Creating back file is done.");		
}

void CMainFrame::OnDataPreparationStreamline()
{
	// TODO: Add your command handler code here
	if(m_streamlineDlg.DoModal() == IDOK)
		MessageBox("Creating streamline file is done.");

}

void CMainFrame::OnDataPreparationHeadwater()
{
	// TODO: Add your command handler code here
	if(m_headwaterDlg.DoModal() == IDOK)
		MessageBox("Creating headwater file is done.");
}

void CMainFrame::OnDataPreparationTerrainpartition()
{
	// TODO: Add your command handler code here
	if(m_terrainPartitionDlg.DoModal() == IDOK)
		MessageBox("Creating terrain partition file is done.");
}

void CMainFrame::OnDatapreparationNdvi()
{
	MessageBox("Compute NDVI");
}

/*************************
Utilities Menu
*************************/

void CMainFrame::OnFileconverterOtherrasterformat()
{
	if(m_gdalTo3drDlg.DoModal() == IDOK)
		MessageBox("Converting other raster formats to 3dr file is done.");
}

void CMainFrame::OnFileconverterShapefile()
{
   CShpTo3drDlg shpTo3drDlg;

	if(shpTo3drDlg.DoModal() == IDOK)
	   MessageBox("Converting shapefile to 3dr file is done.");
}


void CMainFrame::OnFileconverterOtherto3dm()
{
	if(m_gdalTo3dmDlg.DoModal() == IDOK)
		MessageBox("Converting other raster formats to 3dm file is done.");
}


void CMainFrame::OnFileConverter3drToGridascii()
{
	// TODO: Add your command handler code here
	if(m_3drToAsciiDlg.DoModal() == IDOK)
		MessageBox("Converting 3dr file to ASCII file is done.");
}

void CMainFrame::OnFileConverter3drToSaga()
{
	// TODO: Add your command handler code here
	if(m_3drToSagaDlg.DoModal() == IDOK)
		MessageBox("Converting 3dr file to Saga file is done.");
}

/*
void CMainFrame::OnFileconverterGridasciiTo3dr()
{
	// TODO: Add your command handler code here
	if(m_asciiTo3drDlg.DoModal() == IDOK)
		MessageBox("Converting Grid Ascii to 3dr file is done.");
}

void CMainFrame::OnFileconverterGridasciiTo3dm(){
	// TODO: Add your command handler code here
	if(m_asciiTo3dmDlg.DoModal() == IDOK)
		MessageBox("Converting Grid Ascii to 3dm file is done.");
}
*/
void CMainFrame::OnFileconverter3drTo3dm()
{
	// TODO: Add your command handler code here
	if(m_3drTo3dmDlg.DoModal() == IDOK)
		MessageBox("Converting 3dr to 3dm file is done.");
}

void CMainFrame::OnFileCalcstatistics()
{
	// TODO: Add your command handler code here
	if(m_calcStatDlg.DoModal() == IDOK)
		MessageBox("Calculating statistics is done.");
}


/*
void CMainFrame::OnUtilities3drViewer()
{
	//m_3drViewerDlg.DoModal();

	//m_2dViewerDlg.DoModal();

	C2dViewerDlg *dlg=new C2dViewerDlg;
    dlg->Create(IDD_DIALOG_2dViewer,NULL);
	dlg->ShowWindow(SW_SHOW);


}*/


void CMainFrame::OnUtilities3drViewer()
{
	

	CSoLIMApp* app = (CSoLIMApp *)AfxGetApp();	//pointer of the executable program
	int result=(int)(ShellExecute(m_hWnd,_T("open"),_T("SoLIM_DataViewer.exe"),NULL,_T(app->m_exePath),SW_SHOW));
	

}




void CMainFrame::OnProductderivationHardenFuzzymaps()
{
	// TODO: Add your command handler code here
	if(m_hardenMapDlg.DoModal() == IDOK)
		MessageBox("Creating hardened map is done.");
}


void CMainFrame::OnUtilitiesClip()
{
	// TODO: Add your command handler code here
	if(m_clipDlg.DoModal() == IDOK)
		MessageBox("Clipping the input file is done.");
}

void CMainFrame::OnUtilitiesReclass()
{
	// TODO: Add your command handler code here
	if(m_reclassDlg.DoModal() == IDOK)
		MessageBox("Reclassing the input file is done.");
}

void CMainFrame::OnUtilitiesStretch()
{
	// TODO: Add your command handler code here
	if(m_stretchDlg.DoModal() == IDOK)
		MessageBox("Stretching the input file is done.");
}

void CMainFrame::OnUtilitiesOverlay()
{
	// TODO: Add your command handler code here
	if(m_overlayDlg.DoModal() == IDOK)
		MessageBox("Overlay is done.");
}

void CMainFrame::OnUtilitiesFrequency()
{
	// TODO: Add your command handler code here
	if(m_frequencyDlg.DoModal() == IDOK)
		MessageBox("Creating frequency file is done.");	
}


void CMainFrame::OnUtilitiesColor()
{
	// TODO: Add your command handler code here
	if(m_colorDlg.DoModal() == IDOK)
		MessageBox("Assigning color to the input file is done.");		
}


void CMainFrame::OnUtilities3BandsToColor()
{
	if(m_3BandsToColorDlg.DoModal() == IDOK)
		MessageBox("Generating color file from three bands is done.");	

}
void CMainFrame::OnUtilitiesFilter()
{
	// TODO: Add your command handler code here
	if(m_filterDlg.DoModal() == IDOK)
		MessageBox("Filtering the input file is done.");			
}


void CMainFrame::OnUtilities3dvisualization()
{	
	CString BaseFileName;
	static char BASED_CODE szFilter[] = "3dm files (*.3dm)|*.3dm||";
	CFileDialog m_3dmFileDlg(TRUE, NULL, "*.3dm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_3dmFileDlg.m_ofn.lpstrTitle="Choose Base File";	
	if(m_3dmFileDlg.DoModal() == IDOK)
	{
		BaseFileName=m_3dmFileDlg.GetPathName();
		HKEY hKEY;
		LPCTSTR data_Set="SOFTWARE\\Classes\\3dMapperExtension.File\\shell\\open\\command\\";
		long ret0=RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_READ,&hKEY);
		if(ret0!=ERROR_SUCCESS)
		{
			if(AfxMessageBox("3dMapper can not be found in your system! \nDo you want to find it by yourself?",MB_ICONQUESTION|MB_YESNO)==IDYES)
			{
				static char BASED_CODE szFilter[] = "EXE Files (*.exe)|*.exe||";
				CFileDialog m_ldFile(TRUE, NULL, "*.exe", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);		
				m_ldFile.m_ofn.lpstrTitle="Browse to Locate 3dMpper";	

				if(m_ldFile.DoModal() == IDOK)
				{
					CString path= m_ldFile.GetPathName();
					ShellExecute(m_hWnd,"open",path, BaseFileName,"", SW_SHOW );
				}
				return;
		 }
			else 
				return;
		}

		LPBYTE path_Get=new BYTE[MAX_PATH];
		DWORD type=REG_SZ;
		DWORD cbData=80;
		long ret=RegQueryValueEx(hKEY,"",NULL,&type,path_Get,&cbData);
		if(ret!=ERROR_SUCCESS)
		{
			if(AfxMessageBox("3dMapper can not be found in your system!\nDo you want to find it by yourself?",MB_ICONQUESTION|MB_YESNO)==IDYES)
		 {
			 static char BASED_CODE szFilter[] = "EXE Files (*.exe)|*.exe||";
			 CFileDialog m_ldFile(TRUE, NULL, "*.exe", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
			 m_ldFile.m_ofn.lpstrTitle="Browse to Locate 3dMapper";	

			 if(m_ldFile.DoModal() == IDOK)
			 {
				 CString path= m_ldFile.GetPathName();
				 ShellExecute(m_hWnd,"open",path, BaseFileName,"", SW_SHOW );
			 }
			 return;
		 }
			else 
				return;
		}
		CString path=CString(path_Get);
		delete[] path_Get;
		RegCloseKey(hKEY);
		path.TrimRight("%1");
		path.Trim();
		ShellExecute(m_hWnd,"open",path, BaseFileName,"", SW_SHOW );
	}
	else{	
		return;
	}  

}


/*************************
Help
*************************/
void CMainFrame::OnHelp()
{
	char FilePath[MAX_PATH];
	GetModuleFileName(NULL,FilePath,MAX_PATH); //get the path of the .exe file
	CString Path;
	Path.Format("%s",FilePath);
	Path.Replace("SoLIMSolutions.exe","");

	CSoLIMApp* app = (CSoLIMApp *)AfxGetApp();	//pointer of the executable program
	int result=(int)(ShellExecute(m_hWnd,_T("open"),_T("SoLIMSolutions_Help.chm"),NULL,_T(app->m_exePath),SW_SHOW));
	if(result<32)
	{
		if(AfxMessageBox("Help file can not be found! \nDo you want to find it by yourself?",MB_ICONQUESTION|MB_YESNO)==IDYES)
		{
			static char BASED_CODE szFilter[] = "help files (*.chm)|*.chm||";
			CFileDialog m_hlpFileDlg(TRUE, NULL, "*.chm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);	
			m_hlpFileDlg.m_ofn.lpstrTitle="Choose Help File";
			CString hlpFileName;
			CString hlpFilePath;
			if(m_hlpFileDlg.DoModal() == IDOK)
			{
				hlpFileName=m_hlpFileDlg.GetFileName();
				hlpFilePath=m_hlpFileDlg.GetPathName();
				hlpFilePath.Replace(hlpFileName,"");
				int result=(int)(ShellExecute(m_hWnd,_T("open"),hlpFileName,NULL,_T(hlpFilePath),SW_SHOW));
				if(result<32)
				{
					MessageBox("Error in opening help file.");
					return;
				}

			}
			else 
			{
				return;
			}
		}

	}

}

void CMainFrame::OnClose()
{
	CSoLIMDoc *pDoc = (CSoLIMDoc *)GetActiveDocument();
	if(pDoc->m_pProjectEdit!=NULL)
	{
		int result;
		result = MessageBox("Save project before exiting program?","Save Project", MB_YESNOCANCEL);
		if(result==IDYES)
		{
			pDoc->OnProjectSave();
		}
		else if(result==IDCANCEL)
			return;					
	}



    if(pDoc->m_pProjectCaseEdit!=NULL)
	{
		int result;
		result = MessageBox("Save project before exiting program?","Save Project", MB_YESNOCANCEL);
		if(result==IDYES)
		{
			pDoc->OnProjectSave();
		}
		else if(result==IDCANCEL)
			return;					
	}
	CGuiFrameWnd::OnClose();
}

void CMainFrame::OnDatabaseDatalocation()
{
	CSoLIMDoc *pDoc = (CSoLIMDoc *)GetActiveDocument();
	MessageBox("Data location:"+pDoc->m_strProjectDir+"\\GISData","Data Location", MB_ICONINFORMATION);
}




void CMainFrame::OnTypevalidataionPointlistvs()
{
	if(m_errorMatrixPointMapDlg.DoModal() == IDOK)
		AfxMessageBox("Creating error matrix using point list and hardened map is done.");
}

void CMainFrame::OnValidationPropertyvalidation()
{
    if(m_validationPointPropertyMapDlg.DoModal() == IDOK)
		AfxMessageBox("Creating validation result using point list and property map is done.");
}



void CMainFrame::OnPurposivesamplingFcmclustering()
{
	 m_purposiveSampleFCMDlg.DoModal();
}

void CMainFrame::OnPurposivesamplingSampledesign()
{
	 m_purposiveSampleDesignDlg.DoModal();
}


void CMainFrame::OnMappingWithSamples()
{

	
	
}



void CMainFrame::OnKnowledgeAquisitionPurposiveSamplingExpert()
{
	CKnowledgeFromPSExpertDlg dlg;
	dlg.DoModal();

}


void CMainFrame::OnKnowledgeAquisitionPurposiveSamplingNovice()
{
	CKnowledgeFromPSDlg dlg;
	dlg.DoModal();

}


void CMainFrame::OnKnowledgeAquisitionMap()
{

	char FilePath[MAX_PATH];
	GetModuleFileName(NULL,FilePath,MAX_PATH); //get the path of the .exe file
	CString Path;
	Path.Format("%s",FilePath);
	Path.Replace("SoLIMSolutions.exe","");

	CSoLIMApp* app = (CSoLIMApp *)AfxGetApp();	//pointer of the executable program
	int result=(int)(ShellExecute(m_hWnd,_T("open"),_T("SLMiner.exe"),NULL,_T(app->m_exePath),SW_SHOW));


}





void CMainFrame::OnKnowledgeAquisitionExpert()
{


    char FilePath[MAX_PATH];
	GetModuleFileName(NULL,FilePath,MAX_PATH); //get the path of the .exe file
	CString Path;
	Path.Format("%s",FilePath);
	Path.Replace("SoLIMSolutions.exe","");

	CSoLIMApp* app = (CSoLIMApp *)AfxGetApp();	//pointer of the executable program
	int result=(int)(ShellExecute(m_hWnd,_T("open"),_T("SoLIM_ExpertKnowledge.exe"),NULL,_T(app->m_exePath),SW_SHOW));
}





void CMainFrame::OnDataPreparationRemoteSensing()
{
  CRemoteSensingSettingDlg dlg;
  dlg.DoModal();
}

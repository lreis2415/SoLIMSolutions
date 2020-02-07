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
// FuzzyTerrain.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "FuzzyTerrainDlg.h"
#include ".\fuzzyterraindlg.h"


// CFuzzyTerrainDlgdialog

IMPLEMENT_DYNAMIC(CFuzzyTerrainDlg, CDialog)
CFuzzyTerrainDlg::CFuzzyTerrainDlg(CWnd* pParent /*=NULL*/)
: CDialog(CFuzzyTerrainDlg::IDD, pParent), m_RunningType(_T(""))
, m_LeftValue(0)
, m_RightValue(0)
, m_ViewType(_T("Range in Layer"))
, m_Negative(FALSE)
, m_TopValue(1)
, m_MidValue(0.5)
, m_OriginValue(0)
{
	m_ApplyTo = 0;
	m_MemberValue = 1;
	m_EFunc = "Continuous:bell-shape";
	m_PFunc = "Limiting-Factor";
	m_v1 = 0;
	m_realW1 = 0;
	m_w1 = 0;
	m_r1 = 2;
	m_v2 = 0;
	m_realW2 = 0;
	m_w2 = 0;
	m_r2 = 2;
	m_curFeature = NULL;
	m_curAttr = NULL;
	//m_oldAttr = NULL;
	m_curSpatial = NULL;
	m_curRuleList = NULL;
	
	
	m_DistDecayFactor = 0.0f;
	m_UseSpatial = true;
	m_VtoHConvertFactor = 0.0f;
	m_WgtDecayFactor = 0.0f;
	m_SearchDist = 0.0f;

	m_UseDistSimilarity = false;
	m_IntegratingAlgStr = "Maximum";
	theAHP = NULL;

	theEnvDatabase = NULL;	
	theRstKnowledge =NULL;


	m_realV1=0;
    m_realV2=0;  
    m_prevV1=0;
	m_prevV2=0;
    m_prevW1=0;
	m_prevW2=0;
	//m_isInit = false;  


	HINSTANCE hinst = AfxGetInstanceHandle();
	hiconNew = (HICON)LoadImage(hinst, MAKEINTRESOURCE(IDI_newIco), IMAGE_ICON, 14, 14, LR_SHARED);
///	hiconOpen = (HICON)LoadImage(hinst, MAKEINTRESOURCE(IDI_openIco), IMAGE_ICON, 14, 14, LR_SHARED);
//	hiconSave = (HICON)LoadImage(hinst, MAKEINTRESOURCE(IDI_Save1Ico), IMAGE_ICON, 14, 14, LR_SHARED);
	hiconDB = (HICON)LoadImage(hinst, MAKEINTRESOURCE(IDI_FolderIco), IMAGE_ICON, 14, 14, LR_SHARED);

	m_tooltip = NULL;

}

CFuzzyTerrainDlg::~CFuzzyTerrainDlg()
{
	
	m_curFeature = NULL;
	m_curAttr = NULL;
	m_curSpatial = NULL;
	m_curRuleList = NULL;	
	

	if(theAHP!= NULL){
		delete theAHP;
		theAHP = NULL;
	}

	if(theEnvDatabase!=NULL){
		delete theEnvDatabase;
     	theEnvDatabase = NULL;	
	}
	if(theRstKnowledge!=NULL){
		delete theRstKnowledge;
        theRstKnowledge =NULL;
	}
	if(m_tooltip!=NULL){
		delete m_tooltip;
      	m_tooltip = NULL;
	}	
	m_treeRoot = NULL;



	
}

void CFuzzyTerrainDlg::DoDataExchange(CDataExchange* pDX){
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RuleTre, m_theTree);
	DDX_Control(pDX, IDC_FeatureListCtrl, m_theList);
	DDX_Text(pDX, IDC_MembershipFld, m_MemberValue);
	DDX_Text(pDX, IDC_LowerValueFld, m_v1);
	DDX_Text(pDX, IDC_Width1Fld, m_w1);
	DDX_Text(pDX, IDC_r1Fld, m_r1);
	DDX_Text(pDX, IDC_UpperValueFld, m_v2);
	DDX_Text(pDX, IDC_Width2Fld, m_w2);
	DDX_Text(pDX, IDC_r2Fld, m_r2);
	//DDX_Text(pDX, IDC_XFld, m_x);
	//DDX_Text(pDX, IDC_YFld, m_y);
	DDX_Check(pDX, IDC_UseDistSimChk, m_UseDistSimilarity);
	DDX_Text(pDX, IDC_SearchDistFld, m_SearchDist);
	DDX_Text(pDX, IDC_DistDecayFactorFld, m_DistDecayFactor);
	DDX_Text(pDX, IDC_VtoHConvertFactorFld, m_VtoHConvertFactor);
	DDX_CBString(pDX, IDC_IntegratingAlgoCmb, m_IntegratingAlgStr);
	DDX_Text(pDX, IDC_WgtDecayFactorFld, m_WgtDecayFactor);
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
	DDX_Text(pDX, IDC_RunningTypeLbl, m_RunningType);
	DDX_CBString(pDX, IDC_ViewTypeCmb, m_ViewType);
	DDX_Control(pDX, IDC_ViewTypeCmb, m_viewTypeCmb);
	DDX_Radio(pDX, IDC_PositiveRdo, m_Negative);
	DDX_Text(pDX, IDC_TopValueFld, m_TopValue);
	//DDV_MinMaxFloat(pDX, m_TopValue, 0.0001, 1);
	DDX_Text(pDX, IDC_MidValueFld, m_MidValue);
	//DDV_MinMaxFloat(pDX, m_MidValue, 0.00001, 1);
	//DDX_Check(pDX, IDC_FuzzyBndChk, m_MakeFuzzyBnd);
	DDX_Text(pDX, IDC_OriginValueFld, m_OriginValue);
	DDX_Control(pDX, IDC_curveTypeCmb, m_curveTypeCmb);
	DDX_CBString(pDX, IDC_curveTypeCmb, m_CurveType);

}


BEGIN_MESSAGE_MAP(CFuzzyTerrainDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_NewBtn, OnBnClickedNewbtn)
	//ON_BN_CLICKED(IDC_SpatialSettingBtn, OnBnClickedSpatialsettingbtn)
	ON_BN_CLICKED(IDC_EditEnvListBtn, OnBnClickedEditenvlistbtn)	


	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

	ON_BN_CLICKED(IDC_ApplyBtn, OnBnClickedApplybtn)
	ON_EN_KILLFOCUS(IDC_LowerValueFld, OnEnKillfocusLowervaluefld)
	ON_EN_KILLFOCUS(IDC_UpperValueFld, OnEnKillfocusUppervaluefld)
	ON_EN_KILLFOCUS(IDC_Width1Fld, OnEnKillfocusWidth1fld)
	ON_EN_KILLFOCUS(IDC_Width2Fld, OnEnKillfocusWidth2fld)
	ON_EN_KILLFOCUS(IDC_r1Fld, OnEnKillfocusr1fld)
	ON_EN_KILLFOCUS(IDC_r2Fld, OnEnKillfocusr2fld)
	ON_EN_KILLFOCUS(IDC_SearchDistFld, OnEnKillfocusSearchdistfld)
	ON_EN_KILLFOCUS(IDC_VtoHConvertFactorFld, OnEnKillfocusVtohconvertfactorfld)
	ON_EN_KILLFOCUS(IDC_DistDecayFactorFld, OnEnKillfocusDistdecayfactorfld)
	ON_EN_KILLFOCUS(IDC_WgtDecayFactorFld, OnEnKillfocusWgtdecayfactorfld)
	ON_CBN_KILLFOCUS(IDC_IntegratingAlgoCmb, OnCbnKillfocusIntegratingalgocmb)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_LIMITINGFACTOR, OnLimitingfactor)
	ON_COMMAND(ID_WEIGHTEDAVERAGE, OnWeightedaverage)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FeatureListCtrl, OnLvnItemchangedFeaturelistctrl)

	ON_WM_CLOSE()
	ON_COMMAND(ID_Bell_MenuItem, OnBellMenuitem)
	ON_COMMAND(ID_S_MenuItem, OnSMenuitem)
	ON_COMMAND(ID_Z_MenuItem, OnZMenuitem)
	ON_COMMAND(ID_Cyclic_MenuItem, OnCyclicMenuitem)
	ON_COMMAND(ID_Ordinal_MenuItem, OnOrdinalMenuitem)
	ON_COMMAND(ID_Nominal_MenuItem, OnNominalMenuitem)
	//ON_COMMAND(ID_Default_MenuItem, OnDefaultMenuitem)
	ON_BN_CLICKED(IDC_AHPBtn, OnBnClickedAhpbtn)
	ON_COMMAND(ID__MULTIPLICATION, OnMultiplication)
	ON_COMMAND(ID__RAWVALUES, OnRawValuesMenuItem)
	

	ON_CBN_SELCHANGE(IDC_ViewTypeCmb, OnCbnSelchangeViewtypecmb)
	ON_BN_CLICKED(IDC_PositiveRdo, OnBnClickedPositiverdo)
	ON_BN_CLICKED(IDC_NegativeRdo, OnBnClickedNegativerdo)
	ON_EN_KILLFOCUS(IDC_MembershipFld, OnEnKillfocusMembershipfld)


	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_curveTypeCmb, OnCbnSelchangecurvetypecmb)

	ON_BN_CLICKED(IDC_RunInferBtn, OnBnClickedRuninferbtn)


//	ON_NOTIFY(NM_RCLICK, IDC_FeatureListCtrl, OnNMRclickFeaturelistctrl)
	ON_NOTIFY(NM_RCLICK, 0, OnNMRclickFeaturelistHeaderctrl)
	//ON_WM_DESTROY()
    //ON_WM_SETCURSOR()
    //ON_NOTIFY_EX(TTN_NEEDTEXT,0,OnTipNotify)


END_MESSAGE_MAP()


// CFuzzyTerrainDlgmessage handlers
BOOL CFuzzyTerrainDlg::OnInitDialog(){

	CDialog::OnInitDialog();	
	// TODO: Add extra initialization here
	//put icon on a button
	CButton* b = (CButton*)GetDlgItem(IDC_NewBtn);
	b->SetIcon(hiconNew);
	b = (CButton*)GetDlgItem(IDC_EditEnvListBtn);
	b->SetIcon(hiconDB);
	
	//m_isInit = true;
	setInitialForCur();
	updateFields();
	//m_isInit = false;
    setIntialCurveVariables();

	m_ProgressBar.ShowWindow(SW_HIDE);	
	enableDisableItems(); 

	EnableToolTips(true);   
	//if(m_tooltip==NULL)
	m_tooltip=new CToolTipCtrl;
	m_tooltip->Create(this ,0);   
	m_tooltip->Activate(true);
    m_tooltip->AddTool(GetDlgItem(IDC_NewBtn),"ProtoType Raster Layer"); 
    m_tooltip->AddTool(GetDlgItem(IDC_EditEnvListBtn),"Raster Layers"); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////
// KNOWLEDGE TYPE SELECTION
///////////////////////////////////////

/*
void CFuzzyTerrainDlg::OnBnClickedRstcaserdo(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(theRstKnowledge==NULL)
		clearFields();
	else{
		m_theTree.SelectItem(m_treeRoot); //force the root to be selected in order to trigger the treeitemchanged function for updating fields
		setInitialForCur();
	}
	updateFields(); //even if there are no features, use this to set the list header according to T func.
	OnBnClickedApplybtn();
	enableDisableItems();

}*/

///////////////////////////////////////
//	TOOL BUTTONS
///////////////////////////////////////
void CFuzzyTerrainDlg::OnBnClickedNewbtn(){
	// TODO: Add your control notification handler code here
	//Check the env DB
	if(theEnvDatabase == NULL || theEnvDatabase->firstLayer == NULL){
		MessageBox("Please load environmental data first.");
		return;
	}
	newRstCasebase();
	setIntialCurveVariables();
	enableDisableItems();
}


//Call for those "right-click" menus for adding new or removal
/*
void CFuzzyTerrainDlg::OnNMRclickRuletre(NMHDR *pNMHDR, LRESULT *pResult){

	// TODO: Add your control notification handler code here
	CPoint pt;
	GetCursorPos(&pt);
	m_theTree.ScreenToClient(&pt); //Must explicitly specify the client if of the tree window.  Otherwise will use the whole window as the client area.

	// Select the item that is at the point myPoint.
	UINT uFlags;
	HTREEITEM hItem = m_theTree.HitTest(pt, &uFlags);

	if ((hItem != NULL) && (TVHT_ONITEM & uFlags)){	
		m_theTree.Select(hItem, TVGN_CARET);
    	OnTvnSelchangedRuletre(pNMHDR, pResult);	
		
		m_applyMenuDlg.m_knowledgebaseType = 4;
		int level = getSelectedTreeLevel();
		if(level==0){
			m_applyMenuDlg.m_MenuCode = 8;  
			m_applyMenuDlg.DoModal();
		}
		setIntialCurveVariables();
		enableDisableItems();
			
	}
	*pResult = 0;

}*/





void CFuzzyTerrainDlg::OnBnClickedEditenvlistbtn(){
	// TODO: Add your control notification handler code here
	//If loading KB without first loading data, send the feature name list to env dlg,
	//so that the user can load data according to this list
	m_newEnvDlg.theFeatureList = NULL;
	
	if(theRstKnowledge != NULL)
		m_newEnvDlg.theFeatureList=theRstKnowledge->theAttr->theFeatureList;

	if(theEnvDatabase==NULL)
		theEnvDatabase = new EnvDatabase;
	//if(m_newEnvDlg.theEnvDatabase!=NULL)
	//	delete m_newEnvDlg.theEnvDatabase;
	//m_newEnvDlg.theEnvDatabase = new EnvDatabase(*theEnvDatabase);
	m_newEnvDlg.theEnvDatabase = theEnvDatabase;
	if(m_newEnvDlg.DoModal() == IDOK){ //update theEnvDatabase
		//delete theEnvDatabase;
		//theEnvDatabase = new EnvDatabase(*(m_newEnvDlg.theEnvDatabase));
		theEnvDatabase = m_newEnvDlg.theEnvDatabase;
		if(theRstKnowledge != NULL)
			theRstKnowledge->updateFeatures(theEnvDatabase);
	
		if(m_curAttr!=NULL){
			m_curFeature = m_curAttr->theFeatureList->first;
			fillOthers();
			fillList();
		}

		//update AHP
		if(theAHP==NULL){ //first time load
			theAHP = new List<AHP_ScaleList>;
			EnvLayer* tempLayer = theEnvDatabase->firstLayer;
			for(int i=0; i<theEnvDatabase->numOfLayers; i++){
				theAHP->addTail(new AHP_ScaleList(tempLayer->layerName, theEnvDatabase));
				tempLayer = tempLayer->nextLayer;
			}
		}
		else{             //already exists, check updates
			List<AHP_ScaleList>* tempAHP = new List<AHP_ScaleList>;
			EnvLayer* tempLayer = theEnvDatabase->firstLayer;
			for(int i=0; i<theEnvDatabase->numOfLayers; i++){
				tempAHP->addTail(new AHP_ScaleList(tempLayer->layerName, theEnvDatabase));
				tempLayer = tempLayer->nextLayer;
			}
			AHP_ScaleList* tempScaleList = tempAHP->first;
			while(tempScaleList!=NULL){
				AHP_ScaleList* tempScaleList2 = theAHP->first;
				while(tempScaleList2 != NULL){
					if(tempScaleList->name==tempScaleList2->name){
						FCNode* temp = tempScaleList->factorList->first;
						while(temp!=NULL){
							FCNode* temp2 = tempScaleList2->factorList->first;
							while(temp2!=NULL){
								if(temp->c==temp2->c){
									temp->f=temp2->f;
									break;
								}
								temp2 = temp2->next;
							}
							temp = temp->next;
						}
						break;
					}
					tempScaleList2 = tempScaleList2->next;
				}
				tempScaleList = tempScaleList->next;
			}
			delete theAHP;
			theAHP = tempAHP;
		}
		theEnvDatabase->newEnv = false; //the features have already been updated with the new env db (if it's a totally new one)
	}
	else{
		theEnvDatabase = m_newEnvDlg.theEnvDatabase;
	}

	enableDisableItems();
}



//New Raster Case
int CFuzzyTerrainDlg::newRstCasebase(void){
	int s = removeRstCasebase();
	if(s==2) //Cancel
		return s;
	static char BASED_CODE szFilter[] = "Raster Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open File That Contains Raster Case";	
	if(m_ldFile.DoModal() == IDOK){
		
		theRstKnowledge = new RasterCases(m_ldFile.GetFileName());
		theRstKnowledge->updateFeatures(theEnvDatabase);
		setInitialForCur();
		updateFields();

		return 0;
	}
	else
		return 1;
}


//Leads the user to load env data if the user tries to open an existing KB before loading data
/*int CFuzzyTerrainDlg::loadForEmptyEnvDatabase(List<FeatureNode>* theFeatureList){
	m_chkEnvFctOfKBDlg.theFeatureList = theFeatureList;
	if(m_chkEnvFctOfKBDlg.DoModal()!=IDOK)	return -1;
	else{
		OnBnClickedEditenvlistbtn();
		if(theEnvDatabase==NULL || theEnvDatabase->numOfLayers==0)	return -1;
	}
	return 0;
}*/




//Remove Raster Cases
int CFuzzyTerrainDlg::removeRstCasebase(void){
	if(theRstKnowledge==NULL) return 1;

	delete theRstKnowledge;
	clearFields();
	return 0;
}

//Set the instance/case to be positive
void CFuzzyTerrainDlg::OnBnClickedPositiverdo(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_curAttr->positive = true;
	InvalidateRect(graphicRegion, TRUE);
	InvalidateRect(graphicRegion2, TRUE);
	OnBnClickedApplybtn();
	
}

//Set the instance/case to be negative
void CFuzzyTerrainDlg::OnBnClickedNegativerdo(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_MemberValue = 0;
	UpdateData(false);
  
	m_curAttr->positive = false;
	InvalidateRect(graphicRegion, TRUE);
    InvalidateRect(graphicRegion2, TRUE);

	

	OnBnClickedApplybtn();
	
}

//Check the input membership value
void CFuzzyTerrainDlg::OnEnKillfocusMembershipfld(){
	// TODO: Add your control notification handler code here
	OnBnClickedApplybtn();
}


///////////////////////////////////////
// INFERENCE
///////////////////////////////////////
//This function is specially for handling the max/min membership value and positive/negative issues.
//Rules/cases will be pre-separated according to their positive/negative properties.
//Buffers for positive/negative knowledge will be prepared.
//Postprocessing will be done to scale the values from the inference functions by the max/min values
//and integrate the positive and negative results.
//Use this function to avoid the trouble in handling these issues in the inference functions.
//Added on Dec. 28, 2005
void CFuzzyTerrainDlg::runInference(){
	int numPixels = theEnvDatabase->theSetting->nrow*theEnvDatabase->theSetting->ncol;
	float* soilmap_p = new float[numPixels];
	float* soilmap_n = new float[numPixels];
	
	int hasPos = false;
	int hasNeg = false;

		if(theRstKnowledge == NULL){
			MessageBox("No casebase.");
			delete [] soilmap_p;
			delete [] soilmap_n;
		
			return;
		}
		if(theRstKnowledge->theAttr->positive){
			RstInference(soilmap_p, NULL);
			hasPos = true;
		}
		else{ //if negative, directly change the memberValue in theRstKnowledge. After inference, change it back.
			theRstKnowledge->theAttr->memberValue = 1-theRstKnowledge->theAttr->memberValue;
			RstInference(soilmap_n, NULL);
			hasNeg = true;
			theRstKnowledge->theAttr->memberValue = 1-theRstKnowledge->theAttr->memberValue;
		}
	


	//get the actual value for negative instance
	if(hasNeg)
		for(int i=0; i<numPixels; i++)
			soilmap_n[i] = 1-soilmap_n[i]; //e.g. if v = 0.8 and m = 0.3, then v = 1-0.8(1-0.3) = 0.44
	//Integrating positive and negative, using minimum
	if(hasPos && hasNeg)
		for(int i=0; i<numPixels; i++)
			if(soilmap_p[i]>soilmap_n[i])
				soilmap_p[i]=soilmap_n[i];
	//4. Output
	if(hasPos){
		output(soilmap_p, NULL);
		delete [] soilmap_n;
	
	}
	else if(hasNeg){
		output(soilmap_n, NULL); //no positive instances, just negative
		delete [] soilmap_p;
	
	}
}
/*
void CFuzzyTerrainDlg::RBInference(float* soilmap, float* checkmap, RuleList* theRuleList){
	

	  m_ProgressBar.SetRange(1, 100);
	  m_ProgressBar.SetPos(1);

	   m_RunningType = m_curRuleList->name+":";

	   UpdateData(FALSE);
	

	//3. Inference
	int nrow = theEnvDatabase->theSetting->nrow;
	int ncol = theEnvDatabase->theSetting->ncol;
	float cellsize = theEnvDatabase->theSetting->cellsize;
	int numPixels = nrow*ncol;
	bool* idBuf = new bool[numPixels];
	CThreeDR* maskFile = new CThreeDR;
	float nodata = theEnvDatabase->theSetting->nodata;
	int i;

	//initialize soilmap and idBuf
	CheckNodata(soilmap, idBuf);

	//check checkfile setting
	

	ValueNode** featureValueArray = new ValueNode*[theEnvDatabase->numOfLayers]; //An array for holding feature values for each test location
	for(int k = 0; k<theEnvDatabase->numOfLayers; k++)
		featureValueArray[k] = new ValueNode(0, 0);
	for(i = 0; i<numPixels; i++){
	
		  m_ProgressBar.SetPos(100*i/numPixels);

		if(!idBuf[i]) continue;
		EnvLayer* tempLayer = theEnvDatabase->firstLayer;
		for(k = 0; k<theEnvDatabase->numOfLayers; k++){
			featureValueArray[k]->value = tempLayer->theData->getDataBuf()[i];
			featureValueArray[k]->num = tempLayer->id;
			tempLayer = tempLayer->nextLayer;
		}
		Attr* tempAttr = theRuleList->first;
		while(tempAttr != NULL){ //scan all the rules in the list
			int env = 0;
			float v = 0;
			if(tempAttr->algo=="Limiting-Factor")
				v = limitingFactor(featureValueArray, tempAttr, env, nodata);
			else if(tempAttr->algo=="Weighted-Average")
				v = weightedAverage(featureValueArray, tempAttr, env, nodata);
			else if(tempAttr->algo=="Multiplication")
				v = multiplication(featureValueArray, tempAttr, env, nodata);
			//scaled by the max/min membership value. 
			v *= tempAttr->memberValue;
			//apply the mask at the instance level
		
			//set the map value
			if(v > soilmap[i]){ //use the maximum as the final one
				soilmap[i] = v;
		
			}
			tempAttr = tempAttr->next;
		}
	}
	//apply the mask at the soil type (rulelist) level
	if(theRuleList->useMask){
		for(i=0; i<numPixels; i++){
			if(!idBuf[i]) continue;
			if(theRuleList->inverseMaskValue)
				soilmap[i] *= 1-theRuleList->mask->getDataBuf()[i];
			else
				soilmap[i] *= theRuleList->mask->getDataBuf()[i];
		}
	}
	//clean up
	for(k = 0; k<theEnvDatabase->numOfLayers; k++)
		delete featureValueArray[k];
	delete [] featureValueArray;
	delete [] idBuf;
}
*/
void CFuzzyTerrainDlg::RstInference(float* soilmap, float* checkmap){

	
	   m_ProgressBar.SetRange(1, 100);
	   m_ProgressBar.SetPos(1);


	//2. if use spatial similarity, check and set the elevation data
	CThreeDR* theDEM = NULL;
	if(m_UseSpatial){
		theDEM = CheckDEM();
		if(theDEM==NULL){//no elevation data available
			MessageBox("Elevation data must be loaded and must be named as 'elevation'");
			return;
		}
	}
	//3. Inference
	int nrow = theEnvDatabase->theSetting->nrow;
	int ncol = theEnvDatabase->theSetting->ncol;
	int numPixels = nrow*ncol;
	float cellsize = theEnvDatabase->theSetting->cellsize;
	bool* idBuf = new bool[numPixels];
	bool* typicalBuf = new bool[numPixels];
	float nodata = theEnvDatabase->theSetting->nodata;
	EnvLayer* tempLayer;
	int casenum = 0;
	int i, j;
	
	//initialize soilmap and idBuf
	CheckNodata(soilmap, idBuf);

	//check checkfile setting
	if(checkmap!=NULL)
		for(i=0; i<numPixels; i++)
			checkmap[i] = -1.0;
	//check mask file setting


	//assign full membership to those typical pixels
	for(i=0; i<numPixels; i++){
		typicalBuf[i] = false;
		if(idBuf[i] && theRstKnowledge->theRaster->getDataBuf()[i]>0){ //treats all cells with values greater than 0 as cases
			soilmap[i] = theRstKnowledge->theAttr->memberValue;
			idBuf[i] = false;
		}
	}
	//Resample the typical pixels to reduce the running time
	if(theRstKnowledge->resampleRatio>1){
		unsigned char* patchIDBuf = new unsigned char[numPixels];
		for(i=0; i<numPixels; i++){
			if(idBuf[i] || fabs(theRstKnowledge->theRaster->getDataBuf()[i]-nodata)<VERYSMALL)  //if not case pixels or nodata pixels
				patchIDBuf[i] = 4;	//4: sets to be nodata so that resampling function does nothing to it
			else				
				patchIDBuf[i] = 0;	//0: not visited
		}
		for(i=0; i<numPixels; i++){
			if(patchIDBuf[i] == 0){
				Patch<float>* thePatch = new Patch<float>(i, soilmap, patchIDBuf, nrow, ncol, true);
				PatchPixel* tempPixel = thePatch->first;
				while(tempPixel != NULL){
					typicalBuf[i] = true;
					for(int k=0; k<theRstKnowledge->resampleRatio; k++){
						if(tempPixel == NULL) break;
						tempPixel = tempPixel->nextPixel;
					}
				}
				delete thePatch;
			}
		}
		delete [] patchIDBuf;
	}
	else
		for(i=0; i<numPixels; i++)
			if(fabs(soilmap[i]-m_MemberValue)<VERYSMALL)  
				typicalBuf[i] = true;

	//build a list of typcial pixels (a simulation of casebase).
	List<IntNode>* firstPixelList = new List<IntNode>;
	List<IntNode>* tempPixelList = firstPixelList;
	int totCase = 0;
	for(i=0; i<nrow; i++){
		tempPixelList->id = i;
		tempPixelList->value = theRstKnowledge->theRaster->yOfACell(i);
		for(j=0; j<ncol; j++){
			if(typicalBuf[i*ncol+j]){
				totCase++;
				tempPixelList->addTail(new IntNode(j));
			}
		}
		if(i<nrow-1){
			tempPixelList->next = new List<IntNode>;
			tempPixelList = tempPixelList->next;
		}
	}
	ValueNode** featureValueArray = new ValueNode*[theEnvDatabase->numOfLayers]; //An array for holding feature values for each test location
	for(int k = 0; k<theEnvDatabase->numOfLayers; k++)
		featureValueArray[k] = new ValueNode(0, 0);
	List<IIIDDDNode>* theSimilarityList = new List<IIIDDDNode>;
	Attr* tempAttr = new Attr(*theRstKnowledge->theAttr); //Use this copy in order to preserve the original attr value in the KB.
	int n = 0; //for progressbar
	if(m_UseSpatial){
		tempPixelList = firstPixelList;
		while(tempPixelList != NULL){
			n++;
			m_ProgressBar.SetPos(100*n/nrow);

			int startRow = tempPixelList->id-(int)(theRstKnowledge->theSpatial->searchDist/cellsize);
			if(startRow<0) startRow = 0;
			int endRow = tempPixelList->id+(int)(theRstKnowledge->theSpatial->searchDist/cellsize);
			if(endRow>nrow-1) endRow = nrow-1;
			IntNode* tempPixel = tempPixelList->first;
			while(tempPixel!=NULL){
				casenum++;
				int startCol = tempPixel->pos-(int)(theRstKnowledge->theSpatial->searchDist/cellsize);
				if(startCol<0) startCol = 0;
				int endCol = tempPixel->pos+(int)(theRstKnowledge->theSpatial->searchDist/cellsize);
				if(endCol>ncol-1) endCol = ncol-1;

				// Scan the cells in the range, and scan the cases
				//	a. calc topo distance, creat the case list, mark the id buffer
				//  b. calc feature similarities for cases in the list
				//	c. calc distanced similarity
				double startCellY = theEnvDatabase->firstLayer->theData->yOfACell(startRow);
				double cellY = startCellY;
				for(int p=startRow; p<=endRow; p++){
					int startRow2 = p-(int)(theRstKnowledge->theSpatial->searchDist/cellsize);
					if(startRow2<0) startRow2 = 0;
					int endRow2 = p+(int)(theRstKnowledge->theSpatial->searchDist/cellsize);
					if(endRow2>nrow-1) endRow2 = nrow-1;
					List<IntNode>* tempPixelList2 = firstPixelList;
					while(tempPixelList2!=NULL && tempPixelList2->id<startRow2) 
						tempPixelList2 = tempPixelList2->next;
					double startCellX = theRstKnowledge->theRaster->xOfACell(startCol);
					double cellX = startCellX;
					for(int q=startCol; q<=endCol; q++){
						int curPos = p*ncol+q;

					    double x, y;		    
                        y=theEnvDatabase->theSetting->north-(p-1)*cellsize;
		                x=(q-1)*cellsize+theEnvDatabase->theSetting->west;

						if(!idBuf[curPos]) continue;
						idBuf[curPos]=false;
						//read env values for the cell under test
						tempLayer = theEnvDatabase->firstLayer;
						for(k = 0; k<theEnvDatabase->numOfLayers; k++){
							
							if((x-cellsize*0.1)>theEnvDatabase->theSetting->east_min||(x+cellsize*0.1)<theEnvDatabase->theSetting->west_min||(y-cellsize*0.1)>theEnvDatabase->theSetting->north_min||(y-cellsize*0.1)<theEnvDatabase->theSetting->south_min)
			                {
				              featureValueArray[k]->value= nodata;			
			                }
			                else
			                {
			             	//calculate the row column number of piexel i, j in the new layer
			                    int m= ((tempLayer->theData->getNumberOfRows()*cellsize+tempLayer->theData->getYmin())-y)/cellsize+1;
			                    int n= (x-tempLayer->theData->getXmin())/cellsize+1;
			                    featureValueArray[k]->value = tempLayer->theData->getDataBuf()[(m-1)*tempLayer->theData->getNumberOfColumns()+n-1];
					        }
                            featureValueArray[k]->num = tempLayer->id;
			                tempLayer = tempLayer->nextLayer;
							//featureValueArray[k]->value = tempLayer->theData->getDataBuf()[curPos];
							//featureValueArray[k]->num = tempLayer->id;
							//tempLayer = tempLayer->nextLayer;
						}
						int startCol2 = q-(int)(theRstKnowledge->theSpatial->searchDist/cellsize);
						if(startCol2<0) startCol2 = 0;
						int endCol2 = q+(int)(theRstKnowledge->theSpatial->searchDist/cellsize);
						if(endCol2>ncol-1) endCol2 = ncol-1;
						//scan the case pixels for the cell under test
						List<IntNode>* tempPixelList3 = tempPixelList2;
						while(tempPixelList3 != NULL){
							IntNode* tempPixel3 = tempPixelList3->first;
							while(tempPixel3!=NULL){
								if(tempPixel3->pos>endCol2) break;
								while(tempPixel3!=NULL && tempPixel3->pos<startCol2) 
									tempPixel3 = tempPixel3->next;
								if(tempPixel3==NULL) break;
								double caseX = theRstKnowledge->theRaster->xOfACell(tempPixel3->pos);
								float dist = topoDistanceXY(cellX, cellY, caseX, tempPixelList3->value, theDEM, 
													theRstKnowledge->theSpatial->VtoHConvertingFactor, 
													1, theRstKnowledge->theSpatial->searchDist, 1, 1);
								if(dist>0){
									tempAttr->readLocalValuesRW(theEnvDatabase, tempPixelList3->id, tempPixel3->pos);
									int env = -1;	//for holding the id of env layer that determines the fuzzy membership value.
											//this is for creating the check file.  
											//the check file is only meaningful when using limiting-factor alg. at case level, and
											//using 1nn alg at casebase level.
									float sim = 0;
									if(tempAttr->algo=="Limiting-Factor")
										sim = limitingFactor(featureValueArray, tempAttr, env, nodata);
									else if(tempAttr->algo=="Weighted-Average")
										sim = weightedAverage(featureValueArray, tempAttr, env, nodata);
									else if(tempAttr->algo=="Multiplication")
										sim = multiplication(featureValueArray, tempAttr, env, nodata);
									if(theRstKnowledge->theSpatial->useDistSim)
										sim = distancedSimilarity(sim, dist, 
																	theRstKnowledge->theSpatial->searchDist, 
																	theRstKnowledge->theSpatial->distDecayFactor);
									sim *= tempAttr->memberValue;
									//apply the mask at the case level
							
									//Put the value in the list to compare with values from other cases
									theSimilarityList->addTail(new IIIDDDNode(0, env, theRstKnowledge->theSpatial->integratingAlgo,
																sim, dist, theRstKnowledge->theSpatial->wgtDecayFactor));
								}
								tempPixel3 = tempPixel3->next;
								if(tempPixel3 == NULL || tempPixel3->pos>endCol2) break;
							}
							tempPixelList3 = tempPixelList3->next;
							if(tempPixelList3 == NULL || tempPixelList3->id>endRow2) break;
						} //end while tempPixelList3
						soilmap[curPos] = integratingCases(theSimilarityList);
					
						theSimilarityList->clean();
						cellX += cellsize;
					} //end for q
					cellY -= cellsize;
				}//end for p
				tempPixel = tempPixel->next;
			}
			tempPixelList = tempPixelList->next;
		}
	}//end if use spatial
	else{
		for(i=0; i<numPixels; i++){
			n++;
		
		    m_ProgressBar.SetPos(100*n/numPixels);

		    double x, y;
		    if((i+1)%ncol==0)
		    {
		 	  y=theEnvDatabase->theSetting->north-((i+1)/ncol-1)*cellsize;
			  x = ncol;
		    }
		    else
		    {
              y=theEnvDatabase->theSetting->north-(i+1)/ncol*cellsize;
		      x=((i+1)%ncol-1)*cellsize+theEnvDatabase->theSetting->west;
		    }
			if(!idBuf[i]) continue;
			//read env values for the cell under test
			tempLayer = theEnvDatabase->firstLayer;
			for(k = 0; k<theEnvDatabase->numOfLayers; k++){
				if((x-cellsize*0.1)>theEnvDatabase->theSetting->east_min||(x+cellsize*0.1)<theEnvDatabase->theSetting->west_min||(y-cellsize*0.1)>theEnvDatabase->theSetting->north_min||(y-cellsize*0.1)<theEnvDatabase->theSetting->south_min)
				{
				        featureValueArray[k]->value= nodata;			
				}
				else
				{	   	//calculate the row column number of piexel i, j in the new layer
			            int m= ((tempLayer->theData->getNumberOfRows()*cellsize+tempLayer->theData->getYmin())-y)/cellsize+1;
			            int n= (x-tempLayer->theData->getXmin())/cellsize+1;
			            featureValueArray[k]->value = tempLayer->theData->getDataBuf()[(m-1)*tempLayer->theData->getNumberOfColumns()+n-1];
				}
                featureValueArray[k]->num = tempLayer->id;
			    tempLayer = tempLayer->nextLayer;
				//featureValueArray[k]->value = tempLayer->theData->getDataBuf()[i];
				//featureValueArray[k]->num = tempLayer->id;
				//tempLayer = tempLayer->nextLayer;
			}
			List<IIIDDDNode>* theSimilarityList = new List<IIIDDDNode>;
			//scan all the case pixels for the cell under test
			tempPixelList = firstPixelList;
			while(tempPixelList != NULL){
				IntNode* tempPixel = tempPixelList->first;
				while(tempPixel != NULL){
					tempAttr->readLocalValuesRW(theEnvDatabase, tempPixelList->id, tempPixel->pos);
					int env = -1;	//for holding the id of env layer that determines the fuzzy membership value.
									//this is for creating the check file.  
									//the check file is only meaningful when using limiting-factor alg. at case level, and
									//using 1nn alg at casebase level.
					float sim = 0;
					if(tempAttr->algo=="Limiting-Factor")
						sim = limitingFactor(featureValueArray, tempAttr, env, nodata);
					else if(tempAttr->algo=="Weighted-Average")
						sim = weightedAverage(featureValueArray, tempAttr, env, nodata);
					else if(tempAttr->algo=="Multiplication")
						sim = multiplication(featureValueArray, tempAttr, env, nodata);
					//scaled with partial membership
					sim *= tempAttr->memberValue;
					//applies mask at the attr level
				
					//put into the list for comparison with other pixels
					theSimilarityList->addTail(new IIIDDDNode(0, env, 3, sim, 1, 1));
					tempPixel = tempPixel->next;
				}
				tempPixelList = tempPixelList->next;
			}
			soilmap[i] = integratingCases(theSimilarityList);
		
			theSimilarityList->clean();
		}
	}
	//5. clean-up
	for(k = 0; k<theEnvDatabase->numOfLayers; k++)
		delete featureValueArray[k];
	delete [] featureValueArray;
	delete tempAttr;
	delete theSimilarityList;
	tempPixelList = firstPixelList;
	while(tempPixelList != NULL){
		firstPixelList = tempPixelList->next;
		delete tempPixelList;
		tempPixelList = firstPixelList;
	}
	delete [] idBuf;
}


/*********************************************
 Utility Functions Used by Inference Programs
 *********************************************/
//Check if the DEM layer is available for calculating search range and/or distance similarity
CThreeDR* CFuzzyTerrainDlg::CheckDEM(){
	EnvLayer* tempLayer = theEnvDatabase->firstLayer;
	while(tempLayer != NULL){
		if(tempLayer->layerName == "elevation" ||tempLayer->layerName == "Elevation" ||tempLayer->layerName == "ELEVATION")
			return tempLayer->theData;
		tempLayer = tempLayer->nextLayer;
	}
	return NULL;
}

void CFuzzyTerrainDlg::CheckNodata(float* soilmap, bool* idBuf){ //Check if any pixels should be assigned nodata
	int nrow = theEnvDatabase->theSetting->nrow;
	int ncol = theEnvDatabase->theSetting->ncol;
	int numPixels = nrow*ncol;
	int nodata = theEnvDatabase->theSetting->nodata;
	for(int i=0; i<numPixels; i++){
		bool nodataflag = false;
		EnvLayer* tempLayer = theEnvDatabase->firstLayer;
		for(int k = 0; k<theEnvDatabase->numOfLayers; k++)
			if(fabs(tempLayer->theData->getDataBuf()[i]-tempLayer->theData->getNullDataValue())<VERYSMALL){
				nodataflag = true;
				break;
			}
		if(nodataflag){
			soilmap[i] = nodata;
			idBuf[i] = false;
		}
		else{
			soilmap[i] = 0.0;
			idBuf[i] = true;
		}
	}
}


/////////////////////////////////////////////
// RUN BUTTONS
/////////////////////////////////////////////
/*void CFuzzyTerrainDlg::OnBnClickedSpatialsettingbtn(){
	// TODO: Add your control notification handler code here
	// 1 - Get the More/Less button control
	CWnd* pWndButton = this->GetDlgItem(IDC_SpatialSettingBtn);

	// 3 - Find out if we need to expand or collapse the dialog
	CString strCaption;
	pWndButton->GetWindowText(strCaption);

	if(strCaption=="Make Case Local>>"){
		m_UseSpatial = true;
		openSpatialPanel();
	}
	else if(strCaption=="<<Make Case Local"){
		m_UseSpatial = false;
		closeSpatialPanel();
	}
}*/

void CFuzzyTerrainDlg::openSpatialPanel(){
	//CWnd* pWndButton = this->GetDlgItem(IDC_SpatialSettingBtn);
	// 4 - Get current dialog window rectangle
	CRect rcDialog;
	this->GetWindowRect(&rcDialog);
	int nNewWidth = -1;

	// 5a - Change More/Less button caption
	//pWndButton->SetWindowText("<<Make Case Local");

	// 6a - Calculate new dialog height
	CWnd* pWndLarge = this->GetDlgItem(IDC_IntegratingAlgoCmb);
	CRect rcLarge;
	pWndLarge->GetWindowRect(&rcLarge);
	nNewWidth = rcLarge.right+30-rcDialog.left;

	// 7 - Set new dialog height
	ASSERT(nNewWidth>0);
	this->SetWindowPos(NULL, 0, 0, nNewWidth, rcDialog.Height(), SWP_NOMOVE|SWP_NOZORDER);
}


void CFuzzyTerrainDlg::closeSpatialPanel(){
	CWnd* pWndButton = this->GetDlgItem(IDC_SpatialSettingBtn);

	// 4 - Get current dialog window rectangle
	CRect rcDialog;
	this->GetWindowRect(&rcDialog);
	int nNewWidth = -1;

	// 5b - Change More/Less button caption
	//pWndButton->SetWindowText("Make Case Local>>");

	// 6b - Calculate new dialog height
	CWnd* pWndSmall = this->GetDlgItem(IDC_SpatialSettingBtn);
	CRect rcSmall;
	pWndSmall->GetWindowRect(&rcSmall);
	nNewWidth = rcSmall.right+12-rcDialog.left;

	// 7 - Set new dialog height
	ASSERT(nNewWidth>0);
	this->SetWindowPos(NULL, 0, 0, nNewWidth, rcDialog.Height(), SWP_NOMOVE|SWP_NOZORDER);
}



////////////////////////////////////////////////
// DEFINE THE CURVE
////////////////////////////////////////////////
void CFuzzyTerrainDlg::OnPaint(){
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages

	CClientDC wdc(this);

	//Set font (code fragment from CFont::FromHandle)
	CString vString = "";
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));          // clear out structure
	lf.lfHeight = 12;                         // request a 12-pixel-height font
	strcpy(lf.lfFaceName, "Arial");           // request a face name "Arial"
	HFONT hfont = ::CreateFontIndirect(&lf);  // create the font
	CFont* pfont = CFont::FromHandle(hfont); // Convert the HFONT to CFont*.
	CFont* def_font = wdc.SelectObject(pfont);

	CPen myPen0( PS_SOLID, 0.5, RGB(150,150,150) );
	wdc.SelectObject(&myPen0 );

	//x axix
	wdc.MoveTo(leftX, lowY);
	wdc.LineTo(rightX, lowY);
	//y axis and markers
	wdc.MoveTo(leftX, lowY);
	wdc.LineTo(leftX, highY);
	//Assisting lines
	CPen myPen3( PS_SOLID, 0.5, RGB(220,220,220) );
	wdc.SelectObject(&myPen3 );
	wdc.MoveTo(leftX+2, highY);
	wdc.LineTo(rightX, highY);
	wdc.MoveTo(leftX+2, (lowY+highY)/2);
	wdc.LineTo(rightX, (lowY+highY)/2);

	if(m_curFeature==NULL || ((m_curAttr->algo=="Limiting-Factor" || m_curAttr->algo=="Multiplication") && !m_curFeature->useOrNot)) 
		return;

	//draw curve
	CPen myPen1( PS_SOLID, 0.5, RGB(50,255,50) );
	wdc.SelectObject(&myPen1 );

	int tempLowY = lowY;
	int tempHighY = highY;
	if(m_Negative){
		tempLowY = highY;
		tempHighY = lowY;
	}
	if(m_EFunc=="Raw"){
		wdc.MoveTo(centralX, tempHighY);
		wdc.LineTo(leftX, tempHighY);
		wdc.MoveTo(centralX, tempHighY);
		wdc.LineTo(rightX, tempHighY);
		return;
	}
	if(m_EFunc=="Nominal"){
		centralX = leftX + ((m_realV1+m_realV2)/2-m_curFeature->min)/(m_curFeature->max-m_curFeature->min)*(rightX-leftX);
		leftTopX = leftX+(m_realV1-m_curFeature->min)/(m_curFeature->max-m_curFeature->min)*(rightX-leftX);
		rightTopX = leftX+(m_realV2-m_curFeature->min)/(m_curFeature->max-m_curFeature->min)*(rightX-leftX);

		if(leftTopX<leftX){
		  leftTopX = leftX;
		}
		if(leftTopX>rightX){
			leftTopX = rightX;
		}
		if(rightTopX > rightX){
		  rightTopX = rightX;
		}
		if(rightTopX <leftX){
			rightTopX = leftX;
		}
		//left
		wdc.MoveTo(leftTopX, tempHighY);
		wdc.LineTo(leftTopX, tempLowY);
		wdc.MoveTo(leftTopX, tempLowY);
		wdc.LineTo(leftX, tempLowY);
		//right
		wdc.MoveTo(rightTopX, tempHighY);
		wdc.LineTo(rightTopX, tempLowY);
		wdc.MoveTo(rightTopX, tempLowY);
		wdc.LineTo(rightX, tempLowY);

		//center bar 
		wdc.MoveTo(leftTopX, tempHighY);
		wdc.LineTo(rightTopX, tempHighY);
	}
	else if(m_EFunc=="Ordinal"){
		double unitW = (rightX-leftX)/(m_curFeature->max-m_curFeature->min);
		double unitH1 = (lowY-highY)/(m_realV1-m_curFeature->min);
		double unitH2 = (lowY-highY)/(m_curFeature->max-m_realV2);
		centralX = leftX + ((m_realV1+m_realV2)/2-m_curFeature->min)/(m_curFeature->max-m_curFeature->min)*(rightX-leftX);
		leftTopX = leftX+(m_realV1-m_curFeature->min)/(m_curFeature->max-m_curFeature->min)*(rightX-leftX);
		rightTopX = leftX+(m_realV2-m_curFeature->min)/(m_curFeature->max-m_curFeature->min)*(rightX-leftX);

		wdc.MoveTo(leftTopX, tempHighY);
		double x = leftTopX-unitW;
		double y = tempHighY;
		while(x>leftX&&x<rightX){
			wdc.LineTo(x, y);
			wdc.MoveTo(x, y);
			if(!m_Negative){
				y += unitH1;
				if(y>lowY) break;
			}
			else{
				y -= unitH1;
				if(y<highY) break;
			}
			wdc.LineTo(x, y);
			x -= unitW;
		}
		
		//right
		wdc.MoveTo(rightTopX, tempHighY);
		x = rightTopX+unitW;
		y = tempHighY;
		//wdc.MoveTo(x, y);
		while(x<rightX&&x>leftX){
			if(!m_Negative){
				y += unitH2;
				if(y>lowY) break;
			}
			else{
				y -= unitH2;
				if(y<highY) break;
			}
			wdc.LineTo(x, y);
			wdc.MoveTo(x, y);
			x += unitW;
			wdc.LineTo(x, y);
		}


		//center bar
		if(leftTopX<leftX){
		  leftTopX = leftX;
		}
		if(leftTopX>rightX){
			leftTopX = rightX;
		}
		if(rightTopX > rightX){
		  rightTopX = rightX;
		}
		if(rightTopX< leftX){
			rightTopX = leftX;
		}

		
		wdc.MoveTo(leftTopX, tempHighY);
		//x = leftTopX + unitW*(m_realV2-m_realV1);
		wdc.LineTo(rightTopX, tempHighY);
		
	}
	else{	//"Continue" and "Cyclic" Functions:

		///////////"m_knowledgebaseType != 4" was removed by DuFei, I don't know why it was put here 
		if(/*m_knowledgebaseType != 4 &&*/m_ViewType!="Effective Range" || 	 m_EFunc == "Continuous:bell-shape" || m_EFunc == "Cyclic"){ 
			leftTopX = centralX - (m_realV2-m_realV1)/unitX/2;
			rightTopX = centralX + (m_realV2-m_realV1)/unitX/2;
			leftCrossX = leftTopX - m_realW1/unitX;
			rightCrossX = rightTopX + m_realW2/unitX;
		}
		else{ //raster cases, or s/z under relative, no lower and upper values
			leftTopX = centralX;
			rightTopX = centralX;
			leftCrossX = leftTopX - (leftTopX-leftX)/pow(13.28771239, (double)1/m_r1);
			rightCrossX = rightTopX + (rightX-rightTopX)/pow(13.28771239, (double)1/m_r2);
		}

		//Side part
		//left
		int x = leftTopX;
		int y = tempHighY;
		while(x>leftX){
			wdc.MoveTo(x, y);
			x -= step;
			if(m_ViewType=="Effective Range" && m_EFunc == "Continuous:s-shape"){
				if(!m_Negative)
					y = tempLowY - exp(pow(((float)(leftTopX-x))/(leftTopX-leftCrossX), m_r1)*(-0.693))/unitY;
				else
					y = tempLowY + exp(pow(((float)(leftTopX-x))/(leftTopX-leftCrossX), m_r1)*(-0.693))/unitY;
			}
			else{
				if(!m_Negative)
					y = tempLowY - exp(pow((double)(unitX*(leftTopX-x)/m_realW1),(double)m_r1)*(-0.693))/unitY;
				else
					y = tempLowY + exp(pow((double)(unitX*(leftTopX-x)/m_realW1), (double)m_r1)*(-0.693))/unitY;
			}
			if(x>leftX&&x<rightX)
			  wdc.LineTo(x, y);
			
		}
		//right
		x = rightTopX;
		y = tempHighY;
		while(x<rightX){
			wdc.MoveTo(x, y);
			x += step;
			if(m_ViewType=="Effective Range" && m_EFunc == "Continuous:z-shape"){
				if(!m_Negative)
					y = tempLowY - exp(pow(((float)(x-rightTopX))/(rightCrossX-rightTopX), m_r2)*(-0.693))/unitY;
				else
					y = tempLowY + exp(pow(((float)(x-rightTopX))/(rightCrossX-rightTopX), m_r2)*(-0.693))/unitY;
			}
			else{
				if(!m_Negative)
					y = tempLowY - exp(pow((double)(unitX*(x-rightTopX)/m_realW2), (double)m_r2)*(-0.693))/unitY;
				else
					y = tempLowY + exp(pow((double)(unitX*(x-rightTopX)/m_realW2), (double)m_r2)*(-0.693))/unitY;
			}
			if(x>leftX&&x<rightX)
				wdc.LineTo(x, y);
		}

		double x1 = m_realW1*pow(13.28771239, (double)1/m_r1); //y = w*pow(ln(y)/ln(c), 1/r), y = 0.0001, c = 0.5
		double x2 = m_realW2*pow(13.28771239, (double)1/m_r2);
		m_LeftValue = effectiveDigits(m_realV1-x1,4);
		m_RightValue = effectiveDigits(m_realV2+x2, 4);

        
		//left
		int valueY = lowY + 5;
		CRect vHandle;
		CRect crossHandle;
		CPen myPen2( PS_SOLID, 0.1, RGB(255,50,50) );

		if(m_EFunc != "Continuous:z-shape"){
			//lines from value points to x-axis
			//left top
			wdc.SelectObject(&myPen3);
			if(leftTopX>leftX&&leftTopX<rightX){
				wdc.MoveTo(leftTopX, highY);
				wdc.LineTo(leftTopX, lowY-1);
			}
			//left lower value line
			if( m_ViewType!="Effective Range" &&m_EFunc == "Continuous:bell-shape" || m_EFunc == "Cyclic"){
				vHandle.left = centralX - (x1+(m_realV2-m_realV1)/2)/unitX -3;
				vHandle.right = vHandle.left+6;
			}
			else if(m_EFunc == "Continuous:s-shape"){
				vHandle.left = leftX-3;
				vHandle.right = vHandle.left+6;
			}
			if(vHandle.left>leftX&&vHandle.right<rightX){
				wdc.MoveTo(vHandle.left+3, tempLowY);
				wdc.LineTo(vHandle.left+3, lowY);
			}
			//left cross line
			wdc.SelectObject(&myPen0);
			if(leftCrossX>leftX&&leftCrossX<rightX&&leftTopX>leftX&&leftTopX<rightX){
				wdc.MoveTo(leftCrossX, crossY);
				wdc.LineTo(leftTopX, crossY);
			}
			else if(leftTopX>leftX&&leftTopX<rightX){
				wdc.MoveTo(leftX, crossY);
				wdc.LineTo(leftTopX, crossY);
			}
			//Upper value handles
			wdc.SelectObject(&myPen2);
			vHandle.bottom = tempHighY+3;
			vHandle.top = tempHighY-3;
			vHandle.left = leftTopX-3;
			vHandle.right = leftTopX+3;
			if(vHandle.left>leftX&&vHandle.right<rightX)
				wdc.RoundRect(&vHandle, CPoint(5, 5));
			//Cross value handles
			crossHandle.bottom = crossY+3;
			crossHandle.top = crossY-3;
			crossHandle.left = leftCrossX-3;
			crossHandle.right = leftCrossX+3;
			if(crossHandle.left>leftX&&crossHandle.right<rightX)
				wdc.Rectangle(&crossHandle);
			//Cross value
			_gcvt(m_realV1-m_realW1, 4, vString.GetBuffer(100));
			vString.ReleaseBuffer(-1);
			if(crossHandle.left>leftX&&crossHandle.right<rightX)
				wdc.TextOut(leftCrossX-25, crossY-15, vString);
			//Lower value handles
			vHandle.bottom = tempLowY+3;
			vHandle.top = tempLowY-3;
			if(m_ViewType!="Effective Range" ||m_EFunc == "Continuous:bell-shape" || m_EFunc == "Cyclic"){
				vHandle.left = centralX - (x1+(m_realV2-m_realV1)/2)/unitX -3;
				vHandle.right = vHandle.left+6;
			}
			else if(m_EFunc == "Continuous:s-shape"){
				vHandle.left = leftX-3;
				vHandle.right = vHandle.left+6;
			}
			if(vHandle.left>leftX-4&&vHandle.right<rightX+4)
				wdc.RoundRect(&vHandle, CPoint(5, 5));
			//Lower value
			_gcvt(m_LeftValue, 4, vString.GetBuffer(100));
			vString.ReleaseBuffer(-1);
			if(vHandle.left>leftX-4&&vHandle.right<rightX+4)
				wdc.TextOut(vHandle.left-3, valueY, vString);
		}
		//right
		if(m_EFunc != "Continuous:s-shape"){
			//lines from value points to x-axis
			//right top
			wdc.SelectObject(&myPen3);
			if(rightTopX<rightX&&rightTopX>leftX){
				wdc.MoveTo(rightTopX, highY);
				wdc.LineTo(rightTopX, lowY-1);
			}
			//right low value line
			if(m_ViewType!="Effective Range" || m_EFunc == "Continuous:bell-shape"  || m_EFunc == "Cyclic"){
				vHandle.left = centralX + (x2+(m_realV2-m_realV1)/2)/unitX -3;
				vHandle.right = vHandle.left+6;
			}
			else if(m_EFunc == "Continuous:z-shape"){
				vHandle.right = rightX+3;
				vHandle.left = vHandle.right-6;
			}
			if(vHandle.right<rightX&&vHandle.left>leftX){
				wdc.MoveTo(vHandle.right-3, tempLowY);
				wdc.LineTo(vHandle.right-3, lowY);
			}
			//right cross line
			wdc.SelectObject(&myPen0 );
			if(rightCrossX<rightX&&rightCrossX>leftX&&rightTopX<rightX&&rightTopX>leftX){
				wdc.MoveTo(rightCrossX, crossY);
				wdc.LineTo(rightTopX, crossY);
			}
			else if(rightTopX<rightX&&rightTopX>leftX){
				wdc.MoveTo(rightX, crossY);
				wdc.LineTo(rightTopX, crossY);
			}
			//Upper value handle
			wdc.SelectObject(&myPen2);
			vHandle.bottom = tempHighY+3;
			vHandle.top = tempHighY-3;
			vHandle.left = rightTopX-3;
			vHandle.right = rightTopX+3;
			if(vHandle.right<rightX&&vHandle.left>leftX)
				wdc.RoundRect(&vHandle, CPoint(5, 5));
			//Cross value handles
			crossHandle.bottom = crossY+3;
			crossHandle.top = crossY-3;
			crossHandle.left = rightCrossX-3;
			crossHandle.right = rightCrossX+3;
			if(crossHandle.right<rightX&&crossHandle.left>leftX)
				wdc.Rectangle(&crossHandle);
			//Cross value
			_gcvt(m_realV2+m_w2, 4, vString.GetBuffer(100));
			vString.ReleaseBuffer(-1);
			if(crossHandle.right<rightX&&crossHandle.left>leftX)
				wdc.TextOut(rightCrossX+5, crossY-15, vString);
			//Lower value handle
			vHandle.bottom = tempLowY+3;
			vHandle.top = tempLowY-3;
			if(m_ViewType!="Effective Range" || m_EFunc == "Continuous:bell-shape"  || m_EFunc == "Cyclic"){
				vHandle.left = centralX + (x2+(m_realV2-m_realV1)/2)/unitX -3;
				vHandle.right = vHandle.left+6;
			}
			else if(m_EFunc == "Continuous:z-shape"){
				vHandle.right = rightX+3;
				vHandle.left = vHandle.right-6;
			}
			if(vHandle.right<rightX+4&&vHandle.left>leftX-4)
				wdc.RoundRect(&vHandle, CPoint(5, 5));
			//lower value
			_gcvt(m_RightValue, 4, vString.GetBuffer(100));
			vString.ReleaseBuffer(-1);
			if(vHandle.right<rightX+4&&vHandle.left>leftX-4)
				wdc.TextOut(vHandle.left-6, valueY, vString);
		}
		//Std Dev Markers
		if(m_ViewType=="2 Std.Dev.from Mean"){
			wdc.SelectObject(&myPen0);
			wdc.MoveTo((centralX+leftX)/2, lowY);
			wdc.LineTo((centralX+leftX)/2, lowY-10);
			wdc.MoveTo(centralX, lowY);
			wdc.LineTo(centralX, lowY-10);
			wdc.MoveTo((centralX+rightX)/2, lowY);
			wdc.LineTo((centralX+rightX)/2, lowY-10);
			wdc.MoveTo(rightX, lowY);
			wdc.LineTo(rightX, lowY-10);
		}
	}


	//Top level part
	wdc.SelectObject(&myPen1 );
	if(m_EFunc != "Ordinal"&&m_EFunc!="Nominal"&&leftTopX>leftX&&leftTopX<rightX&&rightTopX>leftX&&rightTopX<rightX){//Ordinal already has the top bar drawn 
		wdc.MoveTo(leftTopX+3, tempHighY);
		wdc.LineTo(rightTopX-3, tempHighY);
	}
	//Range value
	if(m_EFunc == "Nominal" || m_EFunc == "Ordinal" || m_ViewType=="Range in Layer"){
		_gcvt(m_curFeature->min, 4, vString.GetBuffer(100));
		vString.ReleaseBuffer(-1);
		wdc.TextOut(leftX-20, lowY+4, vString);
		_gcvt(m_curFeature->max, 4, vString.GetBuffer(100));
		vString.ReleaseBuffer(-1);
		wdc.TextOut(rightX-20, lowY+4, vString);
	}
	wdc.SelectObject(def_font);
	::DeleteObject(hfont); // Done with the font. Delete the font object.
}



void CFuzzyTerrainDlg::OnBnClickedApplybtn(){
	// TODO: Add your control notification handler code here
	float oldMemberValue = m_MemberValue;
	int prevR1 = m_r1;
	int prevR2 = m_r2;

	UpdateData(TRUE);
	if(m_MemberValue<0 || m_MemberValue>1){
	
		m_MemberValue = oldMemberValue;
		UpdateData(FALSE);	
		MessageBox("Membership value must be between 0 and 1.");
	
	}
	
	if(m_Negative&&oldMemberValue==1)
	{
		m_MemberValue = oldMemberValue;
		UpdateData(FALSE);	
		MessageBox("The minimum membership value for a negative instance must be smaller than 1 here.");
	}

		

	if(m_v1>m_v2&&m_EFunc!="Continuous:s-shape"&&m_EFunc!="Continuous:z-shape"){	    
		m_v1=effectiveDigits(m_prevV1, 4);
		m_v2=effectiveDigits(m_prevV2, 4);
		UpdateData(FALSE);
		MessageBox("v1 cannot be greater than v2.");
		
	}
	if(m_w1<0){
	
		m_w1=effectiveDigits(m_prevW1, 4);
		UpdateData(FALSE);
		MessageBox("w1 must be equal to or greater than 0");
	
	}
	if(m_w2<0){
		m_w2=effectiveDigits(m_prevW2, 4);
		UpdateData(FALSE);
		MessageBox("w2 must be equal to or greater than 0");
		
	}
	if(m_r1<=0){
		m_r1=prevR1;
		UpdateData(FALSE);
		MessageBox("r1 must be greater than 0");
		
	}

	if(m_r2<=0){
		m_r2=prevR2;
		UpdateData(FALSE);
		MessageBox("r2 must be greater than 0");
		
	}



	if(!m_Negative){
		m_TopValue = effectiveDigits(m_MemberValue, 2);
		m_MidValue = effectiveDigits(m_MemberValue/2, 2);
		m_OriginValue = 0;
	}
	else{
		m_TopValue = 1;
		m_MidValue = effectiveDigits((1+m_MemberValue)/2, 2);
		m_OriginValue = m_MemberValue;
	}
	UpdateData(FALSE);


	if(m_prevV1!=m_v1)
	{	
		m_realV1 = m_v1;
	}
	if(m_prevV2!=m_v2)
	{
		m_realV2 = m_v2;
	}
	if(m_prevW1!=m_w1)
	{
		m_realW1 = m_w1;
	}
	if(m_prevW2!=m_w2)
	{
		m_realW2 = m_w2;
	}


	InvalidateRect(graphicRegion, TRUE);
	InvalidateRect(graphicRegion2, TRUE);
	enableDisableItems();
	if(m_curFeature==NULL || ((m_curAttr->algo=="Limiting-Factor" || m_curAttr->algo=="Multiplication") && !m_curFeature->useOrNot)){
		return;
	}

	setUnitX();
	if(m_EFunc == "Continuous:s-shape"){

		m_realV2=m_realV1;
		m_v1=effectiveDigits(m_realV1, 4);
		m_prevV1=m_v1;
		m_v2=effectiveDigits(m_realV2, 4);
		m_prevV2=m_v2;


	}
	else if(m_EFunc == "Continuous:z-shape"){
		m_realV1=m_realV2;
		m_v1=effectiveDigits(m_realV1, 4);
		m_prevV1=m_v1;
		m_v2=effectiveDigits(m_realV2, 4);
		m_prevV2 = m_v2;
	}
	else if(m_EFunc == "Raw"){
		/*m_realV1 = m_curFeature->min;
		m_realV2 = m_curFeature->max;
		m_v1=effectiveDigits(m_realV1, 4);
		m_prevV1=m_v1;
		m_v2=effectiveDigits(m_realV2, 4);
		m_prevV2=m_v2;*/

	}

	if(m_EFunc=="Continuous:bell-shape")
       m_CurveType = "Bell-shape";
	else if(m_EFunc=="Continuous:s-shape")
		m_CurveType = "S-shape";
	else if(m_EFunc=="Continuous:z-shape")
        m_CurveType = "Z-shape";
	else if(m_EFunc=="Cyclic")
		m_CurveType = "Cyclic";
	else if(m_EFunc== "Ordinal")
		m_CurveType = "Ordinal";
	else if(m_EFunc == "Nominal")
		m_CurveType = "Nominal";
	//else if(m_EFunc == "Default")
	//	m_CurveType = "Default";
	else if(m_EFunc == "Raw")
		m_CurveType = "Raw Values";


	UpdateData(FALSE);
	OnPaint();
	updateKnowledge();
}

void CFuzzyTerrainDlg::OnLButtonDown(UINT nFlags, CPoint point){
	// TODO: Add your message handler code here and/or call default
	if(m_EFunc=="Nominal" || m_EFunc=="Ordinal" || m_EFunc=="Raw") return;
	if(lowY-point.y-0.5/unitY<6){ //if the cursor is close enough to the cross-point handle
		int x = -1;
		if(m_ViewType=="Effective Range" && m_EFunc == "Continuous:s-shape")
			x = leftCrossX;
		else if(m_ViewType=="Effective Range" && m_EFunc == "Continuous:z-shape")
			x = rightCrossX;
		else if(point.x<leftTopX)
			x = leftTopX - m_realW1/unitX;
		else if(point.x>rightTopX)
			x = rightTopX + m_realW2/unitX;
		if(abs(x-point.x)<6){
			m_LBtnDownW = true;
			startPnt.x = x;
			previousCursorX = startPnt.x;
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CFuzzyTerrainDlg::OnLButtonUp(UINT nFlags, CPoint point){
	// TODO: Add your message handler code here and/or call default
	m_LBtnDownW = false;

	updateKnowledge();
	CDialog::OnLButtonUp(nFlags, point);
}

void CFuzzyTerrainDlg::OnMouseMove(UINT nFlags, CPoint point){
	// TODO: Add your message handler code here and/or call default
	if(point.x<leftX-15 || point.x>rightX+15 || point.y>lowY || point.y<highY-6){
		m_LBtnDownW = false;

	}
	if(m_LBtnDownW && abs(previousCursorX-point.x)>0){
		InvalidateRect(graphicRegion, TRUE);
		InvalidateRect(graphicRegion2, TRUE);
		setUnitX();
		if(startPnt.x<leftTopX && point.x>leftX-15 && point.x<leftTopX){
			double tempW = (leftTopX-point.x)*unitX;
			if(m_EFunc == "Continuous:s-shape" || tempW*pow(13.28771239, (double)1/m_r1)+(m_realV2-m_realV1)/2>(centralX-leftX)*unitX){ //exceeds the visible area
				m_realW1 += m_curFeature->std*0.01*abs(previousCursorX-point.x)/(previousCursorX-point.x);
			}
			else{
				if((previousCursorX-point.x>0 && m_realW1>tempW) || (previousCursorX-point.x<0 && m_realW1<tempW)) //avoid jump
					m_realW1 += m_curFeature->std*0.01*abs(previousCursorX-point.x)/(previousCursorX-point.x);
				else
					m_realW1 = tempW;
			}
			if(m_realW1<VERYSMALL) m_realW1 = VERYSMALL;
			m_w1 = effectiveDigits(m_realW1, 4);
			m_prevW1=m_w1;
			UpdateData(FALSE);
			previousCursorX = point.x; //for determining if the width value should increase or decrease
		}
		else if(startPnt.x>rightTopX && point.x>rightTopX && point.x<rightX+15){
			double tempW = (point.x-rightTopX)*unitX;
			if(m_EFunc == "Continuous:z-shape" || tempW*pow(13.28771239, (double)1/m_r2)+(m_realV2-m_realV1)/2>(rightX - centralX)*unitX){ //exceeds the visible area
				m_realW2 += m_curFeature->std*0.01*abs(point.x-previousCursorX)/(point.x-previousCursorX);
			}
			else{
				if((point.x-previousCursorX>0 && m_realW2>tempW)||(point.x-previousCursorX<0 && m_realW2<tempW))
					m_realW2 += m_curFeature->std*0.01*abs(point.x-previousCursorX)/(point.x-previousCursorX);
				else
					m_realW2 = tempW;
			}
			if(m_realW2<VERYSMALL) m_realW2 = VERYSMALL;
			m_w2 = effectiveDigits(m_realW2, 4);
			m_prevW2=m_w2;
			UpdateData(FALSE);
			previousCursorX = point.x; //for determining if the width value should increase or decrease
		}
	}

	CDialog::OnMouseMove(nFlags, point); //will do OnPaint() anyway
}

void CFuzzyTerrainDlg::OnCbnSelchangeViewtypecmb(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int s = m_viewTypeCmb.GetCurSel();
	m_viewTypeCmb.GetLBText(s, m_ViewType);
	UpdateData(FALSE);

	setUnitX();
	InvalidateRect(graphicRegion, TRUE);
	InvalidateRect(graphicRegion2, TRUE);
	OnPaint();
}

void CFuzzyTerrainDlg::setUnitX(){
	UpdateData(TRUE);
	centralX = (leftX + rightX)/2;
	m_ViewType.Trim();
	if(m_ViewType=="Effective Range" && (m_EFunc == "Continuous:bell-shape" /*|| m_EFunc == "Default"*/ || m_EFunc == "Cyclic")){
		double x1 = m_realW1*pow(13.28771239, (double)1/m_r1); //y = w*pow(ln(y)/ln(c), 1/r), y = 0.01, c = 0.5
		double x2 = m_realW2*pow(13.28771239, (double)1/m_r2);
		if(x1>x2)
			unitX = (x1+(m_realV2-m_realV1)/2)/(centralX-leftX);
		else
			unitX = (x2+(m_realV2-m_realV1)/2)/(rightX-centralX);
	}
	else if(m_ViewType=="2 Std.Dev.from Mean"){
		unitX = m_curFeature->std*4/(rightX-leftX);
	}
	else if(m_ViewType=="Range in Layer"){
		unitX = (m_curFeature->max-m_curFeature->min)/(rightX-leftX);
	}
}

void CFuzzyTerrainDlg::setCurve(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//GetDlgItem(IDC_Width1Fld)->EnableWindow(TRUE);
	//GetDlgItem(IDC_r1Fld)->EnableWindow(TRUE);
	//GetDlgItem(IDC_Width2Fld)->EnableWindow(TRUE);
	//GetDlgItem(IDC_r2Fld)->EnableWindow(TRUE);


	if(m_w1==0 || m_w1==999999){
		m_realW1 = WIDTHFACTOR*m_curFeature->std;
		m_w1 = effectiveDigits(m_realW1, 4);
		m_prevW1=m_w1;
	
	}
	if(m_w2==0 || m_w2==999999){
		m_realW2 = WIDTHFACTOR*m_curFeature->std;
		m_w2 = effectiveDigits(m_realW2, 4);
		m_prevW2=m_w2;
	}
	if(m_r1==0)	m_r1 = 2;
	if(m_r2==0)	m_r2 = 2;
	/*if(m_EFunc == "Continuous:bell-shape"){
		if(m_realW1<m_realW2){
			m_realW2 = m_realW1;
			m_w2 = m_w1;
			m_prevW2=m_w2;
			m_r2 = m_r1;
		}
		else{
			m_realW1 = m_realW2;
			m_w1 = m_w2;
			m_prevW1=m_w1;
			m_r1 = m_r2;
		}
	}*/
	if(m_EFunc == "Continuous:s-shape"){
		//m_realW1 = WIDTHFACTOR*m_curFeature->std;
		m_w1 = effectiveDigits(m_realW1, 4);
		m_prevW1=m_w1;
		m_realW2 = 999999;
		m_w2 = 999999;
		m_prevW2=m_w2;
		m_r1 = 2;
		m_r2 = 2;
	}
	else if(m_EFunc == "Continuous:z-shape"){
		m_realW1 = 999999;
		m_w1 = 999999;
		m_prevW1=m_w1;
		//m_realW2 = WIDTHFACTOR*m_curFeature->std;
		m_w2 = effectiveDigits(m_realW2, 4);
		m_prevW2=m_w2;
		m_r1 = 2;
		m_r2 = 2;
	}
	/*else if(m_EFunc == "Cyclic"){
		if(m_w1<m_w2){
			m_realW2 = m_realW1;
			m_w2 = m_w1;
			m_prevW2=m_w2;
			m_r2 = m_r1;
		}
		else{
			m_realW1 = m_realW2;
			m_w1 = m_w2;
			m_prevW1=m_w1;
			m_r1 = m_r2;
		}
	}*/
	else if(m_EFunc == "Nominal" || m_EFunc == "Ordinal"){
		m_realW1 = 0;
		m_w1 = 0;
		m_prevW1=m_w1;
		m_realW2 = 0;
		m_w2 = 0;
		m_prevW2=m_w2;
		m_r1 = 2;
		m_r2 = 2;
		/*GetDlgItem(IDC_Width1Fld)->EnableWindow(FALSE);
		GetDlgItem(IDC_r1Fld)->EnableWindow(FALSE);
		GetDlgItem(IDC_Width2Fld)->EnableWindow(FALSE);
		GetDlgItem(IDC_r2Fld)->EnableWindow(FALSE);*/
	}
	/*else if(m_EFunc == "Default"){
		
		
		m_realV1=m_curFeature->value1;
		m_v1 = effectiveDigits(m_curFeature->value1, 4);
		m_prevV1= m_v1;
		m_realV2=m_realV1;
		m_v2 = m_v1;
		m_prevV2 = m_v2;
		m_realW1 = WIDTHFACTOR*m_curFeature->std;
		m_w1 = effectiveDigits(m_realW1, 4);
		m_prevW1=m_w1;
		m_realW2 = WIDTHFACTOR*m_curFeature->std;
		m_w2 = effectiveDigits(m_realW2, 4);
		m_prevW2=m_w2;
		m_r1 = 2;
		m_r2 = 2;
	}*/
	else if(m_EFunc == "Raw"){
		m_realW1 = 999999;
		m_w1 = 999999;
		m_prevW1=m_w1;
		m_realW2 = 999999;
		m_w2 = 999999;
		m_prevW2=m_w2;
		m_r1 = 2;
		m_r2 = 2;
	}
	UpdateData(FALSE);
	OnBnClickedApplybtn();
}

void CFuzzyTerrainDlg::OnBellMenuitem(){
	// TODO: Add your command handler code here
	m_EFunc = "Continuous:bell-shape";
	setCurve();
}

void CFuzzyTerrainDlg::OnSMenuitem(){
	// TODO: Add your command handler code here
	m_EFunc = "Continuous:s-shape";
	setCurve();
}

void CFuzzyTerrainDlg::OnZMenuitem(){
	// TODO: Add your command handler code here
	m_EFunc = "Continuous:z-shape";
	setCurve();
}

void CFuzzyTerrainDlg::OnCyclicMenuitem(){
	// TODO: Add your command handler code here
	m_EFunc = "Cyclic";
	setCurve();
}

void CFuzzyTerrainDlg::OnOrdinalMenuitem(){
	// TODO: Add your command handler code here
	m_EFunc = "Ordinal";
	setCurve();
}

void CFuzzyTerrainDlg::OnNominalMenuitem(){
	// TODO: Add your command handler code here
	m_EFunc = "Nominal";
	setCurve();
}

/*void CFuzzyTerrainDlg::OnDefaultMenuitem(){
	// TODO: Add your command handler code here
	m_EFunc = "Default";
	setCurve();
}*/

void CFuzzyTerrainDlg::OnRawValuesMenuItem(){
	// TODO: Add your command handler code here
	m_EFunc = "Raw";
	setCurve();
}

void CFuzzyTerrainDlg::setIntialCurveVariables(){
	CRect rectR1;
	GetDlgItem(IDC_r1Fld)->GetWindowRect(&rectR1);
	ScreenToClient(&rectR1);
	CRect rectR2;
	GetDlgItem(IDC_r2Fld)->GetWindowRect(&rectR2);
	ScreenToClient(&rectR2);
	CRect rectV;
	GetDlgItem(IDC_LowerValueFld)->GetWindowRect(&rectV);
	ScreenToClient(&rectV);

	leftX = rectR1.right+30;
	rightX = rectR2.left-10;
	centralX = (rectR1.right+rectR2.left)/2;
	lowY = rectR1.bottom+20;
	highY = rectV.top;
	crossY = (lowY+highY)/2;
	unitY = 1.0/(lowY-highY);
	startPnt.y = lowY-0.5/unitY; //always the cross-point
	graphicRegion.top = highY-10;
	graphicRegion.bottom = lowY+20;
	graphicRegion.left = leftX-20;
	graphicRegion.right = rightX+20;

    ///////////this part is modified by DuFei
	//this->GetWindowRect(&graphicRegion2);
	//ScreenToClient(&graphicRegion2);
	//graphicRegion2.top = lowY-20;
	//graphicRegion2.bottom = lowY+20;
	graphicRegion2.top=lowY-20;
	graphicRegion2.bottom=lowY+20;
	graphicRegion2.left= leftX-40;
	graphicRegion2.right=rightX+40;
	////////////////////////////////////////

	step = 2;
	m_LBtnDownW = false;
	//m_LBtnDownV = false;
			
	if(m_curFeature!=NULL){
		m_realV1=m_curFeature->value1;
		m_v1 = effectiveDigits(m_curFeature->value1, 4);
		m_prevV1 = m_v1;
		m_realV2=m_curFeature->value2;
		m_v2 = effectiveDigits(m_curFeature->value2, 4);
		m_prevV2 = m_v2;
		m_realW1 = m_curFeature->width1;
		m_w1 = effectiveDigits(m_realW1, 4);  //Changed by DuFei
		m_prevW1=m_w1;
		m_r1 = m_curFeature->r1;
		if(m_r1<=0)
			m_r1=2;
		m_realW2 = m_curFeature->width2;
		m_w2 = effectiveDigits(m_realW2, 4);  //Changed by DuFei
		m_prevW2=m_w2;
		m_r2 = m_curFeature->r2;
		if(m_r2<=0)
			m_r2=2;
	}
	UpdateData(FALSE);
	OnBnClickedApplybtn();
}

////////////////////////////////////////////////
// INTERFACE. Update fields and knowledge
////////////////////////////////////////////////



void CFuzzyTerrainDlg::OnLvnItemchangedFeaturelistctrl(NMHDR *pNMHDR, LRESULT *pResult){

	
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);	

	// TODO: Add your control notification handler code here
	//This function is for updating the current feature before moving to another feature selected by the user
 	if(m_curAttr == NULL || m_curFeature==NULL) return;
	//if(m_oldAttr != m_curAttr) return; //don't do anything here if moves to another attr
	if(m_theList.GetItemText(m_theList.GetItemCount()-1, 1)=="") return;	


	POSITION pos = m_theList.GetFirstSelectedItemPosition(); 
	if(pos == NULL) return; 
	int nItem = m_theList.GetNextSelectedItem(pos); 
	//When "Weighted-Average", don't do anything if no feature is selected
	//or if the user does not select a different feature	
	if(m_curAttr->algo == "Weighted-Average" && m_curFeature->name == m_theList.GetItemText(nItem, 1)){
		return;
	}
	else{
		int n = 0;
		FeatureNode* tempFeature = m_curAttr->theFeatureList->first;
		while(tempFeature!=NULL){
			CString itemName = m_theList.GetItemText(n, 1);
			if(itemName==tempFeature->name){
				if(m_curAttr->algo=="Limiting-Factor" || m_curAttr->algo=="Multiplication"){
					if(m_theList.GetCheck(n)==FALSE)
						tempFeature->useOrNot = false;
					else
						tempFeature->useOrNot = true;
				}
				else if(m_curAttr->algo == "Weighted-Average")
					tempFeature->weight = atof(m_theList.GetItemText(n, 0).GetBuffer(100));
			}
			n++;
			tempFeature = tempFeature->next;
		}
		updateKnowledge(); //update the current feature before move to another one
		//Sets the selected feature to be the m_curFeature
		m_curFeature = m_curAttr->theFeatureList->first;
		pos = m_theList.GetFirstSelectedItemPosition(); 
		if (pos != NULL){
			CString name = m_theList.GetItemText(nItem, 1);
			while(m_curFeature != NULL){
				if(m_curFeature->name==name)
					break;
				m_curFeature = m_curFeature->next;
			}
		}
		//Need to do the following, because this function will be called by fillList, which in turn will be called by
		//OnNewEnv.  If the envLayers are totally replaced by new ones, the above code won't find any selected feature,
		//and the m_curFeature will be NULL, and will cause problem.
		if(m_curFeature==NULL) m_curFeature = m_curAttr->theFeatureList->first;
	}

	fillOthers();
	OnBnClickedApplybtn(); //update E function part
	*pResult = 0;

}


//update the weight of m_curFeature according to the edits in the list
void CFuzzyTerrainDlg::OnLvnEndlabeleditFeaturelistctrl(NMHDR *pNMHDR, LRESULT *pResult){
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	if(m_PFunc != "Weighted-Average") return;
	CString str = pDispInfo->item.pszText;
	if(str == "") return;
	POSITION pos = m_theList.GetFirstSelectedItemPosition();
	if (pos != NULL){
		int nItem = m_theList.GetNextSelectedItem(pos);
		m_theList.SetItemText(nItem, 0, str);
		m_curFeature->weight = atof(str.GetBuffer(100));
		
	}

	*pResult = 0;
}

//Use the values of m_ variables to update the fields on the interface
void CFuzzyTerrainDlg::updateFields(){

	fillList();
	
	if(theRstKnowledge==NULL) return;
	fillTree();
	
}

void CFuzzyTerrainDlg::fillTree(){
	//preserve the selection info
	int level = 0;
	CString name1;
	CString name2;
	HTREEITEM hItem = m_theTree.GetSelectedItem();
	name1 = m_theTree.GetItemText(hItem);
	

	//clean the old tree
	if(m_theTree.GetCount()>0)
		m_theTree.DeleteAllItems();

	m_treeRoot = m_theTree.InsertItem(theRstKnowledge->name, TVI_ROOT, TVI_LAST);

	//expand the whole tree
	m_theTree.Expand(m_treeRoot, TVE_EXPAND);
	HTREEITEM listItem = m_theTree.GetChildItem(m_treeRoot);
	while (listItem != NULL){
		m_theTree.Expand(listItem, TVE_EXPAND);
		listItem = m_theTree.GetNextItem(listItem, TVGN_NEXT);
	}

	//select the previously selected
	bool flag = false;
	if(level<1){ //New KB or at KB level
		m_theTree.SelectItem(m_treeRoot);
	}
	
	if(!flag){ //the previously selected one has been removed, set the last one to be selected
		HTREEITEM listItem = m_theTree.GetChildItem(m_treeRoot);
		HTREEITEM lastListItem = listItem;
		while (listItem != NULL){
			if(level==1)
				lastListItem = listItem;
			else if(level==2 && m_theTree.GetItemText(listItem)==name2){
				HTREEITEM item = m_theTree.GetChildItem(listItem);
				HTREEITEM lastItem = item;
				while(item != NULL){
					lastItem = item;
					item = m_theTree.GetNextItem(item, TVGN_NEXT);
				}
				m_theTree.SelectItem(lastItem);
				flag = true;
			}
			if(flag) break;
			listItem = m_theTree.GetNextItem(listItem, TVGN_NEXT);
		}
		if(level==1)
			m_theTree.SelectItem(lastListItem);
	}

	
}

void CFuzzyTerrainDlg::fillList(){
	
	if(m_curAttr==NULL) return; //the rule list or case list might be empty
	fillOthers();
	//Set the weight field according to different algo option 
	//(check or numeric weight).
	// Delete all columns.
	int nColumnCount = m_theList.GetHeaderCtrl()->GetItemCount();
	for (int i=0; i<nColumnCount; i++){
		m_theList.DeleteColumn(0); //always delete the first
	}
	if(m_curAttr != NULL) m_PFunc = m_curAttr->algo;
	LPRECT r = new RECT;
	m_theList.GetClientRect(r);
	if(m_PFunc == "Limiting-Factor"){
		m_theList.InsertColumn(0, "Limiting", LVCFMT_LEFT, (r->right-r->left)*0.4, 0); 
		m_theList.InsertColumn(1, "Feature", LVCFMT_LEFT, (r->right-r->left)*0.6, 1); 
		m_theList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_CHECKBOXES|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	}
	else if(m_PFunc == "Weighted-Average"){
		m_theList.InsertColumn(0, "Weighting", LVCFMT_LEFT, (r->right-r->left)*0.4, 0); 
		m_theList.InsertColumn(1, "Feature", LVCFMT_LEFT, (r->right-r->left)*0.6, 1); 
		m_theList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	}
	if(m_PFunc == "Multiplication"){
		m_theList.InsertColumn(0, "Multiplicating", LVCFMT_LEFT, (r->right-r->left)*0.4, 0); 
		m_theList.InsertColumn(1, "Feature", LVCFMT_LEFT, (r->right-r->left)*0.6, 1); 
		m_theList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_CHECKBOXES|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	}
	delete r;
   	m_theList.DeleteAllItems();
	
	i=0;
	CString strText;
	FeatureNode* tempFeature = m_curAttr->theFeatureList->first;
	while(tempFeature!=NULL){
		if(m_PFunc == "Limiting-Factor" || m_PFunc == "Multiplication")
			m_theList.InsertItem(LVIF_TEXT|LVIF_STATE, i, "", i==0 ? LVIS_SELECTED : 0, LVIS_SELECTED, 0, 0);
		else if(m_PFunc == "Weighted-Average"){
			strText.Format(TEXT("%.2f"), tempFeature->weight);
			m_theList.InsertItem(LVIF_TEXT|LVIF_STATE, i, strText, i==0 ? LVIS_SELECTED : 0, LVIS_SELECTED, 0, 0);
			float test = atof(m_theList.GetItemText(i, 0).GetBuffer(100));
		}
		m_theList.SetItemText(i, 1, tempFeature->name);
		if(tempFeature->useOrNot)
			m_theList.SetCheck(i, TRUE);
		else
			m_theList.SetCheck(i, FALSE);
		i++;
		tempFeature=tempFeature->next;
	}

	int c = m_theList.GetItemCount();
	for(i=0; i<c; i++){
		if(m_theList.GetItemText(i,1)==m_curFeature->name){
			int nItem = m_theList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			break;
		}
	}


	
}

void CFuzzyTerrainDlg::fillOthers(){

	if(m_curAttr==NULL||m_curFeature==NULL) return;
	m_MemberValue = m_curAttr->memberValue;
	m_Negative = !m_curAttr->positive;

	//the spatial
	
		m_x = m_curSpatial->x;
		m_y = m_curSpatial->y;
		m_SearchDist = m_curSpatial->searchDist;
		m_UseDistSimilarity = m_curSpatial->useDistSim;
		m_DistDecayFactor = m_curSpatial->distDecayFactor;
		m_VtoHConvertFactor = m_curSpatial->VtoHConvertingFactor;
		integratingAlgCodeStr(true);
		m_WgtDecayFactor = m_curSpatial->wgtDecayFactor;

	//the curve
	m_EFunc = m_curFeature->functionType;
	setIntialCurveVariables();
	UpdateData(FALSE);
	
}

//only updates the variables that are outside the tree and list.
//the variables in the tree and list will be updated immediately
//when they are changed.
void CFuzzyTerrainDlg::updateKnowledge(){
	if(m_curAttr == NULL) return;

 	UpdateData(TRUE);
	
	if(m_curAttr->memberValue != m_MemberValue){
		m_curAttr->memberValue = m_MemberValue;
	
	}
	if(m_curAttr->positive == m_Negative){ //the two are opposite
		m_curAttr->positive = !m_Negative;
	
	}
	if(m_curAttr->algo != m_PFunc){
		m_curAttr->algo = m_PFunc;
	
	}

	if(m_curFeature==NULL) return; //it is possible that m_curAttr doesn't have m_curFeature
	if(m_curFeature->functionType != m_EFunc){
		m_curFeature->functionType = m_EFunc;

	}
	if(m_curFeature->value1 != m_realV1){
		m_curFeature->value1 = m_realV1;
		m_prevV1 = m_realV1;
	
	}
	if(m_curFeature->width1 != m_realW1){
		m_curFeature->width1 = m_realW1;
		m_prevW1=m_realW1;
		
	}
	if(m_curFeature->r1 != m_r1){
		m_curFeature->r1 = m_r1;
	
	}
	if(m_curFeature->value2 != m_realV2){
		m_curFeature->value2 = m_realV2;
		m_prevV2= m_realV2;
		
	}
	if(m_curFeature->width2 != m_realW2){
		m_curFeature->width2 = m_realW2;
		m_prevW2 = m_realW2;
		
	}
	if(m_curFeature->r2 != m_r2){
		m_curFeature->r2 = m_r2;
	
	}

	

	//spatial part
	
		if(m_curSpatial==NULL) return;
		if(m_curSpatial->x != m_x){
			m_curSpatial->x = m_x;
			
		}
		if(m_curSpatial->y != m_y){
			m_curSpatial->y = m_y;
		
		}
		if(m_curSpatial->searchDist != m_SearchDist){
			m_curSpatial->searchDist = m_SearchDist;
		
		}
		if(m_curSpatial->useDistSim != m_UseDistSimilarity){
			m_curSpatial->useDistSim = m_UseDistSimilarity;
			
		}
		if(m_curSpatial->distDecayFactor != m_DistDecayFactor){
			m_curSpatial->distDecayFactor = m_DistDecayFactor;
			
		}
		if(m_curSpatial->VtoHConvertingFactor != m_VtoHConvertFactor){
			m_curSpatial->VtoHConvertingFactor = m_VtoHConvertFactor;
		
		}
		int oldAlg = m_curSpatial->integratingAlgo;
		if(oldAlg != integratingAlgCodeStr(false)){
		
		}	
		if(m_curSpatial->wgtDecayFactor != m_WgtDecayFactor){
			m_curSpatial->wgtDecayFactor = m_WgtDecayFactor;
		
		}


	
	if(theRstKnowledge!=NULL)
	{
		theRstKnowledge->checkSave();
	}
}

int CFuzzyTerrainDlg::integratingAlgCodeStr(bool flag){
	if(flag){
		switch(m_curSpatial->integratingAlgo){
		case 1:
			m_IntegratingAlgStr = "Dominant";
			break;
		case 2:
			m_IntegratingAlgStr = "Supplement";
			break;
		case 3:
			m_IntegratingAlgStr = "Maximum";
			break;
		case 4:
			m_IntegratingAlgStr = "Nearest";
			break;
		case 5:
			m_IntegratingAlgStr = "Average";
			break;
		case 6:
			m_IntegratingAlgStr = "InverseDistance";
			break;
		case 7:
			m_IntegratingAlgStr = "SimilarityWeight";
			break;
		}
	}
	else{
		if(m_IntegratingAlgStr == "Dominant")
			m_curSpatial->integratingAlgo = 1;
		else if(m_IntegratingAlgStr == "Supplement")
			m_curSpatial->integratingAlgo = 2;
		else if(m_IntegratingAlgStr == "Maximum")
			m_curSpatial->integratingAlgo = 3;
		else if(m_IntegratingAlgStr == "Nearest")
			m_curSpatial->integratingAlgo = 4;
		else if(m_IntegratingAlgStr == "Average")
			m_curSpatial->integratingAlgo = 5;
		else if(m_IntegratingAlgStr == "InverseDistance")
			m_curSpatial->integratingAlgo = 6;
		else if(m_IntegratingAlgStr == "SimilarityWeight")
			m_curSpatial->integratingAlgo = 7;
	}
	return m_curSpatial->integratingAlgo;
}

void CFuzzyTerrainDlg::OnEnKillfocusLowervaluefld(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_realV1>m_realV2){
		MessageBox("v1 cannot be greater than v2.");
		return;
	}
	OnBnClickedApplybtn();
}

void CFuzzyTerrainDlg::OnEnKillfocusUppervaluefld(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_realV1>m_realV2){
		MessageBox("v1 cannot be greater than v2.");
		return;
	}
	OnBnClickedApplybtn();
}

void CFuzzyTerrainDlg::OnEnKillfocusWidth1fld(){
	// TODO: Add your control notification handler code here
	OnBnClickedApplybtn();
}

void CFuzzyTerrainDlg::OnEnKillfocusWidth2fld(){
	// TODO: Add your control notification handler code here
	OnBnClickedApplybtn();
}

void CFuzzyTerrainDlg::OnEnKillfocusr1fld(){
	// TODO: Add your control notification handler code here
	OnBnClickedApplybtn();
}

void CFuzzyTerrainDlg::OnEnKillfocusr2fld(){
	// TODO: Add your control notification handler code here
	OnBnClickedApplybtn();
}

void CFuzzyTerrainDlg::OnEnKillfocusSearchdistfld(){
	// TODO: Add your control notification handler code here
	OnBnClickedApplybtn();
}

void CFuzzyTerrainDlg::OnEnKillfocusVtohconvertfactorfld(){
	// TODO: Add your control notification handler code here
	OnBnClickedApplybtn();
}

void CFuzzyTerrainDlg::OnEnKillfocusDistdecayfactorfld(){
	// TODO: Add your control notification handler code here
	OnBnClickedApplybtn();
}

void CFuzzyTerrainDlg::OnEnKillfocusWgtdecayfactorfld(){
	// TODO: Add your control notification handler code here
	OnBnClickedApplybtn();
}

void CFuzzyTerrainDlg::OnCbnKillfocusIntegratingalgocmb(){
	// TODO: Add your control notification handler code here
	OnBnClickedApplybtn();
}

void CFuzzyTerrainDlg::setInitialForCur(){	
	if(theRstKnowledge!=NULL){
		m_curAttr = theRstKnowledge->theAttr;
		m_curSpatial = theRstKnowledge->theSpatial;
	 }
	if(m_curAttr!=NULL)
		m_curFeature = m_curAttr->theFeatureList->first;
}

void CFuzzyTerrainDlg::enableDisableItems(){  //this function is changed by DuFei to avoid flicker
	
	GetDlgItem(IDC_EditEnvListBtn)->EnableWindow(TRUE);

	GetDlgItem(IDC_RunInferBtn)->EnableWindow(FALSE);

	if(theEnvDatabase!=NULL&&theEnvDatabase->numOfLayers!=0&&theRstKnowledge!=NULL)
		GetDlgItem(IDC_RunInferBtn)->EnableWindow(true);




	//THE TOP BUTTONS
	if(theEnvDatabase==NULL || theEnvDatabase->numOfLayers==0){
		GetDlgItem(IDC_NewBtn)->EnableWindow(FALSE);
		
	}
	else 
		GetDlgItem(IDC_NewBtn)->EnableWindow(TRUE);

	

   
	
	//Spatial setting button should be turned off for RBR

	//GetDlgItem(IDC_SpatialSettingBtn)->EnableWindow(TRUE);	
	//OTHER ITEMS
	if(m_UseSpatial)	openSpatialPanel();
	else				closeSpatialPanel();


	if(m_curSpatial==NULL){
		GetDlgItem(IDC_UseDistSimChk)->EnableWindow(FALSE);
		GetDlgItem(IDC_SearchDistFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_VtoHConvertFactorFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_DistDecayFactorFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_WgtDecayFactorFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_IntegratingAlgoCmb)->EnableWindow(FALSE);
	}
	else
	{
	   GetDlgItem(IDC_UseDistSimChk)->EnableWindow(TRUE);
	   GetDlgItem(IDC_SearchDistFld)->EnableWindow(TRUE);
	   GetDlgItem(IDC_VtoHConvertFactorFld)->EnableWindow(TRUE);
	   GetDlgItem(IDC_DistDecayFactorFld)->EnableWindow(TRUE);
	   GetDlgItem(IDC_WgtDecayFactorFld)->EnableWindow(TRUE);
	   GetDlgItem(IDC_IntegratingAlgoCmb)->EnableWindow(TRUE);
	}



	if(m_curAttr==NULL){	    
		GetDlgItem(IDC_PositiveRdo)->EnableWindow(FALSE);
		GetDlgItem(IDC_NegativeRdo)->EnableWindow(FALSE);
		GetDlgItem(IDC_MembershipLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_MembershipFld)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_PositiveRdo)->EnableWindow(TRUE);
	    GetDlgItem(IDC_NegativeRdo)->EnableWindow(TRUE);
	    GetDlgItem(IDC_MembershipLbl)->EnableWindow(TRUE);
	    GetDlgItem(IDC_MembershipFld)->EnableWindow(TRUE);
	}


	//if(m_curFeature==NULL){
	//	GetDlgItem(IDC_MoreOptionBtn)->EnableWindow(FALSE);
	//}
	//else
	//   GetDlgItem(IDC_MoreOptionBtn)->EnableWindow(TRUE);


	if(m_curAttr==NULL || m_curAttr->algo != "Weighted-Average")
		GetDlgItem(IDC_AHPBtn)->EnableWindow(FALSE);
	else	
		GetDlgItem(IDC_AHPBtn)->EnableWindow(TRUE);



	
	if(m_curFeature==NULL || m_EFunc=="Raw" || ((m_curAttr->algo=="Limiting-Factor" || m_curAttr->algo=="Multiplication")&& !m_curFeature->useOrNot)){
		GetDlgItem(IDC_LowerValueLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_LowerValueFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_UpperValueLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_UpperValueFld)->EnableWindow(FALSE);
	}
	else
	{
	   GetDlgItem(IDC_LowerValueLbl)->EnableWindow(TRUE);
	   GetDlgItem(IDC_LowerValueFld)->EnableWindow(TRUE);
	   GetDlgItem(IDC_UpperValueLbl)->EnableWindow(TRUE);
	   GetDlgItem(IDC_UpperValueFld)->EnableWindow(TRUE);
	}



    if(m_curFeature==NULL || m_EFunc=="Raw" || ((m_curAttr->algo=="Limiting-Factor" || m_curAttr->algo=="Multiplication")&& !m_curFeature->useOrNot)){
		GetDlgItem(IDC_TopValueFld)->EnableWindow(FALSE);
	    GetDlgItem(IDC_MidValueFld)->EnableWindow(FALSE);
        GetDlgItem(IDC_OriginValueFld)->EnableWindow(FALSE);
		GetDlgItem(IDC_ViewTypeLbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_ViewTypeCmb)->EnableWindow(FALSE);
	
    }
    else 
    {	
	    GetDlgItem(IDC_TopValueFld)->EnableWindow(TRUE);
	    GetDlgItem(IDC_MidValueFld)->EnableWindow(TRUE);
	    GetDlgItem(IDC_OriginValueFld)->EnableWindow(TRUE);
	    GetDlgItem(IDC_ViewTypeLbl)->EnableWindow(TRUE);
	    GetDlgItem(IDC_ViewTypeCmb)->EnableWindow(TRUE);
	   
    }

	  if(m_curFeature==NULL || ((m_curAttr->algo=="Limiting-Factor" || m_curAttr->algo=="Multiplication")&& !m_curFeature->useOrNot)){
         GetDlgItem(IDC_CurveTypeLable)->EnableWindow(false);
	     GetDlgItem(IDC_curveTypeCmb)->EnableWindow(false);
	  }
	  else
	  {	
		  GetDlgItem(IDC_CurveTypeLable)->EnableWindow(true);
	      GetDlgItem(IDC_curveTypeCmb)->EnableWindow(true);
	  }
	

	if(m_curFeature==NULL || m_EFunc=="Raw" || ((m_curAttr->algo=="Limiting-Factor" || m_curAttr->algo=="Multiplication")&& !m_curFeature->useOrNot)){
		
		GetDlgItem(IDC_Width1Lbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_Width1Fld)->EnableWindow(FALSE);
		GetDlgItem(IDC_Width2Lbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_Width2Fld)->EnableWindow(FALSE);
		GetDlgItem(IDC_r1Lbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_r1Fld)->EnableWindow(FALSE);
		GetDlgItem(IDC_r2Lbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_r2Fld)->EnableWindow(FALSE);
	}	
	else if(m_EFunc == "Continuous:s-shape"){	
	
	   GetDlgItem(IDC_UpperValueLbl)->EnableWindow(FALSE);
	   GetDlgItem(IDC_UpperValueFld)->EnableWindow(FALSE);
	   GetDlgItem(IDC_Width1Lbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_Width1Fld)->EnableWindow(TRUE);
		GetDlgItem(IDC_Width2Lbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_Width2Fld)->EnableWindow(FALSE);
		GetDlgItem(IDC_r1Lbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_r1Fld)->EnableWindow(TRUE);
		GetDlgItem(IDC_r2Lbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_r2Fld)->EnableWindow(FALSE);
	}
	else if(m_EFunc == "Continuous:z-shape"){
	   GetDlgItem(IDC_LowerValueLbl)->EnableWindow(FALSE);
	   GetDlgItem(IDC_LowerValueFld)->EnableWindow(FALSE);
	   GetDlgItem(IDC_Width1Lbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_Width1Fld)->EnableWindow(FALSE);
		GetDlgItem(IDC_Width2Lbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_Width2Fld)->EnableWindow(TRUE);
		GetDlgItem(IDC_r1Lbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_r1Fld)->EnableWindow(FALSE);
		GetDlgItem(IDC_r2Lbl)->EnableWindow(TRUE);
		GetDlgItem(IDC_r2Fld)->EnableWindow(TRUE);
	}

	else if(m_EFunc == "Nominal" || m_EFunc == "Ordinal"){
		GetDlgItem(IDC_Width1Lbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_Width1Fld)->EnableWindow(FALSE);
		GetDlgItem(IDC_Width2Lbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_Width2Fld)->EnableWindow(FALSE);
		GetDlgItem(IDC_r1Lbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_r1Fld)->EnableWindow(FALSE);	
		GetDlgItem(IDC_r2Lbl)->EnableWindow(FALSE);
		GetDlgItem(IDC_r2Fld)->EnableWindow(FALSE);
	}
	else{
		GetDlgItem(IDC_Width1Lbl)->EnableWindow(TRUE);
	    GetDlgItem(IDC_Width1Fld)->EnableWindow(TRUE);
	    GetDlgItem(IDC_Width2Lbl)->EnableWindow(TRUE);
	    GetDlgItem(IDC_Width2Fld)->EnableWindow(TRUE);
	    GetDlgItem(IDC_r1Lbl)->EnableWindow(TRUE);
	    GetDlgItem(IDC_r1Fld)->EnableWindow(TRUE);
	    GetDlgItem(IDC_r2Lbl)->EnableWindow(TRUE);
	    GetDlgItem(IDC_r2Fld)->EnableWindow(TRUE);
	}

}

void CFuzzyTerrainDlg::clearFields(){
	m_theTree.DeleteAllItems();
	m_treeRoot = NULL;
	m_theList.DeleteAllItems();
	m_ApplyTo = 0;
	m_MemberValue = 1;
	m_EFunc = "Continuous:bell-shape";
	m_PFunc = "Limiting-Factor";
	m_v1 = 0;
	m_realW1 = 0;
	m_w1 = 0;
	m_r1 = 2;
	m_v2 = 0;
	m_realW2 = 0;
	m_w2 = 0;
	m_r2 = 2;
	

	theRstKnowledge = NULL;

	m_curFeature = NULL;
	m_curAttr = NULL;
	m_curSpatial = NULL;
	
	m_DistDecayFactor = 0.0f;
	m_VtoHConvertFactor = 0.0f;
	m_WgtDecayFactor = 0.0f;
	m_SearchDist = 0.0f;
	m_x = 0.0;
	m_y = 0.0;
	m_UseDistSimilarity = FALSE;
	m_IntegratingAlgStr = "Maximum";

	UpdateData(FALSE);

}

//Manually added notification function for selecting the P_Function by right-clicking the header of the feature list
//Steps: 1.Add a notification message in the mapping part in this file
//       2.Add a function declaration in the .h 
//       3.Add this function definition here
void CFuzzyTerrainDlg::OnNMRclickFeaturelistHeaderctrl(NMHDR *pNMHDR, LRESULT *pResult){
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	// TODO: Add your control notification handler code here
	if(m_curAttr==NULL) return;
	CPoint pt;
	GetCursorPos(&pt);
	CRect rect;
	CRect rect2;

	//Check if in the extent of the first item header. If yes, launch the P_Func Menu.
	m_theList.GetHeaderCtrl()->GetItemRect(0, &rect);
	m_theList.GetHeaderCtrl()->GetWindowRect(&rect2);
	if(pt.x>rect2.left && pt.x<rect2.left+rect.right && pt.y>rect2.top && pt.y<rect2.bottom){
		m_applyMenuDlg.m_MenuCode = 0;
		m_applyMenuDlg.DoModal();
	
	}

	*pResult = 0;
}


/*
void CFuzzyTerrainDlg::OnRButtonDown(UINT nFlags, CPoint point){
	// TODO: Add your message handler code here and/or call default
	if(m_curAttr==NULL) return;

	CPoint pt;
	GetCursorPos(&pt);
	CRect rect;

	//2. Check if in the extent of the curve defining region. If yes, launch the E_Func Menu.
	GetDlgItem(IDC_CurveGrp)->GetWindowRect(&rect);
	if(pt.x>rect.left && pt.x<rect.right && pt.y>rect.top && pt.y<rect.bottom){
		/*m_applyMenuDlg.m_MenuCode = 1;
		if(m_knowledgebaseType==3){ 
		  m_applyMenuDlg.m_MakeFuzzyBnd = m_MakeFuzzyBnd;
		  m_applyMenuDlg.DoModal();
	    }
		else{
		   m_applyMenuDlg.m_MakeFuzzyBnd = false;
		   m_applyMenuDlg.DoModal();
		}
		return;
	}

	//if(m_knowledgebaseType==4) return;

	//4. Check if in the extent of spatial setting 
	if(m_UseSpatial){ //apply the spatial setting
		GetDlgItem(IDC_SptGrp)->GetWindowRect(&rect);
		if(pt.x>rect.left && pt.x<rect.right && pt.y>rect.top && pt.y<rect.bottom){
			m_ApplyTo = 1; //apply spatial
			m_applyMenuDlg.m_MenuCode = 2;
			m_applyMenuDlg.DoModal();
		}
	}
	CDialog::OnRButtonDown(nFlags, point);
}*/

void CFuzzyTerrainDlg::OnLimitingfactor(){
	// TODO: Add your command handler code here
	m_PFunc = "Limiting-Factor";
	if(m_curAttr != NULL)
		m_curAttr->algo = m_PFunc;
	GetDlgItem(IDC_AHPBtn)->EnableWindow(FALSE);
	fillList();
}

void CFuzzyTerrainDlg::OnWeightedaverage(){
	// TODO: Add your command handler code here
	m_PFunc = "Weighted-Average";
	if(m_curAttr != NULL)
		m_curAttr->algo = m_PFunc;
	GetDlgItem(IDC_AHPBtn)->EnableWindow(TRUE);
	fillList();
}

void CFuzzyTerrainDlg::OnMultiplication(){
	// TODO: Add your command handler code here
	m_PFunc = "Multiplication";
	if(m_curAttr != NULL)
		m_curAttr->algo = m_PFunc;
	GetDlgItem(IDC_AHPBtn)->EnableWindow(FALSE);
	fillList();
}



//Call AHP Weighting Dialog
void CFuzzyTerrainDlg::OnBnClickedAhpbtn(){
	// TODO: Add your control notification handler code here
	if(theEnvDatabase == NULL || theEnvDatabase->firstLayer == NULL){
		MessageBox("Load environmental data first.");
		return;
	}
	if(m_AHPDlg.m_theAHP!=NULL)
		delete m_AHPDlg.m_theAHP;
	m_AHPDlg.m_theAHP = new List<AHP_ScaleList>(*theAHP);

	if(m_AHPDlg.DoModal() == IDOK){
		//update theAHP
		if(theAHP!=NULL){
			delete theAHP;
		}
		theAHP = new List<AHP_ScaleList>(*(m_AHPDlg.m_theAHP));

		//Update the weights for the current features
		if(m_curAttr == NULL) return;
		CString strText;
		int i=0;
		FeatureNode* tempFeature = m_curAttr->theFeatureList->first;
		while(tempFeature != NULL){
			AHP_ScaleList* tempList = theAHP->first;
			while(tempList!=NULL){
				if(tempFeature->name==tempList->name){
					tempFeature->weight = tempList->weight;
					strText.Format(TEXT("%.2f"), tempFeature->weight);
					m_theList.SetItemText(i, 0, strText);
					break;
				}
				tempList = tempList->next;
			}
			i++;
			tempFeature = tempFeature->next;
		}
	}
}

//saving function return values: 
//0: successfully saved
//1: no file name (user canceled on the file name dialogbox)
//2: saving canceled
//3: error in writing file
//6: user select yes on asking saving dialogbox
//7: user select no on  asking saving dialogbox
void CFuzzyTerrainDlg::OnClose(){
	// TODO: Add your message handler code here and/or call default
	//updateKnowledge();  //Added by DuFei

   //removeRstCasebase();
	//clearFields();
	ClearDialog();

	OnOK();
}






void CFuzzyTerrainDlg::ClearDialog(void){

	m_ApplyTo = 0;
	m_MemberValue = 1;
	m_EFunc = "Continuous:bell-shape";
	m_PFunc = "Limiting-Factor";
	m_v1 = 0;
	m_realW1 = 0;
	m_w1 = 0;
	m_r1 = 2;
	m_v2 = 0;
	m_realW2 = 0;
	m_w2 = 0;
	m_r2 = 2;
	m_curFeature = NULL;
	m_curAttr = NULL;
	m_curSpatial = NULL;
	m_curRuleList = NULL;	
	
	m_DistDecayFactor = 0.0f;
	m_UseSpatial = true;
	m_VtoHConvertFactor = 0.0f;
	m_WgtDecayFactor = 0.0f;
	m_SearchDist = 0.0f;

	m_UseDistSimilarity = false;
	m_IntegratingAlgStr = "Maximum";
	if(theAHP!=NULL){
		delete theAHP;
		theAHP = NULL;
	}

	if(theEnvDatabase!=NULL){
		delete theEnvDatabase;
     	theEnvDatabase = NULL;	
	}
	if(theRstKnowledge!=NULL){
		delete theRstKnowledge;
        theRstKnowledge =NULL;
	}


	m_realV1=0;
    m_realV2=0;  
    m_prevV1=0;
	m_prevV2=0;
    m_prevW1=0;
	m_prevW2=0;
	//m_isInit = false;  ]
	if(m_tooltip!=NULL){
		delete m_tooltip;
      	m_tooltip = NULL;
	}

	m_theTree.DeleteAllItems();
	m_treeRoot = NULL;
	m_theList.DeleteAllItems();


	
}



void CFuzzyTerrainDlg::OnCbnSelchangecurvetypecmb()
{
	UpdateData(TRUE);
	int s = m_curveTypeCmb.GetCurSel();
	//m_curveTypeCmb.GetLBText(s, m_ViewType);
	UpdateData(FALSE);
	
		switch (s){
           case 0:
	          m_EFunc = "Continuous:bell-shape";
	          setCurve();
	          break;
  	       case 1:
	          m_EFunc = "Continuous:s-shape";
	          setCurve();
	          break;
  	       case 2:
	          m_EFunc = "Continuous:z-shape";
	          setCurve();
	          break;
	       case 3:
	          m_EFunc = "Cyclic";
	          setCurve();
	          break;
	       case 4:
	          m_EFunc = "Ordinal";
	          setCurve();
	          break;
	       case 5:
	          m_EFunc = "Nominal";
	          setCurve();
	          break;
	       /*case 6:
	          m_EFunc = "Default";
	          setCurve();
	          break;*/
	       case 6:
	          m_EFunc = "Raw";
	          setCurve();
	          break;
	       default:
			   break;
	   
	}
	
	// TODO: Add your control notification handler code here
}

BOOL CFuzzyTerrainDlg::PreTranslateMessage( MSG *pMsg )
{
	
	if(m_tooltip->m_hWnd!=NULL){
          m_tooltip->RelayEvent(pMsg);   
	}
   return   CDialog::PreTranslateMessage(pMsg);   


}



void CFuzzyTerrainDlg::OnBnClickedRuninferbtn()
{

	  OnBnClickedApplybtn();
	//browse to get the folder to store the result
     CString dir;

		CString defaultName;
		defaultName = theRstKnowledge->name;
		m_beforeInferenceRepDlg.m_KBStatus = "Will perform an inference using '" + defaultName + "'.";
	    defaultName += ".3dr";
	    static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	    CFileDialog m_ldFile(FALSE, "3dr", defaultName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	    m_ldFile.m_ofn.lpstrTitle="Save Inference Result As";	
	    if(m_ldFile.DoModal() == IDOK)
		   m_OutputFileName = m_ldFile.GetPathName();
	    else{
		   return;
	    }
	    m_beforeInferenceRepDlg.m_OutputStatus = m_OutputFileName;	
		
        if(m_beforeInferenceRepDlg.DoModal() == IDOK){
			m_ProgressBar.ShowWindow(SW_SHOW);
		    runInference();
			MessageBox("Inference is done.");
			m_ProgressBar.ShowWindow(SW_HIDE);
		}
	   

		return;
	// TODO: Add your control notification handler code here
}




void CFuzzyTerrainDlg::output(float* soilmap, float* checkmap){
	int nrow = theEnvDatabase->theSetting->nrow;
	int ncol = theEnvDatabase->theSetting->ncol;
	int numPixels = nrow*ncol;
	int nodata = theEnvDatabase->theSetting->nodata;
	for(int i=0; i<numPixels; i++)
		if(fabs(soilmap[i]-nodata)<VERYSMALL){

		}
		else  
			soilmap[i] *= 100.0;
	CThreeDR* theOutput = new CThreeDR;
	theOutput->setDataBuf(soilmap);
	theOutput->setDataName(extractFileName(m_OutputFileName).GetBuffer(100));
	theOutput->copyBasicFields(theEnvDatabase->firstLayer->theData);
	theOutput->setNullDataValue(nodata);
	theOutput->calcStat();
	int writeFlag = theOutput->writeout(m_OutputFileName.GetBuffer(100), 0);
	delete theOutput;


}

void CFuzzyTerrainDlg::OnBnClickedOk(){
	// TODO: Add your control notification handler code here
	//removeRstCasebase();
	//clearFields();
	ClearDialog();

	//updateKnowledge(); 
	OnOK();
}



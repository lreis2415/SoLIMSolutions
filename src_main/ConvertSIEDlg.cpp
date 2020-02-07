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
// ConvertSIEDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "ConvertSIEDlg.h"
#include ".\convertSIEdlg.h"
#include "ImportOldPrj.h"

#include "RangeRule.h"
#include "EnumRule.h"
#include "SoilType.h"
#include "Instance.h"
#include "Occurrence.h"
#include "Exclusion.h"
#include "FreehandRule.h"
#include "WordRule.h"
#include "PointRule.h"

#include "shpcasebase.h"

#include  <msxml2.h>      
#import   <msxml4.dll>


// CConvertSIEDlg dialog

IMPLEMENT_DYNAMIC(CConvertSIEDlg, CDialog)
CConvertSIEDlg::CConvertSIEDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConvertSIEDlg::IDD, pParent)
	, m_NewPrjName(_T(""))
	, m_NewPrjDir(_T(""))
	, m_SIEListFileName(_T(""))
	, m_SIEKnowledgeFileName(_T(""))
{
		m_KnowledgeType = 0;
}

CConvertSIEDlg::~CConvertSIEDlg()
{
}

void CConvertSIEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NewPrjNameFld, m_NewPrjName);
	DDX_Text(pDX, IDC_NewPrjDirFld, m_NewPrjDir);
	DDX_Text(pDX, IDC_SIEListFileFld, m_SIEListFileName);
	DDX_Text(pDX, IDC_SIEKnowledgeFileFld, m_SIEKnowledgeFileName);
}


BEGIN_MESSAGE_MAP(CConvertSIEDlg, CDialog)
	ON_BN_CLICKED(IDC_ListFileBtn, OnBnClickedListfilebtn)
	ON_BN_CLICKED(IDC_KnowledgeFileBtn, OnBnClickedKnowledgefilebtn)
	ON_BN_CLICKED(IDC_NewDirBtn, OnBnClickedNewdirbtn)
	ON_BN_CLICKED(IDC_RADIO_ATTRIBUTE, OnBnClickedRadioAttribute)
	ON_BN_CLICKED(IDC_RADIO_POINT, OnBnClickedRadioPoint)
	
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()



BOOL CConvertSIEDlg::OnInitDialog(){

	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
   ((CButton*)GetDlgItem(IDC_RADIO_ATTRIBUTE))->SetCheck(1);
	m_KnowledgeType = 0;
	m_SIEKnowledgeFileName="";
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE


}
// CConvertSIEDlg message handlers

void CConvertSIEDlg::OnBnClickedListfilebtn()
{

	UpdateData(TRUE);
	static char BASED_CODE szFilter[] = "List Files (*.lst)|*.lst|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.lst", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	if(m_ldFile.DoModal() == IDOK)
	{
		m_SIEListFileName = m_ldFile.GetPathName();
	
		UpdateData(FALSE);
	}	

	// TODO: Add your control notification handler code here
}

void CConvertSIEDlg::OnBnClickedKnowledgefilebtn()
{
	
	UpdateData(TRUE);
	if(m_KnowledgeType == 0) {
	   static char BASED_CODE szFilter[] = "DBF Files (*.dbf)|*.dbf|All Files (*.*)|*.*||";
  	   CFileDialog m_ldFile(TRUE, NULL, "*.dbf", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
       if(m_ldFile.DoModal() == IDOK){
		   m_SIEKnowledgeFileName = m_ldFile.GetPathName();
		   UpdateData(FALSE);
	   } 	
 	}
	else if(m_KnowledgeType == 1) {
	    static char BASED_CODE szFilter[] = "Shape Files (*.shp)|*.shp|All Files (*.*)|*.*||";
 	    CFileDialog m_ldFile(TRUE, NULL, "*.shp", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	    if(m_ldFile.DoModal() == IDOK){
		    m_SIEKnowledgeFileName = m_ldFile.GetPathName();
			UpdateData(false);
	   }
	}
	

}

void CConvertSIEDlg::OnBnClickedNewdirbtn()
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    m_NewPrjDir = m_getDirDlg.GetDirectory(this, "", "Choose a directory to hold the new project:");
	UpdateData(false);

}

void CConvertSIEDlg::OnBnClickedRadioAttribute()
{
	m_KnowledgeType = 0;
	((CButton*)GetDlgItem(IDC_RADIO_ATTRIBUTE))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_POINT))->SetCheck(0);
	
	m_SIEKnowledgeFileName="";
	UpdateData(false);
	// TODO: Add your control notification handler code here
}

void CConvertSIEDlg::OnBnClickedRadioPoint()
{
	m_KnowledgeType = 1;
	((CButton*)GetDlgItem(IDC_RADIO_ATTRIBUTE))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_POINT))->SetCheck(1);

	m_SIEKnowledgeFileName="";
	UpdateData(false);
	// TODO: Add your control notification handler code here
}

void CConvertSIEDlg::OnBnClickedOk()
{

	UpdateData(TRUE);

	if (m_SIEListFileName.IsEmpty()){
		MessageBox("You must specify the List File!");
		return;
	}
	else if (m_SIEKnowledgeFileName.IsEmpty()){
		MessageBox("You must specify the KnowledgeBase File!");
		return;
	}
	else if (m_NewPrjName.IsEmpty()){
		MessageBox("You must specify the New Project Name!");
		return;
	}
	else if (m_NewPrjDir.IsEmpty()){
		MessageBox("You must specify the New Project Directory!");
	   return;
  }

	else
	{	

		if(MessageBox(" Only rule base and point case base in the old project can be converted currently. \n Mask information on different levels will be lost. \n Do you want to convert now?.", "Warning",MB_ICONQUESTION|MB_YESNO) == IDYES)
		{
		
		   bool success =ConvertSIEProject(m_KnowledgeType,m_SIEListFileName,m_SIEKnowledgeFileName,m_NewPrjName, m_NewPrjDir);
		   if(success)
		   {
			    MessageBox("Convert project successfully!");
		   }
		}		
	   
	    CDialog::OnOK();
	}
}



bool CConvertSIEDlg::ConvertSIEProject(int knowledgebaseType, CString layerListFile,CString knowledgeBaseFile, CString newProjectName, CString newProjectDir)
{
	CString strProjectFullPath = newProjectDir+"\\"+newProjectName+".sip";
	CreateDirectory(newProjectDir+"\\GISData",NULL);	

	CProject* pProject = new CProject;
	pProject->setProjectName(newProjectName);
	pProject->setProjectDir(newProjectDir);
	pProject->setProjectDataType("3dr");

	pProject->m_pEnvDatabase = new CEnvDatabase();		
	pProject->m_pKnowledgeBase = new CKnowledgeBase();	
	pProject->m_pInference = new CInference();

	
	int numOfLayers = 0;

	ifstream fin;
	fin.open(m_SIEListFileName);
	if(fin.fail()){
		MessageBox("Error in converting project.");
		return false;
	}
	///copy data layer
	while(!fin.eof()){
		CString aline;
		fin.getline(aline.GetBuffer(1024),1024);
		aline.ReleaseBuffer(-1);
		int pos;
		pos=aline.Find(":",0);
		pos=pos-1;		
		CString layerName;
		CString fileName;
		layerName=aline.Left(pos);
		layerName.Trim();
		fileName=aline.Right(aline.GetLength()-pos);
		fileName.Trim();
		if(!layerName.IsEmpty()&& !fileName.IsEmpty()){
			////////////Added by DuFei
			CString newLayerPath;
		    CString name=extractFileName(fileName);
			newLayerPath=m_NewPrjDir+"\\GIS Data\\"+name;
			CopyFile(fileName,newLayerPath,false);  //copy env layer to current project directory
		
			
				CEnvLayer *pLayer = new CEnvLayer(numOfLayers,layerName, newLayerPath);
				if(pLayer->getLayerID() != -1)
				{ 
					pProject->m_pEnvDatabase->AddLayer(pLayer);
						numOfLayers++;
				}
				else 
				{
					delete pLayer;
					pLayer = NULL;
					CString msg;
					msg.Format("Error in reading layer %s. This layer will not be added",layerName);
					AfxMessageBox(msg);
					//LayerNum--;
				}

	    }
	}
	fin.close();


		///////////////load rule base/////////////////////////////////////
			if(knowledgebaseType==0){
					        
				RuleBase* theAttrKnowledge = new RuleBase();
				if(theAttrKnowledge == NULL)
				{

					AfxMessageBox("Error in opening project.");
					fin.close();
					return false;
				}

				DBFRuleBase * dbf = new DBFRuleBase();
				if(dbf == NULL)
				{

					AfxMessageBox("Error in opening project.");
					fin.close();
					return false;
				}

				if(dbf->openDBF(knowledgeBaseFile, false)!=0 || dbf->readDBF(theAttrKnowledge)==0){
					delete dbf;

					AfxMessageBox("Error in opening rulebase file.");
					fin.close();
					return false;
				}
				dbf->closeDBF();
				delete dbf;



				RuleList* tempList =theAttrKnowledge->first;
				while(tempList!=NULL){//for every soil type
					CSoilType *pSoilType = new CSoilType();
					pSoilType->setSoilTypeName(tempList->name);

					Attr* tempAttr = tempList->first;
					while(tempAttr != NULL){//for every instance
						CInstance *pInstance = new CInstance();
						pInstance->setInstanceName(tempAttr->name);

						FeatureNode *AttrFeature = tempAttr->theFeatureList->first;
						while(AttrFeature != NULL)  //for every rule
						{							 							 
							if(AttrFeature->functionType == "Nominal")
							{
								CEnumRule *pRule = new CEnumRule();
								pRule->setRuleType(4);
								pRule->setLayerName(AttrFeature->name);
								pRule->setRuleName(AttrFeature->name);
								pRule->setIsUsed(AttrFeature->useOrNot);
								for(int i = (int)(AttrFeature->value1); i<= AttrFeature->value2; i++)
									pRule->AddItem(i);
								pInstance->AddRule(pRule);

							}
							else if(AttrFeature->functionType == "Ordinal" || AttrFeature->functionType == "Cyclic")
							{
								CRangeRule *pRule = new CRangeRule();
								pRule->setRuleType(0);

								pRule->setRuleName(AttrFeature->name);
								pRule->setLayerName(AttrFeature->name);
								pRule->setIsUsed(AttrFeature->useOrNot);
								pRule->setLowUnity(AttrFeature->value1);
								pRule->setHighUnity(AttrFeature->value2);
								pRule->setLowCross(AttrFeature->value1 - AttrFeature->width1);
								pRule->setHighCross(AttrFeature->value2 + AttrFeature->width2);
								pInstance->AddRule(pRule);
							}
							else if(AttrFeature->functionType == "Continuous:bell-shape")
							{
								CRangeRule *pRule = new CRangeRule();
								pRule->setRuleType(0);
								pRule->setCurveType(0);

								pRule->setRuleName(AttrFeature->name);
								pRule->setLayerName(AttrFeature->name);
								pRule->setIsUsed(AttrFeature->useOrNot);
								pRule->setLowUnity(AttrFeature->value1);
								pRule->setHighUnity(AttrFeature->value2);
								pRule->setLowCross(AttrFeature->value1 - AttrFeature->width1);
								pRule->setHighCross(AttrFeature->value2 + AttrFeature->width2);
								pInstance->AddRule(pRule);
							}
							else if(AttrFeature->functionType == "Continuous:s-shape")
							{
								CRangeRule *pRule = new CRangeRule();
								pRule->setRuleType(0);
								pRule->setCurveType(1);

								pRule->setRuleName(AttrFeature->name);
								pRule->setLayerName(AttrFeature->name);
								pRule->setIsUsed(AttrFeature->useOrNot);
								pRule->setLowUnity(AttrFeature->value1);

								pRule->setLowCross(AttrFeature->value1 - AttrFeature->width1);

								pInstance->AddRule(pRule);
							}
							else if(AttrFeature->functionType == "Continuous:z-shape")
							{
								CRangeRule *pRule = new CRangeRule();
								pRule->setRuleType(0);
								pRule->setCurveType(2);
								pRule->setRuleName(AttrFeature->name);
								pRule->setLayerName(AttrFeature->name);
								pRule->setIsUsed(AttrFeature->useOrNot);

								pRule->setHighUnity(AttrFeature->value2);

								pRule->setHighCross(AttrFeature->value2 + AttrFeature->width2);
								pInstance->AddRule(pRule);
							}


							AttrFeature = AttrFeature->next;
						}
						pSoilType->AddInstance(pInstance);
						tempAttr = tempAttr->next;
					}
					pProject->m_pKnowledgeBase->AddSoilType(pSoilType);
					tempList = tempList->next;
				}

				if(theAttrKnowledge!=NULL)
				{
					delete theAttrKnowledge;
					theAttrKnowledge = NULL;
				}
				   



			}

			//////load point casebase////////////////////////////////////////////
			else if(knowledgebaseType==1){
			
				SHPCaseBase * caseio = NULL;
				int flag = 0;
				CaseBase<PointCase>* thePntKnowledge = new CaseBase<PointCase>();
				if(thePntKnowledge == NULL)
				{
					MessageBox("Error in opening project.");
					fin.close();
					return false;
				}


				CString PointCasebasePath = knowledgeBaseFile;
				thePntKnowledge->fileName = PointCasebasePath;
				caseio = new SHPCaseBase(SHPT_POINT);
				if(caseio == NULL)
				{
					AfxMessageBox("Error in opening project.");
					fin.close();
					return false;
				}
				flag = caseio->openFiles(PointCasebasePath);
				if (flag < 0){
					AfxMessageBox("Error in opening point casebase file!");

					delete caseio;
					fin.close();
					return false;
				}
				flag = caseio->readSHP(thePntKnowledge);
				if(flag < 0){
					MessageBox("Error in opening point casebase file");

					delete caseio;
					fin.close();
					return false;
				}
				delete caseio;
				/*
				if(thePntKnowledge->checkFeatureMatch(theEnvDatabase)<0){
					m_envFactorMismatchDlg.theFeatureList = thePntKnowledge->first->first->theAttr->theFeatureList;
					if(m_envFactorMismatchDlg.DoModal()!=IDOK){

						AfxMessageBox("Error in opening project!");
						fin.close();
						return false;
					}
				}*/

				CaseList<PointCase>* tempList = thePntKnowledge->first;

				//RuleList* tempList =theAttrKnowledge->first;
				while(tempList!=NULL){//for every soil type
					CSoilType *pSoilType = new CSoilType();
					pSoilType->setSoilTypeName(tempList->name);

					PointCase* tempAttr = tempList->first;
					while(tempAttr != NULL){//for every instance
						CInstance *pInstance = new CInstance();
						pInstance->setInstanceName(tempAttr->theAttr->name);

						FeatureNode *AttrFeature = tempAttr->theAttr->theFeatureList->first;
						while(AttrFeature != NULL)  //for every rule
						{							 							 
							if(AttrFeature->functionType == "Nominal")
							{
								CEnumRule *pRule = new CEnumRule();
								pRule->setRuleType(4);
								pRule->setLayerName(AttrFeature->name);
								pRule->setRuleName(AttrFeature->name);
								pRule->setIsUsed(AttrFeature->useOrNot);
								for(int i = (int)(AttrFeature->value1); i<= AttrFeature->value2; i++)
									pRule->AddItem(i);
								pInstance->AddRule(pRule);

							}
							else if(AttrFeature->functionType == "Ordinal" || AttrFeature->functionType == "Cyclic")
							{
								CRangeRule *pRule = new CRangeRule();
								pRule->setRuleType(0);

								pRule->setRuleName(AttrFeature->name);
								pRule->setLayerName(AttrFeature->name);
								pRule->setIsUsed(AttrFeature->useOrNot);
								pRule->setLowUnity(AttrFeature->value1);
								pRule->setHighUnity(AttrFeature->value2);
								pRule->setLowCross(AttrFeature->value1 - AttrFeature->width1);
								pRule->setHighCross(AttrFeature->value2 + AttrFeature->width2);
								pInstance->AddRule(pRule);
							}
							else if(AttrFeature->functionType == "Continuous:bell-shape")
							{
								CRangeRule *pRule = new CRangeRule();
								pRule->setRuleType(0);
								pRule->setCurveType(0);

								pRule->setRuleName(AttrFeature->name);
								pRule->setLayerName(AttrFeature->name);
								pRule->setIsUsed(AttrFeature->useOrNot);
								pRule->setLowUnity(AttrFeature->value1);
								pRule->setHighUnity(AttrFeature->value2);
								pRule->setLowCross(AttrFeature->value1 - AttrFeature->width1);
								pRule->setHighCross(AttrFeature->value2 + AttrFeature->width2);
								pInstance->AddRule(pRule);
							}
							else if(AttrFeature->functionType == "Continuous:s-shape")
							{
								CRangeRule *pRule = new CRangeRule();
								pRule->setRuleType(0);
								pRule->setCurveType(1);

								pRule->setRuleName(AttrFeature->name);
								pRule->setLayerName(AttrFeature->name);
								pRule->setIsUsed(AttrFeature->useOrNot);
								pRule->setLowUnity(AttrFeature->value1);

								pRule->setLowCross(AttrFeature->value1 - AttrFeature->width1);

								pInstance->AddRule(pRule);
							}
							else if(AttrFeature->functionType == "Continuous:z-shape")
							{
								CRangeRule *pRule = new CRangeRule();
								pRule->setRuleType(0);
								pRule->setCurveType(2);
								pRule->setRuleName(AttrFeature->name);
								pRule->setLayerName(AttrFeature->name);
								pRule->setIsUsed(AttrFeature->useOrNot);

								pRule->setHighUnity(AttrFeature->value2);

								pRule->setHighCross(AttrFeature->value2 + AttrFeature->width2);
								pInstance->AddRule(pRule);
							}


							AttrFeature = AttrFeature->next;
						}
						pSoilType->AddInstance(pInstance);
						tempAttr = tempAttr->next;
					}
					pProject->m_pKnowledgeBase->AddSoilType(pSoilType);
					tempList = tempList->next;
				}		

					if(thePntKnowledge!=NULL)
				{
					delete thePntKnowledge;
					thePntKnowledge = NULL;
				}
				   

			}


			/*
			else if(m_knowledgebaseType==2) //line case base 
			{
			}
			else if(m_knowledgebaseType==3) //polygon case base
			{
			}
			else if(m_knowledgebaseType==4) //raster case base
			{
			}*/


		WriteProject(strProjectFullPath, pProject);
		if(pProject != NULL)
		{
			delete pProject;
			pProject = NULL;
		}        

	return true;


}





void CConvertSIEDlg::WriteProject(CString strFileName, CProject *pProject)
{
	MSXML2::IXMLDOMDocumentPtr pDoc;
	HRESULT hr;

	hr=CoInitialize(NULL);
	if(FAILED(hr))
	{
		AfxMessageBox("Error in converting project!");
		return;
	}

	hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));
	if(FAILED(hr))
	{
		AfxMessageBox("Error in converting project!");
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
		attri="5.0";
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


	//************* Create KnowledgeBase node  ****************
	MSXML2::IXMLDOMElementPtr pKnowledgebaseNode;
	pKnowledgebaseNode=pDoc->createElement("Knowledgebase");
	if(pKnowledgebaseNode!=NULL)
	{  
		pTextNode = pDoc->createTextNode(L"\n "); 
		pRootElem->appendChild(pTextNode);
		pTextNode.Release();
		pRootElem->appendChild(pKnowledgebaseNode);
	}

	MSXML2::IXMLDOMElementPtr pSoilNameNode;
	for (int n2=0;n2<pProject->m_pKnowledgeBase->getSoilTypeNum();n2++)
	{
		pSoilNameNode=pDoc->createElement("SoilType");
		if(pSoilNameNode!=NULL)
		{
			pTextNode = pDoc->createTextNode(L"\n  "); 
			pKnowledgebaseNode->appendChild(pTextNode);
			pTextNode.Release();

			attrib=pDoc->createAttribute("Name");
			if(attrib!=NULL)
			{
				CString soiltype=pProject->m_pKnowledgeBase->getSoilType(n2)->getSoilTypeName();
				_variant_t soiltype_var((LPCTSTR)soiltype);
				attrib->value=soiltype_var;
				pSoilNameNode->setAttributeNode(attrib);
				attrib.Release();
			}
			pKnowledgebaseNode->appendChild(pSoilNameNode);

			//instances
			MSXML2::IXMLDOMElementPtr pInstancesNode;
			pInstancesNode=pDoc->createElement("Instances");
			if(pInstancesNode!=NULL)
			{
				pTextNode = pDoc->createTextNode(L"\n   "); 
				pSoilNameNode->appendChild(pTextNode);
				pTextNode.Release();

				pSoilNameNode->appendChild(pInstancesNode);

				MSXML2::IXMLDOMElementPtr pInstance;
				for(int n3=0;n3<pProject->m_pKnowledgeBase->getSoilType(n2)->getInstanceNum();n3++)
				{
					pInstance=pDoc->createElement("Instance");
					if(pInstance!=NULL)
					{
						pTextNode = pDoc->createTextNode(L"\n    "); 
						pInstancesNode->appendChild(pTextNode);
						pTextNode.Release();

						attrib=pDoc->createAttribute("Name");
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
							pRuleNode=pDoc->createElement("Rule");
							if(pRuleNode!=NULL)
							{
								pTextNode = pDoc->createTextNode(L"\n     "); 
								pInstance->appendChild(pTextNode);
								pTextNode.Release();

								attrib=pDoc->createAttribute("Name");
								if(attrib!=NULL)
								{
									CString rulename=pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleName();
									_variant_t rulename_var((LPCTSTR)rulename);
									attrib->value=rulename_var;
									pRuleNode->setAttributeNode(attrib);								
								}
								attrib.Release();

								attrib=pDoc->createAttribute("UseOrNot");
								if(attrib!=NULL)
								{
									if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getIsUsed()==true)
										attrib->value="true";
									else 
										attrib->value="false";

									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDoc->createAttribute("Type");
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

								attrib=pDoc->createAttribute("DataLayer");
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
									pCurveTypeNode=pDoc->createElement("CurveType");
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
									pKeyValueNode=pDoc->createElement("KeyValue");
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

									pNodeNum=pDoc->createElement("NodeNum");
									CString str;
									str.Format("%d",m_pCurve->getKnotNum());								
									pNodeNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pNodeNum);
									pNodeNum.Release();

									pCoordinatesNode=pDoc->createElement("Coordinates");
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
									pPraseNode=pDoc->createElement("Prase");
									if(pPraseNode!=NULL)
									{
										CString phrase=m_pWord->getPhrase();
										_bstr_t phrase_bstr=phrase.AllocSysString();
										pPraseNode->text=phrase_bstr;
										pRuleNode->appendChild(pPraseNode);
									}
									pPraseNode.Release();

									MSXML2::IXMLDOMElementPtr pEnvAttriNode;
									pEnvAttriNode=pDoc->createElement("EnvAttri");
									if(pEnvAttriNode!=NULL)
									{
										CString EnvAttri=m_pWord->getEnvAttri();
										_bstr_t EnvAttri_bstr=EnvAttri.AllocSysString();
										pEnvAttriNode->text=EnvAttri_bstr;
										pRuleNode->appendChild(pEnvAttriNode);
									}
									pEnvAttriNode.Release();

									MSXML2::IXMLDOMElementPtr pWordLibNode;
									pWordLibNode=pDoc->createElement("WordLib");
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
									pXYNode=pDoc->createElement("XY");
									if(pXYNode!=NULL)
									{
										CString str;
										str.Format("%f %f",m_pPoint->getCentralX(),m_pPoint->getCentralY());
										pXYNode->text= (_bstr_t)str;
										pRuleNode->appendChild(pXYNode);
									}
									pXYNode.Release();
									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDoc->createElement("CurveType");
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
									pLWidthNode=pDoc->createElement("LWidth");
							
									if(pLWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getLWidth());
										pLWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pLWidthNode);
									}
									pLWidthNode.Release();

									MSXML2::IXMLDOMElementPtr pRWidthNode;
									pRWidthNode=pDoc->createElement("RWidth");
								
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

									pItemNum=pDoc->createElement("ItemNum");
									CString str;
									str.Format("%d",m_pEnum->getItemNum());								
									pItemNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pItemNum);
									pItemNum.Release();

									pEnumValueNode=pDoc->createElement("EnumValue");
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
					pTextNode = pDoc->createTextNode(L"\n    "); 
					pInstance->appendChild(pTextNode);
					pTextNode.Release();
					pInstance.Release();
				}
			}
			pTextNode = pDoc->createTextNode(L"\n   "); 
			pInstancesNode->appendChild(pTextNode);
			pTextNode.Release();

			pInstancesNode.Release();


			//Occurrences
			MSXML2::IXMLDOMElementPtr pOccurrencesNode;
			pOccurrencesNode=pDoc->createElement("Occurrences");
			if(pOccurrencesNode!=NULL)
			{

				pTextNode = pDoc->createTextNode(L"\n   "); 
				pSoilNameNode->appendChild(pTextNode);
				pTextNode.Release();

				pSoilNameNode->appendChild(pOccurrencesNode);

				MSXML2::IXMLDOMElementPtr pOccurrence;
				for(int n5=0;n5<pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrenceNum();n5++)
				{
					pOccurrence=pDoc->createElement("Occurrence");
					if(pOccurrence!=NULL)
					{
						pTextNode = pDoc->createTextNode(L"\n    "); 
						pOccurrencesNode->appendChild(pTextNode);
						pTextNode.Release();

						attrib=pDoc->createAttribute("Name");
						if(attrib!=NULL)
						{
							CString occurname=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getOccurrenceName();
							_variant_t occurname_var((LPCSTR)occurname);
							attrib->value=occurname_var;
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDoc->createAttribute("x");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getCentralX();
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDoc->createAttribute("y");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getCentralY();
							pOccurrence->setAttributeNode(attrib);

						}	
						attrib.Release();						
						attrib=pDoc->createAttribute("SearchDist");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getSearchDist();
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDoc->createAttribute("UseDistanceDecay");
						if(attrib!=NULL)
						{
							if(pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getUseDistSim() == true)
							  attrib->value= "true";
							else
								attrib->value = "false";
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDoc->createAttribute("DistanceDecay");
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
							pRuleNode=pDoc->createElement("Rule");
							if(pRuleNode!=NULL)
							{
								pTextNode = pDoc->createTextNode(L"\n     "); 
								pOccurrence->appendChild(pTextNode);
								pTextNode.Release();

								attrib=pDoc->createAttribute("Name");
								if(attrib!=NULL)
								{
									CString rulename=pProject->m_pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleName();
									_variant_t rulename_var((LPCSTR)rulename);
									attrib->value=rulename_var;
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDoc->createAttribute("UseOrNot");
								if(attrib!=NULL)
								{
									if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n5)->getRule(n6)->getIsUsed()==true)
										attrib->value="true";
									else 
										attrib->value="false";

									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDoc->createAttribute("Type");
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

								attrib=pDoc->createAttribute("DataLayer");
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
									pCurveTypeNode=pDoc->createElement("CurveType");
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
									pKeyValueNode=pDoc->createElement("KeyValue");
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
									pNodeNum=pDoc->createElement("NodeNum");
									CString str;
									str.Format("%d",m_pCurve->getKnotNum());								
									pNodeNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pNodeNum);
									pNodeNum.Release();

									pCoordinatesNode=pDoc->createElement("Coordinates");
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
									pPraseNode=pDoc->createElement("Prase");
									if(pPraseNode!=NULL)
									{
										CString phrase=m_pWord->getPhrase();
										_bstr_t phrase_bstr=phrase.AllocSysString();
										pPraseNode->text=phrase_bstr;
										pRuleNode->appendChild(pPraseNode);
									}
									pPraseNode.Release();


									MSXML2::IXMLDOMElementPtr pEnvAttriNode;
									pEnvAttriNode=pDoc->createElement("EnvAttri");
									if(pEnvAttriNode!=NULL)
									{
										CString EnvAttri=m_pWord->getEnvAttri();
										_bstr_t EnvAttri_bstr=EnvAttri.AllocSysString();
										pEnvAttriNode->text=EnvAttri_bstr;
										pRuleNode->appendChild(pEnvAttriNode);
									}
									pEnvAttriNode.Release();

									MSXML2::IXMLDOMElementPtr pWordLibNode;
									pWordLibNode=pDoc->createElement("WordLib");
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
									pXYNode=pDoc->createElement("XY");
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
									pCurveTypeNode=pDoc->createElement("CurveType");
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
									pLWidthNode=pDoc->createElement("LWidth");
									
									if(pLWidthNode!=NULL)
									{
										CString str;
										str.Format("%f",m_pPoint->getLWidth());
										pLWidthNode->text=(_bstr_t)str;
										pRuleNode->appendChild(pLWidthNode);
									}
									pLWidthNode.Release();

									MSXML2::IXMLDOMElementPtr pRWidthNode;
									pRWidthNode=pDoc->createElement("RWidth");
								
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

									pItemNum=pDoc->createElement("ItemNum");
									CString str;
									str.Format("%d",m_pEnum->getItemNum());								
									pItemNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pItemNum);
									pItemNum.Release();
									pEnumValueNode=pDoc->createElement("EnumValue");
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
					pTextNode = pDoc->createTextNode(L"\n    "); 
					pOccurrence->appendChild(pTextNode);
					pTextNode.Release();
					pOccurrence.Release();
				}
			}



			pTextNode = pDoc->createTextNode(L"\n   "); 
			pOccurrencesNode->appendChild(pTextNode);
			pTextNode.Release();

			pOccurrencesNode.Release();
			//Exclusions
			MSXML2::IXMLDOMElementPtr pExclusionsNode;
			pExclusionsNode=pDoc->createElement("Exclusions");
			if(pExclusionsNode!=NULL)
			{
				pTextNode = pDoc->createTextNode(L"\n   "); 
				pSoilNameNode->appendChild(pTextNode);
				pTextNode.Release();
				pSoilNameNode->appendChild(pExclusionsNode);

				MSXML2::IXMLDOMElementPtr pExclusion;
				for(int n7=0;n7<pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusionNum();n7++)
				{
					pExclusion=pDoc->createElement("Exclusion");
					if(pExclusion!=NULL)
					{
						pTextNode = pDoc->createTextNode(L"\n    "); 
						pExclusionsNode->appendChild(pTextNode);
						pTextNode.Release();

						attrib=pDoc->createAttribute("Name");
						if(attrib!=NULL)
						{
							CString excluname=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getExclusionName();
							_variant_t excluname_var((LPCSTR)excluname);
							attrib->value=excluname_var;
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDoc->createAttribute("x");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getCentralX();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDoc->createAttribute("y");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getCentralY();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						
						attrib=pDoc->createAttribute("SearchDist");
						if(attrib!=NULL)
						{
							attrib->value=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getSearchDist();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDoc->createAttribute("UseDistanceDecay");
						if(attrib!=NULL)
						{
							if(pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getUseDistSim() == true)
								attrib->value = "true";
							else 
								attrib->value = "false";
						
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDoc->createAttribute("DistanceDecay");
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
							pRuleNode=pDoc->createElement("Rule");
							if(pRuleNode!=NULL)
							{
								pTextNode = pDoc->createTextNode(L"\n     "); 
								pExclusion->appendChild(pTextNode);
								pTextNode.Release();

								attrib=pDoc->createAttribute("Name");
								if(attrib!=NULL)
								{
									CString rulename=pProject->m_pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleName();
									_variant_t rulename_var((LPCSTR)rulename);
									attrib->value=rulename_var;
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDoc->createAttribute("UseOrNot");
								if(attrib!=NULL)
								{
									if(pProject->m_pKnowledgeBase->getSoilType(n2)->getInstance(n7)->getRule(n8)->getIsUsed()==true)
										attrib->value="true";
									else 
										attrib->value="false";

									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDoc->createAttribute("Type");
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

								attrib=pDoc->createAttribute("DataLayer");
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
									pCurveTypeNode=pDoc->createElement("CurveType");
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
									pKeyValueNode=pDoc->createElement("KeyValue");
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
									pNodeNum=pDoc->createElement("NodeNum");
									CString str;
									str.Format("%d",m_pCurve->getKnotNum());								
									pNodeNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pNodeNum);
									pNodeNum.Release();

									pCoordinatesNode=pDoc->createElement("Coordinates");
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
									pPraseNode=pDoc->createElement("Prase");
									if(pPraseNode!=NULL)
									{		
										CString phrase=m_pWord->getPhrase();
										_bstr_t phrase_bstr=phrase.AllocSysString();
										pPraseNode->text=phrase_bstr;
										pRuleNode->appendChild(pPraseNode);
									}
									pPraseNode.Release();


									MSXML2::IXMLDOMElementPtr pEnvAttriNode;
									pEnvAttriNode=pDoc->createElement("EnvAttri");
									if(pEnvAttriNode!=NULL)
									{
										CString EnvAttri=m_pWord->getEnvAttri();
										_bstr_t EnvAttri_bstr=EnvAttri.AllocSysString();
										pEnvAttriNode->text=EnvAttri_bstr;
										pRuleNode->appendChild(pEnvAttriNode);
									}
									pEnvAttriNode.Release();

									MSXML2::IXMLDOMElementPtr pWordLibNode;
									pWordLibNode=pDoc->createElement("WordLib");
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
									pXYNode=pDoc->createElement("XY");
									if(pXYNode!=NULL)
									{
										CString str;
										str.Format("%f %f",m_pPoint->getCentralX(),m_pPoint->getCentralY());
										pXYNode->text= (_bstr_t)str;
										pRuleNode->appendChild(pXYNode);
									}
									pXYNode.Release();

									MSXML2::IXMLDOMElementPtr pCurveTypeNode;
									pCurveTypeNode=pDoc->createElement("CurveType");
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
									pLWidthNode=pDoc->createElement("LWidth");
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
									pRWidthNode=pDoc->createElement("RWidth");
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

									pItemNum=pDoc->createElement("ItemNum");
									CString str;
									str.Format("%d",m_pEnum->getItemNum());								
									pItemNum->text=(_bstr_t)str;
									pRuleNode->appendChild(pItemNum);
									pItemNum.Release();

									pEnumValueNode=pDoc->createElement("EnumValue");
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

					pTextNode = pDoc->createTextNode(L"\n    "); 
					pExclusion->appendChild(pTextNode);
					pTextNode.Release();
					pExclusion.Release();
				}
			}
			pTextNode = pDoc->createTextNode(L"\n   "); 
			pExclusionsNode->appendChild(pTextNode);
			pTextNode.Release();
			pExclusionsNode.Release();
		}

		pTextNode = pDoc->createTextNode(L"\n  "); 
		pSoilNameNode->appendChild(pTextNode);
		pTextNode.Release();

		pSoilNameNode.Release();
	}

	pTextNode = pDoc->createTextNode(L"\n "); 
	pKnowledgebaseNode->appendChild(pTextNode);
	pTextNode.Release();

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
	_variant_t varxml(strFileName);
	hr=pDoc->save(varxml);
	if(FAILED(hr))
	{
		AfxMessageBox("Error in converting project!");
		return;
	}
	pDoc.Release();
	CoUninitialize();
}






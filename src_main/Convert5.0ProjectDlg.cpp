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
// ConvertOldProjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "Convert5.0ProjectDlg.h"
#include "ImportOldPrj.h"
#include ".\convertoldprojectdlg.h"


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


// CConvert5ProjectDlg dialog

IMPLEMENT_DYNAMIC(CConvert5ProjectDlg, CDialog)
CConvert5ProjectDlg::CConvert5ProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConvert5ProjectDlg::IDD, pParent)
	, m_OldPrjDir(_T(""))
	, m_OldPrjFile(_T(""))	
	, m_NewPrjDir(_T(""))
	, m_NewPrjName(_T(""))
{
	
}

CConvert5ProjectDlg::~CConvert5ProjectDlg()
{
}

void CConvert5ProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_OldPrjFileFld, m_OldPrjFile);

	
	DDX_Text(pDX, IDC_OldPrjDirFld, m_OldPrjDir);
	DDX_Text(pDX, IDC_NewPrjDirFld, m_NewPrjDir);
	DDX_Text(pDX, IDC_NewPrjNameFld, m_NewPrjName);
}


BEGIN_MESSAGE_MAP(CConvert5ProjectDlg, CDialog)
	ON_BN_CLICKED(IDC_OldPrjFileBtn, OnBnClickedOldPrjFilebtn)	

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_OldDirBtn, OnBnClickedOlddirbtn)
	ON_BN_CLICKED(IDC_NewDirBtn, OnBnClickedNewdirbtn)
	//ON_BN_CLICKED(IDC_RADIO_RBR, OnBnClickedRadioRbr)
	//ON_BN_CLICKED(IDC_RADIO_CBR, OnBnClickedRadioCbr)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	
END_MESSAGE_MAP()



BOOL CConvert5ProjectDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
 //  ((CButton*)GetDlgItem(IDC_RADIO_RBR))->SetCheck(1);

	//m_PrjType = 1;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


// CConvert5ProjectDlg message handlers

void CConvert5ProjectDlg::OnBnClickedOldPrjFilebtn()
{
	UpdateData(TRUE);
	static char BASED_CODE szFilter[] = "Project Files (*.sip)|*.sip|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.sip", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if(m_OldPrjDir != "")
	{
	    CString str = m_OldPrjDir; 	
		char str1[FN_LEN];   	  
		strcpy(str1,str);   
		str1[strlen(str1)+1]=0;   
		m_ldFile.m_ofn.lpstrInitialDir  =   (LPCSTR)str1;  

	}

	if(m_ldFile.DoModal() == IDOK)
	{
		m_OldPrjFile = m_ldFile.GetFileName();

		UpdateData(FALSE);
	}	

}




void CConvert5ProjectDlg::OnBnClickedOk()
{
 	UpdateData(TRUE);


	if (m_OldPrjFile.IsEmpty())
		MessageBox("You must specify the old project File!");

	else if (m_NewPrjName.IsEmpty())
		MessageBox("You must specify the New Project Name!");
	else if (m_OldPrjDir.IsEmpty())
		MessageBox("You must specify the Old Project Directory!");
	else if (m_NewPrjDir.IsEmpty())
		MessageBox("You must specify the New Project Directory!");
	else
	{	

		if(MessageBox(" Only rule base and point case base in the old project can be converted currently. \n Mask information on different levels will be lost. \n Do you want to convert now?.", "Warning",MB_ICONQUESTION|MB_YESNO) == IDYES)
		{
		
		   bool success =ConvertSoLIM2Project(m_OldPrjDir+"\\"+m_OldPrjFile,m_NewPrjName, m_NewPrjDir);
		   if(success)
		   {
			MessageBox("Convert project successfully!");
		   }
		}		
	   
	    CDialog::OnOK();
	}

}



void CConvert5ProjectDlg::OnBnClickedOlddirbtn()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_OldPrjDir = m_getDirDlg1.GetDirectory(this, "", "Choose old project directory :");
	UpdateData(false);

	// TODO: Add your control notification handler code here
}

void CConvert5ProjectDlg::OnBnClickedNewdirbtn()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_NewPrjDir = m_getDirDlg2.GetDirectory(this, m_OldPrjDir, "Choose a directory to hold the new project:");
	UpdateData(false);

}



void CConvert5ProjectDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}




bool CConvert5ProjectDlg::ConvertSoLIM2Project(CString projectFile,CString newProjectName, CString newProjectDir)
{
	CString strProjectFullPath = newProjectDir+"\\"+newProjectName+".sip";
	CreateDirectory(newProjectDir+"\\GISData",NULL);	

	CProject* pProject = new CProject;
	pProject->setProjectName(newProjectName);
	pProject->setProjectDir(newProjectDir);
	pProject->setProjectDataType("3dr");



	CString strProjectDir;

	int tag;
	if(projectFile==""||projectFile.Right(4)!=".sip")
	{

		AfxMessageBox("The old-version SoLIM project file is invalid!");
		return false;
	}
	else
	{
		tag = projectFile.ReverseFind('\\');
		strProjectDir=projectFile.Left(tag);

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
		return false;
	}
	//create instance
	hr=pDoc.CreateInstance(_uuidof(MSXML2::DOMDocument));
	if(FAILED(hr))
	{
		AfxMessageBox("Failed to create DOM Document");
		return false;
	}
	//load xml doc
	_variant_t varxml(projectFile);
	VARIANT_BOOL varOut;
	varOut = pDoc->load(varxml);
	if(!varOut)
	{
		AfxMessageBox("Failed to Load xml file");
		return false;
	}

	//***********parse root node SoLIMProject************************
	pDoc->get_documentElement(&pRootElem);
	if(pRootElem->hasChildNodes() != -1)//-1 is True
	{
		return false;
	}
	if(pProject!=NULL)
	{
		delete pProject;
		pProject = NULL;
	}
	pProject = new CProject;
	pRootElem->get_attributes(&pRootAttr);
	MSXML2::IXMLDOMNodePtr pProjectVersionNode;
	MSXML2::IXMLDOMNodePtr pProjectNameNode;
	MSXML2::IXMLDOMNodePtr pProjectTypeNode;
	pRootAttr->get_item(0,&pProjectVersionNode);
	pRootAttr->get_item(1,&pProjectNameNode); 
	pRootAttr->get_item(2,&pProjectTypeNode);
	pProjectVersionNode->get_nodeTypedValue(&VariantValue);	
	str =(char *)(_bstr_t)VariantValue;
	if(str != "5.0")
	{
		AfxMessageBox("Project version is not 5.0.");
	}
	pProjectNameNode->get_nodeTypedValue(&VariantValue);

	str = (char *)(_bstr_t)VariantValue;
	pProject->setProjectName(str);
	pProjectTypeNode->get_nodeTypedValue(&VariantValue);
	str =(char *)(_bstr_t)VariantValue;
	pProject->setProjectDataType(str);
	pRootElem->get_childNodes(&pRootList);

	//*********** Parse database node *************	
	MSXML2::IXMLDOMNodePtr pDatabaseNode;
	if(pProject->m_pEnvDatabase != NULL)
	{
		delete pProject->m_pEnvDatabase;
		pProject->m_pEnvDatabase = NULL;
	}
	pProject->m_pEnvDatabase = new CEnvDatabase();	   
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
			CString fileName =(char *)(_bstr_t)VariantValue;


	

				CString strOldFilePath=strProjectDir+"\\"+"GIS Data\\"+fileName;

				CString strNewFilePath= newProjectDir+"\\GISData\\"+fileName;
				CopyFile(strOldFilePath,strNewFilePath,false);  //copy env layer to current project directory




		CString strLayerPath = strProjectDir +"\\GISData\\"+ (char *)(_bstr_t)VariantValue;	

		CEnvLayer *pLayer = new CEnvLayer(i,strLayerName, strLayerPath);
		if(pLayer->getLayerID() != -1)
		{ 
			pProject->m_pEnvDatabase->AddLayer(pLayer);
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
	if(pProject->m_pKnowledgeBase != NULL)
	{
		delete pProject->m_pKnowledgeBase;
		pProject->m_pKnowledgeBase = NULL;
	}
	pProject->m_pKnowledgeBase = new CKnowledgeBase();
	pRootList->get_item(1,&pKnowledgeBaseNode);	    
	long soilNum;
	MSXML2::IXMLDOMNodeListPtr pKnowledgeBaseList;
	pKnowledgeBaseNode->get_childNodes(&pKnowledgeBaseList);       
	pKnowledgeBaseList->get_length(&soilNum);		
	//traverse child nodes of knowledge base(soiltypes)
	MSXML2::IXMLDOMNodePtr pSoilTypeNode;
	MSXML2::IXMLDOMNodeListPtr pSoilTypeList;
	MSXML2::IXMLDOMNodePtr pSoilNameNode;
	MSXML2::IXMLDOMNamedNodeMapPtr pSoilTypeAttr;
	for(i=0;i<soilNum;i++)
	{
		CSoilType *pSoilType = new CSoilType;
		pKnowledgeBaseList->get_item(i,&pSoilTypeNode);
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
					CString strOldWordLib = strProjectDir +"\\"+strWordLib;
					CString strNewWordLib = newProjectDir +"\\"+strWordLib;

					CopyFile(strOldWordLib, strNewWordLib, false);

					
					pRule->setLibFileName(strNewWordLib);
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
					pRule->ComputeCentralValue(pProject->m_pEnvDatabase);
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
					CString strOldWordLib = strProjectDir +"\\"+strWordLib;
					CString strNewWordLib = newProjectDir +"\\"+strWordLib;

					CopyFile(strOldWordLib, strNewWordLib, false);

					
					pRule->setLibFileName(strNewWordLib);
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

					pRule->ComputeCentralValue(pProject->m_pEnvDatabase);

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
					CString strOldWordLib = strProjectDir +"\\"+strWordLib;
					CString strNewWordLib = newProjectDir +"\\"+strWordLib;

					CopyFile(strOldWordLib, strNewWordLib, false);

					
					pRule->setLibFileName(strNewWordLib);
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

					pRule->ComputeCentralValue(pProject->m_pEnvDatabase);

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

	pKnowledgeBaseNode.Release();
	pKnowledgeBaseList.Release();


	
	//*********parse inference node ************
	MSXML2::IXMLDOMNodePtr pInferenceNode;
	MSXML2::IXMLDOMNodeListPtr pInferenceList;
	MSXML2::IXMLDOMNodePtr pUseMaskNode,pInferMaskNode,pMaskValueNode,pInferDirNode,pInferCreateCheckNode;
	if(pProject->m_pInference != NULL)
	{
		delete pProject->m_pInference;
		pProject->m_pInference = NULL;
	}
	pProject->m_pInference = new CInference();
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
		pProject->m_pInference->setUseMask(false);
	else
		pProject->m_pInference->setUseMask(true);

	pInferMaskNode->get_nodeTypedValue(&VariantValue);	
	CString strMaskFile =(char *)(_bstr_t)VariantValue;	
	pProject->m_pInference->setMaskFile(strMaskFile);

	pMaskValueNode->get_nodeTypedValue(&VariantValue);	
	CString strMaskValue =(char *)(_bstr_t)VariantValue;	
	int iMaskValue = atoi(strMaskValue);
	pProject->m_pInference->setMaskValue(iMaskValue);

	pInferDirNode->get_nodeTypedValue(&VariantValue);	
	CString strInferDir =(char *)(_bstr_t)VariantValue;
	pProject->m_pInference->setResultDir(strInferDir);

	pInferCreateCheckNode->get_nodeTypedValue(&VariantValue);	
	CString strCreateCheck =(char *)(_bstr_t)VariantValue;	
	if(strCreateCheck=="false")	  
		pProject->m_pInference->setCreateCheckFile(false);
	else 
		pProject->m_pInference->setCreateCheckFile(true);
	pProject->m_pInference->setEnvDatabase(pProject->m_pEnvDatabase);
	pProject->m_pInference->setKnowledgeBase(pProject->m_pKnowledgeBase);
	pInferenceNode.Release();
	pInferenceList.Release();
	pUseMaskNode.Release();
	pMaskValueNode.Release();
	pInferMaskNode.Release();
	pInferDirNode.Release();
	pInferCreateCheckNode.Release();

	pRootList.Release();
	pProjectNameNode.Release();
	pProjectVersionNode.Release();
	pProjectTypeNode.Release();
	pRootElem.Release();
	pRootAttr.Release();
	pDoc.Release();
	CoUninitialize();






		

		WriteProject(strProjectFullPath, pProject);
		if(pProject != NULL)
		{
			delete pProject;
			pProject = NULL;
		}        

	}	
	return true;

}



void CConvert5ProjectDlg::WriteProject(CString strFileName, CProject *pProject)
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
		AfxMessageBox("Error in converting project!");
		return;
	}

	hrKnowledge=pDocKnowledge.CreateInstance(__uuidof(MSXML2::DOMDocument));
	if(FAILED(hrKnowledge))
	{
		AfxMessageBox("Error in converting project!");
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

	if(pProject->m_strKnowledgeBaseFile == "")
		pProject->m_strKnowledgeBaseFile= "KnowledgeBase.xml";






		_variant_t varxmlKnowledge(extractPathName(strFileName) + "\\" + pProject->m_strKnowledgeBaseFile);
	hrKnowledge = pDocKnowledge->save(varxmlKnowledge);
	if(FAILED(hrKnowledge))
	{
		AfxMessageBox("Error in converting project!");
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

		CString knowledgeBaseFile = pProject->m_strKnowledgeBaseFile;
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
		AfxMessageBox("Error in converting project!");
		return;
	}
	pDoc.Release();
	CoUninitialize();
}




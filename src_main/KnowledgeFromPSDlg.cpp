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
// KnowledgeFromPS.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "KnowledgeFromPSDlg.h"

#include "DataFile.h"



#include <vector>


#include  <msxml2.h>      
#import   <msxml4.dll>

#include "RangeRule.h"
#include "WordRule.h"
#include "FreehandRule.h"
#include "PointRule.h"
#include "EnumRule.h"

#include <iostream>
#include <fstream>
#include <cstring>


using namespace std;


// CKnowledgeFromPSDlg dialog

IMPLEMENT_DYNAMIC(CKnowledgeFromPSDlg, CDialog)
CKnowledgeFromPSDlg::CKnowledgeFromPSDlg(CWnd* pParent /*=NULL*/)
: CDialog(CKnowledgeFromPSDlg::IDD, pParent)
, m_strKBFile(_T(""))
{
	m_iSoilNum  = 0;
	m_pEnvDatabase = NULL;
	m_curRow = -1;
	m_curCol = -1;

}

CKnowledgeFromPSDlg::~CKnowledgeFromPSDlg()
{
	if(	m_pEnvDatabase != NULL)
	{
		delete m_pEnvDatabase;
		m_pEnvDatabase = NULL;
	}

}

void CKnowledgeFromPSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_GridControl(pDX, IDC_GRID_PATTERN_TABLE, m_gridPatternTable);
	DDX_Control(pDX, IDC_LIST_LAYER, m_LayerList);
	DDX_Text(pDX, IDC_EDIT_KB_FILE, m_strKBFile);
}


BEGIN_MESSAGE_MAP(CKnowledgeFromPSDlg, CDialog)

	ON_BN_CLICKED(IDOK, OnBnClickedOk)

	ON_BN_CLICKED(IDC_BUTTON_ADD_LAYER, OnBnClickedButtonAddLayer)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_LAYER, OnBnClickedButtonRemoveLayer)

	ON_BN_CLICKED(IDC_BUTTON_LOAD_PATTERN, OnBnClickedButtonLoadPattern)

	ON_NOTIFY(NM_CLICK, IDC_GRID_FIELD_SAMPLE, OnGridClick)
	ON_BN_CLICKED(IDC_BUTTON_KBFILE, OnBnClickedButtonKbfile)
END_MESSAGE_MAP()


// CKnowledgeFromPSDlg message handlers


BOOL CKnowledgeFromPSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CDC* pDC = GetDC();
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	this->m_LayerList.InsertColumn(0, "Name", LVCFMT_LEFT, 20*tm.tmAveCharWidth, 0);
	this->m_LayerList.InsertColumn(1, "File", LVCFMT_LEFT, 70*tm.tmAveCharWidth, 1);
	this->m_LayerList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);



	if(m_pEnvDatabase==NULL)
		m_pEnvDatabase = new CEnvDatabase();
	m_LayerList.DeleteAllItems();



	m_gridPatternTable.SetFont(GetFont());
	m_gridPatternTable.SetColumnCount(3);    
	m_gridPatternTable.SetRowCount(100);    
	m_gridPatternTable.SetFixedRowCount(1);  

	m_gridPatternTable.SetColumnWidth(0, 100);

	m_gridPatternTable.SetColumnWidth(1, 100);
	m_gridPatternTable.SetColumnWidth(2, 100);



	m_gridPatternTable.SetItemText(0,0,"Actual X");

	m_gridPatternTable.SetItemText(0,1,"Actual Y"); 

	m_gridPatternTable.SetItemText(0,2,"Soil Type");

	return true;

}




void CKnowledgeFromPSDlg::OnBnClickedButtonAddLayer()
{

	UpdateData(TRUE);


	CString layerFileName;
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_ALLOWMULTISELECT |OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Add Layer(s)";
	CString   strFileNames;  
	m_ldFile.m_ofn.lpstrFile=strFileNames.GetBuffer(4000); 
	m_ldFile.m_ofn.nMaxFile= 4000 ;  
	if(m_ldFile.DoModal() == IDOK)
	{
		POSITION mPos= m_ldFile.GetStartPosition(); 
		while(mPos!=NULL) 
		{         
			layerFileName = m_ldFile.GetNextPathName(mPos);         
			if(layerFileName == "")
			{
				MessageBox("No input file.");
				return;
			}
			CString strLayerName = "untitled";
			CEnvLayer *pLayer = new CEnvLayer(m_pEnvDatabase->getLayerNum(),strLayerName,  layerFileName );
			if(pLayer->getLayerID() != -1)
			{ 
				strLayerName.Format("%s",pLayer->m_pTheData->getDataName());
				pLayer->setLayerName(strLayerName);
				int i=0;
				for(i=0;i<m_pEnvDatabase->getLayerNum();i++) 
				{
					if((m_pEnvDatabase->getLayer(i)->getLayerName())==strLayerName)
					{
						AfxMessageBox("The name of the layer is the same with one existing layer. Please use another layer.");
						delete pLayer;
						pLayer = NULL;
						break;
					}
				}
				if(i!=m_pEnvDatabase->getLayerNum())
					continue;

				if(m_pEnvDatabase->AddLayer(pLayer))   //if add layer successfully
				{
					int count = m_LayerList.GetItemCount();
					m_LayerList.InsertItem(LVIF_TEXT|LVIF_STATE, count, strLayerName, LVIS_SELECTED, LVIS_SELECTED, 0, 0);
					m_LayerList.SetItemText(count, 1, layerFileName);
				}
			}
			else 
			{
				delete pLayer;
				pLayer = NULL;
				CString msg;
				msg.Format("Error in reading %s. The layer will not be added",layerFileName);
				AfxMessageBox(msg);
			}	

		}

	}

}

void CKnowledgeFromPSDlg::OnBnClickedButtonRemoveLayer()
{

	UpdateData(TRUE);
	POSITION pos = m_LayerList.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nItem = m_LayerList.GetNextSelectedItem(pos);
		if(nItem < 0)
			continue;
		CString strLayerName = m_LayerList.GetItemText(nItem, 0);
		for(int i=0;i<m_pEnvDatabase->getLayerNum();i++) 	
			if((m_pEnvDatabase->getLayer(i)->getLayerName())==strLayerName)
				m_pEnvDatabase->DeleteLayer(i);
		m_LayerList.DeleteItem(nItem);
	}
}

void CKnowledgeFromPSDlg::OnBnClickedButtonLoadPattern()
{

	CString strReportFileName;
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "csv Files (*.csv)|*.csv|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.csv", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){

		for(int i=0; i<m_gridPatternTable.GetRowCount() - 1; i++)
		{
			m_gridPatternTable.DeleteRow(2);

		}
		this->m_vecPatternSamples.clear();
		for(int i=0; i<200; i++)
		{
			m_gridPatternTable.InsertRow("",2);
		}


		strReportFileName  = m_ldFile.GetPathName();
		UpdateData(FALSE);



	    CDataFile *hCSV = new CDataFile((LPCTSTR) strReportFileName, DF::RF_READ_AS_STRING);
	


		int ix = hCSV->GetVariableIndex("Actual X"); 
		if(ix == -1)
		{
			MessageBox("There should be a \"Actual X\" column!");
			
		if(hCSV != NULL)
		{

		     delete hCSV;
			 hCSV = NULL;
		}
			return;
		}
		
		int iy = hCSV->GetVariableIndex("Actual Y");
	    if(iy == -1)
		{
			MessageBox("There should be a \"Actual Y\" column!");
			
		if(hCSV != NULL)
		{

		     delete hCSV;
			 hCSV = NULL;
		}
			return;
		}
		int iSoil = hCSV->GetVariableIndex("Soil Type");
				if(iSoil == -1)
		{
			MessageBox("There should be a \"Soil Type\" column!");
			
		if(hCSV != NULL)
		{

		     delete hCSV;
			 hCSV = NULL;
		}
			return;
		}


	

        int sampleCount = hCSV->GetNumberOfSamples(0)-1;



		for(int i = 0; i< sampleCount; i++)
		{

				if(i + 3 > m_gridPatternTable.GetRowCount())
				m_gridPatternTable.InsertRow("",m_gridPatternTable.GetRowCount());
	       char strX[100];
		   char strY[100];
		   char strSoilType[100];
		   hCSV->GetData(ix, i, strX);
		   hCSV->GetData(iy, i, strY);
		   hCSV->GetData(iSoil, i, strSoilType);

		   m_gridPatternTable.SetItemText(i+1,0,strX);

			m_gridPatternTable.SetItemText(i+1,1,strY); 
		
			m_gridPatternTable.SetItemText(i+1,2,strSoilType); 




		}


		if(hCSV != NULL)
		{

		     delete hCSV;
			 hCSV = NULL;
		}



		/*ifstream tfile(strReportFileName.GetBuffer());

		string line;

		getline(tfile, line);
		getline(tfile, line);

		int curIndex = 1;
		while(!tfile.eof())
		{

			if(curIndex + 2 > m_gridPatternTable.GetRowCount())
				m_gridPatternTable.InsertRow("",m_gridPatternTable.GetRowCount());
			getline(tfile, line);



			CString s;
			s.Format("%s", line.c_str());

			if(s.IsEmpty()) 
				continue;  

			int i = 0;  
			CString token = s.Tokenize(_T(",\t"), i);  
			token = s.Tokenize(_T(",\t"), i);
			token = s.Tokenize(_T(",\t"), i);

			token = s.Tokenize(_T(",\t"), i);
			token = s.Tokenize(_T(",\t"), i);
			token = s.Tokenize(_T(",\t"), i);
			token = s.Tokenize(_T(",\t"), i);
			token = s.Tokenize(_T(",\t"), i);
			token = s.Tokenize(_T(",\t"), i);
			token = s.Tokenize(_T(",\t"), i);

			CString strX = token = s.Tokenize(_T(",\t"), i);;
			m_gridPatternTable.SetItemText(curIndex,0,strX);
			CString strY = s.Tokenize(_T(",\t"), i);
			m_gridPatternTable.SetItemText(curIndex,1,strY); 
			CString strSoilType = s.Tokenize(_T(",\t"), i);
			m_gridPatternTable.SetItemText(curIndex,2,strSoilType); 

			curIndex ++;


		}

		tfile.close();


*/



		m_gridPatternTable.Refresh();

	}
}


/*
void PSFieldSampleDlg::OnBnClickedButtonDeletePattern()
{

vector<int> vecSelectedRows;

int row = m_gridPatternTable.GetRowCount();

int col = m_gridPatternTable.GetColumnCount();


for(int i=0; i<row; i++)
{
bool selected = false;
for(int j=0; j<col; j++)
{

if(m_gridPatternTable.IsCellSelected(i, j) == true)
{
selected = true;
break;
}
}
if(selected == true)
vecSelectedRows.push_back(i);
}


int numOfSelected = vecSelectedRows.size();

for(int i=0; i<numOfSelected; i++)
{
m_gridPatternTable.DeleteRow(vecSelectedRows[i] - i);

m_vecPatternSamples.erase(m_vecPatternSamples.begin()+i);
}

int nRow = m_gridPatternTable.GetRowCount();
m_gridPatternTable.InsertRow("",nRow);

m_gridPatternTable.Refresh();
}

void PSFieldSampleDlg::OnBnClickedButtonInputSample()
{
CPatternFieldSampleDlg dlg;

if(m_curRow < 1)
{
MessageBox("Please first select one pattern.");
return;
}


dlg.m_numOfSamples = this->m_vecPatternSamples[m_curRow -1].size();

//dlg.m_gridFieldSample.SetFont(GetFont());

dlg.m_gridFieldSample.SetColumnCount(3);    
dlg.m_gridFieldSample.SetRowCount(10);    
dlg.m_gridFieldSample.SetFixedRowCount(1);  

dlg.m_gridFieldSample.SetColumnWidth(0, 100);
dlg.m_gridFieldSample.SetColumnWidth(1, 100);
dlg.m_gridFieldSample.SetColumnWidth(2, 200);



//m_gridPatternTable.SetFixedColumnCount(1);


dlg.m_gridFieldSample.SetItemText(0,0,"X");

dlg.m_gridFieldSample.SetItemText(0,1,"Y");


dlg.m_gridFieldSample.SetItemText(0,2,"Soil Type");

for(int i=0; i<dlg.m_numOfSamples; i++)
{
CString str;
str.Format("%f", m_vecPatternSamples[m_curRow-1][i].x);
dlg.m_gridFieldSample.SetItemText(i+1, 0, str);

str.Format("%f", m_vecPatternSamples[m_curRow-1][i].y);
dlg.m_gridFieldSample.SetItemText(i+1, 1, str);

dlg.m_gridFieldSample.SetItemText(i+1, 2, m_vecPatternSamples[m_curRow-1][i].soiltype);

}
if(dlg.DoModal() == IDOK)
{


this->m_vecPatternSamples[m_curRow - 1].clear();

for(int i=0; i<dlg.m_numOfSamples; i++)
{

CString strX = dlg.m_gridFieldSample.GetItemText(i+1, 0);
CString strY = dlg.m_gridFieldSample.GetItemText(i+1, 1);
CString strSoilName = dlg.m_gridFieldSample.GetItemText(i+1, 2);

FieldSample sample;
sample.x = atof(strX.GetBuffer());
sample.y = atof(strY.GetBuffer());
sample.soiltype = strSoilName;


this->m_vecPatternSamples[m_curRow - 1].push_back(sample);

}

int maxNum = 0;

int index = 0;

for(int i=0; i<dlg.m_numOfSamples; i++)
{
int curNum = 0;


CString strSoilName = dlg.m_gridFieldSample.GetItemText(i+1, 2);


for(int j=0; j<dlg.m_numOfSamples; j++)

if(strSoilName == dlg.m_gridFieldSample.GetItemText(j+1, 2))
curNum ++;


if(curNum > maxNum)
{
maxNum = curNum;
index = i;
}           


}

CString strMajority;

if(maxNum > (double)dlg.m_numOfSamples/2)

strMajority = dlg.m_gridFieldSample.GetItemText(index+1, 2);
else 
strMajority = "undetermined";

this->m_gridPatternTable.SetItemText(m_curRow, 2, strMajority);

this->m_gridPatternTable.Refresh();


}


}
*/

void CKnowledgeFromPSDlg::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	m_curRow = pItem->iRow;
	m_curCol = pItem->iColumn;

}


void CKnowledgeFromPSDlg::OnBnClickedButtonKbfile()
{
	static char BASED_CODE szFilter[] = "xml Files (*.xml)|*.xml|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(FALSE, "xml", ".xml", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK){
		this->m_strKBFile = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}


void CKnowledgeFromPSDlg::SaveKnowledgeFile(CString strKBFile, CKnowledgeBase *pKnowledgeBase)
{
	//************* Create KnowledgeBase file****************


	MSXML2::IXMLDOMDocumentPtr pDocKnowledge;
	HRESULT hrKnowledge;

	MSXML2::IXMLDOMAttributePtr attrib;

	_variant_t vNullVal;
	vNullVal.vt = VT_NULL;

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



	MSXML2::IXMLDOMTextPtr   pTextNode; 

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
	for (int n2=0;n2<pKnowledgeBase->getSoilTypeNum();n2++)
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
				CString soiltype=pKnowledgeBase->getSoilType(n2)->getSoilTypeName();
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
				for(int n3=0;n3<pKnowledgeBase->getSoilType(n2)->getInstanceNum();n3++)
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
							CString instancename=pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getInstanceName();
							_variant_t instancename_var((LPCTSTR)instancename);
							attrib->value=instancename_var;
							pInstance->setAttributeNode(attrib);

						}
						attrib.Release();
						pInstancesNode->appendChild(pInstance);

						MSXML2::IXMLDOMElementPtr pRuleNode;
						for(int n4=0;n4<pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRuleNum();n4++)
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
									CString rulename=pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleName();
									_variant_t rulename_var((LPCTSTR)rulename);
									attrib->value=rulename_var;
									pRuleNode->setAttributeNode(attrib);								
								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("UseOrNot");
								if(attrib!=NULL)
								{
									if(pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getIsUsed()==true)
										attrib->value="true";
									else 
										attrib->value="false";

									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("Type");
								if(attrib!=NULL)
								{
									int a=pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType();
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
									CString datalayername=pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getLayerName();
									_variant_t datalayername_var((LPCSTR)datalayername);
									attrib->value=datalayername_var;
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();
								pInstance->appendChild(pRuleNode);



								//Range
								if(pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 0)
								{
									CRangeRule *m_pRange;;
									m_pRange=(CRangeRule *)(pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));

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
								if(pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 1)
								{
									CFreehandRule *m_pCurve; 
									m_pCurve= (CFreehandRule *)(pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));
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
								if(pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 2)
								{
									CWordRule *m_pWord=new CWordRule;
									m_pWord=static_cast<CWordRule *>(pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));
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
								if(pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 3)
								{
									CPointRule *m_pPoint=new CPointRule;
									m_pPoint=static_cast<CPointRule *>(pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));
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
								if(pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4)->getRuleType()== 4)
								{
									CEnumRule *m_pEnum=new CEnumRule;
									m_pEnum=static_cast<CEnumRule *>(pKnowledgeBase->getSoilType(n2)->getInstance(n3)->getRule(n4));
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
				for(int n5=0;n5<pKnowledgeBase->getSoilType(n2)->getOccurrenceNum();n5++)
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
							CString occurname=pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getOccurrenceName();
							_variant_t occurname_var((LPCSTR)occurname);
							attrib->value=occurname_var;
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("x");
						if(attrib!=NULL)
						{
							attrib->value=pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getCentralX();
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("y");
						if(attrib!=NULL)
						{
							attrib->value=pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getCentralY();
							pOccurrence->setAttributeNode(attrib);

						}	
						attrib.Release();						
						attrib=pDocKnowledge->createAttribute("SearchDist");
						if(attrib!=NULL)
						{
							attrib->value=pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getSearchDist();
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("UseDistanceDecay");
						if(attrib!=NULL)
						{
							if(pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getUseDistSim() == true)
								attrib->value= "true";
							else
								attrib->value = "false";
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("DistanceDecay");
						if(attrib!=NULL)
						{
							attrib->value=pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getDistDecayFactor();
							pOccurrence->setAttributeNode(attrib);

						}
						attrib.Release();
						pOccurrencesNode->appendChild(pOccurrence);

						MSXML2::IXMLDOMElementPtr pRuleNode;
						for(int n6=0;n6<pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRuleNum();n6++)
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
									CString rulename=pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleName();
									_variant_t rulename_var((LPCSTR)rulename);
									attrib->value=rulename_var;
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("UseOrNot");
								if(attrib!=NULL)
								{
									if(pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getIsUsed()==true)
										attrib->value="true";
									else 
										attrib->value="false";

									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("Type");
								if(attrib!=NULL)
								{
									int a=pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType();
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
									CString layername=pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getLayerName();
									_variant_t layername_var((LPCSTR)layername);
									attrib->value=layername_var;
									pRuleNode->setAttributeNode(attrib);

								}	
								attrib.Release();
								pOccurrence->appendChild(pRuleNode);


								//Range
								if(pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()==0)
								{
									CRangeRule *m_pRange = new CRangeRule;
									m_pRange=static_cast<CRangeRule *>(pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));

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
								if(pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()== 1)
								{
									CFreehandRule *m_pCurve = new CFreehandRule;
									m_pCurve= static_cast<CFreehandRule *>(pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));
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
								if(pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()== 2)
								{
									CWordRule *m_pWord=new CWordRule;
									m_pWord=static_cast<CWordRule *>(pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));
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
								if(pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()== 3)
								{
									CPointRule *m_pPoint=new CPointRule;
									m_pPoint=static_cast<CPointRule *>(pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));
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
								if(pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6)->getRuleType()== 4)
								{
									CEnumRule *m_pEnum=new CEnumRule;
									m_pEnum=static_cast<CEnumRule *>(pKnowledgeBase->getSoilType(n2)->getOccurrence(n5)->getRule(n6));
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
				for(int n7=0;n7<pKnowledgeBase->getSoilType(n2)->getExclusionNum();n7++)
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
							CString excluname=pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getExclusionName();
							_variant_t excluname_var((LPCSTR)excluname);
							attrib->value=excluname_var;
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("x");
						if(attrib!=NULL)
						{
							attrib->value=pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getCentralX();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("y");
						if(attrib!=NULL)
						{
							attrib->value=pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getCentralY();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();

						attrib=pDocKnowledge->createAttribute("SearchDist");
						if(attrib!=NULL)
						{
							attrib->value=pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getSearchDist();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("UseDistanceDecay");
						if(attrib!=NULL)
						{
							if(pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getUseDistSim() == true)
								attrib->value = "true";
							else 
								attrib->value = "false";

							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						attrib=pDocKnowledge->createAttribute("DistanceDecay");
						if(attrib!=NULL)
						{
							attrib->value=pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getDistDecayFactor();
							pExclusion->setAttributeNode(attrib);

						}
						attrib.Release();
						pExclusionsNode->appendChild(pExclusion);

						MSXML2::IXMLDOMElementPtr pRuleNode;
						for(int n8=0;n8<pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRuleNum();n8++)
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
									CString rulename=pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleName();
									_variant_t rulename_var((LPCSTR)rulename);
									attrib->value=rulename_var;
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("UseOrNot");
								if(attrib!=NULL)
								{
									if(pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getIsUsed()==true)
										attrib->value="true";
									else 
										attrib->value="false";

									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();

								attrib=pDocKnowledge->createAttribute("Type");
								if(attrib!=NULL)
								{
									int a=pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType();
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
									CString datalayername=pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getLayerName();
									_variant_t datalayername_var((LPCSTR)datalayername);
									attrib->value=datalayername_var;
									pRuleNode->setAttributeNode(attrib);

								}
								attrib.Release();
								pExclusion->appendChild(pRuleNode);


								//Range
								if(pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 0)
								{
									CRangeRule *m_pRange = new CRangeRule;
									m_pRange=static_cast<CRangeRule *>(pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));

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
								if(pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 1)
								{
									CFreehandRule *m_pCurve = new CFreehandRule;
									m_pCurve= static_cast<CFreehandRule *>(pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));

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
								if(pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 2)
								{
									CWordRule *m_pWord=new CWordRule;
									m_pWord=static_cast<CWordRule *>(pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));
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
								if(pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 3)
								{
									CPointRule *m_pPoint=new CPointRule;
									m_pPoint=static_cast<CPointRule *>(pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));
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
								if(pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8)->getRuleType()== 4)
								{
									CEnumRule *m_pEnum=new CEnumRule;
									m_pEnum=static_cast<CEnumRule *>(pKnowledgeBase->getSoilType(n2)->getExclusion(n7)->getRule(n8));
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


	_variant_t varxmlKnowledge(strKBFile);
	hrKnowledge = pDocKnowledge->save(varxmlKnowledge);
	if(FAILED(hrKnowledge))
	{
		AfxMessageBox("Error in saving project!");
		return;
	}
	pDocKnowledge.Release();
	CoUninitialize();




}




void CKnowledgeFromPSDlg::OnBnClickedOk()
{

	//OnOK();

	int row = m_gridPatternTable.GetRowCount();

	int col = m_gridPatternTable.GetColumnCount();

	vector<CString> vecSoilSequence;


	for(int i=1; i<row; i++)
	{
		CString soilName = m_gridPatternTable.GetItemText(i, 2);

		if(soilName != "")
		{
			bool existing = false;
			int id = -1;
			for(int k=0; k<vecSoilSequence.size(); k++)
			{
				if(soilName == vecSoilSequence[k])
				{
					existing = true;
					id = k;
					break;
				}
			}
			if(existing == true)
			{

				FieldSample OnePoint;
				OnePoint.x = atof(m_gridPatternTable.GetItemText(i, 0));
				OnePoint.y = atof(m_gridPatternTable.GetItemText(i, 1));
				OnePoint.soiltype = soilName;		
				m_vecPatternSamples[id].push_back(OnePoint);
			}

			if(existing == false)
			{
				vecSoilSequence.push_back(soilName);
				vector<FieldSample> OneSoilSamples;

				FieldSample OnePoint;
				OnePoint.x = atof(m_gridPatternTable.GetItemText(i, 0));
				OnePoint.y = atof(m_gridPatternTable.GetItemText(i, 1));
				OnePoint.soiltype = soilName;		
				OneSoilSamples.push_back(OnePoint);
				m_vecPatternSamples.push_back(OneSoilSamples);
			}
		}

	}




	int numOfLayers = m_pEnvDatabase->getLayerNum();

	CKnowledgeBase* pKnowledgeBase = new CKnowledgeBase();


	int numOfSoils = vecSoilSequence.size();





	for(int i=0; i<numOfSoils; i++)
	{
		//calculate how many instances are needed and store the field samples for each instance
		vector<vector<FieldSample> > vecInstanceSamples;

		vecInstanceSamples.push_back(m_vecPatternSamples[i]);


		CSoilType *pSoil = new CSoilType();
		pSoil->setSoilTypeName(vecSoilSequence[i]);

		int numOfInstances = vecInstanceSamples.size();

		for(int j=0; j<numOfInstances; j++)
		{
			CInstance *pInstance = new CInstance();
			CString strInstName;
			strInstName.Format("instance%d", j+1);
			pInstance->setInstanceName(strInstName);


			for(int k=0;k<numOfLayers; k++)
			{
				CRangeRule *pRule = new CRangeRule();
				pRule->setRuleName(m_pEnvDatabase->getLayer(k)->getLayerName());
				pRule->setLayerName(m_pEnvDatabase->getLayer(k)->getLayerName());

				double x = vecInstanceSamples[j][0].x;
				double y = vecInstanceSamples[j][0].y;

				float value = m_pEnvDatabase->getLayer(k)->getData(x, y);

				float min = value;
				float max = value;

				for(int t=0; t<vecInstanceSamples[j].size(); t++)
				{
					x = vecInstanceSamples[j][t].x;
					y = vecInstanceSamples[j][t].y;

					value = m_pEnvDatabase->getLayer(k)->getData(x, y);
					if(fabs(value - NODOUBLEVALUE) > VERYSMALL)
					{
					  if(value < min)
						min = value;
					  if(value > max)
						max = value;
					}

				}

				pRule->setCurveType(0);
				pRule->setLowUnity(min);
				pRule->setHighUnity(max);
				pRule->setLowCross(min - 1.33 * m_pEnvDatabase->getLayer(k)->m_pTheData->getDataStd());
				pRule->setHighCross(max + 1.33 * m_pEnvDatabase->getLayer(k)->m_pTheData->getDataStd());

				pInstance->AddRule(pRule);
			}


			pSoil->AddInstance(pInstance);
		}
		pKnowledgeBase->AddSoilType(pSoil);       
	}

	//modify the knowledge base 

	for(int i=0;i<numOfLayers; i++)
	{	
		int soilIndexMin = 0;
		int soilIndexMax = 0;
		int instIndexMin = 0;
		int instIndexMax = 0;

		float min = LARGESTFLOAT;
		float max = SMALLESTFLOAT;

		for(int j=0; j<pKnowledgeBase->getSoilTypeNum(); j++)
		{
			for(int k=0; k<pKnowledgeBase->getSoilType(j)->getInstanceNum(); k++)
			{
				CRangeRule * pRule = (CRangeRule *)(pKnowledgeBase->getSoilType(j)->getInstance(k)->getRule(i));
				if(pRule->getLowUnity() <min)
				{
					min = pRule->getLowUnity();
					soilIndexMin = j;
					instIndexMin = k;
				}

				if(pRule->getHighUnity() > max)
				{
					max = pRule->getHighUnity();
					soilIndexMax = j;
					instIndexMax = k;
				}


			}
		}

		if(soilIndexMin != soilIndexMax || instIndexMin != instIndexMax)
		{

			CRangeRule * pRule = (CRangeRule *)(pKnowledgeBase->getSoilType(soilIndexMin)->getInstance(instIndexMin)->getRule(i));
			pRule->setCurveType(2);
			pRule->setLowUnity(NODOUBLEVALUE);
			pRule->setLowCross(NODOUBLEVALUE);

			pRule = (CRangeRule *)(pKnowledgeBase->getSoilType(soilIndexMax)->getInstance(instIndexMax)->getRule(i));
			pRule->setCurveType(1);
			pRule->setHighUnity(NODOUBLEVALUE);
			pRule->setHighCross(NODOUBLEVALUE);
		}
	}


	SaveKnowledgeFile(this->m_strKBFile, pKnowledgeBase);


	if(pKnowledgeBase!=NULL)
	{
		delete pKnowledgeBase;
		pKnowledgeBase = NULL;
	}

	MessageBox("Knowledge extraction from purposive sampling is done!");

	OnOK();



}

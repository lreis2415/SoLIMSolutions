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
// PrunBranchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "PrunBranchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrunBranchDlg dialog


CPrunBranchDlg::CPrunBranchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrunBranchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrunBranchDlg)
	m_InputFileName = _T("");
	m_OutputFileName = _T("");
	m_MinLength = 0;
	m_FlowDirFileName = _T("");
	m_BackgroundV = 0.0f;
	//}}AFX_DATA_INIT
}


void CPrunBranchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrunBranchDlg)
	DDX_Text(pDX, IDC_InputFld, m_InputFileName);
	DDX_Text(pDX, IDC_OutputFld, m_OutputFileName);
	DDX_Text(pDX, IDC_MinLengthFld, m_MinLength);
	DDX_Text(pDX, IDC_FlowDirFld, m_FlowDirFileName);
	DDX_Text(pDX, IDC_BackgroundVFld, m_BackgroundV);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrunBranchDlg, CDialog)
	//{{AFX_MSG_MAP(CPrunBranchDlg)
	ON_BN_CLICKED(IDC_InputBtn, OnInputBtn)
	ON_BN_CLICKED(IDC_OutputBtn, OnOutputBtn)
	ON_BN_CLICKED(IDC_FlowDirBtn, OnFlowDirBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrunBranchDlg message handlers

void CPrunBranchDlg::OnInputBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_InputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}		
}

void CPrunBranchDlg::OnFlowDirBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_FlowDirFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void CPrunBranchDlg::OnOutputBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(FALSE, "3dr", ".3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK){
		m_OutputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}		
}

void CPrunBranchDlg::OnOK(){
	// TODO: Add extra validation here
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	if(m_InputFileName.IsEmpty()){
		MessageBox("No network file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if(m_FlowDirFileName.IsEmpty()){
		MessageBox("No flow direction file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if(m_OutputFileName.IsEmpty()){
		MessageBox("No output file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if(m_MinLength<1){
		MessageBox("Minumum length must be greater than or equal to 1 pixel.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}

	CThreeDR* theInput = new CThreeDR;
	int readFlag = theInput->readin(m_InputFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the network file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	CThreeDR* theFlowDir = new CThreeDR;
	readFlag = theFlowDir->readin(m_FlowDirFileName.GetBuffer(1), 0);
	if(readFlag != 0){
		MessageBox("Cannot open the flow direction file or file format has errors.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}

	float* dataBuf = theInput->getDataBuf();
	float* flowDirBuf = theFlowDir->getDataBuf();
	int nrow = theInput->getNumberOfRows();
	int ncol = theInput->getNumberOfColumns();
	int numPixels = nrow*ncol;

	bool* idBuf = new bool[nrow*ncol];
	unsigned char* patchIdBuf = new unsigned char[nrow*ncol];
	for(int i=0; i<numPixels; i++){
		if(dataBuf[i]!=m_BackgroundV){
			idBuf[i] = true;
			patchIdBuf[i] = 0;
		}
		else{
			idBuf[i] = false;
			patchIdBuf[i] = 4;
		}
	}
	for(i = 0; i<numPixels; i++){
		if(flowDirBuf[i] == 0){ //the outlet of a network
			//1. build the tree
			Tree<IINode>* streamNet = new Tree<IINode>;
			streamNet->addHead(new IINode(i, dataBuf[i]));
			streamNet->thisList->addTail(streamNet->head);
			idBuf[i] = false;
			IINode* temp = streamNet->thisList->first;
			while(temp != NULL){
				int head = findStreamSectionHead(flowDirBuf, dataBuf, nrow, ncol, (int)dataBuf[temp->i1]);
				while(true){ //find children of this link
					int flowInPos = flowInLink(flowDirBuf, idBuf, nrow, ncol, head);
					if(flowInPos<0) break; //no more child
					IINode* newLink = new IINode(flowInPos, dataBuf[flowInPos]);
					temp->children->addTail(newLink);
					idBuf[flowInPos] = false;
				}
				streamNet->thisList->addTail(temp->children); //add the entire children list to thisList for scanning
				temp = temp->next;
			}
		
			//2. prun the tree
			int num = 0;
			do{
				num = 0;
				temp = streamNet->thisList->first;
				while(temp!=NULL){
					IINode* temp2 = temp->children->first;
					if(temp2!=NULL){
						IINode* theNext = temp->children->last->next;
						while(temp2!= theNext){//Note: if the network only has one link, keep it.
							if(temp2->children->first==NULL){ //external link
								IINode* holder = temp2->next;
								Patch<float>* thePatch = new Patch<float>(temp2->i1, dataBuf, patchIdBuf, nrow, ncol, true);
								if(thePatch->numPixels<=m_MinLength){
									thePatch->assignValue(dataBuf, m_BackgroundV);
									//temp->children->avoidHeadTail(temp2); //move head and/or tail, but DO NOT delete it
									temp->children->removeNode(temp2); //delete it
									num++;
								}
								else{ //reset the patchIdBuf, because need to build patch for the same link later
									PatchPixel* tempPixel = thePatch->first;
									while(tempPixel!=NULL){
										patchIdBuf[tempPixel->pos] = 0;
										tempPixel = tempPixel->nextPixel;
									}
								}
								delete thePatch;
								temp2 = holder;
							}
							else
								temp2=temp2->next;
						}
					}
					if(temp->children->numOfNodes == 1){ //combine the single children w/ parent
						Patch<float>* thePatch = new Patch<float>(temp->children->first->i1, dataBuf, patchIdBuf, nrow, ncol, true);
						thePatch->assignValue(dataBuf, (float)temp->i2);
						PatchPixel* tempPixel = thePatch->first;
						while(tempPixel!=NULL){
							patchIdBuf[tempPixel->pos] = 0;
							tempPixel = tempPixel->nextPixel;
						}
						delete thePatch;
						//after merging the single child to the parent, upgrade the previous grand child to child
						List<IINode>* theChildren = temp->children->first->children; //theChildren is a holder of the grand child
						temp2 = temp->children->first; //temp2 is the single child of temp
						//temp->children->avoidHeadTail(temp2);//DO NOT delete it
						temp2->children = new List<IINode>; //to avoid destroying the grand child
						temp->children->removeNode(temp2);
						if(theChildren->numOfNodes<=0) //the grand child has nothing
							temp->children->release();
						else{
							delete temp->children;
							temp->children = theChildren; //upgrade the grand child
						}
					}
					temp=temp->next;
				}
			}while(num>0);
			delete streamNet;
		}
	}

	theInput->setDataName(extractFileName(m_OutputFileName).GetBuffer(1));
	theInput->calcStat();
	int writeFlag = theInput->writeout(m_OutputFileName.GetBuffer(1), 0);

	//clean up
	delete theInput;
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	CDialog::OnOK();
}

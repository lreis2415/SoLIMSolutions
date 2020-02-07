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
// diversitydlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "diversitydlg.h"
#include "stdlib.h"
#include "stdio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiversityDlg dialog


CDiversityDlg::CDiversityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDiversityDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiversityDlg)
	m_NoOfElements = 1;
	m_OutputFileName = _T("");
	m_PolygonFileName = _T("");
	m_RasterFileName = _T("");
	//}}AFX_DATA_INIT
}


void CDiversityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiversityDlg)
	DDX_Control(pDX, IDC_ProgressBar, m_ProgressBar);
	DDX_Text(pDX, IDC_NoOfElementsFld, m_NoOfElements);
	DDX_Text(pDX, IDC_OutputFileNameFld, m_OutputFileName);
	DDX_Text(pDX, IDC_PolygonFileNameFld, m_PolygonFileName);
	DDX_Text(pDX, IDC_RasterFileNameFld, m_RasterFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiversityDlg, CDialog)
	//{{AFX_MSG_MAP(CDiversityDlg)
	ON_BN_CLICKED(IDC_OutputFileNameBtn, OnOutputFileNameBtn)
	ON_BN_CLICKED(IDC_PolygonFileNameBtn, OnPolygonFileNameBtn)
	ON_BN_CLICKED(IDC_RasterFileNameBtn, OnRasterFileNameBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiversityDlg message handlers
void CDiversityDlg::OnPolygonFileNameBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "Polygon Files (*.ply)|*.ply|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.ply", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_PolygonFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}			
}

void CDiversityDlg::OnRasterFileNameBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_RasterFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}			
}

void CDiversityDlg::OnOutputFileNameBtn(){
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";
 	CFileDialog m_ldFile(FALSE, "txt", ".txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK){
		m_OutputFileName = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}		
}

void CDiversityDlg::OnOK(){
	// TODO: Add extra validation here
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);

	//check input fields on the dialogbox
	if(m_PolygonFileName.IsEmpty()){
		MessageBox("No polygon file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_RasterFileName.IsEmpty()){
		MessageBox("No raster file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	if(m_OutputFileName.IsEmpty()){
		MessageBox("No output file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	//open the raster file
	CThreeDR* theRaster = new CThreeDR;
	int readflag = theRaster->readin(m_RasterFileName.GetBuffer(1), 0);
	if(readflag != 0){
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}
	int nrow = theRaster->getNumberOfRows();
	int ncol = theRaster->getNumberOfColumns();
	float cellsize = theRaster->getCellSize();

	m_ProgressBar.SetPos(15);

	ifstream fin_poly;
	fin_poly.open(m_PolygonFileName);
	if(fin_poly.fail()){
		MessageBox("Cannot open the polygon file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_ProgressBar.SetPos(0);
		return;
	}

	//Get the number of polygons in the text file for setting the progress bar.
	int numOfPoly = 0;
	char endMark[10];
	while(!fin_poly.eof()){
		fin_poly.getline(endMark,10);
		if(strcmp(endMark, "END") == 0 || strcmp(endMark, "End") == 0){
			numOfPoly++;
			if(!fin_poly.eof()){
				fin_poly.getline(endMark, 10);
				if(strcmp(endMark, "END") == 0 || strcmp(endMark, "End") == 0) break;
			}
		}
	}
	fin_poly.close();
	fin_poly.open(m_PolygonFileName);

	ofstream fout;
	fout.open(m_OutputFileName);

	CString tempString;
	bool fend = false; //file end
	fin_poly.getline(tempString.GetBuffer(220), 200);
	tempString.ReleaseBuffer(-1);
	tempString.TrimLeft();
	tempString.TrimRight();
	if(tempString == "END" || tempString == "End")
		return;
	int polyID = -1;
	double labelX = NODOUBLEVALUE;
	double labelY = NODOUBLEVALUE;
	sscanf(tempString.GetBuffer(220), "%i %lf %lf", &polyID, &labelX, &labelY);
	tempString.ReleaseBuffer(-1);
	//atoi(tempString.Left(tempString.FindOneOf(" ")));
	int progress = 1;
	while(!fin_poly.eof()){
		int a = progress*85/numOfPoly;
		m_ProgressBar.SetPos(15+progress*85/numOfPoly);
		xPolygon* curPoly = new xPolygon(polyID);
		curPoly->labelX = labelX;
		curPoly->labelY = labelY;
		List<xPolygon>* islands = new List<xPolygon>;
		tempString.Empty();
		fin_poly.getline(tempString.GetBuffer(220), 200);
		tempString.ReleaseBuffer(-1);
		tempString.TrimLeft();
		tempString.TrimRight();
		int vid = 0;
		double x = 0;
		double y = 0;
		while(tempString != "END" && tempString != "End"){
			x = atof(tempString.Left(tempString.FindOneOf(" ")));
			y = atof(tempString.Right(tempString.GetLength()-tempString.FindOneOf(" ")));
			if(curPoly->vertexList->first==NULL||
				fabs(x-curPoly->vertexList->last->d1)>VERYSMALL||
				fabs(y-curPoly->vertexList->last->d2)>VERYSMALL)
				curPoly->vertexList->addTail(new IDDNode(vid, x, y));
			tempString.Empty();
			fin_poly.getline(tempString.GetBuffer(220), 200);
			tempString.ReleaseBuffer(-1);
			tempString.TrimLeft();
			tempString.TrimRight();
			vid++;
		}
		if(!fin_poly.eof()){
			fin_poly.getline(tempString.GetBuffer(220), 200);
			tempString.ReleaseBuffer(-1);
			tempString.TrimLeft();
			tempString.TrimRight();
			if(tempString == "END" || tempString == "End")
				fend = true;
			if(!fend){
				if(tempString.FindOneOf(" ")>0){
					sscanf(tempString.GetBuffer(220), "%i %lf %lf", &polyID, &labelX, &labelY);
					tempString.ReleaseBuffer(-1);
					//polyID = atoi(tempString.Left(tempString.FindOneOf(" ")));
				}
				else
					polyID = atoi(tempString);
			}
			progress++;
		}

		while(!fend && polyID == -99999){ //island
			xPolygon* newIsland = new xPolygon(polyID);
			tempString.Empty();
			fin_poly.getline(tempString.GetBuffer(220), 200);
			tempString.ReleaseBuffer(-1);
			tempString.TrimLeft();
			tempString.TrimRight();
			int vIsland = 0;
			double xIsland = 0;
			double yIsland = 0;
			while(tempString != "END" && tempString != "End"){
				xIsland = atof(tempString.Left(tempString.FindOneOf(" ")));
				yIsland = atof(tempString.Right(tempString.GetLength()-tempString.FindOneOf(" ")));
				if(newIsland->vertexList->first==NULL||
					fabs(x-newIsland->vertexList->last->d1)>VERYSMALL||
					fabs(y-newIsland->vertexList->last->d2)>VERYSMALL)
					newIsland->vertexList->addTail(new IDDNode(vIsland, xIsland, yIsland));
				tempString.Empty();
				fin_poly.getline(tempString.GetBuffer(120), 100);
				tempString.ReleaseBuffer(-1);
				tempString.TrimLeft();
				tempString.TrimRight();
				vIsland++;
			}
			if(fabs(newIsland->vertexList->first->d1-newIsland->vertexList->last->d1)>VERYSMALL || 
				fabs(newIsland->vertexList->first->d2-newIsland->vertexList->last->d2)>VERYSMALL) //not closed
				delete newIsland;
			else
				islands->addTail(newIsland);
			if(!fin_poly.eof()){
				fin_poly.getline(tempString.GetBuffer(220), 200);
				tempString.ReleaseBuffer(-1);
				tempString.TrimLeft();
				tempString.TrimRight();
				if(tempString == "END" || tempString == "End")
					fend = true;
				if(!fend){
					if(tempString.FindOneOf(" ")>0){
						sscanf(tempString.GetBuffer(220), "%i %lf %lf", &polyID, &labelX, &labelY);
						tempString.ReleaseBuffer(-1);
						//polyID = atoi(tempString.Left(tempString.FindOneOf(" ")));
					}
					else
						polyID = atoi(tempString);
				}
				progress++;
			}
		}

		if(fabs(curPoly->vertexList->first->d1-curPoly->vertexList->last->d1)>VERYSMALL || 
			fabs(curPoly->vertexList->first->d2-curPoly->vertexList->last->d2)>VERYSMALL){ //check closure
			delete curPoly;
			delete islands;
			if(fend)
				break;
			else
				continue;
		}

		diversPatch* newD = new diversPatch(curPoly->theID);
		newD->labelX = curPoly->labelX;
		newD->labelY = curPoly->labelY;
		
		//find out the range of relavant pixels
		curPoly->getBound();

		int sr = theRaster->rowOfAPoint(curPoly->Ymax);
		int sc = theRaster->colOfAPoint(curPoly->Xmin);
		int er = theRaster->rowOfAPoint(curPoly->Ymin);
		int ec = theRaster->colOfAPoint(curPoly->Xmax);

		//test if pixels are inside the polygon
		for(int i=sr; i<er+1; i++){
			double pixelY = theRaster->yOfACell(i);
			for(int j=sc; j<ec+1; j++){
				int pixelV = (int)theRaster->getDataBuf()[i*ncol+j];
				double pixelX = theRaster->xOfACell(j);
				IDDNode* thePoint = new IDDNode(pixelV, pixelX, pixelY);
				if(pointInPolygon(curPoly, islands, thePoint)) //if inside the polygon
					newD->addAlien(new alien(pixelV, 1));
				delete thePoint;
			}
		}
		
		//sorting the aliens in the list
		newD->sorting();

		//output the result
		newD->output(m_NoOfElements, fout);
		
		//clean up
		delete curPoly;
		delete islands;
		if(fend)
			break;
	}
	m_ProgressBar.SetPos(100);
	fin_poly.close();
	fout.close();
	CDialog::OnOK();
}

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
// LayerInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LayerInfoDlg.h"
#include "ThreeDR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLayerInfoDlg
extern g_nX;
extern g_nY;
extern g_dValue;
extern g_nZoom;
extern g_fRelate[8];
extern g_bInfoVis=0;
CString g_sFileName;
IMPLEMENT_DYNCREATE(CLayerInfoDlg, CFrameWnd)

CLayerInfoDlg::CLayerInfoDlg()
{
	Create(NULL,"Information",WS_OVERLAPPEDWINDOW|WS_VSCROLL|WS_VISIBLE,CRect(0,0,200,200)); 

}

CLayerInfoDlg::~CLayerInfoDlg()
{
}


BEGIN_MESSAGE_MAP(CLayerInfoDlg, CFrameWnd)
	//{{AFX_MSG_MAP(CLayerInfoDlg)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayerInfoDlg message handlers

void CLayerInfoDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CFrameWnd::OnPaint() for painting messages
	CVis3drApp* app=(CVis3drApp*)AfxGetApp();
	CString f_x,f_y,f_value;
	f_x.Format("%d",g_nX);
	f_y.Format("%d",g_nY);
	f_value.Format("%d",g_dValue);
	dc.TextOut(0,0,f_x);
	dc.TextOut(40,0,f_y);
	dc.TextOut(80,0,f_value);
	CString temp;
	float *tempBuf=app->g_3drData.getDataBuf();
	int tempRow=app->g_3drData.getNumberOfRows();
	int tempCol=app->g_3drData.getNumberOfColumns();
	if(g_fRelate[0]!=-1) {
		temp.Format("%d",g_fRelate[0]);
		dc.TextOut(0,50,temp);
		temp.Empty();
	}
	else {
		temp="N/A";
		dc.TextOut(0,50,temp);
		temp.Empty();
	}
	if(g_fRelate[1]!=-1) {
		temp.Format("%d",g_fRelate[1]);
		dc.TextOut(40,50,temp);
		temp.Empty();
	}
	else {
		temp="N/A";
		dc.TextOut(40,50,temp);
		temp.Empty();
	}
	if(g_fRelate[2]!=-1) {
		temp.Format("%d",g_fRelate[2]);
		dc.TextOut(80,50,temp);
		temp.Empty();
	}
	else {
		temp="N/A";
		dc.TextOut(80,50,temp);
		temp.Empty();
	}
	if(g_fRelate[3]!=-1) {
		temp.Format("%d",g_fRelate[3]);
		dc.TextOut(0,80,temp);
		temp.Empty();
	}
	else {
		temp="N/A";
		dc.TextOut(0,80,temp);
		temp.Empty();
	}
	if(g_fRelate[4]!=-1) {
		temp.Format("%d",g_fRelate[4]);
		dc.TextOut(80,80,temp);
		temp.Empty();
	}
	else {
		temp="N/A";
		dc.TextOut(80,80,temp);
		temp.Empty();
	}
	if(g_fRelate[5]!=-1) {
		temp.Format("%d",g_fRelate[5]);
		dc.TextOut(0,110,temp);
		temp.Empty();
	}
	else {
		temp="N/A";
		dc.TextOut(0,110,temp);
		temp.Empty();
	}
	if(g_fRelate[6]!=-1) {
		temp.Format("%d",g_fRelate[6]);
		dc.TextOut(40,110,temp);
		temp.Empty();
	}
	else {
		temp="N/A";
		dc.TextOut(40,110,temp);
		temp.Empty();
	}
	if(g_fRelate[7]!=-1) {
		temp.Format("%d",g_fRelate[7]);
		dc.TextOut(80,110,temp);
		temp.Empty();
	}
	else {
		temp="N/A";
		dc.TextOut(80,110,temp);
		temp.Empty();
	}
	int pos_x=0,pos_y=140;
	//float* tempbuf=(float*)g_pBuffer;
	//app->g_3drData.readin(g_sFileName.GetBuffer(app->g_sFileName.GetLength()),0);
	temp.Empty();
	/*
	//total info
	for(int i=0;i<app->g_3drData.getNumberOfColumns();i++) {
		for(int j=0;j<app->g_3drData.getNumberOfRows();j++) {
			temp.Format("%d",app->g_3drData.getDataBuf()[i+j*(app->g_3drData.getNumberOfColumns())]);
			dc.TextOut(pos_x,pos_y,temp);
			pos_x+=40;
			temp.Empty();
		}
		pos_x=0;
		pos_y+=30;
	}
	*/
}

void CLayerInfoDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//delete *this;
	CFrameWnd::OnClose();
	g_bInfoVis=0;
	//delete this;
	//this->DestroyWindow();
}

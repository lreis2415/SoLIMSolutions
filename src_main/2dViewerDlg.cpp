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
// 2dViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "2dViewerDlg.h"

#include "stdafx.h"
#include "SoLIM.h"

#include "MainFrm.h"
#include "SoLIMDoc.h"
#include "Utility.h"
#include "ThreeDR.h"
#include ".\2dviewerdlg.h"



// C2dViewerDlg dialog

IMPLEMENT_DYNAMIC(C2dViewerDlg, CDialog)
C2dViewerDlg::C2dViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C2dViewerDlg::IDD, pParent)
{
	m_theInput = NULL;

}

C2dViewerDlg::~C2dViewerDlg()
{

	if(this->m_theInput != NULL)
	{
		delete m_theInput;
		m_theInput = NULL;
	}

}

void C2dViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);


}


BEGIN_MESSAGE_MAP(C2dViewerDlg, CDialog)	

	ON_WM_PAINT()

	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// C2dViewerDlg message handlers



// CDataExplorerView drawing

BOOL C2dViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CRect rect;
	GetClientRect(&rect);
	int cx = rect.right;
	int cy = rect.bottom;
	

	CRect graphRect(0.1*cx/10,1.5*cy/10,9.9*cx/10,9.9*cy/10);
	m_pImagePanel.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,graphRect,this,1000);



		  this->m_pImagePanel.m_pTheData = this->m_theInput;

	     this->m_pImagePanel.m_LayerLoaded = true;


	m_pImagePanel.Invalidate();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control

}






void C2dViewerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

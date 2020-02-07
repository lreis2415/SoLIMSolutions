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
// SpatialSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "SpatialSettingDlg.h"
#include ".\spatialsettingdlg.h"


// CSpatialSettingDlg dialog

IMPLEMENT_DYNAMIC(CSpatialSettingDlg, CDialog)
CSpatialSettingDlg::CSpatialSettingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSpatialSettingDlg::IDD, pParent)
, m_dCentralX(0)
, m_dCentralY(0)
, m_bUseDistSim(FALSE)
, m_fSearchDist(0)
, m_fDistDecayFactor(1)
{
}

CSpatialSettingDlg::~CSpatialSettingDlg()
{
}

void CSpatialSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, m_dCentralX);
	DDX_Text(pDX, IDC_EDIT_Y, m_dCentralY);
	DDX_Check(pDX, IDC_CHECK_USE_SIM, m_bUseDistSim);
	DDX_Text(pDX, IDC_EDIT_SEARCH_DIST, m_fSearchDist);
	DDX_Text(pDX, IDC_EDIT_DIST_DECAY_FACTOR, m_fDistDecayFactor);
}


BEGIN_MESSAGE_MAP(CSpatialSettingDlg, CDialog)

	ON_BN_CLICKED(IDC_CHECK_USE_SIM, OnBnClickedCheckUseSim)
END_MESSAGE_MAP()


// CSpatialSettingDlg message handlers

void CSpatialSettingDlg::OnOK()
{
	CString str;
	UpdateData(true);

	GetDlgItem(IDC_EDIT_X)->GetWindowText(str);
	if(str == "")
	{
		MessageBox("Central X must be specified.");
		return;
	}
	GetDlgItem(IDC_EDIT_Y)->GetWindowText(str);
	if(str == "")
	{
		MessageBox("Central Y must be specified.");
		return;
	}
	GetDlgItem(IDC_EDIT_SEARCH_DIST)->GetWindowText(str);
	if(str == "")
	{
		MessageBox("Radius must be specified.");
		return;
	}
	else if(m_fSearchDist<=0)
	{
		MessageBox("Radius should be greater than 0.");
		return;
	}


	if(m_bUseDistSim == true)
	{
		GetDlgItem(IDC_EDIT_DIST_DECAY_FACTOR)->GetWindowText(str);
		if(str=="")
		{
			MessageBox("Distance decay factor must be specified.");
			return;
		}
		else if(m_fDistDecayFactor<=0)
		{
			MessageBox("Distance decay factor should be greater than 0.");
			return;
		}

	}

	CDialog::OnOK();
}


void CSpatialSettingDlg::OnBnClickedCheckUseSim()
{
	UpdateData(true);
	if(m_bUseDistSim == true)
		GetDlgItem(IDC_EDIT_DIST_DECAY_FACTOR)->EnableWindow(true);
	else
		GetDlgItem(IDC_EDIT_DIST_DECAY_FACTOR)->EnableWindow(false);

}

BOOL CSpatialSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_bUseDistSim == true)
		GetDlgItem(IDC_EDIT_DIST_DECAY_FACTOR)->EnableWindow(true);
	else
		GetDlgItem(IDC_EDIT_DIST_DECAY_FACTOR)->EnableWindow(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	
}

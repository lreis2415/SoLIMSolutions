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
// SmallScaleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SoLIM.h"
#include "CBRSmallScaleDlg.h"
#include ".\cbrsmallscaledlg.h"
#include "CBRGenerateFileDlg.h"
// CBRSmallScaleDlg 对话框

IMPLEMENT_DYNAMIC(CBRSmallScaleDlg, CDialog)
CBRSmallScaleDlg::CBRSmallScaleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBRSmallScaleDlg::IDD, pParent)
{

}

CBRSmallScaleDlg::~CBRSmallScaleDlg()
{
}

void CBRSmallScaleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLIMATE, m_listCilmate);
	DDX_Control(pDX, IDC_LIST_PARENTMATERIAL, m_listParentMaterial);
	DDX_Control(pDX, IDC_LIST_TOPOGRAPHIC, m_listTopographic);
	DDX_Control(pDX, IDC_LIST_VEGETATION, m_listVegetation);
}


BEGIN_MESSAGE_MAP(CBRSmallScaleDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_CLIMATE, OnBnClickedButtonAddClimate)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_CLIMATE, OnBnClickedButtonRemoveClimate)
	ON_BN_CLICKED(IDC_BUTTON_ADD_PARENTMATERIAL, OnBnClickedButtonAddParentmaterial)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_PARENTMARERIAL, OnBnClickedButtonRemoveParentmarerial)
	ON_BN_CLICKED(IDC_BUTTON_ADD_TOPOGRAPHIC, OnBnClickedButtonAddTopographic)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_TOPOGRAPHIC, OnBnClickedButtonRemoveTopographic)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	//ON_BN_CLICKED(IDC_BUTTON_OUTPUT_BROWSE, OnBnClickedButtonOutputBrowse)
	ON_BN_CLICKED(IDC_BUTTON_ADD_VEGETATION, OnBnClickedButtonAddVegetation)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_VEGETATION, OnBnClickedButtonRemoveVegetation)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CBRSmallScaleDlg 消息处理程序

BOOL CBRSmallScaleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//添加ListCtrl的列
	this->m_listCilmate.InsertColumn(1,CString("Name"));
	this->m_listCilmate.SetColumnWidth(0,300);
	this->m_listCilmate.InsertColumn(2,CString("Resolution"));
	this->m_listCilmate.SetColumnWidth(1,80);
	this->m_listParentMaterial.InsertColumn(1,CString("Name"));
	this->m_listParentMaterial.SetColumnWidth(0,300);
	this->m_listParentMaterial.InsertColumn(2,CString("Resolution"));
	this->m_listParentMaterial.SetColumnWidth(1,80);

	this->m_listTopographic.InsertColumn(1,CString("Name"));
	this->m_listTopographic.SetColumnWidth(0,300);
	this->m_listTopographic.InsertColumn(2,CString("Resolution"));
	this->m_listTopographic.SetColumnWidth(1,80);

	this->m_listVegetation.InsertColumn(1,CString("Name"));
	this->m_listVegetation.SetColumnWidth(0,300);
	this->m_listVegetation.InsertColumn(2,CString("Resolution"));
	this->m_listVegetation.SetColumnWidth(1,80);
	//end
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CBRSmallScaleDlg::addToListCtrl(CString filename, CListCtrl& listCtrl)
{
	CThreeDR threedrCurrentLayer;
	threedrCurrentLayer.readin(filename.GetBuffer(),0);
	double dResolution=threedrCurrentLayer.getCellSize();
	if (dResolution>=0)
	{
		listCtrl.InsertItem(0, filename);
		CString strCellSize;
		strCellSize.Format(TEXT("%f"),dResolution);
		listCtrl.SetItemText(0,1,strCellSize);
	}
	else
	{
		AfxMessageBox("Not a 3dr file");
	}
}

void CBRSmallScaleDlg::add3dr(CListCtrl& listCtrl)
{
	static char BASED_CODE szFilter[] = "Layer Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
	CFileDialog dlg(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT, szFilter);
	if (dlg.DoModal()==IDOK)
	{
		POSITION pos;   
		pos = dlg.GetStartPosition();   
		while(pos)   
		{   
			CString szPathName = dlg.GetNextPathName(pos); 
			addToListCtrl(szPathName, listCtrl);
		}   
	}
}

void CBRSmallScaleDlg::OnBnClickedButtonAddClimate()
{
	//Add a layer to climate layers 添加一个气候因子图层
	add3dr(m_listCilmate);
}

void CBRSmallScaleDlg::OnBnClickedButtonRemoveClimate()
{
	// TODO: 在此添加控件通知处理程序代码
	//Delete a selected layer from climate layers 删除一个气候因子图层
	POSITION pos=m_listCilmate.GetFirstSelectedItemPosition();
	int iDeleteIndex=m_listCilmate.GetNextSelectedItem(pos);
	this->m_listCilmate.DeleteItem(iDeleteIndex);
}

void CBRSmallScaleDlg::OnBnClickedButtonAddParentmaterial()
{
	// TODO: 在此添加控件通知处理程序代码
	//Add a layer to parent material layers 添加一个母质因子图层
	add3dr(m_listParentMaterial);
}

void CBRSmallScaleDlg::OnBnClickedButtonRemoveParentmarerial()
{
	// TODO: 在此添加控件通知处理程序代码
	//Delete a selected layer from parent material layers 删除一个母质因子图层
	POSITION pos=this->m_listParentMaterial.GetFirstSelectedItemPosition();
	int iDeleteIndex=this->m_listParentMaterial.GetNextSelectedItem(pos);
	this->m_listParentMaterial.DeleteItem(iDeleteIndex);
}

void CBRSmallScaleDlg::OnBnClickedButtonAddTopographic()
{
	// TODO: 在此添加控件通知处理程序代码
	//Add a layer to topographic layers 添加一个地形因子图层
	add3dr(m_listTopographic);
}

void CBRSmallScaleDlg::OnBnClickedButtonRemoveTopographic()
{
	// TODO: 在此添加控件通知处理程序代码
	//Delete a selected layer from topographic layers 删除一个地形因子图层
	POSITION pos=m_listTopographic.GetFirstSelectedItemPosition();
	int iDeleteIndex=this->m_listTopographic.GetNextSelectedItem(pos);
	this->m_listTopographic.DeleteItem(iDeleteIndex);
}

void CBRSmallScaleDlg::OnBnClickedOk()
{
	if(m_listTopographic.GetItemCount() == 0)
	{
		MessageBox("The topological data are necessary.");
		return;
	}

	//upload all the layers to the global variable 将因子的文件名写入全局变量
	CSoLIMApp* pApp=(CSoLIMApp*)AfxGetApp();
	pApp->g_vecstrClimateLayers.clear();
	pApp->g_vecClimate.clear();
	for(int i=0;i<this->m_listCilmate.GetItemCount();i++)
	{
		pApp->g_vecstrClimateLayers.push_back(this->m_listCilmate.GetItemText(i,0));
	}

	pApp->g_vecstrParentMaterialLayers.clear();
	pApp->g_vecParentMaterial.clear();
	for(int i=0;i<this->m_listParentMaterial.GetItemCount();i++)
	{
		pApp->g_vecstrParentMaterialLayers.push_back(this->m_listParentMaterial.GetItemText(i,0));
	}

	pApp->g_vecstrTopographicLayers.clear();
	pApp->g_vecTopology.clear();
	for(int i=0;i<this->m_listTopographic.GetItemCount();i++)
	{
		pApp->g_vecstrTopographicLayers.push_back(this->m_listTopographic.GetItemText(i,0));
	}

	pApp->g_vecstrVegetationLayers.clear();
	pApp->g_vecVegetation.clear();
	for(int i = 0; i < m_listVegetation.GetItemCount(); ++i)
		pApp->g_vecstrVegetationLayers.push_back(m_listVegetation.GetItemText(i, 0));

	//end
	//OnOK();
	this->ShowWindow(SW_HIDE);
	if (!pApp->pGenerateFileDlg)
	{
		pApp->pGenerateFileDlg = new CBRGenerateFileDlg();
		pApp->pGenerateFileDlg->Create(IDD_CBR_GEN_FILE, NULL);
	}
	pApp->pGenerateFileDlg->init();
	pApp->pGenerateFileDlg->ShowWindow(SW_SHOW);
}

/*void CBRSmallScaleDlg::OnBnClickedButtonOutputBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	// This method is useless 此函数无用

	//GetFilePath();
}*/



void CBRSmallScaleDlg::OnBnClickedButtonAddVegetation()
{
	add3dr(m_listVegetation);
}

void CBRSmallScaleDlg::OnBnClickedButtonRemoveVegetation()
{
	POSITION pos=this->m_listVegetation.GetFirstSelectedItemPosition();
	int iDeleteIndex=this->m_listVegetation.GetNextSelectedItem(pos);
	this->m_listVegetation.DeleteItem(iDeleteIndex);
}

void CBRSmallScaleDlg::OnBnClickedCancel()
{
	OnCancel();

	CSoLIMApp* pApp = (CSoLIMApp*)AfxGetApp();
	if(pApp->pSmallScaleDlg)
	{
		delete pApp->pSmallScaleDlg;
		pApp->pSmallScaleDlg = NULL;
	}
	if(pApp->pGenerateFileDlg)
	{
		delete pApp->pGenerateFileDlg;
		pApp->pGenerateFileDlg = NULL;
	}
	if(pApp->pSampleDlg)
	{
		delete pApp->pSampleDlg;
		pApp->pSampleDlg = NULL;
	}
	if(pApp->pSimiDlg)
	{
		delete pApp->pSimiDlg;
		pApp->pSimiDlg = NULL;
	}
	if(pApp->pUncertaintyDlg)
	{
		delete pApp->pUncertaintyDlg;
		pApp->pUncertaintyDlg = NULL;
	}
		if(pApp->pTypeinferDlg)
	{
		delete pApp->pTypeinferDlg;
		pApp->pTypeinferDlg = NULL;
	}
}

//void CBRSmallScaleDlg::OnDestroy()
//{
//	CDialog::OnDestroy();
//	delete this;
//}

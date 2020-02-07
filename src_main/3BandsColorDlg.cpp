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
// 3BandsColorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SoLIM.h"
#include "3BandsColorDlg.h"
#include ".\3bandscolordlg.h"


// C3BandsColorDlg 对话框

IMPLEMENT_DYNAMIC(C3BandsColorDlg, CDialog)
C3BandsColorDlg::C3BandsColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C3BandsColorDlg::IDD, pParent)
	, m_InputRFn(_T(""))
	, m_InputGFn(_T(""))
	, m_InputBFn(_T(""))
	, m_OutFn(_T(""))
{
}

C3BandsColorDlg::~C3BandsColorDlg()
{
}

void C3BandsColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
	DDX_Text(pDX, IDC_RFileNameFld, m_InputRFn);
	DDX_Text(pDX, IDC_GFileNameFld, m_InputGFn);
	DDX_Text(pDX, IDC_BFileNameFld, m_InputBFn);
	DDX_Text(pDX, IDC_OutFileNameFld, m_OutFn);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
}


BEGIN_MESSAGE_MAP(C3BandsColorDlg, CDialog)
	ON_BN_CLICKED(IDC_RFileNameBtn, OnBnClickedRFileNameBtn)
	ON_BN_CLICKED(IDC_GFileNameBtn, OnBnClickedGFileNameBtn)
	ON_BN_CLICKED(IDC_BFileNameBtn, OnBnClickedBFileNameBtn)
	ON_BN_CLICKED(IDC_OutFileNameBtn, OnBnClickedOutFileNameBtn)
END_MESSAGE_MAP()


// C3BandsColorDlg 消息处理程序

void C3BandsColorDlg::OnBnClickedRFileNameBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_InputRFn = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}		
}

void C3BandsColorDlg::OnBnClickedGFileNameBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_InputGFn = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}
}

void C3BandsColorDlg::OnBnClickedBFileNameBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, NULL, "*.3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Open";	
	if(m_ldFile.DoModal() == IDOK){
		m_InputBFn = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void C3BandsColorDlg::OnBnClickedOutFileNameBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);	
	static char BASED_CODE szFilter[] = "3dr Files (*.3dr)|*.3dr|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(FALSE, "3dr", ".3dr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_ldFile.m_ofn.lpstrTitle="Save As";	
	if(m_ldFile.DoModal() == IDOK){
		m_OutFn = m_ldFile.GetPathName();
		UpdateData(FALSE);
	}	
}

void C3BandsColorDlg::OnOK(void)
{
	//在此处添加核心代码
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	m_ProgressBar.SetRange(1, 100);
	m_ProgressBar.SetPos(5);


	if(m_InputRFn.IsEmpty()){
		MessageBox("No R band data file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if(m_InputGFn.IsEmpty()){
		MessageBox("No G band data file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if(m_InputBFn.IsEmpty()){
		MessageBox("No B band data file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if(m_OutFn.IsEmpty()){
		MessageBox("No output file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}

	CThreeDR* theInputR = new CThreeDR;
	int readFlagR = theInputR->readin(m_InputRFn.GetBuffer(1), 0);
	if(readFlagR != 0){
		MessageBox("Error in openning the data file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	float* dataBufR = theInputR->getDataBuf();

	CThreeDR* theInputG = new CThreeDR;
	int readFlagG = theInputG->readin(m_InputGFn.GetBuffer(1), 0);
	if(readFlagG != 0){
		MessageBox("Error in openning the data file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	float* dataBufG = theInputG->getDataBuf();

	CThreeDR* theInputB = new CThreeDR;
	int readFlagB = theInputB->readin(m_InputBFn.GetBuffer(1), 0);
	if(readFlagB != 0){
		MessageBox("Error in openning the data file.");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	float* dataBufB = theInputB->getDataBuf();

	CThreeDR* theOutput = new CThreeDR;	
	float* dataBufOut = theInputR->getDataBuf();
	int numOfRows = theInputR->getNumberOfRows();
	int numOfCols = theInputR->getNumberOfColumns();
	dataBufOut = new float[numOfRows * numOfCols];	
	
	//3dr file's NumberOfColor field can not be greater than 65536
	const int colorBit = 32;
	int NumberOfColors = colorBit * colorBit * colorBit; // NumberOfColor = 32768	
	//Get the max and min value of R, G, B bands
	float max_R = theInputR->getDataMax();
	float min_R = theInputR->getDataMin();
	float max_G = theInputG->getDataMax();
	float min_G = theInputG->getDataMin();
	float max_B = theInputB->getDataMax();
	float min_B = theInputB->getDataMin();

	for(int i = 0; i < numOfRows * numOfCols; i++){
		//Stretch to 0 to colorBit - 1
		int tempR = int((dataBufR[i] - min_R) * (colorBit - 1) / (max_R - min_R));
		int tempG = int((dataBufG[i] - min_G) * (colorBit - 1) / (max_G - min_G));
		int tempB = int((dataBufB[i] - min_B) * (colorBit - 1) / (max_B - min_B));
		float code = float(colorBit * colorBit * tempR + colorBit * tempG + tempB);
		dataBufOut[i] = code;
	}	
	
	PALENTRY* thePalette = new PALENTRY[NumberOfColors];
	for (int r = 0; r < colorBit; r++)
	{
		for (int g = 0; g < colorBit; g++)
		{
			for (int b = 0; b < colorBit; b++)
			{
				int idx = colorBit * colorBit * r + colorBit * g + b;
				thePalette[idx].index = idx;
				thePalette[idx].rasterval = float(idx);
				thePalette[idx].r = int(r * 255 / (colorBit - 1));
				thePalette[idx].g = int(g * 255 / (colorBit - 1));
				thePalette[idx].b = int(b * 255 / (colorBit - 1));
			}
		}
	}	

	m_ProgressBar.SetPos(50);

	theOutput->copyBasicFields(theInputR);
	theOutput->setDataName(extractFileName(m_OutFn).GetBuffer(1));
	theOutput->setDataBuf(dataBufOut);
	theOutput->calcStat();

	theOutput->setNumberOfColors(NumberOfColors);
	theOutput->setPalette(thePalette);
	int writeFlag = theOutput->writeout(m_OutFn.GetBuffer(1), 0);

	delete theInputR;
	delete theInputG;
	delete theInputB;
	delete theOutput;
	
	m_ProgressBar.SetPos(100);

	GetDlgItem(IDOK)->EnableWindow(TRUE);

	CDialog::OnOK();
}

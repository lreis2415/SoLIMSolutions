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
// RemoteSensingSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoLIM.h"
#include "RemoteSensingSettingDlg.h"
#include ".\remotesensingsettingdlg.h"
#include "RemoteSensingDlg.h"

#include "GetDirDlg.h"

#include <vector>
#include <algorithm>

#include <sstream>
#include <map>
using namespace std;


// CRemoteSensingSettingDlg dialog

IMPLEMENT_DYNAMIC(CRemoteSensingSettingDlg, CDialog)
CRemoteSensingSettingDlg::CRemoteSensingSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteSensingSettingDlg::IDD, pParent)
	, m_DataDir(_T(""))
{
}

CRemoteSensingSettingDlg::~CRemoteSensingSettingDlg()
{
}

void CRemoteSensingSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DATA_FOLDER, m_DataDir);
}


BEGIN_MESSAGE_MAP(CRemoteSensingSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_RS_DATA_FOLDER, OnBnClickedButtonRsDataFolder)
END_MESSAGE_MAP()


// CRemoteSensingSettingDlg message handlers

void CRemoteSensingSettingDlg::OnBnClickedOk()
{
	UpdateData(true);

	CRemoteSensingDlg dlg;


	CFileFind finder;

   // build a string with wildcards
    CString strWildcard = _T(m_DataDir + "\\*.3dr");

	
		// start working for files
   BOOL bWorking = finder.FindFile(strWildcard);

   while (bWorking)
   {
	   bWorking = finder.FindNextFile();
	CString str=   finder.GetFileName();
	
     if (finder.IsDirectory())
		   continue;



		   CString filePath =  finder.GetFilePath();
		   CString fileName = finder.GetFileName();

		   int pos = fileName.Find('.');
		   if(pos == -1){
			   MessageBox("Error in adding data files.");	
			   continue;
		   }
		   fileName = fileName.Left(pos);

		   pos = fileName.Find('_');

		   int day = atoi(fileName.Left(pos));

		   int length = fileName.GetLength();

		   int band = atoi(fileName.Right(length - pos -1));



		   int i = 0;

		   for (i = 0; i< dlg.m_daysVec.size(); i++)
		   {
			   if(day == dlg.m_daysVec[i])
				   break;
		   }
		   if(i == dlg.m_daysVec.size()) //not found
			   dlg.m_daysVec.push_back(day);


		   i = 0;

		   for (i =0; i< dlg.m_bandsVec.size(); i++)
		   {
			   if(band == dlg.m_bandsVec[i])
				   break;
		   }
		   if(i == dlg.m_bandsVec.size()) //not found
			   dlg.m_bandsVec.push_back(band);


   }

   finder.Close();


   std::sort( dlg.m_daysVec.begin( ),dlg.m_daysVec.end( ) );

   std::sort( dlg.m_bandsVec.begin( ),dlg.m_bandsVec.end( ) );



   for (int i = 0; i< dlg.m_daysVec.size(); i++)
   {
	   vector <CString > pathsForOneDay;
	   vector <CString > fileNamesForOneDay;

	   for (int j = 0; j < dlg.m_bandsVec.size(); j++)
	   {
		   
		

		   CString fileName = "";
		   fileName.Format("%d_%d", dlg.m_daysVec[i], dlg.m_bandsVec[j]);

		   CString filePath = this->m_DataDir + "\\" + fileName + ".3dr";

		   CFile f;
		   if (f.Open(filePath,CFile::modeRead))
		   {
			   pathsForOneDay.push_back(filePath);
			   fileNamesForOneDay.push_back(fileName);
		   }
		   else

		   {
			   MessageBox(filePath + " is missing.");
			   return;
		   }


	   }
	   dlg.m_dataPathVec.push_back(pathsForOneDay);
	   dlg.m_dataFileNameVec.push_back(fileNamesForOneDay);
   }



   dlg.m_iNumOfDays = dlg.m_daysVec.size();
   dlg.m_iNumOfBands = dlg.m_bandsVec.size();



	OnOK();
	dlg.DoModal();
	
	// TODO: Add your control notification handler code here
	
}

void CRemoteSensingSettingDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CRemoteSensingSettingDlg::OnBnClickedButtonRsDataFolder()
{
	// TODO: Add your control notification handler code here


	UpdateData(TRUE);	
	CString dir;	

	CGetDirDlg getDirDlg;   //Added by DuFei

	
	dir = getDirDlg.GetDirectory(this, "", "Please specify the data folder");	
	  if(dir == "")
		 return;
	  else
	    m_DataDir = dir;
	UpdateData(FALSE);



}

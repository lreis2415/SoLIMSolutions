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
// WizardPointCenter.cpp 
//

#include "stdafx.h"
#include "SoLIM.h"
#include "WizardPointCenter.h"
#include ".\wizardpointcenter.h"



IMPLEMENT_DYNAMIC(CWizardPointCenter, CDialog)
CWizardPointCenter::CWizardPointCenter(CWnd* pParent /*=NULL*/)
: CDialog(CWizardPointCenter::IDD, pParent)
, m_dCentralX(0)
, m_dCentralY(0)
{
}

CWizardPointCenter::~CWizardPointCenter()
{
}

void CWizardPointCenter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CENTRAL_X, m_dCentralX);
	DDX_Text(pDX, IDC_EDIT_CENTRAL_Y, m_dCentralY);
}


BEGIN_MESSAGE_MAP(CWizardPointCenter, CDialog)
	//ON_BN_CLICKED(IDC_BUTTON_3DMAPPER, OnBnClickedButton3dmapper)
END_MESSAGE_MAP()


/*
void CWizardPointCenter::OnBnClickedButton3dmapper()
{
	CString BaseFileName;
	static char BASED_CODE szFilter[] = "3dm files (*.3dm)|*.3dm||";
	CFileDialog m_3dmFileDlg(TRUE, NULL, "*.3dm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	m_3dmFileDlg.m_ofn.lpstrTitle="Choose Base File";	
	if(m_3dmFileDlg.DoModal() == IDOK)
	{
		BaseFileName=m_3dmFileDlg.GetPathName();
		HKEY hKEY;
		LPCTSTR data_Set="SOFTWARE\\Classes\\3dMapperExtension.File\\shell\\open\\command\\";
		long ret0=RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_READ,&hKEY);
		if(ret0!=ERROR_SUCCESS)
		{
			if(AfxMessageBox("3dMapper can not be found in your system! \nDo you want to find it by yourself?",MB_ICONQUESTION|MB_YESNO)==IDYES)
			{
				static char BASED_CODE szFilter[] = "EXE Files (*.exe)|*.exe||";
				CFileDialog m_ldFile(TRUE, NULL, "*.exe", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);		
				m_ldFile.m_ofn.lpstrTitle="Browse to Locate 3dMpper";	

				if(m_ldFile.DoModal() == IDOK)
				{
					CString path= m_ldFile.GetPathName();
					ShellExecute(m_hWnd,"open",path, BaseFileName,"", SW_SHOW );
				}
				return;
		 }
			else 
				return;
		}

		LPBYTE path_Get=new BYTE[MAX_PATH];
		DWORD type=REG_SZ;
		DWORD cbData=80;
		long ret=RegQueryValueEx(hKEY,"",NULL,&type,path_Get,&cbData);
		if(ret!=ERROR_SUCCESS)
		{
			if(AfxMessageBox("3dMapper can not be found in your system!\nDo you want to find it by yourself?",MB_ICONQUESTION|MB_YESNO)==IDYES)
		 {
			 static char BASED_CODE szFilter[] = "EXE Files (*.exe)|*.exe||";
			 CFileDialog m_ldFile(TRUE, NULL, "*.exe", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
			 m_ldFile.m_ofn.lpstrTitle="Browse to Locate 3dMapper";	

			 if(m_ldFile.DoModal() == IDOK)
			 {
				 CString path= m_ldFile.GetPathName();
				 ShellExecute(m_hWnd,"open",path, BaseFileName,"", SW_SHOW );
			 }
			 return;
		 }
			else 
				return;
		}
		CString path=CString(path_Get);
		delete[] path_Get;
		RegCloseKey(hKEY);
		path.TrimRight("%1");
		path.Trim();
		ShellExecute(m_hWnd,"open",path, BaseFileName,"", SW_SHOW );
	}
	else
	{	
		return;
	}  
}*/

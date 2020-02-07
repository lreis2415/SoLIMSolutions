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
// GetDirDlg.cpp: implementation of the CGetDirDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GetDirDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CString CGetDirDlg::m_sRootDir;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGetDirDlg::CGetDirDlg()
{

}

CGetDirDlg::~CGetDirDlg()
{

}

CString CGetDirDlg::GetDirectory(CWnd *pParent, LPCSTR lpszRoot, LPCSTR lpszTitle)
{
	CString dirString;
	BROWSEINFO bi;

	bi.hwndOwner = pParent->m_hWnd;
	bi.pidlRoot = 0;
	bi.pszDisplayName = 0;
	bi.lpszTitle = lpszTitle;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = 0;
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT|BIF_NEWDIALOGSTYLE|0;

	m_sRootDir = lpszRoot;

	LPITEMIDLIST lpitemld = ::SHBrowseForFolder(&bi);

	if (lpitemld)
	{
		LPTSTR szBuf = dirString.GetBuffer(_MAX_PATH);
		::SHGetPathFromIDList(lpitemld, szBuf);
		::GlobalFree(lpitemld);
		dirString.ReleaseBuffer();
	}

	return dirString;

}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
	TCHAR buf[MAX_PATH];
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		strcpy(buf, CGetDirDlg::m_sRootDir);
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE,(LPARAM)buf);
		break;
	case BFFM_SELCHANGED:
		if (::SHGetPathFromIDList((LPITEMIDLIST)lp, buf))
			::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)buf);
		break;
	}
	return 0;
}

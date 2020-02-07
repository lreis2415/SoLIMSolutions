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
// MruFile.cpp: implementation of the CMruFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MruFile.h"
#include "SoLIM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMruFile::CMruFile()
{
	m_IniFileName = _T("");
	for( int i=0; i<MAXNUM; i++ )
		m_PathName[i] = _T("");
	m_CurNum = 0;
}

CMruFile::~CMruFile()
{

}

//read ini file
void CMruFile::ReadMru()
{
	m_CurNum = ::GetPrivateProfileInt("Mru File", "FileNum", 0, m_IniFileName);  //读取当前文件数
	CString no;
	for( int i=0; i<m_CurNum; i++)
	{
		no.Format( "%d", i+1 );   
		::GetPrivateProfileString("Mru File", no, "", m_PathName[i].GetBuffer(MAX_PATH),
			MAX_PATH, m_IniFileName);   
		m_PathName[i].ReleaseBuffer();
	}
	SetMruMenu();   
}

//write ini file
void CMruFile::WriteMru()
{
	CString no;
	no.Format( "%d", m_CurNum );
	::WritePrivateProfileString( "Mru File", "FileNum", no, m_IniFileName );  
	for( int i=0; i<MAXNUM; i++)
	{
		no.Format( "%d", i+1 );
		::WritePrivateProfileString("Mru File", no, m_PathName[i], m_IniFileName); 
	}
}

//Add latest file to the list
void CMruFile::AddMru(CString nPathName)
{
	int i;
	CString str1, str2;
	if( m_CurNum )
	{
		if( nPathName.CompareNoCase(m_PathName[0])==0 ) 
			return;
	}
	str1 = nPathName;
	i = 0;
	while( i<m_CurNum && nPathName.CompareNoCase(m_PathName[i])!=0 )  
	{
		str2 = m_PathName[i];
		m_PathName[i] = str1;
		str1 = str2;
		i++;
	}
	if( i<m_CurNum )
		m_PathName[i] = str1;  //nPathName already exists
	else if( m_CurNum<MAXNUM )
	{
		m_PathName[m_CurNum] = str1;  //the list is not full
		m_CurNum++;
	}

	SetMruMenu();   
	WriteMru();     
}

//clear recent file list
void CMruFile::ClearMru()
{
	m_CurNum = 0;
	for( int i=0; i<MAXNUM; i++)
		m_PathName[i].Empty();
	SetMruMenu();  
	WriteMru();
}

//Modify recent file list
void CMruFile::SetMruMenu()
{
	CMenu *pFileMenu = pMenu->GetSubMenu(0);		//point to Project subMenu
	int m=pFileMenu ->GetMenuItemCount();
	CMenu *pMruMenu = pFileMenu->GetSubMenu(4);		//point to recent file subMenu
	pMruMenu->RemoveMenu(ID_RECENTFILES_1, MF_BYCOMMAND );	//delet menu items
	pMruMenu->RemoveMenu(ID_RECENTFILES_2, MF_BYCOMMAND );
	pMruMenu->RemoveMenu(ID_RECENTFILES_3, MF_BYCOMMAND );
	pMruMenu->RemoveMenu(ID_RECENTFILES_4, MF_BYCOMMAND );
	if( m_CurNum>0 )								//insert menu items
		pMruMenu->InsertMenu( ID_RECENTFILES_CLEARRECENTFILELIST, MF_BYCOMMAND,
				ID_RECENTFILES_1, m_PathName[0] );
	if( m_CurNum>1 )
		pMruMenu->InsertMenu( ID_RECENTFILES_CLEARRECENTFILELIST, MF_BYCOMMAND,
				ID_RECENTFILES_2, m_PathName[1] );
	if( m_CurNum>2 )
		pMruMenu->InsertMenu(ID_RECENTFILES_CLEARRECENTFILELIST, MF_BYCOMMAND,
				ID_RECENTFILES_3, m_PathName[2] );
	if( m_CurNum>3 )
		pMruMenu->InsertMenu( ID_RECENTFILES_CLEARRECENTFILELIST, MF_BYCOMMAND,
				ID_RECENTFILES_4, m_PathName[3] );
}


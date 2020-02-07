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
////////////////////////////////////////////////////////////////
// MSDN Magazine -- November 2003
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio .NET on Windows XP. Tab size=3.
//
#pragma once
#include "PupText.h"
#include "subclass.h"

//////////////////
// Implement menu tips for any MFC main window. To use:
//
// - instantiate CMenuTipManager in your CMainFrm
// - call Install
// - implement prompt strings the normal way: as resource strings w/ID=command ID.
//
class CMenuTipManager : public CSubclassWnd {
protected:
	CPopupText m_wndTip;		// home-grown "tooltip"
	BOOL m_bMouseSelect;		// whether menu invoked by mouse
	BOOL m_bSticky;			// after first tip appears, show rest immediately

public:
	int m_iDelay;				// tooltip delay: you can change

	CMenuTipManager() : m_iDelay(1000), m_bSticky(FALSE) { }
	~CMenuTipManager() { }

	// call this to install tips
	void Install(CWnd* pWnd) { HookWindow(pWnd); }

	// Useful helpers to get window/rect of current active menu
	static CWnd* GetRunningMenuWnd();
	static void  GetRunningMenuRect(CRect& rcMenu);
	CRect GetMenuTipRect(HMENU hmenu, UINT nID);

	// Useful helper to get the prompt string for a command ID.
	// Like CFrameWnd::GetMessageString, but you don't need a frame wnd.
	static CString GetResCommandPrompt(UINT nID);

	// Get the prompt for given command ID
	virtual CString OnGetCommandPrompt(UINT nID)
	{
		return GetResCommandPrompt(nID);
	}

	// hook fn to trap main window's messages
	virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);

	// Call these handlers from your main window
	void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hMenu);
	void OnEnterIdle(UINT nWhy, HWND hwndWho);
};


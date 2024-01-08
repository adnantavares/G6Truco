
// G6Truco.h : main header file for the G6Truco application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CG6TrucoApp:
// See G6Truco.cpp for the implementation of this class
//

class CG6TrucoApp : public CWinApp
{
public:
	CG6TrucoApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CG6TrucoApp theApp;

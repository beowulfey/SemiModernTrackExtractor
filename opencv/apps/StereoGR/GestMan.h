/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/#if !defined(AFX_GESTREC_H__BECEAC80_7220_4247_AFDC_1BEB5937236A__INCLUDED_)
#define AFX_GESTREC_H__BECEAC80_7220_4247_AFDC_1BEB5937236A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GestRec.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGestRec dialog

class CGestRec : public CDialog
{
// Construction
public:
	void UpdateStatus();
	CGestRec(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGestRec)
	enum { IDD = IDD_GESTREC };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGestRec)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGestRec)
	afx_msg void OnRtrain();
	afx_msg void OnRreco();
	afx_msg void OnSelchangeGestlist();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnRnothing();
	afx_msg void OnBcleargest();
	afx_msg void OnBsavegest();
	afx_msg void OnBloadgest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GESTREC_H__BECEAC80_7220_4247_AFDC_1BEB5937236A__INCLUDED_)

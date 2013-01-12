
// (C) Copyright 2002-2005 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- CDialogSteel.h : Declaration of the CDialogSteel
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "afxwin.h"

	//-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� H�͸��û�����Ի���
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ��
	//          
	//
	//----------------------------------------------------------------------------------------------
class CDialogSteel : public CAcUiDialog {
	DECLARE_DYNAMIC (CDialogSteel)

public:
	CDialogSteel (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DIALOG_STEEL} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	// ѡ�㰴ť
	CAcUiPickButton m_btnPickPonit;
protected:
	// -----------------------------------------------------------------------------
	virtual BOOL OnInitDialog(void);
public:
	// X����
	CEdit m_ctrlPtX;
public:
	// Y����
	CEdit m_ctrlPtY;
public:
	afx_msg void OnBnClickedButtonPick();
public:

public:
	double m_dPtX;
public:
	double m_dPtY;
public:
	double m_dPtZ;
public:
	// �����
	double m_dH;
public:
	// ��Ե��
	double m_dW;
public:
	// ��Ե��
	double m_dA;
public:
	// �����
	double m_dB;
public:
	// ����
	double m_dL;
public:
	afx_msg void OnBnClickedOk();
} ;

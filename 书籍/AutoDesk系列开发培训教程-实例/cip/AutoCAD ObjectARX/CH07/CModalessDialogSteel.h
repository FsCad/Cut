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
//----- CModelessDialogSteel.h : Declaration of the CModelessDialogSteel
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"

//-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ�  ��ģʽ�Ի�����
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ��
	//          
	//
	//----------------------------------------------------------------------------------------------
class CModelessDialogSteel : public CAcUiDialog {
	DECLARE_DYNAMIC (CModelessDialogSteel)

public:
	CModelessDialogSteel (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_MODALESSDIALOG_STEEL} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
protected:
	// -----------------------------------------------------------------------------
	virtual void PostNcDestroy(void);
public:
	// �����ߵĶ���ID
	AcDbObjectId m_idPoly;
public:
	afx_msg void OnBnClickedButtonPick();
public:
	// ���쳤��
	double m_dL;
public:
	afx_msg void OnBnClickedOk();
} ;

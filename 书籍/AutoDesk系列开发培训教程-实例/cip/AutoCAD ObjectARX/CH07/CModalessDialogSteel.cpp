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
//----- CModelessDialogSteel.cpp : Implementation of CModelessDialogSteel
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "CModalessDialogSteel.h"
#include "CSolidHelper.h"

extern CModelessDialogSteel*   g_pDlgModeless;

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CModelessDialogSteel, CAcUiDialog)

BEGIN_MESSAGE_MAP(CModelessDialogSteel, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDCANCEL, &CModelessDialogSteel::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_Pick, &CModelessDialogSteel::OnBnClickedButtonPick)
	ON_BN_CLICKED(IDOK, &CModelessDialogSteel::OnBnClickedOk)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CModelessDialogSteel::CModelessDialogSteel (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CModelessDialogSteel::IDD, pParent, hInstance) , m_dL(0)
{
	m_idPoly=  AcDbObjectId::kNull ;
	m_dL = 200;
}

//-----------------------------------------------------------------------------
void CModelessDialogSteel::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
		DDX_Text(pDX, IDC_EDIT_LEN, m_dL);

}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CModelessDialogSteel::OnAcadKeepFocus (WPARAM, LPARAM) {
	
	return (TRUE) ;//
}

void CModelessDialogSteel::OnBnClickedCancel()
{
	CAcUiDialog::OnCancel () ;
	DestroyWindow();
}

// -----------------------------------------------------------------------------
void CModelessDialogSteel::PostNcDestroy(void)
{
		delete this;
		if(g_pDlgModeless)
		{
			g_pDlgModeless = NULL;
		}
}
//-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ�  �û�������ѡ��������
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ��
	//          
	//
	//----------------------------------------------------------------------------------------------
void CModelessDialogSteel::OnBnClickedButtonPick()
{
	// ��������ڵ�ǰ��ͼ
	acedGetAcadDwgView()->SetFocus();
	ads_name en;
	ads_point pt;
	int ret = acedEntSel(_T("\n��ѡ�������ߣ������ߣ�:\n"), en, pt);
	if(ret == RTNORM)
	{
		Acad::ErrorStatus es  = acdbGetObjectId(m_idPoly, en);
	}
}

	//-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ�  ��������������ʵ��
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ��
	//          
	//
	//----------------------------------------------------------------------------------------------

void CModelessDialogSteel::OnBnClickedOk()
{
	if(m_idPoly == AcDbObjectId::kNull )
	{
		acutPrintf(_T("û��ѡ�������ߣ������ߣ����˳�"));
		return;
	}
	// �����ĵ�
	 AcApDocument* pDoc = acDocManager->curDocument();
    Acad::ErrorStatus es = acDocManager->lockDocument(pDoc);

	AcDbEntity *pEnt = NULL;
	if(Acad::eOk == acdbOpenObject(pEnt, m_idPoly, AcDb::kForWrite))
	{
		AcDbPolyline* pPolyline = NULL;
		if(pEnt->isKindOf (AcDbPolyline::desc ()))
		{
					pPolyline = AcDbPolyline::cast(pEnt);
					AcDbObjectId idSolid;
					CSolidHelper::extrudePoly(pPolyline,m_dL,idSolid);
		}
		else
		{
			acutPrintf(_T(" ��ѡ������������ߣ������ߣ����˳�\n"));
		}
		pEnt->close();
	 }
	// �������ǰ��ͼ
	acedGetAcadDwgView()->SetFocus();
	// �����ĵ�
	acDocManager->unlockDocument(pDoc);
}

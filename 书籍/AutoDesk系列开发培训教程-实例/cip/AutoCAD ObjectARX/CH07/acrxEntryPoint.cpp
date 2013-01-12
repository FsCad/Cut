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
//----- acrxEntryPoint.h
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "CDialogSteel.h"
#include "CModalessDialogSteel.h"

 CModelessDialogSteel*  g_pDlgModeless;

//-----------------------------------------------------------------------------
#define szRDS _RXST("CS")
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint

class CCH07App : public AcRxArxApp {

public:
	CCH07App () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

public:

	//-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� ����ģʽ�Ի���
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ��
	//          
	//
	//----------------------------------------------------------------------------------------------
	// - CSCH07.CreateSteel command (do not rename)
	static void CSCH07CreateSteel(void)
	{
		//��ʽ������Դ
		CAcModuleResourceOverride resOverride;
		CDialogSteel dlgSteel;
		dlgSteel.DoModal();
   }
public:
	//-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ�  ������ģʽ�Ի���
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ��
	//          
	//
	//----------------------------------------------------------------------------------------------
	// - CSCH07.ModalessSteel command (do not rename)
	static void CSCH07ModalessSteel(void)
	{
		// ������ģʽ�Ի���
		CAcModuleResourceOverride resOverride;
		//CModelessDialogSteel *
		if(!g_pDlgModeless)
		{
		g_pDlgModeless = new CModelessDialogSteel();
		g_pDlgModeless->Create(CModelessDialogSteel::IDD);
		g_pDlgModeless->ShowWindow(SW_SHOW);
		}
		else
		{
			g_pDlgModeless->ShowWindow(SW_SHOW);
		}
	}
} ;


//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCH07App)

ACED_ARXCOMMAND_ENTRY_AUTO(CCH07App, CSCH07, CreateSteel, CreateSteel, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CCH07App, CSCH07, ModalessSteel, MSteel, ACRX_CMD_TRANSPARENT, NULL)

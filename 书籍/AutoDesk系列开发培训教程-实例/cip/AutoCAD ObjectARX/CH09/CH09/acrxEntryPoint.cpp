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

//-----------------------------------------------------------------------------
#define szRDS _RXST("CS")
#include "CCmdMonitorReactor.h"
#include "CSDatabaseReactor.h"

// �༭��Ӧ��
CCmdMonitorReactor   *gEditorReactor = NULL; 
//���ݿⷴӦ��
CSDatabaseReactor *gDbReactor = NULL; 

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CCH09App : public AcRxArxApp {

public:
	CCH09App () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
	
		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here
		// �����༭��Ӧ������ʵ��
		if (gEditorReactor == NULL)
			gEditorReactor = new CCmdMonitorReactor ();

		// �������ݿⷴӦ������ʵ��
		if (gDbReactor == NULL)
			gDbReactor = new CSDatabaseReactor (acdbHostApplicationServices()->workingDatabase());


		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		// ����Ӧ�������Ƴ�
		if (gEditorReactor!= NULL)
		{
				delete gEditorReactor;
				gEditorReactor = NULL;
		}
		// ����Ӧ�������Ƴ�
		if (gDbReactor!= NULL)
		{
				delete gDbReactor;
				gDbReactor = NULL;
		}
		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCH09App)


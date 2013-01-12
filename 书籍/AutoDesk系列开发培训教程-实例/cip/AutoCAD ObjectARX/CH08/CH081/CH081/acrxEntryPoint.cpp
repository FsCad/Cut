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
#include "CPipeAttribute.h"
#include <tchar.h>
//-----------------------------------------------------------------------------
#define szRDS _RXST("CS")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CCH081App : public AcRxArxApp {

public:
	CCH081App () : AcRxArxApp () {}

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
	//  ���ܣ� ����AcDbObject�������ݿ���󱣴浽ʵ�����չ�ʵ���
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ��
	//          2007.10.08 �޸� by Qin H.X.
	//
	//----------------------------------------------------------------------------------------------
	// - CSCH081.AddAttribute command (do not rename)
	static void CSCH081AddAttribute(void)
	{
		AcDbObjectId dictObjId,eId, attId;
		AcDbDictionary* pDict;
		//ѡ��ܵ��������ߣ�
		ads_name en;
		ads_point pt;
		if (  acedEntSel(_T("\nѡ��ܵ��������ߣ�: "), en, pt)!= RTNORM)
		{        
			acutPrintf(_T("\nѡ��ʧ�ܣ��˳�: "));
			return ;
		}
		// �򿪶���
		acdbGetObjectId(eId, en);
		AcDbEntity * pEnt;
		acdbOpenObject(pEnt, eId,  AcDb::kForWrite);
		if(!pEnt->isKindOf (AcDbPolyline::desc ()))
		{
			acutPrintf(_T("\nѡ��Ĳ��ǹܵ��������ߣ����˳�: " ));
			return ;
		}

		// �ж�ʵ�����չ�ʵ��Ƿ񴴽������û���򴴽�
		dictObjId = pEnt->extensionDictionary();
		if(	dictObjId ==  AcDbObjectId::kNull )
		{
			pEnt->createExtensionDictionary();
		}

		// ��ȡʵ�����չ�ʵ�
		dictObjId = pEnt->extensionDictionary();
		pEnt->close();

		// 	�жϴʵ��е������Ƿ񴴽�	
		CPipeAttribute* pAttribute;
		acdbOpenObject(pDict, dictObjId, AcDb::kForWrite);
		pDict->getAt (_T("����"),attId);
		if(attId!= AcDbObjectId::kNull )//����Ѿ��������������
		{
			acdbOpenObject(pAttribute, attId, AcDb::kForRead);
			acutPrintf(_T("\n�ܾ���%4.2f " ),pAttribute->m_dRadius);
			acutPrintf(_T("\n�ں�%4.2f " ),pAttribute->m_dThickness );
			acutPrintf(_T("\n���%4.2f " ),pAttribute->m_dDeep );
			acutPrintf(_T("\n���ʣ�%s " ),pAttribute->m_cMaterial  );
		}
		else
		{
			//û���򴴽�����
			 pAttribute = new CPipeAttribute();
			pDict->setAt(_T("����"), pAttribute, attId);
		}
		//�رն���
		pDict->close();
		pAttribute->close();

	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCH081App)

ACED_ARXCOMMAND_ENTRY_AUTO(CCH081App, CSCH081, AddAttribute, AddAttribute, ACRX_CMD_TRANSPARENT, NULL)

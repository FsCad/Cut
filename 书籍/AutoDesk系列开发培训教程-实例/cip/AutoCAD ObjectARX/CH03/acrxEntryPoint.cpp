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
#include "tchar.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("CGD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CCH03App : public AcRxArxApp {

public:
	CCH03App () : AcRxArxApp () {}

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
	//  ���ܣ� ��������Ӽ�¼�������²�)
	//                 
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
	// - CGDCH03.AddLayer command (do not rename)
	static void CGDCH03AddLayer(void)
	{
		//������һ���յĲ��ָ��
		AcDbLayerTable *pLayerTbl;  
		//ͨ����ǰͼ�����ݿ��ȡ������ָ��
		//�򿪲��Ϊд��״̬
		acdbHostApplicationServices()->workingDatabase()  ->getSymbolTable(pLayerTbl, AcDb::kForWrite); 

		//�жϲ��Ƿ��Ѿ�����
		if(!pLayerTbl->has(_T("MyLayer"))) {   
			//�²���¼
			AcDbLayerTableRecord *pLayerTblRcd=  new AcDbLayerTableRecord;
			pLayerTblRcd->setName(_T("MyLayer"));   //�趨ͼ����
			pLayerTblRcd->setIsFrozen(0);   // ͼ��ⶳ
			pLayerTblRcd->setIsOff(0);      // ��ͼ��
			pLayerTblRcd->setVPDFLT(0);   // ʹ��Ĭ���ӿ�
			pLayerTblRcd->setIsLocked(0);   // ͼ�����
			// AcCmColor��ACAD��ɫ������
			AcCmColor color;    
			color.setColorIndex(1);  
			// ͼ����ɫΪ��ɫ    
			pLayerTblRcd->setColor(color);  
			// Ϊ����ͼ���������ͣ�Ҫ�õ����ͱ��¼��ID��
			//  ���µĴ�����ʾ��εõ���������¼ID
			AcDbLinetypeTable *pLinetypeTbl;
			AcDbObjectId ltId;
			acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLinetypeTbl, AcDb::kForRead);
			if ((pLinetypeTbl->getAt(_T("DASHED"), ltId))!= Acad::eOk)	{  
				acutPrintf(_T("\nδ����DASHED����ʹ��CONTINUOUS����"));
				// ÿһ������ȫ�յ�ͼ�����ݿ�����ͱ��ж���������ΪCONTINUOUS ��Ĭ�ϼ�¼
				pLinetypeTbl->getAt(_T("CONTINUOUS"), ltId);
			}
			pLinetypeTbl->close();
			pLayerTblRcd->setLinetypeObjectId(ltId);
			pLayerTbl->add(pLayerTblRcd);
			pLayerTblRcd->close();
			pLayerTbl->close();
		} else {
			pLayerTbl->close();
			acutPrintf(_T("\n���Ѿ�����"));
		}

	}
public:
	//-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� �������ͱ��е����м�¼����ò���ӡ������
	//                 
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
	// - CGDCH03.IterateLinetype command (do not rename)
	static void CGDCH03IterateLinetype(void)
	{
		// ��ȡ���ͱ�
		AcDbLinetypeTable *pLinetypeTbl;
		acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pLinetypeTbl, AcDb::kForRead);

		// �������Ա�ı�����
		AcDbLinetypeTableIterator *pLtIterator;
		pLinetypeTbl->newIterator(pLtIterator);

		// �������Ա����ÿ�����Ա��¼������
		AcDbLinetypeTableRecord *pLtTableRcd;
		const TCHAR *pLtName;
		for (; !pLtIterator->done(); pLtIterator->step()) {
			pLtIterator->getRecord(pLtTableRcd, AcDb::kForRead);
			pLtTableRcd->getName(pLtName);
			pLtTableRcd->close();
			acutPrintf(_T("\n��������:  %s"), pLtName);
		}
		// ɾ�����Ա���������ر����Ա�
		delete pLtIterator;
		pLinetypeTbl->close();

	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCH03App)


ACED_ARXCOMMAND_ENTRY_AUTO(CCH03App, CGDCH03, AddLayer, AddLayer, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CCH03App, CGDCH03, IterateLinetype, IterateLinetype, ACRX_CMD_TRANSPARENT, NULL)

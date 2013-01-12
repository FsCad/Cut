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
class CCH04App : public AcRxArxApp {

public:
	CCH04App () : AcRxArxApp () {}

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
	//  ���ܣ� ɾ������ʵ��-AcDb2dPolyline
	//                 
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
	static void delete2dPolyline(AcDb2dPolyline* pPline)
	{ 
		//�������������
		AcDbObjectIterator* pIter=pPline->vertexIterator();
		AcDbEntity* pEnt; 
		for (; !pIter->done(); ) {
			//����ɾ������
			pEnt=pIter->entity(); 
			pIter->step();
			delete pEnt;
		}
		//ɾ����������ɾ��polyline
		delete pIter;
		pPline->erase();
	}
   //-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� ��������ʵ��-AcDb2dPolyline���Ӷ���
	//                 
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
static void IteratePolyline(AcDb2dPolyline *pLine)
{
	//��polyline����
	//AcDb2dPolyline *pPline;
	//acdbOpenObject(pPline, plineId, AcDb::kForRead);
	//�������������ָ��
	AcDbObjectIterator *pVertIter= pLine->vertexIterator();
	//pPline->close();  
	AcDb2dVertex *pVertex;
	AcGePoint3d location;
	AcDbObjectId vertexObjId;
	//ѭ������polyline����
	for (int vertexNumber = 0; !pVertIter->done();	vertexNumber++, pVertIter->step())
	{
		vertexObjId = pVertIter->objectId();
		acdbOpenObject(pVertex, vertexObjId,AcDb::kForRead);
		location = pVertex->position();
		pVertex->close();
		acutPrintf(_T("���� #%dλ�ã� %0.3f, %0.3f, %0.3f"), vertexNumber,location[X], location[Y], location[Z]);
	}
	//ɾ��������
	delete pVertIter;
}
   //-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� �������ȡ�鶨��Ķ���ID
	//                 
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
static void CreateBlockDef(AcDbObjectId &blockTableRecordId)
{

	//�򿪿��
	AcDbBlockTable *pBlockTable = NULL;
	acdbHostApplicationServices()->workingDatabase()
		->getSymbolTable(pBlockTable, AcDb::kForWrite);
	// 
	if(!pBlockTable->has (_T("MyBlockName")))
	{
		//�½�����¼������������
		AcDbBlockTableRecord *pBlockTableRec = new AcDbBlockTableRecord();
		pBlockTableRec->setName(_T("MyBlockName"));
		// 
		//����½�����¼�������
		pBlockTable->add(blockTableRecordId, pBlockTableRec);
		// 
		//�½�ʵ����󲢸��ӵ�����¼��
		AcDbLine *pLine = new AcDbLine();
		AcDbObjectId lineId;
		pLine->setStartPoint(AcGePoint3d(0, 0, 0));
		pLine->setEndPoint(AcGePoint3d(6, 6, 0));
		pLine->setColorIndex(3);
		pBlockTableRec->appendAcDbEntity(lineId, pLine);
		pLine->close();
		// ����һ��Բ
		AcDbCircle* pCircle = new AcDbCircle();
		pCircle->setCenter (AcGePoint3d(0, 0, 0));
		pCircle->setColorIndex(1);
		pCircle->setRadius (2.0);
		AcDbObjectId cirId;
		pBlockTableRec->setOrigin (AcGePoint3d(0, 0, 0));
		pBlockTableRec->appendAcDbEntity(cirId, pCircle);
		pCircle->close();//�ر�ʵ�����

		//�رտ���¼����
		pBlockTableRec->close();
	}
	else
	{
		pBlockTable->getAt (_T("MyBlockName"),blockTableRecordId);
	}
	//�رտ��
	pBlockTable->close();

}

	//-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� ����ֱ�߶���
	//                 
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
	// - CGDCH04.AddLine command (do not rename)
	static void CGDCH04AddLine(void)
	{
		//�½�ʵ�����
		AcGePoint3d startPt(4.0, 2.0, 0.0);
		AcGePoint3d endPt(10.0, 7.0, 0.0);
		AcDbLine *pLine = new AcDbLine(startPt, endPt);
		//�򿪿���¼��
		AcDbBlockTable *pBlockTable;
		acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pBlockTable, AcDb::kForRead);
		AcDbBlockTableRecord *pBlockTableRecord;
		pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
			AcDb::kForWrite);
		pBlockTable->close();
		//���ʵ����󵽿���¼��
		AcDbObjectId lineId;
		pBlockTableRecord->appendAcDbEntity(lineId, pLine);
		//�رտ���¼��ʵ�壺
		pBlockTableRecord->close();
		pLine->close();
		return ;

	}
public:
	//-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� ��������ʵ��-AcDb2dPolyline
	//                 
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
	// - CGDCH04.AddPolyline command (do not rename)
	static void CGDCH04AddPolyline(void)
	{
		//����pline�Ķ�������
		AcGePoint3dArray ptArr;
		ptArr.setLogicalLength(4);
		for (int i = 0; i < 4; i++) {
			ptArr[i].set((double)(i/2), (double)(i%2), 0.0);
		}
		//�½�AcDb2dPolyline����
		AcDb2dPolyline *pPolyline = new AcDb2dPolyline(
			AcDb::k2dSimplePoly, ptArr, 0.0, Adesk::kTrue);
		//������ɫ
		pPolyline->setColorIndex(3);
		//
		//�õ�������
		AcDbBlockTable *pBlockTable;
		acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pBlockTable, AcDb::kForRead);
		// 
		//�õ�ģ�Ϳռ�
		AcDbBlockTableRecord *pBlockTableRecord;
		pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
		pBlockTable->close();
		// 
		//����AcDb2dPolyline����ģ�Ϳռ�
		AcDbObjectId plineObjId;
		pBlockTableRecord->appendAcDbEntity(plineObjId, pPolyline);
		pBlockTableRecord->close();
		//
		//����ͼ��
		pPolyline->setLayer(_T("0"));
		pPolyline->close();

	}
public:
//-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ��༭ʵ��-�޸���ɫ
	//                 
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
	// - CGDCH04.ChangeColor command (do not rename)
	static void CGDCH04ChangeColor(void)
	{
		//ѡ�����
		ads_name en;
		ads_point pt;
		acedEntSel( _T("\nѡ��ʵ��: "), en, pt); 
		AcDbObjectId id;
		//ת��ads_nameΪAcDbObjectId
		acdbGetObjectId(id,en);
		AcDbEntity * pEnt;
		//�򿪶���
		acdbOpenObject(pEnt, id, AcDb::kForRead );
		//���򿪵Ķ���ת��ΪAcDbLine����ָ��
		if(pEnt->isKindOf (AcDbLine::desc())){
			// �༭���� 
			AcDbLine* pLine = AcDbLine::cast(pEnt);
			pLine->upgradeOpen ();
			pLine->setColorIndex (1);
		}
		//�رն���
		pEnt->close();

	}
public:
//-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� ��������ʵ��-AcDb2dPolyline���Ӷ���
	//                 
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
	// - CGDCH04.ItePolyline command (do not rename)
	static void CGDCH04ItePolyline(void)
	{
		//ѡ�����
		ads_name en;
		ads_point pt;
		acedEntSel( _T("\nѡ��ʵ��: "), en, pt); 
		AcDbObjectId id;
		//ת��ads_nameΪAcDbObjectId
		acdbGetObjectId(id,en);
		AcDbEntity * pEnt;
		//�򿪶���
		acdbOpenObject(pEnt, id, AcDb::kForRead );
		//���򿪵Ķ���ת��ΪAcDb2dPolyline����ָ��
		if(pEnt->isKindOf (AcDb2dPolyline::desc())){
			// �༭���� 
			AcDb2dPolyline* pLine = AcDb2dPolyline::cast(pEnt);
			IteratePolyline(pLine);
		}
		//�رն���
		pEnt->close();
	}
public:
    //-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� ɾ������ʵ��-AcDb2dPolyline
	//                 
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
	// - CGDCH04.DelPolyline command (do not rename)
	static void CGDCH04DelPolyline(void)
	{
		//ѡ�����
		ads_name en;
		ads_point pt;
		acedEntSel( _T("\nѡ��ʵ��: "), en, pt); 
		AcDbObjectId id;
		//ת��ads_nameΪAcDbObjectId
		acdbGetObjectId(id,en);
		AcDbEntity * pEnt;
		//�򿪶���
		acdbOpenObject(pEnt, id, AcDb::kForWrite );
		//���򿪵Ķ���ת��ΪAcDb2dPolyline����ָ��
		if(pEnt->isKindOf (AcDb2dPolyline::desc())){
			// �༭���� 
			AcDb2dPolyline* pLine = AcDb2dPolyline::cast(pEnt);
			delete2dPolyline(pLine);
			// �رն���
			pLine->close();
		}
		// �ػ���
		acedCommand(RTSTR,_T("regen"),RTNONE);

	}
public:
    //-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� �����
	//                 
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
	// - CGDCH04.InsertBlock command (do not rename)
	static void CGDCH04InsertBlock(void)
	{

		//ѡȡ�����ò����
		AcGePoint3d basePoint;
		if (acedGetPoint(NULL, _T("\n�������㣺 "), asDblArray(basePoint)) != RTNORM)
			return;

		//����һ�������ԵĿ鶨��
		AcDbObjectId blockId;
		CreateBlockDef (blockId);
		// 
		//�½������ö���
		AcDbBlockReference *pBlkRef = new AcDbBlockReference;
		// 
		//ָ�������������õĿ鶨���ID
		pBlkRef->setBlockTableRecord(blockId);

		AcGeVector3d normal(0.0, 0.0, 1.0);
		// 
		//���ÿ����õĲ���㡢��ת�ǶȺ�����
		pBlkRef->setPosition(basePoint);
		pBlkRef->setRotation(0.0);
		pBlkRef->setNormal(normal);

		//�򿪵�ǰ���ݿ��ģ�Ϳռ�
		AcDbBlockTable *pBlockTable;
		acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pBlockTable, AcDb::kForRead);
		AcDbBlockTableRecord *pBlockTableRecord;
		pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
		pBlockTable->close();
		//�������Ŀ����ø��ӵ�ģ�Ϳռ����¼
		AcDbObjectId newEntId;
		pBlockTableRecord->appendAcDbEntity(newEntId, pBlkRef);
		pBlockTableRecord->close();

		pBlkRef->close();

	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCH04App)

ACED_ARXCOMMAND_ENTRY_AUTO(CCH04App, CGDCH04, AddLine, AddLine, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CCH04App, CGDCH04, AddPolyline, AddPolyline, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CCH04App, CGDCH04, ChangeColor, ChangeColor, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CCH04App, CGDCH04, ItePolyline, ItePolyline, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CCH04App, CGDCH04, DelPolyline, DelPolyline, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CCH04App, CGDCH04, InsertBlock, InsertBlock, ACRX_CMD_TRANSPARENT, NULL)

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
class CCH05App : public AcRxArxApp {

public:
	CCH05App () : AcRxArxApp () {}

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
	//  ���ܣ� �����չ������֧�ֵĽ����������
	//                 
	//
	//  ���ߣ� 
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
	static void printList(struct resbuf* pRb){
    int rt, i;
    TCHAR buf[133];

    for (i = 0;pRb != NULL;i++, pRb = pRb->rbnext) {
        if (pRb->restype < 1010) {
            rt = RTSTR;
        } else if (pRb->restype < 1040) {
            rt = RT3DPOINT;
        } else if (pRb->restype < 1060) {
            rt = RTREAL;
        } else if (pRb->restype < 1071) {
            rt = RTSHORT;
        } else if (pRb->restype == 1071) {
            rt = RTLONG;
        } else {// 
            rt = pRb->restype; //δ֪����
        }

        switch (rt) {
        case RTSHORT:
            if (pRb->restype == RTSHORT) {
                acutPrintf( _T("�����������ݣ�RTSHORT��Ϊ : %d\n"), pRb->resval.rint);
            } else {
                acutPrintf(_T("(%d . %d)\n"), pRb->restype, pRb->resval.rint);
            };
            break;

        case RTREAL:
            if (pRb->restype == RTREAL) {
                acutPrintf(_T("�����������ݣ�RTREAL��Ϊ: %0.3f\n"),
                    pRb->resval.rreal);
            } else {
                acutPrintf(_T("(%d . %0.3f)\n"), pRb->restype,
                    pRb->resval.rreal);
            };
            break;

        case RTSTR:
            if (pRb->restype == RTSTR) {
                acutPrintf(_T("�ַ��������ݣ�RTSTR��Ϊ: %s\n"),
                    pRb->resval.rstring);
            } else {
                acutPrintf(_T("(%d . \"%s\")\n"), pRb->restype,
                    pRb->resval.rstring);
            };
            break;

        case RT3DPOINT:
            if (pRb->restype == RT3DPOINT) {
                acutPrintf(
                    _T("���ε��������ݣ�RT3DPOINT��Ϊ: %0.3f, %0.3f, %0.3f\n"),
                    pRb->resval.rpoint[X],
                    pRb->resval.rpoint[Y],
                    pRb->resval.rpoint[Z]);
            } else {
                acutPrintf(_T("(%d %0.3f %0.3f %0.3f)\n"),
                    pRb->restype,
                    pRb->resval.rpoint[X],
                    pRb->resval.rpoint[Y],
                    pRb->resval.rpoint[Z]);
            }
            break;

        case RTLONG:
            acutPrintf(_T("�����������ݣ�RTLONG��Ϊ: %dl\n"), pRb->resval.rlong);
            break;
        }

        if ((i == 23) && (pRb->rbnext != NULL)) {
            i = 0;
            acedGetString(0,
                _T("�س�����..."), buf);
        }
    }
}
    //-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� �����չ��¼��֧�ֵĽ����������
	//                 
	//
	//  ���ߣ� 
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
static	void printListXRecord(struct resbuf* pBuf)
{
    int rt, i;
    TCHAR buf[133];

    for (i = 0;pBuf != NULL;i++, pBuf = pBuf->rbnext) {
        if (pBuf->restype < 0)
            // Entity name (or other sentinel)
            rt = pBuf->restype;
        else if (pBuf->restype < 10)
            rt = RTSTR;
        else if (pBuf->restype < 38)
            rt = RT3DPOINT;
        else if (pBuf->restype < 60)
            rt = RTREAL;
        else if (pBuf->restype < 80)
            rt = RTSHORT;
        else if (pBuf->restype < 100)
            rt = RTLONG;
        else if (pBuf->restype < 106)
            rt = RTSTR;
        else if (pBuf->restype < 148)
            rt = RTREAL;
        else if (pBuf->restype < 290)
            rt = RTSHORT;
        else if (pBuf->restype < 330)
            rt = RTSTR;
        else if (pBuf->restype < 370)
            rt = RTENAME;
        else if (pBuf->restype < 999)
            rt = RT3DPOINT;
        else // pBuf->restype is already RTSHORT, RTSTR,
            rt = pBuf->restype; // etc. or it is unknown.

        switch (rt) {
        case RTSHORT:
            if (pBuf->restype == RTSHORT)
                acutPrintf(
				_T("�����������ݣ�RTSHORT��Ϊ : %d\n"), pBuf->resval.rint);
            else
                acutPrintf(_T("(%d . %d)\n"), pBuf->restype,
                    pBuf->resval.rint);
            break;
        case RTREAL:
            if (pBuf->restype == RTREAL)
                acutPrintf(
                _T("�����������ݣ�RTREAL��Ϊ  : %0.3f\n"), pBuf->resval.rreal);
            else
                acutPrintf(_T("(%d . %0.3f)\n"),
                    pBuf->restype, pBuf->resval.rreal);
            break;
        case RTSTR:
            if (pBuf->restype == RTSTR)
                acutPrintf(
                _T(" �ַ��������ݣ�RTSTR��Ϊ : %s\n"), pBuf->resval.rstring);
            else
                acutPrintf(_T("(%d . \"%s\")\n"),
                    pBuf->restype, pBuf->resval.rstring);
            break;
        case RT3DPOINT:
            if (pBuf->restype == RT3DPOINT)
                acutPrintf(
                _T("���������ݣ�RT3DPOINT��Ϊ : %0.3f, %0.3f, %0.3f\n"),
                    pBuf->resval.rpoint[X],
                    pBuf->resval.rpoint[Y],
                    pBuf->resval.rpoint[Z]);
            else
                acutPrintf(_T("(%d %0.3f %0.3f %0.3f)\n"),
                    pBuf->restype,
                    pBuf->resval.rpoint[X],
                    pBuf->resval.rpoint[Y],
                    pBuf->resval.rpoint[Z]);
            break;
        case RTLONG:
            acutPrintf(_T("�����������ݣ�RTLONG��Ϊ : %dl\n"), pBuf->resval.rlong);
            break;
        case -1:
        case RTENAME: // First block entity
            acutPrintf(_T("(%d . <ʵ������: %8lx>)\n"),
                pBuf->restype, pBuf->resval.rlname[0]);
            break;
        case -3: // marks start of xdata
            acutPrintf(_T("(-3)\n"));
        }

        if ((i == 23) && (pBuf->rbnext != NULL)) {
            i = 0;
            acedGetString(0,
                _T("����س�����"), buf);
        }
    }
    return;
}

	//-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� Ϊ���ݿ����������չ����
	//                 
	//
	//  ���ߣ� 
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
	// - CGDCH05.AddXdata command (do not rename)
	static void CGDCH05AddXdata(void)
	{
		//ѡ�����
		ads_name en;
		ads_point pt;
		acedEntSel( _T("\nѡ��ʵ��: "), en, pt); 
		AcDbObjectId id;
		//ת��ads_nameΪAcDbObjectId
		acdbGetObjectId(id,en);
		AcDbObject * pObj;
		//�򿪶���
		acdbOpenObject(pObj, id, AcDb::kForWrite );
		//����Ӧ�ó���������ӵ���չ���ݵ��ַ�
		TCHAR appName[132], resString[200];
		appName[0] = resString[0] = _T('\0');
		acedGetString(NULL, _T("����XDATAע��Ӧ�ó�����: "),appName);
		acedGetString(NULL, _T("������ӵ���չ���ݵ��ַ���: "),resString);
		struct  resbuf  *pRb, *pTemp;
		pRb = pObj->xData(appName);
		if (pRb != NULL) {
			// 
			//����Ѿ�����չ�����ˣ���ָ��ָ����β��
			for (pTemp = pRb; pTemp->rbnext != NULL;
				pTemp = pTemp->rbnext)
			{ ; }
		} else {
			// 
			// �����û����չ���ݣ���ע��Ӧ�ó�����
			acdbRegApp(appName);
			pRb = acutNewRb(AcDb::kDxfRegAppName);
			pTemp = pRb;
			pTemp->resval.rstring
				= (TCHAR*) malloc(_tcslen(appName) + 1);
			_tcscpy(pTemp->resval.rstring, appName);
		}
		// 
		//����û����ݵ�resbuf����
		pTemp->rbnext = acutNewRb(AcDb::kDxfXdAsciiString);
		pTemp = pTemp->rbnext;
		pTemp->resval.rstring
			= (TCHAR*) malloc(_tcslen(resString) + 1);
		_tcscpy(pTemp->resval.rstring, resString);
		// 
		//������չ����
		pObj->upgradeOpen();
		pObj->setXData(pRb);
		pObj->close();
		acutRelRb(pRb);

	}
public:
   //-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� ��ȡ���ݿ����������չ����
	//                 
	//
	//  ���ߣ� 
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
	// - CGDCH05.GetXdata command (do not rename)
	static void CGDCH05GetXdata(void)
	{
		//ѡ�񲢴�ʵ��
		ads_name en;
		ads_point pt;
		acedEntSel( _T("\nѡ��ʵ��: "), en, pt); 
		AcDbObjectId id;
		//ת��ads_nameΪAcDbObjectId
		acdbGetObjectId(id,en);
		AcDbObject * pObj;
		//�򿪶���
		acdbOpenObject(pObj, id, AcDb::kForWrite );
		//���Ӧ�ó�����
		TCHAR appname[133];
		if (acedGetString(NULL,_T("\n����XDATAע��Ӧ�ó�������: "),
			appname) != RTNORM)
		{
			return;
		}
		// 
		//�õ���Ӧ�ó������չ�����б�
		struct resbuf *pRb;
		pRb = pObj->xData(appname);
		if (pRb != NULL) {

			//����д�Ӧ�ó������չ���ݣ����Բ���������
			 printList(pRb);
			acutRelRb(pRb);
		} else {
			acutPrintf(_T("\nû�д�Ӧ�ó����XDATA���ݡ�"));
		}
		pObj->close();

	}
public:
//-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� �����������ʵ�
	//                 
	//
	//  ���ߣ� 
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
	// - CGDCH05.CreateNodXrecord command (do not rename)
	static void CGDCH05CreateNodXrecord(void)
	{
		//�õ���������ʵ�ָ��
		AcDbDictionary *pNamedobj, *pDict;
		acdbHostApplicationServices()->workingDatabase()
			->getNamedObjectsDictionary(pNamedobj, AcDb::kForWrite);
		// ����Ƿ����йؼ���Ϊ�������Ϣ���Ĵʵ䣬��û�У��򴴽�
		if (pNamedobj->getAt(_T("�����Ϣ"), (AcDbObject*&) pDict,
			AcDb::kForWrite) == Acad::eKeyNotFound)
		{
			pDict = new AcDbDictionary;
			AcDbObjectId DictId;
			pNamedobj->setAt(_T("�����Ϣ"), pDict, DictId);
		}
		pNamedobj->close();
		//����AcDbXrecord���󣬲���ӵ���չ�ʵ�
		AcDbXrecord *pXrec = new AcDbXrecord;
		AcDbObjectId xrecObjId;
		pDict->setAt(_T("�����"), pXrec, xrecObjId);
		pDict->close();
		//ΪAcDbXrecord���󴴽�resbuf����
		struct resbuf *pHead;
		ads_point testpt = {1.0, 2.0, 0.0};
		pHead = acutBuildList(
			AcDb::kDxfText, _T(" "),   //����
			AcDb::kDxfText, _T("��Ʋ���"),  //���ڲ���
			AcDb::kDxfReal, 5000.0,         //нˮ
			AcDb::kDxfInt16, 4,             //��������
			0);
		// ΪAcDbXrecord������������
		pXrec->setFromRbChain(*pHead);
		acutRelRb(pHead);
		pXrec->close();

	}
public:
//-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� ��ȡ��������ʵ�������������
	//                 
	//
	//  ���ߣ� 
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
	// - CGDCH05.GetNodXrecord command (do not rename)
	static void CGDCH05GetNodXrecord(void)
	{
		AcDbDictionary *pNamedobj;
		acdbHostApplicationServices()->workingDatabase()
			->getNamedObjectsDictionary(pNamedobj, AcDb::kForRead);
		//�õ��ؼ���Ϊ"�����Ϣ"�Ĵʵ�
		AcDbDictionary *pDict;
		pNamedobj->getAt(_T("�����Ϣ"), (AcDbObject*&)pDict,
			AcDb::kForRead);
		pNamedobj->close();

		//�õ��ؼ���Ϊ"�����"��AcDbXrecord����
		AcDbXrecord *pXrec;
		pDict->getAt(_T("�����"), (AcDbObject*&) pXrec,
			AcDb::kForRead);
		pDict->close();
		struct resbuf *pRbList;
		pXrec->rbChain(&pRbList);
		pXrec->close();
		//���صĽ������Ĵ���ο�ʾ������
		printListXRecord(pRbList);
		acutRelRb(pRbList);

	}
public:
    //-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� Ϊ���ݿ���󴴽���չ�ʵ�
	//                 
	//
	//  ���ߣ� 
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
	// - CGDCH05.CreateXrecord command (do not rename)
	static void CGDCH05CreateXrecord(void)
	{

		//ѡ�񲢴�ʵ��
		ads_name en;
		ads_point pt;
		acedEntSel( _T("\nѡ��ʵ��: "), en, pt); 
		AcDbObjectId id;
		//ת��ads_nameΪAcDbObjectId
		acdbGetObjectId(id,en);
		//�򿪶���
		AcDbObject * pObj;
		acdbOpenObject(pObj, id, AcDb::kForWrite );
		//������չ�ʵ���Ҫ����Ķ���
		AcDbXrecord *pXrec = new AcDbXrecord;
		//ѡ��Ҫ�����չ�ʵ�Ķ���
		AcDbObjectId dictObjId, xrecObjId;
		AcDbDictionary* pDict;
		//Ϊѡ��Ķ��󴴽���չ�ʵ䣬�����У���ʲô������
		pObj->createExtensionDictionary();
		//�õ��˶������չ�ʵ��ID 
		dictObjId = pObj->extensionDictionary();
		pObj->close();
		//����չ�ʵ䣬��������չ�ʵ��¼
		acdbOpenObject(pDict, dictObjId, AcDb::kForWrite);
		pDict->setAt(_T("�ܵ�����"), pXrec, xrecObjId);
		pDict->close();
		//����AcDbXrecord����Ľ������
		struct resbuf* head;
		head = acutBuildList(
			AcDb::kDxfText, _T("TCH-0-8"), // ����
			AcDb::kDxfText, _T("��ˮ��-03"), // ����
			AcDb::kDxfText, _T(" ˮ���"), // ����
			AcDb::kDxfReal, 1.5,//�ܾ�
			AcDb::kDxfReal, 2.5,//����
			0);
		//ΪAcDbXrecord��������
		pXrec->setFromRbChain(*head);
		pXrec->close();
		acutRelRb(head);

	}
public:
//-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� ��ȡ���ݿ�������չ�ʵ�����
	//                 
	//
	//  ���ߣ� 
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
	// - CGDCH05.GetXrecord command (do not rename)
	static void CGDCH05GetXrecord(void)
	{
		//ѡ�񲢴�ʵ��
		ads_name en;
		ads_point pt;
		acedEntSel( _T("\nѡ��ʵ��: "), en, pt); 
		AcDbObjectId id;
		//ת��ads_nameΪAcDbObjectId
		acdbGetObjectId(id,en);
		//�򿪶���
		AcDbObject * pObj;
		acdbOpenObject(pObj, id, AcDb::kForWrite );

		//�õ��˶������չ�ʵ��ID	
		AcDbObjectId dictObjId;
		dictObjId = pObj->extensionDictionary();
		if(dictObjId== AcDbObjectId::kNull)
		{
			acutPrintf(_T("\nѡ��Ķ���û�������չ�ʵ䡣"));
			pObj->close();
			return;
		}
		pObj->close();
		//����չ�ʵ䣬����ȡ��չ��¼
		AcDbObjectId xrecObjId;
		AcDbDictionary* pDict;
		acdbOpenObject(pDict, dictObjId, AcDb::kForRead);

		//�õ��ؼ���Ϊ"�ܵ�����"��AcDbXrecord����
		AcDbXrecord *pXrec;
		pDict->getAt(_T("�ܵ�����"), (AcDbObject*&) pXrec,	AcDb::kForRead);
		pDict->close();
		struct resbuf *pRbList;
		pXrec->rbChain(&pRbList);
		pXrec->close();
		//���صĽ������Ĵ���
		printListXRecord(pRbList);
		acutRelRb(pRbList);
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCH05App)

ACED_ARXCOMMAND_ENTRY_AUTO(CCH05App, CGDCH05, AddXdata, AddXdata, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CCH05App, CGDCH05, GetXdata, GetXdata, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CCH05App, CGDCH05, CreateNodXrecord, CreateNodXrecord, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CCH05App, CGDCH05, GetNodXrecord, GetNodXrecord, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CCH05App, CGDCH05, CreateXrecord, CreateXrecord, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CCH05App, CGDCH05, GetXrecord, GetXrecord, ACRX_CMD_TRANSPARENT, NULL)

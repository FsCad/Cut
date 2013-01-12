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
class CCH02App : public AcRxArxApp {

public:
	CCH02App () : AcRxArxApp () {}

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
	//  ���ܣ� ��ȡ�û�����
	//                 
	//
	//  ���ߣ�Zhao C.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ��
	//      
	//----------------------------------------------------------------------------------------------
	// - CGDCH02.GetData command (do not rename)
	static void CGDCH02GetData(void)
	{
		ads_point insertPt;
		if(acedGetPoint(NULL,_T("\n��ָ��ͬ��Բ��Բ��:"),insertPt)!=RTNORM) return;

		double radius;		//����Բ�İ뾶

		int nCount = 3;
		acedInitGet(RSG_NONEG + RSG_NOZERO, NULL);	//������0�͸���
		acedGetInt(_T("\n������Ҫ����Բ�ĸ���<3>:"), &nCount);

		ads_real fDist = 20.0;	//Բ��Բ�ļ��
		acedInitGet(RSG_NONEG + RSG_NOZERO, NULL);
		acedGetDist(NULL, _T("\n������Բ��Բ�ļ��<20.0>:"), &fDist);
		//ʹ�û�ȡ�����ݡ�����
	}
public:
 //-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� ��ȡ�ؼ��֣�������ѡ��
	//                 
	//
	//  ���ߣ�Zhao C.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ��
	//      
	//----------------------------------------------------------------------------------------------
	// - CGDCH02.GetKWord command (do not rename)
	static void CGDCH02GetKWord(void)
	{
		//�û�����ʵ�����ɫ
		AcCmColor acColor;	
		TCHAR szKword [132];
		szKword[0] = _T('R');	//��szKwordһ��Ĭ��ֵR
		szKword[1] = _T('\0');
		//��ʼ���ؼ��ֱ���д��ĸ�ǹؼ��ֵļ�д
		acedInitGet(0, _T("Red Green Blue"));
		int nReturn;
		//ȡ���û�����Ĺؼ���
		nReturn = acedGetKword(_T("\n��ѡ��ʵ�����ɫRed/Green/Blue<Red>: "), szKword);
		if (nReturn == RTNORM)	//����õ�����Ĺؼ���
		{
			if (_tcscmp(szKword, _T("Red")) == 0)
				acColor.setColorIndex(1);	//��ɫ
			else if (_tcscmp(szKword, _T("Blue")) == 0)
				acColor.setColorIndex(5);	//��ɫ
			else
				acColor.setColorIndex(3);	//��ɫ
		}
		else if (nReturn == RTNONE)	//����û�����Ϊ��ֵ
		{
			acColor.setColorIndex(1);		//Ĭ�Ϻ�ɫ
		}

		//��ȡѡ��
		ads_name ssname;
		acutPrintf(_T("\n��ѡ��Ҫ�ı���ɫ��ʵ�壺"));
		acedSSGet(NULL, NULL, NULL, NULL, ssname);
		//�õ�ѡ�񼯵�ʵ����������Ϊ0�����˳�
		long ssLen = 0;
		acedSSLength(ssname, &ssLen);
		if (ssLen == 0)
			return;
		//����ѡ��
		ads_name ent;						
		AcDbEntity* pEnt = NULL;			
		AcDbObjectId objId;
		long i;
		for(i=0; i<ssLen; i++)
		{
			acedSSName(ssname, i, ent);		
			acdbGetObjectId(objId, ent);	

			if(Acad::eOk == acdbOpenObject(pEnt, objId, AcDb::kForWrite))
			{
				pEnt->setColor(acColor);	//����ʵ�����ɫ
				pEnt->close();
			}
		}
		//�ͷ�ѡ��
		acedSSFree(ssname);
	}
public:
    //-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� ���ڹ�����������ѡ��
	//                 
	//
	//  ���ߣ�Zhao C.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ��  
	//             
	//----------------------------------------------------------------------------------------------

	// - CGDCH02.SSFilter command (do not rename)
	static void CGDCH02SSFilter(void)
	{
				struct resbuf eb1,eb2,eb3;
		TCHAR sbuf1[20];
		TCHAR sbuf2[20];
		ads_name ssname;
		eb1.restype=0; 		//ʵ������
		_tcscpy(sbuf1, _T("LINE"));	//ֱ��
		eb1.resval.rstring=sbuf1;
		eb2.restype=8;			//ͼ����
		_tcscpy(sbuf2, _T("0"));
		eb2.resval.rstring=sbuf2;
		eb3.restype=62;		//ʵ����ɫ
		eb3.resval.rint=1;	//��ɫ
		// eb3.resval.rint=256;	//�����ɫ
		eb3.rbnext=NULL;
		//����������������������������
		eb1.rbnext=&eb2;		
		eb2.rbnext=&eb3;
		//������0ͼ�������к�ɫ��ֱ�ߣ�ѡ�񼯷���ssname��
		acutPrintf(_T("\n��ѡ��ʵ�壬ֻ��0ͼ���ϵĺ�ɫ��ֱ�߲��ܱ�ѡ�С�"));
		acedSSGet(NULL,NULL,NULL,&eb1,ssname);
		// ����ѡ��
		long ssLen = 0;
		acedSSLength(ssname, &ssLen);
		if (ssLen == 0)
			return;
		//����ѡ��
		ads_name ent;						
		AcDbEntity* pEnt = NULL;			
		AcDbObjectId objId;
		for(long i=0; i<ssLen; i++)
		{
			acedSSName(ssname, i, ent);		
			acdbGetObjectId(objId, ent);	
			if(Acad::eOk == acdbOpenObject(pEnt, objId, AcDb::kForWrite))
			{
				pEnt->setColorIndex(2);	//����ʵ�����ɫ
				pEnt->close();
			}
		}

		//�ͷ�ѡ��
		acedSSFree(ssname);
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCH02App)

ACED_ARXCOMMAND_ENTRY_AUTO(CCH02App, CGDCH02, GetData, GetData, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CCH02App, CGDCH02, GetKWord, GetKWord, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CCH02App, CGDCH02, SSFilter, SSFilter, ACRX_CMD_TRANSPARENT, NULL)

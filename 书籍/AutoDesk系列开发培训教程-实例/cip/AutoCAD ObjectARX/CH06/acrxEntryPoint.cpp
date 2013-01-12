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
#include <tchar.h>
#define PI 3.14159265358
//-----------------------------------------------------------------------------
#define szRDS _RXST("CGD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CCH06App : public AcRxArxApp {

public:
	CCH06App () : AcRxArxApp () {}

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
	//  ���ܣ� �ڶ���ֱ�ߵ�����ƶ�����һ��ֱ�ߵ��е�λ�ã�
	//                 ��ͨ��������ת����ʹ�õڶ���ֱ�ߺ͵�һ��ֱ�ߴ�ֱ
	//
	//  ���ߣ�Qin H.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ����
	//
	//----------------------------------------------------------------------------------------------
	// - CGDCH06.TransformLine command (do not rename)
	static void CGDCH06TransformLine(void)
	{


		ads_name      ename1,ename2;
		ads_point     pickpt;
		if (acedEntSel(_T("\nѡ��ֱ��1: "), ename1, pickpt)  != RTNORM)
		{
			acutPrintf(_T("\nѡ��ֱ��1ʧ��"));
			return ;
		}
		if (acedEntSel(_T("\nѡ��ֱ��2: "), ename2, pickpt)  != RTNORM)
		{
			acutPrintf(_T("\nѡ��ֱ��2ʧ��"));
			return ;
		}	

		AcDbObjectId IdLine1,IdLine2;

		acdbGetObjectId(IdLine1, ename1);		
		acdbGetObjectId(IdLine2, ename2);
		//ֱ�߶���
		AcDbLine *pLine1 = NULL;			
		AcDbLine *pLine2 = NULL;	
		//�Զ���ʽ��ʵ��
		if(Acad::eOk == acdbOpenObject(pLine1, IdLine1, AcDb::kForRead))
		{
			if(Acad::eOk == acdbOpenObject(pLine2, IdLine2, AcDb::kForRead))
			{

				// �����һ��ֱ�ߵ��е�
				AcGePoint3d ptMid; 
				ptMid.x =  0.5*(pLine1->endPoint ().x + pLine1->startPoint ().x);
				ptMid.y=  0.5*(pLine1->endPoint ().y + pLine1->startPoint ().y);
				ptMid.z =  0.5*(pLine1->endPoint ().z + pLine1->startPoint ().z);	

				// �ڶ���ֱ����㵽��һ��ֱ�ߵ�����
				AcGeVector3d vecMove = ptMid -  pLine2->startPoint ();
				// ����ƽ�ƾ���
				AcGeMatrix3d  mat,mat1;
				mat = mat.translation (vecMove);
				//  �޸ĵڶ���ֱ��
				pLine2->upgradeOpen ();
				pLine2->transformBy (mat);
				//��������ֱ�ߵļн�
				AcGeVector3d  vec1 = pLine1->endPoint ()- pLine1->startPoint ();
				AcGeVector3d  vec2 = pLine2->endPoint ()- pLine2->startPoint ();
				double dAng;
				dAng = vec1.angleTo(vec2);
				AcGeVector3d vecZ(0.0,0.0,1.0);
				//������ת�Ƕ�
				double dRot;
				if(dAng<0.5*PI)
				{
					dRot = 0.5*PI  - dAng;
				}
				else
				{
					dRot =dAng  -  0.5*PI ;
				}
				// ������ת����
				mat1  = mat1.rotation (dRot,vecZ,pLine2->startPoint ());
				//��ֱ�߽�����ת�任
				pLine2->transformBy (mat1);
				pLine2->setColorIndex (1);
				//�ر�ʵ��
				pLine2->close();
				acutPrintf(_T("\n ������� ��"));

			}
			//�ر�ʵ��
			pLine1->close();	

		}


		// Add your code for command CGDCH06.TransformLine here
	}
public:
    //-------------------------------------------------------------------------------------------
	// 
	//  ���ܣ� ��ֱ�ߵĽ���ͼн�
	//
	//  ���ߣ�Zhao C.X.
	//
	// ���ڣ�200709
	//
	//  ��ʷ��
	//          �������ִ��� BY	Qin H.X.
	//
	//----------------------------------------------------------------------------------------------
	// - CGDCH06.GetIntersect command (do not rename)
	static void CGDCH06GetIntersect(void)
	{

	ads_name      ename1,ename2;
    ads_point     pickpt;
    if (acedEntSel(_T("\nѡ��ֱ��1: "), ename1, pickpt)  != RTNORM)
    {
        acutPrintf(_T("\nѡ��ֱ��1ʧ��"));
        return ;
    }
    if (acedEntSel(_T("\nѡ��ֱ��2: "), ename2, pickpt)  != RTNORM)
    {
        acutPrintf(_T("\nѡ��ֱ��2ʧ��"));
        return ;
    }	
	
	AcDbObjectId IdLine1,IdLine2;

	acdbGetObjectId(IdLine1, ename1);		
	acdbGetObjectId(IdLine2, ename2);
	//ֱ�߶���
	AcDbLine *pLine1 = NULL;			
	AcDbLine *pLine2 = NULL;	
	//�Զ���ʽ��ʵ��
	if(Acad::eOk == acdbOpenObject(pLine1, IdLine1, AcDb::kForRead))
	{
		if(Acad::eOk == acdbOpenObject(pLine2, IdLine2, AcDb::kForRead))
		{
			//��������ֱ��
			AcGeLineSeg3d geLineSeg1, geLineSeg2;
			geLineSeg1.set(pLine1->startPoint(), pLine1->endPoint());	
			geLineSeg2.set(pLine2->startPoint(), pLine2->endPoint());

			//��ֱ�ߵĽ���
			AcGePoint3d ptIntersect;	
			//�������ֱ���н���
			if (geLineSeg1.intersectWith(geLineSeg2, ptIntersect))	
			{
				acutPrintf(_T("\n��ֱ�ߵĽ��㣺(%0.2f, %0.2f, %0.2f)"), ptIntersect.x, ptIntersect.y,ptIntersect.z);
			}
			else
			{
				acutPrintf(_T("\nֱ��û�н��㡣"));
			}

			///��ֱ�ߵļн�
			AcGeVector3d vect1, vect2;	
			vect1 = geLineSeg1.direction();
			vect2 = geLineSeg2.direction();
			double dAngle;
			dAngle = vect1.angleTo(vect2);
			dAngle *= 180.0 / PI;
			pLine2->close();
			acutPrintf(_T("\nֱ�ߵļн�Ϊ��%0.0f��"), dAngle);

		}
			//�ر�ʵ��
		pLine1->close();	

	}

	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCH06App)

ACED_ARXCOMMAND_ENTRY_AUTO(CCH06App, CGDCH06, TransformLine, TransformLine, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CCH06App, CGDCH06, GetIntersect, GetIntersect, ACRX_CMD_TRANSPARENT, NULL)

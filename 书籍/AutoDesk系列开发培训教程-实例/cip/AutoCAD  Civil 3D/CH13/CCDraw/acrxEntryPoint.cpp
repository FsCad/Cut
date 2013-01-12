// (C) Copyright 2003-2007 by Autodesk, Inc. 
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
#define szRDS _RXST("Adsk")

//Helper functions 
bool variantIsArrayOfDoubles( const VARIANT & );
void variantToDoublesArray( const VARIANT &, long *, double ** );
void doublesToVariant( const int , const double *, VARIANT * );
void ComErrorInfo(HRESULT );

//custom draw functions
bool customSurfaceDraw( const AeccDisplayOrientation &,IAeccSurface *,IAcadBlock *);
bool customAlignmentDraw( const AeccDisplayOrientation &,  IAeccAlignment *, IAcadBlock *);
void setDrawCallbacks( AeccCustomDraw & );

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CCDraw : public AcRxArxApp {

protected:
	static TCHAR *pszAppName ;
	static AeccCustomDraw s_customDraw ;

public:

	CCDraw () : AcRxArxApp () {
	
	}
	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;

        //ע��һ���������ƻص�����
		setDrawCallbacks( s_customDraw );

		return (retCode) ;
	}



	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

         //ȡ��������ע��Ļص�����  
		s_customDraw.clearAll();
		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

} ;

//---------------------------------------------------------------------------------------
//
//  ���ܣ� ע��һ���������ƻص�����
//
//  ���������
//
//  ����ֵ��bool
//
// ��  �ߣ�  
//
//  ���ڣ� 2007/08
//
//  �޸ļ�¼����
//
//---------------------------------------------------------------------------------------
void
setDrawCallbacks( AeccCustomDraw &customDraw )
{
	customDraw.setAlignmentDraw(customAlignmentDraw );
	customDraw.setSurfaceDraw(customSurfaceDraw);
}

//---------------------------------------------------------------------------------------
//
//  ���ܣ� ·�߻��ƻص�����
//
//  ���������
//             AeccDisplayOrientation &viewMode
//             IAeccAlignment *pAlign
//             IAcadBlock *pAnonymousBlock 
//
//  ����ֵ��bool
//
// ��  �ߣ�  
//
//  ���ڣ� 2007/08
//
//  �޸ļ�¼����
//
//---------------------------------------------------------------------------------------
bool customAlignmentDraw(const AeccDisplayOrientation &viewMode, IAeccAlignment *pAlign,
						 IAcadBlock *pAnonymousBlock )
{
	AcDbBlockTableRecord* blkRec;
	CString strName;
	CComBSTR bstrName;
	pAnonymousBlock->get_Name( &bstrName);
	strName = bstrName;
	AcDbBlockTable* blkTbl;

	AcDbDatabase* pDb = acdbCurDwg();
	Acad::ErrorStatus es = pDb->getSymbolTable(blkTbl, AcDb::kForRead);
	if (es != Acad::eOk){
		return false;
	}
	//// open named block
	es = blkTbl->getAt(strName, blkRec, AcDb::kForWrite);
	acutPrintf(_T("\nget block"));
	if (es != Acad::eOk) {
		blkTbl->close();    // doesn't need to be open anymore
		return false;
	}

	blkTbl->close();    // doesn't need to be open anymore

	CComQIPtr <IAeccAlignmentEntities> pAlignEntities;
	CComQIPtr <IAeccAlignmentEntity> pAlignEnt;
	CComQIPtr<IAeccAlignmentArc> pAlignArc;
	long iCount;
	pAlign->get_Entities(&pAlignEntities);

	if (pAlignEntities != NULL) {
		pAlignEntities->get_Count(&iCount);

		for (long i = 0; i < iCount; i++) {
			pAlignEnt.Release();
			pAlignEntities->Item(_variant_t(i), &pAlignEnt);
			pAlignArc.Release();
			pAlignArc = pAlignEnt;
			if (pAlignArc != NULL) {
				double radius;
				pAlignArc->get_Radius(&radius);
				//acutPrintf(_T("\nRadius: %f", radius));

				if (radius < 200) {
					double x;
					double y;
					double dStart;
					double dEnd;
					pAlignArc->get_StartingStation(&dStart);
					pAlignArc->get_EndingStation(&dEnd);
					for (double delta = dStart; delta < dEnd; delta += 3.0) {
						pAlign->PointLocation(delta, 0.0, &x, &y);
						AcGePoint3d pt(x,y,0);
						AcDbCircle* cir = new AcDbCircle(pt, AcGeVector3d::kZAxis, 5.0);
						cir->setColorIndex (1, Adesk::kTrue);
						blkRec->appendAcDbEntity(cir);
						cir->close();
					}
				}
			}
		}
	}
	blkRec->close();
	return (true) ;
}
//---------------------------------------------------------------------------------------
//
//  ���ܣ� ������ƻص�����
//
//  ���������
//             AeccDisplayOrientation &viewMode
//              IAeccSurface* surface
//             IAcadBlock *pAnonymousBlock 
//
//  ����ֵ��bool
//
// ��  �ߣ�  
//
//  ���ڣ� 2007/08
//
//  �޸ļ�¼����
//
//---------------------------------------------------------------------------------------
bool customSurfaceDraw( const AeccDisplayOrientation &viewMode,  IAeccSurface* surface,
					   IAcadBlock* pAnonymousBlock )
{
	HRESULT hr = S_OK;

	CComQIPtr<IAeccTinSurface> pSurfaceTin( const_cast<IAeccSurface*>(surface) );
	if ( pSurfaceTin ) {

		VARIANT varTriangles;
		hr = pSurfaceTin->get_OutputTriangles( &varTriangles );
		if ( SUCCEEDED(hr) ) {

			long nDoubles = 0;
			double *pdblArray = 0;
			variantToDoublesArray( varTriangles, &nDoubles, &pdblArray );
			if ( pdblArray ) {
				// �ı����
				const double dblWidthAndHeight = 10.0;

				// ÿ�����㹹��һ��������
				for ( int i = 0, nIndexTriangle = 0; i < nDoubles; i += 9, nIndexTriangle++ ) {

					//���������ε����ߵ�
					double dblCenterPoint[3];
					dblCenterPoint[0] = (pdblArray[i  ] + pdblArray[i+3] + pdblArray[i+6]) / 3;
					dblCenterPoint[1] = (pdblArray[i+1] + pdblArray[i+4] + pdblArray[i+7]) / 3;
					dblCenterPoint[2] = (pdblArray[i+2] + pdblArray[i+5] + pdblArray[i+8]) / 3;
					VARIANT varCenterPoint;
					doublesToVariant( 3, dblCenterPoint, &varCenterPoint );

					// �ı�ת��
					CString strText;
					strText.Format( _T("%.0f"), dblCenterPoint[2] );
					CComBSTR bstrText( strText );

					// ���ı���ӵ�������
					IAcadMText *pIAcadMText = 0;
					hr = pAnonymousBlock->AddMText( varCenterPoint, dblWidthAndHeight, bstrText, &pIAcadMText );
					if ( SUCCEEDED(hr) ) {
						pIAcadMText->put_Height( dblWidthAndHeight );
						pIAcadMText->put_AttachmentPoint( acAttachmentPointMiddleCenter );
					}
					SafeArrayDestroy( *varCenterPoint.pparray );
				}
				free ( pdblArray );
			}
			SafeArrayDestroy( *varTriangles.pparray );
		}
	}
	return (true) ;
}


bool 
variantIsArrayOfDoubles( const VARIANT &var )
{
	long varType = var.vt;
	return ( (varType == (VT_ARRAY | VT_R8)) ||
		(varType == (VT_ARRAY | VT_R8 | VT_BYREF)) );
}

void 
variantToDoublesArray( const VARIANT &var, long *pnDoubles, double **ppdblArray )
{
	if ( variantIsArrayOfDoubles( var ) ) {

		// Get the safearray from VARIANT
		SAFEARRAY* pSA = NULL;

		if (var.vt & VT_BYREF)
			//SafeArrayCopy(*(pVar.pparray), &pSA);
			pSA = *(var.pparray);
		else
			//SafeArrayCopy(pVar.parray, &pSA);
			pSA = var.parray;

		// Check dimension of the array
		if (pSA->cDims == 1) {

			// Number of elements in the array
			int cnt = pSA->rgsabound->cElements;
			if (cnt > 0) {

				*pnDoubles = cnt;

				// Resize the out array
				*ppdblArray = (double*)realloc(*ppdblArray, cnt * sizeof(double));
				if (*ppdblArray != NULL) {

					// Transfer the elements
					for (long i=0; i < cnt; i++) {
						SafeArrayGetElement( pSA, &i, (void **)((*ppdblArray)+i) );
					}
				}
			}
		}
	}
}

void 
doublesToVariant( const int nDoubles, const double *pdblArray, VARIANT *pVal )
{
	pVal->vt = VT_ARRAY | VT_R8;
	SAFEARRAYBOUND rgsaBound;
	rgsaBound.lLbound = 0L;
	rgsaBound.cElements = nDoubles;
	pVal->parray = SafeArrayCreate(VT_R8, 1, &rgsaBound);
	if (pVal->parray ) {
		for (long i = 0; i < nDoubles; i++) {
			SafeArrayPutElement( pVal->parray, &i, (void*)&pdblArray[i] );
		}
	}
}

void ComErrorInfo(HRESULT hres)
{
	if(FAILED(hres))      
	{
		HRESULT hresult;
		HRESULT hr;
		GUID guidErrSrc;
		CComBSTR ErrDesc,ErrSrc;
		CComPtr<IErrorInfo>	pIErrorInfo;
		hr = hres;			// backup orig HRESULT
		hresult = GetErrorInfo(0,&pIErrorInfo);
		if (hresult == S_OK) 
		{
			USES_CONVERSION;
			pIErrorInfo->GetGUID(&guidErrSrc);
			pIErrorInfo->GetSource(&ErrSrc);
			pIErrorInfo->GetDescription(&ErrDesc);

			acutPrintf(OLE2W(ErrSrc));
		}
	}
}

//Global variables
//����һ��AeccCustomDraw��ľ�̬ʵ������
TCHAR *CCDraw::pszAppName = _T("C3DCustomDrawApp");
AeccCustomDraw CCDraw::s_customDraw ( pszAppName );



//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCDraw)


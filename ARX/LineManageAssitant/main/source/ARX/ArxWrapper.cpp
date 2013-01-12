#include <ArxWrapper.h>
#include <GlobalDataConfig.h>
#include <LineCategoryItemData.h>
#include <LineConfigDataManager.h>

#include "acedads.h"
#include "accmd.h"
#include <adscodes.h>

#include <adsdlg.h>

#include <dbapserv.h>

#include <dbregion.h>

#include <gepnt3d.h>

//symbol table
#include <dbsymtb.h>

//3D Object
#include <dbsol3d.h>

// Custom object
#include <ArxCustomObject.h>

using namespace com::guch::assistant::arx;
using namespace com::guch::assistant::data;
using namespace com::guch::assistant::config;

/**
 * �����������ģ�Ϳռ��е�ĳһ����
 **/
AcDbObjectId ArxWrapper::PostToModelSpace(AcDbEntity* pEnt,const wstring& layerName )
{
	if( !pEnt )
		return 0;

	//�򿪿�����ݿ�
	AcDbBlockTable *pBlockTable;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlockTable, AcDb::kForRead);

	//�õ�ģ�Ϳռ�
	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
	AcDb::kForWrite);

	//����ʵ��
	AcDbObjectId entId;
	pBlockTableRecord->appendAcDbEntity(entId, pEnt);
	pEnt->setLayer(layerName.c_str());

	//�ر�ʵ��
	pBlockTable->close();
	pBlockTableRecord->close();
	pEnt->close();

	return entId;
}

/**
 * �����ض����ƵĲ�
 **/
void ArxWrapper::createNewLayer(const wstring& layerName)
{
	//�򿪲�����ݿ�
	AcDbLayerTable *pLayerTable;
    acdbHostApplicationServices()->workingDatabase()
        ->getSymbolTable(pLayerTable, AcDb::kForWrite);

	//������ڣ��򴴽�
	if( pLayerTable )
	{
		AcDbLayerTableRecord *pLayerTableRecord = NULL;

		if(pLayerTable->has(layerName.c_str()))
		{
			//�����µĲ���¼
			AcDbLayerTableRecord *pLayerTableRecord =
				new AcDbLayerTableRecord;
			pLayerTableRecord->setName(layerName.c_str());

			// Defaults are used for other properties of 
			// the layer if they are not otherwise specified.
			pLayerTable->add(pLayerTableRecord);
		}

		pLayerTable->close();

		if( pLayerTableRecord )
			pLayerTableRecord->close();
	}
}

/**
 * ������ʼ�㴴���߶Σ����������ض��Ĳ���
 **/
AcDbObjectId ArxWrapper::createLine( const AcGePoint3d& start,
							const AcGePoint3d& end,
							const wstring& layerName )
{
    AcDbLine *pLine = new AcDbLine(start, end);
    return ArxWrapper::PostToModelSpace(pLine,layerName);
}

/**
 * ������ʼ����У������б������������ض��Ĳ���
 **/
void ArxWrapper::createLine( const Point3dVector& points3d,
							const wstring& layerName )
{
	if( points3d.size() < 2 )
		return;

	AcGePoint3d *pStart = NULL;

	for( Point3dIter iter = points3d.begin();
		iter != points3d.end();
		iter++)
	{
		if( pStart == NULL )
		{
			pStart = *iter;
			continue;
		}
		else
		{
			createLine( *pStart, *(*iter), layerName );
			pStart = *iter;
		}
	}
}

/**
 * ���ݹ���ʵ����ʼ����У������б������������ض��Ĳ���
 **/
void ArxWrapper::createLine( const PointList& points,
							const wstring& layerName )
{
	if( points.size() < 2 )
		return;

	AcGePoint3d *pStart = NULL;

	for( ContstPointIter iter = points.begin();
		iter != points.end();
		iter++)
	{
		if( pStart == NULL )
		{
			pStart = new AcGePoint3d((*iter)->m_Point[X],
										(*iter)->m_Point[Y],
										(*iter)->m_Point[Z]);
			continue;
		}
		else
		{
			AcGePoint3d *pEnd = new AcGePoint3d((*iter)->m_Point[X],
										(*iter)->m_Point[Y],
										(*iter)->m_Point[Z]);

			createLine( *pStart, *pEnd, layerName );

			delete pStart;

			pStart = pEnd;
		}
	}

	if( pStart != NULL )
		delete pStart;
}

/**
 * ������ʼ����У������б������������ض��Ĳ���
 **/
AcDbEntity* ArxWrapper::MoveToBottom(AcDbEntity* pEntry)
{
	if( !pEntry )
		return NULL;

	AcGeVector3d vec(-8,10,0);

	AcGeMatrix3d moveMatrix;
	moveMatrix.setToTranslation(vec);

	pEntry->transformBy(moveMatrix);

	return pEntry;
}

/**
 * ������ʼ����У������б������������ض��Ĳ���
 **/
AcDb3dSolid* ArxWrapper::DrawCylinder(const UINT& lineID,
										const UINT& sequenceID,
										const AcGePoint3d& start,
										const AcGePoint3d& end,
										const wstring& layerName,
										const double& radius )
{
#ifdef DEBUG
	acutPrintf(L"����Բ����ʵ�������뵽ͼ��ռ�\n");
#endif

	LMALineDbObject* lmaLineObj = new LMALineDbObject(Adesk::Int16(lineID),
															Adesk::Int16(sequenceID),
															start,end,radius,NULL);
	PostToModelSpace(lmaLineObj,layerName);

	return lmaLineObj;
}

/**
 * ���ݵ����߶����ã��������߶�3D����
 **/
void ArxWrapper::createLMALine( const LineEntry& lineEntry )
{
#ifdef DEBUG
	acutPrintf(L"��ͼ�㡾%s�����ƹ���ʵ�壬�ϵ������%d��\n",lineEntry.m_LineName.c_str(),lineEntry.m_PointList->size());
#endif

	try
	{
		//�õ�ʵ�������
		LineCategoryItemData* lineCategory = LineConfigDataManager::Instance()->FindByKind(lineEntry.m_LineKind);
		if( !lineCategory )
		{
			acutPrintf(L"���ߡ�%s����š�%s�����͡�%s��û���������ݣ����ܴ���3Dģ�͡�\n",lineEntry.m_LineName.c_str(),lineEntry.m_LineNO.c_str(),lineEntry.m_LineKind.c_str());
			return;
		}
		
		//���ȴ���ͼ��
		createNewLayer(lineEntry.m_LineName);

		//�����Բ��
		if( lineCategory->mShape == GlobalData::LINE_SHAPE_CIRCLE )
		{
			//�õ��뾶
			const wstring& rRadius = lineCategory->mRadius;

			double radius = _wtol(rRadius.c_str());

			//Ȼ�󴴽�����
#ifdef DEBUG
			acutPrintf(L"����Բ���壬�뾶Ϊ��%lf��\n",radius);
#endif
			DrawPolyCylinder(lineEntry,radius);
		}
		else if ( lineCategory->mShape == GlobalData::LINE_SHAPE_SQUARE )
		{
			//�õ�����

			//����������
		}
	}
	catch(...)
	{
	}
}

/**
 * �������߶�3D����
 **/
void ArxWrapper::DrawPolyCylinder( const LineEntry& lineEntry, const double& radius)
{
	const PointList& points = *(lineEntry.m_PointList);
	const wstring& layerName = lineEntry.m_LineName;

	if( points.size() < 2 )
		return;

	AcGePoint3d *pStart = NULL;

	for( ContstPointIter iter = points.begin();
		iter != points.end();
		iter++)
	{
		if( pStart == NULL )
		{
			//���߶εĵ�һ�����
			pStart = new AcGePoint3d((*iter)->m_Point[X],
										(*iter)->m_Point[Y],
										(*iter)->m_Point[Z]);
			continue;
		}
		else
		{
			AcGePoint3d *pEnd = new AcGePoint3d((*iter)->m_Point[X],
										(*iter)->m_Point[Y],
										(*iter)->m_Point[Z]);

			//����3D�������ֱ��
			DrawCylinder( lineEntry.m_LineID, (*iter)->m_PointNO, *pStart, *pEnd, layerName,radius );

			delete pStart;

			//������һ���߶�
			pStart = pEnd;
		}
	}

	if( pStart != NULL )
		delete pStart;
}

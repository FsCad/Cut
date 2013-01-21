#include <ArxWrapper.h>
#include <GlobalDataConfig.h>
#include <LineCategoryItemData.h>
#include <LineConfigDataManager.h>
#include <LineEntryData.h>

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

// Hatch
#include <dbhatch.h>

// Custom object
#include <ArxCustomObject.h>

#include <LMAUtils.h>

void TestHatch();
void TestText();

using namespace com::guch::assistant::arx;
using namespace com::guch::assistant::data;
using namespace com::guch::assistant::config;

/**
 * ����������������ʵ���
 **/
AcDbObjectId ArxWrapper::PostToNameObjectsDict(AcDbObject* pObj,const wstring& key, bool toDelete )
{
	AcDbObjectId id;

	try
	{
		AcDbDictionary *pNamedobj;
		acdbHostApplicationServices()->workingDatabase()->
			getNamedObjectsDictionary(pNamedobj, AcDb::kForWrite);

		// Check to see if the dictionary we want to create is
		// already present. If not, create it and add
		// it to the named object dictionary.
		//
		AcDbDictionary *pDict;
		if (pNamedobj->getAt(key.c_str(), (AcDbObject*&) pDict,
			AcDb::kForWrite) == Acad::eKeyNotFound)
		{
			pDict = new AcDbDictionary;
			AcDbObjectId DictId;
			pNamedobj->setAt(key.c_str(), pDict, DictId);
		}
		pNamedobj->close();

		if (pDict) 
		{
			// New objects to add to the new dictionary, then close them.
			LineEntry* pLineEntry = LineEntry::cast(pObj);

			if( pLineEntry )
			{
				if( toDelete )
				{
					acutPrintf(L"\n�������ʵ�ɾ�����ߡ�%s��",pLineEntry->m_LineName.c_str());
					
					//���ȹرն���Ϊ�������Կ�д��ʽ�򿪣�����ɾ��
					pLineEntry->close();

					//�Կ�д��ʽ��
					AcDbObject* pObjToDel = NULL;
					Acad::ErrorStatus es = pDict->getAt(pLineEntry->m_LineName.c_str(),pObjToDel,AcDb::kForWrite);

					if( es == Acad::eOk )
					{
						//���������erased flag�����ã������ڱ����ʱ��ᱻ���˵�
						//��ֵ��Ƕ����������ñ�־λ�󣬲�û��֪ͨ���ݿ⣨�ļ������£�Ҳ��û�б���
						Acad::ErrorStatus es = pObjToDel->erase();
						pObjToDel->close();

						if (es != Acad::eOk)
						{
							acutPrintf(L"\nɾ��ʧ�ܣ�");
							rxErrorMsg(es);
						}

						//�������ֵ���ɾ���ؼ���
						pDict->remove(pLineEntry->m_LineName.c_str());
					}
					else
					{
						acutPrintf(L"\n�򿪱�ɾ���Ķ���ʧ���ˣ�");
						rxErrorMsg(es);
					}
				}
				else
				{
					acutPrintf(L"\n��ӹ��ߡ�%s���������ʵ�",pLineEntry->m_LineName.c_str());
					Acad::ErrorStatus es = pDict->setAt(pLineEntry->m_LineName.c_str(), pObj, id);

					if (es != Acad::eOk)
					{
						acutPrintf(L"\n���ʧ�ܣ�");
						rxErrorMsg(es);
					}

					pObj->close();
				}
			}

			pDict->close();
		}
	}
	catch(const Acad::ErrorStatus es)
	{
		acutPrintf(L"\n�����ʵ䷢���쳣��");
		rxErrorMsg(es);
	}

	return id;
}

/**

/**
 * ��������ô������ʵ��ж�����
 **/
void ArxWrapper::PullFromNameObjectsDict()
{
	AcDbDictionaryPointer pNamedobj;
	// use a smart pointer to access the objects, the destructor will close them automatically
	pNamedobj.open(acdbHostApplicationServices()->workingDatabase()->namedObjectsDictionaryId(), AcDb::kForRead);
	// if ok
	if (pNamedobj.openStatus() == Acad::eOk)
	{
		AcDbObjectId dictId;
		// get at the dictionary entry itself
		Acad::ErrorStatus es = pNamedobj->getAt(LineEntry::LINE_ENTRY_LAYER.c_str(), dictId);
		// if ok
		if (es == Acad::eOk)
		{
			// now open it for read
			AcDbDictionaryPointer pDict(dictId, AcDb::kForRead);
			// if ok
			if (pDict.openStatus() == Acad::eOk)
			{
				// Get an iterator for the ASDK_DICT dictionary.
				AcDbDictionaryIterator* pDictIter= pDict->newIterator();

				LineEntry *pLineEntry = NULL;
				for (; !pDictIter->done(); pDictIter->next()) 
				{
					// Get the current record, open it for read, and
					es = pDictIter->getObject((AcDbObject*&)pLineEntry, AcDb::kForRead);
					// if ok
					if (es == Acad::eOk)
					{
						pLineEntry->close();
					}
				}
				delete pDictIter;
			}

			// no need to close the dicts as we used smart pointers
		}
	}
}

/**
 * �����������ģ�Ϳռ��е�ĳһ����
 **/
AcDbObjectId ArxWrapper::PostToModelSpace(AcDbEntity* pEnt,const wstring& layerName )
{
	AcDbObjectId entId;

#ifdef DEBUG
	acutPrintf(L"\n����ʵ�塾%p����ͼ�㡾%s��",pEnt,layerName.c_str());
#endif 

	try
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
		pBlockTableRecord->appendAcDbEntity(entId, pEnt);
		pEnt->setLayer(layerName.c_str());

		//�ر�ʵ��
		pBlockTable->close();
		pBlockTableRecord->close();
		pEnt->close();
	}
	catch(const Acad::ErrorStatus es)
	{
		acutPrintf(L"\n�������ݿⷢ���쳣��");
		rxErrorMsg(es);
	}

	return entId;
}


/**
 * �������ģ�Ϳռ��е�ĳһ����ɾ��
 **/
Acad::ErrorStatus ArxWrapper::RemoveFromModelSpace(AcDbEntity* pEnt,const wstring& layerName )
{
	AcDbObjectId entId;

	try
	{
		if( !pEnt )
			return Acad::eOk;

		//�򿪿�����ݿ�
		AcDbBlockTable *pBlockTable;
		acdbHostApplicationServices()->workingDatabase()
			->getBlockTable(pBlockTable, AcDb::kForWrite);

		//�õ�ģ�Ϳռ�
		AcDbBlockTableRecord *pBlockTableRecord;
		pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
		AcDb::kForWrite);

		//�������ݿ�
		AcDbBlockTableRecordIterator* iter;
		Acad::ErrorStatus es = pBlockTableRecord->newIterator(iter);
        if (es != Acad::eOk) 
		{
			acutPrintf(L"\n�����ݿ�ʧ����");
            rxErrorMsg(es);
            pBlockTableRecord->close();
        }
        else 
		{
            AcDbEntity* ent;
            for (; !iter->done(); iter->step()) 
			{
                if (iter->getEntity(ent, AcDb::kForWrite) == Acad::eOk) 
				{
					if( ent == pEnt )
					{
#ifdef DEBUG
						acutPrintf(L"\n�����ҵ��ˣ�ɾ�����رյ�");
#endif						
						ent->erase();
						ent->close();
						break;
					}
                }
            }

			//�ѵ�����ɾ�ˣ����ڴ�й¶
            delete iter;
        }

		//�ر�ʵ��
		pBlockTable->close();
		pBlockTableRecord->close();

		return Acad::eOk;
	}
	catch(const Acad::ErrorStatus es)
	{
		acutPrintf(L"\n�������ݿⷢ���쳣��");
		rxErrorMsg(es);

		return Acad::eOutOfMemory;
	}
}

/**
 * �����ض����ƵĲ�
 **/
bool ArxWrapper::createNewLayer(const wstring& layerName)
{
	bool result = false;

	//�򿪲�����ݿ�
	AcDbLayerTable *pLayerTable;
    acdbHostApplicationServices()->workingDatabase()
        ->getSymbolTable(pLayerTable, AcDb::kForWrite);

	if( pLayerTable )
	{
		AcDbLayerTableRecord *pLayerTableRecord = NULL;

		//������ڣ��򴴽�
		if(!pLayerTable->has(layerName.c_str()))
		{
			acutPrintf(L"\n����ͼ�㡾%s��",layerName.c_str());

			//�����µĲ���¼
			AcDbLayerTableRecord *pLayerTableRecord =
				new AcDbLayerTableRecord;
			pLayerTableRecord->setName(layerName.c_str());

			// Defaults are used for other properties of 
			// the layer if they are not otherwise specified.
			pLayerTable->add(pLayerTableRecord);

			result = true;
		}
		else
		{
			acutPrintf(L"\nͼ�㡾%s���Ѵ���",layerName.c_str());
		}

		pLayerTable->close();

		if( pLayerTableRecord )
			pLayerTableRecord->close();
	}

	return result;
}

/**
 * ɾ���ض�ͼ��
 **/
bool ArxWrapper::DeleteLayer(const wstring& layerName)
{
	bool result = false;

	//�򿪲�����ݿ�
	AcDbLayerTable *pLayerTable;
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()
        ->getSymbolTable(pLayerTable, AcDb::kForWrite);

	if( pLayerTable )
	{
		AcDbLayerTableRecord *pLayerTableRecord = NULL;

		//������ڣ��򴴽�
		if(pLayerTable->has(layerName.c_str()))
		{
			acutPrintf(L"\nɾ��ͼ�㡾%s��",layerName.c_str());

			//�����µĲ���¼
			AcDbLayerTableRecord *pLayerRecord;

			// Defaults are used for other properties of 
			// the layer if they are not otherwise specified.
			Acad::ErrorStatus es = pLayerTable->getAt(layerName.c_str(),pLayerRecord,AcDb::kForWrite);

			if( pLayerRecord )
			{
				pLayerRecord->erase();
				pLayerRecord->close();

				result = true;
			}
			else
			{
				acutPrintf(L"\n��ͼ��ʧ��");
				rxErrorMsg(es);
			}
		}
		else
		{
			acutPrintf(L"\nͼ�㡾%s�������ڣ��޷�ɾ��",layerName.c_str());
		}

		pLayerTable->close();

		if( pLayerTableRecord )
			pLayerTableRecord->close();
	}
	else
	{
		acutPrintf(L"\n�򿪲���¼ʧ�ܣ�");
		rxErrorMsg(es);
	}

	return result;
}

/**
 * ��������ͼ��
 **/
bool ArxWrapper::ShowLayer(const wstring& layerName)
{
	bool result = true;

	//�򿪲�����ݿ�
	AcDbLayerTable *pLayerTable;
    acdbHostApplicationServices()->workingDatabase()
        ->getSymbolTable(pLayerTable, AcDb::kForWrite);

	if( pLayerTable )
	{
		AcDbLayerTableIterator *pLayerIter = NULL;
		Acad::ErrorStatus es = pLayerTable->newIterator(pLayerIter);

		if( es == Acad::eOk )
		{
			for( ;!pLayerIter->done();pLayerIter->step() )
			{
				AcDbLayerTableRecord* pLayerTableRecord = NULL;

				pLayerIter->getRecord(pLayerTableRecord,AcDb::kForWrite);

				if( pLayerTableRecord )
				{
					const TCHAR *pLayerName; 
					pLayerTableRecord->getName(pLayerName);
#ifdef DEBUG
					acutPrintf(L"\n�ҵ��㡾%s��",pLayerName);

					if( wstring(pLayerName) != wstring(L"0") )
					{
#endif
						if( layerName.length() == 0
							|| wstring(pLayerName) == layerName 
						)
						{
							pLayerTableRecord->setIsOff(false);
#ifdef DEBUG
							acutPrintf(L"����ʾ�ò�",pLayerName);
#endif					
						}
						else
						{
							pLayerTableRecord->setIsOff(true);
#ifdef DEBUG
							acutPrintf(L"�����ظò�",pLayerName);
#endif					
						}
					}
					else
					{
						acutPrintf(L"\n0��ͼ�㲻����");
					}

					pLayerTableRecord->close();
				}
			}

			delete pLayerIter;
		}
		else
		{
			acutPrintf(L"\n��ͼ���ʧ��");
			rxErrorMsg(es);

			result = false;
		}

		pLayerTable->close();
	}

	return result;
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
	acutPrintf(L"\n����Բ����ʵ�������뵽ͼ��ռ�\n");
#endif

	LMALineDbObject* lmaLineObj = new LMALineDbObject(Adesk::Int32(lineID),
															Adesk::Int32(sequenceID),
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
	acutPrintf(L"\n��ͼ�㡾%s�����ƹ���ʵ�壬�˵������%d��\n",lineEntry.m_LineName.c_str(),lineEntry.m_PointList->size());
#endif

	try
	{
		//�õ�ʵ�������
		LineCategoryItemData* lineCategory = LineConfigDataManager::Instance()->FindByKind(lineEntry.m_LineKind);
		if( !lineCategory )
		{
			acutPrintf(L"\n���ߡ�%s����š�%s�����͡�%s��û���������ݣ����ܴ���3Dģ�͡�\n",lineEntry.m_LineName.c_str(),lineEntry.m_LineNO.c_str(),lineEntry.m_LineKind.c_str());
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
			acutPrintf(L"\n����Բ���壬�뾶Ϊ��%lf��\n",radius);
#endif
			DrawPolyCylinder(lineEntry,radius);
		}
		else if ( lineCategory->mShape == GlobalData::LINE_SHAPE_SQUARE )
		{
			//�õ�����

			//����������
		}
	}
	catch(const Acad::ErrorStatus es)
	{
		acutPrintf(L"\n�����߶η����쳣��");
		rxErrorMsg(es);
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
			AcDb3dSolid* pNewLine = DrawCylinder( lineEntry.m_LineID, (*iter)->m_PointNO, *pStart, *pEnd, layerName,radius );

			//����ʵ����ObjectID
			//(*iter)->m_EntryId = pNewLine->objectId();
			(*iter)->m_pEntry = pNewLine;

			//ɾ����ʱ����
			delete pStart;

			//������һ���߶�
			pStart = pEnd;
		}
	}

	if( pStart != NULL )
		delete pStart;
}

/**
 * ���ݶ��߶ε����ã�ɾ��3D����
 **/
void ArxWrapper::eraseLMALine(const LineEntry& lineEntry, bool old)
{
	PointList* pPointList = old ? lineEntry.m_PrePointList : lineEntry.m_PointList;

	if( pPointList == NULL )
	{
#ifdef DEBUG
		acutPrintf(L"\n����û�С�%s�����߶�",(old ? L"��Ч" : L"��ǰ"));
#endif
		return;
	}

	const PointList& points = old ? *(lineEntry.m_PrePointList) : *(lineEntry.m_PointList);
	const wstring& layerName = lineEntry.m_LineName;

#ifdef DEBUG
	acutPrintf(L"\nɾ�����ߡ�%s�����С�%s�����߶ι���%d����",lineEntry.m_LineName.c_str(),(old ? L"��Ч" : L"��ǰ"),points.size());
#endif

	if( points.size() < 2 )
		return;

	AcGePoint3d *pStart = NULL;

	for( ContstPointIter iter = points.begin();
		iter != points.end();
		iter++)
	{
		if( iter == points.begin() )
		{
			continue;
		}
		else
		{
			//�õ��߶ε����ݿ����ID
			//AcDbObjectId objId = (*iter)->m_EntryId;
			if( (*iter)->m_pEntry )
			{
#ifdef DEBUG
				acutPrintf(L"\n�߶��յ� ��š�%d�� ���� x:��%lf��y:��%lf��z:��%lf����ɾ��",(*iter)->m_PointNO,(*iter)->m_Point[X],(*iter)->m_Point[Y],(*iter)->m_Point[Z]);
#endif
				RemoveFromModelSpace((*iter)->m_pEntry,lineEntry.m_LineName);
			}
		}
	}
}


/**
 * �л���ǰ��ͼ����WCS��viewDirection��һ�µ���ͼ
 **/
void ArxWrapper::ChangeView(int viewDirection)
{
	AcDbViewTableRecord view;

    // get desired view direction
    AcGeVector3d viewDir = -AcGeVector3d::kYAxis;
	/*
    if (prViewDir.isKeyWordPicked(_T("Top")))
        viewDir = AcGeVector3d::kZAxis;
    else if (prViewDir.isKeyWordPicked(_T("BOttom")))
        viewDir = -AcGeVector3d::kZAxis;
    else if (prViewDir.isKeyWordPicked(_T("BAck")))
        viewDir = AcGeVector3d::kYAxis;
    else if (prViewDir.isKeyWordPicked(_T("Front")))
        viewDir = -AcGeVector3d::kYAxis;
    else if (prViewDir.isKeyWordPicked(_T("Right")))
        viewDir = AcGeVector3d::kXAxis;
    else if (prViewDir.isKeyWordPicked(_T("Left")))
        viewDir = -AcGeVector3d::kXAxis;
    else {
        ASSERT(0);
    }
	*/

#ifdef DEBUG
	acutPrintf(L"\n���õ�ǰ��ͼΪǰ��,���ĵ���50");
#endif

	wstring cmdViewDir;

	if( viewDirection == 1 )
		cmdViewDir = L"RIGHT";
	else if ( viewDirection == 2 )
		cmdViewDir =  L"FRONT";
	else if  ( viewDirection == 3 )
		cmdViewDir =  L"TOP";

#ifdef DEBUG
	acutPrintf(L"\n���õ�ǰ��ͼΪǰ��Ϊ��%s��",cmdViewDir.c_str());
#endif

	acedCommand(RTSTR, _T("._-VIEW"), RTSTR, cmdViewDir.c_str(), 0);

    //view.setViewDirection(viewDir);
	//view.setCenterPoint(AcGePoint2d(50, 50));

	// ������ͼ�����ĵ�
	//view.setCenterPoint(AcGePoint2d((xMin + xMax) / 2,  (yMin + yMax) / 2));

	// ������ͼ�ĸ߶ȺͿ��
	//view.setHeight(fabs(yMax - yMin));
	//view.setWidth(fabs(xMax - xMin));

	// ����ͼ��������Ϊ��ǰ��ͼ
	//Acad::ErrorStatus es = acedSetCurrentView(&view, NULL);
}

AcDbObjectId ArxWrapper::CreateHatch(AcDbObjectIdArray objIds,const wstring& patName, bool bAssociative, const wstring& layerName, const AcGeVector3d& normal, const double& elevation)
{
	Acad::ErrorStatus es;
	AcDbHatch *pHatch = new AcDbHatch();
	
	// �������ƽ��
	pHatch->setNormal(normal);
	pHatch->setElevation(elevation);
	
	// ���ù�����
	pHatch->setAssociative(bAssociative);
	
	// �������ͼ��
#ifdef DEBUG
	acutPrintf(L"\n�������ͼ��Ϊ��%s��",patName.c_str());
#endif

	pHatch->setPattern(AcDbHatch::kPreDefined, patName.c_str());
	
	// ������߽�
	es = pHatch->appendLoop(AcDbHatch::kExternal, objIds);
	
	// ��ʾ������
	es = pHatch->evaluateHatch();
	
	// ��ӵ�ģ�Ϳռ�
	AcDbObjectId hatchId;
	hatchId = PostToModelSpace(pHatch,layerName.c_str());
	
	// ����ǹ����Ե���䣬����������߽�󶨣��Ա�ʹ���ܻ�ñ߽�����޸ĵ�֪ͨ
	if (bAssociative)
	{
		AcDbEntity *pEnt;
		for (int i = 0; i < objIds.length(); i++)
		{
			es = acdbOpenAcDbEntity(pEnt, objIds[i], AcDb::kForWrite);
			if (es == Acad::eOk)
			{
				// ���һ�����÷�Ӧ��
				pEnt->addPersistentReactor(hatchId);
				pEnt->close();
			}
		}
	}
	
	return hatchId;
}

AcDbObjectId ArxWrapper::CreateHatch(AcDbEntity* entity,const wstring& patName, bool bAssociative, const wstring& layerName, const AcGePlane& plane, const double& distance  )
{
	if( entity == NULL )
		return 0;

	AcDbObjectId objId = entity->objectId();
			
	AcGeVector3d normal = plane.normal();
	//double distance = plane.distanceTo(AcGePoint3d::kOrigin);

#ifdef DEBUG
		acutPrintf(L"\n��ƽ�浽ԭ��ľ����ǡ�%lf��",distance);
#endif

	AcDbObjectIdArray objIds;
	objIds.append(objId);

	return ArxWrapper::CreateHatch(objIds,patName,bAssociative,layerName,normal,distance);
}

void ArxWrapper::TestFunction()
{
	//TestHatch();

	TestText();
}

void TestHatch()
{
	// ��ʾ�û�ѡ�����߽�
	ads_name ss;
	int rt = acedSSGet(NULL, NULL, NULL, NULL, ss);

	AcDbObjectIdArray objIds;
	// ��ʼ�����߽��ID����
	if (rt == RTNORM)
	{
		long length;
		acedSSLength(ss, &length);
		for (int i = 0; i < length; i++)
		{
			ads_name ent;
			acedSSName(ss, i, ent);
			AcDbObjectId objId;
			acdbGetObjectId(objId, ent);
			objIds.append(objId);
		}
	}

	acedSSFree(ss); // �ͷ�ѡ��

	ArxWrapper::CreateHatch(objIds, L"JIS_LC_20", true, L"0", AcGeVector3d(0,0,1), 0);
}

static AcDbObjectId CreateText(const AcGePoint3d& ptInsert,
		const wchar_t* text, AcDbObjectId style = AcDbObjectId::kNull,
			double height = 2.5, double rotation = 0);

// ʵ�ֲ���
AcDbObjectId CreateText(const AcGePoint3d& ptInsert,
						const wchar_t* text, AcDbObjectId style,
						double height, double rotation)
{
	AcDbText *pText = new AcDbText(ptInsert, text, style, height, rotation);
	return ArxWrapper::PostToModelSpace(pText,L"0");
}

void TestText()
{
	// Hide the dialog and give control to the editor
    ads_point pt;

    // Get a point
    if (acedGetPoint(NULL, _T("\nѡȡ�����: "), pt) == RTNORM) {
		// ������������
		AcGePoint3d ptInsert(pt[X], pt[Y], pt[Z]);
		CreateText(ptInsert, L"��������");
	}
}

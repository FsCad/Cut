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

// Leader
#include <dblead.h>
#include <dbmleader.h>

// Custom object
#include <ArxCustomObject.h>

#include <LMAException.h>

#include <LMAUtils.h>

void TestHatch();
void TestText();
void TestLeader();
void TestMLeader();

using namespace com::guch::assistant::arx;
using namespace com::guch::assistant::data;
using namespace com::guch::assistant::config;
using namespace com::guch::assistant::exception;

const double ArxWrapper::kPi       = 3.14159265358979323846;
const double ArxWrapper::kHalfPi   = 3.14159265358979323846 / 2.0;
const double ArxWrapper::kTwoPi	  = 3.14159265358979323846 * 2.0;
const double ArxWrapper::kRad45    = 3.14159265358979323846 / 4.0;
const double ArxWrapper::kRad90    = 3.14159265358979323846 / 2.0;
const double ArxWrapper::kRad135   = (3.14159265358979323846 * 3.0) / 4.0;
const double ArxWrapper::kRad180   = 3.14159265358979323846;
const double ArxWrapper::kRad270   = 3.14159265358979323846 * 1.5;
const double ArxWrapper::kRad360   = 3.14159265358979323846 * 2.0;

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

					//pObj->close();
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
	Acad::ErrorStatus es;

	if( !pEnt )
		return 0;

#ifdef DEBUG
	acutPrintf(L"\n����ʵ�塾%p����ͼ�㡾%s��",pEnt,layerName.c_str());
#endif 

	AcDbBlockTable *pBlockTable(NULL);
	AcDbBlockTableRecord *pBlockTableRecord(NULL);

	try
	{
		//�򿪿�����ݿ�
		es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForWrite);
		if( es != Acad::eOk )
			throw ErrorException(L"�򿪿���¼ʧ��");

		//�õ�ģ�Ϳռ�
		es = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
		if( es != Acad::eOk )
			throw ErrorException(L"��ģ��ռ��¼ʧ��");

		//����ʵ��
		es = pBlockTableRecord->appendAcDbEntity(entId, pEnt);
		if( es != Acad::eOk )
			throw ErrorException(L"���ʵ���¼ʧ��");

		es = pEnt->setLayer(layerName.c_str());
		if( es != Acad::eOk )
			throw ErrorException(L"����ʵ�����ڵ�ͼ��ʧ��");

		//�ر�ʵ��
		//pEnt->close();
	}
	catch(const Acad::ErrorStatus es)
	{
		acutPrintf(L"\n�������ݿⷢ���쳣��");
		rxErrorMsg(es);
	}
	catch( const ErrorException& e)
	{
		acutPrintf(L"\n�����µ�ʵ���¼ʧ�ܡ�%s��",e.errMsg.c_str());
		rxErrorMsg(es);
	}

	if( pBlockTableRecord != NULL )
		pBlockTableRecord->close();

	if( pBlockTable != NULL )
		pBlockTable->close();

	return entId;
}


/**
 * �������ģ�Ϳռ��е�ĳһ����ɾ��
 **/
Acad::ErrorStatus ArxWrapper::RemoveFromModelSpace(const AcDbHandle& handle,const wstring& layerName )
{
	AcDbObjectId entId;

	try
	{
		if( handle.isNull() )
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
					AcDbHandle entHandle;
					ent->getAcDbHandle(entHandle);

					if( !entHandle.isNull() )
					{
						if( entHandle == handle )
						{
#ifdef DEBUG
							acutPrintf(L"\n�����ҵ��ˣ�ɾ�����رյ�");
#endif						
							ent->erase();
							ent->close();
							break;
						}
					}
					else
					{
						acutPrintf(L"\n����Handleɾ������ʱ����");
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
	Acad::ErrorStatus es;
		
	AcDbLayerTable *pLayerTable;
	AcDbLayerTableRecord *pLayerTableRecord = NULL;

	try
	{
		//�򿪲�����ݿ�
		es = acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pLayerTable, AcDb::kForWrite);

		if( es != Acad::eOk )
			throw ErrorException(L"�򿪲���¼ʧ��");

		//������ڣ��򴴽�
		if( pLayerTable->has(layerName.c_str()) )
		{
			acutPrintf(L"\nͼ�㡾%s���Ѵ���,���贴��",layerName.c_str());
			pLayerTable->close();

			return true;
		}

#ifdef DEBUG
		acutPrintf(L"\n����ͼ�㡾%s��",layerName.c_str());
#endif

		//�����µĲ���¼
		AcDbLayerTableRecord *pLayerTableRecord = new AcDbLayerTableRecord;
		es = pLayerTableRecord->setName(layerName.c_str());

		if( es != Acad::eOk )
			throw ErrorException(L"�����²���¼������ʧ��");

		// Defaults are used for other properties of 
		// the layer if they are not otherwise specified.
		es = pLayerTable->add(pLayerTableRecord);
		if( es != Acad::eOk )
			throw ErrorException(L"�����µĲ���¼ʧ��");

		result = true; 
	}
	catch( const ErrorException& e)
	{
		acutPrintf(L"\n�����µĲ���¼ʧ�ܡ�%s��",e.errMsg.c_str());
		rxErrorMsg(es);
	}

	if( pLayerTable != NULL )
		pLayerTable->close();

	if( pLayerTableRecord != NULL )
		pLayerTableRecord->close();

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
										LineEntry& entry )
{
#ifdef DEBUG
	acutPrintf(L"\n��������ʵ�������뵽ͼ��ռ�\n");
#endif

	LMALineDbObject* lmaLineObj = new LMALineDbObject(Adesk::Int32(lineID),
															Adesk::Int32(sequenceID),
															start,end,&entry);
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
		//���ȴ���ͼ��
		createNewLayer(lineEntry.m_LineName);

		//����3Dģ��
		DrawPolyCylinder(lineEntry);
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
void ArxWrapper::DrawPolyCylinder( const LineEntry& lineEntry )
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
			AcDb3dSolid* pNewLine = DrawCylinder( lineEntry.m_LineID, (*iter)->m_PointNO, *pStart, *pEnd, layerName, const_cast<LineEntry&>(lineEntry) );

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

				LMALineDbObject* pLineObject = dynamic_cast<LMALineDbObject*>((*iter)->m_pEntry);

				if( pLineObject )
				{
					RemoveFromModelSpace(pLineObject->mHandleDim,lineEntry.m_LineName);
					
					RemoveFromModelSpace(pLineObject->mHandleText,lineEntry.m_LineName);

					RemoveFromModelSpace(pLineObject,lineEntry.m_LineName);
				}
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

AcDbObjectId ArxWrapper::CreateMLeader(const AcGePoint3d& start, const int& offset, const int& direction,
											const wstring& content, const wstring& layerName)
{
	static int leaderOffset = 6;

	//��ע�����
	AcGePoint3d startPoint(start);

		//������Ӧ��ת��
	{
		if( direction == 1 )
		{
#ifdef DEBUG
			acutPrintf(L"\n������X�ᴹֱ,��Z��λ��ת��ΪY����Y��λ��ת��ΪX");
#endif
			startPoint.y = start.z;
			startPoint.x = start.y;
		}
		else if ( direction == 2 )
		{
#ifdef DEBUG
			acutPrintf(L"\n������Y�ᴹֱ����Z��λ��ת��ΪY��λ��");
#endif		
			startPoint.y = start.z;
			startPoint.z = 0;
		} 
	}

	//�۵�Ϊ����6����λ����λ�������·�
	AcGePoint3d endPoint(start.x - leaderOffset, start.y - leaderOffset, start.z);

    AcDbMLeader* leader = new AcDbMLeader;

	//���ñ�ע������
	{
		int index = 0;
		leader->addLeaderLine(startPoint,index);

		leader->addLastVertex(index,endPoint);
		leader->setLastVertex(index,endPoint);

		leader->setContentType(AcDbMLeaderStyle::kMTextContent);

		//���ñ�ע������
		AcDbMText* mtext = new AcDbMText;
		mtext->setContents(content.c_str());

		mtext->setTextHeight(mtext->textHeight()/2);

		leader->setMText(mtext);
	}

	//������Ӧ��ת��
	{
		if( direction == 1 )
		{
#ifdef DEBUG
			acutPrintf(L"\n������X�ᴹֱ,����X�ᷭת��XZƽ�棬Ȼ����Z�ᷭת��ZYƽ�棬�����X��ƽ��");
#endif
			//���з�ת��XZƽ��
			AcGeMatrix3d rotateMatrix = AcGeMatrix3d::rotation( ArxWrapper::kRad90, AcGeVector3d::kYAxis, AcGePoint3d::kOrigin);
			leader->transformBy(rotateMatrix);

			//���з�ת��YZƽ��
			rotateMatrix = AcGeMatrix3d::rotation( ArxWrapper::kRad90, AcGeVector3d::kXAxis, AcGePoint3d::kOrigin);
			leader->transformBy(rotateMatrix);

			//����ƫ��
			AcGeMatrix3d moveMatrix;
			moveMatrix.setToTranslation(AcGeVector3d(offset,0,0));

			leader->transformBy(moveMatrix);
		}
		else if ( direction == 2 )
		{
#ifdef DEBUG
			acutPrintf(L"\n������Y�ᴹֱ,����X�ᷭת��XZƽ�棬Ȼ����Y��ƽ��");
#endif		
			//���з�ת
			AcGeMatrix3d rotateMatrix = AcGeMatrix3d::rotation( ArxWrapper::kRad90, AcGeVector3d::kXAxis, AcGePoint3d::kOrigin);
			leader->transformBy(rotateMatrix);

			//����ƫ��
			AcGeMatrix3d moveMatrix;
			moveMatrix.setToTranslation(AcGeVector3d(0,offset,0));

			leader->transformBy(moveMatrix);
		} 
		else if ( direction == 3 )
		{
#ifdef DEBUG
			acutPrintf(L"\n������Z�ᴹֱ����˱�ע����ƫ�Ƽ���");
#endif		
			//����ƫ��
			AcGeMatrix3d moveMatrix;
			moveMatrix.setToTranslation(AcGeVector3d(0,0,offset));

			leader->transformBy(moveMatrix);
		}
	}

	//��ӵ�ģ�Ϳռ���
	ArxWrapper::PostToModelSpace(leader,layerName);

	return leader->objectId();
}

void ArxWrapper::TestFunction()
{
	//TestHatch();

	//TestText();

	//TestLeader();

	TestMLeader();
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

void TestMLeader()
{
	AcGePoint3dArray vertices;
	AcGePoint3d startPoint,endPoint;

	{
		ads_point pt;

		if (acedGetPoint(NULL, _T("\nѡȡMLeader��ʼ��: "), pt) != RTNORM) 
			return;

		startPoint.x = pt[X];
		startPoint.y = pt[Y];
		startPoint.z = pt[Z];

		//if (acedGetPoint(NULL, _T("\nѡȡMLeader������: "), pt) != RTNORM) 
		//	return;

		endPoint.x = pt[X] - 9;
		endPoint.y = pt[Y] - 9;
		endPoint.z = pt[Z];
	}

    AcDbMLeader* leader = new AcDbMLeader;

	int index = 0;
	leader->addLeaderLine(startPoint,index);

	leader->addLastVertex(index,endPoint);
	leader->setLastVertex(index,endPoint);

	leader->setContentType(AcDbMLeaderStyle::kMTextContent);

	AcDbMText* mtext = new AcDbMText;
	mtext->setContents(L"������1#���뾶10��");

	leader->setMText(mtext);
	ArxWrapper::PostToModelSpace(leader,L"0");
}

void TestLeader()
{
    AcGePoint3dArray vertices;
	AcGePoint3d startPoint;

	{
		ads_point pt;

		if (acedGetPoint(NULL, _T("\nѡȡ�����: "), pt) != RTNORM) 
			return;

		startPoint.x = pt[X];
		startPoint.y = pt[Y];
		startPoint.z = pt[Z];
	}
		
	AcGePoint3d endPoint(startPoint.x + 1,startPoint.y-2,0);
	AcGePoint3d lastPoint(startPoint.x + 5,startPoint.y-2,0);

	vertices.append(startPoint);
	vertices.append(endPoint);
	vertices.append(lastPoint);

    AcDbLeader* leader = new AcDbLeader;

    for (int i=0;i<3;i++)
        leader->appendVertex(vertices[i]);

    leader->setToSplineLeader();    // set to spline just for fun
    leader->setDatabaseDefaults();    // pick up default dimstyle, etc.

	AcDbMText* mtext = new AcDbMText;
    //mtext->setLocation(prInsertPt.value());
    //mtext->setWidth(fabs(prInsertPt.value().x - prCorner.value().x));

	mtext->setContents(L"1#�����ߣ��뾶10");
	mtext->setDirection(AcGeVector3d(1,0,0));
	mtext->setLocation(endPoint);
	mtext->setHeight(0.5);

	ArxWrapper::PostToModelSpace(mtext,L"0");

	leader->setAnnotationObjId(mtext->objectId());
	ArxWrapper::PostToModelSpace(leader,L"0");
}

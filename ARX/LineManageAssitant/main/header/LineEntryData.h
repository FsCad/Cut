// ------------------------------------------------
//                  LineManagementAssistant
// Copyright 2012-2013, Chengyong Yang & Changhai Gu. 
//               All rights reserved.
// ------------------------------------------------
//	LineEntryData.h
//	written by Changhai Gu
// ------------------------------------------------
// $File:\\LineManageAssitant\main\header\LineEntryData.h $
// $Author: Changhai Gu $
// $DateTime: 2013/1/2 01:35:46 $
// $Revision: #1 $
// ------------------------------------------------
#pragma once

#include "stdafx.h"

#include <string>
#include <vector>

#include <dbsymtb.h>
#include <dbapserv.h>
#include <adslib.h>
#include <adui.h>
#include <acui.h>

using namespace std;

namespace com
{

namespace guch
{

namespace assistant
{

namespace data
{

/**
 * ��������ʵ��
 */
struct PointEntry
{
	//���
	UINT m_PointNO;
	ads_point m_Point; 

	wstring m_LevelKind;
	wstring m_Direction;

	PointEntry();
	PointEntry( const UINT& pointNO, const ads_point& point, const wstring& levelKind, const wstring& direction);
	PointEntry( const PointEntry& );
	PointEntry( const wstring& data );

	wstring toString() const;
};

typedef PointEntry *pPointEntry;

typedef vector<pPointEntry> PointList;
typedef PointList::iterator PointIter;
typedef PointList::const_iterator ContstPointIter;

/**
 * ����ʵ��
 */
class LineEntry : public AcDbObject
{
public:

	ACRX_DECLARE_MEMBERS(LineEntry);

	LineEntry();
	LineEntry( const wstring& rLineNO,
				const wstring& rLineName,
				const wstring& rLineKind);

	LineEntry( const wstring& data);

	~LineEntry();

	void SetName( const wstring& rNewName ) { m_LineName = rNewName; }

	int InsertPoint( const PointEntry& newPoint );
	void UpdatePoint( const PointEntry& updatePoint );
	void DeletePoint( const UINT& PointNO );

	void SetPoints( PointList* newPoints);

	PointIter FindPoint( const UINT& PointNO ) const;
	ContstPointIter FindConstPoint( const UINT& PointNO ) const;

	wstring toString();


	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler*);
    virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*)
        const;

    virtual Acad::ErrorStatus dxfInFields (AcDbDxfFiler*);
    virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler*)
        const;

protected:
	void ClearPoints();

public:

	UINT m_LineID;

	wstring m_LineNO;
	wstring m_LineName;
	wstring m_LineKind;

	UINT m_CurrentPointNO;

	PointList* m_PointList;
};

typedef vector<LineEntry*> LineList;
typedef LineList::iterator LineIterator;
typedef LineList::const_iterator ConstLineIterator;

/**
 * ����ʵ���ļ�
 */
class LineEntryFile
{
public:
	LineEntryFile(const wstring& fileName);
	~LineEntryFile();

	void InsertLine( LineEntry* lineEntry);
	BOOL UpdateLine( LineEntry* lineEntry);
	BOOL DeleteLine( const UINT& lineID );

	LineIterator FindLinePos( const UINT& lineID ) const;
	LineIterator FindLinePosByNO( const wstring& lineNO ) const;
	LineIterator FindLinePosByName( const wstring& lineName ) const;

	LineEntry* FindLine( const UINT& lineID ) const;
	LineEntry* FindLineByNO( const wstring& lineNO  ) const;
	LineEntry* FindLineByName( const wstring& lineName  ) const;

	LineEntry* HasAnotherLineByNO( const UINT& lineID, const wstring& lineNO  ) const;
	LineEntry* HasAnotherLineByByName( const UINT& lineID, const wstring& lineName  ) const;

	void Init();
	void Persistent() const;

	LineList* GetList() const {return m_LineList;}

	wstring m_FileName;

private:

	LineList* m_LineList;
};

/**
 * ���ļ�����ʵ��������
 */

typedef vector<LineEntryFile*> EntryFileList;
typedef EntryFileList::iterator EntryFileIter;

class LineEntryFileManager
{
public:

	static LineEntryFile* GetLineEntryFile( const wstring& fileName );

	static LineEntryFile* RegisterLine(const wstring& fileName);

	static bool RegisterLineSegment( const wstring& fileName, UINT lineID, UINT sequence, 
										const AcGePoint3d start, const AcGePoint3d& end );

private:
	static EntryFileList* pEntryFileList;
};

} // end of data

} // end of assistant

} // end of guch

} // end of com

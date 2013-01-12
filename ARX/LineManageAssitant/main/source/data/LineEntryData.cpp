// ------------------------------------------------
//                  LineManagementAssistant
// Copyright 2012-2013, Chengyong Yang & Changhai Gu. 
//               All rights reserved.
// ------------------------------------------------
//	LineEntryData.cpp
//	written by Changhai Gu
// ------------------------------------------------
// $File:\\LineManageAssitant\main\source\data\LineEntryData.h $
// $Author: Changhai Gu $
// $DateTime: 2013/1/2 01:35:46 $
// $Revision: #1 $
// ------------------------------------------------

#include <LineEntryData.h>
#include <LMAUtils.h>

#include <ArxWrapper.h>
#include <acdocman.h>

namespace com
{

namespace guch
{

namespace assistant
{

namespace data
{

#define POINT_START L"PS"
#define POINT_END L"PE"
#define POINTS_SEP L"&&"

///////////////////////////////////////////////////////////////////////////
// Implementation PointEntry

/**
 * ��������ʵ��
 */

PointEntry::PointEntry()
:m_PointNO(0),
m_LevelKind(L""),
m_Direction(L"")	
{
	m_Point[X] = 0;
	m_Point[Y] = 0;
	m_Point[Z] = 0;
}

PointEntry::PointEntry( const UINT& pointNO, const ads_point& point, const wstring& levelKind, const wstring& direction)
:m_PointNO(pointNO),
m_LevelKind(levelKind),
m_Direction(direction)	
{
	m_Point[X] = point[X];
	m_Point[Y] = point[Y];
	m_Point[Z] = point[Z];
}

PointEntry::PointEntry( const PointEntry& pointEntry)
{
	this->m_PointNO = pointEntry.m_PointNO;
	this->m_LevelKind = pointEntry.m_LevelKind;
	this->m_Direction = pointEntry.m_Direction;

	this->m_Point[X] = pointEntry.m_Point[X];
	this->m_Point[Y] = pointEntry.m_Point[Y];
	this->m_Point[Z] = pointEntry.m_Point[Z];
}

PointEntry::PointEntry( const wstring& data )
{
	double temp;

	const static size_t start = wcslen(POINT_START);
	size_t end = data.find_first_of(POINT_END);

	int index = 0;

	wstrVector* dataColumn = vectorContructor(data,POINTS_SEP,start,end);
	wstring& column = (*dataColumn)[index++];

	acdbDisToF(column.c_str(), -1, &temp);
	this->m_PointNO = (UINT)temp;

	column = (*dataColumn)[index++];
	acdbDisToF(column.c_str(), -1, &m_Point[X]);

	column = (*dataColumn)[index++];
	acdbDisToF(column.c_str(), -1, &m_Point[Y]);

	column = (*dataColumn)[index++];
	acdbDisToF(column.c_str(), -1, &m_Point[Z]);

	m_LevelKind = (*dataColumn)[index++];

	m_Direction = (*dataColumn)[index++];

	delete dataColumn;
}

wstring PointEntry::toString() const
{
	CString temp;
	temp.Format(L"%s%d%s%0.2f%s%0.2f%s%0.2f%s%s%s%s%s%s",
				POINT_START,
				m_PointNO, POINTS_SEP,
				m_Point[X], POINTS_SEP,
				m_Point[Y], POINTS_SEP,
				m_Point[Z], POINTS_SEP,
				m_LevelKind,POINTS_SEP,
				m_Direction,POINTS_SEP,
				POINT_END);

	return temp.GetBuffer();
}

///////////////////////////////////////////////////////////////////////////
// Implementation LineEntry

/**
 * ����ʵ��
 */

LineEntry::LineEntry()
	:m_LineID(0),
	m_LineNO(L"0"),
	m_LineName(L""),
	m_LineKind(L""),
	m_CurrentPointNO(0)
{
	m_PointList = new PointList();
}

LineEntry::LineEntry(const wstring& rLineNO,
				const wstring& rLineName,
				const wstring& rLineKind)
	:m_LineID(0),
	m_LineNO(rLineNO),
	m_LineName(rLineName),
	m_LineKind(rLineKind),
	m_CurrentPointNO(0)
{
	m_PointList = new PointList();
}

LineEntry::LineEntry( const wstring& data)
{
	m_PointList = new PointList();

	double temp;

	int index = 0;

	wstrVector* dataColumn = vectorContructor(data,L"\t");

	//�õ��ߵ��������
	wstring& column = (*dataColumn)[index++];
	acdbDisToF(column.c_str(), -1, &temp);
	this->m_LineID = (UINT)temp;

	m_LineNO = (*dataColumn)[index++];
	m_LineName = (*dataColumn)[index++];
	m_LineKind = (*dataColumn)[index++];

	column = (*dataColumn)[index++];
	acdbDisToF(column.c_str(), -1, &temp);
	m_CurrentPointNO = (UINT)temp;

	//�õ�ÿ���������
	int size = (int)dataColumn->size();

	while( index < size )
	{
		column = (*dataColumn)[index++];

		m_PointList->push_back(new PointEntry(column));
	}

	delete dataColumn;
}

LineEntry::~LineEntry()
{
	ClearPoints();
}

void LineEntry::ClearPoints()
{
	if( m_PointList )
	{
		for( PointIter iter = m_PointList->begin();
				iter != m_PointList->end();
				iter++ )
		{
			if(*iter)
				delete *iter;
		}

		delete m_PointList;
		m_PointList = NULL;
	}
}

PointIter LineEntry::FindPoint( const UINT& PointNO ) const
{
	for( PointIter iter = this->m_PointList->begin();
			iter != this->m_PointList->end();
			iter++)
	{
		if( (*iter)->m_PointNO == PointNO )
			return iter;
	}

	return m_PointList->end();
}

ContstPointIter LineEntry::FindConstPoint( const UINT& PointNO ) const
{
	for( ContstPointIter iter = this->m_PointList->begin();
			iter != this->m_PointList->end();
			iter++)
	{
		if( (*iter)->m_PointNO == PointNO )
			return iter;
	}

	return m_PointList->end();
}

int LineEntry::InsertPoint( const PointEntry& newPoint )
{
	pPointEntry point = new PointEntry(newPoint);

	point->m_PointNO = m_CurrentPointNO;

	m_PointList->push_back(point);

	m_CurrentPointNO++;

	return (int)m_PointList->size();
}

void LineEntry::UpdatePoint( const PointEntry& updatePoint )
{
	PointIter findPoint = this->FindPoint(updatePoint.m_PointNO);

	if( findPoint != this->m_PointList->end() )
	{
		delete *findPoint;
		*findPoint = new PointEntry(updatePoint);
	}
}

void LineEntry::DeletePoint( const UINT& PointNO )
{
	PointIter findPoint = this->FindPoint(PointNO);

	if( findPoint != this->m_PointList->end() )
	{
		m_PointList->erase(findPoint);
	}
}


void LineEntry::SetPoints( PointList* newPoints)
{
	ClearPoints();

	m_PointList = newPoints;
}

wstring LineEntry::toString()
{
	wstring lineData;

	CString temp;
	temp.Format(L"%d\t%s\t%s\t%s\t%d",m_LineID,m_LineNO,m_LineName,m_LineKind,m_CurrentPointNO);

#ifdef DEBUG
	acutPrintf(L"ʵ�����ݡ�%s��\n",temp.GetBuffer());
#endif

	lineData = temp;

	for( ContstPointIter iter = this->m_PointList->begin();
			iter != this->m_PointList->end();
			iter++)
	{
		lineData += L"\t";
		lineData += (*iter)->toString();
	}

	//ÿ���лس�
	lineData += L"\n";

	return lineData;
}


// Files data in from a DWG file.
//
Acad::ErrorStatus
LineEntry::dwgInFields(AcDbDwgFiler* pFiler)
{
    assertWriteEnabled();

    AcDbObject::dwgInFields(pFiler);
    // For wblock filing we wrote out our owner as a hard
    // pointer ID so now we need to read it in to keep things
    // in sync.
    //
    if (pFiler->filerType() == AcDb::kWblockCloneFiler) {
        AcDbHardPointerId id;
        pFiler->readItem(&id);
    }

	int lineID;
    pFiler->readItem(&lineID);
	m_LineID = (UINT)lineID;

	ACHAR* buffer = new ACHAR[255];

	memset(buffer,0,sizeof(ACHAR)*255);
	pFiler->readItem(&buffer);
	m_LineNO = wstring(buffer);

	memset(buffer,0,sizeof(ACHAR)*255);
	pFiler->readItem(&buffer);
	m_LineName = wstring(buffer);

	memset(buffer,0,sizeof(ACHAR)*255);
	pFiler->readItem(&buffer);
	m_LineKind = wstring(buffer);

	CString filename;
	dbToStr(this->database(),filename);

#ifdef DEBUG
	acutPrintf(L"�����ݿ��ļ���%s����������ʵ�� ID��%d����š�%s�����ơ�%s�����͡�%s��",
				filename.GetBuffer(),m_LineID,m_LineNO,m_LineName,m_LineKind);
#endif

	wstring fileName(filename.GetBuffer());
	LineEntryFile* entryFile = LineEntryFileManager::RegisterLine(fileName);

	//������ļ��м������line
	entryFile->InsertLine(this);

    return pFiler->filerStatus();
}

// Files data out to a DWG file.
//
Acad::ErrorStatus
LineEntry::dwgOutFields(AcDbDwgFiler* pFiler) const
{
    assertReadEnabled();

    AcDbObject::dwgOutFields(pFiler);
    // Since objects of this class will be in the Named
    // Objects Dictionary tree and may be hard referenced
    // by some other object, to support wblock we need to
    // file out our owner as a hard pointer ID so that it
    // will be added to the list of objects to be wblocked.
    //
    if (pFiler->filerType() == AcDb::kWblockCloneFiler)
        pFiler->writeHardPointerId((AcDbHardPointerId)ownerId());

#ifdef DEBUG
	acutPrintf(L"�������ʵ�嵽���ݿ� ID��%d����š�%s�����ơ�%s�����͡�%s��",
				m_LineID,m_LineNO,m_LineName,m_LineKind);
#endif

    pFiler->writeItem(int(m_LineID));

	pFiler->writeItem(m_LineNO.c_str());
	pFiler->writeItem(m_LineName.c_str());
	pFiler->writeItem(m_LineKind.c_str());

    return pFiler->filerStatus();
}

// Files data in from a DXF file.
//
Acad::ErrorStatus
LineEntry::dxfInFields(AcDbDxfFiler* pFiler)
{
    assertWriteEnabled();

    Acad::ErrorStatus es;
    if ((es = AcDbObject::dxfInFields(pFiler))
        != Acad::eOk)
    {
        return es;
    }

    // Check if we're at the right subclass getLineID marker.
    //
    if (!pFiler->atSubclassData(_T("LineEntryData"))) {
        return Acad::eBadDxfSequence;
    }

    struct resbuf inbuf;
    while (es == Acad::eOk) {
        if ((es = pFiler->readItem(&inbuf)) == Acad::eOk) {

			switch ( inbuf.restype )
			{
				case AcDb::kDxfInt16:
					m_LineID = inbuf.resval.rint;
				//case AcDb::kDxfInt16 + 1:
					//mSequenceNO = inbuf.resval.rint;
			}
        }
    }

    return pFiler->filerStatus();
}

// Files data out to a DXF file.
//
Acad::ErrorStatus
LineEntry::dxfOutFields(AcDbDxfFiler* pFiler) const
{
    assertReadEnabled();

    AcDbObject::dxfOutFields(pFiler);
    pFiler->writeItem(AcDb::kDxfSubclass, _T("LineEntryData"));
    //pFiler->writeItem(AcDb::kDxfInt16, mLineID);
	//pFiler->writeItem(AcDb::kDxfInt16 + 1, mSequenceNO);

    return pFiler->filerStatus();
}


///////////////////////////////////////////////////////////////////////////
// Implementation LineEntryFile

/**
 * ����ʵ���ļ�
 */
LineEntryFile::LineEntryFile(const wstring& fileName)
	:m_FileName(fileName)
{
	m_LineList = new LineList();
	Init();
}

LineEntryFile::~LineEntryFile()
{
	if( m_LineList )
	{
		for( LineIterator iter = m_LineList->begin();
				iter != m_LineList->end();
				iter++ )
		{
			if(*iter)
				delete *iter;
		}

		delete m_LineList;
	}
}

void LineEntryFile::Init()
{
	CFile archiveFile;

	//read data from file PERSISTENT_FILE
	BOOL result = archiveFile.Open(this->m_FileName.c_str(),CFile::modeRead);
	if( !result )
	{
		acutPrintf(L"�򿪹���ʵ�������ļ�ʧ��");
		return;
	}

	//�õ��ļ����ݳ���
	int length = (int)archiveFile.GetLength()+1;

	//�õ��ļ���խ�ַ�����
	char* content = new char[length];
	memset(content,0,length);
	archiveFile.Read(content,length);

	//����ת��Ϊ���ַ�
	string strCnt(content,length);
	wstring wContent = StringToWString( strCnt );

	//���һس��Ծ�����
	size_t lineFrom = 0;
	size_t linePos = wContent.find_first_of(L"\n",lineFrom);

	while( linePos != wstring::npos )
	{
		//�õ�һ������
		wstring& wLine = wContent.substr(lineFrom, linePos-lineFrom);

#ifdef DEBUG
		acutPrintf(L"�õ�һ�й���ʵ�����ݡ�%s��\n",wLine.c_str());
#endif

		if(wLine.length() == 0)
			break;

		LineEntry *newLine = new LineEntry(wLine);
		m_LineList->push_back( newLine );

		//������Ӧ��ͼ��
		ArxWrapper::createNewLayer( newLine->m_LineName );

		//������Ӧ������
		ArxWrapper::createLMALine(*newLine );

		//����һ���ַ���ʼ��������һ��
		lineFrom = linePos + 1;
		linePos = wContent.find_first_of(L"\n",lineFrom + 1);
	}

	//�ر��ļ�
	archiveFile.Close();
}

void LineEntryFile::Persistent() const
{
	acutPrintf(L"��ʼ�������ʵ������\n");

	CFile archiveFile(this->m_FileName.c_str(),CFile::modeCreate|CFile::modeWrite);

	//�������е����Ͷ���
	for( ConstLineIterator iter = m_LineList->begin(); 
			iter != m_LineList->end(); 
			iter++)
	{
		LineEntry* data = *iter;

		if( data )
		{
			//�õ���Ϣ�Ŀ��ַ����л�
			wstring wData = data->toString();

			//תΪխ�ַ�
			string dataStr = WstringToString(wData);

#ifdef DEBUG
			acutPrintf(L"����ʵ������ [%s] [%d]\n",wData.c_str(),(UINT)dataStr.length());
#endif
			//ʹ�� virtual void Write( const void* lpBuf, UINT nCount ); ��խ�ַ�д���ļ�
			archiveFile.Write(dataStr.c_str(),(UINT)dataStr.size());
		}
	}

	acutPrintf(L"����ʵ�����ݱ������\n");
	archiveFile.Close();
}

void LineEntryFile::InsertLine(LineEntry* lineEntry)
{
	if( lineEntry )
		m_LineList->push_back(lineEntry);

	this->Persistent();
}

BOOL LineEntryFile::UpdateLine(LineEntry* lineEntry)
{
	LineIterator iter = FindLinePos(lineEntry->m_LineID);

	if( iter != this->m_LineList->end())
	{
		(*iter)->m_LineName = lineEntry->m_LineName;
		(*iter)->m_LineNO = lineEntry->m_LineNO;

		this->Persistent();
		return TRUE;
	}

	return FALSE;
}

BOOL LineEntryFile::DeleteLine( const UINT& lineID )
{
	LineIterator iter = FindLinePos(lineID);

	if( iter != this->m_LineList->end())
	{
		m_LineList->erase(iter);
		this->Persistent();
		return TRUE;
	}
	else
		return FALSE;
}


LineIterator LineEntryFile::FindLinePos( const UINT& lineID ) const
{
	for( LineIterator iter = this->m_LineList->begin();
			iter != this->m_LineList->end();
			iter++)
	{
		if( (*iter)->m_LineID == lineID )
			return iter;
	}

	return m_LineList->end();
}

LineIterator LineEntryFile::FindLinePosByNO( const wstring& lineNO ) const
{
	for( LineIterator iter = this->m_LineList->begin();
			iter != this->m_LineList->end();
			iter++)
	{
		if( (*iter)->m_LineNO == lineNO )
			return iter;
	}

	return m_LineList->end();
}

LineIterator LineEntryFile::FindLinePosByName( const wstring& lineName ) const
{
	for( LineIterator iter = this->m_LineList->begin();
			iter != this->m_LineList->end();
			iter++)
	{
		if( (*iter)->m_LineName == lineName )
			return iter;
	}

	return m_LineList->end();
}

LineEntry* LineEntryFile::FindLine( const UINT& lineID ) const
{
	LineIterator iter = FindLinePos(lineID);

	if( iter != m_LineList->end())
		return (*iter);
	else
		return NULL;
}

LineEntry* LineEntryFile::FindLineByName( const wstring& lineName ) const
{
	LineIterator iter = FindLinePosByName(lineName);

	if( iter != m_LineList->end())
		return (*iter);
	else
		return NULL;
}

LineEntry* LineEntryFile::FindLineByNO( const wstring& lineNO ) const
{
	LineIterator iter = FindLinePosByNO(lineNO);

	if( iter != m_LineList->end())
		return (*iter);
	else
		return NULL;
}

LineEntry* LineEntryFile::HasAnotherLineByNO( const UINT& lineID, const wstring& lineNO  ) const
{
	for( LineIterator iter = this->m_LineList->begin();
			iter != this->m_LineList->end();
			iter++)
	{
		if( (*iter)->m_LineNO == lineNO && (*iter)->m_LineID != lineID)
			return (*iter);
	}

	return NULL;
}

LineEntry* LineEntryFile::HasAnotherLineByByName( const UINT& lineID, const wstring& lineName  ) const
{
	for( LineIterator iter = this->m_LineList->begin();
			iter != this->m_LineList->end();
			iter++)
	{
		if( (*iter)->m_LineName == lineName && (*iter)->m_LineID != lineID)
			return (*iter);
	}

	return NULL;
}

EntryFileList* LineEntryFileManager::pEntryFileList = NULL;

LineEntryFile* LineEntryFileManager::GetLineEntryFile( const wstring& fileName )
{
	if( pEntryFileList == NULL )
	{
		pEntryFileList = new EntryFileList();
#ifdef DEBUG
		LOG(L"�ļ�ʵ���������δ������");
#endif
		return NULL;
	}

	for( EntryFileIter iter = pEntryFileList->begin();
			iter != pEntryFileList->end();
			iter++)
	{
		if( (*iter)->m_FileName == fileName )
			return (*iter);
	}

#ifdef DEBUG
	acutPrintf(L"û���ҵ��ļ���%s����Ӧ�Ĺ���ʵ�塣",fileName.c_str());
#endif

	return NULL;
}

LineEntryFile* LineEntryFileManager::RegisterLine(const wstring& fileName)
{
	LineEntryFile* entryFile = GetLineEntryFile(fileName);
	if( entryFile == NULL )
	{
		acutPrintf(L"������%s����Ӧ�Ĺ���ʵ�塣",fileName.c_str());

		entryFile = new LineEntryFile(fileName);
	}

	return entryFile;
}

bool LineEntryFileManager::RegisterLineSegment( const wstring& fileName, UINT lineID, UINT sequence, 
										const AcGePoint3d start, const AcGePoint3d& end )
{
	//�ҵ��ļ�������
	LineEntryFile* pFileEntry = GetLineEntryFile(fileName);

	if( pFileEntry == NULL )
	{
#ifdef DEBUG
		acutPrintf(L"�����µĹ���ʵ���ļ���������%s����",fileName.c_str());
#endif
		pFileEntry = new LineEntryFile(fileName);
		pEntryFileList->push_back( pFileEntry );
	}

	//�ҵ�ʵ����
	LineEntry* lineEntry = pFileEntry->FindLine(lineID);

	if( lineEntry == NULL )
	{
		//lineEntry = new LineE
	}

	return true;
}

} // end of data

} // end of assistant

} // end of guch

} // end of com
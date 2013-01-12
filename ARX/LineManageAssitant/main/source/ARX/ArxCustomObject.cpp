// ------------------------------------------------
//                  LineManagementAssistant
// Copyright 2012-2013, Chengyong Yang & Changhai Gu. 
//               All rights reserved.
// ------------------------------------------------
//	ArxWrapper.h
//	written by Changhai Gu
// ------------------------------------------------
// $File:\\LineManageAssitant\main\source\ARX\ArxCustomObject.cpp $
// $Author: Changhai Gu $
// $DateTime: 2013/1/12 06:13:00
// $Revision: #1 $
// ------------------------------------------------

#include <ArxWrapper.h>
#include <ArxCustomObject.h>
#include <LMAUtils.h>

namespace com
{

namespace guch
{

namespace assistant
{

namespace arx
{

void LMADbObjectManager::RegisterClass()
{
	LMALineDbObject::rxInit();
}

void LMADbObjectManager::UnRegisterClass()
{
	deleteAcRxClass(LMALineDbObject::desc());
}

ACRX_DXF_DEFINE_MEMBERS(LMALineDbObject, AcDb3dSolid, 
AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
0,
    LMALineDbObject, LMA);

LMALineDbObject::LMALineDbObject()
	: mLineID(-1)
	,mSequenceNO(-1)
	,mLineEntry(NULL)
	,mStartPoint(AcGePoint3d::kOrigin)
	,mEndPoint(AcGePoint3d::kOrigin)
{
};

Acad::ErrorStatus LMALineDbObject::Init()
{
	if( mLineEntry )
	{
		LOG(L"�����߶�Ϊ�գ�Ѱ��������Ϣ");
	}

	return CreatePipe();
}

// Gets the value of the line ID member.
//
Acad::ErrorStatus
	LMALineDbObject::getLineID(Adesk::Int16& lineId)
{
    // Tells AutoCAD a read operation is taking place.
    //
    assertReadEnabled();
    lineId = mLineID;
    return Acad::eOk;
}

// Sets the value of the line ID member.
//
Acad::ErrorStatus
LMALineDbObject::setLineID(Adesk::Int16 lineId)
{
    // Triggers openedForModify notification.
    //
    assertWriteEnabled();
    mLineID = lineId;
    return Acad::eOk;
}

// Gets the value of the line ID member.
//
Acad::ErrorStatus
	LMALineDbObject::getPointSeqNO(Adesk::Int16& pointSeqNO)
{
    // Tells AutoCAD a read operation is taking place.
    //
    assertReadEnabled();
    pointSeqNO = mSequenceNO;
    return Acad::eOk;
}

// Sets the value of the line ID member.
//
Acad::ErrorStatus
LMALineDbObject::setPointSeqNO(Adesk::Int16 pointSeqNO)
{
    // Triggers openedForModify notification.
    //
    assertWriteEnabled();
    mSequenceNO = pointSeqNO;
    return Acad::eOk;
}

// Files data in from a DWG file.
//
Acad::ErrorStatus
LMALineDbObject::dwgInFields(AcDbDwgFiler* pFiler)
{
    assertWriteEnabled();

    AcDb3dSolid::dwgInFields(pFiler);
    // For wblock filing we wrote out our owner as a hard
    // pointer ID so now we need to read it in to keep things
    // in sync.
    //
    if (pFiler->filerType() == AcDb::kWblockCloneFiler) {
        AcDbHardPointerId id;
        pFiler->readItem(&id);
    }

    pFiler->readItem(&mLineID);
	pFiler->readItem(&mSequenceNO);

	pFiler->readPoint3d(&mStartPoint);
	pFiler->readPoint3d(&mEndPoint);

    return pFiler->filerStatus();
}

// Files data out to a DWG file.
//
Acad::ErrorStatus
LMALineDbObject::dwgOutFields(AcDbDwgFiler* pFiler) const
{
    assertReadEnabled();

    AcDb3dSolid::dwgOutFields(pFiler);
    // Since objects of this class will be in the Named
    // Objects Dictionary tree and may be hard referenced
    // by some other object, to support wblock we need to
    // file out our owner as a hard pointer ID so that it
    // will be added to the list of objects to be wblocked.
    //
    if (pFiler->filerType() == AcDb::kWblockCloneFiler)
        pFiler->writeHardPointerId((AcDbHardPointerId)ownerId());

    pFiler->writeItem(mLineID);
	pFiler->writeItem(mSequenceNO);

	pFiler->writeItem(mStartPoint);
	pFiler->writeItem(mEndPoint);

    return pFiler->filerStatus();
}

// Files data in from a DXF file.
//
Acad::ErrorStatus
LMALineDbObject::dxfInFields(AcDbDxfFiler* pFiler)
{
    assertWriteEnabled();

    Acad::ErrorStatus es;
    if ((es = AcDb3dSolid::dxfInFields(pFiler))
        != Acad::eOk)
    {
        return es;
    }

    // Check if we're at the right subclass getLineID marker.
    //
    if (!pFiler->atSubclassData(_T("LMALineDbObject"))) {
        return Acad::eBadDxfSequence;
    }

    struct resbuf inbuf;
    while (es == Acad::eOk) {
        if ((es = pFiler->readItem(&inbuf)) == Acad::eOk) {

			switch ( inbuf.restype )
			{
				case AcDb::kDxfInt16:
					mLineID = inbuf.resval.rint;
				case AcDb::kDxfInt16 + 1:
					mSequenceNO = inbuf.resval.rint;
			}
        }
    }

    return pFiler->filerStatus();
}

// Files data out to a DXF file.
//
Acad::ErrorStatus
LMALineDbObject::dxfOutFields(AcDbDxfFiler* pFiler) const
{
    assertReadEnabled();

    AcDb3dSolid::dxfOutFields(pFiler);
    pFiler->writeItem(AcDb::kDxfSubclass, _T("LMALineDbObject"));
    pFiler->writeItem(AcDb::kDxfInt16, mLineID);
	pFiler->writeItem(AcDb::kDxfInt16 + 1, mSequenceNO);

    return pFiler->filerStatus();
}

Acad::ErrorStatus LMALineDbObject::CreatePipe()
{
	acutPrintf(L"��ʼ���ƹ���\n");

	//�õ��߶εĳ���
	double length = mStartPoint.distanceTo(mEndPoint);
	if( length < 0.1 )
		return Acad::eInvalidInput;

	acutPrintf(L"�õ�����߶�%lf\n",length);

	//����Բ����
	this->createFrustum(length,mRadius,mRadius,mRadius);

	//�õ��߶���Z��Ĵ�ֱ����
	AcGeVector3d line3dVector(mEndPoint.x - mStartPoint.x,mEndPoint.y - mStartPoint.y, mEndPoint.z-mStartPoint.z);
	AcGeVector3d rotateVctor = line3dVector.crossProduct(AcGeVector3d::kZAxis);

	//�õ���ת�ĽǶ�
	double angle = -line3dVector.angleTo(AcGeVector3d::kZAxis);
	acutPrintf(L"�õ���ת�Ƕ�%lf\n",angle);

	//������ת
	AcGeMatrix3d rotateMatrix = AcGeMatrix3d::rotation( angle, rotateVctor, AcGePoint3d::kOrigin);
	transformBy(rotateMatrix);
	
	//�õ��߶ε����ĵ�
	AcGePoint3d center(mStartPoint.x + mEndPoint.x, mStartPoint.y + mEndPoint.y, mStartPoint.z + mEndPoint.z); 
	center /= 2;
	acutPrintf(L"�õ����ĵ�[%lf][%lf][%lf]\n",center.x,center.y,center.z);

	//����ƫ��
	AcGeMatrix3d moveMatrix;
	moveMatrix.setToTranslation(AcGeVector3d(center.x,center.y,center.z));

	transformBy(moveMatrix);

#ifdef DEBUG
	//acutPrintf(L"���������ߣ����ڽ���");

	//AcDbLine *pLine = new AcDbLine(start, end);
    //ArxWrapper::PostToModelSpace(pLine,mLayerName);
#endif

	return Acad::eOk;
}

} // end of arx

} // end of assistant

} // end of guch

} // end of com

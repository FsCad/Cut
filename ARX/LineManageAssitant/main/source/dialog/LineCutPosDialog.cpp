// LineCutPosDialog.cpp : implementation file
//

#include "stdafx.h"
#include "LineCutPosDialog.h"
#include "afxdialogex.h"

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

#include <ArxWrapper.h>

#include <ArxCustomObject.h>

using namespace com::guch::assistant::arx;

// LineCutPosDialog dialog

CString LineCutPosDialog::m_CutLayerName = L"";

IMPLEMENT_DYNAMIC(LineCutPosDialog, CAcUiDialog)

LineCutPosDialog::LineCutPosDialog(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(LineCutPosDialog::IDD, pParent),
	m_Direction(0),
	m_strOffset(0)
{
}

LineCutPosDialog::~LineCutPosDialog()
{
}

BOOL LineCutPosDialog::OnInitDialog()
{
	//��ҳ�潻������
	CAcUiDialog::OnInitDialog();

	//Ĭ��X��ѡ��
	m_DirectionX.SetCheck(BST_CHECKED);
	m_Direction = 1;

	//Ĭ��ƫ��Ϊ0
	m_EditOffset.SetWindowTextW(L"0.00");

	//����ͼƬ
	m_PickCutPosButton.AutoLoad();

	return TRUE;
}

void LineCutPosDialog::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OFFSET, m_EditOffset);

	DDX_Control(pDX, IDC_X, m_DirectionX);
	DDX_Control(pDX, IDC_Y, m_DirectionY);
	DDX_Control(pDX, IDC_Z, m_DirectionZ);

	DDX_Control(pDX, IDC_BUTTON_PICKCUT,m_PickCutPosButton);
}

void LineCutPosDialog::OnBnClickedOk()
{
	//�õ��û����������
	UpdateData(FALSE);

	//������ͼ
	GenereateCutRegion();

	//��ʾ��ͼ��
	ShowCutRegion();

	//�رնԻ���
	CAcUiDialog::OnOK();
}

void LineCutPosDialog::OnBnClickedX()
{
	m_Direction = 1;
}

void LineCutPosDialog::OnBnClickedY()
{
	m_Direction = 2;
}

void LineCutPosDialog::OnBnClickedZ()
{
	m_Direction = 3;
}

BEGIN_MESSAGE_MAP(LineCutPosDialog, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &LineCutPosDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_X, &LineCutPosDialog::OnBnClickedX)
	ON_BN_CLICKED(IDC_Y, &LineCutPosDialog::OnBnClickedY)
	ON_BN_CLICKED(IDC_Z, &LineCutPosDialog::OnBnClickedZ)
	ON_BN_CLICKED(IDC_BUTTON_PICKCUT, &LineCutPosDialog::OnBnPickCutPos)
END_MESSAGE_MAP()

void Transform(AcDbEntity* pEntry, int xoffset = 0, int yoffset = 0, int zoffset = 0)
{
	AcGeVector3d vec(xoffset,yoffset,zoffset);

	AcGeMatrix3d moveMatrix;
	moveMatrix.setToTranslation(vec);

	pEntry->transformBy(moveMatrix);
}

void LineCutPosDialog::GenerateCutPlane()
{
	//��������
	CString offset;
	m_EditOffset.GetWindowTextW(offset);

	if( offset.GetLength())
		m_strOffset = _wtoi(offset);

	if( m_Direction == 1)
	{
		m_CutLayerName.Format(L"��X�ᴹֱƫ����Ϊ��%d��������",m_strOffset);
		m_CutPlane.set(AcGePoint3d(m_strOffset,0,0),AcGeVector3d(1,0,0));
	}
	else if( m_Direction == 2)
	{
		m_CutLayerName.Format(L"��Y�ᴹֱƫ����Ϊ��%d��������",m_strOffset);
		m_CutPlane.set(AcGePoint3d(0,m_strOffset,0),AcGeVector3d(0,1,0));
	}
	else if( m_Direction == 3)
	{
		m_CutLayerName.Format(L"��Z�ᴹֱƫ����Ϊ��%d��������",m_strOffset);
		m_CutPlane.set(AcGePoint3d(0,0,m_strOffset),AcGeVector3d(0,0,1));
	}

#ifdef DEBUG
	acutPrintf(L"\n����Ϊ��%s��",m_CutLayerName.GetBuffer());
#endif
}

void LineCutPosDialog::GenereateCutRegion()
{
	//���Ȼָ���ͼ
	CutBack();

	//�õ�����
	GenerateCutPlane();

	//�õ���ǰ��ʵ���ļ�������
	LineEntryFile* pLineFile = LineEntryFileManager::GetCurrentLineEntryFile();
	if( pLineFile == NULL )
	{
		acutPrintf(L"\nû���ҵ������ļ�������������°ɣ�");
		return;
	}

	//�õ�ʵ���б�
	LineList* lineList = pLineFile->GetList();
	if( lineList == NULL )
	{
		acutPrintf(L"\n��ǰ�ļ���û�й��ߣ����������ͼ��");
		return;
	}

	//����ʵ���ļ��������ÿһ��ʵ�������ͼ
	LineIterator lineIter = lineList->begin();

	for(;lineIter != lineList->end();
		lineIter++)
	{
#ifdef DEBUG
		acutPrintf(L"\n�Թ��ߡ�%s��������ͼ��",(*lineIter)->m_LineName.c_str());
#endif
		if( *lineIter != NULL )
			GenereateCutRegion(*lineIter);
	}
}

void LineCutPosDialog::GenereateCutRegion(LineEntry* lineEntry)
{
	PointList* pointList = lineEntry->m_PointList;
	if( pointList == NULL )
	{
		acutPrintf(L"\n�ù���û���߶Σ�������ͼ��");
		return;
	}

	//�����е��߶ν��б���
	PointIter pointIter = pointList->begin();
	for(;pointIter != pointList->end();pointIter++)
	{
		if( pointIter == pointList->begin() )
			continue;

		PointEntry* pointEntry = (*pointIter);

		if( pointEntry == NULL )
		{
			acutPrintf(L"\n���߶β��Ϸ�����Ҫע�⣡");
			continue;
		}

#ifdef DEBUG
		acutPrintf(L"\n�Եڡ�%d�����߶ν�����ͼ��",pointEntry->m_PointNO);
#endif

		if( pointEntry->m_pEntry == NULL )
		{
			acutPrintf(L"\n��ǰ�߶�û�ж�Ӧ�����ݿ�ʵ�壬������ͼ��");
			continue;
		}

		LMALineDbObject* pLMALine = LMALineDbObject::cast(pointEntry->m_pEntry);

		if( pLMALine == NULL )
		{
			acutPrintf(L"\n��ǰ�߶β�����Ч�ĸ���ϵͳ�����ʵ�壬��������ͼ��");
			continue;
		}

		//�õ�ʵ�����������еĽ���
		AcDbRegion *pSelectionRegion = NULL;
		pLMALine->getSection(m_CutPlane, pSelectionRegion);

		if( pSelectionRegion )
		{
			//�����������ڵ�ͼ��
			ArxWrapper::createNewLayer(m_CutLayerName.GetBuffer());

			//��������뵽ģ�Ϳռ�
			ArxWrapper::PostToModelSpace(pSelectionRegion,m_CutLayerName.GetBuffer());

			//�����ý�����������
			ArxWrapper::CreateHatch(pSelectionRegion,L"JIS_LC_20", true, m_CutLayerName.GetBuffer(), m_CutPlane, m_strOffset);
		}
	}
}

void LineCutPosDialog::ShowCutRegion()
{
	//ֻ��ʾ����ͼ��
	ArxWrapper::ShowLayer(m_CutLayerName.GetBuffer());

	//�л���ͼ
	ArxWrapper::ChangeView(m_Direction);
}

void LineCutPosDialog::CutBack()
{
	acutPrintf(L"\n���ڵ���ͼΪ��%s����",m_CutLayerName.GetBuffer());

	if( m_CutLayerName.GetLength() > 0 )
	{
		acutPrintf(L"\n�ָ�WCS�Ӵ�");
		acedCommand(RTSTR, _T("_UCS"), RTSTR, L"W", 0);

		acutPrintf(L"\nɾ����ͼ���ڵ�ͼ��");
		if( ArxWrapper::DeleteLayer(m_CutLayerName.GetBuffer()) )
		{
			acutPrintf(L"\n��ʼ������");
			m_CutLayerName.Format(L"");
		}

		acutPrintf(L"\n��ʾ����ͼ��");
		ArxWrapper::ShowLayer(L"");
	}
	else
	{
		acutPrintf(L"\n��ǰϵͳ��û����ͼ��");
	}
}

void LineCutPosDialog::OnBnPickCutPos()
{
	// Hide the dialog and give control to the editor
    BeginEditorCommand();

    ads_point pt;

	CString temp;

    // Get a point
    if (acedGetPoint(NULL, _T("\nѡȡ�и��: "), pt) == RTNORM) {
        // If the point is good, continue
        CompleteEditorCommand();

		if( m_Direction == 1 )
			temp.Format(_T("%g"), pt[X]);
		else if ( m_Direction == 2 )
			temp.Format(_T("%g"), pt[Y]);
		else if ( m_Direction == 3 )
			temp.Format(_T("%g"), pt[Z]);
    } else {
        // otherwise cancel the command (including the dialog)
        CancelEditorCommand();
    }

	m_EditOffset.SetWindowTextW(temp.GetBuffer());

	UpdateData(FALSE);
}

// LineCutPosDialog message handlers

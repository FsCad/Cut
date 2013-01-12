// AddLineDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AddLineDialog.h"
#include "afxdialogex.h"

#include "GlobalDataConfig.h"
#include "LineTypeConfigPropertySheet.h"
#include <LMAException.h>
#include <LMAUtils.h>

using namespace com::guch::assistant::data;
using namespace com::guch::assistant::exception;

// AddBlockConfigDialog dialog
namespace com
{

namespace guch
{

namespace assistant
{

namespace config
{

IMPLEMENT_DYNAMIC(AddBlockConfigDialog, CDialog)

AddBlockConfigDialog::AddBlockConfigDialog(CWnd* pParent /*=NULL*/)
	: AddLineConfigDialog(AddBlockConfigDialog::IDD, pParent)
{
}

void AddBlockConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	AddLineConfigDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK_CAN_THROUGH, m_CanThr);
	DDX_Control(pDX, IDC_COMBO_THROUGH_DIRECTION, m_ThroughDirection);
}

BEGIN_MESSAGE_MAP(AddBlockConfigDialog, CDialog)
	ON_BN_CLICKED(IDOK, &AddBlockConfigDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_CAN_THROUGH, &AddBlockConfigDialog::OnChkCanThroughChange)
	ON_CBN_SELCHANGE(IDC_COMBO_SHAPE, &AddLineConfigDialog::OnCbnShapeChange)
END_MESSAGE_MAP()

BOOL AddBlockConfigDialog::InitConfigData()
{
	//����
	m_LineKind.AddString(GlobalData::BLOCK_CATEGORY_WUSHUI.c_str());
	m_LineKind.AddString(GlobalData::BLOCK_CATEGORY_HANGDAO.c_str());
	m_LineKind.AddString(GlobalData::BLOCK_CATEGORY_NUANQIGUAN.c_str());
	m_LineKind.AddString(GlobalData::BLOCK_CATEGORY_GUODAO.c_str());
	m_LineKind.AddString(GlobalData::BLOCK_CATEGORY_CHEZHAN.c_str());
	m_LineKind.SetCurSel(0);

	//������״
	m_LineShape.InsertString(0,GlobalData::LINE_SHAPE_CIRCLE.c_str());
	m_LineShape.InsertString(1,GlobalData::LINE_SHAPE_SQUARE.c_str());
	m_LineShape.SetCurSel(0);

	//��λ
	m_LineUnit.AddString(GlobalData::LINE_UNIT_MM.c_str());
	m_LineUnit.AddString(GlobalData::LINE_UNIT_CM.c_str());
	m_LineUnit.AddString(GlobalData::LINE_UNIT_M.c_str());
	m_LineUnit.SetCurSel(0);

	//����
	m_LineName.SetWindowTextW(L"");

	//��С
	m_LineRadius.SetWindowTextW(L"0");
	m_LineLength.SetWindowTextW(L"0");
	m_LineWidth.SetWindowTextW(L"0");

	//�ں�
	m_LineWallSize.SetWindowTextW(L"0");

	//��ȫ��Χ
	m_LineSafeSize.SetWindowTextW(L"0");

	//�ɷ�Խ
	m_CanThr.SetCheck( BST_UNCHECKED );

	//��Խ����
	m_ThroughDirection.InsertString(0,GlobalData::BLOCK_CATEGORY_ABOVE.c_str());
	m_ThroughDirection.InsertString(1,GlobalData::BLOCK_CATEGORY_BELLOW.c_str());
	m_ThroughDirection.SetCurSel(-1);

	//����
	m_LineDesc.SetWindowText(L"");

	if( m_OperType == OPER_UPDATE )
	{
		//�������
		FillUpdateData();
	}

	//��̬���ƿؼ�
	ShowControlDynamic();

	return TRUE;
}

void AddBlockConfigDialog::FillUpdateData()
{
	//���Ŀؼ��ı���
	this->SetWindowTextW(L"��������");

	//Get the parent windows
	LineConfigDialog* lineConfig 
		= dynamic_cast<LineConfigDialog*>(this->m_pParentWnd);

	if( !lineConfig )
		return;

	LineCategoryItemData configData;
	
	//�õ�ѡ�������
	lineConfig->GetSelectData(configData);

	//�������
	m_lineIndex = configData.mIndex;
	m_lineID = configData.mID;

	m_LineName.SetWindowTextW(configData.mName.c_str());
	m_LineKind.SelectString(0,configData.mKind.c_str());

	m_LineShape.SelectString(0,configData.mShape.c_str());
	m_LineUnit.SelectString(0,configData.mUnit.c_str());

	m_LineRadius.SetWindowTextW(configData.mRadius.c_str());
	m_LineLength.SetWindowTextW(configData.mLength.c_str());
	m_LineWidth.SetWindowTextW(configData.mWidth.c_str());

	m_LineWallSize.SetWindowTextW(configData.mWallSize.c_str());
	m_LineSafeSize.SetWindowTextW(configData.mSafeSize.c_str());

	if( configData.mCanThrough == L"��" )
	{
		m_CanThr.SetCheck( BST_CHECKED );
		m_ThroughDirection.SelectString(0,configData.mThroughDirection.c_str());
	}
	else if( configData.mCanThrough == L"��" )
	{
		m_CanThr.SetCheck( BST_UNCHECKED );
		m_ThroughDirection.SetCurSel(-1);
	}

	m_LineDesc.SetWindowTextW(configData.mComment.c_str());

	UpdateData(FALSE);
}

AddBlockConfigDialog::~AddBlockConfigDialog()
{
}

void AddBlockConfigDialog::OnBnClickedOk()
{
	//�õ��û����������
	UpdateData(TRUE);

	CString lineKind,lineName,lineShape,lineRadius,lineLength,lineWidth,lineUnit,lineSafeSize,lineWallSize,blockCanThr,blockThrDirection,lineDesc;

	m_LineName.GetWindowTextW(lineName);
	m_LineKind.GetWindowTextW(lineKind);

	m_LineShape.GetWindowTextW(lineShape);
	m_LineUnit.GetWindowTextW(lineUnit);

	m_LineRadius.GetWindowTextW(lineRadius);
	m_LineLength.GetWindowTextW(lineLength);
	m_LineWidth.GetWindowTextW(lineWidth);

	m_LineWallSize.GetWindowTextW(lineWallSize);
	m_LineSafeSize.GetWindowTextW(lineSafeSize);

	if( m_CanThr.GetCheck() == BST_CHECKED )
	{
		blockCanThr = L"��";
	}
	else if(  m_CanThr.GetCheck() == BST_UNCHECKED)
	{
		blockCanThr = L"��";
	}

	m_ThroughDirection.GetWindowTextW(blockThrDirection);

	m_LineDesc.GetWindowTextW(lineDesc);

	//׼���������ݽṹ��
	LineCategoryItemData categoryData;

	categoryData.mIndex = m_lineIndex;
	categoryData.mID = m_lineID;

	categoryData.mCategory = GlobalData::KIND_SEPERATOR;

	categoryData.mName = lineName;
	categoryData.mKind = lineKind;

	categoryData.mUnit = lineUnit;
	categoryData.mShape = lineShape;

	categoryData.mRadius = lineRadius;
	categoryData.mLength = lineLength;
	categoryData.mWidth = lineWidth;

	categoryData.mWallSize = lineWallSize;
	categoryData.mSafeSize = lineSafeSize;

	categoryData.mCanThrough = blockCanThr;
	categoryData.mThroughDirection = blockThrDirection;

	categoryData.mComment = lineDesc;

	try
	{
		//���ø����ڽ��в���
		LineConfigDialog* lineConfig 
				= dynamic_cast<LineConfigDialog*>(this->m_pParentWnd);

		if( lineConfig == NULL )
		{
			acutPrintf(L"!!�����ҵ�����������.\n");
		}

		//���ӻ���¹�������
		if( m_OperType == OPER_ADD )
		{
			acutPrintf(L"���ӹ�������.\n");

			lineConfig->CheckValid(categoryData,true);
			lineConfig->InsertLine(categoryData,false);
		}
		else if( m_OperType == OPER_UPDATE )
		{
			acutPrintf(L"���¹�������.\n");

			lineConfig->CheckValid(categoryData,false);
			lineConfig->UpdateLine(categoryData);
		}

		CDialog::OnOK();
	}
	catch(ErrorException& e)
	{
		MessageBoxW(e.errMsg.c_str(), GlobalData::ERROR_DIALOG_CAPTION.c_str(), MB_OKCANCEL);
	}
}

afx_msg void AddBlockConfigDialog::OnChkCanThroughChange()
{
	ShowControlDynamic();
}

void AddBlockConfigDialog::ShowControlDynamic()
{
	AddLineConfigDialog::ShowControlDynamic();

	if( m_CanThr.GetCheck() == BST_CHECKED )
	{
		m_ThroughDirection.EnableWindow(TRUE);
	}
	else if(  m_CanThr.GetCheck() == BST_UNCHECKED)
	{
		m_ThroughDirection.EnableWindow(FALSE);
	}
}

} // end of config

} // end of assistant

} // end of guch

} // end of com

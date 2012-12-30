// AddLineDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AddLineDialog.h"
#include "afxdialogex.h"
#include "GlobalDataConfig.h"
#include "LineTypeConfigPropertySheet.h"

using namespace com::guch::assistent::data;

// AddLineConfigDialog dialog
namespace com
{

namespace guch
{

namespace assistent
{

namespace config
{

IMPLEMENT_DYNAMIC(AddLineConfigDialog, CDialog)

AddLineConfigDialog::AddLineConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialog(AddLineConfigDialog::IDD, pParent)
{
}

BOOL AddLineConfigDialog::OnInitDialog()
{
	//��ҳ�潻������
	CDialog::OnInitDialog();

	this->InitConfigData();

	return TRUE;
}

void AddLineConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_KIND, m_LineKind);
	DDX_Control(pDX, IDC_COMBO_SHAPE, m_LineShape);
	DDX_Control(pDX, IDC_COMBO_UNIT, m_LineUnit);
	DDX_Control(pDX, IDC_EDIT_NAME, m_LineName);
	DDX_Control(pDX, IDC_EDIT_SIZE, m_LineSize);
	DDX_Control(pDX, IDC_EDIT_SAFESIZE,m_LineSafeSize);
	DDX_Control(pDX, IDC_EDIT_DESC,m_LineDesc);
}

BEGIN_MESSAGE_MAP(AddLineConfigDialog, CDialog)
	ON_BN_CLICKED(IDOK, &AddLineConfigDialog::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL AddLineConfigDialog::InitConfigData()
{
	//����
	m_LineKind.AddString(GlobalData::LINE_CATEGORY_SHANGSHUI.c_str());
	m_LineKind.AddString(GlobalData::LINE_CATEGORY_XIASHUI.c_str());
	m_LineKind.AddString(GlobalData::LINE_CATEGORY_NUANQI.c_str());
	m_LineKind.AddString(GlobalData::LINE_CATEGORY_DIANLAN.c_str());
	m_LineKind.AddString(GlobalData::LINE_CATEGORY_YUSUI.c_str());
	m_LineKind.AddString(GlobalData::LINE_CATEGORY_TONGXIN.c_str());
	m_LineKind.SetCurSel(0);

	//������״
	m_LineShape.AddString(GlobalData::LINE_SHAPE_CIRCLE.c_str());
	m_LineShape.AddString(GlobalData::LINE_SHAPE_SQUARE.c_str());
	m_LineShape.SetCurSel(0);

	//��λ
	m_LineUnit.AddString(GlobalData::LINE_UNIT_MM.c_str());
	m_LineUnit.AddString(GlobalData::LINE_UNIT_CM.c_str());
	m_LineUnit.AddString(GlobalData::LINE_UNIT_M.c_str());
	m_LineUnit.SetCurSel(0);

	//����
	m_LineName.SetWindowTextW(L"");

	//��С
	m_LineSize.SetWindowTextW(L"0");;

	//��ȫ��Χ
	m_LineSafeSize.SetWindowTextW(L"0");;

	//����
	m_LineDesc.SetWindowText(L"");

	if( m_OperType == OPER_UPDATE )
	{
		LineConfigData configData;
		
		//Get the parent windows
		LineConfigDialog* lineConfig 
			= dynamic_cast<LineConfigDialog*>(this->m_pParentWnd);

		if( lineConfig )
		{
			//���Ŀؼ��ı���
			this->SetWindowTextW(L"���߸���");

			CWnd* pOKBtn = this->GetDlgItem(IDOK);
			if( pOKBtn )
			{
				pOKBtn->SetWindowTextW(L"����");
			}

			//�õ�ѡ�������
			lineConfig->GetSelectData(configData);

			//�������
			fillData(configData);

			UpdateData(FALSE);
		}
	}

	return TRUE;
}

void AddLineConfigDialog::fillData(LineConfigData& configData)
{
	m_lineIndex = configData.mIndex;
	m_LineKind.SelectString(0,configData.mCategory.c_str());
	m_LineName.SetWindowTextW(configData.mName.c_str());
	m_LineShape.SelectString(-1,configData.mShape.c_str());
	m_LineSize.SetWindowTextW(configData.mSize.c_str());
	m_LineSafeSize.SetWindowTextW(configData.mSafeSize.c_str());
	m_LineUnit.SelectString(0,configData.mUnit.c_str());
	m_LineDesc.SetWindowTextW(configData.mDesc.c_str());
}

AddLineConfigDialog::~AddLineConfigDialog()
{
}

void AddLineConfigDialog::OnBnClickedOk()
{
	//Get the parent windows
	LineConfigDialog* lineConfig 
			= dynamic_cast<LineConfigDialog*>(this->m_pParentWnd);

	if( lineConfig == NULL )
	{
		acutPrintf(L"!!�����ҵ�����������.\n");
	}

	//Get the user input
	UpdateData(TRUE);

	CString lineKind,lineName,lineShape,lineSize,lineUnit,lineSafeSize,lineDesc;
		
	m_LineKind.GetWindowTextW(lineKind);
	m_LineName.GetWindowTextW(lineName);
	m_LineShape.GetWindowTextW(lineShape);
	m_LineSize.GetWindowTextW(lineSize);
	m_LineUnit.GetWindowTextW(lineUnit);
	m_LineSafeSize.GetWindowTextW(lineSafeSize);
	m_LineDesc.GetWindowTextW(lineDesc);

	if( m_OperType == OPER_ADD )
	{
		acutPrintf(L"���ӹ�������.\n");

		lineConfig->InsertLine(0,L"0",lineName.GetBuffer(),
										GlobalData::KIND_LINE,
										lineKind.GetBuffer(),
										lineShape.GetBuffer(),
										lineSize.GetBuffer(),
										lineSafeSize.GetBuffer(),
										lineUnit.GetBuffer(),
										lineDesc.GetBuffer());
	}
	else if( m_OperType == OPER_UPDATE )
	{
		acutPrintf(L"���¹�������.\n");
		lineConfig->UpdateLine(m_lineIndex,L"0",lineName.GetBuffer(),
										GlobalData::KIND_LINE,
										lineKind.GetBuffer(),
										lineShape.GetBuffer(),
										lineSize.GetBuffer(),
										lineSafeSize.GetBuffer(),
										lineUnit.GetBuffer(),
										lineDesc.GetBuffer());
	}

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

// AddLineDialog dialog

IMPLEMENT_DYNAMIC(AddLineDialog, CDialog)

AddLineDialog::AddLineDialog(CWnd* pParent /*=NULL*/)
	: CDialog(AddLineDialog::IDD, pParent)
{
}

BOOL AddLineDialog::OnInitDialog()
{
	//��ҳ�潻������
	CDialog::OnInitDialog();

	this->InitLineList();

	this->InitLineDetailHeader();

	this->InitLineDetailData();

	return TRUE;
}

BOOL AddLineDialog::InitLineList()
{
	/*
	acutPrintf(L"��ʼ������ʵ������.\n");

	HTREEITEM hKindItem,hCatogreyItem,kLineItem;
	
	//�ڸ���������"����"
	hKindItem = m_LinesTree.InsertItem(L"����",TVI_ROOT);

	//�ڡ����ߡ���������������
	hCatogreyItem = m_LinesTree.InsertItem(L"ˮ��",hKindItem);

	//����������
	kLineItem = m_LinesTree.InsertItem(L"ˮ��#1",hCatogreyItem);
	kLineItem = m_LinesTree.InsertItem(L"ˮ��#2",hCatogreyItem,kLineItem);

	//������������
	hCatogreyItem = m_LinesTree.InsertItem(L"ů��",hKindItem,hCatogreyItem);//��Parent1�����һ���ӽ�㣬����Child1_1����
	hCatogreyItem = m_LinesTree.InsertItem(L"����",hKindItem,hCatogreyItem);

	hKindItem = m_LinesTree.InsertItem(L"�����",TVI_ROOT,hKindItem);   

	//�ڡ�����塱��������������
	hCatogreyItem = m_LinesTree.InsertItem(L"���",hKindItem);

	//����������
	kLineItem = m_LinesTree.InsertItem(L"���#1",hCatogreyItem);
	kLineItem = m_LinesTree.InsertItem(L"���#2",hCatogreyItem,kLineItem);
	*/

	return TRUE;
}

BOOL AddLineDialog::InitLineDetailHeader()
{
	acutPrintf(L"��ʼ���������ݱ�ͷ.\n");

	/*
	int index = 0;

	LVCOLUMN lvColumn;

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 40;
	lvColumn.pszText = L"���";
	this->m_LineDetailList.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 60;
	lvColumn.pszText = L"X����";
	m_LineDetailList.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 60;
	lvColumn.pszText = L"Y����";
	m_LineDetailList.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 60;
	lvColumn.pszText = L"�߳�";
	m_LineDetailList.InsertColumn(index++, &lvColumn);

	m_LineDetailList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	*/

	return TRUE;
}

BOOL AddLineDialog::InitLineDetailData()
{
	acutPrintf(L"��ʼ������������Ϣ.\n");

	/*
	const int MAX_ITEM = 10;

	for( int i = 0; i < MAX_ITEM; i++)
	{
		LVITEM lvItem;
		int nItem;
	
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		
		CString strNumber;
		strNumber.Format(L"%d",i);

		lvItem.pszText = strNumber.GetBuffer();
		nItem = this->m_LineDetailList.InsertItem(&lvItem);

		m_LineDetailList.SetItemText(nItem, 1, L"30.5");
		m_LineDetailList.SetItemText(nItem, 2, L"29.6");
		m_LineDetailList.SetItemText(nItem, 3, L"17.3");
	}

	UpdateData(FALSE);
	*/

	return TRUE;
}

AddLineDialog::~AddLineDialog()
{
}

void AddLineDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_TREE_LINES, m_LinesTree);
	//DDX_Control(pDX, IDC_LIST_LINE_DETAIL, m_LineDetailList);
}


BEGIN_MESSAGE_MAP(AddLineDialog, CDialog)
END_MESSAGE_MAP()

// AddLineDialog message handlers

} // end of config

} // end of assistant

} // end of guch

} // end of com

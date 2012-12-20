// LineConfigDialog.cpp : implementation file
//

#include "stdafx.h"
#include "LineConfigDialog.h"
#include "afxdialogex.h"

#include "resource.h"

#include "AddLineDialog.h"

// LineConfigDialog dialog

IMPLEMENT_DYNAMIC(LineConfigDialog, CAcUiDialog)

LineConfigDialog::LineConfigDialog(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(LineConfigDialog::IDD, pParent)
{
	
}

LineConfigDialog::~LineConfigDialog()
{
}

void LineConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LINE_CONFIG, m_lineConfig);
}

BOOL LineConfigDialog::OnInitDialog()
{
	//��ҳ�潻������
	CAcUiDialog::OnInitDialog();

	this->InitLineHeader();

	this->InitLineData();

	return TRUE; // return TRUE unless you set the focus to a control
}

BOOL LineConfigDialog::InitLineHeader()
{
	acutPrintf(L"��ʼ����������.\n");
	int index = 0;

	LVCOLUMN lvColumn;

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 40;
	lvColumn.pszText = L"����";
	m_lineConfig.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 40;
	lvColumn.pszText = L"����";
	m_lineConfig.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 60;
	lvColumn.pszText = L"������״";
	m_lineConfig.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 60;
	lvColumn.pszText = L"�����С";
	m_lineConfig.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 100;
	lvColumn.pszText = L"����";
	m_lineConfig.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 50;
	lvColumn.pszText = L"��λ";
	m_lineConfig.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 80;
	lvColumn.pszText = L"��Ч��Χ";
	m_lineConfig.InsertColumn(index++, &lvColumn);

	m_lineConfig.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	return TRUE;
}

BOOL LineConfigDialog::InitLineData()
{
	const int MAX_ITEM = 10;

	for( int i = 0; i < MAX_ITEM; i++)
	{
		LVITEM lvItem;
		int nItem;
	
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		lvItem.pszText = L"����";
		nItem = m_lineConfig.InsertItem(&lvItem);

		m_lineConfig.SetItemText(nItem, 1, L"��ˮ");
		m_lineConfig.SetItemText(nItem, 2, L"Բ��");
		m_lineConfig.SetItemText(nItem, 3, L"10");

		m_lineConfig.SetItemText(nItem, 4, L"���Թ���");
		m_lineConfig.SetItemText(nItem, 5, L"����");
		m_lineConfig.SetItemText(nItem, 6, L"30");
	}

	UpdateData(FALSE);

	return TRUE;
}

BEGIN_MESSAGE_MAP(LineConfigDialog, CAcUiDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
END_MESSAGE_MAP()

void LineConfigDialog::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	AddLineDialog dlg(this);
	INT_PTR nReturnValue = dlg.DoModal();
}


// LineConfigDialog message handlers

// EntryManageDialog.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"

#include <GlobalDataConfig.h>
#include <LineTypeConfigPropertySheet.h>
#include <EntryManageDialog.h>
#include <LMAException.h>
#include <LMAUtils.h>
#include <LineConfigDataManager.h>

using namespace com::guch::assistent::data;
using namespace com::guch::assistent::exception;
using namespace com::guch::assistent::config;

namespace com
{

namespace guch
{

namespace assistent
{

namespace entry
{

// EntryManageDialog dialog

IMPLEMENT_DYNAMIC(EntryManageDialog, CDialog)

EntryManageDialog::EntryManageDialog(CWnd* pParent /*=NULL*/)
	: CDialog(EntryManageDialog::IDD, pParent)
{
	m_fileName = L"Entry_Draw.txt";

	//�õ�ʵ�������ļ��е�����
	m_EntryFile = new LineEntryFile( m_fileName );
}

BOOL EntryManageDialog::OnInitDialog()
{
	//��ҳ�潻������
	CDialog::OnInitDialog();

	this->InitEntryList();

	this->InitEntryDetailHeader();

	this->InitEntryDetailData();

	return TRUE;
}

BOOL EntryManageDialog::InitEntryList()
{
	acutPrintf(L"��ʼ������ʵ������.\n");

#ifdef _DEMO_DATA
	HTREEITEM hKindItem ,hCatogreyItem,kLineItem;

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

#else

	//�ڸ���������"����"
	m_lineRoot = m_LinesTree.InsertItem(L"����",TVI_ROOT);

	//�ڸ���������"�����"
	m_blockRoot = m_LinesTree.InsertItem(L"�����",TVI_ROOT, m_lineRoot);   

	LineList* lineList =  m_EntryFile->GetList();

	//��ʼ���������������
	for( LineIterator iter = lineList->begin();
			iter != lineList->end();
			iter++)
	{
		InsertLine((*iter),TRUE);
	}

#endif
	return TRUE;
}

HTREEITEM EntryManageDialog::GetKindNode( const wstring& category, const wstring& kind)
{
	TVITEM item;

	HTREEITEM hCurrent,hParent;

	if( category == GlobalData::KIND_LINE )
	{
		hParent = m_lineRoot;
	}
	else if ( category == GlobalData::KIND_SEPERATOR )
	{
		hParent = m_blockRoot;
	}

	hCurrent = m_LinesTree.GetNextItem(hParent, TVGN_CHILD);
	while (hCurrent != NULL) 
	{
	   // Get the text for the item. Notice we use TVIF_TEXT because
	   // we want to retrieve only the text, but also specify TVIF_HANDLE
	   // because we're getting the item by its handle.
	   TCHAR szText[1024];
	   item.hItem = hCurrent;
	   item.mask = TVIF_TEXT | TVIF_HANDLE;
	   item.pszText = szText;
	   item.cchTextMax = 1024;

	   BOOL bWorked = m_LinesTree.GetItem(&item);

	   // Try to get the next item
	   hCurrent = m_LinesTree.GetNextItem(hCurrent, TVGN_CHILD);

	   // If we successfuly retrieved an item, and the item's text
	   // is the line kind
	   if (bWorked && wstring(item.pszText) == kind)
	   {
#ifdef DEBUG
		   acutPrintf(L"�������ҵ����������͡�%s��\n",kind);
#endif
		  //m_LinesTree.DeleteItem(item.hItem);
		  return item.hItem;
	   }
	}

	//not find the kind, creat a new kind
#ifdef DEBUG
	acutPrintf(L"������û���ҵ����������͡�%s�����ô���\n",kind);
#endif

	hCurrent = m_LinesTree.InsertItem(kind.c_str(),hParent);
	return hCurrent;
}

BOOL EntryManageDialog::InsertLine( LineEntry* lineEntry, BOOL bInitialize )
{
	//�ж���Ϸ���
	try
	{
		if( !lineEntry )
		{
			CString errorMsg;
			errorMsg.Format(L"����Ϊ��");

			throw ErrorException(errorMsg.GetBuffer());
		}

		if( !bInitialize )
		{
			if( m_EntryFile->FindLineByNO( lineEntry->m_LineNO ) )
			{
				CString errorMsg;
				errorMsg.Format(L"");

				throw ErrorException(errorMsg.GetBuffer());
			}

			if( m_EntryFile->FindLineByName( lineEntry->m_LineName ) )
			{
				CString errorMsg;
				errorMsg.Format(L"");

				throw ErrorException(errorMsg.GetBuffer());
			}
		}
	}
	catch(const ErrorException& e)
	{
		MessageBoxW(e.errMsg.c_str(), GlobalData::ERROR_DIALOG_CAPTION.c_str(), MB_OK);
		return FALSE;
	}

	//�õ�������
	wstring category = LineConfigDataManager::Instance()->FindCategory(lineEntry->m_LineKind);

	//�õ���ڵ�
	HTREEITEM kindNode = GetKindNode(category, lineEntry->m_LineKind);

	//�����ʵ������
	m_LinesTree.InsertItem(lineEntry->m_LineName.c_str(),kindNode);

	return TRUE;
}

BOOL EntryManageDialog::InitEntryDetailHeader()
{
	acutPrintf(L"��ʼ���������ݱ�ͷ.\n");

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

	return TRUE;
}

BOOL EntryManageDialog::InitEntryDetailData()
{
	acutPrintf(L"��ʼ������������Ϣ.\n");

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

	return TRUE;
}

EntryManageDialog::~EntryManageDialog()
{
	if( m_EntryFile )
		delete m_EntryFile; 
}

void EntryManageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_TREE_LINES, m_LinesTree);
	DDX_Control(pDX, IDC_LIST_LINE_DETAIL, m_LineDetailList);
}


BEGIN_MESSAGE_MAP(EntryManageDialog, CDialog)
END_MESSAGE_MAP()

// EntryManageDialog message handlers

} // end of config

} // end of assistant

} // end of guch

} // end of com

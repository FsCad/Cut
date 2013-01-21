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
#include <AddEntryDialog.h>
#include <ArxWrapper.h>

using namespace com::guch::assistant::data;
using namespace com::guch::assistant::exception;
using namespace com::guch::assistant::config;

namespace com
{

namespace guch
{

namespace assistant
{

namespace entry
{

// EntryManageDialog dialog

IMPLEMENT_DYNAMIC(EntryManageDialog, CDialog)

EntryManageDialog::EntryManageDialog(CWnd* pParent /*=NULL*/)
	: CDialog(EntryManageDialog::IDD, pParent)
{
	//�õ���ǰ������ĵ�
	m_fileName = curDoc()->fileName();

	//�õ�ʵ�������ļ��е�����
	m_EntryFile = LineEntryFileManager::RegisterEntryFile(m_fileName);
}

BOOL EntryManageDialog::OnInitDialog()
{
	//��ҳ�潻������
	CDialog::OnInitDialog();

	this->InitEntryList();

	this->InitEntryDetailInfo();

	this->InitEntryDetailHeader();

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

	//Ĭ��չ�����ڵ�
	m_LinesTree.Expand(m_lineRoot, TVE_EXPAND);
	m_LinesTree.Expand(m_blockRoot, TVE_EXPAND);

#endif
	return TRUE;
}

BOOL EntryManageDialog::InitEntryDetailInfo()
{
	//�ؼ����ɱ༭
	m_LineNO.EnableWindow(FALSE);
	m_LineName.EnableWindow(FALSE);
	m_LineKind.EnableWindow(FALSE);

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

HTREEITEM EntryManageDialog::FindKindNode( const UINT& lineID)
{
	HTREEITEM hCurrent = m_LinesTree.GetRootItem();
	while (hCurrent != NULL) 
	{
	   // Get the ID for the item.
	   UINT currentID = (UINT)m_LinesTree.GetItemData(hCurrent);

	   // Check the ID
	   if( currentID == lineID)
	   {
#ifdef DEBUG
		   acutPrintf(L"�������ҵ�������ID��%d��\n",lineID);
#endif
		  //m_LinesTree.DeleteItem(item.hItem);
		  return hCurrent;
	   }

	   // Try to get the next visible item
	   hCurrent = m_LinesTree.GetNextItem(hCurrent, TVGN_NEXTVISIBLE);
	}

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

		LineEntry* pLine = NULL;
		if( !bInitialize )
		{
			if( pLine = m_EntryFile->FindLineByNO( lineEntry->m_LineNO ) )
			{
				CString errorMsg;
				errorMsg.Format(L"���ߡ�%s��Ҳ�������ţ���һ����",pLine->m_LineName.c_str());

				throw ErrorException(errorMsg.GetBuffer());
			}

			if( pLine = m_EntryFile->FindLineByName( lineEntry->m_LineName ) )
			{
				CString errorMsg;
				errorMsg.Format(L"���ߡ�%s��Ҳ��������֣���һ����",pLine->m_LineNO.c_str());

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

	HTREEITEM hCatogery;

	if( category == GlobalData::KIND_LINE )
	{
		hCatogery = m_lineRoot;
	}
	else if ( category == GlobalData::KIND_SEPERATOR )
	{
		hCatogery = m_blockRoot;
	}

	//�õ���ڵ�
	HTREEITEM kindNode = GetKindNode(category, lineEntry->m_LineKind);

	//�����ʵ������
	HTREEITEM newItem = m_LinesTree.InsertItem(lineEntry->m_LineName.c_str(), kindNode);

	//Ĭ�ϲ���֮��Ĭ�ϴ򿪸ýڵ�ĸ��ڵ�
	m_LinesTree.Expand(kindNode, TVE_EXPAND);
	
	//���ֱ��ڵ�ɼ�
	m_LinesTree.EnsureVisible(newItem);

	//�������ļ���ʼ��
	if( !bInitialize )
	{		
		//���ɸ����ID
		lineEntry->m_LineID = (UINT)GetTickCount();

		//�������ݵ�������
		m_EntryFile->InsertLine(lineEntry);

		//���浽���ݿ�
		ArxWrapper::PostToNameObjectsDict(lineEntry,lineEntry->LINE_ENTRY_LAYER);

		//���浽�����ļ�
		m_EntryFile->Persistent();
	}

	//���ø����ID
	m_LinesTree.SetItemData(newItem,(DWORD_PTR)lineEntry->m_LineID);

	return TRUE;
}

BOOL EntryManageDialog::UpdateLine( LineEntry* lineEntry )
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

		LineEntry* pAnotherLine = NULL;

		if( pAnotherLine = m_EntryFile->HasAnotherLineByNO( lineEntry->m_LineID, lineEntry->m_LineNO ) )
		{
			CString errorMsg;
			errorMsg.Format(L"���ߡ�%s��Ҳ�������ţ���һ����",pAnotherLine->m_LineName.c_str());

			throw ErrorException(errorMsg.GetBuffer());
		}

		if( pAnotherLine = m_EntryFile->HasAnotherLineByByName( lineEntry->m_LineID, lineEntry->m_LineName ) )
		{
			CString errorMsg;
			errorMsg.Format(L"���ߡ�%s��Ҳ��������֣���һ����",pAnotherLine->m_LineNO.c_str());

			throw ErrorException(errorMsg.GetBuffer());
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
	HTREEITEM hItem = FindKindNode(lineEntry->m_LineID);

	//�򿪸��ڵ�
	HTREEITEM hParent = m_LinesTree.GetParentItem(hItem);
	if (hParent != NULL)
		m_LinesTree.Expand(hParent, TVE_EXPAND);
	
	//���ֱ��ڵ�ɼ�
	m_LinesTree.EnsureVisible(hItem);

	//����������
	if( hItem )
	{
		m_LinesTree.SetItemText(hItem, lineEntry->m_LineName.c_str());

		//��������
		m_EntryFile->UpdateLine(lineEntry);

		//���浽���ݿ�
		ArxWrapper::PostToNameObjectsDict(lineEntry,lineEntry->LINE_ENTRY_LAYER);

		//���浽�����ļ�
		m_EntryFile->Persistent();

		//������ϸ��Ϣ
		InitEntryPointsData();
	}

	return TRUE;
}

BOOL EntryManageDialog::InitEntryDetailHeader()
{
#ifdef DEBUG
	acutPrintf(L"��ʼ�������е����������.\n");
#endif

	int index = 0;

	//�������
	m_LineDetailList.InsertColumn(0, L"���", LVCFMT_LEFT, 80);
	m_LineDetailList.InsertColumn(1, L"X����", LVCFMT_LEFT, 100);
	m_LineDetailList.InsertColumn(2, L"Y����", LVCFMT_LEFT, 100);
	m_LineDetailList.InsertColumn(3, L"�߳�", LVCFMT_LEFT, 100);

	//�����ֻ��
	m_LineDetailList.SetColumnReadOnly(0);

	//Ĭ�ϱ༭��༭
	m_LineDetailList.SetDefaultEditor(NULL, NULL, &m_PointEdit);

	/*
	LVCOLUMN lvColumn;

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 80;
	lvColumn.pszText = L"���";
	this->m_LineDetailList.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 100;
	lvColumn.pszText = L"X����";
	m_LineDetailList.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 100;
	lvColumn.pszText = L"Y����";
	m_LineDetailList.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 100;
	lvColumn.pszText = L"�߳�";
	m_LineDetailList.InsertColumn(index++, &lvColumn);
	*/

	m_LineDetailList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	return TRUE;
}

BOOL EntryManageDialog::InitEntryPointsData(LineEntry* lineEntry)
{
	m_LineDetailList.DeleteAllItems();

	acutPrintf(L"��ʼ��������Ϣ.\n");

	if( lineEntry && lineEntry->m_PointList )
	{
		int index = 0;
		for( PointIter iter = lineEntry->m_PointList->begin();
			iter != lineEntry->m_PointList->end();
			iter++)
		{
			if(*iter)
			{
				CString temp;

				//���
				temp.Format(L"%d",index+1);
				m_LineDetailList.InsertItem(index,temp);

				//x
				temp.Format(L"%0.2f",(*iter)->m_Point[X]);
				m_LineDetailList.SetItemText(index, 1, temp);

				//y
				temp.Format(L"%0.2f",(*iter)->m_Point[Y]);
				m_LineDetailList.SetItemText(index, 2, temp);
				
				//z
				temp.Format(L"%0.2f",(*iter)->m_Point[Z]);
				m_LineDetailList.SetItemText(index, 3, temp);

				index++;
			}
		}
	}

	UpdateData(FALSE);

	return TRUE;
}

EntryManageDialog::~EntryManageDialog()
{
#ifdef DEBUG
	if( m_EntryFile )
		acutPrintf(L"�����ļ���%s��ʵ�����Ի���ر���.\n",m_EntryFile->m_FileName.c_str());
#endif
}

void EntryManageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE_LINES, m_LinesTree);

	DDX_Control(pDX, IDC_EDIT_NO, m_LineNO);
	DDX_Control(pDX, IDC_EDIT_NAME, m_LineName);
	DDX_Control(pDX, IDC_EDIT_KIND, m_LineKind);

	DDX_Control(pDX, IDC_EDIT_POINT, m_PointEdit);

	DDX_Control(pDX, IDC_LIST_LINE_DETAIL, m_LineDetailList);
}

LineEntry* EntryManageDialog::GetLineEntry( const UINT& ID )
{
	return m_EntryFile->FindLine(ID);
}

BEGIN_MESSAGE_MAP(EntryManageDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedButtonOK)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, OnBnClickedButtonMod)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnBnClickedButtonDel)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_LINES, OnTreeSelChanged)
END_MESSAGE_MAP()

void EntryManageDialog::OnBnClickedButtonOK()
{
	PointList* newPoints = new PointList();

	CString temp;
	for( int i = 0; i < m_LineDetailList.GetItemCount(); i++ )
	{
		PointEntry* point = new PointEntry();

		//�õ���ǰ��ţ��������б��е����кţ�
		point->m_PointNO = (UINT)i;

		temp = m_LineDetailList.GetItemText(i,1);
		acdbDisToF(temp.GetBuffer(), -1, &((point->m_Point)[X]));

		temp = m_LineDetailList.GetItemText(i,2);
		acdbDisToF(temp.GetBuffer(), -1, &((point->m_Point)[Y]));

		temp = m_LineDetailList.GetItemText(i,3);
		acdbDisToF(temp.GetBuffer(), -1, &((point->m_Point)[Z]));

		//���뵽������
		newPoints->push_back(point);
	}

	//�õ���ǰ�༭��ֱ��
	LineEntry* selectLine = GetSelectLine();

	//�����µ�����
	if( selectLine )
	{
		selectLine->SetPoints(newPoints);
	}

	//Ĭ�Ͻ���XY��ͼ
	acedCommand(RTSTR, _T("._-VIEW"), RTSTR, L"TOP", 0);

	//���浽��ʱ�ļ�
	m_EntryFile->Persistent();
}

LineEntry* EntryManageDialog::GetSelectLine()
{
	HTREEITEM selectedItem = m_LinesTree.GetSelectedItem();

	if( selectedItem == NULL)
		 return NULL;

	UINT selectedID = (UINT)m_LinesTree.GetItemData(selectedItem);

	return GetLineEntry(selectedID);
}

void EntryManageDialog::OnBnClickedButtonAdd()
{
	AddEntryDialog dlg(this);
	
	//���ò�������
	dlg.SetOperType(AddEntryDialog::OPER_ADD);

	INT_PTR nReturnValue = dlg.DoModal();
}

void EntryManageDialog::OnBnClickedButtonMod()
{
	HTREEITEM selectedItem = m_LinesTree.GetSelectedItem();

	if( selectedItem == NULL)
		 return;

	UINT selectedID = (UINT)m_LinesTree.GetItemData(selectedItem);

	AddEntryDialog dlg(this);
	
	//���ò�������
	dlg.SetOperType(AddEntryDialog::OPER_UPDATE);
	dlg.SetUpdateID(selectedID);

	INT_PTR nReturnValue = dlg.DoModal();
}

void EntryManageDialog::OnBnClickedButtonDel()
{
	//ѡ�����
	HTREEITEM selectedItem = m_LinesTree.GetSelectedItem();

	if( selectedItem == NULL)
		 return;

	UINT selectedID = (UINT)m_LinesTree.GetItemData(selectedItem);

	//�õ�ѡ�������
	LineEntry* pEntry = m_EntryFile->FindLine(selectedID);

	if( pEntry )
	{
		// Initializes the variables to pass to the MessageBox::Show method.
		CString message;
		message.Format(L"ȷʵҪɾ��[%s]��?",pEntry->m_LineName.c_str());

		LPCTSTR caption = L"ɾ��ʵ��";

		// Displays the MessageBox.
		int result = MessageBoxW(message, caption, MB_OKCANCEL);
		if ( result == IDOK )
		{
			//�����ݿ�ɾ�����߱���
			ArxWrapper::PostToNameObjectsDict(pEntry,pEntry->LINE_ENTRY_LAYER,true);

			//�����ݿ�ɾ���������е��߶�
			ArxWrapper::eraseLMALine(*pEntry);

			//ɾ�����е��ڴ�ڵ�
			pEntry->ClearPoints();

			//Delete from the list
			m_EntryFile->DeleteLine(selectedID);

			// delete the selected item. 
			m_LinesTree.DeleteItem(selectedItem);

			//���浽�����ļ�
			m_EntryFile->Persistent();
		}
	}
}

BOOL EntryManageDialog::InitEntryPointsData()
{
	//ѡ�����
	HTREEITEM selectedItem = m_LinesTree.GetSelectedItem();

	UINT selectedID = (UINT)m_LinesTree.GetItemData(selectedItem);

	//�õ�ѡ�������
	LineEntry* pEntry = m_EntryFile->FindLine(selectedID);

	FillLineData(pEntry);

	return TRUE;
}

void EntryManageDialog::OnTreeSelChanged(LPNMHDR pnmhdr, LRESULT *pLResult)
{
	//�����ĳһ�ѡ��
	if( pnmhdr->code == TVN_SELCHANGED )
	{
		InitEntryPointsData();
	}
}

void EntryManageDialog::FillLineData( LineEntry* lineEntry )
{
	if( lineEntry )
	{
		//����ɾ��������
		m_LineDelete.EnableWindow(TRUE);
		m_LineUpdate.EnableWindow(TRUE);

		//������ϸ����
		m_LineNO.SetWindowText(lineEntry->m_LineNO.c_str());
		m_LineName.SetWindowText(lineEntry->m_LineName.c_str());
		m_LineKind.SetWindowText(lineEntry->m_LineKind.c_str());

		//����������Ϣ
		InitEntryPointsData(lineEntry);
	}
	else
	{
		//�����ϸ����
		m_LineNO.SetWindowText(L"");
		m_LineName.SetWindowText(L"");
		m_LineKind.SetWindowText(L"");

		//��ɾ��������
		m_LineDelete.EnableWindow(FALSE);
		m_LineUpdate.EnableWindow(FALSE);

		//�������
		m_LineDetailList.DeleteAllItems();
	}

	UpdateData(FALSE);
}

// EntryManageDialog message handlers

} // end of entry

} // end of assistant

} // end of guch

} // end of com

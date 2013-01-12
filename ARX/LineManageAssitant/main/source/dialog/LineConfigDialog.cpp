// LineConfigDialog.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "resource.h"

#include <GlobalDataConfig.h>
#include <LineConfigDialog.h>
#include <LineConfigDataManager.h>
#include <LMAException.h>
#include <LineTypeConfigPropertySheet.h>

using namespace com::guch::assistant::data;
using namespace com::guch::assistant::exception;

namespace com
{

namespace guch
{

namespace assistant
{

namespace config
{

// LineConfigDialog dialog

IMPLEMENT_DYNAMIC(LineConfigDialog, CPropertyPage)

LineConfigDialog::LineConfigDialog(CWnd* pParent /*=NULL*/)
: CPropertyPage(LineConfigDialog::IDD)
{
	//�������
	this->m_psp.dwFlags |= PSP_USETITLE;
	this->m_psp.pszTitle = L"���߹���";
}

LineConfigDialog::~LineConfigDialog()
{
}

void LineConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LINE_CONFIG, m_lineConfig);
}

BOOL LineConfigDialog::OnInitDialog()
{
	//��ҳ�潻������
	CPropertyPage::OnInitDialog();

	this->InitLineHeader();

	this->InitLineData();

	return TRUE; // return TRUE unless you set the focus to a control
}

BOOL LineConfigDialog::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	//acutPrintf(L"�յ�Notify����WPARAM[%ld] LPARAM[%ld].\n",wParam,lParam);

	return CPropertyPage::OnNotify(wParam,lParam,pResult);
}

BOOL LineConfigDialog::InitLineHeader()
{
	acutPrintf(L"��ʼ��������������.\n");
	int index = 0;

	LVCOLUMN lvColumn;

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 40;
	lvColumn.pszText = L"���";
	//m_lineConfig.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 140;
	lvColumn.pszText = L"����";
	m_lineConfig.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 80;
	lvColumn.pszText = L"����";
	m_lineConfig.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 80;
	lvColumn.pszText = L"��״";
	m_lineConfig.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 60;
	lvColumn.pszText = L"��С";
	m_lineConfig.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 60;
	lvColumn.pszText = L"�ں�";
	m_lineConfig.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 80;
	lvColumn.pszText = L"��ȫ��Χ";
	m_lineConfig.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 80;
	lvColumn.pszText = L"���ȵ�λ";
	m_lineConfig.InsertColumn(index++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 150;
	lvColumn.pszText = L"����";
	m_lineConfig.InsertColumn(index++, &lvColumn);

	m_lineConfig.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	return TRUE;
}

/**
 *
 **/
BOOL LineConfigDialog::InsertLine(LineCategoryItemData& itemData,BOOL initialize)
{
	//���߶εõ���ID
	if( !initialize )
	{
		itemData.mID = LineConfigDataManager::Instance()->GetNextID();
	}

	LVITEM lvItem;

	//��������
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = 0;
	lvItem.iSubItem = 0;
	lvItem.pszText = const_cast<wchar_t*>(itemData.mName.c_str());
	m_lineConfig.InsertItem(&lvItem);

	//����ÿһ�е�ID
	m_lineConfig.SetItemData(lvItem.iItem,itemData.mID);

	//���������е�����
	UpdateUILineData(itemData);

	//�ǳ�ʼ���׶Σ�����������
	if( !initialize )
		LineConfigDataManager::Instance()->AddLine(&itemData);

	return TRUE;
}

/**
 *
 **/
BOOL LineConfigDialog::UpdateLine(const LineCategoryItemData& itemData)
{
	UpdateUILineData(itemData);

	//�������ݽṹ
	LineConfigDataManager::Instance()->UpdateLine(&itemData);

	return TRUE;
}

void LineConfigDialog::UpdateUILineData(const LineCategoryItemData& itemData)
{
	//����ID����
	int nItem = FindItem(itemData.mID);
	if( nItem == -1 )
		return;

	//����ҳ������
	int columIndex = 0;
	m_lineConfig.SetItemText(nItem, columIndex++, itemData.mName.c_str());

	m_lineConfig.SetItemText(nItem, columIndex++, itemData.mKind.c_str());
	m_lineConfig.SetItemText(nItem, columIndex++, itemData.mShape.c_str());

	wstring size;
	
	if( itemData.mShape == GlobalData::LINE_SHAPE_CIRCLE )
	{
		size = itemData.mRadius;
	}
	else if( itemData.mShape == GlobalData::LINE_SHAPE_SQUARE )
	{
		size = itemData.mLength + L"x" + itemData.mWidth;
	}

	m_lineConfig.SetItemText(nItem, columIndex++, size.c_str());
	m_lineConfig.SetItemText(nItem, columIndex++, itemData.mWallSize.c_str());
	m_lineConfig.SetItemText(nItem, columIndex++, itemData.mSafeSize.c_str());
	m_lineConfig.SetItemText(nItem, columIndex++, itemData.mUnit.c_str());

	m_lineConfig.SetItemText(nItem, columIndex++, itemData.mComment.c_str());

	UpdateData(FALSE);
}

/**
 *
 **/
void LineConfigDialog::CheckValid( const LineCategoryItemData& item, BOOL bNew )
{
	//���������Ƿ����
	const LineCategoryVecotr* lineCategoryData
		 = LineConfigDataManager::Instance()->GetData();

	typedef vector<LineCategoryItemData*>::const_iterator DataIterator;

	for( DataIterator iter = lineCategoryData->begin(); 
			iter != lineCategoryData->end(); 
			iter++)
	{
		if( bNew && (*iter)->mName == item.mName
			|| !bNew && (*iter)->mID != item.mID && (*iter)->mName == item.mName )
		{
			CString msg;
			msg.Format(L"�������͡�%s���Ѵ��ڣ��뻻�����ְɡ�",item.mName);
			throw ErrorException(msg.GetBuffer());
		}
	}
}

BOOL LineConfigDialog::InitLineData()
{
	const LineCategoryVecotr* lineCategoryData
		 = LineConfigDataManager::Instance()->GetData();

	typedef vector<LineCategoryItemData*>::const_reverse_iterator DataIterator;

	for( DataIterator iter = lineCategoryData->rbegin(); 
			iter != lineCategoryData->rend(); 
			iter++)
	{
		LineCategoryItemData* data = *iter;

		if( data && data->mCategory == GlobalData::KIND_LINE )
		{
			InsertLine(*data,true);
		}
	}

	UpdateData(FALSE);
	return TRUE;
}

BEGIN_MESSAGE_MAP(LineConfigDialog, CPropertyPage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LINE_CONFIG, OnItemChangedList)
	ON_LBN_DBLCLK(IDC_LIST_LINE_CONFIG,OnBnClickedButtonMod)
END_MESSAGE_MAP()

void LineConfigDialog::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	AddLineConfigDialog dlg(this);
	
	//���ò�������
	dlg.SetOperType(AddLineConfigDialog::OPER_ADD);

	INT_PTR nReturnValue = dlg.DoModal();
}

void LineConfigDialog::OnBnClickedButtonMod()
{
	// TODO: Add your control notification handler code here
	AddLineConfigDialog dlg(this);
	
	//���ò�������
	dlg.SetOperType(AddLineConfigDialog::OPER_UPDATE);

	dlg.DoModal();
}

BOOL LineConfigDialog::GetSelectData( LineCategoryItemData& configData )
{
	//ѡ�����
	int item = m_lineConfig.GetSelectionMark();
	if( item == -1 )
		return FALSE;

	GetItemData(item,configData);

	return TRUE;
}

BOOL LineConfigDialog::GetItemData( int item, LineCategoryItemData& configData)
{
	//�õ�Ҫ��������
	configData.mIndex = item;

	//�õ��е�ID
	DWORD_PTR itemData = m_lineConfig.GetItemData(item);
	configData.mID = (UINT)itemData;

	//Ϊ��������
	configData.mCategory = GlobalData::KIND_LINE;

	//�õ�ÿһ�е�����
	int columIndex = 0;

	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = item;
	lvItem.iSubItem = columIndex++;

	TCHAR szBuffer[255];
	DWORD cchBuf(GlobalData::ITEM_TEXT_MAX_LENGTH);

	lvItem.pszText = szBuffer;
	lvItem.cchTextMax = cchBuf;

	//����
	if( m_lineConfig.GetItem(&lvItem) )
		configData.mName = lvItem.pszText;

	//����
	lvItem.iSubItem = columIndex++;
	if( m_lineConfig.GetItem(&lvItem) )
		configData.mKind = lvItem.pszText;

	//��״
	lvItem.iSubItem = columIndex++;
	if( m_lineConfig.GetItem(&lvItem) )
		configData.mShape = lvItem.pszText;

	//��С
	lvItem.iSubItem = columIndex++;
	if( m_lineConfig.GetItem(&lvItem) )
	{
		if( configData.mShape == GlobalData::LINE_SHAPE_CIRCLE )
		{
			configData.mRadius = lvItem.pszText;
		}
		else if( configData.mShape == GlobalData::LINE_SHAPE_SQUARE )
		{
			wstring size = lvItem.pszText;
			size_t xPos = size.find_first_of(L"x");

			configData.mLength = size.substr(0,xPos);
			configData.mWidth = size.substr(xPos+1);
		}
	}
	
	//�ں�
	lvItem.iSubItem = columIndex++;
	if( m_lineConfig.GetItem(&lvItem) )
		configData.mWallSize = lvItem.pszText;

	//��ȫ��Χ
	lvItem.iSubItem = columIndex++;
	if( m_lineConfig.GetItem(&lvItem) )
		configData.mSafeSize = lvItem.pszText;

	//���ȵ�λ
	lvItem.iSubItem = columIndex++;
	if( m_lineConfig.GetItem(&lvItem) )
		configData.mUnit = lvItem.pszText;

	//��ע
	lvItem.iSubItem = columIndex++;
	if( m_lineConfig.GetItem(&lvItem) )
		configData.mComment = lvItem.pszText;

	return TRUE;
}

void LineConfigDialog::OnBnClickedButtonDel()
{
	// TODO: Add your control notification handler code here

	//ѡ�����
	int item = m_lineConfig.GetSelectionMark();
	if( item == -1 )
		return;

	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = item;
	lvItem.iSubItem = 0;

	TCHAR szBuffer[255];
	DWORD cchBuf(GlobalData::ITEM_TEXT_MAX_LENGTH);

	lvItem.pszText = szBuffer;
	lvItem.cchTextMax = cchBuf;

	if( m_lineConfig.GetItem(&lvItem) )
	{
		// Initializes the variables to pass to the MessageBox::Show method.
		CString message;
		message.Format(L"ȷʵҪɾ��[%s]��?",lvItem.pszText);

		LPCTSTR caption = L"ɾ������";

		// Displays the MessageBox.
		int result = MessageBoxW(message, caption, MB_OKCANCEL);
		if ( result == IDOK )
		{
			//Delete from the list
			LineConfigDataManager::Instance()->DeleteLine( (UINT)m_lineConfig.GetItemData(item) );

			// Closes the parent form. 
			m_lineConfig.DeleteItem(item);
		}
	}
}

void LineConfigDialog::OnBnClickedButtonOK()
{
	//acutPrintf(L"��������������ݵ��ļ�.\n");

	/*
	for(int i = 0; i < m_lineConfig.GetItemCount(); i++)
	{
		LineConfigData configData;
		GetItemData(i,configData);

		LineCategoryItemData* newData = new LineCategoryItemData();

		newData->mID = configData.mID;
		newData->mName = configData.mName;
		newData->mKind = configData.mKind;
		newData->mCategory = configData.mCategory;
		newData->mShape = configData.mShape;
		newData->mSize = configData.mSize;
		newData->mEffectSize = configData.mSafeSize;
		newData->mUnit = configData.mUnit;
		newData->mComment = configData.mDesc;

		LineConfigDataManager::Instance()->AddLine(newData);
	}
	*/
}

int LineConfigDialog::FindItem( UINT id ) const
{
	for( int i = 0; i < this->m_lineConfig.GetItemCount(); i++ )
	{
		if( id == (UINT) m_lineConfig.GetItemData(i) )
			return i;
	}

	return -1;
}

void LineConfigDialog::OnItemChangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    //if (pNMListView->uChanged & LVIF_STATE)
	{
		LineTypeConfigPropertySheet* pParentSheet =
			dynamic_cast<LineTypeConfigPropertySheet*>(this->GetParent());

		UINT selectCnt = m_lineConfig.GetSelectedCount();

		if( selectCnt > 0 )
		{
			//ѡ�������
			pParentSheet ? pParentSheet->ChangeControlStatus(TRUE) : NULL;
		}
		else
		{
			//ûѡ���򲻿���
			pParentSheet ? pParentSheet->ChangeControlStatus(FALSE) : NULL;
		}
		/*
        if(pNMListView->uNewState & LVNI_SELECTED))
		{
			
		}
		*/
	}
}

} // end of config

} // end of assistant

} // end of guch

} // end of com

// LineConfigDialog message handlers

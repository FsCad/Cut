#include "stdafx.h"

#include "LMALineConfigManagerDialog.h"

#include "AddLineDialog.h"

LMALineConfigManagerDialog::LMALineConfigManagerDialog(CWnd* parent)
	:CAcUiDialog(LMALineConfigManagerDialog::IDD, parent)
{
	acutPrintf(L"��ʼ������������������.\n");
	init();
}

LMALineConfigManagerDialog::~LMALineConfigManagerDialog(void)
{
}

void LMALineConfigManagerDialog::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LINE_CONFIG, m_LineList);
}

void LMALineConfigManagerDialog::init()
{
	initLineHeader();
	initLineData();
}

void LMALineConfigManagerDialog::initLineHeader()
{
	acutPrintf(L"��ʼ����������̧ͷ.\n");
	int index = 0;
	m_LineList.InsertColumn(index++,L"����");
	m_LineList.InsertColumn(index++,L"����");
	m_LineList.InsertColumn(index++,L"������״");
	m_LineList.InsertColumn(index++,L"�����С");
	m_LineList.InsertColumn(index++,L"����");
	m_LineList.InsertColumn(index++,L"��λ");
	m_LineList.InsertColumn(index++,L"��Ч��Χ");
}

void LMALineConfigManagerDialog::initLineData()
{
}

//void LMALineConfigManagerDialog::OnBnClickedButtonAdd()
//{
//	// TODO: Add your control notification handler code here
//	AddLineDialog dlg(this);
//	INT_PTR nReturnValue = dlg.DoModal();
//}


BEGIN_MESSAGE_MAP(LMALineConfigManagerDialog, CAcUiDialog)
//	ON_BN_CLICKED(IDC_BUTTON_ADD, &LMALineConfigManagerDialog::OnBnClickedButtonAdd)
END_MESSAGE_MAP()

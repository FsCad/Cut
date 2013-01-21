#pragma once

#include <resource.h>
#include <dbsymtb.h>
#include <dbapserv.h>
#include <adslib.h>
#include <adui.h>
#include <acui.h>
#include "afxwin.h"

#include <LineEntryData.h>

using namespace com::guch::assistant::data;

// LineCutPosDialog dialog

class LineCutPosDialog : public CAcUiDialog
{
	DECLARE_DYNAMIC(LineCutPosDialog)

public:
	LineCutPosDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~LineCutPosDialog();

	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_CUT_POS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CAcUiNumericEdit mOffset;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedX();
	afx_msg void OnBnClickedY();
	afx_msg void OnBnClickedZ();
	afx_msg void OnBnPickCutPos();
	static afx_msg void CutBack();

protected:

	void GenerateCutPlane();

	void GenereateCutRegion();

	void GenereateCutRegion(LineEntry* lineEntry);

	void ShowCutRegion();

private:

	//�����ƫ����
	int m_strOffset;

	//����ķ���
	int m_Direction;

	//����
	AcGePlane m_CutPlane;

	//���������
	static CString m_CutLayerName;

public:

	CButton m_DirectionX;
	CButton m_DirectionY;
	CButton m_DirectionZ;

	CAcUiNumericEdit m_EditOffset;

	CAcUiPickButton m_PickCutPosButton;
};

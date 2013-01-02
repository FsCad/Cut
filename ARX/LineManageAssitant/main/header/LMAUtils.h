#pragma once
#include <afxwin.h> 

#include <string>
#include <vector>

using namespace std;

typedef vector<wstring> wstrVector;

//��stringת����wstring
wstring StringToWString(const string& str);

//��wstringת����string     
string WstringToString(const wstring& wstr);

// Enables and disables an MFC control by name
void EnableControl( CWnd* pWnd, int iControl, bool enable );

void ShowControl( CWnd *pWnd, int iControl, bool show );

void MoveControl( CWnd *pWnd, int iControl, int top, int left, int sizex, int sizey );

void OverControl( CWnd *pWnd, int iControlSrc, int iControlDst );

void SetControlText( CWnd* pWnd, int iControl, LPCTSTR lpszString );

wstrVector* vectorContructor( const wstring& data, const wstring& seq, size_t start = 0, size_t end = 0 );

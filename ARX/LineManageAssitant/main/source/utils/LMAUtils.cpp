#include <LMAUtils.h>

#include "stdafx.h"

#pragma warning(disable : 4267)
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)
#pragma warning(disable : 4309)

//��stringת����wstring     
wstring StringToWString(const string& str)     
{
    wstring result;     
    //��ȡ��������С��������ռ䣬��������С���ַ�����     
    int len = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);     
    TCHAR* buffer = new TCHAR[len + 1];     
    //���ֽڱ���ת���ɿ��ֽڱ���     
    ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);     
    buffer[len] = '/0';             //����ַ�����β     
    //ɾ��������������ֵ     
    result.append(wstring(buffer,len));     
    delete[] buffer;     
    return result;     
} 

//��wstringת����string     
string WstringToString(const wstring& wstr)     
{     
    string result;     
    //��ȡ��������С��������ռ䣬��������С�°��ֽڼ����     
    int len = ::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);     
    char* buffer = new char[len + 1];     
    //���ֽڱ���ת���ɶ��ֽڱ���     
    ::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);     
    buffer[len] = '/0';     
    //ɾ��������������ֵ     
    result.append(string(buffer,len));     
    delete[] buffer;     
    return result;     
}

// Enables and disables an MFC control by name
void EnableControl( CWnd* pWnd, int iControl, bool enable )
{
// Enable Control
     CWnd* wnd_control = (CWnd*)( pWnd->GetDlgItem( iControl ) );
     if( wnd_control != NULL ) {
          wnd_control->EnableWindow( enable ) ;
     }
}

void ShowControl( CWnd *pWnd, int iControl, bool show )
{
     // Hide the Control
     CWnd* wnd_control = (CWnd*)( pWnd->GetDlgItem( iControl ) );
     if( wnd_control != NULL ) {
          if( show ) {
               wnd_control->ShowWindow( SW_SHOW ) ;
          } else {
               wnd_control->ShowWindow( SW_HIDE ) ;
          }
     }
}

void MoveControl( CWnd *pWnd, int iControl, int top, int left, int sizex, int sizey )
{
     // Move the control
     CWnd* wnd_control = (CWnd*)( pWnd->GetDlgItem( iControl ) );
     if( wnd_control != NULL ) {
          LPRECT lpRect = new RECT ;
          wnd_control->GetClientRect( lpRect ) ;
          lpRect->top	 = top;
          lpRect->left	= left;
          if( sizex > 0 || sizey > 0 ) {
               lpRect->right = sizex + left ;
               lpRect->bottom = sizey + top;
          }
          wnd_control->MoveWindow( lpRect ) ;
          delete lpRect ;
     }
}

void OverControl( CWnd *pWnd, int iControlSrc, int iControlDst )
{
     // Move the control
     CWnd* src_control = (CWnd*)( pWnd->GetDlgItem( iControlSrc ) );
	 CWnd* dst_control = (CWnd*)( pWnd->GetDlgItem( iControlDst ) );

     if( src_control != NULL && dst_control != NULL ) {
          LPRECT lpRect = new RECT;
          src_control->GetClientRect( lpRect ) ;
		  src_control->ClientToScreen( lpRect ) ;

		  LPRECT lpDstRect = new RECT;
		  dst_control->GetClientRect( lpDstRect );
		  dst_control->ClientToScreen(lpDstRect);

		  src_control->MoveWindow( lpDstRect->left - lpRect->left,
									lpDstRect->top - lpRect->top,
									lpRect->right - lpRect->left,
									lpRect->bottom - lpRect->top);

          delete lpRect ;
		  delete lpDstRect;
     }
}

void SetControlText( CWnd* pWnd, int iControl, LPCTSTR lpszString )
{
	 CWnd* wnd_control = (CWnd*)( pWnd->GetDlgItem( iControl ) );
     if( wnd_control != NULL ) {
		 wnd_control->SetWindowTextW( lpszString ) ;
     }
}

wstrVector* vectorContructor( const wstring& data, const wstring& sep, size_t start, size_t end )
{
	if(data.length() == 0 || sep.length() == 0 )
		return NULL;

	wstrVector* wstrv = new wstrVector();

	size_t columnFrom = (size_t)start;
	size_t columnPos = data.find_first_of(sep,columnFrom);

	int indexCol = 0;
	while( columnPos != wstring::npos )
	{
		//�õ�һ���ֶ�
		wstring& rColumn = data.substr(columnFrom,columnPos-columnFrom);
		wstrv->push_back( rColumn );

		//������һ��column
		columnFrom = columnPos + sep.length();
		columnPos = data.find_first_of(sep,columnFrom);
	}

	if( end == 0 )
	{
		wstrv->push_back( data.substr(columnFrom) );
	}
	else
	{
		wstrv->push_back( data.substr(columnFrom,data.length() - columnFrom) );
	}

	return wstrv;
}

/****************************************************************************
**
**  ArxDbgUtils::dbToStr
**
**  **jma
**
*************************************/

LPCTSTR
dbToStr(AcDbDatabase* db, CString& str)
{
    if (db == NULL) {
        str = _T("NULL");
        return str;
    }

    const TCHAR* fname;
    Acad::ErrorStatus es = db->getFilename(fname);
    if (es == Acad::eOk)
        str.Format(_T("%p  \"%s\""), db, fname);
    else {
            // see if we can get name from a document
        AcApDocument* doc = acDocManager->document(db);
        if (doc) {
            str.Format(_T("%p  \"%s\""), db, doc->fileName());
        }
        else {
                // last resort, just use the pointer value.  eNotApplicable
                // happens frequently on temporary databases, otherwise we
                // would like to know what is going on.
            if (es == Acad::eNotApplicable)
                str.Format(_T("%p"), db);
            else {
                ASSERT(0);
                str.Format(_T("%p  %d"), db, es);
            }
        }
    }

    return str;
}

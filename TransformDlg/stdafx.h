
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


//#include "GdiplusH.h"
#include <Gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")
using namespace Gdiplus;
#include "Common/Gdiplus/GraphUtils.h"
//#include "GraphUtils.h"

#if !defined(SafeDelete) 
#define SafeDelete(ptr) { if(ptr) {delete ptr; ptr = NULL;}} 
#endif 

#if !defined(SafeRelease)
#define SafeRelease(obj) { if(obj) { obj->Release(); obj = NULL;}}
#endif

#if !defined(SafeDeleteObject)
#define SafeDeleteObject(hObject) {if(hObject) {DeleteObject(hObject); hObject = NULL;}}
#endif

#if !defined(SafeDeleteArray) 
#define SafeDeleteArray(ptr) { if(ptr) {delete[] ptr; ptr = NULL;}} 
#endif 

#ifndef SAFECLOSEHANDLE
#define SAFECLOSEHANDLE(hHandle) \
	if (hHandle != NULL) \
{ \
	CloseHandle(hHandle);\
	hHandle = NULL;\
}
#endif

#ifndef SafeChkMemoryLeak
#ifdef DEBUG
#define SafeChkMemoryLeak(nIndex) { new char[nIndex]; }
#else
#define SafeChkMemoryLeak(nIndex) {}
#endif
#endif






#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif



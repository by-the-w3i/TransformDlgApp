
// TransformDlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTransformDlgApp: 
// �йش����ʵ�֣������ TransformDlg.cpp
//

class CTransformDlgApp : public CWinApp
{
public:
	CTransformDlgApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTransformDlgApp theApp;
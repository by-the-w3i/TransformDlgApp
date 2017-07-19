#include "stdafx.h"
#include "UnitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CUnitDlg::CUnitDlg(LPCTSTR path, CString title, CString message,DOUBLE speed) :
	m_title(title), m_msg(message), m_speed(speed)
{
	m_img.Load(path);
	m_width = title.GetLength() > message.GetLength() ? title.GetLength() : message.GetLength();
}

void CUnitDlg::SetPosition(LONG l, LONG r, LONG t, LONG b)
{
	left = l;
	right = r;
	top = t;
	bottom = b;
}



CUnitDlg::~CUnitDlg()
{
}

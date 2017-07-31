#include "stdafx.h"
#include "BaseDlg.h"



CBaseDlg::CBaseDlg(CString path)
{
	m_img.Load(path);
}

CBaseDlg::~CBaseDlg()
{
}

void CBaseDlg::SetPosition(LONG startX, LONG startY)
{
	// 以左上角定点为准
	m_rcDlg.left = startX;
	m_rcDlg.top = startY;
	m_rcDlg.right = startX + m_width;
	m_rcDlg.bottom = startY + m_height;
}

void CBaseDlg::Move(long speed)
{
	m_rcDlg.left += speed;
	m_rcDlg.right += speed;
}

#include "stdafx.h"
#include "ImgDLg.h"

CImgDLg::CImgDLg(CString path) : CBaseDlg::CBaseDlg(path)
{
	m_width = 300;
	m_height = 300;
	m_imgPad = 4;
}


CImgDLg::~CImgDLg()
{
}

void CImgDLg::DrawDlg(CDC * pCDC)
{
	CDC cdcMEM;
	CBitmap cBmp;
	cdcMEM.CreateCompatibleDC(pCDC);
	cBmp.CreateCompatibleBitmap(pCDC, m_width, m_height);
	cdcMEM.SelectObject(&cBmp); //将位图选择进内存DC

	RECT dlgRc = { 0,0,m_width,m_height };
	cdcMEM.FillSolidRect(&dlgRc, RGB(250,250,250));

	cdcMEM.SetStretchBltMode(COLORONCOLOR);//很有用，使图片缩放后颜色不失真
	m_img.Draw(cdcMEM, m_imgPad, m_imgPad, m_width - 2 * m_imgPad, m_height - 2 * m_imgPad);

	pCDC->BitBlt(m_rcDlg.left, m_rcDlg.top, m_width, m_height, &cdcMEM, 0, 0, SRCCOPY);

	// 释放资源
	cBmp.DeleteObject();
	cdcMEM.DeleteDC();
}

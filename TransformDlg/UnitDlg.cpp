#include "stdafx.h"
#include "UnitDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CUnitDlg::CUnitDlg(CString path, CString title, CString message) : CBaseDlg::CBaseDlg(path)
{
	m_title = title;
	m_msg = message;

	m_hFont = ::CreateFont(m_fontH, m_fontW, 0, 0, 900,
		FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _T("宋体"));

	m_height = 80;
	m_imgPad = 6;

	m_width = (title.GetLength() > message.GetLength() ? title.GetLength() : message.GetLength()) * m_fontW *2.5
		+ m_padding *2 + m_height;

	// default start at (0, 0)
	SetPosition(-m_width, 0);
}



void CUnitDlg::DrawDlg(CDC * pCDC)
{
	//if (m_rcDlg.right < 0 || m_rcDlg.left > m_rcWnd.right) return;
	// image dc for drawing the main image
	CDC cdcMEM;
	CBitmap cBmp;
	cdcMEM.CreateCompatibleDC(pCDC);
	cBmp.CreateCompatibleBitmap(pCDC, m_width, m_height);
	cdcMEM.SelectObject(&cBmp); //将位图选择进内存DC

	RECT dlgRc = { 0,0,m_width,m_height };
	cdcMEM.FillSolidRect(&dlgRc, 0xffffff);

	// mask DC for filtering the white transparent parts
	CDC cMaskDc;
	cMaskDc.CreateCompatibleDC(pCDC);
	cMaskDc.FillSolidRect(&dlgRc, 0xffffff);


	// create the bmp for background in MaskDc
	CBitmap cMaskBmp;
	cMaskBmp.CreateCompatibleBitmap(pCDC, m_width, m_height);
	cMaskDc.SelectObject(&cMaskBmp);


	
	// background
	{
		CBrush gridBrush(RGB(10, 10, 10));
		CBrush *oldBrush = cdcMEM.SelectObject(&gridBrush);
		//cdcMEM.SelectObject(&brush);
		cdcMEM.RoundRect(dlgRc.left, dlgRc.top, dlgRc.right, dlgRc.bottom, m_height, m_height);

		// copy the background
		cMaskDc.BitBlt(0, 0, m_width, m_height, pCDC, m_rcDlg.left, m_rcDlg.top, SRCCOPY);
		// filter the white trans parts
		cMaskDc.TransparentBlt(0, 0, m_width, m_height, &cdcMEM, 0, 0, m_width, m_height, RGB(255,255,255)/*SRCCOPY*/);
		cdcMEM.BitBlt(0, 0, m_width, m_height, pCDC, m_rcDlg.left, m_rcDlg.top, SRCCOPY);
		// alpha channel
		BLENDFUNCTION blend = { 0 };
		blend.AlphaFormat = 0;
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = 0;
		blend.SourceConstantAlpha = 125;
		cdcMEM.AlphaBlend(0,0, m_width, m_height, &cMaskDc, 0, 0, m_width, m_height, blend);//内存DC到内存DC
	}

	// draw img
	{
		int side = m_height - m_imgPad * 2;
		CDC imgDc;
		CBitmap imgBmp;
		imgDc.CreateCompatibleDC(pCDC);
		imgBmp.CreateCompatibleBitmap(pCDC, side, side);
		imgDc.SelectObject(&imgBmp);
		//imgDc.FillSolidRect(0,0, m_height - m_imgPad * 2, m_height - m_imgPad * 2, 0xff00);
		imgDc.SetStretchBltMode(COLORONCOLOR);//很有用，使图片缩放后颜色不失真
		//if (!m_img) {
		//	m_img.Load(m_path);
		//}
		m_img.Draw(imgDc, 0, 0, side, side);

		CDC baseDc;
		CBitmap baseBmp;
		baseDc.CreateCompatibleDC(pCDC);
		baseBmp.CreateCompatibleBitmap(pCDC, side, side);
		baseDc.SelectObject(&baseBmp);
		CBrush gridBrush(RGB(255, 0, 0));
		CBrush *oldBrush = baseDc.SelectObject(&gridBrush);
		baseDc.RoundRect(0, 0, side,side,side,side);

		imgDc.TransparentBlt(0, 0, side, side, &baseDc, 0, 0, side,side, RGB(255, 0, 0));
		cdcMEM.TransparentBlt(m_imgPad, m_imgPad, m_height-2*m_imgPad, m_height- 2*m_imgPad, &imgDc, 0, 0,side,side, RGB(0, 0, 0));
	}

	// profile img
	{
		//Graphics g(cdcMEM);
// 		HRGN rgn = CreateEllipticRgn(m_imgPad + 1, m_imgPad + 1, m_height - m_imgPad + 1, m_height - m_imgPad + 1);
// 		Region region(rgn);
// 		g.SetClip(&region, CombineModeReplace);
		//g.DrawImage(m_pImage, 0, 0, m_height, m_height);

 		Graphics gBorder(cdcMEM);
 		gBorder.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);//抗锯齿
 		Pen blackPen(Color(150, 10,10,10), 4);
 		gBorder.DrawEllipse(&blackPen, m_imgPad, m_imgPad, m_height- 2*m_imgPad, m_height- 2*m_imgPad);
	}

	// title
	{
		::SelectObject(cdcMEM, m_hFont);
		::SetTextColor(cdcMEM, RGB(255, 220, 0));
		::SetBkMode(cdcMEM, TRANSPARENT);
		CRect rcText = { m_height + m_padding , 0, m_width, m_height/2 };
		::DrawText(cdcMEM, m_title, m_title.GetLength(), &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
	// text
	{
		::SetTextColor(cdcMEM, RGB(250, 250, 250));
		CRect rcText = { m_height + m_padding ,  m_height / 2 , m_width, m_height};
		::DrawText(cdcMEM, m_msg, m_msg.GetLength(), &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}

	//CImage cc;
	//cc.Load(L"L.jpg");
	//cdcMEM.SetStretchBltMode(COLORONCOLOR);//很有用，使图片缩放后颜色不失真
	//cc.Draw(cdcMEM, 0, 0, m_height, m_height);

	pCDC->BitBlt(m_rcDlg.left, m_rcDlg.top, m_width, m_height, &cdcMEM, 0, 0, SRCCOPY);

	// 释放资源
	cMaskBmp.DeleteObject();
	cMaskDc.DeleteDC();
	cBmp.DeleteObject();
	cdcMEM.DeleteDC();	
}





CUnitDlg::~CUnitDlg()
{
}


#include "stdafx.h"
#include "UnitDlg.h"



CUnitDlg::CUnitDlg(CString icon, CString title, CString msg, CString img) :
	m_title(title), m_msg(msg)
{
	m_hFont = ::CreateFont(FONTH, FONTW, 0, 0, 900,
		FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _T("宋体"));

	m_textH = 80;
	m_imgPad = 6;

	m_textW = (title.GetLength() > msg.GetLength() ? title.GetLength() : msg.GetLength()) 
		* FONTW *2.5 + m_textH;

	m_icon.Load(icon);

	m_width = m_textW;
	m_height = m_textH;

	if (img!=L"")
	{
		m_img.Load(img);
		m_imgH = FIX_IMGH;
		m_imgML = 2 * m_imgPad;

		long minW = MIN_IMGW + 2 * m_imgPad + m_imgML;
		long maxW = MAX_IMGW + 2 * m_imgPad + m_imgML;
		m_imgW = m_textW > minW ?
			(m_textW < maxW ? MIN_IMGW : MAX_IMGW) : MIN_IMGW;

		long realW = m_imgW + 2 * m_imgPad + m_imgML;
		if (m_width < realW) m_width = realW;

		m_height += m_imgH + TEXT2IMG + 2 * m_imgPad;
	}

	SetPosition(0, 0);
}

CUnitDlg::~CUnitDlg()
{
}





void CUnitDlg::SetPosition(long startX, long startY)
{
	// 以左上角定点为准
	m_globalArea.left = startX;
	m_globalArea.top = startY;
	m_globalArea.right = startX + m_width;
	m_globalArea.bottom = startY + m_height;
}





void CUnitDlg::Draw(CDC * pCDC)
{
	CDC cdcMEM;
	CBitmap cBmp;
	cdcMEM.CreateCompatibleDC(pCDC);
	cBmp.CreateCompatibleBitmap(pCDC, m_width, m_height);
	cdcMEM.SelectObject(&cBmp); //将位图选择进内存DC

	RECT dlgRc = { 0,0,m_width,m_height };
	cdcMEM.FillSolidRect(&dlgRc, 0xffffff);

	// mask DC for filtering the white transparent parts
	RECT textRc = { 0, 0, m_textW, m_textH };
	CDC cMaskDc;
	cMaskDc.CreateCompatibleDC(pCDC);
	cMaskDc.FillSolidRect(&textRc, 0xffffff);
	// create the bmp for background in MaskDc
	CBitmap cMaskBmp;
	cMaskBmp.CreateCompatibleBitmap(pCDC, m_textW, m_textH);
	cMaskDc.SelectObject(&cMaskBmp);

	// background
	{
		CBrush gridBrush(RGB(10, 10, 10));
		CBrush *oldBrush = cdcMEM.SelectObject(&gridBrush);
		cdcMEM.RoundRect(textRc.left, textRc.top, textRc.right, textRc.bottom, m_textH, m_textH);

		// copy the background
		cMaskDc.BitBlt(0, 0, m_textW, m_textH, pCDC, m_globalArea.left, m_globalArea.top, SRCCOPY);
		// filter the white trans parts
		cMaskDc.TransparentBlt(0, 0, m_textW, m_textH, &cdcMEM, 0, 0, m_textW, m_textH, RGB(255, 255, 255));
		cdcMEM.BitBlt(0, 0, m_width, m_height, pCDC, m_globalArea.left, m_globalArea.top, SRCCOPY);
		//cdcMEM.FillSolidRect(&dlgRc, 0xff00ff);
		// alpha channel
		BLENDFUNCTION blend = { 0 };
		blend.AlphaFormat = 0;
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = 0;
		blend.SourceConstantAlpha = 125;
		cdcMEM.AlphaBlend(0, 0, m_textW, m_textH, &cMaskDc, 0, 0, m_textW, m_textH, blend);//内存DC到内存DC

		cMaskBmp.DeleteObject();
		cMaskDc.DeleteDC();
	}
	int side = m_textH - m_imgPad * 2;
	// draw img
	{
		CDC imgDc;
		CBitmap imgBmp;
		imgDc.CreateCompatibleDC(pCDC);
		imgBmp.CreateCompatibleBitmap(pCDC, side, side);
		imgDc.SelectObject(&imgBmp);
		imgDc.SetStretchBltMode(COLORONCOLOR);
		m_icon.Draw(imgDc, 0, 0, side, side);

		CDC baseDc;
		CBitmap baseBmp;
		baseDc.CreateCompatibleDC(pCDC);
		baseBmp.CreateCompatibleBitmap(pCDC, side, side);
		baseDc.SelectObject(&baseBmp);
		CBrush gridBrush(RGB(255, 0, 0));
		CBrush *oldBrush = baseDc.SelectObject(&gridBrush);
		baseDc.RoundRect(0, 0, side, side, side, side);

		imgDc.TransparentBlt(0, 0, side, side, &baseDc, 0, 0, side, side, RGB(255, 0, 0));
		cdcMEM.TransparentBlt(m_imgPad, m_imgPad, side, side, &imgDc, 0, 0, side, side, RGB(0, 0, 0));

		imgBmp.DeleteObject();
		imgDc.DeleteDC();
		baseBmp.DeleteObject();
		baseDc.DeleteDC();
	}

	// icon border
	{
		Graphics gBorder(cdcMEM);
		gBorder.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);//抗锯齿
		Pen blackPen(Color(150, 10, 10, 10), 4);
		gBorder.DrawEllipse(&blackPen, m_imgPad, m_imgPad, side,side);
	}

	// title
	{
		::SelectObject(cdcMEM, m_hFont);
		::SetTextColor(cdcMEM, RGB(255, 220, 0));
		::SetBkMode(cdcMEM, TRANSPARENT);
		CRect rcText = { m_textH , 0, m_textW, m_textH / 2 };
		::DrawText(cdcMEM, m_title, m_title.GetLength(), &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
	// text
	{
		::SetTextColor(cdcMEM, RGB(250, 250, 250));
		CRect rcText = { m_textH ,  m_textH / 2 , m_textW, m_textH };
		::DrawText(cdcMEM, m_msg, m_msg.GetLength(), &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}

	if (m_img) BestFitImg(cdcMEM);

	pCDC->BitBlt(m_globalArea.left, m_globalArea.top, m_width, m_height, &cdcMEM, 0, 0, SRCCOPY);

	cBmp.DeleteObject();
	cdcMEM.DeleteDC();
}



void CUnitDlg::BestFitImg(CDC & cdcMEM)
{
	
	RECT dlgRc = { m_imgML, m_textH+TEXT2IMG, m_imgW+2*m_imgPad + m_imgML, m_height };
	cdcMEM.FillSolidRect(&dlgRc, RGB(250, 250, 250));

	int destX = dlgRc.left + m_imgPad;
	int destY = dlgRc.top + m_imgPad;

	int w = m_img.GetWidth();
	int h = m_img.GetHeight();

	double r = m_imgW / (double)m_imgH; // w/h

	int srcH, srcW, srcX, srcY;
	if ((double)w/h > r) {
		srcH = h;
		srcW = h * r;
		srcY = 0;
		srcX = (w - srcW) / 2.0;
	}
	else {
		srcW = w;
		srcH = w / r;
		srcX = 0;
		srcY = (h - srcH) / 2.0;
	}

	cdcMEM.SetStretchBltMode(COLORONCOLOR);//很有用，使图片缩放后颜色不失真
	m_img.Draw(cdcMEM, destX, destY, m_imgW, m_imgH, srcX, srcY,srcW, srcH);
}



void CUnitDlg::Move(long speed)
{
	m_globalArea.left += speed;
	m_globalArea.right += speed;
}
